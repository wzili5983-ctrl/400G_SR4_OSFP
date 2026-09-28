#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Pack a raw firmware bin into a framed region image (512B frame header
area + payload), per resource_impl/flashresource_impl.h. All six
user-upgradeable regions (APP / PARA / MISC, slots A/B) share one common
128B FrameHead_t; every member sits on a 16B boundary or fills a 16B
hexdump line:

    +0x000  FrameHead_t 128B  crc32data|flashid|rsvd[3]|storeaddr|fwsize,
                              then the 0xFF-padded ASCII identity slots
                              company[16] / hdver[16] / swver[16] /
                              modulename[16] (contents fixed by the
                              FRAME_* macros), rsvd[16] x2 and
                              rsvd[12]|crc32hdr at the tail DWORD (+0x7C)
    +0x080  rsvd         384B  -> 0xFF
    +0x200  payload   fwsize B, 8-aligned, 0xFF padded

The frame header fields live in a JSON file the user may edit (company,
hdver, swver, modulename, store_addr, ...; the macro-fixed defaults above
are written when the JSON is created). fwsize / crc32data / crc32hdr are
READ-ONLY display fields: recomputed on every pack and written back, so a
hand edit is silently discarded.

CRC convention matches the GD32 on-chip CRC peripheral as used by
BoardFlashCrc32 (flashresource_impl.c): poly 0x04C11DB7, init 0xFFFFFFFF,
no reflection, no final xor, data fed as little-endian 32-bit words
(MSB of each word first). Lengths must be a multiple of 4.

Usage (run from the project root):
    python tools/pack_framed_bin.py                 # pack with defaults
    python tools/pack_framed_bin.py --json bin/APP_A_frame.json \
           --bin bin/400G_SR4_OSFP.bin --out bin/APP_A_framed.bin
    python tools/pack_framed_bin.py --verify        # check framed bin

Defaults target APP_A: flash_id FLASH_ID_APP_A, store_addr 0x08010000.
The payload sits at store_addr + 0x200, so the image must be linked at
slot base + 0x200 (APP_A IROM = 0x08010000 in the Keil project); the
reset vector is range-checked against that base.

Per the fixed-A / mirror-B scheme an APP upgrade package is ALWAYS
A-named and goes through slot B first (even for the factory / blank-
device case); FLASH_ID_APP_B is rejected here -- slot B only ever holds
the A-mirror image, never a B-named package.
"""

import argparse
import json
import struct
import sys
from pathlib import Path

# ---------------------------------------------------------------------------
# Constants mirrored from resource_impl/memory_map.h + flashresource_impl.h.
# Keep in sync with the firmware headers.
# ---------------------------------------------------------------------------
SRAM_BASE = 0x20000000
SRAM_SIZE = 0x00020000

FRAME_HDR_AREA_SIZE = 0x00000200          # 512B frame header area
FRAME_PAYLOAD_OFF = 0x00000200            # payload start offset
FRAME_HDR_SIZE = 128                      # FrameHead_t
FRAME_STR_SIZE = 16                       # ASCII identity slot, 0xFF padded
FRAME_CRC32_HDR_OFF = 0x7C                # crc32hdr is the last DWORD
# Identity slot defaults, same strings as the FRAME_* macros in
# flashresource_impl.h.
FRAME_COMPANY = "Terabitcom"
FRAME_HDVER = "HF2026090614"
FRAME_SWVER = "SW2026092614"
FRAME_MODULENAME = "400G-SR4-OSFP-V1"
APP_BIN_LIMIT = 0x0002DE00                # APP payload limit (slot 0x2E000 - 0x200)

# The 6 user-upgradeable regions (FlashId_t order, after the metadata
# regions were re-added), name -> (id, base, size). An APP package is
# always A-named; FLASH_ID_APP_B is kept for geometry reference and
# rejected by pack/verify (slot B receives the A-mirror image).
REGIONS = {
    "FLASH_ID_APP_A":  (5, 0x08010000, 0x0002E000),
    "FLASH_ID_APP_B":  (6, 0x0803E000, 0x0002E000),
    "FLASH_ID_PARA_A": (7, 0x0806C000, 0x00008000),
    "FLASH_ID_PARA_B": (8, 0x08074000, 0x00008000),
    "FLASH_ID_MISC_A": (9, 0x0807C000, 0x00002000),
    "FLASH_ID_MISC_B": (10, 0x0807E000, 0x00002000),
}

PROJECT_ROOT = Path(__file__).resolve().parent.parent
DEFAULT_BIN = PROJECT_ROOT / "bin" / "400G_SR4_OSFP.bin"
DEFAULT_JSON = PROJECT_ROOT / "bin" / "APP_A_frame.json"
DEFAULT_OUT = PROJECT_ROOT / "bin" / "APP_A_framed.bin"

# Keys of the pre-gfw-removal frame layouts; dropped from a loaded JSON so a
# stale config is repaired instead of silently carrying dead fields.
LEGACY_KEYS = (
    "magic", "fw_ver", "hdr_ver", "hdrver", "hw_ver", "attrs",
    "anti_rollback", "link_addr", "fw_size", "entry_off", "reloc_off",
    "reloc_cnt", "crc32_pkg", "crc32_data", "crc32_hdr", "patch_type",
    "reloc", "patch_delta", "build_date", "build_time",
)

# Read-only display fields in the JSON: recomputed on every pack and
# written back; a hand edit is discarded (dropped before packing).
READONLY_KEYS = ("fwsize", "crc32data", "crc32hdr")


# ---------------------------------------------------------------------------
# GD32 on-chip CRC equivalent (see BoardFlashCrc32 / crc_block_data_calculate,
# INPUT_FORMAT_WORD): poly 0x04C11DB7, init 0xFFFFFFFF, MSB-first per 32-bit
# little-endian word, no reflection, no final xor.
# ---------------------------------------------------------------------------
def crc32_gd32(data: bytes, crc: int = 0xFFFFFFFF) -> int:
    if len(data) % 4 != 0:
        raise ValueError("CRC input length must be a multiple of 4")
    crc &= 0xFFFFFFFF
    for (word,) in struct.iter_unpack("<I", data):
        crc ^= word
        for _ in range(32):
            if crc & 0x80000000:
                crc = ((crc << 1) ^ 0x04C11DB7) & 0xFFFFFFFF
            else:
                crc = (crc << 1) & 0xFFFFFFFF
    return crc


def u32(b: bytes, off: int) -> int:
    return struct.unpack_from("<I", b, off)[0]


def fail(msg: str) -> "NoReturn":
    print(f"error: {msg}", file=sys.stderr)
    sys.exit(1)


# ---------------------------------------------------------------------------
# JSON config
# ---------------------------------------------------------------------------
def default_config() -> dict:
    # The payload sits at store_addr + FRAME_PAYLOAD_OFF, so a slot-specific
    # image is always linked at slot base + 0x200 (never the slot base).
    # Identity fields default to the FRAME_* macro strings above and may be
    # overridden per JSON; fwsize / crc32data / crc32hdr are read-only and
    # only appear after the first pack.
    return {
        "company": FRAME_COMPANY,
        "hdver": FRAME_HDVER,
        "flash_id": "FLASH_ID_APP_A",
        "store_addr": "0x08010000",
        "swver": FRAME_SWVER,
        "modulename": FRAME_MODULENAME,
    }


def load_config(path: Path) -> dict:
    if not path.exists():
        cfg = default_config()
        save_config(path, cfg)
        print(f"created default frame header JSON: {path}")
        return cfg
    with path.open("r", encoding="utf-8") as f:
        return json.load(f)


def save_config(path: Path, cfg: dict) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", encoding="utf-8") as f:
        json.dump(cfg, f, indent=2)
        f.write("\n")


def num(v, name: str) -> int:
    """Accept JSON ints or "0x..." strings."""
    if isinstance(v, int):
        return v
    if isinstance(v, str):
        return int(v, 16) if v.lower().startswith("0x") else int(v)
    fail(f"bad numeric value for {name}: {v!r}")


# ---------------------------------------------------------------------------
# Header (de)serialization: the single common FrameHead_t, 128B.
# Every member sits on a 16B boundary or fills a 16B line with its neighbours.
# ---------------------------------------------------------------------------
def fixed_ascii(value, size: int, name: str) -> bytes:
    """Fixed 16B ASCII slot: the string must fit; unused bytes are 0xFF."""
    if not isinstance(value, str):
        fail(f"{name} must be an ASCII string of at most {size} bytes, got {value!r}")
    b = value.encode("ascii")
    if len(b) > size:
        fail(f"{name} must be at most {size} ASCII bytes, got {len(b)} ({value!r})")
    return b + b"\xFF" * (size - len(b))


def build_frame_hdr(cfg, flash_id, fwsize, crc32data, crc32hdr):
    hdr = struct.pack(
        "<IB3xII16s16s16s16s16s16s12sI",    # FrameHead_t, 128B
        crc32data,                          # +0x00 payload CRC32
        flash_id,                           # +0x04 FLASH_ID_xxx
        num(cfg["store_addr"], "store_addr"),  # +0x08
        fwsize,                             # +0x0C payload size
        fixed_ascii(cfg["company"], FRAME_STR_SIZE, "company"),
        fixed_ascii(cfg["hdver"], FRAME_STR_SIZE, "hdver"),
        fixed_ascii(cfg["swver"], FRAME_STR_SIZE, "swver"),
        fixed_ascii(cfg["modulename"], FRAME_STR_SIZE, "modulename"),
        b"\xFF" * 16,                       # +0x50 rsvd0
        b"\xFF" * 16,                       # +0x60 rsvd1
        b"\xFF" * 12,                       # +0x70 rsvd2
        crc32hdr,                           # +0x7C header CRC32 (0 during the pass)
    )
    assert len(hdr) == FRAME_HDR_SIZE
    return hdr


def parse_frame_hdr(hdr: bytes):
    (crc32data, flash_id, storeaddr, fwsize, company, hdver, swver,
     modulename, _r0, _r1, _r2, crc32hdr) = struct.unpack_from(
        "<IB3xII16s16s16s16s16s16s12sI", hdr, 0)

    def s(b: bytes) -> str:
        return b.rstrip(b"\xFF").decode("ascii", "replace")

    return {
        "crc32data": crc32data, "flash_id": flash_id, "storeaddr": storeaddr,
        "fwsize": fwsize, "company": s(company), "hdver": s(hdver),
        "swver": s(swver), "modulename": s(modulename), "crc32hdr": crc32hdr,
    }


# ---------------------------------------------------------------------------
# Pack
# ---------------------------------------------------------------------------
def pack(cfg: dict, raw: bytes, out_path: Path) -> None:
    name = cfg.get("flash_id")
    if name not in REGIONS:
        fail(f"flash_id must be one of {sorted(REGIONS)}, got {name!r}")
    if name == "FLASH_ID_APP_B":
        fail("APP upgrade packages must be A-named (flash_id FLASH_ID_APP_A, "
             "store_addr 0x08010000); slot B only receives the A-mirror image")
    _fid, base, region_size = REGIONS[name]

    # Drop keys of the old gfw-era layouts; the new frame has no place for
    # them and a stale JSON must not keep carrying dead fields.
    for stale in LEGACY_KEYS:
        cfg.pop(stale, None)

    # Read-only display fields: recomputed below, any hand edit discarded.
    for ro in READONLY_KEYS:
        cfg.pop(ro, None)

    store_addr = num(cfg["store_addr"], "store_addr")
    if store_addr != base:
        fail(f"store_addr 0x{store_addr:08X} does not match {name} base 0x{base:08X}")

    limit = APP_BIN_LIMIT if name in ("FLASH_ID_APP_A", "FLASH_ID_APP_B") \
        else region_size - FRAME_HDR_AREA_SIZE

    if len(raw) == 0 or len(raw) > limit:
        fail(f"raw image size {len(raw)} out of range (1..0x{limit:X})")
    pad = (-len(raw)) % 8
    payload = raw + b"\xFF" * pad
    fwsize = len(payload)

    # Vector sanity: the image must have been linked at store_addr + 0x200.
    link_base = store_addr + FRAME_PAYLOAD_OFF
    msp = u32(payload, 0)
    reset = u32(payload, 4) & ~1
    if not (SRAM_BASE <= msp < SRAM_BASE + SRAM_SIZE):
        fail(f"initial MSP 0x{msp:08X} outside SRAM "
             f"[0x{SRAM_BASE:08X}, 0x{SRAM_BASE + SRAM_SIZE:08X})")
    if not (link_base <= reset < link_base + len(raw)):
        fail(f"reset vector 0x{reset:08X} not inside the image linked at "
             f"0x{link_base:08X} .. 0x{link_base + len(raw):08X}; "
             f"re-link the project at the target base (APP_A IROM = 0x08010000)")

    crc32data = crc32_gd32(payload)

    # 128B header with crc32hdr = 0, then the header CRC over itself.
    hdr = build_frame_hdr(cfg, _fid, fwsize, crc32data, 0)
    crc32hdr = crc32_gd32(hdr)
    hdr = build_frame_hdr(cfg, _fid, fwsize, crc32data, crc32hdr)

    # 512B area: FrameHead_t (128B) + rsvd -> 0xFF.
    area = bytearray(b"\xFF" * FRAME_HDR_AREA_SIZE)
    area[0:FRAME_HDR_SIZE] = hdr

    framed = bytes(area) + payload
    if len(framed) % 8 != 0:
        framed += b"\xFF" * ((-len(framed)) % 8)

    out_path.parent.mkdir(parents=True, exist_ok=True)
    out_path.write_bytes(framed)

    # Write computed fields back so the JSON records the packed header.
    cfg.update({
        "fwsize": fwsize,
        "crc32data": f"0x{crc32data:08X}",
        "crc32hdr": f"0x{crc32hdr:08X}",
    })
    print(f"wrote framed image: {out_path} ({len(framed)} B)")
    print_header(parse_frame_hdr(hdr))


# ---------------------------------------------------------------------------
# Verify (mirrors BoardFlashVerifyHdr + VerifySlotImage)
# ---------------------------------------------------------------------------
def print_header(h):
    fid = h["flash_id"]
    fname = next((k for k, v in REGIONS.items() if v[0] == fid), f"<{fid}>")
    print("frame header:")
    print(f"  company        {h['company']!r}")
    print(f"  hdver          {h['hdver']!r}")
    print(f"  swver          {h['swver']!r}")
    print(f"  modulename     {h['modulename']!r}")
    print(f"  flash_id       {fname} ({fid})")
    print(f"  storeaddr      0x{h['storeaddr']:08X}")
    print(f"  fwsize         0x{h['fwsize']:X} ({h['fwsize']} B)")
    print(f"  crc32data      0x{h['crc32data']:08X}")
    print(f"  crc32hdr       0x{h['crc32hdr']:08X}")


def verify(cfg: dict, framed: bytes) -> bool:
    ok = True

    def check(cond, msg):
        nonlocal ok
        print(f"  [{'OK' if cond else 'FAIL'}] {msg}")
        ok = ok and cond

    if len(framed) < FRAME_HDR_AREA_SIZE:
        fail("framed image smaller than the 512B header area")

    h = parse_frame_hdr(framed[:FRAME_HDR_SIZE])

    print_header(h)

    fid = h["flash_id"]
    if fid == REGIONS["FLASH_ID_APP_B"][0]:
        fail("B-named APP package rejected: packages must be A-named "
             "(slot B only receives the A-mirror image)")
    region = next((v for v in REGIONS.values() if v[0] == fid), None)
    if region is None:
        fail(f"flash_id {fid} is not a user-upgradeable region")
    _rid, base, region_size = region
    is_app = fid in (REGIONS["FLASH_ID_APP_A"][0], REGIONS["FLASH_ID_APP_B"][0])

    print("checks:")
    check(h["company"] == FRAME_COMPANY, f"company == {FRAME_COMPANY!r} (got {h['company']!r})")
    check(h["storeaddr"] == base, f"storeaddr 0x{h['storeaddr']:08X} == region base 0x{base:08X}")
    limit = APP_BIN_LIMIT if is_app else region_size - FRAME_HDR_AREA_SIZE
    check(0 < h["fwsize"] <= limit, f"0 < fwsize <= 0x{limit:X}")
    check(h["fwsize"] % 8 == 0, "fwsize 8-aligned")

    hdr = bytearray(framed[:FRAME_HDR_SIZE])
    struct.pack_into("<I", hdr, FRAME_CRC32_HDR_OFF, 0)  # crc32hdr counted as 0
    check(crc32_gd32(bytes(hdr)) == h["crc32hdr"], "crc32hdr matches")

    check(framed[FRAME_STR_SIZE * 5:FRAME_CRC32_HDR_OFF] ==
          b"\xFF" * (FRAME_CRC32_HDR_OFF - FRAME_STR_SIZE * 5)
          and framed[FRAME_HDR_SIZE:FRAME_HDR_AREA_SIZE] ==
          b"\xFF" * (FRAME_HDR_AREA_SIZE - FRAME_HDR_SIZE),
          "rsvd areas (+0x50..+0x7B, +0x80..+0x1FF) all 0xFF")

    body_len = h["fwsize"]
    check(len(framed) >= FRAME_PAYLOAD_OFF + body_len, "image contains the full payload")
    if len(framed) < FRAME_PAYLOAD_OFF + body_len:
        return False

    body = framed[FRAME_PAYLOAD_OFF:FRAME_PAYLOAD_OFF + body_len]
    check(crc32_gd32(body) == h["crc32data"], "crc32data matches payload")

    link_base = base + FRAME_PAYLOAD_OFF
    msp = u32(framed, FRAME_PAYLOAD_OFF)
    reset = u32(framed, FRAME_PAYLOAD_OFF + 4) & ~1
    check(SRAM_BASE <= msp < SRAM_BASE + SRAM_SIZE, f"MSP 0x{msp:08X} in SRAM")
    check(link_base <= reset < link_base + (region_size - FRAME_HDR_AREA_SIZE),
          f"reset vector 0x{reset:08X} in [link_base, link_base + slot)")

    print("verify:", "PASS" if ok else "FAIL")
    return ok


# ---------------------------------------------------------------------------
def main():
    ap = argparse.ArgumentParser(description="Pack/verify a framed region image "
                                             "(512B frame header area + payload).")
    ap.add_argument("--bin", type=Path, default=DEFAULT_BIN,
                    help=f"raw input bin (default {DEFAULT_BIN})")
    ap.add_argument("--json", type=Path, default=DEFAULT_JSON,
                    help=f"frame header JSON (default {DEFAULT_JSON}); "
                         f"created with APP_A defaults when missing")
    ap.add_argument("--out", type=Path, default=DEFAULT_OUT,
                    help=f"framed output bin (default {DEFAULT_OUT})")
    ap.add_argument("--verify", action="store_true",
                    help="verify an existing framed bin instead of packing")
    args = ap.parse_args()

    cfg = load_config(args.json)

    if args.verify:
        if not args.out.exists():
            fail(f"framed bin not found: {args.out}")
        ok = verify(cfg, args.out.read_bytes())
        save_config(args.json, cfg)   # keep JSON in sync (drops stale keys)
        sys.exit(0 if ok else 1)

    if not args.bin.exists():
        fail(f"raw bin not found: {args.bin} (build the Keil project first)")
    pack(cfg, args.bin.read_bytes(), args.out)
    save_config(args.json, cfg)
    print(f"updated frame header JSON: {args.json}")


if __name__ == "__main__":
    main()

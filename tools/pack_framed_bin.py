#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Pack a raw image into a framed image (512B frame header area + payload).

FrameHead_t describes the LOGICAL image, not a physical FlashId_t:

    +0x000  FrameHead_t 112B  crc32data|type|rsvd[3]|storeaddr|fwsize,
                              company[16] / hdver[16] / swver[16] /
                              modulename[16], rsvd[16], rsvd[12]|crc32hdr
    +0x070  rsvd         400B -> 0xFF
    +0x200  payload   fwsize B (unpadded; module pads flash writes as needed)

type is a FlashType_t value and storeaddr is the logical storage address for
that type. Valid pairs are defined by ROUTES below and must match the CDB route
table and FrameTargetsSlot in firmware:

    FLASH_TYPE_APP      + 0x08010000 -> physical APP_B upgrade receiver
    FLASH_TYPE_PARAM    + main/backup base -> physical PARA_A/PARA_B
    FLASH_TYPE_MISC     + A/B base (generic packing only; not a CDB target)
    FLASH_TYPE_EXTFLASH + 0 -> physical DSP_FW/U15 (APP-only CDB target)

For U15, this framed file is the CDB transport package. The raw DSP payload is
programmed at external NOR offset 0 and the 512B management frame is stored at
0x7F000 by firmware; do not program this whole framed file at U15 offset 0.

CRC convention matches the GD32 on-chip CRC peripheral: poly 0x04C11DB7,
init 0xFFFFFFFF, no reflection/final xor, little-endian 32-bit input words.
Lengths must be a multiple of 4.

Usage (run from the project root):
    python tools/pack_framed_bin.py
    python tools/pack_framed_bin.py --json tools/400G_SR4_OSFP_FRAMEHEAD.json \
           --bin bin/400G_SR4_OSFP.bin --out bin/400G_SR4_OSFP_framed.bin
    python tools/pack_framed_bin.py --verify
"""

import argparse
import json
import struct
import sys
from pathlib import Path

# ---------------------------------------------------------------------------
# Constants mirrored from resource_impl/memory_map.h + flashresource_impl.h.
# ---------------------------------------------------------------------------
SRAM_BASE = 0x20000000
SRAM_SIZE = 0x00020000

FRAME_HDR_AREA_SIZE = 0x00000200
FRAME_PAYLOAD_OFF = 0x00000200
FRAME_HDR_SIZE = 112
FRAME_STR_SIZE = 16
FRAME_CRC32_HDR_OFF = 0x6C
FRAME_COMPANY = "Terabitcom"
FRAME_HDVER = "HF2026090614"
FRAME_SWVER = "SW2026092614"
FRAME_MODULENAME = "400G-SR4-OSFP-V1"
APP_BIN_LIMIT = 0x0002DE00
DSP_FW_PAYLOAD_CAP = 0x0007F000

# name -> (FlashType_t value, {storeaddr: route geometry}). "vector" is true
# only for a linked Cortex-M APP image; parameter/data/DSP images have no MSP
# or reset-vector checks.
ROUTES = {
    "FLASH_TYPE_APP": (4, {
        0x08010000: {"region": "APP_A", "size": 0x0002E000,
                     "limit": APP_BIN_LIMIT, "vector": True},
    }),
    "FLASH_TYPE_PARAM": (5, {
        0x0806C000: {"region": "PARA_A", "size": 0x00008000,
                     "limit": 0x00008000 - FRAME_HDR_AREA_SIZE, "vector": False},
        0x08074000: {"region": "PARA_B", "size": 0x00008000,
                     "limit": 0x00008000 - FRAME_HDR_AREA_SIZE, "vector": False},
    }),
    "FLASH_TYPE_MISC": (6, {
        0x0807C000: {"region": "MISC_A", "size": 0x00002000,
                     "limit": 0x00002000 - FRAME_HDR_AREA_SIZE, "vector": False},
        0x0807E000: {"region": "MISC_B", "size": 0x00002000,
                     "limit": 0x00002000 - FRAME_HDR_AREA_SIZE, "vector": False},
    }),
    "FLASH_TYPE_EXTFLASH": (7, {
        0x00000000: {"region": "DSP_FW", "size": 0x00080000,
                     "limit": DSP_FW_PAYLOAD_CAP, "vector": False},
    }),
}

# Migration for JSON files written by the former flash_id-based packer.
LEGACY_FLASH_ID_TO_TYPE = {
    "FLASH_ID_APP_A": "FLASH_TYPE_APP",
    "FLASH_ID_APP_B": "FLASH_TYPE_APP",
    "FLASH_ID_PARA_A": "FLASH_TYPE_PARAM",
    "FLASH_ID_PARA_B": "FLASH_TYPE_PARAM",
    "FLASH_ID_MISC_A": "FLASH_TYPE_MISC",
    "FLASH_ID_MISC_B": "FLASH_TYPE_MISC",
    "FLASH_ID_DSP_FW": "FLASH_TYPE_EXTFLASH",
}

PROJECT_ROOT = Path(__file__).resolve().parent.parent
DEFAULT_BIN = PROJECT_ROOT / "bin" / "400G_SR4_OSFP.bin"
DEFAULT_JSON = PROJECT_ROOT / "tools" / "400G_SR4_OSFP_FRAMEHEAD.json"
DEFAULT_OUT = PROJECT_ROOT / "bin" / "400G_SR4_OSFP_framed.bin"

LEGACY_KEYS = (
    "magic", "fw_ver", "hdr_ver", "hdrver", "hw_ver", "attrs",
    "anti_rollback", "link_addr", "fw_size", "entry_off", "reloc_off",
    "reloc_cnt", "crc32_pkg", "crc32_data", "crc32_hdr", "patch_type",
    "reloc", "patch_delta", "build_date", "build_time",
)
READONLY_KEYS = ("fwsize", "crc32data", "crc32hdr")


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
    return {
        "company": FRAME_COMPANY,
        "hdver": FRAME_HDVER,
        "flash_type": "FLASH_TYPE_APP",
        "store_addr": "0x08010000",
        "swver": FRAME_SWVER,
        "modulename": FRAME_MODULENAME,
    }


def migrate_config(cfg: dict) -> None:
    if "flash_type" not in cfg and "flash_id" in cfg:
        old = cfg["flash_id"]
        if old in LEGACY_FLASH_ID_TO_TYPE:
            cfg["flash_type"] = LEGACY_FLASH_ID_TO_TYPE[old]
    cfg.pop("flash_id", None)


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
    if isinstance(v, int):
        return v
    if isinstance(v, str):
        return int(v, 16) if v.lower().startswith("0x") else int(v)
    fail(f"bad numeric value for {name}: {v!r}")


def config_route(cfg: dict):
    name = cfg.get("flash_type")
    if name not in ROUTES:
        fail(f"flash_type must be one of {sorted(ROUTES)}, got {name!r}")
    type_id, addresses = ROUTES[name]
    store_addr = num(cfg["store_addr"], "store_addr")
    if store_addr not in addresses:
        valid = ", ".join(f"0x{x:08X}" for x in addresses)
        fail(f"{name} store_addr 0x{store_addr:08X} is invalid; expected {valid}")
    return name, type_id, store_addr, addresses[store_addr]


# ---------------------------------------------------------------------------
# Header serialization
# ---------------------------------------------------------------------------
def fixed_ascii(value, size: int, name: str) -> bytes:
    if not isinstance(value, str):
        fail(f"{name} must be an ASCII string of at most {size} bytes, got {value!r}")
    b = value.encode("ascii")
    if len(b) > size:
        fail(f"{name} must be at most {size} ASCII bytes, got {len(b)} ({value!r})")
    return b + b"\xFF" * (size - len(b))


def build_frame_hdr(cfg, flash_type, fwsize, crc32data, crc32hdr):
    hdr = struct.pack(
        "<IB3sII16s16s16s16s16s12sI",
        crc32data,
        flash_type,
        b"\xFF" * 3,
        num(cfg["store_addr"], "store_addr"),
        fwsize,
        fixed_ascii(cfg["company"], FRAME_STR_SIZE, "company"),
        fixed_ascii(cfg["hdver"], FRAME_STR_SIZE, "hdver"),
        fixed_ascii(cfg["swver"], FRAME_STR_SIZE, "swver"),
        fixed_ascii(cfg["modulename"], FRAME_STR_SIZE, "modulename"),
        b"\xFF" * 16,
        b"\xFF" * 12,
        crc32hdr,
    )
    assert len(hdr) == FRAME_HDR_SIZE
    return hdr


def parse_frame_hdr(hdr: bytes):
    (crc32data, flash_type, _pad, storeaddr, fwsize, company, hdver, swver,
     modulename, _r1, _r2, crc32hdr) = struct.unpack_from(
        "<IB3sII16s16s16s16s16s12sI", hdr, 0)

    def s(b: bytes) -> str:
        return b.rstrip(b"\xFF").decode("ascii", "replace")

    return {
        "crc32data": crc32data, "flash_type": flash_type, "storeaddr": storeaddr,
        "fwsize": fwsize, "company": s(company), "hdver": s(hdver),
        "swver": s(swver), "modulename": s(modulename), "crc32hdr": crc32hdr,
    }


# ---------------------------------------------------------------------------
def pack(cfg: dict, raw: bytes, out_path: Path) -> None:
    name, type_id, store_addr, route = config_route(cfg)

    for stale in LEGACY_KEYS:
        cfg.pop(stale, None)
    for ro in READONLY_KEYS:
        cfg.pop(ro, None)

    limit = route["limit"]
    if len(raw) == 0 or len(raw) > limit:
        fail(f"raw image size {len(raw)} out of range (1..0x{limit:X})")
    if len(raw) % 4 != 0:
        fail(f"raw image size {len(raw)} is not word-aligned (multiple of 4 required)")

    payload = raw
    fwsize = len(raw)

    if route["vector"]:
        link_base = store_addr + FRAME_PAYLOAD_OFF
        msp = u32(payload, 0)
        reset = u32(payload, 4) & ~1
        if not (SRAM_BASE <= msp < SRAM_BASE + SRAM_SIZE):
            fail(f"initial MSP 0x{msp:08X} outside SRAM")
        if not (link_base <= reset < link_base + len(raw)):
            fail(f"reset vector 0x{reset:08X} outside APP image linked at "
                 f"0x{link_base:08X}")

    crc32data = crc32_gd32(payload)
    hdr = build_frame_hdr(cfg, type_id, fwsize, crc32data, 0)
    crc32hdr = crc32_gd32(hdr)
    hdr = build_frame_hdr(cfg, type_id, fwsize, crc32data, crc32hdr)

    area = bytearray(b"\xFF" * FRAME_HDR_AREA_SIZE)
    area[0:FRAME_HDR_SIZE] = hdr
    framed = bytes(area) + payload

    out_path.parent.mkdir(parents=True, exist_ok=True)
    out_path.write_bytes(framed)
    cfg.update({
        "fwsize": fwsize,
        "crc32data": f"0x{crc32data:08X}",
        "crc32hdr": f"0x{crc32hdr:08X}",
    })
    print(f"wrote framed image: {out_path} ({len(framed)} B), route={name}/{route['region']}")
    print_header(parse_frame_hdr(hdr))


# ---------------------------------------------------------------------------
def print_header(h):
    type_id = h["flash_type"]
    type_name = next((k for k, v in ROUTES.items() if v[0] == type_id), f"<{type_id}>")
    print("frame header:")
    print(f"  company        {h['company']!r}")
    print(f"  hdver          {h['hdver']!r}")
    print(f"  swver          {h['swver']!r}")
    print(f"  modulename     {h['modulename']!r}")
    print(f"  type           {type_name} ({type_id})")
    print(f"  storeaddr      0x{h['storeaddr']:08X}")
    print(f"  fwsize         0x{h['fwsize']:X} ({h['fwsize']} B)")
    print(f"  crc32data      0x{h['crc32data']:08X}")
    print(f"  crc32hdr       0x{h['crc32hdr']:08X}")


def verify(cfg: dict, framed: bytes) -> bool:
    del cfg  # Route and all checked fields come from the framed image itself.
    ok = True

    def check(cond, msg):
        nonlocal ok
        print(f"  [{'OK' if cond else 'FAIL'}] {msg}")
        ok = ok and cond

    if len(framed) < FRAME_HDR_AREA_SIZE:
        fail("framed image smaller than the 512B header area")

    h = parse_frame_hdr(framed[:FRAME_HDR_SIZE])
    print_header(h)

    found = None
    for name, (type_id, addresses) in ROUTES.items():
        if type_id == h["flash_type"] and h["storeaddr"] in addresses:
            found = (name, addresses[h["storeaddr"]])
            break
    if found is None:
        fail(f"invalid type+storeaddr pair: type={h['flash_type']}, "
             f"storeaddr=0x{h['storeaddr']:08X}")
    name, route = found

    print("checks:")
    check(h["company"] == FRAME_COMPANY, f"company == {FRAME_COMPANY!r}")
    check(True, f"type+storeaddr route == {name}/{route['region']}")
    limit = route["limit"]
    check(0 < h["fwsize"] <= limit, f"0 < fwsize <= 0x{limit:X}")
    check(h["fwsize"] % 4 == 0, "fwsize 4-aligned")

    hdr = bytearray(framed[:FRAME_HDR_SIZE])
    struct.pack_into("<I", hdr, FRAME_CRC32_HDR_OFF, 0)
    check(crc32_gd32(bytes(hdr)) == h["crc32hdr"], "crc32hdr matches")
    check(framed[FRAME_STR_SIZE * 5:FRAME_CRC32_HDR_OFF] ==
          b"\xFF" * (FRAME_CRC32_HDR_OFF - FRAME_STR_SIZE * 5)
          and framed[FRAME_HDR_SIZE:FRAME_HDR_AREA_SIZE] ==
          b"\xFF" * (FRAME_HDR_AREA_SIZE - FRAME_HDR_SIZE),
          "reserved areas are all 0xFF")

    body_len = h["fwsize"]
    check(len(framed) >= FRAME_PAYLOAD_OFF + body_len, "image contains full payload")
    if len(framed) < FRAME_PAYLOAD_OFF + body_len:
        return False
    body = framed[FRAME_PAYLOAD_OFF:FRAME_PAYLOAD_OFF + body_len]
    check(len(body) % 4 == 0 and crc32_gd32(body) == h["crc32data"],
          "crc32data matches payload")

    if route["vector"]:
        link_base = h["storeaddr"] + FRAME_PAYLOAD_OFF
        msp = u32(framed, FRAME_PAYLOAD_OFF)
        reset = u32(framed, FRAME_PAYLOAD_OFF + 4) & ~1
        check(SRAM_BASE <= msp < SRAM_BASE + SRAM_SIZE, "initial MSP is in SRAM")
        check(link_base <= reset < link_base + body_len, "reset vector is inside APP payload")

    print("verify:", "PASS" if ok else "FAIL")
    return ok


# ---------------------------------------------------------------------------
def main():
    ap = argparse.ArgumentParser(description="Pack/verify a type+storeaddr framed image.")
    ap.add_argument("--bin", type=Path, default=DEFAULT_BIN)
    ap.add_argument("--json", type=Path, default=DEFAULT_JSON)
    ap.add_argument("--out", type=Path, default=DEFAULT_OUT)
    ap.add_argument("--verify", action="store_true")
    args = ap.parse_args()

    cfg = load_config(args.json)
    migrate_config(cfg)

    if args.verify:
        if not args.out.exists():
            fail(f"framed bin not found: {args.out}")
        ok = verify(cfg, args.out.read_bytes())
        save_config(args.json, cfg)
        sys.exit(0 if ok else 1)

    if not args.bin.exists():
        fail(f"raw bin not found: {args.bin}")
    pack(cfg, args.bin.read_bytes(), args.out)
    save_config(args.json, cfg)
    print(f"updated frame header JSON: {args.json}")


if __name__ == "__main__":
    main()

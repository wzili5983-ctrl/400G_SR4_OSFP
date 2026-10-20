#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Merge the bootloader and APP slot-A framed image into one full-chip image
(Intel HEX and/or raw bin) for a JTAG / programmer download.

Why not merge the two Keil .hex files directly:
  * the APP .hex is flat (payload starts at 0x08010200) and lacks the 512B
    frame header at 0x08010000, so the bootloader's VerifySlotImage would
    reject slot A and fall into the rescue loop;
  * both .hex files carry RAM shadow records (0x20005700) that must not be
    programmed to flash.
The flat bins have neither problem: 400G_SR4_OSFP_framed.bin already contains the
512B header, and the bins hold pure flash content.

Layout (tools/pack_framed_bin.py ROUTES + resource_impl/memory_map.h):
    0x08000000  BL       32KB   (P0-P3, WRP)
    0x08010000  APP_A    184KB  framed image (512B header + payload)
    0x0803E000  APP_B    184KB  A-mirror (written only with --mirror-b)
    0x0806C000  PARA_A   32KB
    0x08074000  PARA_B   32KB
    0x0807C000  MISC_A   8KB
    0x0807E000  MISC_B   8KB
Everything else (KEY, metadata J-A/J-B, blackbox, APP_B, PARA/MISC) stays
erased (0xFF) in a factory image: the bootloader skips promotion (no valid
OtaInfo_t) and validates/boots slot A directly.

Usage (from the project root):
    python tools/make_full_hex.py                 # BL + APP_A (minimal HEX)
    python tools/make_full_hex.py --bin-out       # also write a raw full-chip bin
    python tools/make_full_hex.py --mirror-b      # also write the A-mirror to APP_B
    python tools/make_full_hex.py --fill          # pad the whole 512KB with 0xFF
    python tools/make_full_hex.py --verify        # re-read the hex, sanity check
"""

import argparse
import struct
import sys
from pathlib import Path

# ---------------------------------------------------------------------------
# Flash geometry (keep in sync with tools/pack_framed_bin.py and
# resource_impl/memory_map.h).
# ---------------------------------------------------------------------------
FLASH_BASE = 0x08000000
FLASH_SIZE = 0x00080000            # 512KB total
BL_BASE = 0x08000000
BL_SIZE = 0x00008000               # 32KB bootloader region
APP_A_BASE = 0x08010000
APP_B_BASE = 0x0803E000
APP_SLOT_SIZE = 0x0002E000         # 184KB APP slot
FRAME_HDR_AREA_SIZE = 0x200        # 512B frame header area
FLASH_TYPE_APP = 4

HEX_LINE_BYTES = 16

PROJECT_ROOT = Path(__file__).resolve().parent.parent
DEFAULT_BL = PROJECT_ROOT / "bin" / "400G_SR4_OSFP_BL.bin"
DEFAULT_APP = PROJECT_ROOT / "bin" / "400G_SR4_OSFP_framed.bin"
DEFAULT_OUT = PROJECT_ROOT / "hex" / "400G_SR4_OSFP_full.hex"
DEFAULT_BIN_OUT = PROJECT_ROOT / "bin" / "400G_SR4_OSFP_full.bin"


def fail(msg: str) -> "NoReturn":
    print(f"error: {msg}", file=sys.stderr)
    sys.exit(1)


def u32(b: bytes, off: int) -> int:
    return struct.unpack_from("<I", b, off)[0]


# ---------------------------------------------------------------------------
# Intel HEX emission
# ---------------------------------------------------------------------------
def _ihex_record(rectype: int, addr: int, data: bytes = b"") -> str:
    assert 0 <= addr <= 0xFFFF
    body = bytes((len(data), (addr >> 8) & 0xFF, addr & 0xFF, rectype)) + data
    csum = (-sum(body)) & 0xFF
    return ":" + (body + bytes((csum,))).hex().upper()


def _emit_segment(lines, base: int, data: bytes) -> None:
    """Emit one contiguous byte range as type-04 base + type-00 data records."""
    upper = None
    for off in range(0, len(data), HEX_LINE_BYTES):
        chunk = data[off:off + HEX_LINE_BYTES]
        addr = base + off
        ub = (addr >> 16) & 0xFFFF
        if ub != upper:
            # Extended linear address: 2 data bytes = upper 16 bits of addr.
            lines.append(_ihex_record(0x04, 0, bytes(((ub >> 8) & 0xFF, ub & 0xFF))))
            upper = ub
        lines.append(_ihex_record(0x00, addr & 0xFFFF, chunk))


# ---------------------------------------------------------------------------
# Input validation
# ---------------------------------------------------------------------------
def _check_framed(framed: bytes) -> None:
    """Reject the flat bin: the framed image must carry a valid APP_A header."""
    if len(framed) < FRAME_HDR_AREA_SIZE:
        fail(f"APP image {len(framed)} B is smaller than the 512B frame header "
             f"area -- did you pass the flat bin? use bin/400G_SR4_OSFP_framed.bin")
    image_type = framed[4]
    storeaddr = u32(framed, 8)
    fwsize = u32(framed, 12)
    if image_type != FLASH_TYPE_APP:
        fail(f"APP frame header type={image_type}, expected FLASH_TYPE_APP "
             f"({FLASH_TYPE_APP}); use the framed image, not the flat bin")
    if storeaddr != APP_A_BASE:
        fail(f"APP frame header storeaddr=0x{storeaddr:08X}, expected "
             f"0x{APP_A_BASE:08X}")
    if fwsize != len(framed) - FRAME_HDR_AREA_SIZE:
        fail(f"APP frame header fwsize={fwsize} != payload size "
             f"{len(framed) - FRAME_HDR_AREA_SIZE}")


def _build_segments(bl: bytes, framed: bytes, mirror_b: bool):
    segs = [(BL_BASE, bl), (APP_A_BASE, framed)]
    if mirror_b:
        segs.append((APP_B_BASE, framed))
    segs.sort()
    # Overlap guard: BL end must not reach APP_A base, APP_A end not APP_B, etc.
    for (a0, d0), (a1, _d1) in zip(segs, segs[1:]):
        if a0 + len(d0) > a1:
            fail(f"segments overlap: 0x{a0:08X}+0x{len(d0):X} reaches into "
                 f"0x{a1:08X}")
    return segs


def _full_flash_buffer(segs) -> bytes:
    """512KB raw flash image (0xFF erased) with the segments overlaid. A raw
    bin has no address metadata, so it is always the full 0x08000000..0x08080000
    range -- load it at FLASH_BASE (0x08000000)."""
    buf = bytearray(b"\xFF" * FLASH_SIZE)
    for base, data in segs:
        buf[base - FLASH_BASE:base - FLASH_BASE + len(data)] = data
    return bytes(buf)


# ---------------------------------------------------------------------------
# Verify (re-read a produced hex)
# ---------------------------------------------------------------------------
def _parse_hex(path: Path):
    """Return {address: byte} for all type-00 records in an Intel HEX file."""
    out = {}
    base = 0
    with path.open("r", encoding="ascii") as f:
        for lineno, raw in enumerate(f, 1):
            line = raw.strip()
            if not line:
                continue
            if line[0] != ":":
                fail(f"{path}:{lineno}: not an Intel HEX record")
            b = bytes.fromhex(line[1:])
            if len(b) < 5 or b[0] != len(b) - 5:
                fail(f"{path}:{lineno}: bad record length")
            if (sum(b) & 0xFF) != 0:
                fail(f"{path}:{lineno}: bad checksum")
            cnt, addr, rectype = b[0], (b[1] << 8) | b[2], b[3]
            data = b[4:4 + cnt]
            if rectype == 0x00:
                full = base + addr
                for i, byte in enumerate(data):
                    out[full + i] = byte
            elif rectype == 0x01:
                break
            elif rectype == 0x04:
                base = ((data[0] << 8) | data[1]) << 16
            elif rectype == 0x02:
                base = ((data[0] << 8) | data[1]) << 4
            # other record types ignored
    return out


def _ranges(imap):
    addrs = sorted(imap)
    if not addrs:
        return []
    ranges = []
    start = prev = addrs[0]
    for a in addrs[1:]:
        if a != prev + 1:
            ranges.append((start, prev))
            start = a
        prev = a
    ranges.append((start, prev))
    return ranges


def verify_hex(path: Path) -> None:
    if not path.exists():
        fail(f"hex not found: {path}")
    imap = _parse_hex(path)
    print(f"parsed {path}: {len(imap)} bytes in {len(_ranges(imap))} range(s)")
    for a, b in _ranges(imap):
        print(f"  0x{a:08X} .. 0x{b:08X}  ({b - a + 1} B)")

    ok = True

    def check(cond, msg):
        nonlocal ok
        print(f"  [{'OK' if cond else 'FAIL'}] {msg}")
        ok = ok and cond

    # BL present at its base.
    check(BL_BASE in imap, f"BL present at 0x{BL_BASE:08X}")
    # APP_A present with a valid frame header.
    if APP_A_BASE in imap:
        hdr = bytes(imap.get(APP_A_BASE + i, 0xFF) for i in range(FRAME_HDR_AREA_SIZE))
        image_type = hdr[4]
        storeaddr = u32(hdr, 8)
        check(image_type == FLASH_TYPE_APP,
              f"APP_A frame header type == FLASH_TYPE_APP (got {image_type})")
        check(storeaddr == APP_A_BASE, f"APP_A frame header storeaddr == 0x{APP_A_BASE:08X}")
    else:
        check(False, "APP_A present at 0x08010000")

    print("verify:", "PASS" if ok else "FAIL")
    sys.exit(0 if ok else 1)


# ---------------------------------------------------------------------------
def main():
    ap = argparse.ArgumentParser(description="Merge BL + APP_A framed image "
                                             "into a full-chip Intel HEX / raw bin.")
    ap.add_argument("--bl", type=Path, default=DEFAULT_BL,
                    help=f"BL flat bin (default {DEFAULT_BL})")
    ap.add_argument("--app", type=Path, default=DEFAULT_APP,
                    help=f"APP_A framed bin (default {DEFAULT_APP})")
    ap.add_argument("--out", type=Path, default=DEFAULT_OUT,
                    help=f"merged Intel HEX output (default {DEFAULT_OUT})")
    ap.add_argument("--bin-out", type=Path, nargs="?", const=DEFAULT_BIN_OUT,
                    default=None,
                    help=f"also write a raw full-chip bin (gaps 0xFF, load at "
                         f"0x08000000); default path {DEFAULT_BIN_OUT}")
    ap.add_argument("--fill", action="store_true",
                    help="pad the whole 512KB flash with 0xFF (strict full-chip)")
    ap.add_argument("--mirror-b", action="store_true",
                    help="also write the A-mirror image to APP_B (0x0803E000)")
    ap.add_argument("--verify", action="store_true",
                    help="re-read an existing hex and sanity check it")
    args = ap.parse_args()

    if args.verify:
        verify_hex(args.out)
        return

    if not args.bl.exists():
        fail(f"BL bin not found: {args.bl} (build the bootloader project first)")
    if not args.app.exists():
        fail(f"APP framed bin not found: {args.app} (build + pack the APP first)")

    bl = args.bl.read_bytes()
    framed = args.app.read_bytes()

    if not (0 < len(bl) <= BL_SIZE):
        fail(f"BL bin size {len(bl)} out of range (1..0x{BL_SIZE:X})")
    _check_framed(framed)
    if not (0 < len(framed) <= APP_SLOT_SIZE):
        fail(f"APP framed image {len(framed)} B exceeds slot 0x{APP_SLOT_SIZE:X}")

    segs = _build_segments(bl, framed, args.mirror_b)
    print("segments:")
    for base, data in segs:
        print(f"  0x{base:08X}  +0x{len(data):X} ({len(data)} B)")

    full = _full_flash_buffer(segs)

    lines = []
    if args.fill:
        _emit_segment(lines, FLASH_BASE, full)
    else:
        for base, data in segs:
            _emit_segment(lines, base, data)
    lines.append(":00000001FF")

    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text("\n".join(lines) + "\n", encoding="ascii")
    print(f"wrote merged hex: {args.out} ({len(lines)} records)")

    if args.bin_out is not None:
        args.bin_out.parent.mkdir(parents=True, exist_ok=True)
        args.bin_out.write_bytes(full)
        print(f"wrote full-chip bin: {args.bin_out} ({len(full)} B)")


if __name__ == "__main__":
    main()

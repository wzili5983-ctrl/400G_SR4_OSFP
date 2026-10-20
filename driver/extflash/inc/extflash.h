/** @file extflash.h
 * @brief U15 (SST25PF040C, 4 Mbit / 512 KB external SPI NOR flash) driver —
 *        boots the DSP (U11, BCM87420).
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * U15 hangs on BUS_ID_EXTFLASH (hardware SPI2: SCK=PG9, MOSI=PG11,
 * MISO=PG10; CS net on PG12 = GPIO_ID_EXTFLASH_CS, active low).
 *
 * SHARED-BUS CONTRACT (2026-10-16): SCK/MOSI/MISO and the CS net are shared
 * with the DSP SPI master — the DSP boots its microcode from U15 over these
 * nets. The MCU owns nothing permanently: every public API below acquires
 * the bus (CS -> output high, SPI2 -> AF master), runs its command
 * session(s), and releases everything back to Hi-Z inputs before returning.
 * Callers must therefore: run in thread context, serialize externally, and
 * expect the whole call (erase included) to block. Only invoke while the DSP
 * is not actively using the bus — practically: DSP held in reset, or after
 * boot with the DSP SPI master idle (verify on bench).
 *
 * Part specifics (SST25PF040C, Microchip DS20005397F): 4 Mbit = 512 KB,
 * 24-bit addresses, 128 uniform 4 KB sectors, 256-byte pages; opcodes WREN
 * 06h / WRDI 04h / RDSR 05h / WRSR 01h (WREN-gated, one status byte) /
 * READ 03h (max 25 MHz) / PP 02h (1-256 B, wraps inside the 256 B page) /
 * SE 20h (4 KB) / BE D8h (64 KB, the only block size — 52h absent) /
 * CE 60h / RDID 9Fh -> 62h 06h 13h 00h repeating (NOT the legacy BF 25 8D,
 * and 90h Read-ID does not exist on this part). Status register: BUSY bit0,
 * WEL bit1, BP0-BP2 bits 2-4, TB bit5, BPL bit7; BP/TB/BPL are non-volatile,
 * so the power-up protection level is whatever was last programmed —
 * init reads RDSR and clears BP/TB only when non-zero (WP#/HOLD# tied
 * to 3.3 V on the board, so the status register is always writable and
 * HOLD# never stalls). Erase times: 4 KB
 * typ 40 / max 150 ms, 64 KB typ 80 / max 250 ms, chip typ 250 ms / max
 * 2 s — no erase suspend exists; a worst-case chip erase (driver WIP
 * budget 3 s) still fits the 4 s FWDGT window with ~1 s margin.
 *
 * ChipOps entry points (32-bit word semantics, matching DspRead/DspWrite and
 * the D9H mailbox convention; words are little-endian, LSB first on the wire):
 *   ExtFlashRead / ExtFlashWrite.
 * Bulk API (for the CDB DSP-microcode upgrade path):
 *   ExtFlashJedecGet / ExtFlashDataRead / ExtFlashProgram /
 *   ExtFlashEraseSector (4 KB) / ExtFlashEraseBlock64K / ExtFlashChipErase.
 *
 * The driver bypasses WriteBus/ReadBus on purpose: NOR command + address +
 * data must stay inside one CS session, while the bus layer wraps each call
 * with its own CS pulse. CS and SPI pins are driven here via mcu_spi and raw
 * GPIO on the GPIO_ID_EXTFLASH_CS net (kept Hi-Z in the GPIO resource
 * table).
 */
#ifndef EXTFLASH_H
#define EXTFLASH_H

#include "basedef.h"

DWORD InitExtFlash(BYTE rsttype);
DWORD ExtFlashWrite(BYTE id, DWORD addr, DWORD data);
DWORD ExtFlashRead(BYTE id, DWORD addr, DWORD *data);

DWORD ExtFlashJedecGet(DWORD *jedec);
DWORD ExtFlashDataRead(DWORD addr, BYTE *buf, WORD len);
DWORD ExtFlashProgram(DWORD addr, CONST BYTE *buf, WORD len);
DWORD ExtFlashEraseSector(DWORD addr);
DWORD ExtFlashEraseBlock64K(DWORD addr);
DWORD ExtFlashChipErase(VOID);

#endif

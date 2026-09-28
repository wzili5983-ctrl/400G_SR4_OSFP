/***************************************************************************/ /**
 *   @file   flashresource.h
 *   @brief  Implementation of flash resource.
 *   @author Oliver Wang (zili.wang@terabitcom.com)
********************************************************************************
 * Copyright 2024~2026(c) Terabitcom, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of Terabitcom, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Terabitcom, INC. “AS IS” AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL Terabitcom, INC. BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/
#ifndef FLASHRESOURCE_H
#define FLASHRESOURCE_H

#include "flashresource_impl.h"

/* Region frame access over the 11 memory_map.h areas (FLASH_ID_xxx);
 * offsets are relative to the region base.
 * Read runs over the memory-mapped flash (XIP); write programs 64-bit
 * doublewords (offset/len must be 8-byte aligned, target area must be erased)
 * and verifies each doubleword; erase wipes the whole region page by page.
 * WRP regions (FLASH_ID_BOOTLOADER / FLASH_ID_KEY) reject write/erase.
 */
DWORD ReadFlash(BYTE id, DWORD offset, BYTE *data, DWORD len);
DWORD WriteFlash(BYTE id, DWORD offset, CONST BYTE *data, DWORD len);
DWORD EraseFlash(BYTE id);
/* Frame header sanity check: company magic / flashid / storeaddr / fwsize /
 * header CRC32 (crc32hdr counted as 0). Valid on the 6 user-upgradeable
 * regions (FLASH_ID_APP_A/B, FLASH_ID_PARA_A/B, FLASH_ID_MISC_A/B), all
 * sharing the same 128B FrameHead_t. */
DWORD VerifyFlashHdr(BYTE id);

/* The OTA metadata ping-pong journal and the Dual-APP rollback state
 * machine were removed with the fixed-A / backup-B scheme; OTA metadata
 * J-A now holds a single OtaInfo_t upgrade record (see
 * flashresource_impl.h), accessed through the generic ReadFlash /
 * WriteFlash / EraseFlash on FLASH_ID_METADATA_A. */

/* APP upgrade helpers (fixed-A / mirror-B scheme): APP Slot A is the fixed
 * running region; APP Slot B is the A-mirror region and the SOLE upgrade
 * entry. Every APP upgrade -- including the factory / blank-device case --
 * goes through B first, and the package must be A-named (flashid
 * FLASH_ID_APP_A, storeaddr MEM_APP_A_BASE), byte-identical to the slot A
 * image; there is no B-named package. InstallSlotImage writes a received
 * APP.bin (512B frame header area with the common 128B FrameHead_t +
 * payload at +0x200) into a slot -- the normal target is FLASH_ID_APP_B;
 * the bin buffer is not modified. Packages are validated (magic /
 * targeting rules / geometry / payload CRC32 crc32data / header CRC32
 * crc32hdr) before the slot is erased; the payload is programmed first
 * and the frame header area last, so a torn install is either the old
 * image or a rejected header-less slot.
 * VerifyFrameHdrBuf is the OTA receive gate: it validates the frame header
 * in a RAM buffer (the SRAM OTA_RX buffer) BEFORE the first byte is
 * programmed into the slot -- a failed header stops the upgrade with the
 * slot untouched.
 * VerifySlotImage is the deep check: flash-readback CRC32 against
 * FrameHead_t.crc32data; run it on slot B after the install and before
 * the upgrade is confirmed. After confirmation the bootloader promotes
 * B to A with a verbatim copy (header already A-named), keeping the
 * steady-state invariant slot A == slot B. */
DWORD InstallSlotImage(BYTE id, BYTE *bin, DWORD len);
DWORD VerifySlotImage(BYTE id);
DWORD VerifyFrameHdrBuf(BYTE id, CONST BYTE *buf, DWORD len);

#endif

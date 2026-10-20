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

/* Region access over the physical FlashId_t resources. Offsets are relative
 * to each region's own address space. Internal flash is XIP and programs
 * 64-bit doublewords; FLASH_ID_DSP_FW dispatches to the U15 SPI NOR adapters.
 * WRP regions reject write/erase. */
DWORD ReadFlash(BYTE id, DWORD offset, BYTE *data, DWORD len);
DWORD WriteFlash(BYTE id, DWORD offset, CONST BYTE *data, DWORD len);
DWORD EraseFlash(BYTE id);
/* Frame header sanity check: company magic / FlashType_t type / storeaddr /
 * fwsize / header CRC32 (crc32hdr counted as 0). */
DWORD VerifyFlashHdr(BYTE id);

/* The OTA metadata ping-pong journal and the Dual-APP rollback state
 * machine were removed with the fixed-A / backup-B scheme; OTA metadata
 * J-A now holds a single OtaInfo_t upgrade record (see
 * flashresource_impl.h), accessed through the generic ReadFlash /
 * WriteFlash / EraseFlash on FLASH_ID_METADATA_A. */

/* APP upgrade helpers (fixed-A / mirror-B scheme): APP Slot A is the fixed
 * running region; APP Slot B is the SOLE APP upgrade receive region. Every
 * APP upgrade goes through B first. The package always declares
 * FLASH_TYPE_APP + MEM_APP_A_BASE, so B can be promoted to A by a verbatim
 * copy; APP_A direct download is not supported.
 * InstallSlotImage writes a received APP.bin (512B frame header area with
 * the common 112B FrameHead_t + payload at +0x200) into an APP slot, with
 * FLASH_ID_APP_B as the normal target; the input buffer is not modified.
 * Packages are validated (magic / type+storeaddr rules / geometry / payload
 * CRC32 crc32data / header CRC32 crc32hdr) before the slot is erased; the
 * payload is programmed first and the frame header area last, so a torn
 * install is either the old image or a rejected header-less slot.
 * VerifyFrameHdrBuf is the OTA receive gate: it validates the frame header
 * in a RAM buffer BEFORE the first byte is programmed into the target --
 * a failed header stops the upgrade with the target untouched.
 * VerifyFramedImage is the generic deep check: flash-readback CRC32 against
 * FrameHead_t.crc32data, for both internal framed images and DSP_FW on U15.
 * VerifySlotImage is the APP-only wrapper used before confirmation. After
 * confirmation the bootloader promotes B to A, keeping the steady-state
 * invariant slot A == slot B. */
DWORD InstallSlotImage(BYTE id, BYTE *bin, DWORD len);
DWORD VerifySlotImage(BYTE id);
DWORD VerifyFrameHdrBuf(BYTE id, CONST BYTE *buf, DWORD len);
/* Erase a region page by page, feeding the independent watchdog after each
 * page (CDB 0101h Start path); see flashresource_impl.h. */
DWORD EraseFlashWithFeed(BYTE id);
/* Confirm the pending upgrade (CDB 0109h Run): write the 64B OtaInfo_t
 * CONFIRMED record into OTA metadata J-A (see flashresource_impl.h). */
DWORD OtaConfirm(VOID);

#endif

/***************************************************************************/ /**
 *   @file   sramresource.h
 *   @brief  SRAM resource public API.
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
#ifndef SRAMRESOURCE_H
#define SRAMRESOURCE_H

#include "sramresource_impl.h"

/* SRAM region access over the 10 memory_map.h areas (SRAM_ID_xxx);
 * offsets are relative to the region base. SRAM is memory mapped: read and
 * write are plain memory copies with bounds and NULL checks
 * (offset + len must fit the region, src and dst must not overlap);
 * erase fills the whole region with 0xFF, mirroring the flash erased state
 * (use it e.g. to invalidate a crash snapshot before reuse).
 * Writes and erases of SRAM_ID_APP_RAM and SRAM_ID_STACK are rejected
 * (ERR): those regions hold live runtime data including the running stack,
 * and the NULL ops behind them lock the regions read-only by design. */
DWORD ReadSram(BYTE id, DWORD offset, BYTE *data, DWORD len);
DWORD WriteSram(BYTE id, DWORD offset, CONST BYTE *data, DWORD len);
DWORD EraseSram(BYTE id);

#endif

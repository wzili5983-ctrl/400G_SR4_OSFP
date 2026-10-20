/***************************************************************************/ /**
 *   @file   chipresource_impl.h
 *   @brief  Implementation of chip resource.
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
#ifndef CHIPRESOURCE_IMPL_H
#define CHIPRESOURCE_IMPL_H

#include "basedef.h"

typedef struct {
    BYTE chipid;
    BYTE busid;
}ChipForm_t;

typedef struct {
    DWORD (*ChipWrite)(BYTE chipid, DWORD addr, DWORD data);
    DWORD (*ChipRead)(BYTE chipid, DWORD addr, DWORD *data);
    DWORD (*ChipInit)(BYTE rsttype);
}ChipOps_t;

typedef struct {
    ChipForm_t chipform;
    ChipOps_t chipops;
}ChipInfo_t;

typedef enum {
    CHIP_ID_MCU = 0,
    CHIP_ID_START,

    CHIP_ID_DSP = CHIP_ID_START,
    CHIP_ID_DRV,
    CHIP_ID_TIA,
    CHIP_ID_PMIC,
    /* U15: SST25PF040C 512KB external SPI NOR flash, boots the DSP (U11).
     * Lives on BUS_ID_EXTFLASH (SPI2, shared with the DSP SPI master);
     * accessed via driver/extflash. */
    CHIP_ID_EXTFLASH,
    CHIP_ID_NUM
}CHIP_ID_ENUM;

#define CHIP_ID_DUMMY 0xFF

CONST ChipInfo_t *GetChipInfo(VOID);
CONST ChipInfo_t *GetChipInfoById(BYTE chipid);

#endif

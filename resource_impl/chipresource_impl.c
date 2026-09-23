/***************************************************************************/ /**
 *   @file   chipresource_impl.c
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
#include "chipresource_impl.h"
#include "busresource.h"
#include "bcm_arm_platform.h"

STATIC DWORD DspWrite(BYTE id, DWORD addr, DWORD data);
STATIC DWORD DspRead(BYTE id, DWORD addr, DWORD *data);
STATIC DWORD InitDspBus(BYTE rsttype);
STATIC DWORD InitInnerBus(BYTE rsttype);

STATIC CONST ChipInfo_t g_ChipInfoImpl[CHIP_ID_NUM - CHIP_ID_START] = {
    {{CHIP_ID_DSP, {.i2caddr = 0xA0}, BUS_ID_PHY}, {DspWrite, DspRead, InitDspBus}},
    /* Excel supplies addresses but not GN1816/GN1848 register framing.
     * Use TransferBus with the chip-specific protocol; do not assume 8-bit registers. */
    {{CHIP_ID_DRV, {.i2caddr = 0xA6}, BUS_ID_INNER}, {NULL, NULL, InitInnerBus}},
    {{CHIP_ID_TIA, {.i2caddr = 0xB6}, BUS_ID_INNER}, {NULL, NULL, InitInnerBus}}
};

CONST ChipInfo_t *GetChipInfo(VOID)
{
    return g_ChipInfoImpl;
}

CONST ChipInfo_t *GetChipInfoById(BYTE id)
{
    return id >= CHIP_ID_START && id < CHIP_ID_NUM ? &g_ChipInfoImpl[id - CHIP_ID_START] : NULL;
}

STATIC DWORD InitDspBus(BYTE rsttype)
{
    (void)rsttype;

    return InitBus(BUS_ID_PHY);
}

STATIC DWORD InitInnerBus(BYTE rsttype)
{
    (void)rsttype;
    return InitBus(BUS_ID_INNER);
}

STATIC DWORD DspWrite(BYTE id, DWORD addr, DWORD data)
{
    if(id != CHIP_ID_DSP) {
        return ERR;
    }

    return BcmArmWrite(BCM_ARM_MEMORY, 0, addr, &data, sizeof(data)) == 0 ? OK : ERR;
}

STATIC DWORD DspRead(BYTE id, DWORD addr, DWORD *data)
{
    DWORD result;

    if(id != CHIP_ID_DSP || data == NULL) {
        return ERR;
    }

    if(BcmArmRead(BCM_ARM_MEMORY, 0, addr, &result, sizeof(result)) != 0) {
        return ERR;
    }

    *data = result;
    return OK;
}

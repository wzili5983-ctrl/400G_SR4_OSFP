/***************************************************************************/ /**
 *   @file   adcresource.c
 *   @brief  Implementation of adc resource.
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
#include "adcresource.h"

DWORD ReadAdc(BYTE id, DWORD *value)
{
    CONST AdcInfo_t *info = GetAdcInfo(id);

    if(info == NULL || info->adcops.AdcRead == NULL || value == NULL) {
        return ERR;
    }

    return info->adcops.AdcRead(id, value);
}

DWORD InitAdc(BYTE id, BYTE rsttype)
{
    CONST AdcInfo_t *info = GetAdcInfo(id);

    if(info == NULL || info->adcops.AdcInit == NULL) {
        return ERR;
    }

    return info->adcops.AdcInit(id, rsttype);
}

DWORD ReadAdcMillivolts(BYTE id, DWORD *value)
{
    CONST AdcInfo_t *info = GetAdcInfo(id);
    DWORD raw;
    if(info == NULL || value == NULL || ReadAdc(id, &raw) != OK) {
        return ERR;
    }
    *value = (raw * info->adcform.reference_mv + 2047U) / 4095U;
    return OK;
}

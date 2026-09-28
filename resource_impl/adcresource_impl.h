/***************************************************************************/ /**
 *   @file   adcresource_impl.h
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
#ifndef ADCRESOURCE_IMPL_H
#define ADCRESOURCE_IMPL_H

#include "basedef.h"

typedef enum {
    MCU_AD_SAMPLE_GROUP,
    RSSI_AD_SAMPLE_GROUP
}AdcGroup_t;

typedef struct {
    BYTE  chipid;
    BYTE channel;
    BYTE adgroup;
    WORD     rsv;
}AdcForm_t;

typedef struct {
    DWORD (*AdcRead)(BYTE id, DWORD *value);
    DWORD (*AdcInit)(BYTE id, BYTE rsttype);
}AdcOps_t;

typedef struct {
    AdcForm_t adcform;
    AdcOps_t adcops;
}AdcInfo_t;

typedef enum {
    ADC_ID_RSSI01_HIGH,
    ADC_ID_RSSI23_HIGH,
    ADC_ID_CURRENT_MAIN,
    ADC_ID_CURRENT_DSP,
    ADC_ID_MODULE_TEMP,
    ADC_ID_VCC_MON,
    ADC_ID_V5_VPD_MON,
    ADC_ID_V3P3_RAIL_MON,
    ADC_ID_V3P3_PHY_MON,
    ADC_ID_V1P8_MON,
    ADC_ID_NUM
}AdcId_t;

typedef AdcId_t ADC_ID_ENUM;
#define ADC_ID_0 ADC_ID_RSSI01_HIGH

CONST AdcInfo_t *GetAdcInfo(BYTE id);

#endif

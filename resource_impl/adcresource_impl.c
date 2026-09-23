/***************************************************************************/ /**
 *   @file   adcresource_impl.c
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
#include "adcresource_impl.h"
#include "gd32e511_512.h"
#include "hr_time.h"

STATIC DWORD BoardAdcRead(BYTE id, DWORD *value);
STATIC DWORD BoardAdcInit(BYTE id, BYTE rsttype);

STATIC BOOL g_AdcReady, g_PinReady[ADC_ID_NUM];
/* GD32E512Mx DS Rev0.2 Table 2-3. Excel channel numbers were STM32 numbers. */
STATIC CONST AdcInfo_t g_AdcInfoImpl[ADC_ID_NUM] = {
    {{ADC_ID_RSSI01_HIGH, 2, 7, 12, GPIOA, GPIO_PIN_2, ADC0, 2500}, {BoardAdcRead, BoardAdcInit}},
    {{ADC_ID_RSSI23_HIGH, 3, 8, 12, GPIOA, GPIO_PIN_3, ADC0, 2500}, {BoardAdcRead, BoardAdcInit}},
    {{ADC_ID_CURRENT_MAIN, 21, 11, 12, GPIOA, GPIO_PIN_6, ADC0, 2500}, {BoardAdcRead, BoardAdcInit}},
    {{ADC_ID_CURRENT_DSP, 22, 12, 12, GPIOA, GPIO_PIN_7, ADC0, 2500}, {BoardAdcRead, BoardAdcInit}},
    {{ADC_ID_MODULE_TEMP, 10, 1, 12, GPIOC, GPIO_PIN_0, ADC0, 2500}, {BoardAdcRead, BoardAdcInit}},
    {{ADC_ID_VCC_MON, 11, 2, 12, GPIOC, GPIO_PIN_1, ADC0, 2500}, {BoardAdcRead, BoardAdcInit}},
    {{ADC_ID_V5_VPD_MON, 12, 3, 12, GPIOC, GPIO_PIN_2, ADC0, 2500}, {BoardAdcRead, BoardAdcInit}},
    {{ADC_ID_V3P3_RAIL_MON, 13, 4, 12, GPIOC, GPIO_PIN_3, ADC0, 2500}, {BoardAdcRead, BoardAdcInit}},
    {{ADC_ID_V3P3_PHY_MON, 23, 13, 12, GPIOC, GPIO_PIN_4, ADC0, 2500}, {BoardAdcRead, BoardAdcInit}},
    {{ADC_ID_V1P8_MON, 24, 14, 12, GPIOC, GPIO_PIN_5, ADC0, 2500}, {BoardAdcRead, BoardAdcInit}},
};

CONST AdcInfo_t *GetAdcInfo(BYTE id)
{
    return id < ADC_ID_NUM ? &g_AdcInfoImpl[id] : NULL;
}

STATIC DWORD WaitCalibration(DWORD mask)
{
    DWORD timeout = 10000;
    while((ADC_CTL1(ADC0) & mask) != 0) {
        if(timeout-- == 0) {
            adc_disable(ADC0);
            return ERR;
        }
        delay_us(1);
    }
    return OK;
}

STATIC DWORD BoardAdcInit(BYTE id, BYTE rsttype)
{
    CONST AdcInfo_t *info = GetAdcInfo(id);
    (void)rsttype;
    if(info == NULL) {
        return ERR;
    }
    rcu_periph_clock_enable(info->adcform.port == GPIOA ? RCU_GPIOA : RCU_GPIOC);
    gpio_mode_set(info->adcform.port, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, info->adcform.pin);
    if(!g_AdcReady) {
        rcu_periph_clock_enable(RCU_ADC0);
        rcu_adc_clock_config(ADC0, RCU_ADCCK_APB2_DIV8);
        adc_deinit(ADC0);
        adc_resolution_config(ADC0, ADC_RESOLUTION_12B);
        adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
        adc_special_function_config(ADC0, ADC_SCAN_MODE | ADC_CONTINUOUS_MODE, DISABLE);
        adc_channel_length_config(ADC0, ADC_ROUTINE_CHANNEL, 1);
        adc_external_trigger_source_config(ADC0, ADC_ROUTINE_CHANNEL, ADC_EXTTRIG_ROUTINE_NONE);
        adc_external_trigger_config(ADC0, ADC_ROUTINE_CHANNEL, ENABLE);
        adc_enable(ADC0);
        delay_us(100);
        ADC_CTL1(ADC0) |= ADC_CTL1_RSTCLB;
        if(WaitCalibration(ADC_CTL1_RSTCLB) != OK) {
            return ERR;
        }
        ADC_CTL1(ADC0) |= ADC_CTL1_CLB;
        if(WaitCalibration(ADC_CTL1_CLB) != OK) {
            return ERR;
        }
        g_AdcReady = TRUE;
    }
    g_PinReady[id] = TRUE;
    return OK;
}

STATIC DWORD BoardAdcRead(BYTE id, DWORD *value)
{
    CONST AdcInfo_t *info = GetAdcInfo(id);
    DWORD timeout = 10000;
    if(info == NULL || value == NULL || !g_AdcReady || !g_PinReady[id]) {
        return ERR;
    }
    adc_routine_channel_config(ADC0, 0, info->adcform.channel, ADC_SAMPLETIME_239POINT5);
    adc_flag_clear(ADC0, ADC_FLAG_EORC | ADC_FLAG_ROVF);
    adc_software_trigger_enable(ADC0, ADC_ROUTINE_CHANNEL);
    while(adc_flag_get(ADC0, ADC_FLAG_EORC) == RESET) {
        if(timeout-- == 0) {
            adc_disable(ADC0);
            g_AdcReady = FALSE;
            return ERR;
        }
        delay_us(1);
    }
    *value = adc_routine_data_read(ADC0);
    return OK;
}

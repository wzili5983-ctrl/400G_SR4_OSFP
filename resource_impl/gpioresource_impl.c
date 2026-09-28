/***************************************************************************/ /**
 *   @file   gpioresource_impl.c
 *   @brief  Implementation of gpio resource.
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
#include "gpioresource_impl.h"
#include "chipresource_impl.h"
#include "gd32e511_512.h"

STATIC DWORD BoardGpioWrite(BYTE id, DWORD value);
STATIC DWORD BoardGpioRead(BYTE id, DWORD *value);
STATIC DWORD BoardGpioInit(BYTE id, BYTE rsttype);

STATIC BOOL g_Ready[GPIO_ID_NUM];

STATIC CONST GpioInfo_t g_GpioInfoImpl[GPIO_ID_NUM] = {
    {{CHIP_ID_MCU, GPIO_ID_M_INT, GPIOB, GPIO_PIN_3, 1, 1, 0, 1}, {BoardGpioWrite, BoardGpioRead, BoardGpioInit}},
    {{CHIP_ID_MCU, GPIO_ID_LPMODE, GPIOC, GPIO_PIN_6, 0, 0, 1, 0}, {NULL, BoardGpioRead, BoardGpioInit}},
    {{CHIP_ID_MCU, GPIO_ID_RSSI_SEL_HIGH, GPIOA, GPIO_PIN_5, 1, 0, 1, 1}, {BoardGpioWrite, BoardGpioRead, BoardGpioInit}},
    {{CHIP_ID_MCU, GPIO_ID_V3P3_RAIL_EN, GPIOA, GPIO_PIN_9, 1, 0, 0, 1}, {BoardGpioWrite, BoardGpioRead, BoardGpioInit}},
    {{CHIP_ID_MCU, GPIO_ID_V3P3_PHY_EN, GPIOA, GPIO_PIN_10, 1, 0, 0, 1}, {BoardGpioWrite, BoardGpioRead, BoardGpioInit}},
    {{CHIP_ID_MCU, GPIO_ID_V1P8_EN, GPIOA, GPIO_PIN_11, 1, 0, 0, 1}, {BoardGpioWrite, BoardGpioRead, BoardGpioInit}},
    {{CHIP_ID_MCU, GPIO_ID_V1P8_PG, GPIOA, GPIO_PIN_12, 0, 0, 0, 1}, {NULL, BoardGpioRead, BoardGpioInit}},
    {{CHIP_ID_MCU, GPIO_ID_I2C_SEL_PHY, GPIOA, GPIO_PIN_15, 1, 0, 0, 1}, {BoardGpioWrite, BoardGpioRead, BoardGpioInit}},
    {{CHIP_ID_MCU, GPIO_ID_CDR_LD_EN, GPIOB, GPIO_PIN_11, 1, 0, 0, 1}, {BoardGpioWrite, BoardGpioRead, BoardGpioInit}},
    {{CHIP_ID_MCU, GPIO_ID_WDOG_RST, GPIOB, GPIO_PIN_12, 1, 0, 1, 0}, {BoardGpioWrite, BoardGpioRead, BoardGpioInit}},
    {{CHIP_ID_MCU, GPIO_ID_RX_RST_HIGH, GPIOC, GPIO_PIN_9, 1, 0, 1, 0}, {BoardGpioWrite, BoardGpioRead, BoardGpioInit}},
    {{CHIP_ID_MCU, GPIO_ID_RESETL, GPIOC, GPIO_PIN_10, 0, 0, 1, 0}, {NULL, BoardGpioRead, BoardGpioInit}},
    {{CHIP_ID_MCU, GPIO_ID_RX_TIA_LOS_HIGH, GPIOC, GPIO_PIN_12, 0, 0, 0, 1}, {NULL, BoardGpioRead, BoardGpioInit}},
    {{CHIP_ID_MCU, GPIO_ID_RESET_PHY, GPIOD, GPIO_PIN_4, 1, 0, 1, 0}, {BoardGpioWrite, BoardGpioRead, BoardGpioInit}},
    {{CHIP_ID_MCU, GPIO_ID_LPMODE_PHY, GPIOD, GPIO_PIN_6, 1, 0, 1, 0}, {BoardGpioWrite, BoardGpioRead, BoardGpioInit}},
    {{CHIP_ID_MCU, GPIO_ID_OPRXLOS_PHY_HIGH, GPIOD, GPIO_PIN_8, 1, 0, 1, 0}, {BoardGpioWrite, BoardGpioRead, BoardGpioInit}},
    {{CHIP_ID_MCU, GPIO_ID_LASI_PHY, GPIOE, GPIO_PIN_7, 0, 0, 0, 0}, {NULL, BoardGpioRead, BoardGpioInit}},
};

CONST GpioInfo_t *GetGpioInfo(BYTE id)
{
    return id < GPIO_ID_NUM ? &g_GpioInfoImpl[id] : NULL;
}

STATIC VOID EnablePort(DWORD port)
{
    switch(port) {
    case GPIOA:
        rcu_periph_clock_enable(RCU_GPIOA);
        break;
    case GPIOB:
        rcu_periph_clock_enable(RCU_GPIOB);
        break;
    case GPIOC:
        rcu_periph_clock_enable(RCU_GPIOC);
        break;
    case GPIOD:
        rcu_periph_clock_enable(RCU_GPIOD);
        break;
    case GPIOE:
        rcu_periph_clock_enable(RCU_GPIOE);
        break;
    default:
        break;
    }
}

STATIC VOID ConfigurePin(CONST GpioForm_t *form)
{
    DWORD port = form->port;
    DWORD pin = form->pin;
    EnablePort(port);
    if(form->output) {
        /* Preload output latch before enabling output to avoid glitches. */
        gpio_bit_write(port, pin, form->default_level ? SET : RESET);
        gpio_output_options_set(port, form->open_drain ? GPIO_OTYPE_OD : GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, pin);
    }
    /* Spreadsheet default input levels describe the external circuit, not pull resistors. */
    gpio_mode_set(port, form->output ? GPIO_MODE_OUTPUT : GPIO_MODE_INPUT, GPIO_PUPD_NONE, pin);
}

STATIC DWORD BoardGpioInit(BYTE id, BYTE rsttype)
{
    CONST GpioInfo_t *info = GetGpioInfo(id);
    (void)rsttype;
    if(info == NULL) {
        return ERR;
    }
    ConfigurePin(&info->gpioform);
    g_Ready[id] = TRUE;
    return OK;
}

STATIC DWORD BoardGpioWrite(BYTE id, DWORD value)
{
    CONST GpioInfo_t *info = GetGpioInfo(id);
    if(info == NULL || !g_Ready[id] || !info->gpioform.output || value > IO_HIGH) {
        return ERR;
    }
    gpio_bit_write(info->gpioform.port, info->gpioform.pin, value ? SET : RESET);
    return OK;
}

STATIC DWORD BoardGpioRead(BYTE id, DWORD *value)
{
    CONST GpioInfo_t *info = GetGpioInfo(id);
    if(info == NULL || value == NULL || !g_Ready[id]) {
        return ERR;
    }
    *value = gpio_input_bit_get(info->gpioform.port, info->gpioform.pin) == SET ? IO_HIGH : IO_LOW;
    return OK;
}

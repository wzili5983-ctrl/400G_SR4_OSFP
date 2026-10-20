/***************************************************************************/ /**
 *   @file   gpioresource_impl.h
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
#ifndef GPIORESOURCE_IMPL_H
#define GPIORESOURCE_IMPL_H

#include "basedef.h"

/* GpioForm_t 布尔成员取值——用宏代替裸 0/1，表驱动初始化处可读性更高 */
#define GPIO_DIR_INPUT          0U   /* output 成员：输入 */
#define GPIO_DIR_OUTPUT         1U   /* output 成员：输出 */

#define GPIO_PUSHPULL           0U   /* open_drain 成员：推挽输出 */
#define GPIO_OPENDRAIN          1U   /* open_drain 成员：开漏输出 */

#define GPIO_LEVEL_LOW          0U   /* default_level / active_level：低电平 */
#define GPIO_LEVEL_HIGH         1U   /* default_level / active_level：高电平 */

typedef struct {
    BYTE chipid; /* Owning chip of the pin (CHIP_ID_xxx); CHIP_ID_MCU for on-chip MCU pins. */
    BYTE id;
    DWORD port; /* One GPIO peripheral per resource. */
    DWORD pin;  /* Exactly one GPIO_PIN_x bit. */
    BYTE output, open_drain, default_level, active_level;
}GpioForm_t;

typedef struct {
    DWORD (*GpioWrite)(BYTE id, DWORD value);
    DWORD (*GpioRead)(BYTE id, DWORD *value);
    DWORD (*GpioInit)(BYTE id, BYTE rsttype);
}GpioOps_t;

typedef struct {
    GpioForm_t gpioform;
    GpioOps_t gpioops;
}GpioInfo_t;

typedef enum {
    GPIO_ID_M_INT,
    GPIO_ID_LPMODE,
    GPIO_ID_RSSI_SEL_HIGH,
    GPIO_ID_V3P3_RAIL_EN,
    GPIO_ID_V3P3_PHY_EN,
    GPIO_ID_V1P8_EN,
    GPIO_ID_V1P8_PG,
    GPIO_ID_I2C_SEL_PHY,
    GPIO_ID_CDR_LD_EN,
    GPIO_ID_WDOG_RST,
    GPIO_ID_RX_RST_HIGH,
    GPIO_ID_RESETL,
    GPIO_ID_RX_TIA_LOS_HIGH,
    GPIO_ID_RESET_PHY,
    GPIO_ID_LPMODE_PHY,
    GPIO_ID_OPRXLOS_PHY_HIGH,
    GPIO_ID_LASI_PHY,
    GPIO_ID_5V_EN,
    /* External SPI flash (U15, SST25PF040C 512KB SPI NOR, boots the DSP) chip
     * select net: PG12, active low. Kept Hi-Z (input) here on purpose — the
     * net is shared with the DSP SPI master (U11 boots from U15); the
     * extflash driver turns it into an output only for the duration of its
     * own bus sessions (2026-10-16). The bus-layer CS wrap in
     * busresource_impl is not used by that driver (NOR needs command+data
     * under one CS). */
    GPIO_ID_EXTFLASH_CS,
    GPIO_ID_NUM
}GpioId_t;

typedef GpioId_t GPIO_ID_ENUM;
#define GPIO_ID_0 GPIO_ID_M_INT

CONST GpioInfo_t *GetGpioInfo(BYTE id);

#endif

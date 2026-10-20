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
#include "bcm_gd32_i2c.h"
#include "drv.h"
#include "odsp_init.h"
#include "pmic.h"
#include "extflash.h"
#include "tia.h"

STATIC DWORD DspWrite(BYTE id, DWORD addr, DWORD data);
STATIC DWORD DspRead(BYTE id, DWORD addr, DWORD *data);
STATIC DWORD InitDsp(BYTE rsttype);

STATIC CONST ChipInfo_t g_ChipInfoImpl[CHIP_ID_NUM - CHIP_ID_START] = {
    {{CHIP_ID_DSP, BUS_ID_DSP}, {DspWrite, DspRead, InitDsp}},
    /* GN1848 (Semtech) laser driver: 8-bit registers, 7-bit address 0x53 on
     * BUS_ID_DRV, single-register read/write via drv.c. */
    {{CHIP_ID_DRV, BUS_ID_DRV}, {DrvWrite, DrvRead, InitDrv}},
    /* GN1816 (Semtech) TIA: 8-bit registers, 7-bit address 0x5B on
     * BUS_ID_TIA, single-register read/write via tia.c. */
    {{CHIP_ID_TIA, BUS_ID_TIA}, {TiaWrite, TiaRead, InitTia}},
    /* SY70300B (Silergy) quad buck PMIC: 8-bit registers, 7-bit address 0x1E
     * (internal MTP default) on BUS_ID_PMIC, single-register read/write via
     * pmic.c; rails come up by OTP config after the global EN pin, so the
     * driver only verifies liveness here. EN <- GPIO_ID_CDR_LD_EN (PB11),
     * WDG_RST <- GPIO_ID_WDOG_RST (PB12); INT pin unconnected. */
    {{CHIP_ID_PMIC, BUS_ID_PMIC}, {PmicWrite, PmicRead, InitPmic}},
    /* U15 (SST25PF040C, 512KB external SPI NOR flash): boots the DSP on
     * BUS_ID_EXTFLASH (SPI2, CS=PG12). The DSP SPI master shares the bus;
     * 32-bit word R/W plus bulk program/erase API in driver/extflash.
     * Registered after the DSP in the InitChip chain — a live DSP already
     * proves U15 delivered a boot image, so this init never faults the
     * module (RDID is informational, power-on block protection is cleared
     * best-effort). Also host-reachable through the D9H mailbox CHIP
     * record (CHIPID=n). */
    {{CHIP_ID_EXTFLASH, BUS_ID_EXTFLASH}, {ExtFlashWrite, ExtFlashRead, InitExtFlash}}
};

CONST ChipInfo_t *GetChipInfo(VOID)
{
    return g_ChipInfoImpl;
}

CONST ChipInfo_t *GetChipInfoById(BYTE id)
{
    return id >= CHIP_ID_START && id < CHIP_ID_NUM ? &g_ChipInfoImpl[id - CHIP_ID_START] : NULL;
}

STATIC DWORD InitDsp(BYTE rsttype)
{
    (void)rsttype;
    /* The DSP memory path (DspWrite/DspRead via BcmArm*) runs on I2C0 through
     * the bcm driver; initialize it here instead of the bus resource layer. */
    BcmGd32I2cInit();
    /* BCM87420 上电初始化：等内部轨稳定 + BSC liveness 读 + datapath 配置
     * 序列（★配置序列占位，待 Broadcom bring-up 文档）。返回 ERR 时
     * InitChip 上抛 → msm_pwr_seq 判 fault 38。 */
    return OdspInit();
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

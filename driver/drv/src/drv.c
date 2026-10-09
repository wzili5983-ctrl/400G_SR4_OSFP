/** @file drv.c
 * @brief GN1848 (DRV) laser driver implementation.
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 */
#include "drv.h"
#include "chipresource.h"
#include "busresource.h"
#include "adcresource_impl.h"
#include "timer2_scheduler.h"

/* 8-bit register space; 16bit 读要求 addr+1 可寻址。 */
#define DRV_ADDR_MAX 0xFFU

/* IBIAS SRC-0 寄存器表（ch0-3，步进 2，2B/通道，DS Rev7 Table 4-1）。 */
STATIC CONST BYTE s_DrvIbiasReg[4] = {
    DRV_REG_ADC_CH0_SRC0, DRV_REG_ADC_CH1_SRC0, DRV_REG_ADC_CH2_SRC0, DRV_REG_ADC_CH3_SRC0
};

STATIC VOID TaskTxAdSamples(VOID *arg);

DWORD InitDrv(BYTE rsttype)
{
    CONST ChipInfo_t *info = GetChipInfoById(CHIP_ID_DRV);
    DWORD id0;
    (void)rsttype;
    if(info == NULL || InitBus(info->chipform.busid) != OK) {
        return ERR;
    }
    /* Liveness check: ID0 must read back the device ID (0x30)。
     * DrvRead 为 16bit 单次事务读，ID0 落在高字节。 */
    if(DrvRead(CHIP_ID_DRV, DRV_REG_ID0, &id0) != OK || ((id0 >> 8) & 0xFFU) != DRV_ID0_VALUE) {
        return ERR;
    }
    /* 监视 ADC 配置（DS §3.3）：使能 + 12bit；Variable-1 选 Temperature，
     * 允许 IBIAS 转换。ID0 已确认总线/器件存活，配置失败即真错误。 */
    if(DrvWrite(CHIP_ID_DRV, DRV_REG_ADC_CTRL0, DRV_ADC_CTRL0_EN_12BIT) != OK) {
        return ERR;
    }
    if(DrvWrite(CHIP_ID_DRV, DRV_REG_ADC_CTRL1, DRV_ADC_CTRL1_TEMP_IBIAS) != OK) {
        return ERR;
    }
    /* 监视采集任务：延时 100ms、间隔 1s（假定调度器先于 DRV 初始化）。
     * 槽位存调度器集中表 g_TaskIds[TASK_ID_TX_AD_SAMPLES]。 */
    g_TaskIds[TASK_ID_TX_AD_SAMPLES] = Timer2SchedulerAdd(TaskTxAdSamples, NULL, 100U, 1000U, TIMER2_TASK_FOREVER);
    if(g_TaskIds[TASK_ID_TX_AD_SAMPLES] < 0) {
        return ERR;
    }
    return OK;
}

DWORD DrvWrite(BYTE id, DWORD addr, DWORD data)
{
    CONST ChipInfo_t *info = GetChipInfoById(id);
    BYTE tx[2];
    if(info == NULL || addr > DRV_ADDR_MAX) {
        return ERR;
    }
    tx[0] = (BYTE)addr;
    tx[1] = (BYTE)data;
    /* The chip id selects the ChipInfo_t entry; its busid selects the bus. */
    return WriteBus(info->chipform.busid, tx, (BYTE)sizeof(tx));
}

DWORD DrvRead(BYTE id, DWORD addr, DWORD *data)
{
    CONST ChipInfo_t *info = GetChipInfoById(id);
    BYTE reg, buf[2];
    if(info == NULL || addr > (DWORD)DRV_ADDR_MAX - 1U || data == NULL) {
        return ERR;
    }
    /* Random read per DS 3.5: the dummy write loads the address pointer and
     * stays valid until the next access or RSTB. Single-transaction 2-byte
     * read per DS 3.5.2: the pointer auto-increments, so one READ delivers
     * [addr]=MSB then [addr+1]=LSB; two separate 1-byte reads could mix
     * bytes from different ADC conversions while the bus is idle between
     * them. 8-bit registers: caller takes the high byte. */
    reg = (BYTE)addr;
    if(WriteBus(info->chipform.busid, &reg, 1U) != OK) {
        return ERR;
    }
    if(ReadBus(info->chipform.busid, buf, (BYTE)sizeof(buf)) != OK) {
        return ERR;
    }
    *data = ((DWORD)buf[0] << 8) | (DWORD)buf[1];
    return OK;
}

/* Collect GN1848 (DRV) monitor ADCs; registered in InitDrv, period 1000 ms.
 *   SRC-0 0x20-0x26 = 每通道 VCSEL Bias Current (IBIAS)
 *     → ADC_MSA_ID_LASER_BIAS0..3，并同值写入 ADC_MSA_ID_PWR_TX1..4
 *       （线性驱动无 Tx 光功率检测，IBIAS 作 TxPwr 代理——产品约定★）；
 *   SRC-2 0x30 (Variable-1 = Temperature) → ADC_MSA_ID_LASER_TEMP
 *     （芯片单温度传感器，ch0 读数即全局温度）。
 * DS §3.3 换算（归 MAL 层 cal，此处存 RAW 12bit）：
 *   T = ADC×222.8/2^16 − 66.8 [°C]；IBIAS = ADC/2^16×22.19 + 0.16 [mA]（±15%）。
 * 数字量不做中值滤波；任一项读失败仅跳过该项，槽保持上次值。 */
STATIC VOID TaskTxAdSamples(VOID *arg)
{
    DWORD raw;
    BYTE ch;
    (void)arg;
    for(ch = 0U; ch < 4U; ++ch) {
        if(DrvRead(CHIP_ID_DRV, s_DrvIbiasReg[ch], &raw) == OK) {
            (VOID)SetAdcMsaFilteredValue((BYTE)(ADC_MSA_ID_LASER_BIAS0 + ch), (WORD)raw);
            (VOID)SetAdcMsaFilteredValue((BYTE)(ADC_MSA_ID_PWR_TX1 + ch), (WORD)raw);
        }
    }
    if(DrvRead(CHIP_ID_DRV, DRV_REG_ADC_CH0_SRC2, &raw) == OK) {
        (VOID)SetAdcMsaFilteredValue((BYTE)ADC_MSA_ID_LASER_TEMP, (WORD)raw);
    }
}

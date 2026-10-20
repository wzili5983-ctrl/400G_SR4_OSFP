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
#include "adpat.h"              /* ReadSramProtocolMsaSpace：B4H 影子 */
#include "ral.h"                /* PAGE_ID_B4H / PAGE_BXH_* 布局宏 */

/* 8-bit register space; 16bit 读要求 addr+1 可寻址。 */
#define DRV_ADDR_MAX 0xFFU

/* IBIAS_DAC 寄存器表（ch0-3，步进 0x10，2B/通道，DS Rev7 Table 4-1）。
 * LASER_BIAS 槽代理源：固件写入的偏置电流 DAC 码（CHx_CTRL0 9:0）。 */
STATIC CONST BYTE s_DrvBiasDacReg[4] = {
    DRV_REG_CH0_CTRL0, DRV_REG_CH1_CTRL0, DRV_REG_CH2_CTRL0, DRV_REG_CH3_CTRL0
};
/* IBIAS SRC-0 寄存器表（ch0-3，步进 2，2B/通道）。
 * PWR_TX 槽代理源：片上 ADC 实测 IBIAS（线性驱动无 Tx PD——产品约定★）。 */
STATIC CONST BYTE s_DrvIbiasAdcReg[4] = {
    DRV_REG_ADC_CH0_SRC0, DRV_REG_ADC_CH1_SRC0, DRV_REG_ADC_CH2_SRC0, DRV_REG_ADC_CH3_SRC0
};

STATIC VOID TaskTxAdSamples(VOID *arg);

/* 监视采集任务只注册一次：PwrDn 后 DRV 掉电、任务空转（DrvRead 无 ACK
 * 逐项跳过），重新 PwrUp 后自动恢复采样——避免每次 PwrUp 重复注册耗尽
 * 任务槽。 */
STATIC BYTE s_TxAdTaskAdded = 0U;

/* B4H/B5H 寄存器初始化页消费（0x00=REGNUM u8 + N×{ADDR u16BE@0x04+4n,
 * VALUE u16BE@0x06+4n}，布局见 ral.h PAGE_BXH_*）。ADDR 高字节=写宽度
 * （0x00=8bit/0x01=16bit，DrvWrite 内分派）。B4H=全局、B5H=lane 级。 */
STATIC DWORD DrvApplyRegInitPage(TabIndex_t pageid)
{
    BYTE page[128U];            /* 页大小 128B */
    BYTE regnum;
    BYTE n;

    /* 整页快照可能被从机 ISR 的并发 host 写撕裂（memory_map.md R5「整页写
     * 窗口」）。包影子锁使 ISR 不得在拷贝期内写影子，快照原子一致
     * （方案 A：AdpatShadowLock/Unlock，取代原双读比对）。 */
    AdpatShadowLock();
    if(ReadSramProtocolMsaSpace(pageid, 0U, page, (BYTE)sizeof(page)) != OK) {
        AdpatShadowUnlock();
        return ERR;
    }
    AdpatShadowUnlock();

    regnum = page[PAGE_BXH_ADDR_REGNUM];             /* DRVREGNUM u8 */
    if(regnum == 0U) { return OK; }                 /* 空表 */
    if(regnum > PAGE_BXH_REGINIT_NUM_MAX) { return ERR; }  /* 防呆 */

    for(n = 0U; n < regnum; ++n) {
        DWORD addr = ((DWORD)page[PAGE_BXH_ADDR_REGADDR(n)] << 8) |
                     (DWORD)page[PAGE_BXH_ADDR_REGADDR(n) + 1U];
        DWORD value = ((DWORD)page[PAGE_BXH_ADDR_REGVALUE(n)] << 8) |
                      (DWORD)page[PAGE_BXH_ADDR_REGVALUE(n) + 1U];
        if(DrvWrite(CHIP_ID_DRV, addr, value) != OK) {  /* addr 高字节=宽度，DrvWrite 内分派 */
            return ERR;         /* 配置写失败 = 芯片不应答，交 InitChip → fault 38 */
        }
    }
    return OK;
}

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
    /* 寄存器初始化页消费（表驱动，取代原硬编码 ADC 配置）：B4H=全局
     * （含 ADC_CTRL0/1 监视使能+分辨率+源选择）、B5H=lane 级（IBIAS_DAC 等）。
     * ID0 已确认总线/器件存活，配置失败即真错误（写不进 = 不应答）。 */
    if(DrvApplyRegInitPage(PAGE_ID_B4H) != OK) {
        return ERR;
    }
    if(DrvApplyRegInitPage(PAGE_ID_B5H) != OK) {
        return ERR;
    }
    /* 监视采集任务：延时 100ms、间隔 1s（假定调度器先于 DRV 初始化）。
     * 槽位存调度器集中表 g_TaskIds[TASK_ID_TX_AD_SAMPLES]；只注册一次
     * （见 s_TxAdTaskAdded 注释）。 */
    if(s_TxAdTaskAdded == 0U) {
        g_TaskIds[TASK_ID_TX_AD_SAMPLES] = Timer2SchedulerAdd(TaskTxAdSamples, NULL, 100U, 1000U, TIMER2_TASK_FOREVER);
        if(g_TaskIds[TASK_ID_TX_AD_SAMPLES] < 0) {
            return ERR;
        }
        s_TxAdTaskAdded = 1U;
    }
    return OK;
}

DWORD DrvWrite(BYTE id, DWORD addr, DWORD data)
{
    CONST ChipInfo_t *info = GetChipInfoById(id);
    BYTE width = (BYTE)((addr >> 8) & 0xFFU);   /* addr bit8-15 = 写宽度标志 */
    BYTE reg   = (BYTE)(addr & 0xFFU);          /* addr bit7-0  = 寄存器地址 */
    BYTE tx[3];
    BYTE len;

    if(info == NULL) {
        return ERR;
    }
    if(width == DRV_WRITE_WIDTH_8BIT) {                 /* 8bit 写：tx=2 */
        if(reg > DRV_ADDR_MAX) {
            return ERR;
        }
        tx[0] = reg;
        tx[1] = (BYTE)data;
        len   = 2U;
    } else if(width == DRV_WRITE_WIDTH_16BIT) {         /* 16bit 单事务写：tx=3，MSB-first */
        if(reg > (DWORD)DRV_ADDR_MAX - 1U) {            /* 需 reg+1 可寻址 */
            return ERR;
        }
        tx[0] = reg;
        tx[1] = (BYTE)(data >> 8);
        tx[2] = (BYTE)data;
        len   = 3U;
    } else {
        return ERR;                                     /* 未知宽度 */
    }
    /* The chip id selects the ChipInfo_t entry; its busid selects the bus. */
    return WriteBus(info->chipform.busid, tx, len);
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
 *   CHx_CTRL0 0x40-0x70 = 每通道 IBIAS_DAC 偏置电流设定位（10bit）
 *     → MON_MSA_ID_LASER_BIAS0..3（固件指令值，LASER_BIAS 槽代理源）；
 *   ADC_CHx_SRC0 0x20-0x26 = 每通道 VCSEL Bias Current 实测 ADC（16bit）
 *     → MON_MSA_ID_PWR_TX1..4（线性驱动无 Tx 光功率检测，IBIAS ADC 作
 *       TxPwr 代理——产品约定★）；
 *   ADC_CH0_SRC2 0x30 (Variable-1 = Temperature) → MON_MSA_ID_LASER_TEMP
 *     （芯片单温度传感器，ch0 读数即全局温度）。
 * DS §3.3 换算（归 MAL 层 cal，此处存 RAW）：
 *   T = ADC×222.8/2^16 − 66.8 [°C]；
 *   PWR_TX(IBIAS ADC) = ADC/2^16×22.19 + 0.16 [mA]（±15%）；
 *   LASER_BIAS(IBIAS_DAC) 反推 Equation 3-2：mA = DecimalCode/69.0 + 1.0。
 * 数字量不做中值滤波；任一项读失败仅跳过该项，槽保持上次值。 */
STATIC VOID TaskTxAdSamples(VOID *arg)
{
    DWORD raw;
    BYTE ch;
    (void)arg;
    for(ch = 0U; ch < 4U; ++ch) {
        if(DrvRead(CHIP_ID_DRV, s_DrvBiasDacReg[ch], &raw) == OK) {
            (VOID)SetAdcMsaFilteredValue((BYTE)(MON_MSA_ID_LASER_BIAS0 + ch), (WORD)raw);
        }
        if(DrvRead(CHIP_ID_DRV, s_DrvIbiasAdcReg[ch], &raw) == OK) {
            (VOID)SetAdcMsaFilteredValue((BYTE)(MON_MSA_ID_PWR_TX1 + ch), (WORD)raw);
        }
    }
    if(DrvRead(CHIP_ID_DRV, DRV_REG_ADC_CH0_SRC2, &raw) == OK) {
        (VOID)SetAdcMsaFilteredValue((BYTE)MON_MSA_ID_LASER_TEMP, (WORD)raw);
    }
}

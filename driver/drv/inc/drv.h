/** @file drv.h
 * @brief GN1848 (DRV) laser driver API, I2C register access.
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * GN1848 Final Data Sheet Rev7 (Semtech PDS-062679):
 * I2C slave, 8-bit register address, 8-bit data, address pointer
 * auto-increments and stays valid between transactions until RSTB.
 * Default 8-bit device address 0xA6 -> 7-bit 0x53 on BUS_ID_DRV.
 *
 * 读路径统一为 16bit 单次事务读（DS §3.5.2：多字节寄存器——尤其 ADC
 * 值——必须单次事务连续读，分两次单字节读会混入新转换值）：
 * DrvRead 发起 dummy-write 定址后一条 READ 连续收 [addr]=MSB、[addr+1]
 * =LSB，*data = 16bit（高字节 = 低地址）；访问 8bit 寄存器的调用者自取
 * 高 8 位。监视采集（TaskTxAdSamples：SRC-0 IBIAS → ADC_MSA_ID_LASER_
 * BIAS0..3 / ADC_MSA_ID_PWR_TX1..4，SRC-2 Temperature → ADC_MSA_ID_
 * LASER_TEMP，1s 周期）在 InitDrv 中配置（ADC_CTRL0/1）并注册调度任务。
 */
#ifndef DRV_H
#define DRV_H

#include "basedef.h"

/* Register addresses, GN1848 DS Rev7 Table 4-1. */
#define DRV_REG_ID0               0x00U
#define DRV_REG_ID1               0x01U
#define DRV_REG_GCTRL8            0x0DU
#define DRV_ID0_VALUE             0x30U
#define DRV_GCTRL8_SOFT_RESET     0x01U

/* ADC monitor registers（DS Rev7 §3.3 / Table 4-1）：
 * SRC-0 = 每通道 VCSEL Bias Current(IBIAS)，15:0 RO，低地址为 MSB；
 * SRC-2 = Variable-1（由 ADC_CTRL1 4:2 选源：001=Temperature），
 * 芯片仅一个集成温度传感器，温度读 ch0 即可。 */
#define DRV_REG_ADC_CTRL0         0x17U   /* bit0=ADC_EN；bits2:1=分辨率(01=9b,10=10b,11=12b) */
#define DRV_REG_ADC_CTRL1         0x18U   /* bits4:2=Variable-1 源(001=Temp,010=VCC,100=VCC5)；bit0=1 允许 IBIAS 转换 */
#define DRV_REG_ADC_CH0_SRC0      0x20U   /* IBIAS ch0 */
#define DRV_REG_ADC_CH1_SRC0      0x22U
#define DRV_REG_ADC_CH2_SRC0      0x24U
#define DRV_REG_ADC_CH3_SRC0      0x26U
#define DRV_REG_ADC_CH0_SRC2      0x30U   /* Variable-1 ch0（Temperature 时全局有效） */
/* ADC 配置值：ADC_CTRL0 = 使能 + 12bit；ADC_CTRL1 = Variable-1 选温度 + 允许 IBIAS。 */
#define DRV_ADC_CTRL0_EN_12BIT    0x07U
#define DRV_ADC_CTRL1_TEMP_IBIAS  0x05U

DWORD InitDrv(BYTE rsttype);
DWORD DrvWrite(BYTE id, DWORD addr, DWORD data);
/* 16bit 单次事务读：*data 高字节 = [addr]（MSB first），低字节 = [addr+1]。
 * 访问 8bit 寄存器时调用者取 *data 高 8 位（(data >> 8) & 0xFF）。 */
DWORD DrvRead(BYTE id, DWORD addr, DWORD *data);

#endif

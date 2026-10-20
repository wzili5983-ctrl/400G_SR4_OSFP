/** @file pmic.h
 * @brief SY70300B (PMIC) register API, I2C single-register access.
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * SY70300B Data Sheet Rev.1.0 (Silergy, DS_SY70300B "for iSmart Group"):
 * quad-output synchronous buck regulator, 5A per phase, VOUT 0.45-2.0V
 * I2C programmable. I2C slave up to 3.4Mbps (Fast-mode 400kHz on this
 * board), 8-bit register address, 8-bit data. Default 7-bit slave address
 * 0x1E is set by internal MTP. Global EN pin powers up the device and OTP
 * config loads in ~1.4ms; WDG_RST falling edge resets BUCKx_DVS0CFG1 and
 * BUCKx_EN_DVS0 to OTP default; INT is an open-drain fault output.
 *
 * Board wiring (2026-10-12 user decision): EN is driven by
 * GPIO_ID_CDR_LD_EN (PB11, active high, PUPD none, default LOW), WDG_RST by
 * GPIO_ID_WDOG_RST (PB12, active low, default HIGH idle); the INT pin is
 * left unconnected, so PMIC faults are only visible via I2C fault-record
 * registers (FLT_RECORD*), not through an MCU interrupt line.
 */
#ifndef PMIC_H
#define PMIC_H

#include "basedef.h"

/* Register addresses, DS Rev1.0 "Register Address Map". */
#define PMIC_REG_IO_CHIPNAME        0x01U   /* R   manufacturer ID (OTP, DS default 0xFF) */
#define PMIC_REG_IO_CHIPVERSION     0x02U   /* R   version number (OTP, DS default 0x00) */
#define PMIC_REG_IO_SOFTRESET       0x0FU   /* R/W bit0 SOFTRESET: 1=reset all regs + reload OTP, self-clear */

/* Fault record registers, read to clear (COR). */
#define PMIC_REG_FLT_RECORDTEMP     0x13U   /* bit7 VIN OVP, bit2 die OT (109C), bit1 OT shutdown (150C) */
#define PMIC_REG_FLT_RECORDBUCK1    0x14U   /* bit7 PG, bit6 OC, bit5 OV, bit4 UV */
#define PMIC_REG_FLT_RECORDBUCK3    0x15U
#define PMIC_REG_FLT_RECORDBUCK2    0x16U
#define PMIC_REG_FLT_RECORDBUCK4    0x17U

#define PMIC_REG_IO_I2CCFG          0x22U
#define PMIC_REG_IO_RSTDVS          0x25U   /* watchdog reset debounce / disable */

#define PMIC_REG_FLT_OT_CTRL        0x30U
#define PMIC_REG_FLT_MASKTEMP       0x32U
#define PMIC_REG_FLT_MASKBUCK1      0x33U
#define PMIC_REG_FLT_MASKBUCK3      0x34U
#define PMIC_REG_FLT_MASKBUCK2      0x35U
#define PMIC_REG_FLT_MASKBUCK4      0x36U
#define PMIC_REG_FLT_BUCK1_CTRL     0x37U
#define PMIC_REG_FLT_BUCK3_CTRL     0x38U
#define PMIC_REG_FLT_BUCK2_CTRL     0x39U
#define PMIC_REG_FLT_BUCK4_CTRL     0x3AU

/* Per-buck converter registers; bit fields per DS "Register Description" (★). */
#define PMIC_REG_BUCK1_RAMP         0x3EU
#define PMIC_REG_BUCK1_CFG0         0x42U   /* incl. VOUT_DISCHARGE_EN */
#define PMIC_REG_BUCK1_DVS0CFG1     0x48U   /* output voltage setting; EN bit (BUCK1_EN_DVS0) in this block */
#define PMIC_REG_BUCK1_DVS0CFG0     0x49U   /* forced PWM / auto PFM-PWM */
#define PMIC_REG_BUCK1_RSPCFG       0x54U
#define PMIC_REG_BUCK1_SLEWCTRL     0x55U   /* soft-start / DVS slew rate */

#define PMIC_REG_BUCK3_RAMP         0x5BU
#define PMIC_REG_BUCK3_CFG0         0x5FU
#define PMIC_REG_BUCK3_DVS0CFG1     0x62U
#define PMIC_REG_BUCK3_DVS0CFG0     0x63U
#define PMIC_REG_BUCK3_RSPCFG       0x6EU
#define PMIC_REG_BUCK3_SLEWCTRL     0x6FU

#define PMIC_REG_BUCK2_RAMP         0x75U
#define PMIC_REG_BUCK2_CFG0         0x79U
#define PMIC_REG_BUCK2_DVS0CFG1     0x7CU
#define PMIC_REG_BUCK2_DVS0CFG0     0x7DU
#define PMIC_REG_BUCK2_RSPCFG       0x88U
#define PMIC_REG_BUCK2_SLEWCTRL     0x89U

#define PMIC_REG_BUCK4_RAMP         0x8FU
#define PMIC_REG_BUCK4_CFG0         0x93U
#define PMIC_REG_BUCK4_DVS0CFG1     0x96U
#define PMIC_REG_BUCK4_DVS0CFG0     0x97U
#define PMIC_REG_BUCK4_RSPCFG       0xA2U
#define PMIC_REG_BUCK4_SLEWCTRL     0xA3U

DWORD InitPmic(BYTE rsttype);
DWORD PmicWrite(BYTE id, DWORD addr, DWORD data);
DWORD PmicRead(BYTE id, DWORD addr, DWORD *data);

#endif

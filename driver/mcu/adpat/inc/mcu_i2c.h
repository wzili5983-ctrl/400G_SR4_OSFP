/** @file mcu_i2c.h
 * @brief MCU I2C master: hardware I2Cx via firmware library and software bit-bang.
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * Hardware master: I2C0/I2C1/I2C2 selected by i2cno (AF4 open drain, I2C
 * kernel clocked from IRC8M = 8 MHz), bus rate per I2cSpeed_t:
 *   I2C_SPEED_STD  = 100 kHz  (PSC=0, SCLL/SCLH = 39/39 -> 8M/80)
 *   I2C_SPEED_FAST = 400 kHz  (PSC=0, SCLL/SCLH = 11/6  -> ~421 k,
 *                              tLOW=1.375us / tHIGH=0.75us, Fast-mode spec)
 *   I2C_SPEED_HIGH = 1 MHz    (PSC=0, SCLL/SCLH = 4/3   -> ~889 k,
 *                              Fm+ tLOW/tHIGH met, nearest to 1 M)
 * Software master: any GPIO port/pins, open drain bit-bang. The speed class
 * maps to the half-period delay: STD -> 5us (~100 k real), FAST/HIGH -> 1us
 * (~300 k real) — the 1us SysTick granularity makes ~300 kHz the bit-bang
 * ceiling, so FAST and HIGH run at the same real rate. All callers must
 * serialize bus access.
 */
#ifndef MCU_I2C_H
#define MCU_I2C_H

#include "basedef.h"

/* Hardware I2Cx instance IDs — also the index of the per-instance pin table
 * g_I2cHwInfo in mcu_i2c.c. I2C_2 is the master bus to the DSP (PB6/PB7);
 * I2C_0 (PB8/PB9) shares its pins with the TIA/DRV software bit-bang bus
 * and has no BUS_HW consumer; I2C_1 is the PA0/PA1 instance default while
 * the host slave link physically runs on PB13/PB14, configured by
 * protocol/mci i2c_slave.c itself (that driver owns its own pins). */
typedef enum {
    I2C_0 = 0,
    I2C_1,
    I2C_2,
    I2C_NUM
}I2cNum_t;

/* Bus rate classes for both hardware and software masters (real software
 * rates are bit-bang-limited, see file header). */
typedef enum {
    I2C_SPEED_STD = 0,   /* 100 kHz */
    I2C_SPEED_FAST,      /* 400 kHz */
    I2C_SPEED_HIGH,      /* 1 MHz */
    I2C_SPEED_NUM
}I2cSpeed_t;

/* Hardware master: pin mux per instance from g_I2cHwInfo; timing per speed
 * class from the timing table in mcu_i2c.c. */
VOID I2cBusInit(BYTE i2cno, I2cSpeed_t speed);
DWORD I2cBusWrite(BYTE i2cno, BYTE addr7, CONST BYTE *tx, BYTE txlen);
DWORD I2cBusRead(BYTE i2cno, BYTE addr7, BYTE *rx, BYTE rxlen);

DWORD I2cBusSoftwareInit(DWORD port, DWORD scl, DWORD sda, I2cSpeed_t speed);
DWORD I2cBusSoftwareWrite(DWORD port, DWORD scl, DWORD sda, I2cSpeed_t speed, BYTE addr7, CONST BYTE *tx, BYTE txlen);
DWORD I2cBusSoftwareRead(DWORD port, DWORD scl, DWORD sda, I2cSpeed_t speed, BYTE addr7, BYTE *rx, BYTE rxlen);

#endif

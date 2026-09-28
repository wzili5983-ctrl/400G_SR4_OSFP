/** @file mcu_i2c.h
 * @brief MCU I2C master: hardware I2Cx via firmware library and software bit-bang.
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * Hardware master: I2C0/I2C1/I2C2 selected by i2cno, AF4 open drain,
 * IRC8M clock, conservative timing below 100 kHz. Software master: any
 * GPIO port/pins, open drain bit-bang. All callers must serialize bus access.
 */
#ifndef MCU_I2C_H
#define MCU_I2C_H

#include "basedef.h"

VOID I2cBusInit(BYTE i2cno, DWORD port, DWORD scl, DWORD sda);
DWORD I2cBusWrite(BYTE i2cno, BYTE addr7, CONST BYTE *tx, BYTE txlen);
DWORD I2cBusRead(BYTE i2cno, BYTE addr7, BYTE *rx, BYTE rxlen);

DWORD I2cBusSoftwareInit(DWORD port, DWORD scl, DWORD sda);
DWORD I2cBusSoftwareWrite(DWORD port, DWORD scl, DWORD sda, BYTE addr7, CONST BYTE *tx, BYTE txlen);
DWORD I2cBusSoftwareRead(DWORD port, DWORD scl, DWORD sda, BYTE addr7, BYTE *rx, BYTE rxlen);

#endif

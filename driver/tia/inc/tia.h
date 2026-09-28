/** @file tia.h
 * @brief GN1816 (TIA) register API, I2C single-register access.
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * GN1816 Final Data Sheet Rev6 (Semtech PDS-062663):
 * I2C slave, 8-bit register address, 8-bit data, address pointer
 * auto-increments and stays valid between transactions until RSTB.
 * Default 8-bit device address 0xB6 -> 7-bit 0x5B on BUS_ID_TIA.
 */
#ifndef TIA_H
#define TIA_H

#include "basedef.h"

/* Register addresses, GN1816 DS Rev6 Table 5-1. */
#define TIA_REG_DEVICE_ID1    0x00U
#define TIA_REG_DEVICE_ID2    0x01U
#define TIA_ID1_VALUE         0x19U

DWORD InitTia(BYTE rsttype);
DWORD TiaWrite(BYTE id, DWORD addr, DWORD data);
DWORD TiaRead(BYTE id, DWORD addr, DWORD *data);

#endif

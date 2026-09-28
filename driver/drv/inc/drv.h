/** @file drv.h
 * @brief GN1848 (DRV) laser driver API, I2C single-register access.
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * GN1848 Final Data Sheet Rev7 (Semtech PDS-062679):
 * I2C slave, 8-bit register address, 8-bit data, address pointer
 * auto-increments and stays valid between transactions until RSTB.
 * Default 8-bit device address 0xA6 -> 7-bit 0x53 on BUS_ID_DRV.
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

DWORD InitDrv(BYTE rsttype);
DWORD DrvWrite(BYTE id, DWORD addr, DWORD data);
DWORD DrvRead(BYTE id, DWORD addr, DWORD *data);

#endif

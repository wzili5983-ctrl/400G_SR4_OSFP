/** @file busresource_impl.h
 * @brief Board resource implementation.
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 */
#ifndef BUSRESOURCE_IMPL_H
#define BUSRESOURCE_IMPL_H

#include "basedef.h"

typedef enum {
    BUS_I2C_MASTER,
    BUS_I2C_SLAVE,
    BUS_DSP_SPI
}BusMode_t;

typedef struct {
    BYTE id;
    BusMode_t mode;
    DWORD port, scl, sda;
    BOOL software;
}BusForm_t;

typedef struct {
    DWORD (*BusInit)(BYTE id);
    DWORD (*BusTransfer)(BYTE id, BYTE address7, CONST BYTE *tx, BYTE txlen, BYTE *rx, BYTE rxlen);
}BusOps_t;

typedef struct {
    BusForm_t busform;
    BusOps_t busops;
}BusInfo_t;

typedef enum {
    BUS_ID_PHY,
    BUS_ID_INNER,
    BUS_ID_HOST,
    BUS_ID_NUM
}BusId_t;

CONST BusInfo_t *GetBusInfo(BYTE id);

#endif

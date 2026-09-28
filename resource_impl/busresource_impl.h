/** @file busresource_impl.h
 * @brief Board resource implementation.
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 */
#ifndef BUSRESOURCE_IMPL_H
#define BUSRESOURCE_IMPL_H

#include "basedef.h"

typedef union {
    BYTE i2caddr;
    BYTE spicsio;
}ChipAddr_t;

typedef struct {
    BYTE i2cno;
    ChipAddr_t i2caddr;
    DWORD port, scl, sda;
}I2cForm_t;

typedef struct 
{
    BYTE spino;
    ChipAddr_t  spicsid;
    DWORD port, sck , mosi, miso;
}SpiForm_t;

typedef union {
    I2cForm_t i2c;
    SpiForm_t spi;
}BusForm_t;

typedef enum {
    BUS_TYPE_I2C,
    BUS_TYPE_SPI
}BusType_t;

typedef struct {
    BYTE id;
    BusType_t bus_type;
    BusForm_t busform;
    BOOL      software;
}BusTotalForm_t;

typedef struct {
    DWORD (*BusInit)(BYTE id);
    DWORD (*BusWrite)(CONST BusTotalForm_t *bus, CONST BYTE *tx, BYTE txlen);
    DWORD (*BusRead)(CONST BusTotalForm_t *bus, BYTE *rx, BYTE rxlen);
}BusOps_t;

typedef struct {
    BusTotalForm_t bustotalform;
    BusOps_t busops;
}BusInfo_t;

typedef enum {
    BUS_ID_TIA,
    BUS_ID_DRV,
    BUS_ID_DSP,
    BUS_ID_NUM
}BusId_t;

CONST BusInfo_t *GetBusInfo(BYTE id);

#endif

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

/* Bus implementation mode (BusTotalForm_t.software). BUS_SW bit-bangs on
 * the form's own port/pin fields; BUS_HW lets the driver mux the pins from
 * its per-instance table (g_I2cHwInfo/g_SpiHwInfo in mcu_i2c.c/mcu_spi.c),
 * keyed by the form's i2cno/spino. */
typedef enum {
    BUS_HW = 0,   /* hardware I2Cx/SPIx, driver instance table */
    BUS_SW = 1    /* software bit-bang on the form's GPIO fields */
}BusSw_t;

/* Bus rate classes (BusTotalForm_t.speed). The real rate is bus-type
 * specific: I2C STD/FAST/HIGH = 100k/400k/1M (hardware; bit-bang tops out
 * ~300k, see mcu_i2c.c), SPI STD/FAST/HIGH = 1M/4M/10M nominal (1.25M/5M/
 * 10M real by power-of-2 prescaler, see mcu_spi.c). Positional parity with
 * I2cSpeed_t/SpiSpeed_t in the driver headers is required — busresource_impl.c
 * casts between them. */
typedef enum {
    BUS_SPEED_STD = 0,   /* I2C 100 kHz / SPI 1 MHz class */
    BUS_SPEED_FAST,      /* I2C 400 kHz / SPI 4 MHz class */
    BUS_SPEED_HIGH,      /* I2C 1 MHz / SPI 10 MHz class */
    BUS_SPEED_NUM
}BusSpeed_t;

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
    BusSw_t   software;
    BusSpeed_t speed;
    /* TRUE = the bus nets are shared with a peer master (SPI only today:
     * the DSP boots U15 over the SPI2 nets), so BoardBusInit must NOT park
     * the peripheral — the owner driver configures the bus per session and
     * tri-states afterwards. Ignored (FALSE) on private buses. */
    BOOL      shared;
    BusForm_t busform;
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
    BUS_ID_PMIC,
    /* External SPI flash bus (2026-10-16): SCK=PG9 / MOSI=PG11 / MISO=PG10,
     * CS=PG12 (GPIO_ID_EXTFLASH_CS). Carries U15 (SST25PF040C, 512KB SPI
     * NOR) that boots the DSP (U11, BCM87420): the DSP SPI master and MCU
     * SPI2 share the bus, so the MCU may only drive it while the DSP is not
     * owning it (reset held, or after boot once the DSP SPI master is idle
     * — verify on bench). */
    BUS_ID_EXTFLASH,
    BUS_ID_NUM
}BusId_t;

CONST BusInfo_t *GetBusInfo(BYTE id);

#endif

/** @file busresource_impl.c
 * @brief Board resource implementation.
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 */
#include "busresource_impl.h"
#include "gd32e511_512.h"
#include "gpioresource.h"
#include "mcu_i2c.h"
#include "mcu_spi.h"

STATIC DWORD BoardBusInit(BYTE id);
STATIC DWORD BoardBusWrite(CONST BusTotalForm_t *bus, CONST BYTE *tx, BYTE txlen);
STATIC DWORD BoardBusRead(CONST BusTotalForm_t *bus, BYTE *rx, BYTE rxlen);

STATIC BOOL g_Ready[BUS_ID_NUM];

/* BusTotalForm_t: id, bus_type, busform (.i2c/.spi), software.
 * I2cForm_t: i2cno, i2caddr (7-bit), port, scl, sda.
 * SpiForm_t: spino, spicsid (a GPIO ID, active low CS), port, sck, mosi, miso.
 * TIA and DRV each get a software I2C entry on PB8(SCL)/PB9(SDA); DSP uses the
 * hardware I2C0 bus (PB6=SCL, PB7=SDA, 7-bit address 0x50). The SPI chip
 * select is driven by the bus layer around each WriteBus/ReadBus call. */
STATIC CONST BusInfo_t g_BusInfo[BUS_ID_NUM] = {
    {{BUS_ID_TIA, BUS_TYPE_I2C, {.i2c = {0, 0x5B, GPIOB, GPIO_PIN_8, GPIO_PIN_9}}, TRUE}, {BoardBusInit, BoardBusWrite, BoardBusRead}},
    {{BUS_ID_DRV, BUS_TYPE_I2C, {.i2c = {0, 0x53, GPIOB, GPIO_PIN_8, GPIO_PIN_9}}, TRUE}, {BoardBusInit, BoardBusWrite, BoardBusRead}},
    {{BUS_ID_DSP, BUS_TYPE_I2C, {.i2c = {0, 0x50, GPIOB, GPIO_PIN_6, GPIO_PIN_7}}, FALSE}, {BoardBusInit, BoardBusWrite, BoardBusRead}},
};

CONST BusInfo_t *GetBusInfo(BYTE id)
{
    return id < BUS_ID_NUM ? &g_BusInfo[id] : NULL;
}

STATIC DWORD BoardBusInit(BYTE id)
{
    CONST BusInfo_t *info = GetBusInfo(id);
    if(info == NULL) {
        return ERR;
    }
    if(g_Ready[id]) {
        return OK;
    }
    switch(info->bustotalform.bus_type) {
    case BUS_TYPE_I2C: {
        CONST I2cForm_t *i2c = &info->bustotalform.busform.i2c;
        if(info->bustotalform.software) {
            if(I2cBusSoftwareInit(i2c->port, i2c->scl, i2c->sda) != OK) {
                return ERR;
            }
        } else {
            I2cBusInit(i2c->i2cno, i2c->port, i2c->scl, i2c->sda);
        }
        break;
    }
    case BUS_TYPE_SPI: {
        CONST SpiForm_t *spi = &info->bustotalform.busform.spi;
        /* Release the chip select; the GPIO itself is initialized by BoardInit. */
        (void)WriteGpio(spi->spicsid.spicsio, IO_HIGH);
        if(info->bustotalform.software) {
            (void)SpiBusSoftwareInit(spi->port, spi->sck, spi->mosi, spi->miso);
        } else {
            SpiBusInit(spi->spino, spi->port, spi->sck, spi->mosi, spi->miso);
        }
        break;
    }
    default:
        return ERR;
    }
    g_Ready[id] = TRUE;
    return OK;
}

STATIC DWORD BoardBusWrite(CONST BusTotalForm_t *bus, CONST BYTE *tx, BYTE txlen)
{
    if(bus == NULL || !g_Ready[bus->id]) {
        return ERR;
    }
    switch(bus->bus_type) {
    case BUS_TYPE_I2C: {
        CONST I2cForm_t *i2c = &bus->busform.i2c;
        if(bus->software) {
            return I2cBusSoftwareWrite(i2c->port, i2c->scl, i2c->sda, i2c->i2caddr.i2caddr, tx, txlen);
        }
        return I2cBusWrite(i2c->i2cno, i2c->i2caddr.i2caddr, tx, txlen);
    }
    case BUS_TYPE_SPI: {
        CONST SpiForm_t *spi = &bus->busform.spi;
        BYTE cs = spi->spicsid.spicsio;
        DWORD result;
        /* Active low chip select around the whole write. */
        if(WriteGpio(cs, IO_LOW) != OK) {
            return ERR;
        }
        result = bus->software ? SpiBusSoftwareWrite(spi->port, spi->sck, spi->mosi, spi->miso, tx, txlen)
                               : SpiBusWrite(spi->spino, tx, txlen);
        (void)WriteGpio(cs, IO_HIGH);
        return result;
    }
    default:
        return ERR;
    }
}

STATIC DWORD BoardBusRead(CONST BusTotalForm_t *bus, BYTE *rx, BYTE rxlen)
{
    if(bus == NULL || !g_Ready[bus->id]) {
        return ERR;
    }
    switch(bus->bus_type) {
    case BUS_TYPE_I2C: {
        CONST I2cForm_t *i2c = &bus->busform.i2c;
        if(bus->software) {
            return I2cBusSoftwareRead(i2c->port, i2c->scl, i2c->sda, i2c->i2caddr.i2caddr, rx, rxlen);
        }
        return I2cBusRead(i2c->i2cno, i2c->i2caddr.i2caddr, rx, rxlen);
    }
    case BUS_TYPE_SPI: {
        CONST SpiForm_t *spi = &bus->busform.spi;
        BYTE cs = spi->spicsid.spicsio;
        DWORD result;
        if(WriteGpio(cs, IO_LOW) != OK) {
            return ERR;
        }
        result = bus->software ? SpiBusSoftwareRead(spi->port, spi->sck, spi->mosi, spi->miso, rx, rxlen)
                               : SpiBusRead(spi->spino, rx, rxlen);
        (void)WriteGpio(cs, IO_HIGH);
        return result;
    }
    default:
        return ERR;
    }
}

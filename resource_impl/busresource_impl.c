/** @file busresource_impl.c
 * @brief Board resource implementation.
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 */
#include "busresource_impl.h"
#include "gd32e511_512.h"
#include "gpioresource.h"
#include "gpioresource_impl.h"
#include "mcu_i2c.h"
#include "mcu_spi.h"

STATIC DWORD BoardBusInit(BYTE id);
STATIC DWORD BoardBusWrite(CONST BusTotalForm_t *bus, CONST BYTE *tx, BYTE txlen);
STATIC DWORD BoardBusRead(CONST BusTotalForm_t *bus, BYTE *rx, BYTE rxlen);

STATIC BOOL g_Ready[BUS_ID_NUM];

/* BusTotalForm_t: id, bus_type, software (BusSw_t), speed (BusSpeed_t),
 * shared, busform (.i2c/.spi).
 * software = BUS_SW: the bus is bit-banged on the form's own port/pin
 * fields; the speed class maps to the bit-bang timing (I2C half-period
 * delay, SPI fixed delays — see mcu_i2c.c/mcu_spi.c for real rates).
 * software = BUS_HW: the driver muxes the pins from its per-instance table
 * (g_I2cHwInfo/g_SpiHwInfo, keyed by the form's i2cno/spino) and the speed
 * class selects the peripheral rate (I2C timing registers / SPI prescaler).
 * BusSpeed_t is positionally identical to the driver I2cSpeed_t/SpiSpeed_t
 * (STD=0/FAST=1/HIGH=2), so the layer casts between them.
 * I2cForm_t: i2cno, i2caddr (7-bit), port, scl, sda.
 * SpiForm_t: spino, spicsid (a GPIO ID, active low CS), port, sck, mosi,
 * miso.
 * shared (SPI only): the nets have a peer master — the DSP boots U15 over
 * the SPI2 nets — so BoardBusInit skips the row and driver/extflash
 * acquires/releases the bus per session.
 * TIA and DRV share a software I2C entry on PB8(SCL)/PB9(SDA); the PMIC
 * (SY70300B, 7-bit 0x1E internal-MTP default) gets its own software I2C on
 * PG14(SCL)/PG13(SDA); both run the FAST class (~300 kHz real on the
 * bit-bang ceiling). DSP uses hardware I2C (7-bit address 0x50) at the STD
 * = 100 kHz class — its pin mux lives in the driver's g_I2cHwInfo and the
 * row's port/scl/sda are documentation only on hardware rows.
 * BUS_ID_EXTFLASH is hardware SPI (CS = GPIO_ID_EXTFLASH_CS, PG12) shared
 * with the DSP SPI master for U15, the SST25PF040C that boots the DSP —
 * see the BusId_t comment for the ownership rules. The bus-layer CS wrap
 * only suits plain software SPI rows; driver/extflash bypasses
 * WriteBus/ReadBus (NOR needs command+address+data under one CS). */
STATIC CONST BusInfo_t g_BusInfo[BUS_ID_NUM] = {
    {{BUS_ID_TIA, BUS_TYPE_I2C, BUS_SW, BUS_SPEED_FAST, FALSE, {.i2c = {0, 0x5B, GPIOB, GPIO_PIN_8, GPIO_PIN_9}}}, {BoardBusInit, BoardBusWrite, BoardBusRead}},
    {{BUS_ID_DRV, BUS_TYPE_I2C, BUS_SW, BUS_SPEED_FAST, FALSE, {.i2c = {0, 0x53, GPIOB, GPIO_PIN_8, GPIO_PIN_9}}}, {BoardBusInit, BoardBusWrite, BoardBusRead}},
    /* DSP management bus (BSC): hardware I2C instance 2 (PB6=SCL/PB7=SDA
     * per the driver's g_I2cHwInfo), STD = 100 kHz class. The row's
     * port/scl/sda document the wiring only — the driver muxes from its own
     * instance table on BUS_HW rows. */
    {{BUS_ID_DSP, BUS_TYPE_I2C, BUS_HW, BUS_SPEED_STD, FALSE, {.i2c = {2, 0x50, GPIOB, GPIO_PIN_6, GPIO_PIN_7}}}, {BoardBusInit, BoardBusWrite, BoardBusRead}},
    {{BUS_ID_PMIC, BUS_TYPE_I2C, BUS_SW, BUS_SPEED_FAST, FALSE, {.i2c = {0, 0x1E, GPIOG, GPIO_PIN_14, GPIO_PIN_13}}}, {BoardBusInit, BoardBusWrite, BoardBusRead}},
    /* U15 (SST25PF040C, 512KB external SPI NOR flash) on hardware SPI2, FAST
     * class (5 MHz real by the /16 prescaler; well inside the part's 25 MHz
     * READ limit — downgrade to BUS_SPEED_STD at bring-up only if U15
     * misbehaves). shared: the nets are shared with the DSP SPI master (it
     * boots its microcode from U15), so BoardBusInit must NOT park SPI2 as
     * an AF master — driver/extflash acquires and releases the bus per
     * session instead. The pin mux (PG9/PG11/PG10, AF 0 — user-confirmed
     * 2026-10-16) lives in the driver's per-instance table g_SpiHwInfo
     * (mcu_spi.c); the call sites only pass spino + speed. CS is
     * GPIO_ID_EXTFLASH_CS (PG12, Hi-Z input by default in the GPIO resource
     * table). */
    {{BUS_ID_EXTFLASH, BUS_TYPE_SPI, BUS_HW, BUS_SPEED_FAST, TRUE, {.spi = {2, {GPIO_ID_EXTFLASH_CS}, GPIOG, GPIO_PIN_9, GPIO_PIN_11, GPIO_PIN_10}}}, {BoardBusInit, BoardBusWrite, BoardBusRead}},
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
        if(info->bustotalform.software == BUS_SW) {
            /* Bit-bang on the form's own pins; the speed class maps to the
             * half-period delay inside the driver. */
            if(I2cBusSoftwareInit(i2c->port, i2c->scl, i2c->sda, (I2cSpeed_t)info->bustotalform.speed) != OK) {
                return ERR;
            }
        } else {
            /* Hardware I2C: the SCL/SDA pin mux comes from the driver's
             * per-instance table g_I2cHwInfo (mcu_i2c.c), not from this
             * row — the row's port/scl/sda members only serve the software
             * bit-bang rows. The speed class programs the timing registers. */
            I2cBusInit(i2c->i2cno, (I2cSpeed_t)info->bustotalform.speed);
        }
        break;
    }
    case BUS_TYPE_SPI: {
        CONST SpiForm_t *spi = &info->bustotalform.busform.spi;
        /* shared rows carry a peer master on the nets (the DSP booting U15
         * over them): their owner driver configures the bus per session
         * and tri-states afterwards — parking an AF master here would
         * fight the peer. */
        if(!info->bustotalform.shared) {
            /* Release the chip select; the GPIO itself is initialized by BoardInit. */
            (void)WriteGpio(spi->spicsid.spicsio, IO_HIGH);
            if(info->bustotalform.software == BUS_SW) {
                (void)SpiBusSoftwareInit(spi->port, spi->sck, spi->mosi, spi->miso);
            } else {
                /* Hardware SPI: the SCK/MOSI/MISO pin mux comes from the
                 * driver's per-instance table g_SpiHwInfo (mcu_spi.c), not
                 * from this row; the speed class selects the prescaler. */
                SpiBusInit(spi->spino, (SpiSpeed_t)info->bustotalform.speed);
            }
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
        if(bus->software == BUS_SW) {
            return I2cBusSoftwareWrite(i2c->port, i2c->scl, i2c->sda, (I2cSpeed_t)bus->speed, i2c->i2caddr.i2caddr, tx, txlen);
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
        result = bus->software == BUS_SW ? SpiBusSoftwareWrite(spi->port, spi->sck, spi->mosi, spi->miso, tx, txlen)
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
        if(bus->software == BUS_SW) {
            return I2cBusSoftwareRead(i2c->port, i2c->scl, i2c->sda, (I2cSpeed_t)bus->speed, i2c->i2caddr.i2caddr, rx, rxlen);
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
        result = bus->software == BUS_SW ? SpiBusSoftwareRead(spi->port, spi->sck, spi->mosi, spi->miso, rx, rxlen)
                                         : SpiBusRead(spi->spino, rx, rxlen);
        (void)WriteGpio(cs, IO_HIGH);
        return result;
    }
    default:
        return ERR;
    }
}

/** @file mcu_spi.c
 * @brief MCU SPI master implementation on the GD32 firmware library.
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 */
#include "mcu_spi.h"
#include "mcu_clock.h"
#include "gd32e511_512.h"

#define SPI_WAIT_US 10000U
#define SPI_NUM_MAX 3U

STATIC BOOL g_SpiReady[SPI_NUM_MAX + 1U];

/* ---------------- hardware SPIx master (mode 0, MSB first, /16 = 5 MHz) ---------------- */

STATIC DWORD SpiPeriph(BYTE spino)
{
    if(spino == 0U) {
        return SPI0;
    }
    if(spino == 1U) {
        return SPI1;
    }
    if(spino == 2U) {
        return SPI2;
    }
    return SPI3;
}

STATIC DWORD SpiRcuClock(BYTE spino)
{
    if(spino == 0U) {
        return RCU_SPI0;
    }
    if(spino == 1U) {
        return RCU_SPI1;
    }
    if(spino == 2U) {
        return RCU_SPI2;
    }
    return RCU_SPI3;
}

STATIC DWORD SpiHardwareRcuPort(DWORD port)
{
    if(port == GPIOA) {
        return RCU_GPIOA;
    }
    if(port == GPIOB) {
        return RCU_GPIOB;
    }
    return RCU_GPIOC;
}

STATIC VOID SpiHardwareConfig(BYTE spino)
{
    spi_parameter_struct spar;
    spi_deinit(SpiPeriph(spino));
    spi_struct_para_init(&spar);
    spar.device_mode = SPI_MASTER;
    spar.trans_mode = SPI_TRANSMODE_FULLDUPLEX;
    spar.frame_size = SPI_FRAMESIZE_8BIT;
    spar.nss = SPI_NSS_SOFT;
    spar.endian = SPI_ENDIAN_MSB;
    spar.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spar.prescale = SPI_PSC_16;
    spi_init(SpiPeriph(spino), &spar);
    spi_enable(SpiPeriph(spino));
}

VOID SpiBusInit(BYTE spino, DWORD port, DWORD sck, DWORD mosi, DWORD miso)
{
    if(spino > SPI_NUM_MAX) {
        return;
    }
    rcu_periph_clock_enable(SpiHardwareRcuPort(port));
    rcu_periph_clock_enable(SpiRcuClock(spino));
    gpio_af_set(port, GPIO_AF_5, sck | mosi | miso);
    gpio_mode_set(port, GPIO_MODE_AF, GPIO_PUPD_NONE, sck | mosi | miso);
    gpio_output_options_set(port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, sck | mosi);
    SpiHardwareConfig(spino);
    g_SpiReady[spino] = TRUE;
}

STATIC DWORD SpiWaitFlag(DWORD spi, DWORD flag, FlagStatus state)
{
    DWORD timeout = SPI_WAIT_US;
    do {
        if(spi_flag_get(spi, flag) == state) {
            return OK;
        }
        McuClockDelayUs(1);
    } while(--timeout != 0U);
    return ERR;
}

DWORD SpiBusWrite(BYTE spino, CONST BYTE *tx, BYTE txlen)
{
    BYTE i;
    DWORD spi;
    if(spino > SPI_NUM_MAX || !g_SpiReady[spino] || tx == NULL || txlen == 0) {
        return ERR;
    }
    spi = SpiPeriph(spino);
    for(i = 0; i < txlen; ++i) {
        if(SpiWaitFlag(spi, SPI_FLAG_TBE, SET) != OK) {
            goto fail;
        }
        spi_data_transmit(spi, tx[i]);
        if(SpiWaitFlag(spi, SPI_FLAG_RBNE, SET) != OK) {
            goto fail;
        }
        (void)spi_data_receive(spi);
    }
    return OK;
fail:
    SpiHardwareConfig(spino);
    return ERR;
}

DWORD SpiBusRead(BYTE spino, BYTE *rx, BYTE rxlen)
{
    BYTE i;
    DWORD spi;
    if(spino > SPI_NUM_MAX || !g_SpiReady[spino] || rx == NULL || rxlen == 0) {
        return ERR;
    }
    spi = SpiPeriph(spino);
    for(i = 0; i < rxlen; ++i) {
        if(SpiWaitFlag(spi, SPI_FLAG_TBE, SET) != OK) {
            goto fail;
        }
        spi_data_transmit(spi, 0xFFU);
        if(SpiWaitFlag(spi, SPI_FLAG_RBNE, SET) != OK) {
            goto fail;
        }
        rx[i] = (BYTE)spi_data_receive(spi);
    }
    return OK;
fail:
    SpiHardwareConfig(spino);
    while(rxlen-- != 0U) {
        *rx++ = 0xFF;
    }
    return ERR;
}

/* ---------------- software bit-bang master (mode 0, MSB first) ---------------- */

STATIC DWORD SpiSoftwareRcuPort(DWORD port)
{
    if(port == GPIOA) {
        return RCU_GPIOA;
    }
    if(port == GPIOB) {
        return RCU_GPIOB;
    }
    return RCU_GPIOC;
}

DWORD SpiBusSoftwareInit(DWORD port, DWORD sck, DWORD mosi, DWORD miso)
{
    rcu_periph_clock_enable(SpiSoftwareRcuPort(port));
    gpio_bit_reset(port, sck | mosi);
    gpio_output_options_set(port, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, sck | mosi);
    gpio_mode_set(port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, sck | mosi);
    gpio_mode_set(port, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, miso);
    return OK;
}

/* One mode-0 bit: MOSI is set while SCK is low, sampled on the rising edge. */
STATIC DWORD SpiSoftwareTransferByte(DWORD port, DWORD sck, DWORD mosi, DWORD miso, BYTE tx, BYTE *rx)
{
    BYTE i;
    BYTE data = 0;
    for(i = 0; i < 8; ++i) {
        gpio_bit_write(port, mosi, (tx & 0x80U) != 0 ? SET : RESET);
        McuClockDelayUs(1);
        gpio_bit_set(port, sck);
        McuClockDelayUs(1);
        data = (BYTE)((data << 1) | (gpio_input_bit_get(port, miso) == SET ? 1U : 0U));
        gpio_bit_reset(port, sck);
        McuClockDelayUs(1);
        tx <<= 1;
    }
    *rx = data;
    return OK;
}

DWORD SpiBusSoftwareWrite(DWORD port, DWORD sck, DWORD mosi, DWORD miso, CONST BYTE *tx, BYTE txlen)
{
    BYTE i, dummy;
    if(tx == NULL || txlen == 0) {
        return ERR;
    }
    for(i = 0; i < txlen; ++i) {
        (void)SpiSoftwareTransferByte(port, sck, mosi, miso, tx[i], &dummy);
    }
    return OK;
}

DWORD SpiBusSoftwareRead(DWORD port, DWORD sck, DWORD mosi, DWORD miso, BYTE *rx, BYTE rxlen)
{
    BYTE i;
    if(rx == NULL || rxlen == 0) {
        return ERR;
    }
    for(i = 0; i < rxlen; ++i) {
        (void)SpiSoftwareTransferByte(port, sck, mosi, miso, 0xFFU, &rx[i]);
    }
    return OK;
}

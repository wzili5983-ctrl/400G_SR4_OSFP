/** @file mcu_spi.c
 * @brief MCU SPI master implementation on the GD32 firmware library.
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 */
#include "mcu_spi.h"
#include "mcu_clock.h"
#include "gd32e511_512.h"

#define SPI_WAIT_US 10000U

STATIC BOOL g_SpiReady[SPI_NUM];
STATIC SpiSpeed_t g_SpiSpeed[SPI_NUM];  /* per-instance rate, set by SpiBusInit */

/* Prescaler per speed class. SPI2/SPI3 kernel clock is PCLK1 = 80 MHz
 * (APB1 undivided, see McuClockInit); power-of-2 prescalers give the
 * closest fit to the nominal class rates:
 *   STD  = 1 MHz  class -> PSC/64 = 1.25 MHz real
 *   FAST = 4 MHz  class -> PSC/16 = 5 MHz real
 *   HIGH = 10 MHz class -> PSC/8  = 10 MHz exact */
STATIC CONST DWORD k_SpiHwPsc[SPI_SPEED_NUM] = {
    SPI_PSC_64,
    SPI_PSC_16,
    SPI_PSC_8,
};

/* ---------------- hardware SPIx master (mode 0, MSB first) ---------------- */

typedef struct {
    DWORD port;     /* GPIO port for SCK/MOSI/MISO */
    DWORD sck;      /* GPIO pin for SCK */
    DWORD miso;     /* GPIO pin for MISO */
    DWORD mosi;     /* GPIO pin for MOSI */
    DWORD alt_func_num; /* Alternate function number */
}SpiHwForm_t;

/* Hardware SPIx instance pin registry, indexed by SpiNum_t (mcu_spi.h):
 * SPI_2 is the U15 boot-flash bus (SCK/MOSI/MISO shared with the DSP SPI
 * master — the driver tri-states the nets again via SpiBusRelease() after
 * every session); SPI_0/SPI_1/SPI_3 have no consumer on this board (zeros).
 * SpiBusInit() muxes SCK/MOSI/MISO from this table; the peripheral base
 * address and RCU gate are register identities, resolved by
 * SpiHwPeriph()/SpiHwRcuClock() below, not board data. SPI_2 on GPIOG is
 * AF 0 (user-confirmed 2026-10-16). */
STATIC CONST SpiHwForm_t g_SpiHwInfo[SPI_NUM] = {
    {GPIOB, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_AF_0},        /* SPI_0: unused */
    {GPIOB, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15, GPIO_AF_0},     /* SPI_1: unused */
    {GPIOG, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11, GPIO_AF_0},      /* SPI_2: U15 flash bus */
    {0, 0, 0, 0, 0},                                               /* SPI_3: unused */
};

/* Peripheral register identity, keyed by SpiNum_t; callers range-guard
 * spino < SPI_NUM, so the fall-through is only reachable for SPI_3. */
STATIC DWORD SpiHwPeriph(BYTE spino)
{
    if(spino == SPI_0) {
        return SPI0;
    }
    if(spino == SPI_1) {
        return SPI1;
    }
    if(spino == SPI_2) {
        return SPI2;
    }
    return SPI3;
}

STATIC DWORD SpiHwRcuClock(BYTE spino)
{
    if(spino == SPI_0) {
        return RCU_SPI0;
    }
    if(spino == SPI_1) {
        return RCU_SPI1;
    }
    if(spino == SPI_2) {
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
    if(port == GPIOC) {
        return RCU_GPIOC;
    }
    if(port == GPIOD) {
        return RCU_GPIOD;
    }
    if(port == GPIOE) {
        return RCU_GPIOE;
    }
    if(port == GPIOF) {
        return RCU_GPIOF;
    }
    return RCU_GPIOG;
}

STATIC VOID SpiHardwareConfig(BYTE spino)
{
    spi_parameter_struct spar;
    DWORD speed = g_SpiSpeed[spino] < SPI_SPEED_NUM ? g_SpiSpeed[spino] : SPI_SPEED_STD;

    spi_deinit(SpiHwPeriph(spino));
    spi_struct_para_init(&spar);
    spar.device_mode = SPI_MASTER;
    spar.trans_mode = SPI_TRANSMODE_FULLDUPLEX;
    spar.frame_size = SPI_FRAMESIZE_8BIT;
    spar.nss = SPI_NSS_SOFT;
    spar.endian = SPI_ENDIAN_MSB;
    spar.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spar.prescale = k_SpiHwPsc[speed];
    spi_init(SpiHwPeriph(spino), &spar);
    spi_enable(SpiHwPeriph(spino));
}

VOID SpiBusInit(BYTE spino, SpiSpeed_t speed)
{
    CONST SpiHwForm_t *form;
    if(spino >= SPI_NUM) {
        return;
    }
    g_SpiSpeed[spino] = speed < SPI_SPEED_NUM ? speed : SPI_SPEED_STD;
    form = &g_SpiHwInfo[spino];
    rcu_periph_clock_enable(SpiHardwareRcuPort(form->port));
    rcu_periph_clock_enable(SpiHwRcuClock(spino));
    gpio_af_set(form->port, form->alt_func_num, form->sck | form->mosi | form->miso);
    gpio_mode_set(form->port, GPIO_MODE_AF, GPIO_PUPD_NONE, form->sck | form->mosi | form->miso);
    gpio_output_options_set(form->port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, form->sck | form->mosi);
    SpiHardwareConfig(spino);
    g_SpiReady[spino] = TRUE;
}

VOID SpiBusRelease(BYTE spino)
{
    CONST SpiHwForm_t *form;
    if(spino >= SPI_NUM) {
        return;
    }
    form = &g_SpiHwInfo[spino];
    /* Back to Hi-Z: the bus may be shared with a peer master (the DSP boots
     * from U15 over these nets), so the MCU must not idle-drive SCK/MOSI. */
    gpio_mode_set(form->port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, form->sck | form->mosi | form->miso);
    g_SpiReady[spino] = FALSE;
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
    if(spino >= SPI_NUM || !g_SpiReady[spino] || tx == NULL || txlen == 0) {
        return ERR;
    }
    spi = SpiHwPeriph(spino);
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
    if(spino >= SPI_NUM || !g_SpiReady[spino] || rx == NULL || rxlen == 0) {
        return ERR;
    }
    spi = SpiHwPeriph(spino);
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
/* Fixed ~1us delays (3 delay sites per bit) give ~333 kbit/s real — below
 * even the 1 MHz STD class, so the bit-bang master takes no speed argument;
 * the SpiSpeed_t classes only select the hardware prescaler. */

STATIC DWORD SpiSoftwareRcuPort(DWORD port)
{
    if(port == GPIOA) {
        return RCU_GPIOA;
    }
    if(port == GPIOB) {
        return RCU_GPIOB;
    }
    if(port == GPIOC) {
        return RCU_GPIOC;
    }
    if(port == GPIOD) {
        return RCU_GPIOD;
    }
    if(port == GPIOE) {
        return RCU_GPIOE;
    }
    if(port == GPIOF) {
        return RCU_GPIOF;
    }
    return RCU_GPIOG;
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

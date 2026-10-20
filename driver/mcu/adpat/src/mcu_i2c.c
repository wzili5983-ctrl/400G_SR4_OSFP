/** @file mcu_i2c.c
 * @brief MCU I2C master implementation on the GD32 firmware library.
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 */
#include "mcu_i2c.h"
#include "mcu_clock.h"
#include "gd32e511_512.h"

#define I2C_WAIT_US     10000U
#define I2C_ERROR_FLAGS (I2C_FLAG_NACK | I2C_FLAG_BERR | I2C_FLAG_LOSTARB | I2C_FLAG_OUERR)

STATIC BOOL g_I2cReady[I2C_NUM];
STATIC I2cSpeed_t g_I2cSpeed[I2C_NUM];  /* per-instance rate, set by I2cBusInit */

/* Hardware timing per speed class. I2C kernel clock is IRC8M = 8 MHz and
 * tSCL = 8 MHz / (PSC+1) / (SCLL + SCLH + 2); SCLDELY/SDADELY are the
 * data setup/hold delays in tI2CCLK (125 ns at PSC=0). */
typedef struct {
    BYTE psc;       /* I2C_TIMING_PSC (4-bit) */
    BYTE scldely;   /* I2C_TIMING_SCLDELY */
    BYTE sdadely;   /* I2C_TIMING_SDADELY */
    BYTE scll;      /* I2C_TIMING_SCLL */
    BYTE sclh;      /* I2C_TIMING_SCLH */
}I2cHwTiming_t;

STATIC CONST I2cHwTiming_t k_I2cHwTiming[I2C_SPEED_NUM] = {
    /* STD = 100 kHz: 8M/(39+39+2) = 100 kHz — the original proven values. */
    {0, 9, 2, 39, 39},
    /* FAST = 400 kHz class: 8M/(11+6+2) = ~421 kHz, tLOW = 1.375 us /
     * tHIGH = 0.75 us (Fast-mode tLOW>=1.3 us / tHIGH>=0.6 us met). */
    {0, 4, 2, 11, 6},
    /* HIGH = 1 MHz class: 8M/(4+3+2) = ~889 kHz, nearest to 1 MHz with
     * Fm+ tLOW = 0.5 us / tHIGH = 0.375 us. */
    {0, 2, 1, 4, 3},
};

/* ---------------- hardware I2Cx master (open drain, AF per g_I2cHwInfo) ---------------- */

typedef struct {
    DWORD port;     /* GPIO port for SCL/SDA */
    DWORD scl;      /* GPIO pin for SCL */
    DWORD sda;      /* GPIO pin for SDA */
    DWORD alt_func_num; /* Alternate function number */
}I2cHwForm_t;

/* Hardware I2Cx instance pin registry, indexed by I2cNum_t (mcu_i2c.h);
 * row data per the GD32E511 pin muxing table (user-filled 2026-10-16, ★
 * verify at bring-up):
 *   I2C_0: PB8/PB9, AF1 — shares its pins with the TIA/DRV software
 *     bit-bang bus; safe only because no BUS_HW bus row uses i2cno 0.
 *   I2C_1: PA0/PA1, AF4 — instance default; the host slave link physically
 *     runs on PB13/PB14 and is configured by protocol/mci i2c_slave.c
 *     itself (this row is the instance reference only).
 *   I2C_2: PB6/PB7, AF4 — master bus to the DSP (BUS_ID_DSP row, i2cno=2).
 * I2cBusInit() muxes SCL/SDA from this table; the peripheral base address
 * and RCU gate are register identities, resolved by I2cHwPeriph()/
 * I2cHwRcuClock() below, not board data. All rows are open drain. */
STATIC CONST I2cHwForm_t g_I2cHwInfo[I2C_NUM] = {
    {GPIOB, GPIO_PIN_8,  GPIO_PIN_9,  GPIO_AF_1},   /* I2C_0: PB8/PB9 (shared with TIA/DRV SW bus pins) */
    {GPIOA, GPIO_PIN_0,  GPIO_PIN_1,  GPIO_AF_4},   /* I2C_1: PA0/PA1 instance default */
    {GPIOB, GPIO_PIN_6,  GPIO_PIN_7,  GPIO_AF_4},   /* I2C_2: master to DSP */
};

/* Peripheral register identity, keyed by I2cNum_t; callers range-guard
 * i2cno < I2C_NUM, so the fall-through is only reachable for I2C_2. */
STATIC DWORD I2cHwPeriph(BYTE i2cno)
{
    if(i2cno == I2C_0) {
        return I2C0;
    }
    if(i2cno == I2C_1) {
        return I2C1;
    }
    return I2C2;
}

STATIC DWORD I2cHwRcuClock(BYTE i2cno)
{
    if(i2cno == I2C_0) {
        return RCU_I2C0;
    }
    if(i2cno == I2C_1) {
        return RCU_I2C1;
    }
    return RCU_I2C2;
}

STATIC DWORD I2cHardwareRcuPort(DWORD port)
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

STATIC VOID I2cHardwareConfig(BYTE i2cno)
{
    DWORD i2c = I2cHwPeriph(i2cno);
    CONST I2cHwTiming_t *timing = &k_I2cHwTiming[g_I2cSpeed[i2cno] < I2C_SPEED_NUM ? g_I2cSpeed[i2cno] : I2C_SPEED_STD];

    i2c_deinit(i2c);
    rcu_i2c_clock_config(i2c, RCU_I2CSRC_IRC8M);
    i2c_timing_config(i2c, timing->psc, timing->scldely, timing->sdadely);
    i2c_master_clock_config(i2c, timing->sclh, timing->scll);
    i2c_enable(i2c);
}

STATIC DWORD I2cWaitFlag(DWORD i2c, DWORD flag, FlagStatus state)
{
    DWORD timeout = I2C_WAIT_US;
    do {
        if((I2C_STAT(i2c) & I2C_ERROR_FLAGS) != 0) {
            return ERR;
        }
        if(i2c_flag_get(i2c, flag) == state) {
            return OK;
        }
        McuClockDelayUs(1);
    } while(--timeout != 0U);
    return ERR;
}

VOID I2cBusInit(BYTE i2cno, I2cSpeed_t speed)
{
    CONST I2cHwForm_t *form;
    if(i2cno >= I2C_NUM) {
        return;
    }
    g_I2cSpeed[i2cno] = speed < I2C_SPEED_NUM ? speed : I2C_SPEED_STD;
    form = &g_I2cHwInfo[i2cno];
    rcu_periph_clock_enable(I2cHardwareRcuPort(form->port));
    rcu_periph_clock_enable(I2cHwRcuClock(i2cno));
    gpio_af_set(form->port, form->alt_func_num, form->scl | form->sda);
    gpio_mode_set(form->port, GPIO_MODE_AF, GPIO_PUPD_NONE, form->scl | form->sda);
    gpio_output_options_set(form->port, GPIO_OTYPE_OD, GPIO_OSPEED_2MHZ, form->scl | form->sda);
    I2cHardwareConfig(i2cno);
    g_I2cReady[i2cno] = TRUE;
}

DWORD I2cBusWrite(BYTE i2cno, BYTE addr7, CONST BYTE *tx, BYTE txlen)
{
    BYTE i;
    DWORD i2c;
    if(i2cno >= I2C_NUM || !g_I2cReady[i2cno] || addr7 < 8 || addr7 > 0x77 || tx == NULL || txlen == 0) {
        return ERR;
    }
    i2c = I2cHwPeriph(i2cno);
    if(I2cWaitFlag(i2c, I2C_FLAG_I2CBSY, RESET) != OK) {
        goto fail;
    }
    i2c_flag_clear(i2c, I2C_FLAG_STPDET | I2C_ERROR_FLAGS);
    i2c_reload_disable(i2c);
    i2c_master_addressing(i2c, (DWORD)addr7 << 1, I2C_MASTER_TRANSMIT);
    i2c_transfer_byte_number_config(i2c, txlen);
    i2c_automatic_end_enable(i2c);
    i2c_start_on_bus(i2c);
    for(i = 0; i < txlen; ++i) {
        if(I2cWaitFlag(i2c, I2C_FLAG_TI, SET) != OK) {
            goto fail;
        }
        i2c_data_transmit(i2c, tx[i]);
    }
    if(I2cWaitFlag(i2c, I2C_FLAG_STPDET, SET) != OK) {
        goto fail;
    }
    i2c_flag_clear(i2c, I2C_FLAG_STPDET);
    return OK;
fail:
    /* Do not generate STOP after arbitration loss; another master owns the bus. */
    if(i2c_flag_get(i2c, I2C_FLAG_LOSTARB) == RESET) {
        i2c_stop_on_bus(i2c);
        McuClockDelayUs(10);
    }
    I2cHardwareConfig(i2cno);
    return ERR;
}

DWORD I2cBusRead(BYTE i2cno, BYTE addr7, BYTE *rx, BYTE rxlen)
{
    BYTE i;
    DWORD i2c;
    if(i2cno >= I2C_NUM || !g_I2cReady[i2cno] || addr7 < 8 || addr7 > 0x77 || rx == NULL || rxlen == 0) {
        return ERR;
    }
    i2c = I2cHwPeriph(i2cno);
    if(I2cWaitFlag(i2c, I2C_FLAG_I2CBSY, RESET) != OK) {
        goto fail;
    }
    i2c_flag_clear(i2c, I2C_FLAG_STPDET | I2C_ERROR_FLAGS);
    i2c_reload_disable(i2c);
    i2c_master_addressing(i2c, (DWORD)addr7 << 1, I2C_MASTER_RECEIVE);
    i2c_transfer_byte_number_config(i2c, rxlen);
    i2c_automatic_end_enable(i2c);
    i2c_start_on_bus(i2c);
    for(i = 0; i < rxlen; ++i) {
        if(I2cWaitFlag(i2c, I2C_FLAG_RBNE, SET) != OK) {
            goto fail;
        }
        rx[i] = i2c_data_receive(i2c);
    }
    if(I2cWaitFlag(i2c, I2C_FLAG_STPDET, SET) != OK) {
        goto fail;
    }
    i2c_flag_clear(i2c, I2C_FLAG_STPDET);
    return OK;
fail:
    if(i2c_flag_get(i2c, I2C_FLAG_LOSTARB) == RESET) {
        i2c_stop_on_bus(i2c);
        McuClockDelayUs(10);
    }
    I2cHardwareConfig(i2cno);
    while(rxlen-- != 0U) {
        *rx++ = 0xFF;
    }
    return ERR;
}

/* ---------------- software bit-bang master ---------------- */
/* Speed class -> half-period delay per edge. There are three delay sites
 * per bit (SDA setup before SCL rising, SCL high hold, SCL low hold), so
 * the real rate is roughly 1 / (3 * (half_us + ~0.5us SysTick/call
 * overhead)):
 *   I2C_SPEED_STD  -> half_us = 5: ~60-100 kHz (nominal 100 k class).
 *   I2C_SPEED_FAST -> half_us = 1: ~300 kHz real.
 *   I2C_SPEED_HIGH -> half_us = 1: ~300 kHz real — the 1us SysTick
 *     granularity makes ~300 kHz the bit-bang ceiling, so FAST and HIGH
 *     are identical in software mode (true 400 k/1 M timing needs the
 *     hardware I2Cx path).
 * half_us < 1 is clamped to 1: McuClockDelayUs(0) would underflow SysTick
 * LOAD. */
STATIC BYTE I2cSwHalfUs(I2cSpeed_t speed)
{
    if(speed == I2C_SPEED_STD) {
        return 5U;
    }
    return 1U;
}

STATIC DWORD I2cSoftwareRcuPort(DWORD port)
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
    if(port == GPIOG) {
        return RCU_GPIOG;
    }
    return RCU_GPIOC;
}

STATIC VOID I2cSoftwareSda(DWORD port, DWORD sda, BOOL high)
{
    gpio_bit_write(port, sda, high ? SET : RESET);
}

STATIC VOID I2cSoftwareSclLow(DWORD port, DWORD scl, BYTE half_us)
{
    gpio_bit_reset(port, scl);
    McuClockDelayUs(half_us);
}

STATIC DWORD I2cSoftwareSclHigh(DWORD port, DWORD scl, BYTE half_us)
{
    DWORD timeout = 10000;
    gpio_bit_set(port, scl);
    /* 1µs poll granularity for clock stretching, independent of half_us. */
    while(gpio_input_bit_get(port, scl) == RESET) {
        if(timeout-- == 0U) {
            return ERR;
        }
        McuClockDelayUs(1);
    }
    McuClockDelayUs(half_us);
    return OK;
}

STATIC DWORD I2cSoftwareStart(DWORD port, DWORD scl, DWORD sda, BYTE half_us)
{
    I2cSoftwareSda(port, sda, TRUE);
    McuClockDelayUs(half_us);
    if(I2cSoftwareSclHigh(port, scl, half_us) != OK || gpio_input_bit_get(port, sda) == RESET) {
        return ERR;
    }
    I2cSoftwareSda(port, sda, FALSE);
    McuClockDelayUs(half_us);
    I2cSoftwareSclLow(port, scl, half_us);
    return OK;
}

STATIC DWORD I2cSoftwareStop(DWORD port, DWORD scl, DWORD sda, BYTE half_us)
{
    I2cSoftwareSclLow(port, scl, half_us);
    I2cSoftwareSda(port, sda, FALSE);
    McuClockDelayUs(half_us);
    if(I2cSoftwareSclHigh(port, scl, half_us) != OK) {
        I2cSoftwareSda(port, sda, TRUE);
        return ERR;
    }
    I2cSoftwareSda(port, sda, TRUE);
    McuClockDelayUs(half_us);
    return OK;
}

STATIC DWORD I2cSoftwareSendByte(DWORD port, DWORD scl, DWORD sda, BYTE half_us, BYTE value)
{
    BYTE i;
    DWORD result;
    for(i = 0; i < 8; ++i) {
        I2cSoftwareSda(port, sda, (value & 0x80U) != 0);
        McuClockDelayUs(half_us);
        if(I2cSoftwareSclHigh(port, scl, half_us) != OK) {
            return ERR;
        }
        I2cSoftwareSclLow(port, scl, half_us);
        value <<= 1;
    }
    I2cSoftwareSda(port, sda, TRUE);
    McuClockDelayUs(half_us);
    if(I2cSoftwareSclHigh(port, scl, half_us) != OK) {
        return ERR;
    }
    result = gpio_input_bit_get(port, sda) == RESET ? OK : ERR;
    I2cSoftwareSclLow(port, scl, half_us);
    return result;
}

STATIC DWORD I2cSoftwareReceiveByte(DWORD port, DWORD scl, DWORD sda, BYTE half_us, BYTE *value, BOOL last)
{
    BYTE i, data = 0;
    I2cSoftwareSda(port, sda, TRUE);
    for(i = 0; i < 8; ++i) {
        if(I2cSoftwareSclHigh(port, scl, half_us) != OK) {
            return ERR;
        }
        data = (BYTE)((data << 1) | (gpio_input_bit_get(port, sda) == SET ? 1U : 0U));
        I2cSoftwareSclLow(port, scl, half_us);
    }
    I2cSoftwareSda(port, sda, last);
    McuClockDelayUs(half_us);
    if(I2cSoftwareSclHigh(port, scl, half_us) != OK) {
        return ERR;
    }
    I2cSoftwareSclLow(port, scl, half_us);
    I2cSoftwareSda(port, sda, TRUE);
    *value = data;
    return OK;
}

DWORD I2cBusSoftwareInit(DWORD port, DWORD scl, DWORD sda, I2cSpeed_t speed)
{
    BYTE half_us = I2cSwHalfUs(speed);

    if(half_us == 0U) {
        half_us = 1U;
    }
    rcu_periph_clock_enable(I2cSoftwareRcuPort(port));
    gpio_bit_set(port, scl | sda);
    gpio_output_options_set(port, GPIO_OTYPE_OD, GPIO_OSPEED_2MHZ, scl | sda);
    gpio_mode_set(port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, scl | sda);
    if(I2cSoftwareSclHigh(port, scl, half_us) != OK || gpio_input_bit_get(port, sda) == RESET) {
        return ERR;
    }
    return OK;
}

DWORD I2cBusSoftwareWrite(DWORD port, DWORD scl, DWORD sda, I2cSpeed_t speed, BYTE addr7, CONST BYTE *tx, BYTE txlen)
{
    BYTE i;
    BYTE half_us = I2cSwHalfUs(speed);
    BYTE address = (BYTE)(addr7 << 1);
    if(half_us == 0U) {
        half_us = 1U;
    }
    if(addr7 < 8 || addr7 > 0x77 || tx == NULL || txlen == 0) {
        return ERR;
    }
    if(I2cSoftwareStart(port, scl, sda, half_us) != OK) {
        I2cSoftwareSda(port, sda, TRUE);
        gpio_bit_set(port, scl);
        return ERR;
    }
    if(I2cSoftwareSendByte(port, scl, sda, half_us, address) != OK) {
        goto fail;
    }
    for(i = 0; i < txlen; ++i) {
        if(I2cSoftwareSendByte(port, scl, sda, half_us, tx[i]) != OK) {
            goto fail;
        }
    }
    return I2cSoftwareStop(port, scl, sda, half_us);
fail:
    (void)I2cSoftwareStop(port, scl, sda, half_us);
    return ERR;
}

DWORD I2cBusSoftwareRead(DWORD port, DWORD scl, DWORD sda, I2cSpeed_t speed, BYTE addr7, BYTE *rx, BYTE rxlen)
{
    BYTE i;
    BYTE half_us = I2cSwHalfUs(speed);
    BYTE address = (BYTE)((addr7 << 1) | 1U);
    if(half_us == 0U) {
        half_us = 1U;
    }
    if(addr7 < 8 || addr7 > 0x77 || rx == NULL || rxlen == 0) {
        return ERR;
    }
    if(I2cSoftwareStart(port, scl, sda, half_us) != OK) {
        I2cSoftwareSda(port, sda, TRUE);
        gpio_bit_set(port, scl);
        return ERR;
    }
    if(I2cSoftwareSendByte(port, scl, sda, half_us, address) != OK) {
        goto fail;
    }
    for(i = 0; i < rxlen; ++i) {
        if(I2cSoftwareReceiveByte(port, scl, sda, half_us, &rx[i], i == rxlen - 1U) != OK) {
            goto fail;
        }
    }
    return I2cSoftwareStop(port, scl, sda, half_us);
fail:
    (void)I2cSoftwareStop(port, scl, sda, half_us);
    return ERR;
}

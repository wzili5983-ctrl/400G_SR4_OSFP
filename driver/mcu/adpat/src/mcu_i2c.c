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
#define I2C_NUM_MAX     2U

STATIC BOOL g_I2cReady[I2C_NUM_MAX + 1U];

/* ---------------- hardware I2Cx master (AF4) ---------------- */

STATIC DWORD I2cPeriph(BYTE i2cno)
{
    if(i2cno == 0U) {
        return I2C0;
    }
    if(i2cno == 1U) {
        return I2C1;
    }
    return I2C2;
}

STATIC DWORD I2cRcuClock(BYTE i2cno)
{
    if(i2cno == 0U) {
        return RCU_I2C0;
    }
    if(i2cno == 1U) {
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
    return RCU_GPIOC;
}

STATIC VOID I2cHardwareConfig(BYTE i2cno)
{
    DWORD i2c = I2cPeriph(i2cno);
    i2c_deinit(i2c);
    rcu_i2c_clock_config(i2c, RCU_I2CSRC_IRC8M);
    i2c_timing_config(i2c, 0, 9, 2);
    i2c_master_clock_config(i2c, 39, 39);
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

VOID I2cBusInit(BYTE i2cno, DWORD port, DWORD scl, DWORD sda)
{
    if(i2cno > I2C_NUM_MAX) {
        return;
    }
    rcu_periph_clock_enable(I2cHardwareRcuPort(port));
    rcu_periph_clock_enable(I2cRcuClock(i2cno));
    gpio_af_set(port, GPIO_AF_4, scl | sda);
    gpio_mode_set(port, GPIO_MODE_AF, GPIO_PUPD_NONE, scl | sda);
    gpio_output_options_set(port, GPIO_OTYPE_OD, GPIO_OSPEED_2MHZ, scl | sda);
    I2cHardwareConfig(i2cno);
    g_I2cReady[i2cno] = TRUE;
}

DWORD I2cBusWrite(BYTE i2cno, BYTE addr7, CONST BYTE *tx, BYTE txlen)
{
    BYTE i;
    DWORD i2c;
    if(i2cno > I2C_NUM_MAX || !g_I2cReady[i2cno] || addr7 < 8 || addr7 > 0x77 || tx == NULL || txlen == 0) {
        return ERR;
    }
    i2c = I2cPeriph(i2cno);
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
    if(i2cno > I2C_NUM_MAX || !g_I2cReady[i2cno] || addr7 < 8 || addr7 > 0x77 || rx == NULL || rxlen == 0) {
        return ERR;
    }
    i2c = I2cPeriph(i2cno);
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

STATIC DWORD I2cSoftwareRcuPort(DWORD port)
{
    if(port == GPIOA) {
        return RCU_GPIOA;
    }
    if(port == GPIOB) {
        return RCU_GPIOB;
    }
    return RCU_GPIOC;
}

STATIC VOID I2cSoftwareSda(DWORD port, DWORD sda, BOOL high)
{
    gpio_bit_write(port, sda, high ? SET : RESET);
}

STATIC VOID I2cSoftwareSclLow(DWORD port, DWORD scl)
{
    gpio_bit_reset(port, scl);
    McuClockDelayUs(5);
}

STATIC DWORD I2cSoftwareSclHigh(DWORD port, DWORD scl)
{
    DWORD timeout = 10000;
    gpio_bit_set(port, scl);
    while(gpio_input_bit_get(port, scl) == RESET) {
        if(timeout-- == 0U) {
            return ERR;
        }
        McuClockDelayUs(1);
    }
    McuClockDelayUs(5);
    return OK;
}

STATIC DWORD I2cSoftwareStart(DWORD port, DWORD scl, DWORD sda)
{
    I2cSoftwareSda(port, sda, TRUE);
    McuClockDelayUs(5);
    if(I2cSoftwareSclHigh(port, scl) != OK || gpio_input_bit_get(port, sda) == RESET) {
        return ERR;
    }
    I2cSoftwareSda(port, sda, FALSE);
    McuClockDelayUs(5);
    I2cSoftwareSclLow(port, scl);
    return OK;
}

STATIC DWORD I2cSoftwareStop(DWORD port, DWORD scl, DWORD sda)
{
    I2cSoftwareSclLow(port, scl);
    I2cSoftwareSda(port, sda, FALSE);
    McuClockDelayUs(5);
    if(I2cSoftwareSclHigh(port, scl) != OK) {
        I2cSoftwareSda(port, sda, TRUE);
        return ERR;
    }
    I2cSoftwareSda(port, sda, TRUE);
    McuClockDelayUs(5);
    return OK;
}

STATIC DWORD I2cSoftwareSendByte(DWORD port, DWORD scl, DWORD sda, BYTE value)
{
    BYTE i;
    DWORD result;
    for(i = 0; i < 8; ++i) {
        I2cSoftwareSda(port, sda, (value & 0x80U) != 0);
        McuClockDelayUs(5);
        if(I2cSoftwareSclHigh(port, scl) != OK) {
            return ERR;
        }
        I2cSoftwareSclLow(port, scl);
        value <<= 1;
    }
    I2cSoftwareSda(port, sda, TRUE);
    McuClockDelayUs(5);
    if(I2cSoftwareSclHigh(port, scl) != OK) {
        return ERR;
    }
    result = gpio_input_bit_get(port, sda) == RESET ? OK : ERR;
    I2cSoftwareSclLow(port, scl);
    return result;
}

STATIC DWORD I2cSoftwareReceiveByte(DWORD port, DWORD scl, DWORD sda, BYTE *value, BOOL last)
{
    BYTE i, data = 0;
    I2cSoftwareSda(port, sda, TRUE);
    for(i = 0; i < 8; ++i) {
        if(I2cSoftwareSclHigh(port, scl) != OK) {
            return ERR;
        }
        data = (BYTE)((data << 1) | (gpio_input_bit_get(port, sda) == SET ? 1U : 0U));
        I2cSoftwareSclLow(port, scl);
    }
    I2cSoftwareSda(port, sda, last);
    McuClockDelayUs(5);
    if(I2cSoftwareSclHigh(port, scl) != OK) {
        return ERR;
    }
    I2cSoftwareSclLow(port, scl);
    I2cSoftwareSda(port, sda, TRUE);
    *value = data;
    return OK;
}

DWORD I2cBusSoftwareInit(DWORD port, DWORD scl, DWORD sda)
{
    rcu_periph_clock_enable(I2cSoftwareRcuPort(port));
    gpio_bit_set(port, scl | sda);
    gpio_output_options_set(port, GPIO_OTYPE_OD, GPIO_OSPEED_2MHZ, scl | sda);
    gpio_mode_set(port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, scl | sda);
    if(I2cSoftwareSclHigh(port, scl) != OK || gpio_input_bit_get(port, sda) == RESET) {
        return ERR;
    }
    return OK;
}

DWORD I2cBusSoftwareWrite(DWORD port, DWORD scl, DWORD sda, BYTE addr7, CONST BYTE *tx, BYTE txlen)
{
    BYTE i;
    BYTE address = (BYTE)(addr7 << 1);
    if(addr7 < 8 || addr7 > 0x77 || tx == NULL || txlen == 0) {
        return ERR;
    }
    if(I2cSoftwareStart(port, scl, sda) != OK) {
        I2cSoftwareSda(port, sda, TRUE);
        gpio_bit_set(port, scl);
        return ERR;
    }
    if(I2cSoftwareSendByte(port, scl, sda, address) != OK) {
        goto fail;
    }
    for(i = 0; i < txlen; ++i) {
        if(I2cSoftwareSendByte(port, scl, sda, tx[i]) != OK) {
            goto fail;
        }
    }
    return I2cSoftwareStop(port, scl, sda);
fail:
    (void)I2cSoftwareStop(port, scl, sda);
    return ERR;
}

DWORD I2cBusSoftwareRead(DWORD port, DWORD scl, DWORD sda, BYTE addr7, BYTE *rx, BYTE rxlen)
{
    BYTE i;
    BYTE address = (BYTE)((addr7 << 1) | 1U);
    if(addr7 < 8 || addr7 > 0x77 || rx == NULL || rxlen == 0) {
        return ERR;
    }
    if(I2cSoftwareStart(port, scl, sda) != OK) {
        I2cSoftwareSda(port, sda, TRUE);
        gpio_bit_set(port, scl);
        return ERR;
    }
    if(I2cSoftwareSendByte(port, scl, sda, address) != OK) {
        goto fail;
    }
    for(i = 0; i < rxlen; ++i) {
        if(I2cSoftwareReceiveByte(port, scl, sda, &rx[i], i == rxlen - 1U) != OK) {
            goto fail;
        }
    }
    return I2cSoftwareStop(port, scl, sda);
fail:
    (void)I2cSoftwareStop(port, scl, sda);
    return ERR;
}

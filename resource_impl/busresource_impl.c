/** @file busresource_impl.c
 * @brief Board resource implementation.
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 */
#include "busresource_impl.h"
#include "gd32e511_512.h"
#include "bcm_gd32_i2c.h"
#include "hr_time.h"

STATIC DWORD BoardBusInit(BYTE id);
STATIC DWORD BoardBusTransfer(BYTE id, BYTE address7, CONST BYTE *tx, BYTE txlen, BYTE *rx, BYTE rxlen);

STATIC BOOL g_Ready[BUS_ID_NUM];

STATIC CONST BusInfo_t g_BusInfo[BUS_ID_NUM] = {
    /* PHY/ODSP I2C is connected to MCU PB6 (SCL) and PB7 (SDA). */
    {{BUS_ID_PHY, BUS_I2C_MASTER, GPIOB, GPIO_PIN_6, GPIO_PIN_7, FALSE}, {BoardBusInit, BoardBusTransfer}},
    {{BUS_ID_INNER, BUS_I2C_MASTER, GPIOB, GPIO_PIN_8, GPIO_PIN_9, TRUE}, {BoardBusInit, BoardBusTransfer}},
    /* Slave protocol/address not specified; never drive host pins as a master. */
    {{BUS_ID_HOST, BUS_I2C_SLAVE, GPIOB, GPIO_PIN_13, GPIO_PIN_14, FALSE}, {NULL, NULL}},
};

CONST BusInfo_t *GetBusInfo(BYTE id)
{
    return id < BUS_ID_NUM ? &g_BusInfo[id] : NULL;
}

STATIC VOID Sda(BOOL high)
{
    gpio_bit_write(GPIOB, GPIO_PIN_9, high ? SET : RESET);
}

STATIC VOID SclLow(VOID)
{
    gpio_bit_reset(GPIOB, GPIO_PIN_8);
    delay_us(5);
}

STATIC DWORD SclHigh(VOID)
{
    DWORD timeout = 10000;
    gpio_bit_set(GPIOB, GPIO_PIN_8);
    while(gpio_input_bit_get(GPIOB, GPIO_PIN_8) == RESET) {
        if(timeout-- == 0) {
            return ERR;
        }
        delay_us(1);
    }
    delay_us(5);
    return OK;
}

STATIC DWORD Start(VOID)
{
    Sda(TRUE);
    delay_us(5);
    if(SclHigh() != OK || gpio_input_bit_get(GPIOB, GPIO_PIN_9) == RESET) {
        return ERR;
    }
    Sda(FALSE);
    delay_us(5);
    SclLow();
    return OK;
}

STATIC DWORD Stop(VOID)
{
    SclLow();
    Sda(FALSE);
    delay_us(5);
    if(SclHigh() != OK) {
        Sda(TRUE);
        return ERR;
    }
    Sda(TRUE);
    delay_us(5);
    return OK;
}

STATIC DWORD SendByte(BYTE value)
{
    BYTE i;
    DWORD result;
    for(i = 0; i < 8; ++i) {
        Sda((value & 0x80U) != 0);
        delay_us(5);
        if(SclHigh() != OK) {
            return ERR;
        }
        SclLow();
        value <<= 1;
    }
    Sda(TRUE);
    delay_us(5);
    if(SclHigh() != OK) {
        return ERR;
    }
    result = gpio_input_bit_get(GPIOB, GPIO_PIN_9) == RESET ? OK : ERR;
    SclLow();
    return result;
}

STATIC DWORD ReceiveByte(BYTE *value, BOOL last)
{
    BYTE i, data = 0;
    Sda(TRUE);
    for(i = 0; i < 8; ++i) {
        if(SclHigh() != OK) {
            return ERR;
        }
        data = (BYTE)((data << 1) | (gpio_input_bit_get(GPIOB, GPIO_PIN_9) == SET ? 1U : 0U));
        SclLow();
    }
    Sda(last);
    delay_us(5);
    if(SclHigh() != OK) {
        return ERR;
    }
    SclLow();
    Sda(TRUE);
    *value = data;
    return OK;
}

STATIC DWORD BoardBusInit(BYTE id)
{
    if(id >= BUS_ID_NUM) {
        return ERR;
    }
    if(g_Ready[id]) {
        return OK;
    }
    if(id == BUS_ID_PHY) {
        BcmGd32I2cInit();
    } else if(id == BUS_ID_INNER) {
        rcu_periph_clock_enable(RCU_GPIOB);
        gpio_bit_set(GPIOB, GPIO_PIN_8 | GPIO_PIN_9);
        gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_2MHZ, GPIO_PIN_8 | GPIO_PIN_9);
        gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_8 | GPIO_PIN_9);
        if(SclHigh() != OK || gpio_input_bit_get(GPIOB, GPIO_PIN_9) == RESET) {
            return ERR;
        }
    } else {
        return ERR;
    }
    g_Ready[id] = TRUE;
    return OK;
}

/* Arguments and bus readiness are checked by BoardBusTransfer. */
STATIC DWORD SoftwareI2cTransfer(BYTE address7, CONST BYTE *tx, BYTE txlen, BYTE *rx, BYTE rxlen)
{
    BYTE i;
    BYTE address = (BYTE)(address7 << 1);
    if(Start() != OK) {
        Sda(TRUE);
        gpio_bit_set(GPIOB, GPIO_PIN_8);
        return ERR;
    }
    if(txlen != 0) {
        if(SendByte(address) != OK) {
            goto fail;
        }
        for(i = 0; i < txlen; ++i) {
            if(SendByte(tx[i]) != OK) {
                goto fail;
            }
        }
    }
    if(rxlen != 0) {
        if(txlen != 0 && Start() != OK) {
            goto fail;
        }
        if(SendByte((BYTE)(address | 1U)) != OK) {
            goto fail;
        }
        for(i = 0; i < rxlen; ++i) {
            if(ReceiveByte(&rx[i], i == rxlen - 1U) != OK) {
                goto fail;
            }
        }
    }
    return Stop();
fail:
    (void)Stop();
    return ERR;
}

STATIC DWORD BoardBusTransfer(BYTE id, BYTE address7, CONST BYTE *tx, BYTE txlen, BYTE *rx, BYTE rxlen)
{
    if(id >= BUS_ID_NUM || !g_Ready[id] || address7 < 8 || address7 > 0x77 ||
       (txlen == 0 && rxlen == 0) || (txlen != 0 && tx == NULL) || (rxlen != 0 && rx == NULL)) {
        return ERR;
    }
    switch(id) {
    case BUS_ID_PHY:
        return BcmGd32I2cTransfer(address7, tx, txlen, rx, rxlen) == 0 ? OK : ERR;
    case BUS_ID_INNER:
        return SoftwareI2cTransfer(address7, tx, txlen, rx, rxlen);
    default:
        return ERR;
    }
}

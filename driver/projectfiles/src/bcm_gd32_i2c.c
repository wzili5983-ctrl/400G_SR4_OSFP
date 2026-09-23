/** @file bcm_gd32_i2c.c
 * @brief Polling I2C0 master, bounded waits and repeated START reads.
 * PB6/PB7 AF4 open drain; external pull-ups required. IRC8M clock,
 * conservative timing below 100 kHz. All callers must serialize bus access.
 */
#include "gd32e511_512.h"
#include "bcm_gd32_i2c.h"
#include "bcm_bsc.h"
#include "hr_time.h"
#include <string.h>
#define I2C_WAIT_US 10000U
#define I2C_ERROR_FLAGS (I2C_FLAG_NACK | I2C_FLAG_BERR | I2C_FLAG_LOSTARB | I2C_FLAG_OUERR)
static uint8_t dsp_address = BCM_DSP_I2C_ADDRESS_7BIT;
static BcmDspMemoryTransfer memory_transfer = BcmBscMemoryTransfer;
static int initialized;
static void configure(void)
{
    i2c_deinit(I2C0);
    rcu_i2c_clock_config(I2C0, RCU_I2CSRC_IRC8M);
    i2c_timing_config(I2C0, 0, 9, 2);
    i2c_master_clock_config(I2C0, 39, 39);
    i2c_enable(I2C0);
}
static int wait_flag(uint32_t flag, FlagStatus state)
{
    uint32_t timeout = I2C_WAIT_US;
    do {
        if ((I2C_STAT(I2C0) & I2C_ERROR_FLAGS) != 0) { return -4; }
        if (i2c_flag_get(I2C0, flag) == state) { return 0; }
        delay_us(1);
    } while (--timeout != 0);
    return -5;
}
int BcmGd32I2cTransfer(uint8_t address7, const uint8_t *tx, uint8_t tx_size,
                      uint8_t *rx, uint8_t rx_size)
{
    int result;
    uint32_t i;
    if (address7 < 8 || address7 > 0x77 || (tx_size == 0 && rx_size == 0) ||
        (tx_size != 0 && tx == NULL) || (rx_size != 0 && rx == NULL)) { return BCM_ARM_INVALID; }
    if (!initialized) { return BCM_ARM_NO_BUS; }
    result = wait_flag(I2C_FLAG_I2CBSY, RESET);
    if (result != 0) { goto fail; }
    i2c_flag_clear(I2C0, I2C_FLAG_STPDET | I2C_ERROR_FLAGS);
    i2c_reload_disable(I2C0);
    if (tx_size != 0) {
        i2c_master_addressing(I2C0, (uint32_t)address7 << 1, I2C_MASTER_TRANSMIT);
        i2c_transfer_byte_number_config(I2C0, tx_size);
        if (rx_size != 0) { i2c_automatic_end_disable(I2C0); }
        else { i2c_automatic_end_enable(I2C0); }
        i2c_start_on_bus(I2C0);
        for (i = 0; i < tx_size; ++i) {
            result = wait_flag(I2C_FLAG_TI, SET);
            if (result != 0) { goto fail; }
            i2c_data_transmit(I2C0, tx[i]);
        }
        result = wait_flag(rx_size != 0 ? I2C_FLAG_TC : I2C_FLAG_STPDET, SET);
        if (result != 0) { goto fail; }
    }
    if (rx_size != 0) {
        i2c_master_addressing(I2C0, (uint32_t)address7 << 1, I2C_MASTER_RECEIVE);
        i2c_transfer_byte_number_config(I2C0, rx_size);
        i2c_automatic_end_enable(I2C0);
        i2c_start_on_bus(I2C0);
        for (i = 0; i < rx_size; ++i) {
            result = wait_flag(I2C_FLAG_RBNE, SET);
            if (result != 0) { goto fail; }
            rx[i] = i2c_data_receive(I2C0);
        }
        result = wait_flag(I2C_FLAG_STPDET, SET);
        if (result != 0) { goto fail; }
    }
    i2c_flag_clear(I2C0, I2C_FLAG_STPDET);
    return 0;
fail:
    /* Do not generate STOP after arbitration loss; another master owns the bus. */
    if (i2c_flag_get(I2C0, I2C_FLAG_LOSTARB) == RESET) {
        i2c_stop_on_bus(I2C0);
        delay_us(10);
    }
    configure();
    if (rx != NULL) { memset(rx, 0xFF, rx_size); }
    return result;
}
static int target_address(uint32_t target)
{
    /* Legacy CAPI I2C functions use 7-bit addresses; accept the board's
     * documented 8-bit write address as well, but never shift twice. */
    if (target > 0x7F) {
        if (target > 0xEE || (target & 1U) != 0) { return -1; }
        target >>= 1;
    }
    return target >= 8 && target <= 0x77 ? (int)target : -1;
}
static int read_bus(BcmArmSpace space, uint32_t target, uint32_t address, void *data, uint32_t size)
{
    uint8_t reg = (uint8_t)address;
    int device;
    if (space == BCM_ARM_MEMORY) {
        return memory_transfer != NULL ? memory_transfer(dsp_address, address, data, size, 0) : BCM_ARM_UNSUPPORTED;
    }
    if (space != BCM_ARM_I2C) { return BCM_ARM_UNSUPPORTED; }
    device = target_address(target);
    if (device < 0 || address > 255 || size > 255 || size > 256 - address) { return BCM_ARM_INVALID; }
    return BcmGd32I2cTransfer((uint8_t)device, &reg, 1, data, (uint8_t)size);
}
static int write_bus(BcmArmSpace space, uint32_t target, uint32_t address, const void *data, uint32_t size)
{
    uint8_t buffer[255];
    int device;
    if (space == BCM_ARM_MEMORY) {
        /* Memory callback treats the buffer as read-only for write == 1. */
        return memory_transfer != NULL ? memory_transfer(dsp_address, address, (void *)data, size, 1) : BCM_ARM_UNSUPPORTED;
    }
    if (space != BCM_ARM_I2C) { return BCM_ARM_UNSUPPORTED; }
    device = target_address(target);
    if (device < 0 || address > 255 || size > 254 || size > 256 - address) { return BCM_ARM_INVALID; }
    buffer[0] = (uint8_t)address;
    memcpy(buffer + 1, data, size);
    return BcmGd32I2cTransfer((uint8_t)device, buffer, (uint8_t)(size + 1), NULL, 0);
}
void BcmGd32SetMemoryTransfer(BcmDspMemoryTransfer transfer) { memory_transfer = transfer; }
void BcmGd32SetTarget(uint8_t address7)
{
    if (address7 < 8 || address7 > 0x77) { BcmArmSetError(BCM_ARM_INVALID); return; }
    dsp_address = address7;
    BcmArmSetError(0);
}
void BcmGd32I2cInit(void)
{
    const BcmArmBus bus = { read_bus, write_bus };
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_I2C0);
    gpio_af_set(GPIOB, GPIO_AF_4, GPIO_PIN_6 | GPIO_PIN_7);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6 | GPIO_PIN_7);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_2MHZ, GPIO_PIN_6 | GPIO_PIN_7);
    configure();
    initialized = 1;
    BcmArmSetBus(&bus);
}

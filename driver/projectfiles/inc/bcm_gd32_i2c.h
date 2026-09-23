/** @file bcm_gd32_i2c.h @brief GD32E512ME I2C0 on PB6 SCL / PB7 SDA. */
#ifndef BCM_GD32_I2C_H
#define BCM_GD32_I2C_H
#include "bcm_arm_platform.h"
#define BCM_DSP_I2C_ADDRESS_7BIT 0x50U /* DS103 default: 0x50; alternate hardware address: 0x51. */
/* DSP memory translation must follow the chip's indirect-access protocol.
 * It can use BcmGd32I2cTransfer; return 0 only after a successful transaction.
 * DS103 BSC indirect access is registered by default. */
typedef int (*BcmDspMemoryTransfer)(uint8_t address7, uint32_t address,
                                  void *data, uint32_t size, int write);
void BcmGd32I2cInit(void);
void BcmGd32SetMemoryTransfer(BcmDspMemoryTransfer transfer);
void BcmGd32SetTarget(uint8_t address7);
int BcmGd32I2cTransfer(uint8_t address7, const uint8_t *tx, uint8_t tx_size,
                      uint8_t *rx, uint8_t rx_size);
#endif

/** @file bcm_bsc.c
 * @brief BCM87420-DS103 sections 2.1.1-2.1.3, pages 7-10.
 * Select page FF, program address/length little endian, control 03 write / 01
 * read, stream WFIFO 87 / RFIFO 90. Chunking keeps MCU NBYTES below 256;
 * each chunk gets its own complete indirect command and incremented address.
 */
#include "bcm_bsc.h"
#include "bcm_gd32_i2c.h"
#include <string.h>
int BcmBscMemoryTransfer(uint8_t address7, uint32_t address, void *data, uint32_t size, int write)
{
    const uint8_t page[] = {0x7F, 0xFF};
    uint8_t packet[248];
    uint8_t *bytes = data;
    int result;
    if (data == NULL || size == 0 || size - 1 > UINT32_MAX - address ||
        (write != 0 && write != 1)) { return BCM_ARM_INVALID; }
    result = BcmGd32I2cTransfer(address7, page, sizeof(page), NULL, 0);
    if (result != 0) { return result; }
    while (size != 0) {
        uint8_t count = (uint8_t)(size > 240U ? 240U : size);
        packet[0] = 0x80;
        packet[1] = (uint8_t)address;
        packet[2] = (uint8_t)(address >> 8);
        packet[3] = (uint8_t)(address >> 16);
        packet[4] = (uint8_t)(address >> 24);
        packet[5] = count;
        packet[6] = 0;
        packet[7] = write ? 0x03 : 0x01;
        if (write) {
            memcpy(packet + 8, bytes, count);
            result = BcmGd32I2cTransfer(address7, packet, (uint8_t)(count + 8U), NULL, 0);
        } else {
            const uint8_t fifo = 0x90;
            result = BcmGd32I2cTransfer(address7, packet, 8, NULL, 0);
            if (result == 0) { result = BcmGd32I2cTransfer(address7, &fifo, 1, bytes, count); }
        }
        if (result != 0) { return result; }
        size -= count;
        bytes += count;
        if (size != 0) { address += count; }
    }
    return 0;
}

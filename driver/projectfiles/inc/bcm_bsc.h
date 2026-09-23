/** @file bcm_bsc.h @brief BCM87420 DS103 section 2.1 BSC indirect access. */
#ifndef BCM_BSC_H
#define BCM_BSC_H
#include <stdint.h>
int BcmBscMemoryTransfer(uint8_t address7, uint32_t address, void *data, uint32_t size, int write);
#endif

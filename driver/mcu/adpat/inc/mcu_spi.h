/** @file mcu_spi.h
 * @brief MCU SPI master: hardware SPIx via firmware library and software bit-bang.
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * Both masters transfer 8-bit frames, MSB first, SPI mode 0 (CPOL=0, CPHA=0).
 * Hardware master: SPI0..SPI3 selected by spino (APB2 or APB1 /16 = 5 MHz),
 * pins must map to the SPI AF5 pins of the selected peripheral. Chip select
 * is NOT handled here: the caller drives the CS GPIO before and after each
 * write/read pair. All callers must serialize bus access.
 */
#ifndef MCU_SPI_H
#define MCU_SPI_H

#include "basedef.h"

VOID SpiBusInit(BYTE spino, DWORD port, DWORD sck, DWORD mosi, DWORD miso);
DWORD SpiBusWrite(BYTE spino, CONST BYTE *tx, BYTE txlen);
DWORD SpiBusRead(BYTE spino, BYTE *rx, BYTE rxlen);

DWORD SpiBusSoftwareInit(DWORD port, DWORD sck, DWORD mosi, DWORD miso);
DWORD SpiBusSoftwareWrite(DWORD port, DWORD sck, DWORD mosi, DWORD miso, CONST BYTE *tx, BYTE txlen);
DWORD SpiBusSoftwareRead(DWORD port, DWORD sck, DWORD mosi, DWORD miso, BYTE *rx, BYTE rxlen);

#endif

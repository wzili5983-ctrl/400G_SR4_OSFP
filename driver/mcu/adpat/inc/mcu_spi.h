/** @file mcu_spi.h
 * @brief MCU SPI master: hardware SPIx via firmware library and software bit-bang.
 * @author Firmware team
 * @copyright 2026 Terabitcom, Inc.
 *
 * Both masters transfer 8-bit frames, MSB first, SPI mode 0 (CPOL=0, CPHA=0).
 * Hardware master: instance/pin mux per SpiNum_t from the per-instance table
 * g_SpiHwInfo in mcu_spi.c; the SPI kernel clock is PCLK1 = 80 MHz (APB1
 * undivided) and the speed class picks the closest power-of-2 prescaler:
 *   SPI_SPEED_STD  = 1 MHz  class -> PSC/64 = 1.25 MHz real
 *   SPI_SPEED_FAST = 4 MHz  class -> PSC/16 = 5 MHz real
 *   SPI_SPEED_HIGH = 10 MHz class -> PSC/8  = 10 MHz exact
 * Chip select is NOT handled here: the caller drives the CS GPIO before and
 * after each write/read pair; on a shared bus (SPI_2, the U15 boot flash)
 * the caller pairs SpiBusInit with SpiBusRelease to tri-state the nets for
 * the peer master (the DSP) afterwards. The software bit-bang master runs a
 * fixed ~333 kbit/s (1 us delays) regardless of class — below even the
 * 1 MHz STD class, so speed only selects the hardware prescaler. All
 * callers must serialize bus access.
 */
#ifndef MCU_SPI_H
#define MCU_SPI_H

#include "basedef.h"

/* Hardware SPIx instance IDs — also the index of the per-instance pin table
 * g_SpiHwInfo in mcu_spi.c. SPI_2 is the U15 boot-flash bus (PG9/PG11/PG10,
 * shared with the DSP SPI master); SPI_0/SPI_1/SPI_3 have no consumer on
 * this board. */
typedef enum {
    SPI_0 = 0,
    SPI_1,
    SPI_2,
    SPI_3,
    SPI_NUM
}SpiNum_t;

/* Bus rate classes for the hardware master (nominal vs real see file
 * header; the software bit-bang master is class-independent). */
typedef enum {
    SPI_SPEED_STD = 0,   /* 1 MHz class */
    SPI_SPEED_FAST,      /* 4 MHz class */
    SPI_SPEED_HIGH,      /* 10 MHz class */
    SPI_SPEED_NUM
}SpiSpeed_t;

VOID SpiBusInit(BYTE spino, SpiSpeed_t speed);
/* Tri-state SCK/MOSI/MISO (GPIO input) so a bus peer (e.g. the DSP SPI
 * master on the shared U15 boot-flash bus) can own the nets. */
VOID SpiBusRelease(BYTE spino);
DWORD SpiBusWrite(BYTE spino, CONST BYTE *tx, BYTE txlen);
DWORD SpiBusRead(BYTE spino, BYTE *rx, BYTE rxlen);

DWORD SpiBusSoftwareInit(DWORD port, DWORD sck, DWORD mosi, DWORD miso);
DWORD SpiBusSoftwareWrite(DWORD port, DWORD sck, DWORD mosi, DWORD miso, CONST BYTE *tx, BYTE txlen);
DWORD SpiBusSoftwareRead(DWORD port, DWORD sck, DWORD mosi, DWORD miso, BYTE *rx, BYTE rxlen);

#endif

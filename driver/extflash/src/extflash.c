/** @file extflash.c
 * @brief U15 (SST25PF040C, 4 Mbit / 512 KB SPI NOR) driver implementation.
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * Part command set (SST25PF040C, Microchip DS20005397F): WREN 06h / WRDI 04h
 * / RDSR 05h / WRSR 01h (WREN-gated, one data byte) / READ 03h (max 25 MHz)
 * / Page-Program 02h (1-256 B, wraps inside the 256 B page) / Sector-Erase
 * 20h (4 KB; D7h is a listed alias) / Block-Erase D8h (64 KB, the only block
 * size — 32 KB 52h does NOT exist on this part) / Chip-Erase 60h (C7h
 * alias) / RDID 9Fh -> 62h 06h 13h 00h (4 bytes, repeats until CS high;
 * legacy 90h Read-ID also absent). Status register: BUSY bit0, WEL bit1,
 * BP0-BP2 bits 2-4, TB bit5 (top/bottom select), RES bit6, BPL bit7.
 * BP0-BP2/TB/BPL are NON-VOLATILE bits — the power-up protection level is
 * the last programmed value (datasheet default "0 or 1"), never a fixed
 * all-protected state — so init reads RDSR and clears BP/TB only when
 * non-zero (WP# low with BPL=1 would block the clear; best-effort here).
 * Timing (Table 6-8): TPP typ 4 / max 5 ms; TSE typ 40 / max 150 ms; TBE
 * typ 80 / max 250 ms; TCE typ 250 ms / max 2 s (no erase suspend exists
 * on this part); TWRSR max 10-15 ms. Host must wait TPU >= 100 us after
 * power-good before the first instruction — trivially met here, init runs
 * seconds after power-up. 512 KB -> 24-bit addresses, 128 x 4 KB sectors,
 * 256-byte pages. Blocking, thread context only, caller serializes. NOTE:
 * a chip erase can hold the bus up to 2 s — callers on the upgrade path
 * must outlive that in their watchdog/polling budget.
 *
 * Shared-bus discipline (2026-10-16): SCK/MOSI/MISO and the CS net are shared
 * with the DSP (U11) SPI master — the DSP boots its microcode from U15 over
 * these nets. The MCU therefore owns NOTHING permanently: every public API
 * acquires the bus (CS pin -> output high, SPI2 -> master AF) for the
 * duration of the call and releases it afterwards (CS pin -> Hi-Z input,
 * SCK/MOSI/MISO -> Hi-Z input via SpiBusRelease). The DSP may be held in
 * reset or have finished booting whenever these functions run.
 */
#include "extflash.h"
#include "chipresource.h"
#include "busresource.h"
#include "busresource_impl.h"
#include "gpioresource.h"
#include "gpioresource_impl.h"
#include "mcu_spi.h"
#include "mcu_clock.h"
#include "gd32e511_512.h"

/* U15 geometry (SST25PF040C: 4 Mbit = 512 KB). */
#define EXTFLASH_SIZE         0x80000UL                 /* 512 KB */
#define EXTFLASH_ADDR_MAX     (EXTFLASH_SIZE - 1U)
#define EXTFLASH_PAGE_SIZE    256U
#define EXTFLASH_SECTOR_SIZE  0x1000UL                  /* 4 KB, SE 20h */
#define EXTFLASH_BLOCK64_SIZE 0x10000UL                 /* 64 KB, BE D8h */

/* SST25PF040C opcodes. */
#define EXTFLASH_CMD_WREN     0x06U
#define EXTFLASH_CMD_WRDI     0x04U
#define EXTFLASH_CMD_RDSR     0x05U
#define EXTFLASH_CMD_WRSR     0x01U
#define EXTFLASH_CMD_READ     0x03U
#define EXTFLASH_CMD_PP       0x02U
#define EXTFLASH_CMD_SE       0x20U
#define EXTFLASH_CMD_BE64     0xD8U
#define EXTFLASH_CMD_CE       0x60U
#define EXTFLASH_CMD_RDID     0x9FU

/* Status register bits (RDSR, Table 4-2). */
#define EXTFLASH_SR_WIP       0x01U                     /* write in progress (RO) */
#define EXTFLASH_SR_WEL       0x02U                     /* write enable latch (RO) */
#define EXTFLASH_SR_BP_MASK   0x1CU                     /* BP2:BP0 bits 4:2 (NVRAM) */
#define EXTFLASH_SR_TB        0x20U                     /* top/bottom protect select (NVRAM) */
#define EXTFLASH_SR_BPL       0x80U                     /* BP/TB lock-down, armed by WP# low (NVRAM) */

/* WIP poll budgets, generous over the datasheet typ/max (see header). */
#define EXTFLASH_PP_TIMEOUT_MS   10U                    /* page program */
#define EXTFLASH_SE_TIMEOUT_MS   400U                   /* 4 KB sector erase */
#define EXTFLASH_BE_TIMEOUT_MS   800U                   /* 64 KB block erase */
#define EXTFLASH_CE_TIMEOUT_MS   3000U                  /* chip erase */
#define EXTFLASH_WRSR_TIMEOUT_MS 40U                    /* status register write */
#define EXTFLASH_RD_CHUNK        240U                   /* <= 240B tx/rx per CS */
#define EXTFLASH_PP_CHUNK        240U                   /* SpiBusWrite len is BYTE; never pass 256 */

STATIC DWORD s_Jedec;                   /* 0 = RDID never ran / failed */
STATIC CONST SpiForm_t *s_Form;         /* BUS_ID_EXTFLASH row (flash table) */
STATIC SpiSpeed_t s_Speed;              /* bus-level rate class (prescaler) */
STATIC DWORD s_CsPort, s_CsPin;         /* CS net taken from the GPIO resource */

/* Take the shared bus: CS net becomes an output, released (high); SPI2
 * becomes the AF master at the bus speed class. */
STATIC DWORD ExtFlashAcquire(VOID)
{
    if(s_Form == NULL) {
        return ERR;
    }
    gpio_bit_set(s_CsPort, s_CsPin);
    gpio_output_options_set(s_CsPort, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, s_CsPin);
    gpio_mode_set(s_CsPort, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, s_CsPin);
    /* Pin mux (PG9/PG11/PG10 and the AF number) comes from the driver's
     * per-instance table g_SpiHwInfo (mcu_spi.c); the speed class selects
     * the prescaler. SPI2-on-GPIOG AF = 0 (user-confirmed 2026-10-16). */
    SpiBusInit(s_Form->spino, s_Speed);
    return OK;
}

/* Hand the bus back to the peer: CS and SCK/MOSI/MISO all Hi-Z inputs. */
STATIC VOID ExtFlashRelease(VOID)
{
    if(s_Form == NULL) {
        return;
    }
    gpio_mode_set(s_CsPort, GPIO_MODE_INPUT, GPIO_PUPD_NONE, s_CsPin);
    SpiBusRelease(s_Form->spino);
}

/* One CS session: optional command/address bytes out, then optional data in. */
STATIC DWORD ExtFlashSession(CONST BYTE *tx, BYTE txlen, BYTE *rx, BYTE rxlen)
{
    DWORD result = OK;

    gpio_bit_reset(s_CsPort, s_CsPin);
    if(tx != NULL && txlen > 0U) {
        result = SpiBusWrite(s_Form->spino, tx, txlen);
    }
    if(result == OK && rx != NULL && rxlen > 0U) {
        result = SpiBusRead(s_Form->spino, rx, rxlen);
    }
    gpio_bit_set(s_CsPort, s_CsPin);
    return result;
}

/* RDSR poll until WIP clears. Erase/program blocking budget: caller context. */
STATIC DWORD ExtFlashWaitReady(DWORD timeout_ms)
{
    BYTE sr;
    BYTE cmd = EXTFLASH_CMD_RDSR;
    DWORD elapsed = 0U;

    do {
        if(ExtFlashSession(&cmd, 1U, &sr, 1U) != OK) {
            return ERR;
        }
        if((sr & EXTFLASH_SR_WIP) == 0U) {
            return OK;
        }
        McuClockDelayMs(1U);
    } while(++elapsed < timeout_ms);
    return ERR;
}

STATIC DWORD ExtFlashWriteEnable(VOID)
{
    BYTE cmd = EXTFLASH_CMD_WREN;

    return ExtFlashSession(&cmd, 1U, NULL, 0U);
}

/* Clear block protection when the non-volatile BP/TB bits are non-zero
 * (their power-up value is the last programmed one, indeterminate on a
 * fresh part). WRSR is WREN-gated on this part (WREN session, then the
 * WRSR session with exactly one status byte — a WRSR spanning two or more
 * bus cycles is not recognized). Board wiring fact (2026-10-16): U15's
 * WP# and HOLD# are both tied to 3.3 V externally, so the status register
 * is always writable (the BPL lock-down can never arm) and HOLD# never
 * pauses a sequence — the clear below always takes effect. */
STATIC DWORD ExtFlashUnprotect(VOID)
{
    BYTE sr;
    BYTE cmd = EXTFLASH_CMD_RDSR;
    BYTE wrsr[2];
    DWORD result;

    result = ExtFlashSession(&cmd, 1U, &sr, 1U);
    if(result != OK || (sr & (EXTFLASH_SR_BP_MASK | EXTFLASH_SR_TB)) == 0U) {
        return result;
    }
    if(ExtFlashWriteEnable() != OK) {
        return ERR;
    }
    wrsr[0] = EXTFLASH_CMD_WRSR;
    wrsr[1] = 0x00U;
    result = ExtFlashSession(wrsr, (BYTE)sizeof(wrsr), NULL, 0U);
    if(result == OK) {
        result = ExtFlashWaitReady(EXTFLASH_WRSR_TIMEOUT_MS);
    }
    return result;
}

DWORD InitExtFlash(BYTE rsttype)
{
    CONST BusInfo_t *info = GetBusInfo(BUS_ID_EXTFLASH);
    CONST GpioInfo_t *cs;

    (void)rsttype;
    if(info == NULL) {
        return ERR;
    }
    s_Form = &info->bustotalform.busform.spi;
    /* The rate class lives at the BusTotalForm_t level (BUS_ID_EXTFLASH
     * row); BusSpeed_t is positionally identical to the driver's
     * SpiSpeed_t. */
    s_Speed = (SpiSpeed_t)info->bustotalform.speed;
    /* CS port/pin come from the GPIO resource row (kept Hi-Z there: the
     * resource layer never drives it, the driver does, per session). */
    cs = GetGpioInfo(GPIO_ID_EXTFLASH_CS);
    if(cs == NULL) {
        return ERR;
    }
    s_CsPort = cs->gpioform.port;
    s_CsPin = cs->gpioform.pin;
    /* Unprotect + RDID are informational: this chip init runs after InitDsp
     * in the InitChip chain, so a responding DSP already proves U15
     * delivered a boot image; an absent/unknown part must not fault the
     * module here. Failures leave s_Jedec = 0 and the protection as-is.
     * Every acquire/release leaves the shared bus Hi-Z for DSP re-boots.
     * (TPU >= 100 us after power-good before first instruction — trivially
     * met, this chain runs seconds after power-up.) */
    if(ExtFlashAcquire() == OK) {
        (void)ExtFlashUnprotect();
        ExtFlashRelease();
    }
    (void)ExtFlashJedecGet(&s_Jedec);
    return OK;
}

DWORD ExtFlashRead(BYTE id, DWORD addr, DWORD *data)
{
    BYTE tx[4];
    BYTE rx[4];
    BYTE i;
    DWORD result;

    if(id != CHIP_ID_EXTFLASH || data == NULL || addr > EXTFLASH_ADDR_MAX
       || (addr & 0x3U) != 0U) {
        return ERR;
    }
    if(ExtFlashAcquire() != OK) {
        return ERR;
    }
    tx[0] = EXTFLASH_CMD_READ;
    tx[1] = (BYTE)(addr >> 16);
    tx[2] = (BYTE)(addr >> 8);
    tx[3] = (BYTE)addr;
    result = ExtFlashSession(tx, (BYTE)sizeof(tx), rx, (BYTE)sizeof(rx));
    ExtFlashRelease();
    if(result != OK) {
        return ERR;
    }
    /* Little-endian word, LSB first on the wire (DspRead convention). */
    *data = 0U;
    for(i = 0U; i < 4U; i++) {
        *data |= (DWORD)rx[i] << (8U * i);
    }
    return OK;
}

DWORD ExtFlashWrite(BYTE id, DWORD addr, DWORD data)
{
    BYTE tx[8];
    BYTE i;
    DWORD result;

    if(id != CHIP_ID_EXTFLASH || addr > EXTFLASH_ADDR_MAX
       || (addr & 0x3U) != 0U) {
        return ERR;
    }
    if(ExtFlashAcquire() != OK) {
        return ERR;
    }
    /* A 4-byte word program never crosses a 256B page boundary when aligned. */
    if(ExtFlashWriteEnable() != OK) {
        ExtFlashRelease();
        return ERR;
    }
    tx[0] = EXTFLASH_CMD_PP;
    tx[1] = (BYTE)(addr >> 16);
    tx[2] = (BYTE)(addr >> 8);
    tx[3] = (BYTE)addr;
    for(i = 0U; i < 4U; i++) {
        tx[4U + i] = (BYTE)(data >> (8U * i));
    }
    result = ExtFlashSession(tx, (BYTE)sizeof(tx), NULL, 0U);
    if(result == OK) {
        result = ExtFlashWaitReady(EXTFLASH_PP_TIMEOUT_MS);
    }
    ExtFlashRelease();
    return result;
}

DWORD ExtFlashJedecGet(DWORD *jedec)
{
    BYTE tx = EXTFLASH_CMD_RDID;
    BYTE rx[3];
    DWORD result;

    if(jedec == NULL || ExtFlashAcquire() != OK) {
        return ERR;
    }
    result = ExtFlashSession(&tx, 1U, rx, (BYTE)sizeof(rx));
    ExtFlashRelease();
    if(result != OK) {
        return ERR;
    }
    *jedec = ((DWORD)rx[0] << 16) | ((DWORD)rx[1] << 8) | (DWORD)rx[2];
    return OK;
}

DWORD ExtFlashDataRead(DWORD addr, BYTE *buf, WORD len)
{
    BYTE tx[4];
    DWORD result = OK;

    if(buf == NULL || len == 0U || (DWORD)addr + len - 1U > EXTFLASH_ADDR_MAX
       || ExtFlashAcquire() != OK) {
        return ERR;
    }
    tx[0] = EXTFLASH_CMD_READ;
    tx[1] = (BYTE)(addr >> 16);
    tx[2] = (BYTE)(addr >> 8);
    tx[3] = (BYTE)addr;
    gpio_bit_reset(s_CsPort, s_CsPin);
    if(SpiBusWrite(s_Form->spino, tx, (BYTE)sizeof(tx)) != OK) {
        result = ERR;
    }
    while(result == OK && len != 0U) {
        WORD chunk = len > EXTFLASH_RD_CHUNK ? EXTFLASH_RD_CHUNK : len;

        if(SpiBusRead(s_Form->spino, buf, (BYTE)chunk) != OK) {
            result = ERR;
            break;
        }
        buf += chunk;
        len = (WORD)(len - chunk);
    }
    gpio_bit_set(s_CsPort, s_CsPin);
    ExtFlashRelease();
    return result;
}

DWORD ExtFlashProgram(DWORD addr, CONST BYTE *buf, WORD len)
{
    BYTE tx[4];
    WORD page;
    DWORD result = OK;

    if(buf == NULL || len == 0U || (DWORD)addr + len - 1U > EXTFLASH_ADDR_MAX
       || ExtFlashAcquire() != OK) {
        return ERR;
    }
    while(len != 0U && result == OK) {
        /* PP wraps inside the 256B page, so never cross a page boundary. */
        page = (WORD)(EXTFLASH_PAGE_SIZE - (addr & (EXTFLASH_PAGE_SIZE - 1U)));
        if(page > len) {
            page = len;
        }
        if(page > EXTFLASH_PP_CHUNK) {
            page = EXTFLASH_PP_CHUNK;
        }
        if(ExtFlashWriteEnable() != OK) {
            result = ERR;
            break;
        }
        tx[0] = EXTFLASH_CMD_PP;
        tx[1] = (BYTE)(addr >> 16);
        tx[2] = (BYTE)(addr >> 8);
        tx[3] = (BYTE)addr;
        gpio_bit_reset(s_CsPort, s_CsPin);
        if(SpiBusWrite(s_Form->spino, tx, (BYTE)sizeof(tx)) != OK
           || SpiBusWrite(s_Form->spino, buf, (BYTE)page) != OK) {
            result = ERR;
        }
        gpio_bit_set(s_CsPort, s_CsPin);
        if(result == OK) {
            result = ExtFlashWaitReady(EXTFLASH_PP_TIMEOUT_MS);
        }
        addr += page;
        buf += page;
        len = (WORD)(len - page);
    }
    ExtFlashRelease();
    return result;
}

DWORD ExtFlashEraseSector(DWORD addr)
{
    BYTE tx[4];
    DWORD result;

    if(addr > EXTFLASH_ADDR_MAX || (addr & (EXTFLASH_SECTOR_SIZE - 1U)) != 0U
       || ExtFlashAcquire() != OK) {
        return ERR;
    }
    if(ExtFlashWriteEnable() != OK) {
        ExtFlashRelease();
        return ERR;
    }
    tx[0] = EXTFLASH_CMD_SE;
    tx[1] = (BYTE)(addr >> 16);
    tx[2] = (BYTE)(addr >> 8);
    tx[3] = (BYTE)addr;
    result = ExtFlashSession(tx, (BYTE)sizeof(tx), NULL, 0U);
    if(result == OK) {
        result = ExtFlashWaitReady(EXTFLASH_SE_TIMEOUT_MS);
    }
    ExtFlashRelease();
    return result;
}

DWORD ExtFlashEraseBlock64K(DWORD addr)
{
    BYTE tx[4];
    DWORD result;

    if(addr > EXTFLASH_ADDR_MAX || (addr & (EXTFLASH_BLOCK64_SIZE - 1U)) != 0U
       || ExtFlashAcquire() != OK) {
        return ERR;
    }
    if(ExtFlashWriteEnable() != OK) {
        ExtFlashRelease();
        return ERR;
    }
    tx[0] = EXTFLASH_CMD_BE64;
    tx[1] = (BYTE)(addr >> 16);
    tx[2] = (BYTE)(addr >> 8);
    tx[3] = (BYTE)addr;
    result = ExtFlashSession(tx, (BYTE)sizeof(tx), NULL, 0U);
    if(result == OK) {
        result = ExtFlashWaitReady(EXTFLASH_BE_TIMEOUT_MS);
    }
    ExtFlashRelease();
    return result;
}

DWORD ExtFlashChipErase(VOID)
{
    BYTE tx = EXTFLASH_CMD_CE;
    DWORD result;

    if(ExtFlashAcquire() != OK) {
        return ERR;
    }
    if(ExtFlashWriteEnable() != OK) {
        ExtFlashRelease();
        return ERR;
    }
    result = ExtFlashSession(&tx, 1U, NULL, 0U);
    if(result == OK) {
        result = ExtFlashWaitReady(EXTFLASH_CE_TIMEOUT_MS);
    }
    ExtFlashRelease();
    return result;
}

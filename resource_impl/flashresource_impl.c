/***************************************************************************/ /**
 *   @file   flashresource_impl.c
 *   @brief  Implementation of flash resource.
 *   @author Oliver Wang (zili.wang@terabitcom.com)
********************************************************************************
 * Copyright 2024~2026(c) Terabitcom, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of Terabitcom, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Terabitcom, INC. “AS IS” AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL Terabitcom, INC. BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/
#include "flashresource_impl.h"
#include "gd32e511_512.h"

STATIC DWORD BoardFlashRead(BYTE id, DWORD offset, BYTE *data, DWORD len);
STATIC DWORD BoardFlashWrite(BYTE id, DWORD offset, CONST BYTE *data, DWORD len);
STATIC DWORD BoardFlashErase(BYTE id);
STATIC DWORD BoardFlashVerifyHdr(BYTE id);
STATIC DWORD BoardFlashCrc32(CONST VOID *data, DWORD len, DWORD *crc32);

/* GD32E512 flash is programmed 64-bit at a time; offset/len must be 8-byte aligned. */
#define FLASH_PROGRAM_UNIT   8U

STATIC BOOL g_CrcReady;

/* Expected frame magic template for the user-upgradeable regions
 * (VerifyHdr reference). */
STATIC CONST FrameHead_t g_FrameTemplate = {0U, 0U, 0U, 0U, FRAME_COMPANY};

/* The 11 memory_map.h flash regions in address order; attributes per FlashForm_t. */
STATIC CONST FlashInfo_t g_FlashInfoImpl[FLASH_ID_NUM] = {
    {{FLASH_ID_BOOTLOADER, FLASH_TYPE_BOOT,     0,  4, MEM_BOOT_BASE,        MEM_BOOT_SIZE,       TRUE},  {BoardFlashRead, BoardFlashWrite, BoardFlashErase, BoardFlashVerifyHdr}},
    {{FLASH_ID_KEY,        FLASH_TYPE_KEY,      4,  1, MEM_KEY_BASE,         MEM_KEY_SIZE,        TRUE},  {BoardFlashRead, BoardFlashWrite, BoardFlashErase, BoardFlashVerifyHdr}},
    {{FLASH_ID_METADATA_A, FLASH_TYPE_META,     5,  1, MEM_OTA_META_A_BASE,  MEM_OTA_META_SIZE,   FALSE}, {BoardFlashRead, BoardFlashWrite, BoardFlashErase, BoardFlashVerifyHdr}},
    {{FLASH_ID_METADATA_B, FLASH_TYPE_META,     6,  1, MEM_OTA_META_B_BASE,  MEM_OTA_META_SIZE,   FALSE}, {BoardFlashRead, BoardFlashWrite, BoardFlashErase, BoardFlashVerifyHdr}},
    {{FLASH_ID_BLACKBOX,   FLASH_TYPE_BLACKBOX, 7,  1, MEM_BLACKBOX_BASE,    MEM_BLACKBOX_SIZE,   FALSE}, {BoardFlashRead, BoardFlashWrite, BoardFlashErase, BoardFlashVerifyHdr}},
    {{FLASH_ID_APP_A,      FLASH_TYPE_APP,      8, 23, MEM_APP_A_BASE,       MEM_APP_SLOT_SIZE,   FALSE}, {BoardFlashRead, BoardFlashWrite, BoardFlashErase, BoardFlashVerifyHdr}},
    {{FLASH_ID_APP_B,      FLASH_TYPE_APP,     31, 23, MEM_APP_B_BASE,       MEM_APP_SLOT_SIZE,   FALSE}, {BoardFlashRead, BoardFlashWrite, BoardFlashErase, BoardFlashVerifyHdr}},
    {{FLASH_ID_PARA_A,     FLASH_TYPE_PARAM,   54,  4, MEM_PARAM_MAIN_BASE,  MEM_PARAM_AREA_SIZE, FALSE}, {BoardFlashRead, BoardFlashWrite, BoardFlashErase, BoardFlashVerifyHdr}},
    {{FLASH_ID_PARA_B,     FLASH_TYPE_PARAM,   58,  4, MEM_PARAM_BACKUP_BASE,MEM_PARAM_AREA_SIZE, FALSE}, {BoardFlashRead, BoardFlashWrite, BoardFlashErase, BoardFlashVerifyHdr}},
    {{FLASH_ID_MISC_A,     FLASH_TYPE_MISC,    62,  1, MEM_MISC_A_BASE,      MEM_MISC_SIZE,       FALSE}, {BoardFlashRead, BoardFlashWrite, BoardFlashErase, BoardFlashVerifyHdr}},
    {{FLASH_ID_MISC_B,     FLASH_TYPE_MISC,    63,  1, MEM_MISC_B_BASE,      MEM_MISC_SIZE,       FALSE}, {BoardFlashRead, BoardFlashWrite, BoardFlashErase, BoardFlashVerifyHdr}},
};

CONST FlashInfo_t *GetFlashInfo(BYTE id)
{
    if(id >= FLASH_ID_NUM) {
        return NULL;
    }

    return &g_FlashInfoImpl[id];
}

CONST FrameHead_t *GetFrameHead(BYTE id)
{
    CONST FlashInfo_t *info = GetFlashInfo(id);

    if(info == NULL || !FLASH_TYPE_HAS_FRAME(info->flashform.type)) {
        return NULL;
    }

    /* Flash is memory-mapped (XIP); the frame header is read in place. */
    return (CONST FrameHead_t *)info->flashform.base;
}

/* FrameHead_t.company is a 16B 0xFF-padded ASCII slot; compare only the
 * actual FRAME_COMPANY string, not the padding. */
STATIC BOOL FrameMagicMatch(CONST FrameHead_t *hdr)
{
    BYTE i;

    for(i = 0U; i < (BYTE)(sizeof(FRAME_COMPANY) - 1U); i++) {
        if(hdr->company[i] != g_FrameTemplate.company[i]) {
            return FALSE;
        }
    }

    return TRUE;
}

/* Which frame header a slot may carry, per the fixed-A / mirror-B scheme.
 * Slot A (the fixed running region) only accepts a header that names A.
 * Slot B is the A-mirror region and the SOLE upgrade entry: every package
 * received into B must be A-named (flashid FLASH_ID_APP_A, storeaddr
 * MEM_APP_A_BASE) and byte-identical to the slot A image -- the B-named
 * package no longer exists, and even the factory / blank-device upgrade
 * goes through B first. Every other framed region only accepts a header
 * naming itself. */
STATIC BOOL FrameTargetsSlot(CONST FrameHead_t *hdr, CONST FlashInfo_t *info)
{
    BYTE id = info->flashform.flashid;

    if(id == FLASH_ID_APP_B) {
        return (BOOL)(hdr->flashid == FLASH_ID_APP_A && hdr->storeaddr == MEM_APP_A_BASE);
    }

    return (BOOL)(hdr->flashid == id && hdr->storeaddr == info->flashform.base);
}

/* On-chip CRC peripheral: poly 0x04C11DB7, init 0xFFFFFFFF, no reflection.
 * Used for APP frame / image CRCs; the packer must use the same convention.
 * BoardCrcBegin resets the register; BoardCrcAccum continues over the next
 * len bytes (len must be a multiple of 4) and returns the running CRC. */
STATIC VOID BoardCrcBegin(VOID)
{
    if(!g_CrcReady) {
        rcu_periph_clock_enable(RCU_CRC);
        g_CrcReady = TRUE;
    }

    crc_data_register_reset();
}

STATIC DWORD BoardCrcAccum(CONST VOID *data, DWORD len)
{
    return crc_block_data_calculate((VOID *)data, len / 4U, INPUT_FORMAT_WORD);
}

STATIC DWORD BoardFlashCrc32(CONST VOID *data, DWORD len, DWORD *crc32)
{
    if(data == NULL || crc32 == NULL || (len % 4U) != 0U) {
        return ERR;
    }

    BoardCrcBegin();
    *crc32 = BoardCrcAccum(data, len);

    return OK;
}

STATIC DWORD BoardFlashRead(BYTE id, DWORD offset, BYTE *data, DWORD len)
{
    CONST FlashInfo_t *info = GetFlashInfo(id);
    CONST BYTE *src;
    DWORD i;

    /* Every region is readable over the memory-mapped flash, WRP included. */
    if(info == NULL || data == NULL) {
        return ERR;
    }

    if(offset >= info->flashform.size || len > (info->flashform.size - offset)) {
        return ERR;
    }

    src = (CONST BYTE *)(info->flashform.base + offset);
    for(i = 0; i < len; i++) {
        data[i] = src[i];
    }

    return OK;
}

STATIC DWORD BoardFlashWrite(BYTE id, DWORD offset, CONST BYTE *data, DWORD len)
{
    CONST FlashInfo_t *info = GetFlashInfo(id);
    DWORD addr, i;
    DDWORD word;

    if(info == NULL || data == NULL) {
        return ERR;
    }

    if(info->flashform.wrp) {
        return ERR;
    }

    if((offset % FLASH_PROGRAM_UNIT) != 0U || (len % FLASH_PROGRAM_UNIT) != 0U) {
        return ERR;
    }

    if(offset >= info->flashform.size || len > (info->flashform.size - offset)) {
        return ERR;
    }

    fmc_unlock();
    for(i = 0; i < len; i += FLASH_PROGRAM_UNIT) {
        addr = info->flashform.base + offset + i;
        word = (DDWORD)data[i]
             | ((DDWORD)data[i + 1U] << 8)
             | ((DDWORD)data[i + 2U] << 16)
             | ((DDWORD)data[i + 3U] << 24)
             | ((DDWORD)data[i + 4U] << 32)
             | ((DDWORD)data[i + 5U] << 40)
             | ((DDWORD)data[i + 6U] << 48)
             | ((DDWORD)data[i + 7U] << 56);

        if(fmc_doubleword_program(addr, word) != FMC_READY) {
            fmc_lock();
            return ERR;
        }

        /* Read back and verify; programming over a non-erased area must fail here. */
        if(*(volatile DDWORD *)addr != word) {
            fmc_lock();
            return ERR;
        }
    }
    fmc_lock();

    return OK;
}

STATIC DWORD BoardFlashErase(BYTE id)
{
    CONST FlashInfo_t *info = GetFlashInfo(id);
    DWORD page, addr;

    if(info == NULL) {
        return ERR;
    }

    if(info->flashform.wrp) {
        return ERR;
    }

    fmc_unlock();
    for(page = 0; page < info->flashform.page_num; page++) {
        addr = info->flashform.base + page * MEM_FLASH_PAGE_SIZE;
        if(fmc_page_erase(addr) != FMC_READY) {
            fmc_lock();
            return ERR;
        }
    }
    fmc_lock();

    return OK;
}

/* Common 128B frame header validation against a region instance: company
 * magic, targeting rules (FrameTargetsSlot), fwsize geometry and the header
 * CRC32 (crc32hdr field counted as 0). Shared by the flash-side check
 * below and the buffer-side VerifyFrameHdrBuf; the caller's header is
 * never modified. */
STATIC DWORD VerifyHdrCommon(CONST FrameHead_t *hdr, CONST FlashInfo_t *info)
{
    BYTE buf[sizeof(FrameHead_t)];
    DWORD crc32, i;

    if(hdr == NULL) {
        return ERR;
    }

    if(!FrameMagicMatch(hdr)) {
        return ERR;
    }

    if(!FrameTargetsSlot(hdr, info)) {
        return ERR;
    }

    if(hdr->fwsize == 0U
       || hdr->fwsize > (info->flashform.size - FRAME_HDR_AREA_SIZE)
       || (hdr->fwsize % FLASH_PROGRAM_UNIT) != 0U) {
        return ERR;
    }

    /* Copy the header out and zero crc32hdr for the CRC pass. */
    for(i = 0U; i < sizeof(FrameHead_t); i++) {
        buf[i] = ((CONST BYTE *)hdr)[i];
    }
    ((FrameHead_t *)buf)->crc32hdr = 0U;

    if(BoardFlashCrc32(buf, sizeof(FrameHead_t), &crc32) != OK || crc32 != hdr->crc32hdr) {
        return ERR;
    }

    return OK;
}

/* Frame header sanity check on the flash-resident header, per APP_Slot.txt.
 * Valid on every FLASH_TYPE_HAS_FRAME region (APP / PARA / MISC A/B), all
 * sharing the same 128B FrameHead_t. Shallow check: the payload CRC32
 * (crc32data) is covered by VerifySlotImage (deep check, APP only). */
STATIC DWORD BoardFlashVerifyHdr(BYTE id)
{
    CONST FlashInfo_t *info = GetFlashInfo(id);
    CONST FrameHead_t *hdr;

    if(info == NULL || !FLASH_TYPE_HAS_FRAME(info->flashform.type)) {
        return ERR;
    }

    hdr = GetFrameHead(id);
    if(hdr == NULL) {
        return ERR;
    }

    return VerifyHdrCommon(hdr, info);
}

/* OTA receive gate, per the upgrade flow: validate the 128B FrameHead_t
 * sitting in a RAM buffer (the SRAM OTA_RX buffer) against the targeting
 * rules of slot id BEFORE the first byte is programmed into flash. A
 * failed header stops the upgrade with the slot untouched. The caller's
 * buffer is NOT modified. */
DWORD VerifyFrameHdrBuf(BYTE id, CONST BYTE *buf, DWORD len)
{
    CONST FlashInfo_t *info = GetFlashInfo(id);

    if(info == NULL || !FLASH_TYPE_HAS_FRAME(info->flashform.type)
       || buf == NULL || len < sizeof(FrameHead_t)) {
        return ERR;
    }

    return VerifyHdrCommon((CONST FrameHead_t *)buf, info);
}

DWORD InstallSlotImage(BYTE id, BYTE *bin, DWORD len)
{
    CONST FlashInfo_t *info = GetFlashInfo(id);
    FrameHead_t *hdr;
    DWORD body_len, crc32;

    if(info == NULL || info->flashform.type != FLASH_TYPE_APP || bin == NULL) {
        return ERR;
    }

    if((len % FLASH_PROGRAM_UNIT) != 0U || len < FRAME_HDR_AREA_SIZE || len > info->flashform.size) {
        return ERR;
    }

    /* Header sanity, before anything is programmed. Same targeting rules
     * as BoardFlashVerifyHdr: slot B receives the A-named A-mirror image,
     * slot A receives A-named only. The caller's buffer is NOT modified. */
    hdr = (FrameHead_t *)bin;
    if(VerifyHdrCommon(hdr, info) != OK) {
        return ERR;
    }

    body_len = hdr->fwsize;
    if(len < (FRAME_PAYLOAD_OFF + body_len)) {
        return ERR;
    }

    /* Transport integrity: CRC32 of the payload as received. */
    if(BoardFlashCrc32(bin + FRAME_PAYLOAD_OFF, body_len, &crc32) != OK || crc32 != hdr->crc32data) {
        return ERR;
    }

    if(info->flashops.FlashErase == NULL || info->flashops.FlashWrite == NULL) {
        return ERR;
    }

    /* Erase as late as possible; program payload first and the 512B frame
     * header area last, so a power loss mid-install leaves either the old
     * image or a header-less (magic-less, rejected) slot, never a mixed one. */
    if(info->flashops.FlashErase(id) != OK) {
        return ERR;
    }

    if(info->flashops.FlashWrite(id, FRAME_PAYLOAD_OFF, bin + FRAME_PAYLOAD_OFF, body_len) != OK) {
        return ERR;
    }

    return info->flashops.FlashWrite(id, 0U, bin, FRAME_HDR_AREA_SIZE);
}

DWORD VerifySlotImage(BYTE id)
{
    STATIC BYTE buf[1024];
    CONST FlashInfo_t *info = GetFlashInfo(id);
    CONST FrameHead_t *hdr;
    DWORD body_len, done, chunk, crc32;

    if(info == NULL || info->flashform.type != FLASH_TYPE_APP) {
        return ERR;
    }

    hdr = GetFrameHead(id);
    if(hdr == NULL || BoardFlashVerifyHdr(id) != OK) {
        return ERR;
    }

    body_len = hdr->fwsize;

    BoardCrcBegin();
    for(done = 0U; done < body_len; done += chunk) {
        chunk = body_len - done;
        if(chunk > sizeof(buf)) {
            chunk = sizeof(buf);
        }

        if(BoardFlashRead(id, FRAME_PAYLOAD_OFF + done, buf, chunk) != OK) {
            return ERR;
        }
        crc32 = BoardCrcAccum(buf, chunk);
    }

    return (crc32 == hdr->crc32data) ? OK : ERR;
}


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
#include "watchdog.h"   /* WatchdogKick：逐页擦除时喂狗防复位 */
#ifndef BL_BUILD
#include "extflash.h"
#endif

STATIC DWORD BoardFlashRead(BYTE id, DWORD offset, BYTE *data, DWORD len);
STATIC DWORD BoardFlashWrite(BYTE id, DWORD offset, CONST BYTE *data, DWORD len);
STATIC DWORD BoardFlashErase(BYTE id);
STATIC DWORD BoardFlashVerifyHdr(BYTE id);
STATIC DWORD BoardFlashCrc32(CONST VOID *data, DWORD len, DWORD *crc32);
#ifndef BL_BUILD
STATIC DWORD BoardExtFlashRead(BYTE id, DWORD offset, BYTE *data, DWORD len);
STATIC DWORD BoardExtFlashWrite(BYTE id, DWORD offset, CONST BYTE *data, DWORD len);
STATIC DWORD BoardExtFlashErase(BYTE id);
#define DSP_FW_READ_OP     BoardExtFlashRead
#define DSP_FW_WRITE_OP    BoardExtFlashWrite
#define DSP_FW_ERASE_OP    BoardExtFlashErase
#else
/* BL keeps the same enum/table layout but has no external-flash access path. */
#define DSP_FW_READ_OP     NULL
#define DSP_FW_WRITE_OP    NULL
#define DSP_FW_ERASE_OP    NULL
#endif

/* GD32E512 flash is programmed 64-bit at a time; offset/len must be 8-byte aligned. */
#define FLASH_PROGRAM_UNIT   8U
#define FLASH_OFFSET_INVALID 0xFFFFFFFFUL
#define EXTFLASH_RW_CHUNK    1024U

STATIC BOOL g_CrcReady;

/* Expected frame magic template for the user-upgradeable regions
 * (VerifyHdr reference). */
STATIC CONST FrameHead_t g_FrameTemplate = {0U, 0U, {0U, 0U, 0U}, 0U, 0U, FRAME_COMPANY};

/* Physical resources in FlashId_t order. The external row keeps the same
 * index in APP and BL; only APP binds real U15 operations. */
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
    {{FLASH_ID_DSP_FW,     FLASH_TYPE_EXTFLASH, 0,  0, MEM_DSP_FW_BASE,      MEM_DSP_FW_SIZE,     FALSE}, {DSP_FW_READ_OP, DSP_FW_WRITE_OP, DSP_FW_ERASE_OP, BoardFlashVerifyHdr}},
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

    if(info == NULL || !FLASH_TYPE_HAS_FRAME(info->flashform.type)
       || info->flashform.type == FLASH_TYPE_EXTFLASH) {
        return NULL;
    }

    /* Internal flash is memory-mapped (XIP); U15 must use ReadFrameHead(). */
    return (CONST FrameHead_t *)info->flashform.base;
}

DWORD FlashFrameHeaderOffset(BYTE id)
{
    CONST FlashInfo_t *info = GetFlashInfo(id);

    if(info == NULL || !FLASH_TYPE_HAS_FRAME(info->flashform.type)) {
        return FLASH_OFFSET_INVALID;
    }
    return (info->flashform.type == FLASH_TYPE_EXTFLASH) ? MEM_DSP_FW_META_BASE : 0U;
}

DWORD FlashFramePayloadOffset(BYTE id)
{
    CONST FlashInfo_t *info = GetFlashInfo(id);

    if(info == NULL || !FLASH_TYPE_HAS_FRAME(info->flashform.type)) {
        return FLASH_OFFSET_INVALID;
    }
    return (info->flashform.type == FLASH_TYPE_EXTFLASH) ? 0U : FRAME_PAYLOAD_OFF;
}

DWORD FlashFramePayloadCapacity(BYTE id)
{
    CONST FlashInfo_t *info = GetFlashInfo(id);

    if(info == NULL || !FLASH_TYPE_HAS_FRAME(info->flashform.type)) {
        return 0U;
    }
    if(info->flashform.type == FLASH_TYPE_EXTFLASH) {
        return MEM_DSP_FW_PAYLOAD_CAP;
    }
    return (info->flashform.size >= FRAME_HDR_AREA_SIZE)
         ? (info->flashform.size - FRAME_HDR_AREA_SIZE) : 0U;
}

DWORD ReadFrameHead(BYTE id, FrameHead_t *hdr)
{
    CONST FlashInfo_t *info = GetFlashInfo(id);
    DWORD offset = FlashFrameHeaderOffset(id);

    if(info == NULL || hdr == NULL || offset == FLASH_OFFSET_INVALID
       || info->flashops.FlashRead == NULL) {
        return ERR;
    }
    return info->flashops.FlashRead(id, offset, (BYTE *)hdr, sizeof(*hdr));
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

/* Logical (type, storeaddr) ownership rule for a physical target. APP_A and
 * APP_B both carry the same APP/A-address image so BL can copy B to A without
 * rewriting the header. PARAM/MISC name their own physical base; DSP_FW uses
 * the external NOR's zero-based payload address. */
STATIC BOOL FrameTargetsSlot(CONST FrameHead_t *hdr, CONST FlashInfo_t *info)
{
    if(hdr == NULL || info == NULL || hdr->type != info->flashform.type) {
        return FALSE;
    }
    if(info->flashform.type == FLASH_TYPE_APP) {
        return (BOOL)(hdr->storeaddr == MEM_APP_A_BASE);
    }
    return (BOOL)(hdr->storeaddr == info->flashform.base);
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

/* 导出包装：BL 提升流程改写 OtaInfo_t.state 后须重算 crc32（撕裂写守卫，
 * 覆盖 0x00..0x37、本字段按 0 参与），与 OtaConfirm 同一 CRC 约定。 */
DWORD FlashCrc32(CONST VOID *data, DWORD len, DWORD *crc32)
{
    return BoardFlashCrc32(data, len, crc32);
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

#ifndef BL_BUILD
/* U15 adapters keep the common FlashOps signature. Lengths are chunked before
 * entering the WORD-length extflash API; all addresses are NOR offsets. */
STATIC DWORD BoardExtFlashRead(BYTE id, DWORD offset, BYTE *data, DWORD len)
{
    CONST FlashInfo_t *info = GetFlashInfo(id);
    DWORD done = 0U;

    if(info == NULL || info->flashform.type != FLASH_TYPE_EXTFLASH || data == NULL) {
        return ERR;
    }
    if(offset >= info->flashform.size || len > (info->flashform.size - offset)) {
        return ERR;
    }
    while(done < len) {
        DWORD chunk = len - done;
        if(chunk > EXTFLASH_RW_CHUNK) {
            chunk = EXTFLASH_RW_CHUNK;
        }
        if(ExtFlashDataRead(offset + done, data + done, (WORD)chunk) != OK) {
            return ERR;
        }
        done += chunk;
    }
    return OK;
}

STATIC DWORD BoardExtFlashWrite(BYTE id, DWORD offset, CONST BYTE *data, DWORD len)
{
    CONST FlashInfo_t *info = GetFlashInfo(id);
    DWORD done = 0U;

    if(info == NULL || info->flashform.type != FLASH_TYPE_EXTFLASH || data == NULL) {
        return ERR;
    }
    if(offset >= info->flashform.size || len > (info->flashform.size - offset)) {
        return ERR;
    }
    while(done < len) {
        DWORD chunk = len - done;
        if(chunk > EXTFLASH_RW_CHUNK) {
            chunk = EXTFLASH_RW_CHUNK;
        }
        if(ExtFlashProgram(offset + done, data + done, (WORD)chunk) != OK) {
            return ERR;
        }
        done += chunk;
    }
    return OK;
}

STATIC DWORD BoardExtFlashErase(BYTE id)
{
    CONST FlashInfo_t *info = GetFlashInfo(id);

    if(info == NULL || info->flashform.type != FLASH_TYPE_EXTFLASH) {
        return ERR;
    }
    return ExtFlashChipErase();
}
#endif

/* 逐页擦除 + 每页喂狗（CDB 0101h Start 用）。APP 槽 23 页 × 8KB，整槽擦除约
 * 数百 ms~1s，超过看门狗超时窗口：普通 EraseFlash 期间不喂狗会复位。与
 * BoardFlashErase 同逻辑，仅每页擦除后插一脚 WatchdogKick。 */
DWORD EraseFlashWithFeed(BYTE id)
{
    CONST FlashInfo_t *info = GetFlashInfo(id);
    DWORD page, addr;

    if(info == NULL) {
        return ERR;
    }

    if(info->flashform.wrp) {
        return ERR;
    }
    if(info->flashform.type == FLASH_TYPE_EXTFLASH) {
        return (info->flashops.FlashErase != NULL)
             ? info->flashops.FlashErase(id) : ERR;
    }

    fmc_unlock();
    for(page = 0; page < info->flashform.page_num; page++) {
        addr = info->flashform.base + page * MEM_FLASH_PAGE_SIZE;
        if(fmc_page_erase(addr) != FMC_READY) {
            fmc_lock();
            return ERR;
        }
        WatchdogKick();
    }
    fmc_lock();

    return OK;
}

/* Common 112B frame header validation against a region instance: company
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
       || hdr->fwsize > FlashFramePayloadCapacity(info->flashform.flashid)
       || (hdr->fwsize % 4U) != 0U) {
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

/* Flash-resident header check for both XIP and external targets. The payload
 * CRC is covered separately by VerifyFramedImage(). */
STATIC DWORD BoardFlashVerifyHdr(BYTE id)
{
    CONST FlashInfo_t *info = GetFlashInfo(id);
    FrameHead_t hdr;

    if(info == NULL || !FLASH_TYPE_HAS_FRAME(info->flashform.type)) {
        return ERR;
    }
    if(ReadFrameHead(id, &hdr) != OK) {
        return ERR;
    }

    return VerifyHdrCommon(&hdr, info);
}

/* OTA receive gate, per the upgrade flow: validate the 112B FrameHead_t
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

    /* Header sanity before programming: both physical APP slots accept the
     * same FLASH_TYPE_APP + MEM_APP_A_BASE logical header. */
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

DWORD VerifyFramedImage(BYTE id)
{
    STATIC BYTE buf[1024];
    CONST FlashInfo_t *info = GetFlashInfo(id);
    FrameHead_t hdr;
    DWORD body_len, done, chunk, crc32 = 0U;
    DWORD payload_off;

    if(info == NULL || !FLASH_TYPE_HAS_FRAME(info->flashform.type)
       || info->flashops.FlashRead == NULL) {
        return ERR;
    }
    if(ReadFrameHead(id, &hdr) != OK || VerifyHdrCommon(&hdr, info) != OK) {
        return ERR;
    }

    body_len = hdr.fwsize;
    payload_off = FlashFramePayloadOffset(id);
    if(payload_off == FLASH_OFFSET_INVALID) {
        return ERR;
    }

    BoardCrcBegin();
    for(done = 0U; done < body_len; done += chunk) {
        chunk = body_len - done;
        if(chunk > sizeof(buf)) {
            chunk = sizeof(buf);
        }

        if(info->flashops.FlashRead(id, payload_off + done, buf, chunk) != OK) {
            return ERR;
        }
        crc32 = BoardCrcAccum(buf, chunk);
    }

    return (crc32 == hdr.crc32data) ? OK : ERR;
}

DWORD VerifySlotImage(BYTE id)
{
    CONST FlashInfo_t *info = GetFlashInfo(id);

    if(info == NULL || info->flashform.type != FLASH_TYPE_APP) {
        return ERR;
    }
    return VerifyFramedImage(id);
}

/* OTA upgrade confirm (CDB 0109h Run): build the 64B OtaInfo_t record
 * (magic/ver/state/src + the slot-B image identity fields swver/fwsize/
 * crc32data/crc32hdr + a persistent update_cnt + crc32 over 0x00..0x37),
 * erase OTA metadata J-A and write it. The bootloader reads it on the next
 * boot and promotes B -> A. The crc32 field lands after everything it
 * covers, so a torn write is always caught. */

/* A record is valid when the magic matches and the crc32 (over 0x00..0x37,
 * this field counted as 0) passes. Used to read back the previous upgrade
 * count; an erased (0xFF) or torn record reads invalid -> count restarts. */
STATIC BOOL OtaInfoValid(CONST OtaInfo_t *r)
{
    DWORD crc32;
    BYTE  i;

    if(r == NULL) {
        return FALSE;
    }
    for(i = 0U; i < (BYTE)(sizeof(OTA_INFO_MAGIC) - 1U); i++) {
        if(r->magic[i] != (BYTE)OTA_INFO_MAGIC[i]) {
            return FALSE;
        }
    }
    if(BoardFlashCrc32(r, OTA_INFO_CRC_OFF, &crc32) != OK) {
        return FALSE;
    }
    return (BOOL)(crc32 == r->crc32);
}

DWORD OtaConfirm(VOID)
{
    CONST FlashInfo_t *meta = GetFlashInfo(FLASH_ID_METADATA_A);
    CONST FrameHead_t *hdr;
    CONST OtaInfo_t   *prev;
    OtaInfo_t info;
    DWORD prev_cnt, crc32, i;

    if(meta == NULL) {
        return ERR;
    }
    hdr = GetFrameHead(FLASH_ID_APP_B);
    if(hdr == NULL || BoardFlashVerifyHdr(FLASH_ID_APP_B) != OK) {
        return ERR;   /* slot B has no valid frame header */
    }

    /* Previous upgrade count: keep it when the record is still valid, else
     * restart from 0; increment by one for this upgrade. */
    prev = (CONST OtaInfo_t *)meta->flashform.base;   /* XIP in-place read */
    prev_cnt = OtaInfoValid(prev) ? prev->update_cnt : 0U;

    for(i = 0U; i < sizeof(info); i++) {
        ((BYTE *)&info)[i] = 0xFFU;                   /* rsvd stays erased 0xFF */
    }
    for(i = 0U; i < (sizeof(OTA_INFO_MAGIC) - 1U); i++) {
        info.magic[i] = (BYTE)OTA_INFO_MAGIC[i];      /* first 8B "TBCOTAUP", rest 0xFF */
    }
    info.ver        = OTA_INFO_VER;
    info.state      = OTA_ST_CONFIRMED;
    info.src        = OTA_SRC_HOST;
    for(i = 0U; i < FRAME_STR_SIZE; i++) {
        info.swver[i] = hdr->swver[i];                /* target version (slot B copy) */
    }
    info.fwsize     = hdr->fwsize;
    info.crc32data  = hdr->crc32data;
    info.crc32hdr   = hdr->crc32hdr;
    info.update_cnt = prev_cnt + 1U;

    if(BoardFlashCrc32(&info, OTA_INFO_CRC_OFF, &crc32) != OK) {  /* 0x38=56B, %4==0 */
        return ERR;
    }
    info.crc32 = crc32;

    if(BoardFlashErase(FLASH_ID_METADATA_A) != OK) {
        return ERR;
    }
    return BoardFlashWrite(FLASH_ID_METADATA_A, 0U, (CONST BYTE *)&info, sizeof(OtaInfo_t));
}


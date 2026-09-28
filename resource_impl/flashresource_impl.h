/***************************************************************************/ /**
 *   @file   flashresource_impl.h
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
#ifndef FLASHRESOURCE_IMPL_H
#define FLASHRESOURCE_IMPL_H

#include "basedef.h"
#include "memory_map.h"

/* ---------------------------------------------------------------------------
 * Region frame header, per flashmap/APP_Slot.txt. Every user-upgradeable
 * region (FLASH_ID_APP_A/B, FLASH_ID_PARA_A/B, FLASH_ID_MISC_A/B) carries
 * the same 512B frame header area at its base; the payload follows at
 * +0x200. All six regions share one common 128B FrameHead_t, 8 x 16B
 * hexdump lines: the payload CRC32 (crc32data) sits at the very first
 * address, the header CRC32 (crc32hdr) at the very last DWORD (+0x7C),
 * the scalar members fill the rest of the first line, and the identity
 * slots (company / hdver / swver / modulename) are fixed by the macros
 * below. An erased region reads crc32data 0xFF.. at offset 0 and fails
 * any header check.
 * ------------------------------------------------------------------------- */
#define FRAME_HDR_AREA_SIZE   0x00000200U           /* 512B frame header area */
#define FRAME_PAYLOAD_OFF     0x00000200U           /* payload start offset */
#define FRAME_STR_SIZE        16U                   /* ASCII identity field slot, 0xFF padded */

/* Identity slot contents, fixed by macros; the packer carries the same
 * strings (bin/APP_A_frame.json defaults). */
#define FRAME_COMPANY         "Terabitcom"          /* company slot */
#define FRAME_HDVER           "HF2026090614"         /* hdver slot, header format version */
#define FRAME_SWVER           "SW2026092614"         /* swver slot, firmware version */
#define FRAME_MODULENAME      "400G-SR4-OSFP-V1"    /* modulename slot */

/* Common part of the 512B frame header, present at the base of every
 * user-upgradeable region, exactly 8 x 16B hexdump lines. The identity
 * fields are fixed 16B ASCII slots (0xFF padded, no NUL terminator) whose
 * contents come from the FRAME_* macros above; the scalar members share
 * the first line and crc32hdr shares the last line with the trailing rsvd,
 * so one hexdump line always shows whole member(s). crc32data covers the
 * payload at +0x200; crc32hdr covers the full 128B FrameHead_t with this
 * field counted as 0 during the pass. */
typedef struct {
    DWORD  crc32data;                     /* +0x00 payload CRC32 (on-chip CRC convention) */
    BYTE   flashid;                       /* +0x04 FLASH_ID_xxx of the owning region */
    DWORD  storeaddr;                     /* +0x08 firmware position = region base */
    DWORD  fwsize;                        /* +0x0C payload size, 8-aligned */
    BYTE   company[FRAME_STR_SIZE];       /* +0x10 = FRAME_COMPANY */
    BYTE   hdver[FRAME_STR_SIZE];         /* +0x20 = FRAME_HDVER */
    BYTE   swver[FRAME_STR_SIZE];         /* +0x30 = FRAME_SWVER */
    BYTE   modulename[FRAME_STR_SIZE];    /* +0x40 = FRAME_MODULENAME */
    BYTE   rsvd0[FRAME_STR_SIZE];         /* +0x50 -> 0xFF */
    BYTE   rsvd1[FRAME_STR_SIZE];         /* +0x60 -> 0xFF */
    BYTE   rsvd2[12];                     /* +0x70 -> 0xFF */
    DWORD  crc32hdr;                      /* +0x7C header CRC32, this field counted as 0 */
} FrameHead_t;                            /* 128B */

typedef struct {
    FrameHead_t  hdr;          /* 128B +0x000 */
    BYTE         rsvd[384];    /* +0x080 -> 0xFF */
} GfwFrame_t;

/* ---------------------------------------------------------------------------
 * APP upgrade info record, stored at the base of OTA metadata J-A
 * (MEM_OTA_META_A_BASE, P5, 8KB). Fixed-A / backup-B scheme: while a
 * firmware upgrade is being received / installed / confirmed, the APP
 * keeps one 128B OtaInfo_t record here (page erase before every rewrite,
 * the rest of the page stays 0xFF). J-B is reserved.
 *
 *   +0x00  magic[16]    OTA_INFO_MAGIC + 0xFF pad
 *   +0x10  ver|state|src|rsvd[2]
 *   +0x20  swver[16]    target firmware version (package swver copy)
 *   +0x30  fwsize / crc32data / crc32hdr / rsvd
 *   +0x40  total_len / rx_done / blk_size / rsvd     (resume progress)
 *   +0x50  fail_cnt / update_cnt / rsvd[8]
 *   +0x60  crc32        over 0x00..0x5F, this field counted as 0
 *   +0x64  rsvd[28]     -> 0xFF
 *
 * crc32 uses the on-chip CRC convention (same as FrameHead_t), so the
 * record can be checked with BoardFlashCrc32 / the packer crc32_gd32.
 * ------------------------------------------------------------------------- */
#define OTA_INFO_REC_SIZE     128U                    /* OtaInfo_t */
#define OTA_INFO_MAGIC        "TBCOTAUP"              /* 8B + 0xFF pad */
#define OTA_INFO_VER          0x00010000U             /* record layout version 1.0 */
#define OTA_INFO_CRC_OFF      0x60U                   /* crc32 field offset */

/* Upgrade state (OtaInfo_t.state). */
typedef enum {
    OTA_ST_IDLE = 0,      /* erased / no upgrade activity */
    OTA_ST_RECEIVING,     /* package receiving, resume via rx_done */
    OTA_ST_RECEIVED,      /* package complete, awaiting InstallSlotImage */
    OTA_ST_INSTALLED,     /* stored in slot B and deep-verified, awaiting confirm */
    OTA_ST_CONFIRMED,     /* APP confirmed, BL promotes B -> A */
    OTA_ST_FAILED         /* receive / verify / install error */
} OtaState_t;

/* Who triggered the upgrade (OtaInfo_t.src). */
typedef enum {
    OTA_SRC_NONE = 0,     /* not specified */
    OTA_SRC_HOST,         /* host / upper computer */
    OTA_SRC_COMM,         /* communication port (UART / I2C master) */
    OTA_SRC_DEBUG         /* local debug / forced */
} OtaSrc_t;

typedef struct {
    BYTE  magic[16];      /* +0x00 OTA_INFO_MAGIC + 0xFF pad */
    DWORD ver;            /* +0x10 = OTA_INFO_VER */
    BYTE  state;          /* +0x14 OTA_ST_xxx */
    BYTE  src;            /* +0x15 OTA_SRC_xxx */
    BYTE  rsvd0[2];       /* +0x16 -> 0xFF */
    BYTE  swver[FRAME_STR_SIZE];  /* +0x20 target version (package swver copy) */
    DWORD fwsize;         /* +0x30 package payload size */
    DWORD crc32data;      /* +0x34 package payload CRC32 (FrameHead_t copy) */
    DWORD crc32hdr;       /* +0x38 package header CRC32 */
    DWORD rsvd1;          /* +0x3C -> 0xFF */
    DWORD total_len;      /* +0x40 framed bin total length (header area + payload) */
    DWORD rx_done;        /* +0x44 bytes received so far (resume point) */
    DWORD blk_size;       /* +0x48 transfer block size, 0 = whole image */
    DWORD rsvd2;          /* +0x4C -> 0xFF */
    DWORD fail_cnt;       /* +0x50 consecutive failure counter */
    DWORD update_cnt;     /* +0x54 accumulated successful upgrades */
    BYTE  rsvd3[8];       /* +0x58 -> 0xFF */
    DWORD crc32;          /* +0x60 CRC32 over 0x00..0x5F, this field counted as 0 */
    BYTE  rsvd4[28];      /* +0x64 -> 0xFF */
} OtaInfo_t;              /* 128B */

/* Region types of the 11 memory_map.h flash regions. */
typedef enum {
    FLASH_TYPE_BOOT,      /* P0-P3  bootloader, 32KB, WRP */
    FLASH_TYPE_KEY,       /* P4     key / device identity, 8KB, WRP */
    FLASH_TYPE_META,      /* P5/P6  OTA metadata, 8KB (J-A: APP upgrade info) */
    FLASH_TYPE_BLACKBOX,  /* P7     black box / diagnostics, 8KB */
    FLASH_TYPE_APP,       /* P8+    APP slot with frame header, 184KB */
    FLASH_TYPE_PARAM,     /* P54+   user parameters, 32KB */
    FLASH_TYPE_MISC       /* P62/63 misc parameters journal, 8KB */
}FlashType_t;

/* The 6 user-upgradeable regions (APP / PARAM / MISC, slots A/B) carry a
 * 512B frame header area at the region base; BOOT / KEY (WRP), META and
 * BLACKBOX regions do not. */
#define FLASH_TYPE_HAS_FRAME(t) (((t) == FLASH_TYPE_APP) || ((t) == FLASH_TYPE_PARAM) || ((t) == FLASH_TYPE_MISC))

/* Region attributes, one entry per memory_map.h area. All regions are
 * readable over the memory-mapped flash (XIP); wrp regions reject
 * write/erase. Only FLASH_TYPE_HAS_FRAME regions carry a frame, and all
 * of them use the same common FrameHead_t. */
typedef struct {
    BYTE  flashid;     /* FLASH_ID_xxx */
    BYTE  type;        /* FLASH_TYPE_xxx */
    WORD  page_first;  /* first 8KB page index, 0-63 */
    WORD  page_num;    /* 8KB page count */
    DWORD base;        /* region base address, 8KB aligned */
    DWORD size;        /* region size in bytes */
    BOOL  wrp;         /* write protected: write/erase return ERR */
}FlashForm_t;

typedef struct {
    DWORD (*FlashRead)(BYTE id, DWORD offset, BYTE *data, DWORD len);
    DWORD (*FlashWrite)(BYTE id, DWORD offset, CONST BYTE *data, DWORD len);
    DWORD (*FlashErase)(BYTE id);
    DWORD (*FlashVerifyHdr)(BYTE id);
}FlashOps_t;

typedef struct {
    FlashForm_t flashform;
    FlashOps_t flashops;
}FlashInfo_t;

typedef enum {
    FLASH_ID_BOOTLOADER,
    FLASH_ID_KEY,
    FLASH_ID_METADATA_A,
    FLASH_ID_METADATA_B,
    FLASH_ID_BLACKBOX,
    FLASH_ID_APP_A,
    FLASH_ID_APP_B,
    FLASH_ID_PARA_A,
    FLASH_ID_PARA_B,
    FLASH_ID_MISC_A,
    FLASH_ID_MISC_B,
    FLASH_ID_NUM
}FlashId_t;

typedef FlashId_t FLASH_ID_ENUM;
#define FLASH_ID_0 FLASH_ID_APP_A

CONST FlashInfo_t *GetFlashInfo(BYTE id);
/* Memory-mapped common frame header (FrameHead_t) of a user-upgradeable
 * region; NULL when id is invalid or the region carries no frame. */
CONST FrameHead_t *GetFrameHead(BYTE id);

/* ---------------------------------------------------------------------------
 * APP upgrade helpers, per the fixed-A / mirror-B scheme. APP Slot A is the
 * fixed running region (always booted). APP Slot B is the A-mirror region
 * and the SOLE upgrade entry: every APP upgrade -- including the factory /
 * blank-device case -- is received into B first, and the package must be
 * A-named and byte-identical to the slot A image (there is no B-named
 * package). After the APP confirms the upgrade, the bootloader promotes
 * B to A with a verbatim copy (the header is already A-named, nothing to
 * rewrite), so the steady-state invariant is slot A == slot B. The upgrade
 * progress is tracked in one OtaInfo_t record at the base of OTA metadata
 * J-A (see above); there is no ping-pong journal and no rollback state
 * machine.
 * ------------------------------------------------------------------------- */
/* Install a received APP.bin (512B frame header area: 128B FrameHead_t +
 * rsvd -> 0xFF, then the payload) into an APP slot, len 8-byte aligned.
 * The normal target is FLASH_ID_APP_B; the package must carry the A-named
 * header (flashid FLASH_ID_APP_A, storeaddr MEM_APP_A_BASE) -- the same
 * A-mirror image that slot A holds. Slot A accepts the A-named header as
 * well and should never be programmed with an untrusted package.
 * Validation (before the slot is erased):
 *   - magic and the targeting rules above (FrameTargetsSlot);
 *   - fwsize geometry;
 *   - payload CRC32 (crc32data) and header CRC32 (crc32hdr counted as 0);
 *   - the caller's buffer is NOT modified.
 * Erase the slot and program the payload first, the 512B frame header
 * area last, so a power loss mid-install leaves the old image or a
 * header-less (rejected) slot, never a mix. */
DWORD InstallSlotImage(BYTE id, BYTE *bin, DWORD len);
/* Deep verify: CRC32 of the installed payload (flash readback) against
 * FrameHead_t.crc32data. Slower than VerifyFlashHdr; run it on slot B
 * after the install and before the upgrade is confirmed. */
DWORD VerifySlotImage(BYTE id);
/* OTA receive gate: validate the 128B FrameHead_t in a RAM buffer (the
 * SRAM OTA_RX buffer) against slot id BEFORE the first byte is written to
 * flash; a failed header stops the upgrade with the slot untouched. The
 * caller's buffer is NOT modified. */
DWORD VerifyFrameHdrBuf(BYTE id, CONST BYTE *buf, DWORD len);

#endif

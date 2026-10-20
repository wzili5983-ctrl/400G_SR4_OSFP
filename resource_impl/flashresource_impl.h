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
 * Region frame header. APP/PARA/MISC internal regions keep the 512B frame
 * header area at the region base and place the payload at +0x200. DSP_FW uses
 * the same 112B FrameHead_t and 512B frame area, but its raw DSP boot payload
 * starts at U15 offset 0 and the frame area lives at MEM_DSP_FW_META_BASE.
 *
 * FrameHead_t describes the LOGICAL image: type is a FlashType_t value and
 * storeaddr is the logical storage address for that type. It does not name a
 * physical FlashId_t. CDB maps (type, storeaddr) to one physical target.
 * crc32data covers only the payload; crc32hdr covers the 112B header with the
 * crc32hdr field counted as zero.
 * ------------------------------------------------------------------------- */
#define FRAME_HDR_AREA_SIZE   0x00000200U           /* 512B frame header area */
#define FRAME_PAYLOAD_OFF     0x00000200U           /* payload start offset */
#define FRAME_STR_SIZE        16U                   /* ASCII identity field slot, 0xFF padded */

/* Identity slot contents, fixed by macros; the packer carries the same
 * strings (tools/400G_SR4_OSFP_FRAMEHEAD.json defaults). */
#define FRAME_COMPANY         "Terabitcom"          /* company slot */
#define FRAME_HDVER           "HF2026090614"         /* hdver slot, header format version */
#define FRAME_SWVER           "SW2026092614"         /* swver slot, firmware version */
#define FRAME_MODULENAME      "400G-SR4-OSFP-V1"    /* modulename slot */

/* Common 112B FrameHead_t (7 x 16B hexdump lines). Identity fields are fixed
 * 16B ASCII slots (0xFF padded, no NUL terminator). type/storeaddr describe
 * the logical image; the physical target rules are implemented by
 * FrameTargetsSlot and the CDB route table. */
typedef struct {
    DWORD  crc32data;                     /* +0x00 payload CRC32 (on-chip CRC convention) */
    BYTE   type;                          /* +0x04 FLASH_TYPE_xxx */
    BYTE   rsvd0[3];                      /* +0x05 -> 0xFF (DWORD alignment, explicit) */
    DWORD  storeaddr;                     /* +0x08 logical storage address for type */
    DWORD  fwsize;                        /* +0x0C payload size */
    BYTE   company[FRAME_STR_SIZE];       /* +0x10 = FRAME_COMPANY */
    BYTE   hdver[FRAME_STR_SIZE];         /* +0x20 = FRAME_HDVER */
    BYTE   swver[FRAME_STR_SIZE];         /* +0x30 = FRAME_SWVER */
    BYTE   modulename[FRAME_STR_SIZE];    /* +0x40 = FRAME_MODULENAME */
    BYTE   rsvd1[FRAME_STR_SIZE];         /* +0x50 -> 0xFF */
    BYTE   rsvd2[12];                     /* +0x60 -> 0xFF */
    DWORD  crc32hdr;                      /* +0x6C header CRC32, this field counted as 0 */
} FrameHead_t;                            /* 112B */

typedef struct {
    FrameHead_t  hdr;          /* 112B +0x000 */
    BYTE         rsvd[400];    /* +0x070 -> 0xFF */
} GfwFrame_t;

/* 帧头尺寸钉死 == CDB 0101h Start 命令 VendorData 长度（112B）；
 * 结构体一旦漂移即编译期硬错误（仿 ral.c 的断言惯用法）。 */
typedef BYTE FrameHeadSizeCheck_t[(sizeof(FrameHead_t) == 112U) ? 1 : -1];

/* ---------------------------------------------------------------------------
 * APP upgrade confirm record, stored at the base of OTA metadata J-A
 * (MEM_OTA_META_A_BASE, P5, 8KB). Fixed-A / backup-B scheme: the APP writes
 * this record once, when it confirms the upgrade (CDB 0109h Run); the
 * bootloader reads it on boot and, on a valid CONFIRMED record, promotes
 * B -> A. It is a confirm flag carrying the target image's integrity fields
 * plus a persistent upgrade counter (update_cnt) -- no resume journal, no
 * retry counter -- so the only states that ever reach flash are IDLE
 * (erased) and CONFIRMED. crc32 is the torn-write guard. J-B is reserved.
 *
 *   +0x00  magic[16]    OTA_INFO_MAGIC + 0xFF pad
 *   +0x10  ver          OTA_INFO_VER
 *   +0x14  state        OTA_ST_xxx (IDLE / CONFIRMED)
 *   +0x15  src          OTA_SRC_xxx (HOST)
 *   +0x16  rsvd[2]      -> 0xFF
 *   +0x18  swver[16]    target firmware version (package swver copy)
 *   +0x28  fwsize       payload size
 *   +0x2C  crc32data    payload CRC32 (FrameHead_t copy)
 *   +0x30  crc32hdr     header CRC32
 *   +0x34  update_cnt   OTA upgrade count (persistent, +1 per confirm)
 *   +0x38  crc32        over 0x00..0x37, this field counted as 0
 *   +0x3C  rsvd[4]      -> 0xFF
 *
 * crc32 uses the on-chip CRC convention (same as FrameHead_t), so the
 * record can be checked with BoardFlashCrc32 / the packer crc32_gd32.
 * ------------------------------------------------------------------------- */
#define OTA_INFO_REC_SIZE     64U                     /* OtaInfo_t */
#define OTA_INFO_MAGIC        "TBCOTAUP"              /* 8B + 0xFF pad */
#define OTA_INFO_VER          0x00010000U             /* record layout version 1.0 */
#define OTA_INFO_CRC_OFF      0x38U                   /* crc32 field offset */

/* Upgrade state (OtaInfo_t.state): confirm-only, no resume/retry states. */
typedef enum {
    OTA_ST_IDLE = 0,      /* erased / no upgrade activity */
    OTA_ST_CONFIRMED      /* APP confirmed, BL promotes B -> A */
} OtaState_t;

/* Who triggered the upgrade (OtaInfo_t.src). */
typedef enum {
    OTA_SRC_NONE = 0,     /* not specified */
    OTA_SRC_HOST          /* host / upper computer */
} OtaSrc_t;

typedef struct {
    BYTE  magic[16];      /* +0x00 OTA_INFO_MAGIC + 0xFF pad */
    DWORD ver;            /* +0x10 = OTA_INFO_VER */
    BYTE  state;          /* +0x14 OTA_ST_xxx */
    BYTE  src;            /* +0x15 OTA_SRC_xxx */
    BYTE  rsvd0[2];       /* +0x16 -> 0xFF */
    BYTE  swver[FRAME_STR_SIZE];  /* +0x18 target version (package swver copy) */
    DWORD fwsize;         /* +0x28 payload size */
    DWORD crc32data;      /* +0x2C payload CRC32 (FrameHead_t copy) */
    DWORD crc32hdr;       /* +0x30 header CRC32 */
    DWORD update_cnt;     /* +0x34 OTA upgrade count (persistent) */
    DWORD crc32;          /* +0x38 CRC32 over 0x00..0x37, this field counted as 0 */
    BYTE  rsvd1[4];       /* +0x3C -> 0xFF */
} OtaInfo_t;              /* 64B */

/* 记录尺寸钉死 64B（8 字节对齐，WriteFlash 双字写友好）；漂移即编译期硬错误。 */
typedef BYTE OtaInfoSizeCheck_t[(sizeof(OtaInfo_t) == OTA_INFO_REC_SIZE) ? 1 : -1];

/* Logical image / physical region types. FrameHead_t.type uses these values;
 * FlashId_t below remains the physical resource index. */
typedef enum {
    FLASH_TYPE_BOOT,      /* P0-P3  bootloader, 32KB, WRP */
    FLASH_TYPE_KEY,       /* P4     key / device identity, 8KB, WRP */
    FLASH_TYPE_META,      /* P5/P6  OTA metadata, 8KB (J-A: APP upgrade info) */
    FLASH_TYPE_BLACKBOX,  /* P7     black box / diagnostics, 8KB */
    FLASH_TYPE_APP,       /* P8+    APP slot with frame header, 184KB */
    FLASH_TYPE_PARAM,     /* P54+   user parameters, 32KB */
    FLASH_TYPE_MISC,      /* P62/63 misc parameters journal, 8KB */
    FLASH_TYPE_EXTFLASH   /* U15 external NOR, DSP raw boot image + management frame */
}FlashType_t;

/* APP/PARAM/MISC and the external DSP_FW image carry the common FrameHead_t.
 * BOOT / KEY (WRP), META and BLACKBOX do not. */
#define FLASH_TYPE_HAS_FRAME(t) (((t) == FLASH_TYPE_APP) || \
                                 ((t) == FLASH_TYPE_PARAM) || \
                                 ((t) == FLASH_TYPE_MISC) || \
                                 ((t) == FLASH_TYPE_EXTFLASH))

/* Physical region attributes. Internal regions are memory-mapped (XIP);
 * FLASH_TYPE_EXTFLASH uses its own NOR offset address space and FlashOps.
 * page_first/page_num describe internal 8KB pages only and are zero for U15. */
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
    FLASH_ID_DSP_FW,      /* physical U15 resource; CDB download is APP-only */
    FLASH_ID_NUM
}FlashId_t;

typedef FlashId_t FLASH_ID_ENUM;
#define FLASH_ID_0 FLASH_ID_APP_A

CONST FlashInfo_t *GetFlashInfo(BYTE id);
/* XIP frame header access for internal regions only; EXTFLASH returns NULL. */
CONST FrameHead_t *GetFrameHead(BYTE id);
/* Target-independent frame access/layout helpers. Offsets are relative to the
 * physical region's own address space (U15 uses NOR offsets, not MCU addresses). */
DWORD ReadFrameHead(BYTE id, FrameHead_t *hdr);
DWORD FlashFrameHeaderOffset(BYTE id);
DWORD FlashFramePayloadOffset(BYTE id);
DWORD FlashFramePayloadCapacity(BYTE id);
/* On-chip CRC32 (GD32 CRC unit, INPUT_FORMAT_WORD)；len 须 4 对齐。
 * BL 提升流程改写 OtaInfo_t 后重算 crc32 用（OtaConfirm 同约定）。 */
DWORD FlashCrc32(CONST VOID *data, DWORD len, DWORD *crc32);

/* ---------------------------------------------------------------------------
 * APP upgrade helpers, per the fixed-A / mirror-B scheme. APP Slot A is the
 * fixed running region (always booted). APP Slot B is the A-mirror region
 * and the SOLE upgrade entry: every APP upgrade -- including the factory /
 * blank-device case -- is received into B first. Its header always carries
 * FLASH_TYPE_APP and storeaddr=MEM_APP_A_BASE, and its payload is
 * byte-identical to slot A (there is no APP/B-address package). After the
 * APP confirms the upgrade, the bootloader promotes B to A with a verbatim
 * copy, so the steady-state invariant is slot A == slot B. The upgrade
 * progress is tracked in one OtaInfo_t record at the base of OTA metadata
 * J-A (see above); there is no ping-pong journal and no rollback state
 * machine.
 * ------------------------------------------------------------------------- */
/* Install a received APP.bin (512B frame header area: 112B FrameHead_t +
 * rsvd -> 0xFF, then the payload) into an APP slot, len 8-byte aligned.
 * The normal target is FLASH_ID_APP_B; the package carries FLASH_TYPE_APP
 * and storeaddr=MEM_APP_A_BASE -- the same A-mirror image that slot A holds.
 * Slot A accepts the same logical header as well and should never be
 * programmed with an untrusted package.
 * Validation (before the slot is erased):
 *   - magic and the targeting rules above (FrameTargetsSlot);
 *   - fwsize geometry;
 *   - payload CRC32 (crc32data) and header CRC32 (crc32hdr counted as 0);
 *   - the caller's buffer is NOT modified.
 * Erase the slot and program the payload first, the 512B frame header
 * area last, so a power loss mid-install leaves the old image or a
 * header-less (rejected) slot, never a mix. */
DWORD InstallSlotImage(BYTE id, BYTE *bin, DWORD len);
/* Deep verify for any framed target: read the physical frame through FlashOps,
 * then stream the payload and compare crc32data. */
DWORD VerifyFramedImage(BYTE id);
/* APP-only compatibility wrapper used by the fixed-A / mirror-B bootloader. */
DWORD VerifySlotImage(BYTE id);
/* OTA receive gate: validate the 112B FrameHead_t in a RAM buffer (the
 * SRAM OTA_RX buffer) against slot id BEFORE the first byte is written to
 * flash; a failed header stops the upgrade with the slot untouched. The
 * caller's buffer is NOT modified. */
DWORD VerifyFrameHdrBuf(BYTE id, CONST BYTE *buf, DWORD len);
/* Erase a region page by page, feeding the independent watchdog after each
 * page (CDB 0101h Start path). A full APP slot (23 x 8KB pages) erases in
 * ~1s -- longer than the watchdog window -- so the plain EraseFlash would
 * reset the MCU mid-erase. Same semantics as EraseFlash otherwise. */
DWORD EraseFlashWithFeed(BYTE id);
/* Confirm the pending upgrade (CDB 0109h Run): write a single 64B OtaInfo_t
 * record with state=CONFIRMED into OTA metadata J-A, carrying the target
 * image's identity/CRC fields copied from slot B and incrementing the
 * persistent update_cnt. On success the bootloader promotes B -> A on the
 * next boot. Returns OK/ERR (ERR leaves the slot image untouched). */
DWORD OtaConfirm(VOID);

#endif

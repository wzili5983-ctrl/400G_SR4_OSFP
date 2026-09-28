/***************************************************************************/ /**
 *   @file   sramresource_impl.c
 *   @brief  Implementation of SRAM resource, per memory_map.h 10-区方案.
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
#include "sramresource_impl.h"

/* 越界检查（溢出安全形式）：offset / len 任一越界即 ERR。 */
static BOOL SramRangeOk(CONST SramForm_t *form, DWORD offset, DWORD len)
{
    if (offset > form->size) {
        return FALSE;
    }
    if (len > (form->size - offset)) {
        return FALSE;
    }
    return TRUE;
}

static DWORD SramReadOp(BYTE id, DWORD offset, BYTE *data, DWORD len)
{
    CONST SramForm_t *form = GetSramForm(id);
    CONST BYTE *src;
    DWORD i;

    if ((form == NULL) || (data == NULL)) {
        return ERR;
    }
    if (!SramRangeOk(form, offset, len)) {
        return ERR;
    }

    src = (CONST BYTE *)form->base + offset;
    for (i = 0U; i < len; ++i) {
        data[i] = src[i];
    }
    return OK;
}

static DWORD SramWriteOp(BYTE id, DWORD offset, CONST BYTE *data, DWORD len)
{
    CONST SramForm_t *form = GetSramForm(id);
    BYTE *dst;
    DWORD i;

    if ((form == NULL) || (data == NULL)) {
        return ERR;
    }
    if (!SramRangeOk(form, offset, len)) {
        return ERR;
    }

    dst = (BYTE *)form->base + offset;
    for (i = 0U; i < len; ++i) {
        dst[i] = data[i];
    }
    return OK;
}

static DWORD SramEraseOp(BYTE id)
{
    CONST SramForm_t *form = GetSramForm(id);
    BYTE *dst;
    DWORD i;

    if (form == NULL) {
        return ERR;
    }

    dst = (BYTE *)form->base;
    for (i = 0U; i < form->size; ++i) {
        dst[i] = 0xFFU;
    }
    return OK;
}

/* SRAM 10 区实例表，索引即 SRAM_ID_xxx，地址/容量直接引用 memory_map.h 宏，
 * 改布局只动 memory_map.h，本表自动跟随。
 * [8] size = RW 区总容量(0x12B00) - 栈(0x2000)，为计划值；
 * [9] base = SRAM 顶部 - 8KB，为计划栈顶；实际以链接结果为准。
 * [0]-[7] 挂可读写操作；[8] APP_RAM / [9] STACK 覆盖运行期活数据
 * （含当前栈），写/擦除挂 NULL 锁定为只读，公共层返回 ERR。 */
CONST SramInfo_t g_SramInfo[SRAM_ID_NUM] = {
    /* [0] 保留备用(原共享区)  512B    通用读写开放 */
    { { SRAM_ID_SHARE,    SRAM_TYPE_SHARED, 0, MEM_SHARE_BASE,          MEM_SHARE_SIZE },
      { SramReadOp, SramWriteOp, SramEraseOp } },
    /* [1] 崩溃快照区          256B    BL+APP 共用 */
    { { SRAM_ID_CRASH,    SRAM_TYPE_SHARED, 0, MEM_CRASH_SNAP_BASE,     MEM_CRASH_SNAP_SIZE },
      { SramReadOp, SramWriteOp, SramEraseOp } },
    /* [2] Flash 页缓冲区      8KB     BL+APP 共用，页编程暂存 */
    { { SRAM_ID_FLASHBUF, SRAM_TYPE_SHARED, 0, MEM_FLASH_PAGE_BUF_BASE, MEM_FLASH_PAGE_BUF_SIZE },
      { SramReadOp, SramWriteOp, SramEraseOp } },
    /* [3] 加密/哈希工作区     512B    BL+APP 共用 */
    { { SRAM_ID_CRYPTO,   SRAM_TYPE_SHARED, 0, MEM_CRYPTO_BASE,         MEM_CRYPTO_SIZE },
      { SramReadOp, SramWriteOp, SramEraseOp } },
    /* [4] OTA 接收缓冲        4KB     APP */
    { { SRAM_ID_OTA_RX,   SRAM_TYPE_APP,    0, MEM_OTA_RX_BUF_BASE,     MEM_OTA_RX_BUF_SIZE },
      { SramReadOp, SramWriteOp, SramEraseOp } },
    /* [5] DMA 缓冲区          4KB     APP，0x3500 已 32B 对齐 */
    { { SRAM_ID_DMA,      SRAM_TYPE_APP,    0, MEM_DMA_BUF_BASE,        MEM_DMA_BUF_SIZE },
      { SramReadOp, SramWriteOp, SramEraseOp } },
    /* [6] 通信缓冲区          4KB     APP */
    { { SRAM_ID_COMM,     SRAM_TYPE_APP,    0, MEM_COMM_BUF_BASE,       MEM_COMM_BUF_SIZE },
      { SramReadOp, SramWriteOp, SramEraseOp } },
    /* [7] 用户参数缓冲区      32KB    APP，↔ Flash 主参数区 */
    { { SRAM_ID_PARAM,    SRAM_TYPE_APP,    0, MEM_PARAM_BUF_BASE,      MEM_PARAM_BUF_SIZE },
      { SramReadOp, SramWriteOp, SramEraseOp } },
    /* [8] APP .data + .bss    ~66.75KB APP，起点固定、终点链接器决定。
     * 覆盖运行期活数据：只读，写/擦除挂 NULL，公共层返回 ERR。 */
    { { SRAM_ID_APP_RAM,  SRAM_TYPE_APP,    0, MEM_APP_RAM_BASE,
        (MEM_SRAM_LIMIT - MEM_APP_RAM_BASE - MEM_STACK_SIZE) },
      { SramReadOp, NULL, NULL } },
    /* [9] MSP 主栈            8KB     APP，计划位于 SRAM 顶部。
     * 含当前运行栈：只读，写/擦除挂 NULL，公共层返回 ERR。 */
    { { SRAM_ID_STACK,    SRAM_TYPE_APP,    0, (MEM_SRAM_LIMIT - MEM_STACK_SIZE),
        MEM_STACK_SIZE },
      { SramReadOp, NULL, NULL } },
};

CONST SramInfo_t *GetSramInfo(BYTE id)
{
    if (id >= SRAM_ID_NUM) {
        return NULL;
    }
    return &g_SramInfo[id];
}

CONST SramForm_t *GetSramForm(BYTE id)
{
    CONST SramInfo_t *info = GetSramInfo(id);

    if (info == NULL) {
        return NULL;
    }
    return &info->sramform;
}

BYTE *GetSramAddr(BYTE id)
{
    CONST SramForm_t *form = GetSramForm(id);

    if (form == NULL) {
        return NULL;
    }
    return (BYTE *)form->base;
}

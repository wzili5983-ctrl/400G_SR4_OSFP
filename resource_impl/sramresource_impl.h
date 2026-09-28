/***************************************************************************/ /**
 *   @file   sramresource_impl.h
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
#ifndef SRAMRESOURCE_IMPL_H
#define SRAMRESOURCE_IMPL_H

#include "basedef.h"
#include "memory_map.h"

/* ---------------------------------------------------------------------------
 * SRAM 10 区实例表，按 memory_map.h 的地址图一一对应：
 *
 *   [0] SHARE        0x2000_0000  512B   保留备用（原 BL/APP 共享工作区）
 *   [1] CRASH        0x2000_0200  256B   崩溃快照区
 *   [2] FLASHBUF     0x2000_0300  8KB    Flash 页缓冲区
 *   [3] CRYPTO       0x2000_2300  512B   加密/哈希工作区
 *   [4] OTA_RX       0x2000_2500  4KB    OTA 接收缓冲
 *   [5] DMA          0x2000_3500  4KB    DMA 缓冲区（32B 对齐）
 *   [6] COMM         0x2000_4500  4KB    通信缓冲区
 *   [7] PARAM        0x2000_5500  32KB   用户参数缓冲区（↔ Flash 主参数区）
 *   [8] APP_RAM      0x2000_D500  ~66.75KB  APP .data + .bss（起点固定，
 *                    终点由链接脚本决定，表中为计划值）
 *   [9] STACK        0x2001_E000  8KB    MSP 主栈（计划位于 SRAM 顶部，
 *                    实际位置以链接结果为准）
 *
 * [0] 保留备用：原 BL/APP 共享工作区协议已整体删除，512B 区保留，
 * 读 / 写 / 擦除经通用资源层开放，作为未来 BL/APP 协议的存放地。
 * [1]-[3] 为 BL/APP 共享区：bootloader 与 APP 工程的本头文件及
 * memory_map.h 定义必须保持一致，否则跨工程数据交换会互相踩踏。
 * ------------------------------------------------------------------------- */

/* 区号，同时是实例表 g_SramInfo[] 的索引。 */
typedef enum {
    SRAM_ID_SHARE,       /* [0] 保留备用（原 BL/APP 共享工作区） */
    SRAM_ID_CRASH,       /* [1] 崩溃快照区 */
    SRAM_ID_FLASHBUF,    /* [2] Flash 页缓冲区 */
    SRAM_ID_CRYPTO,      /* [3] 加密/哈希工作区 */
    SRAM_ID_OTA_RX,      /* [4] OTA 接收缓冲 */
    SRAM_ID_DMA,         /* [5] DMA 缓冲区 */
    SRAM_ID_COMM,        /* [6] 通信缓冲区 */
    SRAM_ID_PARAM,       /* [7] 用户参数缓冲区 */
    SRAM_ID_APP_RAM,     /* [8] APP .data + .bss */
    SRAM_ID_STACK,       /* [9] MSP 主栈 */
    SRAM_ID_NUM          /* 区数，非有效 id */
} SramId_t;

/* 归属：BL+APP 共享区两边工程都要用，APP 区仅本工程使用。 */
typedef enum {
    SRAM_TYPE_SHARED,    /* [0]-[3]，BL/APP 共用，布局必须与 BL 一致 */
    SRAM_TYPE_APP        /* [4]-[9]，仅 APP 使用 */
} SramType_t;

/* 区属性：id 与表中索引一致；base / size 与 memory_map.h 宏一一对应。
 * [8] size 为计划值（RW 区 0x12B00 - 栈 0x2000），[9] base 为计划栈顶；
 * 两者实际值以链接结果为准。 */
typedef struct {
    BYTE  id;            /* SRAM_ID_xxx，等于本表索引 */
    BYTE  type;          /* SRAM_TYPE_xxx */
    WORD  rsvd;          /* 对齐填充，写 0 */
    DWORD base;          /* 区起始地址 */
    DWORD size;          /* 区字节数 */
} SramForm_t;

/* 区操作：SRAM 内存映射，read/write 为普通内存拷贝（offset/len 必须落在
 * 区内，src 与目标区不得重叠），erase 全区填 0xFF（与 flash 擦除态一致）。
 * 函数指针允许挂 NULL 表示禁止该操作（公共层返回 ERR）：SRAM_ID_APP_RAM /
 * SRAM_ID_STACK 覆盖运行期活数据（含当前栈），其写/擦除即按此锁定，只读开放。 */
typedef struct {
    DWORD (*SramRead)(BYTE id, DWORD offset, BYTE *data, DWORD len);
    DWORD (*SramWrite)(BYTE id, DWORD offset, CONST BYTE *data, DWORD len);
    DWORD (*SramErase)(BYTE id);
} SramOps_t;

typedef struct {
    SramForm_t sramform;
    SramOps_t  sramops;
} SramInfo_t;

#define SRAM_TYPE_IS_SHARED(t)  ((t) == SRAM_TYPE_SHARED)

/* 区实例表（属性 + 操作），索引即 SRAM_ID_xxx。 */
extern CONST SramInfo_t g_SramInfo[SRAM_ID_NUM];

/* 按 id 取区实例（属性 + 操作）；id 越界返回 NULL。 */
CONST SramInfo_t *GetSramInfo(BYTE id);
/* 按 id 取区属性；id 越界返回 NULL。 */
CONST SramForm_t *GetSramForm(BYTE id);
/* 按 id 取区起始地址（可直接强转为具体类型使用）；id 越界返回 NULL。 */
BYTE *GetSramAddr(BYTE id);

/* ---------------------------------------------------------------------------
 * SRAM [0] SHARE（512B @ 0x2000_0000 = MEM_SHARE_BASE）：保留备用。
 * 原 BL/APP 共享工作区协议（BootShare_t 状态区 + 服务表 + 有效性检查）
 * 已整体删除，本区暂不使用；读 / 写 / 擦除经通用资源层开放，作为未来
 * BL/APP 协议的存放地。
 * ------------------------------------------------------------------------- */

#endif

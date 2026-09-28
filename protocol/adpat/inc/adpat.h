/** @file adpat.h
 * @brief ADPAT 适配层：management map 的物理存储适配（RAL 表 ↔ SRAM/Flash）。
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * CMIS RAL 层（protocol/ral）用 memtype 标记每个寄存器值的存储地
 * （MEMT_FLASH / MEMT_SRAM / MEMT_NONE）。ADPAT 负责把 management map
 * 的页落到真实硬件资源，本文件定义其 SRAM 页缓存：
 *
 *   g_AdpatMgmtMapBuf：一维数组，长度 = PAGE_TAB_NUM × 128B（CMIS
 *   management 单页固定 128 字节），首地址绝对定位于 SRAM [7] 用户参数
 *   缓冲区基址 + 512B 处（0x20005700）。如此 Flash 主参数区
 *   （MEM_PARAM_MAIN_BASE 0x0806C000，32KB）可原封不动整体拷贝到 [7] 区，
 *   management map 影子自然落位，拷贝后无需搬移；管理页在 Flash 参数区
 *   内的偏移同为 +512 起（前 512B 预留作参数头/其它用途，参数区布局
 *   文档需登记此约定）。
 *
 *   页槽位约定：g_OsfpTabForm[i]（页表索引 i ∈ [0, PAGE_TAB_NUM)）的
 *   SRAM 槽位 = 数组 [i*128, (i+1)*128)。Page Low、Page 00h、banked 页
 *   各占独立槽位，经 ADPAT_MGMT_MAP_PAGE(i) 寻址；RAL 引擎对 MEMT_SRAM
 *   寄存器的读写落在对应槽位的 offset 处。
 *
 * 注意：[7] 区是用户参数的 SRAM 镜像，management map 影子只占
 * ADPAT_MGMT_MAP_BUF_OFFSET 起的 PAGE_TAB_NUM×128 字节——参数管理须为
 * 其预留这段，不与其它参数用途重叠；影子的持久化（写回 Flash 主参数区）
 * 由参数区流程负责。
 */
#ifndef ADPAT_H
#define ADPAT_H

#include "basedef.h"
#include "memory_map.h"     /* MEM_PARAM_BUF_BASE / MEM_PARAM_BUF_SIZE */
#include "ral.h"            /* TabIndex_t / PAGE_TAB_NUM / g_OsfpTabForm */

#define ADPAT_MGMT_PAGE_SIZE        128U        /* CMIS management map 单页字节数 */

/* 影子在 SRAM [7] 区内的偏移：前 512B 预留（参数头/其它用途），
 * Flash 主参数区整体原样拷贝到 [7] 后管理页即落位此偏移处。 */
#define ADPAT_MGMT_MAP_BUF_OFFSET   512U

/* 数组绝对定位地址 = SRAM [7] 区基址 + 512B。独立定义为无后缀字面量，
 * 供 section 名字符串化（.ARM.__at_0x20005700，armlink 绝对定位 ZI 段
 * 约定写法，AC5/AC6 均支持）。 */
#define ADPAT_MGMT_MAP_ADDR         0x20005700
#if ((ADPAT_MGMT_MAP_ADDR != (MEM_PARAM_BUF_BASE + ADPAT_MGMT_MAP_BUF_OFFSET)) || \
     (ADPAT_MGMT_MAP_BUF_OFFSET >= MEM_PARAM_BUF_SIZE))
#error "ADPAT_MGMT_MAP_ADDR must equal MEM_PARAM_BUF_BASE + 512 (SRAM [7] + 512B)"
#endif

/* management map SRAM 页缓存：一维数组，长度 = PAGE_TAB_NUM × 128B，
 * 首地址固定于 SRAM [7] 用户参数缓冲区。ZI 放置（无初始化值），
 * 不占 RW_IRAM1。容量校验（≤32KB）在 adpat.c 以编译期断言完成。 */
extern BYTE g_AdpatMgmtMapBuf[PAGE_TAB_NUM * ADPAT_MGMT_PAGE_SIZE];

/* Protocol MSA space（= management map SRAM 影子）的总字节数 */
#define ADPAT_PROTOCOL_MSA_SIZE     ((DWORD)PAGE_TAB_NUM * ADPAT_MGMT_PAGE_SIZE)

/* 读 Protocol MSA space（management application layer 用）。
 * page_id：页表索引（TabIndex_t，即 g_OsfpTabForm 下标，如 PAGE_ID_LOW /
 * PAGE_ID_00H；banked 页的 bank 由具体枚举项体现，如 PAGE_ID_A1H_2）；
 * offset：页内地址偏移；buf/len：接收数据的缓冲及其长度，要求
 * offset + len ≤ ADPAT_MGMT_PAGE_SIZE(128)。
 * 从该页槽位的 offset 处拷贝 len 字节到 buf；页索引越界、页内范围越界、
 * buf 为空均返回 ERR，成功返回 OK。经 SRAM 公共 API ReadSram 落地。 */
DWORD ReadSramProtocolMsaSpace(TabIndex_t page_id, BYTE offset, BYTE *buf, BYTE len);

/* 页缓存槽位寻址宏：RAL 引擎经此读写 g_OsfpTabForm[i] 的 SRAM 影子 */
#define ADPAT_MGMT_MAP_PAGE(i)      (&g_AdpatMgmtMapBuf[(DWORD)(i) * ADPAT_MGMT_PAGE_SIZE])

#endif

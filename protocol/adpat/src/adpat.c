/***************************************************************************//**
 *   @file   adpat.c
 *   @brief  ADPAT 适配层实现：management map 的 SRAM 页缓存定义。
 *   @author Firmware team
 *******************************************************************************
 * Copyright 2026 Terabitcom, Inc.
 *
 * g_AdpatMgmtMapBuf 绝对定位于 SRAM [2] Flash 页缓冲区首地址
 * （0x20000300），作为 management map 的 SRAM 影子：g_OsfpTabForm[i]
 * 对应数组的 [i*128, (i+1)*128) 槽位。详见 adpat.h。
 ******************************************************************************/
#include "adpat.h"
#include "sramresource.h"   /* ReadSram / SRAM_ID_PARAM */

#define ADPAT_STR_(x)               #x
#define ADPAT_STR(x)                ADPAT_STR_(x)
#define ADPAT_AT_SEC                ".ARM.__at_" ADPAT_STR(ADPAT_MGMT_MAP_ADDR)

/* 容量校验：页缓存总长不得超过 SRAM [7] 区影子段（基址+512 起，
 * 32KB-512B = 最多 255 个管理页）。PAGE_TAB_NUM 是枚举常量，#if 不可见，
 * 用数组长度技巧做编译期断言；失败分支取 -1（负长数组硬错误），
 * 0 长数组是 GNU 扩展会被静默接受。 */
typedef BYTE AdpatSizeCheck_t[(PAGE_TAB_NUM * ADPAT_MGMT_PAGE_SIZE <= (MEM_PARAM_BUF_SIZE - ADPAT_MGMT_MAP_BUF_OFFSET)) ? 1 : -1];

/* 绝对定位段：armlink 见到 .ARM.__at_<addr> 即在 <addr> 建立执行区；
 * 无初始化值故为 ZI，无 flash 加载副本，也不占用 RW_IRAM1(0x2000D500+)。
 * 该区属 SRAM [7] 用户参数缓冲区（与 Flash 主参数区镜像），ZI 链接器
 * 不清零——上电内容由参数区加载流程负责建立。 */
BYTE g_AdpatMgmtMapBuf[PAGE_TAB_NUM * ADPAT_MGMT_PAGE_SIZE] __attribute__((section(ADPAT_AT_SEC)));

/* 读 Protocol MSA space：页表索引 + 页内偏移 + 目的缓冲，经 SRAM 公共
 * API 拷贝。banked 页的 bank 由 page_id 的具体枚举项体现（如
 * PAGE_ID_A1H_2），本函数不做 bank 运算。
 * 页槽位相对 SRAM [7] 区首的偏移 = ADPAT_MGMT_MAP_BUF_OFFSET
 *                                + page_id*128 + offset（影子起点 +512）。 */
DWORD ReadSramProtocolMsaSpace(TabIndex_t page_id, BYTE offset, BYTE *buf, BYTE len)
{
    if(((DWORD)page_id >= (DWORD)PAGE_TAB_NUM) ||
       ((DWORD)offset + (DWORD)len > (DWORD)ADPAT_MGMT_PAGE_SIZE)) {
        return ERR;                         /* 页索引越界或页内范围越界 */
    }
    return ReadSram(SRAM_ID_PARAM,
                    (DWORD)ADPAT_MGMT_MAP_BUF_OFFSET
                    + (DWORD)page_id * ADPAT_MGMT_PAGE_SIZE + (DWORD)offset,
                    buf, (DWORD)len);
}

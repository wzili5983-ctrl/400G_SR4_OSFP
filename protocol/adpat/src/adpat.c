/***************************************************************************//**
 *   @file   adpat.c
 *   @brief  ADPAT 适配层实现：management map 的 SRAM 页缓存定义。
 *   @author Firmware team
 *******************************************************************************
 * Copyright 2026 Terabitcom, Inc.
 *
 * g_AdpatMgmtMapBuf 绝对定位于 SRAM [7] 用户参数缓冲区基址 + 512B 处
 * （0x20005700），作为 management map 的 SRAM 影子：g_OsfpTabForm[i]
 * 对应数组的 [i*128, (i+1)*128) 槽位。Flash 主参数区 32KB 可原封整体
 * 拷贝进 [7] 区，影子自然落位。详见 adpat.h。
 ******************************************************************************/
#include "adpat.h"
#include "sramresource.h"   /* ReadSram / SRAM_ID_PARAM */
#include "flashresource.h"  /* ReadFlash / WriteFlash / EraseFlash / FLASH_ID_PARA_A */

#define ADPAT_STR_(x)               #x
#define ADPAT_STR(x)                ADPAT_STR_(x)
#define ADPAT_AT_SEC                ".ARM.__at_" ADPAT_STR(ADPAT_MGMT_MAP_ADDR)

/* 容量校验：页缓存总长不得超过 SRAM [7] 区影子段（基址+512 起，
 * 32KB-512B = 最多 252 个管理页）。PAGE_TAB_NUM 是枚举常量，#if 不可见，
 * 用数组长度技巧做编译期断言；失败分支取 -1（负长数组硬错误），
 * 0 长数组是 GNU 扩展会被静默接受。 */
typedef BYTE AdpatSizeCheck_t[(PAGE_TAB_NUM * ADPAT_MGMT_PAGE_SIZE <= (MEM_PARAM_BUF_SIZE - ADPAT_MGMT_MAP_BUF_OFFSET)) ? 1 : -1];

/* 绝对定位段：armlink 见到 .ARM.__at_<addr> 即在 <addr> 建立执行区；
 * 无初始化值故为 ZI，无 flash 加载副本，也不占用 RW_IRAM1(0x2000D500+)。
 * 该区属 SRAM [7] 用户参数缓冲区（与 Flash 主参数区镜像），ZI 链接器
 * 不清零——上电内容由参数区加载流程负责建立。 */
BYTE g_AdpatMgmtMapBuf[PAGE_TAB_NUM * ADPAT_MGMT_PAGE_SIZE] __attribute__((section(ADPAT_AT_SEC)));

/*================ 协议空间（影子）访问 API 实现 ================
 * 共享入参校验 + 区偏移合成：页槽位相对 SRAM [7] 区首的偏移 =
 * ADPAT_MGMT_MAP_BUF_OFFSET + page_id*128 + offset（影子起点 +512）。
 * banked 页的 bank 由 page_id 枚举项体现，本层不做 bank 运算。 */

/* 槽位区偏移合成宏 */
#define ADPAT_MSA_SLOT_OFF(page_id, offset) \
    ((DWORD)ADPAT_MGMT_MAP_BUF_OFFSET + (DWORD)(page_id) * (DWORD)ADPAT_MGMT_PAGE_SIZE + (DWORD)(offset))

/* 入参校验：页索引越界 / len=0（无意义访问）/ 页内范围越界 → ERR */
STATIC DWORD AdpatMsaChk(TabIndex_t page_id, BYTE offset, BYTE len)
{
    if(((DWORD)page_id >= (DWORD)PAGE_TAB_NUM) ||
       (len == 0U) ||
       ((DWORD)offset + (DWORD)len > (DWORD)ADPAT_MGMT_PAGE_SIZE)) {
        return ERR;
    }
    return OK;
}

/* 读：页表索引 + 页内偏移 + 目的缓冲，经 SRAM 公共 API 拷贝。 */
DWORD ReadSramProtocolMsaSpace(TabIndex_t page_id, BYTE offset, BYTE *buf, BYTE len)
{
    if((buf == NULL) || (AdpatMsaChk(page_id, offset, len) != OK)) {
        return ERR;
    }
    return ReadSram(SRAM_ID_PARAM, ADPAT_MSA_SLOT_OFF(page_id, offset), buf, (DWORD)len);
}

/* 写：源缓冲 + 页表索引 + 页内偏移，经 SRAM 公共 API 拷贝，即刻生效。
 * 仅维护 SRAM 影子；MEMT_FLASH 寄存器的持久化由参数管理流程负责。 */
DWORD WriteSramProtocolMsaSpace(TabIndex_t page_id, BYTE offset, CONST BYTE *buf, BYTE len)
{
    if((buf == NULL) || (AdpatMsaChk(page_id, offset, len) != OK)) {
        return ERR;
    }
    return WriteSram(SRAM_ID_PARAM, ADPAT_MSA_SLOT_OFF(page_id, offset), buf, (DWORD)len);
}

/* 整页 128B 便捷封装：读相位快照 / 页初始化 / 整页刷新场景。 */
DWORD ReadSramProtocolMsaPage(TabIndex_t page_id, BYTE *buf)
{
    return ReadSramProtocolMsaSpace(page_id, 0U, buf, (BYTE)ADPAT_MGMT_PAGE_SIZE);
}

DWORD WriteSramProtocolMsaPage(TabIndex_t page_id, CONST BYTE *buf)
{
    return WriteSramProtocolMsaSpace(page_id, 0U, buf, (BYTE)ADPAT_MGMT_PAGE_SIZE);
}

/*================ Flash 协议空间（用户参数·主区影子）访问 API 实现 ================
 * 槽位寻址与 SRAM 侧同构（ADPAT_MSA_SLOT_OFF，区相对偏移 = 512 起）。
 * 写路径：WriteFlash 按 8B doubleword 编程且要求对齐 + 目标已擦除，
 * 故寄存器级写入先做 doubleword 读-改-写合并（仅对齐，不豁免擦除前提）；
 * 整页 128B 天然 8 对齐，直写绕过合并。 */

/* Flash 写合并：把 [base, base+len) 向两侧扩到 8B 边界，逐 doubleword
 * 读回→叠入调用者字节→写回。前置条件：目标 doubleword 已擦除（0xFF），
 * 未擦除时 WriteFlash 校验失败返回 ERR。 */
STATIC DWORD AdpatFlashMsaWrite(DWORD base, CONST BYTE *buf, BYTE len)
{
    DWORD algn_end = (base + (DWORD)len + 7U) & ~7U;
    BYTE  merge[8];
    DWORD dw;
    DWORD i;

    for(dw = base & ~7U; dw < algn_end; dw += 8U) {
        if(ReadFlash(FLASH_ID_PARA_A, dw, merge, 8U) != OK) {
            return ERR;
        }
        for(i = 0U; i < 8U; i++) {
            DWORD cur = dw + i;
            if((cur >= base) && (cur < (base + (DWORD)len))) {
                merge[i] = buf[cur - base];
            }
        }
        if(WriteFlash(FLASH_ID_PARA_A, dw, merge, 8U) != OK) {
            return ERR;
        }
    }
    return OK;
}

/* 读：XIP 内存映射直读，字节级任意范围。 */
DWORD ReadFlashProtocolMsaSpace(TabIndex_t page_id, BYTE offset, BYTE *buf, BYTE len)
{
    if((buf == NULL) || (AdpatMsaChk(page_id, offset, len) != OK)) {
        return ERR;
    }
    return ReadFlash(FLASH_ID_PARA_A, ADPAT_MSA_SLOT_OFF(page_id, offset), buf, (DWORD)len);
}

/* 写：doubleword 合并写，目标须已擦除；ms 级、XIP 停等，仅线程上下文。 */
DWORD WriteFlashProtocolMsaSpace(TabIndex_t page_id, BYTE offset, CONST BYTE *buf, BYTE len)
{
    if((buf == NULL) || (AdpatMsaChk(page_id, offset, len) != OK)) {
        return ERR;
    }
    return AdpatFlashMsaWrite(ADPAT_MSA_SLOT_OFF(page_id, offset), buf, len);
}

/* 整页 128B：512+page*128 与 128B 均 8B 对齐，直写免合并。 */
DWORD ReadFlashProtocolMsaPage(TabIndex_t page_id, BYTE *buf)
{
    if(buf == NULL) {
        return ERR;
    }
    if((DWORD)page_id >= (DWORD)PAGE_TAB_NUM) {
        return ERR;
    }
    return ReadFlash(FLASH_ID_PARA_A, ADPAT_MSA_SLOT_OFF(page_id, 0U), buf, (DWORD)ADPAT_MGMT_PAGE_SIZE);
}

DWORD WriteFlashProtocolMsaPage(TabIndex_t page_id, CONST BYTE *buf)
{
    if(buf == NULL) {
        return ERR;
    }
    if((DWORD)page_id >= (DWORD)PAGE_TAB_NUM) {
        return ERR;
    }
    return WriteFlash(FLASH_ID_PARA_A, ADPAT_MSA_SLOT_OFF(page_id, 0U), buf, (DWORD)ADPAT_MGMT_PAGE_SIZE);
}

/* 整区擦除：4×8KB 扇区 → 0xFF，连带 512B 参数头，调用方须整体重建。
 * ms 级、XIP 停等，仅线程上下文。 */
DWORD EraseFlashParamMainArea(VOID)
{
    return EraseFlash(FLASH_ID_PARA_A);
}

/*================ memtype 分派统一封装 ================
 * 引擎按寄存器 OsfpRegAttr_t.memtype 标记直接调用，无需自行判断 SRAM/Flash。
 *   MEMT_FLASH → Flash 持久层（XIP 直读 / 8B doubleword 合并写，仅线程上下文）；
 *   MEMT_SRAM  → SRAM 影子（纯内存拷贝，无阻塞）；
 *   MEMT_NONE  → ERR（无存储：WO 触发位/派生值，由 oprfunc 或上层特判）。
 * 参数校验复用各自底层函数（页越界/范围越界/NULL/len==0 → ERR）。 */

DWORD ReadProtocolMsaSpace(BYTE memtype, TabIndex_t page_id, BYTE offset, BYTE *buf, BYTE len)
{
    if(memtype == MEMT_FLASH) {
        return ReadFlashProtocolMsaSpace(page_id, offset, buf, len);
    }
    if(memtype == MEMT_SRAM) {
        return ReadSramProtocolMsaSpace(page_id, offset, buf, len);
    }
    return ERR;   /* MEMT_NONE：无存储 */
}

DWORD WriteProtocolMsaSpace(BYTE memtype, TabIndex_t page_id, BYTE offset, CONST BYTE *buf, BYTE len)
{
    if(memtype == MEMT_FLASH) {
        return WriteFlashProtocolMsaSpace(page_id, offset, buf, len);
    }
    if(memtype == MEMT_SRAM) {
        return WriteSramProtocolMsaSpace(page_id, offset, buf, len);
    }
    return ERR;   /* MEMT_NONE：无存储 */
}

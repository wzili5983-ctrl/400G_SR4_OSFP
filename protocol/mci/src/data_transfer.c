/***************************************************************************//**
 *   @file   data_transfer.c
 *   @brief  MCI 数据搬运上下文与指令解析：当前模块操作的 page / bank
 *            （Lower Memory byte 126 BankSelect & byte 127 PageSelect），
 *            以及窗口地址 → OsfpRegAttr_t 的二分查表定位。
 *   @author Firmware team
 *******************************************************************************
 * Copyright 2026 Terabitcom, Inc.
 *
 * 上下文由 MCI 在收到 host 写 Lower Memory byte 126/127 的帧时更新，
 * 读相位快照与 RAL 引擎按此上下文确定"当前页/ bank"。指令解析流程
 * （DtParse）：
 *   1. g_MgmtMemAddr bit7 选半区：0~127 → Page Low，128~255 → 当前 page；
 *   2. 左偏二分 g_OsfpTabForm[] 得 tab 最左下标，+ bank 得匹配项下标；
 *   3. 在匹配项的 OsfpRegAttr_t 表中二分 offset 得寄存器属性。
 * 详见 data_transfer.h。
 ******************************************************************************/
#include "data_transfer.h"

#define DT_TAB_NONE     0xFFFFFFFFUL    /* 二分未命中标记 */

STATIC DtTabCtrl_t g_DtTabCtrl = {0x00U, BANK_ID_0};    /* 上电默认 page 00h / bank 0 */

VOID DtTabCtrlInit(VOID)
{
    g_DtTabCtrl.page = 0x00U;
    g_DtTabCtrl.bank = BANK_ID_0;
}

VOID DtTabCtrlSet(BYTE page, BYTE bank)
{
    g_DtTabCtrl.page = page;
    g_DtTabCtrl.bank = (DtBankId_t)(bank & 0x03U);      /* BankSelect 仅低 2 位有效 */
}

BYTE DtTabCtrlGetPage(VOID)
{
    return g_DtTabCtrl.page;
}

BYTE DtTabCtrlGetBank(VOID)
{
    return (BYTE)g_DtTabCtrl.bank;
}

/* 页表索引左偏二分：在 [lo, PAGE_TAB_NUM) 内返回 tab 最左匹配下标，
 * 未命中返回 DT_TAB_NONE。
 * 依赖 g_OsfpTabForm[] 存放约束（见 ral.c）：[0]=Page Low 固定，[1..)
 * 按 tab 升序、banked 页 4 项连续连排；元素数 == TabIndex_t 的
 * PAGE_TAB_NUM。 */
STATIC DWORD DtTabLeftFind(BYTE tab, DWORD lo)
{
    DWORD hi = (DWORD)PAGE_TAB_NUM;

    while(lo < hi) {
        DWORD mid = lo + ((hi - lo) >> 1);
        if(g_OsfpTabForm[mid].tab < tab) {
            lo = mid + 1U;
        } else {
            hi = mid;
        }
    }
    /* lo = [lo, PAGE_TAB_NUM) 内首个 tab >= 目标的下标；等于目标即最左匹配 */
    if((lo < (DWORD)PAGE_TAB_NUM) && (g_OsfpTabForm[lo].tab == tab)) {
        return lo;
    }
    return DT_TAB_NONE;
}

/* 寄存器属性二分：在 form->inf[0..num) 中精确匹配 offset，未命中返回
 * NULL（Reserved/Custom 区，由调用方按默认处理）。inf 表须按 offset
 * 升序存放。 */
STATIC CONST OsfpRegAttr_t *DtRegFind(CONST OsfpTabForm_t *form, BYTE offset)
{
    BYTE lo = 0U;
    BYTE hi = form->num;

    while(lo < hi) {
        BYTE mid = (BYTE)(lo + ((hi - lo) >> 1));
        if(form->inf[mid].offset < offset) {
            lo = (BYTE)(mid + 1U);
        } else if(form->inf[mid].offset > offset) {
            hi = mid;
        } else {
            return &form->inf[mid];
        }
    }
    return NULL;
}

CONST OsfpRegAttr_t *DtParse(BYTE memaddr)
{
    BYTE page;
    DWORD idx;
    CONST OsfpTabForm_t *form;

    if((memaddr & 0x80U) == 0U) {           /* 00h-7Fh：Lower 半区 → Page Low */
        /* Page Low 固定 g_OsfpTabForm[0]，直取不二分：其 tab 值与 Page 00h
         * 同为 0x00 会撞 LeftFind；且 bank 不影响索引（per-bank 的 Flags
         * Summary 属表内数据，非索引职责） */
        form = &g_OsfpTabForm[0];
        return DtRegFind(form, (BYTE)(memaddr & 0x7FU));
    }

    /* 80h-FFh：Upper 半区 → 当前 page，从 [1] 起搜（跳过 [0] 的 Page Low） */
    page = g_DtTabCtrl.page;

    idx = DtTabLeftFind(page, 1U);
    if(idx == DT_TAB_NONE) {
        return NULL;                        /* 页未登记 */
    }
    if(g_OsfpTabForm[idx].banked != 0U) {   /* banked 页：索引顺延 bank 个 */
        idx += (DWORD)g_DtTabCtrl.bank;
        if((idx >= (DWORD)PAGE_TAB_NUM) || (g_OsfpTabForm[idx].tab != page)) {
            return NULL;                    /* bank 越界（该页不足 4 个 bank） */
        }
    }
    /* unbanked 页：CMIS 规定 BankSelect 被忽略，不加 bank 偏移 */

    form = &g_OsfpTabForm[idx];
    return DtRegFind(form, (BYTE)(memaddr & 0x7FU));    /* offset = 半区内页内偏移 */
}

/***************************************************************************/ /**
 *   @file   bl_ral_operate.c
 *   @brief  BL 救援模式 oprfunc 裁剪集：ModuleControl / BankPageSelect / 9fCmdId。
 *   @author Firmware team
 ********************************************************************************
 * Copyright 2026 Terabitcom, Inc.
 *
 * BL 裁剪页表（bl_ral_tab.c）引用的全部 oprfunc，函数体与 ral_operate.c
 * 逐字一致（除注释裁剪）。密码子系统只保留状态 globals（cdb.c Query 命令
 * Status 字节读 g_PwdUnlocked——BL 无密码 Entry/Change 回调，恒 locked，
 * 与「救援模式未解锁」语义一致；CMIS 固件下载命令的密码门由 host 决定）。
 ******************************************************************************/
#include "ral.h"            /* PAGE_ID_LOW / LOW_ADDR_*（BL_BUILD 裁剪） */
#include "ral_operate.h"
#include "data_transfer.h"  /* DtTabCtrlSet / g_CurPageId：页/ bank 上下文 */
#include "adpat.h"          /* WriteSramProtocolMsaSpace：影子维护 */
#include "gd32e511_512.h"   /* NVIC_SystemReset：SoftwareReset */
#include "cdb.h"            /* CdbTrigger：9Fh CMDID 写触发 */

#define OSFP_PWD_HOST_DEFAULT   0x00001011UL            /* 出厂 Host Password（CMIS 定值） */

DWORD g_PwdHost = OSFP_PWD_HOST_DEFAULT;                /* 运行时 Host 密码（BL 无改密路径，恒出厂值） */
volatile BYTE g_PwdUnlocked = 0U;                       /* BL 恒 locked（无 Entry 回调） */
volatile BYTE g_PwdPersistPending = 0U;                 /* BL 恒 0（无改密路径） */

/* 0x1A Module Control：bit3 SoftwareReset → NVIC_SystemReset()（不返回）；
 * 其余位整字节写影子。写方向；OPF_IMMEDIATE。 */
DWORD OsfpOperateModuleControl(BYTE *cmd, BYTE len)
{
    if((cmd == NULL) || (len != 1U)) {
        return ERR;
    }

    if((cmd[0] & 0x08U) != 0U) {
        NVIC_SystemReset();     /* SoftwareReset：内核系统复位，函数不返回 */
    }

    (void)WriteSramProtocolMsaSpace(PAGE_ID_LOW, LOW_ADDR_MODULE_CONTROL, cmd, 1U);
    return OK;
}

/* 0x7E-0x7F BankSelect+PageSelect 合并域（RWW，2B）：cmd[0]=Bank cmd[1]=Page，
 * len 必须==2。写不支持的页 → PageSelect 自清零回 Page 00h，bank 低 2 位
 * 保持生效（g_CurPageId 哨兵判定）。写方向；OPF_IMMEDIATE。 */
DWORD OsfpOperateBankPageSelect(BYTE *cmd, BYTE len)
{
    BYTE bank;
    BYTE page;

    if((cmd == NULL) || (len != 2U)) {
        return ERR;     /* 合并域须一次写满 2B */
    }

    bank = (BYTE)(cmd[0] & 0x03U);
    page = cmd[1];

    DtTabCtrlSet(page, bank);
    if(g_CurPageId == (TabIndex_t)PAGE_TAB_NUM) {
        page = 0x00U;                   /* 页未登记（不支持）：自清零回 Page 00h */
        DtTabCtrlSet(page, bank);
    }

    /* 影子写生效值：bank 已掩码、page 可能已回零，host 读回与上下文一致 */
    cmd[0] = bank;
    cmd[1] = page;
    (void)WriteSramProtocolMsaSpace(PAGE_ID_LOW, LOW_ADDR_BANK_PAGE_SELECT, cmd, 2U);
    return OK;
}

/* 0x00-0x01 CMDID（u16 大端）：写触发命令——转发 CdbTrigger（cdb.c）置
 * busy+In Progress+pending+前台 NACK hold-off，真正执行下沉主循环
 * CdbExeBackground。写方向；OPF_IMMEDIATE。 */
DWORD OsfpOperate9fCmdId(BYTE *cmd, BYTE len)
{
    return CdbTrigger(cmd, len);
}

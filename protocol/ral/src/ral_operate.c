/***************************************************************************//**
 *   @file   ral_operate.c
 *   @brief  RAL 寄存器操作回调（oprfunc）实现。
 *   @author Firmware team
 *******************************************************************************
 * Copyright 2026 Terabitcom, Inc.
 *
 * 集中实现 ral.c 寄存器属性表里 oprfunc 字段引用的回调。回调只写「动作本身」；
 * 调用时机（ISR 同步 vs 主循环下沉）由 RAL 引擎按 OsfpRegAttr_t.opftype 决定，
 * 本文件不实现调度。方向约定见 ral_operate.h。
 *
 * 当前各回调为占位实现（返回 OK = 等效 NULL 纯存储直读直写），待引擎与
 * 页/bank 上下文（DtTabCtrl）、影子访问（ADPAT）、密码存储等接通后逐一实现。
 * TODO 注释给出各回调的最终语义。
 ******************************************************************************/
#include "ral.h"          /* OsfpRegAttr_t / OprateFunc / ACC_* / MEMT_* / LOW_ADDR_* / PAGE_TAB_NUM */
#include "ral_operate.h"
#include "data_transfer.h"/* DtTabCtrlSet / g_CurPageId：页/ bank 上下文 */
#include "adpat.h"        /* WriteSramProtocolMsaSpace：影子维护 */
#include "gd32e511_512.h" /* NVIC_SystemReset：SoftwareReset 直接请求内核系统复位 */


/*================ 密码子系统状态（CMIS 5.3 §8.2.14，寄存器 00h:118-125 / 00h:42.3-0）================
 * 密码值与锁状态由模块内部维护（规范：registers are self-clearing, the module
 * maintains the state internally）——密码区 MEMT_NONE 不落影子，host 读回值无定义。 */
#define OSFP_PWD_RES_MOD_ACC    0x01U                   /* 0001b module 密码 entry 已接受 */
#define OSFP_PWD_RES_HOST_ACC   0x02U                   /* 0010b host 密码 entry/change 已接受 */
#define OSFP_PWD_RES_REJECT     0x03U                   /* 0011b 未接受 */

#define OSFP_PWD_HOST_DEFAULT   0x00001011UL            /* 出厂 Host Password（CMIS 定值） */
#define OSFP_PWD_MODULE_DEFAULT 0x80005A5AUL            /* TODO：厂商定义 Module Password，后续移交参数管理 NV */

DWORD g_PwdHost = OSFP_PWD_HOST_DEFAULT;                /* 运行时 Host 密码；参数管理启动时以 NV 值覆写 */
STATIC DWORD g_PwdModule = OSFP_PWD_MODULE_DEFAULT;     /* Module 密码：host 不可改 */
volatile BYTE g_PwdUnlocked = 0U;                       /* MgmtInit 出态 = locked */
volatile BYTE g_PwdPersistPending = 0U;                 /* Host 密码已变更、待参数管理持久化（其清零） */

/* U32 大端拼装（密码区为 4B 大端写入） */
STATIC DWORD OsfpPwdBe32(CONST BYTE *cmd)
{
    return ((DWORD)cmd[0] << 24) | ((DWORD)cmd[1] << 16) |
           ((DWORD)cmd[2] << 8)  |  (DWORD)cmd[3];
}

/* 写 PasswordCmdResult（0x2A 低 4 位，保留位恒 0）。0x2A 属引擎专属 RO 寄存器、
 * 本密码子系统是唯一写者，单字节写原子、无并发 RMW，无需 BASEPRI 锁。 */
STATIC VOID OsfpPwdResultSet(BYTE code)
{
    BYTE b = (BYTE)(code & 0x0FU);

    (void)WriteSramProtocolMsaSpace(PAGE_ID_LOW, LOW_ADDR_MISC_STATUS, &b, 1U);
}

/* 0x1A Module Control（混合字节：bit7-4/2-0 普通 RW，bit3 SoftwareReset 为 WO/SC）。
 * 写方向；OPF_IMMEDIATE：
 *   - bit3 = 1 → 直接 NVIC_SystemReset() 请求内核系统复位（不返回）——复位
 *     后管理态自然回到 MgmtInit，无需软件安排复位时机；
 *   - bit3 = 0 → 整字节写影子（本路径写入值 bit3 已为 0，WO/SC 读回恒 0）。
 * 接线后纯存储路径不再覆盖 0x1A，影子维护是 oprfunc 职责。顺序写跨界
 * （len>1）由 len 门禁拒收——跨寄存器顺序写应落在纯存储区。 */
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

/* 0x76 PasswordChangeEntryArea（WO/SC，U32 大端，CMIS 5.3 §8.2.14 Table 8-24）。
 * 写方向；OPF_DEFERRED（主循环经 CmdExeBackground 执行）：
 *   - len 必须==4（size-matched 4 字节写）；
 *   - 仅已解锁（此前成功 entry）且新值 MSB=0（Host 范围）时接受：更新 g_PwdHost、
 *     置 g_PwdPersistPending（Flash 持久化归参数管理，本回调不直接写 NV）；
 *     写 Module 范围值效果未定义，按拒收处理、原密码保持；
 *   - 结果写 0x2A PasswordCmdResult（0010b 接受 / 0011b 拒收）。
 * 注意：改密成功不改变解锁状态（规范仅规定无效 entry 回锁）。 */
DWORD OsfpOperatePasswordChange(BYTE *cmd, BYTE len)
{
    DWORD pw;
    BYTE  res;

    if((cmd == NULL) || (len != 4U)) {
        return ERR;
    }

    pw  = OsfpPwdBe32(cmd);
    res = OSFP_PWD_RES_REJECT;
    if((g_PwdUnlocked != 0U) && ((pw & 0x80000000UL) == 0UL)) {
        g_PwdHost           = pw;
        g_PwdPersistPending = 1U;
        res                 = OSFP_PWD_RES_HOST_ACC;
    }
    OsfpPwdResultSet(res);
    return OK;
}

/* 0x7A PasswordEntryArea（WO/SC，U32 大端，CMIS 5.3 §8.2.14 Table 8-24）。
 * 写方向；OPF_DEFERRED：
 *   - len 必须==4；
 *   - 与 Module/Host 当前密码比对：命中即解锁（Module 命中 0001b，Host 命中
 *     0010b）；未命中回锁——规范：解锁态保持到「无效 entry 或模块重初始化」；
 *   - 结果写 0x2A PasswordCmdResult。
 * 校验纯 SRAM 比较，可为 IMMEDIATE；保持 DEFERRED 与声明一致，并给未来
 * NVM 慢速 Module 密码校验留窗口（1000b validation-in-progress 语义）。 */
DWORD OsfpOperatePasswordEntry(BYTE *cmd, BYTE len)
{
    DWORD pw;
    BYTE  res;

    if((cmd == NULL) || (len != 4U)) {
        return ERR;
    }

    pw  = OsfpPwdBe32(cmd);
    res = OSFP_PWD_RES_REJECT;
    if(pw == g_PwdModule) {
        g_PwdUnlocked = 1U;
        res           = OSFP_PWD_RES_MOD_ACC;
    } else if(pw == g_PwdHost) {
        g_PwdUnlocked = 1U;
        res           = OSFP_PWD_RES_HOST_ACC;
    } else {
        g_PwdUnlocked = 0U;
    }
    OsfpPwdResultSet(res);
    return OK;
}

/* 0x7E-0x7F BankSelect+PageSelect 合并域（RWW，2B）：cmd[0]=Bank(126)
 * cmd[1]=Page(127)，len 必须==2（单写 127 已在 RAL 表结构层丢弃）。
 * 语义（CMIS 5.3）：
 *   - BankSelect 仅低 2 位有效（DtTabCtrlSet 内掩码）；
 *   - 写不支持的页 → PageSelect 自清零回 Page 00h，bank 保持生效
 *     （用 g_CurPageId 哨兵判定：DtTabCtrlSet 内已按新上下文刷新解析）；
 *   - 影子维护：cmd 就地修正为生效值后写 126/127 影子，host 读回得见
 *     RWW 当前值（纯 SRAM 拷贝，ISR 安全）。
 * 返回 OK/ERR；OPF_IMMEDIATE，仅写方向触发。 */
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

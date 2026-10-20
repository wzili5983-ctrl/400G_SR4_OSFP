/***************************************************************************/ /**
 *   @file   bl_ral_tab.c
 *   @brief  BL 救援模式裁剪页表：Lower + 9Fh 四 bank。
 *   @author Firmware team
 ********************************************************************************
 * Copyright 2026 Terabitcom, Inc.
 *
 * BL 编译定义 BL_BUILD：ral.h 的 TabIndex_t 裁剪为 5 槽（PAGE_ID_LOW +
 * PAGE_ID_9FH_0..3）。本文件 g_OsfpTabForm 行号与 enum 槽号一一对应
 * （行号==ordinal==影子槽号），cdb.c 以 PAGE_ID_9FH_0 + bank 求 9Fh 槽。
 * 表项内容与 ral.c 同寄存器逐字一致（9Fh 表/ BankPageSelect 行 verbatim）：
 *   - Lower 覆盖救援所需最小集：标识 0x00-0x02（MEMT_FLASH）、0x03/0x08
 *     （状态/Flags COR——CdbStatus/CdbCmdComplete 落点）、0x1A（Software
 *     Reset，host 可复位模块）、0x25（CdbStatus1/2）、0x27（FW 版本，
 *     BL 恒 0="无固件"——改 MEMT_SRAM）、0x7E（BankPageSelect 必须，
 *     否则 host 到不了 9Fh 页）；
 *   - 9Fh 四 bank 同一属性表（CDB 命令消息页，见 ral.c verbatim）。
 * 未登记页写 → g_CurPageId 哨兵（==PAGE_TAB_NUM=5）→ BankPageSelect 自
 * 清零回 Page 00h（BL 中亦未登记）→ host 读 0xFF dummy，安全。
 ******************************************************************************/
#include "basedef.h"
#include "ral.h"            /* OsfpRegAttr_t / ACC_* / MEMT_* / OPF_* / TabIndex_t（BL_BUILD 裁剪） */
#include "ral_operate.h"    /* OsfpOperateModuleControl / OsfpOperateBankPageSelect / OsfpOperate9fCmdId */

/* Page Low 裁剪表（救援模式最小集；offset 升序，DtParse 二分依赖） */
CONST OsfpRegAttr_t g_OsfpInfPageLow[] = {
    {0x00, 1, ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* SFF8024Identifier */
    {0x01, 1, ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* CmisRevision */
    {0x02, 1, ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* MgmtChar */
    {0x03, 1, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* GlobalStatus: ModuleState */
    {0x08, 1, ACC_RO_COR, MEMT_SRAM,  NULL, OPF_NONE},   /* ModuleFlags：CdbCmdComplete 等 COR 落点 */
    {0x1A, 1, ACC_RW,     MEMT_SRAM,  OsfpOperateModuleControl, OPF_IMMEDIATE},   /* bit3 SoftwareReset→NVIC_SystemReset */
    {0x25, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* CdbStatus1/2（cdb.c 发布） */
    {0x27, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* FW Major/Minor：BL 恒 0=无固件（APP 侧存 Flash） */
    {0x7E, 2, ACC_RWW,    MEMT_SRAM,  OsfpOperateBankPageSelect, OPF_IMMEDIATE},   /* BankPageSelect：切页/ bank 入口 */
};

/* 9Fh CDB 消息页属性表（ral.c verbatim）：CMDID 写触发命令（OPF_IMMEDIATE，
 * ISR 内 CdbTrigger 置 busy+hold-off，执行下沉主循环 CdbExeBackground）。 */
CONST OsfpRegAttr_t g_OsfpInfPage9FH[] = {
    {0x00, 2,   ACC_RW,   MEMT_SRAM, OsfpOperate9fCmdId, OPF_IMMEDIATE},     /* CMDID u16BE：写触发命令 */
    {0x02, 2,   ACC_RW,   MEMT_SRAM, NULL,                OPF_NONE},          /* EPLLength u16BE（恒 0，无 EPL） */
    {0x04, 1,   ACC_RW,   MEMT_SRAM, NULL,                OPF_NONE},          /* LPLLength u8 */
    {0x05, 1,   ACC_RW,   MEMT_SRAM, NULL,                OPF_NONE},          /* CdbChkCode u8 */
    {0x06, 1,   ACC_RO,   MEMT_SRAM, NULL,                OPF_NONE},          /* RPLLength u8（模块回填） */
    {0x07, 1,   ACC_RO,   MEMT_SRAM, NULL,                OPF_NONE},          /* RPLChkCode u8（模块回填） */
    {0x08, 120, ACC_RW,   MEMT_SRAM, NULL,                OPF_NONE},          /* LPL 120B：命令负载/应答负载 */
};

/* 页表索引：行号==TabIndex_t 槽号（BL_BUILD 裁剪后 PAGE_TAB_NUM=5）。
 * [0]=Page Low（下半区直取，不参与二分）；[1..4]=9Fh 四 bank 连排
 * （banked=1，DtTabLeftFind 按 tab 升序 + bank 顺延）。 */
CONST OsfpTabForm_t g_OsfpTabForm[PAGE_TAB_NUM] = {
    {0x00, (BYTE)(sizeof(g_OsfpInfPageLow) / sizeof(g_OsfpInfPageLow[0])), 0, g_OsfpInfPageLow},   /* [0] Page Low（unbanked） */
    {0x9F, (BYTE)(sizeof(g_OsfpInfPage9FH) / sizeof(g_OsfpInfPage9FH[0])), 1, g_OsfpInfPage9FH},   /* [1] 9Fh bank0 */
    {0x9F, (BYTE)(sizeof(g_OsfpInfPage9FH) / sizeof(g_OsfpInfPage9FH[0])), 1, g_OsfpInfPage9FH},   /* [2] 9Fh bank1 */
    {0x9F, (BYTE)(sizeof(g_OsfpInfPage9FH) / sizeof(g_OsfpInfPage9FH[0])), 1, g_OsfpInfPage9FH},   /* [3] 9Fh bank2 */
    {0x9F, (BYTE)(sizeof(g_OsfpInfPage9FH) / sizeof(g_OsfpInfPage9FH[0])), 1, g_OsfpInfPage9FH},   /* [4] 9Fh bank3 */
};

/* 表行数==PAGE_TAB_NUM 编译期硬断言（enum 槽号与影子槽一一对应的基础） */
typedef BYTE RalTabFormNumCheck_t[(sizeof(g_OsfpTabForm) / sizeof(g_OsfpTabForm[0]) == (DWORD)PAGE_TAB_NUM) ? 1 : -1];

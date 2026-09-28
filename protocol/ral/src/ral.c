/***************************************************************************//**
 *   @file   ral.c
 *   @brief  CMIS RAL (Register Access Layer) 寄存器属性表：每 page 一张
 *            OsfpRegAttr_t 数组，由 g_OsfpTabForm[] 按页号索引。
 *   @author Firmware team
 *******************************************************************************
 * Copyright 2026 Terabitcom, Inc.
 *
 * 只登记 CMIS 定义的寄存器；Reserved/Custom 不进表（引擎按默认处理）。
 * memtype 约定：静态常量（ID/宣告类）= MEMT_FLASH；运行动态值 = MEMT_SRAM；
 * 触发位/派生值 = MEMT_NONE。
 ******************************************************************************/
#include "ral.h"


/* Page Low 寄存器属性表（CMIS 5.3 Table 8-5 ~ 8-25） */
CONST OsfpRegAttr_t g_OsfpInfPageLow[] = {
    /* Byte 0-3: Management Characteristics + Global Status */
    {0x00, 1, ACC_RO,     MEMT_FLASH, NULL},   /* SFF8024Identifier */
    {0x01, 1, ACC_RO,     MEMT_FLASH, NULL},   /* CmisRevision (53h=5.3) */
    {0x02, 1, ACC_RO,     MEMT_FLASH, NULL},   /* MgmtChar: MemoryModel/SteppedConfigOnly/MciMaxSpeed */
    {0x03, 1, ACC_RO,     MEMT_SRAM,  NULL},   /* GlobalStatus: ModuleState + InterruptDeasserted */
    /* Byte 4-7: Flags Summary (读汇总不清底层 Flag) */
    {0x04, 4, ACC_RO,     MEMT_SRAM,  NULL},
    /* Byte 8-13: Module-Level Flags (RO/COR) */
    {0x08, 1, ACC_RO_COR, MEMT_SRAM,  NULL},   /* CdbCmdComplete/DP FW Error/Module FW Error/StateChanged */
    {0x09, 1, ACC_RO_COR, MEMT_SRAM,  NULL},   /* Vcc+Temp Flags */
    {0x0A, 1, ACC_RO_COR, MEMT_SRAM,  NULL},   /* Aux2+Aux1 Flags */
    {0x0B, 1, ACC_RO_COR, MEMT_SRAM,  NULL},   /* Aux3+CustomMon Flags */
    {0x0D, 1, ACC_RO_COR, MEMT_SRAM,  NULL},   /* Custom Flags */
    /* Byte 14-25: Module-Level Monitors (大端 2B) */
    {0x0E, 2, ACC_RO,     MEMT_SRAM,  NULL},   /* TempMon S16 1/256°C */
    {0x10, 2, ACC_RO,     MEMT_SRAM,  NULL},   /* VccMon U16 100µV */
    {0x12, 2, ACC_RO,     MEMT_SRAM,  NULL},   /* Aux1Mon */
    {0x14, 2, ACC_RO,     MEMT_SRAM,  NULL},   /* Aux2Mon */
    {0x16, 2, ACC_RO,     MEMT_SRAM,  NULL},   /* Aux3Mon */
    {0x18, 2, ACC_RO,     MEMT_SRAM,  NULL},   /* CustomMon */
    /* Byte 26-30: Module-Level Controls */
    {0x1A, 1, ACC_RW,     MEMT_SRAM,  NULL},   /* Control: BankBroadcast/LowPwrAllowHW/Squelch/LowPwrRequestSW；
                                                  bit3 SoftwareReset 为 WO/SC，引擎对该 bit 特判 */
    {0x1B, 1, ACC_RW,     MEMT_SRAM,  NULL},   /* MciSpeedConfiguration */
    /* Byte 31-36: Module-Level Masks (默认全 1) */
    {0x1F, 6, ACC_RW,     MEMT_SRAM,  NULL},
    /* Byte 37-41: CDB 状态 / FW 版本 / 故障 */
    {0x25, 2, ACC_RO,     MEMT_SRAM,  NULL},   /* CdbStatus1/2 */
    {0x27, 2, ACC_RO,     MEMT_FLASH, NULL},   /* FW Major/Minor */
    {0x29, 1, ACC_RO,     MEMT_SRAM,  NULL},   /* ModuleFaultCause */
    /* Byte 42: Misc */
    {0x2A, 1, ACC_RO,     MEMT_SRAM,  NULL},   /* PasswordCmdResult */
    /* Byte 56-62: Extended Module Info */
    {0x38, 1, ACC_RO,     MEMT_FLASH, NULL},   /* CmisSmSupport */
    {0x39, 1, ACC_RO,     MEMT_FLASH, NULL},   /* ModuleFunctionType */
    {0x3C, 1, ACC_RO,     MEMT_FLASH, NULL},   /* SFF8024ModuleSubtype */
    {0x3D, 1, ACC_RO,     MEMT_FLASH, NULL},   /* FiberFaceType */
    {0x3E, 1, ACC_RO,     MEMT_FLASH, NULL},   /* LowPowerRestrictions */
    /* Byte 85-117: Media Type + App Descriptor 1-8 前 4 字节 */
    {0x55, 1, ACC_RO,     MEMT_FLASH, NULL},   /* MediaType */
    {0x56, 32, ACC_RO,    MEMT_FLASH, NULL},   /* App1-8: HostIfID/MediaIfID/LaneCount/LaneAssign ×8 */
    /* Byte 118-127: Password / Page Mapping */
    {0x76, 4, ACC_WO_SC,  MEMT_NONE,  NULL},   /* PasswordChangeEntryArea (U32 BE) */
    {0x7A, 4, ACC_WO_SC,  MEMT_NONE,  NULL},   /* PasswordEntryArea (U32 BE) */
    {0x7E, 1, ACC_RWW,    MEMT_SRAM,  NULL},   /* BankSelect：与 PageSelect 同一次 WRITE；越界保持原值 */
    {0x7F, 1, ACC_RWW,    MEMT_SRAM,  NULL},   /* PageSelect：写不支持页自清零回 Page 00h */
};

/* Page 00h 寄存器属性表（CMIS 5.3 §8.3 Table 8-26 ~ 8-41，Administrative
 * Information，全部静态 RO）。offset = 页内偏移（宿主窗口地址 - 80h）。
 *
 * Table 8-26 Overview 原表排印错位（地址列自 202 起偏移），下图为勘定后的
 * 字节图——依据：§8.3.3~§8.3.12 normative 文本的显式字节号 + 5.3 修订记录
 * （00h:208 加衰减节点、00h:213 加 MCI 宣告），并经 CMIS 4.0 §8.3 交叉核对：
 *   128       SFF8024IdentifierCopy
 *   129-144   VendorName        145-147 VendorOUI      148-163 VendorPN
 *   164-165   VendorRev         166-181 VendorSN       182-189 DateCode
 *   190-199   CLEICode
 *   200-201   ModulePowerCharacteristics（200 bit7-5 PowerClass，201 MaxPower/0.25W）
 *   202       CableAssemblyLinkLength（可分离光纤介质规格要求 = 0）
 *   203       ConnectorType           204-209 CopperCableAttenuation（非铜缆 = 0，含 209 Res.）
 *   210       MediaLaneUnsupported    211       FarEndConfiguration（可分离媒体 = 0）
 *   212       MediaInterfaceTechnology（850 nm VCSEL = 00h）
 *   213-214   MCIRelatedAdvertisement（213 MciFlowControlDuration + 214 Res.，5.3 新增）
 *   215-220   Reserved（继承 4.0 的 213-220 Reserved 区，5.3 插入 MCI 后剩余段）
 *   221       Custom（4.0 Table 8-15 定义为 Custom，无细节节描述）
 *   222       PageChecksum = sum(128..221) 低 8 位（§8.3.11）
 *   223-255   Custom（non-volatile，§8.3.12）
 * 本模块取值备注：400G-SR4 OSFP 光纤模块，202/204-211 按规格填 0，
 * MediaInterfaceTechnology = 00h（850 nm VCSEL）。
 * 215-221 不进表（Reserved/Custom），但其读值参与 PageChecksum，镜像中
 * 必须为确定值（约定 0x00）。 */
CONST OsfpRegAttr_t g_OsfpInfPage00H[] = {
    /* Byte 128: Identifier Copy */
    {0x00, 1,  ACC_RO,     MEMT_FLASH, NULL},   /* SFF8024IdentifierCopy：= Lower 00h:0 */
    /* Byte 129-199: Vendor Information（ASCII） */
    {0x01, 16, ACC_RO,     MEMT_FLASH, NULL},   /* VendorName (129-144) */
    {0x11, 3,  ACC_RO,     MEMT_FLASH, NULL},   /* VendorOUI (145-147) */
    {0x14, 16, ACC_RO,     MEMT_FLASH, NULL},   /* VendorPN (148-163) */
    {0x24, 2,  ACC_RO,     MEMT_FLASH, NULL},   /* VendorRev (164-165) */
    {0x26, 16, ACC_RO,     MEMT_FLASH, NULL},   /* VendorSN (166-181) */
    {0x36, 8,  ACC_RO,     MEMT_FLASH, NULL},   /* DateCode (182-189) */
    {0x3E, 10, ACC_RO,     MEMT_FLASH, NULL},   /* CLEICode (190-199) */
    /* Byte 200-209: 功率 / 长度 / 连接器 / 铜缆衰减 */
    {0x48, 2,  ACC_RO,     MEMT_FLASH, NULL},   /* ModulePowerCharacteristics (200-201)：200 bit7-5 PowerClass，201 MaxPower/0.25W */
    {0x4A, 1,  ACC_RO,     MEMT_FLASH, NULL},   /* CableAssemblyLinkLength (202)：可分离光纤介质规格要求填 0 */
    {0x4B, 1,  ACC_RO,     MEMT_FLASH, NULL},   /* ConnectorType (203)：码值见 SFF-8024 Table 4-3 */
    {0x4C, 6,  ACC_RO,     MEMT_FLASH, NULL},   /* CopperCableAttenuation (204-209)：非铜缆规格规定全 0（含 209 Reserved） */
    /* Byte 210-214: Lane / FarEnd / 媒体技术 / MCI 宣告 */
    {0x52, 1,  ACC_RO,     MEMT_FLASH, NULL},   /* MediaLaneUnsupported (210)：SR4 四通道全支持 = 0 */
    {0x53, 1,  ACC_RO,     MEMT_FLASH, NULL},   /* FarEndConfiguration (211)：可分离媒体 = 0 */
    {0x54, 1,  ACC_RO,     MEMT_FLASH, NULL},   /* MediaInterfaceTechnology (212) = 00h：850 nm VCSEL */
    {0x55, 2,  ACC_RO,     MEMT_FLASH, NULL},   /* MCIRelatedAdvertisement (213-214)：213 = MciFlowControlDuration(bit7 Encoding + bit6-0 Duration)，214 Reserved；仅 SPIMCI 有意义，I2C 模块 = 0 */
    /* Byte 215-220 Reserved / 221 Custom：不进表，但读值参与 PageChecksum，
     * 镜像中约定填 0x00 */
    /* Byte 222: Page Checksum */
    {0x5E, 1,  ACC_RO,     MEMT_FLASH, NULL},   /* PageChecksum (222) = sum(128..221) 低 8 位，需与镜像数据一致 */
    /* Byte 223-255: Custom（non-volatile），不进表 */
};

/* 页表索引：页号(PageSelect 值) -> 本页寄存器属性表。
 * 整页只读/未实现 page 无索引项。
 * 存放约束（data_transfer 的 DtParse 二分依赖）：
 *   1) g_OsfpTabForm[0] 固定为 Page Low：下半区(00h-7Fh)直取 [0]，不参与
 *      二分，bank 不影响索引（per-bank 的 Flags Summary 是表内数据的事）；
 *   2) [1..) 按 tab 升序存放；banked 页同 tab 连续 4 项连排（最左项 = bank0，
 *      banked 成员 = 1），unbanked 页 banked = 0；元素数 == TabIndex_t 的
 *      PAGE_TAB_NUM；
 *   3) Page Low 与 Page 00h 的 tab 值同为 0x00（CMIS 页号本就如此，Page
 *      Select=00h 即 Page 00h），靠宿主窗口半区区分——故下半区禁用二分；
 *   4) 各 inf 表按 offset 升序存放。 */
CONST OsfpTabForm_t g_OsfpTabForm[PAGE_TAB_NUM] = {
    {0x00, (BYTE)(sizeof(g_OsfpInfPageLow) / sizeof(g_OsfpInfPageLow[0])), 0, g_OsfpInfPageLow},    /* [0] Page Low（unbanked） */
    {0x00, (BYTE)(sizeof(g_OsfpInfPage00H) / sizeof(g_OsfpInfPage00H[0])), 0, g_OsfpInfPage00H},    /* [1] Page 00h（unbanked） */
};

/* 编译期一致性断言：g_OsfpTabForm[] 元素数必须与 TabIndex_t 的
 * PAGE_TAB_NUM 一致（DtParse 二分以 PAGE_TAB_NUM 为上界）。
 * 定义处当前按 [PAGE_TAB_NUM] 定长书写本已恒等，此断言防的是将来被
 * 改成无长度 [] 定义后与枚举悄悄失配。失败分支取 -1（负长数组硬错误）：
 * 0 长数组是 GNU 扩展，gcc/armclang 均会静默接受，断言形同虚设。
 * PAGE_TAB_NUM 是枚举常量，#if 不可见，用数组长度技巧。 */
typedef BYTE RalTabFormNumCheck_t[(sizeof(g_OsfpTabForm) / sizeof(g_OsfpTabForm[0]) == (DWORD)PAGE_TAB_NUM) ? 1 : -1];

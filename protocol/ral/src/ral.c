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
 * opftype 约定：当前各表 oprfunc 均 NULL，故一律 OPF_NONE；待引擎接入
 * oprfunc 时，短耗时/无阻塞的即时操作标 OPF_IMMEDIATE，需置标志下沉
 * 主循环的操作标 OPF_DEFERRED。
 ******************************************************************************/
#include "ral.h"
#include "ral_operate.h"    /* OsfpOperate*：表项 oprfunc 接线 */


/* Page Low 寄存器属性表（CMIS 5.3 Table 8-5 ~ 8-25） */
CONST OsfpRegAttr_t g_OsfpInfPageLow[] = {
    /* Byte 0-3: Management Characteristics + Global Status */
    {0x00, 1, ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* SFF8024Identifier */
    {0x01, 1, ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* CmisRevision (53h=5.3) */
    {0x02, 1, ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* MgmtChar: MemoryModel/SteppedConfigOnly/MciMaxSpeed */
    {0x03, 1, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* GlobalStatus: ModuleState + InterruptDeasserted */
    /* Byte 4-7: Flags Summary (读汇总不清底层 Flag) */
    {0x04, 4, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},
    /* Byte 8-13: Module-Level Flags (RO/COR) */
    {0x08, 1, ACC_RO_COR, MEMT_SRAM,  NULL, OPF_NONE},   /* CdbCmdComplete/DP FW Error/Module FW Error/StateChanged */
    {0x09, 1, ACC_RO_COR, MEMT_SRAM,  NULL, OPF_NONE},   /* Vcc+Temp Flags */
    {0x0A, 1, ACC_RO_COR, MEMT_SRAM,  NULL, OPF_NONE},   /* Aux2+Aux1 Flags */
    {0x0B, 1, ACC_RO_COR, MEMT_SRAM,  NULL, OPF_NONE},   /* Aux3+CustomMon Flags */
    {0x0D, 1, ACC_RO_COR, MEMT_SRAM,  NULL, OPF_NONE},   /* Custom Flags */
    /* Byte 14-25: Module-Level Monitors (大端 2B) */
    {0x0E, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* TempMon S16 1/256°C */
    {0x10, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* VccMon U16 100µV */
    {0x12, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* Aux1Mon */
    {0x14, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* Aux2Mon */
    {0x16, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* Aux3Mon */
    {0x18, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* CustomMon */
    /* Byte 26-30: Module-Level Controls */
    {0x1A, 1, ACC_RW,     MEMT_SRAM,  OsfpOperateModuleControl, OPF_IMMEDIATE},   /* Control：bit3 SoftwareReset→NVIC_SystemReset；其余位写影子（bit3 读回 0） */
    {0x1B, 1, ACC_RW,     MEMT_SRAM,  NULL, OPF_NONE},   /* MciSpeedConfiguration */
    /* Byte 31-36: Module-Level Masks (默认全 1) */
    {0x1F, 6, ACC_RW,     MEMT_SRAM,  NULL, OPF_NONE},
    /* Byte 37-41: CDB 状态 / FW 版本 / 故障 */
    {0x25, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* CdbStatus1/2 */
    {0x27, 2, ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* FW Major/Minor */
    {0x29, 1, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* ModuleFaultCause */
    /* Byte 42: Misc */
    {0x2A, 1, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* PasswordCmdResult */
    /* Byte 56-62: Extended Module Info */
    {0x38, 1, ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* CmisSmSupport */
    {0x39, 1, ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* ModuleFunctionType */
    {0x3C, 1, ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* SFF8024ModuleSubtype */
    {0x3D, 1, ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* FiberFaceType */
    {0x3E, 1, ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* LowPowerRestrictions */
    /* Byte 85-117: Media Type + App Descriptor 1-8 前 4 字节 */
    {0x55, 1, ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* MediaType */
    {0x56, 32, ACC_RO,    MEMT_FLASH, NULL, OPF_NONE},   /* App1-8: HostIfID/MediaIfID/LaneCount/LaneAssign ×8 */
    /* Byte 118-127: Password / Page Mapping */
    {0x76, 4, ACC_WO_SC,  MEMT_NONE,  OsfpOperatePasswordChange, OPF_DEFERRED},   /* PasswordChangeEntryArea (U32 BE)：解锁+Host 范围才接受 */
    {0x7A, 4, ACC_WO_SC,  MEMT_NONE,  OsfpOperatePasswordEntry, OPF_DEFERRED},    /* PasswordEntryArea (U32 BE)：比对解锁，结果写 0x2A */
    /* Byte 126-127: BankSelect + PageSelect 合并域（CMIS 5.3：二者须同一次
     * WRITE）。cmd[0]=BankSelect(126) cmd[1]=PageSelect(127)，len 须=2；
     * 表内不再有 0x7F 独立项——单独写 127 不命中任何寄存器被丢弃，同写约束
     * 由此在 RAL 表结构层强制。oprfunc 切换页/ bank 上下文并维护影子。 */
    {0x7E, 2, ACC_RWW,    MEMT_SRAM,  OsfpOperateBankPageSelect, OPF_IMMEDIATE},   /* BankPageSelect：不支持页自清零回 00h，bank 低 2 位 */
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
    {0x00, 1,  ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* SFF8024IdentifierCopy：= Lower 00h:0 */
    /* Byte 129-199: Vendor Information（ASCII） */
    {0x01, 16, ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* VendorName (129-144) */
    {0x11, 3,  ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* VendorOUI (145-147) */
    {0x14, 16, ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* VendorPN (148-163) */
    {0x24, 2,  ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* VendorRev (164-165) */
    {0x26, 16, ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* VendorSN (166-181) */
    {0x36, 8,  ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* DateCode (182-189) */
    {0x3E, 10, ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* CLEICode (190-199) */
    /* Byte 200-209: 功率 / 长度 / 连接器 / 铜缆衰减 */
    {0x48, 2,  ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* ModulePowerCharacteristics (200-201)：200 bit7-5 PowerClass，201 MaxPower/0.25W */
    {0x4A, 1,  ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* CableAssemblyLinkLength (202)：可分离光纤介质规格要求填 0 */
    {0x4B, 1,  ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* ConnectorType (203)：码值见 SFF-8024 Table 4-3 */
    {0x4C, 6,  ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* CopperCableAttenuation (204-209)：非铜缆规格规定全 0（含 209 Reserved） */
    /* Byte 210-214: Lane / FarEnd / 媒体技术 / MCI 宣告 */
    {0x52, 1,  ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* MediaLaneUnsupported (210)：SR4 四通道全支持 = 0 */
    {0x53, 1,  ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* FarEndConfiguration (211)：可分离媒体 = 0 */
    {0x54, 1,  ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* MediaInterfaceTechnology (212) = 00h：850 nm VCSEL */
    {0x55, 2,  ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* MCIRelatedAdvertisement (213-214)：213 = MciFlowControlDuration(bit7 Encoding + bit6-0 Duration)，214 Reserved；仅 SPIMCI 有意义，I2C 模块 = 0 */
    /* Byte 215-220 Reserved / 221 Custom：不进表，但读值参与 PageChecksum，
     * 镜像中约定填 0x00 */
    /* Byte 222: Page Checksum */
    {0x5E, 1,  ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* PageChecksum (222) = sum(128..221) 低 8 位，需与镜像数据一致 */
    /* Byte 223-255: Custom（non-volatile），不进表 */
};

/* Page 01h 寄存器属性表（CMIS 5.3 §8.4 Table 8-42 ~ 8-60，Advertising，
 * paged 模块必配的只读静态能力清单）。offset = 页内偏移（宿主窗口 128-255
 * 减 80h）。本页全部静态 RO，统一 ACC_RO/MEMT_FLASH（无 RW/WO/COR）。
 *
 * 字节图（Table 8-42，含进表/不进表划分）：
 *   128-131    InactiveFW/HW Revisions（128-129 不参与 checksum）
 *   132-137    Supported Link Length（SMF/OM5/OM4/OM3/OM2）
 *   138-141    Wavelength Information（NominalWavelength + Tolerance）
 *   142        Supported Pages（BanksSupported/Page03h/CmisFf/Coherent/Diag/VDM/NP）
 *   143-144    Durations（ModSelWaitTime m·2^e + MaxDurationDPInit/Deinit）
 *   145-154    Module Characteristics（TxInputClocking、Aux Mon 含义等）
 *   155-156    Supported Controls      157-158 Supported Flags
 *   159-160    Supported Monitors      161-162 Supported SI Controls
 *   163-166    CDB Advertisement      167-169 Additional Durations
 *   170-174    Reserved（不进表）
 *   175        NAD Support
 *   176-190    MediaLaneAssignmentOptions App1-15
 *   191-222    Custom（不进表）
 *   223-250    AppDescriptor 9-15 前 4 字节
 *   251-254    Misc Advertisements（FullPageRead/Password/ScratchPad 等）
 *   255        PageChecksum = sum(130..254) 低 8 位（128-129 有意排除）
 * 170-174 Reserved / 191-222 Custom 不进表，读值按默认 0x00 处理。
 * 本模块取值要点：8-lane 模块 BanksSupported=00（142.1-0）；SR4 宣告
 * TxInputClockingCapabilities=11（145.6-5，lanes 可异步）。 */
CONST OsfpRegAttr_t g_OsfpInfPage01H[] = {
    /* Byte 128-131: Inactive FW / HW 版本 */
    {0x00, 4,  ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* InactiveFwMaj(128)/Min(129)/HwMaj(130)/HwMin(131)；128-129 不参与 checksum */
    /* Byte 132-137: Supported Link Length */
    {0x04, 6,  ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* SMF(132)/OM5(133)/OM4(134)/OM3(135)/OM2(136)/SMF2 Multiplier(137) */
    /* Byte 138-141: Wavelength Information */
    {0x0A, 4,  ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* NominalWavelength U16 (138-139, 0.05nm) + WavelengthTolerance U16 (140-141, 0.005nm) */
    /* Byte 142: Supported Pages */
    {0x0E, 1,  ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* bit7 NetworkPath bit6 VDM bit5 Diag bit4 Coherent bit3 CmisFf bit2 Page03h bit1-0 BanksSupported */
    /* Byte 143-144: Durations */
    {0x0F, 2,  ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* 143 ModSelWaitTime m·2^e；144 MaxDurationDPDeinit/DPInit（4-bit，Table 8-48） */
    /* Byte 145-154: Module Characteristics */
    {0x11, 10, ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* 145 Cooling/TxInputClocking(6-5)/ePPS/Page15h/Aux1-3Mon含义(2-0)；
                                                   146-147 TempMax/Min S8；148-149 PropagationDelay；150 OperatingVoltageMin；
                                                   151 Detector/RxEq/LOS/TxDisable 特性；152 CDRPowerSaved；153 RxOutLevel/TxInputEqMax；
                                                   154 RxOutputEqPre/PostCursorMax */
    /* Byte 155-156: Supported Controls */
    {0x1B, 2,  ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* 155 WavelengthCtrl/Tunable/Squelch/ForcedSquelch/AutoSquelch/OutDisable/InputPolFlip(Tx)；
                                                   156 BankBroadcast/AutoSquelchRx/OutDisableRx/OutputPolFlipRx */
    /* Byte 157-158: Supported Flags */
    {0x1D, 2,  ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* 157 AdaptiveEqFail/CDRLOL/LOS/Failure (Tx)；158 CDRLOL/LOS (Rx) */
    /* Byte 159-160: Supported Monitors */
    {0x1F, 2,  ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* 159 Custom/Aux3/Aux2/Aux1/Vcc/Temp Mon；160 TxBias/RxPwr/TxPwr/TxBiasScaling */
    /* Byte 161-162: Supported SI Controls */
    {0x21, 2,  ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* 161 TxInputEqRecall/Freeze/Adaptive/HostCtrl/TxCDRBypass/TxCDR；162 VCS/UnidirReconfig/SCS1/RxOutputEq/RxCDR */
    /* Byte 163-166: CDB Advertisement */
    {0x23, 4,  ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* 163 Instances(7-6)/Background(5)/AutoPaging(4)/MaxPagesEPL(3-0)；164 ReadWriteLengthExtension；
                                                   165 TriggerMethod(7)/ExtMaxBusyTime(4-0)；166 MaxBusySpecMethod(7)/MaxBusyTime(6-0) */
    /* Byte 167-169: Additional Durations */
    {0x27, 3,  ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* 167 ModulePwrDn/Up；168 DPTxTurnOff/On；169 MaxDurationBPC(3-0) */
    /* Byte 170-174 Reserved：不进表，读值按默认 0x00 处理 */
    /* Byte 175: NAD Support */
    {0x2F, 1,  ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* bit3-0 NADBanksSupported：0=不支持，n=Page 1Ch n 个 Bank */
    /* Byte 176-190: MediaLaneAssignmentOptions App1-15 */
    {0x30, 15, ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* App1..15 各 1B，bit0-7 对应 Media Lane 1-8 起始位（与 AppDescriptor 经 AppSel 关联） */
    /* Byte 191-222 Custom：不进表，读值按默认 0x00 处理 */
    /* Byte 223-250: AppDescriptor 9-15 前 4 字节 */
    {0x5F, 28, ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* App9..15 各 4B：HostIfID/MediaIfID/LaneCount/LaneAssign（布局同 Lower 0x56 App1-8） */
    /* Byte 251-254: Misc Advertisements */
    {0x7B, 4,  ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* 251 ScratchPad(7-6)/PasswordEntry(5-4)/PasswordEntryResult(3-2)/FullPageRead(1-0)；
                                                   252 HostLaneSwitching(7)/LinkTraining(6) */
    /* Byte 255: Page Checksum */
    {0x7F, 1,  ACC_RO,     MEMT_FLASH, NULL, OPF_NONE},   /* PageChecksum = sum(130..254) 低 8 位，需与镜像数据一致（128-129 排除） */
};

/* Page 02h 寄存器属性表（CMIS 5.3 §8.5 Table 8-61 ~ 8-63，Module and Lane
 * Thresholds，paged 模块必配的只读阈值表）。offset = 页内偏移（宿主窗口
 * 128-255 减 80h）。全部字段只读（§8.5.1「All fields on Page 02h are
 * read-only」），统一 ACC_RO；阈值随 commissioned Application 变化（Data
 * Path / Network Path 初始化时更新），故 memtype = MEMT_SRAM（运行动态值，
 * 引擎在应用委任时刷新）。
 *
 * 字节图（Table 8-61）：
 *   128-175    Module-level monitor thresholds（6 monitor × 4 阈值 × 2B）
 *   176-199    Lane-specific monitor thresholds（3 monitor × 4 阈值 × 2B）
 *   200-229    Reserved（不进表）
 *   230-254    Custom（不进表）
 *   255        PageChecksum = sum(128..254) 低 8 位
 * 每个 monitor 的 4 阈值顺序：HighAlarm / LowAlarm / HighWarning / LowWarning
 * （各 2B 大端）。lane 阈值作用于模块全部 lane（§8.5.2），非 banked。
 * 200-229 Reserved / 230-254 Custom 不进表，读值按默认 0x00 处理。
 * 阈值单位与 Lower Memory 对应 monitor 一致：Temp 1/256°C、Vcc 100µV、
 * 光功率 0.1µW、LaserBias 2µA × 倍率（01h:160.4-3）。 */
CONST OsfpRegAttr_t g_OsfpInfPage02H[] = {
    /* Byte 128-175: Module-Level Monitor Thresholds */
    {0x00, 8,  ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* TempMon High/Low Alarm/Warning（S16，1/256°C） */
    {0x08, 8,  ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* VccMon High/Low Alarm/Warning（U16，100µV） */
    {0x10, 8,  ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* Aux1Mon High/Low Alarm/Warning（S16，TEC 电流或 Custom） */
    {0x18, 8,  ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* Aux2Mon High/Low Alarm/Warning（S16，TEC 电流或 Laser Temp） */
    {0x20, 8,  ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* Aux3Mon High/Low Alarm/Warning（S16，Laser Temp 或 Vcc2） */
    {0x28, 8,  ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* CustomMon High/Low Alarm/Warning（S16/U16） */
    /* Byte 176-199: Lane-Related Monitor Thresholds */
    {0x30, 8,  ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* OpticalPowerTx High/Low Alarm/Warning（U16，0.1µW） */
    {0x38, 8,  ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* LaserBiasCurrent High/Low Alarm/Warning（U16，2µA × 倍率 01h:160.4-3） */
    {0x40, 8,  ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* OpticalPowerRx High/Low Alarm/Warning（U16，0.1µW） */
    /* Byte 200-229 Reserved / 230-254 Custom：不进表，读值按默认 0x00 处理 */
    /* Byte 255: Page Checksum */
    {0x7F, 1,  ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* PageChecksum = sum(128..254) 低 8 位；阈值变化时引擎须重算 */
};

/* Page 10h 寄存器属性表（CMIS 5.3 §8.9 Table 8-67 ~ 8-81，Banked Page：
 * Lane Control and Data Path Control）。banked 页：每 bank = 一组 8 条
 * host lane，bank↔lane 对应由应用/DPSM 层解释，引擎按 bank 选
 * PAGE_ID_10H_0..3 影子槽。offset = 页内偏移（宿主窗口地址 - 80h），
 * 连续同属性字段合并为大 size 项（DtRegFind 精确匹配写起始 offset）。
 * Apply* 触发寄存器（ApplyDPInit/ApplyImmediate/ApplyImmediateTx/Rx）登记为
 * WO_SC + MEMT_NONE：影子不落数据（spec：WO 读值未指定）；Provision /
 * Provision-and-Commission 语义与 ConfigStatus 反馈待 DPSM 实现后接
 * oprfunc，当前 host 写入被丢弃——等效 spec 的 "WRITE access is ignored"。
 * SteppedConfigOnly=1b（00h:2.6）→ ApplyImmediate 不支持，按 spec 写忽略，
 * 登记 RW 影子即「忽略」语义。
 * 0x69-0x7F（宿主 233-255：Reserved[7] + Custom[16]）不进表：读回 boot
 * 零镜像 0x00（§8.4.17 PageChecksum 仅定义 01h:255，10h 无 checksum）。 */
CONST OsfpRegAttr_t g_OsfpInfPage10H[] = {
    /* 128: Data Path Control */
    {0x00, 1,  ACC_RW,    MEMT_SRAM, NULL, OPF_NONE},    /* DPDeinitLane1-8（RW Rqd；0b=初始化/1b=去初始化，DPSM 语义待实现） */
    /* 129-142: Lane-Specific Direct Effect Control（RW Adv，App 层按 01h:155-156 宣告逐位支持） */
    {0x01, 14, ACC_RW,    MEMT_SRAM, NULL, OPF_NONE},    /* InputPolarityFlipTx/OutputDisableTx/AutoSquelchDisableTx/OutputSquelchForceTx/AdaptiveInputEqFreezeTx/AdaptiveInputEqStoreTx/OutputPolarityFlipRx/OutputDisableRx/AutoSquelchDisableRx（含 0x07/0x0A 位保留） */
    /* 143-177: Staged Control Set 0（SCS0；表 8-70~8-75） */
    {0x0F, 1,  ACC_WO_SC, MEMT_NONE, NULL, OPF_NONE},    /* SCS0::ApplyDPInitLane1-8（WO Rqd；单字节写限制未在引擎强制，语义待 DPSM） */
    {0x10, 1,  ACC_RW,    MEMT_SRAM, NULL, OPF_NONE},    /* SCS0::ApplyImmediateLane1-8（SteppedConfigOnly=1 → 不支持，写忽略=存影子） */
    {0x11, 8,  ACC_RW,    MEMT_SRAM, NULL, OPF_NONE},    /* SCS0::DPConfigLane1-8（AppSelCode/DataPathID/ExplicitControl，RW Rqd） */
    {0x19, 23, ACC_RW,    MEMT_SRAM, NULL, OPF_NONE},    /* SCS0 Tx/Rx SI 控制（宿主153-175）：AdaptiveInputEqEnableTx/RecallTx/HostControlledInputEqTargetTx/CDREnableTx/CDREnableRx/OutputEqPre/PostCursorTargetRx/OutputAmplitudeTargetRx */
    {0x30, 1,  ACC_WO_SC, MEMT_NONE, NULL, OPF_NONE},    /* SCS0::ApplyImmediateTxLane1-8（宿主176；WO Cnd；01h:162.6=0 不支持，写丢弃） */
    {0x31, 1,  ACC_WO_SC, MEMT_NONE, NULL, OPF_NONE},    /* SCS0::ApplyImmediateRxLane1-8（宿主177；同上） */
    /* 178-212: Staged Control Set 1（SCS1；表 8-76~8-80，行为同 SCS0，01h:162.5=0 未宣告支持） */
    {0x32, 1,  ACC_WO_SC, MEMT_NONE, NULL, OPF_NONE},    /* SCS1::ApplyDPInitLane1-8（宿主178） */
    {0x33, 1,  ACC_RW,    MEMT_SRAM, NULL, OPF_NONE},    /* SCS1::ApplyImmediateLane1-8（宿主179；不支持，写忽略） */
    {0x34, 8,  ACC_RW,    MEMT_SRAM, NULL, OPF_NONE},    /* SCS1::DPConfigLane1-8（宿主180-187） */
    {0x3C, 23, ACC_RW,    MEMT_SRAM, NULL, OPF_NONE},    /* SCS1 Tx/Rx SI 控制（宿主188-210；同 SCS0 布局） */
    {0x53, 1,  ACC_WO_SC, MEMT_NONE, NULL, OPF_NONE},    /* SCS1::ApplyImmediateTxLane1-8（宿主211） */
    {0x54, 1,  ACC_WO_SC, MEMT_NONE, NULL, OPF_NONE},    /* SCS1::ApplyImmediateRxLane1-8（宿主212） */
    /* 213-232: Lane-Specific Masks（表 8-81；DPStateChangedMask RW Rqd，其余
     * RW Adv——未宣告支持的掩码由 app 层保持 0，host 写入照常落影子） */
    {0x55, 20, ACC_RW,    MEMT_SRAM, NULL, OPF_NONE},
};

/* Page 11h 寄存器属性表（CMIS 5.3 §8.10 Table 8-82 ~ 8-97，Banked Page：
 * Lane Status and Data Path Status）。banked 语义同 Page 10h（每 bank = 8
 * 条 lane，影子槽 PAGE_ID_11H_0..3）。全页字段只读——动态状态由 DPSM /
 * 监视任务写影子，host 只读；写路径表项仅承担 RO 门禁。
 * offset = 页内偏移（宿主窗口地址 - 80h）。COR 区（Flags）读后清位归
 * app 层（flags&=~快照值）；ACS/ConfigStatus 由 DPSM 在 Apply 命令
 * 执行后回填（10h 的 Apply* 触发寄存器接 oprfunc 时联动）。
 * 0x6C-0x6F（宿主 236-239 Reserved[4]）不进表：读回 0x00；11h 无
 * PageChecksum（同 10h，§8.4.17 只管 01h:255）。 */
CONST OsfpRegAttr_t g_OsfpInfPage11H[] = {
    /* 128-131: Data Path States（表 8-83/8-84；RO Rqd；每半字节一个 lane，
     * 编码 1h=DPDeactivated/2h=DPInit/3h=DPDeinit/4h=DPActivated/5h=DPTxTurnOn/
     * 6h=DPTxTurnOff/7h=DPInitialized，DPSM 维护，未用 lane=1h） */
    {0x00, 4,  ACC_RO,     MEMT_SRAM, NULL, OPF_NONE},
    /* 132-133: Lane Output Status（表 8-85；OutputStatusRx/Tx，RO Rqd） */
    {0x04, 2,  ACC_RO,     MEMT_SRAM, NULL, OPF_NONE},
    /* 134-153: Lane-Specific Flags（表 8-86/8-87/8-88；RO/COR；
     * DPStateChangedFlag(134) 与 OutputStatusChangedFlagRx(153) Rqd，
     * 其余 Adv——未支持的 Flag 由 app 层保持 0） */
    {0x06, 1,  ACC_RO_COR, MEMT_SRAM, NULL, OPF_NONE},    /* DPStateChangedFlag */
    {0x07, 19, ACC_RO_COR, MEMT_SRAM, NULL, OPF_NONE},    /* Tx Flags(135-146) + Rx Flags(147-153) */
    /* 154-201: Lane-Specific Monitors（表 8-89；RO Adv；U16 大端：
     * OpticalPowerTx 0x1A/0.1µW、LaserBiasTx 0x2A/2µA×倍率、
     * OpticalPowerRx 0x3A/0.1µW——监视任务周期写影子） */
    {0x1A, 48, ACC_RO,     MEMT_SRAM, NULL, OPF_NONE},
    /* 202-205: ConfigStatusLane1-8（表 8-90/8-91；RO Rqd；半字节/lane，
     * 0h=无状态/1h=成功/Ch=进行中/2h-Bh=拒收原因，Apply 命令协议的结果反馈） */
    {0x4A, 4,  ACC_RO,     MEMT_SRAM, NULL, OPF_NONE},
    /* 206-234: Active Control Set（表 8-93/8-94/8-95；Provision 成功后由
     * DPSM 从 SCS 拷贝回填：DPConfig 0x4E(RO Rqd)、Tx 控制 0x56、
     * Rx 控制 0x5E(CDREnableRx 1B + Pre/PostCursor/AmplitudeTargetRx 各 4B)） */
    {0x4E, 8,  ACC_RO,     MEMT_SRAM, NULL, OPF_NONE},
    {0x56, 8,  ACC_RO,     MEMT_SRAM, NULL, OPF_NONE},
    {0x5E, 13, ACC_RO,     MEMT_SRAM, NULL, OPF_NONE},
    /* 235: DPInitPendingLane1-8（表 8-96；RO Rqd；ApplyDPInit 成功后由 DPSM 置位） */
    {0x6B, 1,  ACC_RO,     MEMT_SRAM, NULL, OPF_NONE},
    /* 240-255: Media Lane↔Wavelength/Fiber Mapping（表 8-97；RO Cnd；
     * WDM 应用才使用，SR4 恒 0——由 boot 镜像决定，不登记亦可） */
    {0x70, 16, ACC_RO,     MEMT_SRAM, NULL, OPF_NONE},
};

/* 页表索引：页号(PageSelect 值) -> 本页寄存器属性表。
 * 整页未实现/未登记 page 以「空表占位行」参与排序（tab=真实页号、num=0、
 * banked=0、inf=NULL）：保持 DtTabLeftFind 二分单调性；命中后写全丢弃
 * （DtRegFind 空表恒 NULL），读回该页影子槽内容（boot 零镜像 0x00）。
 * enum 之外、无影子槽的页才无索引行（DtTabLeftFind 未命中 → 哨兵 →
 * Upper 读 0xFF dummy）。
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
    {0x01, (BYTE)(sizeof(g_OsfpInfPage01H) / sizeof(g_OsfpInfPage01H[0])), 0, g_OsfpInfPage01H},    /* [2] Page 01h（Advertising，unbanked） */
    {0x02, (BYTE)(sizeof(g_OsfpInfPage02H) / sizeof(g_OsfpInfPage02H[0])), 0, g_OsfpInfPage02H},    /* [3] Page 02h（Thresholds，unbanked） */
    /* [4..16] Page 03h-0Fh 空表占位（tab=真实页号保二分单调；num=0 → 写全
     * 丢弃、读回影子槽 0x00——注意：较占位前 0xFF dummy 的行为变化，且
     * BankPageSelect 选这些页不再自清零） */
    {0x03, 0, 0, NULL},    /* [4]  Page 03h（Reserved，占位） */
    {0x04, 0, 0, NULL},    /* [5]  Page 04h（Reserved，占位） */
    {0x05, 0, 0, NULL},    /* [6]  Page 05h（Reserved，占位） */
    {0x06, 0, 0, NULL},    /* [7]  Page 06h（Reserved，占位） */
    {0x07, 0, 0, NULL},    /* [8]  Page 07h（Reserved，占位） */
    {0x08, 0, 0, NULL},    /* [9]  Page 08h（Reserved，占位） */
    {0x09, 0, 0, NULL},    /* [10] Page 09h（Reserved，占位） */
    {0x0A, 0, 0, NULL},    /* [11] Page 0Ah（Reserved，占位） */
    {0x0B, 0, 0, NULL},    /* [12] Page 0Bh（Reserved，占位） */
    {0x0C, 0, 0, NULL},    /* [13] Page 0Ch（Reserved，占位） */
    {0x0D, 0, 0, NULL},    /* [14] Page 0Dh（Reserved，占位） */
    {0x0E, 0, 0, NULL},    /* [15] Page 0Eh（Reserved，占位） */
    {0x0F, 0, 0, NULL},    /* [16] Page 0Fh（Reserved，占位） */
    /* [17..20] Page 10h banked ×4（DtParseForm 命中最左 [17] 后按 bank 顺延；
     * 4 行同表；影子槽 = PAGE_ID_10H_0..3，每 bank = 8 条 host lane） */
    {0x10, (BYTE)(sizeof(g_OsfpInfPage10H) / sizeof(g_OsfpInfPage10H[0])), 1, g_OsfpInfPage10H},    /* [17] Page 10h Bank0 */
    {0x10, (BYTE)(sizeof(g_OsfpInfPage10H) / sizeof(g_OsfpInfPage10H[0])), 1, g_OsfpInfPage10H},    /* [18] Page 10h Bank1 */
    {0x10, (BYTE)(sizeof(g_OsfpInfPage10H) / sizeof(g_OsfpInfPage10H[0])), 1, g_OsfpInfPage10H},    /* [19] Page 10h Bank2 */
    {0x10, (BYTE)(sizeof(g_OsfpInfPage10H) / sizeof(g_OsfpInfPage10H[0])), 1, g_OsfpInfPage10H},    /* [20] Page 10h Bank3 */
    /* [21..24] Page 11h banked ×4（Lane Status/Data Path Status，全页 RO） */
    {0x11, (BYTE)(sizeof(g_OsfpInfPage11H) / sizeof(g_OsfpInfPage11H[0])), 1, g_OsfpInfPage11H},    /* [21] Page 11h Bank0 */
    {0x11, (BYTE)(sizeof(g_OsfpInfPage11H) / sizeof(g_OsfpInfPage11H[0])), 1, g_OsfpInfPage11H},    /* [22] Page 11h Bank1 */
    {0x11, (BYTE)(sizeof(g_OsfpInfPage11H) / sizeof(g_OsfpInfPage11H[0])), 1, g_OsfpInfPage11H},    /* [23] Page 11h Bank2 */
    {0x11, (BYTE)(sizeof(g_OsfpInfPage11H) / sizeof(g_OsfpInfPage11H[0])), 1, g_OsfpInfPage11H},    /* [24] Page 11h Bank3 */
};

/* 编译期一致性断言：g_OsfpTabForm[] 元素数必须与 TabIndex_t 的
 * PAGE_TAB_NUM 一致（DtParse 二分以 PAGE_TAB_NUM 为上界）。
 * 定义处当前按 [PAGE_TAB_NUM] 定长书写本已恒等，此断言防的是将来被
 * 改成无长度 [] 定义后与枚举悄悄失配。失败分支取 -1（负长数组硬错误）：
 * 0 长数组是 GNU 扩展，gcc/armclang 均会静默接受，断言形同虚设。
 * PAGE_TAB_NUM 是枚举常量，#if 不可见，用数组长度技巧。 */
typedef BYTE RalTabFormNumCheck_t[(sizeof(g_OsfpTabForm) / sizeof(g_OsfpTabForm[0]) == (DWORD)PAGE_TAB_NUM) ? 1 : -1];

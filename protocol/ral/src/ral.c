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
     * WRITE）。cmd[0]=BankSelect(126) cmd[1]=PageSelect(127)。表内不再有
     * 0x7F 独立项，同写约束由两道门禁强制：引擎触发项整寄存器写
     * （off==0x7E 且 len==2，单独写 127 或 126 都丢弃），回调 OsfpOperate*
     * BankPageSelect 再自检 len==2 兜底。oprfunc 切换页/ bank 上下文并维护
     * 影子。 */
    {0x7E, 2, ACC_RWW,    MEMT_SRAM,  OsfpOperateBankPageSelect, OPF_IMMEDIATE},   /* BankPageSelect：不支持页自清零回 00h，bank 低 2 位 */
};

/* Page 00h 寄存器属性表已删（2026-10-15 占位化，省 ~114B Flash）：Page 00h
 * 是纯静态身份页（CMIS 5.3 §8.3 Administrative，全 RO），无任何动态行为需
 * 表驱动——g_OsfpTabForm[1] 改为空表占位 {0x00, 0, 0, NULL}，host 读/写
 * 行为与 03h-0Fh 占位段一致（读影子槽镜像、写丢弃）。影子槽 [1] 仍由
 * MgiParamLoad 从 Flash 参数区偏移 0x00280 整区拷贝载入，数据契约不变。
 * 下字节图仅作 CMIS 布局文档参考（勘定依据：§8.3.3~§8.3.12 + 5.3 修订记录
 * + CMIS 4.0 §8.3 交叉核对；Table 8-26 原表排印错位，地址列自 202 起偏移）：
 *   128       SFF8024IdentifierCopy
 *   129-144   VendorName        145-147 VendorOUI      148-163 VendorPN
 *   164-165   VendorRev         166-181 VendorSN       182-189 DateCode
 *   190-199   CLEICode
 *   200-201   ModulePowerCharacteristics（200 bit7-5 PowerClass=2(12W)=0x20，
 *             201 MaxPower=48(12W/0.25W)=0x30）
 *   202       CableAssemblyLinkLength（可分离光纤介质规格要求 = 0）
 *   203       ConnectorType（0Ch = MPO-12，SFF-8024 Table 4-3）
 *   204-209   CopperCableAttenuation（非铜缆规格规定全 0，含 209 Res.）
 *   210       MediaLaneUnsupported    211       FarEndConfiguration（可分离媒体 = 0）
 *   212       MediaInterfaceTechnology（850 nm VCSEL = 00h）
 *   213-214   MCIRelatedAdvertisement（213 MciFlowControlDuration + 214 Res.，5.3 新增）
 *   215-220   Reserved（继承 4.0 的 213-220 Reserved 区，5.3 插入 MCI 后剩余段）
 *   221       Custom（4.0 Table 8-15 定义为 Custom，无细节节描述）
 *   222       PageChecksum = sum(128..221) 低 8 位（§8.3.11）
 *   223-255   Custom（non-volatile，§8.3.12）
 * 215-221 不进表（Reserved/Custom），但其读值参与 PageChecksum，镜像中
 * 必须为确定值（约定 0x00）。
 * 本模块取值备注：400G-SR4 OSFP 光纤模块，202/204-211 按规格填 0。 */

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
    {0x7F, 1,  ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* PageChecksum = sum(130..254) 低 8 位（128-129 固件版本有意排除，CMIS §8.4）；镜像变化时须重算 */
};

/* Page 10h 寄存器属性表（CMIS 5.3 §8.9 Table 8-67 ~ 8-81，Banked Page：
 * Lane Control and Data Path Control）。banked 页：每 bank = 一组 8 条
 * host lane，bank↔lane 对应由应用/DPSM 层解释，引擎按 bank 选
 * PAGE_ID_10H_0..3 影子槽。offset = 页内偏移（宿主窗口地址 - 80h），
 * 连续同属性字段合并为大 size 项（DtRegFind 按区间包含定位，合并块内任意
 * 字节均可作写帧起点；触发型字段例外，须整寄存器写故不参与合并）。
 * Apply* 触发寄存器分两类：
 *   - ApplyDPInit（SCS0 0x0F）登记 WO_SC + MEMT_NONE + oprfunc
 *     （OsfpOperateApplyDpInit，OPF_IMMEDIATE）：影子不落数据（spec：WO 读
 *     值未指定），写触发 Provision（SCS0→ACS 拷贝 + DpsmApplyDpInit，
 *     ConfigStatus/DPInitPending 反馈在 Page 11h）；
 *   - ApplyImmediate / ApplyImmediateTx/Rx：SteppedConfigOnly=1b（00h:2.6）
 *     不支持，按 spec 写忽略——ApplyImmediate 登记 RW 影子即「忽略」语义，
 *     Tx/Rx 登记 WO_SC + MEMT_NONE 写丢弃。
 * 0x69-0x7F（宿主 233-255：Reserved[7] + Custom[16]）不进表：读回 boot
 * 零镜像 0x00（§8.4.17 PageChecksum 仅定义 01h:255，10h 无 checksum）。 */
CONST OsfpRegAttr_t g_OsfpInfPage10H[] = {
    /* 128: Data Path Control */
    {0x00, 1,  ACC_RW,    MEMT_SRAM, NULL, OPF_NONE},    /* DPDeinitLane1-8（RW Rqd；0b=初始化/1b=去初始化，DPSM 语义待实现） */
    /* 129-142: Lane-Specific Direct Effect Control（RW Adv，App 层按 01h:155-156 宣告逐位支持） */
    {0x01, 14, ACC_RW,    MEMT_SRAM, NULL, OPF_NONE},    /* InputPolarityFlipTx/OutputDisableTx/AutoSquelchDisableTx/OutputSquelchForceTx/AdaptiveInputEqFreezeTx/AdaptiveInputEqStoreTx/OutputPolarityFlipRx/OutputDisableRx/AutoSquelchDisableRx（含 0x07/0x0A 位保留） */
    /* 143-177: Staged Control Set 0（SCS0；表 8-70~8-75） */
    {0x0F, 1,  ACC_WO_SC, MEMT_NONE, OsfpOperateApplyDpInit, OPF_IMMEDIATE},    /* SCS0::ApplyDPInitLane1-8（WO Rqd；单字节 WRITE 触发 Provision：SCS0→ACS 拷贝 + DpsmApplyDpInit） */
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
 * 执行后回填（10h 的 ApplyDPInit 触发寄存器接 OsfpOperateApplyDpInit 联动）。
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
     * RAL 引擎 OsfpOperateApplyDpInit 从 SCS0 拷贝回填：DPConfig 0x4E
     * (RO Rqd)、Tx 控制 0x56、Rx 控制 0x5E(CDREnableRx 1B + Pre/PostCursor/
     * AmplitudeTargetRx 各 4B)） */
    {0x4E, 8,  ACC_RO,     MEMT_SRAM, NULL, OPF_NONE},
    {0x56, 8,  ACC_RO,     MEMT_SRAM, NULL, OPF_NONE},
    {0x5E, 13, ACC_RO,     MEMT_SRAM, NULL, OPF_NONE},
    /* 235: DPInitPendingLane1-8（表 8-96；RO Rqd；ApplyDPInit 成功后由 DPSM 置位） */
    {0x6B, 1,  ACC_RO,     MEMT_SRAM, NULL, OPF_NONE},
    /* 240-255: Media Lane↔Wavelength/Fiber Mapping（表 8-97；RO Cnd；
     * WDM 应用才使用，SR4 恒 0——由 boot 镜像决定，不登记亦可） */
    {0x70, 16, ACC_RO,     MEMT_SRAM, NULL, OPF_NONE},
};

/* B4H..B7H 寄存器初始化页属性表（64 × 2B 大端槽，ACC_RW + MEMT_SRAM 纯存储）：
 * 布局见 ral.h「B4H..B7H 寄存器初始化页布局」（0x00=REGNUM u8 + N×{ADDR@0x04+4n,
 * VALUE@0x06+4n}）。B4H=DRV 全局、B5H=DRV lane 级、B6H/B7H=TIA(GN1816) 四页共用
 * 本表（同布局，2026-10-14 用户定稿）。MEMT_SRAM：host I2C 写经 DtFrameCb 直落
 * SRAM 影子槽（DtFrameCb 对 MEMT_FLASH 纯存储写暂丢弃——Flash 持久化归参数区
 * 流程★），消费端 InitDrv/InitTia 经 ReadSramProtocolMsaSpace 读回后逐项写芯片。 */
#define BXH_INIT_PAIR(n) \
    { (BYTE)(4U * (n)),      2, ACC_RW, MEMT_SRAM, NULL, OPF_NONE }, \
    { (BYTE)(4U * (n) + 2U), 2, ACC_RW, MEMT_SRAM, NULL, OPF_NONE }
CONST OsfpRegAttr_t g_OsfpInfPageBxH[] = {
    BXH_INIT_PAIR( 0), BXH_INIT_PAIR( 1), BXH_INIT_PAIR( 2), BXH_INIT_PAIR( 3),
    BXH_INIT_PAIR( 4), BXH_INIT_PAIR( 5), BXH_INIT_PAIR( 6), BXH_INIT_PAIR( 7),
    BXH_INIT_PAIR( 8), BXH_INIT_PAIR( 9), BXH_INIT_PAIR(10), BXH_INIT_PAIR(11),
    BXH_INIT_PAIR(12), BXH_INIT_PAIR(13), BXH_INIT_PAIR(14), BXH_INIT_PAIR(15),
    BXH_INIT_PAIR(16), BXH_INIT_PAIR(17), BXH_INIT_PAIR(18), BXH_INIT_PAIR(19),
    BXH_INIT_PAIR(20), BXH_INIT_PAIR(21), BXH_INIT_PAIR(22), BXH_INIT_PAIR(23),
    BXH_INIT_PAIR(24), BXH_INIT_PAIR(25), BXH_INIT_PAIR(26), BXH_INIT_PAIR(27),
    BXH_INIT_PAIR(28), BXH_INIT_PAIR(29), BXH_INIT_PAIR(30), BXH_INIT_PAIR(31),
};
#undef BXH_INIT_PAIR

/* D9H 通用芯片/GPIO/DAC/ADC 读写邮箱属性表（2026-10-12 用户定稿布局，见 ral.h）：
 * 0x00-0x0C CHIP 记录：CHIPID 纯存储；REGADDR/REGVALUE 挂 oprfunc 写触发
 * 芯片读/写事务（OPF_DEFERRED——芯片 I2C 事务禁 ISR）；REGRBVALUE 固件回填
 * host RO；0x0D-0x0F GPIO 记录：GPIOID 写触发读（ReadGpio→GPIOSTATUS 回填）、
 * GPIOVALUE 写触发写（WriteGpio），均 OPF_IMMEDIATE（GPIO 直读直写 ISR 安全）；
 * 0x10-0x18 DAC 记录：DACID 写触发读（ReadDac→DACRDVALUE 回填）、DACVALUE 写
 * 触发写（WriteDac），均 OPF_DEFERRED（DAC 可能走外部芯片 I2C）；
 * 0x19-0x1D ADC 记录：ADCID 写触发读（ReadAdc→ADCVALUE 回填），OPF_DEFERRED
 * （ADC 可能走外部芯片 I2C）；
 * 触发寄存器值由回调自行持久化影子；0x1E-0x7F 保留不登记。 */
CONST OsfpRegAttr_t g_OsfpInfPageD9H[] = {
    {0x00, 1, ACC_RW,      MEMT_SRAM, NULL,                    OPF_NONE},      /* CHIPID：目标芯片 ID（CHIP_ID_TIA/DRV/DSP/PMIC） */
    {0x01, 4, ACC_RW,      MEMT_SRAM, OsfpOperateD9hRegAddr,   OPF_DEFERRED},  /* REGADDR：写触发 ReadChip→REGRBVALUE 回填 */
    {0x05, 4, ACC_RW,      MEMT_SRAM, OsfpOperateD9hRegValue,  OPF_DEFERRED},  /* REGVALUE：写触发 WriteChip */
    {0x09, 4, ACC_RO,      MEMT_SRAM, NULL,                    OPF_NONE},      /* REGRBVALUE：固件回填读回值，host RO */
    {0x0D, 1, ACC_RW,      MEMT_SRAM, OsfpOperateD9hGpioId,    OPF_IMMEDIATE}, /* GPIOID：写触发 ReadGpio→GPIOSTATUS 回填 */
    {0x0E, 1, ACC_RW,      MEMT_SRAM, OsfpOperateD9hGpioValue, OPF_IMMEDIATE}, /* GPIOVALUE：写触发 WriteGpio */
    {0x0F, 1, ACC_RO,      MEMT_SRAM, NULL,                    OPF_NONE},      /* GPIOSTATUS：固件回填读回电平，host RO */
    {0x10, 1, ACC_RW,      MEMT_SRAM, OsfpOperateD9hDacId,     OPF_DEFERRED},  /* DACID：写触发 ReadDac→DACRDVALUE 回填 */
    {0x11, 4, ACC_RW,      MEMT_SRAM, OsfpOperateD9hDacValue,  OPF_DEFERRED},  /* DACVALUE：写触发 WriteDac */
    {0x15, 4, ACC_RO,      MEMT_SRAM, NULL,                    OPF_NONE},      /* DACRDVALUE：固件回填读回值，host RO */
    {0x19, 1, ACC_RW,      MEMT_SRAM, OsfpOperateD9hAdcId,     OPF_DEFERRED},  /* ADCID：写触发 ReadAdc→ADCVALUE 回填 */
    {0x1A, 4, ACC_RO,      MEMT_SRAM, NULL,                    OPF_NONE},      /* ADCVALUE：固件回填读回值，host RO */
};

/* 9Fh CDB 消息页属性表（2026-10-14 V5 定稿布局，见 ral.h「9Fh 布局」块）：
 * CMDID 挂 oprfunc（OsfpOperate9fCmdId→CdbTrigger，OPF_IMMEDIATE）——host 单独
 * 写 2B CMDID 触发命令；EPLLength/LPLLength/CdbChkCode/LPL 纯存储（host 写落
 * 影子）；RPLLength/RPLChkCode 模块回填 host RO。
 * 触发语义（oprfunc 按帧首地址分发）：CMDID 触发须单写（帧首=0x00 且 len==2）；
 * 顺序写 CMDID+EPLLength+LPLLength+CdbChkCode（帧首 0x00 多字节）由 CdbTrigger
 * len!=2 拒收，符合 CMIS「最后一次单独写 CMDID，STOP 落 129」触发模型。 */
CONST OsfpRegAttr_t g_OsfpInfPage9FH[] = {
    {0x00, 2,   ACC_RW,   MEMT_SRAM, OsfpOperate9fCmdId, OPF_IMMEDIATE},     /* CMDID u16BE：写触发命令 */
    {0x02, 2,   ACC_RW,   MEMT_SRAM, NULL,                OPF_NONE},          /* EPLLength u16BE（恒 0，无 EPL） */
    {0x04, 1,   ACC_RW,   MEMT_SRAM, NULL,                OPF_NONE},          /* LPLLength u8 */
    {0x05, 1,   ACC_RW,   MEMT_SRAM, NULL,                OPF_NONE},          /* CdbChkCode u8 */
    {0x06, 1,   ACC_RO,   MEMT_SRAM, NULL,                OPF_NONE},          /* RPLLength u8（模块回填） */
    {0x07, 1,   ACC_RO,   MEMT_SRAM, NULL,                OPF_NONE},          /* RPLChkCode u8（模块回填） */
    {0x08, 120, ACC_RW,   MEMT_SRAM, NULL,                OPF_NONE},          /* LPL 120B：命令负载/应答负载 */
};

/* ---- VDM (Vendor Defined Module) Page 属性表 ----
 * Group 1: 32 instances (SNR/LTP/BER/FERC), descriptor+sample+threshold+flag/mask。
 * Page 20h=Descriptor, 24h=Sample, 28h=Threshold, 2Ch=Flag, 2Dh=Mask, 2Fh=Advertisement+Control。
 * 2Ch/2Dh/2Fh 仅 Bank0 实现，Bank1-3 继续占位（num=0）。
 * 20h/28h 静态内容（描述符/阈值）为 Flash 参数区默认值——MgiParamLoad 整区
 * memcpy 进影子即定稿，固件不再初始化；表项按「空表占位行」登记（num=0 →
 * host 写丢弃、读影子镜像），故 g_OsfpInfPage20H/28H 已删（2026-10-19）。 */

/* Page 24h Bank0: VDM Samples 寄存器属性表（CMIS 5.3 §8.22，X16 大端，全 RO）。
 * 32 个已填充实例（Group 1）各 2B，offset = VdmInstanceId_t 序号×2，与
 * g_VdmInsInfo 的 vdminfo.valueOffset 一一对应；未填充实例 32-255 不进表。
 * 逐实例登记（不合并成大块）：区间包含查找下合并块已可写，逐条保留是为
 * 了每实例可单独命名定位；历史动机（精确匹配时非起始偏移查不到）见
 * data_transfer.c DtRegFind 注释。 */
CONST OsfpRegAttr_t g_OsfpInfPage24H[] = {
    /* Byte 128-143: SNR Samples (大端 2B, DB256) */
    {0x00, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* SNR Media Lane1 */
    {0x02, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* SNR Media Lane2 */
    {0x04, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* SNR Media Lane3 */
    {0x06, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* SNR Media Lane4 */
    {0x08, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* SNR Host Lane1 */
    {0x0A, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* SNR Host Lane2 */
    {0x0C, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* SNR Host Lane3 */
    {0x0E, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* SNR Host Lane4 */
    /* Byte 144-159: LTP Samples (大端 2B, DB256) */
    {0x10, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* LTP Media Lane1 */
    {0x12, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* LTP Media Lane2 */
    {0x14, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* LTP Media Lane3 */
    {0x16, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* LTP Media Lane4 */
    {0x18, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* LTP Host Lane1 */
    {0x1A, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* LTP Host Lane2 */
    {0x1C, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* LTP Host Lane3 */
    {0x1E, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* LTP Host Lane4 */
    /* Byte 160-175: Pre-FEC BER Samples (大端 2B, F16) */
    {0x20, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* BER Cur Media */
    {0x22, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* BER Avg Media */
    {0x24, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* BER Min Media */
    {0x26, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* BER Max Media */
    {0x28, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* BER Cur Host */
    {0x2A, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* BER Avg Host */
    {0x2C, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* BER Min Host */
    {0x2E, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* BER Max Host */
    /* Byte 176-191: FERC Samples (大端 2B, U16CNT) */
    {0x30, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* FERC Cur Media */
    {0x32, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* FERC Avg Media */
    {0x34, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* FERC Min Media */
    {0x36, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* FERC Max Media */
    {0x38, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* FERC Cur Host */
    {0x3A, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* FERC Avg Host */
    {0x3C, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* FERC TOT Media */
    {0x3E, 2, ACC_RO,     MEMT_SRAM,  NULL, OPF_NONE},   /* FERC TOT Host */
};

/* Page 2Ch Bank0: VDM Flags 寄存器属性表（CMIS 5.3 §8.22，布局同 2Dh，RO/COR）。
 * 16B 对应 32 个已填充实例：byte n 装实例 2n（高 nibble bit7-4）与 2n+1
 * （低 nibble bit3-0），每 nibble 自低位向高位 = HighTCAlarm/LowTCAlarm/
 * HighTCWarning/LowTCWarning；offset = 实例号/2，与 g_VdmInsInfo 的
 * flag.offset 一一对应。VdmThresholdCheck 只置位不清（COR：host 读过的
 * 字节由 DtCorClear 清零），host 写一律丢弃（ACC_RO_COR）。
 * 逐字节登记（不合并成大块）：区间包含查找下合并块已可读，逐条保留是为
 * 了每 Flag 字节可单独命名定位；历史动机（精确匹配时非起始偏移的 Flag
 * 读落空）见 data_transfer.c DtRegFind 注释。 */
CONST OsfpRegAttr_t g_OsfpInfPage2CH[] = {
    /* Byte 128-131: SNR Flags */
    {0x00, 1, ACC_RO_COR, MEMT_SRAM,  NULL, OPF_NONE},   /* Flag SNR Media Lane1/2 */
    {0x01, 1, ACC_RO_COR, MEMT_SRAM,  NULL, OPF_NONE},   /* Flag SNR Media Lane3/4 */
    {0x02, 1, ACC_RO_COR, MEMT_SRAM,  NULL, OPF_NONE},   /* Flag SNR Host Lane1/2 */
    {0x03, 1, ACC_RO_COR, MEMT_SRAM,  NULL, OPF_NONE},   /* Flag SNR Host Lane3/4 */
    /* Byte 132-135: LTP Flags */
    {0x04, 1, ACC_RO_COR, MEMT_SRAM,  NULL, OPF_NONE},   /* Flag LTP Media Lane1/2 */
    {0x05, 1, ACC_RO_COR, MEMT_SRAM,  NULL, OPF_NONE},   /* Flag LTP Media Lane3/4 */
    {0x06, 1, ACC_RO_COR, MEMT_SRAM,  NULL, OPF_NONE},   /* Flag LTP Host Lane1/2 */
    {0x07, 1, ACC_RO_COR, MEMT_SRAM,  NULL, OPF_NONE},   /* Flag LTP Host Lane3/4 */
    /* Byte 136-139: Pre-FEC BER Flags */
    {0x08, 1, ACC_RO_COR, MEMT_SRAM,  NULL, OPF_NONE},   /* Flag BER Cur/Avg Media */
    {0x09, 1, ACC_RO_COR, MEMT_SRAM,  NULL, OPF_NONE},   /* Flag BER Min/Max Media */
    {0x0A, 1, ACC_RO_COR, MEMT_SRAM,  NULL, OPF_NONE},   /* Flag BER Cur/Avg Host */
    {0x0B, 1, ACC_RO_COR, MEMT_SRAM,  NULL, OPF_NONE},   /* Flag BER Min/Max Host */
    /* Byte 140-143: FERC Flags */
    {0x0C, 1, ACC_RO_COR, MEMT_SRAM,  NULL, OPF_NONE},   /* Flag FERC Cur/Avg Media */
    {0x0D, 1, ACC_RO_COR, MEMT_SRAM,  NULL, OPF_NONE},   /* Flag FERC Min/Max Media */
    {0x0E, 1, ACC_RO_COR, MEMT_SRAM,  NULL, OPF_NONE},   /* Flag FERC Cur/Avg Host */
    {0x0F, 1, ACC_RO_COR, MEMT_SRAM,  NULL, OPF_NONE},   /* Flag FERC TOT Media/Host */
};

/* Page 2Dh Bank0: VDM Masks 寄存器属性表（CMIS 5.3 §8.22，布局同 2Ch，RW）。
 * 16B 对应 32 个已填充实例：byte n 装实例 2n（高 nibble bit7-4）与 2n+1
 * （低 nibble bit3-0），每 nibble 自低位向高位 = HighTCAlarm/LowTCAlarm/
 * HighTCWarning/LowTCWarning；offset = 实例号/2，与 g_VdmInsInfo 的
 * flagmsk.offset 一一对应。默认全 1（全屏蔽，见 g_MsaSpaceDefaultSeedInSram）。
 * 逐字节登记（不合并成大块）：区间包含查找下合并块已可写，逐条保留是为
 * 了每掩码字节可单独命名定位；历史动机（精确匹配时非起始偏移的掩码写
 * 落空）见 data_transfer.c DtRegFind 注释。 */
CONST OsfpRegAttr_t g_OsfpInfPage2DH[] = {
    /* Byte 128-131: SNR Masks */
    {0x00, 1, ACC_RW,     MEMT_SRAM,  NULL, OPF_NONE},   /* Mask SNR Media Lane1/2 */
    {0x01, 1, ACC_RW,     MEMT_SRAM,  NULL, OPF_NONE},   /* Mask SNR Media Lane3/4 */
    {0x02, 1, ACC_RW,     MEMT_SRAM,  NULL, OPF_NONE},   /* Mask SNR Host Lane1/2 */
    {0x03, 1, ACC_RW,     MEMT_SRAM,  NULL, OPF_NONE},   /* Mask SNR Host Lane3/4 */
    /* Byte 132-135: LTP Masks */
    {0x04, 1, ACC_RW,     MEMT_SRAM,  NULL, OPF_NONE},   /* Mask LTP Media Lane1/2 */
    {0x05, 1, ACC_RW,     MEMT_SRAM,  NULL, OPF_NONE},   /* Mask LTP Media Lane3/4 */
    {0x06, 1, ACC_RW,     MEMT_SRAM,  NULL, OPF_NONE},   /* Mask LTP Host Lane1/2 */
    {0x07, 1, ACC_RW,     MEMT_SRAM,  NULL, OPF_NONE},   /* Mask LTP Host Lane3/4 */
    /* Byte 136-139: Pre-FEC BER Masks */
    {0x08, 1, ACC_RW,     MEMT_SRAM,  NULL, OPF_NONE},   /* Mask BER Cur/Avg Media */
    {0x09, 1, ACC_RW,     MEMT_SRAM,  NULL, OPF_NONE},   /* Mask BER Min/Max Media */
    {0x0A, 1, ACC_RW,     MEMT_SRAM,  NULL, OPF_NONE},   /* Mask BER Cur/Avg Host */
    {0x0B, 1, ACC_RW,     MEMT_SRAM,  NULL, OPF_NONE},   /* Mask BER Min/Max Host */
    /* Byte 140-143: FERC Masks */
    {0x0C, 1, ACC_RW,     MEMT_SRAM,  NULL, OPF_NONE},   /* Mask FERC Cur/Avg Media */
    {0x0D, 1, ACC_RW,     MEMT_SRAM,  NULL, OPF_NONE},   /* Mask FERC Min/Max Media */
    {0x0E, 1, ACC_RW,     MEMT_SRAM,  NULL, OPF_NONE},   /* Mask FERC Cur/Avg Host */
    {0x0F, 1, ACC_RW,     MEMT_SRAM,  NULL, OPF_NONE},   /* Mask FERC TOT Media/Host */
};

/* Page 2Fh Bank0: VDM 宣告 + Freeze/Unfreeze 控制寄存器属性表（CMIS 5.3 §8.22，Mixed）。
 * 页内偏移 = CMIS 字节号 − 128。逐寄存器登记（2Dh 样式）：
 *   0x00 = CMIS byte 128：bit1-0 VDMSupport（0=Group1，本模块 32 实例）、
 *          bit2 PowerSavingSupport、bit7-3 保留——模块宣告，host RO；
 *   0x01 = CMIS byte 129-130：FineIntervalLength（U16 大端，0.1ms 单位，
 *          BER/FERC 单样本测量窗口）——模块宣告，host RO；
 *   0x10 = CMIS byte 144：bit7 FreezeRequest（写 1 定格 / 写 0 恢复）、
 *          bit6 UnfreezeReq（本项目 W1S 约定）——回调持久化 host 写入值；
 *   0x11 = CMIS byte 145：bit7 FreezeDone / bit6 UnfreezeDone 状态位，
 *          唯一写者 = VdmFreezeApply/VdmUnfreezeApply（回调不落 host 写入值）。
 * 宣告字节无 seed/参数区定稿值，影子为零镜像：VDMSupport=0 恰为 Group1
 * （语义正确）、FineIntervalLength=0 占位待补★。0x03-0x0F 保留不登记
 * （DtRegFind 落空：写丢弃 / 读影子镜像）。 */
CONST OsfpRegAttr_t g_OsfpInfPage2FH[] = {
    /* Byte 128: VDM Support */
    {0x00, 1, ACC_RO, MEMT_SRAM, NULL,                      OPF_NONE},       /* VDMSupport/PowerSavingSupport */
    /* Byte 129-130: FineIntervalLength */
    {0x01, 2, ACC_RO, MEMT_SRAM, NULL,                      OPF_NONE},       /* FineIntervalLength U16BE 0.1ms */
    /* Byte 144-145: Freeze/Unfreeze */
    {0x10, 1, ACC_RW, MEMT_SRAM, OsfpOperate2FhFreezeReq,   OPF_IMMEDIATE},  /* FreezeRequest/UnfreezeReq */
    {0x11, 1, ACC_RW, MEMT_SRAM, OsfpOperate2FhUnfreezeReq, OPF_IMMEDIATE},  /* FreezeDone/UnfreezeDone */
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
/* 2026-10-12 占位补全：enum 全部槽位与页表行一一对应（行号 == ordinal == SRAM
 * 影子槽号）。12h-2Fh(28 页)/9Fh/A0h-AFh(16 页) banked 占位 ×4、B0h-B3h 系数页
 * 占位（unbanked）——此前这些槽零填充 tab=0x00 破坏 DtTabLeftFind 二分单调性，
 * 尾部追加 B4H..B7H 实表时 0xB7 查找会漏命中；占位行语义同 03h-0Fh 段（num=0 →
 * host 写丢弃、读回影子槽 boot 零镜像，区别于「页未登记」的 0xFF dummy）。 */
CONST OsfpTabForm_t g_OsfpTabForm[PAGE_TAB_NUM] = {
    {0x00, (BYTE)(sizeof(g_OsfpInfPageLow) / sizeof(g_OsfpInfPageLow[0])), 0, g_OsfpInfPageLow},    /* [0] Page Low（unbanked） */
    {0x00, 0, 0, NULL},    /* [1] Page 00h（Administrative，占位 num=0——表已删
                             * 2026-10-15，纯静态身份页无动态行为需表驱动；影子
                             * 仍由参数区镜像载入，host 读影子/写丢弃，同 03h-0Fh
                             * 占位段） */
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
    /* ---- [25..204] 未实现页占位（banked 页 4 连排同 tab、banked=1；num=0 写丢弃/读影子零镜像） ---- */
    /* [25..28] Page 12h Bank0-3（占位） */
    {0x12, 0, 1, NULL},
    {0x12, 0, 1, NULL},
    {0x12, 0, 1, NULL},
    {0x12, 0, 1, NULL},

    /* [29..32] Page 13h Bank0-3（占位） */
    {0x13, 0, 1, NULL},
    {0x13, 0, 1, NULL},
    {0x13, 0, 1, NULL},
    {0x13, 0, 1, NULL},

    /* [33..36] Page 14h Bank0-3（占位） */
    {0x14, 0, 1, NULL},
    {0x14, 0, 1, NULL},
    {0x14, 0, 1, NULL},
    {0x14, 0, 1, NULL},

    /* [37..40] Page 15h Bank0-3（占位） */
    {0x15, 0, 1, NULL},
    {0x15, 0, 1, NULL},
    {0x15, 0, 1, NULL},
    {0x15, 0, 1, NULL},

    /* [41..44] Page 16h Bank0-3（占位） */
    {0x16, 0, 1, NULL},
    {0x16, 0, 1, NULL},
    {0x16, 0, 1, NULL},
    {0x16, 0, 1, NULL},

    /* [45..48] Page 17h Bank0-3（占位） */
    {0x17, 0, 1, NULL},
    {0x17, 0, 1, NULL},
    {0x17, 0, 1, NULL},
    {0x17, 0, 1, NULL},

    /* [49..52] Page 18h Bank0-3（占位） */
    {0x18, 0, 1, NULL},
    {0x18, 0, 1, NULL},
    {0x18, 0, 1, NULL},
    {0x18, 0, 1, NULL},

    /* [53..56] Page 19h Bank0-3（占位） */
    {0x19, 0, 1, NULL},
    {0x19, 0, 1, NULL},
    {0x19, 0, 1, NULL},
    {0x19, 0, 1, NULL},

    /* [57..60] Page 1Ah Bank0-3（占位） */
    {0x1A, 0, 1, NULL},
    {0x1A, 0, 1, NULL},
    {0x1A, 0, 1, NULL},
    {0x1A, 0, 1, NULL},

    /* [61..64] Page 1Bh Bank0-3（占位） */
    {0x1B, 0, 1, NULL},
    {0x1B, 0, 1, NULL},
    {0x1B, 0, 1, NULL},
    {0x1B, 0, 1, NULL},

    /* [65..68] Page 1Ch Bank0-3（占位） */
    {0x1C, 0, 1, NULL},
    {0x1C, 0, 1, NULL},
    {0x1C, 0, 1, NULL},
    {0x1C, 0, 1, NULL},

    /* [69..72] Page 1Dh Bank0-3（占位） */
    {0x1D, 0, 1, NULL},
    {0x1D, 0, 1, NULL},
    {0x1D, 0, 1, NULL},
    {0x1D, 0, 1, NULL},

    /* [73..76] Page 20h Bank0-3（VDM Descriptors：Flash 参数区静态内容，
     * MgiParamLoad 落影子；num=0 占位 → 写丢弃/读影子镜像，固件不初始化） */
    {0x20, 0, 1, NULL},
    {0x20, 0, 1, NULL},
    {0x20, 0, 1, NULL},
    {0x20, 0, 1, NULL},

    /* [77..80] Page 21h Bank0-3（占位） */
    {0x21, 0, 1, NULL},
    {0x21, 0, 1, NULL},
    {0x21, 0, 1, NULL},
    {0x21, 0, 1, NULL},

    /* [81..84] Page 22h Bank0-3（占位） */
    {0x22, 0, 1, NULL},
    {0x22, 0, 1, NULL},
    {0x22, 0, 1, NULL},
    {0x22, 0, 1, NULL},

    /* [85..88] Page 23h Bank0-3（占位） */
    {0x23, 0, 1, NULL},
    {0x23, 0, 1, NULL},
    {0x23, 0, 1, NULL},
    {0x23, 0, 1, NULL},

    /* [89..92] Page 24h Bank0-3（VDM Samples） */
    {0x24, (BYTE)(sizeof(g_OsfpInfPage24H) / sizeof(g_OsfpInfPage24H[0])), 1, g_OsfpInfPage24H},
    {0x24, 0, 1, NULL},
    {0x24, 0, 1, NULL},
    {0x24, 0, 1, NULL},

    /* [93..96] Page 25h Bank0-3（占位） */
    {0x25, 0, 1, NULL},
    {0x25, 0, 1, NULL},
    {0x25, 0, 1, NULL},
    {0x25, 0, 1, NULL},

    /* [97..100] Page 26h Bank0-3（占位） */
    {0x26, 0, 1, NULL},
    {0x26, 0, 1, NULL},
    {0x26, 0, 1, NULL},
    {0x26, 0, 1, NULL},

    /* [101..104] Page 27h Bank0-3（占位） */
    {0x27, 0, 1, NULL},
    {0x27, 0, 1, NULL},
    {0x27, 0, 1, NULL},
    {0x27, 0, 1, NULL},

    /* [105..108] Page 28h Bank0-3（VDM Threshold Sets：Flash 参数区静态内容，
     * MgiParamLoad 落影子；num=0 占位 → 写丢弃/读影子镜像，host 运行时改阈值未支持） */
    {0x28, 0, 1, NULL},
    {0x28, 0, 1, NULL},
    {0x28, 0, 1, NULL},
    {0x28, 0, 1, NULL},

    /* [109..112] Page 29h Bank0-3（占位） */
    {0x29, 0, 1, NULL},
    {0x29, 0, 1, NULL},
    {0x29, 0, 1, NULL},
    {0x29, 0, 1, NULL},

    /* [113..116] Page 2Ah Bank0-3（占位） */
    {0x2A, 0, 1, NULL},
    {0x2A, 0, 1, NULL},
    {0x2A, 0, 1, NULL},
    {0x2A, 0, 1, NULL},

    /* [117..120] Page 2Bh Bank0-3（占位） */
    {0x2B, 0, 1, NULL},
    {0x2B, 0, 1, NULL},
    {0x2B, 0, 1, NULL},
    {0x2B, 0, 1, NULL},

    /* [121..124] Page 2Ch Bank0-3（VDM Flags） */
    {0x2C, (BYTE)(sizeof(g_OsfpInfPage2CH) / sizeof(g_OsfpInfPage2CH[0])), 1, g_OsfpInfPage2CH},
    {0x2C, 0, 1, NULL},
    {0x2C, 0, 1, NULL},
    {0x2C, 0, 1, NULL},

    /* [125..128] Page 2Dh Bank0-3（VDM Masks） */
    {0x2D, (BYTE)(sizeof(g_OsfpInfPage2DH) / sizeof(g_OsfpInfPage2DH[0])), 1, g_OsfpInfPage2DH},
    {0x2D, 0, 1, NULL},
    {0x2D, 0, 1, NULL},
    {0x2D, 0, 1, NULL},

    /* [129..132] Page 2Eh Bank0-3（占位） */
    {0x2E, 0, 1, NULL},
    {0x2E, 0, 1, NULL},
    {0x2E, 0, 1, NULL},
    {0x2E, 0, 1, NULL},

    /* [133..136] Page 2Fh Bank0-3（VDM Advertisement + Freeze/Unfreeze） */
    {0x2F, (BYTE)(sizeof(g_OsfpInfPage2FH) / sizeof(g_OsfpInfPage2FH[0])), 1, g_OsfpInfPage2FH},
    {0x2F, 0, 1, NULL},
    {0x2F, 0, 1, NULL},
    {0x2F, 0, 1, NULL},

    /* [137..140] Page 9Fh Bank0-3（CDB 消息页：CMDID 写触发/头+LPL；四 bank
     * 共用 g_OsfpInfPage9FH，见 ral.h「9Fh 布局」块） */
    {0x9F, (BYTE)(sizeof(g_OsfpInfPage9FH) / sizeof(g_OsfpInfPage9FH[0])), 1, g_OsfpInfPage9FH},
    {0x9F, (BYTE)(sizeof(g_OsfpInfPage9FH) / sizeof(g_OsfpInfPage9FH[0])), 1, g_OsfpInfPage9FH},
    {0x9F, (BYTE)(sizeof(g_OsfpInfPage9FH) / sizeof(g_OsfpInfPage9FH[0])), 1, g_OsfpInfPage9FH},
    {0x9F, (BYTE)(sizeof(g_OsfpInfPage9FH) / sizeof(g_OsfpInfPage9FH[0])), 1, g_OsfpInfPage9FH},

    /* [141..144] Page A0h Bank0-3（占位） */
    {0xA0, 0, 1, NULL},
    {0xA0, 0, 1, NULL},
    {0xA0, 0, 1, NULL},
    {0xA0, 0, 1, NULL},

    /* [145..148] Page A1h Bank0-3（占位） */
    {0xA1, 0, 1, NULL},
    {0xA1, 0, 1, NULL},
    {0xA1, 0, 1, NULL},
    {0xA1, 0, 1, NULL},

    /* [149..152] Page A2h Bank0-3（占位） */
    {0xA2, 0, 1, NULL},
    {0xA2, 0, 1, NULL},
    {0xA2, 0, 1, NULL},
    {0xA2, 0, 1, NULL},

    /* [153..156] Page A3h Bank0-3（占位） */
    {0xA3, 0, 1, NULL},
    {0xA3, 0, 1, NULL},
    {0xA3, 0, 1, NULL},
    {0xA3, 0, 1, NULL},

    /* [157..160] Page A4h Bank0-3（占位） */
    {0xA4, 0, 1, NULL},
    {0xA4, 0, 1, NULL},
    {0xA4, 0, 1, NULL},
    {0xA4, 0, 1, NULL},

    /* [161..164] Page A5h Bank0-3（占位） */
    {0xA5, 0, 1, NULL},
    {0xA5, 0, 1, NULL},
    {0xA5, 0, 1, NULL},
    {0xA5, 0, 1, NULL},

    /* [165..168] Page A6h Bank0-3（占位） */
    {0xA6, 0, 1, NULL},
    {0xA6, 0, 1, NULL},
    {0xA6, 0, 1, NULL},
    {0xA6, 0, 1, NULL},

    /* [169..172] Page A7h Bank0-3（占位） */
    {0xA7, 0, 1, NULL},
    {0xA7, 0, 1, NULL},
    {0xA7, 0, 1, NULL},
    {0xA7, 0, 1, NULL},

    /* [173..176] Page A8h Bank0-3（占位） */
    {0xA8, 0, 1, NULL},
    {0xA8, 0, 1, NULL},
    {0xA8, 0, 1, NULL},
    {0xA8, 0, 1, NULL},

    /* [177..180] Page A9h Bank0-3（占位） */
    {0xA9, 0, 1, NULL},
    {0xA9, 0, 1, NULL},
    {0xA9, 0, 1, NULL},
    {0xA9, 0, 1, NULL},

    /* [181..184] Page AAh Bank0-3（占位） */
    {0xAA, 0, 1, NULL},
    {0xAA, 0, 1, NULL},
    {0xAA, 0, 1, NULL},
    {0xAA, 0, 1, NULL},

    /* [185..188] Page ABh Bank0-3（占位） */
    {0xAB, 0, 1, NULL},
    {0xAB, 0, 1, NULL},
    {0xAB, 0, 1, NULL},
    {0xAB, 0, 1, NULL},

    /* [189..192] Page ACh Bank0-3（占位） */
    {0xAC, 0, 1, NULL},
    {0xAC, 0, 1, NULL},
    {0xAC, 0, 1, NULL},
    {0xAC, 0, 1, NULL},

    /* [193..196] Page ADh Bank0-3（占位） */
    {0xAD, 0, 1, NULL},
    {0xAD, 0, 1, NULL},
    {0xAD, 0, 1, NULL},
    {0xAD, 0, 1, NULL},

    /* [197..200] Page AEh Bank0-3（占位） */
    {0xAE, 0, 1, NULL},
    {0xAE, 0, 1, NULL},
    {0xAE, 0, 1, NULL},
    {0xAE, 0, 1, NULL},

    /* [201..204] Page AFh Bank0-3（占位） */
    {0xAF, 0, 1, NULL},
    {0xAF, 0, 1, NULL},
    {0xAF, 0, 1, NULL},
    {0xAF, 0, 1, NULL},

    /* [205] Page B0h（占位：厂商系数页本体未建表——mal-monitor 经 ReadSramProtocolMsaSpace 直读影子；host 读影子零镜像/写丢弃） */
    {0xB0, 0, 0, NULL},

    /* [206] Page B1h（占位：厂商系数页本体未建表——mal-monitor 经 ReadSramProtocolMsaSpace 直读影子；host 读影子零镜像/写丢弃） */
    {0xB1, 0, 0, NULL},

    /* [207] Page B2h（占位：厂商系数页本体未建表——mal-monitor 经 ReadSramProtocolMsaSpace 直读影子；host 读影子零镜像/写丢弃） */
    {0xB2, 0, 0, NULL},

    /* [208] Page B3h（占位：厂商系数页本体未建表——mal-monitor 经 ReadSramProtocolMsaSpace 直读影子；host 读影子零镜像/写丢弃） */
    {0xB3, 0, 0, NULL},

    /* [209] Page B4h（B4H DRV(GN1848) 寄存器初始化表；四页共用 g_OsfpInfPageBxH，64×2B ACC_RW/MEMT_SRAM） */
    {0xB4, (BYTE)(sizeof(g_OsfpInfPageBxH) / sizeof(g_OsfpInfPageBxH[0])), 0, g_OsfpInfPageBxH},

    /* [210] Page B5h（B5H DRV 初始化扩展页（预留，同布局）；四页共用 g_OsfpInfPageBxH，64×2B ACC_RW/MEMT_SRAM） */
    {0xB5, (BYTE)(sizeof(g_OsfpInfPageBxH) / sizeof(g_OsfpInfPageBxH[0])), 0, g_OsfpInfPageBxH},

    /* [211] Page B6h（B6H TIA(GN1816) 寄存器初始化表；四页共用 g_OsfpInfPageBxH，64×2B ACC_RW/MEMT_SRAM） */
    {0xB6, (BYTE)(sizeof(g_OsfpInfPageBxH) / sizeof(g_OsfpInfPageBxH[0])), 0, g_OsfpInfPageBxH},

    /* [212] Page B7h（B7H TIA 初始化扩展页；四页共用 g_OsfpInfPageBxH，64×2B ACC_RW/MEMT_SRAM） */
    {0xB7, (BYTE)(sizeof(g_OsfpInfPageBxH) / sizeof(g_OsfpInfPageBxH[0])), 0, g_OsfpInfPageBxH},
    /* [213] Page D9h（D9H 通用芯片/GPIO/DAC/ADC 读写邮箱：0x00-0x0C CHIP 记录——REGADDR 写触发读/
     * REGVALUE 写触发写/REGRBVALUE 回填 host RO（OPF_DEFERRED）+ 0x0D-0x0F GPIO 记录——
     * GPIOID 写触发读/GPIOSTATUS 回填、GPIOVALUE 写触发写（OPF_IMMEDIATE）+ 0x10-0x18 DAC 记录——
     * DACID 写触发读/DACRDVALUE 回填、DACVALUE 写触发写（OPF_DEFERRED）+ 0x19-0x1D ADC 记录——
     * ADCID 写触发读/ADCVALUE 回填（OPF_DEFERRED）；0x1E-0x7F 保留；
     * 布局见 ral.h） */
    {0xD9, (BYTE)(sizeof(g_OsfpInfPageD9H) / sizeof(g_OsfpInfPageD9H[0])), 0, g_OsfpInfPageD9H},
};

/* 编译期一致性断言：g_OsfpTabForm[] 元素数必须与 TabIndex_t 的
 * PAGE_TAB_NUM 一致（DtParse 二分以 PAGE_TAB_NUM 为上界）。
 * 定义处当前按 [PAGE_TAB_NUM] 定长书写本已恒等，此断言防的是将来被
 * 改成无长度 [] 定义后与枚举悄悄失配。失败分支取 -1（负长数组硬错误）：
 * 0 长数组是 GNU 扩展，gcc/armclang 均会静默接受，断言形同虚设。
 * PAGE_TAB_NUM 是枚举常量，#if 不可见，用数组长度技巧。 */
typedef BYTE RalTabFormNumCheck_t[(sizeof(g_OsfpTabForm) / sizeof(g_OsfpTabForm[0]) == (DWORD)PAGE_TAB_NUM) ? 1 : -1];

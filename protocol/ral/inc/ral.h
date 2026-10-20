#ifndef RAL_H
#define RAL_H
#include "basedef.h"
#ifdef BL_BUILD
/* BL 救援模式裁剪页表（bootloader/src/bl_ral_tab.c）：只登记 Lower + 9Fh
 * 四 bank。槽号必须与 bl_ral_tab.c 的 g_OsfpTabForm 行号一一对应
 * （行号==ordinal==影子槽号），cdb.c 以 PAGE_ID_9FH_0 + bank 求槽号。 */
typedef enum {
    PAGE_ID_LOW = 0,
    PAGE_ID_9FH_0,
    PAGE_ID_9FH_1,
    PAGE_ID_9FH_2,
    PAGE_ID_9FH_3,
    PAGE_TAB_NUM
} TabIndex_t;
#else /* BL_BUILD */
typedef enum {
    PAGE_ID_LOW = 0,
    PAGE_ID_00H,
    PAGE_ID_01H,
    PAGE_ID_02H,
    PAGE_ID_03H,
    PAGE_ID_04H,
    PAGE_ID_05H,
    PAGE_ID_06H,
    PAGE_ID_07H,
    PAGE_ID_08H,
    PAGE_ID_09H,
    PAGE_ID_0AH,
    PAGE_ID_0BH,
    PAGE_ID_0CH,
    PAGE_ID_0DH,
    PAGE_ID_0EH,
    PAGE_ID_0FH,
    PAGE_ID_10H_0,
    PAGE_ID_10H_1,
    PAGE_ID_10H_2,
    PAGE_ID_10H_3,
    PAGE_ID_11H_0,
    PAGE_ID_11H_1,
    PAGE_ID_11H_2,
    PAGE_ID_11H_3,
    PAGE_ID_12H_0,
    PAGE_ID_12H_1,
    PAGE_ID_12H_2,
    PAGE_ID_12H_3,
    PAGE_ID_13H_0,
    PAGE_ID_13H_1,
    PAGE_ID_13H_2,
    PAGE_ID_13H_3,
    PAGE_ID_14H_0,
    PAGE_ID_14H_1,
    PAGE_ID_14H_2,
    PAGE_ID_14H_3,
    PAGE_ID_15H_0,
    PAGE_ID_15H_1,
    PAGE_ID_15H_2,
    PAGE_ID_15H_3,
    PAGE_ID_16H_0,
    PAGE_ID_16H_1,
    PAGE_ID_16H_2,
    PAGE_ID_16H_3,
    PAGE_ID_17H_0,
    PAGE_ID_17H_1,
    PAGE_ID_17H_2,
    PAGE_ID_17H_3,
    PAGE_ID_18H_0,
    PAGE_ID_18H_1,
    PAGE_ID_18H_2,
    PAGE_ID_18H_3,
    PAGE_ID_19H_0,
    PAGE_ID_19H_1,
    PAGE_ID_19H_2,
    PAGE_ID_19H_3,
    PAGE_ID_1AH_0,
    PAGE_ID_1AH_1,
    PAGE_ID_1AH_2,
    PAGE_ID_1AH_3,
    PAGE_ID_1BH_0,
    PAGE_ID_1BH_1,
    PAGE_ID_1BH_2,
    PAGE_ID_1BH_3,
    PAGE_ID_1CH_0,
    PAGE_ID_1CH_1,
    PAGE_ID_1CH_2,
    PAGE_ID_1CH_3,
    PAGE_ID_1DH_0,
    PAGE_ID_1DH_1,
    PAGE_ID_1DH_2,
    PAGE_ID_1DH_3,
    PAGE_ID_20H_0,
    PAGE_ID_20H_1,
    PAGE_ID_20H_2,
    PAGE_ID_20H_3,
    PAGE_ID_21H_0,
    PAGE_ID_21H_1,
    PAGE_ID_21H_2,
    PAGE_ID_21H_3,
    PAGE_ID_22H_0,
    PAGE_ID_22H_1,
    PAGE_ID_22H_2,
    PAGE_ID_22H_3,
    PAGE_ID_23H_0,
    PAGE_ID_23H_1,
    PAGE_ID_23H_2,
    PAGE_ID_23H_3,
    PAGE_ID_24H_0,
    PAGE_ID_24H_1,
    PAGE_ID_24H_2,
    PAGE_ID_24H_3,
    PAGE_ID_25H_0,
    PAGE_ID_25H_1,
    PAGE_ID_25H_2,
    PAGE_ID_25H_3,
    PAGE_ID_26H_0,
    PAGE_ID_26H_1,
    PAGE_ID_26H_2,
    PAGE_ID_26H_3,
    PAGE_ID_27H_0,
    PAGE_ID_27H_1,
    PAGE_ID_27H_2,
    PAGE_ID_27H_3,
    PAGE_ID_28H_0,
    PAGE_ID_28H_1,
    PAGE_ID_28H_2,
    PAGE_ID_28H_3,
    PAGE_ID_29H_0,
    PAGE_ID_29H_1,
    PAGE_ID_29H_2,
    PAGE_ID_29H_3,
    PAGE_ID_2AH_0,
    PAGE_ID_2AH_1,
    PAGE_ID_2AH_2,
    PAGE_ID_2AH_3,
    PAGE_ID_2BH_0,
    PAGE_ID_2BH_1,
    PAGE_ID_2BH_2,
    PAGE_ID_2BH_3,
    PAGE_ID_2CH_0,
    PAGE_ID_2CH_1,
    PAGE_ID_2CH_2,
    PAGE_ID_2CH_3,
    PAGE_ID_2DH_0,
    PAGE_ID_2DH_1,
    PAGE_ID_2DH_2,
    PAGE_ID_2DH_3,
    PAGE_ID_2EH_0,
    PAGE_ID_2EH_1,
    PAGE_ID_2EH_2,
    PAGE_ID_2EH_3,
    PAGE_ID_2FH_0,
    PAGE_ID_2FH_1,
    PAGE_ID_2FH_2,
    PAGE_ID_2FH_3,
    PAGE_ID_9FH_0,
    PAGE_ID_9FH_1,
    PAGE_ID_9FH_2,
    PAGE_ID_9FH_3,
    PAGE_ID_A0H_0,
    PAGE_ID_A0H_1,
    PAGE_ID_A0H_2,
    PAGE_ID_A0H_3,
    PAGE_ID_A1H_0,
    PAGE_ID_A1H_1,
    PAGE_ID_A1H_2,
    PAGE_ID_A1H_3,
    PAGE_ID_A2H_0,
    PAGE_ID_A2H_1,
    PAGE_ID_A2H_2,
    PAGE_ID_A2H_3,
    PAGE_ID_A3H_0,
    PAGE_ID_A3H_1,
    PAGE_ID_A3H_2,
    PAGE_ID_A3H_3,
    PAGE_ID_A4H_0,
    PAGE_ID_A4H_1,
    PAGE_ID_A4H_2,
    PAGE_ID_A4H_3,
    PAGE_ID_A5H_0,
    PAGE_ID_A5H_1,
    PAGE_ID_A5H_2,
    PAGE_ID_A5H_3,
    PAGE_ID_A6H_0,
    PAGE_ID_A6H_1,
    PAGE_ID_A6H_2,
    PAGE_ID_A6H_3,
    PAGE_ID_A7H_0,
    PAGE_ID_A7H_1,
    PAGE_ID_A7H_2,
    PAGE_ID_A7H_3,
    PAGE_ID_A8H_0,
    PAGE_ID_A8H_1,
    PAGE_ID_A8H_2,
    PAGE_ID_A8H_3,
    PAGE_ID_A9H_0,
    PAGE_ID_A9H_1,
    PAGE_ID_A9H_2,
    PAGE_ID_A9H_3,
    PAGE_ID_AAH_0,
    PAGE_ID_AAH_1,
    PAGE_ID_AAH_2,
    PAGE_ID_AAH_3,
    PAGE_ID_ABH_0,
    PAGE_ID_ABH_1,
    PAGE_ID_ABH_2,
    PAGE_ID_ABH_3,
    PAGE_ID_ACH_0,
    PAGE_ID_ACH_1,
    PAGE_ID_ACH_2,
    PAGE_ID_ACH_3,
    PAGE_ID_ADH_0,
    PAGE_ID_ADH_1,
    PAGE_ID_ADH_2,
    PAGE_ID_ADH_3,
    PAGE_ID_AEH_0,
    PAGE_ID_AEH_1,
    PAGE_ID_AEH_2,
    PAGE_ID_AEH_3,
    PAGE_ID_AFH_0,
    PAGE_ID_AFH_1,
    PAGE_ID_AFH_2,
    PAGE_ID_AFH_3,
    PAGE_ID_B0H,            // 存放模块级校正参数K和B MON_TYPE_TEMP~MON_TYPE_CUSTOM (8B*6)
    PAGE_ID_B1H,            // 存放lane级校正参数K和B MON_TYPE_OPTPWR_TX~MON_TYPE_LASER_BIAS (8B*8+8B*8)
    PAGE_ID_B2H,            // 存放lane级校正参数K和B MON_TYPE_OPTPWR_RX ((8B+16B)*4)
    PAGE_ID_B3H,            // 预留：RxPwr 扩展通道系数页（ch4-7）
    PAGE_ID_B4H,            // DRV(GN1848) 寄存器初始化表（全局）：0x00=REGNUM u8 + N×{ADDR u16BE, VALUE u16BE}，N≤31
    PAGE_ID_B5H,            // DRV(GN1848) 寄存器初始化表（lane 级）：同 B4H 布局
    PAGE_ID_B6H,            // TIA(GN1816) 寄存器初始化表（同 B4H 布局）
    PAGE_ID_B7H,            // TIA 寄存器初始化扩展页（同 B4H 布局）
    PAGE_ID_D9H,            // 通用芯片/GPIO/DAC/ADC 读写邮箱：CHIPID@0x00 / REGADDR u32BE@0x01（写触发读）/ REGVALUE u32BE@0x05（写触发写）/ REGRBVALUE u32BE@0x09 / GPIOID@0x0D / GPIOVALUE@0x0E / GPIOSTATUS@0x0F / DACID@0x10 / DACVALUE u32BE@0x11（写触发写）/ DACRDVALUE u32BE@0x15 / ADCID@0x19（写触发读）/ ADCVALUE u32BE@0x1A
    PAGE_TAB_NUM
} TabIndex_t;
#endif /* BL_BUILD */

/*==================== 影子偏移地址定义（按 PAGE 值递增） ====================
 * 顺序：Lower Memory（页号 00h，常驻）→ 10h → 11h → 9Fh → B2h → B4h-B7h → D9h。
 * 页内偏移 = bank0 页内偏移 = CMIS byte − 128（Lower Memory 例外：直接 = CMIS byte）。
 * 同一影子偏移只允许本区一处宏定义。 */

/*-------------------- Lower Memory Byte 定义（CMIS 5.3 Table 8-5 ~ 8-25） --------------------*/
/* Byte 0-2: Management Characteristics */
#define LOW_ADDR_SFF8024_IDENTIFIER     0x00U   /* RO  SFF-8024 模块类型 ID */
#define LOW_ADDR_CMIS_REVISION          0x01U   /* RO  高4位主版本+低4位次版本 (53h=5.3) */
#define LOW_ADDR_MEMORY_MODEL           0x02U   /* RO  bit7 MemoryModel(0=paged,1=flat) 等管理特性 */
/* Byte 3: Global Status */
#define LOW_ADDR_MODULE_STATE           0x03U   /* RO  bit3-1 ModuleState, bit0 InterruptDeasserted */
/* Byte 4-7: Flags Summary (RO, 非COR) */
#define LOW_ADDR_FLAGS_SUMMARY_B0       0x04U   /* RO  Bank0: bit3-0 = Page 11h/12h/14h/2Ch */
#define LOW_ADDR_FLAGS_SUMMARY_B1       0x05U   /* RO  Bank1 同上 */
#define LOW_ADDR_FLAGS_SUMMARY_B2       0x06U   /* RO  Bank2 同上 */
#define LOW_ADDR_FLAGS_SUMMARY_B3       0x07U   /* RO  Bank3 同上 */
/* Byte 8-13: Module-Level Flags (RO/COR) */
#define LOW_ADDR_MODULE_FLAGS           0x08U   /* COR bit7-6 CdbCmdComplete2/1, bit2 DP FW Error, bit1 Module FW Error, bit0 ModuleStateChanged */
#define LOW_ADDR_VCC_TEMP_FLAGS         0x09U   /* COR VccMon+TempMon 各 Low/High x Warning/Alarm */
#define LOW_ADDR_AUX2_AUX1_FLAGS        0x0AU   /* COR Aux2+Aux1 同上布局 */
#define LOW_ADDR_AUX3_CUSTOM_FLAGS      0x0BU   /* COR CustomMon+Aux3 同上布局 */
#define LOW_ADDR_MODULE_FLAGS_RSV       0x0CU   /* Reserved */
#define LOW_ADDR_MODULE_FLAGS_CUSTOM    0x0DU   /* COR 厂商自定义 */
/* Byte 14-25: Module-Level Monitors (大端 2B) */
#define LOW_ADDR_TEMP_MON               0x0EU   /* RO  S16, 1/256 °C */
#define LOW_ADDR_VCC_MON                0x10U   /* RO  U16, 100 µV */
#define LOW_ADDR_AUX1_MON               0x12U   /* RO  S16, 含义 01h:145.0 宣告 */
#define LOW_ADDR_AUX2_MON               0x14U   /* RO  S16, 含义 01h:145.1 宣告 */
#define LOW_ADDR_AUX3_MON               0x16U   /* RO  S16, 含义 01h:145.2 宣告 */
#define LOW_ADDR_CUSTOM_MON             0x18U   /* RO  S16/U16 */
/* Byte 26-30: Module-Level Controls */
#define LOW_ADDR_MODULE_CONTROL         0x1AU   /* RW  bit7 BankBroadcast, bit6 LowPwrAllowHW, bit5 SquelchMethod, bit4 LowPwrRequestSW, bit3 SoftwareReset(WO/SC) */
#define LOW_ADDR_MCI_SPEED_CFG          0x1BU   /* RW  bit3-0 MciSpeedConfiguration */
#define LOW_ADDR_MODULE_CONTROL_RSV     0x1CU   /* Reserved */
#define LOW_ADDR_MODULE_CONTROL_CUS0    0x1DU   /* Custom */
#define LOW_ADDR_MODULE_CONTROL_CUS1    0x1EU   /* Custom */
/* Byte 31-36: Module-Level Masks (RW, 默认全 1) */
#define LOW_ADDR_MODULE_MASKS           0x1FU   /* RW  布局与 Byte 8-13 一一对应 */
#define LOW_ADDR_VCC_TEMP_MASKS         0x20U   /* RW */
#define LOW_ADDR_AUX2_AUX1_MASKS        0x21U   /* RW */
#define LOW_ADDR_AUX3_CUSTOM_MASKS      0x22U   /* RW */
#define LOW_ADDR_MODULE_MASKS_RSV       0x23U   /* Reserved */
#define LOW_ADDR_MODULE_MASKS_CUSTOM    0x24U   /* RW  Custom */
/* Byte 37-41: CDB 状态 / FW 版本 / 故障 */
#define LOW_ADDR_CDB_STATUS1            0x25U   /* RO  bit7 IsBusy, bit6 HasFailed, bit5-0 CommandResult */
#define LOW_ADDR_CDB_STATUS2            0x26U   /* RO  同上 */
#define LOW_ADDR_FW_MAJOR_REV           0x27U   /* RO  0=无固件, FF=固件无效 */
#define LOW_ADDR_FW_MINOR_REV           0x28U   /* RO */
#define LOW_ADDR_MODULE_FAULT_CAUSE     0x29U   /* RO  0=无 1=TEC 2=数据存储 3=程序存储 4=Tx 5=Rx 6=温度 */
/* Byte 42-55: Misc / Reserved */
#define LOW_ADDR_MISC_STATUS            0x2AU   /* RO  bit3-0 PasswordCmdResult */
                                                /* 0x2B-0x37 Reserved */
/* Byte 56-63: Extended Module Information */
#define LOW_ADDR_CMIS_SM_SUPPORT        0x38U   /* RO  0=legacy 1=无SM 2=仅MSM 3=MSM+DPSM 4=MSM+DPSM+NPSM */
#define LOW_ADDR_MODULE_FUNC_TYPE       0x39U   /* RO  0=Transmission Module, 1=ELSFP Resource Module */
                                                /* 0x3A-0x3B Reserved */
#define LOW_ADDR_MODULE_SUBTYPE         0x3CU   /* RO  bit3-0 SFF8024ModuleSubtype */
#define LOW_ADDR_FIBER_FACE_TYPE        0x3DU   /* RO  bit1-0: 00=未知 01=PC/UPC 10=APC */
#define LOW_ADDR_LOW_PWR_RESTRICTIONS   0x3EU   /* RO  bit3-0 CdbQuery/FwQuery/Cmds/FwCmds Unsupported */
                                                /* 0x3F Reserved */
/* Byte 64-84: Custom */
                                                /* 0x40-0x54 Custom */
/* Byte 85-117: Applications Advertising */
#define LOW_ADDR_MEDIA_TYPE             0x55U   /* RO  01h=MMF 02h=SMF 03h=Passive Cu 04h=Active Cable 05h=BASE-T */
#define LOW_ADDR_APP1_DESC0             0x56U   /* RO  App1 HostInterfaceID (FFh=未用) */
#define LOW_ADDR_APP1_DESC1             0x57U   /* RO  App1 MediaInterfaceID */
#define LOW_ADDR_APP1_DESC2             0x58U   /* RO  App1 HostLaneCount/MediaLaneCount */
#define LOW_ADDR_APP1_DESC3             0x59U   /* RO  App1 HostLaneAssignmentOptions */
#define LOW_ADDR_APP2_DESC0             0x5AU
#define LOW_ADDR_APP2_DESC1             0x5BU
#define LOW_ADDR_APP2_DESC2             0x5CU
#define LOW_ADDR_APP2_DESC3             0x5DU
#define LOW_ADDR_APP3_DESC0             0x5EU
#define LOW_ADDR_APP3_DESC1             0x5FU
#define LOW_ADDR_APP3_DESC2             0x60U
#define LOW_ADDR_APP3_DESC3             0x61U
#define LOW_ADDR_APP4_DESC0             0x62U
#define LOW_ADDR_APP4_DESC1             0x63U
#define LOW_ADDR_APP4_DESC2             0x64U
#define LOW_ADDR_APP4_DESC3             0x65U
#define LOW_ADDR_APP5_DESC0             0x66U
#define LOW_ADDR_APP5_DESC1             0x67U
#define LOW_ADDR_APP5_DESC2             0x68U
#define LOW_ADDR_APP5_DESC3             0x69U
#define LOW_ADDR_APP6_DESC0             0x6AU
#define LOW_ADDR_APP6_DESC1             0x6BU
#define LOW_ADDR_APP6_DESC2             0x6CU
#define LOW_ADDR_APP6_DESC3             0x6DU
#define LOW_ADDR_APP7_DESC0             0x6EU
#define LOW_ADDR_APP7_DESC1             0x6FU
#define LOW_ADDR_APP7_DESC2             0x70U
#define LOW_ADDR_APP7_DESC3             0x71U
#define LOW_ADDR_APP8_DESC0             0x72U
#define LOW_ADDR_APP8_DESC1             0x73U
#define LOW_ADDR_APP8_DESC2             0x74U
#define LOW_ADDR_APP8_DESC3             0x75U
/* Byte 118-127: Password / Page Mapping */
#define LOW_ADDR_PASSWORD_CHANGE        0x76U   /* 0x76-0x79 U32 大端 WO/SC */
#define LOW_ADDR_PASSWORD_ENTRY         0x7AU   /* 0x7A-0x7D U32 大端 WO/SC; 出厂主机密码 0000_1011h */
#define LOW_ADDR_BANK_PAGE_SELECT       0x7EU   /* RWW 合并域 0x7E-0x7F（2B）：cmd[0]=BankSelect cmd[1]=PageSelect，须同一次 WRITE */
#define LOW_ADDR_BANK_SELECT            0x7EU   /* RWW BankSelect（合并域低字节） */
#define LOW_ADDR_PAGE_SELECT            0x7FU   /* RWW PageSelect（合并域高字节）；写不支持页模块自清零回 Page 00h */

/*-------------------- Page 10h lane/DP 控制偏移（CMIS 5.3 §8.9；页内偏移 = CMIS byte − 128） --------------------
 * SCS0（Staged Control Set）在 Page 10h，ACS（Active Control Set）在 Page 11h。
 * 控制字节为 RW 影子（host 写经 RAL 引擎落到这些字节）；SCS0::DPConfigLane 同时
 * 是 ApplyDPInit 的 AppSel 校验读源（见 dpsm.c DpsmApplyDpInit）。 */
#define PAGE_10H_ADDR_DP_DEINIT       0x00U   /* 10h:128 DPDeinitLane1-8 */
#define PAGE_10H_ADDR_OUT_DISABLE_TX  0x02U   /* 10h:130 OutputDisableTx */
#define PAGE_10H_ADDR_SQUELCH_TX      0x04U   /* 10h:132 OutputSquelchForceTx */
#define PAGE_10H_ADDR_SCS_DPCFG       0x11U   /* 10h:145-152 SCS0::DPConfigLane1-8 */
#define PAGE_10H_ADDR_SCS_TX          0x19U   /* 10h:153-160 SCS0 Tx 控制 */
#define PAGE_10H_ADDR_SCS_RX          0x21U   /* 10h:161-173 SCS0 Rx 控制有效段 */
#define PAGE_10H_ADDR_DP_INIT_PEND    0x6BU   /* 10h:235 DPInitPendingLane1-8 */

/*-------------------- Page 11h lane/DP 状态偏移（CMIS 5.3 §8.10） -------------------- */
#define PAGE_11H_ADDR_DP_STATE        0x00U   /* 11h:128-131 DPState lane1-8(4bit/lane) */
#define PAGE_11H_ADDR_DP_STATE_FLAG   0x0AU   /* 11h:134 DPStateChangedFlag */
#define PAGE_11H_ADDR_CONFIG_STATUS   0x4AU   /* 11h:202-203 ConfigStatusLane1-4 */
#define PAGE_11H_ADDR_ACS_DPCFG       0x4EU   /* 11h:206-213 ACS::DPConfigLane1-8 */
#define PAGE_11H_ADDR_ACS_TX          0x56U   /* 11h:214-221 ACS Tx 控制 */
#define PAGE_11H_ADDR_ACS_RX          0x5EU   /* 11h:222-234 ACS Rx 控制 */
#define PAGE_11H_ADDR_DP_INIT_PEND    0x71U   /* 11h:235 DPInitPendingLane1-8 */

/*-------------------- Page 2Fh VDM Freeze/Unfreeze 布局（CMIS 5.3 §8.22） --------------------
 * 页内偏移 = CMIS byte − 128。★ 全部布局常量集中本处（与 PAGE_10H/11H/9FH/D9H
 * 同一套约定：值 = 页内偏移，注释给 CMIS 字节号），vdm.c / ral_operate.c 只消费。
 * 解冻双路径：host 清 FreezeRequest（CMIS 语义）或写 UnfreezeRequest bit6。 */
#define PAGE_2FH_ADDR_VDM_SUPPORT   0x00U   /* 2Fh:128 VDMSupport */
#define PAGE_2FH_ADDR_FREEZE_REQ    0x10U   /* 2Fh:144 FreezeRequest/UnfreezeRequest */
#define PAGE_2FH_ADDR_FREEZE_STS    0x11U   /* 2Fh:145 FreezeDone/UnfreezeDone */
#define PAGE_2FH_BIT_FREEZE_REQ     0x80U   /* 2Fh:144.7 FreezeRequest */
#define PAGE_2FH_BIT_UNFREEZE_REQ   0x40U   /* 2Fh:144.6 UnfreezeRequest（本项目约定） */
#define PAGE_2FH_BIT_FREEZE_DONE    0x80U   /* 2Fh:145.7 FreezeDone */
#define PAGE_2FH_BIT_UNFREEZE_DONE  0x40U   /* 2Fh:145.6 UnfreezeDone */

/*-------------------- 9Fh CDB 消息页布局（2026-10-14 V5 定稿） --------------------
 * 页内偏移 = CMIS byte − 128。命令头/应答头/LPL 均落 Page 9Fh:128-255：
 *   0x00-0x01 CMDID      u16   host 写触发命令（最后一次单独写，STOP 落 129；
 *                             触发挂 OsfpOperate9fCmdId，OPF_IMMEDIATE）
 *   0x02-0x03 EPLLength  u16   host 写（本实现恒 0，无 EPL）
 *   0x04      LPLLength  u8    host 写（LPL 字节数）
 *   0x05      CdbChkCode u8    host 写；= ~(sum CMDID+EPLLength+LPLLength+LPL)
 *   0x06      RPLLength  u8    模块回填（host RO）
 *   0x07      RPLChkCode u8    模块回填（host RO）；= ~sum(应答体)，无体=0
 *   0x08-0x7F LPL        120B  host 写命令负载 / 模块回填应答负载
 *
 * 命令执行模型见 protocol/cdb/inc/cdb.h 与 src/cdb.c（前台单实例：CMDID 触发在
 * STPDET ISR 置 busy+In Progress，真正执行下沉主循环 CdbExeBackground）。 */
#define PAGE_9FH_ADDR_CMDID    0x00U
#define PAGE_9FH_ADDR_EPLLEN   0x02U
#define PAGE_9FH_ADDR_LPLLEN   0x04U
#define PAGE_9FH_ADDR_CHKCODE  0x05U
#define PAGE_9FH_ADDR_RPLLEN   0x06U
#define PAGE_9FH_ADDR_RPLCHK   0x07U
#define PAGE_9FH_ADDR_LPL      0x08U
#define PAGE_9FH_LPL_SIZE     120U

/*-------------------- Page B2h RxPwr 阈值偏移（厂商系数页尾区） --------------------
 * RxPwr 三段拟合阈值基址：B2H 影子 0x7A-0x7F（暗/小/大 各 2B 大端连续）。 */
#define PAGE_B2H_ADDR_RXPWR_THR       0x7AU

/*-------------------- B4H..B7H 寄存器初始化页布局（2026-10-14 用户定稿 v2） --------------------
 * 页头 4B + 条目区：
 *   0x00        REGNUM  u8   本页有效条目数 N（0 ≤ N ≤ 31；0=空表跳过，>31 消费端拒）
 *   0x01-0x03   保留         恒 0
 *   0x04..      N 条 { ADDR u16BE @0x04+4n, VALUE u16BE @0x06+4n }，n=0..N-1
 * ADDR 高字节 = 写宽度标志：0x00=8bit 写 / 0x01=16bit 单事务写（DrvWrite 内解释，
 * 见 driver/drv/inc/drv.h DRV_WRITE_WIDTH_*）；低字节 = 芯片寄存器地址（8bit 空间）。
 * VALUE = 写入值：8bit 条目写低字节；16bit 条目写全 16bit（MSB-first，10bit 码放
 * 9:0、bits15:10 由生成工具按 DS 填）。N=31 时末条 n=30 填满 0x7C-0x7F。 */
#define PAGE_BXH_ADDR_REGNUM            0x00U
#define PAGE_BXH_REGINIT_NUM_MAX       31U
#define PAGE_BXH_ADDR_REGADDR(n)   ((BYTE)(4U + 4U * (BYTE)(n)))
#define PAGE_BXH_ADDR_REGVALUE(n)  ((BYTE)(6U + 4U * (BYTE)(n)))

/*-------------------- D9H 通用芯片/GPIO/DAC/ADC 读写邮箱布局（2026-10-12 用户定稿） --------------------
 * 0x00-0x0C = CHIP 邮箱记录；0x0D-0x0F = GPIO 邮箱记录；0x10-0x18 = DAC 邮箱记录；
 * 0x19-0x1D = ADC 邮箱记录；多字节字段一律大端。
 *
 * CHIP 记录（0x00-0x0C）：
 *   0x00      CHIPID    u8    目标芯片 ID（CHIP_ID_* 枚举值——CHIP_ID_TIA/DRV/DSP/
 *                             PMIC，纯存储；越界值由芯片资源层拒：ReadChip/WriteChip
 *                             经 GetChipInfoById 判空返 ERR）
 *   0x01-0x04 REGADDR   u32   芯片寄存器地址（写触发→ReadChip(CHIPID,REGADDR)，
 *                             结果大端回填 REGRBVALUE）
 *   0x05-0x08 REGVALUE  u32   待写入值（写触发→WriteChip(CHIPID,REGADDR,REGVALUE)）
 *   0x09-0x0C REGRBVALUE u32  读回值（固件回填，host RO；读事务失败保持旧值★无
 *                             valid 位，host 无法区分「旧值」与「新读到相同值」）
 *
 * GPIO 记录（0x0D-0x0F）：
 *   0x0D      GPIOID     u8    目标 GPIO ID（GPIO_ID_* 枚举值；写触发读事务：
 *                             ReadGpio(GPIOID) → 结果保存到 GPIOSTATUS；越界/未
 *                             初始化由 ReadGpio 经 GetGpioInfo 拒）
 *   0x0E      GPIOVALUE  u8    待写电平（写触发写事务：WriteGpio(GPIOID, GPIOVALUE)，
 *                             非零→IO_HIGH/零→IO_LOW 归一化——WriteGpio 值域仅 0/1）
 *   0x0F      GPIOSTATUS u8    引脚当前电平（固件回填，host RO；读失败保持旧值★）
 *
 * DAC 记录（0x10-0x18）：
 *   0x10      DACID      u8    目标 DAC ID（DAC_ID_* 枚举值；写触发读事务：
 *                             ReadDac(DACID) → 结果保存到 DACRDVALUE；越界/未初始
 *                             化由 ReadDac 经 GetDacInfo 拒）
 *   0x11-0x14 DACVALUE   u32   待写入值（写触发写事务：WriteDac(DACID, DACVALUE)）
 *   0x15-0x18 DACRDVALUE u32   读回值（固件回填，host RO；读事务失败保持旧值★无
 *                             valid 位）
 *
 * ADC 记录（0x19-0x1D）：
 *   0x19      ADCID      u8    目标 ADC ID（ADC_ID_* 枚举值；写触发读事务：
 *                             ReadAdc(ADCID) → 结果保存到 ADCVALUE；越界/未初始化
 *                             由 ReadAdc 经 GetAdcInfo 拒）
 *   0x1A-0x1D ADCVALUE   u32   读回值（固件回填，host RO；读事务失败保持旧值★无
 *                             valid 位）
 *
 * 0x1E-0x7F 保留（不登记 → host 写丢弃、读影子零镜像）。
 *
 * 触发语义（2026-10-12 用户定稿）：CHIP 记录 REGADDR/REGVALUE 挂 oprfunc 均
 * OPF_DEFERRED（芯片 I2C 事务禁止在 STPDET ISR 内执行）——回调
 * OsfpOperateD9hRegAddr / OsfpOperateD9hRegValue；GPIO 记录 GPIOID/GPIOVALUE 挂
 * oprfunc 均 OPF_IMMEDIATE（GPIO 直读直写、无 I2C/无阻塞，ISR 安全）——回调
 * OsfpOperateD9hGpioId / OsfpOperateD9hGpioValue；DAC 记录 DACID/DACVALUE 挂
 * oprfunc 均 OPF_DEFERRED（DAC 可能挂在外部芯片上走 I2C，禁止在 STPDET ISR 内
 * 执行）——回调 OsfpOperateD9hDacId / OsfpOperateD9hDacValue；ADC 记录 ADCID 挂
 * oprfunc OPF_DEFERRED（ADC 可能走外部芯片 I2C，禁止在 STPDET ISR 内执行）——
 * 回调 OsfpOperateD9hAdcId（均见 ral_operate.c）。
 * 引擎约束：oprfunc 按「帧首地址」分发——触发须单写（帧首=本寄存器偏移且
 * len==本寄存器字节数）；顺序写整记录落 ID 纯存储路径，不触发任何 oprfunc。
 * CHIP/DAC/ADC 记录 DEFERRED 延迟：写后影子持久化/RB 回填在主循环下一次 DtDeferredPoll
 * 生效（非 ISR 即时）——host 不可即写即读，须轮询 RB 字段；GPIO 记录 IMMEDIATE
 * 即时生效，GPIOSTATUS 同步回填。 */
#define PAGE_D9H_ADDR_CHIPID       0x00U
#define PAGE_D9H_ADDR_REGADDR      0x01U
#define PAGE_D9H_ADDR_REGVALUE     0x05U
#define PAGE_D9H_ADDR_REGRBVALUE   0x09U
#define PAGE_D9H_REGADDR_SIZE     4U
#define PAGE_D9H_REGVALUE_SIZE    4U
#define PAGE_D9H_REGRBVALUE_SIZE  4U
/* GPIO 记录（0x0D-0x0F，各 1B） */
#define PAGE_D9H_ADDR_GPIOID       0x0DU
#define PAGE_D9H_ADDR_GPIOVALUE    0x0EU
#define PAGE_D9H_ADDR_GPIOSTATUS   0x0FU
/* DAC 记录（0x10-0x18） */
#define PAGE_D9H_ADDR_DACID        0x10U
#define PAGE_D9H_ADDR_DACVALUE     0x11U
#define PAGE_D9H_ADDR_DACRDVALUE   0x15U
#define PAGE_D9H_DACVALUE_SIZE    4U
#define PAGE_D9H_DACRDVALUE_SIZE  4U
/* ADC 记录（0x19-0x1D；0x1E-0x7F 保留） */
#define PAGE_D9H_ADDR_ADCID        0x19U
#define PAGE_D9H_ADDR_ADCVALUE     0x1AU
#define PAGE_D9H_ADCVALUE_SIZE    4U

/*-------------------- Access Type（CMIS 5.3 Table 8-3，互斥取值） --------------------*/
#define ACC_RO      0x00U   /* 只读；host 写无效果（不算协议违规） */
#define ACC_RW      0x01U   /* 可读可写，写后读回须得写入值 */
#define ACC_WO      0x02U   /* 只写；host 读允许但值不可预测 */
#define ACC_RWW     0x03U   /* RW 且模块也可修改；写非法值时模块拒绝并保持原值（如 PageSelect） */
#define ACC_WO_SC   0x04U   /* WO + 写后自清（触发位）；host 读恒 0（瞬态除外） */
#define ACC_RO_COR  0x05U   /* RO + 读后清零（锁存 Flag）；条件仍在会再置位 */

/*-------------------- 数据存储地（memtype，互斥取值） --------------------*/
#define MEMT_SRAM   0x00U   /* 存 SRAM：掉电丢失，运行时工作值 */
#define MEMT_FLASH  0x01U   /* 存 Flash：掉电保存（用户 EEPROM、校准等） */
#define MEMT_NONE   0x02U   /* 无存储：WO 触发位、派生/计算值等 */

/* 寄存器读写触发的回调：cmd=数据缓冲（写方向=host 写入值，读方向=待回填/已读出值），
 * len=字节数；返回 OK/ERR。NULL=纯存储，引擎直读直写。 */
typedef DWORD (*OprateFunc)(BYTE* cmd, BYTE len);

/*-------------------- oprfunc 运行类型（OsfpRegAttr_t.opftype，互斥取值） --------------------
 * 标注 oprfunc 的执行上下文/时机，供 RAL 引擎决定在何处触发回调：
 *   OPF_NONE      无 oprfunc（oprfunc == NULL），纯存储，引擎直读直写；
 *   OPF_IMMEDIATE 立即操作型：回调运行时间短、无阻塞副作用（不碰 Flash/XIP、
 *                 无 ms 级等待、不加锁），可在 ISR 或引擎写路径中同步执行；
 *   OPF_DEFERRED  间接操作型：置标志位，下沉主循环（线程上下文）执行——
 *                 用于 Flash 擦写、长延时、需访问共享资源/持锁的操作。 */
#define OPF_NONE       0x00U
#define OPF_IMMEDIATE  0x01U
#define OPF_DEFERRED   0x02U

/* 寄存器区域描述（页表元素） */
typedef struct {
    BYTE offset;        /* 页内偏移地址：Lower/Upper 均为 00h-7Fh；Upper 的宿主窗口地址 bit7 由 MCI 半区选择时剥离（DtParse 用 memaddr & 0x7F 查找） */
    BYTE size;          /* 寄存器长度（字节），如 LOW_ADDR_TEMP_MON=2 */
    BYTE acctype;       /* 访问类型：Table 8-3 的 ACCESS TYPE（ACC_*） */
    BYTE memtype;       /* 数据存储地（MEMT_*） */
    OprateFunc oprfunc; /* 读写该寄存器触发的回调；NULL=纯存储 */
    BYTE opftype;       /* oprfunc 运行类型（OPF_*）；oprfunc==NULL 时恒 OPF_NONE */
} OsfpRegAttr_t;

/* 每个 page 的结构体：页号 + bank 属性 + 本页寄存器属性表（页表索引元素） */
typedef struct {
    BYTE tab;                    /* 页号 = PageSelect 值（page 01h → 0x01）；Lower Memory 用 00h */
    BYTE num;                    /* 本页 OsfpRegAttr_t 元素数量 */
    BYTE banked;                 /* 本页是否分 bank：0 = unbanked（BankSelect 被忽略），
                                  * 1 = banked（g_OsfpTabForm 中同 tab 连续 4 项，
                                  * 匹配索引 = 最左项 + bank） */
    CONST OsfpRegAttr_t *inf;    /* 本页寄存器属性表（OsfpRegAttr_t 数组，按 offset 升序） */
} OsfpTabForm_t;

extern CONST OsfpTabForm_t g_OsfpTabForm[];      /* 页表索引：页号 -> 本页寄存器属性表 */
extern CONST OsfpRegAttr_t g_OsfpInfPageLow[]; /* Page Low 寄存器属性表，见 ral.c */
/* g_OsfpInfPage00H 已删（2026-10-15 占位化，见 ral.c 页表 [1] 注释） */
extern CONST OsfpRegAttr_t g_OsfpInfPage01H[]; /* Page 01h 寄存器属性表（CMIS 5.3 §8.4 Table 8-42~8-60），见 ral.c */
extern CONST OsfpRegAttr_t g_OsfpInfPage02H[]; /* Page 02h 寄存器属性表（CMIS 5.3 §8.5 Table 8-61~8-63），见 ral.c */
extern CONST OsfpRegAttr_t g_OsfpInfPage9FH[]; /* Page 9Fh 寄存器属性表（CMIS 5.3 §8.23 CDB 消息页），见 ral.c */

#endif // RAL_H

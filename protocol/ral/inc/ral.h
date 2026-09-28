#ifndef RAL_H
#define RAL_H
#include "basedef.h"
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

    PAGE_TAB_NUM
} TabIndex_t;

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

/* 寄存器区域描述（页表元素） */
typedef struct {
    BYTE offset;        /* 页内偏移地址：Lower/Upper 均为 00h-7Fh；Upper 的宿主窗口地址 bit7 由 MCI 半区选择时剥离（DtParse 用 memaddr & 0x7F 查找） */
    BYTE size;          /* 寄存器长度（字节），如 LOW_ADDR_TEMP_MON=2 */
    BYTE acctype;       /* 访问类型：Table 8-3 的 ACCESS TYPE（ACC_*） */
    BYTE memtype;       /* 数据存储地（MEMT_*） */
    OprateFunc oprfunc; /* 读写该寄存器触发的回调 */
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
extern CONST OsfpRegAttr_t g_OsfpInfPage00H[]; /* Page 00h 寄存器属性表（CMIS 5.3 Table 8-26~8-41），见 ral.c */

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
#define LOW_ADDR_BANK_SELECT            0x7EU   /* RWW BankSelect; 与 PageSelect 同一次 WRITE */
#define LOW_ADDR_PAGE_SELECT            0x7FU   /* RWW PageSelect; 写不支持页模块自清零回 Page 00h */

#endif // RAL_H

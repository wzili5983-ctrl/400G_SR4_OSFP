/** @file vdm.h
 * @brief CMIS 5.3 VDM (Vendor Defined Module) pages 20h-2Fh.
 *
 * Group 1 only (64 instances).  32 instances populated:
 *   0-7   SNR  (Media/Host × Lane1-4)              Basic      DB256
 *   8-15  LTP  (Media/Host × Lane1-4)              Basic      DB256
 *  16-23  Pre-FEC BER (cur/avg/min/max × Media/Host) Basic/Stat F16
 *  24-31  FERC (cur/avg/min/max media, cur/avg host,
 *                tot media/host)                    Basic/Stat U16CNT
 *
 * 描述符（Page 20h）、阈值（Page 28h）与 Page 01h byte 0x0E bit6 的 VDM
 * 宣告均为 Flash 参数区默认值——MgiParamLoad 装入 SRAM 影子即定稿，固件
 * 运行时不再初始化这些页（VdmInit 只清 Freeze 态并挂任务；Flag 由 2Ch
 * 影子本身承担，无固件私有缓冲）。运行时
 * 职责：发布样本（24h）→ 读影子阈值比较 → COR 置 Flag（2Ch）→ 汇总
 * flag&~mask（2Dh）；Freeze/Unfreeze 经 Page 2Fh 触发。
 *
 * Freeze: host 写 FreezeRequest → statistic 类实例（Instancetype =
 *         VDM_INST_STATISTIC）保持上次发布值，basic 类继续更新。
 *
 * Copyright 2026 Terabitcom, Inc.
 */
#ifndef VDM_H
#define VDM_H

#include "basedef.h"
#include "monitor_impl.h"       /* PerfMonInfo_t / MsaMonitorId_t */

typedef enum {
    VDM_ID_SNR_MEDIA1= 0U,
    VDM_ID_SNR_MEDIA2,
    VDM_ID_SNR_MEDIA3,
    VDM_ID_SNR_MEDIA4,
    VDM_ID_SNR_HOST1,
    VDM_ID_SNR_HOST2,
    VDM_ID_SNR_HOST3,
    VDM_ID_SNR_HOST4,
    VDM_ID_LTP_MEDIA1,
    VDM_ID_LTP_MEDIA2,
    VDM_ID_LTP_MEDIA3,
    VDM_ID_LTP_MEDIA4,
    VDM_ID_LTP_HOST1,
    VDM_ID_LTP_HOST2,
    VDM_ID_LTP_HOST3,
    VDM_ID_LTP_HOST4,
    VDM_ID_BER_CUR_MEDIA,
    VDM_ID_BER_AVG_MEDIA,
    VDM_ID_BER_MIN_MEDIA,
    VDM_ID_BER_MAX_MEDIA,
    VDM_ID_BER_CUR_HOST,
    VDM_ID_BER_AVG_HOST,
    VDM_ID_BER_MIN_HOST,
    VDM_ID_BER_MAX_HOST,
    VDM_ID_FERC_CUR_MEDIA,
    VDM_ID_FERC_AVG_MEDIA,
    VDM_ID_FERC_MIN_MEDIA,
    VDM_ID_FERC_MAX_MEDIA,
    VDM_ID_FERC_CUR_HOST,
    VDM_ID_FERC_AVG_HOST,
    VDM_ID_FERC_TOT_MEDIA,
    VDM_ID_FERC_TOT_HOST,
    VDM_INSTANCE_NUM
}VdmInstanceId_t;

/** VDM 实例描述符（CMIS Page 20h 可见元数据）。 */
typedef struct VdmInsDescroptor_t {
    TabIndex_t              valueTab;           /* 监视值落点影子页槽（Page 20h） */
    BYTE                    valueOffset;        /* 监视值落点页内偏移 */
    BYTE                    Instancetype;       /* 实例类型:basic or statistic */
    BYTE                    telSide;            /* ODSP_TEL_SIDE_* */
} VdmDescForm_t;

/* 实例类型（VdmDescForm_t.Instancetype）：statistic 类 Freeze 期间保持
 * 上次发布值（data holding），basic 类照常更新。 */
#define VDM_INST_BASIC          0U
#define VDM_INST_STATISTIC      1U

/** VDM 实例信息（描述符 + CMIS 落点描述表）。
 *  效仿 g_StdPerfMon（monitor_impl.c）：desc 为 Page 20h 描述符元数据，
 *  vdminfo 为全套落点——value→24h 样本、flag→2Ch（COR）、flagmsk→2Dh、
 *  thr→28h 阈值、adindex = g_SampleFilttedValues 采样槽。
 *  采集值存 g_SampleFilttedValues[vdminfo.adindex]；vdminfo.cal 恒 NULL
 *  （样本来自 odsp_telemetry，不经 ADC 校准链）。 */
typedef struct {
    VdmDescForm_t           desc;               /* 描述符 */
    PerfMonInfo_t           vdminfo;            /* 监视值落点 + 编码值缓存 + Flag/Mask/Thr */
} VdmInsInfo_t;

/* --- Instance counts ---------------------------------------------------- */
#define VDM_DESCRIPTOR_SIZE     2U      /* bytes per descriptor */
#define VDM_SAMPLE_SIZE         2U      /* bytes per sample (X16 BE) */
#define VDM_THRESHOLD_SET_SIZE  8U      /* 4 thresholds × 2B */
#define VDM_FLAG_INST_PER_BYTE  2U      /* 2 instances packed per flag byte */

/* --- Instance table (vdm.c) --------------------------------------------- */
extern VdmInsInfo_t g_VdmInsInfo[VDM_INSTANCE_NUM];

/* --- Observable Type IDs (CMIS 5.3 Table 8-170) ------------------------- */
#define VDM_OBS_SNR_MEDIA       5U
#define VDM_OBS_SNR_HOST        6U
#define VDM_OBS_LTP_MEDIA       7U
#define VDM_OBS_LTP_HOST        8U
#define VDM_OBS_BER_MIN_MEDIA   11U
#define VDM_OBS_BER_MAX_MEDIA   12U
#define VDM_OBS_BER_AVG_MEDIA   13U
#define VDM_OBS_BER_MIN_HOST    9U
#define VDM_OBS_BER_MAX_HOST    10U
#define VDM_OBS_BER_AVG_HOST    14U
#define VDM_OBS_BER_CUR_MEDIA   15U
#define VDM_OBS_BER_CUR_HOST    16U
#define VDM_OBS_FERC_MIN_MEDIA  17U
#define VDM_OBS_FERC_MAX_MEDIA  18U
#define VDM_OBS_FERC_AVG_MEDIA  19U
#define VDM_OBS_FERC_MIN_HOST   20U
#define VDM_OBS_FERC_MAX_HOST   21U
#define VDM_OBS_FERC_AVG_HOST   22U
#define VDM_OBS_FERC_CUR_MEDIA  23U
#define VDM_OBS_FERC_CUR_HOST   24U
#define VDM_OBS_FERC_TOT_MEDIA  25U
#define VDM_OBS_FERC_TOT_HOST   26U

/* --- MonitoredResource encoding ----------------------------------------- */
#define VDM_RES_LANE1           0U
#define VDM_RES_LANE2           1U
#define VDM_RES_LANE3           2U
#define VDM_RES_LANE4           3U
#define VDM_RES_MODULE          15U

/* --- Special F16 sentinel values ---------------------------------------- */
#define VDM_F16_NAN             0x7E00U
#define VDM_F16_POS_INF         0x7C00U
#define VDM_F16_MAX_FINITE      0x7BFFU     /* 65504.0 */
#define VDM_F16_UNAVAILABLE     0xFFFFU     /* module not ready */

/* Page 2Fh 布局常量（PAGE_2FH_ADDR_* / PAGE_2FH_BIT_*）已统一定义于 ral.h——ral.h 是寄存器布局
 * 唯一出处；本头只留 VDM 业务 API/描述符类型。经 monitor_impl.h → ral.h
 * 传递可见，此处不重复 include。 */

/* --- API ----------------------------------------------------------------- */
VOID VdmInit(VOID);
VOID VdmTask(VOID *arg);
WORD VdmFloatToF16(double val);

/* Page 2Fh Freeze/Unfreeze 应用（统计量定格/恢复 + 状态位）。
 * 由 ral_operate.c 的 OsfpOperate2FhFreezeReq/OsfpOperate2FhUnfreezeReq
 * 调用；回调 OPF_IMMEDIATE，故本函数须 ISR 安全（只碰单字节影子、不加锁）。 */
VOID VdmFreezeApply(VOID);
VOID VdmUnfreezeApply(VOID);

#endif /* VDM_H */

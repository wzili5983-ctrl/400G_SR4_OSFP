/** @file odsp_telemetry.h
 * @brief BCM87420 运行期遥测采集 API（DSP 业务逻辑方案 v5 §6）。
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * 1s 周期采集 DSP 侧 VDM 源数据：
 *   - SNR/LTP：87420 FW fast polling（COMMAND_ID_DIAG_SET_CMIS_SNR_LTP_INFO
 *     使能、GET_CMIS_SNR_LTP_INFO 取 lane_data[]，float dB）
 *   - FEC 统计：capi_fec_ber_cmis_latch_request/release 双侧
 *     （capi_kp4fec_ber_state_t，double BER + 帧/错误计数）
 *   - 心跳：capi_get_chip_status 查 uc_ready 掉线
 *
 * 边界：本模块只采集与快照，不写 CMIS 影子页——Freeze 握手 / Flag
 * 评估 / VDM 页写入由 VDM 模块消费本层 Getter 完成。类型保持
 * cAPI 无关（POD），消费方无需包含 cAPI 头。
 *
 * 上下文：OdspAdaptTask（Timer2 任务侧，可阻塞 I2C）；不涉影子 RMW，
 * 无需 AdpatShadowLock。电源重入：Update 仅在 OdspInitIsReady 后被调用；
 * PwrDn 期间由 OdspAdaptTask 调 OdspTelemetrySleep() 解除使能状态，
 * 下次 PwrUp 自动重新使能 SNR/LTP 轮询。
 */
#ifndef ODSP_TELEMETRY_H
#define ODSP_TELEMETRY_H

#include "basedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 侧选择：HOST = 金手指电侧，MEDIA = 光侧（TIA/DRV）。 */
#define ODSP_TEL_SIDE_HOST      0u
#define ODSP_TEL_SIDE_MEDIA     1u
#define ODSP_TEL_SIDE_NUM       2u
#define ODSP_TEL_LANE_NUM       4u

/* FEC 快照（最近一次成功采集；VDM 三件套源数据）。
 * ber_* 为原始 double（VDM 层负责 F16 编码）；
 * ferc_total 为本层饱和累加值（跨 interval 持续累加不可纠正帧）。 */
typedef struct {
    double  ber_cur;            /* Pre-FEC BER 当前值 */
    double  ber_min;
    double  ber_max;
    double  ber_avg;            /* 平均（cAPI 内部累计相除，抗采样抖动） */
    DDWORD  frame_total;        /* 本区间收帧总数（avg 语义分母） */
    DDWORD  ferc_interval;      /* 本区间不可纠正帧数（FERC current） */
    DDWORD  ferc_total;         /* 累计不可纠正帧（饱和实现） */
    DWORD   sew_max;            /* t_code = 单帧最大 symbol 错误数（SEWmax） */
    BYTE    valid;              /* 1=该侧已成功采集至少一次 */
} OdspTelFecSnap_t;

/* 初始化（OdspAdaptInit 调用一次）：清状态、清分频。 */
VOID OdspTelemetryInit(VOID);

/* 100ms 入口（OdspAdaptTask 在电源守卫通过后调用）：
 * 内部 10 拍分频，每秒执行：SNR/LTP GET 双侧 + FEC latch 双侧 + 心跳。
 * 首次进入（或重入）时先对各侧下发 SET 使能 fast polling。 */
VOID OdspTelemetryUpdate(VOID);

/* PwrDn 期间挂起（OdspAdaptTask 未就绪分支调用）：
 * 解除 SNR/LTP 轮询使能与有效标志；下次 PwrUp 重新使能。 */
VOID OdspTelemetrySleep(VOID);

/* SNR/LTP 读取：返回 1=该侧最近一次采样有效（fresh），0=未就绪/失败。
 * val 输出 dB×256 的 U16（CMIS VDM 编码可直接使用；host 侧与 media 侧
 * 各自最近一次 fast-poll 快照）。 */
BYTE OdspTelSnrGet(BYTE side, BYTE lane, WORD *val);
BYTE OdspTelLtpGet(BYTE side, BYTE lane, WORD *val);

/* FEC 快照读取（拷贝输出；调用方无需关心内部状态所有权）。 */
VOID OdspTelFecSnapGet(BYTE side, OdspTelFecSnap_t *snap);

/* DSP 心跳：1=最近一次 uc_ready 查询正常；0=失联（交 msm Fault 决策）。 */
BYTE OdspTelDspAliveGet(VOID);

#ifdef __cplusplus
}
#endif

#endif

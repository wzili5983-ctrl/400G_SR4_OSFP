/** @file odsp_telemetry.c
 * @brief BCM87420 运行期遥测采集实现（方案 v5 §6）。
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * 节拍：OdspAdaptTask 100ms → 内部 10 拍分频 = 1s 采集周期。
 *
 * SNR/LTP 通道（每侧独立实例）：
 *   SET COMMAND_ID_DIAG_SET_CMIS_SNR_LTP_INFO(70)：
 *     param.command.start_stop=1, poll_interval=1000ms → 87420 FW 内部
 *     1s 轮询并把快照写到 FW SRAM，应答 memory_info（ref_address/len/
 *     hw_mutex）——该 memory_info 必须保留，供后续 GET 使用。
 *   GET COMMAND_ID_DIAG_GET_CMIS_SNR_LTP_INFO(72)：
 *     host_get_memory_data_info 按 memory_info 直读 FW SRAM 快照
 *     （内部 hw_mutex 保护），lane_data[MAX_LW_LANES] 为 float dB。
 *   phy.core_ip 选择 HOST/MEDIA 侧 ready 寄存器 → 两侧各一套
 *   SET/GET 状态（s_SnrReq[side]），lane_mask=0xF 取全 4 lane。
 *
 * FEC 通道（每侧独立状态）：
 *   s_FecState[side] 为 cAPI 输入输出状态体（err_latch_times 驱动
 *   average），跨调用保持；fec_mode = CLIENT/LINE。
 *   每秒 request（建立 CMIS 语义 latch 边界）→ release（读回 + 清
 *   硬件计数开新 interval，clr_cnt=TRUE）。★latch 精确语义 bring-up
 *   复核（指南 ch18：request 定格统计 / release 读+释放）。
 *
 * 编码：SNR/LTP float dB → U16（×256 四舍五入，夹取 [0, 65534]；
 * 65535/65534 特殊编码属 VDM 层职责，本层给原始夹取值）。
 */
#include "type_defns.h"         /* cAPI 基础类型：必须先于其它 cAPI 头 */
#include "common_def.h"
#include "chip_mode_def.h"      /* txfir_info_t —— capi_def.h 的前置依赖（cAPI 头序） */
#include "capi_def.h"
#include "capi.h"
#include "capi_diag_def.h"      /* capi_usr_diag_info_t 等 —— capi_diag.h 的前置依赖 */
#include "capi_diag.h"          /* capi_fec_ber_cmis_latch_* / capi_diag_command_request_info */

#include "odsp_telemetry.h"

/* 100ms 任务 10 拍 = 1s 采集周期（与 OptMon 同分频风格）。 */
#define ODSP_TEL_UPDATE_DIV         10u
/* fast polling 周期（87420 FW 内部快照节拍，ms）。 */
#define ODSP_TEL_SNR_POLL_MS        1000u
/* SNR/LTP dB→U16 转换的夹取上限（65535/65534 留给 VDM 特殊编码）。 */
#define ODSP_TEL_DB16_MAX           65534UL

/* 侧 → core_ip / fec_mode 映射表（索引 = ODSP_TEL_SIDE_*）。 */
STATIC CONST core_ip_t k_TelSideCoreIp[ODSP_TEL_SIDE_NUM] = {
    CORE_IP_HOST_DSP,
    CORE_IP_MEDIA_DSP
};
STATIC CONST capi_fec_mode_t k_TelSideFecMode[ODSP_TEL_SIDE_NUM] = {
    CAPI_FEC_CLIENT,
    CAPI_FEC_LINE
};

/*=========================== 可变状态（STATIC 单独区） ===========================*/

/* SNR/LTP：每侧一份命令上下文（SET 应答的 memory_info 随结构保留）。 */
STATIC capi_diag_command_request_info_t s_SnrReq[ODSP_TEL_SIDE_NUM];
STATIC BYTE  s_SnrPollOn[ODSP_TEL_SIDE_NUM];    /* 该侧 SET 使能已成功 */
STATIC WORD  s_Snr[ODSP_TEL_SIDE_NUM][ODSP_TEL_LANE_NUM];
STATIC WORD  s_Ltp[ODSP_TEL_SIDE_NUM][ODSP_TEL_LANE_NUM];
STATIC BYTE  s_SnrFresh[ODSP_TEL_SIDE_NUM];     /* 最近一次 GET 成功 */

/* FEC：每侧一份 cAPI 状态体（输入输出态，跨秒保持）。 */
STATIC capi_kp4fec_ber_state_t s_FecState[ODSP_TEL_SIDE_NUM];
STATIC DDWORD s_FercAcc[ODSP_TEL_SIDE_NUM];     /* 累计不可纠正帧（饱和） */
STATIC BYTE   s_FecValid[ODSP_TEL_SIDE_NUM];

STATIC BYTE s_DspAlive = 0u;
STATIC BYTE s_TickDiv  = 0u;

/*=========================== 内部辅助 ===========================*/

STATIC VOID OdspTelPhyFill(capi_phy_info_t *phy, BYTE side)
{
    *phy = (capi_phy_info_t){ 0 };
    phy->phy_id    = 0u;
    phy->core_ip   = k_TelSideCoreIp[side];
    phy->lane_mask = 0x0Fu;
}

/* SNR/LTP：单侧 SET 使能（幂等——仅未使能时下发）。 */
STATIC VOID OdspTelSnrPollStart(BYTE side)
{
    capi_phy_info_t phy;

    if(s_SnrPollOn[side] != 0u) {
        return;
    }
    OdspTelPhyFill(&phy, side);
    s_SnrReq[side] = (capi_diag_command_request_info_t){ 0 };
    s_SnrReq[side].command_id = COMMAND_ID_DIAG_SET_CMIS_SNR_LTP_INFO;
    s_SnrReq[side].type.cmis_snr_ltp.param.command.start_stop   = 1u;
    s_SnrReq[side].type.cmis_snr_ltp.param.command.poll_interval = ODSP_TEL_SNR_POLL_MS;
    if(capi_diag_command_request_info(&phy, &s_SnrReq[side]) == RR_SUCCESS) {
        s_SnrPollOn[side] = 1u;
    }
}

/* SNR/LTP：float dB → U16（×256 四舍五入）。
 * 负值/异常值夹到 0（FW 异常保护，C 负数 float→整数转换是未定义行为，
 * 必须先判符号）；超上限夹到 65534（65535/65534 特殊编码归 VDM 层）。 */
STATIC WORD OdspTelDbToU16(FLOAT db)
{
    DWORD v;

    if(db <= 0.0f) {
        return 0u;
    }
    v = (DWORD)(db * 256.0f + 0.5f);
    if(v > ODSP_TEL_DB16_MAX) {
        v = ODSP_TEL_DB16_MAX;
    }
    return (WORD)v;
}

/* SNR/LTP：单侧 GET 快照 → U16 缓存。 */
STATIC VOID OdspTelSnrPollGet(BYTE side)
{
    BYTE lane;
    capi_phy_info_t phy;

    if(s_SnrPollOn[side] == 0u) {
        return;
    }
    OdspTelPhyFill(&phy, side);
    s_SnrReq[side].command_id = COMMAND_ID_DIAG_GET_CMIS_SNR_LTP_INFO;
    if(capi_diag_command_request_info(&phy, &s_SnrReq[side]) != RR_SUCCESS) {
        s_SnrFresh[side] = 0u;
        return;
    }
    for(lane = 0u; lane < ODSP_TEL_LANE_NUM; lane++) {
        s_Snr[side][lane] = OdspTelDbToU16(
            s_SnrReq[side].type.cmis_snr_ltp.param.lane_data[lane].cmis_snr_value);
        s_Ltp[side][lane] = OdspTelDbToU16(
            s_SnrReq[side].type.cmis_snr_ltp.param.lane_data[lane].cmis_ltp_value);
    }
    s_SnrFresh[side] = 1u;
}

/* FEC：单侧 latch request + release，快照入内部缓存。
 * release 以 clr_cnt=TRUE 开新 interval（每 1s 一个 interval，
 * 对应 CMIS host Freeze 语义未接入前的固定采样区间）。 */
STATIC VOID OdspTelFecCollect(BYTE side)
{
    capi_phy_info_t phy;
    DDWORD interval_ferc;

    OdspTelPhyFill(&phy, side);

    s_FecState[side].fec_mode = k_TelSideFecMode[side];
    s_FecState[side].clr_cnt  = 1u;     /* bool TRUE：读后清硬件计数 */
    s_FecState[side].only_cur = 0u;     /* 取 cur/min/max/avg 全套 */

    if(capi_fec_ber_cmis_latch_request(&phy, &s_FecState[side]) != RR_SUCCESS) {
        return;
    }
    if(capi_fec_ber_cmis_latch_release(&phy, &s_FecState[side]) != RR_SUCCESS) {
        return;
    }

    /* 累计不可纠正帧（饱和实现：CMIS FERC total 语义，host 读慢不回绕） */
    interval_ferc = s_FecState[side].cur_err.tot_frame_uncorr_cnt;
    if(s_FercAcc[side] + interval_ferc < s_FercAcc[side]) {
        s_FercAcc[side] = ~0ULL;        /* 饱和到 U64 全 1 */
    } else {
        s_FercAcc[side] += interval_ferc;
    }
    s_FecValid[side] = 1u;
}

/* 心跳：uc_ready 掉线检测（失联只置标志，不自动复位——交 msm Fault 决策）。 */
STATIC VOID OdspTelHeartbeat(VOID)
{
    capi_phy_info_t phy;
    capi_chip_status_info_t st;

    phy = (capi_phy_info_t){ 0 };
    phy.phy_id = 0u;
    st = (capi_chip_status_info_t){ 0 };
    st.param.is.uc_ready = 1u;
    if(capi_get_chip_status(&phy, &st) == RR_SUCCESS
       && st.value.uc_ready == 1u) {
        s_DspAlive = 1u;
    } else {
        s_DspAlive = 0u;
    }
}

/*=========================== 对外 API ===========================*/

VOID OdspTelemetryInit(VOID)
{
    BYTE side;

    for(side = 0u; side < ODSP_TEL_SIDE_NUM; side++) {
        s_SnrPollOn[side]  = 0u;
        s_SnrFresh[side]   = 0u;
        s_FecValid[side]   = 0u;
        s_FercAcc[side]    = 0u;
    }
    s_DspAlive = 0u;
    s_TickDiv  = 0u;
}

VOID OdspTelemetryUpdate(VOID)
{
    BYTE side;

    s_TickDiv++;
    if(s_TickDiv < ODSP_TEL_UPDATE_DIV) {
        return;
    }
    s_TickDiv = 0u;

    for(side = 0u; side < ODSP_TEL_SIDE_NUM; side++) {
        OdspTelSnrPollStart(side);
        OdspTelSnrPollGet(side);
        OdspTelFecCollect(side);
    }
    OdspTelHeartbeat();
}

VOID OdspTelemetrySleep(VOID)
{
    BYTE side;

    /* PwrDn：DSP 掉电后 FW SRAM 的轮询实例不复存在——解除使能标志，
     * 下次 PwrUp 由 Update 重新 SET。快照/累计保留（语义上随之失效，
     * valid/fresh 同步清）。 */
    for(side = 0u; side < ODSP_TEL_SIDE_NUM; side++) {
        s_SnrPollOn[side] = 0u;
        s_SnrFresh[side]  = 0u;
        s_FecValid[side]  = 0u;
    }
    s_DspAlive = 0u;
}

BYTE OdspTelSnrGet(BYTE side, BYTE lane, WORD *val)
{
    if((side >= ODSP_TEL_SIDE_NUM) || (lane >= ODSP_TEL_LANE_NUM)
       || (val == NULL)) {
        return 0u;
    }
    *val = s_Snr[side][lane];
    return s_SnrFresh[side];
}

BYTE OdspTelLtpGet(BYTE side, BYTE lane, WORD *val)
{
    if((side >= ODSP_TEL_SIDE_NUM) || (lane >= ODSP_TEL_LANE_NUM)
       || (val == NULL)) {
        return 0u;
    }
    *val = s_Ltp[side][lane];
    return s_SnrFresh[side];
}

VOID OdspTelFecSnapGet(BYTE side, OdspTelFecSnap_t *snap)
{
    if((side >= ODSP_TEL_SIDE_NUM) || (snap == NULL)) {
        return;
    }
    snap->ber_cur   = s_FecState[side].pre_fec_ber.cur_ber;
    snap->ber_min   = s_FecState[side].pre_fec_ber.min_ber;
    snap->ber_max   = s_FecState[side].pre_fec_ber.max_ber;
    snap->ber_avg   = s_FecState[side].pre_fec_ber.average_ber;
    snap->frame_total   = s_FecState[side].cur_err.tot_frame_cnt;
    snap->ferc_interval = s_FecState[side].cur_err.tot_frame_uncorr_cnt;
    snap->ferc_total    = s_FercAcc[side];
    snap->sew_max       = (DWORD)s_FecState[side].cur_err.t_code;
    snap->valid         = s_FecValid[side];
}

BYTE OdspTelDspAliveGet(VOID)
{
    return s_DspAlive;
}

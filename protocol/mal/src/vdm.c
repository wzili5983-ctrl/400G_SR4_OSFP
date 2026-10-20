/***************************************************************************//**
 *   @file   vdm.c
 *   @brief  CMIS 5.3 VDM (Vendor Defined Module) — Group 1 implementation.
 *   @author Firmware team
 *******************************************************************************
 * Copyright 2026 Terabitcom, Inc.
 *
 * Data flow:  odsp_telemetry getters → g_SampleFilttedValues (统一采样缓冲)
 *             → VDM shadow pages (24h) → threshold compare (28h 影子)
 *             → Flag (2Ch, COR) → interrupt aggregate (2Dh)
 *
 * 页面内容来源（v5 方案：静态页全部走 Flash 参数区）：
 *   Page 20h 描述符 / Page 28h 阈值 / Page 01h byte 0x0E bit6 VDM 宣告
 *   均为 Flash 参数区默认值——MgiParamLoad 整区 memcpy 进 SRAM 影子即定稿，
 *   本文件不再初始化这些页。Page 2Dh Mask 默认 0xFF 由 g_MsaSpaceDefaultSeedInSram
 *   （mgmt_init.c）种入。固件运行时只做：发布样本 → 阈值比较置 Flag →
 *   中断汇总 → Freeze 门禁。
 *
 * Instance table g_VdmInsInfo[VDM_INSTANCE_NUM] is the single source of
 * truth for instance classification (Instancetype/telSide) and all CMIS
 * landing points (value/flag/flagmsk/thr), mirroring g_StdPerfMon's role
 * for the standard monitors.  表项 = { desc（Page 20h 描述符元数据）,
 * vdminfo（PerfMonInfo_t 全套落点 + 采样槽）}。
 *
 * Freeze:  host 写 Page 2Fh FreezeRequest → VdmFreezeApply 把 statistic
 *          类实例（desc.Instancetype == VDM_INST_STATISTIC）定格在上次
 *          发布值；basic 类继续更新。无独立快照缓冲——样本存
 *          g_SampleFilttedValues，阈值比较直读该缓冲。
 ******************************************************************************/
#include "vdm.h"
#include "monitor_impl.h"
#include "ral.h"
#include "adpat.h"
#include "odsp_telemetry.h"
#include "timer2_scheduler.h"
#include <string.h>

/* =========================================================================
 * Instance table — 32 instances.  Layout:
 *   0-7   SNR  (Media/Host × Lane1-4)              Basic      DB256
 *   8-15  LTP  (Media/Host × Lane1-4)              Basic      DB256
 *  16-23  Pre-FEC BER (cur/avg/min/max × Media/Host) Basic/Stat F16
 *  24-31  FERC (cur/avg/min/max media, cur/avg host,
 *               tot media/host)                    Basic/Stat U16CNT
 *
 * Field layout per row (VdmInsInfo_t):
 *   desc   = { valueTab(20h), valueOffset = i*2, Instancetype, telSide }
 *   vdminfo= { adindex, cal=NULL,
 *              value  = { perfvalue, tab(24h), offset = i*2, scale },
 *              flag   = { tab(2Ch), offsets = {i/2 ×4}, bits = nibble of instance },
 *              flagmsk= { tab(2Dh), 同 flag },
 *              thr    = { tab(28h), {set*8+6, +4, +2, +0}, scale },
 *              calcoe = { B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } }
 * Flag/Mask bits are in AlarmWarning_t order [WARNING_LOW, WARNING_HIGH,
 * ALARM_LOW, ALARM_HIGH]; even instance i occupies the high nibble of byte
 * i/2, odd instance the low nibble.  CMIS 5.3 2Ch 每 nibble 自低位向高位 =
 * HighTCAlarm/LowTCAlarm/HighTCWarning/LowTCWarning，故
 *   偶实例（高 nibble）= { BIT7, BIT6, BIT5, BIT4 }
 *   奇实例（低 nibble）= { BIT3, BIT2, BIT1, BIT0 }
 * Thr offsets are in the same AlarmWarning_t order: LW/HW/LA/HA at
 * set*8+6/+4/+2/+0（Page 28h 寄存器序 HA@+0 LA@+2 HW@+4 LW@+6）。
 * scale 分族：0-15 → DB256、16-23 → F16、24-31 → U16CNT。
 */
VdmInsInfo_t g_VdmInsInfo[VDM_INSTANCE_NUM] = {
    /*  0 SNR media lane1 */
    { { PAGE_ID_20H_0,  0U, VDM_INST_BASIC, ODSP_TEL_SIDE_MEDIA },
      { MON_MSA_ID_SNR_MEDIA1, NULL,
        { 0U, PAGE_ID_24H_0,  0U, MON_SCALE_DB256 },
        { PAGE_ID_2CH_0, {  0U,  0U,  0U,  0U }, { BIT7, BIT6, BIT5, BIT4 } },
        { PAGE_ID_2DH_0, {  0U,  0U,  0U,  0U }, { BIT7, BIT6, BIT5, BIT4 } },
        { PAGE_ID_28H_0, {  6U,  4U,  2U,  0U }, MON_SCALE_DB256 },
        { PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } } },
    /*  1 SNR media lane2 */
    { { PAGE_ID_20H_0,  2U, VDM_INST_BASIC, ODSP_TEL_SIDE_MEDIA },
      { MON_MSA_ID_SNR_MEDIA2, NULL,
        { 0U, PAGE_ID_24H_0,  2U, MON_SCALE_DB256 },
        { PAGE_ID_2CH_0, {  0U,  0U,  0U,  0U }, { BIT3, BIT2, BIT1, BIT0 } },
        { PAGE_ID_2DH_0, {  0U,  0U,  0U,  0U }, { BIT3, BIT2, BIT1, BIT0 } },
        { PAGE_ID_28H_0, {  6U,  4U,  2U,  0U }, MON_SCALE_DB256 },
        { PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } } },
    /*  2 SNR media lane3 */
    { { PAGE_ID_20H_0,  4U, VDM_INST_BASIC, ODSP_TEL_SIDE_MEDIA },
      { MON_MSA_ID_SNR_MEDIA3, NULL,
        { 0U, PAGE_ID_24H_0,  4U, MON_SCALE_DB256 },
        { PAGE_ID_2CH_0, {  1U,  1U,  1U,  1U }, { BIT7, BIT6, BIT5, BIT4 } },
        { PAGE_ID_2DH_0, {  1U,  1U,  1U,  1U }, { BIT7, BIT6, BIT5, BIT4 } },
        { PAGE_ID_28H_0, {  6U,  4U,  2U,  0U }, MON_SCALE_DB256 },
        { PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } } },
    /*  3 SNR media lane4 */
    { { PAGE_ID_20H_0,  6U, VDM_INST_BASIC, ODSP_TEL_SIDE_MEDIA },
      { MON_MSA_ID_SNR_MEDIA4, NULL,
        { 0U, PAGE_ID_24H_0,  6U, MON_SCALE_DB256 },
        { PAGE_ID_2CH_0, {  1U,  1U,  1U,  1U }, { BIT3, BIT2, BIT1, BIT0 } },
        { PAGE_ID_2DH_0, {  1U,  1U,  1U,  1U }, { BIT3, BIT2, BIT1, BIT0 } },
        { PAGE_ID_28H_0, {  6U,  4U,  2U,  0U }, MON_SCALE_DB256 },
        { PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } } },
    /*  4 SNR host lane1 */
    { { PAGE_ID_20H_0,  8U, VDM_INST_BASIC, ODSP_TEL_SIDE_HOST },
      { MON_MSA_ID_SNR_HOST1, NULL,
        { 0U, PAGE_ID_24H_0,  8U, MON_SCALE_DB256 },
        { PAGE_ID_2CH_0, {  2U,  2U,  2U,  2U }, { BIT7, BIT6, BIT5, BIT4 } },
        { PAGE_ID_2DH_0, {  2U,  2U,  2U,  2U }, { BIT7, BIT6, BIT5, BIT4 } },
        { PAGE_ID_28H_0, {  6U,  4U,  2U,  0U }, MON_SCALE_DB256 },
        { PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } } },
    /*  5 SNR host lane2 */
    { { PAGE_ID_20H_0, 10U, VDM_INST_BASIC, ODSP_TEL_SIDE_HOST },
      { MON_MSA_ID_SNR_HOST2, NULL,
        { 0U, PAGE_ID_24H_0, 10U, MON_SCALE_DB256 },
        { PAGE_ID_2CH_0, {  2U,  2U,  2U,  2U }, { BIT3, BIT2, BIT1, BIT0 } },
        { PAGE_ID_2DH_0, {  2U,  2U,  2U,  2U }, { BIT3, BIT2, BIT1, BIT0 } },
        { PAGE_ID_28H_0, {  6U,  4U,  2U,  0U }, MON_SCALE_DB256 },
        { PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } } },
    /*  6 SNR host lane3 */
    { { PAGE_ID_20H_0, 12U, VDM_INST_BASIC, ODSP_TEL_SIDE_HOST },
      { MON_MSA_ID_SNR_HOST3, NULL,
        { 0U, PAGE_ID_24H_0, 12U, MON_SCALE_DB256 },
        { PAGE_ID_2CH_0, {  3U,  3U,  3U,  3U }, { BIT7, BIT6, BIT5, BIT4 } },
        { PAGE_ID_2DH_0, {  3U,  3U,  3U,  3U }, { BIT7, BIT6, BIT5, BIT4 } },
        { PAGE_ID_28H_0, {  6U,  4U,  2U,  0U }, MON_SCALE_DB256 },
        { PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } } },
    /*  7 SNR host lane4 */
    { { PAGE_ID_20H_0, 14U, VDM_INST_BASIC, ODSP_TEL_SIDE_HOST },
      { MON_MSA_ID_SNR_HOST4, NULL,
        { 0U, PAGE_ID_24H_0, 14U, MON_SCALE_DB256 },
        { PAGE_ID_2CH_0, {  3U,  3U,  3U,  3U }, { BIT3, BIT2, BIT1, BIT0 } },
        { PAGE_ID_2DH_0, {  3U,  3U,  3U,  3U }, { BIT3, BIT2, BIT1, BIT0 } },
        { PAGE_ID_28H_0, {  6U,  4U,  2U,  0U }, MON_SCALE_DB256 },
        { PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } } },
    /*  8 LTP media lane1 */
    { { PAGE_ID_20H_0, 16U, VDM_INST_BASIC, ODSP_TEL_SIDE_MEDIA },
      { MON_MSA_ID_LTP_MEDIA1, NULL,
        { 0U, PAGE_ID_24H_0, 16U, MON_SCALE_DB256 },
        { PAGE_ID_2CH_0, {  4U,  4U,  4U,  4U }, { BIT7, BIT6, BIT5, BIT4 } },
        { PAGE_ID_2DH_0, {  4U,  4U,  4U,  4U }, { BIT7, BIT6, BIT5, BIT4 } },
        { PAGE_ID_28H_0, { 14U, 12U, 10U,  8U }, MON_SCALE_DB256 },
        { PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } } },
    /*  9 LTP media lane2 */
    { { PAGE_ID_20H_0, 18U, VDM_INST_BASIC, ODSP_TEL_SIDE_MEDIA },
      { MON_MSA_ID_LTP_MEDIA2, NULL,
        { 0U, PAGE_ID_24H_0, 18U, MON_SCALE_DB256 },
        { PAGE_ID_2CH_0, {  4U,  4U,  4U,  4U }, { BIT3, BIT2, BIT1, BIT0 } },
        { PAGE_ID_2DH_0, {  4U,  4U,  4U,  4U }, { BIT3, BIT2, BIT1, BIT0 } },
        { PAGE_ID_28H_0, { 14U, 12U, 10U,  8U }, MON_SCALE_DB256 },
        { PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } } },
    /* 10 LTP media lane3 */
    { { PAGE_ID_20H_0, 20U, VDM_INST_BASIC, ODSP_TEL_SIDE_MEDIA },
      { MON_MSA_ID_LTP_MEDIA3, NULL,
        { 0U, PAGE_ID_24H_0, 20U, MON_SCALE_DB256 },
        { PAGE_ID_2CH_0, {  5U,  5U,  5U,  5U }, { BIT7, BIT6, BIT5, BIT4 } },
        { PAGE_ID_2DH_0, {  5U,  5U,  5U,  5U }, { BIT7, BIT6, BIT5, BIT4 } },
        { PAGE_ID_28H_0, { 14U, 12U, 10U,  8U }, MON_SCALE_DB256 },
        { PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } } },
    /* 11 LTP media lane4 */
    { { PAGE_ID_20H_0, 22U, VDM_INST_BASIC, ODSP_TEL_SIDE_MEDIA },
      { MON_MSA_ID_LTP_MEDIA4, NULL,
        { 0U, PAGE_ID_24H_0, 22U, MON_SCALE_DB256 },
        { PAGE_ID_2CH_0, {  5U,  5U,  5U,  5U }, { BIT3, BIT2, BIT1, BIT0 } },
        { PAGE_ID_2DH_0, {  5U,  5U,  5U,  5U }, { BIT3, BIT2, BIT1, BIT0 } },
        { PAGE_ID_28H_0, { 14U, 12U, 10U,  8U }, MON_SCALE_DB256 },
        { PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } } },
    /* 12 LTP host lane1 */
    { { PAGE_ID_20H_0, 24U, VDM_INST_BASIC, ODSP_TEL_SIDE_HOST },
      { MON_MSA_ID_LTP_HOST1, NULL,
        { 0U, PAGE_ID_24H_0, 24U, MON_SCALE_DB256 },
        { PAGE_ID_2CH_0, {  6U,  6U,  6U,  6U }, { BIT7, BIT6, BIT5, BIT4 } },
        { PAGE_ID_2DH_0, {  6U,  6U,  6U,  6U }, { BIT7, BIT6, BIT5, BIT4 } },
        { PAGE_ID_28H_0, { 14U, 12U, 10U,  8U }, MON_SCALE_DB256 },
        { PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } } },
    /* 13 LTP host lane2 */
    { { PAGE_ID_20H_0, 26U, VDM_INST_BASIC, ODSP_TEL_SIDE_HOST },
      { MON_MSA_ID_LTP_HOST2, NULL,
        { 0U, PAGE_ID_24H_0, 26U, MON_SCALE_DB256 },
        { PAGE_ID_2CH_0, {  6U,  6U,  6U,  6U }, { BIT3, BIT2, BIT1, BIT0 } },
        { PAGE_ID_2DH_0, {  6U,  6U,  6U,  6U }, { BIT3, BIT2, BIT1, BIT0 } },
        { PAGE_ID_28H_0, { 14U, 12U, 10U,  8U }, MON_SCALE_DB256 },
        { PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } } },
    /* 14 LTP host lane3 */
    { { PAGE_ID_20H_0, 28U, VDM_INST_BASIC, ODSP_TEL_SIDE_HOST },
      { MON_MSA_ID_LTP_HOST3, NULL,
        { 0U, PAGE_ID_24H_0, 28U, MON_SCALE_DB256 },
        { PAGE_ID_2CH_0, {  7U,  7U,  7U,  7U }, { BIT7, BIT6, BIT5, BIT4 } },
        { PAGE_ID_2DH_0, {  7U,  7U,  7U,  7U }, { BIT7, BIT6, BIT5, BIT4 } },
        { PAGE_ID_28H_0, { 14U, 12U, 10U,  8U }, MON_SCALE_DB256 },
        { PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } } },
    /* 15 LTP host lane4 */
    { { PAGE_ID_20H_0, 30U, VDM_INST_BASIC, ODSP_TEL_SIDE_HOST },
      { MON_MSA_ID_LTP_HOST4, NULL,
        { 0U, PAGE_ID_24H_0, 30U, MON_SCALE_DB256 },
        { PAGE_ID_2CH_0, {  7U,  7U,  7U,  7U }, { BIT3, BIT2, BIT1, BIT0 } },
        { PAGE_ID_2DH_0, {  7U,  7U,  7U,  7U }, { BIT3, BIT2, BIT1, BIT0 } },
        { PAGE_ID_28H_0, { 14U, 12U, 10U,  8U }, MON_SCALE_DB256 },
        { PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } } },
    /* 16 BER cur media */
    { { PAGE_ID_20H_0, 32U, VDM_INST_BASIC, ODSP_TEL_SIDE_MEDIA },
      { MON_MSA_ID_BER_CUR_MEDIA, NULL,
        { 0U, PAGE_ID_24H_0, 32U, MON_SCALE_F16 },
        { PAGE_ID_2CH_0, {  8U,  8U,  8U,  8U }, { BIT7, BIT6, BIT5, BIT4 } },
        { PAGE_ID_2DH_0, {  8U,  8U,  8U,  8U }, { BIT7, BIT6, BIT5, BIT4 } },
        { PAGE_ID_28H_0, { 22U, 20U, 18U, 16U }, MON_SCALE_F16 },
        { PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } } },
    /* 17 BER avg media */
    { { PAGE_ID_20H_0, 34U, VDM_INST_STATISTIC, ODSP_TEL_SIDE_MEDIA },
      { MON_MSA_ID_BER_AVG_MEDIA, NULL,
        { 0U, PAGE_ID_24H_0, 34U, MON_SCALE_F16 },
        { PAGE_ID_2CH_0, {  8U,  8U,  8U,  8U }, { BIT3, BIT2, BIT1, BIT0 } },
        { PAGE_ID_2DH_0, {  8U,  8U,  8U,  8U }, { BIT3, BIT2, BIT1, BIT0 } },
        { PAGE_ID_28H_0, { 22U, 20U, 18U, 16U }, MON_SCALE_F16 },
        { PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } } },
    /* 18 BER min media */
    { { PAGE_ID_20H_0, 36U, VDM_INST_STATISTIC, ODSP_TEL_SIDE_MEDIA },
      { MON_MSA_ID_BER_MIN_MEDIA, NULL,
        { 0U, PAGE_ID_24H_0, 36U, MON_SCALE_F16 },
        { PAGE_ID_2CH_0, {  9U,  9U,  9U,  9U }, { BIT7, BIT6, BIT5, BIT4 } },
        { PAGE_ID_2DH_0, {  9U,  9U,  9U,  9U }, { BIT7, BIT6, BIT5, BIT4 } },
        { PAGE_ID_28H_0, { 22U, 20U, 18U, 16U }, MON_SCALE_F16 },
        { PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } } },
    /* 19 BER max media */
    { { PAGE_ID_20H_0, 38U, VDM_INST_STATISTIC, ODSP_TEL_SIDE_MEDIA },
      { MON_MSA_ID_BER_MAX_MEDIA, NULL,
        { 0U, PAGE_ID_24H_0, 38U, MON_SCALE_F16 },
        { PAGE_ID_2CH_0, {  9U,  9U,  9U,  9U }, { BIT3, BIT2, BIT1, BIT0 } },
        { PAGE_ID_2DH_0, {  9U,  9U,  9U,  9U }, { BIT3, BIT2, BIT1, BIT0 } },
        { PAGE_ID_28H_0, { 22U, 20U, 18U, 16U }, MON_SCALE_F16 },
        { PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } } },
    /* 20 BER cur host */
    { { PAGE_ID_20H_0, 40U, VDM_INST_BASIC, ODSP_TEL_SIDE_HOST },
      { MON_MSA_ID_BER_CUR_HOST, NULL,
        { 0U, PAGE_ID_24H_0, 40U, MON_SCALE_F16 },
        { PAGE_ID_2CH_0, { 10U, 10U, 10U, 10U }, { BIT7, BIT6, BIT5, BIT4 } },
        { PAGE_ID_2DH_0, { 10U, 10U, 10U, 10U }, { BIT7, BIT6, BIT5, BIT4 } },
        { PAGE_ID_28H_0, { 22U, 20U, 18U, 16U }, MON_SCALE_F16 },
        { PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } } },
    /* 21 BER avg host */
    { { PAGE_ID_20H_0, 42U, VDM_INST_STATISTIC, ODSP_TEL_SIDE_HOST },
      { MON_MSA_ID_BER_AVG_HOST, NULL,
        { 0U, PAGE_ID_24H_0, 42U, MON_SCALE_F16 },
        { PAGE_ID_2CH_0, { 10U, 10U, 10U, 10U }, { BIT3, BIT2, BIT1, BIT0 } },
        { PAGE_ID_2DH_0, { 10U, 10U, 10U, 10U }, { BIT3, BIT2, BIT1, BIT0 } },
        { PAGE_ID_28H_0, { 22U, 20U, 18U, 16U }, MON_SCALE_F16 },
        { PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } } },
    /* 22 BER min host */
    { { PAGE_ID_20H_0, 44U, VDM_INST_STATISTIC, ODSP_TEL_SIDE_HOST },
      { MON_MSA_ID_BER_MIN_HOST, NULL,
        { 0U, PAGE_ID_24H_0, 44U, MON_SCALE_F16 },
        { PAGE_ID_2CH_0, { 11U, 11U, 11U, 11U }, { BIT7, BIT6, BIT5, BIT4 } },
        { PAGE_ID_2DH_0, { 11U, 11U, 11U, 11U }, { BIT7, BIT6, BIT5, BIT4 } },
        { PAGE_ID_28H_0, { 22U, 20U, 18U, 16U }, MON_SCALE_F16 },
        { PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } } },
    /* 23 BER max host */
    { { PAGE_ID_20H_0, 46U, VDM_INST_STATISTIC, ODSP_TEL_SIDE_HOST },
      { MON_MSA_ID_BER_MAX_HOST, NULL,
        { 0U, PAGE_ID_24H_0, 46U, MON_SCALE_F16 },
        { PAGE_ID_2CH_0, { 11U, 11U, 11U, 11U }, { BIT3, BIT2, BIT1, BIT0 } },
        { PAGE_ID_2DH_0, { 11U, 11U, 11U, 11U }, { BIT3, BIT2, BIT1, BIT0 } },
        { PAGE_ID_28H_0, { 22U, 20U, 18U, 16U }, MON_SCALE_F16 },
        { PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } } },
    /* 24 FERC cur media */
    { { PAGE_ID_20H_0, 48U, VDM_INST_BASIC, ODSP_TEL_SIDE_MEDIA },
      { MON_MSA_ID_FERC_CUR_MEDIA, NULL,
        { 0U, PAGE_ID_24H_0, 48U, MON_SCALE_U16CNT },
        { PAGE_ID_2CH_0, { 12U, 12U, 12U, 12U }, { BIT7, BIT6, BIT5, BIT4 } },
        { PAGE_ID_2DH_0, { 12U, 12U, 12U, 12U }, { BIT7, BIT6, BIT5, BIT4 } },
        { PAGE_ID_28H_0, { 30U, 28U, 26U, 24U }, MON_SCALE_U16CNT },
        { PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } } },
    /* 25 FERC avg media */
    { { PAGE_ID_20H_0, 50U, VDM_INST_STATISTIC, ODSP_TEL_SIDE_MEDIA },
      { MON_MSA_ID_FERC_AVG_MEDIA, NULL,
        { 0U, PAGE_ID_24H_0, 50U, MON_SCALE_U16CNT },
        { PAGE_ID_2CH_0, { 12U, 12U, 12U, 12U }, { BIT3, BIT2, BIT1, BIT0 } },
        { PAGE_ID_2DH_0, { 12U, 12U, 12U, 12U }, { BIT3, BIT2, BIT1, BIT0 } },
        { PAGE_ID_28H_0, { 30U, 28U, 26U, 24U }, MON_SCALE_U16CNT },
        { PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } } },
    /* 26 FERC min media */
    { { PAGE_ID_20H_0, 52U, VDM_INST_STATISTIC, ODSP_TEL_SIDE_MEDIA },
      { MON_MSA_ID_FERC_MIN_MEDIA, NULL,
        { 0U, PAGE_ID_24H_0, 52U, MON_SCALE_U16CNT },
        { PAGE_ID_2CH_0, { 13U, 13U, 13U, 13U }, { BIT7, BIT6, BIT5, BIT4 } },
        { PAGE_ID_2DH_0, { 13U, 13U, 13U, 13U }, { BIT7, BIT6, BIT5, BIT4 } },
        { PAGE_ID_28H_0, { 30U, 28U, 26U, 24U }, MON_SCALE_U16CNT },
        { PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } } },
    /* 27 FERC max media */
    { { PAGE_ID_20H_0, 54U, VDM_INST_STATISTIC, ODSP_TEL_SIDE_MEDIA },
      { MON_MSA_ID_FERC_MAX_MEDIA, NULL,
        { 0U, PAGE_ID_24H_0, 54U, MON_SCALE_U16CNT },
        { PAGE_ID_2CH_0, { 13U, 13U, 13U, 13U }, { BIT3, BIT2, BIT1, BIT0 } },
        { PAGE_ID_2DH_0, { 13U, 13U, 13U, 13U }, { BIT3, BIT2, BIT1, BIT0 } },
        { PAGE_ID_28H_0, { 30U, 28U, 26U, 24U }, MON_SCALE_U16CNT },
        { PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } } },
    /* 28 FERC cur host */
    { { PAGE_ID_20H_0, 56U, VDM_INST_BASIC, ODSP_TEL_SIDE_HOST },
      { MON_MSA_ID_FERC_CUR_HOST, NULL,
        { 0U, PAGE_ID_24H_0, 56U, MON_SCALE_U16CNT },
        { PAGE_ID_2CH_0, { 14U, 14U, 14U, 14U }, { BIT7, BIT6, BIT5, BIT4 } },
        { PAGE_ID_2DH_0, { 14U, 14U, 14U, 14U }, { BIT7, BIT6, BIT5, BIT4 } },
        { PAGE_ID_28H_0, { 30U, 28U, 26U, 24U }, MON_SCALE_U16CNT },
        { PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } } },
    /* 29 FERC avg host */
    { { PAGE_ID_20H_0, 58U, VDM_INST_STATISTIC, ODSP_TEL_SIDE_HOST },
      { MON_MSA_ID_FERC_AVG_HOST, NULL,
        { 0U, PAGE_ID_24H_0, 58U, MON_SCALE_U16CNT },
        { PAGE_ID_2CH_0, { 14U, 14U, 14U, 14U }, { BIT3, BIT2, BIT1, BIT0 } },
        { PAGE_ID_2DH_0, { 14U, 14U, 14U, 14U }, { BIT3, BIT2, BIT1, BIT0 } },
        { PAGE_ID_28H_0, { 30U, 28U, 26U, 24U }, MON_SCALE_U16CNT },
        { PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } } },
    /* 30 FERC tot media */
    { { PAGE_ID_20H_0, 60U, VDM_INST_STATISTIC, ODSP_TEL_SIDE_MEDIA },
      { MON_MSA_ID_FERC_TOT_MEDIA, NULL,
        { 0U, PAGE_ID_24H_0, 60U, MON_SCALE_U16CNT },
        { PAGE_ID_2CH_0, { 15U, 15U, 15U, 15U }, { BIT7, BIT6, BIT5, BIT4 } },
        { PAGE_ID_2DH_0, { 15U, 15U, 15U, 15U }, { BIT7, BIT6, BIT5, BIT4 } },
        { PAGE_ID_28H_0, { 30U, 28U, 26U, 24U }, MON_SCALE_U16CNT },
        { PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } } },
    /* 31 FERC tot host */
    { { PAGE_ID_20H_0, 62U, VDM_INST_STATISTIC, ODSP_TEL_SIDE_HOST },
      { MON_MSA_ID_FERC_TOT_HOST, NULL,
        { 0U, PAGE_ID_24H_0, 62U, MON_SCALE_U16CNT },
        { PAGE_ID_2CH_0, { 15U, 15U, 15U, 15U }, { BIT3, BIT2, BIT1, BIT0 } },
        { PAGE_ID_2DH_0, { 15U, 15U, 15U, 15U }, { BIT3, BIT2, BIT1, BIT0 } },
        { PAGE_ID_28H_0, { 30U, 28U, 26U, 24U }, MON_SCALE_U16CNT },
        { PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT } } },
};

/* =========================================================================
 * State
 * ========================================================================= */
STATIC volatile BYTE s_VdmFrozen   = 0U;
/* ★ Flag 无固件私有缓冲：2Ch 影子本身就是闩锁（COR——置位 OR 进影子、host
 * 读经 DtCorClear 清），VdmThresholdCheck 的 newFlags[] 只是每周期栈上草稿。 */

/* =========================================================================
 * Task timing — same 1s cadence as MonTask, staggered
 * ========================================================================= */
#define VDM_TASK_DELAY_TICKS    150U    /* stagger from MonTask (100) */
#define VDM_TASK_INTERVAL_TICKS 300U    /* ~1s effective with 10 calls */

/* Page 2Fh 布局常量（PAGE_2FH_ADDR_xxx / PAGE_2FH_BIT_xxx）统一定义于 ral.h，
 * 本文件只消费——避免两处私定义漂移。 */

/* =========================================================================
 * Shadow helpers — 多字节读写锁内执行（线程上下文防 I2C1 ISR 并发撕裂）；
 * 单字节读写不加锁（M33 对齐单字节访存原子，ISR 上下文禁锁）。
 * ========================================================================= */
STATIC DWORD VdmShadowRead16(TabIndex_t tab, BYTE offset, WORD *val)
{
    BYTE b[2];
    AdpatShadowLock();
    if(ReadSramProtocolMsaSpace(tab, offset, b, 2U) != OK) {
        AdpatShadowUnlock();
        return ERR;
    }
    AdpatShadowUnlock();
    *val = (WORD)(((WORD)b[0] << 8) | (WORD)b[1]);
    return OK;
}

STATIC DWORD VdmShadowWrite16(TabIndex_t tab, BYTE offset, WORD val)
{
    BYTE buf[2];
    DWORD r;
    buf[0] = (BYTE)(val >> 8);
    buf[1] = (BYTE)(val & 0xFFU);
    AdpatShadowLock();
    r = WriteSramProtocolMsaSpace(tab, offset, buf, 2U);
    AdpatShadowUnlock();
    return r;
}

STATIC VOID VdmShadowWrite8(TabIndex_t tab, BYTE offset, BYTE val)
{
    (void)WriteSramProtocolMsaSpace(tab, offset, &val, 1U);
}

STATIC BYTE VdmShadowRead8(TabIndex_t tab, BYTE offset)
{
    BYTE val = 0U;
    (void)ReadSramProtocolMsaSpace(tab, offset, &val, 1U);
    return val;
}

/* =========================================================================
 * F16 encoder — IEEE 754 binary16
 *
 * Input:  double (typically 1e-4 .. 1e-9 for BER).
 * Output: U16 F16 bit pattern.
 *
 * Special values:
 *   NaN       → 0x7E00
 *   +Inf      → 0x7C00
 *   >65504    → 0x7BFF (saturated)
 *   <F16 min  → 0x0000 (flushed to zero)
 *
 * ★ 注：CMIS 的 F16 是 SFF-8636 十进制 m·10^(s-24) 编码，与 IEEE binary16
 *   位模式不同；此处保持 IEEE binary16（正数位模式单调，阈值可直接 U16
 *   整型比较），与阈值推荐值同编码即可工作。
 * ========================================================================= */
WORD VdmFloatToF16(double val)
{
    union { double d; DWORD u[2]; } conv;
    DWORD bits;
    DWORD sign;
    int   srcExp;
    DWORD srcMant;
    int   dstExp;
    DWORD dstMant;
    double absval;

    if(val != val) { return VDM_F16_NAN; }       /* NaN check */
    conv.d = val;
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    bits = conv.u[0];
#else
    bits = conv.u[1];     /* little-endian: high 32 bits contain sign+exp+mant_hi */
#endif

    sign = (bits >> 16) & 0x8000U;

    if(val == 0.0) { return (WORD)sign; }

    /* Handle infinities and overflow */
    if(val > 65504.0)  { return (WORD)(sign | VDM_F16_MAX_FINITE); }
    if(val < -65504.0) { return (WORD)(sign | VDM_F16_MAX_FINITE); }

    /* F16 denormals: |val| < 2^-24 → flush to zero */
    absval = (val < 0.0) ? -val : val;
    if(absval < 5.9604644775390625e-8) { return (WORD)sign; }

    /* Extract double fields */
    srcExp  = (int)((bits >> 20) & 0x7FFU) - 1023;
    srcMant = (bits & 0xFFFFFU) | 0x100000U;    /* implicit 1 */

    /* Convert to F16 biased exponent */
    dstExp = srcExp + 15;
    if(dstExp >= 0x1F) { return (WORD)(sign | VDM_F16_MAX_FINITE); }  /* overflow */
    if(dstExp < 1) {
        /* F16 denormal: shift mantissa right by (1-dstExp) */
        int shift = 1 - dstExp;
        if(shift > 11) { return (WORD)sign; }   /* underflow */
        dstMant = (srcMant >> (shift + 1)) & 0x3FFU;
        return (WORD)(sign | dstMant);
    }

    dstMant = (srcMant >> 11) & 0x3FFU;         /* top 10 bits of mantissa */
    return (WORD)(sign | ((DWORD)dstExp << 10) | dstMant);
}

/* =========================================================================
 * Saturate DDWORD to U16
 * ========================================================================= */
STATIC WORD SaturateDdwordToU16(DDWORD v)
{
    if(v > 0xFFFFULL) { return 0xFFFFU; }
    return (WORD)v;
}

/* =========================================================================
 * Sample publication — odsp_telemetry getters → g_SampleFilttedValues →
 * shadow (Page 24h).  Statistic 类实例（desc.Instancetype）Freeze 门禁：
 * 冻结期间样本缓冲与影子均保持上次发布值。
 * Getter 失败 → VDM_F16_UNAVAILABLE 落两处。
 * Samples are Big Endian X16 in shadow.
 * ========================================================================= */
STATIC VOID VdmPublishSamples(VOID)
{
    BYTE i;
    WORD val;
    CONST VdmInsInfo_t *p;

    for(i = 0U; i < (BYTE)VDM_INSTANCE_NUM; i++) {
        p = &g_VdmInsInfo[i];

        /* Statistic 类 Freeze 期间保持上次发布值 */
        if((s_VdmFrozen != 0U) && (p->desc.Instancetype == VDM_INST_STATISTIC)) {
            continue;
        }

        val = VDM_F16_UNAVAILABLE;

        if(i <= (BYTE)VDM_ID_SNR_HOST4) {
            /* SNR: instances 0-7，lane = i & 3 */
            if(OdspTelSnrGet(p->desc.telSide, (BYTE)(i & 3U), &val) == 0U) {
                val = VDM_F16_UNAVAILABLE;
            }
        } else if(i <= (BYTE)VDM_ID_LTP_HOST4) {
            /* LTP: instances 8-15，lane = (i-8) & 3 = i & 3 */
            if(OdspTelLtpGet(p->desc.telSide, (BYTE)(i & 3U), &val) == 0U) {
                val = VDM_F16_UNAVAILABLE;
            }
        } else {
            /* BER/FERC 族 — 每侧一次 FEC 快照 */
            OdspTelFecSnap_t snap;
            OdspTelFecSnapGet(p->desc.telSide, &snap);
            if(snap.valid == 0U) {
                val = VDM_F16_UNAVAILABLE;
            } else {
                switch(i) {
                case VDM_ID_BER_CUR_MEDIA:
                case VDM_ID_BER_CUR_HOST:
                    val = VdmFloatToF16(snap.ber_cur);
                    break;
                case VDM_ID_BER_AVG_MEDIA:
                case VDM_ID_BER_AVG_HOST:
                    val = VdmFloatToF16(snap.ber_avg);
                    break;
                case VDM_ID_BER_MIN_MEDIA:
                case VDM_ID_BER_MIN_HOST:
                    val = VdmFloatToF16(snap.ber_min);
                    break;
                case VDM_ID_BER_MAX_MEDIA:
                case VDM_ID_BER_MAX_HOST:
                    val = VdmFloatToF16(snap.ber_max);
                    break;
                case VDM_ID_FERC_TOT_MEDIA:
                case VDM_ID_FERC_TOT_HOST:
                    val = SaturateDdwordToU16(snap.ferc_total);
                    break;
                default:
                    /* FERC cur(24/28) = 区间计数；avg/min/max(25-27/29)
                     * 暂用同一区间计数代理（telemetry 无独立 FERC 统计）。 */
                    val = SaturateDdwordToU16(snap.ferc_interval);
                    break;
                }
            }
        }

        g_SampleFilttedValues[p->vdminfo.adindex] = val;
        (void)VdmShadowWrite16(p->vdminfo.value.tab, p->vdminfo.value.offset, val);
    }
}

/* =========================================================================
 * Threshold comparison → Flag set (COR)
 *
 * 阈值与监视值同量纲同编码，直接 U16 整型比较（F16 用 IEEE binary16，
 * 正数位模式单调）。阈值读 28h 影子（Flash 参数区默认值经 MgiParamLoad
 * 落入；host 改阈值未支持——28h 表项 num=0 写丢弃）。
 * 门禁约定：high 侧 thr==0xFFFF 视为禁用，low 侧 thr==0x0000 视为禁用。
 * ========================================================================= */

/* Evaluate one alarm/warning class and accumulate its Flag bit.
 * isHigh: 1 = High side (thr 0xFFFF disables), 0 = Low side (0 = disables). */
STATIC VOID VdmFlagBitSet(WORD sample, WORD thr, BYTE isHigh,
                          BYTE offset, Bit_t bit, BYTE *newFlags)
{
    BYTE trip;

    if(isHigh != 0U) {
        trip = (BYTE)(((thr != 0xFFFFU) && (sample > thr)) ? 1U : 0U);
    } else {
        trip = (BYTE)(((thr != 0x0000U) && (sample < thr)) ? 1U : 0U);
    }
    if((trip != 0U) && (offset < (VDM_INSTANCE_NUM / VDM_FLAG_INST_PER_BYTE))) {
        newFlags[offset] |= (BYTE)(1U << bit);
    }
}

STATIC VOID VdmThresholdCheck(VOID)
{
    BYTE i, idx;
    WORD sample;
    BYTE newFlags[VDM_INSTANCE_NUM / VDM_FLAG_INST_PER_BYTE];
    CONST VdmInsInfo_t *p;

    memset(newFlags, 0, sizeof(newFlags));

    for(i = 0U; i < (BYTE)VDM_INSTANCE_NUM; i++) {
        p = &g_VdmInsInfo[i];
        sample = g_SampleFilttedValues[p->vdminfo.adindex];

        /* idx 按 AlarmWarning_t 序 [LoWr, HiWr, LoAl, HiAl]，与
         * flag.offset[]/flag.bits[]/thr.offset[] 同序 */
        for(idx = 0U; idx < (BYTE)ALARM_WARNING_NUM; idx++) {
            WORD thr;
            BYTE isHigh;
            if(p->vdminfo.thr.offset[idx] == (BYTE)MAL_MON_THR_NONE) { continue; }
            if(VdmShadowRead16(p->vdminfo.thr.tab, p->vdminfo.thr.offset[idx], &thr) != OK) {
                continue;
            }
            isHigh = (BYTE)(((idx == (BYTE)WARNING_HIGH) || (idx == (BYTE)ALARM_HIGH)) ? 1U : 0U);
            VdmFlagBitSet(sample, thr, isHigh,
                          p->vdminfo.flag.offset[idx], p->vdminfo.flag.bits[idx],
                          newFlags);
        }
    }

    /* COR 语义：OR 进 2Ch 影子（只置不清，清由 host 读触发引擎 DtCorClear）。
     * 字节 RMW 须锁内防 ISR 清零交错丢更新（同 MonFlagUpdate）。 */
    AdpatShadowLock();
    for(i = 0U; i < (BYTE)(VDM_INSTANCE_NUM / VDM_FLAG_INST_PER_BYTE); i++) {
        BYTE existing = VdmShadowRead8(PAGE_ID_2CH_0, i);
        VdmShadowWrite8(PAGE_ID_2CH_0, i, (BYTE)(existing | newFlags[i]));
    }
    AdpatShadowUnlock();
}

/** 中断汇总：逐字节 flag & ~mask，存在未屏蔽挂起位返回 1。 */
STATIC BYTE VdmIntAggregate(VOID)
{
    BYTE i;
    for(i = 0U; i < (BYTE)(VDM_INSTANCE_NUM / VDM_FLAG_INST_PER_BYTE); i++) {
        BYTE flag = VdmShadowRead8(PAGE_ID_2CH_0, i);
        BYTE mask = VdmShadowRead8(PAGE_ID_2DH_0, i);
        if((flag & (BYTE)~mask) != 0U) { return 1U; }
    }
    return 0U;
}

/* =========================================================================
 * Freeze / Unfreeze — 由 ral_operate.c 的 OsfpOperate2FhFreezeReq /
 * OsfpOperate2FhUnfreezeReq 调用（OPF_IMMEDIATE，STPDET ISR 上下文）。
 *
 * 门禁只作用于 statistic 类实例（desc.Instancetype == VDM_INST_STATISTIC），
 * basic/current 类照常更新。无独立快照缓冲。
 *
 * 并发契约：只碰单字节影子（不加锁、无阻塞、无 Flash），ISR 安全。
 *   byte 145 状态字节唯一写者 = 本两函数（同 ISR 上下文串行，无 RMW 竞态）；
 *   s_VdmFrozen 单字节 volatile，跨上下文可见性由其原子性保证。
 * ========================================================================= */
VOID VdmFreezeApply(VOID)
{
    BYTE sts;

    s_VdmFrozen = 1U;

    /* 置 FreezeDone（byte 145 bit7） */
    sts = VdmShadowRead8(PAGE_ID_2FH_0, PAGE_2FH_ADDR_FREEZE_STS);
    sts |= PAGE_2FH_BIT_FREEZE_DONE;
    VdmShadowWrite8(PAGE_ID_2FH_0, PAGE_2FH_ADDR_FREEZE_STS, sts);
}

VOID VdmUnfreezeApply(VOID)
{
    BYTE sts;

    s_VdmFrozen = 0U;

    /* 清 FreezeDone、置 UnfreezeDone（byte 145 bit7/bit6） */
    sts = VdmShadowRead8(PAGE_ID_2FH_0, PAGE_2FH_ADDR_FREEZE_STS);
    sts = (BYTE)((sts & (BYTE)~PAGE_2FH_BIT_FREEZE_DONE) | PAGE_2FH_BIT_UNFREEZE_DONE);
    VdmShadowWrite8(PAGE_ID_2FH_0, PAGE_2FH_ADDR_FREEZE_STS, sts);
}

/* =========================================================================
 * VdmInit — called from MgiInit() after MonInit/OdspAdaptInit.
 * 描述符（20h）/阈值（28h）/Mask（2Dh）/01h bit6 宣告均来自 Flash 参数区
 * 与 MgiMgmtDefaultSeed，此处不再初始化；只清运行时态并挂周期任务。
 * ========================================================================= */
VOID VdmInit(VOID)
{
    s_VdmFrozen = 0U;

    /* Register VDM periodic task */
    g_TaskIds[TASK_ID_VDM] = Timer2SchedulerAdd(VdmTask, NULL,
                             VDM_TASK_DELAY_TICKS, VDM_TASK_INTERVAL_TICKS,
                             TIMER2_TASK_FOREVER);
}

/* =========================================================================
 * VdmTask — main VDM periodic function (1s effective, 300ms timer call)
 * ========================================================================= */
VOID VdmTask(VOID *arg)
{
    (VOID)arg;

    if(OdspTelDspAliveGet() == 0U) { return; }

    /* 1. Publish samples → g_SampleFilttedValues + shadow (24h);
     *    statistic 类 Freeze 门禁 */
    VdmPublishSamples();

    /* 2. Threshold comparison → Flag (2Ch, COR) */
    VdmThresholdCheck();

    /* 3. Interrupt aggregate（供监视层/INT 汇总接入） */
    (VOID)VdmIntAggregate();
}

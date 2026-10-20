/** @file odsp_opt_mon.h
 * @brief BCM87420 (ODSP) optical power monitor API.
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * BSC 读 BCM87420 内部光功率 ADC 寄存器，轮询 4 lane Rx 光功率原始码。
 * 当前为标定前占位实现：寄存器地址待 Broadcom 提供（NDA），原始码仅
 * 存本地静态缓冲、不入影子；标定公式定稿后接入 monitor_impl 的 RxPwr
 * cal 链（替换现有 MON_MSA_ID_RSSI0..3 TIA 模拟量代理路径）。
 */
#ifndef ODSP_OPT_MON_H
#define ODSP_OPT_MON_H

#include "basedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 监视通道数（4 lane，与 CMIS 11h:170-177 OpticalPowerRx1..4 对齐）。 */
#define ODSP_OPT_MON_LANE_NUM    4U

/* 光功率监视初始化（由 OdspAdaptInit 调用一次）。 */
VOID OdspOptMonInit(VOID);

/* 光功率监视轮询（OdspAdaptTask 周期调用，100ms）：
 * 内部 10 拍分频 = 1s 采样周期（与 GN1848 采集周期对齐）。
 * 读 4 lane 原始码存入 s_RawPwr[4]；不写影子——标定公式未定，
 * 避免发布垃圾值。标定后接入 monitor_impl 的 RxPwr cal 链（替换 TIA RSSI 代理）。 */
VOID OdspOptMonPoll(VOID);

/* 最近一次采样原始码（ODSP_OPT_MON_LANE_NUM=4 通道；未采样=0）。
 * 仅供调试/标定阶段查询。 */
DWORD OdspOptMonRawGet(BYTE lane);

#ifdef __cplusplus
}
#endif

#endif

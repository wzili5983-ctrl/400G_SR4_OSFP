/** @file odsp_los_mon.h
 * @brief OPRXLOS (PD8) 周期采样 + 11h:147 LOSFlagRx COR 置位。
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * OPRXLOS 是 BCM87420 DSP 输出的聚合光接收 LOS（高有效），4 路 media lane
 * 共享一根线。本模块被 OdspAdaptTask 以 100ms 周期调用 OdspLosMonPoll，
 * 在上升沿（无 LOS → LOS）时经 AdpatShadowLock 保护读-改-写 SRAM 影子
 * 11h:147 LOSFlagRx bit0-3（SR4 lane1-4 同步置位）。
 */
#ifndef ODSP_LOS_MON_H
#define ODSP_LOS_MON_H

#include "basedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* OPRXLOS 监视初始化（由 OdspAdaptInit 调用一次）。
 * 当前为空实现——所有状态在首次 Poll 时惰性初始化。 */
VOID OdspLosMonInit(VOID);

/* OPRXLOS 轮询（OdspAdaptTask 周期调用，100ms）：
 * 上升沿（无 LOS → LOS）时置位 11h:147 LOSFlagRx bit0-3（SR4 lane1-4 共享）。
 * 下降沿不清零——COR 语义，清零由 host 读触发 RAL 引擎 DtCorClear。
 * 调用前须 OdspInitIsReady()==1（OdspAdaptTask 已做电源守卫）。 */
VOID OdspLosMonPoll(VOID);

#ifdef __cplusplus
}
#endif

#endif

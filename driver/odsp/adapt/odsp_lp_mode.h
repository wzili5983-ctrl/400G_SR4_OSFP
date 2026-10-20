/** @file odsp_lp_mode.h
 * @brief LPMODE 转发：host LPMODE → BCM87420 LPMODE_PHY (PD6)。
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * 同相转发（两端均为低有效，极性一致）：MSL LPMODE 低（host 请求低功耗）
 * → LPMODE_PHY 拉低；MSL LPMODE 高 → LPMODE_PHY 拉高。
 *
 * 本模块仅做引脚电平同步，与 MSM 状态机解耦：模块进/出低功耗由
 * msm_pwr_seq 走 PwrDn/PwrUp 序列完成，此处不影响电源轨时序。
 * msm_pwr_seq.c:131 注释「LPMODE_PHY(PD6) 转发归 MSL 层」即指本接线点。
 */
#ifndef ODSP_LP_MODE_H
#define ODSP_LP_MODE_H

#include "basedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/** LPMODE 转发初始化：按当前 host LPMODE 静态电平落定 LPMODE_PHY 初值。
 *  由 OdspAdaptInit 调用一次（须晚于 MSL 层 MslInit）。 */
VOID OdspLpModeInit(VOID);

/** LPMODE 转发轮询（OdspAdaptTask 周期调用，100ms）：
 *  MslLpReqHwIsActive() 变化时同步到 GPIO_ID_LPMODE_PHY。
 *  同相转发：MSL LPMODE 低（host 请求低功耗） → LPMODE_PHY 拉低；
 *           MSL LPMODE 高 → LPMODE_PHY 拉高。 */
VOID OdspLpModePoll(VOID);

#ifdef __cplusplus
}
#endif

#endif /* ODSP_LP_MODE_H */

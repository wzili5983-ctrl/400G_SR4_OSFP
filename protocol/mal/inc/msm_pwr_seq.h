/** @file msm_pwr_seq.h
 * @brief MSM 电源序列钩子声明（PwrUp/PwrDn 板级动作，主方案 A）。
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * 实现 msm.h MsmPwrSeqHookSet() 的板级钩子：电源轨开关时序（含 2026-10-13
 * 定案① V1P8_EN）、PG 确认、PMIC FLT_RECORD 读清、DSP 复位释放、芯片初始
 * 化（InitChip：DSP→DRV→TIA→PMIC）。定案③：PwrUp 芯片 liveness 失败 →
 * MsmFaultTrigger(MSM_FAULT_CHIP_INIT=38) → MSM FAULT 态执行 PwrDn → 物理
 * 低功耗并停在 Fault（主方案 A，CMIS Table 6-14 ModuleFault|*Low）。
 *
 * 钩子按 MSM 任务周期（10ms/拍）推进，非阻塞——WDG 主循环照常喂狗；
 * 单段超时（500 拍=5s）由 MSM 判 fault 33/34 兜底。
 */
#ifndef MSM_PWR_SEQ_H
#define MSM_PWR_SEQ_H

#include "basedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 注册电源序列钩子（PwrUp/PwrDn）。须在 MsmInit() 之后调用一次
 *  （钩子由 MSM 任务周期驱动），幂等（重复调用仅重置序列步进）。 */
VOID MsmPwrSeqInit(VOID);

#ifdef __cplusplus
}
#endif

#endif /* MSM_PWR_SEQ_H */

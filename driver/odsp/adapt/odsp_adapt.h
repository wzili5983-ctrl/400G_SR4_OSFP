/** @file odsp_adapt.h
 * @brief ODSP (BCM87420 DSP) 适配层顶层入口：子模块初始化 + 100ms 周期任务分发。
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * 统辖 4 个子模块（同目录，头文件 odsp_lp_mode.h / odsp_los_mon.h /
 * odsp_lasi.h / odsp_opt_mon.h）：
 *   - lp_mode ：LPMODE 引脚转发（MSL EXTI6 → LPMODE_PHY GPIO）
 *   - los_mon ：OPRXLOS 周期监视（OPRXLOS_PHY_HIGH → CMIS LOS Flag）
 *   - lasi    ：LASI 中断处理（PE7 EXTI → BCM87420 中断状态分发）
 *   - opt_mon ：光功率监视（BCM87420 内部 ADC 经 BSC 读取，★寄存器地址
 *               待 Broadcom NDA 文档，现占位）
 *
 * 分层约定：BCM87420 上电初始化（OdspInit——SPI 微码/CAPI datapath 配置）
 * 不在本层——由 msm_pwr_seq 的 PWRUP_STEP_CHIP_INIT → InitChip → InitDsp →
 * OdspInit 路径触发；本层仅承担运行期管理职能（GPIO 转发 / 周期轮询 /
 * 中断分发），首次 PwrUp 前各 Poll 须能空转容忍（芯片未上电无响应）。
 *
 * 依赖（不在本文件内，由主控保证）：
 *   - TASK_ID_ODSP_ADAPT 已由主控登记在 driver/mcu/adpat/inc/timer2_scheduler.h
 *     的 TASK_ID_FW_RESET 之后（任务 ID 集中定义，本层不修改该文件）。
 */
#ifndef ODSP_ADAPT_H
#define ODSP_ADAPT_H

#include "basedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ODSP 适配层初始化（由 mgmt_init.c 在 MonInit 之后调用一次）：
 * 1. 初始化 lp_mode / los_mon / lasi / opt_mon 子模块
 * 2. 注册 100ms 周期任务 OdspAdaptTask 到 Timer2（槽位 g_TaskIds[TASK_ID_ODSP_ADAPT]）
 * 注意：BCM87420 上电初始化（OdspInit）不在此处——由 msm_pwr_seq 的
 * PWRUP_STEP_CHIP_INIT → InitChip → InitDsp → OdspInit 路径调用。 */
VOID OdspAdaptInit(VOID);

/* ODSP 周期任务体（Timer2 调度器回调，100ms 一拍）：
 * 依次分发 OdspLpModePoll / OdspLosMonPoll / OdspLasiPoll / OdspOptMonPoll。 */
VOID OdspAdaptTask(VOID *arg);

#ifdef __cplusplus
}
#endif

#endif

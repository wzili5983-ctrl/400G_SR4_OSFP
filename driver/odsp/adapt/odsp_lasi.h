/** @file odsp_lasi.h
 * @brief LASI (PE7) EXTI 双沿中断接线 + 共享 IRQ 分发 + 任务侧处理。
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * BCM87420 (DSP) 通过 LASI 引脚上报中断（FEC 锁定/失锁、LOS、温度告警
 * 等）。本模块只负责 EXTI 接线、ISR 置标志与任务侧的影子置位：
 *
 *   - OdspLasiInit：PE7 → EXTI line 7 双沿触发（下降沿 = 中断置位，
 *     上升沿 = 撤销），由 OdspAdaptInit 调用一次；NVIC 优先级/使能已由
 *     MSL 配置（EXTI4_15_IRQn prio 3），本层不重复；
 *   - OdspLasiIrqHandler：被 msl.c 的 EXTI4_15_IRQHandler 分发调用，
 *     ISR 上下文只置 g_OdspLasiIrqPending 标志速回，严禁 BSC/I2C 事务；
 *   - OdspLasiPoll：OdspAdaptTask 周期调用（100ms），高电平时读 DSP
 *     中断状态（占位 ★，寄存器地址属 Broadcom NDA，占位期间不做影子
 *     置位——避免误报 LOS）。
 *
 * 共享 IRQ 约束：EXTI4_15_IRQn 被 MSL 占用 line 6 (LPMODE)，本模块追加
 * line 7 (LASI)——msl.c 的 EXTI4_15_IRQHandler 需扩展调用
 * OdspLasiIrqHandler()（主控修改，不在本层）。
 */
#ifndef ODSP_LASI_H
#define ODSP_LASI_H

#include "basedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* LASI EXTI 接线（由 OdspAdaptInit 调用一次）：
 * PE7 → EXTI line 7 双沿触发（下降沿=中断置位，上升沿=撤销）；
 * NVIC 优先级/使能已由 MSL 配置（EXTI4_15_IRQn prio 3），本层不重复。
 * 需在 msl.c EXTI4_15_IRQHandler 增加 OdspLasiIrqHandler() 分发（主控修改）。 */
VOID OdspLasiInit(VOID);

/* LASI 任务侧处理（OdspAdaptTask 周期调用，100ms）：
 * 取走挂起标志，高电平时读 DSP 中断状态寄存器（占位 ★）。
 * 寄存器地址属 Broadcom NDA——占位期间不做影子置位，避免任何 DSP 中断
 * 被误报为 LOS。完整版按中断源寄存器按位分发到 LOSFlagRx / LOLFlagRx /
 * FaultFlagRx / 00h:9 温度 Flags。 */
VOID OdspLasiPoll(VOID);

/* LASI EXTI ISR 分支（被 msl.c EXTI4_15_IRQHandler 调用）：
 * 仅置 g_OdspLasiIrqPending = 1，速回。ISR 上下文严禁 BSC 读（I2C 事务）。 */
VOID OdspLasiIrqHandler(VOID);

#ifdef __cplusplus
}
#endif

#endif

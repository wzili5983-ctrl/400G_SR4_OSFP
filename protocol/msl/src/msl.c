/***************************************************************************//**
 *   @file   msl.c
 *   @brief  MSL 模块信号层（Module Signal Layer）实现。
 *   @author Firmware team
 *******************************************************************************
 * Copyright 2026 Terabitcom, Inc.
 *
 * 语义与设计约束见 msl.h 文件头。要点：
 *   - M_INT 是 MSA INT/RSTn 管脚的功能选择线（不是 INT 驱动器，也
 *     不与 MCU NRST 直连本体）：低 = INT/RSTn 直连 NRST（host 控复位，
 *     Reset 由硬件触发）、高 = INT/RSTn 为模块中断输出；
 *   - LPMODE 下降沿 → EXTI line6（与 EXTI4-15 共享 IRQ，优先级 3）→
 *     ISR 仅置 g_MslLpReqHw 标志，主循环 MslLpReqHwConsume() 消费；
 *   - 本层不做引脚初始化（InitBoardResources 已完成），只接 EXTI/NVIC
 *     与引脚电平的显式落定。
 */
#include "msl.h"

#include "gpioresource.h"
#include "gpioresource_impl.h"
#include "odsp_lasi.h"         /* OdspLasiIrqHandler：EXTI4_15 共享 IRQ 分发 */

#include "gd32e511_512.h"
#include "gd32e511_512_exti.h"
#include "gd32e511_512_syscfg.h"

/** LPMODE(PC6) 映射的 EXTI 线号：PC6 → EXTI line 6。 */
#define MSL_LPMODE_EXTI_LINE        EXTI_6

/** EXTI4_15 共享中断的 NVIC 优先级（与 NVIC 规划一致：3）。 */
#define MSL_LPMODE_IRQ_PRIO         3U

volatile BYTE g_MslLpReqHw = 0U;

/** 当前 INT/RSTn 功能模式影子（MSL_INTRSTN_MODE_*），与 M_INT 实际
 *  电平一致：单写者 MslIntRstnModeSet，避免为读状态多走一次 GPIO 读。 */
STATIC BYTE s_IntRstnMode = MSL_INTRSTN_MODE_RESET;

VOID MslInit(VOID)
{
    /* 落定 M_INT 初始模式 = 复位直连（低，与板级默认电平一致）：
     * INT/RSTn 管脚直连 MCU NRST，host 可持模块复位；固件就绪后可
     * 经 MslIntRstnModeSet(MSL_INTRSTN_MODE_INT) 切到中断输出模式。
     * M_INT 低不复位 MCU（拓扑见 msl.h），此处显式驱动保证确定态。 */
    (void)WriteGpio(GPIO_ID_M_INT, IO_LOW);
    s_IntRstnMode = MSL_INTRSTN_MODE_RESET;

    /* LPMODE(PC6) → EXTI line 6，下降沿触发（LowPwrRequestHW 低有效）。
     * GD32E511 的 SYSCFG 无独立时钟门控，无需 rcu 使能。 */
    syscfg_exti_line_config(EXTI_SOURCE_GPIOC, EXTI_SOURCE_PIN6);
    exti_init(MSL_LPMODE_EXTI_LINE, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(MSL_LPMODE_EXTI_LINE);
    exti_interrupt_enable(MSL_LPMODE_EXTI_LINE);

    /* EXTI4-15 共享 IRQ；优先级 3（ISR 只置标志，短可抢占主循环）。 */
    NVIC_SetPriority(EXTI4_15_IRQn, MSL_LPMODE_IRQ_PRIO);
    NVIC_EnableIRQ(EXTI4_15_IRQn);
}

VOID MslIntRstnModeSet(BYTE mode)
{
    if(mode == MSL_INTRSTN_MODE_INT) {
        /* 释放 OD 拉高：INT/RSTn 管脚切换为模块 Interrupt 输出 */
        (void)WriteGpio(GPIO_ID_M_INT, IO_HIGH);
        s_IntRstnMode = MSL_INTRSTN_MODE_INT;
    } else if(mode == MSL_INTRSTN_MODE_RESET) {
        /* 拉低：INT/RSTn 管脚直连 MCU NRST，复位通道交还 host */
        (void)WriteGpio(GPIO_ID_M_INT, IO_LOW);
        s_IntRstnMode = MSL_INTRSTN_MODE_RESET;
    } else {
        /* 非法值忽略 */
    }
}

BYTE MslIntRstnModeGet(VOID)
{
    return s_IntRstnMode;
}

BYTE MslLpReqHwConsume(VOID)
{
    if(g_MslLpReqHw != 0U) {
        g_MslLpReqHw = 0U;
        return 1U;
    }
    return 0U;
}

BYTE MslLpReqHwIsActive(VOID)
{
    DWORD level = IO_HIGH;

    if(ReadGpio(GPIO_ID_LPMODE, &level) != OK) {
        return 0U;    /* 读失败按未请求处理（LPMODE 只读引脚不应失败） */
    }
    return (level == IO_LOW) ? 1U : 0U;    /* 低有效 */
}

/*! EXTI line 4-15 共享中断：LPMODE(PC6, line 6) 下降沿置标志回线程；
 *  LASI(PE7, line 7) 双沿置标志回线程（ODSP 适配层消费）。
 *  两线共享 IRQ，ISR 只置标志不做事（BSC I2C 事务严禁进 ISR）。 */
void EXTI4_15_IRQHandler(void)
{
    if(exti_interrupt_flag_get(MSL_LPMODE_EXTI_LINE) != RESET) {
        exti_interrupt_flag_clear(MSL_LPMODE_EXTI_LINE);
        g_MslLpReqHw = 1U;
    }
    OdspLasiIrqHandler();
}

/** @file odsp_lp_mode.c
 * @brief LPMODE 转发实现：MslLpReqHwIsActive() → WriteGpio(GPIO_ID_LPMODE_PHY)。
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * 备注（与 msm_pwr_seq.c:131 的接线约定对应）：
 *  - LPMODE_PHY 低有效（active_level=LOW），与 host LPMODE 极性一致
 *    → 同相转发，无需反相。
 *  - 转发时机与 MSM 状态机解耦，仅引脚电平同步；模块进低功耗由
 *    msm_pwr_seq 走 PwrDn 序列完成，本模块不触碰电源轨。
 *  - MslLpReqHwIsActive() 返回非 0 = host LPMODE 现为低（低有效，正在
 *    请求低功耗）；静态电平查询，可在 OdspAdaptTask 线程上下文安全调用。
 *  - 窄脉冲覆盖：100ms 轮询会漏采 <100ms 的低脉冲，叠加 MslLpReqHwConsume()
 *    边沿标志——EXTI 捕获到的下降沿在本拍视为有效请求，下一拍若 host 已
 *    释放（静态电平回高）自然回高。
 */
#include "odsp_lp_mode.h"
#include "gpioresource.h"   /* WriteGpio / GPIO_ID_LPMODE_PHY；IO_LOW/IO_HIGH 见 basedef.h */
#include "msl.h"            /* MslLpReqHwIsActive / MslLpReqHwConsume */

/* 上次已同步的 host LPMODE 状态；0xFF = 未知，首拍强制写初值。 */
STATIC BYTE s_LpReqLast = 0xFFU;

VOID OdspLpModeInit(VOID)
{
    /* 与 OdspLpModePoll 同逻辑，但无条件落定初值：上电时 host 可能已
     * 拉低 LPMODE（无线沿可触发 EXTI），静态电平查询兜底。 */
    BYTE cur = MslLpReqHwIsActive();
    s_LpReqLast = cur;
    (void)WriteGpio(GPIO_ID_LPMODE_PHY, cur ? IO_LOW : IO_HIGH);
}

VOID OdspLpModePoll(VOID)
{
    /* 电平 + 边沿混合判定：静态电平为主，EXTI 捕获的下降沿兜底窄脉冲
     * （<100ms 的低脉冲静态电平查询会漏采，但 MSL EXTI6 已置标志）。
     * MslLpReqHwConsume() 消费后清零，下一拍若静态电平已恢复则自然回高。 */
    BYTE cur = (MslLpReqHwConsume() != 0U || MslLpReqHwIsActive() != 0U) ? 1U : 0U;
    if(cur == s_LpReqLast) {
        return;
    }
    s_LpReqLast = cur;
    /* 同相转发：cur 非 0（host LPMODE 低，请求低功耗）→ LPMODE_PHY 拉低。 */
    (void)WriteGpio(GPIO_ID_LPMODE_PHY, cur ? IO_LOW : IO_HIGH);
}

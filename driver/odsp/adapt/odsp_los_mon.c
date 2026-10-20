/** @file odsp_los_mon.c
 * @brief OPRXLOS (PD8) 周期采样 + 11h:147 LOSFlagRx COR 置位实现。
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * COR 语义：本模块只置位，清零由 host 读触发 RAL 引擎 DtCorClear
 * （与 monitor.c MonFlagUpdate 一致）。4 lane 共享：SR4 模块 DSP 只输出
 * 聚合后的 OPRXLOS，4 路 media lane 同步置位 LOSFlagRx bit0-3，故一个
 * 沿事件直接写 0x0F 而非按位对应具体 lane。
 */
#include "odsp_los_mon.h"
#include "gpioresource.h"           /* ReadGpio / GPIO_ID_OPRXLOS_PHY_HIGH / IO_HIGH */
#include "gpioresource_impl.h"      /* GPIO_ID_* */
#include "adpat.h"                  /* Read/WriteSramProtocolMsaSpace / AdpatShadowLock/Unlock */
#include "ral.h"                    /* PAGE_ID_11H_0 */

/* 11h:147 是页内偏移 147-128 = 19 = 0x13 */
#define ODSP_LOSMON_LANE_FLAG_PAGE      PAGE_ID_11H_0
#define ODSP_LOSMON_LANE_FLAG_OFFSET    0x13U   /* 11h:147 LOSFlagRx8..1 */
#define ODSP_LOSMON_LANE_FLAG_MASK      0x0FU   /* SR4 lane1-4 */

/* 上次电平缓存：0xFF=未知，首拍强制与本次采样比较（惰性初始化）。
 * 模块可变状态只此一处——OdspLosMonInit 保持空实现即可。 */
STATIC BYTE s_LosLast = 0xFFU;

VOID OdspLosMonInit(VOID)
{
    /* 空实现：所有状态在首次 Poll 时惰性初始化（s_LosLast=0xFF 哨兵）。
     * GPIO 方向/初始化归 gpioresource_impl.c 表驱动，本层不重复 InitGpio。 */
}

VOID OdspLosMonPoll(VOID)
{
    DWORD level;
    BYTE los;
    BYTE flag;

    if(ReadGpio(GPIO_ID_OPRXLOS_PHY_HIGH, &level) != OK) {
        return;                         /* GPIO 未就绪/读失败：本拍跳过，下拍再试 */
    }
    los = (level == IO_HIGH) ? 1U : 0U;
    if(los == s_LosLast) {
        return;                         /* 无沿：影子不必写 */
    }
    s_LosLast = los;
    if(los == 0U) {
        return;                         /* 下降沿：COR 只置不清 */
    }
    /* 上升沿：影子 RMW，锁内防 I2C1 ISR 并发丢更新 */
    AdpatShadowLock();
    if(ReadSramProtocolMsaSpace(ODSP_LOSMON_LANE_FLAG_PAGE, ODSP_LOSMON_LANE_FLAG_OFFSET, &flag, 1U) == OK) {
        flag |= ODSP_LOSMON_LANE_FLAG_MASK;
        (void)WriteSramProtocolMsaSpace(ODSP_LOSMON_LANE_FLAG_PAGE, ODSP_LOSMON_LANE_FLAG_OFFSET, &flag, 1U);
    }
    AdpatShadowUnlock();
}

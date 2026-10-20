/** @file odsp_adapt.c
 * @brief ODSP 适配层顶层实现：子模块初始化编排 + 100ms 周期任务分发。
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 */
#include "odsp_adapt.h"

#include "odsp_init.h"          /* OdspInitIsReady：电源状态守卫 */
#include "odsp_lp_mode.h"       /* OdspLpModeInit / OdspLpModePoll：LPMODE 转发 */
#include "odsp_los_mon.h"       /* OdspLosMonInit / OdspLosMonPoll：OPRXLOS 监视 */
#include "odsp_lasi.h"          /* OdspLasiInit / OdspLasiPoll：LASI 中断分发 */
#include "odsp_opt_mon.h"       /* OdspOptMonInit / OdspOptMonPoll：光功率监视（★占位） */
#include "odsp_telemetry.h"     /* OdspTelemetryInit/Update/Sleep：1s 遥测采集（VDM 上游） */
#include "cdb.h"                /* CdbDspUpgradeActive：U15 升级期间禁止 DSP 访问 */
#include "timer2_scheduler.h"   /* g_TaskIds / TASK_ID_ODSP_ADAPT / Timer2SchedulerAdd */

/* 调度时序：首次 200ms——MgmtInit 刚完成、首轮影子写入窗口已结束，避开
 * bring-up 期与 I2C1 host 访问的争用；周期 100ms——LPMODE/LOS 的响应
 * 延迟与轮询负载的折中（性能采样已由 monitor 的 300ms 任务承担，
 * 本任务只做轻量 GPIO/标志分发）。 */
#define ODSP_ADAPT_TASK_DELAY_TICKS     200U
#define ODSP_ADAPT_TASK_INTERVAL_TICKS  100U

VOID OdspAdaptInit(VOID)
{
    /* 子模块初始化按依赖序：lp_mode / los_mon 仅 GPIO + 影子写入，先立；
     * lasi 依赖 MSL 已布线的 LASI 引脚（EXTI 配置挂在中断控制器上）；
     * opt_mon 为占位★，最后。任务体 OdspAdaptTask 内做电源状态守卫
     * （OdspInitIsReady），首次 PwrUp 前除 lp_mode 外各 Poll 空转。 */
    OdspLpModeInit();
    OdspLosMonInit();
    OdspLasiInit();
    OdspOptMonInit();
    OdspTelemetryInit();

    /* 槽位存调度器集中表 g_TaskIds[TASK_ID_ODSP_ADAPT]（timer2_scheduler.h）。
     * TASK_ID_ODSP_ADAPT 由主控登记于 TASK_ID_FW_RESET 之后，本层不修改
     * 该集中表；句柄 -1 失败时调度器任务不启用，不影响其它任务。 */
    g_TaskIds[TASK_ID_ODSP_ADAPT] = Timer2SchedulerAdd(
        OdspAdaptTask, NULL,
        ODSP_ADAPT_TASK_DELAY_TICKS, ODSP_ADAPT_TASK_INTERVAL_TICKS,
        TIMER2_TASK_FOREVER);
}

VOID OdspAdaptTask(VOID *arg)
{
    (void)arg;

    /* U15 升级期间 DSP 保持复位，且 SPI2 归 MCU 擦写会话使用；除 LPMODE
     * 引脚转发外暂停全部 DSP 轮询。该状态由 CDB 保持到 MCU 复位。 */
    if(CdbDspUpgradeActive() != 0U) {
        OdspLpModePoll();
        OdspTelemetrySleep();
        return;
    }

    /* 电源状态守卫：BCM87420 未上电（OdspInit 未成功）时，OPRXLOS/LASI
     * 引脚悬空（GPIO_PUPD_NONE）、OptMon 的 BSC 读必然 NACK——3 个 Poll
     * 全部空转；telemetry 同步挂起（解除 SNR/LTP 轮询使能，下次 PwrUp
     * 自动重使能）。lp_mode 与电源状态无关（纯引脚转发），不受守卫限制。 */
    if(OdspInitIsReady() == 0U) {
        OdspLpModePoll();
        OdspTelemetrySleep();
        return;
    }

    /* 依次分发各子模块周期体。各 Poll 内部自行做影子锁（AdpatShadowLock）
     * / 防抖 / 未上电空转守卫，此处仅作编排、不叠加业务逻辑。
     * telemetry 排最前：DSP I2C 事务较重，放前面避免被前面 Poll 的
     * 早退路径挤压（当前各 Poll 均无早退，顺序仅为约定）。 */
    OdspTelemetryUpdate();
    OdspLpModePoll();
    OdspLosMonPoll();
//    OdspLasiPoll();
//    OdspOptMonPoll();
}

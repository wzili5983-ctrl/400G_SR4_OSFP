/** @file mgmt_init.h
 * @brief MgmtInit（管理接口初始化，MSM 复位域内阶段）声明。
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * CMIS 5.3 §6.3.2 MSM 的复位域细分：Resetting（MCU 硬件重启）→ Reset →
 * MgmtInit。本模块实现 **MgmtInit 阶段**的编排（management side 上电，
 * tMgmtInit ≤ 2000ms），把「管理侧」各子系统按依赖顺序拉起；**高功率
 * 数据通路（电源轨 PA8/PA9/PA10/PA11/PB11、retimer/GN1848/TIA 初始化）
 * 不在本阶段**——它们由 MSM 进入 ModulePwrUp 时经
 * MsmPwrSeqHookSet() 注册的板级钩子下发（见 msm.h）。
 *
 * 本阶段只初始化「可管理的低功耗域」：
 *   ① MCI 就绪（I2C1 从机 0xA0，host 通道）；
 *   ② RAL 页表上下文初始化 + management map 影子加载（Flash 主参数区
 *      → SRAM [7] 镜像）；
 *   ③ SRAM 常驻控制位/掩码的复位默认值落定（ModuleControl / 掩码，
 *      这些寄存器 MEMT_SRAM、掉电即失，须每次 boot 重立默认）；
 *   ④ ModuleState 播报（MSM 初始化即发布 Resetting）+ 复位源识别；
 *   ⑤ DPSM 创建（默认 DP 表）；
 *   ⑥ 监视任务启动；
 *   ⑦ 致命故障自检；
 *   ⑧ 通报 MSM「管理接口就绪」MsmMgmtInitDone()——MSM 由此转
 *      ModuleLowPwr，且 MSM 内部以 2000ms 超时看门狗兜底（超时进
 *      ModuleFault cause=32，见 msm.c）。
 *
 * 调用前置（boot 顺序约定，见 main.c 接入时须满足）：
 *   SystemCoreClockUpdate() → InitBoardResources()（GPIO/ADC/总线）
 *   → Timer2SchedulerInit()（本模块注册的 Msm/Dpsm/Mon 任务依赖调度器）
 *   → MgiInit() → 主循环（Timer2SchedulerRun / DtDeferredPoll /
 *   CmdExeBackground）。
 *
 * 复位源识别结果固件内部使用（无标准 CMIS 寄存器承载，供诊断/上位机
 * 查询），见 MgiResetCause_t / MgiResetCauseGet()。
 */
#ifndef MGMT_INIT_H
#define MGMT_INIT_H

#include "basedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 复位源编码（MgmtInit 期间识别并记录，一次 boot 只识别一次）。 */
typedef enum {
    MGI_RESET_NONE     = 0U,   /* 未识别（MgiInit 尚未执行） */
    MGI_RESET_POWER    = 1U,   /* PORRSTF：上电复位 */
    MGI_RESET_PIN      = 2U,   /* EPRSTF：外部引脚复位（INT/RSTn 或 NRST） */
    MGI_RESET_WATCHDOG = 3U,   /* FWDGTRSTF/WWDGTRSTF：看门狗复位 */
    MGI_RESET_SOFTWARE = 4U,   /* SWRSTF：软件复位（00h:26.3 / NVIC_SystemReset） */
    MGI_RESET_LOWPOWER = 5U,   /* LPRSTF：低功耗复位 */
    MGI_RESET_OTHER    = 6U    /* 其它/未知（OBL/V12/保留标志） */
} MgiResetCause_t;

/** MgmtInit 阶段入口：按上述清单编排管理侧 bring-up。
 *  返回 OK = 管理接口就绪并已向 MSM 通报；ERR = 致命故障自检未通过
 *  （已走 MsmFaultTrigger，模块将锁存 ModuleFault，不进入管理就绪）。
 *  本函数一次性调用（boot 阶段）。 */
DWORD MgiInit(VOID);

/** 查询本次 boot 的复位源（MGI_RESET_*，未初始化返回 MGI_RESET_NONE）。 */
MgiResetCause_t MgiResetCauseGet(VOID);

#ifdef __cplusplus
}
#endif

#endif /* MGMT_INIT_H */

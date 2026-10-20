/** @file msm.h
 * @brief MSM（Module State Machine，模块状态机）声明。
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * CMIS 5.3 §6.3.2 MSM 实现：每模块一个实例，管理复位域 → 低功耗 ⇄ 高功耗
 * 全功能 → 故障锁存的生命周期，并通过 ModuleState（Lower 00h:3）与
 * ModuleStateChangedFlag（00h:8 bit0）向 host 播报。
 *
 * 范围（AppSel 1 方案，与 dpsm.h 配对）：
 *  - paged 模块、SteppedConfigOnly=1（Lower 00h:2 宣告一致）；
 *  - 上电默认应用 = AppSel 1（400G SR4，单 Data Path，lane1-4），
 *    由 dpsm.c 的默认 DP 表承载；
 *  - ResetS 来源中的 ResetL 引脚复位由硬件触发 MCU 重启（天然回到
 *    本机的 MgmtInit 入口），SoftwareReset 经 00h:26.3 已接
 *    NVIC_SystemReset（ral.c OsfpOperateModuleControl），故本机
 *    只显式建模 FaultS / LowPwrS / LowPwrExS 三条信号 + MgmtInit 超时；
 *  - 电源轨动作（1.8V_EN / 3.3V_PHY_EN / CDR_LD_EN 等）经
 *    MsmPwrSeqHookSet() 注册的板级钩子下发，本层不直接摸 GPIO；
 *  - LowPwrExS 依赖「所有 DP 均 DPDeactivated」→ 调 DpsmAllDeactivated()
 *    与 dpsm.c 握手（msm.c 只依赖 dpsm.h，不反向耦合 monitor）。
 *
 * 信号真值表（CMIS Table 6-11/6-12，电平语义，周期采样）：
 *  LowPwrS = LowPwrRequestSW(00h:26.4)
 *            OR ( LowPwrAllowRequestHW(00h:26.6,默认 1)
 *                 AND LowPwrRequestHW(LPMode 引脚低电平, MSL 静态电平) )
 *  LowPwrExS = LowPwrS AND ModuleDeactivatedT(所有 host lane DPState=DPDeactivated)
 *  上电默认 LowPwrRequestSW=0 / LowPwrAllowRequestHW=1 → LPMode 引脚电平决定
 *  是否停留在 ModuleLowPwr。
 *
 * 时序约束：tMgmtInit ≤ 2000ms（boot 调 MsmMgmtInitDone 前不得超时，
 * 超时进 ModuleFault，cause=32 厂商自定义）；MaxDurationModulePwrUp/PwrDn
 * 由钩子返回节拍控制，超时同样按故障处理。
 */
#ifndef MSM_H
#define MSM_H

#include "basedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/** MSM 状态（编码 = CMIS Lower 00h:3 bit3-1 的 ModuleState 码）。 */
typedef enum {
    MSM_STATE_RESET      = 0U,   /* 复位域（Resetting/Reset/MgmtInit 对外统一 000） */
    MSM_STATE_LOW_PWR    = 1U,   /* ModuleLowPwr：低功耗、可管理 */
    MSM_STATE_PWR_UP     = 2U,   /* ModulePwrUp（瞬态）：正在切高功率 */
    MSM_STATE_READY      = 3U,   /* ModuleReady：全功能 */
    MSM_STATE_PWR_DN     = 4U,   /* ModulePwrDn（瞬态）：正在退回低功耗 */
    MSM_STATE_FAULT      = 5U    /* ModuleFault：故障锁存，等 host 动作 */
} MsmState_t;

/** ModuleFaultCause 编码（Lower 00h:29 = Byte 41，CMIS 5.3 Table 8-13）。
 *  0-31 为 CMIS 标准码，32-63 为厂商自定义。本工程当前只用 2（数据存储
 *  损坏）与 32-38（各序列超时 + 芯片初始化失败）。MsmFaultTrigger() 入参
 *  即此枚举。 */
typedef enum {
    MSM_FAULT_NONE             = 0U,   /* 无故障（保留，勿作触发值） */
    MSM_FAULT_TEC_RUNAWAY      = 1U,   /* TEC 失控 */
    MSM_FAULT_DATA_MEM         = 2U,   /* 数据存储损坏 */
    MSM_FAULT_PROGRAM_MEM      = 3U,   /* 程序存储损坏 */
    MSM_FAULT_TX               = 4U,   /* Tx 故障 */
    MSM_FAULT_RX               = 5U,   /* Rx 故障 */
    MSM_FAULT_TEMP             = 6U,   /* 温度相关 */
    /* 7-31 = CMIS 保留 */

    MSM_FAULT_MGMTINIT_TIMEOUT = 32U,  /* 自定义：tMgmtInit 超时 */
    MSM_FAULT_PWRUP_TIMEOUT    = 33U,  /* 自定义：ModulePwrUp 序列超时 */
    MSM_FAULT_PWRDN_TIMEOUT    = 34U,  /* 自定义：ModulePwrDn 序列超时 */
    MSM_FAULT_DPINIT_TIMEOUT   = 35U,  /* 自定义：DPInit commissioning 超时 */
    MSM_FAULT_TXSW_TIMEOUT     = 36U,  /* 自定义：Tx 开关超时 */
    MSM_FAULT_DPDEINIT_TIMEOUT = 37U,  /* 自定义：DPDeinit 超时 */
    MSM_FAULT_CHIP_INIT        = 38U   /* 自定义：PwrUp 芯片初始化/liveness 失败（电源已上但芯片不应答） */
} MsmFaultCause_t;

/** 电源序列钩子：返回 OK 表示本段序列完成；非 OK 则下拍重试，
 *  超过 MSM_PWR_SEQ_TIMEOUT_TICKS 按故障处理。注册 NULL = 立即完成
 *  （影子/调试构建用）。PwrUp 钩子里完成：3.3V_PHY/1.8V/CDR 上电 +
 *  PG 确认 + retimer/GN1848 初始化；PwrDn 反之。 */
typedef DWORD (*MsmPwrSeqHook_t)(VOID);

/** MSM 初始化：状态进 MgmtInit（复位域）、注册 Timer2 周期任务
 *  （10ms 一拍）。须在 Timer2SchedulerInit() 之后、影子区可访问后调用；
 *  boot 完成管理接口初始化（I2C 从机就绪、影子加载完）后须立即调
 *  MsmMgmtInitDone()（合计不得超过 tMgmtInit=2000ms）。 */
VOID MsmInit(VOID);

/** boot 侧通报「管理接口初始化完成」：MgmtInit → ModuleLowPwr，
 *  置 ModuleStateChangedFlag。只允许调用一次，重复调用忽略。 */
VOID MsmMgmtInitDone(VOID);

/** MSM 任务体（调度器周期回调；也可脱离调度器直接周期调用）：
 *  采样 LowPwrS/LowPwrExS/FaultS，按状态图迁移，发布 ModuleState 与
 *  ModuleStateChangedFlag。详见 msm.c 状态图注释。 */
VOID MsmTask(VOID *arg);

/** FaultS 入口（模块自监督：TEC 失控/存储损坏/Tx/Rx fault/温度/各序列
 *  超时等）：任意状态可触发，进 ModuleFault 并写 00h:29。cause 取
 *  MsmFaultCause_t 枚举，MSM_FAULT_NONE(0) 保留勿用（会被归到自定义）。 */
VOID MsmFaultTrigger(MsmFaultCause_t cause);

/** 注册电源序列钩子（NULL 清除，恢复"立即完成"）。 */
VOID MsmPwrSeqHookSet(MsmPwrSeqHook_t up, MsmPwrSeqHook_t dn);

/** 当前 MSM 状态查询。 */
MsmState_t MsmStateGet(VOID);

/** ModuleReadyT：供 dpsm.c 求 DPDeinitS（NOT ModuleReadyT 项）。 */
BYTE MsmModuleReadyT(VOID);

/** LowPwrS 当前电平：供 dpsm.c 求 DPDeinitS；也供调试查询。 */
BYTE MsmLowPwrS(VOID);

#ifdef __cplusplus
}
#endif

#endif /* MSM_H */

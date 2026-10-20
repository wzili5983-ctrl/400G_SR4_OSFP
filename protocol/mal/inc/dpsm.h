/** @file dpsm.h
 * @brief DPSM（Data Path State Machine，数据通路状态机）声明。
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * CMIS 5.3 §6.3.3 DPSM 实现：每个 Data Path 一个实例，管理
 * DPDeactivated → DPInit → DPInitialized → DPTxTurnOn → DPActivated
 * （及反向 DPTxTurnOff / DPDeinit）的通路生命周期，上报
 * DPState（Page 11h:128-131，每 lane 4bit）与
 * DPStateChangedFlag（11h:134）。
 *
 * 范围（AppSel 1 方案，与 msm.h 配对）：
 *  - 默认 DP 表 = 单 DP：DataPathID=0，host lane1-4（mask 0x0F）↔
 *    media lane1-4；即 AppSel 1 = 400G SR4 独占模块；
 *  - SteppedConfigOnly=1：ApplyImmediate 不支持（RAL 引擎按 00h:2
 *    宣告丢弃），本机只响应 ApplyDPInit（置 DPInitPending 通道）；
 *  - Apply 触发的 SCS→ACS 拷贝归 RAL 引擎（Cmd 层），引擎拷贝完成后
 *    调 DpsmApplyDpInit(lane_mask) 置 ConfigStatus/DPInitPending 并
 *    允许 DPInit——本层不做 SCS 内容校验以外的配置语义；
 *  - commissioning 硬件动作（retimer 配置 / Tx 使能）经
 *    DpsmHookSet() 注册，NULL = 立即完成（影子/调试构建）；
 *  - lane5-8 悬空：不参与任何 DP，DPState 固定报 DPDeactivated(1h)。
 *
 * 影子地址（bank0 页内偏移；host 地址 = +0x80）：
 *   10h:128→0x00 DPDeinitLane1-8（静态请求,同 DP 同值）
 *   10h:130→0x02 OutputDisableTx；10h:132→0x04 OutputSquelchForceTx
 *   10h:235→0x6B DPInitPendingLane1-8（10h 侧）
 *   11h:128-131→0x00-0x03 DPState（每 lane 4bit,Table 8-84 编码）
 *   11h:134→0x0A DPStateChangedFlag
 *   11h:202-203→0x4A-0x4B ConfigStatusLane1-4（每 lane 4bit）
 *   11h:235→0x71 DPInitPendingLane1-8（11h 只读侧与 10h 镜像）
 *
 * DPState 编码：1h=DPDeactivated 2h=DPInit 3h=DPDeinit 4h=DPActivated
 *              5h=DPTxTurnOn 6h=DPTxTurnOff 7h=DPInitialized
 * ConfigStatus 编码：1h=ConfigSuccess 3h=ConfigRejectedInvalidAppSel
 *                    7h=ConfigRejectedPartialDataPath Ch=ConfigInProgress
 *                    （Table 8-91；4h/6h 待 DP 组合/breakout 校验启用）
 */
#ifndef DPSM_H
#define DPSM_H

#include "basedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/** DPSM 状态（内部枚举；对外发布用 DPState 编码，见 dpsm.c 映射表）。 */
typedef enum {
    DPSM_STATE_DEACTIVATED = 0U,   /* 稳：DP 未建立 */
    DPSM_STATE_INIT,               /* 瞬：commissioning 执行中 */
    DPSM_STATE_INITIALIZED,        /* 稳：待命，Tx 输出受 disable/squelch 位 */
    DPSM_STATE_TX_TURN_ON,         /* 瞬：输出抬起中 */
    DPSM_STATE_ACTIVATED,          /* 稳：业务通路打开 */
    DPSM_STATE_TX_TURN_OFF,        /* 瞬：输出关回中 */
    DPSM_STATE_DEINIT              /* 瞬：拆除中 */
} DpsmState_t;

/** 通路硬件钩子：DPInit 里的 commissioning 与 Tx 开关动作。
 *  hook_dp_init：进 DPInit 时调用一次（配 retimer/GN1848 按 ACS）；
 *  hook_tx_on  / hook_tx_off：进 DPTxTurnOn / DPTxTurnOff 时调用一次。
 *  返回 OK = 动作完成（瞬态一拍走完）；非 OK 下拍重试，超过
 *  DPSM_*_TIMEOUT_TICKS 按故障走 MsmFaultTrigger(35/36)。 */
typedef DWORD (*DpsmHook_t)(VOID);

/** DPSM 初始化：建立默认 DP 表（AppSel 1 单 DP）、注册 Timer2 周期
 *  任务（10ms 一拍）。须在影子区可访问、Timer2SchedulerInit() 之后、
 *  MSM 初始化之前调用（msm 任务会读 DpsmAllDeactivated()）。 */
VOID DpsmInit(VOID);

/** DPSM 任务体（调度器周期回调；也可脱离调度器直接周期调用）：
 *  求值 DPDeinitS/DPDeactivateS/DPInitPending，驱动各 DP 状态迁移，
 *  发布 DPState / DPStateChangedFlag / DPInitPending 清除。 */
VOID DpsmTask(VOID *arg);

/** ApplyDPInit 入口（RAL 引擎在 SCS→ACS 拷贝完成后调用）：
 *  lane_mask = 本次触发写 1 的 host lane 位图（bit0=lane1）。
 *  行为（CMIS 过程命令协议）：
 *  - 任一所触 lane 的 DP 处于瞬态（DPInit/DPDeinit/TxTurnOn/Off）或
 *    ConfigInProgress → 整个触发静默忽略（返回 OK，无副作用）；
 *  - mask 未完整覆盖某个 DP（partial data path）→ 该 DP 全 lane 的
 *    ConfigStatus 写 ConfigRejectedPartialDataPath(7h)；
 *  - 触发 lane 的 SCS0::DPConfigLane AppSelCode 非 App1 → 该 DP 全 lane
 *    写 ConfigRejectedInvalidAppSel(3h)（单 DP 只运行 AppSel 1）；
 *  - 完整覆盖且 AppSel 合法 → 该 DP 所有 lane 写 ConfigSuccess(1h)，置位
 *    DPInitPending（10h:235 与 11h:235 同步），DPSM 在
 *    DPDeactivated 且 DPDeinitS=FALSE 时自动进 DPInit。
 *  SteppedConfigOnly=1：ApplyImmediate 由 RAL 引擎直接丢弃，不进本层。 */
DWORD DpsmApplyDpInit(BYTE lane_mask);

/** 注册通路硬件钩子（NULL 清除，恢复"立即完成"）。 */
VOID DpsmHookSet(DpsmHook_t hook_dp_init, DpsmHook_t hook_tx_on, DpsmHook_t hook_tx_off);

/** ModuleDeactivatedT：所有 DP 均 DPDeactivated——msm.c 求 LowPwrExS
 *  用。返回 1 = 允许 ModuleReady → ModulePwrDn。 */
BYTE DpsmAllDeactivated(VOID);

/** 指定 DP 当前状态查询（dpid = 表内下标；越界返回 DEACTIVATED）。 */
DpsmState_t DpsmStateGet(BYTE dpid);

#ifdef __cplusplus
}
#endif

#endif /* DPSM_H */

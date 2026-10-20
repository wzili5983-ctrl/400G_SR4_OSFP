/***************************************************************************//**
 *   @file   dpsm.c
 *   @brief  DPSM（Data Path State Machine）实现：CMIS 5.3 §6.3.3。
 *   @author Firmware team
 *******************************************************************************
 * Copyright 2026 Terabitcom, Inc.
 *
 * 状态图（每 DP 一个实例；AppSel 1 = 单 DP：host lane1-4 ↔ media lane1-4）：
 *
 *              DPInitPending=1 且 DPDeinitS=FALSE
 *   DEACTIVATED ───────────────────────────────► INIT ──完成──► INITIALIZED
 *        ▲                                          │DPDeinitS    │!DPDeactivateS
 *        │                                          ▼            ▼
 *        │                                        DEINIT    TX_TURN_ON ─完成─► ACTIVATED
 *        │                                          ▲            ▲──────────────│
 *        └────────── 完成 ◄── DEINIT ◄── DPDeinitS ──┴────────────┴── TX_TURN_OFF ◄──
 *                                              （任一稳态可被 DPDeinitS 拉回；
 *                        ACTIVATED/TX_TURN_ON 可被 DPDeactivateS 拉回 INITIALIZED 链）
 *
 * 信号（CMIS Table 6-15/6-16，电平语义，10ms 周期求值）：
 *   DPDeinitS   = (NOT ModuleReadyT) OR LowPwrS OR DPDeinitT
 *                 DPDeinitT = 10h:128 该 DP 所有 lane 均写 1（静态请求）
 *   DPDeactivateS = DPDeinitS OR DPTxDisableT OR DPTxForceSquelchT
 *                 = DPDeinitS OR (OutputDisableTx|OutputSquelchForceTx)(10h:130/132)
 *                   该 DP 任一进 1 → 整 DP 离开 ACTIVATED（规范：部分 lane
 *                   关断会破坏 AM 组帧，整 DP 动作）
 *   SteppedConfigOnly=1：无 DPReDeinitS/DPReinitT 旁路，重配只能
 *   host 先 DPDeinit 再靠 DPInitPending 回 INIT。
 *
 * 规范要点落点：
 *  - 进 INIT 清除该 DP 全部 lane 的 DPInitPending（10h:235 + 11h:235）；
 *  - INITIALIZED 且 !DPDeactivateS → 自动 TX_TURN_ON（默认 DPDeinit=0 的
 *    "进 Ready 自动激活"语义）；
 *  - DPStateChangedFlag 只在进稳态（INITIALIZED/DEACTIVATED/ACTIVATED）
 *    时置位，瞬态抑制；
 *  - Apply 触发在瞬态被静默忽略（ConfigInProgress 语义）；
 *  - Apply 校验 AppSelCode==App1（单 DP 只运行 AppSel 1），不支持的应用
 *    （App2/App3 已宣告未实现）拒 ConfigRejectedInvalidAppSel(3h)；
 *  - DPSM 生命周期：DpsmInit 建立（对应 MgmtInit 创建），模块复位 = MCU
 *    重启，天然重建。
 ******************************************************************************/
#include "dpsm.h"
#include "msm.h"                /* MsmModuleReadyT / MsmLowPwrS / MsmFaultTrigger */
#include "adpat.h"              /* Read/WriteSramProtocolMsaSpace */
#include "ral.h"                /* PAGE_ID_10H_0 / PAGE_ID_11H_0 */
#include "timer2_scheduler.h"   /* Timer2SchedulerAdd / g_TaskIds */

/*-------------------- 调度与超时参数（1 tick = 1ms） --------------------*/
#define DPSM_TASK_DELAY_TICKS       10U
#define DPSM_TASK_INTERVAL_TICKS    10U
#define DPSM_INIT_TIMEOUT_TICKS     200U    /* DPInit commissioning 超时 */
#define DPSM_TX_TURN_TIMEOUT_TICKS  400U    /* Tx 开关物理时延上限(toff_txdis 档) */
#define DPSM_DEINIT_TIMEOUT_TICKS   100U

/* 影子偏移统一见 ral.h PAGE_10H_ADDR_* / PAGE_11H_ADDR_* */
/*-------------------- DPState / ConfigStatus 编码（Table 8-84/8-91） --------------------*/
#define DPST_DEACTIVATED    0x1U    /* DPDeactivated（未用 lane 亦报此值） */
#define DPST_INIT           0x2U    /* DPInit */
#define DPST_DEINIT         0x3U    /* DPDeinit */
#define DPST_ACTIVATED      0x4U    /* DPActivated */
#define DPST_TX_TURN_ON     0x5U    /* DPTxTurnOn */
#define DPST_TX_TURN_OFF    0x6U    /* DPTxTurnOff */
#define DPST_INITIALIZED    0x7U    /* DPInitialized */

/*-------------------- ConfigStatus 编码（Table 8-91） --------------------
 * 已接线（DpsmApplyDpInit 用）：1h Success / 3h InvalidAppSel /
 * 7h PartialDataPath / Ch InProgress。
 * 现仅登记未用（DP 组合/breakout 校验启用后接线）：
 *   4h ConfigRejectedInvalidDataPath——「lane 组合与 AppSel 不符」：DPConfigLane
 *      的 DataPathID 分组/lane 数与 AppSel 定义不符；异构 breakout（多 DP
 *      混用不同 AppSelCode，用户定稿要求全模块活跃 DP 同一 AppSelCode）也归此码。
 *   6h ConfigRejectedLanesInUse——「有 lane 不在 DPDeactivated」：改 DP 组成
 *      （AppSel/DataPathID/lane 宽度）且涉及 lane 未 DPDeactivated（ch06
 *      anti-pattern）。★非 ApplyDPInit 状态门——SteppedConfigOnly=1 下
 *      ApplyDPInit 任意稳态均接受，此码只在「改组成」内容校验里查 lane 状态。 */
#define CFST_SUCCESS                0x1U    /* ConfigSuccess */
#define CFST_REJECT_INVALID_APPSEL  0x3U    /* ConfigRejectedInvalidAppSel（AppSelCode 非本模块可运行应用） */
#define CFST_REJECT_DP              0x4U    /* ConfigRejectedInvalidDataPath（lane 组合与 AppSel 不符；待 breakout） */
#define CFST_REJECT_LANES_IN_USE    0x6U    /* ConfigRejectedLanesInUse（改组成时 lane 未 DPDeactivated；待 breakout） */
#define CFST_REJECT_PARTIAL_DP      0x7U    /* ConfigRejectedPartialDataPath（mask 未覆盖整 DP） */
#define CFST_IN_PROGRESS            0xCU    /* ConfigInProgress */

/*-------------------- DP 实例表（AppSel 1 方案：单 DP） --------------------*/
/** 单 DP 描述：host_lanes = lane 位图（bit0=lane1）。breakout 方案
 *  （4×100G / 2×200G）确认后在此表追加项即可，任务体逻辑与 DP 数无关。 */
typedef struct {
    BYTE        host_lanes;     /* 本 DP 占用的 host lane 位图 */
    DpsmState_t state;
    DWORD       dwell;          /* 瞬态已停留拍数（超时看门狗用） */
    BYTE        cfg_in_prog;    /* ConfigInProgress 锁存（Apply 协议） */
} DpsmDp_t;

STATIC DpsmDp_t s_DpTbl[] = {
    { 0x0FU, DPSM_STATE_DEACTIVATED, 0U, 0U },   /* DP0: DataPathID=0, lane1-4 */
};
#define DPSM_DP_NUM (sizeof(s_DpTbl) / sizeof(s_DpTbl[0]))

#define DPSM_ALL_LANES_14   0x0FU   /* AppSel 1 整模块 lane 集合 = DP0 */

/* AppSel 校验（DPConfigLane 字段，Table 8-71/8-92） */
#define DPSM_DPCFG_SIZE             8U      /* DPConfigLane1-8 字节数 */
#define DPSM_DPCFG_APPSEL_SHIFT     4U      /* AppSelCode 位于 bit7-4 */
#define DPSM_APP_SEL_CODE_APP1      0x1U    /* 本模块唯一可运行应用：AppSel 1 = 400G SR4 单 DP */

STATIC DpsmHook_t s_HookDpInit = NULL;
STATIC DpsmHook_t s_HookTxOn   = NULL;
STATIC DpsmHook_t s_HookTxOff  = NULL;

/*-------------------- 01b 自动 commissioning 一次性请求 --------------------
 * AutoCommissioning=01b（Lower 00h:2.1-0）：模块进 ModuleReady 后自动把
 * Data Path commission 到硬件，无需 host 发 ApplyDPInit。boot 时武装、
 * 进 Ready 首拍消费一次；之后 host 接管（DPDeinit 后不再自动重激活）。
 * 不写 host 可见的 DPInitPending（10h/11h:235）——那是 Apply 专用位。 */
STATIC BYTE s_AutoCommission = 0U;

/*-------------------- 影子读写小工具 --------------------*/
STATIC BYTE DpsmShadowReadByte(TabIndex_t tab, BYTE off)
{
    BYTE v = 0U;
    (VOID)ReadSramProtocolMsaSpace(tab, off, &v, 1U);
    return v;
}

STATIC VOID DpsmShadowWriteByte(TabIndex_t tab, BYTE off, BYTE v)
{
    (VOID)WriteSramProtocolMsaSpace(tab, off, &v, 1U);
}

/* ISR 上下文（DpsmApplyDpInit ← ApplyDPInit OPF_IMMEDIATE）：影子访问原子，
 * 禁止上锁（锁仅线程侧）。 */
STATIC VOID DpsmShadowLatchBit(TabIndex_t tab, BYTE off, BYTE bit)
{
    BYTE v = DpsmShadowReadByte(tab, off);
    DpsmShadowWriteByte(tab, off, (BYTE)(v | bit));
}

/*-------------------- 发布 --------------------*/
/** DPState → Table 8-84 编码。 */
STATIC BYTE DpsmStateCode(DpsmState_t s)
{
    switch(s)
    {
    case DPSM_STATE_INIT:        return DPST_INIT;
    case DPSM_STATE_INITIALIZED: return DPST_INITIALIZED;
    case DPSM_STATE_TX_TURN_ON:  return DPST_TX_TURN_ON;
    case DPSM_STATE_ACTIVATED:   return DPST_ACTIVATED;
    case DPSM_STATE_TX_TURN_OFF: return DPST_TX_TURN_OFF;
    case DPSM_STATE_DEINIT:      return DPST_DEINIT;
    case DPSM_STATE_DEACTIVATED:
    default:                     return DPST_DEACTIVATED;
    }
}

/** 发布全部 8 条 lane 的 DPState（11h:128-131，每 lane 4bit）。
 *  DP 内 lane 同值；未用 lane（5-8）恒报 DPDeactivated(1h)。
 *  线程上下文：每 lane nibble RMW 整体包影子锁。 */
STATIC VOID DpsmPublishState(VOID)
{
    BYTE lane;
    AdpatShadowLock();
    for(lane = 0U; lane < 8U; lane++)
    {
        BYTE code = DPST_DEACTIVATED;
        BYTE d;
        for(d = 0U; d < (BYTE)DPSM_DP_NUM; d++)
        {
            if((s_DpTbl[d].host_lanes & (BYTE)(1U << lane)) != 0U) { code = DpsmStateCode(s_DpTbl[d].state); break; }
        }
        /* 4bit/lane：字节号 = lane/2，半字节 = lane%2（低 nibble = 奇数 lane） */
        {
            BYTE off  = (BYTE)(PAGE_11H_ADDR_DP_STATE + (lane >> 1));
            BYTE v    = DpsmShadowReadByte(PAGE_ID_11H_0, off);
            BYTE nib  = (BYTE)(code & 0x0FU);
            v = ((lane & 1U) == 0U) ? (BYTE)((v & 0xF0U) | nib)
                                    : (BYTE)((v & 0x0FU) | (BYTE)(nib << 4));
            DpsmShadowWriteByte(PAGE_ID_11H_0, off, v);
        }
    }
    AdpatShadowUnlock();
}

/** 进稳态门铃：按 DP 的 lane 位图置 DPStateChangedFlag(11h:134) RMW。
 *  COR 字节——线程上下文 RMW 须影子锁内，防与 DtCorClear 清零交错丢更新。 */
STATIC VOID DpsmFlagStateChanged(BYTE lane_mask)
{
    BYTE v;
    AdpatShadowLock();
    v = DpsmShadowReadByte(PAGE_ID_11H_0, PAGE_11H_ADDR_DP_STATE_FLAG);
    DpsmShadowWriteByte(PAGE_ID_11H_0, PAGE_11H_ADDR_DP_STATE_FLAG, (BYTE)(v | lane_mask));
    AdpatShadowUnlock();
}

/** 写 ConfigStatus nibble：lane1-4 → 11h:202/203 的 4 个 nibble。
 *  ISR 上下文（DpsmApplyDpInit ← ApplyDPInit OPF_IMMEDIATE）：影子访问原子，
 *  禁止上锁（锁仅线程侧）。 */
STATIC VOID DpsmPublishConfigStatus(BYTE lane_mask, BYTE code)
{
    BYTE lane;
    for(lane = 0U; lane < 4U; lane++)
    {
        if((lane_mask & (BYTE)(1U << lane)) != 0U)
        {
            BYTE off = (BYTE)(PAGE_11H_ADDR_CONFIG_STATUS + (lane >> 1));
            BYTE v   = DpsmShadowReadByte(PAGE_ID_11H_0, off);
            BYTE nib = (BYTE)(code & 0x0FU);
            v = ((lane & 1U) == 0U) ? (BYTE)((v & 0xF0U) | nib)
                                    : (BYTE)((v & 0x0FU) | (BYTE)(nib << 4));
            DpsmShadowWriteByte(PAGE_ID_11H_0, off, v);
        }
    }
}

/** 进 INIT 时清 DPInitPending（10h:235 与 11h:235 双写，按 lane 位图）。
 *  线程上下文：双字节 RMW 整体包影子锁。 */
STATIC VOID DpsmClearInitPending(BYTE lane_mask)
{
    BYTE v10;
    BYTE v11;
    AdpatShadowLock();
    v10 = DpsmShadowReadByte(PAGE_ID_10H_0, PAGE_10H_ADDR_DP_INIT_PEND);
    v11 = DpsmShadowReadByte(PAGE_ID_11H_0, PAGE_11H_ADDR_DP_INIT_PEND);
    DpsmShadowWriteByte(PAGE_ID_10H_0, PAGE_10H_ADDR_DP_INIT_PEND, (BYTE)(v10 & (BYTE)~lane_mask));
    DpsmShadowWriteByte(PAGE_ID_11H_0, PAGE_11H_ADDR_DP_INIT_PEND, (BYTE)(v11 & (BYTE)~lane_mask));
    AdpatShadowUnlock();
}

/** 置 DPInitPending（Apply 成功后，双写）。 */
STATIC VOID DpsmSetInitPending(BYTE lane_mask)
{
    DpsmShadowLatchBit(PAGE_ID_10H_0, PAGE_10H_ADDR_DP_INIT_PEND, lane_mask);
    DpsmShadowLatchBit(PAGE_ID_11H_0, PAGE_11H_ADDR_DP_INIT_PEND, lane_mask);
}

/*-------------------- 状态迁移 --------------------*/
STATIC BYTE DpsmInTransient(DpsmState_t s)
{
    return ((s == DPSM_STATE_INIT) || (s == DPSM_STATE_DEINIT) ||
            (s == DPSM_STATE_TX_TURN_ON) || (s == DPSM_STATE_TX_TURN_OFF)) ? 1U : 0U;
}

/** 进新态：状态/计时更新 + DPState 发布 + 稳态门铃。 */
STATIC VOID DpsmEnter(DpsmDp_t *dp, DpsmState_t next)
{
    dp->state = next;
    dp->dwell = 0U;
    DpsmPublishState();
    if((next == DPSM_STATE_INITIALIZED) || (next == DPSM_STATE_DEACTIVATED) || (next == DPSM_STATE_ACTIVATED))
    {
        DpsmFlagStateChanged(dp->host_lanes);
    }
}

/** 钩子步进：NULL 立即完成；超时按故障上报（不自行裁决，交 MSM）。 */
STATIC BYTE DpsmHookStep(DpsmHook_t hook, DpsmDp_t *dp, DWORD limit, MsmFaultCause_t fault_cause)
{
    if(hook == NULL) { return 1U; }
    if(hook() == OK) { return 1U; }
    if(++dp->dwell >= limit)
    {
        MsmFaultTrigger(fault_cause);
        dp->dwell = 0U;
    }
    return 0U;
}

/** AppSel 校验：本 DP 触发 lane 的 SCS0::DPConfigLane AppSelCode（bit7-4）
 *  须 == DPSM_APP_SEL_CODE_APP1。单 DP 方案 lane1-4 全用；AppSelCode=0
 *  （未用）或 2-15（App2/App3 已宣告但 firmware 尚未实现）均不支持。
 *  返回 1=通过，0=存在不支持的应用。DataPathID 一致性校验属 DPConfigLane
 *  重建 DP 表范畴（breakout），暂不在本步。 */
STATIC BYTE DpsmValidateAppSel(BYTE lane_mask, CONST BYTE *dpcfg)
{
    BYTE lane;
    for(lane = 0U; lane < 4U; lane++)
    {
        if((lane_mask & (BYTE)(1U << lane)) != 0U)
        {
            BYTE appsel = (BYTE)((dpcfg[lane] >> DPSM_DPCFG_APPSEL_SHIFT) & 0x0FU);
            if(appsel != DPSM_APP_SEL_CODE_APP1) { return 0U; }
        }
    }
    return 1U;
}

/*-------------------- ApplyDPInit（RAL 引擎入口） --------------------*/
DWORD DpsmApplyDpInit(BYTE lane_mask)
{
    BYTE d;
    BYTE mask  = (BYTE)(lane_mask & DPSM_ALL_LANES_14);   /* lane5-8 悬空，剥离 */
    BYTE dpcfg[DPSM_DPCFG_SIZE] = {0U};

    if(mask == 0U) { return OK; }                        /* 无本模块 lane：视为空触发 */

    /* 预读 SCS0::DPConfigLane1-8（AppSel 校验用）。单 DP 仅 bank0 有效；
     * SCS→ACS 拷贝已在上游完成，这里校验 host 暂存的原始 AppSelCode。 */
    (void)ReadSramProtocolMsaSpace(PAGE_ID_10H_0, PAGE_10H_ADDR_SCS_DPCFG, dpcfg, DPSM_DPCFG_SIZE);

    for(d = 0U; d < (BYTE)DPSM_DP_NUM; d++)
    {
        DpsmDp_t *dp   = &s_DpTbl[d];
        BYTE cov       = (BYTE)(mask & dp->host_lanes);
        if(cov == 0U) { continue; }

        /* ① acceptance：瞬态或 ConfigInProgress → 静默忽略整个触发 */
        if(DpsmInTransient(dp->state) != 0U) { return OK; }
        if(dp->cfg_in_prog != 0U)            { return OK; }

        dp->cfg_in_prog = 1U;
        DpsmPublishConfigStatus(dp->host_lanes, CFST_IN_PROGRESS);

        /* ② validation：mask 未完整覆盖本 DP → 拒 ConfigRejectedPartialDataPath(7h)。
         * 写 host_lanes 全组（而非仅 cov）：部分覆盖时整 DP 的 Apply 均被拒，
         * 全组覆盖也一并清掉上一步的 InProgress 中间值，不留 0xCh 残留。
         * 注意：SteppedConfigOnly=1 下 ApplyDPInit 在任意稳态均接受（CMIS
         * Table 6-3「任意」→ 仅 Provision + 置 DPInitPending），不做状态门；
         * ConfigRejectedLanesInUse(6h) 仅当「改 DP 组合（AppSel/DPID/lane 数）
         * 且 lane 未在 DPDeactivated」时触发，属 DP 组合校验范畴，待补。 */
        if(cov != dp->host_lanes)
        {
            DpsmPublishConfigStatus(dp->host_lanes, CFST_REJECT_PARTIAL_DP);
            dp->cfg_in_prog = 0U;
            continue;
        }

        /* ③ AppSel validation：本 DP 触发 lane 的 AppSelCode 须为 App1。
         * 本模块单 DP 只运行 AppSel 1；App2/App3 已在 01h 宣告但 firmware
         * 尚未实现（breakout 待建），host 对它们发起 Apply 一律拒
         * ConfigRejectedInvalidAppSel(3h)——不做「宣告即成功」的假通过。 */
        if(DpsmValidateAppSel(dp->host_lanes, dpcfg) == 0U)
        {
            DpsmPublishConfigStatus(dp->host_lanes, CFST_REJECT_INVALID_APPSEL);
            dp->cfg_in_prog = 0U;
            continue;
        }

        /* ④ execution（SCS→ACS 拷贝已在上游完成）：成功 = Success + 置欠账 */
        DpsmPublishConfigStatus(dp->host_lanes, CFST_SUCCESS);
        DpsmSetInitPending(dp->host_lanes);
        dp->cfg_in_prog = 0U;
    }
    return OK;
}

/*-------------------- 任务体 --------------------*/
VOID DpsmTask(VOID *arg)
{
    BYTE dpDeinitByte, outDisByte, squelchByte, pendByte;
    BYTE moduleReadyT, lowPwrS;
    BYTE d;
    (VOID)arg;

    /* 信号源采样（bank0 影子） */
    dpDeinitByte = DpsmShadowReadByte(PAGE_ID_10H_0, PAGE_10H_ADDR_DP_DEINIT);
    outDisByte   = DpsmShadowReadByte(PAGE_ID_10H_0, PAGE_10H_ADDR_OUT_DISABLE_TX);
    squelchByte  = DpsmShadowReadByte(PAGE_ID_10H_0, PAGE_10H_ADDR_SQUELCH_TX);
    pendByte     = DpsmShadowReadByte(PAGE_ID_10H_0, PAGE_10H_ADDR_DP_INIT_PEND);
    moduleReadyT = MsmModuleReadyT();
    lowPwrS      = MsmLowPwrS();

    for(d = 0U; d < (BYTE)DPSM_DP_NUM; d++)
    {
        DpsmDp_t *dp      = &s_DpTbl[d];
        BYTE m            = dp->host_lanes;
        BYTE dpt          = (BYTE)(((dpDeinitByte & m) == m) ? 1U : 0U);   /* 全 lane=1 才算 */
        BYTE dpDeinitS    = (BYTE)((moduleReadyT == 0U) || (lowPwrS != 0U) || (dpt != 0U));
        BYTE anyDis       = (BYTE)((((outDisByte | squelchByte) & m) != 0U));
        BYTE dpDeactS     = (BYTE)((dpDeinitS != 0U) || (anyDis != 0U));
        BYTE pend         = (BYTE)(((pendByte & m) == m) ? 1U : 0U);     /* 全 lane 置位才算 */

        switch(dp->state)
        {
        case DPSM_STATE_DEACTIVATED:
            /* 00b：仅 host ApplyDPInit 打欠条(pend)触发；01b：另加模块自动
             * commissioning——boot 武装的一次性请求，进 Ready(dpDeinitS==0)
             * 首拍消费，之后 host 用 ApplyDPInit 显式接管。 */
            if(((pend != 0U) || (s_AutoCommission != 0U)) && (dpDeinitS == 0U))
            {
                if(pend != 0U) { DpsmClearInitPending(m); }   /* Apply 欠条兑现即销账 */
                s_AutoCommission = 0U;                         /* 自动请求一次性消费 */
                DpsmEnter(dp, DPSM_STATE_INIT);
            }
            break;

        case DPSM_STATE_INIT:
            if(dpDeinitS != 0U) { DpsmEnter(dp, DPSM_STATE_DEINIT); break; }
            if(DpsmHookStep(s_HookDpInit, dp, DPSM_INIT_TIMEOUT_TICKS, MSM_FAULT_DPINIT_TIMEOUT) != 0U)
            {
                DpsmEnter(dp, DPSM_STATE_INITIALIZED);
            }
            break;

        case DPSM_STATE_INITIALIZED:
            if(dpDeinitS != 0U) { DpsmEnter(dp, DPSM_STATE_DEINIT); break; }
            if(dpDeactS == 0U)  { DpsmEnter(dp, DPSM_STATE_TX_TURN_ON); }
            break;

        case DPSM_STATE_TX_TURN_ON:
            if(dpDeactS != 0U) { DpsmEnter(dp, DPSM_STATE_TX_TURN_OFF); break; }
            if(DpsmHookStep(s_HookTxOn, dp, DPSM_TX_TURN_TIMEOUT_TICKS, MSM_FAULT_TXSW_TIMEOUT) != 0U)
            {
                DpsmEnter(dp, DPSM_STATE_ACTIVATED);
            }
            break;

        case DPSM_STATE_ACTIVATED:
            if(dpDeactS != 0U) { DpsmEnter(dp, DPSM_STATE_TX_TURN_OFF); }
            break;

        case DPSM_STATE_TX_TURN_OFF:
            if(DpsmHookStep(s_HookTxOff, dp, DPSM_TX_TURN_TIMEOUT_TICKS, MSM_FAULT_TXSW_TIMEOUT) != 0U)
            {
                DpsmEnter(dp, DPSM_STATE_INITIALIZED);   /* 掉头/退回都落 INITIALIZED */
            }
            break;

        case DPSM_STATE_DEINIT:
            if(DpsmHookStep(s_HookTxOff, dp, DPSM_DEINIT_TIMEOUT_TICKS, MSM_FAULT_DPDEINIT_TIMEOUT) != 0U)
            {
                DpsmEnter(dp, DPSM_STATE_DEACTIVATED);
            }
            break;

        default:
            DpsmEnter(dp, DPSM_STATE_DEACTIVATED);
            break;
        }
    }
}

/* 对外查询：ModuleDeactivatedT——msm.c 求 LowPwrExS 用 */
BYTE DpsmAllDeactivated(VOID)
{
    BYTE d;
    for(d = 0U; d < (BYTE)DPSM_DP_NUM; d++)
    {
        if(s_DpTbl[d].state != DPSM_STATE_DEACTIVATED) { return 0U; }
    }
    return 1U;
}

DpsmState_t DpsmStateGet(BYTE dpid)
{
    return (dpid < (BYTE)DPSM_DP_NUM) ? s_DpTbl[dpid].state : DPSM_STATE_DEACTIVATED;
}

VOID DpsmHookSet(DpsmHook_t hook_dp_init, DpsmHook_t hook_tx_on, DpsmHook_t hook_tx_off)
{
    s_HookDpInit = hook_dp_init;
    s_HookTxOn   = hook_tx_on;
    s_HookTxOff  = hook_tx_off;
}

VOID DpsmInit(VOID)
{
    BYTE d;
    for(d = 0U; d < (BYTE)DPSM_DP_NUM; d++)
    {
        s_DpTbl[d].state       = DPSM_STATE_DEACTIVATED;
        s_DpTbl[d].dwell       = 0U;
        s_DpTbl[d].cfg_in_prog = 0U;
    }
    DpsmPublishState();         /* 上电全 DP=DPDeactivated；lane5-8 同报 1h */

    s_AutoCommission = 1U;      /* 01b：boot 武装自动 commissioning 请求 */

    g_TaskIds[TASK_ID_DPSM] = Timer2SchedulerAdd(DpsmTask, NULL,
                              DPSM_TASK_DELAY_TICKS, DPSM_TASK_INTERVAL_TICKS,
                              TIMER2_TASK_FOREVER);
}

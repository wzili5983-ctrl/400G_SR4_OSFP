/***************************************************************************//**
 *   @file   msm.c
 *   @brief  MSM（Module State Machine）实现：CMIS 5.3 §6.3.2。
 *   @author Firmware team
 *******************************************************************************
 * Copyright 2026 Terabitcom, Inc.
 *
 * 状态图（AppSel 1 / SteppedConfigOnly=1 方案）：
 *
 *   [复位域: RESET] --MsmMgmtInitDone()--> [LOW_PWR] --!LowPwrS--> [PWR_UP]
 *        ▲                                    ▲  ▲                       |
 *        |                                    |  |                       v
 *        |                                    |  +-------完成------ [READY]
 *        |                                    |          LowPwrExS     |
 *        |                                    |     (=LowPwrS &&       |
 *        |                                    |      DpsmAllDeactT)    v
 *        |                                    +-------完成------ [PWR_DN]
 *        |                                                            |
 *        +-------------------- FaultS(任意态) ---> [FAULT] <---------+
 *
 *  FAULT（主方案 A，2026-10-13 用户定）：进入即复用 s_HookDn 执行 PwrDn
 *  序列（写 LOW 幂等——LOW_PWR 下触发 fault 时轨道本已关断），完成后停在
 *  Fault：ModuleState=FAULT + 物理低功耗，等 host 动作（CMIS Table 6-14
 *  ModuleFault|*Low 语义）；Dn 期间不求值 LowPwrS，超时停拍不级联 fault。
 *
 * 规范要点落点：
 *  - ModulePwrDn 是单行道：PWR_DN 期间即使 LowPwrS 变 FALSE 也不许掉头
 *    回 PWR_UP，必须走完到 LOW_PWR 再采样（CMIS 反模式，状态内忽略
 *    LowPwrS）；
 *  - ModuleLowPwr 停留期间持续采样 LowPwrS（本任务 10ms 周期即采样）；
 *  - ModuleStateChangedFlag 只在进入 LOW_PWR / READY / FAULT 三个稳态时
 *    置位（进入即离开的状态抑制置位——本实现中状态迁移按拍推进，
 *    "进入即离开"只会发生在同一拍内判定，迁移函数对目标态再求一次
 *    退出条件即可自然抑制，见 MsmTransition()）；
 *  - 所有 shadow 发布经 WriteSramProtocolMsaSpace（ADPAT 层）；
 *    ModuleStateChangedFlag 是 COR 字节中的锁存位：本层做读-改-写置位，
 *    host 读后由 RAL 引擎清零（并发风险 R3 登记在 memory_map.md，
 *    与 monitor 层同纪律：不上锁，单层写者）。
 *
 * 影子地址（页内偏移）：
 *   00h:3   GlobalStatus = ModuleState(bit3-1) | InterruptDeasserted(bit0=1)
 *   00h:8   Module-Level Flags byte0：bit0 = ModuleStateChangedFlag
 *   00h:26  ModuleControl：bit4 LowPwrRequestSW / bit6 LowPwrAllowRequestHW
 *   00h:29  ModuleFaultCause
 ******************************************************************************/
#include "msm.h"
#include "adpat.h"              /* Read/WriteSramProtocolMsaSpace */
#include "ral.h"                /* PAGE_ID_LOW */
#include "msl.h"                /* MslLpReqHwIsActive */
#include "dpsm.h"               /* DpsmAllDeactivated */
#include "cdb.h"                /* CdbDspUpgradeActive：DSP_FW 会话冻结上电序列 */
#include "timer2_scheduler.h"   /* Timer2SchedulerAdd / g_TaskIds */

/*-------------------- 调度与超时参数（1 tick = 1ms） --------------------*/
#define MSM_TASK_DELAY_TICKS        10U     /* 首拍 10ms 后启动 */
#define MSM_TASK_INTERVAL_TICKS     10U     /* 周期 10ms：LowPwrS 采样粒度 */
#define MSM_MGMT_INIT_TIMEOUT_TICKS 2000U   /* tMgmtInit = 2000ms 上限 */
#define MSM_PWR_SEQ_TIMEOUT_TICKS   500U    /* PwrUp/PwrDn 单段序列超时 */

/* 影子偏移统一见 ral.h LOW_ADDR_*（00h:3/8/26/29 → MODULE_STATE/MODULE_FLAGS/MODULE_CONTROL/FAULT_CAUSE） */

/* ModuleControl 位（CMIS Table 8-11） */
#define MSM_CTL_LOWPWR_REQ_SW       0x10U   /* 0x1A.4：1=请求停留低功耗 */
#define MSM_CTL_LOWPWR_ALLOW_HW     0x40U   /* 0x1A.6：1=评估 LPMode(默认 1) */

/*-------------------- 内部状态 --------------------*/
STATIC MsmState_t   s_State = MSM_STATE_RESET;
STATIC BYTE         s_MgmtInitDone = 0U;        /* MsmMgmtInitDone() 只生效一次 */
STATIC DWORD        s_DwellTicks   = 0U;        /* 瞬态(PWR_UP/PWR_DN)已停留拍数 */
STATIC MsmPwrSeqHook_t s_HookUp = NULL;
STATIC MsmPwrSeqHook_t s_HookDn = NULL;

/*-------------------- 影子读写小工具 --------------------*/
/** 读 Lower 页 1 字节；读失败返回 0（保守：控制位缺省 0）。 */
STATIC BYTE MsmShadowReadByte(BYTE off)
{
    BYTE v = 0U;
    (VOID)ReadSramProtocolMsaSpace(PAGE_ID_LOW, off, &v, 1U);
    return v;
}

/** 写 Lower 页 1 字节（发布 ModuleState / FaultCause 用）。 */
STATIC VOID MsmShadowWriteByte(BYTE off, BYTE v)
{
    (VOID)WriteSramProtocolMsaSpace(PAGE_ID_LOW, off, &v, 1U);
}

/** 锁存位置位（读-改-写）：COR 字节里置某一位。线程上下文调用——RMW 须在
 *  影子锁内，否则与 I2C1 ISR 的 DtCorClear 清零交错丢更新。 */
STATIC VOID MsmShadowLatchBit(BYTE off, BYTE bit)
{
    BYTE v;
    AdpatShadowLock();
    v = MsmShadowReadByte(off);
    v = (BYTE)(v | bit);
    MsmShadowWriteByte(off, v);
    AdpatShadowUnlock();
}

/*-------------------- 发布 --------------------*/
/** 发布 ModuleState + InterruptDeasserted(=1，INT 引脚驱动归 monitor/MSL)。
 *  ModuleState 编码即 MsmState_t 值（000-101）。 */
STATIC VOID MsmPublishState(VOID)
{
    MsmShadowWriteByte(LOW_ADDR_MODULE_STATE,
                       (BYTE)(((BYTE)s_State << 1) & 0x0FU) | 0x01U);
}

/** 进入 LOW_PWR/READY/FAULT 的"门铃"：置 ModuleStateChangedFlag(00h:8.0)。 */
STATIC VOID MsmFlagStateChanged(VOID)
{
    MsmShadowLatchBit(LOW_ADDR_MODULE_FLAGS, 0x01U);  /* bit0 ModuleStateChangedFlag */
}

/*-------------------- 信号求值 --------------------*/
/** LowPwrS = LowPwrRequestSW OR (AllowRequestHW AND LowPwrRequestHW 电平)。
 *  电平语义：LPMode 静态低电平（MslLpReqHwIsActive），非边沿——
 *  ModuleLowPwr 停留期间持续为真，符合"停留期间持续采样"要求。 */
STATIC BYTE MsmEvalLowPwrS(VOID)
{
    BYTE ctl = MsmShadowReadByte(LOW_ADDR_MODULE_CONTROL);
    if((ctl & MSM_CTL_LOWPWR_REQ_SW) != 0U) { return 1U; }
    if(((ctl & MSM_CTL_LOWPWR_ALLOW_HW) != 0U) && (MslLpReqHwIsActive() != 0U))
    {
        return 1U;
    }
    return 0U;
}

/*-------------------- 状态迁移 --------------------*/
/** 电源序列推进：钩子 NULL 视为立即完成；返回 1=本拍完成。 */
STATIC BYTE MsmPwrSeqStep(MsmPwrSeqHook_t hook)
{
    if(hook == NULL) { return 1U; }
    return (hook() == OK) ? 1U : 0U;
}

/** 进新态的统一出口：发布状态码 + 门铃（按规范只对
 *  LOW_PWR/READY/FAULT 三个稳态置 ModuleStateChangedFlag）。 */
STATIC VOID MsmEnter(MsmState_t next)
{
    s_State = next;
    s_DwellTicks = 0U;
    MsmPublishState();
    if((next == MSM_STATE_LOW_PWR) || (next == MSM_STATE_READY) || (next == MSM_STATE_FAULT))
    {
        MsmFlagStateChanged();
    }
}

/** FaultS：任意状态进 FAULT，写 ModuleFaultCause + 置 ModuleFaultFlag
 *  （00h:8.1，COR 锁存位——与 bit0 ModuleStateChangedFlag 同纪律，host
 *  读后由 RAL 引擎清零；进 fault 只锁存一次）。 */
STATIC VOID MsmGotoFault(MsmFaultCause_t cause)
{
    MsmShadowWriteByte(LOW_ADDR_MODULE_FAULT_CAUSE, (BYTE)cause);
    MsmShadowLatchBit(LOW_ADDR_MODULE_FLAGS, 0x02U);  /* bit1 ModuleFaultFlag */
    MsmEnter(MSM_STATE_FAULT);
}

VOID MsmFaultTrigger(MsmFaultCause_t cause)
{
    if(cause == MSM_FAULT_NONE) { cause = MSM_FAULT_MGMTINIT_TIMEOUT; }  /* 0 保留给"清除"，误用则归自定义 */
    MsmGotoFault(cause);
}

/*-------------------- 任务体 --------------------*/
VOID MsmTask(VOID *arg)
{
    BYTE lowPwrS;
    (VOID)arg;

    switch(s_State)
    {
    case MSM_STATE_RESET:
        /* MgmtInit 等待/超时裁决在 MsmMgmtInitDone 与这里的看门狗之间 */
        if(s_MgmtInitDone != 0U)
        {
            MsmEnter(MSM_STATE_LOW_PWR);
            break;
        }
        if(s_DwellTicks >= MSM_MGMT_INIT_TIMEOUT_TICKS)
        {
            MsmGotoFault(MSM_FAULT_MGMTINIT_TIMEOUT);   /* 厂商自定义：tMgmtInit 超时 */
            break;
        }
        s_DwellTicks++;
        break;

    case MSM_STATE_LOW_PWR:
        /* 持续采样 LowPwrS：变 FALSE 立即上电。DSP_FW 会话期间 CDB 独占
         * RESET_PHY/U15，留在 LOW_PWR，禁止重新发起 DSP 上电。 */
        if((MsmEvalLowPwrS() == 0U) && (CdbDspUpgradeActive() == 0U))
        {
            MsmEnter(MSM_STATE_PWR_UP);
        }
        break;

    case MSM_STATE_PWR_UP:
        /* 瞬态：跑电源上电序列；LowPwrS 回摆允许转 PWR_DN（规范允许） */
        if(MsmEvalLowPwrS() != 0U)
        {
            MsmEnter(MSM_STATE_PWR_DN);
            break;
        }
        /* Start 可能在本序列中途接管 DSP：冻结当前步且不计超时，后续不得
         * 执行 DSP_RELEASE/CHIP_INIT；状态保持到 CDB Run 触发 MCU 复位。 */
        if(CdbDspUpgradeActive() != 0U)
        {
            break;
        }
        if(MsmPwrSeqStep(s_HookUp) != 0U)
        {
            MsmEnter(MSM_STATE_READY);
            break;
        }
        if(++s_DwellTicks >= MSM_PWR_SEQ_TIMEOUT_TICKS)
        {
            MsmGotoFault(MSM_FAULT_PWRUP_TIMEOUT);      /* 厂商自定义：上电序列超时 */
        }
        break;

    case MSM_STATE_READY:
        /* LowPwrExS = LowPwrS AND 所有 DP 均已 DPDeactivated */
        lowPwrS = MsmEvalLowPwrS();
        if((lowPwrS != 0U) && (DpsmAllDeactivated() != 0U))
        {
            MsmEnter(MSM_STATE_PWR_DN);
        }
        break;

    case MSM_STATE_PWR_DN:
        /* 单行道：本态内忽略 LowPwrS，必须走完到 LOW_PWR（规范反模式
         *  防护——这里刻意不求值 LowPwrS） */
        if(MsmPwrSeqStep(s_HookDn) != 0U)
        {
            MsmEnter(MSM_STATE_LOW_PWR);
            break;
        }
        if(++s_DwellTicks >= MSM_PWR_SEQ_TIMEOUT_TICKS)
        {
            MsmGotoFault(MSM_FAULT_PWRDN_TIMEOUT);      /* 厂商自定义：下电序列超时 */
        }
        break;

    case MSM_STATE_FAULT:
        /* 主方案 A（2026-10-13 用户定）：进 Fault 即执行 PwrDn 序列（复用
         * s_HookDn；写 LOW 幂等——LOW_PWR 下触发 fault 时轨道本已关断）。
         * 完成或超时后停拍（s_DwellTicks 冻结），停在 Fault 等 host 动作
         * （读 00h:29/00h:8/复位模块）。ResetS 路径由硬件重启承载。 */
        if((s_HookDn != NULL) && (s_DwellTicks < MSM_PWR_SEQ_TIMEOUT_TICKS))
        {
            if(MsmPwrSeqStep(s_HookDn) != 0U)
            {
                s_DwellTicks = MSM_PWR_SEQ_TIMEOUT_TICKS;   /* 完成：冻结，不再调用 */
            }
            else
            {
                s_DwellTicks++;
            }
        }
        break;

    default:
        break;
    }
}

/*-------------------- 初始化与对外 API --------------------*/
VOID MsmInit(VOID)
{
    s_State = MSM_STATE_RESET;      /* 复位域入口（Resetting/Reset 由 MCU 重启天然完成） */
    s_MgmtInitDone = 0U;
    s_DwellTicks = 0U;
    MsmPublishState();

    g_TaskIds[TASK_ID_MSM] = Timer2SchedulerAdd(MsmTask, NULL,
                             MSM_TASK_DELAY_TICKS, MSM_TASK_INTERVAL_TICKS,
                             TIMER2_TASK_FOREVER);
}

VOID MsmMgmtInitDone(VOID)
{
    if(s_MgmtInitDone == 0U)
    {
        s_MgmtInitDone = 1U;        /* RESET 态任务拍内完成迁移并置门铃 */
    }
}

VOID MsmPwrSeqHookSet(MsmPwrSeqHook_t up, MsmPwrSeqHook_t dn)
{
    s_HookUp = up;
    s_HookDn = dn;
}

MsmState_t MsmStateGet(VOID)
{
    return s_State;
}

BYTE MsmModuleReadyT(VOID)
{
    return (s_State == MSM_STATE_READY) ? 1U : 0U;
}

BYTE MsmLowPwrS(VOID)
{
    return MsmEvalLowPwrS();
}

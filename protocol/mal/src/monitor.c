/***************************************************************************//**
 *   @file   monitor.c
 *   @brief  MAL 监视任务：g_StdPerfMon 驱动的采集发布 / 阈值 Flag / 中断汇总。
 *   @author Firmware team
 *******************************************************************************
 * Copyright 2026 Terabitcom, Inc.
 *
 * 任务本体（MonTask）每周期（300ms）遍历 g_StdPerfMon[PERF_MON_NUM]：
 *   采集（cal / 默认换算）→ value.perfvalue → 写影子发布
 *   → 阈值比较置 Flag（只置不清，COR 锁存）→ 汇总 flag&~mask →
 *   g_MonIntPending → MonIntModeUpdate：挂起切 INT 模式（管脚 = 模块
 *   中断输出，高有效），无挂起回复位直连模式。
 *
 * 语义要点（对照 CMIS 5.3）：
 *   - Flags 是 COR（Read-on-Clear）锁存位：违例周期把位置 1，模块
 *     永不自清；host 读后由 MCI/RAL 引擎清——表项描述符见
 *     monitor_impl.h，引擎 COR 规则见 protocol/cmd。
 *   - Mask 只影响中断汇总（flag & ~mask → INT），不影响 Flags 本体。
 *   - 监视值寄存器 2B 大端（S16：MON_SCALE_T256；其余 scale U16）。
 *   - 阈值与监视值同量纲同编码，直接按整型比较（无需还原物理量）。
 *
 * ★ 待办（产品决策/bring-up）：
 *   - MonDefaultConvert：cal = NULL 表项的 raw→寄存器默认换算——各量纲
 *     换算系数（ADC 分压/采样公式）未拍板（resource 层注记
 *     "sensor/divider formula is unspecified"），暂一律 ERR 跳过；
 *     逐量纲补齐后放开（MON_SCALE_T256 的 NTC 码→°C 公式由 cal 的
 *     K/B 线性化承担，不在默认换算范围）。
 ******************************************************************************/
#include "monitor.h"
#include "monitor_impl.h"        /* g_StdPerfMon / PerfMonInfo_t / MAL_MON_* */
#include "adcresource_impl.h"    /* GetAdcMsaFilteredValue / NO_ADC_MSA_ID */
#include "adpat.h"               /* Read/WriteSramProtocolMsaSpace（影子） */
#include "msl.h"                 /* MslIntRstnModeSet（INT/RSTn 管脚模式） */
#include "timer2_scheduler.h"    /* Timer2SchedulerAdd / g_TaskIds */

/* 任务节拍（tick = 1ms；首次 100ms 后启动，周期 300ms——快于 GN1848
 *  监视 1s 采集周期，阈值/Flag 响应以 300ms 为粒度；★ 可调） */
#define MON_TASK_DELAY_TICKS        100U
#define MON_TASK_INTERVAL_TICKS     300U

volatile BYTE g_MonIntPending = 0U;

/* 影子 2B 大端读写（CMIS 多字节字段字节序） */
STATIC DWORD MonShadowRead16(TabIndex_t tab, BYTE offset, WORD *val)
{
    BYTE b[2];
    if(ReadSramProtocolMsaSpace(tab, offset, b, 2U) != OK) { return ERR; }
    *val = (WORD)(((WORD)b[0] << 8) | (WORD)b[1]);
    return OK;
}

STATIC DWORD MonShadowWrite16(TabIndex_t tab, BYTE offset, WORD val)
{
    BYTE b[2];
    b[0] = (BYTE)(val >> 8);
    b[1] = (BYTE)val;
    return WriteSramProtocolMsaSpace(tab, offset, b, 2U);
}

/** 默认换算（cal = NULL 的表项）：raw 按本条目 value.scale 默认换算入
 *  value.perfvalue。
 *  ★ 各量纲 raw→寄存器换算系数未拍板（ADC 分压/传感器公式待定，
 *  resource 层注记 "sensor/divider formula is unspecified"）——暂一律
 *  ERR，监视任务跳过该条目发布；逐量纲补齐后放开。 */
STATIC DWORD MonDefaultConvert(PerfMonInfo_t *info)
{
    (VOID)info;
    return ERR;    /* ★ 默认换算系数未拍板 */
}

/** 发布：value.perfvalue 按 2B 大端写影子（value.tab/value.offset）。 */
STATIC VOID MonValuePublish(CONST PerfMonInfo_t *info)
{
    (VOID)MonShadowWrite16(info->value.tab, info->value.offset, info->value.perfvalue);
}

/** 阈值比较 + Flag 置位（COR 锁存：只置不清；清由 host 读触发引擎清）。
 *  val = 本次发布的编码值，与 thr 同量纲直接比较；MON_SCALE_T256 按
 *  S16 有符号，其余 scale 按 U16。thr.offset[idx] = MAL_MON_THR_NONE
 *  的项跳过。 */
STATIC VOID MonFlagUpdate(CONST PerfMonInfo_t *info, WORD val)
{
    BYTE idx;
    for(idx = 0U; idx < (BYTE)ALARM_WARNING_NUM; idx++)
    {
        WORD thr;
        BYTE violated;
        if(info->thr.offset[idx] == (BYTE)MAL_MON_THR_NONE) { continue; }    /* 无该项阈值 */
        if(MonShadowRead16(info->thr.tab, info->thr.offset[idx], &thr) != OK) { continue; }
        if(info->value.scale == MON_SCALE_T256)
        {
            /* S16 有符号比较（温度类量纲） */
            signed short sval = (signed short)val;
            signed short sthr = (signed short)thr;
            violated = ((idx == (BYTE)WARNING_LOW)  || (idx == (BYTE)ALARM_LOW))
                       ? (BYTE)(sval < sthr) : (BYTE)(sval > sthr);
        }
        else
        {
            /* U16 无符号比较 */
            violated = ((idx == (BYTE)WARNING_LOW)  || (idx == (BYTE)ALARM_LOW))
                       ? (BYTE)(val < thr) : (BYTE)(val > thr);
        }
        if(violated != 0U)
        {
            BYTE fb;
            if(ReadSramProtocolMsaSpace(info->flag.tab, info->flag.offset[idx], &fb, 1U) == OK)
            {
                fb |= (BYTE)(1U << info->flag.bits[idx]);    /* RMW 只置位，不写 0 */
                (VOID)WriteSramProtocolMsaSpace(info->flag.tab, info->flag.offset[idx], &fb, 1U);
            }
        }
    }
}

/** 中断汇总：逐条目读 Flag 与 Mask 字节，(flag & bit) 且 (mask & bit) == 0
 *  （未屏蔽的挂起 Flag）→ 返回 1。Mask 只影响 INT 汇总、不影响 Flags 本体。
 *  Mask 字节读失败按全屏蔽处理（fail-safe：不拉高 INT）。 */
STATIC BYTE MonIntAggregate(VOID)
{
    BYTE i;
    for(i = 0U; i < (BYTE)PERF_MON_NUM; i++)
    {
        CONST PerfMonInfo_t *info = MonGetStdPerfMonEntry((PerfMon_t)i);
        BYTE idx;
        if(info == NULL) { continue; }
        for(idx = 0U; idx < (BYTE)ALARM_WARNING_NUM; idx++)
        {
            BYTE f, m;
            BYTE bit = (BYTE)(1U << info->flag.bits[idx]);
            if(ReadSramProtocolMsaSpace(info->flag.tab, info->flag.offset[idx], &f, 1U) != OK) { continue; }
            if(ReadSramProtocolMsaSpace(info->flagmsk.tab, info->flagmsk.offset[idx], &m, 1U) != OK) { m = 0xFFU; }    /* fail-safe 全屏蔽 */
            if(((f & bit) != 0U) && ((m & (BYTE)(1U << info->flagmsk.bits[idx])) == 0U))
            {
                return 1U;
            }
        }
    }
    return 0U;
}

/** INT/RSTn 管脚动作：g_MonIntPending 判定后经 MSL 落定管脚功能模式——
 *  存在未屏蔽挂起 Flag → MSL_INTRSTN_MODE_INT（管脚切为模块 Interrupt
 *  输出，高有效 3.3V，即"对外产生中断"）；无挂起 → MSL_INTRSTN_MODE_RESET
 *  （管脚回直连 MCU NRST，中断撤销、复位通道交还 host）。边沿落定：仅
 *  模式翻转时调用 MslIntRstnModeSet，无冗余 GPIO 写。
 *  语义依据 msl.h：M_INT 是功能选择线，高 = 管脚为中断输出。 */
STATIC VOID MonIntModeUpdate(VOID)
{
    STATIC BYTE s_ModeApplied = 0xFFU;    /* 0xFF = 未知：强制首周期落定 */
    BYTE want = (g_MonIntPending != 0U) ? MSL_INTRSTN_MODE_INT : MSL_INTRSTN_MODE_RESET;
    if(want == s_ModeApplied) { return; }
    s_ModeApplied = want;
    MslIntRstnModeSet(want);
}

VOID MonTask(VOID *arg)
{
    BYTE i;
    (VOID)arg;
    for(i = 0U; i < (BYTE)PERF_MON_NUM; i++)
    {
        PerfMonInfo_t *info = MonGetStdPerfMonEntryRw((PerfMon_t)i);
        if(info == NULL) { continue; }
        if((BYTE)info->adindex == (BYTE)NO_ADC_MSA_ID) { continue; }    /* 无采集源：整项跳过 */
        if(info->cal != NULL)
        {
            if(info->cal((VOID *)info) != OK) { continue; }             /* 本次跳过发布 */
        }
        else
        {
            if(MonDefaultConvert(info) != OK) { continue; }             /* ★ 默认换算未拍板 */
        }
        MonValuePublish(info);
        MonFlagUpdate(info, info->value.perfvalue);
    }
    g_MonIntPending = MonIntAggregate();
    MonIntModeUpdate();    /* INT/RSTn 管脚：挂起→中断输出模式，无→复位直连 */
}

VOID MonInit(VOID)
{
    /* 槽位存调度器集中表 g_TaskIds[TASK_ID_PERF_MON]（timer2_scheduler.h）。 */
    g_TaskIds[TASK_ID_PERF_MON] = Timer2SchedulerAdd(MonTask, NULL,
                             MON_TASK_DELAY_TICKS, MON_TASK_INTERVAL_TICKS,
                             TIMER2_TASK_FOREVER);
}

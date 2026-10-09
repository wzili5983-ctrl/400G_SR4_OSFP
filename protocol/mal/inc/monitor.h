/** @file monitor.h
 * @brief MAL 监视任务（采集发布 / 阈值 Flag / 中断汇总）声明。
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * MAL（Management Application Layer）监视任务本体，驱动
 * monitor_impl 的静态描述表 g_StdPerfMon：每个周期把表内各被测物理量
 * 的原始采样换算成 CMIS 寄存器编码值写入影子（对 host 发布），做阈值
 * 比较置 Alarm/Warning Flag（COR 锁存语义），并汇总 flag & ~mask 得到
 * 模块中断挂起状态。
 *
 * 数据流（单周期，见 MonTask）：
 *   g_StdPerfMon[i] ──adindex──> ADC 滤波值（cal 内部经
 *     GetAdcMsaFilteredValue 读取；cal = NULL 走默认换算）
 *     ──cal/scale──> value.perfvalue ──> 影子（value.tab/value.offset）
 *     发布；──thr──> Flag 置位（flag.tab/flag.offset/bits，只置不清）；
 *     全表 flag & ~flagmsk ──> g_MonIntPending（INT 引脚电平驱动 ★ 待补）。
 *
 * 分层：本文件是任务公共入口；表与 cal 函数在 monitor_impl.{c,h}。
 */
#ifndef MONITOR_H
#define MONITOR_H

#include "basedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 最近一个 MonTask 周期汇总出的未屏蔽中断挂起标志：非 0 表示存在
 *  未被 Mask 的 Flag 置位（按 CMIS 语义此时应拉低 INT 引脚；引脚驱动
 *  ★ 落地前由本变量承载状态，供调试/上位机查询）。 */
extern volatile BYTE g_MonIntPending;

/** MAL 监视任务初始化：向 Timer2 调度器注册周期任务
 *  （tick = 1ms：首次 100ms 后启动，周期 300ms——快于 GN1848 监视 1s
 *  采集周期，阈值/Flag 响应以 300ms 为粒度；★ 可调）。在
 *  Timer2SchedulerInit() 之后、调度器启动之前调用。 */
VOID MonInit(VOID);

/** MAL 监视任务体（调度器周期回调；也可脱离调度器直接周期调用）：
 *  1. adindex = NO_ADC_MSA_ID 的条目整项跳过（无采集源）；
 *  2. cal != NULL：调 cal(info)——内部经 adindex 读 ADC 滤波值、标定、
 *     编码入 value.perfvalue；cal = NULL：MonDefaultConvert 默认换算
 *     （★ 各量纲默认换算系数未拍板，暂返回 ERR 跳过本次发布）；
 *     任一路径返回 ERR 本次跳过发布；
 *  3. perfvalue 按 2B 大端写影子（value.tab/value.offset）发布；
 *  4. 阈值比较（thr 影子，量纲同 scale；MON_SCALE_T256 按 S16 有符号，
 *     其余 U16）：违例置 Flag 位——COR 锁存语义，只置不清（模块不自清
 *     Flags，host 读后由引擎清）；thr.offset[idx] = MAL_MON_THR_NONE
 *     的项跳过；
 *  5. 汇总全部条目 flag & ~flagmsk → g_MonIntPending，随后
 *     MonIntModeUpdate 经 MslIntRstnModeSet 落定 INT/RSTn 管脚：挂起 =
 *     MSL_INTRSTN_MODE_INT（管脚为模块中断输出，高有效 3.3V）；无挂起 =
 *     MSL_INTRSTN_MODE_RESET（管脚直连 MCU NRST，复位通道交还 host）。
 *     边沿落定，仅模式翻转时调用。
 */
VOID MonTask(VOID *arg);

#ifdef __cplusplus
}
#endif

#endif /* MONITOR_H */

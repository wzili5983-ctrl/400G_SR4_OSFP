/** @file msl.h
 * @brief MSL 模块信号层（Module Signal Layer）声明。
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * MSL 位于资源层（gpioresource）之上，封装模块级硬件信号语义。
 *
 * 1) M_INT（GPIO_ID_M_INT，PB3，开漏输出）——MSA INT/RSTn 管脚的功能
 *    选择线（M_INT 与模块边缘的 INT/RSTn 管脚不是一个管脚；二者经
 *    TI 选择器 U18/U19 切换连接，拓扑以硬件原理图为准）：
 *    - 低电平（上电默认）：INT/RSTn 管脚硬件等效直连 MCU 的 NRST；
 *      管脚电平由 host（用户）自行决定——host 拉低即硬件复位模块
 *      （MCU 的 Reset 管家，Reset signal 由硬件触发、无需软件参与）。
 *      固件拉低 M_INT 不会复位 MCU，只是把复位通道交给 host。
 *    - 高电平：INT/RSTn 管脚为模块 Interrupt 输出（高有效，3.3V，
 *      原理表「高电平有效；高电平时，模块输出 INT（3.3V）」）。
 *    - 推论：本引脚是功能选择线，不是 INT 驱动器——INT 的动态波形
 *      （中断置位/撤销电平）不经 M_INT 驱动，由选择器另一路承载；
 *      固件对 M_INT 只做模式切换。M_INT 电平可由 ReadGpio 回读
 *      （OD 引脚读回的是引脚实际电平）。
 *    - 中断输出语义澄清：M_INT 高 ≠ 「INT 正在触发」。高电平是 INT
 *      功能的输出载体（host 经选择器在该管脚上接收 3.3V 电平的中断
 *      信号）；真正的中断触发/撤销由模块 INT 侧驱动源产生，MSL 只
 *      负责把管脚切到 INT 模式（高）。
 *
 * 2) LowPwrRequestHW（GPIO_ID_LPMODE，PC6，输入，低有效）：
 *    host 拉低请求进入低功耗；EXTI 下降沿触发，ISR（NVIC 优先级 3，
 *    与 NVIC 规划一致）只置 volatile 标志，主循环取走处理。
 *    【上电补查】上电时 host 已拉低则无线沿可触发，ISR 不会置位，
 *    启动判定须以 MslLpReqHwIsActive() 静态电平为准。
 *
 * M_INT/LPMODE 引脚模式由板级资源初始化（InitBoardResources）完成，
 * 本层只做 EXTI/NVIC 接线与引脚电平落定，不重复初始化引脚。
 */
#ifndef MSL_H
#define MSL_H

#include "basedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/** INT/RSTn 管脚功能模式（M_INT 电平编码）：低=复位直连，高=中断输出。 */
#define MSL_INTRSTN_MODE_RESET      0U    /* M_INT 低：INT/RSTn 直连 MCU NRST，host 控复位 */
#define MSL_INTRSTN_MODE_INT        1U    /* M_INT 高：INT/RSTn 为模块 Interrupt 输出 */

/** LowPwrRequestHW 触发标志（EXTI4_15_IRQHandler 置 1，主循环
 *  MslLpReqHwConsume() 读走清零；不上锁，单写者 ISR + 单读者线程）。 */
extern volatile BYTE g_MslLpReqHw;

/** MSL 初始化：落定 M_INT 初始模式（复位直连模式 = 上电默认，host 可
 *  持模块复位），并接线 LPMODE(PC6) 的 EXTI 下降沿 + NVIC。
 *  在 InitBoardResources() 之后调用。 */
VOID MslInit(VOID);

/** 切换 MSA INT/RSTn 管脚功能模式（见 MSL_INTRSTN_MODE_*）。
 *  mode = MSL_INTRSTN_MODE_RESET：M_INT 拉低，管脚直连 MCU NRST，
 *         host 获得硬件复位通道（host 电平决定复位，与固件无关）；
 *  mode = MSL_INTRSTN_MODE_INT  ：M_INT 释放拉高，管脚为模块 Interrupt
 *         输出（高有效 3.3V）。
 *  非法值忽略。 */
VOID MslIntRstnModeSet(BYTE mode);

/** 读回当前 INT/RSTn 功能模式（MSL_INTRSTN_MODE_* 之一）。 */
BYTE MslIntRstnModeGet(VOID);

/** 取走 LowPwrRequestHW 触发标志：返回非 0 表示自上次取走后发生过
 *  LPMODE 下降沿（host 请求过低功耗），读后清零。主循环调用。 */
BYTE MslLpReqHwConsume(VOID);

/** LowPwrRequestHW 当前原始电平：返回非 0 表示 LPMODE 现为低电平
 *  （低有效，host 正在请求低功耗）。用于上电静态判定与去抖参考。 */
BYTE MslLpReqHwIsActive(VOID);

#ifdef __cplusplus
}
#endif

#endif /* MSL_H */

/***************************************************************************//**
 *   @file   main.c
 *   @brief  main file
 *   @author Oliver Wang (zili.wang@terabitcom.com)
********************************************************************************
 * Copyright 2024~2026(c) Terabitcom, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of Terabitcom, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Terabitcom, INC. ��AS IS�� AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL Terabitcom, INC. BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

#include "gd32e511_512.h"
#include "mcu_clock.h"          /* McuClockInit：系统时钟 120MHz → 80MHz */
#include "boardresource.h"
#include "timer2_scheduler.h"   /* Timer2SchedulerInit / Timer2SchedulerRun */
#include "mgmt_init.h"          /* MgiInit：MgmtInit 管理侧 bring-up */
#include "data_transfer.h"      /* CmdExeBackground：排空 DEFERRED 回调 */
#include "msl.h"                /* MslLpReqHwConsume：主循环消费 LPMODE 触发标志 */
#include "watchdog.h"           /* WatchdogInit / WatchdogKick：独立看门狗 */
#include "cdb.h"                /* CdbExeBackground：主循环执行 pending CDB 命令（固件下载） */

/* Inspect in debugger: OK after all documented resources are initialized. */
volatile DWORD g_BoardResourceStatus = ERR;

/* 周期任务执行：驱动到期的周期任务（Msm/Dpsm/Mon/ADC/DRV）。main 每次
 * 循环调一次。 */
STATIC VOID TaskExe(VOID)
{
    Timer2SchedulerRun();
}

int main(void)
{
    /* 系统时钟降到 80MHz（与 BL 救援环一致）：Timer2 调度器分频按 80MHz
     * 硬编码，SystemInit 默认 120MHz 会让 1ms tick 变成 0.667ms（快 1.5×）。
     * McuClockInit 内部已调 SystemCoreClockUpdate，无需再单独调。 */
    McuClockInit();

    /* ① 板级资源：GPIO 引脚模式 / ADC / I2C 总线（含 DSP/PMIC 软总线）。
     *    这是「平台基座」，管理与后续高功率数据通路都依赖它，故留在
     *    main 而不并入 MgiInit（MgiInit 只做管理侧 bring-up）。 */
    g_BoardResourceStatus = InitBoardResources();

    /* ② 系统调度器：80MHz 时钟 / 1ms tick（入参现被实现忽略、内部固定
     *    80MHz/1ms）。Msm/Dpsm/Mon/ADC/DRV 任务注册都依赖它，必须先于
     *    MgiInit。 */
    Timer2SchedulerInit(80000000U, 1000U);

    /* ③ MgmtInit：管理侧 bring-up（MCI/RAL 影子/MSM/DPSM/监视），结束
     *    后模块停在 ModuleLowPwr，可被 host 管理。 */
    (void)MgiInit();

    /* ④ 独立看门狗：管理接口就绪后启动（此后无法关闭）。主循环每拍喂狗，
     *    任一环节阻塞超过 4000ms 即复位 MCU（复位源识别报 MGI_RESET_WATCHDOG）。 */
    WatchdogInit(4000U);

    /* ⑤ 主循环：周期任务 + 喂狗 + DEFERRED 回调 + LPMODE 触发标志。 */
    while(1)
    {
        TaskExe();                  /* 周期任务（Msm/Dpsm/Mon/ADC/DRV） */
        WatchdogKick();             /* 喂狗：卡死超时即复位 */
        CmdExeBackground();         /* 排空 OPF_DEFERRED 队列（密码/D9H 邮箱等） */
        CdbExeBackground();         /* 执行 pending CDB 命令（固件下载/通用命令） */
        (void)MslLpReqHwConsume();  /* 取走 LPMODE 下降沿标志（MSM 用静态电平判 LowPwrS） */
    }
    return 0;
}



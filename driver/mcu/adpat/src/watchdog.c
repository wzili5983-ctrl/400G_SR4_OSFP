/***************************************************************************//**
 *   @file   watchdog.c
 *   @brief  独立看门狗（FWDGT）驱动实现。
 *   @author Firmware team
 *******************************************************************************
 * Copyright 2026 Terabitcom, Inc.
 *
 * FWDGT（独立看门狗）由 IRC40K（40kHz 内部 RC）驱动，启动后不可关闭。
 * 主循环每拍 WatchdogKick() 重载计数器；若卡死超过超时时间则复位 MCU。
 ******************************************************************************/
#include "watchdog.h"

#include "gd32e511_512_rcu.h"   /* rcu_osci_on / rcu_osci_stab_wait / RCU_IRC40K */
#include "gd32e511_512_fwdgt.h" /* fwdgt_config / fwdgt_enable / fwdgt_counter_reload */

/* 预分频换算表：FWDGT 分频比 ∈ {4,8,16,32,64,128,256}，寄存器编码依次为
 * FWDGT_PSC_DIV4..FWDGT_PSC_DIV256（PSC_PSC(0..6)）。 */
typedef struct {
    WORD psc;   /* 实际分频比 */
    BYTE reg;   /* FWDGT_PSC_* 寄存器编码 */
} WatchdogPsc_t;

STATIC CONST WatchdogPsc_t s_PscTable[] = {
    {   4U, FWDGT_PSC_DIV4   },
    {   8U, FWDGT_PSC_DIV8   },
    {  16U, FWDGT_PSC_DIV16  },
    {  32U, FWDGT_PSC_DIV32  },
    {  64U, FWDGT_PSC_DIV64  },
    { 128U, FWDGT_PSC_DIV128 },
    { 256U, FWDGT_PSC_DIV256 },
};

#define WATCHDOG_PSC_NUM   (sizeof(s_PscTable) / sizeof(s_PscTable[0]))

VOID WatchdogInit(DWORD timeout_ms)
{
    DWORD target;   /* 需要的总计数 = (reload+1) × 分频比 = timeout_ms × 40 */
    DWORD reload;   /* 0..4095 */
    BYTE  reg = FWDGT_PSC_DIV256;   /* 兜底：最大分频（夹取后必被覆盖） */
    BYTE  i;

    /* 夹取到可表示范围。超时 = (reload+1) × 分频 / 40kHz，reload 为 12bit、
     * 分频最大 256 → 最大 (4096×256)/40 = 26214.4ms，取整 26214ms；最小约
     * 0.1ms。IRC40K 精度有限（±容差），实际超时以实机为准。 */
    if(timeout_ms < WATCHDOG_TIMEOUT_MIN_MS) { timeout_ms = WATCHDOG_TIMEOUT_MIN_MS; }
    if(timeout_ms > WATCHDOG_TIMEOUT_MAX_MS) { timeout_ms = WATCHDOG_TIMEOUT_MAX_MS; }

    target = timeout_ms * 40U;   /* 40 计数/ms（IRC40K = 40kHz） */

    /* 选最小分频使 reload+1 ≤ 4096：分频越小分辨率越高、超时越准。 */
    for(i = 0U; i < (BYTE)WATCHDOG_PSC_NUM; i++)
    {
        reload = (target + s_PscTable[i].psc - 1U) / s_PscTable[i].psc;   /* ceil → reload+1 */
        if(reload <= 4096U)
        {
            reload -= 1U;   /* 得真正的 reload 值（0..4095） */
            reg = s_PscTable[i].reg;
            break;
        }
    }

    /* 使能 IRC40K 并等待稳定（FWDGT 唯一时钟源）。 */
    rcu_osci_on(RCU_IRC40K);
    (void)rcu_osci_stab_wait(RCU_IRC40K);

    /* 配置重载值 + 预分频（fwdgt_config 内部已开写访问并重载一次），
     * 再启动计数。启动后无法关闭，复位前一直运行。 */
    (void)fwdgt_config((uint16_t)reload, reg);
    fwdgt_enable();
}

VOID WatchdogKick(VOID)
{
    fwdgt_counter_reload();
}

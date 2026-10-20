/** @file odsp_lasi.c
 * @brief LASI (PE7) EXTI 双沿中断接线 + 共享 IRQ 分发 + 任务侧处理实现。
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * 实现要点：
 *   - EXTI4_15 共享 IRQ：MSL 已占 line 6 (LPMODE)，本模块加 line 7
 *     (LASI)——msl.c 的 EXTI4_15_IRQHandler 已扩展调用 OdspLasiIrqHandler()
 *     （主控接线）；
 *   - ISR 只置标志不做事：BSC/I2C 事务会阻塞 SCL 拉伸，ISR 内严禁调用，
 *     实际处理全部下沉到任务侧 OdspLasiPoll；
 *   - 占位期保守策略：DSP 中断源寄存器地址属 Broadcom NDA，OdspLasiPoll
 *     高电平时不做影子置位——避免任何 DSP 中断被误报为 LOS。完整版按
 *     中断源寄存器按位分发到 LOSFlagRx / LOLFlagRx / FaultFlagRx / 温度
 *     Flags，届时再引入 AdpatShadowLock 保护影子 RMW。
 */
#include "odsp_lasi.h"

#include "gpioresource.h"
#include "gpioresource_impl.h"

#include "gd32e511_512.h"
#include "gd32e511_512_exti.h"
#include "gd32e511_512_syscfg.h"

/** LASI(PE7) 映射的 EXTI 线号：PE7 → EXTI line 7。 */
#define ODSP_LASI_EXTI_LINE         EXTI_7

/*=========================== 可变状态（STATIC 单独区） ===========================*/

/* 单写者 = OdspLasiIrqHandler (ISR)，单读者 = OdspLasiPoll (任务)。
 * volatile：跨上下文可见性；字节写原子，无需额外同步。
 * STATIC：仅本编译单元使用，避免外部链接符号污染。 */
STATIC volatile BYTE g_OdspLasiIrqPending = 0U;

VOID OdspLasiInit(VOID)
{
    /* PE7 → EXTI line 7，双沿触发：下降沿 = DSP 中断置位，上升沿 = 撤销。
     * GD32E511 的 SYSCFG 无独立时钟门控，无需 rcu 使能（同 msl.c 用法）。
     * NVIC（EXTI4_15_IRQn prio 3）已由 MslInit 配置使能，此处不重复。 */
    syscfg_exti_line_config(EXTI_SOURCE_GPIOE, EXTI_SOURCE_PIN7);
    exti_init(ODSP_LASI_EXTI_LINE, EXTI_INTERRUPT, EXTI_TRIG_BOTH);
    exti_interrupt_flag_clear(ODSP_LASI_EXTI_LINE);
    exti_interrupt_enable(ODSP_LASI_EXTI_LINE);
}

VOID OdspLasiIrqHandler(VOID)
{
    /* ISR 上下文严禁 BSC/I2C 读（SCL 拉伸会阻塞 I2C1 从机背压），
     * 仅清标志置挂起位速回，实际处理在任务侧 OdspLasiPoll。 */
    if(exti_interrupt_flag_get(ODSP_LASI_EXTI_LINE) != RESET) {
        exti_interrupt_flag_clear(ODSP_LASI_EXTI_LINE);
        g_OdspLasiIrqPending = 1U;
    }
}

VOID OdspLasiPoll(VOID)
{
    DWORD level;

    if(g_OdspLasiIrqPending == 0U) {
        return;
    }

    if(ReadGpio(GPIO_ID_LASI_PHY, &level) != OK) {
        return;    /* 读失败：保留 pending=1，下拍重试——LASI 为电平保持型
                    * 中断线，若先清 pending 此事件会被永久丢弃 */
    }
    g_OdspLasiIrqPending = 0U;    /* GPIO 读成功才清：事件已确认接收 */

    if(level != IO_HIGH) {
        return;    /* 上升沿撤销中断，无挂起事件 */
    }

    /* ★ 完整版：ReadChip(CHIP_ID_DSP, ODSP_LASI_STATUS_REG, &val) 读 DSP
     * 中断源寄存器，按位分发（FEC lock/unlock / LOS / CDR LOL / 过温）到
     * LOSFlagRx / LOLFlagRx / FaultFlagRx / 00h:9 温度 Flags。
     * 寄存器地址属 Broadcom NDA，占位期间不做影子置位——避免任何 DSP
     * 中断被误报为 LOS（误导 host CMIS 解析）。 */
}

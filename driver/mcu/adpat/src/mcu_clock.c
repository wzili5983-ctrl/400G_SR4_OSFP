/**
 * @file mcu_clock.c
 * @brief IRC8M to 80 MHz system clock and SysTick polling based us/ms/s delays.
 */
#include "mcu_clock.h"
#include "gd32e511_512.h"
#include "gd32e511_512_fmc.h"
#include "gd32e511_512_rcu.h"

/* SysTick 为 24 位递减计数器，单次装载上限 2^24-1。
 * 80 MHz 下单次最长定时约 209 ms，更长延时按分片循环。 */
#define MCU_SYSTICK_MAX_US   (0x00FFFFFFU / (SystemCoreClock / 1000000U))

VOID McuClockInit(VOID)
{
    __IO DWORD reg_temp;

    RCU_CTL0 |= RCU_CTL0_IRC8MEN;
    while ((RCU_CTL0 & RCU_CTL0_IRC8MSTB) == 0U) {
    }
    fmc_wscnt_set(WS_WSCNT_3);
    rcu_ahb_clock_config(RCU_AHB_CKSYS_DIV1);
    rcu_apb1_clock_config(RCU_APB1_CKAHB_DIV1);
    rcu_apb2_clock_config(RCU_APB2_CKAHB_DIV1);
    RCU_CFG0 &= ~(RCU_CFG0_PLLSEL | RCU_CFG0_PLLMF);
    RCU_CFG0 |= RCU_PLLSRC_IRC8M_DIV2 | RCU_PLL_MUL20;
    RCU_CTL0 |= RCU_CTL0_PLLEN;
    while ((RCU_CTL0 & RCU_CTL0_PLLSTB) == 0U) {
    }
    reg_temp = RCU_CFG0 & ~RCU_CFG0_SCS;
    RCU_CFG0 = reg_temp | RCU_CKSYSSRC_PLL;
    while ((RCU_CFG0 & RCU_CFG0_SCSS) != RCU_SCSS_PLL) {
    }
    SystemCoreClockUpdate();
    SysTick->CTRL = 0U;
}

VOID McuClockDelayUs(DWORD microsecs)
{
    CONST DWORD ticks_per_us = SystemCoreClock / 1000000U;

    while (microsecs > 0U) {
        DWORD chunk = (microsecs > MCU_SYSTICK_MAX_US) ? MCU_SYSTICK_MAX_US : microsecs;

        SysTick->LOAD = chunk * ticks_per_us - 1U;
        SysTick->VAL = 0U;
        SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
        while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0U) {
        }
        SysTick->CTRL = 0U;

        microsecs -= chunk;
    }
}

VOID McuClockDelayMs(DWORD millisecs)
{
    while (millisecs-- != 0U) {
        McuClockDelayUs(1000U);
    }
}

VOID McuClockDelayS(DWORD secs)
{
    while (secs-- != 0U) {
        McuClockDelayMs(1000U);
    }
}



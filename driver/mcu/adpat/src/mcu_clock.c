/**
 * @file mcu_clock.c
 * @brief IRC8M to 80 MHz system clock and 1 us SysTick delay.
 */
#include "mcu_clock.h"
#include "gd32e511_512.h"
#include "gd32e511_512_fmc.h"
#include "gd32e511_512_rcu.h"

STATIC volatile DWORD g_SystickUs;

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
    g_SystickUs = 0U;
    SysTick_Config(SystemCoreClock / 1000000U);
    NVIC_SetPriority(SysTick_IRQn, 0U);
}

VOID SysTick_Handler(VOID)
{
    ++g_SystickUs;
}

VOID delay_us(DWORD microsecs)
{
    DWORD start = g_SystickUs;

    while ((DWORD)(g_SystickUs - start) < microsecs) {
        __NOP();
    }
}

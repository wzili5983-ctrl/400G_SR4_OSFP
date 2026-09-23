/*!
    \file    gd32e511_512_pmu.c
    \brief   PMU driver

    \version 2025-12-25, V0.2.0, firmware for GD32E511_512
*/

/*
    Copyright (c) 2025, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include "gd32e511_512_pmu.h"

#define PMU_WAKEUP_PIN_MASK       ((uint32_t)0x00003300U)      /* bit mask of PMU wakeup pin */
#define PMU_CLEAR_MASK            ((uint32_t)0xFFFFFFF3U)      /* bit mask of PMU flag clear function */
#define PMU_GET_MASK              ((uint32_t)0x00000007U)      /* bit mask of PMU flag get function */
#define PMU_PLLSEL_MASK           ((uint32_t)0x00000100U)      /* bit mask of PMU PLL output selection */

/*!
    \brief      reset PMU registers (API_ID(0x0001U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_deinit(void)
{
    uint32_t pll_back = 0U, ahbpsc_back = 0U, reg = RCU_CFG0;
    uint16_t i = 0U;
    pll_back = PMU_PLL_CTL & PMU_PLL_CTL_PLLOSEL;

    /* when PMU_PLL_CTL_PLLOSEL is 1, AHB frequency should be devided by 2 before reset PMU */
    if(0U != pll_back){
        ahbpsc_back = GET_BITS(RCU_CFG0, 4, 7);
        reg &= ~RCU_CFG0_AHBPSC;
        if(ahbpsc_back < 0x8U){
            RCU_CFG0 = reg | RCU_AHB_CKSYS_DIV2;
        }else if(ahbpsc_back < 0xBU){
            RCU_CFG0 = reg | CFG0_AHBPSC(ahbpsc_back + 1U);
        }else{
            /* no operation */
        }
        for(i = 0U; i < 0x50U; i++){
        }
    }

    /* reset PMU */
    rcu_periph_reset_enable(RCU_PMURST);
    rcu_periph_reset_disable(RCU_PMURST);

    /* recover PMU_PLL_CTL_PLLOSEL setting and AHB frequency */
    if(0U != pll_back){
        for(i = 0U; i < 0x50U; i++){
        }
        RCU_APB1EN |= RCU_APB1EN_PMUEN;
        PMU_PLL_CTL |= PMU_PLL_CTL_PLLOSEL;
        RCU_CFG0 = reg | CFG0_AHBPSC(ahbpsc_back);
    }
}

/*!
    \brief      select low voltage detector threshold (API_ID(0x0002U))
    \param[in]  lvdt_n:
                only one parameter can be selected which is shown as below:
      \arg        PMU_LVDT_0: voltage threshold is 2.1V
      \arg        PMU_LVDT_1: voltage threshold is 2.3V
      \arg        PMU_LVDT_2: voltage threshold is 2.4V
      \arg        PMU_LVDT_3: voltage threshold is 2.6V
      \arg        PMU_LVDT_4: voltage threshold is 2.7V
      \arg        PMU_LVDT_5: voltage threshold is 2.9V
      \arg        PMU_LVDT_6: voltage threshold is 3.0V
      \arg        PMU_LVDT_7: voltage threshold is 3.1V
    \param[out] none
    \retval     none
*/
void pmu_lvd_select(uint32_t lvdt_n)
{
    /* disable LVD */
    PMU_CTL &= ~PMU_CTL_LVDEN;
    /* clear LVDT bits */
    PMU_CTL &= ~PMU_CTL_LVDT;
    /* set LVDT bits according to lvdt_n */
    PMU_CTL |= lvdt_n & PMU_CTL_LVDT;
    /* enable LVD */
    PMU_CTL |= PMU_CTL_LVDEN;
}

/*!
    \brief      select LDO output voltage (API_ID(0x0003U))
                these bits set by software when the main PLL closed
    \param[in]  ldo_output:
                only one parameter can be selected which is shown as below:
      \arg        PMU_LDOVS_LOW: LDO output voltage low mode
      \arg        PMU_LDOVS_HIGH: LDO output voltage high mode
    \param[out] none
    \retval     none
*/
void pmu_ldo_output_select(uint32_t ldo_output)
{
    PMU_CTL &= ~PMU_CTL_LDOVS;
    PMU_CTL |= ldo_output & PMU_CTL_LDOVS;
}

/*!
    \brief      disable PMU lvd (API_ID(0x0004U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_lvd_disable(void)
{
    /* disable LVD */
    PMU_CTL &= ~PMU_CTL_LVDEN;
}

/*!
    \brief      PMU work in sleep mode (API_ID(0x0005U))
    \param[in]  sleepmodecmd:
                only one parameter can be selected which is shown as below:
      \arg        WFI_CMD: use WFI command
      \arg        WFE_CMD: use WFE command
    \param[out] none
    \retval     none
*/
void pmu_to_sleepmode(uint8_t sleepmodecmd)
{
    /* clear sleepdeep bit of Cortex-M33 system control register */
    SCB->SCR &= ~((uint32_t)SCB_SCR_SLEEPDEEP_Msk);

    /* select WFI or WFE command to enter sleep mode */
    if(WFI_CMD == sleepmodecmd) {
        __WFI();
    } else {
        __SEV();
        __WFE();
        __WFE();
    }
}

/*!
    \brief      PMU work in deepsleep mode (API_ID(0x0006U))
    \param[in]  ldo:
                only one parameter can be selected which is shown as below:
      \arg        PMU_LDO_NORMAL: LDO operates normally when PMU enter deepsleep mode
      \arg        PMU_LDO_LOWPOWER: LDO work at low power mode when PMU enter deepsleep mode (note that
                                    when the system clock is 120MHz, this mode cannot be set)
    \param[in]  deepsleepmodecmd:
                only one parameter can be selected which is shown as below:
      \arg        WFI_CMD: use WFI command
      \arg        WFE_CMD: use WFE command
    \param[out] none
    \retval     none
*/
void pmu_to_deepsleepmode(uint32_t ldo, uint8_t deepsleepmodecmd)
{
    /* clear stbmod and ldolp bits */
    PMU_CTL &= ~((uint32_t)(PMU_CTL_STBMOD | PMU_CTL_LDOLP));

    /* set ldolp bit according to ldo */
    PMU_CTL |= ldo & PMU_CTL_LDOLP;

    /* set sleepdeep bit of Cortex-M33 system control register */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    /* select WFI or WFE command to enter deepsleep mode */
    if(WFI_CMD == deepsleepmodecmd) {
        __WFI();
    } else {
        __SEV();
        __WFE();
        __WFE();
    }
    /* reset sleepdeep bit of Cortex-M33 system control register */
    SCB->SCR &= ~((uint32_t)SCB_SCR_SLEEPDEEP_Msk);
}

/*!
    \brief      PMU work in standby mode (API_ID(0x0007U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_to_standbymode(void)
{
    /* switch to IRC8M clock as system clock, close HXTAL */
    RCU_CFG0 &= ~RCU_CFG0_SCS;
    RCU_CTL0 &= ~RCU_CTL0_HXTALEN;

    /* clear LVDEN bit to reduce power consumption */
    PMU_CTL &= ~PMU_CTL_LVDEN;

    /* set stbmod bit */
    PMU_CTL |= PMU_CTL_STBMOD;

    /* reset wakeup flag */
    PMU_CTL |= PMU_CTL_WURST;

    /* set sleepdeep bit of Cortex-M33 system control register */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    REG32(0xE000E010U) &= 0x00010004U;
    REG32(0xE000E180U)  = 0XFFFFFFFBU;
    REG32(0xE000E184U)  = 0XFFFFFFFFU;
    REG32(0xE000E188U)  = 0xFFFFFFFFU;

    /* select WFI command to enter standby mode */
    __WFI();
}

/*!
    \brief      enable PMU wakeup pin (API_ID(0x0008U))
    \param[in]  wakeup_pin:
                one or more parameters can be selected which are shown as below:
      \arg        PMU_WAKEUP_PIN0: WKUP Pin 0 (PA0)
      \arg        PMU_WAKEUP_PIN1: WKUP Pin 1 (PC13)
      \arg        PMU_WAKEUP_PIN4: WKUP Pin 4 (PC5)
      \arg        PMU_WAKEUP_PIN5: WKUP Pin 5 (PB5)
    \param[out] none
    \retval     none
*/
void pmu_wakeup_pin_enable(uint32_t wakeup_pin)
{
    PMU_CS |= wakeup_pin & PMU_WAKEUP_PIN_MASK;
}

/*!
    \brief      disable PMU wakeup pin (API_ID(0x0009U))
    \param[in]  wakeup_pin:
                one or more parameters can be selected which are shown as below:
      \arg        PMU_WAKEUP_PIN0: WKUP Pin 0 (PA0)
      \arg        PMU_WAKEUP_PIN1: WKUP Pin 1 (PC13)
      \arg        PMU_WAKEUP_PIN4: WKUP Pin 4 (PC5)
      \arg        PMU_WAKEUP_PIN5: WKUP Pin 5 (PB5)
    \param[out] none
    \retval     none
*/
void pmu_wakeup_pin_disable(uint32_t wakeup_pin)
{
    PMU_CS &= ~(wakeup_pin & PMU_WAKEUP_PIN_MASK);
}

/*!
    \brief      enable backup domain write (API_ID(0x000AU))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_backup_write_enable(void)
{
    PMU_CTL |= PMU_CTL_BKPWEN;
}

/*!
    \brief      disable backup domain write (API_ID(0x000BU))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_backup_write_disable(void)
{
    PMU_CTL &= ~PMU_CTL_BKPWEN;
}

/*!
    \brief      PLL output to system clock selection (API_ID(0x000CU))
    \param[in]  pllsel:
                only one parameter can be selected which is shown as below:
      \arg        PMU_SELECT_PLL: select PLL output to system clock
      \arg        PMU_SELECT_PLL_DIV2: select PLL/2 output to system clock
    \param[out] none
    \retval     none
*/
void pmu_pll_as_system_clock_selection(uint32_t pllsel)
{
    uint32_t reg = PMU_PLL_CTL;
    reg &= ~PMU_PLL_CTL_PLLOSEL;
    reg |= pllsel & PMU_PLLSEL_MASK;
    PMU_PLL_CTL = reg;
}

/*!
    \brief      get flag state (API_ID(0x000DU))
    \param[in]  flag:
                only one parameter can be selected which is shown as below:
      \arg        PMU_FLAG_WAKEUP: wakeup flag
      \arg        PMU_FLAG_STANDBY: standby flag
      \arg        PMU_FLAG_LVD: lvd flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus pmu_flag_get(uint32_t flag)
{
    FlagStatus ret_status = RESET;

    if(RESET != (PMU_CS & (flag & PMU_GET_MASK))) {
        ret_status = SET;
    }
    return ret_status;
}

/*!
    \brief      clear flag bit (API_ID(0x000EU))
    \param[in]  flag:
                one or more parameters can be selected which are shown as below:
      \arg        PMU_FLAG_WAKEUP: reset wakeup flag
      \arg        PMU_FLAG_STANDBY: reset standby flag
    \param[out] none
    \retval     none
*/
void pmu_flag_clear(uint32_t flag)
{
    uint32_t reg = PMU_CTL & PMU_CLEAR_MASK;

    if(RESET != (flag & PMU_FLAG_WAKEUP)) {
        /* reset wakeup flag */
        PMU_CTL = reg | PMU_CTL_WURST;
    }
    if(RESET != (flag & PMU_FLAG_STANDBY)) {
        /* reset standby flag */
        PMU_CTL = reg | PMU_CTL_STBRST;
    }
}

/*!
    \file  system_gd32e511_512.c
    \brief CMSIS Cortex-M33 Device Peripheral Access Layer Source File for
           GD32E511_512 Device Series
*/

/*
 * Copyright (c) 2009-2018 Arm Limited. All rights reserved.
 * Copyright (c) 2025, GigaDevice Semiconductor Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* This file refers the CMSIS standard, some adjustments are made according to GigaDevice chips */

#include "gd32e511_512.h"

/* system frequency define */
#define __IRC8M           (IRC8M_VALUE)            /* internal 8 MHz RC oscillator frequency */
#define __HXTAL           (HXTAL_VALUE)            /* high speed crystal oscillator frequency */
#define __SYS_OSC_CLK     (__IRC8M)                /* main oscillator frequency */

/* vector table base offset: the APP is linked at the APP_A slot payload
 * base (memory_map.h MEM_APP_A_BASE + the 512B frame header area, see
 * flashresource_impl.h FRAME_PAYLOAD_OFF), so VTOR must point at
 * 0x08010200, not at the flash base. The APP_B build carries its own link
 * base; the bootloader sets SCB->VTOR to the started slot's payload base
 * before jumping -- this value only covers the standalone / debugger path. */
#define VECT_TAB_OFFSET  (uint32_t)0x00010200        /* APP_A payload base - flash base */

/* select a system clock by uncommenting the following line */
//#define __SYSTEM_CLOCK_8M_HXTAL              (__HXTAL)
//#define __SYSTEM_CLOCK_8M_IRC8M              (__IRC8M)
//#define __SYSTEM_CLOCK_24M_PLL_HXTAL         (uint32_t)(24000000)
//#define __SYSTEM_CLOCK_24M_PLL_IRC8M         (uint32_t)(24000000)
//#define __SYSTEM_CLOCK_72M_PLL_HXTAL         (uint32_t)(72000000)
//#define __SYSTEM_CLOCK_72M_PLL_IRC8M_DIV2    (uint32_t)(72000000)
//#define __SYSTEM_CLOCK_100M_PLL_HXTAL         (uint32_t)(100000000)
//#define __SYSTEM_CLOCK_100M_PLL_IRC8M_DIV2    (uint32_t)(100000000)
//#define __SYSTEM_CLOCK_120M_PLL_HXTAL         (uint32_t)(120000000)
#define __SYSTEM_CLOCK_120M_PLL_IRC8M_DIV2    (uint32_t)(120000000)

/* The following is to prevent  Vcore fluctuations caused by frequency switching.
   It is strongly recommended to include it to avoid issues caused by self-removal. */
#define RCU_MODIFY_DE_2(__delay)  do{                                     \
                                      volatile uint32_t i,reg;            \
                                      if(0U != (__delay)){                   \
                                          /* Insert a software delay */   \
                                          for(i=0U; i<(__delay); i++){       \
                                          }                               \
                                          reg = RCU_CFG0;                 \
                                          reg &= ~(RCU_CFG0_AHBPSC);     \
                                          reg |= RCU_AHB_CKSYS_DIV2;     \
                                          /* AHB = SYSCLK/2 */           \
                                          RCU_CFG0 = reg;                \
                                          /* Insert a software delay */  \
                                          for(i=0U; i<(__delay); i++){      \
                                          }                              \
                                          reg = RCU_CFG0;                \
                                          reg &= ~(RCU_CFG0_AHBPSC);     \
                                          reg |= RCU_AHB_CKSYS_DIV4;     \
                                          /* AHB = SYSCLK/4 */           \
                                          RCU_CFG0 = reg;                \
                                          /* Insert a software delay */  \
                                          for(i=0U; i<(__delay); i++){      \
                                          }                              \
                                      }                                  \
                                  }while(0)

                                
/* The following is to prevent Vcore fluctuations caused by frequency switching. 
   It is strongly recommended to include it to avoid issues caused by self-removal. */
#define RCU_MODIFY_UP(__delay)  do{                                     \
                                      volatile uint32_t i,reg;            \
                                      if(0 != __delay){                   \
                                          for(i=0; i<__delay; i++){       \
                                          }                               \
                                          reg = RCU_CFG0;                 \
                                          reg &= ~(RCU_CFG0_AHBPSC);      \
                                          reg |= RCU_AHB_CKSYS_DIV2;      \
                                          RCU_CFG0 = reg;                 \
                                          for(i=0; i<__delay; i++){       \
                                          }                               \
                                          reg = RCU_CFG0;                 \
                                          reg &= ~(RCU_CFG0_AHBPSC);      \
                                          reg |= RCU_AHB_CKSYS_DIV1;      \
                                          RCU_CFG0 = reg;                 \
                                      }                                   \
                                  }while(0)

#define SEL_IRC8M       0x00
#define SEL_HXTAL       0x01
#define SEL_PLL         0x02

/* set the system clock frequency and declare the system clock configuration function */
#ifdef __SYSTEM_CLOCK_8M_HXTAL
uint32_t SystemCoreClock = __SYSTEM_CLOCK_8M_HXTAL;
static void system_clock_8m_hxtal(void);

#elif defined (__SYSTEM_CLOCK_24M_PLL_IRC8M)
uint32_t SystemCoreClock = __SYSTEM_CLOCK_24M_PLL_IRC8M;
static void system_clock_24m_irc8m(void);

#elif defined (__SYSTEM_CLOCK_24M_PLL_HXTAL)
uint32_t SystemCoreClock = __SYSTEM_CLOCK_24M_PLL_HXTAL;
static void system_clock_24m_hxtal(void);

#elif defined (__SYSTEM_CLOCK_72M_PLL_HXTAL)
uint32_t SystemCoreClock = __SYSTEM_CLOCK_72M_PLL_HXTAL;
static void system_clock_72m_hxtal(void);

#elif defined (__SYSTEM_CLOCK_72M_PLL_IRC8M_DIV2)
uint32_t SystemCoreClock = __SYSTEM_CLOCK_72M_PLL_IRC8M_DIV2;
static void system_clock_72m_irc8m(void);

#elif defined (__SYSTEM_CLOCK_100M_PLL_HXTAL)
uint32_t SystemCoreClock = __SYSTEM_CLOCK_100M_PLL_HXTAL;
static void system_clock_100m_hxtal(void);

#elif defined (__SYSTEM_CLOCK_100M_PLL_IRC8M_DIV2)
uint32_t SystemCoreClock = __SYSTEM_CLOCK_100M_PLL_IRC8M_DIV2;
static void system_clock_100m_irc8m(void);

#elif defined (__SYSTEM_CLOCK_120M_PLL_HXTAL)
uint32_t SystemCoreClock = __SYSTEM_CLOCK_120M_PLL_HXTAL;
static void system_clock_120m_hxtal(void);

#elif defined (__SYSTEM_CLOCK_120M_PLL_IRC8M_DIV2)
uint32_t SystemCoreClock = __SYSTEM_CLOCK_120M_PLL_IRC8M_DIV2;
static void system_clock_120m_irc8m(void);

#else
uint32_t SystemCoreClock = __SYSTEM_CLOCK_8M_IRC8M;
static void system_clock_8m_irc8m(void);
#endif /* __SYSTEM_CLOCK_8M_HXTAL */

/* configure the system clock */
static void system_clock_config(void);
/* soft delay */
static void _soft_delay_(uint32_t time);

/* software delay to prevent the impact of Vcore fluctuations.
   It is strongly recommended to include it to avoid issues caused by self-removal. */
static void _soft_delay_(uint32_t time)
{
    __IO uint32_t i;
    for(i = 0U; i < time * 10U; i++) {
    }
}

/*!
    \brief      setup the micro-controller system, initialize the system
    \param[in]  none
    \param[out] none
    \retval     none
    \note       This function contains scenarios leading to an infinite loop.
                Modify according to the user's actual usage scenarios.
*/
void SystemInit(void)
{
    /* FPU settings */
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1U)
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); /* set CP10 and CP11 Full Access */
#endif

    /* enable IRC8M */
    RCU_CTL0 |= RCU_CTL0_IRC8MEN;
    while(0U == (RCU_CTL0 & RCU_CTL0_IRC8MSTB)) {
    }

    if(((RCU_CFG0 & RCU_CFG0_SCSS) == RCU_SCSS_PLL)) {
        RCU_MODIFY_DE_2(0x50U);
    }

    RCU_CFG0 &= ~RCU_CFG0_SCS;
    _soft_delay_(10U);

    /* reset HXTALEN, CKMEN, PLLEN, HXTALBPS bits */
    RCU_CTL0 &= ~(RCU_CTL0_HXTALEN | RCU_CTL0_CKMEN | RCU_CTL0_PLLEN | RCU_CTL0_HXTALBPS);

    /* reset RCU */
    RCU_CFG0 &= ~(RCU_CFG0_SCS | RCU_CFG0_AHBPSC | RCU_CFG0_APB1PSC | RCU_CFG0_APB2PSC | \
                  RCU_CFG0_ADC0PSC | RCU_CFG0_CKOUTSEL | RCU_CFG0_CKOUTDIV | RCU_CFG0_PLLDV);
    RCU_CFG0 &= ~(RCU_CFG0_PLLSEL | RCU_CFG0_PLLPREDV | RCU_CFG0_PLLMF);
    RCU_CTL0 &= ~(RCU_CTL0_HXTALEN | RCU_CTL0_CKMEN | RCU_CTL0_PLLEN | RCU_CTL0_HXTALBPS);
    RCU_CFG1 &= ~(RCU_CFG1_PREDV);
    RCU_CFG2 &= ~(RCU_CFG2_USART0SEL | RCU_CFG2_I2C0SEL | RCU_CFG2_I2C1SEL | RCU_CFG2_I2C2SEL | RCU_CFG2_ADC0SEL | RCU_CFG2_ADC1SEL);
    RCU_CFG2 &= ~(RCU_CFG2_ADC0_IRC80MPSC | RCU_CFG2_ADC1_IRC80MPSC);
    RCU_CFG2 &= ~(RCU_CFG2_ADC0PSC2 | RCU_CFG2_ADC1PSC);
    RCU_CTL1 &= ~RCU_CTL1_IRC80MEN;
    RCU_INT = 0x00000000U;

    /* configure system clock */
    system_clock_config();

#ifdef VECT_TAB_SRAM
    nvic_vector_table_set(NVIC_VECTTAB_RAM, VECT_TAB_OFFSET);
#else
    nvic_vector_table_set(NVIC_VECTTAB_FLASH, VECT_TAB_OFFSET);
#endif
}

/*!
    \brief      configure the system clock
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void system_clock_config(void)
{
#ifdef __SYSTEM_CLOCK_8M_HXTAL
    system_clock_8m_hxtal();
#elif defined (__SYSTEM_CLOCK_24M_PLL_IRC8M)
    system_clock_24m_irc8m();
#elif defined (__SYSTEM_CLOCK_24M_PLL_HXTAL)
    system_clock_24m_hxtal();
#elif defined (__SYSTEM_CLOCK_72M_PLL_HXTAL)
    system_clock_72m_hxtal();
#elif defined (__SYSTEM_CLOCK_72M_PLL_IRC8M_DIV2)
    system_clock_72m_irc8m();
#elif defined (__SYSTEM_CLOCK_100M_PLL_HXTAL)
    system_clock_100m_hxtal();
#elif defined (__SYSTEM_CLOCK_100M_PLL_IRC8M_DIV2)
    system_clock_100m_irc8m();
#elif defined (__SYSTEM_CLOCK_120M_PLL_HXTAL)
    system_clock_120m_hxtal();
#elif defined (__SYSTEM_CLOCK_120M_PLL_IRC8M_DIV2)
    system_clock_120m_irc8m();
#else
    system_clock_8m_irc8m();
#endif /* __SYSTEM_CLOCK_8M_HXTAL */
}

#ifdef __SYSTEM_CLOCK_8M_HXTAL
/*!
    \brief      configure the system clock to 8M by HXTAL
    \param[in]  none
    \param[out] none
    \retval     none
    \note       This function includes timeout exit scenarios.
                Modify according to the user's actual usage scenarios.
                This function contains scenarios leading to an infinite loop.
                Modify according to the user's actual usage scenarios.
*/
static void system_clock_8m_hxtal(void)
{
    uint32_t timeout = 0U;
    uint32_t stab_flag = 0U;
    __IO uint32_t reg_temp;

    /* enable HXTAL */
    RCU_CTL0 |= RCU_CTL0_HXTALEN;

    /* wait until HXTAL is stable or the startup time is longer than HXTAL_STARTUP_TIMEOUT */
    do {
        timeout++;
        stab_flag = (RCU_CTL0 & RCU_CTL0_HXTALSTB);
    } while((0U == stab_flag) && (HXTAL_STARTUP_TIMEOUT != timeout));
    /* if fail */
    if(0U == (RCU_CTL0 & RCU_CTL0_HXTALSTB)) {
        while(1) {
        }
    }

    FMC_WS = (FMC_WS & (~FMC_WS_WSCNT)) | WS_WSCNT_0;

    /* HXTAL is stable */
    /* AHB = SYSCLK */
    RCU_CFG0 |= RCU_AHB_CKSYS_DIV1;
    /* APB2 = AHB */
    RCU_CFG0 |= RCU_APB2_CKAHB_DIV1;
    /* APB1 = AHB */
    RCU_CFG0 |= RCU_APB1_CKAHB_DIV1;

    reg_temp = RCU_CFG0;
    /* select HXTAL as system clock */
    reg_temp &= ~RCU_CFG0_SCS;
    reg_temp |= RCU_CKSYSSRC_HXTAL;
    RCU_CFG0 = reg_temp;

    /* wait until HXTAL is selected as system clock */
    while(RCU_SCSS_HXTAL != (RCU_CFG0 & RCU_CFG0_SCSS)) {
    }
}
#elif defined (__SYSTEM_CLOCK_24M_PLL_IRC8M)
/*!
    \brief      configure the system clock to 24M by IRC8M
    \param[in]  none
    \param[out] none
    \retval     none
    \note       This function includes timeout exit scenarios.
                Modify according to the user's actual usage scenarios.
                This function contains scenarios leading to an infinite loop.
                Modify according to the user's actual usage scenarios.
*/
static void system_clock_24m_irc8m(void)
{
    uint32_t timeout = 0U;
    uint32_t stab_flag = 0U;
    __IO uint32_t reg_temp;

    /* enable IRC8M */
    RCU_CTL0 |= RCU_CTL0_IRC8MEN;

    /* wait until IRC8M is stable or the startup time is longer than IRC8M_STARTUP_TIMEOUT */
    do {
        timeout++;
        stab_flag = (RCU_CTL0 & RCU_CTL0_IRC8MSTB);
    } while((0U == stab_flag) && (IRC8M_STARTUP_TIMEOUT != timeout));

    /* if fail */
    if(0U == (RCU_CTL0 & RCU_CTL0_IRC8MSTB)) {
        while(1) {
        }
    }
#ifndef VECT_TAB_SRAM
    FMC_WS = (FMC_WS & (~FMC_WS_WSCNT)) | WS_WSCNT_1;
#endif
    /* AHB = SYSCLK/4 */
    RCU_CFG0 |= RCU_AHB_CKSYS_DIV4;
    /* APB2 = AHB */
    RCU_CFG0 |= RCU_APB2_CKAHB_DIV1;
    /* APB1 = AHB */
    RCU_CFG0 |= RCU_APB1_CKAHB_DIV1;
    /* PLL = (IRC8M/2) * 6 = 24 MHz */
    RCU_CFG0 &= ~(RCU_CFG0_PLLSEL | RCU_CFG0_PLLMF);
    RCU_CFG0 |= (RCU_PLLSRC_IRC8M_DIV2 | RCU_PLL_MUL6);

    /* enable PLL */
    RCU_CTL0 |= RCU_CTL0_PLLEN;

    /* wait until PLL is stable */
    while(0U == (RCU_CTL0 & RCU_CTL0_PLLSTB)) {
    }

    reg_temp = RCU_CFG0;
    /* select PLL as system clock */
    reg_temp &= ~RCU_CFG0_SCS;
    reg_temp |= RCU_CKSYSSRC_PLL;
    RCU_CFG0 = reg_temp;

    /* wait until PLL is selected as system clock */
    while(RCU_SCSS_PLL != (RCU_CFG0 & RCU_CFG0_SCSS)) {
    }
    RCU_MODIFY_UP(0x50);
}
#elif defined (__SYSTEM_CLOCK_24M_PLL_HXTAL)
/*!
    \brief      configure the system clock to 24M by PLL which selects HXTAL as its clock source
    \param[in]  none
    \param[out] none
    \retval     none
    \note       This function includes timeout exit scenarios.
                Modify according to the user's actual usage scenarios.
                This function contains scenarios leading to an infinite loop.
                Modify according to the user's actual usage scenarios.
*/
static void system_clock_24m_hxtal(void)
{
    uint32_t timeout = 0U;
    uint32_t stab_flag = 0U;
    __IO uint32_t reg_temp;

    /* enable HXTAL */
    RCU_CTL0 |= RCU_CTL0_HXTALEN;

    /* wait until HXTAL is stable or the startup time is longer than HXTAL_STARTUP_TIMEOUT */
    do {
        timeout++;
        stab_flag = (RCU_CTL0 & RCU_CTL0_HXTALSTB);
    } while((0U == stab_flag) && (HXTAL_STARTUP_TIMEOUT != timeout));
    /* if fail */
    if(0U == (RCU_CTL0 & RCU_CTL0_HXTALSTB)) {
        while(1) {
        }
    }
#ifndef VECT_TAB_SRAM
    FMC_WS = (FMC_WS & (~FMC_WS_WSCNT)) | WS_WSCNT_1;
#endif
    /* HXTAL is stable */
    /* AHB = SYSCLK/4 */
    RCU_CFG0 |= RCU_AHB_CKSYS_DIV4;
    /* APB2 = AHB */
    RCU_CFG0 |= RCU_APB2_CKAHB_DIV1;
    /* APB1 = AHB */
    RCU_CFG0 |= RCU_APB1_CKAHB_DIV1;

    /* PLL = HXTAL / 2 * 6 = 24 MHz */
    RCU_CFG1 &= ~RCU_CFG1_PREDV;
    RCU_CFG1 |= RCU_PLL_PREDV2;
    RCU_CFG0 &= ~(RCU_CFG0_PLLSEL | RCU_CFG0_PLLMF | RCU_CFG0_PLLDV);
    RCU_CFG0 |= (RCU_PLLSRC_HXTAL | RCU_PLL_MUL6);

    /* enable PLL */
    RCU_CTL0 |= RCU_CTL0_PLLEN;

    /* wait until PLL is stable */
    while(0U == (RCU_CTL0 & RCU_CTL0_PLLSTB)) {
    }

    reg_temp = RCU_CFG0;
    /* select PLL as system clock */
    reg_temp &= ~RCU_CFG0_SCS;
    reg_temp |= RCU_CKSYSSRC_PLL;
    RCU_CFG0 = reg_temp;

    /* wait until PLL is selected as system clock */
    while(RCU_SCSS_PLL != (RCU_CFG0 & RCU_CFG0_SCSS)) {
    }
    RCU_MODIFY_UP(0x50);
}
#elif defined (__SYSTEM_CLOCK_72M_PLL_HXTAL)
/*!
    \brief      configure the system clock to 72M by PLL which selects HXTAL as its clock source
    \param[in]  none
    \param[out] none
    \retval     none
    \note       This function includes timeout exit scenarios.
                Modify according to the user's actual usage scenarios.
                This function contains scenarios leading to an infinite loop.
                Modify according to the user's actual usage scenarios.
*/
static void system_clock_72m_hxtal(void)
{
    uint32_t timeout = 0U;
    uint32_t stab_flag = 0U;
    __IO uint32_t reg_temp;

    /* enable HXTAL */
    RCU_CTL0 |= RCU_CTL0_HXTALEN;

    /* wait until HXTAL is stable or the startup time is longer than HXTAL_STARTUP_TIMEOUT */
    do {
        timeout++;
        stab_flag = (RCU_CTL0 & RCU_CTL0_HXTALSTB);
    } while((0U == stab_flag) && (HXTAL_STARTUP_TIMEOUT != timeout));
    /* if fail */
    if(0U == (RCU_CTL0 & RCU_CTL0_HXTALSTB)) {
        while(1) {
        }
    }

    FMC_WS = (FMC_WS & (~FMC_WS_WSCNT)) | WS_WSCNT_3;

    /* HXTAL is stable */
    /* AHB = SYSCLK/4 */
    RCU_CFG0 |= RCU_AHB_CKSYS_DIV4;
    /* APB2 = AHB */
    RCU_CFG0 |= RCU_APB2_CKAHB_DIV1;
    /* APB1 = AHB */
    RCU_CFG0 |= RCU_APB1_CKAHB_DIV1;

    /* PLL = HXTAL / 2 * 18 = 72 MHz */
    RCU_CFG1 &= ~RCU_CFG1_PREDV;
    RCU_CFG1 |= RCU_PLL_PREDV2;
    RCU_CFG0 &= ~(RCU_CFG0_PLLSEL | RCU_CFG0_PLLMF | RCU_CFG0_PLLDV);
    RCU_CFG0 |= (RCU_PLLSRC_HXTAL | RCU_PLL_MUL18);

    /* enable PLL */
    RCU_CTL0 |= RCU_CTL0_PLLEN;

    /* wait until PLL is stable */
    while(0U == (RCU_CTL0 & RCU_CTL0_PLLSTB)) {
    }

    reg_temp = RCU_CFG0;
    /* select PLL as system clock */
    reg_temp &= ~RCU_CFG0_SCS;
    reg_temp |= RCU_CKSYSSRC_PLL;
    RCU_CFG0 = reg_temp;

    /* wait until PLL is selected as system clock */
    while(RCU_SCSS_PLL != (RCU_CFG0 & RCU_CFG0_SCSS)) {
    }
    RCU_MODIFY_UP(0x50);
}

#elif defined (__SYSTEM_CLOCK_72M_PLL_IRC8M_DIV2)
/*!
    \brief      configure the system clock to 72M by PLL which selects IRC8M/2 as its clock source
    \param[in]  none
    \param[out] none
    \retval     none
    \note       This function includes timeout exit scenarios.
                Modify according to the user's actual usage scenarios.
                This function contains scenarios leading to an infinite loop.
                Modify according to the user's actual usage scenarios.
*/
static void system_clock_72m_irc8m(void)
{
    uint32_t timeout = 0U;
    uint32_t stab_flag = 0U;
    __IO uint32_t reg_temp;

    /* enable IRC8M */
    RCU_CTL0 |= RCU_CTL0_IRC8MEN;

    /* wait until IRC8M is stable or the startup time is longer than IRC8M_STARTUP_TIMEOUT */
    do {
        timeout++;
        stab_flag = (RCU_CTL0 & RCU_CTL0_IRC8MSTB);
    } while((0U == stab_flag) && (IRC8M_STARTUP_TIMEOUT != timeout));

    /* if fail */
    if(0U == (RCU_CTL0 & RCU_CTL0_IRC8MSTB)) {
        while(1) {
        }
    }

    FMC_WS = (FMC_WS & (~FMC_WS_WSCNT)) | WS_WSCNT_3;

    /* AHB = SYSCLK/4 */
    RCU_CFG0 |= RCU_AHB_CKSYS_DIV4;
    /* APB2 = AHB */
    RCU_CFG0 |= RCU_APB2_CKAHB_DIV1;
    /* APB1 = AHB */
    RCU_CFG0 |= RCU_APB1_CKAHB_DIV1;
    /* PLL = (IRC8M/2) * 18 = 72 MHz */
    RCU_CFG0 &= ~(RCU_CFG0_PLLSEL | RCU_CFG0_PLLMF);
    RCU_CFG0 |= (RCU_PLLSRC_IRC8M_DIV2 | RCU_PLL_MUL18);

    /* enable PLL */
    RCU_CTL0 |= RCU_CTL0_PLLEN;

    /* wait until PLL is stable */
    while(0U == (RCU_CTL0 & RCU_CTL0_PLLSTB)) {
    }

    reg_temp = RCU_CFG0;
    /* select PLL as system clock */
    reg_temp &= ~RCU_CFG0_SCS;
    reg_temp |= RCU_CKSYSSRC_PLL;
    RCU_CFG0 = reg_temp;

    /* wait until PLL is selected as system clock */
    while(RCU_SCSS_PLL != (RCU_CFG0 & RCU_CFG0_SCSS)) {
    }
    RCU_MODIFY_UP(0x50);
}
#elif defined (__SYSTEM_CLOCK_100M_PLL_HXTAL)
/*!
    \brief      configure the system clock to 100M by PLL which selects HXTAL as its clock source
    \param[in]  none
    \param[out] none
    \retval     none
    \note       This function includes timeout exit scenarios.
                Modify according to the user's actual usage scenarios.
                This function contains scenarios leading to an infinite loop.
                Modify according to the user's actual usage scenarios.
*/
static void system_clock_100m_hxtal(void)
{
    uint32_t timeout = 0U;
    uint32_t stab_flag = 0U;
    __IO uint32_t reg_temp;

    /* enable HXTAL */
    RCU_CTL0 |= RCU_CTL0_HXTALEN;

    /* wait until HXTAL is stable or the startup time is longer than HXTAL_STARTUP_TIMEOUT */
    do {
        timeout++;
        stab_flag = (RCU_CTL0 & RCU_CTL0_HXTALSTB);
    } while((0U == stab_flag) && (HXTAL_STARTUP_TIMEOUT != timeout));
    /* if fail */
    if(0U == (RCU_CTL0 & RCU_CTL0_HXTALSTB)) {
        while(1) {
        }
    }

    FMC_WS = (FMC_WS & (~FMC_WS_WSCNT)) | WS_WSCNT_4;

    /* HXTAL is stable */
    /* AHB = SYSCLK/4 */
    RCU_CFG0 |= RCU_AHB_CKSYS_DIV4;
    /* APB2 = AHB */
    RCU_CFG0 |= RCU_APB2_CKAHB_DIV1;
    /* APB1 = AHB */
    RCU_CFG0 |= RCU_APB1_CKAHB_DIV1;

    /* PLL = HXTAL / 2 * 25 = 100 MHz */
    RCU_CFG1 &= ~RCU_CFG1_PREDV;
    RCU_CFG1 |= RCU_PLL_PREDV2;
    RCU_CFG0 &= ~(RCU_CFG0_PLLSEL | RCU_CFG0_PLLMF | RCU_CFG0_PLLDV);
    RCU_CFG0 |= (RCU_PLLSRC_HXTAL | RCU_PLL_MUL25);

    /* enable PLL */
    RCU_CTL0 |= RCU_CTL0_PLLEN;

    /* wait until PLL is stable */
    while(0U == (RCU_CTL0 & RCU_CTL0_PLLSTB)) {
    }

    reg_temp = RCU_CFG0;
    /* select PLL as system clock */
    reg_temp &= ~RCU_CFG0_SCS;
    reg_temp |= RCU_CKSYSSRC_PLL;
    RCU_CFG0 = reg_temp;

    /* wait until PLL is selected as system clock */
    while(RCU_SCSS_PLL != (RCU_CFG0 & RCU_CFG0_SCSS)) {
    }
    RCU_MODIFY_UP(0x50);
}

#elif defined (__SYSTEM_CLOCK_100M_PLL_IRC8M_DIV2)
/*!
    \brief      configure the system clock to 72M by PLL which selects IRC8M/2 as its clock source
    \param[in]  none
    \param[out] none
    \retval     none
    \note       This function includes timeout exit scenarios.
                Modify according to the user's actual usage scenarios.
                This function contains scenarios leading to an infinite loop.
                Modify according to the user's actual usage scenarios.
*/
static void system_clock_100m_irc8m(void)
{
    uint32_t timeout = 0U;
    uint32_t stab_flag = 0U;
    __IO uint32_t reg_temp;

    /* enable IRC8M */
    RCU_CTL0 |= RCU_CTL0_IRC8MEN;

    /* wait until IRC8M is stable or the startup time is longer than IRC8M_STARTUP_TIMEOUT */
    do {
        timeout++;
        stab_flag = (RCU_CTL0 & RCU_CTL0_IRC8MSTB);
    } while((0U == stab_flag) && (IRC8M_STARTUP_TIMEOUT != timeout));

    /* if fail */
    if(0U == (RCU_CTL0 & RCU_CTL0_IRC8MSTB)) {
        while(1) {
        }
    }

    FMC_WS = (FMC_WS & (~FMC_WS_WSCNT)) | WS_WSCNT_4;

    /* AHB = SYSCLK/4 */
    RCU_CFG0 |= RCU_AHB_CKSYS_DIV4;
    /* APB2 = AHB */
    RCU_CFG0 |= RCU_APB2_CKAHB_DIV1;
    /* APB1 = AHB */
    RCU_CFG0 |= RCU_APB1_CKAHB_DIV1;
    /* PLL = (IRC8M/2) * 25 = 100 MHz */
    RCU_CFG0 &= ~(RCU_CFG0_PLLSEL | RCU_CFG0_PLLMF);
    RCU_CFG0 |= (RCU_PLLSRC_IRC8M_DIV2 | RCU_PLL_MUL25);

    /* enable PLL */
    RCU_CTL0 |= RCU_CTL0_PLLEN;

    /* wait until PLL is stable */
    while(0U == (RCU_CTL0 & RCU_CTL0_PLLSTB)) {
    }

    reg_temp = RCU_CFG0;
    /* select PLL as system clock */
    reg_temp &= ~RCU_CFG0_SCS;
    reg_temp |= RCU_CKSYSSRC_PLL;
    RCU_CFG0 = reg_temp;

    /* wait until PLL is selected as system clock */
    while(RCU_SCSS_PLL != (RCU_CFG0 & RCU_CFG0_SCSS)) {
    }
    RCU_MODIFY_UP(0x50);
}
#elif defined (__SYSTEM_CLOCK_120M_PLL_HXTAL)
/*!
    \brief      configure the system clock to 120M by PLL which selects HXTAL as its clock source
    \param[in]  none
    \param[out] none
    \retval     none
    \note       This function includes timeout exit scenarios.
                Modify according to the user's actual usage scenarios.
                This function contains scenarios leading to an infinite loop.
                Modify according to the user's actual usage scenarios.
*/
static void system_clock_120m_hxtal(void)
{
    uint32_t timeout = 0U;
    uint32_t stab_flag = 0U;
    __IO uint32_t reg_temp;

    /* enable HXTAL */
    RCU_CTL0 |= RCU_CTL0_HXTALEN;

    /* wait until HXTAL is stable or the startup time is longer than HXTAL_STARTUP_TIMEOUT */
    do {
        timeout++;
        stab_flag = (RCU_CTL0 & RCU_CTL0_HXTALSTB);
    } while((0U == stab_flag) && (HXTAL_STARTUP_TIMEOUT != timeout));
    /* if fail */
    if(0U == (RCU_CTL0 & RCU_CTL0_HXTALSTB)) {
        while(1) {
        }
    }

    FMC_WS = (FMC_WS & (~FMC_WS_WSCNT)) | WS_WSCNT_4;

    /* HXTAL is stable */
    /* AHB = SYSCLK/4 */
    RCU_CFG0 |= RCU_AHB_CKSYS_DIV4;
    /* APB2 = AHB */
    RCU_CFG0 |= RCU_APB2_CKAHB_DIV1;
    /* APB1 = AHB */
    RCU_CFG0 |= RCU_APB1_CKAHB_DIV1;

    /* PLL = HXTAL / 2 * 30 = 120 MHz */
    RCU_CFG1 &= ~RCU_CFG1_PREDV;
    RCU_CFG1 |= RCU_PLL_PREDV2;
    RCU_CFG0 &= ~(RCU_CFG0_PLLSEL | RCU_CFG0_PLLMF | RCU_CFG0_PLLDV);
    RCU_CFG0 |= (RCU_PLLSRC_HXTAL | RCU_PLL_MUL30);

    /* enable PLL */
    RCU_CTL0 |= RCU_CTL0_PLLEN;

    /* wait until PLL is stable */
    while(0U == (RCU_CTL0 & RCU_CTL0_PLLSTB)) {
    }

    reg_temp = RCU_CFG0;
    /* select PLL as system clock */
    reg_temp &= ~RCU_CFG0_SCS;
    reg_temp |= RCU_CKSYSSRC_PLL;
    RCU_CFG0 = reg_temp;

    /* wait until PLL is selected as system clock */
    while(RCU_SCSS_PLL != (RCU_CFG0 & RCU_CFG0_SCSS)) {
    }
    RCU_MODIFY_UP(0x50);
}

#elif defined (__SYSTEM_CLOCK_120M_PLL_IRC8M_DIV2)
/*!
    \brief      configure the system clock to 120M by PLL which selects IRC8M/2 as its clock source
    \param[in]  none
    \param[out] none
    \retval     none
    \note       This function includes timeout exit scenarios.
                Modify according to the user's actual usage scenarios.
                This function contains scenarios leading to an infinite loop.
                Modify according to the user's actual usage scenarios.
*/
static void system_clock_120m_irc8m(void)
{
    uint32_t timeout = 0U;
    uint32_t stab_flag = 0U;
    __IO uint32_t reg_temp;

    /* enable IRC8M */
    RCU_CTL0 |= RCU_CTL0_IRC8MEN;

    /* wait until IRC8M is stable or the startup time is longer than IRC8M_STARTUP_TIMEOUT */
    do {
        timeout++;
        stab_flag = (RCU_CTL0 & RCU_CTL0_IRC8MSTB);
    } while((0U == stab_flag) && (IRC8M_STARTUP_TIMEOUT != timeout));

    /* if fail */
    if(0U == (RCU_CTL0 & RCU_CTL0_IRC8MSTB)) {
        while(1) {
        }
    }

    FMC_WS = (FMC_WS & (~FMC_WS_WSCNT)) | WS_WSCNT_4;

    /* AHB = SYSCLK/4 */
    RCU_CFG0 |= RCU_AHB_CKSYS_DIV4;
    /* APB2 = AHB */
    RCU_CFG0 |= RCU_APB2_CKAHB_DIV1;
    /* APB1 = AHB */
    RCU_CFG0 |= RCU_APB1_CKAHB_DIV1;
    /* PLL = (IRC8M/2) * 30 = 120 MHz */
    RCU_CFG0 &= ~(RCU_CFG0_PLLSEL | RCU_CFG0_PLLMF);
    RCU_CFG0 |= (RCU_PLLSRC_IRC8M_DIV2 | RCU_PLL_MUL30);

    /* enable PLL */
    RCU_CTL0 |= RCU_CTL0_PLLEN;

    /* wait until PLL is stable */
    while(0U == (RCU_CTL0 & RCU_CTL0_PLLSTB)) {
    }

    reg_temp = RCU_CFG0;
    /* select PLL as system clock */
    reg_temp &= ~RCU_CFG0_SCS;
    reg_temp |= RCU_CKSYSSRC_PLL;
    RCU_CFG0 = reg_temp;

    /* wait until PLL is selected as system clock */
    while(RCU_SCSS_PLL != (RCU_CFG0 & RCU_CFG0_SCSS)) {
    }
    RCU_MODIFY_UP(0x50);
}
#else
/*!
    \brief      configure the system clock to 8M by IRC8M
    \param[in]  none
    \param[out] none
    \retval     none
    \note       This function includes timeout exit scenarios.
                Modify according to the user's actual usage scenarios.
                This function contains scenarios leading to an infinite loop.
                Modify according to the user's actual usage scenarios.
*/
static void system_clock_8m_irc8m(void)
{
    uint32_t timeout = 0U;
    uint32_t stab_flag = 0U;
    __IO uint32_t reg_temp;

    /* enable IRC8M */
    RCU_CTL0 |= RCU_CTL0_IRC8MEN;

    /* wait until IRC8M is stable or the startup time is longer than IRC8M_STARTUP_TIMEOUT */
    do {
        timeout++;
        stab_flag = (RCU_CTL0 & RCU_CTL0_IRC8MSTB);
    } while((0U == stab_flag) && (IRC8M_STARTUP_TIMEOUT != timeout));

    /* if fail */
    if(0U == (RCU_CTL0 & RCU_CTL0_IRC8MSTB)) {
        while(1) {
        }
    }

    FMC_WS = (FMC_WS & (~FMC_WS_WSCNT)) | WS_WSCNT_0;

    /* AHB = SYSCLK */
    RCU_CFG0 |= RCU_AHB_CKSYS_DIV1;
    /* APB2 = AHB */
    RCU_CFG0 |= RCU_APB2_CKAHB_DIV1;
    /* APB1 = AHB */
    RCU_CFG0 |= RCU_APB1_CKAHB_DIV1;

    reg_temp = RCU_CFG0;
    /* select IRC8M as system clock */
    reg_temp &= ~RCU_CFG0_SCS;
    reg_temp |= RCU_CKSYSSRC_IRC8M;
    RCU_CFG0 = reg_temp;

    /* wait until IRC8M is selected as system clock */
    while(RCU_SCSS_IRC8M != (RCU_CFG0 & RCU_CFG0_SCSS)) {
    }
}
#endif /* __SYSTEM_CLOCK_8M_HXTAL */

/*!
    \brief      update the SystemCoreClock with current core clock retrieved from CPU registers
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SystemCoreClockUpdate(void)
{
    uint32_t sws = 0U;
    uint32_t pllmf = 0U, pllmf4 = 0U, pllsel = 0U, idx = 0U, clk_exp = 0U;
    /* exponent of AHB clock divider */
    const uint8_t ahb_exp[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};

    sws = GET_BITS(RCU_CFG0, 2, 3);
    switch(sws) {
    /* IRC8M is selected as CK_SYS */
    case SEL_IRC8M:
        SystemCoreClock = IRC8M_VALUE;
        break;
    /* HXTAL is selected as CK_SYS */
    case SEL_HXTAL:
        SystemCoreClock = HXTAL_VALUE;
        break;
    /* PLL is selected as CK_SYS */
    case SEL_PLL:
        /* get the value of PLLMF[3:0] */
        pllmf = GET_BITS(RCU_CFG0, 18, 21);
        pllmf4 = GET_BITS(RCU_CFG0, 27, 27);
        /* high 16 bits */
        if(1U == pllmf4) {
            pllmf += 17U;
        } else if(15U == pllmf) {
            pllmf = 16U;
        } else {
            pllmf += 2U;
        }

        /* PLL clock source selection, HXTAL or IRC8M/2 */
        pllsel = GET_BITS(RCU_CFG0, 16, 16);
        if(0U != pllsel) {
            uint32_t prediv = (GET_BITS(RCU_CFG1, 0, 3) + 1U);
            SystemCoreClock = (HXTAL_VALUE / prediv) * pllmf;
        } else {
            SystemCoreClock = (IRC8M_VALUE >> 1) * pllmf;
        }
        break;
    /* IRC8M is selected as CK_SYS */
    default:
        SystemCoreClock = IRC8M_VALUE;
        break;
    }
    /* calculate AHB clock frequency */
    idx = GET_BITS(RCU_CFG0, 4, 7);
    clk_exp = ahb_exp[idx];
    SystemCoreClock >>= clk_exp;
}

#ifdef __FIRMWARE_VERSION_DEFINE
/*!
    \brief      get firmware version
    \param[in]  none
    \param[out] none
    \retval     firmware version
*/
uint32_t gd32e511_512_firmware_version_get(void)
{
    return (uint32_t)__GD32E511_512_STDPERIPH_VERSION;
}
#endif /* __FIRMWARE_VERSION_DEFINE */

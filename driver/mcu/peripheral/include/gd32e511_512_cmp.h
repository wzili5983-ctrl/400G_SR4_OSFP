/*!
    \file    gd32e511_512_cmp.h
    \brief   definitions for the CMP

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

#ifndef GD32E511_512_CMP_H
#define GD32E511_512_CMP_H

#include "gd32e511_512.h"

/* CMP definitions */
#define CMP                                      CMP_BASE                       /*!< CMP base address */

/* registers definitions */
#define CMP_CS0                                  REG32((CMP) + 0x0000001CU)     /*!< CMP control and status register */
#define CMP_CS1                                  REG32((CMP) + 0x00000024U)     /*!< CMP control and status register */

/* bits definitions */
/* CMP_CS */
#define CMP_CS0_CMP0EN                           BIT(0)                         /*!< CMP0 enable */
#define CMP_CS0_CMP0SW                           BIT(1)                         /*!< CMP switch mode enable */
#define CMP_CS0_CMP0M                            BITS(2,3)                      /*!< CMP0 mode */
#define CMP_CS0_CMP0MSEL                         BITS(4,6)                      /*!< CMP_IM input selection */
#define CMP_CS0_CMP0OSEL                         BITS(8,10)                     /*!< CMP0 output selection */
#define CMP_CS0_CMP0PL                           BIT(11)                        /*!< CMP0 output polarity */
#define CMP_CS0_CMP0HST                          BITS(12,13)                    /*!< CMP0 hysteresis */
#define CMP_CS0_CMP0O                            BIT(14)                        /*!< CMP0 output state bit */
#define CMP_CS0_CMP0LK                           BIT(15)                        /*!< CMP0 lock */
#define CMP_CS0_CMP1EN                           BIT(16)                        /*!< CMP1 enable */
#define CMP_CS0_CMP1M                            BITS(18,19)                    /*!< CMP1 mode */
#define CMP_CS0_CMP1MSEL                         BITS(20,22)                    /*!< CMP_IM input selection */
#define CMP_CS0_WNDEN                            BIT(23)                        /*!< CMP window mode enable */
#define CMP_CS0_CMP1OSEL                         BITS(24,26)                    /*!< CMP1 output selection */
#define CMP_CS0_CMP1PL                           BIT(27)                        /*!< CMP1 output polarity */
#define CMP_CS0_CMP1HST                          BITS(28,29)                    /*!< CMP1 hysteresis */
#define CMP_CS0_CMP1O                            BIT(30)                        /*!< CMP1 output state bit */
#define CMP_CS0_CMP1LK                           BIT(31)                        /*!< CMP1 lock */

/* bits definitions, 3239H new added functions */
/* CMP_CS_2 */
#define CMP_CS1_CMP0MSEL                        BIT(10)                         /*!< CMP_IM input selection */
#define CMP_CS1_CMP0BEN                         BIT(12)                         /*!< resistance bridge enable */
#define CMP_CS1_CMP0PSEL                        BITS(14,15)                     /*!< CMP_IP input selection */
#define CMP_CS1_CMP1MSEL                        BIT(26)                         /*!< CMP_IM input selection */
#define CMP_CS1_CMP1BEN                         BIT(28)                         /*!< resistance bridge enable */
#define CMP_CS1_CMP1PSEL                        BITS(30,31)                     /*!< CMP_IP input selection */


/* constants definitions */
/* CMP units */
typedef enum{
    CMP0,                                                                       /*!< comparator 0 */
    CMP1                                                                        /*!< comparator 1 */
}cmp_enum;

/* CMP operating mode */
#define CS0_CMPXM(regval)                        (BITS(2,3) & ((uint32_t)(regval) << 2U))
#define CMP_MODE_HIGHSPEED                       CS0_CMPXM(0)                   /*!< CMP mode high speed */
#define CMP_MODE_MIDDLESPEED                     CS0_CMPXM(1)                   /*!< CMP mode middle speed */
#define CMP_MODE_LOWSPEED                        CS0_CMPXM(2)                   /*!< CMP mode low speed */
#define CMP_MODE_VERYLOWSPEED                    CS0_CMPXM(3)                   /*!< CMP mode very low speed */

/* CMP hysteresis */
#define CS0_CMPXHST(regval)                      (BITS(12,13) & ((uint32_t)(regval) << 12U))
#define CMP_HYSTERESIS_NO                        CS0_CMPXHST(0)                 /*!< CMP output no hysteresis */
#define CMP_HYSTERESIS_LOW                       CS0_CMPXHST(1)                 /*!< CMP output low hysteresis */
#define CMP_HYSTERESIS_MIDDLE                    CS0_CMPXHST(2)                 /*!< CMP output middle hysteresis */
#define CMP_HYSTERESIS_HIGH                      CS0_CMPXHST(3)                 /*!< CMP output high hysteresis */

/* CMP inverting input */
#define CMPXMSEL(regval)                         ((BITS(4,6) & ((uint32_t)(regval) << 4U)) | (BIT(10) & ((uint32_t)(regval) << 7U)))
#define CMP_INVERTING_INPUT_1_4VREFINT           CMPXMSEL(0)                    /*!< CMP inverting input 1/4 Vrefint */
#define CMP_INVERTING_INPUT_1_2VREFINT           CMPXMSEL(1)                    /*!< CMP inverting input 1/2 Vrefint */
#define CMP_INVERTING_INPUT_3_4VREFINT           CMPXMSEL(2)                    /*!< CMP inverting input 3/4 Vrefint */
#define CMP_INVERTING_INPUT_VREFINT              CMPXMSEL(3)                    /*!< CMP inverting input Vrefint */
#define CMP_INVERTING_INPUT_PA4_PA6              CMPXMSEL(4)                    /*!< CMP inverting input PA4(DAC0_OUT0) for CMP0 or PA6(DAC1_OUT0) for CMP1 */
#define CMP_INVERTING_INPUT_PA5_PA7              CMPXMSEL(5)                    /*!< CMP inverting input PA5(DAC0_OUT1) for CMP0 or PA7(DAC1_OUT1) for CMP1 */
#define CMP_INVERTING_INPUT_PA0_PA2              CMPXMSEL(6)                    /*!< CMP inverting input PA0 for CMP0 or PA2 for CMP1 */
#define CMP_INVERTING_INPUT_PA2_DAC0_OUT0        CMPXMSEL(7)                    /*!< CMP inverting input PA2 for CMP0 or DAC0_OUT0 for CMP1*/
#define CMP_INVERTING_INPUT_DAC0_OUT0_OUT1       CMPXMSEL(8)                    /*!< CMP inverting input DAC0_OUT0 for CMP0 or DAC0_OUT1 for CMP1*/
#define CMP_INVERTING_INPUT_DAC0_OUT1_PB3        CMPXMSEL(9)                    /*!< CMP inverting input DAC0_OUT1 for CMP0 or PB3 for CMP1*/
#define CMP_INVERTING_INPUT_PB1_PB7              CMPXMSEL(10)                   /*!< CMP inverting input PB1 for CMP0 or PB7 for CMP1*/
#define CMP_INVERTING_INPUT_PC4_VREFP            CMPXMSEL(11)                   /*!< CMP inverting input PC4 for CMP0 or VREFP for CMP1*/
#define CMP_INVERTING_INPUT_VREFP                CMPXMSEL(12)                   /*!< CMP inverting input VREFP for CMP0 */

/* CMP output */
#define CS0_CMPXOSEL(regval)                     (BITS(8,10) & ((uint32_t)(regval) << 8U))
#define CMP_OUTPUT_NONE                          CS0_CMPXOSEL(0)                /*!< CMP output none  */
#define CMP_OUTPUT_TIMER0_BKIN                   CS0_CMPXOSEL(1)                /*!< CMP output TIMER0 break input */
#define CMP_OUTPUT_TIMER0_IC0                    CS0_CMPXOSEL(2)                /*!< CMP output TIMER0_CH0 input capture */
#define CMP_OUTPUT_TIMER0_OCPRECLR               CS0_CMPXOSEL(3)                /*!< CMP output TIMER0 OCPRE_CLR input */
#define CMP_OUTPUT_TIMER1_IC3                    CS0_CMPXOSEL(4)                /*!< CMP output TIMER1_CH3 input capture */
#define CMP_OUTPUT_TIMER1_OCPRECLR               CS0_CMPXOSEL(5)                /*!< CMP output TIMER1 OCPRE_CLR input */
#define CMP_OUTPUT_TIMER2_IC0                    CS0_CMPXOSEL(6)                /*!< CMP output TIMER2_CH0 input capture */
#define CMP_OUTPUT_TIMER2_OCPRECLR               CS0_CMPXOSEL(7)                /*!< CMP output TIMER2 OCPRE_CLR input */

/* CMP output polarity*/
#define CS0_CMPXPL(regval)                       (BIT(11) & ((uint32_t)(regval) << 11U))
#define CMP_OUTPUT_POLARITY_NONINVERTED          CS0_CMPXPL(0)                  /*!< CMP output not inverted */
#define CMP_OUTPUT_POLARITY_INVERTED             CS0_CMPXPL(1)                  /*!< CMP output inverted */

/* CMP output level */
#define CMP_OUTPUTLEVEL_HIGH                     ((uint32_t)0x00000001U)        /*!< CMP output high */
#define CMP_OUTPUTLEVEL_LOW                      ((uint32_t)0x00000000U)        /*!< CMP output low */

/* function definitions, 3239H new added functions */
/* CMP_CS1 */
/* CMP non-inverting input */
#define CS1_CMPXPSEL(regval)                    (BITS(14,15) & ((uint32_t)(regval) << 14U))
#define CMP_NON_INVERTING_INPUT_PA1_PA3          CS1_CMPXPSEL(0)                /*!< CMP non-inverting input PA1 for CMP0 or PA3 for CMP1 */
#define CMP_NON_INVERTING_INPUT_PC5_PB4          CS1_CMPXPSEL(1)                /*!< CMP non-inverting input PC5 for CMP0 or PB4 for CMP1 */
#define CMP_NON_INVERTING_INPUT_PB2_PB6          CS1_CMPXPSEL(2)                /*!< CMP non-inverting input PB2 for CMP0 or PB6 for CMP1 */

/* CMP resistance bridge enable */
#define CS1_CMPXBEN(regval)                     (BIT(12) & ((uint32_t)(regval) << 12U))
#define CMP_BEN_ENABLE                           CS1_CMPXBEN(0)                 /*!< CMP resistance bridge enable */
#define CMP_BEN_DISABLE                          CS1_CMPXBEN(1)                 /*!< CMP resistance bridge disable */

/* function declarations */
/* initialization functions */
/* CMP deinit */
void cmp_deinit(cmp_enum cmp_periph);
/* CMP mode init */
void cmp_mode_init(cmp_enum cmp_periph, uint32_t operating_mode, uint32_t inverting_input, uint32_t output_hysteresis);
/* CMP input init*/
void cmp_noninverting_input_select(cmp_enum cmp_periph, uint32_t noninverting_input);
/* CMP output init */
void cmp_output_init(cmp_enum cmp_periph, uint32_t output_selection, uint32_t output_polarity);

/* enable functions */
/* enable CMP */
void cmp_enable(cmp_enum cmp_periph);
/* disable CMP */
void cmp_disable(cmp_enum cmp_periph);
/* enable CMP switch */
void cmp_switch_enable(void);
/* disable CMP switch */
void cmp_switch_disable(void);
/* enable the window mode */
void cmp_window_enable(void);
/* disable the window mode */
void cmp_window_disable(void);
/* enable scaler bridge */
void cmp_scaler_bridge_enable(cmp_enum cmp_periph);
/* disable scaler bridge */
void cmp_scaler_bridge_disable(cmp_enum cmp_periph);
/* lock the CMP */
void cmp_lock_enable(cmp_enum cmp_periph);

/* get state related functions */
/* get output level */
uint32_t cmp_output_level_get(cmp_enum cmp_periph);

#endif /* GD32E511_512_CMP_H */

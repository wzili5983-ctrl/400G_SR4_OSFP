/*!
    \file    gd32e511_512_cmp.c
    \brief   CMP driver

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

#include "gd32e511_512_cmp.h"

/*!
    \brief      CMP deinit(API_ID(0x0001U))
    \param[in]  cmp_periph
      \arg        CMP0: comparator 0
      \arg        CMP1: comparator 1
    \param[out] none
    \retval     none
*/
void cmp_deinit(cmp_enum cmp_periph)
{
    if(CMP0 == cmp_periph) {
        CMP_CS0 &= ((uint32_t)0xFFFF0000U);
        CMP_CS1 &= ((uint32_t)0xFFFF0000U);
    } else if(CMP1 == cmp_periph) {
        CMP_CS0 &= ((uint32_t)0x0000FFFFU);
        CMP_CS1 &= ((uint32_t)0x0000FFFFU);
    } else {
    }
}

/*!
    \brief      CMP mode init (API_ID(0x0002U))
    \param[in]  cmp_periph
      \arg        CMP0: comparator 0
      \arg        CMP1: comparator 1
    \param[in]  operating_mode
      \arg        CMP_MODE_HIGHSPEED: high speed mode
      \arg        CMP_MODE_MIDDLESPEED: medium speed mode
      \arg        CMP_MODE_LOWSPEED: low speed mode
      \arg        CMP_MODE_VERYLOWSPEED: very-low speed mode
    \param[in]  inverting_input
      \arg        CMP_INVERTING_INPUT_1_4VREFINT: VREFINT *1/4 input
      \arg        CMP_INVERTING_INPUT_1_2VREFINT: VREFINT *1/2 input
      \arg        CMP_INVERTING_INPUT_3_4VREFINT: VREFINT *3/4 input
      \arg        CMP_INVERTING_INPUT_VREFINT: VREFINT input
      \arg        CMP_INVERTING_INPUT_PA4_PA6: CMP inverting input PA4(DAC0_OUT0) for CMP0 or PA6(DAC1_OUT0) for CMP1
      \arg        CMP_INVERTING_INPUT_PA5_PA7: CMP inverting input PA5(DAC0_OUT1) for CMP0 or PA7(DAC1_OUT1) for CMP1
      \arg        CMP_INVERTING_INPUT_PA0_PA2: CMP inverting input PA0 for CMP0 or PA2 for CMP1
      \arg        CMP_INVERTING_INPUT_PA2_DAC0_OUT0: CMP inverting input PA2 for CMP0 or DAC0_OUT0 for CMP1
      \arg        CMP_INVERTING_INPUT_DAC0_OUT0_OUT1: CMP inverting input DAC0_OUT0 for CMP0 or DAC0_OUT1 for CMP1
      \arg        CMP_INVERTING_INPUT_DAC0_OUT1_PB3: CMP inverting input DAC0_OUT1 for CMP0 or PB3 for CMP1
      \arg        CMP_INVERTING_INPUT_PB1_PB7: CMP inverting input PB1 for CMP0 or PB7 for CMP1
      \arg        CMP_INVERTING_INPUT_PC4_VREFP: CMP inverting input PC4 for CMP0 or VREFP for CMP1
      \arg        CMP_INVERTING_INPUT_VREFP: CMP inverting input VREFP for CMP0
    \param[in]  output_hysteresis
      \arg        CMP_HYSTERESIS_NO: output no hysteresis
      \arg        CMP_HYSTERESIS_LOW: output low hysteresis
      \arg        CMP_HYSTERESIS_MIDDLE: output middle hysteresis
      \arg        CMP_HYSTERESIS_HIGH: output high hysteresis
    \param[out] none
    \retval     none
*/
void cmp_mode_init(cmp_enum cmp_periph, uint32_t operating_mode, uint32_t inverting_input, uint32_t output_hysteresis)
{
    uint32_t temp = 0U;

    if(CMP0 == cmp_periph) {
        /* initialize comparator 0 mode */
        temp = CMP_CS0;
        temp &= ~(uint32_t)(CMP_CS0_CMP0M | CMP_CS0_CMP0MSEL | CMP_CS0_CMP0HST);
        temp |= (uint32_t)((operating_mode & CMP_CS0_CMP0M) | (output_hysteresis & CMP_CS0_CMP0HST) \
                           | ((uint32_t)(inverting_input) & CMP_CS0_CMP0MSEL));
        CMP_CS0 = temp;
        temp = CMP_CS1;
        temp &= ~(uint32_t)CMP_CS1_CMP0MSEL;
        temp |= ((uint32_t)(inverting_input) & CMP_CS1_CMP0MSEL);
        CMP_CS1 = temp;
    } else if(CMP1 == cmp_periph) {
        /* initialize comparator 1 mode */
        temp = CMP_CS0;
        temp &= ~(uint32_t)(CMP_CS0_CMP1M | CMP_CS0_CMP1MSEL | CMP_CS0_CMP1HST);
        temp |= (uint32_t)(((operating_mode << 16U) & CMP_CS0_CMP1M) | ((output_hysteresis << 16U) & CMP_CS0_CMP1HST) \
                           | (((uint32_t)(inverting_input) << 16U) & CMP_CS0_CMP1MSEL));
        CMP_CS0 = temp;
        temp = CMP_CS1;
        temp &= ~(uint32_t)CMP_CS1_CMP1MSEL;
        temp |= (((uint32_t)(inverting_input) << 16U)& CMP_CS1_CMP1MSEL);
        CMP_CS1 = temp;
    } else {
    }
}

/*!
    \brief      CMP non_inverting input init(API_ID(0x0003U))
    \param[in]  cmp_periph
      \arg        CMP0: comparator 0
      \arg        CMP1: comparator 1
    \param[in]  noninverting_input
      \arg        CMP_NON_INVERTING_INPUT_PA1_PA3: CMP non-inverting input PA1 for CMP0 or PA3 for CMP1
      \arg        CMP_NON_INVERTING_INPUT_PC5_PB4: CMP non-inverting input PC5 for CMP0 or PB4 for CMP1
      \arg        CMP_NON_INVERTING_INPUT_PB2_PB6: CMP non-inverting input PB2 for CMP0 or PB6 for CMP1
    \param[out] none
    \retval     none
*/
void cmp_noninverting_input_select(cmp_enum cmp_periph, uint32_t noninverting_input)
{
    uint32_t temp = 0U;

    if(CMP0 == cmp_periph) {
        temp = CMP_CS1;
        temp &= ~(uint32_t)CMP_CS1_CMP0PSEL;
        temp |= (uint32_t)(noninverting_input & CMP_CS1_CMP0PSEL);
        CMP_CS1 = temp;
    } else if(CMP1 == cmp_periph) {
        temp = CMP_CS1;
        temp &= ~(uint32_t)CMP_CS1_CMP1PSEL;
        temp |= (uint32_t)((noninverting_input << 16U) & CMP_CS1_CMP1PSEL);
        CMP_CS1 = temp;
    } else {
    }
}

/*!
    \brief      CMP output init (API_ID(0x0004U))
    \param[in]  cmp_periph
      \arg        CMP0: comparator 0
      \arg        CMP1: comparator 1
    \param[in]  output_selection
      \arg        CMP_OUTPUT_NONE: CMP output none
      \arg        CMP_OUTPUT_TIMER0_BKIN: CMP output TIMER0 break input
      \arg        CMP_OUTPUT_TIMER0_IC0: CMP output TIMER0_CH0 input capture
      \arg        CMP_OUTPUT_TIMER0_OCPRECLR: CMP output TIMER0 OCPRE_CLR input
      \arg        CMP_OUTPUT_TIMER1_IC3: CMP output TIMER1_CH3 input capture
      \arg        CMP_OUTPUT_TIMER1_OCPRECLR: CMP output TIMER1 OCPRE_CLR input
      \arg        CMP_OUTPUT_TIMER2_IC0: CMP output TIMER2_CH0 input capture
      \arg        CMP_OUTPUT_TIMER2_OCPRECLR: CMP output TIMER2 OCPRE_CLR input
    \param[in]  output_polarity
      \arg        CMP_OUTPUT_POLARITY_INVERTED: output is inverted
      \arg        CMP_OUTPUT_POLARITY_NONINVERTED: output is not inverted
    \param[out] none
    \retval     none
*/
void cmp_output_init(cmp_enum cmp_periph, uint32_t output_selection, uint32_t output_polarity)
{
    uint32_t temp = 0U;
    if(CMP0 == cmp_periph) {
        /* initialize comparator 0 output */
        temp = CMP_CS0;
        temp &= ~(uint32_t)CMP_CS0_CMP0OSEL;
        temp |= (uint32_t)((uint32_t)output_selection & CMP_CS0_CMP0OSEL);
        /* output polarity */
        temp &= ~(uint32_t)CMP_CS0_CMP0PL;
        temp |= (uint32_t)((uint32_t)output_polarity & CMP_CS0_CMP0PL);
        CMP_CS0 = temp;
    } else if(CMP1 == cmp_periph) {
        /* initialize comparator 1 output */
        temp = CMP_CS0;
        temp &= ~(uint32_t)CMP_CS0_CMP1OSEL;
        temp |= (uint32_t)((uint32_t)(output_selection << 16U) & CMP_CS0_CMP1OSEL);
        /* output polarity */
        temp &= ~(uint32_t)CMP_CS0_CMP1PL;
        temp |= (uint32_t)((uint32_t)(output_polarity << 16U) & CMP_CS0_CMP1PL);
        CMP_CS0 = temp;
    } else {
    }
}

/*!
    \brief      enable CMP (API_ID(0x0005U))
    \param[in]  cmp_periph
      \arg        CMP0: comparator 0
      \arg        CMP1: comparator 1
    \param[out] none
    \retval     none
*/
void cmp_enable(cmp_enum cmp_periph)
{
    if(CMP0 == cmp_periph) {
        CMP_CS0 |= (uint32_t)CMP_CS0_CMP0EN;
    } else if(CMP1 == cmp_periph) {
        CMP_CS0 |= (uint32_t)CMP_CS0_CMP1EN;
    } else {
    }
}

/*!
    \brief      disable CMP (API_ID(0x0006U))
    \param[in]  cmp_periph
      \arg        CMP0: comparator 0
      \arg        CMP1: comparator 1
    \param[out] none
    \retval     none
*/
void cmp_disable(cmp_enum cmp_periph)
{
    if(CMP0 == cmp_periph) {
        CMP_CS0 &= ~(uint32_t)CMP_CS0_CMP0EN;
    } else if(CMP1 == cmp_periph) {
        CMP_CS0 &= ~(uint32_t)CMP_CS0_CMP1EN;
    } else {
    }
}

/*!
    \brief      enable CMP switch (API_ID(0x0007U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void cmp_switch_enable(void)
{
    CMP_CS0 |= (uint32_t)CMP_CS0_CMP0SW;
}

/*!
    \brief      disable CMP switch (API_ID(0x0008U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void cmp_switch_disable(void)
{
    CMP_CS0 &= ~(uint32_t)CMP_CS0_CMP0SW;
}

/*!
    \brief      enable the window mode (API_ID(0x0009U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void cmp_window_enable(void)
{
    CMP_CS0 |= (uint32_t)CMP_CS0_WNDEN;
}

/*!
    \brief      disable the window mode (API_ID(0x000AU))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void cmp_window_disable(void)
{
    CMP_CS0 &= ~(uint32_t)CMP_CS0_WNDEN;
}

/*!
    \brief      enable the scaler bridge (API_ID(0x000BU))
    \param[in]  cmp_periph
      \arg        CMP0: comparator 0
      \arg        CMP1: comparator 1
    \param[out] none
    \retval     none
*/
void cmp_scaler_bridge_enable(cmp_enum cmp_periph)
{
    if(CMP0 == cmp_periph) {
        CMP_CS1 &= ~(uint32_t)CMP_CS1_CMP0BEN;
    } else if(CMP1 == cmp_periph) {
        CMP_CS1 &= ~(uint32_t)CMP_CS1_CMP1BEN;
    } else {
    }
}

/*!
    \brief      disable the scaler bridge (API_ID(0x000CU))
    \param[in]  cmp_periph
      \arg        CMP0: comparator 0
      \arg        CMP1: comparator 1
    \param[out] none
    \retval     none
*/
void cmp_scaler_bridge_disable(cmp_enum cmp_periph)
{
    if(CMP0 == cmp_periph) {
        CMP_CS1 |= (uint32_t)CMP_CS1_CMP0BEN;
    } else if(CMP1 == cmp_periph) {
        CMP_CS1 |= (uint32_t)CMP_CS1_CMP1BEN;
    } else {
    }
}

/*!
    \brief      lock the CMP (API_ID(0x000DU))
    \param[in]  cmp_periph
      \arg        CMP0: comparator 0
      \arg        CMP1: comparator 1
    \param[out] none
    \retval     none
*/
void cmp_lock_enable(cmp_enum cmp_periph)
{
    if(CMP0 == cmp_periph) {
        /* lock CMP0 */
        CMP_CS0 |= (uint32_t)CMP_CS0_CMP0LK;
    } else if(CMP1 == cmp_periph) {
        /* lock CMP1 */
        CMP_CS0 |= (uint32_t)CMP_CS0_CMP1LK;
    } else {
    }
}

/*!
    \brief      get output level (API_ID(0x000EU))
    \param[in]  cmp_periph
      \arg        CMP0: comparator 0
      \arg        CMP1: comparator 1
    \param[out] none
    \retval     the output level
*/
uint32_t cmp_output_level_get(cmp_enum cmp_periph)
{
    uint32_t cmp_output = CMP_OUTPUTLEVEL_LOW;
    if(CMP0 == cmp_periph) {
        /* get output level of CMP0 */
        if((uint32_t)RESET != (CMP_CS0 & CMP_CS0_CMP0O)) {
            cmp_output = CMP_OUTPUTLEVEL_HIGH;
        }
    } else {
        /* get output level of CMP1 */
        if((uint32_t)RESET != (CMP_CS0 & CMP_CS0_CMP1O)) {
            cmp_output = CMP_OUTPUTLEVEL_HIGH;
        }
    }
    return cmp_output;
}



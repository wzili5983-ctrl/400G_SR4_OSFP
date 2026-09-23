/*!
    \file    gd32e511_512_cla.c
    \brief   CLA driver

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

#include "gd32e511_512_cla.h"

#define CLA_ENABLE_MASK     ((uint32_t)0x000000FFU)
#define CLA_INT_MASK        ((uint32_t)0x0000FFFFU)
#define CLA_FLAG_MASK       ((uint32_t)0x0000FFFFU)

/*!
    \brief      reset CLA (API_ID(0x0001U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void cla_deinit(void)
{
    rcu_periph_reset_enable(RCU_CLARST);
    rcu_periph_reset_disable(RCU_CLARST);
}

/*!
    \brief      enable CLA (API_ID(0x0002U))
    \param[in]  cla_periph: CLAx(x=0,1,2,3,,4,5,6,7)
    \param[out] none
    \retval     none
*/
void cla_enable(uint32_t cla_periph)
{
    CLA_GCTL |= (1U << cla_periph);
}

/*!
    \brief      disable CLA (API_ID(0x0003U))
    \param[in]  cla_periph: CLAx(x=0,1,2,3,,4,5,6,7)
    \param[out] none
    \retval     none
*/
void cla_disable(uint32_t cla_periph)
{
    CLA_GCTL &= ~(1U << cla_periph);
}

/*!
    \brief      get CLA output state (API_ID(0x0004U))
    \param[in]  cla_periph: CLAx(x=0,1,2,3,,4,5,6,7)
    \param[out] none
    \retval     cla_outputstatus_enum: CLA_OUTPUT_HIGH or CLA_OUTPUT_LOW
*/
cla_outputstatus_enum cla_output_state_get(uint32_t cla_periph)
{
    cla_outputstatus_enum status;
    uint32_t returnval;

    returnval = (CLA_STAT & (1U << cla_periph));

    if(returnval) {
        status = CLA_OUTPUT_HIGH;
    } else {
        status = CLA_OUTPUT_LOW;
    }
    return status;
}

/*!
    \brief      configure signal selector input (API_ID(0x0005U))
    \param[in]  cla_periph: CLAx(x=0,1,2,3,,4,5,6,7)
    \param[in]  sigs: signal selector
                only one parameter can be selected which is shown as below:
                SIGS0: signal selector 0
                SIGS1: signal selector 1
    \param[in]  input: input of signal selector

     SIGS0 input selection:
     only one parameter can be selected which is shown as below:
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |  SIGS0[4:0]|      CLA0SIGS0             |      CLA1SIGS0             |      CLA2SIGS0             |     CLA3SIGS0            |      CLA4SIGS0             |      CLA5SIGS0             |      CLA6SIGS0             |     CLA7SIGS0            |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    00000   | CLA0SIGS0_CLA0_ASYNC_OUT   | CLA1SIGS0_CLA0_ASYNC_OUT   | CLA2SIGS0_CLA0_ASYNC_OUT   | CLA3SIGS0_CLA0_ASYNC_OUT | CLA4SIGS0_CLA0_ASYNC_OUT   | CLA5SIGS0_CLA0_ASYNC_OUT   | CLA6SIGS0_CLA0_ASYNC_OUT   | CLA7SIGS0_CLA0_ASYNC_OUT |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    00001   | CLA0SIGS0_CLA1_ASYNC_OUT   | CLA1SIGS0_CLA1_ASYNC_OUT   | CLA2SIGS0_CLA1_ASYNC_OUT   | CLA3SIGS0_CLA1_ASYNC_OUT | CLA4SIGS0_CLA1_ASYNC_OUT   | CLA5SIGS0_CLA1_ASYNC_OUT   | CLA6SIGS0_CLA1_ASYNC_OUT   | CLA7SIGS0_CLA1_ASYNC_OUT |
     -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    00010   | CLA0SIGS0_CLA2_ASYNC_OUT   | CLA1SIGS0_CLA2_ASYNC_OUT   | CLA2SIGS0_CLA2_ASYNC_OUT   | CLA3SIGS0_CLA2_ASYNC_OUT | CLA4SIGS0_CLA2_ASYNC_OUT   | CLA5SIGS0_CLA2_ASYNC_OUT   | CLA6SIGS0_CLA2_ASYNC_OUT   | CLA7SIGS0_CLA2_ASYNC_OUT |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    00011   | CLA3SIGS0_CLA3_ASYNC_OUT   | CLA1SIGS0_CLA3_ASYNC_OUT   | CLA2SIGS0_CLA3_ASYNC_OUT   | CLA3SIGS0_CLA3_ASYNC_OUT | CLA4SIGS0_CLA3_ASYNC_OUT   | CLA5SIGS0_CLA3_ASYNC_OUT   | CLA6SIGS0_CLA3_ASYNC_OUT   | CLA7SIGS0_CLA3_ASYNC_OUT |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    00100   | CLA0SIGS0_TIMER1_TRGO      | CLA1SIGS0_TIMER1_TRGO      | CLA2SIGS0_TIMER1_TRGO      | CLA3SIGS0_TIMER1_TRGO    | CLA4SIGS0_TIMER1_TRGO      | CLA5SIGS0_TIMER1_TRGO      | CLA6SIGS0_TIMER1_TRGO      | CLA7SIGS0_TIMER1_TRGO    |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    00101   | CLA0SIGS0_TIMER0_CH0       | CLA1SIGS0_TIMER0_CH0       | CLA2SIGS0_TIMER0_CH1       | CLA3SIGS0_TIMER0_CH2     | CLA4SIGS0_TIMER0_CH0       | CLA5SIGS0_TIMER0_CH0       | CLA6SIGS0_TIMER0_CH1       | CLA7SIGS0_TIMER0_CH2     |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    00110   | CLA0SIGS0_TIMER0_CH1       | CLA1SIGS0_TIMER0_CH3       | CLA2SIGS0_TIMER0_CH3       | CLA3SIGS0_TIMER1_CH0     | CLA4SIGS0_TIMER0_CH1       | CLA5SIGS0_TIMER0_CH3       | CLA6SIGS0_TIMER0_CH3       | CLA7SIGS0_TIMER1_CH0     |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    00111   | CLA0SIGS0_TIMER0_CH2       | CLA1SIGS0_TIMER1_CH0       | CLA2SIGS0_TIMER1_CH1       | CLA3SIGS0_TIMER1_CH1     | CLA4SIGS0_TIMER0_CH2       | CLA5SIGS0_TIMER1_CH0       | CLA6SIGS0_TIMER1_CH1       | CLA7SIGS0_TIMER1_CH1     |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    01000   | CLA0SIGS0_CLAIN0           | CLA1SIGS0_CLAIN4           | CLA2SIGS0_CLAIN0           | CLA3SIGS0_CLAIN2         | CLA4SIGS0_CLAIN0           | CLA5SIGS0_CLAIN4           | CLA6SIGS0_CLAIN0           | CLA7SIGS0_CLAIN2         |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    01001   | CLA0SIGS0_CLAIN2           | CLA1SIGS0_CLAIN5           | CLA2SIGS0_CLAIN1           | CLA3SIGS0_CLAIN3         | CLA4SIGS0_CLAIN2           | CLA5SIGS0_CLAIN5           | CLA6SIGS0_CLAIN1           | CLA7SIGS0_CLAIN3         |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    01010   | CLA0SIGS0_CLAIN4           | CLA1SIGS0_CLAIN8           | CLA2SIGS0_CLAIN8           | CLA3SIGS0_CLAIN6         | CLA4SIGS0_CLAIN4           | CLA5SIGS0_CLAIN8           | CLA6SIGS0_CLAIN8           | CLA7SIGS0_CLAIN6         |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    01011   | CLA0SIGS0_CLAIN6           | CLA1SIGS0_CLAIN10          | CLA2SIGS0_CLAIN9           | CLA3SIGS0_CLAIN7         | CLA4SIGS0_CLAIN6           | CLA5SIGS0_CLAIN10          | CLA6SIGS0_CLAIN9           | CLA7SIGS0_CLAIN7         |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    01100   | CLA0SIGS0_CLAIN8           | CLA1SIGS0_CLAIN12          | CLA2SIGS0_CLAIN14          | CLA3SIGS0_CLAIN10        | CLA4SIGS0_CLAIN8           | CLA5SIGS0_CLAIN12          | CLA6SIGS0_CLAIN14          | CLA7SIGS0_CLAIN10        |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    01101   | CLA0SIGS0_CLAIN10          | CLA1SIGS0_CLAIN13          | CLA2SIGS0_CLAIN15          | CLA3SIGS0_CLAIN11        | CLA4SIGS0_CLAIN10          | CLA5SIGS0_CLAIN13          | CLA6SIGS0_CLAIN15          | CLA7SIGS0_CLAIN11        |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    01110   | CLA0SIGS0_CLAIN12          | CLA1SIGS0_CLAIN16          | CLA2SIGS0_CLAIN16          | CLA3SIGS0_CLAIN18        | CLA4SIGS0_CLAIN12          | CLA5SIGS0_CLAIN16          | CLA6SIGS0_CLAIN16          | CLA7SIGS0_CLAIN18        |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    01111   | CLA0SIGS0_CLAIN14          | CLA1SIGS0_CLAIN18          | CLA2SIGS0_CLAIN17          | CLA3SIGS0_CLAIN19        | CLA4SIGS0_CLAIN14          | CLA5SIGS0_CLAIN18          | CLA6SIGS0_CLAIN17          | CLA7SIGS0_CLAIN19        |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    10000   | CLA0SIGS0_CLA4_ASYNC_OUT   | CLA1SIGS0_CLA4_ASYNC_OUT   | CLA2SIGS0_CLA4_ASYNC_OUT   | CLA3SIGS0_CLA4_ASYNC_OUT | CLA4SIGS0_CLA4_ASYNC_OUT   | CLA5SIGS0_CLA4_ASYNC_OUT   | CLA6SIGS0_CLA4_ASYNC_OUT   | CLA7SIGS0_CLA4_ASYNC_OUT |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    10001   | CLA0SIGS0_CLA5_ASYNC_OUT   | CLA1SIGS0_CLA5_ASYNC_OUT   | CLA2SIGS0_CLA5_ASYNC_OUT   | CLA3SIGS0_CLA5_ASYNC_OUT | CLA4SIGS0_CLA5_ASYNC_OUT   | CLA5SIGS0_CLA5_ASYNC_OUT   | CLA6SIGS0_CLA5_ASYNC_OUT   | CLA7SIGS0_CLA5_ASYNC_OUT |
     --------------------------------------------------------------------------------------------------------                  ------------------------------------------------------------------------------------------------------------------------
     |    10010   |           ---              | CLA1SIGS0_CLA6_ASYNC_OUT   | CLA2SIGS0_CLA6_ASYNC_OUT   | CLA3SIGS0_CLA6_ASYNC_OUT | CLA4SIGS0_CLA6_ASYNC_OUT   | CLA5SIGS0_CLA6_ASYNC_OUT   | CLA6SIGS0_CLA6_ASYNC_OUT   | CLA7SIGS0_CLA6_ASYNC_OUT |
     ------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------
     |    10011   | CLA0SIGS0_CLA7_ASYNC_OUT   | CLA1SIGS0_CLA7_ASYNC_OUT   | CLA2SIGS0_CLA7_ASYNC_OUT   | CLA3SIGS0_CLA_7ASYNC_OUT | CLA4SIGS0_CLA7_ASYNC_OUT   | CLA5SIGS0_CLA7_ASYNC_OUT   | CLA6SIGS0_CLA7_ASYNC_OUT   | CLA7SIGS0_CLA_7ASYNC_OUT |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    SWBEN0  | CLA0SIGS0_SOFTWARE         | CLA1SIGS0_SOFTWARE         | CLA2SIGS0_SOFTWARE         | CLA3SIGS0_SOFTWARE       | CLA4SIGS0_SOFTWARE         | CLA5SIGS0_SOFTWARE         | CLA6SIGS0_SOFTWARE         | CLA7SIGS0_SOFTWARE       |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     SIGS1 input selection:
     only one parameter can be selected which is shown as below:
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |  SIGS1[4:0]|      CLA0SIGS1             |      CLA1SIGS1             |       CLA2SIGS1            |     CLA3SIGS1            |      CLA4SIGS1             |      CLA5SIGS1             |       CLA6SIGS1            |     CLA7SIGS1            |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    00000   | CLA0SIGS1_CLA0_ASYNC_OUT   | CLA1SIGS1_CLA0_ASYNC_OUT   | CLA2SIGS1_CLA0_ASYNC_OUT   | CLA3SIGS1_CLA0_ASYNC_OUT | CLA4SIGS1_CLA0_ASYNC_OUT   | CLA5SIGS1_CLA0_ASYNC_OUT   | CLA6SIGS1_CLA0_ASYNC_OUT   | CLA7SIGS1_CLA0_ASYNC_OUT |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    00001   | CLA0SIGS1_CLA1_ASYNC_OUT   | CLA1SIGS1_CLA1_ASYNC_OUT   | CLA2SIGS1_CLA1_ASYNC_OUT   | CLA3SIGS1_CLA1_ASYNC_OUT | CLA4SIGS1_CLA1_ASYNC_OUT   | CLA5SIGS1_CLA1_ASYNC_OUT   | CLA6SIGS1_CLA1_ASYNC_OUT   | CLA7SIGS1_CLA1_ASYNC_OUT |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    00010   | CLA0SIGS1_CLA2_ASYNC_OUT   | CLA1SIGS1_CLA2_ASYNC_OUT   | CLA2SIGS1_CLA2_ASYNC_OUT   | CLA3SIGS1_CLA2_ASYNC_OUT | CLA4SIGS1_CLA2_ASYNC_OUT   | CLA5SIGS1_CLA2_ASYNC_OUT   | CLA6SIGS1_CLA2_ASYNC_OUT   | CLA7SIGS1_CLA2_ASYNC_OUT |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    00011   | CLA0SIGS1_CLA3_ASYNC_OUT   | CLA1SIGS1_CLA3_ASYNC_OUT   | CLA2SIGS1_CLA3_ASYNC_OUT   | CLA3SIGS1_CLA3_ASYNC_OUT | CLA4SIGS1_CLA3_ASYNC_OUT   | CLA5SIGS1_CLA3_ASYNC_OUT   | CLA6SIGS1_CLA3_ASYNC_OUT   | CLA7SIGS1_CLA3_ASYNC_OUT |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    00100   | CLA0SIGS1_ADC0_CONV        | CLA1SIGS1_ADC0_CONV        | CLA2SIGS1_ADC0_CONV        | CLA3SIGS1_ADC0_CONV      | CLA4SIGS1_ADC0_CONV        | CLA5SIGS1_ADC0_CONV        | CLA6SIGS1_ADC0_CONV        | CLA7SIGS1_ADC0_CONV      |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    00101   | CLA0SIGS1_TIMER0_CH3       | CLA1SIGS1_TIMER0_CH1       | CLA2SIGS1_TIMER0_CH0       | CLA3SIGS1_TIMER0_CH0     | CLA4SIGS1_TIMER0_CH3       | CLA5SIGS1_TIMER0_CH1       | CLA6SIGS1_TIMER0_CH0       | CLA7SIGS1_TIMER0_CH0     |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    00110   | CLA0SIGS1_TIMER1_CH0       | CLA1SIGS1_TIMER0_CH2       | CLA2SIGS1_TIMER0_CH2       | CLA3SIGS1_TIMER0_CH1     | CLA4SIGS1_TIMER1_CH0       | CLA5SIGS1_TIMER0_CH2       | CLA6SIGS1_TIMER0_CH2       | CLA7SIGS1_TIMER0_CH1     |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    00111   | CLA0SIGS1_TIMER1_CH1       | CLA1SIGS1_TIMER1_CH1       | CLA2SIGS1_TIMER1_CH0       | CLA3SIGS1_TIMER0_CH3     | CLA4SIGS1_TIMER2_CH1       | CLA5SIGS1_TIMER2_CH1       | CLA6SIGS1_TIMER1_CH0       | CLA7SIGS1_TIMER0_CH3     |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    01000   | CLA0SIGS1_CLAIN1           | CLA1SIGS1_CLAIN6           | CLA2SIGS1_CLAIN2           | CLA3SIGS1_CLAIN0         | CLA4SIGS1_CLAIN1           | CLA5SIGS1_CLAIN6           | CLA6SIGS1_CLAIN2           | CLA7SIGS1_CLAIN0         |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    01001   | CLA0SIGS1_CLAIN3           | CLA1SIGS1_CLAIN7           | CLA2SIGS1_CLAIN3           | CLA3SIGS1_CLAIN1         | CLA4SIGS1_CLAIN3           | CLA5SIGS1_CLAIN7           | CLA6SIGS1_CLAIN3           | CLA7SIGS1_CLAIN1         |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    01010   | CLA0SIGS1_CLAIN5           | CLA1SIGS1_CLAIN9           | CLA2SIGS1_CLAIN10          | CLA3SIGS1_CLAIN4         | CLA4SIGS1_CLAIN5           | CLA5SIGS1_CLAIN9           | CLA6SIGS1_CLAIN10          | CLA7SIGS1_CLAIN4         |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    01011   | CLA0SIGS1_CLAIN7           | CLA1SIGS1_CLAIN11          | CLA2SIGS1_CLAIN11          | CLA3SIGS1_CLAIN5         | CLA4SIGS1_CLAIN6           | CLA5SIGS1_CLAIN11          | CLA6SIGS1_CLAIN11          | CLA7SIGS1_CLAIN5         |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    01100   | CLA0SIGS1_CLAIN9           | CLA1SIGS1_CLAIN14          | CLA2SIGS1_CLAIN12          | CLA3SIGS1_CLAIN8         | CLA4SIGS1_CLAIN9           | CLA5SIGS1_CLAIN14          | CLA6SIGS1_CLAIN12          | CLA7SIGS1_CLAIN8         |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    01101   | CLA0SIGS1_CLAIN11          | CLA1SIGS1_CLAIN15          | CLA2SIGS1_CLAIN13          | CLA3SIGS1_CLAIN9         | CLA4SIGS1_CLAIN11          | CLA5SIGS1_CLAIN15          | CLA6SIGS1_CLAIN13          | CLA7SIGS1_CLAIN9         |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    01110   | CLA0SIGS1_CLAIN11          | CLA1SIGS1_CLAIN15          | CLA2SIGS1_CLAIN13          | CLA3SIGS1_CLAIN9         | CLA4SIGS1_CLAIN11          | CLA5SIGS1_CLAIN15          | CLA6SIGS1_CLAIN13          | CLA7SIGS1_CLAIN9         |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    01111   | CLA0SIGS1_CLAIN13          | CLA1SIGS1_CLAIN17          | CLA2SIGS1_CLAIN18          | CLA3SIGS1_CLAIN16        | CLA4SIGS1_CLAIN13          | CLA5SIGS1_CLAIN17          | CLA6SIGS1_CLAIN18          | CLA7SIGS1_CLAIN16        |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    10000   | CLA0SIGS1_CLA4_ASYNC_OUT   | CLA1SIGS1_CLA4_ASYNC_OUT   | CLA2SIGS1_CLA4_ASYNC_OUT   | CLA3SIGS1_CLA4_ASYNC_OUT | CLA4SIGS1_CLA4_ASYNC_OUT   | CLA5SIGS1_CLA4_ASYNC_OUT   | CLA6SIGS1_CLA4_ASYNC_OUT   | CLA7SIGS1_CLA4_ASYNC_OUT |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    10001   | CLA0SIGS1_CLA5_ASYNC_OUT   | CLA1SIGS1_CLA5_ASYNC_OUT   | CLA2SIGS1_CLA5_ASYNC_OUT   | CLA3SIGS1_CLA5_ASYNC_OUT | CLA4SIGS1_CLA5_ASYNC_OUT   | CLA5SIGS1_CLA5_ASYNC_OUT   | CLA6SIGS1_CLA5_ASYNC_OUT   | CLA7SIGS1_CLA5_ASYNC_OUT |
     -------------------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------
     |    10010   |            ---             | CLA1SIGS1_CLA6_ASYNC_OUT   | CLA2SIGS1_CLA6_ASYNC_OUT   | CLA3SIGS1_CLA6_ASYNC_OUT | CLA4SIGS1_CLA6_ASYNC_OUT   | CLA5SIGS1_CLA6_ASYNC_OUT   | CLA6SIGS1_CLA6_ASYNC_OUT   | CLA7SIGS1_CLA6_ASYNC_OUT |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    10011   | CLA0SIGS1_CLA7_ASYNC_OUT   | CLA1SIGS1_CLA7_ASYNC_OUT   | CLA2SIGS1_CLA7_ASYNC_OUT   | CLA3SIGS1_CLA7_ASYNC_OUT | CLA4SIGS1_CLA7_ASYNC_OUT   | CLA5SIGS1_CLA7_ASYNC_OUT   | CLA6SIGS1_CLA7_ASYNC_OUT   | CLA7SIGS1_CLA7_ASYNC_OUT |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    10100   | CLA0SIGS1_ADC1_CONV        | CLA1SIGS1_ADC1_CONV        | CLA2SIGS1_ADC1_CONV        | CLA3SIGS1_ADC1_CONV      | CLA4SIGS1_ADC1_CONV        | CLA5SIGS1_ADC1_CONV        | CLA6SIGS1_ADC1_CONV        | CLA7SIGS1_ADC1_CONV      |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     |    SWBEN1  | CLA0SIGS1_SOFTWARE         | CLA1SIGS1_SOFTWARE         | CLA2SIGS1_SOFTWARE         | CLA3SIGS1_SOFTWARE       | CLA4SIGS1_SOFTWARE         | CLA5SIGS1_SOFTWARE         | CLA6SIGS1_SOFTWARE         | CLA7SIGS1_SOFTWARE       |
     --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    \param[out] none
    \retval     none
*/
void cla_sigs_input_config(uint32_t cla_periph, cla_sigs_enum sigs, uint32_t input)
{
    if(SIGS0 == sigs) {
        if(CLA_CTL_SWSIGS0EN == input) {
            CLA_CTL(cla_periph) |= CLA_CTL_SWSIGS0EN;
        } else {
            CLA_CTL(cla_periph) &= (~CLA_CTL_SWSIGS0EN);
            CLA_SIGS(cla_periph) &= (~(CLA_SIGS_SIGS0 | CLA_SIGS_SIGS0_4));
            CLA_SIGS(cla_periph) |= input & (CLA_SIGS_SIGS0 | CLA_SIGS_SIGS0_4);
        }
    } else {
        if(CLA_CTL_SWSIGS1EN == input) {
            CLA_CTL(cla_periph) |= CLA_CTL_SWSIGS1EN;
        } else {
            CLA_CTL(cla_periph) &= (~CLA_CTL_SWSIGS1EN);
            CLA_SIGS(cla_periph) &= (~(CLA_SIGS_SIGS1 | CLA_SIGS_SIGS1_4));
            CLA_SIGS(cla_periph) |= input & (CLA_SIGS_SIGS1 | CLA_SIGS_SIGS1_4);
        }
    }
}

/*!
    \brief      configure CLA LCU control register value (API_ID(0x0006U))
    \param[in]  cla_periph: CLAx(x=0,1,2,3,,4,5,6,7)
    \param[in]  lcuctl_value: the value need to set
    \param[out] none
    \retval     none
*/
void cla_lcu_control_config(uint32_t cla_periph, uint8_t lcuctl_value)
{
    CLA_LCUCTL(cla_periph) &= (~CLA_LCU);
    CLA_LCUCTL(cla_periph) |= (LCUCTL(lcuctl_value) & CLA_LCU);
}

/*!
    \brief      configure multiplexer software output (API_ID(0x0007U))
    \param[in]  cla_periph: CLAx(x=0,1,2,3,,4,5,6,7)
    \param[in]  sigs: signal selector
                only one parameter can be selected which is shown as below:
                SIGS0: multiplexer 0
                SIGS1: multiplexer 1
    \param[in]  output: multiplexer output value
     only one parameter can be selected which is shown as below:
                SOFTWARE_OUTPUT_HIGH: multiplexer output 1
                SOFTWARE_OUTPUT_LOW: multiplexer output 0
    \param[out] none
    \retval     none
*/
void cla_sigs_software_output_set(uint32_t cla_periph, cla_sigs_enum sigs, uint8_t output)
{
    if(SIGS0 == sigs) {
        if(SOFTWARE_OUTPUT_LOW == output) {
            CLA_CTL(cla_periph) &= (~CLA_CTL_SWSIGS0);
        } else {
            CLA_CTL(cla_periph) |= CLA_CTL_SWSIGS0;
        }
    } else {
        if(SOFTWARE_OUTPUT_LOW == output) {
            CLA_CTL(cla_periph) &= (~CLA_CTL_SWSIGS1);
        } else {
            CLA_CTL(cla_periph) |= CLA_CTL_SWSIGS1;
        }
    }
}

/*!
    \brief      configure CLA output (API_ID(0x0008U))
    \param[in]  cla_periph: CLAx(x=0,1,2,3,,4,5,6,7)
    \param[in]  output: output of CLA
                only one parameter can be selected which is shown as below:
                FLIP_FLOP_OUTPUT: flip-flop output is selected as CLA output
                LCU_RESULT: LUT result is selected as CLA output
    \param[out] none
    \retval     none
*/
void cla_output_config(uint32_t cla_periph, uint32_t output)
{
    CLA_CTL(cla_periph) &= (~CLA_CTL_OSEL);
    CLA_CTL(cla_periph) |= output & CLA_CTL_OSEL;
}

/*!
    \brief      enable CLA output (API_ID(0x0009U))
    \param[in]  cla_periph: CLAx(x=0,1,2,3,,4,5,6,7)
    \param[out] none
    \retval     none
*/
void cla_output_enable(uint32_t cla_periph)
{
    CLA_CTL(cla_periph) |= CLA_CTL_OEN;
}

/*!
    \brief      disable CLA output (API_ID(0x000AU))
    \param[in]  cla_periph: CLAx(x=0,1,2,3,,4,5,6,7)
    \param[out] none
    \retval     none
*/
void cla_output_disable(uint32_t cla_periph)
{
    CLA_CTL(cla_periph) &= (~CLA_CTL_OEN);
}

/*!
    \brief      reset the flip-flop output asynchronously (API_ID(0x000BU))
    \param[in]  cla_periph: CLAx(x=0,1,2,3,,4,5,6,7)
    \param[out] none
    \retval     none
*/
void cla_flip_flop_output_reset(uint32_t cla_periph)
{
    CLA_CTL(cla_periph) |= CLA_CTL_FFRST;
}

/*!
    \brief      configure clock polarity of flip-flop (API_ID(0x000CU))
    \param[in]  cla_periph: CLAx(x=0,1,2,3,,4,5,6,7)
    \param[in]  polarity: clock polarity of flip-flop
                only one parameter can be selected which is shown as below:
                CLA_CLOCKPOLARITY_POSEDGE: clock posedge is valid
                CLA_CLOCKPOLARITY_NEGEDGE: clock negedge is valid
    \param[out] none
    \retval     none
*/
void cla_flip_flop_clockpolarity_config(uint32_t cla_periph, uint32_t polarity)
{
    CLA_CTL(cla_periph) &= (~CLA_CTL_CPOL);
    CLA_CTL(cla_periph) |= polarity & CLA_CTL_CPOL;
}

/*!
    \brief      configure clock source of flip-flop (API_ID(0x000DU))
    \param[in]  cla_periph: CLAx(x=0,1,2,3,,4,5,6,7)
    \param[in]  clock_source: clock source of flip-flop
                only one parameter can be selected which is shown as below:
                PRE_CLA_LUT_RESULT: the result of the previous CLA units
                SIGS0_OUTPUT: the signal selector output of SIGS0
                HCLK: HCLK
                TIMER_TRGO: TIMER_TRGO
    \param[out] none
    \retval     none
*/
void cla_flip_flop_clocksource_config(uint32_t cla_periph, uint32_t clock_source)
{
    CLA_CTL(cla_periph) &= (~CLA_CTL_CSEL);
    CLA_CTL(cla_periph) |= clock_source & CLA_CTL_CSEL;
}

/*!
    \brief      check CLA flag is set or not (API_ID(0x000EU))
    \param[in]  flag: CLA flags
                only one parameter can be selected which is shown as below:
                  CLA0_FLAG_NEGEDGE: CLA0 unit negedge flag
                  CLA0_FLAG_POSEDGE: CLA0 unit posedge flag
                  CLA1_FLAG_NEGEDGE: CLA1 unit negedge flag
                  CLA1_FLAG_POSEDGE: CLA1 unit posedge flag
                  CLA2_FLAG_NEGEDGE: CLA2 unit negedge flag
                  CLA2_FLAG_POSEDGE: CLA2 unit posedge flag
                  CLA3_FLAG_NEGEDGE: CLA3 unit negedge flag
                  CLA3_FLAG_POSEDGE: CLA3 unit posedge flag
                  CLA4_FLAG_NEGEDGE: CLA4 unit negedge flag
                  CLA4_FLAG_POSEDGE: CLA4 unit posedge flag
                  CLA5_FLAG_NEGEDGE: CLA5 unit negedge flag
                  CLA5_FLAG_POSEDGE: CLA5 unit posedge flag
                  CLA6_FLAG_NEGEDGE: CLA6 unit negedge flag
                  CLA6_FLAG_POSEDGE: CLA6 unit posedge flag
                  CLA7_FLAG_NEGEDGE: CLA7 unit negedge flag
                  CLA7_FLAG_POSEDGE: CLA7 unit posedge flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus cla_flag_get(uint32_t flag)
{
    FlagStatus status;

    /* get the CLA flag */
    if(CLA_INTF & flag) {
        status = SET;
    } else {
        status = RESET;
    }
    return status;
}

/*!
    \brief      clear CLA flag (API_ID(0x000FU))
    \param[in]  flag: CLA flags
                one or more parameters can be selected which is shown as below:
                  CLA0_FLAG_NEGEDGE: CLA0 unit negedge flag
                  CLA0_FLAG_POSEDGE: CLA0 unit posedge flag
                  CLA1_FLAG_NEGEDGE: CLA1 unit negedge flag
                  CLA1_FLAG_POSEDGE: CLA1 unit posedge flag
                  CLA2_FLAG_NEGEDGE: CLA2 unit negedge flag
                  CLA2_FLAG_POSEDGE: CLA2 unit posedge flag
                  CLA3_FLAG_NEGEDGE: CLA3 unit negedge flag
                  CLA3_FLAG_POSEDGE: CLA3 unit posedge flag
                  CLA4_FLAG_NEGEDGE: CLA4 unit negedge flag
                  CLA4_FLAG_POSEDGE: CLA4 unit posedge flag
                  CLA5_FLAG_NEGEDGE: CLA5 unit negedge flag
                  CLA5_FLAG_POSEDGE: CLA5 unit posedge flag
                  CLA6_FLAG_NEGEDGE: CLA6 unit negedge flag
                  CLA6_FLAG_POSEDGE: CLA6 unit posedge flag
                  CLA7_FLAG_NEGEDGE: CLA7 unit negedge flag
                  CLA7_FLAG_POSEDGE: CLA7 unit posedge flag
    \param[out] none
    \retval     none
*/
void cla_flag_clear(uint32_t flag)
{
    CLA_INTF &= ~(flag & CLA_FLAG_MASK);
}

/*!
    \brief      enable CLA interrupt (API_ID(0x0010U))
    \param[in]  interrupt: CLA interrupt enable
                  one or more parameters can be selected which is shown as below:
                    CLA0_INT_NEGEDGE: CLA0 negedge interrupt enable
                    CLA0_INT_POSEDGE: CLA0 posedge interrupt enable
                    CLA1_INT_NEGEDGE: CLA1 negedge interrupt enable
                    CLA1_INT_POSEDGE: CLA1 posedge interrupt enable
                    CLA2_INT_NEGEDGE: CLA2 negedge interrupt enable
                    CLA2_INT_POSEDGE: CLA2 posedge interrupt enable
                    CLA3_INT_NEGEDGE: CLA3 negedge interrupt enable
                    CLA3_INT_POSEDGE: CLA3 posedge interrupt enable
                    CLA4_INT_NEGEDGE: CLA4 negedge interrupt enable
                    CLA4_INT_POSEDGE: CLA4 posedge interrupt enable
                    CLA5_INT_NEGEDGE: CLA5 negedge interrupt enable
                    CLA5_INT_POSEDGE: CLA5 posedge interrupt enable
                    CLA6_INT_NEGEDGE: CLA6 negedge interrupt enable
                    CLA6_INT_POSEDGE: CLA6 posedge interrupt enable
                    CLA7_INT_NEGEDGE: CLA7 negedge interrupt enable
                    CLA7_INT_POSEDGE: CLA7 posedge interrupt enable
      \param[out] none
    \retval     none
*/
void cla_interrupt_enable(uint32_t interrupt)
{
    CLA_INTE |= interrupt & CLA_INT_MASK;
}

/*!
    \brief      disable CLA interrupt (API_ID(0x0011U))
    \param[in]  interrupt: CLA negedge interrupt disable
                one or more parameters can be selected which is shown as below:
                    CLA0_INT_NEGEDGE: CLA0 negedge interrupt enable
                    CLA0_INT_POSEDGE: CLA0 posedge interrupt enable
                    CLA1_INT_NEGEDGE: CLA1 negedge interrupt enable
                    CLA1_INT_POSEDGE: CLA1 posedge interrupt enable
                    CLA2_INT_NEGEDGE: CLA2 negedge interrupt enable
                    CLA2_INT_POSEDGE: CLA2 posedge interrupt enable
                    CLA3_INT_NEGEDGE: CLA3 negedge interrupt enable
                    CLA3_INT_POSEDGE: CLA3 posedge interrupt enable
                    CLA4_INT_NEGEDGE: CLA4 negedge interrupt enable
                    CLA4_INT_POSEDGE: CLA4 posedge interrupt enable
                    CLA5_INT_NEGEDGE: CLA5 negedge interrupt enable
                    CLA5_INT_POSEDGE: CLA5 posedge interrupt enable
                    CLA6_INT_NEGEDGE: CLA6 negedge interrupt enable
                    CLA6_INT_POSEDGE: CLA6 posedge interrupt enable
                    CLA7_INT_NEGEDGE: CLA7 negedge interrupt enable
                    CLA7_INT_POSEDGE: CLA7 posedge interrupt enable
      \param[out] none
    \retval     none
*/
void cla_interrupt_disable(uint32_t interrupt)
{
    CLA_INTE &= ~(interrupt & CLA_INT_MASK);
}

/*!
    \brief      check CLA interrupt flag is set or not (API_ID(0x0012U))
    \param[in]  int_flag: CLA interrupt flags
                only one parameter can be selected which is shown as below:
                  CLA0_INT_FLAG_NEGEDGE: CLA0 unit negedge interrupt flag
                  CLA0_INT_FLAG_POSEDGE: CLA0 unit posedge interrupt flag
                  CLA1_INT_FLAG_NEGEDGE: CLA1 unit negedge interrupt flag
                  CLA1_INT_FLAG_POSEDGE: CLA1 unit posedge interrupt flag
                  CLA2_INT_FLAG_NEGEDGE: CLA2 unit negedge interrupt flag
                  CLA2_INT_FLAG_POSEDGE: CLA2 unit posedge interrupt flag
                  CLA3_INT_FLAG_NEGEDGE: CLA3 unit negedge interrupt flag
                  CLA3_INT_FLAG_POSEDGE: CLA3 unit posedge interrupt flag
                  CLA4_INT_FLAG_NEGEDGE: CLA4 unit negedge interrupt flag
                  CLA4_INT_FLAG_POSEDGE: CLA4 unit posedge interrupt flag
                  CLA5_INT_FLAG_NEGEDGE: CLA5 unit negedge interrupt flag
                  CLA5_INT_FLAG_POSEDGE: CLA5 unit posedge interrupt flag
                  CLA6_INT_FLAG_NEGEDGE: CLA6 unit negedge interrupt flag
                  CLA6_INT_FLAG_POSEDGE: CLA6 unit posedge interrupt flag
                  CLA7_INT_FLAG_NEGEDGE: CLA7 unit negedge interrupt flag
                  CLA7_INT_FLAG_POSEDGE: CLA7 unit posedge interrupt flag
    \param[out] none
    \retval     none
*/
FlagStatus cla_interrupt_flag_get(uint32_t int_flag)
{
    uint32_t cla_inte = 0U;
    uint32_t cla_intf = 0U;
    FlagStatus status;

    /* get the register value */
    switch(int_flag) {
    case CLA0_INT_FLAG_NEGEDGE:
        cla_inte = CLA_INTE & CLA_INTE_CLA0NIE;
        cla_intf = CLA_INTF & CLA_INTF_CLA0NF;
        break;
    case CLA0_INT_FLAG_POSEDGE:
        cla_inte = CLA_INTE & CLA_INTE_CLA0PIE;
        cla_intf = CLA_INTF & CLA_INTF_CLA0PF;
        break;
    case CLA1_INT_FLAG_NEGEDGE:
        cla_inte = CLA_INTE & CLA_INTE_CLA1NIE;
        cla_intf = CLA_INTF & CLA_INTF_CLA1NF;
        break;
    case CLA1_INT_FLAG_POSEDGE:
        cla_inte = CLA_INTE & CLA_INTE_CLA1PIE;
        cla_intf = CLA_INTF & CLA_INTF_CLA1PF;
        break;
    case CLA2_INT_FLAG_NEGEDGE:
        cla_inte = CLA_INTE & CLA_INTE_CLA2NIE;
        cla_intf = CLA_INTF & CLA_INTF_CLA2NF;
        break;
    case CLA2_INT_FLAG_POSEDGE:
        cla_inte = cla_inte & CLA_INTE_CLA2PIE;
        cla_intf = CLA_INTF & CLA_INTF_CLA2PF;
        break;
    case CLA3_INT_FLAG_NEGEDGE:
        cla_inte = CLA_INTE & CLA_INTE_CLA3NIE;
        cla_intf = CLA_INTF & CLA_INTF_CLA3NF;
        break;
    case CLA3_INT_FLAG_POSEDGE:
        cla_inte = CLA_INTE & CLA_INTE_CLA3PIE;
        cla_intf = CLA_INTF & CLA_INTF_CLA3PF;
        break;
    case CLA4_INT_FLAG_NEGEDGE:
        cla_inte = CLA_INTE & CLA_INTE_CLA4NIE;
        cla_intf = CLA_INTF & CLA_INTF_CLA4NF;
        break;
    case CLA4_INT_FLAG_POSEDGE:
        cla_inte = CLA_INTE & CLA_INTE_CLA4PIE;
        cla_intf = CLA_INTF & CLA_INTF_CLA4PF;
        break;
    case CLA5_INT_FLAG_NEGEDGE:
        cla_inte = CLA_INTE & CLA_INTE_CLA5NIE;
        cla_intf = CLA_INTF & CLA_INTF_CLA5NF;
        break;
    case CLA5_INT_FLAG_POSEDGE:
        cla_inte = CLA_INTE & CLA_INTE_CLA5PIE;
        cla_intf = CLA_INTF & CLA_INTF_CLA5PF;
        break;
    case CLA6_INT_FLAG_NEGEDGE:
        cla_inte = CLA_INTE & CLA_INTE_CLA6NIE;
        cla_intf = CLA_INTF & CLA_INTF_CLA6NF;
        break;
    case CLA6_INT_FLAG_POSEDGE:
        cla_inte = CLA_INTE & CLA_INTE_CLA6PIE;
        cla_intf = CLA_INTF & CLA_INTF_CLA6PF;
        break;
    case CLA7_INT_FLAG_NEGEDGE:
        cla_inte = CLA_INTE & CLA_INTE_CLA7NIE;
        cla_intf = CLA_INTF & CLA_INTF_CLA7NF;
        break;
    case CLA7_INT_FLAG_POSEDGE:
        cla_inte = CLA_INTE & CLA_INTE_CLA7PIE;
        cla_intf = CLA_INTF & CLA_INTF_CLA7PF;
        break;
    default:
        break;
    }
    if(cla_inte & cla_intf) {
        status = SET;
    } else {
        status = RESET;
    }
    return status;
}

/*!
    \brief      clear CLA interrupt flag (API_ID(0x0013U))
    \param[in]  int_flag: CLA interrupt flags,refer to cla_interrupt_flag_enum
                one or more parameters can be selected which is shown as below:
                  CLA0_INT_FLAG_NEGEDGE: CLA0 unit negedge interrupt flag
                  CLA0_INT_FLAG_POSEDGE: CLA0 unit posedge interrupt flag
                  CLA1_INT_FLAG_NEGEDGE: CLA1 unit negedge interrupt flag
                  CLA1_INT_FLAG_POSEDGE: CLA1 unit posedge interrupt flag
                  CLA2_INT_FLAG_NEGEDGE: CLA2 unit negedge interrupt flag
                  CLA2_INT_FLAG_POSEDGE: CLA2 unit posedge interrupt flag
                  CLA3_INT_FLAG_NEGEDGE: CLA3 unit negedge interrupt flag
                  CLA3_INT_FLAG_POSEDGE: CLA3 unit posedge interrupt flag
                  CLA4_INT_FLAG_NEGEDGE: CLA4 unit negedge interrupt flag
                  CLA4_INT_FLAG_POSEDGE: CLA4 unit posedge interrupt flag
                  CLA5_INT_FLAG_NEGEDGE: CLA5 unit negedge interrupt flag
                  CLA5_INT_FLAG_POSEDGE: CLA5 unit posedge interrupt flag
                  CLA6_INT_FLAG_NEGEDGE: CLA6 unit negedge interrupt flag
                  CLA6_INT_FLAG_POSEDGE: CLA6 unit posedge interrupt flag
                  CLA7_INT_FLAG_NEGEDGE: CLA7 unit negedge interrupt flag
                  CLA7_INT_FLAG_POSEDGE: CLA7 unit posedge interrupt flag
    \param[out] none
    \retval     none
*/
void cla_interrupt_flag_clear(uint32_t int_flag)
{
    CLA_INTF &= ~(int_flag & CLA_FLAG_MASK);
}


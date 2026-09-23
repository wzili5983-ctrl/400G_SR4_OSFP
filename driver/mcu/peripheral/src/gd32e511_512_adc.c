/*!
    \file    gd32e511_512_adc.c
    \brief   ADC driver

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

#include "gd32e511_512_adc.h"

#define ADC_CHANNEL_INTERNAL_MASK                  ((uint32_t)0x19000000U)
#define ADC_LATCH_DATA_MASK                        ((uint32_t)0x00000003U)
#define ADC_FLAG_MASK                              ((uint32_t)0x0000007FU)
#define ADC_INT_MASK                               ((uint32_t)0x0C0000E0U)
#define ADC_INT_FLAG_MASK                          ((uint32_t)0x00000067U)

/*!
    \brief      reset ADC (API_ID(0x0001U))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[out] none
    \retval     none
*/
void adc_deinit(uint32_t adc_periph)
{
    switch(adc_periph) {
    case ADC0:
        rcu_periph_reset_enable(RCU_ADC0RST);
        rcu_periph_reset_disable(RCU_ADC0RST);
        break;
    case ADC1:
        rcu_periph_reset_enable(RCU_ADC1RST);
        rcu_periph_reset_disable(RCU_ADC1RST);
        break;
    default:
        break;
    }
}

/*!
    \brief      enable ADC interface (API_ID(0x0002U))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[out] none
    \retval     none
*/
void adc_enable(uint32_t adc_periph)
{
    if(RESET == (ADC_CTL1(adc_periph) & ADC_CTL1_ADCON)) {
        ADC_CTL1(adc_periph) |= (uint32_t)ADC_CTL1_ADCON;
    }
}

/*!
    \brief      disable ADC interface (API_ID(0x0003U))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[out] none
    \retval     none
*/
void adc_disable(uint32_t adc_periph)
{
    ADC_CTL1(adc_periph) &= ~((uint32_t)ADC_CTL1_ADCON);
}

/*!
    \brief      ADC calibration and reset calibration (API_ID(0x0004U))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[out] none
    \retval     none
    \note     This function contains scenarios leading to an infinite loop.
                Modify according to the user's actual usage scenarios.
*/
void adc_calibration_enable(uint32_t adc_periph)
{
    /* reset the selected ADC calibration register */
    ADC_CTL1(adc_periph) |= (uint32_t) ADC_CTL1_RSTCLB;
    /* check the RSTCLB bit state */
    while((ADC_CTL1(adc_periph) & ADC_CTL1_RSTCLB)) {
    }

    /* enable ADC calibration process */
    ADC_CTL1(adc_periph) |= ADC_CTL1_CLB;
    /* check the CLB bit state */
    while((ADC_CTL1(adc_periph) & ADC_CTL1_CLB)) {
    }
}

/*!
    \brief      enable DMA request (API_ID(0x0005U))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[in]  adc_sequence: select the sequence
                only one parameter can be selected which is shown as below:
      \arg        ADC_ROUTINE_CHANNEL: routine sequence
      \arg        ADC_INSERTED_CHANNEL: inserted sequence
    \param[out] none
    \retval     none
*/
void adc_dma_mode_enable(uint32_t adc_periph, uint8_t adc_sequence)
{
    switch(adc_sequence) {
    case ADC_ROUTINE_CHANNEL:
        /* DMA request enable for routine channel */
        ADC_CTL1(adc_periph) |= (uint32_t)(ADC_CTL1_RDMA);
        break;
    case ADC_INSERTED_CHANNEL:
        /* DMA request enable for inserted channel */
        ADC_CTL2(adc_periph) |= (uint32_t)(ADC_CTL2_IDMA);
        break;
    default:
        break;
    }
}

/*!
    \brief      disable DMA request (API_ID(0x0006U))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[in]  adc_sequence: select the sequence
                only one parameter can be selected which is shown as below:
      \arg        ADC_ROUTINE_CHANNEL: routine sequence
      \arg        ADC_INSERTED_CHANNEL: inserted sequence
    \param[out] none
    \retval     none
*/
void adc_dma_mode_disable(uint32_t adc_periph, uint8_t adc_sequence)
{
    switch(adc_sequence) {
    case ADC_ROUTINE_CHANNEL:
        /* DMA request disable for routine channel */
        ADC_CTL1(adc_periph) &= ~((uint32_t)ADC_CTL1_RDMA);
        break;
    case ADC_INSERTED_CHANNEL:
        /* DMA request disable for inserted channel */
        ADC_CTL2(adc_periph) &= ~((uint32_t)ADC_CTL2_IDMA);
        break;
    default:
        break;
    }
}

/*!
    \brief      configure end of conversion mode (API_ID(0x0007U))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[in]  end_selection: end of conversion mode
                only one parameter can be selected which is shown as below:
      \arg        ADC_EORC_SET_SEQUENCE: only at the end of a sequence of routine conversions, the EORC bit is set.
      \arg        ADC_EORC_SET_CONVERSION: at the end of each routine conversion, the EOC bit is set.
    \param[out] none
    \retval     none
*/
void adc_end_of_conversion_config(uint32_t adc_periph, uint32_t end_selection)
{
    /* only at the end of a sequence of routine conversions, the EORC bit is set */
    ADC_CTL1(adc_periph) &= ~((uint32_t)ADC_CTL1_EORCM);
    /* at the end of each routine conversion, the EORC bit is set. Overflow is detected automatically */
    ADC_CTL1(adc_periph) |= end_selection & ADC_CTL1_EORCM;
}

/*!
    \brief      enable or disable ADC internal channels (API_ID(0x0008U))
    \param[in]  internal_channel: the internal channels
                only one parameter can be selected which is shown as below:
      \arg        ADC_CHANNEL_INTERNAL_VREFINT: vrefint channel
      \arg        ADC_CHANNEL_INTERNAL_VDDA: vdda channel
      \arg        ADC_CHANNEL_INTERNAL_HP_TEMPSENSOR: high-precision temperature sensor channel
    \param[in]  newvalue: ENABLE or DISABLE
    \param[out] none
    \retval     none
*/
void adc_internal_channel_config(uint32_t internal_channel, ControlStatus newvalue)
{
    if(ENABLE == newvalue) {
        ADC_CTL1(ADC0) |= (uint32_t)(internal_channel & ADC_CHANNEL_INTERNAL_MASK);
    } else {
        ADC_CTL1(ADC0) &= ~((uint32_t)(internal_channel & ADC_CHANNEL_INTERNAL_MASK));
        if((internal_channel == ADC_CHANNEL_INTERNAL_VREFINT) || (internal_channel == ADC_CHANNEL_INTERNAL_HP_TEMPSENSOR)) {
            ADC_CTL1(ADC0) &= ~(ADC_CTL1_TSVREN);
        }
    }
}

/*!
    \brief      configure ADC discontinuous mode (API_ID(0x0009U))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[in]  adc_sequence: select the sequence
                only one parameter can be selected which is shown as below:
      \arg        ADC_ROUTINE_CHANNEL: routine sequence
      \arg        ADC_INSERTED_CHANNEL: inserted sequence
      \arg        ADC_CHANNEL_DISCON_DISABLE: disable discontinuous mode of routine and inserted channel
    \param[in]  length: number of conversions in discontinuous mode, the number can be 1..8
                        for routine channel, the number has no effect for inserted channel
    \param[out] none
    \retval     none
*/
void adc_discontinuous_mode_config(uint32_t adc_periph, uint8_t adc_sequence, uint8_t length)
{
    ADC_CTL0(adc_periph) &= ~((uint32_t)(ADC_CTL0_DISRC | ADC_CTL0_DISIC));

    switch(adc_sequence) {
    case ADC_ROUTINE_CHANNEL:
        /* configure the number of conversions in discontinuous mode */
        ADC_CTL0(adc_periph) &= ~((uint32_t)ADC_CTL0_DISNUM);
        if((length <= 8U) && (length >= 1U)) {
            ADC_CTL0(adc_periph) |= CTL0_DISNUM(((uint32_t)length - 1U));
        }
        /* enable routine sequence discontinuous mode */
        ADC_CTL0(adc_periph) |= (uint32_t)ADC_CTL0_DISRC;
        break;
    case ADC_INSERTED_CHANNEL:
        /* enable inserted sequence discontinuous mode */
        ADC_CTL0(adc_periph) |= (uint32_t)ADC_CTL0_DISIC;
        break;
    case ADC_CHANNEL_DISCON_DISABLE:
        /* disable discontinuous mode of routine & inserted sequence */
        break;
    default:
        break;
    }
}

/*!
    \brief      configure ADC special function (API_ID(0x000AU))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[in]  function: the function to configure
                one or more parameters can be selected which is shown as below:
      \arg        ADC_SCAN_MODE: scan mode select
      \arg        ADC_INSERTED_CHANNEL_AUTO: inserted sequence convert automatically
      \arg        ADC_CONTINUOUS_MODE: continuous mode select
    \param[in]  newvalue: ENABLE or DISABLE
    \param[out] none
    \retval     none
*/
void adc_special_function_config(uint32_t adc_periph, uint32_t function, ControlStatus newvalue)
{
    if(ENABLE == newvalue) {
        /* enable ADC scan mode */
        if(0U != (function & ADC_SCAN_MODE)) {
            ADC_CTL0(adc_periph) |= (uint32_t)ADC_SCAN_MODE;
        }
        /* enable ADC inserted sequence convert automatically */
        if(0U != (function & ADC_INSERTED_CHANNEL_AUTO)) {
            ADC_CTL0(adc_periph) |= (uint32_t)ADC_INSERTED_CHANNEL_AUTO;
        }
        /* enable ADC continuous mode */
        if(0U != (function & ADC_CONTINUOUS_MODE)) {
            ADC_CTL1(adc_periph) |= (uint32_t)ADC_CONTINUOUS_MODE;
        }
    } else {
        /* disable ADC scan mode */
        if(0U != (function & ADC_SCAN_MODE)) {
            ADC_CTL0(adc_periph) &= ~((uint32_t)ADC_SCAN_MODE);
        }
        /* disable ADC inserted sequence convert automatically */
        if(0U != (function & ADC_INSERTED_CHANNEL_AUTO)) {
            ADC_CTL0(adc_periph) &= ~((uint32_t)ADC_INSERTED_CHANNEL_AUTO);
        }
        /* disable ADC continuous mode */
        if(0U != (function & ADC_CONTINUOUS_MODE)) {
            ADC_CTL1(adc_periph) &= ~((uint32_t)ADC_CONTINUOUS_MODE);
        }
    }
}

/*!
    \brief      configure ADC data alignment (API_ID(0x000BU))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[in]  data_alignment: data alignment select
                only one parameter can be selected which is shown as below:
      \arg        ADC_DATAALIGN_RIGHT: right alignment
      \arg        ADC_DATAALIGN_LEFT: left alignment
    \param[out] none
    \retval     none
*/
void adc_data_alignment_config(uint32_t adc_periph, uint32_t data_alignment)
{
    ADC_CTL1(adc_periph) &= ~((uint32_t)ADC_CTL1_DAL);
    ADC_CTL1(adc_periph) |= data_alignment & ADC_CTL1_DAL;
}

/*!
    \brief      configure the length of routine sequence or inserted sequence (API_ID(0x000CU))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[in]  adc_sequence: select the sequence
                only one parameter can be selected which is shown as below:
      \arg        ADC_ROUTINE_CHANNEL: routine sequence
      \arg        ADC_INSERTED_CHANNEL: inserted sequence
    \param[in]  length: the length of the channel
                        routine channel 1-25 for ADC0,1-20 for ADC1
                        inserted channel 1-4
    \param[out] none
    \retval     none
*/
void adc_channel_length_config(uint32_t adc_periph, uint8_t adc_sequence, uint32_t length)
{
    switch(adc_sequence) {
    case ADC_ROUTINE_CHANNEL:
        /* configure the length of routine sequence */
        if((length >= 1U) && (((adc_periph == ADC0) && (length <= 25U)) || \
                              ((adc_periph == ADC1) && (length <= 20U)))) {
            ADC_RSQ0(adc_periph) &= ~((uint32_t)ADC_RSQ0_RL);
            ADC_RSQ0(adc_periph) |= RSQ0_RL((uint32_t)(length - 1U));
        }
        break;
    case ADC_INSERTED_CHANNEL:
        /* configure the length of inserted sequence */
        if((length >= 1U) && (length <= 4U)) {
            ADC_ISQ(adc_periph) &= ~((uint32_t)ADC_ISQ_IL);
            ADC_ISQ(adc_periph) |= ISQ_IL((uint32_t)(length - 1U));
        }
        break;
    default:
        break;
    }
}

/*!
    \brief      configure ADC routine channel (API_ID(0x000DU))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[in]  rank: the routine sequence rank, this parameter must be between 0 to 24 for ADC0 and between 0 to 19 for ADC1
    \param[in]  adc_channel: the selected ADC channel
                only one parameter can be selected which is shown as below:
      \arg        ADC_CHANNEL_x: ADC Channelx
                  x must be between 0 to 24 for ADC0 and between 0 to 19 for ADC1
    \param[in]  sample_time: the sample time value
                only one parameter can be selected which is shown as below:
      \arg        ADC_SAMPLETIME_1POINT5: 1.5 cycles
      \arg        ADC_SAMPLETIME_7POINT5: 7.5 cycles
      \arg        ADC_SAMPLETIME_13POINT5: 13.5 cycles
      \arg        ADC_SAMPLETIME_28POINT5: 28.5 cycles
      \arg        ADC_SAMPLETIME_41POINT5: 41.5 cycles
      \arg        ADC_SAMPLETIME_55POINT5: 55.5 cycles
      \arg        ADC_SAMPLETIME_71POINT5: 71.5 cycles
      \arg        ADC_SAMPLETIME_239POINT5: 239.5 cycles
      \arg        ADC_SAMPLETIME_3POINT5: 3.5 cycles
      \arg        ADC_SAMPLETIME_7POINT5: 9.5 cycles
      \arg        ADC_SAMPLETIME_479POINT5: 479.5 cycles
    \param[out] none
    \retval     none
*/
void adc_routine_channel_config(uint32_t adc_periph, uint8_t rank, uint8_t adc_channel, uint32_t sample_time)
{
    uint32_t rsq, sampt;

    if(((adc_periph == ADC0) && (rank < 25U) && (adc_channel <= ADC_CHANNEL_24)) || \
            ((adc_periph == ADC1) && (rank < 20U) && (adc_channel <= ADC_CHANNEL_19))) {

        /* configure ADC routine sequence */
        if(rank < 16U) {
            rsq = ADC_RSQ(adc_periph, (2U - rank / 6U));
            rsq &=  ~((uint32_t)(ADC_RSQX_RSQN << (5U * (rank % 6U))));
            rsq |= ((uint32_t)adc_channel << (5U * (rank % 6U)));
            ADC_RSQ(adc_periph, (2U - rank / 6U)) = rsq;
        } else if(rank == 16U) {
            rsq = ADC_RSQ(adc_periph, 0U);
            rsq &= ~((uint32_t)(ADC_RSQX_RSQN << (25U)));
            rsq |= ((uint32_t)adc_channel << (25U));
            ADC_RSQ(adc_periph, 0U) = rsq;
        } else {
            rsq = ADC_RSQ3_4(adc_periph, (rank + 1U) / 6U);
            rsq &= ~((uint32_t)(ADC_RSQX_RSQN << (5U * ((rank + 1U) % 6U))));
            rsq |= ((uint32_t)adc_channel << (5U * ((rank + 1U) % 6U)));
            ADC_RSQ3_4(adc_periph, (rank + 1U) / 6U) = rsq;
        }


        /* configure ADC sampling time */
        if(adc_channel <= ADC_CHANNEL_19) {
            sampt = ADC_SAMPT(adc_periph, (1U - adc_channel / 10U));
            sampt &= ~((uint32_t)(ADC_SAMPTX_SPTN << (3U * (adc_channel % 10U))));
            sampt |= (uint32_t)((sample_time & ADC_SAMPTX_SPTN) << (3U * (adc_channel % 10U)));
            ADC_SAMPT(adc_periph, (1U - adc_channel / 10U)) = sampt;
        } else if((adc_periph == ADC0) && (adc_channel <= ADC_CHANNEL_24)) {
            sampt = ADC_SAMPT2(adc_periph);
            sampt &= ~((uint32_t)(ADC_SAMPTX_SPTN << (3U * (adc_channel - 20U))));
            sampt |= (uint32_t)((sample_time & ADC_SAMPTX_SPTN) << (3U * (adc_channel - 20U)));
            ADC_SAMPT2(adc_periph) = sampt;
        } else {
            /* illegal parameters */
        }
        /* support 3.5/9.5/479.5 sampling cycles */
        if(sample_time & SAMPT3_SPT) {
            sampt = ADC_ESAMPT(adc_periph);
            sampt &= ~((uint32_t)1U << adc_channel);
            sampt |= ((uint32_t)1U << adc_channel);
            ADC_ESAMPT(adc_periph) = sampt;
        }
    }
}

/*!
    \brief      configure ADC inserted channel (API_ID(0x000EU))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[in]  rank: the inserted sequencer rank,this parameter must be between 0 to 3
    \param[in]  adc_channel: the selected ADC channel
                only one parameter can be selected which is shown as below:
      \arg        ADC_CHANNEL_x: ADC Channelx
                  x must be between 0 to 24 for ADC0 and between 0 to 19 for ADC1
    \param[in]  sample_time: The sample time value
                only one parameter can be selected which is shown as below:
      \arg        ADC_SAMPLETIME_1POINT5: 1.5 cycles
      \arg        ADC_SAMPLETIME_7POINT5: 7.5 cycles
      \arg        ADC_SAMPLETIME_13POINT5: 13.5 cycles
      \arg        ADC_SAMPLETIME_28POINT5: 28.5 cycles
      \arg        ADC_SAMPLETIME_41POINT5: 41.5 cycles
      \arg        ADC_SAMPLETIME_55POINT5: 55.5 cycles
      \arg        ADC_SAMPLETIME_71POINT5: 71.5 cycles
      \arg        ADC_SAMPLETIME_239POINT5: 239.5 cycles
      \arg        ADC_SAMPLETIME_3POINT5: 3.5 cycles, 3239H exclusive function
      \arg        ADC_SAMPLETIME_9POINT5: 9.5 cycles, 3239H exclusive function
      \arg        ADC_SAMPLETIME_479POINT5: 479.5 cycles, 3239H exclusive function
    \param[out] none
    \retval     none
*/
void adc_inserted_channel_config(uint32_t adc_periph, uint8_t rank, uint8_t adc_channel, uint32_t sample_time)
{
    uint8_t inserted_length;
    uint32_t isq, sampt;
    if(((adc_periph == ADC0) && (adc_channel <= ADC_CHANNEL_24)) || \
            ((adc_periph == ADC1) && (adc_channel <= ADC_CHANNEL_19))) {

        /* configure ADC inserted sequence */
        if(rank < 4U) {
            inserted_length = (uint8_t)GET_BITS(ADC_ISQ(adc_periph), 20U, 21U);
            isq = ADC_ISQ(adc_periph);
            isq &= ~((uint32_t)(ADC_ISQ_ISQN << (15U - (inserted_length - rank) * 5U)));
            isq |= ((uint32_t)adc_channel << (15U - (inserted_length - rank) * 5U));
            ADC_ISQ(adc_periph) = isq;
        }

        /* configure ADC sampling time */
        if(adc_channel <= ADC_CHANNEL_19) {
            sampt = ADC_SAMPT(adc_periph, (1U - adc_channel / 10U));
            sampt &= ~((uint32_t)(ADC_SAMPTX_SPTN << (3U * (adc_channel % 10U))));
            sampt |= (uint32_t)((sample_time & ADC_SAMPTX_SPTN) << (3U * (adc_channel % 10U)));
            ADC_SAMPT(adc_periph, (1U - adc_channel / 10U)) = sampt;
        } else if((adc_periph == ADC0) && (adc_channel <= ADC_CHANNEL_24)) {
            sampt = ADC_SAMPT2(adc_periph);
            sampt &= ~((uint32_t)(ADC_SAMPTX_SPTN << (3U * (adc_channel - 20U))));
            sampt |= (uint32_t)((sample_time & ADC_SAMPTX_SPTN) << (3U * (adc_channel - 20U)));
            ADC_SAMPT2(adc_periph) = sampt;
        } else {
            /* illegal parameters */
        }
        /* support 3.5/9.5/479.5 sampling cycles */
        if(sample_time & SAMPT3_SPT) {
            sampt = ADC_ESAMPT(adc_periph);
            sampt &= ~((uint32_t)1U << adc_channel);
            sampt |= ((uint32_t)1U << adc_channel);
            ADC_ESAMPT(adc_periph) = sampt;
        }
    }
}

/*!
    \brief      configure ADC inserted channel offset. inserted_channel must be less or equal than inserted_length, otherwise function does nothing (API_ID(0x000FU))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[in]  inserted_channel: insert channel select
                only one parameter can be selected which is shown as below:
      \arg        ADC_INSERTED_CHANNEL_0: ADC inserted channel 0
      \arg        ADC_INSERTED_CHANNEL_1: ADC inserted channel 1
      \arg        ADC_INSERTED_CHANNEL_2: ADC inserted channel 2
      \arg        ADC_INSERTED_CHANNEL_3: ADC inserted channel 3
    \param[in]  offset: the offset data
    \param[out] none
    \retval     none
*/
void adc_inserted_channel_offset_config(uint32_t adc_periph, uint8_t inserted_channel, uint16_t offset)
{
    uint8_t inserted_length;
    uint32_t num = 0U;

    inserted_length = (uint8_t)GET_BITS(ADC_ISQ(adc_periph), 20U, 21U);
    num = 3U - (inserted_length - inserted_channel);

    if(num <= 3U) {
        /* calculate the offset of the register */
        num = num * 4U;
        /* configure the offset of the selected channels */
        REG32((adc_periph) + 0x14U + num) = IOFFX_IOFF((uint32_t)offset);
    }
}

/*!
    \brief      configure ADC inserted sequence repeat times (API_ID(0x0010U))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[in]  time: a value between 0 and 63
    \param[out] none
    \retval     none
*/
void adc_inserted_channel_repeat_time_config(uint32_t adc_periph, uint8_t time)
{
    ADC_CTL2(adc_periph) &= (~(uint32_t)ADC_CTL2_IREP);
    ADC_CTL2(adc_periph) |= (uint32_t)(((uint32_t)time << 2U) & ADC_CTL2_IREP);
}

/*!
    \brief      configure ADC external trigger (API_ID(0x0011U))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[in]  adc_sequence: select the sequence
                only one parameter can be selected which is shown as below:
      \arg        ADC_ROUTINE_CHANNEL: routine sequence
      \arg        ADC_INSERTED_CHANNEL: inserted sequence
    \param[in]  newvalue: ENABLE or DISABLE
    \param[out] none
    \retval     none
*/
void adc_external_trigger_config(uint32_t adc_periph, uint8_t adc_sequence, ControlStatus newvalue)
{
    if(ENABLE == newvalue) {
        /* external trigger enable for routine channel */
        if(0U != (adc_sequence & ADC_ROUTINE_CHANNEL)) {
            ADC_CTL1(adc_periph) |= ADC_CTL1_ETMRC;
        }
        /* external trigger enable for inserted channel */
        if(0U != (adc_sequence & ADC_INSERTED_CHANNEL)) {
            ADC_CTL1(adc_periph) |= ADC_CTL1_ETMIC;
        }
    } else {
        /* external trigger disable for routine channel */
        if(0U != (adc_sequence & ADC_ROUTINE_CHANNEL)) {
            ADC_CTL1(adc_periph) &= ~ADC_CTL1_ETMRC;
        }
        /* external trigger disable for inserted channel */
        if(0U != (adc_sequence & ADC_INSERTED_CHANNEL)) {
            ADC_CTL1(adc_periph) &= ~ADC_CTL1_ETMIC;
        }
    }
}

/*!
    \brief      configure ADC external trigger source (API_ID(0x0012U))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[in]  adc_sequence: select the sequence
                only one parameter can be selected which is shown as below:
      \arg        ADC_ROUTINE_CHANNEL: routine sequence
      \arg        ADC_INSERTED_CHANNEL: inserted sequence
    \param[in]  external_trigger_source: routine or inserted sequence trigger source
                only one parameter can be selected which is shown as below:
                for routine channel:
      \arg        ADC_EXTTRIG_ROUTINE_T0_CH0: TIMER0 CH0 event select
      \arg        ADC_EXTTRIG_ROUTINE_T0_CH1: TIMER0 CH1 event select
      \arg        ADC_EXTTRIG_ROUTINE_T0_CH2: TIMER0 CH2 event select
      \arg        ADC_EXTTRIG_ROUTINE_T1_CH1: TIMER1 CH1 event select
      \arg        ADC_EXTTRIG_ROUTINE_T2_TRGO: TIMER2 TRGO event select
      \arg        ADC_EXTTRIG_ROUTINE_T14_CH0: TIMER14 CH0 event select
      \arg        ADC_EXTTRIG_ROUTINE_EXTI_11: external interrupt line 11
      \arg        ADC_EXTTRIG_ROUTINE_NONE: software trigger
      \arg        ADC_EXTTRIG_ROUTINE_CLA0: CLA0 output
      \arg        ADC_EXTTRIG_ROUTINE_CLA2: CLA2 output
                for inserted channel:
      \arg        ADC_EXTTRIG_INSERTED_T0_TRGO: TIMER0 TRGO event select
      \arg        ADC_EXTTRIG_INSERTED_T0_CH3: TIMER0 CH3 event select
      \arg        ADC_EXTTRIG_INSERTED_T1_TRGO: TIMER1 TRGO event select
      \arg        ADC_EXTTRIG_INSERTED_T1_CH0: TIMER1 CH0 event select
      \arg        ADC_EXTTRIG_INSERTED_T2_CH3: TIMER2 CH3 event select
      \arg        ADC_EXTTRIG_INSERTED_T14_TRGO: TIMER14 TRGO event select
      \arg        ADC_EXTTRIG_INSERTED_EXTI_15: external interrupt line 15
      \arg        ADC_EXTTRIG_INSERTED_NONE: software trigger
      \arg        ADC_EXTTRIG_INSERTED_CLA1: CLA1 output
      \arg        ADC_EXTTRIG_INSERTED_CLA3: CLA3 output
    \param[out] none
    \retval     none
*/
void adc_external_trigger_source_config(uint32_t adc_periph, uint8_t adc_sequence, uint32_t external_trigger_source)
{
    switch(adc_sequence) {
    case ADC_ROUTINE_CHANNEL:
        /* external trigger select for routine channel */
        ADC_CTL1(adc_periph) &= ~((uint32_t)ADC_CTL1_ETSRC);
        ADC_CTL1(adc_periph) |= (uint32_t)((uint32_t)(external_trigger_source) & ADC_CTL1_ETSRC);
        break;
    case ADC_INSERTED_CHANNEL:
        /* external trigger select for inserted channel */
        ADC_CTL1(adc_periph) &= ~((uint32_t)ADC_CTL1_ETSIC);
        ADC_CTL1(adc_periph) |= (uint32_t)((uint32_t)(external_trigger_source) &  ADC_CTL1_ETSIC);
        break;
    default:
        break;
    }
}

/*!
    \brief      enable ADC software trigger (API_ID(0x0013U))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[in]  adc_sequence: select the sequence
                only one parameter can be selected which is shown as below:
      \arg        ADC_ROUTINE_CHANNEL: routine sequence
      \arg        ADC_INSERTED_CHANNEL: inserted sequence
    \param[out] none
    \retval     none
*/
void adc_software_trigger_enable(uint32_t adc_periph, uint8_t adc_sequence)
{
    /* enable routine sequence software trigger */
    if(0U != (adc_sequence & ADC_ROUTINE_CHANNEL)) {
        ADC_CTL1(adc_periph) |= ADC_CTL1_SWRCST;
    }

    /* enable inserted sequence software trigger */
    if(0U != (adc_sequence & ADC_INSERTED_CHANNEL)) {
        ADC_CTL1(adc_periph) |= ADC_CTL1_SWICST;
    }
}

/*!
    \brief      read ADC routine sequence data register (API_ID(0x0014U))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[out] none
    \retval     the conversion value:
*/
uint16_t adc_routine_data_read(uint32_t adc_periph)
{
    return (uint16_t)(ADC_RDATA(adc_periph));
}

/*!
    \brief      read ADC inserted sequence data register (API_ID(0x0015U))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[out] none
    \retval     the conversion value:
*/
uint16_t adc_inserted_data_read(uint32_t adc_periph)
{
    return (uint16_t)(ADC_IDATA(adc_periph));
}

/*!
    \brief      read ADC latch data register (API_ID(0x0016U))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[in]  latch_data: latch data select
                only one parameter can be selected which is shown as below:
      \arg        ADC_LATCH_DATA_0: ADC latch data 0
      \arg        ADC_LATCH_DATA_1: ADC latch data 1
      \arg        ADC_LATCH_DATA_2: ADC latch data 2
      \arg        ADC_LATCH_DATA_3: ADC latch data 3
    \param[out] none
    \retval     the conversion value
*/
uint16_t adc_latch_data_read(uint32_t adc_periph, uint8_t latch_data)
{
    uint32_t ldata = 0U;

    /* read the data of the selected channel */
    switch(latch_data) {
    case ADC_LATCH_DATA_0:
        ldata = ADC_LDATA0(adc_periph);
        break;
    case ADC_LATCH_DATA_1:
        ldata = ADC_LDATA1(adc_periph);
        break;
    case ADC_LATCH_DATA_2:
        ldata = ADC_LDATA2(adc_periph);
        break;
    case ADC_LATCH_DATA_3:
        ldata = ADC_LDATA3(adc_periph);
        break;
    default:
        break;
    }
    return (uint16_t)ldata;
}

/*!
    \brief      configure ADC latch data source (API_ID(0x0017U))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[in]  latch_data: latch data select
                only one parameter can be selected which is shown as below:
      \arg        ADC_LATCH_DATA_0: ADC latch data 0
      \arg        ADC_LATCH_DATA_1: ADC latch data 1
      \arg        ADC_LATCH_DATA_2: ADC latch data 2
      \arg        ADC_LATCH_DATA_3: ADC latch data 3
    \param[in]  adc_sequence: select the sequence
                only one parameter can be selected which is shown as below:
      \arg        ADC_ROUTINE_CHANNEL: routine sequence
      \arg        ADC_INSERTED_CHANNEL: inserted sequence
    \param[in]  rank: the routine sequence rank, this parameter must be between 0 to 24 for ADC0 and between 0 to 19 for ADC1
                      the inserted sequencer rank,this parameter must be between 0 to 3
    \param[out] none
    \retval     none
*/
void adc_latch_data_source_config(uint32_t adc_periph, uint8_t latch_data, uint8_t adc_sequence, uint8_t rank)
{
    uint32_t ldctl;
    ldctl = ADC_LDCTL(adc_periph);
    ldctl &= ~((ADC_LDCTL_COVSELx | ADC_LDCTL_SEQSELx) << (24U - (latch_data & ADC_LATCH_DATA_MASK) * 8U));

    switch(adc_sequence) {
    case ADC_ROUTINE_CHANNEL:
        /* ADC latch data source selection for routine channel */
        ldctl |= (ADC_LDCTL_SEQSELx << (24U - (latch_data & ADC_LATCH_DATA_MASK) * 8U));
        if(((adc_periph == ADC0) && (rank < 25U)) || ((adc_periph == ADC1) && (rank < 20U))) {
            ldctl |= ((uint32_t)rank << (24U - (latch_data & ADC_LATCH_DATA_MASK) * 8U));
        }
        break;
    case ADC_INSERTED_CHANNEL:
        /* ADC latch data source selection for inserted channel */
        if(rank < 4U) {
            ldctl |= ((uint32_t)rank << (24U - (latch_data & ADC_LATCH_DATA_MASK) * 8U));
        }
        break;
    default:
        break;
    }

    ADC_LDCTL(adc_periph) = ldctl;
}


/*!
    \brief      configure ADC analog watchdog 0 single channel (API_ID(0x0018U))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[in]  adc_channel: the selected ADC channel
                only one parameter can be selected which is shown as below:
      \arg        ADC_CHANNEL_x: ADC Channelx
                  x must be between 0 to 24 for ADC0 and between 0 to 19 for ADC1
    \param[out] none
    \retval     none
*/
void adc_watchdog0_single_channel_enable(uint32_t adc_periph, uint8_t adc_channel)
{
    ADC_CTL0(adc_periph) &= (uint32_t)~(ADC_CTL0_RWD0EN | ADC_CTL0_IWD0EN | ADC_CTL0_WD0SC | ADC_CTL0_WD0CHSEL);

    if(((adc_periph == ADC0) && (adc_channel <= ADC_CHANNEL_24)) || \
            ((adc_periph == ADC1) && (adc_channel <= ADC_CHANNEL_19))) {
        ADC_CTL0(adc_periph) |= (uint32_t)adc_channel;
        ADC_CTL0(adc_periph) |= (uint32_t)(ADC_CTL0_RWD0EN | ADC_CTL0_IWD0EN | ADC_CTL0_WD0SC);
    }
}

/*!
    \brief      configure ADC analog watchdog 0 sequence channel (API_ID(0x0019U))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[in]  adc_sequence: the sequence use analog watchdog 0
                only one parameter can be selected which is shown as below:
      \arg        ADC_ROUTINE_CHANNEL: routine sequence
      \arg        ADC_INSERTED_CHANNEL: inserted sequence
      \arg        ADC_ROUTINE_INSERTED_CHANNEL: both routine and inserted sequence
    \param[out] none
    \retval     none
*/
void adc_watchdog0_sequence_channel_enable(uint32_t adc_periph, uint8_t adc_sequence)
{
    ADC_CTL0(adc_periph) &= (uint32_t)~(ADC_CTL0_RWD0EN | ADC_CTL0_IWD0EN | ADC_CTL0_WD0SC);

    /* select the sequence */
    switch(adc_sequence) {
    case ADC_ROUTINE_CHANNEL:
        ADC_CTL0(adc_periph) |= (uint32_t)ADC_CTL0_RWD0EN;
        break;
    case ADC_INSERTED_CHANNEL:
        ADC_CTL0(adc_periph) |= (uint32_t)ADC_CTL0_IWD0EN;
        break;
    case ADC_ROUTINE_INSERTED_CHANNEL:
        ADC_CTL0(adc_periph) |= (uint32_t)(ADC_CTL0_RWD0EN | ADC_CTL0_IWD0EN);
        break;
    default:
        break;
    }
}

/*!
    \brief      disable ADC analog watchdog 0 (API_ID(0x001AU))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[out] none
    \retval     none
*/
void adc_watchdog0_disable(uint32_t adc_periph)
{
    ADC_CTL0(adc_periph) &= (uint32_t)~(ADC_CTL0_RWD0EN | ADC_CTL0_IWD0EN | ADC_CTL0_WD0SC | ADC_CTL0_WD0CHSEL);
}

/*!
    \brief      configure ADC analog watchdog 0 threshold (API_ID(0x001BU))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[in]  low_threshold: analog watchdog 0 low threshold, 0..2^20-1
    \param[in]  high_threshold: analog watchdog 0 high threshold, 0..2^20-1
    \param[out] none
    \retval     none
*/
void adc_watchdog0_threshold_config(uint32_t adc_periph, uint32_t low_threshold, uint32_t high_threshold)
{
    ADC_WD0LT(adc_periph) = (uint32_t)WD0LT_WD0LT(low_threshold);
    ADC_WD0HT(adc_periph) = (uint32_t)WD0HT_WD0HT(high_threshold);
}

/*!
    \brief      configure ADC resolution (API_ID(0x001CU))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[in]  resolution: ADC resolution
                only one parameter can be selected which is shown as below:
      \arg        ADC_RESOLUTION_12B: 12-bit ADC resolution
      \arg        ADC_RESOLUTION_10B: 10-bit ADC resolution
      \arg        ADC_RESOLUTION_8B: 8-bit ADC resolution
      \arg        ADC_RESOLUTION_6B: 6-bit ADC resolution
    \param[out] none
    \retval     none
*/
void adc_resolution_config(uint32_t adc_periph, uint32_t resolution)
{
    ADC_CTL0(adc_periph) &= ~((uint32_t)ADC_CTL0_DRES);
    ADC_CTL0(adc_periph) |= (uint32_t)(resolution & ADC_CTL0_DRES);
}

/*!
    \brief      configure ADC oversample mode (API_ID(0x001DU))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[in]  mode: ADC oversampling mode
                only one parameter can be selected which is shown as below:
      \arg        ADC_OVERSAMPLING_ALL_CONVERT: all oversampled conversions for a channel are done consecutively after a trigger
      \arg        ADC_OVERSAMPLING_ONE_CONVERT: each oversampled conversion for a channel needs a trigger
    \param[in]  shift: ADC oversampling shift
                only one parameter can be selected which is shown as below:
      \arg        ADC_OVERSAMPLING_SHIFT_NONE: no oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_1B: 1-bit oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_2B: 2-bit oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_3B: 3-bit oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_4B: 3-bit oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_5B: 5-bit oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_6B: 6-bit oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_7B: 7-bit oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_8B: 8-bit oversampling shift
    \param[in]  ratio: ADC oversampling ratio
                only one parameter can be selected which is shown as below:
      \arg        ADC_OVERSAMPLING_RATIO_MUL2: oversampling ratio multiple 2
      \arg        ADC_OVERSAMPLING_RATIO_MUL4: oversampling ratio multiple 4
      \arg        ADC_OVERSAMPLING_RATIO_MUL8: oversampling ratio multiple 8
      \arg        ADC_OVERSAMPLING_RATIO_MUL16: oversampling ratio multiple 16
      \arg        ADC_OVERSAMPLING_RATIO_MUL32: oversampling ratio multiple 32
      \arg        ADC_OVERSAMPLING_RATIO_MUL64: oversampling ratio multiple 64
      \arg        ADC_OVERSAMPLING_RATIO_MUL128: oversampling ratio multiple 128
      \arg        ADC_OVERSAMPLING_RATIO_MUL256: oversampling ratio multiple 256
    \param[out] none
    \retval     none
*/
void adc_oversample_mode_config(uint32_t adc_periph, uint8_t mode, uint16_t shift, uint8_t ratio)
{
    /* configure ADC oversampling mode */
    if(ADC_OVERSAMPLING_ONE_CONVERT == mode) {
        ADC_OVSAMPCTL(adc_periph) |= (uint32_t)ADC_OVSAMPCTL_TOVS;
    } else {
        ADC_OVSAMPCTL(adc_periph) &= ~((uint32_t)ADC_OVSAMPCTL_TOVS);
    }

    /* configure the shift and ratio */
    ADC_OVSAMPCTL(adc_periph) &= ~((uint32_t)(ADC_OVSAMPCTL_OVSR | ADC_OVSAMPCTL_OVSS));
    ADC_OVSAMPCTL(adc_periph) |= (((uint32_t)shift & ADC_OVSAMPCTL_OVSS) | ((uint32_t)ratio & ADC_OVSAMPCTL_OVSR));
}

/*!
    \brief      enable ADC oversample mode (API_ID(0x001EU))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[out] none
    \retval     none
*/
void adc_oversample_mode_enable(uint32_t adc_periph)
{
    ADC_OVSAMPCTL(adc_periph) |= ADC_OVSAMPCTL_OVSEN;
}

/*!
    \brief      disable ADC oversample mode (API_ID(0x001FU))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[out] none
    \retval     none
*/
void adc_oversample_mode_disable(uint32_t adc_periph)
{
    ADC_OVSAMPCTL(adc_periph) &= ~((uint32_t)ADC_OVSAMPCTL_OVSEN);
}

/*!
    \brief    configure the delay between 2 sampling phases in ADC sync modes (API_ID(0x0020U))
    \param[in]  sample_delay:  the delay between 2 sampling phases in ADC sync modes
                only one parameter can be selected which is shown as below:
      \arg        ADC_SYNC_DELAY_xCYCLE: x=5..20,the delay between 2 sampling phases in ADC sync modes is x ADC clock cycles
    \param[out] none
    \retval     none
*/
void adc_sync_delay_config(uint32_t sample_delay)
{
    ADC_SYNCCTL(ADC0) &= ~((uint32_t)ADC_SYNCCTL_SYNCDLY);
    ADC_SYNCCTL(ADC0) |= (uint32_t)((uint32_t)sample_delay & ADC_SYNCCTL_SYNCDLY);
}

/*!
    \brief    configure ADC sync DMA mode selection (API_ID(0x0021U))
    \param[in]  dma_mode:  ADC0 sync DMA mode
                only one parameter can be selected which is shown as below:
      \arg        ADC_SYNC_DMA_DISABLE: ADC sync DMA disabled
      \arg        ADC_SYNC_DMA_MODE0: ADC sync DMA mode 0
      \arg        ADC_SYNC_DMA_MODE1: ADC sync DMA mode 1
    \param[out] none
    \retval     none
*/
void adc_sync_dma_config(uint32_t dma_mode)
{
    ADC_SYNCCTL(ADC0) &= ~((uint32_t)ADC_SYNCCTL_SYNCDMA);
    ADC_SYNCCTL(ADC0) |= (uint32_t)((uint32_t)dma_mode & ADC_SYNCCTL_SYNCDMA);
}

/*!
    \brief    read ADC sync routine data register 0 (API_ID(0x0022U))
    \param[in]  none
    \param[out] none
    \retval     sync routine data 0
*/
uint32_t adc_sync_routine_data0_read(void)
{
    return (uint32_t)ADC_SYNCDATA0(ADC0);
}

/*!
    \brief      read ADC sync master adc routine data register 0 (API_ID(0x0023U))
    \param[in]  none
    \param[out] none
    \retval     sync routine data 0
*/
uint32_t adc_sync_master_adc_routine_data0_read(void)
{
    return (uint32_t)(ADC_SYNCDATA0(ADC0) & ADC_SYNCDATA0_SYNCDATA0);
}

/*!
    \brief      read ADC sync slave adc routine data register 0 (API_ID(0x0024U))
    \param[in]  none
    \param[out] none
    \retval     sync routine data 0
*/
uint32_t adc_sync_slave_adc_routine_data0_read(void)
{
    return (uint32_t)(ADC_SYNCDATA0(ADC0) & ADC_SYNCDATA0_SYNCDATA1) >> 16;
}

/*!
    \brief      get the ADC flag (API_ID(0x0025U))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[in]  flag: the ADC flag bits
                only one parameter can be selected which is shown as below:
      \arg        ADC_FLAG_WD0E: analog watchdog 0 event flag
      \arg        ADC_FLAG_EORC: end of routine sequence conversion flag
      \arg        ADC_FLAG_EOIC: end of inserted sequence conversion flag
      \arg        ADC_FLAG_STIC: start flag of inserted sequence
      \arg        ADC_FLAG_STRC: start flag of routine sequence
      \arg        ADC_FLAG_ROVF: flag of routine data register overflow
      \arg        ADC_FLAG_IOVF: flag of inserted data register overflow
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus adc_flag_get(uint32_t adc_periph, uint32_t flag)
{
    FlagStatus reval = RESET;
    if(0U != (ADC_STAT(adc_periph) & flag)) {
        reval = SET;
    }
    return reval;
}

/*!
    \brief      clear the ADC flag (API_ID(0x0026U))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[in]  flag: the ADC flag
                one or more parameters can be selected which is shown as below:
      \arg        ADC_FLAG_WD0E: analog watchdog 0 event flag
      \arg        ADC_FLAG_EORC: end of routine sequence conversion flag
      \arg        ADC_FLAG_EOIC: end of inserted sequence conversion flag
      \arg        ADC_FLAG_STIC: start flag of inserted sequence
      \arg        ADC_FLAG_STRC: start flag of routine sequence
      \arg        ADC_FLAG_ROVF: flag of routine data register overflow
      \arg        ADC_FLAG_IOVF: flag of inserted data register overflow
    \param[out] none
    \retval     none
*/
void adc_flag_clear(uint32_t adc_periph, uint32_t flag)
{
    ADC_STAT(adc_periph) &= ~((uint32_t)flag & ADC_FLAG_MASK);
}

/*!
    \brief      enable ADC interrupt (API_ID(0x0027U))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[in]  interrupt: the ADC interrupt
                one or more parameters can be selected which is shown as below:
      \arg        ADC_INT_WD0E: analog watchdog 0 interrupt
      \arg        ADC_INT_EORC: end of routine sequence conversion interrupt
      \arg        ADC_INT_EOIC: end of inserted sequence conversion interrupt
      \arg        ADC_INT_ROVF: routine data register overflow interrupt
      \arg        ADC_INT_IOVF: inserted data register overflow interrupt
    \param[out] none
    \retval     none
*/
void adc_interrupt_enable(uint32_t adc_periph, uint32_t interrupt)
{
    ADC_CTL0(adc_periph) |= (uint32_t) interrupt & ADC_INT_MASK;
}

/*!
    \brief      disable ADC interrupt (API_ID(0x0028U))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[in]  interrupt: the ADC interrupt flag
                one or more parameters can be selected which is shown as below:
      \arg        ADC_INT_WD0E: analog watchdog 0 interrupt
      \arg        ADC_INT_EORC: end of routine sequence conversion interrupt
      \arg        ADC_INT_EOIC: end of inserted sequence conversion interrupt
      \arg        ADC_INT_ROVF: routine data register overflow interrupt
      \arg        ADC_INT_IOVF: inserted data register overflow interrupt
    \param[out] none
    \retval     none
*/
void adc_interrupt_disable(uint32_t adc_periph, uint32_t interrupt)
{
    ADC_CTL0(adc_periph) &= ~((uint32_t)interrupt & ADC_INT_MASK);
}

/*!
    \brief      get the ADC interrupt flag (API_ID(0x0029U))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[in]  flag: the ADC interrupt flag
                only one parameter can be selected which is shown as below:
      \arg        ADC_INT_FLAG_WD0E: analog watchdog 0 interrupt flag
      \arg        ADC_INT_FLAG_EORC: end of routine sequence conversion interrupt flag
      \arg        ADC_INT_FLAG_EOIC: end of inserted sequence conversion interrupt flag
      \arg        ADC_INT_FLAG_ROVF: routine data register overflow interrupt flag
      \arg        ADC_INT_FLAG_IOVF: inserted data register overflow interrupt flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus adc_interrupt_flag_get(uint32_t adc_periph, uint32_t flag)
{
    FlagStatus interrupt_flag = RESET;
    uint32_t state;

    /* check the interrupt bits */
    switch(flag) {
    case ADC_INT_FLAG_WD0E:
        state = ADC_STAT(adc_periph) & ADC_STAT_WD0E;
        if((0U != (ADC_CTL0(adc_periph) & ADC_CTL0_WD0EIE)) && (0U != state)) {
            interrupt_flag = SET;
        }
        break;
    case ADC_INT_FLAG_EORC:
        state = ADC_STAT(adc_periph) & ADC_STAT_EORC;
        if((0U != (ADC_CTL0(adc_periph) & ADC_CTL0_EORCIE)) && (0U != state)) {
            interrupt_flag = SET;
        }
        break;
    case ADC_INT_FLAG_EOIC:
        state = ADC_STAT(adc_periph) & ADC_STAT_EOIC;
        if((0U != (ADC_CTL0(adc_periph) & ADC_CTL0_EOICIE)) && (0U != state)) {
            interrupt_flag = SET;
        }
        break;
    case ADC_INT_FLAG_ROVF:
        state = ADC_STAT(adc_periph) & ADC_STAT_ROVF;
        if((0U != (ADC_CTL0(adc_periph) & ADC_CTL0_ROVFIE)) && (0U != state)) {
            interrupt_flag = SET;
        }
        break;
    case ADC_INT_FLAG_IOVF:
        state = ADC_STAT(adc_periph) & ADC_STAT_IOVF;
        if((0U != (ADC_CTL0(adc_periph) & ADC_CTL0_IOVFIE)) && (0U != state)) {
            interrupt_flag = SET;
        }
        break;
    default:
        break;
    }

    return interrupt_flag;
}

/*!
    \brief      clear ADC interrupt flag (API_ID(0x002AU))
    \param[in]  adc_periph: ADCx, x=0,1
    \param[in]  flag: the ADC interrupt flag
                one or more parameters can be selected which is shown as below:
      \arg        ADC_INT_FLAG_WD0E: analog watchdog 0 interrupt flag
      \arg        ADC_INT_FLAG_EORC: end of routine sequence conversion interrupt flag
      \arg        ADC_INT_FLAG_EOIC: end of inserted sequence conversion interrupt flag
      \arg        ADC_INT_FLAG_ROVF: routine data register overflow interrupt flag
      \arg        ADC_INT_FLAG_IOVF: inserted data register overflow interrupt flag
    \param[out] none
    \retval     none
*/
void adc_interrupt_flag_clear(uint32_t adc_periph, uint32_t flag)
{
    ADC_STAT(adc_periph) &= ~((uint32_t)flag & ADC_INT_FLAG_MASK);
}

/*!
    \brief    configure the ADC0 sync mode (API_ID(0x002BU))
    \param[in]  sync_mode: ADC sync mode
                only one parameter can be selected which is shown as below:
      \arg        ADC_SYNC_MODE_INDEPENDENT: all the ADCs work independently
      \arg        ADC_DAUL_ROUTINE_PARALLEL_INSERTED_PARALLEL: combined routine parallel & inserted parallel mode
      \arg        ADC_DAUL_ROUTINE_PARALLEL_INSERTED_ROTATION: combined routine parallel & trigger rotation mode
      \arg        ADC_DAUL_INSERTED_PARALLEL: inserted parallel mode
      \arg        ADC_DAUL_ROUTINE_PARALLEL: routine parallel mode
      \arg        ADC_DAUL_ROUTINE_FOLLOW_UP: follow-up mode
      \arg        ADC_DAUL_INSERTED_TRRIGGER_ROTATION: trigger rotation mode
    \param[out] none
    \retval     none
*/
void adc_sync_mode_config(uint32_t sync_mode)
{
    ADC_SYNCCTL(ADC0) &= ~((uint32_t)ADC_SYNCCTL_SYNCM);
    ADC_SYNCCTL(ADC0) |= (uint32_t)((uint32_t)sync_mode & ADC_SYNCCTL_SYNCM);
}

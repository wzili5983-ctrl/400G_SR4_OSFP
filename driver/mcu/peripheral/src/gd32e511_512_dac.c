/*!
    \file    gd32e511_512_dac.c
    \brief   DAC driver

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

#include "gd32e511_512_dac.h"

/* DAC register bit offset */
#define OUT1_REG_OFFSET           ((uint32_t)0x00000010U)
#define DH_12BIT_OFFSET           ((uint32_t)0x00000010U)
#define DH_8BIT_OFFSET            ((uint32_t)0x00000008U)

#define DAC_STAT_FLAG_MASK0       (DAC_FLAG_DDUDR0 | DAC_FLAG_DDUDR1)
#define DAC_INT_EN_MASK0          (DAC_INT_DDUDR0 | DAC_INT_DDUDR1)
#define DAC_INT_FLAG_MASK0        (DAC_INT_FLAG_DDUDR0 | DAC_INT_FLAG_DDUDR1)

/*!
    \brief      deinitialize DAC (API_ID(0x0001U))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[out] none
    \retval     none
*/
void dac_deinit(uint32_t dac_periph)
{
    switch(dac_periph) {
    case DAC0:
        /* reset DAC0 */
        rcu_periph_reset_enable(RCU_DAC0RST);
        rcu_periph_reset_disable(RCU_DAC0RST);
        break;
    case DAC1:
        /* reset DAC1 */
        rcu_periph_reset_enable(RCU_DAC1RST);
        rcu_periph_reset_disable(RCU_DAC1RST);
        break;
    case DAC2:
        /* reset DAC2 */
        rcu_periph_reset_enable(RCU_DAC2RST);
        rcu_periph_reset_disable(RCU_DAC2RST);
        break;
    case DAC3:
        /* reset DAC3 */
        rcu_periph_reset_enable(RCU_DAC3RST);
        rcu_periph_reset_disable(RCU_DAC3RST);
        break;
    default:
        /* illegal parameters */
        break;
    }
}

/*!
    \brief      enable DAC (API_ID(0x0002U))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[in]  dac_out: DAC_OUTx(x=0,1)
    \param[out] none
    \retval     none
*/
void dac_enable(uint32_t dac_periph, uint8_t dac_out)
{
    if(DAC_OUT0 == dac_out) {
        DAC_CTL0(dac_periph) |= (uint32_t)DAC_CTL0_DEN0;
    } else if(DAC_OUT1 == dac_out) {
        DAC_CTL0(dac_periph) |= (uint32_t)DAC_CTL0_DEN1;
    } else {
        /* illegal parameters */
    }
}

/*!
    \brief      disable DAC (API_ID(0x0003U))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[in]  dac_out: DAC_OUTx(x=0,1)
    \param[out] none
    \retval     none
*/
void dac_disable(uint32_t dac_periph, uint8_t dac_out)
{
    if(DAC_OUT0 == dac_out) {
        DAC_CTL0(dac_periph) &= (uint32_t)(~DAC_CTL0_DEN0);
    } else if(DAC_OUT1 == dac_out) {
        DAC_CTL0(dac_periph) &= (uint32_t)(~DAC_CTL0_DEN1);
    } else {
        /* illegal parameters */
    }
}

/*!
    \brief      enable DAC DMA function (API_ID(0x0004U))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[in]  dac_out: DAC_OUTx(x=0,1)
    \param[out] none
    \retval     none
*/
void dac_dma_enable(uint32_t dac_periph, uint8_t dac_out)
{
    if(DAC_OUT0 == dac_out) {
        DAC_CTL0(dac_periph) |= (uint32_t)DAC_CTL0_DDMAEN0;
    } else if(DAC_OUT1 == dac_out) {
        DAC_CTL0(dac_periph) |= (uint32_t)DAC_CTL0_DDMAEN1;
    } else {
        /* illegal parameters */
    }
}

/*!
    \brief      disable DAC DMA function (API_ID(0x0005U))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[in]  dac_out: DAC_OUTx(x=0,1)
    \param[out] none
    \retval     none
*/
void dac_dma_disable(uint32_t dac_periph, uint8_t dac_out)
{
    if(DAC_OUT0 == dac_out) {
        DAC_CTL0(dac_periph) &= (uint32_t)(~DAC_CTL0_DDMAEN0);
    } else if(DAC_OUT1 == dac_out) {
        DAC_CTL0(dac_periph) &= (uint32_t)(~DAC_CTL0_DDMAEN1);
    } else {
        /* illegal parameters */
    }
}

/*!
    \brief      enable DAC output buffer (API_ID(0x0006U))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[in]  dac_out: DAC_OUTx(x=0,1)
    \param[out] none
    \retval     none
*/
void dac_output_buffer_enable(uint32_t dac_periph, uint8_t dac_out)
{
    if(DAC_OUT0 == dac_out) {
        DAC_CTL0(dac_periph) &= (uint32_t)(~DAC_CTL0_DBOFF0);
    } else if(DAC_OUT1 == dac_out) {
        DAC_CTL0(dac_periph) &= (uint32_t)(~DAC_CTL0_DBOFF1);
    } else {
        /* illegal parameters */
    }
}

/*!
    \brief      disable DAC output buffer (API_ID(0x0007U))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[in]  dac_out: DAC_OUTx(x=0,1)
    \param[out] none
    \retval     none
*/
void dac_output_buffer_disable(uint32_t dac_periph, uint8_t dac_out)
{
    if(DAC_OUT0 == dac_out) {
        DAC_CTL0(dac_periph) |= (uint32_t)DAC_CTL0_DBOFF0;
    } else if(DAC_OUT1 == dac_out) {
        DAC_CTL0(dac_periph) |= (uint32_t)DAC_CTL0_DBOFF1;
    } else {
        /* illegal parameters */
    }
}

/*!
    \brief      configure DAC output buffer gain (API_ID(0x0008U))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[in]  dac_out: DAC_OUTx(x=0,1)
    \param[in]  buffer_gain: DAC_OUT output buffer gain
                only one parameter can be selected which is shown as below:
      \arg        DAC_OUT_GAIN_1: DAC_OUT output buffer gain is 1
      \arg        DAC_OUT_GAIN_2: DAC_OUT output buffer gain is 2
    \param[out] none
    \retval     none
*/
void dac_buffer_gain_config(uint32_t dac_periph, uint8_t dac_out, uint32_t buffer_gain)
{
    if(DAC_OUT0 == dac_out) {
        /* configure DACx_OUT0 triangle noise mode */
        DAC_OCTL(dac_periph) &= (uint32_t)(~DAC_OCTL_GS0);
        DAC_OCTL(dac_periph) |= buffer_gain & DAC_OCTL_GS0;
    } else if(DAC_OUT1 == dac_out) {
        /* configure DACx_OUT1 triangle noise mode */
        DAC_OCTL(dac_periph) &= (uint32_t)(~DAC_OCTL_GS1);
        DAC_OCTL(dac_periph) |= (buffer_gain << OUT1_REG_OFFSET) & DAC_OCTL_GS1;
    } else {
        /* illegal parameters */
    }
}

/*!
    \brief      get DAC output value (API_ID(0x0009U))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[in]  dac_out: DAC_OUTx(x=0,1)
    \param[out] none
    \retval     DAC output data: 0~4095
*/
uint16_t dac_output_value_get(uint32_t dac_periph, uint8_t dac_out)
{
    uint16_t data = 0U;
    if(DAC_OUT0 == dac_out) {
        /* store the DACx_OUT0 output value */
        data = (uint16_t)DAC_OUT0_DO(dac_periph);
    } else if(DAC_OUT1 == dac_out) {
        /* store the DACx_OUT1 output value */
        data = (uint16_t)DAC_OUT1_DO(dac_periph);
    } else {
        /* illegal parameters */
    }
    return data;
}

/*!
    \brief      set DAC data holding register value (API_ID(0x000AU))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[in]  dac_out: DAC_OUTx(x=0,1)
    \param[in]  dac_align: DAC data alignment mode
                only one parameter can be selected which is shown as below:
      \arg        DAC_ALIGN_12B_R: 12-bit right-aligned data
      \arg        DAC_ALIGN_12B_L: 12-bit left-aligned data
      \arg        DAC_ALIGN_8B_R: 8-bit right-aligned data
    \param[in]  data: data to be loaded(0~4095)
    \param[out] none
    \retval     none
*/
void dac_data_set(uint32_t dac_periph, uint8_t dac_out, uint32_t dac_align, uint16_t data)
{
    if(DAC_OUT0 == dac_out) {
        switch(dac_align) {
        /* 12-bit right-aligned data */
        case DAC_ALIGN_12B_R:
            DAC_OUT0_R12DH(dac_periph) = data;
            break;
        /* 12-bit left-aligned data */
        case DAC_ALIGN_12B_L:
            DAC_OUT0_L12DH(dac_periph) = data;
            break;
        /* 8-bit right-aligned data */
        case DAC_ALIGN_8B_R:
            DAC_OUT0_R8DH(dac_periph) = data;
            break;
        default:
            /* illegal parameters */
            break;
        }
    } else if(DAC_OUT1 == dac_out) {
        /* DAC_OUT1 data alignment */
        switch(dac_align) {
        /* 12-bit right-aligned data */
        case DAC_ALIGN_12B_R:
            DAC_OUT1_R12DH(dac_periph) = data;
            break;
        /* 12-bit left-aligned data */
        case DAC_ALIGN_12B_L:
            DAC_OUT1_L12DH(dac_periph) = data;
            break;
        /* 8-bit right-aligned data */
        case DAC_ALIGN_8B_R:
            DAC_OUT1_R8DH(dac_periph) = data;
            break;
        default:
            /* illegal parameters */
            break;
        }
    } else {
        /* illegal parameters */
    }
}

/*!
    \brief      enable DAC trigger (API_ID(0x000BU))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[in]  dac_out: DAC_OUTx(x=0,1)
    \param[out] none
    \retval     none
*/
void dac_trigger_enable(uint32_t dac_periph, uint8_t dac_out)
{
    if(DAC_OUT0 == dac_out) {
        DAC_CTL0(dac_periph) |= (uint32_t)DAC_CTL0_DTEN0;
    } else if(DAC_OUT1 == dac_out) {
        DAC_CTL0(dac_periph) |= (uint32_t)DAC_CTL0_DTEN1;
    } else {
        /* illegal parameters */
    }
}

/*!
    \brief      disable DAC trigger (API_ID(0x000CU))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[in]  dac_out: DAC_OUTx(x=0,1)
    \param[out] none
    \retval     none
*/
void dac_trigger_disable(uint32_t dac_periph, uint8_t dac_out)
{
    if(DAC_OUT0 == dac_out) {
        DAC_CTL0(dac_periph) &= (uint32_t)(~DAC_CTL0_DTEN0);
    } else if(DAC_OUT1 == dac_out) {
        DAC_CTL0(dac_periph) &= (uint32_t)(~DAC_CTL0_DTEN1);
    } else {
        /* illegal parameters */
    }
}

/*!
    \brief      configure DAC trigger source (API_ID(0x000DU))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[in]  dac_out: DAC_OUTx(x=0,1)
    \param[in]  triggersource: external trigger of DAC
                only one parameter can be selected which is shown as below:
      \arg        DAC_TRIGGER_T5_TRGO: TIMER5 TRGO
      \arg        DAC_TRIGGER_T14_TRGO: TIMER14 TRGO
      \arg        DAC_TRIGGER_T6_TRGO: TIMER6 TRGO
      \arg        DAC_TRIGGER_T2_TRGO: TIMER2 TRGO
      \arg        DAC_TRIGGER_T1_TRGO: TIMER1 TRGO
      \arg        DAC_TRIGGER_T0_TRGO: TIMER0 TRGO
      \arg        DAC_TRIGGER_EXTI_9: EXTI interrupt line9 event
      \arg        DAC_TRIGGER_SOFTWARE: software trigger
      \arg        DAC_TRIGGER_CLA0: CLA0 trigger
      \arg        DAC_TRIGGER_CLA1: CLA1 trigger
      \arg        DAC_TRIGGER_CLA2: CLA2 trigger
      \arg        DAC_TRIGGER_CLA3: CLA3 trigger
    \param[out] none
    \retval     none
*/
void dac_trigger_source_config(uint32_t dac_periph, uint8_t dac_out, uint32_t triggersource)
{
    if(DAC_OUT0 == dac_out) {
        /* configure DACx_OUT0 trigger source */
        DAC_CTL0(dac_periph) &= (uint32_t)(~(DAC_CTL0_DTSEL0 | DAC_CTL0_DTSEL0_3));
        DAC_CTL0(dac_periph) |= (uint32_t)triggersource & (DAC_CTL0_DTSEL0 | DAC_CTL0_DTSEL0_3);
    } else if(DAC_OUT1 == dac_out) {
        /* configure DACx_OUT1 trigger source */
        DAC_CTL0(dac_periph) &= (uint32_t)(~(DAC_CTL0_DTSEL1 | DAC_CTL0_DTSEL1_3));
        DAC_CTL0(dac_periph) |= (((uint32_t)triggersource) << OUT1_REG_OFFSET) & (DAC_CTL0_DTSEL1 | DAC_CTL0_DTSEL1_3);
    } else {
        /* illegal parameters */
    }
}

/*!
    \brief      enable DAC software trigger (API_ID(0x000EU))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[in]  dac_out: DAC_OUTx(x=0,1)
    \retval     none
*/
void dac_software_trigger_enable(uint32_t dac_periph, uint8_t dac_out)
{
    if(DAC_OUT0 == dac_out) {
        DAC_SWT(dac_periph) |= (uint32_t)DAC_SWT_SWTR0;
    } else if(DAC_OUT1 == dac_out) {
        DAC_SWT(dac_periph) |= (uint32_t)DAC_SWT_SWTR1;
    } else {
        /* illegal parameters */
    }
}

/*!
    \brief      configure DAC wave mode (API_ID(0x000FU))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[in]  dac_out: DAC_OUTx(x=0,1)
    \param[in]  wave_mode: DAC wave mode
                only one parameter can be selected which is shown as below:
      \arg        DAC_WAVE_DISABLE: wave mode disable
      \arg        DAC_WAVE_MODE_LFSR: LFSR noise mode
      \arg        DAC_WAVE_MODE_TRIANGLE: triangle noise mode
    \param[out] none
    \retval     none
*/
void dac_wave_mode_config(uint32_t dac_periph, uint8_t dac_out, uint32_t wave_mode)
{
    if(DAC_OUT0 == dac_out) {
        /* configure DACx_OUT0 wave mode */
        DAC_CTL0(dac_periph) &= (uint32_t)(~DAC_CTL0_DWM0);
        DAC_CTL0(dac_periph) |= wave_mode & DAC_CTL0_DWM0;
    } else if(DAC_OUT1 == dac_out) {
        /* configure DACx_OUT1 wave mode */
        DAC_CTL0(dac_periph) &= (uint32_t)(~DAC_CTL0_DWM1);
        DAC_CTL0(dac_periph) |= (wave_mode << OUT1_REG_OFFSET) & DAC_CTL0_DWM1;
    } else {
        /* illegal parameters */
    }
}

/*!
    \brief      configure DAC LFSR noise mode (API_ID(0x0010U))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[in]  dac_out: DAC_OUTx(x=0,1)
    \param[in]  unmask_bits: LFSR noise unmask bits
                only one parameter can be selected which is shown as below:
      \arg        DAC_LFSR_BIT0: unmask the LFSR bit0
      \arg        DAC_LFSR_BITS1_0: unmask the LFSR bits[1:0]
      \arg        DAC_LFSR_BITS2_0: unmask the LFSR bits[2:0]
      \arg        DAC_LFSR_BITS3_0: unmask the LFSR bits[3:0]
      \arg        DAC_LFSR_BITS4_0: unmask the LFSR bits[4:0]
      \arg        DAC_LFSR_BITS5_0: unmask the LFSR bits[5:0]
      \arg        DAC_LFSR_BITS6_0: unmask the LFSR bits[6:0]
      \arg        DAC_LFSR_BITS7_0: unmask the LFSR bits[7:0]
      \arg        DAC_LFSR_BITS8_0: unmask the LFSR bits[8:0]
      \arg        DAC_LFSR_BITS9_0: unmask the LFSR bits[9:0]
      \arg        DAC_LFSR_BITS10_0: unmask the LFSR bits[10:0]
      \arg        DAC_LFSR_BITS11_0: unmask the LFSR bits[11:0]
    \param[out] none
    \retval     none
*/
void dac_lfsr_noise_config(uint32_t dac_periph, uint8_t dac_out, uint32_t unmask_bits)
{
    if(DAC_OUT0 == dac_out) {
        /* configure DACx_OUT0 LFSR noise mode */
        DAC_CTL0(dac_periph) &= (uint32_t)(~DAC_CTL0_DWBW0);
        DAC_CTL0(dac_periph) |= (uint32_t)unmask_bits & DAC_CTL0_DWBW0;
    } else if(DAC_OUT1 == dac_out) {
        /* configure DACx_OUT1 LFSR noise mode */
        DAC_CTL0(dac_periph) &= (uint32_t)(~DAC_CTL0_DWBW1);
        DAC_CTL0(dac_periph) |= (((uint32_t)unmask_bits) << OUT1_REG_OFFSET) & DAC_CTL0_DWBW1;
    } else {
        /* illegal parameters */
    }
}

/*!
    \brief      configure DAC triangle noise mode (API_ID(0x0011U))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[in]  dac_out: DAC_OUTx(x=0,1)
    \param[in]  amplitude: the amplitude of the triangle
                only one parameter can be selected which is shown as below:
      \arg        DAC_TRIANGLE_AMPLITUDE_1: triangle amplitude is 1
      \arg        DAC_TRIANGLE_AMPLITUDE_3: triangle amplitude is 3
      \arg        DAC_TRIANGLE_AMPLITUDE_7: triangle amplitude is 7
      \arg        DAC_TRIANGLE_AMPLITUDE_15: triangle amplitude is 15
      \arg        DAC_TRIANGLE_AMPLITUDE_31: triangle amplitude is 31
      \arg        DAC_TRIANGLE_AMPLITUDE_63: triangle amplitude is 63
      \arg        DAC_TRIANGLE_AMPLITUDE_127: triangle amplitude is 127
      \arg        DAC_TRIANGLE_AMPLITUDE_255: triangle amplitude is 255
      \arg        DAC_TRIANGLE_AMPLITUDE_511: triangle amplitude is 511
      \arg        DAC_TRIANGLE_AMPLITUDE_1023: triangle amplitude is 1023
      \arg        DAC_TRIANGLE_AMPLITUDE_2047: triangle amplitude is 2047
      \arg        DAC_TRIANGLE_AMPLITUDE_4095: triangle amplitude is 4095
    \param[out] none
    \retval     none
*/
void dac_triangle_noise_config(uint32_t dac_periph, uint8_t dac_out, uint32_t amplitude)
{
    if(DAC_OUT0 == dac_out) {
        /* configure DACx_OUT0 triangle noise mode */
        DAC_CTL0(dac_periph) &= (uint32_t)(~DAC_CTL0_DWBW0);
        DAC_CTL0(dac_periph) |= (uint32_t)amplitude & DAC_CTL0_DWBW0;
    } else if(DAC_OUT1 == dac_out) {
        /* configure DACx_OUT1 triangle noise mode */
        DAC_CTL0(dac_periph) &= (uint32_t)(~DAC_CTL0_DWBW1);
        DAC_CTL0(dac_periph) |= (((uint32_t)amplitude) << OUT1_REG_OFFSET) & DAC_CTL0_DWBW1;
    } else {
        /* illegal parameters */
    }
}

/*!
    \brief      enable DAC concurrent mode (API_ID(0x0012U))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[out] none
    \retval     none
*/
void dac_concurrent_enable(uint32_t dac_periph)
{
    uint32_t ctl;
    ctl = (uint32_t)(DAC_CTL0_DEN0 | DAC_CTL0_DEN1);
    DAC_CTL0(dac_periph) |= (uint32_t)ctl;
}

/*!
    \brief      disable DAC concurrent mode (API_ID(0x0013U))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[out] none
    \retval     none
*/
void dac_concurrent_disable(uint32_t dac_periph)
{
    uint32_t ctl;
    ctl = (uint32_t)(DAC_CTL0_DEN0 | DAC_CTL0_DEN1);
    DAC_CTL0(dac_periph) &= (uint32_t)(~ctl);
}

/*!
    \brief      enable DAC concurrent software trigger (API_ID(0x0014U))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[out] none
    \retval     none
*/
void dac_concurrent_software_trigger_enable(uint32_t dac_periph)
{
    uint32_t swt;
    swt = (uint32_t)(DAC_SWT_SWTR0 | DAC_SWT_SWTR1);
    DAC_SWT(dac_periph) |= (uint32_t)swt;
}

/*!
    \brief      enable DAC concurrent buffer function (API_ID(0x0015U))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[out] none
    \retval     none
*/
void dac_concurrent_output_buffer_enable(uint32_t dac_periph)
{
    uint32_t ctl;
    ctl = (uint32_t)(DAC_CTL0_DBOFF0 | DAC_CTL0_DBOFF1);
    DAC_CTL0(dac_periph) &= (uint32_t)(~ctl);
}

/*!
    \brief      disable DAC concurrent buffer function (API_ID(0x0016U))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[out] none
    \retval     none
*/
void dac_concurrent_output_buffer_disable(uint32_t dac_periph)
{
    uint32_t ctl;
    ctl = (uint32_t)(DAC_CTL0_DBOFF0 | DAC_CTL0_DBOFF1);
    DAC_CTL0(dac_periph) |= (uint32_t)ctl;
}

/*!
    \brief      set DAC concurrent mode data holding register value (API_ID(0x0017U))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[in]  dac_align: DAC data alignment mode
                only one parameter can be selected which is shown as below:
      \arg        DAC_ALIGN_12B_R: 12-bit right-aligned data
      \arg        DAC_ALIGN_12B_L: 12-bit left-aligned data
      \arg        DAC_ALIGN_8B_R: 8-bit right-aligned data
    \param[in]  data0: data to be loaded(0~4095)
    \param[in]  data1: data to be loaded(0~4095)
    \param[out] none
    \retval     none
*/
void dac_concurrent_data_set(uint32_t dac_periph, uint32_t dac_align, uint16_t data0, uint16_t data1)
{
    uint32_t data;
    switch(dac_align) {
    /* 12-bit right-aligned data */
    case DAC_ALIGN_12B_R:
        data = (uint32_t)(((uint32_t)data1 << DH_12BIT_OFFSET) | data0);
        DACC_R12DH(dac_periph) = (uint32_t)data;
        break;
    /* 12-bit left-aligned data */
    case DAC_ALIGN_12B_L:
        data = (uint32_t)(((uint32_t)data1 << DH_12BIT_OFFSET) | data0);
        DACC_L12DH(dac_periph) = (uint32_t)data;
        break;
    /* 8-bit right-aligned data */
    case DAC_ALIGN_8B_R:
        data = (uint32_t)(((uint32_t)data1 << DH_8BIT_OFFSET) | data0);
        DACC_R8DH(dac_periph) = (uint32_t)data;
        break;
    default:
        break;
    }
}

/*!
    \brief      enable DAC reset persisting mode (API_ID(0x0018U))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[in]  dac_out: DAC_OUTx(x=0,1)
    \param[out] none
    \retval     none
*/
void dac_reset_persist_enable(uint32_t dac_periph, uint8_t dac_out)
{
    if(DAC_OUT0 == dac_out) {
        DAC_CTL0(dac_periph) |= (uint32_t)DAC_CTL0_DRSTMD0;
    } else if(DAC_OUT1 == dac_out) {
        DAC_CTL0(dac_periph) |= (uint32_t)DAC_CTL0_DRSTMD1;
    } else {
        /* illegal parameters */
    }
}

/*!
    \brief      disable DAC reset persisting mode (API_ID(0x0019U))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[in]  dac_out: DAC_OUTx(x=0,1)
    \param[out] none
    \retval     none
*/
void dac_reset_persist_disable(uint32_t dac_periph, uint8_t dac_out)
{
    if(DAC_OUT0 == dac_out) {
        DAC_CTL0(dac_periph) &= (uint32_t)(~DAC_CTL0_DRSTMD0);
    } else if(DAC_OUT1 == dac_out) {
        DAC_CTL0(dac_periph) &= (uint32_t)(~DAC_CTL0_DRSTMD1);
    } else {
        /* illegal parameters */
    }
}

/*!
    \brief      get the specified DAC flag (API_ID(0x001AU))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[in]  flag: the DAC status flags, only one parameter can be selected which is shown
                as below:
      \arg        DAC_FLAG_DDUDR0: DACx_OUT0 DMA underrun flag
      \arg        DAC_FLAG_DDUDR1: DACx_OUT1 DMA underrun flag
    \param[out] none
    \retval     the state of DAC bit(SET or RESET)
*/
FlagStatus dac_flag_get(uint32_t dac_periph, uint32_t flag)
{
    FlagStatus reval = RESET;
    if(0U != (flag & DAC_STAT_FLAG_MASK0)) {
        /* check DAC_STAT0 flag */
        if(0U != (DAC_STAT0(dac_periph) & flag)) {
            reval = SET;
        } else {
            /* illegal parameters */
        }
    } else {
        /* illegal parameters */
    }
    return reval;
}

/*!
    \brief      clear the specified DAC flag (API_ID(0x001BU))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[in]  flag: DAC flag
                one or more parameters can be selected which are shown as below:
      \arg        DAC_FLAG_DDUDR0: DACx_OUT0 DMA underrun flag
      \arg        DAC_FLAG_DDUDR1: DACx_OUT1 DMA underrun flag
    \param[out] none
    \retval     none
*/
void dac_flag_clear(uint32_t dac_periph, uint32_t flag)
{
    if(0U != (flag & DAC_STAT_FLAG_MASK0)) {
        /* check DAC_STAT0 flag */
        DAC_STAT0(dac_periph) = (uint32_t)(flag & DAC_STAT_FLAG_MASK0);
    } else {
        /* illegal parameters */
    }
}

/*!
    \brief      enable DAC interrupt (API_ID(0x001CU))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[in]  interrupt: the DAC interrupt
                one or more parameters can be selected which are shown as below:
      \arg        DAC_INT_DDUDR0: DACx_OUT0 DMA underrun interrupt
      \arg        DAC_INT_DDUDR1: DACx_OUT1 DMA underrun interrupt
    \param[out] none
    \retval     none
*/
void dac_interrupt_enable(uint32_t dac_periph, uint32_t interrupt)
{
    if(0U != (interrupt & DAC_INT_EN_MASK0)) {
        /* enable underrun interrupt */
        DAC_CTL0(dac_periph) |= (uint32_t)(interrupt & DAC_INT_EN_MASK0);
    } else {
        /* illegal parameters */
    }
}

/*!
    \brief      disable DAC interrupt (API_ID(0x001DU))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[in]  interrupt: the DAC interrupt
                one or more parameters can be selected which are shown as below:
      \arg        DAC_INT_DDUDR0: DACx_OUT0 DMA underrun interrupt
      \arg        DAC_INT_DDUDR1: DACx_OUT1 DMA underrun interrupt
    \param[out] none
    \retval     none
*/
void dac_interrupt_disable(uint32_t dac_periph, uint32_t interrupt)
{
    if(0U != (interrupt & DAC_INT_EN_MASK0)) {
        /* disable underrun interrupt */
        DAC_CTL0(dac_periph) &= (uint32_t)(~(interrupt & DAC_INT_EN_MASK0));
    } else {
        /* illegal parameters */
    }
}

/*!
    \brief      get DAC interrupt flag (API_ID(0x001EU))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[in]  int_flag: DAC interrupt flag
                only one parameter can be selected which is shown as below:
      \arg        DAC_INT_FLAG_DDUDR0: DACx_OUT0 DMA underrun interrupt flag
      \arg        DAC_INT_FLAG_DDUDR1: DACx_OUT1 DMA underrun interrupt flag
    \param[out] none
    \retval     the state of DAC interrupt flag(SET or RESET)
*/
FlagStatus dac_interrupt_flag_get(uint32_t dac_periph, uint32_t int_flag)
{
    FlagStatus reval = RESET;
    uint32_t reg1 = 0U, reg2 = 0U;
    if(0U != (int_flag & DAC_INT_FLAG_MASK0)) {
        /* check underrun interrupt int_flag */
        reg1 = DAC_STAT0(dac_periph) & int_flag;
        reg2 = DAC_CTL0(dac_periph) & int_flag;
    } else {
        /* illegal parameters */
    }

    /*get DAC interrupt flag status */
    if((0U != reg1) && (0U != reg2)) {
        reval = SET;
    } else {
        /* illegal parameters */
    }
    return reval;
}

/*!
    \brief      clear DAC interrupt flag (API_ID(0x001FU))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[in]  int_flag: DAC interrupt flag
                one or more parameters can be selected which are shown as below:
      \arg        DAC_INT_FLAG_DDUDR0: DACx_OUT0 DMA underrun interrupt flag
      \arg        DAC_INT_FLAG_DDUDR1: DACx_OUT1 DMA underrun interrupt flag
    \param[out] none
    \retval     none
*/
void dac_interrupt_flag_clear(uint32_t dac_periph, uint32_t int_flag)
{
    /* clear underrun interrupt int_flag */
    if(0U != (int_flag & DAC_INT_FLAG_MASK0)) {
        DAC_STAT0(dac_periph) = (uint32_t)(int_flag & DAC_INT_FLAG_MASK0);
    } else {
        /* illegal parameters */
    }
}

/*!
    \brief      configure DAC output channel connects external pin and internal CMP (API_ID(0x0020U))
    \param[in]  dac_periph: DACx(x=0)
    \param[in]  dac_out: DAC_OUTx(x=0,1)
    \param[out] none
    \retval     none
*/
void dac_output_ddisc_disable(uint32_t dac_periph, uint8_t dac_out)
{
    if(dac_periph == DAC0) {
        if(DAC_OUT0 == dac_out) {
            /* configure DACx_OUT0 connects external pin */
            DAC_DISC(dac_periph) &= ~(uint32_t)(DAC_OUT_DISC_0);
        } else if(DAC_OUT1 == dac_out) {
            /* configure DACx_OUT1 connects external pin */
            DAC_DISC(dac_periph) &= ~(uint32_t)(DAC_OUT_DISC_1);
        } else {
            /* illegal parameters */
        }
    }
}

/*!
    \brief      configure DAC output channel disconnects external pin (API_ID(0x0021U))
    \param[in]  dac_periph: DACx(x=0)
    \param[in]  dac_out: DAC_OUTx(x=0,1)
    \param[out] none
    \retval     none
*/
void dac_output_ddisc_enable(uint32_t dac_periph, uint8_t dac_out)
{
    if(dac_periph == DAC0) {
        if(DAC_OUT0 == dac_out) {
            /* configure DACx_OUT0 disconnects external pin */
            DAC_DISC(dac_periph) |= (uint32_t)DAC_OUT_DISC_0;
        } else if(DAC_OUT1 == dac_out) {
            /* configure DACx_OUT1 disconnects external pin */
            DAC_DISC(dac_periph) |= (uint32_t)DAC_OUT_DISC_1;
        } else {
            /* illegal parameters */
        }
    }
}

/*!
    \brief      configure DAC output buffer calr level (API_ID(0x0022U))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[in]  dac_out: DAC_OUTx(x=0,1)
    \param[in]  dac_calr_level: DAC_CALR_LEVEL(x), x=0,1,...,31
    \param[out] none
    \retval     none
*/
void dac_output_buffer_calr_level_cfg(uint32_t dac_periph, uint8_t dac_out, uint8_t dac_calr_level)
{
    uint32_t reg_val;
    reg_val = DAC_CALR(dac_periph);

    if(DAC_OUT0 == dac_out) {
        /* configure DACx_OUT0 calr */
        reg_val &= (uint32_t)(~DAC_CALR_LEVEL_0);
        reg_val |= dac_calr_level & DAC_CALR_LEVEL_0;
        DAC_CALR(dac_periph) = reg_val;
    } else if(DAC_OUT1 == dac_out) {
        /* configure DACx_OUT1 calr */
        reg_val &= (uint32_t)(~DAC_CALR_LEVEL_1);
        reg_val |= (((uint32_t)dac_calr_level) << 16);
        DAC_CALR(dac_periph) = reg_val & DAC_CALR_LEVEL_1;
    } else {
        /* illegal parameters */
    }
}

/*!
    \brief      disable DAC output buffer calibration (API_ID(0x0023U))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[in]  dac_out: DAC_OUTx(x=0,1)
    \param[out] none
    \retval     none
*/
void dac_output_buffer_calr_disable(uint32_t dac_periph, uint8_t dac_out)
{
    if(DAC_OUT0 == dac_out) {
        /* configure DACx_OUT0 calr disable */
        DAC_CALR(dac_periph) &= ~(uint32_t)(DAC_OUT_CALR_ENABLE_0);
    } else if(DAC_OUT1 == dac_out) {
        /* configure DACx_OUT1 calr disable */
        DAC_CALR(dac_periph) &= ~(uint32_t)(DAC_OUT_CALR_ENABLE_1);
    } else {
        /* illegal parameters */
    }
}

/*!
    \brief      enable DAC output buffer calibration (API_ID(0x0024U))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[in]  dac_out: DAC_OUTx(x=0,1)
    \param[out] none
    \retval     none
*/
void dac_output_buffer_calr_enable(uint32_t dac_periph, uint8_t dac_out)
{
    if(DAC_OUT0 == dac_out) {
        /* configure DACx_OUT0 calr enable */
        DAC_CALR(dac_periph) |= (uint32_t)DAC_OUT_CALR_ENABLE_0;
    } else if(DAC_OUT1 == dac_out) {
        /* configure DACx_OUT1 calr enable */
        DAC_CALR(dac_periph) |= (uint32_t)DAC_OUT_CALR_ENABLE_1;
    } else {
        /* illegal parameters */
    }
}

/*!
    \brief      get DAC output buffer calibration offset flag (API_ID(0x0025U))
    \param[in]  dac_periph: DACx(x=0,1,2,3)
    \param[in]  dac_out: DAC_OUTx(x=0,1)
    \param[out] none
    \retval     the state of DAC output buffer calibration offset flag(SET or RESET)
*/
FlagStatus dac_output_buffer_calr_flag_get(uint32_t dac_periph, uint8_t dac_out)
{
    FlagStatus reval = RESET;
    if(DAC_OUT0 == dac_out) {
        /* store the DACx_OUT0 calr flag */
        if(0U != (DAC_CALR(dac_periph) & DAC_OUT_CALR_FLAG_0)) {
            reval = SET;
        } else {
            /* illegal parameters */
        }
    } else if(DAC_OUT1 == dac_out) {
        /* store the DACx_OUT1 calr flag */
        if(0U != (DAC_CALR(dac_periph) & DAC_OUT_CALR_FLAG_1)) {
            reval = SET;
        } else {
            /* illegal parameters */
        }
    } else {
        /* illegal parameters */
    }
    return reval;
}

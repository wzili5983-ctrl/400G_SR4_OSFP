/*!
    \file    gd32e511_512_opa.c
    \brief   OPA driver

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

#include "gd32e511_512_opa.h"

/* mask for 5-bit NMOS trim value */
#define TRIM_NMOS_MASK      ((uint32_t)0x1FU)
/* mask for 5-bit PMOS trim value */
#define TRIM_PMOS_MASK      ((uint32_t)0x1FU)
/* position of PMOS trim value in register */
#define TRIM_PMOS_POS       ((uint32_t)8U)

/*!
    \brief      reset OPA (API_ID(0x0001U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void opa_deinit(void)
{
    /* reset OPA registers */
    rcu_periph_reset_enable(RCU_OPARST);
    rcu_periph_reset_disable(RCU_OPARST);
}

/*!
    \brief      enable OPA (API_ID(0x0002U))
    \param[in]  opa_periph: OPAx(x=0,1)
    \param[out] none
    \retval     none
*/
void opa_enable(uint32_t opa_periph)
{
    OPA_CTL(opa_periph) |= OPA_CTL_OPAEN;
}

/*!
    \brief      disable OPA (API_ID(0x0003U))
    \param[in]  opa_periph: OPAx(x=0,1)
    \param[out] none
    \retval     none
*/
void opa_disable(uint32_t opa_periph)
{
    OPA_CTL(opa_periph) &= ~OPA_CTL_OPAEN;
}

/*!
    \brief      configure OPA mode (API_ID(0x0004U))
    \param[in]  opa_periph: OPAx(x=0,1)
    \param[in]  mode: OPA mode
                only one parameter can be selected which is shown as below:
      \arg        OPA_MODE_INDEPENDENT: OPA in independent mode
      \arg        OPA_MODE_PGA: OPA in PGA mode
      \arg        OPA_MODE_FOLLOWER: OPA in follower mode
    \param[out] none
    \retval     none
*/
void opa_mode_config(uint32_t opa_periph, uint32_t mode)
{
    /* clear OPA mode bits */
    OPA_CTL(opa_periph) &= ~OPA_CTL_MODE;
    /* set OPA mode */
    OPA_CTL(opa_periph) |= (mode & OPA_CTL_MODE);
}

/*!
    \brief      configure OPA power mode (API_ID(0x0005U))
    \param[in]  opa_periph: OPAx(x=0,1)
    \param[in]  powermode: OPA power mode
                only one parameter can be selected which is shown as below:
      \arg        OPA_POWM_NORMAL: OPA in normal power mode
      \arg        OPA_POWM_LOW: OPA in low power mode
    \param[out] none
    \retval     none
*/
void opa_power_mode_config(uint32_t opa_periph, opa_power_mode_enum powermode)
{
    if(OPA_POWM_LOW == powermode) {
        OPA_CTL(opa_periph) |= OPA_CTL_LPEN;
    } else {
        OPA_CTL(opa_periph) &= ~OPA_CTL_LPEN;
    }
}

/*!
    \brief      configure OPA PGA gain (API_ID(0x0006U))
    \param[in]  opa_periph: OPAx(x=0,1)
    \param[in]  gain: OPA PGA gain
                only one parameter can be selected which is shown as below:
      \arg        OPA_PGA_GAIN_2: PGA gain is 2
      \arg        OPA_PGA_GAIN_4: PGA gain is 4
      \arg        OPA_PGA_GAIN_8: PGA gain is 8
      \arg        OPA_PGA_GAIN_16: PGA gain is 16
    \param[out] none
    \retval     none
*/
void opa_pga_gain_config(uint32_t opa_periph, uint32_t gain)
{
    /* clear OPA PGA gain bits */
    OPA_CTL(opa_periph) &= ~OPA_CTL_GAIN;
    /* set OPA PGA gain */
    OPA_CTL(opa_periph) |= (gain & OPA_CTL_GAIN);
}

/*!
    \brief      configure OPA positive input (API_ID(0x0007U))
    \param[in]  opa_periph: OPAx(x=0,1)
    \param[in]  positive_input: OPA positive input
                only one parameter can be selected which is shown as below:
      \arg        OPA_VP_SOURCE_GPIO: VINP connected to GPIO
      \arg        OPA_VP_SOURCE_DAC: VINP connected to DAC_OUT
    \param[out] none
    \retval     none
*/
void opa_positive_input_config(uint32_t opa_periph, opa_vp_source_enum positive_input)
{
    if(OPA_VP_SOURCE_DAC == positive_input) {
        OPA_CTL(opa_periph) |= OPA_CTL_VPSEL;
    } else {
        OPA_CTL(opa_periph) &= ~OPA_CTL_VPSEL;
    }
}

/*!
    \brief      configure OPA negative input (API_ID(0x0008U))
    \param[in]  opa_periph: OPAx(x=0,1)
    \param[in]  negative_input: OPA negative input
                only one parameter can be selected which is shown as below:
      \arg        OPA_VM_SOURCE_GPIO: VINM connected to GPIO
      \arg        OPA_VM_SOURCE_PGA: VINM connected to internal signal(PGA mode)
    \param[out] none
    \retval     none
*/
void opa_negative_input_config(uint32_t opa_periph, opa_vm_source_enum negative_input)
{
    if(OPA_VM_SOURCE_PGA == negative_input) {
        OPA_CTL(opa_periph) |= OPA_CTL_VMSEL;
    } else {
        OPA_CTL(opa_periph) &= ~OPA_CTL_VMSEL;
    }
}

/*!
    \brief      enable OPA calibration (API_ID(0x0009U))
    \param[in]  opa_periph: OPAx(x=0,1)
    \param[out] none
    \retval     none
*/
void opa_calibration_enable(uint32_t opa_periph)
{
    OPA_CTL(opa_periph) |= OPA_CTL_CALEN;
}

/*!
    \brief      disable OPA calibration (API_ID(0x000AU))
    \param[in]  opa_periph: OPAx(x=0,1)
    \param[out] none
    \retval     none
*/
void opa_calibration_disable(uint32_t opa_periph)
{
    OPA_CTL(opa_periph) &= ~OPA_CTL_CALEN;
}

/*!
    \brief      configure OPA calibration mode (API_ID(0x000BU))
    \param[in]  opa_periph: OPAx(x=0,1)
    \param[in]  calmode: OPA calibration mode
                only one parameter can be selected which is shown as below:
      \arg        OPA_CALSEL_NMOS: NMOS calibration (200mV applied on OPA inputs)
      \arg        OPA_CALSEL_PMOS: PMOS calibration (VDDA-200mV applied on OPA inputs)
    \param[out] none
    \retval     none
*/
void opa_calibration_mode_config(uint32_t opa_periph, opa_calsel_enum calmode)
{
    if(OPA_CALSEL_PMOS == calmode) {
        OPA_CTL(opa_periph) |= OPA_CTL_CALSEL;
    } else {
        OPA_CTL(opa_periph) &= ~OPA_CTL_CALSEL;
    }
}

/*!
    \brief      configure OPA trim mode (API_ID(0x000CU))
    \param[in]  opa_periph: OPAx(x=0,1)
    \param[in]  trimmode: OPA trim mode
                only one parameter can be selected which is shown as below:
      \arg        OPA_TRIMSEL_FACTORY: default factory values
      \arg        OPA_TRIMSEL_USER: user programmed values
    \param[out] none
    \retval     none
*/
void opa_trim_mode_config(uint32_t opa_periph, opa_trimsel_enum trimmode)
{
    if(OPA_TRIMSEL_USER == trimmode) {
        OPA_CTL(opa_periph) |= OPA_CTL_TRIMSEL;
    } else {
        OPA_CTL(opa_periph) &= ~OPA_CTL_TRIMSEL;
    }
}

/*!
    \brief      configure OPA normal mode NMOS trim value (API_ID(0x000DU))
    \param[in]  opa_periph: OPAx(x=0,1)
    \param[in]  trim_value: 5-bit NMOS trim value in normal mode
    \param[out] none
    \retval     none
*/
void opa_normal_mode_nmos_trim_config(uint32_t opa_periph, uint32_t trim_value)
{
    /* clear normal mode NMOS trim value bits */
    OPA_NMOT(opa_periph) &= ~OPA_NMOT_TRIM_NMOS;
    /* set normal mode NMOS trim value */
    OPA_NMOT(opa_periph) |= (trim_value & TRIM_NMOS_MASK);
}

/*!
    \brief      configure OPA normal mode PMOS trim value (API_ID(0x000EU))
    \param[in]  opa_periph: OPAx(x=0,1)
    \param[in]  trim_value: 5-bit PMOS trim value in normal mode
    \param[out] none
    \retval     none
*/
void opa_normal_mode_pmos_trim_config(uint32_t opa_periph, uint32_t trim_value)
{
    /* clear normal mode PMOS trim value bits */
    OPA_NMOT(opa_periph) &= ~OPA_NMOT_TRIM_PMOS;
    /* set normal mode PMOS trim value */
    OPA_NMOT(opa_periph) |= ((trim_value & TRIM_PMOS_MASK) << TRIM_PMOS_POS);
}

/*!
    \brief      configure OPA low power mode NMOS trim value (API_ID(0x000FU))
    \param[in]  opa_periph: OPAx(x=0,1)
    \param[in]  trim_value: 5-bit NMOS trim value in low power mode
    \param[out] none
    \retval     none
*/
void opa_lowpower_mode_nmos_trim_config(uint32_t opa_periph, uint32_t trim_value)
{
    /* clear low power mode NMOS trim value bits */
    OPA_LPOT(opa_periph) &= ~OPA_LPOT_TRIM_NMOS;
    /* set low power mode NMOS trim value */
    OPA_LPOT(opa_periph) |= (trim_value & TRIM_NMOS_MASK);
}

/*!
    \brief      configure OPA low power mode PMOS trim value (API_ID(0x0010U))
    \param[in]  opa_periph: OPAx(x=0,1)
    \param[in]  trim_value: 5-bit PMOS trim value in low power mode
    \param[out] none
    \retval     none
*/
void opa_lowpower_mode_pmos_trim_config(uint32_t opa_periph, uint32_t trim_value)
{
    /* clear low power mode PMOS trim value bits */
    OPA_LPOT(opa_periph) &= ~OPA_LPOT_TRIM_PMOS;
    /* set low power mode PMOS trim value */
    OPA_LPOT(opa_periph) |= ((trim_value & TRIM_PMOS_MASK) << TRIM_PMOS_POS);
}

/*!
    \brief      get OPA flag (API_ID(0x0011U))
    \param[in]  opa_periph: OPAx(x=0,1)
    \param[in]  flag: OPA flag
                only one parameter can be selected which is shown as below:
      \arg        OPA_FLAG_CALOUT: OPA calibration output flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus opa_flag_get(uint32_t opa_periph, uint32_t flag)
{
    FlagStatus reval;
    if(0U != (OPA_CTL(opa_periph) & flag)) {
        reval = SET;
    } else {
        reval = RESET;
    }
    return reval;
}

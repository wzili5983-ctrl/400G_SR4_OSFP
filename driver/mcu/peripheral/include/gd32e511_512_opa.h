/*!
    \file    gd32e511_512_opa.h
    \brief   definitions for the OPA
    
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

#ifndef GD32E511_512_OPA_H
#define GD32E511_512_OPA_H

#include "gd32e511_512.h"

/* OPA definitions */
#define OPA0                              OPA_BASE                        /*!< OPA0 periph address */
#define OPA1                              (OPA_BASE + 0x10U)              /*!< OPA1 periph address */

/* registers definitions */
#define OPA_CTL(opa)                       REG32((opa) + 0x00000000U)      /*!< OPA control register */
#define OPA_NMOT(opa)                      REG32((opa) + 0x00000004U)      /*!< OPA normal mode offset trim register */
#define OPA_LPOT(opa)                      REG32((opa) + 0x00000008U)      /*!< OPA low-power mode offset trim register */

/* bits definitions */
/* OPA_CTL */
#define OPA_CTL_OPAEN                    BIT(0)                          /*!< OPA enable */
#define OPA_CTL_LPEN                     BIT(1)                          /*!< OPA low power mode enable */
#define OPA_CTL_MODE                     BITS(2,3)                       /*!< OPA mode selection */
#define OPA_CTL_GAIN                     BITS(4,5)                       /*!< OPA PGA gain value */
#define OPA_CTL_VMSEL                    BIT(9)                          /*!< OPA inverting input selection */
#define OPA_CTL_VPSEL                    BIT(10)                         /*!< OPA non-inverted input selection */
#define OPA_CTL_CALEN                    BIT(12)                         /*!< OPA calibration mode enable */
#define OPA_CTL_CALSEL                   BIT(13)                         /*!< OPA calibration mode selection */
#define OPA_CTL_TRIMSEL                  BIT(14)                         /*!< OPA trim value selection */
#define OPA_CTL_CALOUT                   BIT(15)                         /*!< OPA calibration output */

/* OPA_NMOT */
#define OPA_NMOT_TRIM_NMOS               BITS(0,4)                       /*!< OPA normal mode NMOS trim value */
#define OPA_NMOT_TRIM_PMOS               BITS(8,12)                      /*!< OPA normal mode PMOS trim value */

/* OPA_LPOT */
#define OPA_LPOT_TRIM_NMOS               BITS(0,4)                       /*!< OPA low-power mode NMOS trim value */
#define OPA_LPOT_TRIM_PMOS               BITS(8,12)                      /*!< OPA low-power mode PMOS trim value */

/* constants definitions */
/* OPA non-inverting input definitions */
typedef enum {
    OPA_VP_SOURCE_GPIO = 0U,                                             /*!< VINP connected to GPIO */
    OPA_VP_SOURCE_DAC  = 1U                                              /*!< VINP connected to DAC_OUT */
} opa_vp_source_enum;

/* OPA inverting input definitions */
typedef enum {
    OPA_VM_SOURCE_GPIO = 0U,                                             /*!< VINM connected to GPIO */
    OPA_VM_SOURCE_PGA  = 1U                                              /*!< VINM connected to internal signal(PGA mode) */
} opa_vm_source_enum;

/* OPA mode definitions */
#define OPA_MODE(regval)                 (BITS(2, 3) & ((uint32_t)(regval) << 2U))
#define OPA_MODE_INDEPENDENT             OPA_MODE(0)                     /*!< OPA in independent mode */
#define OPA_MODE_PGA                     OPA_MODE(2)                     /*!< OPA in PGA mode */
#define OPA_MODE_FOLLOWER                OPA_MODE(3)                     /*!< OPA in follower mode */

/* OPA power mode definitions */
typedef enum {
    OPA_POWM_NORMAL = 0U,                                               /*!< OPA in normal power mode */
    OPA_POWM_LOW    = 1U                                                /*!< OPA in low power mode */
} opa_power_mode_enum;

/* OPA gain definitions */
#define OPA_GAIN(regval)                 (BITS(4, 5) & ((uint32_t)(regval) << 4U))
#define OPA_PGA_GAIN_2                       OPA_GAIN(0)                     /*!< OPA gain 2 */
#define OPA_PGA_GAIN_4                       OPA_GAIN(1)                     /*!< OPA gain 4 */
#define OPA_PGA_GAIN_8                       OPA_GAIN(2)                     /*!< OPA gain 8 */
#define OPA_PGA_GAIN_16                      OPA_GAIN(3)                     /*!< OPA gain 16 */

/* OPA calibration mode definitions */
typedef enum {
    OPA_CALSEL_NMOS = 0x00000000U,                                       /*!< NMOS calibration (200mV applied on OPA inputs) */
    OPA_CALSEL_PMOS = 0x00000001U                                        /*!< PMOS calibration (VDDA-200mV applied on OPA inputs) */
} opa_calsel_enum;

/* OPA trim value source definitions */
typedef enum {
    OPA_TRIMSEL_FACTORY = 0x00000000U,                                   /*!< Default factory values */
    OPA_TRIMSEL_USER = 0x00000001U                                       /*!< User programmed values */
} opa_trimsel_enum;

/* OPA flag definitions */
#define OPA_FLAG_CALOUT                  OPA_CTL_CALOUT                  /*!< OPA calibration output flag */


/* function declarations */
/* initialization functions */
/* reset OPA */
void opa_deinit(void);
/* enable OPA */
void opa_enable(uint32_t opa_periph);
/* disable OPA */
void opa_disable(uint32_t opa_periph);

/* configuration functions */
/* configure OPA mode */
void opa_mode_config(uint32_t opa_periph, uint32_t mode);
/* configure OPA power mode */
void opa_power_mode_config(uint32_t opa_periph, opa_power_mode_enum powermode);
/* configure OPA PGA gain */
void opa_pga_gain_config(uint32_t opa_periph, uint32_t gain);
/* configure OPA positive input */
void opa_positive_input_config(uint32_t opa_periph, opa_vp_source_enum positive_input);
/* configure OPA negative input */
void opa_negative_input_config(uint32_t opa_periph, opa_vm_source_enum negative_input);

/* calibration functions */
/* enable OPA calibration */
void opa_calibration_enable(uint32_t opa_periph);
/* disable OPA calibration */
void opa_calibration_disable(uint32_t opa_periph);
/* configure OPA calibration mode */
void opa_calibration_mode_config(uint32_t opa_periph, opa_calsel_enum calmode);
/* configure OPA trim mode */
void opa_trim_mode_config(uint32_t opa_periph, opa_trimsel_enum trimmode);
/* configure OPA normal mode NMOS trim value */
void opa_normal_mode_nmos_trim_config(uint32_t opa_periph, uint32_t trim_value);
/* configure OPA normal mode PMOS trim value */
void opa_normal_mode_pmos_trim_config(uint32_t opa_periph, uint32_t trim_value);
/* configure OPA low power mode NMOS trim value */
void opa_lowpower_mode_nmos_trim_config(uint32_t opa_periph, uint32_t trim_value);
/* configure OPA low power mode PMOS trim value */
void opa_lowpower_mode_pmos_trim_config(uint32_t opa_periph, uint32_t trim_value);

/* flag and interrupt functions */
/* get OPA flag */
FlagStatus opa_flag_get(uint32_t opa_periph, uint32_t flag);

#endif /* GD32E511_512_OPA_H */

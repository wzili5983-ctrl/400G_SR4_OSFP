/*!
    \file    gd32e511_512_exti.c
    \brief   EXTI driver

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

#include "gd32e511_512_exti.h"

#define EXTI_INTEN_RESET_VAL    ((uint32_t)0x0F940000U)      /*!< EXTI INTEN registers reset value */
#define EXTI_REG_RESET_VAL      ((uint32_t)0x00000000U)      /*!< EXTI registers reset value for EXTI deint */
#define EXTI_LINE_NUM_MASK_1    ((uint32_t)0x0FFFFFFFU)      /*!< EXTI line number mask 1 for registers EXTI_INTEN / EXTI_EVEN */
#define EXTI_LINE_NUM_MASK_2    ((uint32_t)0x006BFFFFU)      /*!< EXTI line number mask 2 for registers EXTI_RTEN / EXTI_FTEN / EXTI_SWIEV / EXTI_PD */
/*!
    \brief      deinitialize the EXTI (API_ID(0x0001U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void exti_deinit(void)
{
    /* reset the value of all the EXTI registers */
    EXTI_INTEN = EXTI_INTEN_RESET_VAL;
    EXTI_EVEN  = EXTI_REG_RESET_VAL;
    EXTI_RTEN  = EXTI_REG_RESET_VAL;
    EXTI_FTEN  = EXTI_REG_RESET_VAL;
    EXTI_SWIEV = EXTI_REG_RESET_VAL;
}

/*!
    \brief      initialize the EXTI, enable the configuration of EXTI initialize (API_ID(0x0002U))
    \param[in]  linex: EXTI line number, refer to exti_line_enum
                only one parameter can be selected which is shown as below:
      \arg        EXTI_x (x=0..17,19,21,22): EXTI line x
    \param[in]  mode: interrupt or event mode, refer to exti_mode_enum
                only one parameter can be selected which is shown as below:
      \arg        EXTI_INTERRUPT: interrupt mode
      \arg        EXTI_EVENT: event mode
    \param[in]  trig_type: interrupt trigger type, refer to exti_trig_type_enum
                only one parameter can be selected which is shown as below:
      \arg        EXTI_TRIG_RISING: rising edge trigger
      \arg        EXTI_TRIG_FALLING: falling trigger
      \arg        EXTI_TRIG_BOTH: rising and falling trigger
      \arg        EXTI_TRIG_NONE: without rising edge or falling edge trigger
    \param[out] none
    \retval     none
*/
void exti_init(exti_line_enum linex, \
               exti_mode_enum mode, \
               exti_trig_type_enum trig_type)
{
    uint32_t line_num;
    line_num = (uint32_t)linex & EXTI_LINE_NUM_MASK_2;
    /* reset the EXTI line x */
    EXTI_INTEN &= ~(uint32_t)line_num;
    EXTI_EVEN  &= ~(uint32_t)line_num;
    EXTI_RTEN  &= ~(uint32_t)line_num;
    EXTI_FTEN  &= ~(uint32_t)line_num;

    /* set the EXTI mode and enable the interrupts or events from EXTI line x */
    switch(mode) {
    case EXTI_INTERRUPT:
        EXTI_INTEN |= (uint32_t)line_num;
        break;
    case EXTI_EVENT:
        EXTI_EVEN |= (uint32_t)line_num;
        break;
    default:
        break;
    }

    /* set the EXTI trigger type */
    switch(trig_type) {
    case EXTI_TRIG_RISING:
        EXTI_RTEN |= (uint32_t)line_num;
        EXTI_FTEN &= ~(uint32_t)line_num;
        break;
    case EXTI_TRIG_FALLING:
        EXTI_RTEN &= ~(uint32_t)line_num;
        EXTI_FTEN |= (uint32_t)line_num;
        break;
    case EXTI_TRIG_BOTH:
        EXTI_RTEN |= (uint32_t)line_num;
        EXTI_FTEN |= (uint32_t)line_num;
        break;
    case EXTI_TRIG_NONE:
    default:
        break;
    }
}

/*!
    \brief      enable the interrupts from EXTI line x  (API_ID(0x0003U))
    \param[in]  linex: EXTI line number, refer to exti_line_enum
                only one parameter can be selected which is shown as below:
      \arg        EXTI_x (x=0..27): EXTI line x
    \param[out] none
    \retval     none
*/
void exti_interrupt_enable(exti_line_enum linex)
{
    EXTI_INTEN |= (uint32_t)linex & EXTI_LINE_NUM_MASK_1;
}

/*!
    \brief      disable the interrupt from EXTI line x (API_ID(0x0004U))
    \param[in]  linex: EXTI line number, refer to exti_line_enum
                only one parameter can be selected which is shown as below:
      \arg        EXTI_x (x=0..27): EXTI line x
    \param[out] none
    \retval     none
*/
void exti_interrupt_disable(exti_line_enum linex)
{
    EXTI_INTEN &= ~(uint32_t)linex & EXTI_LINE_NUM_MASK_1;
}

/*!
    \brief      enable the events from EXTI line x (API_ID(0x0005U))
    \param[in]  linex: EXTI line number, refer to exti_line_enum
                only one parameter can be selected which is shown as below:
      \arg        EXTI_x (x=0..27): EXTI line x
    \param[out] none
    \retval     none
*/
void exti_event_enable(exti_line_enum linex)
{
    EXTI_EVEN |= (uint32_t)linex & EXTI_LINE_NUM_MASK_1;
}

/*!
    \brief      disable the events from EXTI line x (API_ID(0x0006U))
    \param[in]  linex: EXTI line number, refer to exti_line_enum
                only one parameter can be selected which is shown as below:
      \arg        EXTI_x (x=0..27): EXTI line x
    \param[out] none
    \retval     none
*/
void exti_event_disable(exti_line_enum linex)
{
    EXTI_EVEN &= ~(uint32_t)linex & EXTI_LINE_NUM_MASK_1;
}

/*!
    \brief      enable EXTI software interrupt event (API_ID(0x0007U))
    \param[in]  linex: EXTI line number, refer to exti_line_enum
                only one parameter can be selected which is shown as below:
      \arg        EXTI_x (x=0..17,19,21,22): EXTI line x
    \param[out] none
    \retval     none
*/
void exti_software_interrupt_enable(exti_line_enum linex)
{
    EXTI_SWIEV |= (uint32_t)linex & EXTI_LINE_NUM_MASK_2;
}

/*!
    \brief      disable EXTI software interrupt event (API_ID(0x0008U))
    \param[in]  linex: EXTI line number, refer to exti_line_enum
                only one parameter can be selected which is shown as below:
      \arg        EXTI_x (x=0..17,19,21,22): EXTI line x
    \param[out] none
    \retval     none
*/
void exti_software_interrupt_disable(exti_line_enum linex)
{
    EXTI_SWIEV &= ~(uint32_t)linex & EXTI_LINE_NUM_MASK_2;
}

/*!
    \brief      get EXTI line x interrupt pending flag (API_ID(0x0009U))
    \param[in]  linex: EXTI line number, refer to exti_line_enum
                only one parameter can be selected which is shown as below:
      \arg        EXTI_x (x=0..17,19,21,22): EXTI line x
    \param[out] none
    \retval     FlagStatus: status of flag (RESET or SET)
*/
FlagStatus exti_flag_get(exti_line_enum linex)
{
    FlagStatus reval;
    if(RESET != (EXTI_PD & (uint32_t)linex)) {
        reval = SET;
    } else {
        reval = RESET;
    }
    return reval;
}

/*!
    \brief      clear EXTI line x interrupt pending flag (API_ID(0x000AU))
    \param[in]  linex: EXTI line number, refer to exti_line_enum
                only one parameter can be selected which is shown as below:
      \arg        EXTI_x (x=0..17,19,21,22): EXTI line x
    \param[out] none
    \retval     none
*/
void exti_flag_clear(exti_line_enum linex)
{
    EXTI_PD = (uint32_t)linex & EXTI_LINE_NUM_MASK_2;
}

/*!
    \brief      get EXTI line x interrupt pending flag (API_ID(0x000BU))
    \param[in]  linex: EXTI line number, refer to exti_line_enum
                only one parameter can be selected which is shown as below:
      \arg        EXTI_x (x=0..17,19,21,22): EXTI line x
    \param[out] none
    \retval     FlagStatus: status of flag (RESET or SET)
*/
FlagStatus exti_interrupt_flag_get(exti_line_enum linex)
{
    FlagStatus reval;
    if(RESET != (EXTI_PD & (uint32_t)linex)) {
        reval = SET;
    } else {
        reval = RESET;
    }
    return reval;
}

/*!
    \brief      clear EXTI line x interrupt pending flag (API_ID(0x000CU))
    \param[in]  linex: EXTI line number, refer to exti_line_enum
                only one parameter can be selected which is shown as below:
      \arg        EXTI_x (x=0..17,19,21,22): EXTI line x
    \param[out] none
    \retval     none
*/
void exti_interrupt_flag_clear(exti_line_enum linex)
{
    EXTI_PD = (uint32_t)linex & EXTI_LINE_NUM_MASK_2;
}

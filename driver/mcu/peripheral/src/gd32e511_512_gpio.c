/*!
    \file    gd32e511_512_gpio.c
    \brief   GPIO driver

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

#include "gd32e511_512_gpio.h"

/* GPIO parameter mask */
#define GPIO_MODE_MASK1          ((uint32_t)0x00000003U)    /*!< GPIO mode mask */
#define GPIO_PULL_MASK1          ((uint32_t)0x00000003U)    /*!< GPIO pull-up/pull-down mask */
#define GPIO_OSPD_MASK1          ((uint32_t)0x00000003U)    /*!< GPIO output max speed mask */
#define GPIO_AFR_MASK1           ((uint32_t)0x0000000FU)    /*!< GPIO alternate function mask */

/*!
    \brief      reset GPIO port (API_ID(0x0001U))
    \param[in]  gpio_periph: GPIOx(x = A,B,C..G)
                only one parameter can be selected which is shown as below:
      \arg        GPIOx(x = A,B,C..G)
    \param[out] none
    \retval     none
*/
void gpio_deinit(uint32_t gpio_periph)
{
    switch(gpio_periph) {
    case GPIOA:
        /* reset GPIOA */
        rcu_periph_reset_enable(RCU_GPIOARST);
        rcu_periph_reset_disable(RCU_GPIOARST);
        break;
    case GPIOB:
        /* reset GPIOB */
        rcu_periph_reset_enable(RCU_GPIOBRST);
        rcu_periph_reset_disable(RCU_GPIOBRST);
        break;
    case GPIOC:
        /* reset GPIOC */
        rcu_periph_reset_enable(RCU_GPIOCRST);
        rcu_periph_reset_disable(RCU_GPIOCRST);
        break;
    case GPIOD:
        /* reset GPIOD */
        rcu_periph_reset_enable(RCU_GPIODRST);
        rcu_periph_reset_disable(RCU_GPIODRST);
        break;
    case GPIOE:
        /* reset GPIOE */
        rcu_periph_reset_enable(RCU_GPIOERST);
        rcu_periph_reset_disable(RCU_GPIOERST);
        break;
    case GPIOF:
        /* reset GPIOF */
        rcu_periph_reset_enable(RCU_GPIOFRST);
        rcu_periph_reset_disable(RCU_GPIOFRST);
        break;
    case GPIOG:
        /* reset GPIOG */
        rcu_periph_reset_enable(RCU_GPIOGRST);
        rcu_periph_reset_disable(RCU_GPIOGRST);
        break;
    default:
        break;
    }
}

/*!
    \brief      set GPIO mode (API_ID(0x0002U))
    \param[in]  gpio_periph: GPIOx(x = A,B,C..G)
                only one parameter can be selected which is shown as below:
      \arg        GPIOx(x = A,B,C..G)
    \param[in]  mode: gpio pin mode
                only one parameter can be selected which is shown as below:
      \arg        GPIO_MODE_INPUT: input mode
      \arg        GPIO_MODE_OUTPUT: output mode
      \arg        GPIO_MODE_AF: alternate function mode
      \arg        GPIO_MODE_ANALOG: analog mode
    \param[in]  pull_up_down: gpio pin with pull-up or pull-down resistor
                only one parameter can be selected which is shown as below:
      \arg        GPIO_PUPD_NONE: floating mode, no pull-up and pull-down resistors
      \arg        GPIO_PUPD_PULLUP: with pull-up resistor
      \arg        GPIO_PUPD_PULLDOWN:with pull-down resistor
    \param[in]  pin: GPIO pin
                one or more parameters can be selected which are shown as below:
      \arg        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    \param[out] none
    \retval     none
*/
void gpio_mode_set(uint32_t gpio_periph, uint32_t mode, uint32_t pull_up_down, uint32_t pin)
{
    uint16_t i;
    uint32_t ctl, pupd;

    ctl = GPIO_CTL(gpio_periph);
    pupd = GPIO_PUD(gpio_periph);

    for(i = 0U; i < 16U; i++) {
        if((1U << i) & pin) {
            /* clear the specified pin mode bits */
            ctl &= ~GPIO_MODE_MASK(i);
            /* set the specified pin mode bits */
            ctl |= GPIO_MODE_SET(i, mode & GPIO_MODE_MASK1);

            /* clear the specified pin pupd bits */
            pupd &= ~GPIO_PUPD_MASK(i);
            /* set the specified pin pupd bits */
            pupd |= GPIO_PUPD_SET(i, pull_up_down & GPIO_PULL_MASK1);
        }
    }

    GPIO_CTL(gpio_periph) = ctl;
    GPIO_PUD(gpio_periph) = pupd;
}

/*!
    \brief      set GPIO output type and speed (API_ID(0x0003U))
    \param[in]  gpio_periph: GPIOx(x = A,B,C..G)
                only one parameter can be selected which is shown as below:
      \arg        GPIOx(x = A,B,C..G)
    \param[in]  otype: gpio pin output mode
                only one parameter can be selected which is shown as below:
      \arg        GPIO_OTYPE_PP: push pull mode
      \arg        GPIO_OTYPE_OD: open drain mode
    \param[in]  speed: gpio pin output max speed
                only one parameter can be selected which is shown as below:
      \arg        GPIO_OSPEED_2MHZ: output max speed 2MHz
      \arg        GPIO_OSPEED_10MHZ: output max speed 10MHz
      \arg        GPIO_OSPEED_50MHZ: output max speed 50MHz
    \param[in]  pin: GPIO pin
                one or more parameters can be selected which are shown as below:
      \arg        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    \param[out] none
    \retval     none
*/
void gpio_output_options_set(uint32_t gpio_periph, uint8_t otype, uint32_t speed, uint32_t pin)
{
    uint16_t i;
    uint32_t ospeed;

    if(GPIO_OTYPE_OD == otype) {
        GPIO_OMODE(gpio_periph) |= (uint32_t)(pin & GPIO_PIN_ALL);
    } else {
        GPIO_OMODE(gpio_periph) &= (uint32_t)(~(pin & GPIO_PIN_ALL));
    }

    /* get the specified pin output speed bits value */
    ospeed = GPIO_OSPD(gpio_periph);

    for(i = 0U; i < 16U; i++) {
        if((1U << i) & pin) {
            /* clear the specified pin output speed bits */
            ospeed &= ~GPIO_OSPEED_MASK(i);
            /* set the specified pin output speed bits */
            ospeed |= GPIO_OSPEED_SET(i, speed & GPIO_OSPD_MASK1);
        }
    }
    GPIO_OSPD(gpio_periph) = ospeed;
}

/*!
    \brief      set GPIO pin bit (API_ID(0x0004U))
    \param[in]  gpio_periph: GPIOx(x = A,B,C..G)
                only one parameter can be selected which is shown as below:
      \arg        GPIOx(x = A,B,C..G)
    \param[in]  pin: GPIO pin
                one or more parameters can be selected which are shown as below:
      \arg        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    \param[out] none
    \retval     none
*/
void gpio_bit_set(uint32_t gpio_periph, uint32_t pin)
{
    GPIO_BOP(gpio_periph) = pin & GPIO_PIN_ALL ;
}

/*!
    \brief      reset GPIO pin bit (API_ID(0x0005U))
    \param[in]  gpio_periph: GPIOx(x = A,B,C..G)
                only one parameter can be selected which is shown as below:
      \arg        GPIOx(x = A,B,C..G)
    \param[in]  pin: GPIO pin
                one or more parameters can be selected which are shown as below:
      \arg        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    \param[out] none
    \retval     none
*/
void gpio_bit_reset(uint32_t gpio_periph, uint32_t pin)
{
    GPIO_BC(gpio_periph) = pin & GPIO_PIN_ALL;
}

/*!
    \brief      write data to the specified GPIO pin (API_ID(0x0006U))
    \param[in]  gpio_periph: GPIOx(x = A,B,C..G)
                only one parameter can be selected which is shown as below:
      \arg        GPIOx(x = A,B,C..G)
    \param[in]  pin: GPIO pin
                one or more parameters can be selected which are shown as below:
      \arg        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    \param[in]  bit_value: SET or RESET
                only one parameter can be selected which is shown as below:
      \arg        RESET: clear the port pin
      \arg        SET: set the port pin
    \param[out] none
    \retval     none
*/
void gpio_bit_write(uint32_t gpio_periph, uint32_t pin, bit_status bit_value)
{
    if(RESET != bit_value) {
        GPIO_BOP(gpio_periph) = pin & GPIO_PIN_ALL;
    } else {
        GPIO_BC(gpio_periph) = pin & GPIO_PIN_ALL;
    }
}

/*!
    \brief      write data to the specified GPIO port (API_ID(0x0007U))
    \param[in]  gpio_periph: GPIOx(x = A,B,C..G)
                only one parameter can be selected which is shown as below:
      \arg        GPIOx(x = A,B,C..G)
    \param[in]  data: specify the value to be written to the port output control register
    \param[out] none
    \retval     none
*/
void gpio_port_write(uint32_t gpio_periph, uint16_t data)
{
    GPIO_OCTL(gpio_periph) = (uint32_t)data;
}

/*!
    \brief      get GPIO pin input status (API_ID(0x0008U))
    \param[in]  gpio_periph: GPIOx(x = A,B,C..G)
                only one parameter can be selected which is shown as below:
      \arg        GPIOx(x = A,B,C..G)
    \param[in]  pin: GPIO pin
                one or more parameters can be selected which are shown as below:
      \arg        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    \param[out] none
    \retval     SET or RESET
*/
FlagStatus gpio_input_bit_get(uint32_t gpio_periph, uint32_t pin)
{
    FlagStatus reval;
    if((uint32_t)RESET != (GPIO_ISTAT(gpio_periph) & (pin))) {
        reval = SET;
    } else {
        reval = RESET;
    }
    return reval;
}

/*!
    \brief      get GPIO port input status (API_ID(0x0009U))
    \param[in]  gpio_periph: GPIOx(x = A,B,C..G)
                only one parameter can be selected which is shown as below:
      \arg        GPIOx(x = A,B,C..G)
    \param[out] none
    \retval     state of GPIO all pins
*/
uint16_t gpio_input_port_get(uint32_t gpio_periph)
{
    return (uint16_t)GPIO_ISTAT(gpio_periph);
}

/*!
    \brief      get GPIO pin output status (API_ID(0x000AU))
    \param[in]  gpio_periph: GPIOx(x = A,B,C..G)
                only one parameter can be selected which is shown as below:
      \arg        GPIOx(x = A,B,C..G)
    \param[in]  pin: GPIO pin
                one or more parameters can be selected which are shown as below:
      \arg        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    \param[out] none
    \retval     SET or RESET
*/
FlagStatus gpio_output_bit_get(uint32_t gpio_periph, uint32_t pin)
{
    FlagStatus reval;
    if((uint32_t)RESET != (GPIO_OCTL(gpio_periph) & (pin))) {
        reval = SET;
    } else {
        reval = RESET;
    }
    return reval;
}

/*!
    \brief      get GPIO port output status (API_ID(0x000BU))
    \param[in]  gpio_periph: GPIOx(x = A,B,C..G)
                only one parameter can be selected which is shown as below:
      \arg        GPIOx(x = A,B,C..G)
    \param[out] none
    \retval     state of GPIO all pins
*/
uint16_t gpio_output_port_get(uint32_t gpio_periph)
{
    return (uint16_t)GPIO_OCTL(gpio_periph);
}

/*!
    \brief      set GPIO alternate function (API_ID(0x000CU))
    \param[in]  gpio_periph: GPIOx(x = A,B,C..G)
                only one parameter can be selected which is shown as below:
      \arg        GPIOx(x = A,B,C..G)
    \param[in]  alt_func_num: GPIO pin af function, please refer to specific device datasheet
                only one parameter can be selected which is shown as below:
      \arg        GPIO_AF_0: alternate function 0
      \arg        GPIO_AF_1: alternate function 1
      \arg        GPIO_AF_2: alternate function 2
      \arg        GPIO_AF_3: alternate function 3
      \arg        GPIO_AF_4: alternate function 4
      \arg        GPIO_AF_5: alternate function 5
      \arg        GPIO_AF_6: alternate function 6
      \arg        GPIO_AF_7: alternate function 7
      \arg        GPIO_AF_8: alternate function 8
      \arg        GPIO_AF_9: alternate function 9
      \arg        GPIO_AF_10: alternate function 10
    \param[in]  pin: GPIO pin
                one or more parameters can be selected which are shown as below:
      \arg        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    \param[out] none
    \retval     none
*/
void gpio_af_set(uint32_t gpio_periph, uint32_t alt_func_num, uint32_t pin)
{
    uint16_t i;
    uint32_t afrl, afrh;

    afrl = GPIO_AFSEL0(gpio_periph);
    afrh = GPIO_AFSEL1(gpio_periph);

    for(i = 0U; i < 8U; i++) {
        if((1U << i) & pin) {
            /* clear the specified pin alternate function bits */
            afrl &= ~GPIO_AFR_MASK(i);
            afrl |= GPIO_AFR_SET(i, alt_func_num & GPIO_AFR_MASK1);
        }
    }

    for(i = 8U; i < 16U; i++) {
        if((1U << i) & pin) {
            /* clear the specified pin alternate function bits */
            afrh &= ~GPIO_AFR_MASK(i - 8U);
            afrh |= GPIO_AFR_SET(i - 8U, alt_func_num & GPIO_AFR_MASK1);
        }
    }

    GPIO_AFSEL0(gpio_periph) = afrl;
    GPIO_AFSEL1(gpio_periph) = afrh;
}

/*!
    \brief      lock GPIO pin bit (API_ID(0x000DU))
    \param[in]  gpio_periph: GPIOx(x = A,B,C..G)
                only one parameter can be selected which is shown as below:
      \arg        GPIOx(x = A,B,C..G)
    \param[in]  pin: GPIO pin
                one or more parameters can be selected which are shown as below:
      \arg        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    \param[out] none
    \retval     none
*/
void gpio_pin_lock(uint32_t gpio_periph, uint32_t pin)
{
    uint32_t lock = 0x00010000U;
    lock |= pin & GPIO_PIN_ALL;

    /* lock key writing sequence: write 1->write 0->write 1->read 0->read 1 */
    GPIO_LOCK(gpio_periph) = (uint32_t)lock;
    GPIO_LOCK(gpio_periph) = (uint32_t)(pin & GPIO_PIN_ALL);
    GPIO_LOCK(gpio_periph) = (uint32_t)lock;
    lock = GPIO_LOCK(gpio_periph);
    lock = GPIO_LOCK(gpio_periph);
}

/*!
    \brief      toggle GPIO pin status (API_ID(0x000EU))
    \param[in]  gpio_periph: GPIOx(x = A,B,C..G)
                only one parameter can be selected which is shown as below:
      \arg        GPIOx(x = A,B,C..G)
    \param[in]  pin: GPIO pin
                one or more parameters can be selected which are shown as below:
      \arg        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    \param[out] none
    \retval     none
*/
void gpio_bit_toggle(uint32_t gpio_periph, uint32_t pin)
{
    GPIO_TG(gpio_periph) = pin & GPIO_PIN_ALL;
}

/*!
    \brief      toggle GPIO port status (API_ID(0x000FU))
    \param[in]  gpio_periph: GPIOx(x = A,B,C..G)
                only one parameter can be selected which is shown as below:
      \arg        GPIOx(x = A,B,C..G)
    \param[out] none
    \retval     none
*/
void gpio_port_toggle(uint32_t gpio_periph)
{
    GPIO_TG(gpio_periph) = 0x0000FFFFU;
}

/*!
    \brief      enable GPIO pin output level hold (API_ID(0x0010U))
    \param[in]  gpio_periph: GPIOx(x = A,B,C..G)
                only one parameter can be selected which is shown as below:
      \arg        GPIOx(x = A,B,C..G)
    \param[in]  pin: GPIO pin
                one or more parameters can be selected which are shown as below:
      \arg        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    \param[out] none
    \retval     none
*/
void gpio_output_hold_enable(uint32_t gpio_periph, uint32_t pin)
{
    GPIO_OMODE(gpio_periph) |= (uint32_t)(pin << 16U);
}

/*!
    \brief      disable GPIO pin output level hold (API_ID(0x0011U))
    \param[in]  gpio_periph: GPIOx(x = A,B,C..G)
                only one parameter can be selected which is shown as below:
      \arg        GPIOx(x = A,B,C..G)
    \param[in]  pin: GPIO pin
                one or more parameters can be selected which are shown as below:
      \arg        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    \param[out] none
    \retval     none
*/
void gpio_output_hold_disable(uint32_t gpio_periph, uint32_t pin)
{
    GPIO_OMODE(gpio_periph) &= ~(uint32_t)(pin << 16U);
}

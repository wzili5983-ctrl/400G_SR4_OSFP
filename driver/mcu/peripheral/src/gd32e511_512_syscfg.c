/*!
    \file    gd32e511_512_syscfg.c
    \brief   SYSCFG driver

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

#include "gd32e511_512_syscfg.h"

#define DMA_RMP_MASK              ((uint32_t)0x00001FD0U)      /* bit mask of RMP */
#define HIGH_CURRENT_MASK         ((uint32_t)0x07FB0000U)      /* bit mask of HIGH CURRENT */
#define SYSCFG_LOCK_MASK          ((uint32_t)0x00000007U)      /* bit mask of SYSCFG LOCK */

/*!
    \brief      reset the SYSCFG registers (API_ID(0x0001U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void syscfg_deinit(void)
{
    rcu_periph_reset_enable(RCU_CFGCMPRST);
    rcu_periph_reset_disable(RCU_CFGCMPRST);
}

/*!
    \brief      configure the boot mode (API_ID(0x0002U))
    \param[in]  none
    \param[out] none
    \retval     boot mode is shown as below:
      \arg        SYSCFG_BOOTMODE_FLASH: boot from the main flash
      \arg        SYSCFG_BOOTMODE_SYSTEM: boot from the system flash memory
      \arg        SYSCFG_BOOTMODE_SRAM: boot from the embedded SRAM
*/
uint8_t syscfg_bootmode_get(void)
{
    /* get the SYSCFG_CFG0_BOOT_MODE bit */
    return (uint8_t)(SYSCFG_CFG0 & SYSCFG_CFG0_BOOT_MODE);
}

/*!
    \brief      enable the DMA channels remapping (API_ID(0x0003U))
    \param[in]  syscfg_dma_remap: specify the DMA channels to remap
                one or more parameters can be selected which are shown as below:
      \arg        SYSCFG_DMA_REMAP_TIMER16: remap TIMER16 channel0 and UP DMA requests to channel1(default channel0)
      \arg        SYSCFG_DMA_REMAP_TIMER15: remap TIMER15 channel2 and UP DMA requests to channel3(default channel2)
      \arg        SYSCFG_DMA_REMAP_USART0RX: remap USART0 Rx DMA request to channel4(default channel2)
      \arg        SYSCFG_DMA_REMAP_USART0TX: remap USART0 Tx DMA request to channel3(default channel1)
      \arg        SYSCFG_DMA_REMAP_ADC0: remap ADC0 DMA requests from channel0 to channel1
      \arg        SYSCFG_DMA_REMAP_ADC1: remap ADC1 DMA requests from channel2 to channel3
      \arg        SYSCFG_BOOT0_REMAP_PD2: remap BOOT0 PD2
      \arg        SYSCFG_PA11_REMAP_PA12: remap PA11 PA12
    \param[out] none
    \retval     none
*/
void syscfg_dma_remap_enable(uint32_t syscfg_dma_remap)
{
    SYSCFG_CFG0 |= syscfg_dma_remap & DMA_RMP_MASK;
}

/*!
    \brief      disable the DMA channels remapping (API_ID(0x0004U))
    \param[in]  syscfg_dma_remap: specify the DMA channels to remap
                one or more parameters can be selected which are shown as below:
      \arg        SYSCFG_DMA_REMAP_TIMER16: remap TIMER16 channel0 and UP DMA requests to channel1(default channel0)
      \arg        SYSCFG_DMA_REMAP_TIMER15: remap TIMER15 channel2 and UP DMA requests to channel3(default channel2)
      \arg        SYSCFG_DMA_REMAP_USART0RX: remap USART0 Rx DMA request to channel4(default channel2)
      \arg        SYSCFG_DMA_REMAP_USART0TX: remap USART0 Tx DMA request to channel3(default channel1)
      \arg        SYSCFG_DMA_REMAP_ADC0: remap ADC0 DMA requests from channel0 to channel1
      \arg        SYSCFG_DMA_REMAP_ADC1: remap ADC1 DMA requests from channel0 to channel1
      \arg        SYSCFG_BOOT0_REMAP_PD2: remap BOOT0 PD2
      \arg        SYSCFG_PA11_REMAP_PA12: remap PA11 PA12
    \param[out] none
    \retval     none
*/
void syscfg_dma_remap_disable(uint32_t syscfg_dma_remap)
{
    SYSCFG_CFG0 &= ~(syscfg_dma_remap & DMA_RMP_MASK);
}

/*!
    \brief      enable PBx(x=6,7,8,9,13,14), PCx(x=0,1), PGx(x=13,14) high current capability (API_ID(0x0005U))
    \param[in]  syscfg_high_current: specify the high current capability pin(s)
                one or more parameters can be selected which are shown as below:
      \arg        SYSCFG_PB6_HIGH_CURRENT: PB6 pin high current capability enable
      \arg        SYSCFG_PB7_HIGH_CURRENT: PB7 pin high current capability enable
      \arg        SYSCFG_PB9_HIGH_CURRENT: PB9 pin high current capability enable
      \arg        SYSCFG_PG14_HIGH_CURRENT: PG14 pin high current capability enable
      \arg        SYSCFG_PG13_HIGH_CURRENT: PG13 pin high current capability enable
      \arg        SYSCFG_PC1_HIGH_CURRENT: PC1 pin high current capability enable
      \arg        SYSCFG_PC0_HIGH_CURRENT: PC0 pin high current capability enable
      \arg        SYSCFG_PB14_HIGH_CURRENT: PB14 pin high current capability enable
      \arg        SYSCFG_PB13_HIGH_CURRENT: PB13 pin high current capability enable
      \arg        SYSCFG_PB8_HIGH_CURRENT: PB8 pin high current capability enable
    \param[out] none
    \retval     none
*/
void syscfg_high_current_enable(uint32_t syscfg_high_current)
{
    SYSCFG_CFG0 |= syscfg_high_current & HIGH_CURRENT_MASK;
}

/*!
    \brief      disable PBx(x=6,7,8,9,13,14), PCx(x=0,1), PGx(x=13,14) high current capability (API_ID(0x0006U))
    \param[in]  syscfg_high_current: specify the high current capability pin(s)
                one or more parameters can be selected which are shown as below:
      \arg        SYSCFG_PB6_HIGH_CURRENT: PB6 pin high current capability enable
      \arg        SYSCFG_PB7_HIGH_CURRENT: PB7 pin high current capability enable
      \arg        SYSCFG_PB9_HIGH_CURRENT: PB9 pin high current capability enable
      \arg        SYSCFG_PG14_HIGH_CURRENT: PG14 pin high current capability enable
      \arg        SYSCFG_PG13_HIGH_CURRENT: PG13 pin high current capability enable
      \arg        SYSCFG_PC1_HIGH_CURRENT: PC1 pin high current capability enable
      \arg        SYSCFG_PC0_HIGH_CURRENT: PC0 pin high current capability enable
      \arg        SYSCFG_PB14_HIGH_CURRENT: PB14 pin high current capability enable
      \arg        SYSCFG_PB13_HIGH_CURRENT: PB13 pin high current capability enable
      \arg        SYSCFG_PB8_HIGH_CURRENT: PB8 pin high current capability enable
    \param[out] none
    \retval     none
*/
void syscfg_high_current_disable(uint32_t syscfg_high_current)
{
    SYSCFG_CFG0 &= ~(syscfg_high_current & HIGH_CURRENT_MASK);
}

/*!
    \brief      enable VREF function (API_ID(0x0007U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void syscfg_vref_enable(void)
{
    SYSCFG_CFG2 |= SYSCFG_CFG2_VREF_BUF_EN;
}

/*!
    \brief      disable VREF function (API_ID(0x0008U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void syscfg_vref_disable(void)
{
    SYSCFG_CFG2 &= ~SYSCFG_CFG2_VREF_BUF_EN;
}

/*!
    \brief      configure the GPIO pin reset mode (API_ID(0x0009U))
    \param[in]  syscfg_pin_reset_mode: specifies the GPIO pin reset mode.
                only one parameter can be selected which is shown as below:
      \arg        SYSCFG_PIN_NRST: GPIO pin configuration will retain state across any reset event except for the POR event
      \arg        SYSCFG_PIN_RST: GPIO pin configuration is reset when any reset event occurs
    \param[out] none
    \retval     none
*/
void syscfg_pin_reset_mode_config(uint32_t syscfg_pin_reset_mode)
{
    uint32_t reg;

    reg = SYSCFG_CFG2;
    reg &= ~SYSCFG_CFG2_PIN_RSTMD;
    SYSCFG_CFG2 = (reg | (syscfg_pin_reset_mode & SYSCFG_CFG2_PIN_RSTMD));
}

/*!
    \brief      configure the GPIO pin as EXTI Line (API_ID(0x000AU))
    \param[in]  exti_port: specify the GPIO port used in EXTI
                only one parameter can be selected which is shown as below:
      \arg        EXTI_SOURCE_GPIOx(x = A,B,C,D,E,F,G): EXTI GPIO port
    \param[in]  exti_pin: specify the EXTI line
                only one parameter can be selected which is shown as below:
      \arg        EXTI_SOURCE_PINx(GPIOA x = 0..15,
                                   GPIOB x = 0..15,
                                   GPIOC x = 0..15,
                                   GPIOD x = 2.4.5.6.7.8.9,
                                   GPIOE x = 7.8,
                                   GPIOF x = 0.1.4.5.6.7,
                                   GPIOG x = 7.8.9.10.11.12.13.14): EXTI GPIO pin
    \param[out] none
    \retval     none
*/
void syscfg_exti_line_config(uint8_t exti_port, uint8_t exti_pin)
{
    uint32_t clear_exti_mask = ~((uint32_t)EXTI_SS_MASK << (EXTI_SS_MSTEP(exti_pin)));
    uint32_t config_exti_mask = ((uint32_t)exti_port) << (EXTI_SS_MSTEP(exti_pin));

    switch(exti_pin / EXTI_SS_JSTEP) {
    case EXTISS0:
        /* clear EXTI source line(0..3) */
        SYSCFG_EXTISS0 &= clear_exti_mask;
        /* configure EXTI source line(0..3) */
        SYSCFG_EXTISS0 |= config_exti_mask;
        break;
    case EXTISS1:
        /* clear EXTI source line(4..7) */
        SYSCFG_EXTISS1 &= clear_exti_mask;
        /* configure EXTI source line(4..7) */
        SYSCFG_EXTISS1 |= config_exti_mask;
        break;
    case EXTISS2:
        /* clear EXTI source line(8..11) */
        SYSCFG_EXTISS2 &= clear_exti_mask;
        /* configure EXTI source line(8..11) */
        SYSCFG_EXTISS2 |= config_exti_mask;
        break;
    case EXTISS3:
        /* clear EXTI source line(12..15) */
        SYSCFG_EXTISS3 &= clear_exti_mask;
        /* configure EXTI source line(12..15) */
        SYSCFG_EXTISS3 |= config_exti_mask;
        break;
    default:
        break;
    }
}

/*!
    \brief      connect TIMER0/14/15/16 break input to the selected parameter (API_ID(0x000BU))
    \param[in]  syscfg_lock: specify the parameter to be connected
                only one parameter can be selected which is shown as below:
      \arg        SYSCFG_LOCK_LOCKUP: Cortex-M33 lockup output connected to the break input
      \arg        SYSCFG_LOCK_SRAM_PARITY_ERROR: SRAM_PARITY check error connected to the break input
      \arg        SYSCFG_LOCK_LVD: LVD interrupt connected to the break input
    \param[out] none
    \retval     none
*/
void syscfg_lock_config(uint32_t syscfg_lock)
{
    SYSCFG_CFG2 |= syscfg_lock & SYSCFG_LOCK_MASK;
}

/*!
    \brief      get ECC error address in SYSCFG_CFG3 (API_ID(0x000CU))
    \param[in]  none
    \param[out] none
    \retval     the error address returned.
*/
uint32_t syscfg_ecc_error_address_get(void)
{
    return ((SYSCFG_CFG3 & SYSCFG_CFG3_SRAMECCEADDR) >> 17U);
}

/*!
    \brief      get ECC single-bit correctable error bit in SYSCFG_CFG3 (API_ID(0x000DU))
    \param[in]  none
    \param[out] none
    \retval     the error bit returned.
*/
uint32_t syscfg_ecc_error_bit_get(void)
{
    return ((SYSCFG_CFG3 & SYSCFG_CFG3_SRAMECCSERRBIT) >> 9U);
}

/*!
    \brief      unlock the bit8 of SYSCFG_MEMRMP register operation (API_ID(0x000EU))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void syscfg_flash_bank_remap_unlock(void)
{
    uint32_t key = (SYSCFG_FMC_SWP_UNLOCK_KEY << 16U);

    SYSCFG_MEMRMP &= ~SYSCFG_MEMRMP_UNLOCK_KEY;
    SYSCFG_MEMRMP |= key;
}

/*!
    \brief      enable flash bank remap (API_ID(0x000FU))
    \param[in]  value:
                only one parameter can be selected which is shown as below:
      \arg        SYSCFG_FLASH_BANK0_MAPPED: Flash Bank 1 mapped at 0x08000000,and Flash Bank 0 mapped at 0x08040000(depend on the specific series of bank size)
      \arg        SYSCFG_FLASH_BANK1_MAPPED: Flash Bank 0 mapped at 0x08000000,and Flash Bank 1 mapped at 0x08040000(depend on the specific series of bank size)
      \param[out] none
    \retval     none
*/
void syscfg_flash_bank_remap_set(uint32_t value)
{
    if(SYSCFG_FLASH_BANK0_MAPPED == value) {
        SYSCFG_MEMRMP &= ~SYSCFG_MEMRMP_FMC_SWP;
    } else {
        SYSCFG_MEMRMP |= SYSCFG_MEMRMP_FMC_SWP;
    }
}

/*!
    \brief      check if the specified flag in SYSCFG_CFG2 is set or not (API_ID(0x0010U))
    \param[in]  syscfg_flag: specify the flag in SYSCFG_CFG2 to check.
      \arg        SYSCFG_SRAM_PCEF: SRAM parity check error flag.
    \param[out] none
    \retval     the syscfg_flag state returned (SET or RESET).
  */
FlagStatus syscfg_flag_get(uint32_t syscfg_flag)
{
    FlagStatus retval = RESET;

    if((SYSCFG_CFG2 & syscfg_flag) != (uint32_t)RESET) {
        retval = SET;
    }

    return retval;
}

/*!
    \brief      clear the flag in SYSCFG_CFG2 by writing 1 (API_ID(0x0011U))
    \param[in]  syscfg_flag: Specify the flag in SYSCFG_CFG2 to clear.
      \arg        SYSCFG_SRAM_PCEF: SRAM parity check error flag.
    \param[out] none
    \retval     none
*/
void syscfg_flag_clear(uint32_t syscfg_flag)
{
    SYSCFG_CFG2 |= (uint32_t) syscfg_flag & SYSCFG_CFG2_SRAM_PCEF;
}

/*!
    \brief      enable ECC interrupt (API_ID(0x0012U))
    \param[in]  inttype: interrupt type
                only one parameter can be selected which is shown as below:
      \arg        SYSCFG_INT_ECCME: SRAM multi bits non-correction event interrupt
      \arg        SYSCFG_INT_ECCSE: SRAM single bits non-correction event interrupt
      \arg        SYSCFG_INT_ECCME_FLASH: FLASH multi-bit correction event detected interrupt
      \arg        SYSCFG_INT_ECCSE_FLASH: FLASH single-bit correction event detected interrupt
      \arg        SYSCFG_INT_DECCE_FLASH: FLASH double bit non-correction event detected interrupt
    \param[out] none
    \retval     none
*/
void syscfg_interrupt_enable(syscfg_interrupt_enum inttype)
{
    SYSCFG_CFG3 |= (uint32_t)inttype;
}

/*!
    \brief      disable ECC interrupt (API_ID(0x0013U))
    \param[in]  inttype: interrupt type
                only one parameter can be selected which is shown as below:
      \arg        SYSCFG_INT_ECCME: SRAM multi bits non-correction event interrupt
      \arg        SYSCFG_INT_ECCSE: SRAM single bits non-correction event interrupt
      \arg        SYSCFG_INT_ECCME_FLASH: FLASH multi-bit correction event detected interrupt
      \arg        SYSCFG_INT_ECCSE_FLASH: FLASH single-bit correction event detected interrupt
      \arg        SYSCFG_INT_DECCE_FLASH: FLASH double bit non-correction event detected interrupt
    \param[out] none
    \retval     none
*/
void syscfg_interrupt_disable(syscfg_interrupt_enum inttype)
{
    SYSCFG_CFG3 &= ~(uint32_t)inttype;
}

/*!
    \brief      check if the specified interrupt flag in SYSCFG_SR is set or not (API_ID(0x0014U))
    \param[in]  int_flag: specify the flag in SYSCFG_SR to check.
                only one parameter can be selected which is shown as below:
      \arg        SYSCFG_INT_FLAG_ECCME: SRAM multi bits non-correction event interrupt flag
      \arg        SYSCFG_INT_FLAG_ECCSE: SRAM single bits non-correction event interrupt flag
      \arg        SYSCFG_INT_FLAG_ECCME_FLASH: FLASH multi-bit correction event detected interrupt flag
      \arg        SYSCFG_INT_FLAG_ECCSE_FLASH: FLASH single-bit correction event detected interrupt flag
      \arg        SYSCFG_INT_FLAG_DECCE_FLASH: FLASH double bit non-correction event detected interrupt flag
    \param[out] none
    \retval     the int_flag state returned (SET or RESET).
*/
FlagStatus syscfg_interrupt_flag_get(syscfg_interrupt_flag_enum int_flag)
{
    FlagStatus retval = RESET;

    if((SYSCFG_STAT & (uint32_t)int_flag) != (uint32_t)RESET) {
        retval = SET;
    }

    return retval;
}

/*!
    \brief      clear the interrupt flag in SYSCFG_SR by writing 1 (API_ID(0x0015U))
    \param[in]  int_flag: specify the flag in SYSCFG_SR to check.
                only one parameter can be selected which is shown as below:
      \arg        SYSCFG_INT_FLAG_ECCME: SRAM multi bits non-correction event interrupt flag
      \arg        SYSCFG_INT_FLAG_ECCSE: SRAM single bits non-correction event interrupt flag
      \arg        SYSCFG_INT_FLAG_ECCME_FLASH: FLASH multi-bit correction event detected interrupt flag
      \arg        SYSCFG_INT_FLAG_ECCSE_FLASH: FLASH single-bit correction event detected interrupt flag
      \arg        SYSCFG_INT_FLAG_DECCE_FLASH: FLASH double bit non-correction event detected interrupt flag
    \param[out] none
    \retval     the int_flag state returned (SET or RESET).
*/
void syscfg_interrupt_flag_clear(syscfg_interrupt_flag_enum int_flag)
{
    SYSCFG_STAT |= (uint32_t) int_flag;
}

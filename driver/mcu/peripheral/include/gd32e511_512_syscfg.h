/*!
    \file    gd32e511_512_syscfg.h
    \brief   definitions for the SYSCFG

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

#ifndef GD32E511_512_SYSCFG_H
#define GD32E511_512_SYSCFG_H

#include "gd32e511_512.h"

/* SYSCFG definitions */
#define SYSCFG                              SYSCFG_BASE

/* registers definitions */
#define SYSCFG_CFG0                         REG32(SYSCFG + 0x00U)               /*!< system configuration register 0 */
#define SYSCFG_EXTISS0                      REG32(SYSCFG + 0x08U)               /*!< EXTI sources selection register 0 */
#define SYSCFG_EXTISS1                      REG32(SYSCFG + 0x0CU)               /*!< EXTI sources selection register 1 */
#define SYSCFG_EXTISS2                      REG32(SYSCFG + 0x10U)               /*!< EXTI sources selection register 2 */
#define SYSCFG_EXTISS3                      REG32(SYSCFG + 0x14U)               /*!< EXTI sources selection register 3 */
#define SYSCFG_CFG2                         REG32(SYSCFG + 0x18U)               /*!< system configuration register 2 */
#define SYSCFG_STAT                         REG32(SYSCFG + 0x28U)               /*!< system configuration status register */
#define SYSCFG_CFG3                         REG32(SYSCFG + 0x2CU)               /*!< system configuration ECC config register */
#define SYSCFG_MEMRMP                       REG32(SYSCFG + 0x30U)               /*!< system configuration memory remap register */

/* SYSCFG_CFG0 bits definitions */
#define SYSCFG_CFG0_BOOT_MODE               BITS(0,1)                           /*!< SYSCFG memory remap config, read only */
#define SYSCFG_CFG0_PA11_PA12_RMP           BIT(4)                              /*!< PA11 and PA12 remapping bit for small packages (28 and 20 pins) */
#define SYSCFG_CFG0_BOOT0_PD2_RMP           BIT(6)                              /*!< BOOT0 and PD2 remapping bit */
#define SYSCFG_CFG0_ADC1_DMA_RMP            BIT(7)                              /*!< ADC1 DMA remap config */
#define SYSCFG_CFG0_ADC0_DMA_RMP            BIT(8)                              /*!< ADC0 DMA remap config */
#define SYSCFG_CFG0_USART0_TX_DMA_RMP       BIT(9)                              /*!< USART0 Tx DMA remap config */
#define SYSCFG_CFG0_USART0_RX_DMA_RMP       BIT(10)                             /*!< USART0 Rx DMA remap config */
#define SYSCFG_CFG0_TIMER15_DMA_RMP         BIT(11)                             /*!< TIMER 15 DMA remap config */
#define SYSCFG_CFG0_TIMER16_DMA_RMP         BIT(12)                             /*!< TIMER 16 DMA remap config */
#define SYSCFG_CFG0_PB6_HCCE                BIT(16)                             /*!< PB6 pin high current capability enable */
#define SYSCFG_CFG0_PB7_HCCE                BIT(17)                             /*!< PB7 pin high current capability enable */
#define SYSCFG_CFG0_PB9_HCCE                BIT(19)                             /*!< PB9 pin high current capability enable */
#define SYSCFG_CFG0_PG14_HCCE               BIT(20)                             /*!< PG14 pin high current capability enable */
#define SYSCFG_CFG0_PG13_HCCE               BIT(21)                             /*!< PG13 pin high current capability enable */
#define SYSCFG_CFG0_PC1_HCCE                BIT(22)                             /*!< PC1 pin high current capability enable */
#define SYSCFG_CFG0_PC0_HCCE                BIT(23)                             /*!< PC0 pin high current capability enable */
#define SYSCFG_CFG0_PB14_HCCE               BIT(24)                             /*!< PB14 pin high current capability enable */
#define SYSCFG_CFG0_PB13_HCCE               BIT(25)                             /*!< PB13 pin high current capability enable */
#define SYSCFG_CFG0_PB8_HCCE                BIT(26)                             /*!< PB8 pin high current capability enable */

/* SYSCFG_EXTISS0 bits definitions */
#define SYSCFG_EXTISS0_EXTI0_SS             BITS(0,3)                           /*!< EXTI 0 configuration */
#define SYSCFG_EXTISS0_EXTI1_SS             BITS(4,7)                           /*!< EXTI 1 configuration */
#define SYSCFG_EXTISS0_EXTI2_SS             BITS(8,11)                          /*!< EXTI 2 configuration */
#define SYSCFG_EXTISS0_EXTI3_SS             BITS(12,15)                         /*!< EXTI 3 configuration */

/* SYSCFG_EXTISS1 bits definitions */
#define SYSCFG_EXTISS1_EXTI4_SS             BITS(0,3)                           /*!< EXTI 4 configuration */
#define SYSCFG_EXTISS1_EXTI5_SS             BITS(4,7)                           /*!< EXTI 5 configuration */
#define SYSCFG_EXTISS1_EXTI6_SS             BITS(8,11)                          /*!< EXTI 6 configuration */
#define SYSCFG_EXTISS1_EXTI7_SS             BITS(12,15)                         /*!< EXTI 7 configuration */

/* SYSCFG_EXTISS2 bits definitions */
#define SYSCFG_EXTISS2_EXTI8_SS             BITS(0,3)                           /*!< EXTI 8 configuration */
#define SYSCFG_EXTISS2_EXTI9_SS             BITS(4,7)                           /*!< EXTI 9 configuration */
#define SYSCFG_EXTISS2_EXTI10_SS            BITS(8,11)                          /*!< EXTI 10 configuration */
#define SYSCFG_EXTISS2_EXTI11_SS            BITS(12,15)                         /*!< EXTI 11 configuration */

/* SYSCFG_EXTISS3 bits definitions */
#define SYSCFG_EXTISS3_EXTI12_SS            BITS(0,3)                           /*!< EXTI 12 configuration */
#define SYSCFG_EXTISS3_EXTI13_SS            BITS(4,7)                           /*!< EXTI 13 configuration */
#define SYSCFG_EXTISS3_EXTI14_SS            BITS(8,11)                          /*!< EXTI 14 configuration */
#define SYSCFG_EXTISS3_EXTI15_SS            BITS(12,15)                         /*!< EXTI 15 configuration */

/* SYSCFG_CFG2 bits definitions */
#define SYSCFG_CFG2_LOCKUP_LOCK             BIT(0)                              /*!< enable and lock the LOCKUP (Hardfault) output of Cortex-M23 with break input of TIMER0/14/15/16 */
#define SYSCFG_CFG2_SRAM_PARITY_ERROR_LOCK  BIT(1)                              /*!< enable and lock the SRAM_PARITY error signal with break input of TIMER0/14/15/16 */
#define SYSCFG_CFG2_LVD_LOCK                BIT(2)                              /*!< enable and lock the LVD connection with TIMER0/14/15/16 break input and also the LVD_EN and LVDSEL[2:0] bits of the power control interface */
#define SYSCFG_CFG2_SRAM_PCEF               BIT(8)                              /*!< SRAM parity check error flag */
#define SYSCFG_CFG2_VREF_BUF_EN             BIT(16)                             /*!< VREF enable */
#define SYSCFG_CFG2_PIN_RSTMD               BIT(24)                             /*!< pin reset mode */

/* SYSCFG_STAT bits definitions */
#define SYSCFG_STAT_ECCMEIF                 BIT(0)                              /*!< SRAM multi bits non-correction event flag */
#define SYSCFG_STAT_ECCSEIF                 BIT(1)                              /*!< SRAM single bit correction event flag */
#define SYSCFG_STAT_FLASHECCMEIF            BIT(6)                              /*!< FLASH multi-bit correction event detected flag */
#define SYSCFG_STAT_FLASHECCSEIF            BIT(7)                              /*!< FLASH single-bit correction event detected flag*/
#define SYSCFG_STAT_FLASHECCDEIF            BIT(8)                              /*!< FLASH double bit non-correction event detected flag*/

/* SYSCFG_CFG3 bits definitions */
#define SYSCFG_CFG3_SRAMECCMEIE             BIT(0)                              /*!< enable SRAMC multi-bit non-correction interrupt */
#define SYSCFG_CFG3_SRAMECCSEIE             BIT(1)                              /*!< enable SRAMC single-bit non-correction interrupt */
#define SYSCFG_CFG3_FLASHECCMEIE            BIT(6)                              /*!< enable FLASH multi-bit SRAM non-correction interrupt */
#define SYSCFG_CFG3_FLASHECCSEIE            BIT(7)                              /*!< enable FLASH single-bit SRAM non-correction interrupt */
#define SYSCFG_CFG3_FLASHECCDEIE            BIT(8)                              /*!< enable FLASH double bit non-correction interrupt */
#define SYSCFG_CFG3_SRAMECCSERRBIT          BITS(9,14)                          /*!< indicates which one bit has an ECC single-bit correctable error */
#define SYSCFG_CFG3_SRAMECCEADDR            BITS(17,31)                         /*!< indicates the last address of ECC event on SRAMC occurred */

/* SYSCFG_MEMRMP bits definitions */
#define SYSCFG_MEMRMP_FMC_SWP               BIT(8)                              /*!< Flash Bank mode selection */
#define SYSCFG_MEMRMP_UNLOCK_KEY            BITS(16,31)                         /*!< Flash Bank mode selection unlock key bits, write only */

/* constants definitions */
/* boot mode definitions */
#define SYSCFG_BOOTMODE_FLASH               ((uint8_t)0x00U)                    /*!< boot from the main flash */
#define SYSCFG_BOOTMODE_SYSTEM              ((uint8_t)0x01U)                    /*!< boot from the system flash memory */
#define SYSCFG_BOOTMODE_SRAM                ((uint8_t)0x03U)                    /*!< boot from the embedded SRAM */

/* DMA remap definitions */
#define SYSCFG_PA11_REMAP_PA12              SYSCFG_CFG0_PA11_PA12_RMP           /*!< PA11 PA12 remap */
#define SYSCFG_BOOT0_REMAP_PD2              SYSCFG_CFG0_BOOT0_PD2_RMP           /*!< BOOT0 PD2 remap */
#define SYSCFG_DMA_REMAP_ADC0               SYSCFG_CFG0_ADC0_DMA_RMP            /*!< ADC0 DMA remap */
#define SYSCFG_DMA_REMAP_ADC1               SYSCFG_CFG0_ADC1_DMA_RMP            /*!< ADC1 DMA remap */
#define SYSCFG_DMA_REMAP_USART0TX           SYSCFG_CFG0_USART0_TX_DMA_RMP       /*!< USART0_TX DMA remap */
#define SYSCFG_DMA_REMAP_USART0RX           SYSCFG_CFG0_USART0_RX_DMA_RMP       /*!< USART0_RX DMA remap */
#define SYSCFG_DMA_REMAP_TIMER15            SYSCFG_CFG0_TIMER15_DMA_RMP         /*!< TIMER15 DMA remap */
#define SYSCFG_DMA_REMAP_TIMER16            SYSCFG_CFG0_TIMER16_DMA_RMP         /*!< TIMER16 DMA remap */

/* pin high current capability definitions */
#define SYSCFG_PB6_HIGH_CURRENT             SYSCFG_CFG0_PB6_HCCE                /*!< PB6 pin high current capability enable */
#define SYSCFG_PB7_HIGH_CURRENT             SYSCFG_CFG0_PB7_HCCE                /*!< PB7 pin high current capability enable */
#define SYSCFG_PB9_HIGH_CURRENT             SYSCFG_CFG0_PB9_HCCE                /*!< PB9 pin high current capability enable */
#define SYSCFG_PG14_HIGH_CURRENT            SYSCFG_CFG0_PG14_HCCE               /*!< PG14 pin high current capability enable */
#define SYSCFG_PG13_HIGH_CURRENT            SYSCFG_CFG0_PG13_HCCE               /*!< PG13 pin high current capability enable */
#define SYSCFG_PC1_HIGH_CURRENT             SYSCFG_CFG0_PC1_HCCE                /*!< PC1 pin high current capability enable */
#define SYSCFG_PC0_HIGH_CURRENT             SYSCFG_CFG0_PC0_HCCE                /*!< PC0 pin high current capability enable */
#define SYSCFG_PB14_HIGH_CURRENT            SYSCFG_CFG0_PB14_HCCE               /*!< PB14 pin high current capability enable */
#define SYSCFG_PB13_HIGH_CURRENT            SYSCFG_CFG0_PB13_HCCE               /*!< PB13 pin high current capability enable */
#define SYSCFG_PB8_HIGH_CURRENT             SYSCFG_CFG0_PB8_HCCE                /*!< PB8 pin high current capability enable */

/* EXTI source select definition */
#define EXTISS0                             ((uint8_t)0x00U)                    /*!< EXTI source select register 0 */
#define EXTISS1                             ((uint8_t)0x01U)                    /*!< EXTI source select register 1 */
#define EXTISS2                             ((uint8_t)0x02U)                    /*!< EXTI source select register 2 */
#define EXTISS3                             ((uint8_t)0x03U)                    /*!< EXTI source select register 3 */

/* EXTI source select mask bits definition */
#define EXTI_SS_MASK                        BITS(0,3)                           /*!< EXTI source select mask */

/* EXTI source select jumping step definition */
#define EXTI_SS_JSTEP                       ((uint8_t)(0x04U))                  /*!< EXTI source select jumping step */

/* EXTI source select moving step definition */
#define EXTI_SS_MSTEP(pin)                  (EXTI_SS_JSTEP * ((pin) % EXTI_SS_JSTEP))   /*!< EXTI source select moving step */

/* EXTI source port definitions */
#define EXTI_SOURCE_GPIOA                   ((uint8_t)0x00U)                    /*!< EXTI GPIOA configuration */
#define EXTI_SOURCE_GPIOB                   ((uint8_t)0x01U)                    /*!< EXTI GPIOB configuration */
#define EXTI_SOURCE_GPIOC                   ((uint8_t)0x02U)                    /*!< EXTI GPIOC configuration */
#define EXTI_SOURCE_GPIOD                   ((uint8_t)0x03U)                    /*!< EXTI GPIOD configuration */
#define EXTI_SOURCE_GPIOE                   ((uint8_t)0x04U)                    /*!< EXTI GPIOE configuration */
#define EXTI_SOURCE_GPIOF                   ((uint8_t)0x05U)                    /*!< EXTI GPIOF configuration */
#define EXTI_SOURCE_GPIOG                   ((uint8_t)0x06U)                    /*!< EXTI GPIOG configuration */

/* EXTI source pin definitions */
#define EXTI_SOURCE_PIN0                    ((uint8_t)0x00U)                    /*!< EXTI GPIO pin0 configuration */
#define EXTI_SOURCE_PIN1                    ((uint8_t)0x01U)                    /*!< EXTI GPIO pin1 configuration */
#define EXTI_SOURCE_PIN2                    ((uint8_t)0x02U)                    /*!< EXTI GPIO pin2 configuration */
#define EXTI_SOURCE_PIN3                    ((uint8_t)0x03U)                    /*!< EXTI GPIO pin3 configuration */
#define EXTI_SOURCE_PIN4                    ((uint8_t)0x04U)                    /*!< EXTI GPIO pin4 configuration */
#define EXTI_SOURCE_PIN5                    ((uint8_t)0x05U)                    /*!< EXTI GPIO pin5 configuration */
#define EXTI_SOURCE_PIN6                    ((uint8_t)0x06U)                    /*!< EXTI GPIO pin6 configuration */
#define EXTI_SOURCE_PIN7                    ((uint8_t)0x07U)                    /*!< EXTI GPIO pin7 configuration */
#define EXTI_SOURCE_PIN8                    ((uint8_t)0x08U)                    /*!< EXTI GPIO pin8 configuration */
#define EXTI_SOURCE_PIN9                    ((uint8_t)0x09U)                    /*!< EXTI GPIO pin9 configuration */
#define EXTI_SOURCE_PIN10                   ((uint8_t)0x0AU)                    /*!< EXTI GPIO pin10 configuration */
#define EXTI_SOURCE_PIN11                   ((uint8_t)0x0BU)                    /*!< EXTI GPIO pin11 configuration */
#define EXTI_SOURCE_PIN12                   ((uint8_t)0x0CU)                    /*!< EXTI GPIO pin12 configuration */
#define EXTI_SOURCE_PIN13                   ((uint8_t)0x0DU)                    /*!< EXTI GPIO pin13 configuration */
#define EXTI_SOURCE_PIN14                   ((uint8_t)0x0EU)                    /*!< EXTI GPIO pin14 configuration */
#define EXTI_SOURCE_PIN15                   ((uint8_t)0x0FU)                    /*!< EXTI GPIO pin15 configuration */

/* lock definitions */
#define SYSCFG_LOCK_LOCKUP                  SYSCFG_CFG2_LOCKUP_LOCK             /*!< LOCKUP output lock */
#define SYSCFG_LOCK_SRAM_PARITY_ERROR       SYSCFG_CFG2_SRAM_PARITY_ERROR_LOCK  /*!< SRAM parity error lock */
#define SYSCFG_LOCK_LVD                     SYSCFG_CFG2_LVD_LOCK                /*!< LVD lock */

/* SRAM parity check error flag definitions */
#define SYSCFG_SRAM_PCEF                    SYSCFG_CFG2_SRAM_PCEF               /*!< SRAM parity check error flag */

/* pin reset mode definitions */
#define SYSCFG_PIN_NRST                     ((uint32_t)0x01000000U)             /*!< GPIO pin configuration will retain state across any reset event except for the POR event */
#define SYSCFG_PIN_RST                      ((uint32_t)0x00000000U)             /*!< GPIO pin configuration is reset when any reset event occurs */

/* Flash bank mode definitions */
#define SYSCFG_FLASH_BANK0_MAPPED           ((uint32_t)0x00000000U)             /*!< Flash Bank1 mapped at 0x08000000 (and aliased @0x00000000, when BFB2 bit is set) and Flash Bank0 mapped at 0x08040000 (and aliased at 0x00080000) */
#define SYSCFG_FLASH_BANK1_MAPPED           SYSCFG_MEMRMP_FMC_SWP               /*!< Flash Bank0 mapped at 0x08000000 (and aliased @0x00000000) and Flash Bank1 mapped at 0x08040000 (and aliased at 0x00080000) */

/* unlock key */
#define SYSCFG_FMC_SWP_UNLOCK_KEY           ((uint32_t)0x0000CDEFU)             /*!< Flash Bank mode selection unlock key */

/* SYSCFG ECC interrupt flags */
typedef enum
{
    /* interrupt flags in SYSCFG_STAT register */
    SYSCFG_INT_FLAG_ECCME = SYSCFG_STAT_ECCMEIF,                                /*!< SRAM multi bits non-correction event interrupt flag */
    SYSCFG_INT_FLAG_ECCSE = SYSCFG_STAT_ECCSEIF,                                /*!< SRAM single bits non-correction event interrupt flag */
    SYSCFG_INT_FLAG_ECCME_FLASH = SYSCFG_STAT_FLASHECCMEIF,                     /*!< FLASH multi-bit correction event detected interrupt flag */
    SYSCFG_INT_FLAG_ECCSE_FLASH = SYSCFG_STAT_FLASHECCSEIF,                     /*!< FLASH single-bit correction event detected interrupt flag */
    SYSCFG_INT_FLAG_DECCE_FLASH = SYSCFG_STAT_FLASHECCDEIF                      /*!< FLASH double bit non-correction event detected interrupt flag */
}syscfg_interrupt_flag_enum;

/* enable or disable SYSCFG ECC interrupt */
typedef enum {
    /* interrupt in ECC_CR register */
    SYSCFG_INT_ECCME = SYSCFG_CFG3_SRAMECCMEIE,                                 /*!< SRAM multi bits non-correction event interrupt */
    SYSCFG_INT_ECCSE = SYSCFG_CFG3_SRAMECCSEIE,                                 /*!< SRAM single bits non-correction event interrupt */
    SYSCFG_INT_ECCME_FLASH = SYSCFG_CFG3_FLASHECCMEIE,                          /*!< FLASH multi-bit correction event detected interrupt */
    SYSCFG_INT_ECCSE_FLASH = SYSCFG_CFG3_FLASHECCSEIE,                          /*!< FLASH single-bit correction event detected interrupt */
    SYSCFG_INT_DECCE_FLASH = SYSCFG_CFG3_FLASHECCDEIE                           /*!< FLASH double bit non-correction event detected interrupt */
} syscfg_interrupt_enum;

/* function declarations */
/* initialization functions */
/* reset the SYSCFG registers */
void syscfg_deinit(void);

/* function configuration */
/* configure the boot mode */
uint8_t syscfg_bootmode_get(void);

/* enable the DMA channels remapping */
void syscfg_dma_remap_enable(uint32_t syscfg_dma_remap);
/* disable the DMA channels remapping */
void syscfg_dma_remap_disable(uint32_t syscfg_dma_remap);

/* enable PBx(x=6,7,8,9,13,14), PCx(x=0,1), PGx(x=13,14) high current capability */
void syscfg_high_current_enable(uint32_t syscfg_high_current);
/* disable PBx(x=6,7,8,9,13,14), PCx(x=0,1), PGx(x=13,14) high current capability */
void syscfg_high_current_disable(uint32_t syscfg_high_current);

/* enable VREF function */
void syscfg_vref_enable(void);
/* disable VREF function */
void syscfg_vref_disable(void);

/* configure the GPIO pin reset mode */
void syscfg_pin_reset_mode_config(uint32_t syscfg_pin_reset_mode);

/* configure the GPIO pin as EXTI Line */
void syscfg_exti_line_config(uint8_t exti_port, uint8_t exti_pin);
/* connect TIMER0/14/15/16 break input to the selected parameter */
void syscfg_lock_config(uint32_t syscfg_lock);

/* get ECC error address in SYSCFG_CFG3 */
uint32_t syscfg_ecc_error_address_get(void);
/* get ECC single-bit correctable error bit in SYSCFG_CFG3 */
uint32_t syscfg_ecc_error_bit_get(void);

/* Memory functions */
/* unlock the bit8 of SYSCFG_MEMRMP register operation */
void syscfg_flash_bank_remap_unlock(void);
/* switch flash Bank0 and Bank1 address */
void syscfg_flash_bank_remap_set(uint32_t value);

/* check if the specified flag in SYSCFG_CFG2 is set or not */
FlagStatus syscfg_flag_get(uint32_t syscfg_flag);
/* clear the flag in SYSCFG_CFG2 by writing 1 */
void syscfg_flag_clear(uint32_t syscfg_flag);

/* enable ECC interrupt */
void syscfg_interrupt_enable(syscfg_interrupt_enum inttype);
/* disable ECC interrupt */
void syscfg_interrupt_disable(syscfg_interrupt_enum inttype);
/* check if the specified interrupt flag in SYSCFG_STAT is set or not */
FlagStatus syscfg_interrupt_flag_get(syscfg_interrupt_flag_enum int_flag);
/* clear the interrupt flag in SYSCFG_STAT by writing 1 */
void syscfg_interrupt_flag_clear(syscfg_interrupt_flag_enum int_flag);

#endif /* GD32E511_512_SYSCFG_H */

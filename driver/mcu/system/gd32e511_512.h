/*!
    \file    gd32e511_512.h
    \brief   general definitions for GD32E511_512

    \version 2025-12-25, V0.2.0, firmware for GD32E511_512
*/

/*
 * Copyright (c) 2009-2018 Arm Limited. All rights reserved.
 * Copyright (c) 2025, GigaDevice Semiconductor Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef GD32E511_512_H
#define GD32E511_512_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* define GD32E511_512 */
#if !defined (GD32E511_512)
 #define GD32E511_512
#endif /* define GD32E511_512 */
#if !defined (GD32E511_512)
 #error "Please select the target GD32E511_512 device used in your application (in gd32e511_512.h file)"
#endif /* undefine GD32E511_512 tip */

/* define value of high speed crystal oscillator (HXTAL) in Hz */
#if !defined  (HXTAL_VALUE)
#define HXTAL_VALUE    ((uint32_t)8000000)
#endif /* high speed crystal oscillator value */

/* define startup timeout value of high speed crystal oscillator (HXTAL) */
#if !defined  (HXTAL_STARTUP_TIMEOUT)
#define HXTAL_STARTUP_TIMEOUT   ((uint16_t)0x0FFFF)
#endif /* high speed crystal oscillator startup timeout */

/* define value of internal 8MHz RC oscillator (IRC8M) in Hz */
#if !defined  (IRC8M_VALUE) 
#define IRC8M_VALUE  ((uint32_t)8000000)
#endif /* internal 8MHz RC oscillator value */

/* define startup timeout value of internal 8MHz RC oscillator (IRC8M) */
#if !defined  (IRC8M_STARTUP_TIMEOUT)
#define IRC8M_STARTUP_TIMEOUT   ((uint16_t)0x0500)
#endif /* internal 8MHz RC oscillator startup timeout */

/* define value of internal RC oscillator for ADC in Hz */
#if !defined  (IRC80M_VALUE) 
#define IRC80M_VALUE ((uint32_t)80000000)
#endif /* IRC42M_VALUE */

/* define value of internal 40KHz RC oscillator(IRC40K) in Hz */
#if !defined  (IRC40K_VALUE) 
#define IRC40K_VALUE  ((uint32_t)40000)
#endif /* internal 40KHz RC oscillator value */

/* define value of low speed crystal oscillator (LXTAL)in Hz */
#if !defined  (LXTAL_VALUE) 
#define LXTAL_VALUE  ((uint32_t)32768)
#endif /* low speed crystal oscillator value */

/* GD32E511_512 firmware library version number V0.2.0 */
#define __GD32E511_512_STDPERIPH_VERSION_MAIN   (0x00) /*!< [31:24] main version     */
#define __GD32E511_512_STDPERIPH_VERSION_SUB1   (0x02) /*!< [23:16] sub1 version     */
#define __GD32E511_512_STDPERIPH_VERSION_SUB2   (0x00) /*!< [15:8]  sub2 version     */
#define __GD32E511_512_STDPERIPH_VERSION_RC     (0x00) /*!< [7:0]  release candidate */ 
#define __GD32E511_512_STDPERIPH_VERSION        ((__GD32E511_512_STDPERIPH_VERSION_MAIN << 24)\
                                                |(__GD32E511_512_STDPERIPH_VERSION_SUB1 << 16)\
                                                |(__GD32E511_512_STDPERIPH_VERSION_SUB2 << 8)\
                                                |(__GD32E511_512_STDPERIPH_VERSION_RC))

/* configuration of the Cortex-M33 processor and core peripherals                                            */
#define __CM33_REV                0x0003U       /*!< Core revision r0p3                                      */
#define __SAUREGION_PRESENT       0U            /*!< SAU regions are not present                             */
#define __MPU_PRESENT             0U            /*!< MPU is present                                          */
#define __VTOR_PRESENT            1U            /*!< VTOR is present                                         */
#define __NVIC_PRIO_BITS          4U            /*!< Number of Bits used for Priority Levels                 */
#define __Vendor_SysTickConfig    0U            /*!< Set to 1 if different SysTick Config is used            */
#define __FPU_PRESENT             1             /*!< FPU present                                             */
#define __DSP_PRESENT             1             /*!< DSP present                                             */

/* define interrupt number */
typedef enum IRQn
{
    /* Cortex-M33 processor exceptions numbers */
    NonMaskableInt_IRQn          = -14,         /*!< non mask-able interrupt                                  */
    HardFault_IRQn               = -13,         /*!< hard-fault interrupt                                     */
    MemoryManagement_IRQn        = -12,         /*!< 4 Cortex-M33 memory management interrupt                 */
    BusFault_IRQn                = -11,         /*!< 5 Cortex-M33 bus fault interrupt                         */
    UsageFault_IRQn              = -10,         /*!< 6 Cortex-M33 usage fault interrupt                       */
    SVCall_IRQn                  = -5,          /*!< 11 Cortex-M33 sv call interrupt                          */
    DebugMonitor_IRQn            = -4,          /*!< 12 Cortex-M33 debug monitor interrupt                    */
    PendSV_IRQn                  = -2,          /*!< 14 Cortex-M33 pend sv interrupt                          */
    SysTick_IRQn                 = -1,          /*!< 15 Cortex-M33 system tick interrupt                      */
    /* interrupt numbers */
    WWDGT_IRQn                   = 0,           /*!< window watchdog timer interrupt                          */
    LVD_IRQn                     = 1,           /*!< LVD through EXTI line detection interrupt                */
    RTC_IRQn                     = 2,           /*!< RTC global interrupt                                     */
    FMC_IRQn                     = 3,           /*!< FMC interrupt                                            */
    RCU_IRQn                     = 4,           /*!< RCU interrupt                                            */
    EXTI0_1_IRQn                 = 5,           /*!< EXTI line0-1 interrupt                                   */
    EXTI2_3_IRQn                 = 6,           /*!< EXTI line2-3 interrupt                                   */
    EXTI4_15_IRQn                = 7,           /*!< EXTI line4-15 interrupt                                  */
    DMA_Channel0_IRQn            = 9,           /*!< DMA channel 0 global interrupt                           */
    DMA_Channel1_2_IRQn          = 10,          /*!< DMA channel 1-2 global interrupt                         */
    DMA_Channel3_4_IRQn          = 11,          /*!< DMA channel 3-4 global interrupt                         */
    ADC_CMP_IRQn                 = 12,          /*!< ADC0-1 and CMP0-1 interrupt                              */
    TIMER0_BRK_UP_TRG_COM_IRQn   = 13,          /*!< TIMER0 break, update, trigger and commutation interrupt  */
    TIMER0_Channel_IRQn          = 14,          /*!< TIMER0 capture compare interrupt                         */
    TIMER1_IRQn                  = 15,          /*!< TIMER1 global interrupt                                  */
    TIMER2_IRQn                  = 16,          /*!< TIMER2 global interrupt                                  */
    TIMER5_DAC0_1_IRQn           = 17,          /*!< TIMER5 interrupt, DAC0, DAC1 global interrupt            */
    TIMER6_DAC2_3_IRQn           = 18,          /*!< TIMER6, DAC2, DAC3 interrupt                             */
    TIMER13_IRQn                 = 19,          /*!< TIMER13 global interrupt                                 */
    TIMER14_IRQn                 = 20,          /*!< TIMER14 global interrupt                                 */
    TIMER15_IRQn                 = 21,          /*!< TIMER15 global interrupt                                 */
    TIMER16_IRQn                 = 22,          /*!< TIMER16 global interrupt                                 */
    I2C0_EV_IRQn                 = 23,          /*!< I2C0 event interrupt                                     */
    I2C1_EV_IRQn                 = 24,          /*!< I2C1 event interrupt                                     */
    SPI0_IRQn                    = 25,          /*!< SPI0 global interrupt                                    */
    SPI1_IRQn                    = 26,          /*!< SPI1 global interrupt                                    */
    USART0_IRQn                  = 27,          /*!< USART0 global interrupt                                  */
    UART3_IRQn                   = 28,          /*!< UART3 global interrupt                                   */
    I2C0_ER_IRQn                 = 32,          /*!< I2C0 error interrupt                                     */
    I2C1_ER_IRQn                 = 34,          /*!< I2C1 error interrupt                                     */
    I2C2_EV_IRQn                 = 35,          /*!< I2C2 event interrupt                                     */
    I2C2_ER_IRQn                 = 36,          /*!< I2C2 error interrupt                                     */
    CLA_IRQn                     = 39,          /*!< CLA global interrupt                                     */
    MDIO_IRQn                    = 40,          /*!< MDIO global interrupt                                    */
    DMA_Channel5_6_IRQn          = 48,          /*!< DMA channel5-6 global interrupt                          */
    I3C_EV_IRQn                  = 49,          /*!< I3C event interrupt                                      */
    I3C_ER_IRQn                  = 50,          /*!< I3C error interrupt                                      */
    SPI2_IRQn                    = 51,          /*!< SPI2 global interrupt                                    */
    SPI3_IRQn                    = 52,          /*!< SPI3 global interrupt                                    */
    DMA_Channel7_IRQn            = 53,          /*!< DMA channel 7 global interrupt                           */
    SRAM_ECC_IRQn                = 54,          /*!< SRAM ECC interrupt                                       */
    FPU_IRQn                     = 68           /*!< FPU interrupt                                            */
} IRQn_Type;

/* includes */
#include "core_cm33.h"
#include "system_gd32e511_512.h"
#include <stdint.h>
#ifdef FW_DEBUG_ERR_REPORT
#include "gd32e511_512_err_report.h"
#endif /* FW_DEBUG_ERR_REPORT */

/* enum definitions */
typedef enum {DISABLE = 0, ENABLE = !DISABLE} EventStatus, ControlStatus;
typedef enum {RESET = 0, SET = !RESET} FlagStatus;
typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrStatus;

/* bit operations */
#define REG64(addr)                  (*(volatile uint64_t *)(uint32_t)(addr))
#define REG32(addr)                  (*(volatile uint32_t *)(uint32_t)(addr))
#define REG16(addr)                  (*(volatile uint16_t *)(uint32_t)(addr))
#define REG8(addr)                   (*(volatile uint8_t *)(uint32_t)(addr))
#define BIT(x)                       ((uint32_t)((uint32_t)0x01U<<(x)))
#define BITS(start, end)             ((0xFFFFFFFFUL << (start)) & (0xFFFFFFFFUL >> (31U - (uint32_t)(end)))) 
#define GET_BITS(regval, start, end) (((regval) & BITS((start),(end))) >> (start))

/* main flash and SRAM memory map */
#define FLASH_BASE            ((uint32_t)0x08000000U)           /*!< main FLASH base address          */
#define SRAM_BASE             ((uint32_t)0x20000000U)           /*!< SRAM base address                */
/* peripheral memory map */
#define APB1_BUS_BASE         ((uint32_t)0x40000000U)           /*!< apb1 base address                */
#define APB2_BUS_BASE         ((uint32_t)0x40010000U)           /*!< apb2 base address                */
#define AHB1_BUS_BASE         ((uint32_t)0x40020000U)           /*!< ahb1 base address                */
#define AHB2_BUS_BASE         ((uint32_t)0x48000000U)           /*!< ahb2 base address                */
/* advanced peripheral bus 1 memory map */
#define TIMER_BASE            (APB1_BUS_BASE + 0x00000000U)     /*!< TIMER base address               */
#define RTC_BASE              (APB1_BUS_BASE + 0x00002800U)     /*!< RTC base address                 */
#define WWDGT_BASE            (APB1_BUS_BASE + 0x00002C00U)     /*!< WWDGT base address               */
#define FWDGT_BASE            (APB1_BUS_BASE + 0x00003000U)     /*!< FWDGT base address               */
#define SPI_BASE              (APB1_BUS_BASE + 0x00003800U)     /*!< SPI base address                 */
#define UART_BASE             (APB1_BUS_BASE + 0x00004400U)     /*!< UART base address               */
#define I2C_BASE              (APB1_BUS_BASE + 0x00005400U)     /*!< I2C base address                 */
#define PMU_BASE              (APB1_BUS_BASE + 0x00007000U)     /*!< PMU base address                 */
#define DAC0_BASE             (APB1_BUS_BASE + 0x00007400U)     /*!< DAC0 base address                */
#define OPA_BASE              (APB1_BUS_BASE + 0x00007C00U)     /*!< OPA base address                */
#define DAC1_BASE             (APB1_BUS_BASE + 0x0000CC00U)     /*!< DAC1 base address                */
#define MDIO_BASE             (APB1_BUS_BASE + 0x0000D800U)     /*!< MDIO base address                */
/* advanced peripheral bus 2 memory map */
#define SYSCFG_BASE           (APB2_BUS_BASE + 0x00000000U)     /*!< SYSCFG base address              */
#define CMP_BASE              (APB2_BUS_BASE + 0x00000000U)     /*!< CMP base address                 */
#define EXTI_BASE             (APB2_BUS_BASE + 0x00000400U)     /*!< EXTI base address                */
#define ADC_BASE              (APB2_BUS_BASE + 0x00002400U)     /*!< ADC base address                 */
#define I3C_BASE              (APB2_BUS_BASE + 0x00005C00U)     /*!< I3C base address                 */
/* advanced high performance bus 1 memory map */
#define DMA_BASE              (AHB1_BUS_BASE + 0x00000000U)     /*!< DMA base address                 */
#define DMA_CHANNEL_BASE      (DMA_BASE + 0x00000008U)          /*!< DMA channel base address         */
#define RCU_BASE              (AHB1_BUS_BASE + 0x00001000U)     /*!< RCU base address                 */
#define FMC_BASE              (AHB1_BUS_BASE + 0x00002000U)     /*!< FMC base address                 */
#define CRC_BASE              (AHB1_BUS_BASE + 0x00003000U)     /*!< CRC base address                 */
#define CLA_BASE              (AHB1_BUS_BASE + 0x00018000U)     /*!< CLA base address                 */
/* advanced high performance bus 2 memory map */    
#define GPIO_BASE             (AHB2_BUS_BASE + 0x00000000U)     /*!< GPIO base address                */
/* option byte and debug memory map */
#define OB_BASE               ((uint32_t)0x1FFFF800U)           /*!< OB base address                  */
#define DBG_BASE              ((uint32_t)0xE0044000U)           /*!< DBG base address                 */

/* define marco USE_STDPERIPH_DRIVER */
#if !defined USE_STDPERIPH_DRIVER
#define USE_STDPERIPH_DRIVER
#endif
#ifdef USE_STDPERIPH_DRIVER
#include "gd32e511_512_libopt.h"
#endif /* USE_STDPERIPH_DRIVER */

#ifdef __cplusplus
}
#endif

#endif /* GD32E511_512_H */

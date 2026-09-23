;/*!
;    \file    startup_gd32e511_512.s
;    \brief   start up file
;
;    \version 2025-12-25, V0.2.0, firmware for GD32E511_512
;*/
;
;/*
; * Copyright (c) 2009-2018 Arm Limited. All rights reserved.
; * Copyright (c) 2025, GigaDevice Semiconductor Inc.
; *
; * SPDX-License-Identifier: Apache-2.0
; *
; * Licensed under the Apache License, Version 2.0 (the License); you may
; * not use this file except in compliance with the License.
; * You may obtain a copy of the License at
; *
; * www.apache.org/licenses/LICENSE-2.0
; *
; * Unless required by applicable law or agreed to in writing, software
; * distributed under the License is distributed on an AS IS BASIS, WITHOUT
; * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; * See the License for the specific language governing permissions and
; * limitations under the License.
; */
;
;/* This file refers the CMSIS standard, some adjustments are made according to GigaDevice chips */

; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00002000

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000800

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                PRESERVE8
                THUMB

;               /* reset Vector Mapped to at Address 0 */
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp                      ; Top of Stack
                DCD     Reset_Handler                     ; Reset Handler
                DCD     NMI_Handler                       ; non mask-able interrupt
                DCD     HardFault_Handler                 ; hard-fault interrupt
                DCD     MemManage_Handler                 ; 4 Cortex-M33 memory management interrupt
                DCD     BusFault_Handler                  ; 5 Cortex-M33 bus fault interrupt
                DCD     UsageFault_Handler                ; 6 Cortex-M33 usage fault interrupt
                DCD     0                                 ; Reserved
                DCD     0                                 ; Reserved
                DCD     0                                 ; Reserved
                DCD     0                                 ; Reserved
                DCD     SVC_Handler                       ; 11 Cortex-M33 sv call interrupt
                DCD     DebugMon_Handler                  ; 12 Cortex-M33 debug monitor interrupt
                DCD     0                                 ; Reserved
                DCD     PendSV_Handler                    ; 14 Cortex-M33 pend sv interrupt
                DCD     SysTick_Handler                   ; 15 Cortex-M33 system tick interrupt

;               /* external interrupts handler */
                DCD     WWDGT_IRQHandler                  ; window watchdog timer interrupt
                DCD     LVD_IRQHandler                    ; LVD through EXTI line detection interrupt
                DCD     RTC_IRQHandler                    ; RTC global interrupt
                DCD     FMC_IRQHandler                    ; FMC interrupt
                DCD     RCU_IRQHandler                    ; RCU interrupt
                DCD     EXTI0_1_IRQHandler                ; EXTI line0-1 interrupt
                DCD     EXTI2_3_IRQHandler                ; EXTI line2-3 interrupt
                DCD     EXTI4_15_IRQHandler               ; EXTI line4-15 interrupt
                DCD     0                                 ; Reserved
                DCD     DMA_Channel0_IRQHandler           ; DMA channel 0 global interrupt
                DCD     DMA_Channel1_2_IRQHandler         ; DMA channel 1-2 global interrupt
                DCD     DMA_Channel3_4_IRQHandler         ; DMA channel 3-4 global interrupt
                DCD     ADC_CMP_IRQHandler                ; ADC0-1 and CMP0-1 interrupt
                DCD     TIMER0_BRK_UP_TRG_COM_IRQHandler  ; TIMER0 break, update, trigger and commutation interrupt
                DCD     TIMER0_Channel_IRQHandler         ; TIMER0 capture compare interrupt
                DCD     TIMER1_IRQHandler                 ; TIMER1 global interrupt
                DCD     TIMER2_IRQHandler                 ; TIMER2 global interrupt
                DCD     TIMER5_DAC0_1_IRQHandler          ; TIMER5 interrupt, DAC0, DAC1 global interrupt
                DCD     TIMER6_DAC2_3_IRQHandler          ; TIMER6, DAC2, DAC3 interrupt
                DCD     TIMER13_IRQHandler                ; TIMER13 global interrupt
                DCD     TIMER14_IRQHandler                ; TIMER14 global interrupt
                DCD     TIMER15_IRQHandler                ; TIMER15 global interrupt
                DCD     TIMER16_IRQHandler                ; TIMER16 global interrupt
                DCD     I2C0_EV_IRQHandler                ; I2C0 event interrupt
                DCD     I2C1_EV_IRQHandler                ; I2C1 event interrupt
                DCD     SPI0_IRQHandler                   ; SPI0 global interrupt
                DCD     SPI1_IRQHandler                   ; SPI1 global interrupt
                DCD     USART0_IRQHandler                 ; USART0 global interrupt
                DCD     UART3_IRQHandler                  ; UART3 global interrupt
                DCD     0                                 ; Reserved
                DCD     0                                 ; Reserved
                DCD     0                                 ; Reserved
                DCD     I2C0_ER_IRQHandler                ; I2C0 error interrupt
                DCD     0                                 ; Reserved
                DCD     I2C1_ER_IRQHandler                ; I2C1 error interrupt
                DCD     I2C2_EV_IRQHandler                ; I2C2 event interrupt
                DCD     I2C2_ER_IRQHandler                ; I2C2 error interrupt
                DCD     0                                 ; Reserved
                DCD     0                                 ; Reserved
                DCD     CLA_IRQHandler                    ; CLA global interrupt
                DCD     MDIO_IRQHandler                   ; MDIO global interrupt
                DCD     0                                 ; Reserved
                DCD     0                                 ; Reserved
                DCD     0                                 ; Reserved
                DCD     0                                 ; Reserved
                DCD     0                                 ; Reserved
                DCD     0                                 ; Reserved
                DCD     0                                 ; Reserved
                DCD     DMA_Channel5_6_IRQHandler         ; DMA channel5-6 global interrupt
                DCD     I3C_EV_IRQHandler                 ; I3C event interrupt
                DCD     I3C_ER_IRQHandler                 ; I3C error interrupt
                DCD     SPI2_IRQHandler                   ; SPI2 global interrupt
                DCD     SPI3_IRQHandler                   ; SPI3 global interrupt
                DCD     DMA_Channel7_IRQHandler           ; DMA channel 7 global interrupt
                DCD     SRAM_ECC_IRQHandler               ; SRAM ECC interrupt
                DCD     0                                 ; Reserved
                DCD     0                                 ; Reserved
                DCD     0                                 ; Reserved
                DCD     0                                 ; Reserved
                DCD     0                                 ; Reserved
                DCD     0                                 ; Reserved
                DCD     0                                 ; Reserved
                DCD     0                                 ; Reserved
                DCD     0                                 ; Reserved
                DCD     0                                 ; Reserved
                DCD     0                                 ; Reserved
                DCD     0                                 ; Reserved
                DCD     0                                 ; Reserved
                DCD     FPU_IRQHandler                    ; FPU interrupt
__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY

;/* reset Handler */
Reset_Handler   PROC
                EXPORT  Reset_Handler                     [WEAK]
                IMPORT  SystemInit
                IMPORT  __main
                LDR     SP, =__initial_sp

                IMPORT |Image$$RW_IRAM1$$RW$$Base|
                LDR     R0, =|Image$$RW_IRAM1$$RW$$Base|
                ADD     R1, R0, #0x8000
                LDR     R2, =0x0
MEM_INIT        STRD    R2, R2, [ R0 ] , #8
                CMP     R0, R1
                BNE     MEM_INIT

                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP

;/* dummy Exception Handlers */
NMI_Handler\
                PROC
                EXPORT  NMI_Handler                       [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler                 [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler                 [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler                  [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler                [WEAK]
                B       .
                ENDP
SVC_Handler\
                PROC
                EXPORT  SVC_Handler                       [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler                  [WEAK]
                B       .
                ENDP
PendSV_Handler\
                PROC
                EXPORT  PendSV_Handler                    [WEAK]
                B       .
                ENDP
SysTick_Handler\
                PROC
                EXPORT  SysTick_Handler                   [WEAK]
                B       .
                ENDP

Default_Handler PROC
;               /* external interrupts handler */
                EXPORT  WWDGT_IRQHandler                  [WEAK]
                EXPORT  LVD_IRQHandler                    [WEAK]
                EXPORT  RTC_IRQHandler                    [WEAK]
                EXPORT  FMC_IRQHandler                    [WEAK]
                EXPORT  RCU_IRQHandler                    [WEAK]
                EXPORT  EXTI0_1_IRQHandler                [WEAK]
                EXPORT  EXTI2_3_IRQHandler                [WEAK]
                EXPORT  EXTI4_15_IRQHandler               [WEAK]
                EXPORT  DMA_Channel0_IRQHandler           [WEAK]
                EXPORT  DMA_Channel1_2_IRQHandler         [WEAK]
                EXPORT  DMA_Channel3_4_IRQHandler         [WEAK]
                EXPORT  ADC_CMP_IRQHandler                [WEAK]
                EXPORT  TIMER0_BRK_UP_TRG_COM_IRQHandler  [WEAK]
                EXPORT  TIMER0_Channel_IRQHandler         [WEAK]
                EXPORT  TIMER1_IRQHandler                 [WEAK]
                EXPORT  TIMER2_IRQHandler                 [WEAK]
                EXPORT  TIMER5_DAC0_1_IRQHandler          [WEAK]
                EXPORT  TIMER6_DAC2_3_IRQHandler          [WEAK]
                EXPORT  TIMER13_IRQHandler                [WEAK]
                EXPORT  TIMER14_IRQHandler                [WEAK]
                EXPORT  TIMER15_IRQHandler                [WEAK]
                EXPORT  TIMER16_IRQHandler                [WEAK]
                EXPORT  I2C0_EV_IRQHandler                [WEAK]
                EXPORT  I2C1_EV_IRQHandler                [WEAK]
                EXPORT  SPI0_IRQHandler                   [WEAK]
                EXPORT  SPI1_IRQHandler                   [WEAK]
                EXPORT  USART0_IRQHandler                 [WEAK]
                EXPORT  UART3_IRQHandler                  [WEAK]
                EXPORT  I2C0_ER_IRQHandler                [WEAK]
                EXPORT  I2C1_ER_IRQHandler                [WEAK]
                EXPORT  I2C2_EV_IRQHandler                [WEAK]
                EXPORT  I2C2_ER_IRQHandler                [WEAK]
                EXPORT  CLA_IRQHandler                    [WEAK]
                EXPORT  MDIO_IRQHandler                   [WEAK]
                EXPORT  DMA_Channel5_6_IRQHandler         [WEAK]
                EXPORT  I3C_EV_IRQHandler                 [WEAK]
                EXPORT  I3C_ER_IRQHandler                 [WEAK]
                EXPORT  SPI2_IRQHandler                   [WEAK]
                EXPORT  SPI3_IRQHandler                   [WEAK]
                EXPORT  DMA_Channel7_IRQHandler           [WEAK]
                EXPORT  SRAM_ECC_IRQHandler               [WEAK]
                EXPORT  FPU_IRQHandler                    [WEAK]

;/* external interrupts handler */
WWDGT_IRQHandler
LVD_IRQHandler
RTC_IRQHandler
FMC_IRQHandler
RCU_IRQHandler
EXTI0_1_IRQHandler
EXTI2_3_IRQHandler
EXTI4_15_IRQHandler
DMA_Channel0_IRQHandler
DMA_Channel1_2_IRQHandler
DMA_Channel3_4_IRQHandler
ADC_CMP_IRQHandler
TIMER0_BRK_UP_TRG_COM_IRQHandler
TIMER0_Channel_IRQHandler
TIMER1_IRQHandler
TIMER2_IRQHandler
TIMER5_DAC0_1_IRQHandler   
TIMER6_DAC2_3_IRQHandler
TIMER13_IRQHandler
TIMER14_IRQHandler
TIMER15_IRQHandler
TIMER16_IRQHandler
I2C0_EV_IRQHandler
I2C1_EV_IRQHandler
SPI0_IRQHandler
SPI1_IRQHandler
USART0_IRQHandler
UART3_IRQHandler
I2C0_ER_IRQHandler
I2C1_ER_IRQHandler
I2C2_EV_IRQHandler
I2C2_ER_IRQHandler
CLA_IRQHandler
MDIO_IRQHandler
DMA_Channel5_6_IRQHandler
I3C_EV_IRQHandler
I3C_ER_IRQHandler
SPI2_IRQHandler
SPI3_IRQHandler
DMA_Channel7_IRQHandler
SRAM_ECC_IRQHandler
FPU_IRQHandler

                B       .
                ENDP

                ALIGN

; user Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap

__user_initial_stackheap PROC
                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR
                ENDP

                ALIGN

                ENDIF

                END

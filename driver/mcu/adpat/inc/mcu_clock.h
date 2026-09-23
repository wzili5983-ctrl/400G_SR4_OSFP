/**
 * @file mcu_clock.h
 * @brief IRC8M based 80 MHz clock and SysTick setup.
 */
#ifndef MCU_CLOCK_H
#define MCU_CLOCK_H

#include "basedef.h"

VOID McuClockInit(VOID);
VOID SysTick_Handler(VOID);
VOID delay_us(DWORD microsecs);

#endif

/**
 * @file mcu_clock.h
 * @brief IRC8M based 80 MHz clock and SysTick polling based delays.
 */
#ifndef MCU_CLOCK_H
#define MCU_CLOCK_H

#include "basedef.h"

VOID McuClockInit(VOID);
VOID McuClockDelayUs(DWORD microsecs);
VOID McuClockDelayMs(DWORD millisecs);
VOID McuClockDelayS(DWORD secs);

#endif

/** @file hr_time_arm.c @brief Cortex-M33 cycle-counter delays, independent of DSP. */
#include "gd32e511_512.h"
#include "hr_time.h"
void delay_us(uint32_t microsecs)
{
    uint32_t cycles_per_us = (SystemCoreClock + 999999U) / 1000000U;
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    while (microsecs != 0) {
        uint32_t chunk = microsecs > 1000U ? 1000U : microsecs;
        uint32_t start = DWT->CYCCNT;
        uint32_t cycles = chunk * cycles_per_us;
        while ((uint32_t)(DWT->CYCCNT - start) < cycles) { __NOP(); }
        microsecs -= chunk;
    }
}
void delay_ms(unsigned int msecs) { while (msecs-- != 0) { delay_us(1000); } }
void delay_s(unsigned int secs) { while (secs-- != 0) { delay_ms(1000); } }
void phy_delay_us(uint32_t microsecs) { delay_us(microsecs); }
void HATS_DelayMs(unsigned int msecs) { delay_ms(msecs); }
void HATS_DelayUsec(unsigned int microsecs) { delay_us(microsecs); }

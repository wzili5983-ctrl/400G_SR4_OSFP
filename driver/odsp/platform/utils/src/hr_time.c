/* $Id: hr_time.c 389 2014-03-17 23:39:34Z binliu $
 *
 *  Broadcom Confidential/Proprietary.
 *  Copyright (c) 2013 Broadcom Corporation. All rights reserved.
 */

/** @file hr_time.c
 * Implementation of high-resolution timer in Microsoft Windows.
 */

#if defined __linux__
#include <unistd.h> 
#elif defined _WIN32
#include <Windows.h>
#endif
#include "common_def.h"
#include "access.h"
#include "hr_time.h"

void phy_delay_us(uint32_t microsecs);

/** add system delay for seconds.
 * @param secs seconds.
 * @return 
 */
void delay_s(unsigned int secs)
{
#if defined __linux__
    sleep(secs);
#else
    delay_ms(secs * 1000);
#endif
}

/** add system delay for mseconds.
 * @param secs mseconds.
 * @return 
 */
void delay_ms(unsigned int msecs)
{
#if defined _WIN32
    Sleep(msecs);
#else
    delay_us(msecs * 1000);
#endif
}

void delay_us(uint32_t microsecs)
{
#if defined __linux__
    usleep(microsecs);
#elif defined _WIN32
    LARGE_INTEGER freq;
    LARGE_INTEGER tm_end, tm_now;

    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&tm_end);
    tm_end.QuadPart += (freq.QuadPart * microsecs) / 1000000;

    do {
        QueryPerformanceCounter(&tm_now);
    } while (tm_now.QuadPart <= tm_end.QuadPart);
#else
    // delay using PHY on chip timer
    phy_delay_us(microsecs);
#endif
}

#if defined _WIN32
double get_elapsed_ms(void *liStart)
{
     LARGE_INTEGER Frequency;
     double dftDuration ;
     LONGLONG llTimeDiff;
      LARGE_INTEGER liStop;
      LARGE_INTEGER *liStart_ptr = (LARGE_INTEGER *)liStart;

    if(!QueryPerformanceFrequency(&Frequency))
        return 0;      // Installed hardware does not support high-resolution counter
    else
    {
      // Stop time measurement
      QueryPerformanceCounter(&liStop);

      llTimeDiff = liStop.QuadPart - liStart_ptr->QuadPart;

      // To get duration in milliseconds
      dftDuration = (double) llTimeDiff * 1000.0 / (double) Frequency.QuadPart;
      return dftDuration;
    }
}


double get_elapsed_us(void *liStart)
{
     LARGE_INTEGER Frequency;
     double dftDuration ;
     LONGLONG llTimeDiff;
      LARGE_INTEGER liStop;
      LARGE_INTEGER *liStart_ptr = (LARGE_INTEGER *)liStart;

    if(!QueryPerformanceFrequency(&Frequency))
        return 0;      // Installed hardware does not support high-resolution counter
    else
    {
      // Stop time measurement
      QueryPerformanceCounter(&liStop);

      llTimeDiff = liStop.QuadPart - liStart_ptr->QuadPart;

      // To get duration in us
      dftDuration = (double) llTimeDiff * 1000.0 / (double) Frequency.QuadPart*1000.0;
      return dftDuration;
    }
}
#endif

void HATS_DelayMs(unsigned int msecs)
{
    delay_ms(msecs);
}

#ifndef LW_SIMULATOR
void HATS_DelayUsec(unsigned int microsecs)
{
    delay_us(microsecs);
}
#endif

#if !defined(__linux__) && !defined(_WIN32)

/* PHY M0 timestamp register running at 156.25 Mhz clock rate */
#define GPTIMER0_CURR_CNT 0x40003004 
/* PHY M0 clock rate is 156.25 Mhz */
#define GPTIMER_1_US_INTERVAL (156)

/* Get current timestamp */
static uint32_t _get_timestamp(void)
{
    phy_info_t phy_info;

    phy_info.phy_id = 0;
    phy_info.base_addr = 0;
    return hsip_rd_reg_(&phy_info, GPTIMER0_CURR_CNT);
}

/* timestamp elapsed in CPU cycles */
static uint32_t _get_elapsed_timestamp(uint32_t lts)
{
    uint32_t elapsed_time;
    uint32_t cur_time = _get_timestamp();

    /* timestamp decrements over time */
    if (cur_time < lts)
        elapsed_time = lts - cur_time;
    else
        elapsed_time = 0xFFFFFFFF - cur_time + lts;

    return (uint32_t)(elapsed_time );
}

/* delay using PHY on chip timer */
void phy_delay_us(uint32_t microsecs)
{
    uint32_t passed_time_stamp =0;
    uint32_t start = _get_timestamp();

    do {
        passed_time_stamp = _get_elapsed_timestamp(start);
        if (passed_time_stamp > (microsecs * GPTIMER_1_US_INTERVAL)) {
            return;
        }
    } while(1);
}
#endif


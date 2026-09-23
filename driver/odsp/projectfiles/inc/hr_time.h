/* $Id: hr_time.h 389 2014-03-17 23:39:34Z binliu $
 *
 *  Broadcom Confidential/Proprietary.
 *  Copyright (c) 2013 Broadcom Corporation. All rights reserved.
 */

/** @file hr_time.h
 * High-resolution time related functions in Microsoft Windows environment.
 * A high-resolution time data type is provided along with time and difftime
 * clone from <time.h>. Additional utility functions are also included.
 */
#ifndef HR_TIME_H_
#define HR_TIME_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void delay_s(unsigned int secs);
void delay_ms(unsigned int msecs);
void delay_us(uint32_t microsecs);

#if defined _WIN32
double get_elapsed_ms(void *liStart);

double get_elapsed_us(void *liStart);
#endif

#ifdef __cplusplus
}
#endif

#endif /*HR_TIME_H_ */

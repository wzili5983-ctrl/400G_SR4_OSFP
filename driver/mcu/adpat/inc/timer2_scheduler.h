/***************************************************************************//**
 *   @file   timer2_scheduler.h
 *   @brief  Timer2 task scheduler interface.
 *   @author Oliver Wang (zili.wang@terabitcom.com)
*******************************************************************************/
#ifndef TIMER2_SCHEDULER_H
#define TIMER2_SCHEDULER_H
#include "basedef.h"
#define TIMER2_SCHEDULER_MAX_TASKS 20U
#define TIMER2_TASK_FOREVER        0U

typedef VOID (*Timer2TaskFunction_t)(VOID *arg);
typedef signed char Timer2TaskId_t;

VOID Timer2SchedulerInit(DWORD timer_clock_hz, DWORD tick_hz);
Timer2TaskId_t Timer2SchedulerAdd(Timer2TaskFunction_t function, VOID *arg, DWORD delay_ticks, DWORD run_count);
DWORD Timer2SchedulerRemove(Timer2TaskId_t id);
DWORD Timer2SchedulerStart(Timer2TaskId_t id);
DWORD Timer2SchedulerStop(Timer2TaskId_t id);
VOID Timer2SchedulerRun(VOID);
VOID Timer2SchedulerTickISR(VOID);
#endif

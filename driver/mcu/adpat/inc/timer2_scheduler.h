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

/* Task IDs are defined here centrally; modules use these TASK_ID_* values
 * and keep the handles returned by Timer2SchedulerAdd in g_TaskIds. */
typedef enum {
    TASK_ID_BASE_AD_SAMPLES = 0,
    TASK_ID_RSSI_AD_SAMPLES,
    TASK_ID_TX_AD_SAMPLES,
    TASK_ID_PERF_MON,
    TASK_ID_NUM
}Timer2TaskIdEnum_t;

/* 任务槽位表：g_TaskIds[TASK_ID_*] 保存对应任务注册得到的调度器槽位，
 * 集中存放、各模块共用——Init 时写入，Modify/Remove/Stop 时按 TASK_ID_*
 * 取用，不再各自维护私有句柄。-1 = 该 ID 尚未注册（Init 时统一置 -1）。
 * 生命周期注意：Timer2SchedulerInit() 会整体重置本表，任务注册须在其后。 */
extern Timer2TaskId_t g_TaskIds[TASK_ID_NUM];

VOID Timer2SchedulerInit(DWORD timer_clock_hz, DWORD tick_hz);
Timer2TaskId_t Timer2SchedulerAdd(Timer2TaskFunction_t function, VOID *arg, DWORD delay_ticks, DWORD interval_ticks, DWORD run_count);
DWORD Timer2SchedulerModify(Timer2TaskId_t id, Timer2TaskFunction_t function, VOID *arg, DWORD delay_ticks, DWORD interval_ticks, DWORD run_count);
DWORD Timer2SchedulerRemove(Timer2TaskId_t id);
DWORD Timer2SchedulerStart(Timer2TaskId_t id);
DWORD Timer2SchedulerStop(Timer2TaskId_t id);
VOID Timer2SchedulerRun(VOID);
VOID Timer2SchedulerTickISR(VOID);
#endif

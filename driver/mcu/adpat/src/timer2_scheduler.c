/***************************************************************************//**
 *   @file   timer2_scheduler.c
 *   @brief  Timer2 task scheduler implementation.
 *   @author Oliver Wang (zili.wang@terabitcom.com)
*******************************************************************************/
#include "timer2_scheduler.h"
#include "gd32e511_512.h"
#include "gd32e511_512_timer.h"
#include "gd32e511_512_rcu.h"

typedef struct {
    Timer2TaskFunction_t function;
    VOID *arg;
    volatile DWORD delay;
    DWORD interval;
    DWORD remaining;
    volatile BYTE used;
    volatile BYTE running;
    volatile BYTE pending;
} Timer2Task_t;

STATIC Timer2Task_t g_Tasks[TIMER2_SCHEDULER_MAX_TASKS];

STATIC BOOL Timer2TaskIdValid(Timer2TaskId_t id)
{
    return id >= 0 && id < (Timer2TaskId_t)TIMER2_SCHEDULER_MAX_TASKS && g_Tasks[id].used;
}

VOID Timer2SchedulerInit(DWORD timer_clock_hz, DWORD tick_hz)
{
    timer_parameter_struct parameter;

    /* Timer2 is driven by the 80 MHz system clock: 80 MHz / 8000 / 10 = 1 ms. */
    (void)timer_clock_hz;
    (void)tick_hz;

    for (DWORD index = 0U; index < TIMER2_SCHEDULER_MAX_TASKS; ++index) {
        g_Tasks[index] = (Timer2Task_t){0};
    }

    rcu_periph_clock_enable(RCU_TIMER2);
    timer_deinit(TIMER2);
    timer_struct_para_init(&parameter);
    parameter.prescaler = 7999U;
    parameter.alignedmode = TIMER_COUNTER_EDGE;
    parameter.counterdirection = TIMER_COUNTER_UP;
    parameter.period = 9U;
    parameter.clockdivision = TIMER_CKDIV_DIV1;
    parameter.repetitioncounter = 0U;
    timer_init(TIMER2, &parameter);
    timer_interrupt_enable(TIMER2, TIMER_INT_UP);
    /* SysTick uses priority 0; Timer2 runs immediately below it. */
    NVIC_SetPriority(TIMER2_IRQn, 1U);
    NVIC_EnableIRQ(TIMER2_IRQn);
    timer_enable(TIMER2);
}
/* delay_ticks: ticks before the first run; interval_ticks: ticks between runs;
 * run_count: TIMER2_TASK_FOREVER(0) runs forever, otherwise the run count. */
Timer2TaskId_t Timer2SchedulerAdd(Timer2TaskFunction_t function, VOID *arg, DWORD delay_ticks, DWORD interval_ticks, DWORD run_count)
{
    if (function == NULL) {
        return -1;
    }
    for (DWORD index = 0U; index < TIMER2_SCHEDULER_MAX_TASKS; ++index) {
        if (!g_Tasks[index].used) {
            g_Tasks[index] = (Timer2Task_t){function, arg, delay_ticks, interval_ticks, run_count, TRUE, TRUE, FALSE};
            return (Timer2TaskId_t)index;
        }
    }
    return -1;
}

/* Reconfigure a created task: new function/arg, restart with a new first delay,
 * interval and run count; pending state is cleared. */
DWORD Timer2SchedulerModify(Timer2TaskId_t id, Timer2TaskFunction_t function, VOID *arg, DWORD delay_ticks, DWORD interval_ticks, DWORD run_count)
{
    if (!Timer2TaskIdValid(id) || function == NULL) {
        return ERR;
    }
    g_Tasks[id].function = function;
    g_Tasks[id].arg = arg;
    g_Tasks[id].delay = delay_ticks;
    g_Tasks[id].interval = interval_ticks;
    g_Tasks[id].remaining = run_count;
    g_Tasks[id].running = TRUE;
    g_Tasks[id].pending = FALSE;
    return OK;
}

DWORD Timer2SchedulerRemove(Timer2TaskId_t id)
{
    if (!Timer2TaskIdValid(id)) {
        return ERR;
    }
    g_Tasks[id].used = FALSE;
    return OK;
}

DWORD Timer2SchedulerStart(Timer2TaskId_t id)
{
    if (!Timer2TaskIdValid(id)) {
        return ERR;
    }
    g_Tasks[id].running = TRUE;
    return OK;
}

DWORD Timer2SchedulerStop(Timer2TaskId_t id)
{
    if (!Timer2TaskIdValid(id)) {
        return ERR;
    }
    g_Tasks[id].running = FALSE;
    return OK;
}

VOID Timer2SchedulerTickISR(VOID)
{
    for (DWORD index = 0U; index < TIMER2_SCHEDULER_MAX_TASKS; ++index) {
        if (g_Tasks[index].used && g_Tasks[index].running) {
            if (g_Tasks[index].delay > 0U) {
                --g_Tasks[index].delay;
            }
            if (g_Tasks[index].delay == 0U) {
                g_Tasks[index].pending = TRUE;
                g_Tasks[index].delay = g_Tasks[index].interval;
            }
        }
    }
}

VOID Timer2SchedulerRun(VOID)
{
    for (DWORD index = 0U; index < TIMER2_SCHEDULER_MAX_TASKS; ++index) {
        if (g_Tasks[index].used && g_Tasks[index].pending) {
            g_Tasks[index].pending = FALSE;
            g_Tasks[index].function(g_Tasks[index].arg);
            if (g_Tasks[index].remaining > 0U && --g_Tasks[index].remaining == 0U) {
                g_Tasks[index].running = FALSE;
            }
        }
    }
}

VOID TIMER2_IRQHandler(VOID)
{
    if (timer_interrupt_flag_get(TIMER2, TIMER_INT_UP) != RESET) {
        timer_interrupt_flag_clear(TIMER2, TIMER_INT_UP);
        Timer2SchedulerTickISR();
    }
}

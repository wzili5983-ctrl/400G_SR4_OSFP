/***************************************************************************/ /**
 *   @file   adcresource_impl.c
 *   @brief  Implementation of adc resource.
 *   @author Oliver Wang (zili.wang@terabitcom.com)
********************************************************************************
 * Copyright 2024~2026(c) Terabitcom, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of Terabitcom, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Terabitcom, INC. “AS IS” AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL Terabitcom, INC. BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/
#include "adcresource_impl.h"
#include "monitor_impl.h"       /* g_SampleFilttedValues / MsaMonitorId_t */
#include "chipresource_impl.h"
#include "gpioresource_impl.h"
#include "gd32e511_512.h"
#include "mcu_clock.h"
#include "timer2_scheduler.h"

STATIC DWORD BoardAdcRead(BYTE id, DWORD *value);
STATIC DWORD BoardAdcInit(BYTE id, BYTE rsttype);
STATIC VOID TaskBaseAdSamples(VOID *arg);
STATIC VOID TaskRssiAdSamples(VOID *arg);

/* Number of sample rounds per ADC ID in the DMA circular buffer. */
#define SAMPLE_TIMES 6U

typedef struct {
    DWORD port;
    DWORD pin;
    BYTE rsv;
}AdcPin_t;

STATIC BOOL g_AdcReady, g_PinReady[ADC_ID_NUM];
/* GD32E512Mx DS Rev0.2 Table 2-3. Channel = ADC0 channel of the assigned pin:
 * PA0-PA7 -> ADC_CHANNEL_0-7, PC0-PC5 -> ADC_CHANNEL_10-15.
 * AdcForm_t: chipid = sampling chip (CHIP_ID_MCU), channel = ADC0 channel number,
 * adgroup = MCU_AD_SAMPLE_GROUP (housekeeping) or RSSI_AD_SAMPLE_GROUP (RSSI). */
STATIC CONST AdcInfo_t g_AdcInfoImpl[ADC_ID_NUM] = {
    {{CHIP_ID_MCU, ADC_CHANNEL_2, RSSI_AD_SAMPLE_GROUP, 0}, {BoardAdcRead, BoardAdcInit}},
    {{CHIP_ID_MCU, ADC_CHANNEL_3, RSSI_AD_SAMPLE_GROUP, 0}, {BoardAdcRead, BoardAdcInit}},
    {{CHIP_ID_MCU, ADC_CHANNEL_21, MCU_AD_SAMPLE_GROUP, 0}, {BoardAdcRead, BoardAdcInit}},
    {{CHIP_ID_MCU, ADC_CHANNEL_22, MCU_AD_SAMPLE_GROUP, 0}, {BoardAdcRead, BoardAdcInit}},
    {{CHIP_ID_MCU, ADC_CHANNEL_10, MCU_AD_SAMPLE_GROUP, 0}, {BoardAdcRead, BoardAdcInit}},
    {{CHIP_ID_MCU, ADC_CHANNEL_11, MCU_AD_SAMPLE_GROUP, 0}, {BoardAdcRead, BoardAdcInit}},
    {{CHIP_ID_MCU, ADC_CHANNEL_12, MCU_AD_SAMPLE_GROUP, 0}, {BoardAdcRead, BoardAdcInit}},
    {{CHIP_ID_MCU, ADC_CHANNEL_13, MCU_AD_SAMPLE_GROUP, 0}, {BoardAdcRead, BoardAdcInit}},
    {{CHIP_ID_MCU, ADC_CHANNEL_23, MCU_AD_SAMPLE_GROUP, 0}, {BoardAdcRead, BoardAdcInit}},
    {{CHIP_ID_MCU, ADC_CHANNEL_24, MCU_AD_SAMPLE_GROUP, 0}, {BoardAdcRead, BoardAdcInit}},
};

/* GPIO port/pin per ADC ID; removed from AdcForm_t but still needed for MCU pin setup. */
STATIC CONST AdcPin_t g_AdcPinImpl[ADC_ID_NUM] = {
    {GPIOA, GPIO_PIN_2, 0},
    {GPIOA, GPIO_PIN_3, 0},
    {GPIOA, GPIO_PIN_6, 0},
    {GPIOA, GPIO_PIN_7, 0},
    {GPIOC, GPIO_PIN_0, 0},
    {GPIOC, GPIO_PIN_1, 0},
    {GPIOC, GPIO_PIN_2, 0},
    {GPIOC, GPIO_PIN_3, 0},
    {GPIOC, GPIO_PIN_4, 0},
    {GPIOC, GPIO_PIN_5, 0},
};

/* DMA circular buffer: round 0 of all ADC_ID_NUM channels, then round 1, ... */
STATIC volatile WORD g_AdSampleRawValues[ADC_ID_NUM * SAMPLE_TIMES];
/* 任务槽位统一存调度器集中表 g_TaskIds[TASK_ID_*]（timer2_scheduler.h）。 */

CONST AdcInfo_t *GetAdcInfo(BYTE id)
{
    return id < ADC_ID_NUM ? &g_AdcInfoImpl[id] : NULL;
}

/* 读出指定 MsaMonitorId_t 槽的滤波值——MAL 监视任务按 g_StdPerfMon[].adindex
 * 消费本接口（adindex 取值空间 = MsaMonitorId_t）。ADC 未初始化完成或 id
 * 非法时返回 ERR。 */
DWORD GetAdcMsaFilteredValue(BYTE msaId, WORD *value)
{
    if(msaId >= MON_MSA_ID_NUM || value == NULL || !g_AdcReady) {
        return ERR;
    }
    *value = g_SampleFilttedValues[msaId];
    return OK;
}

/* 写 MsaMonitorId_t 槽：GN1848 监视任务（driver/drv TaskTxAdSamples）等
 * 非 MCU-ADC 源的落槽入口；仅界检查，不依赖 g_AdcReady（TX 采集与
 * MCU ADC 初始化互相独立）。 */
DWORD SetAdcMsaFilteredValue(BYTE msaId, WORD value)
{
    if(msaId >= MON_MSA_ID_NUM) {
        return ERR;
    }
    g_SampleFilttedValues[msaId] = value;
    return OK;
}

STATIC DWORD WaitCalibration(DWORD mask)
{
    DWORD timeout = 10000;
    while((ADC_CTL1(ADC0) & mask) != 0) {
        if(timeout-- == 0) {
            adc_disable(ADC0);
            return ERR;
        }
        McuClockDelayUs(1);
    }
    return OK;
}

/* Median-mean filter: sort the SAMPLE_TIMES raw samples of one channel,
 * drop the max and min, then average the rest. */
STATIC WORD AdSampleMedianMeanFilter(BYTE id)
{
    WORD samples[SAMPLE_TIMES];
    WORD temp;
    BYTE i, j;
    DWORD sum = 0U;

    for(i = 0U; i < (BYTE)SAMPLE_TIMES; ++i) {
        samples[i] = g_AdSampleRawValues[(DWORD)i * ADC_ID_NUM + id];
    }
    for(i = 1U; i < (BYTE)SAMPLE_TIMES; ++i) {
        temp = samples[i];
        for(j = i; j > 0U && samples[j - 1U] > temp; --j) {
            samples[j] = samples[j - 1U];
        }
        samples[j] = temp;
    }
    for(i = 1U; i < (BYTE)SAMPLE_TIMES - 1U; ++i) {
        sum += samples[i];
    }
    return (WORD)(sum / ((BYTE)SAMPLE_TIMES - 2U));
}

/* Filter all housekeeping channels except the two RSSI channels; period 400 ms. */
STATIC VOID TaskBaseAdSamples(VOID *arg)
{
    BYTE id;
    (void)arg;
    for(id = ADC_ID_CURRENT_MAIN; id < ADC_ID_NUM; ++id) {
        g_SampleFilttedValues[MON_MSA_ID_CURRENT_MAIN + (id - ADC_ID_CURRENT_MAIN)] = AdSampleMedianMeanFilter(id);
    }
}

/* Filter the two RSSI channels; period 100 ms.
 * ADC_ID_RSSI01/23_HIGH 两路经 GPIO_ID_RSSI_SEL_HIGH 模拟开关复用 4 路 TIA RSSI：
 *   SEL=HIGH：RSSI01→TIA RSSI0，RSSI23→TIA RSSI3
 *   SEL=LOW ：RSSI01→TIA RSSI1，RSSI23→TIA RSSI2
 * 每次运行先按当前开关相位滤波落槽，再翻转开关供下次运行采另一相——
 * 100ms 任务间隔 >> DMA 整缓冲刷新时间（10 通道×6 轮，约 1ms），采样时
 * 缓冲已全为当前相位样本，无需额外 settle 等待；每个 RSSI 槽 200ms 刷新。
 * 上电 default_level=1（HIGH），首个周期采 RSSI0/RSSI3。
 * 电平读失败（GPIO 未初始化）本轮放弃并返回，待板级初始化补齐自动恢复；
 * 写翻转失败则相位不再交替，只有当前相位的两个槽有更新（降级可观察）。 */
STATIC VOID TaskRssiAdSamples(VOID *arg)
{
    CONST GpioInfo_t *sel = GetGpioInfo(GPIO_ID_RSSI_SEL_HIGH);
    DWORD level = IO_LOW;
    (void)arg;
    if(sel == NULL || sel->gpioops.GpioRead == NULL ||
       sel->gpioops.GpioRead(GPIO_ID_RSSI_SEL_HIGH, &level) != OK) {
        return;
    }
    if(level == IO_HIGH) {
        g_SampleFilttedValues[MON_MSA_ID_RSSI0] = AdSampleMedianMeanFilter(ADC_ID_RSSI01_HIGH);
        g_SampleFilttedValues[MON_MSA_ID_RSSI3] = AdSampleMedianMeanFilter(ADC_ID_RSSI23_HIGH);
    } else {
        g_SampleFilttedValues[MON_MSA_ID_RSSI1] = AdSampleMedianMeanFilter(ADC_ID_RSSI01_HIGH);
        g_SampleFilttedValues[MON_MSA_ID_RSSI2] = AdSampleMedianMeanFilter(ADC_ID_RSSI23_HIGH);
    }
    if(sel->gpioops.GpioWrite != NULL) {
        sel->gpioops.GpioWrite(GPIO_ID_RSSI_SEL_HIGH, (level == IO_HIGH) ? IO_LOW : IO_HIGH);
    }
}

STATIC DWORD BoardAdcInit(BYTE id, BYTE rsttype)
{
    CONST AdcInfo_t *info = GetAdcInfo(id);
    (void)rsttype;
    if(info == NULL) {
        return ERR;
    }
    rcu_periph_clock_enable(g_AdcPinImpl[id].port == GPIOA ? RCU_GPIOA : RCU_GPIOC);
    gpio_mode_set(g_AdcPinImpl[id].port, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, g_AdcPinImpl[id].pin);
    if(!g_AdcReady) {
        dma_parameter_struct dma_init_struct;
        BYTE ch;
        rcu_periph_clock_enable(RCU_DMA);
        rcu_periph_clock_enable(RCU_ADC0);
        rcu_adc_clock_config(ADC0, RCU_ADCCK_APB2_DIV8);
        adc_deinit(ADC0);
        /* ADC0 DMA request is fixed to DMA channel 0; circular per-round storage. */
        dma_struct_para_init(&dma_init_struct);
        dma_init_struct.periph_addr  = (uint32_t)&ADC_RDATA(ADC0);
        dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
        dma_init_struct.memory_addr  = (uint32_t)g_AdSampleRawValues;
        dma_init_struct.memory_width = DMA_MEMORY_WIDTH_16BIT;
        dma_init_struct.number       = ADC_ID_NUM * SAMPLE_TIMES;
        dma_init_struct.priority     = DMA_PRIORITY_HIGH;
        dma_init_struct.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
        dma_init_struct.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
        dma_init_struct.direction    = DMA_PERIPHERAL_TO_MEMORY;
        dma_init(DMA_CH0, &dma_init_struct);
        dma_circulation_enable(DMA_CH0);
        dma_channel_enable(DMA_CH0);
        adc_resolution_config(ADC0, ADC_RESOLUTION_12B);
        adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
        adc_special_function_config(ADC0, ADC_SCAN_MODE | ADC_CONTINUOUS_MODE, ENABLE);
        adc_channel_length_config(ADC0, ADC_ROUTINE_CHANNEL, ADC_ID_NUM);
        for(ch = 0; ch < ADC_ID_NUM; ++ch) {
            adc_routine_channel_config(ADC0, ch, g_AdcInfoImpl[ch].adcform.channel, ADC_SAMPLETIME_239POINT5);
        }
        adc_external_trigger_source_config(ADC0, ADC_ROUTINE_CHANNEL, ADC_EXTTRIG_ROUTINE_NONE);
        adc_external_trigger_config(ADC0, ADC_ROUTINE_CHANNEL, ENABLE);
        adc_enable(ADC0);
        McuClockDelayUs(100);
        ADC_CTL1(ADC0) |= ADC_CTL1_RSTCLB;
        if(WaitCalibration(ADC_CTL1_RSTCLB) != OK) {
            return ERR;
        }
        ADC_CTL1(ADC0) |= ADC_CTL1_CLB;
        if(WaitCalibration(ADC_CTL1_CLB) != OK) {
            return ERR;
        }
        adc_dma_mode_enable(ADC0, ADC_ROUTINE_CHANNEL);
        adc_software_trigger_enable(ADC0, ADC_ROUTINE_CHANNEL);
        /* Scheduler tick is 1 ms: first run 100 ms after init, then base housekeeping
         * filter every 400 ms and RSSI filter every 100 ms, both forever.
         * GN1848 monitor collection (TaskTxAdSamples, 1 s) is registered by
         * InitDrv in driver/drv. */
        g_TaskIds[TASK_ID_BASE_AD_SAMPLES] = Timer2SchedulerAdd(TaskBaseAdSamples, NULL, 100U, 400U, TIMER2_TASK_FOREVER);
        g_TaskIds[TASK_ID_RSSI_AD_SAMPLES] = Timer2SchedulerAdd(TaskRssiAdSamples, NULL, 100U, 100U, TIMER2_TASK_FOREVER);
        g_AdcReady = TRUE;
    }
    g_PinReady[id] = TRUE;
    return OK;
}

STATIC DWORD BoardAdcRead(BYTE id, DWORD *value)
{
    CONST AdcInfo_t *info = GetAdcInfo(id);
    if(info == NULL || value == NULL || !g_AdcReady || !g_PinReady[id]) {
        return ERR;
    }
    /* Continuous scan + circular DMA keeps sampling; return the last completed round. */
    *value = g_AdSampleRawValues[(SAMPLE_TIMES - 1U) * ADC_ID_NUM + id];
    return OK;
}

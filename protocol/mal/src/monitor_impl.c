/***************************************************************************//**
 *   @file   monitor_impl.c
 *   @brief  MAL 标准性能监视器表：CMIS 5.3 被测物理量的全套落点描述。
 *   @author Firmware team
 *******************************************************************************
 * Copyright 2026 Terabitcom, Inc.
 *
 * g_StdPerfMon[PERF_MON_NUM] 与 PerfMon_t 枚举序一一对应（表项身份由
 * 下标给出，结构内不再存 montype/lane 字段）。字段语义与寻址约定见
 * monitor_impl.h 头注释，此处记录生成时对照 spec 的位布局速查：
 *
 * 模块级 Flag（00h:8-13，RO/COR；表 8-9）：
 *   9  bit7-4 = Vcc（LoWr/HiWr/LoAl/HiAl），bit3-0 = Temp（同序）
 *   10 bit7-4 = Aux2，bit3-0 = Aux1
 *   11 bit7-4 = CustomMon，bit3-0 = Aux3
 *   每个 nibble 内 [LowWarning, HighWarning, LowAlarm, HighAlarm] 自高位
 *   向低位排（HighAlarm = nibble LSB）；Mask 在 00h:32-34 同位一一对应。
 *
 * Lane 级 Flag（11h:139-152，RO/COR；表 8-86~8-88）：每个量占连续 4 字节
 *   HiAl/LoAl/HiWr/LoWr，字节内 bit i ↔ lane i+1；Mask 在 10h:213-232
 *   （表 8-81）与 Flag 字节一一对应。表内 offset[ALARM_HIGH] = 首字节
 *   （HiAl），offset[] 按 AlarmWarning_t 序各指一字节（数组序与字节
 *   内存序相反），bits[4] 同填 BIT(lane-1)。
 *
 * 阈值（Page 02h，RO，4×2B 大端，HiAl/LoAl/HiWr/LoWr 连续；表 8-62/8-63）：
 *   模块级 0x00-0x2F（Temp/Vcc/Aux1/Aux2/Aux3/Custom 各 8B），lane 级
 *   0x30/0x38/0x40（TxPwr/Bias/RxPwr 各 8B）——lane 阈值全 lane 共享。
 *
 * 校准系数（厂商页，calcoe 成员）：系数块先 K/k 后 B/b（f32 4B 一个），
 *   offset = 本条目块基址、无系数项填 MAL_MON_COEFFI_NONE。
 *   B0H = 模块级 6 类型：0x00/0x08/0x10/0x18/0x20/0x28（48B）；
 *   B1H = lane 级 TxPwr + Bias 整页 128B：TxPwr ch0-7 @0x00-0x3F、
 *   Bias ch0-7 @0x40-0x7F（ch n 基址 = 段基 + 8n）；
 *   B2H = RxPwr ch0-3（96B）：小光 K/B 段 @0x00-0x1F（ch n @8n，K@+0
 *   B@+4），大光 k3/k2/k1/b 段 @0x20-0x5F（ch n 基址 = 0x20+16n，
 *   k3@+0 k2@+4 k1@+8 b@+12）——offset 指向小光块，大光块按
 *   ch = offset>>3 推导；SR4 无 ch4-7，系数页 B3H 预留扩展。
 *
 * flag/flagmsk 的 offset[] / bits[] 与 thr.offset[] 均按 AlarmWarning_t
 * 序：[WARNING_LOW, WARNING_HIGH, ALARM_LOW, ALARM_HIGH]。模块级
 * Flag/Mask 的 offset[] 4 项同指一字节。
 *
 * 产品映射假设（bring-up 时核对，★ 标记项）：
 *   Aux1 ← 预留（不采集；01h:145.0 宣告 Aux1 未用）
 *   Aux2 ← GN1848 Temperature Monitor（SRC-2 ch0，芯片单温度传感器即全局值；
 *          T = ADC×222.8/2^16 − 66.8 °C，单点外部校准，DS §3.3.1）
 *   Aux3 ← V3P3_PHY_MON（3.3V_PHY 轨=U21 PMIC 输入；工程惯例：以其 ADC 值
 *          间接代理 DSP 电压检测——DSP 0.9V/0.75V/DVDD_AVS 均经 U21 取自该轨，
 *          2026-10-12 用户定；V1P8_MON 备选（1.8V 暂时另有他用）；U11 DSP 无
 *          3.3V 供电 用户确认）★
 *   Custom ← CURRENT_MAIN（模块总电流，mA 为自定义量纲）★
 *   RxPwr lane1-4 ← MON_MSA_ID_RSSI0..3（TIA RSSI0-3；两路 ADC 经
 *          GPIO_ID_RSSI_SEL_HIGH 开关复用，相位调度与滤波落槽见
 *          TaskRssiAdSamples；每槽 200ms 刷新）；lane5-8 无实体。
 *   TxPwr lane1-4 ← MON_MSA_ID_PWR_TX1..4，GN1848 IBIAS ADC 作功率代理
 *          （ADC_CHx_SRC0 0x20-0x26 实测电流，线性驱动无 Tx PD——产品约定★）；
 *   Bias lane1-4 ← MON_MSA_ID_LASER_BIAS0..3，GN1848 IBIAS_DAC 指令码
 *          （CHx_CTRL0 0x40-0x70，10bit；mA = code/69.0 + 1.0，DS Eq.3-2
 *          反推）；均 1s 周期，TaskTxAdSamples 采集（driver/drv）。
 *
 * adindex 取值空间 = MsaMonitorId_t（g_SampleFilttedValues 槽位，经
 * GetAdcMsaFilteredValue 读取）；NO_MON_MSA_ID(0xFF) = 无采集源。
 * cal：模块级 6 条全已实现——TEMPERATURE/LASER_TMP 复用 PerfMon
 * CalTemperature（T256 线性 K/B 标定）；VOLTAGE/DSP_VOLTAGE 复用
 * PerfMonCalVoltage（UV100 线性 K/B 标定）；CUSTOM 用 PerfMonCalCustom
 * （MON_SCALE_NONE：mA、1 mA/LSB、乘数 1）；系数落点均经 info->calcoe
 * 自描述读 B0H 影子，编码值入 value.perfvalue。lane 级：TxPwr lane1-4
 * 挂 PerfMonCalTxPwr（UW01 线性 K/B，B1H 每 lane 各 8B 系数块）——同族
 * 函数 lane 条目复用模式同模块级；Bias lane1-4 挂 PerfMonCalBias（UA2
 * 线性 K/B，B1H Bias 段每 lane 各 8B 系数块，2µA/LSB 倍率 1）；RxPwr
 * lane1-4 挂 PerfMonCalRxPwr（UW01 **三段拟合**：暗光<阈值→0、小光线
 * 性 K/B、大光三次多项式——三段阈值 B2H:0x7A-0x7F 全局共享，线性块 B2H
 * 每 lane 8B、三次块 +0x20 各 16B）。至此 30 条全挂 cal（lane5-8 预留
 * 8 条 adindex=NO_MON_MSA_ID 整项跳过）。
 * 监视任务随后把 perfvalue 转写影子（value.tab/value.
 * offset 指向的 2B）即对 host 发布。
 ******************************************************************************/
#include "monitor_impl.h"
#include "adcresource_impl.h"    /* AdcId_t / ADC_ID_* */
#include "adpat.h"               /* ReadSramProtocolMsaSpace（系数影子读取） */

/* 监视采样滤波值池（MON_MSA_ID_NUM 槽 = ADC + VDM；写入方/读取方见头文件注记） */
WORD g_SampleFilttedValues[MON_MSA_ID_NUM];

/*-------------------- cal 标定函数（NULL 表项由监视任务按 scale 默认换算） --------------------*/

/* B0H 系数 f32 解码：大端（随 CMIS 多字节字段惯例；字节序若改小端只动本函数）。 */
STATIC float MonCoeffF32(CONST BYTE *p)
{
    union { DWORD u; float f; } v;
    v.u = ((DWORD)p[0] << 24) | ((DWORD)p[1] << 16) | ((DWORD)p[2] << 8) | (DWORD)p[3];
    return v.f;
}

/** 温度类监视器（MON_SCALE_T256：TempMon/Aux2Mon 等）的线性 K/B cal：
 * 原始 ADC 码 → 1/256°C 寄存器值。入参为表项指针（VOID* 形参，函数内
 * 转 PerfMonInfo_t*）；原始采样由 info->adindex 经 GetAdcMsaFilteredValue
 * 定位；线性标定 T = K·raw + B（°C），系数落点经 info->calcoe 自描述
 * （影子页槽 + 页内基址，先 K 后 B 各 f32 4B）读本表项系数块——同量纲
 * 条目（TEMPERATURE、LASER_TMP）复用本函数，仅 calcoe.offset/adindex
 * 不同。注册前提：本表项 value.scale == MON_SCALE_T256。无系数落点
 * （calcoe.offset == MAL_MON_COEFFI_NONE）、系数未加载（K=B=0，影子 ZI
 * 初值）或 ADC 滤波值未就绪返回 ERR——监视任务跳过发布，不上电垃圾值。
 * 结果限幅 S16 后按 MON_SCALE_T256 编码入 info->value.perfvalue（监视
 * 任务转写影子发布）。 */
STATIC DWORD PerfMonCalTemperature(VOID *p)
{
    PerfMonInfo_t *info = (PerfMonInfo_t *)p;
    BYTE coe[8];
    WORD raw;
    DWORD out;
    float k, b, t;

    if(info == NULL) {
        return ERR;
    }
    if(info->calcoe.offset == MAL_MON_COEFFI_NONE) {    /* 本条目无系数落点 */
        return ERR;
    }
    if(ReadSramProtocolMsaSpace(info->calcoe.tab, info->calcoe.offset, coe, (BYTE)sizeof(coe)) != OK) {
        return ERR;
    }
    k = MonCoeffF32(&coe[0]);
    b = MonCoeffF32(&coe[4]);
    if((k == 0.0f) && (b == 0.0f)) {                    /* 系数未加载 */
        return ERR;
    }
    if(GetAdcMsaFilteredValue((BYTE)info->adindex, &raw) != OK) {   /* adindex 定位原始采样 */
        return ERR;
    }
    t = k * (float)raw + b;                             /* °C */
    if(t >= 127.99609375f) {                            /* 32767/256 */
        out = 0x7FFFU;
    } else if(t <= -128.0f) {                           /* -32768/256 */
        out = 0x8000U;
    } else {
        out = ((DWORD)(short)(t * 256.0f)) & 0xFFFFU;
    }
    info->value.perfvalue = (WORD)out;                  /* 编码值入 perfvalue 缓存 */
    return OK;
}

/** 厂商自定义量纲（MON_SCALE_NONE：CustomMon = 模块总电流）的线性 K/B
 * cal：原始 ADC 码 → U16 寄存器值。产品量纲约定：mA，1 mA/LSB（U16），
 * 量纲乘数为 1——K/B 直接以 mA（= 寄存器 LSB）表达：I = K·raw + B（mA），
 * 截断入 U16，限幅 0-65535、负值归零。★ 若产品改定其它自定义编码
 * （倍率 ≠ 1 或 S16），只改本函数末尾编码两行，守卫与系数读取不变。
 * 入参/系数自描述/ERR 守卫同 PerfMonCalTemperature（注册前提：
 * 本表项 value.scale == MON_SCALE_NONE）。 */
STATIC DWORD PerfMonCalCustom(VOID *p)
{
    PerfMonInfo_t *info = (PerfMonInfo_t *)p;
    BYTE coe[8];
    WORD raw;
    DWORD out;
    float k, b, i_ma;

    if(info == NULL) {
        return ERR;
    }
    if(info->calcoe.offset == MAL_MON_COEFFI_NONE) {    /* 本条目无系数落点 */
        return ERR;
    }
    if(ReadSramProtocolMsaSpace(info->calcoe.tab, info->calcoe.offset, coe, (BYTE)sizeof(coe)) != OK) {
        return ERR;
    }
    k = MonCoeffF32(&coe[0]);
    b = MonCoeffF32(&coe[4]);
    if((k == 0.0f) && (b == 0.0f)) {                    /* 系数未加载 */
        return ERR;
    }
    if(GetAdcMsaFilteredValue((BYTE)info->adindex, &raw) != OK) {   /* adindex 定位原始采样 */
        return ERR;
    }
    i_ma = k * (float)raw + b;                          /* mA（= 寄存器 LSB，乘数 1） */
    if(i_ma <= 0.0f) {
        out = 0U;                                       /* U16 无负值：归零 */
    } else if(i_ma >= 65535.0f) {
        out = 0xFFFFU;
    } else {
        out = (DWORD)i_ma;                              /* 1 mA/LSB，截断 */
    }
    info->value.perfvalue = (WORD)out;                  /* 编码值入 perfvalue 缓存 */
    return OK;
}
/** PERF_MON_VOLTAGE 的 cal：主供电 Vcc 原始 ADC 码 → 100µV 寄存器值。
 * 入参为表项指针（VOID* 形参，函数内转 PerfMonInfo_t*）；原始采样由
 * info->adindex 经 GetAdcMsaFilteredValue 定位；线性标定 V = K·raw +
 * B（V），系数落点经 info->calcoe 自描述（影子页槽 + 页内基址，先 K
 * 后 B 各 f32 4B）读本表项系数块——注册前提：本表项 value.scale ==
 * MON_SCALE_UV100。无系数落点（MAL_MON_COEFFI_NONE）、系数未加载
 * （K=B=0）或 ADC 滤波值未就绪返回 ERR——监视任务跳过发布。编码 U16
 * = V ÷ 100µV（×10000，截断同 TEMPERATURE 惯例），限幅 0-65535
 * （0-6.5535 V，负值归零）。 */
STATIC DWORD PerfMonCalVoltage(VOID *p)
{
    PerfMonInfo_t *info = (PerfMonInfo_t *)p;
    BYTE coe[8];
    WORD raw;
    DWORD out;
    float k, b, v;

    if(info == NULL) {
        return ERR;
    }
    if(info->calcoe.offset == MAL_MON_COEFFI_NONE) {    /* 本条目无系数落点 */
        return ERR;
    }
    if(ReadSramProtocolMsaSpace(info->calcoe.tab, info->calcoe.offset, coe, (BYTE)sizeof(coe)) != OK) {
        return ERR;
    }
    k = MonCoeffF32(&coe[0]);
    b = MonCoeffF32(&coe[4]);
    if((k == 0.0f) && (b == 0.0f)) {                    /* 系数未加载 */
        return ERR;
    }
    if(GetAdcMsaFilteredValue((BYTE)info->adindex, &raw) != OK) {   /* adindex 定位原始采样 */
        return ERR;
    }
    v = k * (float)raw + b;                             /* V */
    if(v <= 0.0f) {
        out = 0U;                                       /* U16 无负值：归零 */
    } else if(v >= 6.5535f) {                           /* 65535 × 100µV */
        out = 0xFFFFU;
    } else {
        out = (DWORD)(v * 10000.0f);                    /* 100µV/LSB，截断 */
    }
    info->value.perfvalue = (WORD)out;                  /* 编码值入 perfvalue 缓存 */
    return OK;
}

/** 发射光功率监视器（MON_SCALE_UW01：OpticalPowerTx lane1-8）的线性 K/B
 * cal：原始 ADC 码（GN1848 IBIAS 代理采样）→ 0.1µW 寄存器值。P = K·raw
 * + B（µW），系数经 info->calcoe 自描述读本 lane 系数块（B1H 每 lane 各
 * 8B：K@+0、B@+4，f32 大端）——同族 lane 条目（TX1-8）复用本函数，差
 * 异全在 calcoe.offset/adindex。注册前提：本表项 value.scale ==
 * MON_SCALE_UW01（Bias 条目属 MON_SCALE_UA2 族，编码不同，勿挂本函
 * 数）。ERR 守卫同 PerfMonCalTemperature。编码 U16 = P ÷ 0.1µW（×10，
 * 截断），限幅 0-65535（0-6553.5µW，负值归零）。 */
STATIC DWORD PerfMonCalTxPwr(VOID *p)
{
    PerfMonInfo_t *info = (PerfMonInfo_t *)p;
    BYTE coe[8];
    WORD raw;
    DWORD out;
    float k, b, puw;

    if(info == NULL) {
        return ERR;
    }
    if(info->calcoe.offset == MAL_MON_COEFFI_NONE) {    /* 本条目无系数落点 */
        return ERR;
    }
    if(ReadSramProtocolMsaSpace(info->calcoe.tab, info->calcoe.offset, coe, (BYTE)sizeof(coe)) != OK) {
        return ERR;
    }
    k = MonCoeffF32(&coe[0]);
    b = MonCoeffF32(&coe[4]);
    if((k == 0.0f) && (b == 0.0f)) {                    /* 系数未加载 */
        return ERR;
    }
    if(GetAdcMsaFilteredValue((BYTE)info->adindex, &raw) != OK) {   /* adindex 定位原始采样 */
        return ERR;
    }
    puw = k * (float)raw + b;                           /* µW */
    if(puw <= 0.0f) {
        out = 0U;                                       /* U16 无负值：归零 */
    } else if(puw >= 6553.5f) {                         /* 65535 × 0.1µW */
        out = 0xFFFFU;
    } else {
        out = (DWORD)(puw * 10.0f);                     /* 0.1µW/LSB，截断 */
    }
    info->value.perfvalue = (WORD)out;                  /* 编码值入 perfvalue 缓存 */
    return OK;
}

/** 激光偏置电流监视器（MON_SCALE_UA2：LaserBiasTx lane1-8）的线性 K/B
 * cal：原始 ADC 码（GN1848 IBIAS 采样）→ 2µA 寄存器值。I = K·raw + B
 * （mA——GN1848 DS §3.3.2 的 IBIAS 公式即以 mA 表达），系数经 info->
 * calcoe 自描述读本 lane 系数块（B1H Bias 段 ch n 基址 = 0x40+8n：
 * K@+0、B@+4，f32 大端）——同族 lane 条目（BIAS1-8）复用本函数，差异
 * 全在 calcoe.offset/adindex。注册前提：本表项 value.scale ==
 * MON_SCALE_UA2（TxPwr 条目属 MON_SCALE_UW01 族，编码不同，勿挂本函
 * 数）。ERR 守卫同 PerfMonCalTemperature。编码 U16 = I ÷ 2µA（mA×500，
 * 截断），限幅 0-65535（0-131.07mA，负值归零）；★ CMIS 01h:160.4-3 的
 * Bias 倍率固定按 1 处理——若产品需宣告倍率 ≠ 1，本函数编码行 ×倍率
 * 且 01h 宣告位同步改。 */
STATIC DWORD PerfMonCalBias(VOID *p)
{
    PerfMonInfo_t *info = (PerfMonInfo_t *)p;
    BYTE coe[8];
    WORD raw;
    DWORD out;
    float k, b, i_ma;

    if(info == NULL) {
        return ERR;
    }
    if(info->calcoe.offset == MAL_MON_COEFFI_NONE) {    /* 本条目无系数落点 */
        return ERR;
    }
    if(ReadSramProtocolMsaSpace(info->calcoe.tab, info->calcoe.offset, coe, (BYTE)sizeof(coe)) != OK) {
        return ERR;
    }
    k = MonCoeffF32(&coe[0]);
    b = MonCoeffF32(&coe[4]);
    if((k == 0.0f) && (b == 0.0f)) {                    /* 系数未加载 */
        return ERR;
    }
    if(GetAdcMsaFilteredValue((BYTE)info->adindex, &raw) != OK) {   /* adindex 定位原始采样 */
        return ERR;
    }
    i_ma = k * (float)raw + b;                          /* mA */
    if(i_ma <= 0.0f) {
        out = 0U;                                       /* U16 无负值：归零 */
    } else if(i_ma >= 131.07f) {                        /* 65535 × 2µA */
        out = 0xFFFFU;
    } else {
        out = (DWORD)(i_ma * 500.0f);                   /* 2µA/LSB（倍率 1），截断 */
    }
    info->value.perfvalue = (WORD)out;                  /* 编码值入 perfvalue 缓存 */
    return OK;
}

/* RxPwr 三段拟合阈值基址：B2H 影子 0x7A-0x7F（厂商系数页尾区，0x00-0x5F
 * 为线性/三次系数块，0x60-0x7F 空闲），暗/小/大 各 2B 大端连续，ADC 滤波
 * 值（g_SampleFilttedValues）域，全 lane 共享一份。
 * ★B2H 尚未在 g_OsfpTabForm 登记（同 B0H/B1H/B3H）——host 读写/参数持久
 * 化需补登记；当前按产品定稿地址直读影子。 */

/** 接收光功率监视器（MON_SCALE_UW01：OpticalPowerRx lane1-8）的**三段
 * 拟合** cal：原始 ADC 码（TIA RSSI）→ 0.1µW 寄存器值。分段门限 =
 * PAGE_B2H_ADDR_RXPWR_THR 起 6B（B2H 影子 0x7A-0x7F：暗 0x7A/小 0x7C/大
 * 0x7E，16bit 大端，ADC 滤波值域，全 lane 共享一份）：
 *   raw < 暗光阈值            → 功率最小值 0x0000（不读任何系数）；
 *   暗光阈值 ≤ raw ≤ 大光阈值   → 小光线性段 P = K·raw + B（µW），系数块
 *                              8B @info->calcoe.offset（B2H 小光段，K@+0
 *                              B@+4）；
 *     ——其中 [小光阈值, 大光阈值] 为规格留白过渡带，本实现按线性外延保
 *     证两段交接单调（★ 产品若另定过渡带归属，只改本分支条件）；
 *   raw > 大光阈值            → 大光三次段 P = ((k3·raw + k2)·raw + k1)
 *                              ·raw + b（Horner，µW），系数块 16B @
 *                              calcoe.offset + 0x20（B2H 大光段：k3@+0
 *                              k2@+4 k1@+8 b@+12）。
 * 同族 lane 条目（RX1-4）复用本函数，差异全在 calcoe.offset/adindex。
 * 注册前提：本表项 value.scale == MON_SCALE_UW01。ERR 守卫：info
 * NULL、calcoe 未配、阈值未加载/非递增（须 暗 < 小 ≤ 大，全 0 亦拒）、
 * 本段系数未加载（线性 K=B=0 / 大光四项全 0）、ADC 未就绪——监视任务
 * 跳过发布。编码 U16 = P ÷ 0.1µW（×10 截断），限幅 0-65535（负值归
 * 零），同 PerfMonCalTxPwr。 */
STATIC DWORD PerfMonCalRxPwr(VOID *p)
{
    PerfMonInfo_t *info = (PerfMonInfo_t *)p;
    BYTE thb[6];
    WORD raw, dark_thr, small_thr, large_thr;
    DWORD out;
    float puw;

    if(info == NULL) {
        return ERR;
    }
    if(info->calcoe.offset == MAL_MON_COEFFI_NONE) {    /* 本条目无系数落点 */
        return ERR;
    }
    if(ReadSramProtocolMsaSpace(PAGE_ID_B2H, PAGE_B2H_ADDR_RXPWR_THR, thb, (BYTE)sizeof(thb)) != OK) {
        return ERR;
    }
    dark_thr  = (WORD)(((WORD)thb[0] << 8) | (WORD)thb[1]);         /* 16bit 大端 */
    small_thr = (WORD)(((WORD)thb[2] << 8) | (WORD)thb[3]);
    large_thr = (WORD)(((WORD)thb[4] << 8) | (WORD)thb[5]);
    if((dark_thr >= small_thr) || (small_thr > large_thr)) {        /* 未加载/非递增（全 0 亦拒） */
        return ERR;
    }
    if(GetAdcMsaFilteredValue((BYTE)info->adindex, &raw) != OK) {   /* adindex 定位原始采样 */
        return ERR;
    }
    if(raw < dark_thr) {
        out = 0U;                                       /* 暗光段：功率最小值 */
    } else {
        BYTE coe[8];
        if(raw <= large_thr) {                          /* 小光线性段（含过渡带线性外延 ★） */
            float k, b;
            if(ReadSramProtocolMsaSpace(info->calcoe.tab, info->calcoe.offset, coe, (BYTE)sizeof(coe)) != OK) {
                return ERR;
            }
            k = MonCoeffF32(&coe[0]);
            b = MonCoeffF32(&coe[4]);
            if((k == 0.0f) && (b == 0.0f)) {            /* 线性系数未加载 */
                return ERR;
            }
            puw = k * (float)raw + b;                   /* µW */
        } else {                                        /* 大光三次段 */
            BYTE cco[16];
            float k3, k2, k1, b, r;
            if(ReadSramProtocolMsaSpace(info->calcoe.tab, (BYTE)(info->calcoe.offset + 0x20U), cco, (BYTE)sizeof(cco)) != OK) {
                return ERR;
            }
            k3 = MonCoeffF32(&cco[0]);
            k2 = MonCoeffF32(&cco[4]);
            k1 = MonCoeffF32(&cco[8]);
            b  = MonCoeffF32(&cco[12]);
            if((k3 == 0.0f) && (k2 == 0.0f) && (k1 == 0.0f) && (b == 0.0f)) {   /* 大光系数未加载 */
                return ERR;
            }
            r = (float)raw;
            puw = ((k3 * r + k2) * r + k1) * r + b;     /* Horner，µW */
        }
        if(puw <= 0.0f) {
            out = 0U;                                   /* U16 无负值：归零 */
        } else if(puw >= 6553.5f) {                     /* 65535 × 0.1µW */
            out = 0xFFFFU;
        } else {
            out = (DWORD)(puw * 10.0f);                 /* 0.1µW/LSB，截断 */
        }
    }
    info->value.perfvalue = (WORD)out;                  /* 编码值入 perfvalue 缓存 */
    return OK;
}

STATIC PerfMonInfo_t g_StdPerfMon[PERF_MON_NUM] = {
    /* ============ 模块级 ×6：监视值 00h:14-25 / Flag 00h:9-11 /
     *              Mask 00h:32-34 / 阈值 02h:0x00-0x2F ============ */
    /* [PERF_MON_TEMPERATURE] 模块温度 TempMon（S16，1/256°C） */
    {
        MON_MSA_ID_MODULE_TEMP,             /* 板内 NTC */
        PerfMonCalTemperature,              /* T = K·raw + B，K/B@B0H:0x00/0x04 */
        {0U, PAGE_ID_LOW, 0x0E, MON_SCALE_T256},
        {PAGE_ID_LOW, {0x09, 0x09, 0x09, 0x09}, {BIT3, BIT2, BIT1, BIT0}},           /* 00h:9 bit3-0 */
        {PAGE_ID_LOW, {0x20, 0x20, 0x20, 0x20}, {BIT3, BIT2, BIT1, BIT0}},           /* 00h:32 同位 */
        {PAGE_ID_02H, {0x06, 0x04, 0x02, 0x00}, MON_SCALE_T256}, /* @02h:0x00-0x07 */
        {PAGE_ID_B0H, 0x00, MON_SCALE_FLOAT},   /* 系数@B0H:TEMPERATURE(K@+0,B@+4 f32) */
    },
    /* [PERF_MON_VOLTAGE] 主供电 VccMon（U16，100µV） */
    {
        MON_MSA_ID_VCC_MON,
        PerfMonCalVoltage,                  /* V = K·raw + B，K/B@B0H:0x08/0x0C */
        {0U, PAGE_ID_LOW, 0x10, MON_SCALE_UV100},
        {PAGE_ID_LOW, {0x09, 0x09, 0x09, 0x09}, {BIT7, BIT6, BIT5, BIT4}},           /* 00h:9 bit7-4 */
        {PAGE_ID_LOW, {0x20, 0x20, 0x20, 0x20}, {BIT7, BIT6, BIT5, BIT4}},           /* 00h:32 */
        {PAGE_ID_02H, {0x0E, 0x0C, 0x0A, 0x08}, MON_SCALE_UV100},/* @02h:0x08-0x0F */
        {PAGE_ID_B0H, 0x08, MON_SCALE_FLOAT},   /* 系数@B0H:VOLTAGE(K@+0,B@+4 f32) */
    },
    /* [PERF_MON_AUX1] Aux1Mon——预留：adindex=NO_MON_MSA_ID，监视任务整项跳过
     * （不采样/不阈值比较/不置 flag），发布值保持 0；偏移字段仅留作 CMIS
     * 位置描述。01h:145.0 宣告位须报 Aux1 未用。 */
    {
        NO_MON_MSA_ID,                      /* 预留，无采集源 */
        NULL,
        {0U, PAGE_ID_LOW, 0x12, MON_SCALE_NONE},                 /* 量纲产品自定义 */
        {PAGE_ID_LOW, {0x0A, 0x0A, 0x0A, 0x0A}, {BIT3, BIT2, BIT1, BIT0}},           /* 00h:10 bit3-0 */
        {PAGE_ID_LOW, {0x21, 0x21, 0x21, 0x21}, {BIT3, BIT2, BIT1, BIT0}},           /* 00h:33 */
        {PAGE_ID_02H, {0x16, 0x14, 0x12, 0x10}, MON_SCALE_NONE}, /* @02h:0x10-0x17 */
        {PAGE_ID_B0H, 0x10, MON_SCALE_FLOAT},   /* 系数@B0H:AUX1(K@+0,B@+4 f32) */
    },
    /* [PERF_MON_LASER_TMP] Aux2Mon = 激光器温度（S16，1/256°C；01h:145.1） */
    {
        MON_MSA_ID_LASER_TEMP,              /* GN1848 Temperature Monitor（SRC-2 ch0，单传感器全局值） */
        PerfMonCalTemperature,              /* 与 TEMPERATURE 同量纲同编码，函数经 calcoe/adindex 自描述复用；K/B@B0H:0x18/0x1C */
        {0U, PAGE_ID_LOW, 0x14, MON_SCALE_T256},
        {PAGE_ID_LOW, {0x0A, 0x0A, 0x0A, 0x0A}, {BIT7, BIT6, BIT5, BIT4}},           /* 00h:10 bit7-4 */
        {PAGE_ID_LOW, {0x21, 0x21, 0x21, 0x21}, {BIT7, BIT6, BIT5, BIT4}},           /* 00h:33 */
        {PAGE_ID_02H, {0x1E, 0x1C, 0x1A, 0x18}, MON_SCALE_T256}, /* @02h:0x18-0x1F */
        {PAGE_ID_B0H, 0x18, MON_SCALE_FLOAT},   /* 系数@B0H:LASER_TMP(Aux2)(K@+0,B@+4 f32) */
    },
    /* [PERF_MON_DSP_VOLTAGE] Aux3Mon = DSP 电源电压（U16，100µV；01h:145.2） */
    {
        MON_MSA_ID_V3P3_PHY_MON,            /* 3.3V_PHY 轨（U21 输入；DSP 电压间接代理：0.9/0.75/0.58V 经 U21 取自该轨，2026-10-12 用户定；U11 无 3.3V 供电）★，V1P8_MON 备选（1.8V 暂另有他用） */
        PerfMonCalVoltage,                  /* 与 VOLTAGE 同量纲同编码，复用；K/B@B0H:0x20/0x24 */
        {0U, PAGE_ID_LOW, 0x16, MON_SCALE_UV100},
        {PAGE_ID_LOW, {0x0B, 0x0B, 0x0B, 0x0B}, {BIT3, BIT2, BIT1, BIT0}},           /* 00h:11 bit3-0 */
        {PAGE_ID_LOW, {0x22, 0x22, 0x22, 0x22}, {BIT3, BIT2, BIT1, BIT0}},           /* 00h:34 */
        {PAGE_ID_02H, {0x26, 0x24, 0x22, 0x20}, MON_SCALE_UV100},/* @02h:0x20-0x27 */
        {PAGE_ID_B0H, 0x20, MON_SCALE_FLOAT},   /* 系数@B0H:DSP_VOLTAGE(Aux3)(K@+0,B@+4 f32) */
    },
    /* [PERF_MON_CUSTOM] CustomMon（S16/U16，厂商自定义=模块总电流 mA ★） */
    {
        MON_MSA_ID_CURRENT_MAIN,            /* ★产品映射假设，bring-up 核对 */
        PerfMonCalCustom,                   /* I = K·raw + B（mA，1mA/LSB U16），K/B@B0H:0x28/0x2C */
        {0U, PAGE_ID_LOW, 0x18, MON_SCALE_NONE},
        {PAGE_ID_LOW, {0x0B, 0x0B, 0x0B, 0x0B}, {BIT7, BIT6, BIT5, BIT4}},           /* 00h:11 bit7-4 */
        {PAGE_ID_LOW, {0x22, 0x22, 0x22, 0x22}, {BIT7, BIT6, BIT5, BIT4}},           /* 00h:34 */
        {PAGE_ID_02H, {0x2E, 0x2C, 0x2A, 0x28}, MON_SCALE_NONE}, /* @02h:0x28-0x2F */
        {PAGE_ID_B0H, 0x28, MON_SCALE_FLOAT},   /* 系数@B0H:CUSTOM(K@+0,B@+4 f32) */
    },

    /* ============ Lane 级：监视值 11h:154-201（0.1µW / 2µA×倍率）/
     *   Flag 11h:139-152（4B/量，HiAl 首字节，bit i = lane i+1；
     *   offset[4] 按 AlarmWarning_t 序各指一字节，bits[4]=BIT(lane-1)）/
     *   Mask 10h:218-231（对应 4B/量）/ 阈值 02h:0x30/0x38/0x40（全 lane 共享）
     *   SR4 仅 lane1-4 有实体；lane5-8 描述符保留，发布值保持 0 ============ */
    /* [PERF_MON_OPT_PWR_TX1..8] OpticalPowerTx（U16 0.1µW；GN1848 IBIAS 代理，★）
     *   lane1-4 ← MON_MSA_ID_PWR_TX1..4（SRC-0 ch0-3）；lane5-8 预留 */
    {
        MON_MSA_ID_PWR_TX1, PerfMonCalTxPwr, /* GN1848 IBIAS 作 TxPwr 代理（SRC-0 ch0）★；P = K·raw + B（µW，0.1µW/LSB U16），K/B@B1H:0x00/0x04 */
        {0U, PAGE_ID_11H_0, 0x1A, MON_SCALE_UW01},
        {PAGE_ID_11H_0, {0x0E, 0x0D, 0x0C, 0x0B}, {BIT0, BIT0, BIT0, BIT0}},         /* TxPwr Flag@139-142 */
        {PAGE_ID_10H_0, {0x5D, 0x5C, 0x5B, 0x5A}, {BIT0, BIT0, BIT0, BIT0}},         /* Mask@10h:218-221 */
        {PAGE_ID_02H, {0x36, 0x34, 0x32, 0x30}, MON_SCALE_UW01},
        {PAGE_ID_B1H, 0x00, MON_SCALE_FLOAT},   /* 系数@B1H:TX1 ch0(K@0x00,B@0x04 f32) */
    },
    {
        MON_MSA_ID_PWR_TX2, PerfMonCalTxPwr, /* SRC-0 ch1 ★；K/B@B1H:0x08/0x0C */
        {0U, PAGE_ID_11H_0, 0x1C, MON_SCALE_UW01},
        {PAGE_ID_11H_0, {0x0E, 0x0D, 0x0C, 0x0B}, {BIT1, BIT1, BIT1, BIT1}},
        {PAGE_ID_10H_0, {0x5D, 0x5C, 0x5B, 0x5A}, {BIT1, BIT1, BIT1, BIT1}},
        {PAGE_ID_02H, {0x36, 0x34, 0x32, 0x30}, MON_SCALE_UW01},
        {PAGE_ID_B1H, 0x08, MON_SCALE_FLOAT},   /* 系数@B1H:TX2 ch1(K@0x08,B@0x0C f32) */
    },
    {
        MON_MSA_ID_PWR_TX3, PerfMonCalTxPwr, /* SRC-0 ch2 ★；K/B@B1H:0x10/0x14 */
        {0U, PAGE_ID_11H_0, 0x1E, MON_SCALE_UW01},
        {PAGE_ID_11H_0, {0x0E, 0x0D, 0x0C, 0x0B}, {BIT2, BIT2, BIT2, BIT2}},
        {PAGE_ID_10H_0, {0x5D, 0x5C, 0x5B, 0x5A}, {BIT2, BIT2, BIT2, BIT2}},
        {PAGE_ID_02H, {0x36, 0x34, 0x32, 0x30}, MON_SCALE_UW01},
        {PAGE_ID_B1H, 0x10, MON_SCALE_FLOAT},   /* 系数@B1H:TX3 ch2(K@0x10,B@0x14 f32) */
    },
    {
        MON_MSA_ID_PWR_TX4, PerfMonCalTxPwr, /* SRC-0 ch3 ★；K/B@B1H:0x18/0x1C */
        {0U, PAGE_ID_11H_0, 0x20, MON_SCALE_UW01},
        {PAGE_ID_11H_0, {0x0E, 0x0D, 0x0C, 0x0B}, {BIT3, BIT3, BIT3, BIT3}},
        {PAGE_ID_10H_0, {0x5D, 0x5C, 0x5B, 0x5A}, {BIT3, BIT3, BIT3, BIT3}},
        {PAGE_ID_02H, {0x36, 0x34, 0x32, 0x30}, MON_SCALE_UW01},
        {PAGE_ID_B1H, 0x18, MON_SCALE_FLOAT},   /* 系数@B1H:TX4 ch3(K@0x18,B@0x1C f32) */
    },
    {
        NO_MON_MSA_ID, NULL,                                        /* SR4 无 lane5 */
        {0U, PAGE_ID_11H_0, 0x22, MON_SCALE_UW01},
        {PAGE_ID_11H_0, {0x0E, 0x0D, 0x0C, 0x0B}, {BIT4, BIT4, BIT4, BIT4}},
        {PAGE_ID_10H_0, {0x5D, 0x5C, 0x5B, 0x5A}, {BIT4, BIT4, BIT4, BIT4}},
        {PAGE_ID_02H, {0x36, 0x34, 0x32, 0x30}, MON_SCALE_UW01},
        {PAGE_ID_B1H, 0x20, MON_SCALE_FLOAT},   /* 系数@B1H:TX5 ch4(K@0x20,B@0x24 f32) */
    },
    {
        NO_MON_MSA_ID, NULL,                                        /* SR4 无 lane6 */
        {0U, PAGE_ID_11H_0, 0x24, MON_SCALE_UW01},
        {PAGE_ID_11H_0, {0x0E, 0x0D, 0x0C, 0x0B}, {BIT5, BIT5, BIT5, BIT5}},
        {PAGE_ID_10H_0, {0x5D, 0x5C, 0x5B, 0x5A}, {BIT5, BIT5, BIT5, BIT5}},
        {PAGE_ID_02H, {0x36, 0x34, 0x32, 0x30}, MON_SCALE_UW01},
        {PAGE_ID_B1H, 0x28, MON_SCALE_FLOAT},   /* 系数@B1H:TX6 ch5(K@0x28,B@0x2C f32) */
    },
    {
        NO_MON_MSA_ID, NULL,                                        /* SR4 无 lane7 */
        {0U, PAGE_ID_11H_0, 0x26, MON_SCALE_UW01},
        {PAGE_ID_11H_0, {0x0E, 0x0D, 0x0C, 0x0B}, {BIT6, BIT6, BIT6, BIT6}},
        {PAGE_ID_10H_0, {0x5D, 0x5C, 0x5B, 0x5A}, {BIT6, BIT6, BIT6, BIT6}},
        {PAGE_ID_02H, {0x36, 0x34, 0x32, 0x30}, MON_SCALE_UW01},
        {PAGE_ID_B1H, 0x30, MON_SCALE_FLOAT},   /* 系数@B1H:TX7 ch6(K@0x30,B@0x34 f32) */
    },
    {
        NO_MON_MSA_ID, NULL,                                        /* SR4 无 lane8 */
        {0U, PAGE_ID_11H_0, 0x28, MON_SCALE_UW01},
        {PAGE_ID_11H_0, {0x0E, 0x0D, 0x0C, 0x0B}, {BIT7, BIT7, BIT7, BIT7}},
        {PAGE_ID_10H_0, {0x5D, 0x5C, 0x5B, 0x5A}, {BIT7, BIT7, BIT7, BIT7}},
        {PAGE_ID_02H, {0x36, 0x34, 0x32, 0x30}, MON_SCALE_UW01},
        {PAGE_ID_B1H, 0x38, MON_SCALE_FLOAT},   /* 系数@B1H:TX8 ch7(K@0x38,B@0x3C f32) */
    },
    /* [PERF_MON_LASER_BIAS1..8] LaserBiasTx（U16，2µA×倍率 01h:160.4-3）
     *   lane1-4 ← MON_MSA_ID_LASER_BIAS0..3（GN1848 IBIAS，SRC-0 ch0-3）；
     *   lane5-8 预留 */
    {
        MON_MSA_ID_LASER_BIAS0, PerfMonCalBias, /* GN1848 IBIAS（SRC-0 ch0）；I = K·raw + B（mA，2µA/LSB U16 倍率1），K/B@B1H:0x40/0x44 */
        {0U, PAGE_ID_11H_0, 0x2A, MON_SCALE_UA2},
        {PAGE_ID_11H_0, {0x12, 0x11, 0x10, 0x0F}, {BIT0, BIT0, BIT0, BIT0}},         /* Bias Flag@143-146 */
        {PAGE_ID_10H_0, {0x61, 0x60, 0x5F, 0x5E}, {BIT0, BIT0, BIT0, BIT0}},         /* Mask@10h:222-225 */
        {PAGE_ID_02H, {0x3E, 0x3C, 0x3A, 0x38}, MON_SCALE_UA2},
        {PAGE_ID_B1H, 0x40, MON_SCALE_FLOAT},   /* 系数@B1H:BIAS1 ch0(K@0x40,B@0x44 f32) */
    },
    {
        MON_MSA_ID_LASER_BIAS1, PerfMonCalBias, /* SRC-0 ch1；K/B@B1H:0x48/0x4C */
        {0U, PAGE_ID_11H_0, 0x2C, MON_SCALE_UA2},
        {PAGE_ID_11H_0, {0x12, 0x11, 0x10, 0x0F}, {BIT1, BIT1, BIT1, BIT1}},
        {PAGE_ID_10H_0, {0x61, 0x60, 0x5F, 0x5E}, {BIT1, BIT1, BIT1, BIT1}},
        {PAGE_ID_02H, {0x3E, 0x3C, 0x3A, 0x38}, MON_SCALE_UA2},
        {PAGE_ID_B1H, 0x48, MON_SCALE_FLOAT},   /* 系数@B1H:BIAS2 ch1(K@0x48,B@0x4C f32) */
    },
    {
        MON_MSA_ID_LASER_BIAS2, PerfMonCalBias, /* SRC-0 ch2；K/B@B1H:0x50/0x54 */
        {0U, PAGE_ID_11H_0, 0x2E, MON_SCALE_UA2},
        {PAGE_ID_11H_0, {0x12, 0x11, 0x10, 0x0F}, {BIT2, BIT2, BIT2, BIT2}},
        {PAGE_ID_10H_0, {0x61, 0x60, 0x5F, 0x5E}, {BIT2, BIT2, BIT2, BIT2}},
        {PAGE_ID_02H, {0x3E, 0x3C, 0x3A, 0x38}, MON_SCALE_UA2},
        {PAGE_ID_B1H, 0x50, MON_SCALE_FLOAT},   /* 系数@B1H:BIAS3 ch2(K@0x50,B@0x54 f32) */
    },
    {
        MON_MSA_ID_LASER_BIAS3, PerfMonCalBias, /* SRC-0 ch3；K/B@B1H:0x58/0x5C */
        {0U, PAGE_ID_11H_0, 0x30, MON_SCALE_UA2},
        {PAGE_ID_11H_0, {0x12, 0x11, 0x10, 0x0F}, {BIT3, BIT3, BIT3, BIT3}},
        {PAGE_ID_10H_0, {0x61, 0x60, 0x5F, 0x5E}, {BIT3, BIT3, BIT3, BIT3}},
        {PAGE_ID_02H, {0x3E, 0x3C, 0x3A, 0x38}, MON_SCALE_UA2},
        {PAGE_ID_B1H, 0x58, MON_SCALE_FLOAT},   /* 系数@B1H:BIAS4 ch3(K@0x58,B@0x5C f32) */
    },
    {
        NO_MON_MSA_ID, NULL,                                        /* SR4 无 bias5 */
        {0U, PAGE_ID_11H_0, 0x32, MON_SCALE_UA2},
        {PAGE_ID_11H_0, {0x12, 0x11, 0x10, 0x0F}, {BIT4, BIT4, BIT4, BIT4}},
        {PAGE_ID_10H_0, {0x61, 0x60, 0x5F, 0x5E}, {BIT4, BIT4, BIT4, BIT4}},
        {PAGE_ID_02H, {0x3E, 0x3C, 0x3A, 0x38}, MON_SCALE_UA2},
        {PAGE_ID_B1H, 0x60, MON_SCALE_FLOAT},   /* 系数@B1H:BIAS5 ch4(K@0x60,B@0x64 f32) */
    },
    {
        NO_MON_MSA_ID, NULL,                                        /* SR4 无 bias6 */
        {0U, PAGE_ID_11H_0, 0x34, MON_SCALE_UA2},
        {PAGE_ID_11H_0, {0x12, 0x11, 0x10, 0x0F}, {BIT5, BIT5, BIT5, BIT5}},
        {PAGE_ID_10H_0, {0x61, 0x60, 0x5F, 0x5E}, {BIT5, BIT5, BIT5, BIT5}},
        {PAGE_ID_02H, {0x3E, 0x3C, 0x3A, 0x38}, MON_SCALE_UA2},
        {PAGE_ID_B1H, 0x68, MON_SCALE_FLOAT},   /* 系数@B1H:BIAS6 ch5(K@0x68,B@0x6C f32) */
    },
    {
        NO_MON_MSA_ID, NULL,                                        /* SR4 无 bias7 */
        {0U, PAGE_ID_11H_0, 0x36, MON_SCALE_UA2},
        {PAGE_ID_11H_0, {0x12, 0x11, 0x10, 0x0F}, {BIT6, BIT6, BIT6, BIT6}},
        {PAGE_ID_10H_0, {0x61, 0x60, 0x5F, 0x5E}, {BIT6, BIT6, BIT6, BIT6}},
        {PAGE_ID_02H, {0x3E, 0x3C, 0x3A, 0x38}, MON_SCALE_UA2},
        {PAGE_ID_B1H, 0x70, MON_SCALE_FLOAT},   /* 系数@B1H:BIAS7 ch6(K@0x70,B@0x74 f32) */
    },
    {
        NO_MON_MSA_ID, NULL,                                        /* SR4 无 bias8 */
        {0U, PAGE_ID_11H_0, 0x38, MON_SCALE_UA2},
        {PAGE_ID_11H_0, {0x12, 0x11, 0x10, 0x0F}, {BIT7, BIT7, BIT7, BIT7}},
        {PAGE_ID_10H_0, {0x61, 0x60, 0x5F, 0x5E}, {BIT7, BIT7, BIT7, BIT7}},
        {PAGE_ID_02H, {0x3E, 0x3C, 0x3A, 0x38}, MON_SCALE_UA2},
        {PAGE_ID_B1H, 0x78, MON_SCALE_FLOAT},   /* 系数@B1H:BIAS8 ch7(K@0x78,B@0x7C f32) */
    },
    /* [PERF_MON_OPT_PWR_RX1..8] OpticalPowerRx（U16 0.1µW；RSSI ADC 双通道组） */
    {
        MON_MSA_ID_RSSI0,                   /* TIA RSSI0（SEL 相位调度见 TaskRssiAdSamples）★ */
        PerfMonCalRxPwr,                    /* 三段拟合：raw<暗阈(B2H:0x7A)→0；暗阈..大阈(0x7E)→线性K/B@B2H:0x00/0x04；>大阈→三次@B2H:0x20-0x2F */
        {0U, PAGE_ID_11H_0, 0x3A, MON_SCALE_UW01},
        {PAGE_ID_11H_0, {0x18, 0x17, 0x16, 0x15}, {BIT0, BIT0, BIT0, BIT0}},         /* RxPwr Flag@149-152 */
        {PAGE_ID_10H_0, {0x67, 0x66, 0x65, 0x64}, {BIT0, BIT0, BIT0, BIT0}},         /* Mask@10h:228-231 */
        {PAGE_ID_02H, {0x46, 0x44, 0x42, 0x40}, MON_SCALE_UW01},
        {PAGE_ID_B2H, 0x00, MON_SCALE_FLOAT},   /* 系数@B2H:RX1 ch0 小光K/B(K@0x00,B@0x04);大光k3k2k1b@0x20-0x2F f32 */
    },
    {
        MON_MSA_ID_RSSI1,                   /* TIA RSSI1 ★ */
        PerfMonCalRxPwr,                    /* 三段拟合；线性K/B@B2H:0x08/0x0C，三次@B2H:0x30-0x3F */
        {0U, PAGE_ID_11H_0, 0x3C, MON_SCALE_UW01},
        {PAGE_ID_11H_0, {0x18, 0x17, 0x16, 0x15}, {BIT1, BIT1, BIT1, BIT1}},
        {PAGE_ID_10H_0, {0x67, 0x66, 0x65, 0x64}, {BIT1, BIT1, BIT1, BIT1}},
        {PAGE_ID_02H, {0x46, 0x44, 0x42, 0x40}, MON_SCALE_UW01},
        {PAGE_ID_B2H, 0x08, MON_SCALE_FLOAT},   /* 系数@B2H:RX2 ch1 小光K/B(K@0x08,B@0x0C);大光k3k2k1b@0x30-0x3F f32 */
    },
    {
        MON_MSA_ID_RSSI2,                   /* TIA RSSI2 ★ */
        PerfMonCalRxPwr,                    /* 三段拟合；线性K/B@B2H:0x10/0x14，三次@B2H:0x40-0x4F */
        {0U, PAGE_ID_11H_0, 0x3E, MON_SCALE_UW01},
        {PAGE_ID_11H_0, {0x18, 0x17, 0x16, 0x15}, {BIT2, BIT2, BIT2, BIT2}},
        {PAGE_ID_10H_0, {0x67, 0x66, 0x65, 0x64}, {BIT2, BIT2, BIT2, BIT2}},
        {PAGE_ID_02H, {0x46, 0x44, 0x42, 0x40}, MON_SCALE_UW01},
        {PAGE_ID_B2H, 0x10, MON_SCALE_FLOAT},   /* 系数@B2H:RX3 ch2 小光K/B(K@0x10,B@0x14);大光k3k2k1b@0x40-0x4F f32 */
    },
    {
        MON_MSA_ID_RSSI3,                   /* TIA RSSI3 ★ */
        PerfMonCalRxPwr,                    /* 三段拟合；线性K/B@B2H:0x18/0x1C，三次@B2H:0x50-0x5F */
        {0U, PAGE_ID_11H_0, 0x40, MON_SCALE_UW01},
        {PAGE_ID_11H_0, {0x18, 0x17, 0x16, 0x15}, {BIT3, BIT3, BIT3, BIT3}},
        {PAGE_ID_10H_0, {0x67, 0x66, 0x65, 0x64}, {BIT3, BIT3, BIT3, BIT3}},
        {PAGE_ID_02H, {0x46, 0x44, 0x42, 0x40}, MON_SCALE_UW01},
        {PAGE_ID_B2H, 0x18, MON_SCALE_FLOAT},   /* 系数@B2H:RX4 ch3 小光K/B(K@0x18,B@0x1C);大光k3k2k1b@0x50-0x5F f32 */
    },
    {
        NO_MON_MSA_ID, NULL,                    /* SR4 无 lane5；RSSI45 组不存在 */
        {0U, PAGE_ID_11H_0, 0x42, MON_SCALE_UW01},
        {PAGE_ID_11H_0, {0x18, 0x17, 0x16, 0x15}, {BIT4, BIT4, BIT4, BIT4}},
        {PAGE_ID_10H_0, {0x67, 0x66, 0x65, 0x64}, {BIT4, BIT4, BIT4, BIT4}},
        {PAGE_ID_02H, {0x46, 0x44, 0x42, 0x40}, MON_SCALE_UW01},
        {PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT},   /* SR4 无 ch4；系数页 B3H 预留扩展 */
    },
    {
        NO_MON_MSA_ID, NULL,                    /* SR4 无 lane6 */
        {0U, PAGE_ID_11H_0, 0x44, MON_SCALE_UW01},
        {PAGE_ID_11H_0, {0x18, 0x17, 0x16, 0x15}, {BIT5, BIT5, BIT5, BIT5}},
        {PAGE_ID_10H_0, {0x67, 0x66, 0x65, 0x64}, {BIT5, BIT5, BIT5, BIT5}},
        {PAGE_ID_02H, {0x46, 0x44, 0x42, 0x40}, MON_SCALE_UW01},
        {PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT},
    },
    {
        NO_MON_MSA_ID, NULL,                    /* SR4 无 lane7 */
        {0U, PAGE_ID_11H_0, 0x46, MON_SCALE_UW01},
        {PAGE_ID_11H_0, {0x18, 0x17, 0x16, 0x15}, {BIT6, BIT6, BIT6, BIT6}},
        {PAGE_ID_10H_0, {0x67, 0x66, 0x65, 0x64}, {BIT6, BIT6, BIT6, BIT6}},
        {PAGE_ID_02H, {0x46, 0x44, 0x42, 0x40}, MON_SCALE_UW01},
        {PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT},
    },
    {
        NO_MON_MSA_ID, NULL,                    /* SR4 无 lane8 */
        {0U, PAGE_ID_11H_0, 0x48, MON_SCALE_UW01},
        {PAGE_ID_11H_0, {0x18, 0x17, 0x16, 0x15}, {BIT7, BIT7, BIT7, BIT7}},
        {PAGE_ID_10H_0, {0x67, 0x66, 0x65, 0x64}, {BIT7, BIT7, BIT7, BIT7}},
        {PAGE_ID_02H, {0x46, 0x44, 0x42, 0x40}, MON_SCALE_UW01},
        {PAGE_ID_B0H, MAL_MON_COEFFI_NONE, MON_SCALE_FLOAT},
    },
};

/*-------------------- 表访问 API（g_StdPerfMon 私有，外部经此访问，见头文件注记） --------------------*/

CONST PerfMonInfo_t *MonGetStdPerfMonEntry(PerfMon_t id)
{
    if((DWORD)id >= (DWORD)PERF_MON_NUM) { return NULL; }
    return &g_StdPerfMon[id];
}

PerfMonInfo_t *MonGetStdPerfMonEntryRw(PerfMon_t id)
{
    if((DWORD)id >= (DWORD)PERF_MON_NUM) { return NULL; }
    return &g_StdPerfMon[id];
}

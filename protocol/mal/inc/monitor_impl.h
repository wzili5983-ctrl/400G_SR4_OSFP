/** @file monitor_impl.h
 * @brief MAL 标准性能监视器描述表（g_StdPerfMon）类型与访问 API。
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * 「标准性能监视器」静态描述表：每项把一个被测物理量与其全套 CMIS
 * 落点（监视值 value / Flag / Mask / 阈值 thr / 校准系数 calcoe）绑定。
 * 本头是类型与访问 API 的单一入口；位布局速查、厂商系数页布局、产品
 * 映射假设见 monitor_impl.c 文件头。
 *
 * 表本体 g_StdPerfMon 为 monitor_impl.c 私有（STATIC），外部一律经
 * MonGetStdPerfMonEntry()/MonGetStdPerfMonEntryRw() 按下标访问——
 * 只读视图供外部查询描述符，读写视图仅供 MAL 监视任务（monitor.c，
 * cal 需写 value.perfvalue）。
 *
 * 覆盖 = CMIS 5.3 标准监视量：模块级 6（00h:14-25）+ lane 级 3×8
 * （Page 11h Bank0；SR4 仅 lane1-4 有实体，lane5-8 描述符保留，
 * adindex 填 NO_ADC_MSA_ID，监视任务整项跳过）。不入表：Page 12h
 * （可调激光器，SR4 VCSEL 非可调）、VDM 20h-2Fh（厂商自定义扩展，
 * 后续单独描述符表）。
 */
#ifndef MONITOR_IMPL_H
#define MONITOR_IMPL_H

#include "basedef.h"
#include "ral.h"                  /* TabIndex_t / PAGE_ID_* */
#include "adcresource_impl.h"     /* AdcMsaId_t（adindex 取值空间） */

#ifdef __cplusplus
extern "C" {
#endif

/** 监视物理量类型（MON_TYPE_NUM = 类型数）。 */
#define MON_TYPE_TEMP        0U    /* 模块/激光器温度 */
#define MON_TYPE_VCC         1U    /* 主供电电压 */
#define MON_TYPE_AUX1        2U    /* Aux1（功能由 01h:145.0 宣告） */
#define MON_TYPE_AUX2        3U    /* Aux2（01h:145.1） */
#define MON_TYPE_AUX3        4U    /* Aux3（01h:145.2） */
#define MON_TYPE_CUSTOM      5U    /* 厂商自定义模块级监视量 */
#define MON_TYPE_OPTPWR_TX   6U    /* Tx 输出光功率 */
#define MON_TYPE_LASER_BIAS  7U    /* Tx 激光偏流 */
#define MON_TYPE_OPTPWR_RX   8U    /* Rx 输入光功率 */
#define MON_TYPE_NUM         9U

/** 编码量纲：寄存器 LSB 单位（2B 大端存放；有符号性按量纲约定——
 *  MON_SCALE_T256 为 S16，其余 U16）。 */
#define MON_SCALE_T256       0U    /* S16，1/256 °C（温度） */
#define MON_SCALE_UV100      1U    /* U16，100 µV（电压） */
#define MON_SCALE_UW01       2U    /* U16，0.1 µW（光功率，0-6.5535 mW ≈ -40~+8.2 dBm） */
#define MON_SCALE_UA2        3U    /* U16，2 µA × 倍率 01h:160.4-3（偏流，倍率 1 = 0-131 mA） */
#define MON_SCALE_NONE       4U    /* 厂商自定义量纲（CustomMon，S16/U16 由产品定） */
#define MON_SCALE_FLOAT      5U    /* f32（IEEE 754）：校准系数（K/B）量纲——
                                    * 存厂商页影子（calcoe）；host 可见的监视
                                    * 值/阈值仍按上述整型 scale 编码发布 */

/** 表项缺省哨兵（页内偏移取值 0-127，0xFF 恒非法，作"无"标记）。 */
#define MAL_MON_THR_NONE     0xFFU   /* thr.offset[]：该告警/警告类型无阈值 */
#define MAL_MON_COEFFI_NONE  0xFFU   /* calcoe.offset：该条目无校准系数 */

/** 告警/警告类型序：flag/flagmsk 的 offset[]/bits[] 与 thr.offset[]
 *  均按下标 [WARNING_LOW, WARNING_HIGH, ALARM_LOW, ALARM_HIGH]。 */
typedef enum {
    WARNING_LOW = 0,
    WARNING_HIGH,
    ALARM_LOW,
    ALARM_HIGH,
    ALARM_WARNING_NUM
} AlarmWarning_t;

/* cal 契约：info = 本表项指针（VOID* 形参，函数内转 PerfMonInfo_t*）。
 * 原始采样由 info->adindex 经 GetAdcMsaFilteredValue 定位；标定系数经
 * info->calcoe 自描述取（影子页槽 + 页内基址，先 K/k 后 B/b 各 f32 4B），
 * 不硬编码页/偏移。返回 OK 时算得的性能值按本条目 value.scale 编码后入
 * info->value.perfvalue——监视任务转写影子发布，不再做默认换算；返回
 * ERR 时本次跳过发布。cal = NULL 的表项由监视任务按 scale 默认换算
 * （同样入 value.perfvalue）。 */
typedef struct PerfMonInfo_t PerfMonInfo_t;   /* 前向声明：PerfCal 形参引用本类型 */
typedef DWORD (*PerfCal)(VOID *info);

/** 监视值落点：tab/offset 指影子页槽 + 页内偏移（2B 大端，S16/U16 按
 *  scale）；perfvalue = 编码值缓存——cal 写入（cal = NULL 时监视任务
 *  默认换算写入），监视任务随后转写影子发布。 */
typedef struct {
    WORD        perfvalue;  /* 编码值缓存（cal/默认换算写入） */
    TabIndex_t  tab;        /* 影子页槽（PAGE_ID_LOW / PAGE_ID_11H_0 …） */
    BYTE        offset;     /* 页内偏移 */
    BYTE        scale;      /* MON_SCALE_* 编码量纲 */
} PerfMonValueForm_t;

/** Flag 落点：offset[]/bits[] 按 AlarmWarning_t 序。模块级条目 4 项
 *  offset 同指一字节（4 bit 共存，字节内 HiAl = nibble LSB）；lane 级
 *  条目 4 项各指一字节（HiAl/LoAl/HiWr/LoWr 连续，offset[ALARM_HIGH]
 *  = 首字节，数组序与内存序相反），bits[4] 同填 BIT(lane-1)。 */
typedef struct {
    TabIndex_t  tab;                        /* 影子页槽（模块级 00h / lane 级 11h） */
    BYTE        offset[ALARM_WARNING_NUM];  /* 页内偏移，4 项按 AlarmWarning_t 序 */
    Bit_t       bits[ALARM_WARNING_NUM];    /* 位位置，序同上 */
} PerfMonFlagForm_t;

/** Mask 落点：形参同 Flag（页槽不同：模块级 00h:32-34 / lane 级 10h
 *  与 Flag 字节一一对应）。Mask 只影响中断汇总，不影响 Flags 本体。 */
typedef struct {
    TabIndex_t  tab;                        /* 影子页槽（模块级 00h / lane 级 10h） */
    BYTE        offset[ALARM_WARNING_NUM];  /* 页内偏移，序同 Flag */
    Bit_t       bits[ALARM_WARNING_NUM];    /* 位位置，序同上 */
} PerfMonFlagMskForm_t;

/** 阈值落点：Page 02h 影子，4×2B 大端与监视值同量纲同编码，可直接整型
 *  比较；模块级 0x00-0x2F、lane 级 0x30/0x38/0x40 全 lane 共享。 */
typedef struct {
    TabIndex_t  tab;                        /* 影子页槽（PAGE_ID_02H） */
    BYTE        offset[ALARM_WARNING_NUM];  /* 页内偏移，4 项按 AlarmWarning_t 序 */
    BYTE        scale;                      /* MON_SCALE_* 编码量纲 */
} PerfMonThresholdForm_t;

/** 校准系数落点：系数块先 K/k 后 B/b（f32 4B 一个），offset = 本条目
 *  系数块页内基址（无系数项填 MAL_MON_COEFFI_NONE），scale 恒
 *  MON_SCALE_FLOAT。厂商系数页布局：
 *  B0H = 模块级 6 类型 ×8B（0x00 起 8B 步距，共 48B）；
 *  B1H = TxPwr+Bias 整页 128B（TxPwr ch0-7 @0x00-0x3F、Bias ch0-7
 *        @0x40-0x7F，ch n 基址 = 段基 + 8n）；
 *  B2H = RxPwr ch0-3 双段 96B（小光 K/B 段 @0x00-0x1F ch n @8n；
 *        大光 k3/k2/k1/b 段 @0x20-0x5F ch n @0x20+16n，k3@+0 k2@+4
 *        k1@+8 b@+12）——offset 指向小光块，大光块按 ch = offset>>3
 *        推导；ch4-7 系数页 B3H 预留。 */
typedef struct {
    TabIndex_t  tab;        /* 系数影子页槽（模块级 B0H / lane 级 B1H、B2H） */
    BYTE        offset;     /* 本条目系数块页内基址（K/k@+0 f32, B/b@+4 f32；无 = MAL_MON_COEFFI_NONE） */
    BYTE        scale;      /* MON_SCALE_FLOAT */
} PerfMonCalCoeffiForm_t;

/** 标准性能监视器表项（描述符 + 运行时缓存的复合体；表本体 RAM 私有）。 */
typedef struct PerfMonInfo_t {
    AdcMsaId_t             adindex;   /* ADC 索引（AdcMsaId_t；NO_ADC_MSA_ID = 无采集源） */
    PerfCal                cal;       /* 性能计算函数（NULL = 监视任务按 scale 默认换算） */
    PerfMonValueForm_t     value;     /* 监视值落点 + 编码值缓存 */
    PerfMonFlagForm_t      flag;      /* Alarm/Warning Flag 落点（COR，引擎读后清） */
    PerfMonFlagMskForm_t   flagmsk;   /* Flag Mask 落点（只影响中断汇总） */
    PerfMonThresholdForm_t thr;       /* 阈值落点（02h 影子） */
    PerfMonCalCoeffiForm_t calcoe;    /* 校准系数落点（厂商页影子） */
} PerfMonInfo_t;

/** 标准性能监视器 ID = g_StdPerfMon 下标（表项身份由枚举给出，结构内
 *  不再存类型/lane 字段）。命名：LASER_BIAS1..8（1 起始）↔
 *  ADC_MSA_ID_LASER_BIAS0..3（0 起始），表内显式对应。 */
typedef enum {
    PERF_MON_TEMPERATURE,
    PERF_MON_VOLTAGE,
    PERF_MON_AUX1,                 /* Aux1（预留） */
    PERF_MON_LASER_TMP,            /* Aux2 */
    PERF_MON_DSP_VOLTAGE,          /* Aux3 */
    PERF_MON_CUSTOM,               /* Custom module-level monitor */
    PERF_MON_OPT_PWR_TX1,
    PERF_MON_OPT_PWR_TX2,
    PERF_MON_OPT_PWR_TX3,
    PERF_MON_OPT_PWR_TX4,
    PERF_MON_OPT_PWR_TX5,
    PERF_MON_OPT_PWR_TX6,
    PERF_MON_OPT_PWR_TX7,
    PERF_MON_OPT_PWR_TX8,
    PERF_MON_LASER_BIAS1,
    PERF_MON_LASER_BIAS2,
    PERF_MON_LASER_BIAS3,
    PERF_MON_LASER_BIAS4,
    PERF_MON_LASER_BIAS5,
    PERF_MON_LASER_BIAS6,
    PERF_MON_LASER_BIAS7,
    PERF_MON_LASER_BIAS8,
    PERF_MON_OPT_PWR_RX1,
    PERF_MON_OPT_PWR_RX2,
    PERF_MON_OPT_PWR_RX3,
    PERF_MON_OPT_PWR_RX4,
    PERF_MON_OPT_PWR_RX5,
    PERF_MON_OPT_PWR_RX6,
    PERF_MON_OPT_PWR_RX7,
    PERF_MON_OPT_PWR_RX8,
    PERF_MON_NUM
} PerfMon_t;

/** 只读表访问：返回 g_StdPerfMon[id] 的 CONST 视图——外部模块（RAL /
 *  diag / 上位机路径）查描述符用，不可改表；id 越界返回 NULL。 */
CONST PerfMonInfo_t *MonGetStdPerfMonEntry(PerfMon_t id);

/** 读写表访问：返回 g_StdPerfMon[id] 的可写指针——仅限 MAL 监视任务
 *  （monitor.c，cal 需写 value.perfvalue）；其余外部一律用只读版。
 *  id 越界返回 NULL。 */
PerfMonInfo_t *MonGetStdPerfMonEntryRw(PerfMon_t id);

#ifdef __cplusplus
}
#endif

#endif /* MONITOR_IMPL_H */

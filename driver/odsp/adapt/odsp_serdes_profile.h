/** @file odsp_serdes_profile.h
 * @brief BCM87420 SerDes bring-up 常量表 API（DSP 业务逻辑方案 v5 §2）。
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * 设计约定：
 *   - 配置即数据：极性 / lane swap / TX FIR 电平族全是"版图事实 + SI 实测
 *     结果"，生命周期 = 本次上电，用编译期常量表表达；
 *   - 默认全关可 bisect：所有 apply 开关为 0 即完全使用芯片默认，bring-up
 *     每定位一项开一项；
 *   - 改表重编是预期操作：编辑本表 → Keil 构建 → pack_framed_bin 升级；
 *     不进影子页、不进 Flash 资源区、host 不可见不可写。
 *   - 本头文件保持 cAPI 无关（纯数据类型），枚举常量（如 TXFIR_TAPS_7TAP）
 *     只在 odsp_serdes_profile.c 内引用 chip_mode_def.h 使用。
 */
#ifndef ODSP_SERDES_PROFILE_H
#define ODSP_SERDES_PROFILE_H

#include "basedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* S7 单侧单 lane 的 TX FIR + 电平族配置项。
 * 字段序即位置初始化序，改动须与 .c 中初始化宏同步。
 * tap[7] 排布：pre2, pre1, main, post1, post2, post3, post4。
 * level_shift_keep_current=1：boot 时先 get 现值随新 taps 一起回写
 * （满足官方"txfir 与 dsp_level_shift 必须成对下发"硬约束，且不动
 * 芯片电平校准）；=0 时使用 dsp_level_shift[4] 显式值。 */
typedef struct {
    BYTE    apply;                      /* 0=该 lane 跳过（芯片默认） */
    int16_t tap[7];                     /* pre2,pre1,main,post1..post4 */
    BYTE    num_of_taps;                /* txfir_taps_t（5 = TXFIR_TAPS_7TAP） */
    BYTE    level_shift_keep_current;   /* 1=set 前 get 现值回写（推荐） */
    int8_t  dsp_level_shift[4];         /* keep_current=0 时的显式值 */
    BYTE    apply_high_swing;           /* 以下为可选伴随参数，各自独立开关 */
    BYTE    high_swing;
    BYTE    apply_hs_dac_cur;           /* line 侧 DAC 电流档 [0,4] */
    BYTE    hs_dac_cur;
} OdspSerdesLaneTx_t;

/* 完整 bring-up 档。polarity 掩码 bit i = lane i 的 P/N 反接标志。 */
typedef struct {
    /* ---- S4 极性（4 维各一反极性掩码）----
     * 定案 2026-10-16（用户 PCB 连接核对）：
     *   host 入（金手指 Tx → DSP CL_I*）：TxP→CL_IN / TxN→CL_IP = 交叉
     *   host 出（DSP CL_O* → 金手指 Rx）：RxN→CL_OP / RxP→CL_ON = 交叉
     *   media 出（DSP LN_O* → DRV IN*）：LNOP→INP / LNON→INN     = 直通
     *   media 入（TIA OUT* → DSP LN_I*）：OUTP→LNIP / OUTN→LNIN  = 直通
     * ★4 lane 同 pattern 假设，layout 逐 lane 复核。 */
    BYTE    polarity_apply;             /* 0=跳过 S4 全部（芯片默认极性） */
    BYTE    host_ingress_inv_mask;      /* 本定案 = 0xF */
    BYTE    host_egress_inv_mask;       /* 本定案 = 0xF */
    BYTE    media_ingress_inv_mask;     /* 本定案 = 0x00 */
    BYTE    media_egress_inv_mask;      /* 本定案 = 0x00 */

    /* ---- S5 lane swap（推荐直通 = apply 0）----
     * 映射表语义（lane_swap_info_t）：下标 = 封装物理 lane id，
     * 元素值 = 原理图逻辑 lane id。直通档填 0..7 恒等表存档。 */
    BYTE    lane_swap_apply;
    BYTE    lane_swap_auto_restore_dis; /* 1=先写 DISABLE_AUTO_RESTORE（未用） */
    BYTE    host_rx_lane_list[8];
    BYTE    host_tx_lane_list[8];
    BYTE    media_rx_lane_list[8];
    BYTE    media_tx_lane_list[8];

    /* ---- S7 TX FIR + 电平族（每侧 4 lane 一份）---- */
    BYTE                tx_apply;       /* 0=跳过 S7 全部（芯片默认） */
    OdspSerdesLaneTx_t  host_lane[4];
    OdspSerdesLaneTx_t  media_lane[4];
} OdspSerdesProfile_t;

/* 唯一实例（odsp_serdes_profile.c，STATIC CONST 编译期定案）。 */
extern CONST OdspSerdesProfile_t k_OdspSerdesProfile;

#ifdef __cplusplus
}
#endif

#endif

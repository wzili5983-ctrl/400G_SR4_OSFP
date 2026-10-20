/** @file odsp_serdes_profile.c
 * @brief BCM87420 SerDes bring-up 常量表实例（BRINGUP 定案档）。
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * 当前生效档 = BRINGUP（方案 v5 §2.3，2026-10-16 用户定案）：
 *   S4 极性：host 双方向交叉（0xF/0xF），media 双方向直通（0/0）
 *   S5 lane swap：直通（apply=0，恒等表存档）
 *   S7 TX FIR：双侧 4 lane 指南典型 7-tap（pre2=0,pre1=-4,main=120），
 *              dsp_level_shift 采用 keep_current（先 get 现值随 taps 回写）
 *
 * 改档方式：直接编辑本文件数值 → 重编 → 升级镜像（不进影子页）。
 * 多档并存时复制本文件内容改为另一 STATIC CONST + 选择宏，参照 v5 §2.3。
 */
#include "odsp_serdes_profile.h"

#include "chip_mode_def.h"      /* TXFIR_TAPS_7TAP */

/* 单 lane TX 定案初始化（位置序 = 结构体字段序，C90 兼容）：
 * apply=1, tap={0,-4,120,0,0,0,0}, 7TAP, keep_current=1,
 * dsp_level_shift={0,0,0,0}（占位，keep_current=1 时不使用）,
 * high_swing/hs_dac_cur 均不下发。 */
#define ODSP_LANE_TX_BRINGUP_INIT \
    { 1u, { 0, -4, 120, 0, 0, 0, 0 }, TXFIR_TAPS_7TAP, 1u, { 0, 0, 0, 0 }, 0u, 0u, 0u, 0u }

/* 8 lane 恒等映射表存档（S5 直通档语义参考）。 */
#define ODSP_LANE_SWAP_IDENTITY_INIT { 0u, 1u, 2u, 3u, 4u, 5u, 6u, 7u }

CONST OdspSerdesProfile_t k_OdspSerdesProfile = {
    /* ---- S4 极性（PCB 连接定案 2026-10-16）----
     * host 入：TxP→CL_IN / TxN→CL_IP = 交叉 → 0xF
     * host 出：RxN→CL_OP / RxP→CL_ON = 交叉 → 0xF
     * media 入：OUTP→LNIP / OUTN→LNIN = 直通 → 0x00
     * media 出：LNOP→INP / LNON→INN = 直通 → 0x00 */
    1u,         /* polarity_apply */
    0xFu,       /* host_ingress_inv_mask */
    0xFu,       /* host_egress_inv_mask */
    0x00u,      /* media_ingress_inv_mask */
    0x00u,      /* media_egress_inv_mask */

    /* ---- S5 lane swap：推荐直通 ---- */
    0u,         /* lane_swap_apply */
    0u,         /* lane_swap_auto_restore_dis（FW 默认上电恢复即可——
                   本 boot 每次重发 swap，无需禁能自动恢复） */
    ODSP_LANE_SWAP_IDENTITY_INIT,   /* host_rx_lane_list */
    ODSP_LANE_SWAP_IDENTITY_INIT,   /* host_tx_lane_list */
    ODSP_LANE_SWAP_IDENTITY_INIT,   /* media_rx_lane_list */
    ODSP_LANE_SWAP_IDENTITY_INIT,   /* media_tx_lane_list */

    /* ---- S7 TX FIR：双侧 4 lane 指南典型 7-tap ---- */
    1u,         /* tx_apply */
    { ODSP_LANE_TX_BRINGUP_INIT, ODSP_LANE_TX_BRINGUP_INIT,
      ODSP_LANE_TX_BRINGUP_INIT, ODSP_LANE_TX_BRINGUP_INIT },     /* host_lane[4] */
    { ODSP_LANE_TX_BRINGUP_INIT, ODSP_LANE_TX_BRINGUP_INIT,
      ODSP_LANE_TX_BRINGUP_INIT, ODSP_LANE_TX_BRINGUP_INIT },     /* media_lane[4] */
};

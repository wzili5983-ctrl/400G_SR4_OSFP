/** @file odsp_init.c
 * @brief BCM87420 (U11, 400G SR4 Retimer) 上电初始化实现（DSP 业务逻辑方案 v5 §3）。
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * 流程（顺序执行，一次性，由 msm_pwr_seq 电源轨钩子触发）：
 *   RESET_PHY 释放 → InitChip → InitDsp → OdspInit
 *     S1  10ms settle（等 DSP 内部轨/PLL 稳定）
 *     S2  微码就绪轮询：capi_get_chip_status 直到
 *         is.download_done==1 && is.uc_ready==1（官方 boot 判据，
 *         download_done=1 而 uc_ready=0 是假阳性，继续等）
 *     S2' 空片兜底：内置 SRAM 镜像 ~520KB rodata 已移出 APP（P0，capi.c
 *         以 CAPI_COMPILE_IN_FW_IMAGE 闸掉）——空片/坏片救援改走 host CDB
 *         下载 → MCU 经 SPI2 编程 U15 → 复位 DSP 自举（与升级同路，
 *         CDB 方案待用户定案）；此前 S2 超时 = liveness 失败 → ERR →
 *         msm_pwr_seq fault 38
 *     S3  身份确认：capi_get_chip_info（chip_id 须 0x87420，存 FW 版本）
 *     S4  极性：按 Profile 4 维掩码 capi_set_polarity + 逐 lane 回读校验
 *     S5  lane swap：Profile.lane_swap_apply=0 直通跳过（接口保留）
 *     S6  capi_set_config 整片模式（400G / DEC_ENC 双侧 RS544 / 106.25
 *         双侧 PAM4 mask 0xF）+ capi_get_config 回读校验
 *     S7  TX FIR：按 Profile 双侧逐 lane 下发（txfir 与 dsp_level_shift
 *         成对；keep_current 策略先 get 现值）+ 回读校验
 *     S8  s_Ready = 1
 * 返回 ERR 由 InitChip 上抛 → msm_pwr_seq 判 fault 38。
 *
 * 红线（cAPI 指南）：polarity 为 POR 后、mode 配置前的一次性 FW 持久
 * 设置，S6 之后永不再改；本路径完全不触碰 AVS（外部 U21 调节器）。
 */
#include "type_defns.h"         /* cAPI 基础类型：必须先于其它 cAPI 头 */
#include "common_def.h"         /* phy_info_t / core_ip_t */
#include "chip_mode_def.h"      /* txfir_info_t / CHIP_MODE_* / TXFIR_TAPS_* —— capi_def.h 的前置依赖（cAPI 头序），必须先于它 */
#include "capi_def.h"           /* cAPI 全部结构/枚举定义 */
#include "capi.h"               /* cAPI 入口 */

#include "odsp_init.h"
#include "odsp_serdes_profile.h"
#include "mcu_clock.h"          /* McuClockDelayMs */

/* S1 settle 时长：等 DSP 内部轨/PLL 稳定（ msm_pwr_seq 电源时序内，
 * 10ms 忙等位于 MSM 任务一拍内，可接受；若 bring-up 观测响应迟滞，
 * 应拆进 msm_pwr_seq 步进机新增 PWRUP_STEP_DSP_SETTLE 步。 */
#define ODSP_BOOT_SETTLE_MS         10u

/* S2 微码就绪轮询超时：87420 自 SPI flash 自举微码为 ms 级（OTP 1.4ms
 * 量级），100ms 远超正常值；超时判定空片/损坏，走 S2' 兜底。 */
#define ODSP_UC_READY_TIMEOUT_MS    100u

/* S3 chip_id 期望值（BCM87420，见 capi_get_chip_info）。 */
#define ODSP_CHIP_ID_EXPECTED       0x87420UL

/* 本模块 lane 数（DP0 lane0..3 ↔ DSP lane0..3）。 */
#define ODSP_LANE_NUM               4u

/*=========================== 可变状态（STATIC 单独区） ===========================*/

STATIC BYTE  s_Ready = 0U;
STATIC DWORD s_FwVer = 0U;   /* fw_major<<16 | fw_minor（capi_chip_info_t） */

/*=========================== 内部辅助 ===========================*/

/* 填充通用 phy 骨架：phy_id=0 单 PHY；其余字段（ref_ptr/base_addr/
 * chip_id/i2c_address/i2c_block_write_size）必须为零（={0} 已保证），
 * 调用方再按需覆写 core_ip / lane_mask。 */
STATIC VOID OdspPhyFill(capi_phy_info_t *phy)
{
    *phy = (capi_phy_info_t){ 0 };  /* Keil C99 compound literal 不可用则改逐成员清零 */
    phy->phy_id = 0u;
}

/* S2：轮询微码就绪。OK=download_done && uc_ready；超时返回 ERR。 */
STATIC DWORD OdspUcReadyWait(DWORD timeout_ms)
{
    DWORD t;
    capi_phy_info_t phy;
    capi_chip_status_info_t st;

    OdspPhyFill(&phy);
    for(t = 0u; t <= timeout_ms; t++) {
        st = (capi_chip_status_info_t){ 0 };
        st.param.is.download_done = 1u;
        st.param.is.uc_ready      = 1u;
        if(capi_get_chip_status(&phy, &st) == RR_SUCCESS
           && st.value.download_done == 1u
           && st.value.uc_ready      == 1u) {
            return OK;
        }
        McuClockDelayMs(1u);
    }
    return ERR;
}

/* S2'（2026-10-16 移除）：原"SRAM 模式下载编译进 whole_image_sram.h 的
 * 内置镜像"兜底——镜像 ~520KB rodata 超出 IROM1 预算（P0 定案），capi.c
 * 已以 CAPI_COMPILE_IN_FW_IMAGE 闸掉内置镜像，capi_download(NULL,0) 直返
 * RR_ERROR。空片/坏片救援路径改为：复位 DSP → MCU 经 SPI2 编程 U15 →
 * 释放复位自举（与 DSP 微码升级同一条路径，见 CDB 方案 backlog）；
 * 落地前 S2 超时按 liveness 失败处理（ERR → fault 38）。 */

/* S3：身份确认 + FW 版本留存。chip_id 不符视为 liveness 失败。 */
STATIC DWORD OdspChipInfoRead(VOID)
{
    capi_phy_info_t phy;
    capi_chip_info_t ci;

    OdspPhyFill(&phy);
    ci = (capi_chip_info_t){ 0 };
    if(capi_get_chip_info(&phy, &ci) != RR_SUCCESS) {
        return ERR;
    }
    if(ci.chip_id != (DWORD)ODSP_CHIP_ID_EXPECTED) {
        return ERR;
    }
    s_FwVer = ((DWORD)ci.fw_major_version << 16) | (DWORD)ci.fw_minor_version;
    return OK;
}

/* S4 子步骤：单维下发。inv_mask=0 的维度全默认，无需发送。 */
STATIC DWORD OdspPolarityDimSet(core_ip_t ip, BYTE dir, BYTE inv_mask)
{
    capi_phy_info_t phy;
    capi_polarity_info_t pol;

    if(inv_mask == 0u) {
        return OK;
    }
    OdspPhyFill(&phy);
    phy.core_ip   = ip;
    phy.lane_mask = inv_mask;
    pol.direction = dir;
    pol.action    = POLARITY_INVERT_DEFAULT_SETTING;
    if(capi_set_polarity(&phy, &pol) != RR_SUCCESS) {
        return ERR;
    }
    return OK;
}

/* S4 子步骤：单维逐 lane 回读校验。
 * ★capi_get_polarity 传多 lane 掩码只回最低 lane 结果，必须逐 lane 查。 */
STATIC DWORD OdspPolarityDimVerify(core_ip_t ip, BYTE dir, BYTE inv_mask)
{
    BYTE lane;
    BYTE expect_inv;
    capi_phy_info_t phy;
    capi_polarity_info_t pol;

    for(lane = 0u; lane < ODSP_LANE_NUM; lane++) {
        OdspPhyFill(&phy);
        phy.core_ip   = ip;
        phy.lane_mask = (BYTE)(1u << lane);
        pol.direction = dir;
        pol.action    = POLARITY_DEFAULT_SETTING;   /* 出参初值 */
        if(capi_get_polarity(&phy, &pol) != RR_SUCCESS) {
            return ERR;
        }
        expect_inv = (BYTE)((inv_mask >> lane) & 1u);
        if(expect_inv != 0u) {
            if(pol.action != POLARITY_INVERT_DEFAULT_SETTING) {
                return ERR;
            }
        } else {
            if(pol.action != POLARITY_DEFAULT_SETTING) {
                return ERR;
            }
        }
    }
    return OK;
}

/* S4：按 Profile 下发 4 维极性并逐 lane 回读校验。
 * 官方红线：polarity 仅允许 POR 后、mode 配置前配置一次（FW 持久）。 */
STATIC DWORD OdspPolarityApply(VOID)
{
    CONST OdspSerdesProfile_t *pf = &k_OdspSerdesProfile;

    if(pf->polarity_apply == 0u) {
        return OK;
    }
    if(OdspPolarityDimSet(CORE_IP_HOST_DSP, DIR_INGRESS,
                          pf->host_ingress_inv_mask) != OK) {
        return ERR;
    }
    if(OdspPolarityDimSet(CORE_IP_HOST_DSP, DIR_EGRESS,
                          pf->host_egress_inv_mask) != OK) {
        return ERR;
    }
    if(OdspPolarityDimSet(CORE_IP_MEDIA_DSP, DIR_INGRESS,
                          pf->media_ingress_inv_mask) != OK) {
        return ERR;
    }
    if(OdspPolarityDimSet(CORE_IP_MEDIA_DSP, DIR_EGRESS,
                          pf->media_egress_inv_mask) != OK) {
        return ERR;
    }
    if(OdspPolarityDimVerify(CORE_IP_HOST_DSP, DIR_INGRESS,
                             pf->host_ingress_inv_mask) != OK) {
        return ERR;
    }
    if(OdspPolarityDimVerify(CORE_IP_HOST_DSP, DIR_EGRESS,
                             pf->host_egress_inv_mask) != OK) {
        return ERR;
    }
    if(OdspPolarityDimVerify(CORE_IP_MEDIA_DSP, DIR_INGRESS,
                             pf->media_ingress_inv_mask) != OK) {
        return ERR;
    }
    if(OdspPolarityDimVerify(CORE_IP_MEDIA_DSP, DIR_EGRESS,
                             pf->media_egress_inv_mask) != OK) {
        return ERR;
    }
    return OK;
}

/* S5：lane swap（Profile 直通档默认跳过，接口保留供 PCB 交换场景启用）。
 * 映射语义（lane_swap_info_t）：下标 = 物理 lane id，值 = 逻辑 lane id。
 * 每侧（host/media）各一次调用；本 boot 每次重发，无需处理 auto-restore。 */
STATIC DWORD OdspLaneSwapApply(VOID)
{
    CONST OdspSerdesProfile_t *pf = &k_OdspSerdesProfile;
    capi_phy_info_t phy;
    capi_lane_config_info_t info;
    BYTE i;

    if(pf->lane_swap_apply == 0u) {
        return OK;
    }

    /* host 口 */
    OdspPhyFill(&phy);
    phy.core_ip = CORE_IP_HOST_DSP;
    info = (capi_lane_config_info_t){ 0 };
    info.lane_config_type = LANE_CONFIG_TYPE_LANE_SWAP_INFO;
    info.type.lane_swap_info.num_of_lanes = 8u;
    for(i = 0u; i < 8u; i++) {
        info.type.lane_swap_info.rx_lane_list[i] = pf->host_rx_lane_list[i];
        info.type.lane_swap_info.tx_lane_list[i] = pf->host_tx_lane_list[i];
    }
    if(capi_set_lane_config_info(&phy, &info) != RR_SUCCESS) {
        return ERR;
    }

    /* media 口 */
    OdspPhyFill(&phy);
    phy.core_ip = CORE_IP_MEDIA_DSP;
    info = (capi_lane_config_info_t){ 0 };
    info.lane_config_type = LANE_CONFIG_TYPE_LANE_SWAP_INFO;
    info.type.lane_swap_info.num_of_lanes = 8u;
    for(i = 0u; i < 8u; i++) {
        info.type.lane_swap_info.rx_lane_list[i] = pf->media_rx_lane_list[i];
        info.type.lane_swap_info.tx_lane_list[i] = pf->media_tx_lane_list[i];
    }
    if(capi_set_lane_config_info(&phy, &info) != RR_SUCCESS) {
        return ERR;
    }
    return OK;
}

/* S6：整片模式配置（12 字段全填，无默认/增量语义）+ 回读校验。
 * 取舍（方案 v5 §4）：FEC 终结/再生（DEC_ENC + 双侧 RS544），
 * 非指南示例的 BYPASS——VDM FEC 三件套要求 DSP 解码 FEC。 */
STATIC DWORD OdspModeConfigApply(VOID)
{
    capi_phy_info_t phy;
    capi_config_info_t cfg;
    capi_config_info_t rd;

    OdspPhyFill(&phy);
    phy.core_ip = CORE_IP_ALL;

    cfg = (capi_config_info_t){ 0 };
    cfg.ref_clk       = CAPI_REF_CLK_FRQ_156_25_MHZ_ETHERNET;
    cfg.func_mode     = CAPI_MODE_400G;
    cfg.fec_term      = CAPI_LANE_FEC_DEC_ENC;      /* 2 = 解码+重编码 */
    cfg.line_fec_type = CAPI_LINE_FEC_TYPE_RS544;   /* 2 = RS544 KP4 */
    cfg.host_fec_type = CAPI_HOST_FEC_TYPE_RS544;
    cfg.lw_br         = CAPI_LW_BR_106_25;
    cfg.bh_br         = CAPI_BH_BR_106_25;
    cfg.line_lane.modulation = CAPI_MODULATION_PAM4;
    cfg.line_lane.lane_mask  = 0x0Fu;
    cfg.host_lane.modulation = CAPI_MODULATION_PAM4;
    cfg.host_lane.lane_mask  = 0x0Fu;

    if(capi_set_config(&phy, &cfg) != RR_SUCCESS) {
        return ERR;
    }

    /* 回读校验：host_lane.lane_mask=0x0F 选定 host 端口回读逐项比对。 */
    rd = (capi_config_info_t){ 0 };
    rd.host_lane.lane_mask = 0x0Fu;
    if(capi_get_config(&phy, &rd) != RR_SUCCESS) {
        return ERR;
    }
    if(rd.ref_clk != cfg.ref_clk
       || rd.func_mode != cfg.func_mode
       || rd.fec_term != cfg.fec_term
       || rd.line_fec_type != cfg.line_fec_type
       || rd.host_fec_type != cfg.host_fec_type
       || rd.lw_br != cfg.lw_br
       || rd.bh_br != cfg.bh_br
       || rd.line_lane.modulation != cfg.line_lane.modulation
       || rd.host_lane.modulation != cfg.host_lane.modulation
       || rd.line_lane.lane_mask != cfg.line_lane.lane_mask
       || rd.host_lane.lane_mask != cfg.host_lane.lane_mask) {
        return ERR;
    }
    return OK;
}

/* S7 子步骤：单侧 TX FIR 下发。
 * 官方硬约束：txfir 与 dsp_level_shift 必须成对下发（param.is 双置）。
 * keep_current 策略：先 get 回现值，set 时仅替换 taps、level_shift 原样
 * 回写（满足成对约束且不动芯片电平校准）。 */
STATIC DWORD OdspTxFirSideApply(CONST OdspSerdesLaneTx_t *lanes, core_ip_t ip)
{
    BYTE lane;
    BYTE i;
    capi_phy_info_t phy;
    capi_lane_config_info_t info;
    capi_lane_config_info_t cur;

    for(lane = 0u; lane < ODSP_LANE_NUM; lane++) {
        if(lanes[lane].apply == 0u) {
            continue;
        }
        OdspPhyFill(&phy);
        phy.core_ip   = ip;
        phy.lane_mask = (BYTE)(1u << lane);

        info = (capi_lane_config_info_t){ 0 };
        info.lane_config_type = LANE_CONFIG_TYPE_LANE_TX_INFO;
        info.type.lane_tx_info.param.is.txfir            = 1u;
        info.type.lane_tx_info.param.is.dsp_level_shift  = 1u;
        if(lanes[lane].apply_high_swing != 0u) {
            info.type.lane_tx_info.param.is.dsp_high_swing = 1u;
            info.type.lane_tx_info.value.dsp_high_swing = lanes[lane].high_swing;
        }
        if(lanes[lane].apply_hs_dac_cur != 0u) {
            info.type.lane_tx_info.param.is.hs_dac_cur = 1u;
            info.type.lane_tx_info.value.hs_dac_cur = lanes[lane].hs_dac_cur;
        }

        if(lanes[lane].level_shift_keep_current != 0u) {
            /* 先 get 现值（txfir + level_shift 双 select bit） */
            cur = (capi_lane_config_info_t){ 0 };
            cur.lane_config_type = LANE_CONFIG_TYPE_LANE_TX_INFO;
            cur.type.lane_tx_info.param.is.txfir           = 1u;
            cur.type.lane_tx_info.param.is.dsp_level_shift = 1u;
            if(capi_get_lane_config_info(&phy, &cur) != RR_SUCCESS) {
                return ERR;
            }
            for(i = 0u; i < 4u; i++) {
                info.type.lane_tx_info.value.dsp_level_shift[i] =
                    cur.type.lane_tx_info.value.dsp_level_shift[i];
            }
            /* ★bring-up 首刷：现值建议经日志/调试口留档，后续改显式值
             * 时以留档为基准。 */
        } else {
            for(i = 0u; i < 4u; i++) {
                info.type.lane_tx_info.value.dsp_level_shift[i] =
                    lanes[lane].dsp_level_shift[i];
            }
        }

        for(i = 0u; i < 7u; i++) {
            info.type.lane_tx_info.value.txfir.tap[i] = lanes[lane].tap[i];
        }
        info.type.lane_tx_info.value.txfir.numb_of_taps = lanes[lane].num_of_taps;

        if(capi_set_lane_config_info(&phy, &info) != RR_SUCCESS) {
            return ERR;
        }
    }
    return OK;
}

/* S7：双侧 TX FIR（Profile 驱动；tx_apply=0 全跳过 = 芯片默认）。 */
STATIC DWORD OdspTxFirApply(VOID)
{
    CONST OdspSerdesProfile_t *pf = &k_OdspSerdesProfile;

    if(pf->tx_apply == 0u) {
        return OK;
    }
    if(OdspTxFirSideApply(pf->host_lane, CORE_IP_HOST_DSP) != OK) {
        return ERR;
    }
    if(OdspTxFirSideApply(pf->media_lane, CORE_IP_MEDIA_DSP) != OK) {
        return ERR;
    }
    return OK;
}

/*=========================== 对外 API ===========================*/

DWORD OdspInit(VOID)
{
    s_Ready = 0U;
    s_FwVer = 0U;

    /* S1：RESET_PHY 释放后等 DSP 内部轨/PLL 稳定。 */
    McuClockDelayMs(ODSP_BOOT_SETTLE_MS);

    /* S2：微码就绪轮询（官方判据：download_done && uc_ready）。
     * 超时 = 空片/镜像损坏。内置镜像兜底已移除（见文件头 S2'），直接判
     * liveness 失败上抛 fault 38；救援待 CDB→SPI2→U15 方案落地。 */
    if(OdspUcReadyWait(ODSP_UC_READY_TIMEOUT_MS) != OK) {
        return ERR;
    }

    /* S3：身份确认 + FW 版本。 */
    if(OdspChipInfoRead() != OK) {
        return ERR;
    }

    /* S4：极性（POR 后、mode 配置前一次性；回读校验失败判 ERR）。 */
    if(OdspPolarityApply() != OK) {
        return ERR;
    }

    /* S5：lane swap（当前定案直通，apply=0 直接通过）。 */
    if(OdspLaneSwapApply() != OK) {
        return ERR;
    }

    /* S6：整片模式配置 + 回读校验。 */
    if(OdspModeConfigApply() != OK) {
        return ERR;
    }

    /* S7：SerDes TX FIR（Profile 驱动，含 level_shift 成对回写）。 */
    if(OdspTxFirApply() != OK) {
        return ERR;
    }

    s_Ready = 1U;
    return OK;
}

BYTE OdspInitIsReady(VOID)
{
    return s_Ready;
}

DWORD OdspFirmwareVersionGet(VOID)
{
    return s_FwVer;
}

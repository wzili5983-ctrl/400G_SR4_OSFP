/** @file odsp_init.h
 * @brief BCM87420 (U11, 400G SR4 Retimer) 上电初始化 API（方案 v5 §3）。
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * BCM87420 经 I2C0（DSP = 硬件 I2C0 总线，BUS_ID_DSP）由 BcmArm* 传输层
 * 读写；上电时序由 msm_pwr_seq 编排（PWRUP_STEP_DSP_RELEASE 释放
 * RESET_PHY 后 InitChip → InitDsp → OdspInit）。实现为官方 cAPI boot
 * 序列（无 AVS 分支）：S1 settle → S2 download_done&&uc_ready 轮询
 * （S2' 空片走 capi_download SRAM 内置镜像兜底）→ S3 chip_info →
 * S4 极性（odsp_serdes_profile.c 定案：host 双向 0xF / media 0）→
 * S5 lane swap（直通跳过）→ S6 capi_set_config（400G / DEC_ENC /
 * 双侧 RS544 / 106.25 双侧 PAM4 mask 0xF）→ S7 TX FIR（Profile 7-tap，
 * level_shift 成对回写）。SerDes 细调值全部来自 odsp_serdes_profile.c
 * 常量表——改表重编即生效，host 不可见。
 */
#ifndef ODSP_INIT_H
#define ODSP_INIT_H

#include "basedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* BCM87420 上电初始化（被 chipresource_impl.c InitDsp 调用）。
 * 返回 OK 成功；ERR 失败（liveness 不应答 / 配置失败），
 * 由 InitChip 上抛 → msm_pwr_seq 判 fault 38。
 * 前提：RESET_PHY 已由 msm_pwr_seq 的 PWRUP_STEP_DSP_RELEASE 释放；
 *      BcmGd32I2cInit() 已由 InitDsp 先行调用。 */
DWORD OdspInit(VOID);

/* 查询初始化是否成功完成（1=已完成；0=未完成/失败）。 */
BYTE OdspInitIsReady(VOID);

/* 微码/FW 版本（S3 capi_get_chip_info 实读：fw_major<<16 | fw_minor；
 * 0 = 未读取成功）。 */
DWORD OdspFirmwareVersionGet(VOID);

#ifdef __cplusplus
}
#endif

#endif

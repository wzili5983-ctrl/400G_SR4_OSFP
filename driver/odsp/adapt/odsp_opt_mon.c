/** @file odsp_opt_mon.c
 * @brief BCM87420 (ODSP) optical power monitor implementation.
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * 经 BSC 读 BCM87420 内部光功率 ADC 寄存器：
 *   - 寄存器地址当前为占位全 0（★待 Broadcom 提供光功率 ADC 寄存器映射，
 *     NDA 文档）。占位期 ReadChip 读全 0 地址会失败（不应答/NACK）或返回
 *     垃圾值，s_RawPwr 保持 0/上次值，属预期——占位宏填真实地址后采样
 *     自动生效，无需改代码。
 *   - 采样周期 1s（OdspAdaptTask 100ms × SAMPLE_DIV 10），与 GN1848
 *     TaskTxAdSamples 的 1s 采集周期对齐，标定期可同刻比对 DSP 数字读数
 *     与 TIA 模拟 RSSI。
 *
 * 影子发布接入点（标定公式定稿后启用，★）：
 *   - 11h:170-177 OpticalPowerRx1..4（CMIS 5.3 Table 8-89，U16 大端 0.1µW）；
 *   - monitor_impl 的 RxPwr cal 链（三段拟合），替换现有 MON_MSA_ID_RSSI0..3
 *     （TIA 模拟 RSSI）代理路径。DSP 数字读数与 TIA 模拟读数为冗余双路径，
 *     标定期互为基准，量产后可二选一或交叉校验。
 */
#include "odsp_opt_mon.h"
#include "chipresource.h"           /* ReadChip / CHIP_ID_DSP */

/* 100ms 轮询 × 10 拍 = 1s 采样周期（与 GN1848 1s 采集对齐）。 */
#define ODSP_OPT_MON_SAMPLE_DIV  10U

/* ★ 占位寄存器地址：待 Broadcom 提供光功率 ADC 寄存器映射。
 * 全 0 期 ReadChip 失败，s_RawPwr 保持 0。 */
#define ODSP_OPT_RX_PWR_CH0  0x00000000U  /* ★占位 */
#define ODSP_OPT_RX_PWR_CH1  0x00000000U  /* ★占位 */
#define ODSP_OPT_RX_PWR_CH2  0x00000000U  /* ★占位 */
#define ODSP_OPT_RX_PWR_CH3  0x00000000U  /* ★占位 */

/* 只读配置：4 lane 光功率 ADC 寄存器地址表。 */
STATIC CONST DWORD s_RxPwrReg[ODSP_OPT_MON_LANE_NUM] = {
    ODSP_OPT_RX_PWR_CH0, ODSP_OPT_RX_PWR_CH1, ODSP_OPT_RX_PWR_CH2, ODSP_OPT_RX_PWR_CH3
};

/* 可变状态：最近一次采样原始码（DWORD 对齐字读写原子，无需锁）。 */
STATIC DWORD s_RawPwr[ODSP_OPT_MON_LANE_NUM] = {0U, 0U, 0U, 0U};
STATIC BYTE s_DivCnt = 0U;

VOID OdspOptMonInit(VOID)
{
    BYTE i;
    s_DivCnt = 0U;
    for(i = 0U; i < ODSP_OPT_MON_LANE_NUM; ++i) {
        s_RawPwr[i] = 0U;
    }
}

VOID OdspOptMonPoll(VOID)
{
    BYTE i;
    if(++s_DivCnt < ODSP_OPT_MON_SAMPLE_DIV) {
        return;
    }
    s_DivCnt = 0U;
    for(i = 0U; i < ODSP_OPT_MON_LANE_NUM; ++i) {
        DWORD v;
        if(ReadChip(CHIP_ID_DSP, s_RxPwrReg[i], &v) == OK) {
            s_RawPwr[i] = v;
        }
        /* 读失败仅跳过该 lane，槽保持上次值（占位期全 0）。 */
    }
    /* ★ 标定后：写影子 11h:170-177 OpticalPowerRx1..4（U16 大端 0.1µW），
     * 并接入 monitor_impl RxPwr cal 链发布 perfvalue。 */
}

DWORD OdspOptMonRawGet(BYTE lane)
{
    if(lane >= ODSP_OPT_MON_LANE_NUM) {
        return 0U;
    }
    return s_RawPwr[lane];
}

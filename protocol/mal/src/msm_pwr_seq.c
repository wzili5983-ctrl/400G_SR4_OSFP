/***************************************************************************//**
 *   @file   msm_pwr_seq.c
 *   @brief  MSM 电源序列钩子实现：PwrUp/PwrDn 板级动作（主方案 A）。
 *   @author Firmware team
 *******************************************************************************
 * Copyright 2026 Terabitcom, Inc.
 *
 * 钩子契约（msm.h MsmPwrSeqHook_t）：OK=本段序列完成；非 OK 下拍重试。
 * 本文件实现为步进机，每拍（MSM 任务周期 10ms）推进一步；单段超时由 MSM
 * 判 fault（PWR_UP→33 / PWR_DN→34），本文件不自行计总超时。
 *
 * PwrUp 时序（2026-10-12/13 电源树定案，含定案① V1P8_EN）：
 *   PA8(5V_EN) → 5V 稳定 → PA9(V3P3_RAIL_EN) → PA11(V1P8_EN) →
 *   PA10(V3P3_PHY_EN) → PB11(CDR_LD_EN) → SY70300B OTP ~1.4ms →
 *   PG 确认（V1P8_PG 直读；PMIC CHIPNAME/FLT_RECORD* 尽力而为读清）→
 *   DSP 复位释放（RESET_PHY=IO_LOW）→ InitChip（DSP→DRV→TIA→PMIC，
 *   fail-fast；liveness 失败 = 定案③ → MsmFaultTrigger(38)）。
 * PwrDn 反序：PB11 → PA10 → PA11 → PA9 → PA8，收尾 RESET_PHY 恢复保持
 *  （IO_HIGH，与 boot 默认电平一致）。PB12 WDOG_RST 全程 HIGH 不动。
 *
 * 主方案 A（2026-10-13 用户定）：PwrUp 芯片 liveness 失败 → 模块进 Fault
 * （cause=38）→ FAULT 态复用本 Dn 钩子执行 PwrDn → 物理低功耗并停在
 * Fault，等 host 动作。
 ******************************************************************************/
#include "msm_pwr_seq.h"
#include "msm.h"              /* MsmPwrSeqHookSet / MsmFaultTrigger / MSM_FAULT_CHIP_INIT */
#include "gpioresource.h"     /* WriteGpio / ReadGpio / IO_HIGH / IO_LOW */
#include "chipresource.h"     /* InitChip / CHIP_ID_PMIC */
#include "pmic.h"             /* PmicRead / PMIC_REG_IO_CHIPNAME / PMIC_REG_FLT_RECORD* */

/* 每拍 = MSM 任务周期 10ms；步内停留拍数（延时余量，10ms/拍） */
#define PWR_SEQ_SETTLE_5V_TICKS   10U    /* Boost U2 软启动余量★量级待查 DS */
#define PWR_SEQ_OTP_TICKS          2U    /* SY70300B OTP ~1.4ms → 2 拍(20ms)余量 */
#define PWR_SEQ_DISCHG_TICKS       2U    /* 每级关断后放电/回摆余量 */

/*-------------------- PwrUp 序列步进机 --------------------*/
typedef enum {
    PWRUP_STEP_5V_ON = 0,      /* ① U2 Boost → VCC_5V（TIA VPD + DRV VCC5） */
    PWRUP_STEP_5V_SETTLE,      /*    Boost 软启动等待 */
    PWRUP_STEP_RAIL_ON,        /* ② U5 → 3.3V_Rail（TIA VCC + DRV VCC/VCCIO） */
    PWRUP_STEP_V1P8_ON,        /* ③ U6 → DSP 1.8V 域（定案①：正式纳入时序） */
    PWRUP_STEP_PHY_ON,         /* ④ U4 → 3.3V_PHY → U21 PVIN（DSP 核心轨总开关） */
    PWRUP_STEP_CDR_ON,         /* ⑤ PB11 → SY70300B EN，OTP 载入 */
    PWRUP_STEP_OTP_WAIT,       /*    OTP ~1.4ms 等待 */
    PWRUP_STEP_PG_CHECK,       /* ⑥ PG 确认：V1P8_PG 直读 + PMIC 读清 */
    PWRUP_STEP_DSP_RELEASE,    /* ⑦ DSP 复位释放 */
    PWRUP_STEP_CHIP_INIT,      /* ⑧ InitChip（liveness 失败 → fault 38） */
    PWRUP_STEP_DONE
} PwrUpStep_t;

STATIC PwrUpStep_t s_PwrUpStep  = PWRUP_STEP_5V_ON;
STATIC DWORD       s_PwrUpDwell = 0U;

/*-------------------- PwrDn 序列步进机（反序，FAULT 态复用） --------------------*/
typedef enum {
    PWRDN_STEP_CDR_OFF = 0,    /* ① PB11=0：PMIC EN 关断，DSP 核心轨先断 */
    PWRDN_STEP_CDR_SETTLE,     /*    放电等待 */
    PWRDN_STEP_PHY_OFF,        /* ② PA10=0：3.3V_PHY 关断（U21 输入断） */
    PWRDN_STEP_V1P8_OFF,       /* ③ PA11=0：DSP 1.8V 域关断（定案①） */
    PWRDN_STEP_RAIL_OFF,       /* ④ PA9=0：3.3V_Rail 关断（TIA/DRV 供电断） */
    PWRDN_STEP_5V_OFF,         /* ⑤ PA8=0：Boost 关断，VCC_5V 掉 */
    PWRDN_STEP_RST_ASSERT,     /* ⑥ DSP 复位恢复保持（与 boot 默认电平一致） */
    PWRDN_STEP_DONE
} PwrDnStep_t;

STATIC PwrDnStep_t s_PwrDnStep  = PWRDN_STEP_CDR_OFF;
STATIC DWORD       s_PwrDnDwell = 0U;

/*-------------------- PwrUp 钩子 --------------------*/
STATIC DWORD MsmPwrSeqUpHook(VOID)
{
    DWORD val;

    switch(s_PwrUpStep)
    {
    case PWRUP_STEP_5V_ON:
        if(WriteGpio(GPIO_ID_5V_EN, IO_HIGH) != OK) { break; }
        s_PwrUpStep++;
        break;

    case PWRUP_STEP_5V_SETTLE:
        if(++s_PwrUpDwell < PWR_SEQ_SETTLE_5V_TICKS) { break; }
        s_PwrUpDwell = 0U;
        s_PwrUpStep++;
        break;

    case PWRUP_STEP_RAIL_ON:
        if(WriteGpio(GPIO_ID_V3P3_RAIL_EN, IO_HIGH) != OK) { break; }
        s_PwrUpStep++;
        break;

    case PWRUP_STEP_V1P8_ON:
        if(WriteGpio(GPIO_ID_V1P8_EN, IO_HIGH) != OK) { break; }
        s_PwrUpStep++;
        break;

    case PWRUP_STEP_PHY_ON:
        if(WriteGpio(GPIO_ID_V3P3_PHY_EN, IO_HIGH) != OK) { break; }
        s_PwrUpStep++;
        break;

    case PWRUP_STEP_CDR_ON:
        if(WriteGpio(GPIO_ID_CDR_LD_EN, IO_HIGH) != OK) { break; }
        s_PwrUpStep++;
        break;

    case PWRUP_STEP_OTP_WAIT:
        if(++s_PwrUpDwell < PWR_SEQ_OTP_TICKS) { break; }
        s_PwrUpDwell = 0U;
        s_PwrUpStep++;
        break;

    case PWRUP_STEP_PG_CHECK:
        /* 硬确认点：1.8V PG 必须已拉高（定案①后成为有效确认点），不满足
         * 停步重试（MSM 判超时 fault 33）。PMIC 读为尽力而为：CHIPNAME
         * 探测 + FLT_RECORD* 读清（INT 悬空，故障只能走 I2C），失败不卡
         * 序列——PMIC 硬 liveness 归 CHIP_INIT 步的 InitPmic。 */
        if(ReadGpio(GPIO_ID_V1P8_PG, &val) != OK || val != IO_HIGH) { break; }
        (VOID)PmicRead(CHIP_ID_PMIC, PMIC_REG_IO_CHIPNAME, &val);
        (VOID)PmicRead(CHIP_ID_PMIC, PMIC_REG_FLT_RECORDTEMP, &val);
        (VOID)PmicRead(CHIP_ID_PMIC, PMIC_REG_FLT_RECORDBUCK1, &val);
        (VOID)PmicRead(CHIP_ID_PMIC, PMIC_REG_FLT_RECORDBUCK3, &val);
        (VOID)PmicRead(CHIP_ID_PMIC, PMIC_REG_FLT_RECORDBUCK2, &val);
        (VOID)PmicRead(CHIP_ID_PMIC, PMIC_REG_FLT_RECORDBUCK4, &val);
        s_PwrUpStep++;
        break;

    case PWRUP_STEP_DSP_RELEASE:
        /* DSP 复位释放：IO_LOW 释放（boot 默认电平 1=复位保持，极性以硬件
         * 实测为准★）。I2C_SEL_PHY(PA15) 选择电平待硬件定案，暂不写入；
         * LPMODE_PHY(PD6) 转发归 MSL 层，不在此处。 */
        if(WriteGpio(GPIO_ID_RESET_PHY, IO_LOW) != OK) { break; }
        s_PwrUpStep++;
        break;

    case PWRUP_STEP_CHIP_INIT:
        /* 芯片初始化段（fail-fast：DSP→DRV→TIA→PMIC，含 DRV/TIA 的
         * B4H/B6H 寄存器初始化页消费）。liveness 失败 = 定案③：
         * MsmFaultTrigger(38) → MSM 已进 FAULT（FAULT 态会跑 Dn 钩子），
         * 本钩子保持 ERR，不再被调。rsttype 无消费者传 0。 */
        if(InitChip(0U) != OK) {
            MsmFaultTrigger(MSM_FAULT_CHIP_INIT);
            break;
        }
        s_PwrUpStep++;
        break;

    case PWRUP_STEP_DONE:
        /* 收尾：复位本序列步进（下次 PwrUp 从头开始） */
        s_PwrUpStep  = PWRUP_STEP_5V_ON;
        s_PwrUpDwell = 0U;
        return OK;

    default:
        break;                  /* 未知步：停留，由 MSM 超时兜底 */
    }
    return ERR;
}

/*-------------------- PwrDn 钩子（PWR_DN 态与 FAULT 态复用，幂等） --------------------*/
STATIC DWORD MsmPwrSeqDnHook(VOID)
{
    switch(s_PwrDnStep)
    {
    case PWRDN_STEP_CDR_OFF:
        if(WriteGpio(GPIO_ID_CDR_LD_EN, IO_LOW) != OK) { break; }
        s_PwrDnStep++;
        break;

    case PWRDN_STEP_CDR_SETTLE:
        if(++s_PwrDnDwell < PWR_SEQ_DISCHG_TICKS) { break; }
        s_PwrDnDwell = 0U;
        s_PwrDnStep++;
        break;

    case PWRDN_STEP_PHY_OFF:
        if(WriteGpio(GPIO_ID_V3P3_PHY_EN, IO_LOW) != OK) { break; }
        s_PwrDnStep++;
        break;

    case PWRDN_STEP_V1P8_OFF:
        if(WriteGpio(GPIO_ID_V1P8_EN, IO_LOW) != OK) { break; }
        s_PwrDnStep++;
        break;

    case PWRDN_STEP_RAIL_OFF:
        if(WriteGpio(GPIO_ID_V3P3_RAIL_EN, IO_LOW) != OK) { break; }
        s_PwrDnStep++;
        break;

    case PWRDN_STEP_5V_OFF:
        if(WriteGpio(GPIO_ID_5V_EN, IO_LOW) != OK) { break; }
        s_PwrDnStep++;
        break;

    case PWRDN_STEP_RST_ASSERT:
        /* DSP 复位恢复保持（与上电 DSP_RELEASE 相反；此时轨已全断）。
         * 亦保证中途打断的 PwrUp 后重启时从复位态开始。 */
        if(WriteGpio(GPIO_ID_RESET_PHY, IO_HIGH) != OK) { break; }
        s_PwrDnStep++;
        break;

    case PWRDN_STEP_DONE:
        /* 收尾：复位两个序列的步进（PwrUp 可能被中途打断过——LowPwrS
         * 回摆 PWR_UP→PWR_DN 或 CHIP_INIT 失败进 FAULT，都经本 Dn 走完） */
        s_PwrDnStep  = PWRDN_STEP_CDR_OFF;
        s_PwrDnDwell = 0U;
        s_PwrUpStep  = PWRUP_STEP_5V_ON;
        s_PwrUpDwell = 0U;
        return OK;

    default:
        break;                  /* 未知步：停留，由 MSM 超时兜底 */
    }
    return ERR;
}

/*-------------------- 注册入口 --------------------*/
VOID MsmPwrSeqInit(VOID)
{
    /* 序列步进复位（幂等：重复调用只是从头开始） */
    s_PwrUpStep  = PWRUP_STEP_5V_ON;
    s_PwrUpDwell = 0U;
    s_PwrDnStep  = PWRDN_STEP_CDR_OFF;
    s_PwrDnDwell = 0U;

    MsmPwrSeqHookSet(MsmPwrSeqUpHook, MsmPwrSeqDnHook);
}

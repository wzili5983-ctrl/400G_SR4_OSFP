/***************************************************************************//**
 *   @file   mgmt_init.c
 *   @brief  MgmtInit（管理接口初始化）编排实现。
 *   @author Firmware team
 *******************************************************************************
 * Copyright 2026 Terabitcom, Inc.
 *
 * 实现 mgmt_init.h 声明的 MgiInit()：按依赖顺序编排 CMIS 5.3 MgmtInit 阶段
 * 的「管理侧」bring-up。与 msm.c / dpsm.c / monitor.c 分层：
 *   本文件只做编排（顺序 + 一次性的复位默认/复位源），不实现状态机、
 *   监视、通路语义——那些在各自模块。
 *
 * 高功率数据通路明确不在本阶段：电源轨（V3P3_PHY/1.8V/CDR 等）与
 * retimer/GN1848/TIA 初始化归 MSM 的 PwrUp 钩子（见 msm.h
 * MsmPwrSeqHookSet）。本阶段结束后模块停留在 ModuleLowPwr，可被 host
 * 管理、尚未开光。
 ******************************************************************************/
#include "mgmt_init.h"

#include "i2c_slave.h"        /* I2cSlaveInit / I2C_SLAVE_OWN_ADDR：MCI 从机 */
#include "cdb.h"              /* CdbInit：CDB 引擎复位（9Fh 命令-应答 + 固件下载） */
#include "data_transfer.h"    /* DtTabCtrlInit：RAL 页表上下文（默认 Page 00h） */
#include "flashresource.h"    /* ReadFlash / FLASH_ID_PARA_A：影子加载源 */
#include "sramresource.h"     /* WriteSram / SRAM_ID_PARAM：影子加载目的地 */
#include "memory_map.h"       /* MEM_PARAM_AREA_SIZE */
#include "adpat.h"            /* WriteSramProtocolMsaSpace：影子字节落位 */
#include "ral.h"              /* PAGE_ID_LOW / LOW_ADDR_MODULE_CONTROL / LOW_ADDR_MODULE_MASKS */
#include "msm.h"              /* MsmInit / MsmMgmtInitDone / MsmFaultTrigger */
#include "msm_pwr_seq.h"      /* MsmPwrSeqInit：电源序列钩子注册（PwrUp/PwrDn 板级动作） */
#include "dpsm.h"            /* DpsmInit */
#include "monitor.h"          /* MonInit */
#include "msl.h"              /* MslInit：M_INT / LPMODE 信号接线 */
#include "odsp_adapt.h"       /* OdspAdaptInit：ODSP 适配层（LPMODE 转发/OPRXLOS/LASI/光功率） */
#include "vdm.h"              /* VdmInit：VDM 页面（20h-2Fh）描述符/阈值/Freeze */
#include "gd32e511_512_rcu.h" /* RCU_FLAG_* / rcu_flag_get / rcu_all_reset_flag_clear */


/*================ 复位源识别（RCU_RSTSCK）================
 * 一次 boot 只读一次并立即清旗，避免复位标志跨复位累积。复位标志在
 * MCU 硬件复位后仍保留（由各自复位事件置位），MgmtInit 据此区分上电/
 * 引脚/软件/看门狗/低功耗复位。 */
STATIC MgiResetCause_t s_ResetCause = MGI_RESET_NONE;

STATIC VOID MgiResetCauseIdentify(VOID)
{
    /* 优先级：更基础/更早的复位源靠前（上电 > 引脚 > 看门狗 > 软件 >
     * 低功耗 > 其它）。★ 注意：GD32E5 上 NVIC_SystemReset（SYSRESETREQ，
     * 00h:26.3 SoftwareReset 通路）与 NRST 引脚的复位标志归属
     * （SWRSTF vs EPRSTF）须实机确认，此处按手册语义归类。 */
    if(rcu_flag_get(RCU_FLAG_PORRST) == SET) {
        s_ResetCause = MGI_RESET_POWER;
    } else if(rcu_flag_get(RCU_FLAG_EPRST) == SET) {
        s_ResetCause = MGI_RESET_PIN;
    } else if((rcu_flag_get(RCU_FLAG_FWDGTRST) == SET) ||
              (rcu_flag_get(RCU_FLAG_WWDGTRST) == SET)) {
        s_ResetCause = MGI_RESET_WATCHDOG;
    } else if(rcu_flag_get(RCU_FLAG_SWRST) == SET) {
        s_ResetCause = MGI_RESET_SOFTWARE;
    } else if(rcu_flag_get(RCU_FLAG_LPRST) == SET) {
        s_ResetCause = MGI_RESET_LOWPOWER;
    } else {
        s_ResetCause = MGI_RESET_OTHER;
    }

    /* 识别完立即清旗：避免标志跨复位累积误导下一次识别。 */
    rcu_all_reset_flag_clear();
}

/*================ management map 影子加载（Flash 主参数区 → SRAM [7]）================
 * Flash 主参数区（FLASH_ID_PARA_A, MEM_PARAM_MAIN_BASE 0x0806C000, 32KB）
 * 与 SRAM [7] 用户参数缓冲区（SRAM_ID_PARAM, 0x20005500, 32KB）一一对应，
 * 可整体原样拷贝——management map 影子（g_AdpatMgmtMapBuf，偏移 +512 起）
 * 落位后无需搬移。分块拷贝避免 32KB 栈缓冲。 */
#define MGI_PARAM_COPY_CHUNK   256U          /* 每次拷贝块（栈缓冲，勿过大） */

STATIC BYTE s_ParamLoadOk = 0U;

STATIC DWORD MgiParamLoad(VOID)
{
    BYTE  buf[MGI_PARAM_COPY_CHUNK];
    DWORD off;

    for(off = 0U; off < MEM_PARAM_AREA_SIZE; off += MGI_PARAM_COPY_CHUNK) {
        if(ReadFlash(FLASH_ID_PARA_A, off, buf, MGI_PARAM_COPY_CHUNK) != OK) {
            return ERR;
        }
        if(WriteSram(SRAM_ID_PARAM, off, buf, MGI_PARAM_COPY_CHUNK) != OK) {
            return ERR;
        }
    }
    return OK;
}

/*================ SRAM 常驻寄存器复位默认值落定 ================
 * ModuleControl / 中断掩码等寄存器 MEMT_SRAM（掉电即失、不落 NV），
 * 每次 boot 须重立 CMIS 复位默认值——不能依赖影子加载带回的 Flash
 * 镜像里那些字节（那只是参数区布局占位，非持久语义）。Flash 持久化
 * 的身份/配置（Page 00h 等 MEMT_FLASH）由 MgiParamLoad 载入，本函数
 * 不触碰。
 *
 * 表驱动：g_MsaSpaceDefaultSeedInSram 逐条 { page_id, offset, len, seed }
 * 描述要落定的影子字节与默认值；seed 指向 STATIC CONST 数据（ROM 常驻，
 * 不占 RAM）。加默认值只加表行，不动消费循环。
 *
 * 复位默认（CMIS 5.3）：
 *   - 00h:26 ModuleControl：bit6 LowPwrAllowRequestHW=1（允许 host 经
 *     LPMode 引脚控制低功耗）、bit4 LowPwrRequestSW=0、bit3
 *     SoftwareReset=0（WO/SC，读回恒 0）→ 0x40；
 *   - 00h:31-36 模块级中断掩码：复位默认全屏蔽（Mask 位=1 不产生 INT）
 *     → 0xFF（0x23 保留字节同置 0xFF，值与掩码无关、引擎按保留区忽略）；
 *   - 2Dh:128-143 VDM 中断掩码：同上默认全屏蔽 → 0xFF×16（CMIS 5.3
 *     §8.22 Table 8-179「VDMMasks 默认 1」）。
 * ModuleState（00h:3）由 MsmInit 的 MsmPublishState 发布（Resetting），
 * 本函数不重复写。 */
typedef struct {
    TabIndex_t  page_id;   /* 目标页 */
    BYTE        offset;    /* 页内偏移（0-127） */
    BYTE        len;       /* 默认值字节数（须 offset+len ≤ 128） */
    CONST BYTE *seed;      /* 默认值字节（STATIC CONST） */
} MsaSpaceDefaultSeed_t;

STATIC CONST BYTE s_SeedModuleControl   = 0x40U;   /* 0x1A: bit6 LowPwrAllowHW=1，其余 0 */
STATIC CONST BYTE s_SeedModuleMasks[6U] = {0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU};
STATIC CONST BYTE s_SeedVdmMasks[16U]   = {0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
                                           0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
                                           0xFFU, 0xFFU, 0xFFU, 0xFFU};

STATIC CONST MsaSpaceDefaultSeed_t g_MsaSpaceDefaultSeedInSram[] = {
    { PAGE_ID_LOW,   LOW_ADDR_MODULE_CONTROL, 1U,  &s_SeedModuleControl },
    { PAGE_ID_LOW,   LOW_ADDR_MODULE_MASKS,   6U,  s_SeedModuleMasks    },
    { PAGE_ID_2DH_0, 0U,                      16U, s_SeedVdmMasks       },
};
#define MGI_DEFAULT_SEED_NUM  (sizeof(g_MsaSpaceDefaultSeedInSram) / sizeof(g_MsaSpaceDefaultSeedInSram[0]))

STATIC VOID MgiMgmtDefaultSeed(VOID)
{
    DWORD i;
    for(i = 0U; i < MGI_DEFAULT_SEED_NUM; i++) {
        CONST MsaSpaceDefaultSeed_t *e = &g_MsaSpaceDefaultSeedInSram[i];
        (void)WriteSramProtocolMsaSpace(e->page_id, e->offset, e->seed, e->len);
    }
}

/*================ 致命故障自检 ================
 * MgmtInit 收尾前对「管理域自身的致命故障」做一次自检：现仅核影子加载
 * 结果（存储不可读 = 数据存储故障）。★ 完整自检（参数区校验和/损坏、
 * 程序存储完整性、关键寄存器回读）待参数管理与 monitor 接入后补充，
 * 挂在本函数里、MsmFaultTrigger 之前。 */
STATIC DWORD MgiFatalSelfCheck(VOID)
{
    if(s_ParamLoadOk == 0U) {
        return ERR;                                   /* 数据存储故障（00h:29 = 2） */
    }
    return OK;
}

/*================ MgmtInit 编排入口 ================ */
DWORD MgiInit(VOID)
{
    /* ① 复位源识别：最早、无依赖，供诊断记录。 */
    MgiResetCauseIdentify();

    /* ② MCI 就绪：I2C1 从机 0xA0（PB13/PB14），默认接线 DtFrameCb /
     *    DtReadSource / DtCorClear。 */
    I2cSlaveInit(I2C_SLAVE_OWN_ADDR);

    /* ②+ CDB 引擎就绪：复位引擎状态（清 busy/pending、发布 CdbStatus=IDLE、
     *    关前台 NACK hold-off）。须在 I2cSlaveInit 之后（CdbInit 关 hold-off
     *    会写 I2C1 寄存器）；影子槽此刻尚未从 Flash 装载，CdbStatus=IDLE
     *    (0x00) 随后被影子加载覆盖回 0x00，无影响。 */
    CdbInit();

    /* ③ MSL 信号接线：M_INT 初始模式（复位直连）+ LPMODE EXTI。 */
    MslInit();

    /* ④ RAL 页表上下文初始化（默认 Page 00h / Bank0）。 */
    DtTabCtrlInit();

    /* ⑤ management map 影子加载：Flash 主参数区 → SRAM [7] 镜像。 */
    s_ParamLoadOk = (MgiParamLoad() == OK) ? 1U : 0U;

    /* ⑥ SRAM 常驻控制位/掩码复位默认值落定（须在影子加载之后，覆盖
     *    掉参数区镜像里这些非持久字节的占位值）。 */
    MgiMgmtDefaultSeed();

    /* ⑦ DPSM 创建：默认 DP 表（AppSel 1 单 DP）。须在 MsmInit 之前
     *    （msm 任务会读 DpsmAllDeactivated()）。 */
    DpsmInit();

    /* ⑧ MSM 初始化：进复位域、注册 10ms 任务、发布 ModuleState=
     *    Resetting。 */
    MsmInit();

    /* ⑧+ 电源序列钩子注册：PwrUp/PwrDn 板级动作（电源轨/DSP/芯片初始化；
     *    主方案 A：PwrUp 芯片 liveness 失败 → Fault(38) → PwrDn → 停
     *    Fault）。须在 MsmInit 之后（钩子由 MSM 任务周期驱动）。 */
    MsmPwrSeqInit();

    /* ⑨ 监视任务启动：注册 300ms 周期任务。 */
    MonInit();

    /* ⑨+ ODSP 适配层启动：LPMODE 转发 / OPRXLOS 监视 / LASI 中断 / 光功率
     *    采样子模块初始化 + 100ms 周期任务注册（槽位 TASK_ID_ODSP_ADAPT）。
     *    须在 MonInit 之后（影子已加载，CMIS 影子可写）；BCM87420 上电初
     *    始化（OdspInit）不在此处——由 msm_pwr_seq 的 CHIP_INIT 步触发。 */
    OdspAdaptInit();

    /* ⑨++ VDM 初始化：描述符/阈值/掩码落影子 + SupportedPages bit6 置 1 +
     *     1s 周期任务注册（槽位 TASK_ID_VDM）。须在 OdspAdaptInit 之后
     *     （VDM 消费 telemetry getter）。 */
    VdmInit();

    /* ⑩ 致命故障自检：未通过 → 进 ModuleFault（数据存储故障），不通报
     *    管理就绪；MSM 的 2000ms 超时看门狗亦会兜底。 */
    if(MgiFatalSelfCheck() != OK) {
        MsmFaultTrigger(MSM_FAULT_DATA_MEM);          /* 00h:29 = 2：数据存储损坏 */
        return ERR;
    }

    /* ⑪ 通报 MSM「管理接口就绪」：MgmtInit → ModuleLowPwr。只允许一次。 */
    MsmMgmtInitDone();

    return OK;
}

MgiResetCause_t MgiResetCauseGet(VOID)
{
    return s_ResetCause;
}

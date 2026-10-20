/***************************************************************************/ /**
 *   @file   main.c
 *   @brief  Bootloader 主程序：启动决策（提升/跳转）→ 救援模式 I2C 从机环。
 *   @author Firmware team
 ********************************************************************************
 * Copyright 2026 Terabitcom, Inc.
 *
 * 固定 A / 镜像 B 方案 BL @ 0x08000000（P0-P3，32KB）：
 *   1) BlBootDecide：CONFIRMED 记录 → 提升 B→A（幂等、掉电重试）→ 深校验
 *      槽 A → 合法则 BlJumpToApp 跳转（0x08010200，不返回）；
 *   2) 槽 A 无效 → 救援模式：复位后无中断使能、无外设占用——清零 RAL
 *      影子 → Timer2 调度 → 页表 → CDB 引擎 → I2C1 从机 0xA0（PB13/PB14
 *      自配置）。环内：Timer2SchedulerRun + CdbExeBackground（CDB 命令
 *      前台单实例模型：CMDID 在 STPDET ISR 触发，Flash 擦写深校验在主
 *      循环执行）。BL 无 DEFERRED oprfunc，不跑 CmdExeBackground。
 *   3) 不启动 FWDGT（启动后不可关，会带进 APP 窗口期）：擦除期 EraseFlash
 *      WithFeed 的逐页喂狗在未启动看门狗上是安全 no-op 语义。
 *
 * 跳转前提（提升失败回退）：槽 A 镜像的深校验（VerifySlotImage：帧头
 * magic/目标槽/几何/头 CRC + 负载回读 CRC）通过才跳；否则停留救援环，
 * host 可经 CDB 9Fh 正常升级流程重灌槽 B。
 ******************************************************************************/
#include "basedef.h"
#include "bl_promote.h"
#include "gd32e511_512.h"   /* SystemCoreClockUpdate / SystemCoreClock */
#include "mcu_clock.h"      /* McuClockInit：SystemInit 120MHz → 80MHz（与 APP 同序） */
#include "data_transfer.h"  /* DtTabCtrlInit / DtTabCtrlSet */
#include "i2c_slave.h"      /* I2cSlaveInit / I2C_SLAVE_OWN_ADDR */
#include "cdb.h"            /* CdbInit / CdbExeBackground */
#include "timer2_scheduler.h" /* Timer2SchedulerInit / Timer2SchedulerRun */

int main(void)
{
    /* ---- 启动决策：提升 B→A / 跳转 APP（合法时，不返回） ---- */
    if(BlBootDecide() == BL_BOOT_JUMP_APP) {
        BlJumpToApp();
    }

    /* ---- 救援模式：I2C1 从机 0xA0 + CDB 9Fh 固件下载 ----
     * 时钟先降到 80MHz（Timer2Scheduler 分频按 80MHz 硬编码）；随后
     * 顺序敏感：影子清零 + 标识装载必须在 I2C 使能（host 可读）之前；
     * CdbInit 在页表之后（引擎状态归零）；I2C 最后使能（回调链齐备）。 */
    McuClockInit();
    BlShadowClear();
    BlIdentityLoad();               /* 00h:0-2 标识字节：Flash 参数区 → 影子 */
    Timer2SchedulerInit(80000000U, 1000U);
    DtTabCtrlInit();
    CdbInit();
    I2cSlaveInit(I2C_SLAVE_OWN_ADDR);

    for(;;) {
        Timer2SchedulerRun();       /* 1ms tick；CDB Run 的延时复位任务在此执行 */
        CdbExeBackground();         /* CDB 命令执行（Flash 擦写/CRC 深校验） */
    }
}

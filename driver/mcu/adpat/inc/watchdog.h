/** @file watchdog.h
 * @brief 独立看门狗（FWDGT）驱动声明。
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * 独立看门狗（FWDGT，GD32E511 的 IWDG）由 IRC40K（40kHz 内部 RC）驱动，
 * 一旦启动即无法关闭（复位前一直运行）。用途：主循环任一环节卡死超过
 * 设定的超时时间即复位 MCU；复位后 mgmt_init 的复位源识别（RCU_RSTSCK
 * 的 FWDGTRSTF）会报 MGI_RESET_WATCHDOG。
 *
 * 超时换算：超时(ms) = (reload+1) × 预分频 / 40，reload 为 12bit
 * （0-4095），预分频 ∈ {4,8,16,32,64,128,256}。调用方只需给 timeout_ms，
 * WatchdogInit 内部选最小预分频组合。IRC40K 精度有限（±容差），实际超时
 * 以实机为准。
 */
#ifndef WATCHDOG_H
#define WATCHDOG_H

#include "basedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 可设置的超时范围（ms）：最小约 0.1ms（分频 4、reload 0），最大 =
 * (4096 × 256) / 40 = 26214.4ms，取整 26214ms。越界由 WatchdogInit 夹取。 */
#define WATCHDOG_TIMEOUT_MIN_MS   1U
#define WATCHDOG_TIMEOUT_MAX_MS   26214U

/** 独立看门狗初始化：使能 IRC40K 并等待稳定，按 timeout_ms 配置超时并启动
 *  计数。此后无法关闭，只能在主循环每拍 WatchdogKick 喂狗防止超时复位。
 *  timeout_ms：期望的看门狗超时（毫秒），夹取到 [WATCHDOG_TIMEOUT_MIN_MS,
 *  WATCHDOG_TIMEOUT_MAX_MS]；内部选最小预分频换算（实际值受 IRC40K 容差）。
 *  在板级资源与管理接口初始化完成后、主循环开始前调用。 */
VOID WatchdogInit(DWORD timeout_ms);

/** 喂狗：重载计数器。主循环每拍调用一次。 */
VOID WatchdogKick(VOID);

#ifdef __cplusplus
}
#endif

#endif /* WATCHDOG_H */

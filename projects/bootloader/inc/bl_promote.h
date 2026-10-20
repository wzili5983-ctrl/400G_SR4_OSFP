/***************************************************************************/ /**
 *   @file   bl_promote.h
 *   @brief  Bootloader 启动决策：OTA 确认提升 B→A + 跳转 APP / 进入救援模式。
 *   @author Firmware team
 ********************************************************************************
 * Copyright 2026 Terabitcom, Inc.
 *
 * 固定 A / 镜像 B 方案：BL @ 0x08000000（P0-P3），APP A @ 0x08010000 为固定
 * 运行槽（payload 基址 +0x200 帧头区），APP B @ 0x0803E000 是唯一升级入口。
 * APP 确认升级（CDB 0109h Run → OtaConfirm 写 CONFIRMED 记录）后复位，BL 启动：
 *   1) 读到合法 CONFIRMED 记录 → 校验槽 B 镜像 → 提升（逐段拷贝 B→A：负载
 *      先行、512B 帧头最后）→ 校验槽 A → 重写 METADATA_A 记录为 IDLE
 *      （保留 update_cnt、重算 crc32，撕裂写守卫）；
 *   2) 槽 A 镜像深校验通过 → 直接跳转 APP（__set_MSP + VTOR + 复位向量）；
 *   3) 槽 A 无效 → 救援模式：I2C1 从机 0xA0 + CDB 9Fh 固件下载（host 可经
 *      正常升级流程重灌槽 B，下一轮 CONFIRMED 再提升）。
 * 掉电安全：提升中途掉电（记录仍 CONFIRMED）下次启动重试提升——负载先
 * 行/帧头最后的拷贝序保证槽 A 半途状态永远是「无头槽」而非新旧混杂。
 * 记录绑定：CONFIRMED 记录与槽 B 镜像按 crc32data/fwsize 比对，B 现内容
 * 不符（host 换入新镜像未 Run）即视为陈旧记录清回 IDLE——Run 激活门保持。
 ******************************************************************************/
#ifndef BL_PROMOTE_H
#define BL_PROMOTE_H

#include "basedef.h"

/* 启动决策结果 */
#define BL_BOOT_JUMP_APP   0U   /* 槽 A 镜像有效 → 跳转 APP */
#define BL_BOOT_RESCUE     1U   /* 槽 A 镜像无效 → I2C 救援模式 */

/* 启动决策：CONFIRMED 记录 → 提升 B→A（掉电重试幂等）；随后深校验槽 A。
 * 返回 BL_BOOT_*（本函数不跳转、不初始化任何外设）。 */
DWORD BlBootDecide(VOID);

/* 跳转 APP 槽 A（payload 基址 0x08010200）：关中断、置 VTOR 后，切栈+清
 * PRIMASK+取复位向量跳转经全寄存器内联汇编一次完成（切栈后 C 栈失效，
 * 不得经栈取跳转目标；PRIMASK 必须清零——函数指针跳转非硬件复位，且
 * APP 启动序列无重开点）。APP 重跑 Reset_Handler/SystemInit/__main（BL 与
 * APP 共享 0x2000D500 起的 RW/ZI 区，__main 重新做分散加载初始化）。不返回。 */
VOID BlJumpToApp(VOID);

/* 清零 RAL 影子区 g_AdpatMgmtMapBuf（BL 链接脚本不归零绝对定位 ZI；
 * 640B = PAGE_TAB_NUM×128）。须在 I2C 从机使能前调用。 */
VOID BlShadowClear(VOID);

/* 救援模式标识装载：00h:0-2（Identifier/CmisRevision/MgmtChar）从 Flash
 * 主参数区 Page Low 槽（+512）读入影子，host 枚举读到真实模块身份。
 * 须在 BlShadowClear 之后、I2C 从机使能前调用。 */
VOID BlIdentityLoad(VOID);

#endif /* BL_PROMOTE_H */

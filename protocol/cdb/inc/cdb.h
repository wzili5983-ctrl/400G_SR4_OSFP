/** @file cdb.h
 * @brief CMIS CDB (Command Data Block) 引擎：9Fh 消息页上的命令-应答交换，
 *        聚焦固件下载（0101h/0103h/0107h）与四条通用命令。
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * CDB 是 RAL 之上的命令协议：host 把 Command Header + Local Payload 写进
 * Page 9Fh:128-255，最后一次单独写 CMDID（128-129，两字节，STOP 落 129）触发；
 * 模块在同一页回填 Reply Header（RPLLength/RPLChkCode）+ Reply Payload，并把
 * 执行状态发布到 Lower Memory CdbStatus1（00h:37）与 CdbCmdCompleteFlag1
 * （00h:8.6）。
 *
 * 本实现只支持前台（foreground）单实例模式：
 *   - 单 CDB 实例绑定当前 9Fh bank（host 用 bank0），一次一条命令；
 *   - CMDID 触发在 ISR 内立即置 busy + In Progress（oprfunc OPF_IMMEDIATE），
 *     否则 host 的 Acknowledge Polling 会读到旧状态误判完成；
 *   - 真正的命令执行（Flash 擦写、CRC 深校验）下沉主循环 CdbExeBackground()；
 *   - 前台 hold-off：busy 期间模块对一切 host 事务 NACK（I2cSlaveHoldOff），
 *     host 反复重试，直到 busy 清除读回最终 CdbStatus。
 *
 * 命令集（暂定稿）：
 *   0000h Query / 0004h Abort / 0040h Module Features / 0041h Firmware
 *   Management Features（通用）；0101h Start / 0103h Write Block / 0107h
 *   Complete（固件下载核心）；0109h Run（确认落盘 + 延时复位，拒 Hitless）
 *   / 010Ah Commit（本方案已收敛入 Run，直接 SUCCESS）。
 *
 * 固件下载传输约定（与 pack_framed_bin.py 产物对齐）：
 *   - 镜像 = 负载（fwsize 字节，%4==0）；512B 帧头区是元数据，经 0101h
 *     VendorData[112]（= FrameHead_t）携带、0107h 由模块重建写入；
 *   - ImageSize = fwsize（同时容忍 fwsize+0x200 的整帧口径），StartCmdPayloadSize=0，
 *     BlockAddress = 负载字节偏移（0 基）；
 *   - 0107h 先补写 512B 帧头区（帧头最后写，掉电安全序）再 VerifyFramedImage 深校验；
 *   - 尾块按 ⌈len/8⌉·8 垫 0xFF 双字写（擦除区写 0xFF 是 no-op）。
 */
#ifndef CDB_H
#define CDB_H

#include "basedef.h"

/* CdbInit：MgmtInit 内、I2cSlaveInit 之后调用。复位引擎、清 busy/状态。 */
VOID  CdbInit(VOID);
/* CdbTrigger：9Fh CMDID oprfunc（OPF_IMMEDIATE，I2C STPDET ISR）入口。
 * cmd = 2 字节 CMDID（大端），len 必须==2。置 busy + In Progress + pending，
 * 并使能前台 NACK hold-off。返回 OK/ERR。 */
DWORD CdbTrigger(CONST BYTE *cmd, BYTE len);
/* CdbExeBackground：主循环每轮调用（与 data_transfer 的 CmdExeBackground
 * 对应）。执行 pending 命令（ChkCode 校验 + Flash 操作），完成后清 busy、
 * 关 hold-off、发布最终 CdbStatus + Completion Flag。 */
VOID  CdbExeBackground(VOID);
/* CdbBusy：非 0 = 前台命令执行中（hold-off 生效）。供 I2C 从机 hold-off 门查询。 */
BYTE  CdbBusy(VOID);
/* DSP_FW 升级会话有效：U15 已/将被擦写，CDB 独占 RESET_PHY/SPI2；
 * ODSP 周期任务须暂停 DSP 访问，MSM 不得重新执行 DSP 上电序列。
 * 一旦置位保持到 MCU 复位；Abort 不释放，以免把不完整镜像当作可运行镜像。 */
BYTE  CdbDspUpgradeActive(VOID);

#endif /* CDB_H */

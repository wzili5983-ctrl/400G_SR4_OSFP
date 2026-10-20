/** @file mci.h
 * @brief CMIS MCI (Comms for transferring ACCESS primitives): I2C1 on
 *        PB13 (SCL) / PB14 (SDA), host command receiver.
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * Host -> device entry: the upper computer reaches this device as an I2C
 * slave at the 7-bit own address I2C_SLAVE_OWN_ADDR (0xA0). Receive is
 * interrupt driven; the EV / ER handlers use the CMSIS startup vector
 * names (I2C1_EV_IRQHandler / I2C1_ER_IRQHandler) with NVIC configured
 * through the CMSIS core API.
 *
 * Frame model: bytes between address-match and STOP land in the staging
 * buffer g_RxBuf -- slot I2C_RX_ADDR holds the first byte (window
 * address), the bytes that follow it are data, their count is stored in
 * slot I2C_RX_LEN. At STOP the registered frame callback fires in ISR
 * context (default DtFrameCb from data_transfer.c, wired in
 * I2cSlaveInit); heavy consumer work is deferred to the main loop via
 * the data_transfer deferred FIFO, not drained from a ring here.
 *
 * The host read direction is served by a registered read source (see
 * I2cSlaveRegisterReadSource): at the read-phase address match the
 * callback snapshots 128 bytes from the register file, and the TI
 * interrupt streams them with 128-byte wrap. Until a source is
 * registered, host reads return dummy 0xFF bytes.
 */
#ifndef I2C_SLAVE_H
#define I2C_SLAVE_H

#include "basedef.h"

/* Own 7-bit slave address per the board design (host entry). */
#define I2C_SLAVE_OWN_ADDR      0xA0U
#define MEM_ADD_INC(x, y)	(((x) & 0x80) | (((x) + (y)) & 0x7f))
typedef enum {
   I2C_STATE_IDLE = 0,
   I2C_STATE_RXADDR,
   I2C_STATE_RXDATA,
   I2C_STATE_TXDATA,
   I2C_STATE_NUM	
} I2C_STATE_ENUM;

typedef enum {
   I2C_RX_LEN = 0,    /* g_RxBuf 槽位：I2C_STATE_RXADDR 之后收到的数据长度 */
   I2C_RX_ADDR = 2,   /* g_RxBuf 槽位：帧首字节（窗口地址） */
   I2C_RX_DATA,       /* g_RxBuf 槽位：数据区起点 */
   I2C_RX_INDEX_NUM
} I2C_RX_BUFF_INDEX;

/* Start the I2C1 slave receiver: PB13 = SCL, PB14 = SDA, AF open drain,
 * 7-bit address addr7, SCL stretch backpressure, interrupts on.
 * addr7 must be passed already shifted-free (e.g. I2C_SLAVE_OWN_ADDR). */
VOID  I2cSlaveInit(BYTE addr7);
/* Stop the slave receiver and release the I2C1 peripheral. */
VOID  I2cSlaveDeinit(VOID);

/* Frames completed so far (one STOP = one frame). Monotonic counter. */
DWORD I2cSlaveFrameCount(VOID);
/* Overrun / bus-error / arbitration-loss count. Monotonic counter. */
DWORD I2cSlaveErrorCount(VOID);

/* 当前 Management Memory Map 绝对地址(00h-FFh)：写相位首字节更新，
 * 读相位按 128B 半区回卷自增，跨帧保持。供 data_transfer 的 DtParse
 * 解析当前窗口位置。 */
BYTE I2cSlaveGetMgmtMemAddr(VOID);

/* STPDET 帧回调上下文内有效：返回当前帧暂存缓冲 g_RxBuf（[I2C_RX_LEN]=数据长度、
 * [I2C_RX_ADDR]=窗口地址、[I2C_RX_DATA+]=数据字节）。仅供 data_transfer 的
 * 帧回调（DtFrameCb）在回调内读取；回调返回后内容即被下一帧覆盖，勿跨帧持有。 */
BYTE *I2cSlaveGetRxBuf(VOID);

/* Frame-complete hook: runs in the I2C1 EV ISR at STOP detection with
 * the frame length in bytes; keep it short (set a flag only). NULL = off.
 * I2cSlaveInit wires DtFrameCb here by default; register again to
 * override. Frame data stays in g_RxBuf and is only valid during the
 * callback (see I2cSlaveGetRxBuf). */
VOID  I2cSlaveRegisterFrameCb(VOID (*callback)(DWORD frame_bytes));

/* Host-read data source: runs in the I2C1 EV ISR at the read-phase
 * address match; the callback must fill buf with len bytes starting at
 * register-window address memaddr (caller streams buf with 128-byte
 * wrap). Register the RAL read primitive here. NULL = dummy 0xFF. */
VOID  I2cSlaveRegisterReadSource(VOID (*callback)(BYTE memaddr, BYTE *buf, DWORD len));

/* Host-read complete hook: runs in the I2C1 EV ISR at STOP detection,
 * only when the transaction included a read phase. memaddr = read-phase
 * start window address (recorded at address match, before TI
 * auto-increment); rdlen = bytes actually read, saturated to 128
 * (g_TxIdx wraps mod 128: 0 means a full 128-byte half-page or more).
 * Keep it short. I2cSlaveInit wires DtCorClear (COR read-clear) here by
 * default; register again to override. */
VOID  I2cSlaveRegisterReadDone(VOID (*callback)(BYTE memaddr, BYTE rdlen));

/* 前台 hold-off 门（CMIS CDB foreground 模式）：on=1 → 模块对一切 host
 * 事务 NACK（I2C_CTL1.NACKEN），host Acknowledge Polling 反复重试直到
 * on=0。由 cdb.c 在命令执行期间置位/清除；CDB 执行期间 host 读写都被拒，
 * 执行完成后读回最终 CdbStatus。 */
VOID  I2cSlaveHoldOff(BYTE on);

#endif

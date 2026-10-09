/***************************************************************************//**
 *   @file   i2c_slave.c
 *   @brief  CMIS MCI (Comms for transferring ACCESS primitives): I2C1
 *            slave receiver for the host link (PB13 SCL / PB14 SDA).
 *   @author Firmware team
 *******************************************************************************
 * Copyright 2026 Terabitcom, Inc.
 *
 * MCI = CMIS 最底层通信接口，负责在 host 与模块间搬运 ACCESS 原语
 * （READ / WRITE / TEST 的 256B 窗口事务）。本文件实现 MCI 的 I2C
 * 传输侧：Interrupt-driven I2C slave on I2C1, fixed to the host-entry pins:
 * PB13 = I2C1_SCL, PB14 = I2C1_SDA, alternate function 4, open drain
 * (bus pull-ups on the board). Own address 0xA0, 7-bit mode.
 *
 * Receive path: address match (ADDSEND) opens a frame into the staging
 * buffer g_RxBuf: byte 0 = I2C_STATE_RXADDR 之后收到的数据长度，
 * byte 2 = window address, byte 3.. = data. STOP detection (STPDET)
 * closes the frame and invokes the registered frame callback
 * (g_FrameCb, default DtFrameCb from data_transfer.c) in ISR context;
 * the consumer side defers heavy work via the data_transfer deferred
 * FIFO instead of a thread-context drain ring.
 *
 * Transmit path: at the host read-phase address match the registered
 * read source (I2cSlaveRegisterReadSource) snapshots 128 bytes from
 * the register file; the TI interrupt streams them with 128-byte wrap.
 * Without a source, 0xFF dummy bytes keep the bus moving.
 *
 * SCL clock stretching is enabled, so a late-draining host is throttled
 * by hardware; remaining overruns / bus errors are counted.
 ******************************************************************************/
#include "i2c_slave.h"
#include "data_transfer.h"  /* DtFrameCb：I2cSlaveInit 内注册的默认帧回调 */
#include "gd32e511_512.h"
#include "gd32e511_512_i2c.h"
#include "gd32e511_512_rcu.h"
#include "gd32e511_512_gpio.h"

#define I2C_SLAVE_RX_BUF_SIZE   256U                    /* frame staging buffer */
#define I2C_SLAVE_DUMMY_BYTE    0xFFU                   /* host-read filler */

STATIC volatile BYTE g_I2cState = I2C_STATE_IDLE;
STATIC volatile BYTE g_MgmtMemAddr = 0U;                /* Management Memory Map 绝对地址(00h-FFh)，写相位首字节更新，读相位按 128B 半区回卷自增，跨帧保持 */
STATIC volatile BYTE g_RxData = 0U;

STATIC BYTE  g_RxBuf[I2C_SLAVE_RX_BUF_SIZE];            /* frame staging */

STATIC volatile DWORD g_FrameBytes;                     /* bytes of the frame in progress */
STATIC volatile DWORD g_FrameCnt;                       /* completed frames */

/* 错误/异常计数：ER ISR（NACK/OUERR → over，BERR/LOSTARB → err），
 * 单调递增不清零，I2cSlaveErrorCount() 返回 err + over。 */
typedef struct {
    volatile DWORD err;                     /* bus error / arbitration loss */
    volatile DWORD over;                    /* NACK / OUERR */
} I2cSlaveErrCnt_t;
STATIC I2cSlaveErrCnt_t g_ErrCnt;
STATIC VOID (*g_FrameCb)(DWORD frame_bytes);

/* Host-read snapshot: filled by the registered read source at the
 * read-phase address match, streamed by the TI interrupt. */
#define I2C_SLAVE_TX_BUF_SIZE   128U
STATIC BYTE  g_TxBuf[I2C_SLAVE_TX_BUF_SIZE];
STATIC BYTE  g_TxIdx;
STATIC VOID (*g_ReadSrcCb)(BYTE memaddr, BYTE *buf, DWORD len);
STATIC VOID (*g_ReadDoneCb)(BYTE memaddr, BYTE rdlen);
STATIC BYTE  s_TxStartAddr;             /* 读相位起始窗口地址：ADDSEND+TR 记录
                                         * （TI 自增前），STPDET 清 COR 用 */

VOID I2cSlaveInit(BYTE addr7)
{
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_I2C1);

    /* PB13 = I2C1_SCL, PB14 = I2C1_SDA: AF4 open drain (AF number per
     * the GD32E511 pin multiplexing table, same AF as the I2C0 wiring). */
    gpio_af_set(GPIOB, GPIO_AF_4, GPIO_PIN_13 | GPIO_PIN_14);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_13 | GPIO_PIN_14);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_2MHZ, GPIO_PIN_13 | GPIO_PIN_14);

    i2c_deinit(I2C1);
    rcu_i2c_clock_config(I2C1, RCU_I2CSRC_IRC8M);
    i2c_timing_config(I2C1, 0U, 9U, 2U);
    i2c_slave_response_to_gcall_disable(I2C1);
    /* Stretch SCL while the previous byte is still unread: hardware
     * backpressure when the app drains the ring buffer late. */
    i2c_stretch_scl_low_enable(I2C1);
    /* The library stores the address in SADDR0.ADDRESS[7:1]: pass addr7
     * pre-shifted; 7-bit address format. */
    i2c_address_config(I2C1, (DWORD)addr7 << 1, I2C_ADDFORMAT_7BITS);

    g_I2cState = I2C_STATE_IDLE;
    g_MgmtMemAddr = 0U;
    g_RxBuf[I2C_RX_LEN] = 0U;
    g_FrameBytes = 0U;
    g_FrameCnt = 0U;
    g_ErrCnt.err = 0U;
    g_ErrCnt.over = 0U;
    g_FrameCb = NULL;
    g_TxIdx = 0U;
    g_ReadSrcCb = NULL;
    g_ReadDoneCb = NULL;

    /* 默认帧回调：指令解析（DtParse + oprfunc 分发）挂在 STPDET 路径。
     * 注册须在 i2c_enable 之前完成，保证首帧到达时回调已就位；
     * 应用可用 I2cSlaveRegisterFrameCb 另行覆盖。 */
    I2cSlaveRegisterFrameCb(DtFrameCb);

    /* 默认读数据源：读相位页快照（Lower→Page Low 影子槽，Upper→g_CurPageId
     * 影子槽，未登记页填 0xFF dummy）。同样须在 i2c_enable 前就位；
     * 应用可用 I2cSlaveRegisterReadSource 另行覆盖。 */
    I2cSlaveRegisterReadSource(DtReadSource);

    /* 默认读完成回调：COR（Clear-On-Read）寄存器读后清除——STPDET 且本
     * 事务含读相位时，按「读起始地址 + 实际读长度」清影子（DtCorClear）。
     * 须在 i2c_enable 前就位；应用可用 I2cSlaveRegisterReadDone 另行覆盖。 */
    I2cSlaveRegisterReadDone(DtCorClear);

    /* ADDM: address match, RBNE: rx byte pending, TI: feed read data,
     * STPDET: frame end, ERR: overrun / bus errors. */
    i2c_interrupt_enable(I2C1, I2C_INT_ERR | I2C_INT_ADDM | I2C_INT_RBNE | I2C_INT_TI | I2C_INT_STPDET);
    /* Priorities below SysTick (0) and the Timer2 scheduler (1). */
    NVIC_SetPriority(I2C1_EV_IRQn, 2U);
    NVIC_SetPriority(I2C1_ER_IRQn, 3U);
    NVIC_EnableIRQ(I2C1_EV_IRQn);
    NVIC_EnableIRQ(I2C1_ER_IRQn);

    i2c_enable(I2C1);
}

VOID I2cSlaveDeinit(VOID)
{
    NVIC_DisableIRQ(I2C1_ER_IRQn);
    NVIC_DisableIRQ(I2C1_EV_IRQn);
    i2c_interrupt_disable(I2C1, I2C_INT_ERR | I2C_INT_ADDM | I2C_INT_RBNE | I2C_INT_TI | I2C_INT_STPDET);
    i2c_disable(I2C1);
    i2c_deinit(I2C1);
}

DWORD I2cSlaveFrameCount(VOID)
{
    return g_FrameCnt;
}

DWORD I2cSlaveErrorCount(VOID)
{
    return (g_ErrCnt.err + g_ErrCnt.over);
}

BYTE I2cSlaveGetMgmtMemAddr(VOID)
{
    return g_MgmtMemAddr;
}

BYTE *I2cSlaveGetRxBuf(VOID)
{
    return g_RxBuf;
}

VOID I2cSlaveRegisterFrameCb(VOID (*callback)(DWORD frame_bytes))
{
    g_FrameCb = callback;
}

VOID I2cSlaveRegisterReadSource(VOID (*callback)(BYTE memaddr, BYTE *buf, DWORD len))
{
    g_ReadSrcCb = callback;
}

VOID I2cSlaveRegisterReadDone(VOID (*callback)(BYTE memaddr, BYTE rdlen))
{
    g_ReadDoneCb = callback;
}

/* I2C1 event interrupt: address match / rx byte / tx empty / stop. */
void I2C1_EV_IRQHandler(void)
{
    if(i2c_flag_get(I2C1, I2C_FLAG_ADDSEND) != RESET) {
        i2c_flag_clear(I2C1, I2C_FLAG_ADDSEND);

        if(i2c_flag_get(I2C1, I2C_FLAG_TR)){
            I2C_STAT(I2C1) |= I2C_STAT_TBE;
            g_I2cState = I2C_STATE_TXDATA;
            /* Read phase begins: snapshot the register window from the
             * read source; the TI interrupt streams g_TxBuf afterwards. */
            g_TxIdx = 0U;
            s_TxStartAddr = g_MgmtMemAddr;   /* TI 自增前记录读起始地址 */
            if(g_ReadSrcCb != NULL) {
                g_ReadSrcCb(g_MgmtMemAddr, g_TxBuf, I2C_SLAVE_TX_BUF_SIZE);
            }
        }else{
            g_I2cState = I2C_STATE_RXADDR;
        }
    }

    if(i2c_flag_get(I2C1, I2C_FLAG_RBNE) != RESET) {
        g_RxData = i2c_data_receive(I2C1);
        if(g_I2cState == I2C_STATE_RXADDR) {
            /* 帧首字节 = 窗口地址；I2C_RX_LEN 复位，随后记数据长度 */
            g_RxBuf[I2C_RX_ADDR] = g_RxData;
            g_RxBuf[I2C_RX_LEN]  = 0U;
            g_MgmtMemAddr = g_RxData;
        } else {
            /* 数据字节：进帧暂存区，长度记入 g_RxBuf[I2C_RX_LEN]；
             * 暂存满后多余字节丢弃（host 违反 <=8B 约定时的防护） */
            if(g_RxBuf[I2C_RX_LEN] < (I2C_SLAVE_RX_BUF_SIZE - I2C_RX_DATA)) {
                g_RxBuf[I2C_RX_DATA + g_RxBuf[I2C_RX_LEN]] = g_RxData;
                g_RxBuf[I2C_RX_LEN]++;
            }
        }
        g_FrameBytes++;
        g_I2cState = I2C_STATE_RXDATA;
    }

    if(i2c_flag_get(I2C1, I2C_FLAG_TI) != RESET) {
        if(g_ReadSrcCb != NULL) {
            i2c_data_transmit(I2C1, g_TxBuf[g_TxIdx]);
            g_TxIdx = (g_TxIdx + 1U) & (I2C_SLAVE_TX_BUF_SIZE - 1U);
            g_MgmtMemAddr = MEM_ADD_INC(g_MgmtMemAddr, 1U);
        } else {
            i2c_data_transmit(I2C1, I2C_SLAVE_DUMMY_BYTE);
        }
    }

    if(i2c_flag_get(I2C1, I2C_FLAG_STPDET) != RESET) {
        i2c_flag_clear(I2C1, I2C_FLAG_STPDET);

        g_FrameCnt++;
        if(g_FrameCb != NULL) {
            g_FrameCb(g_FrameBytes);
        }
        /* 读相位收尾（本事务含读相位时状态停在 TXDATA）：按实际读出的字节
         * 数回调清 COR——g_TxIdx 为 mod 128 计数，0 表示读出整半区(≥128)。 */
        if((g_ReadDoneCb != NULL) && (g_I2cState == I2C_STATE_TXDATA)) {
            g_ReadDoneCb(s_TxStartAddr, (g_TxIdx == 0U) ? 128U : g_TxIdx);
        }
        g_FrameBytes = 0U;
    }

        /* Clear the I2C state to idle after stop condition is detected. */
        g_I2cState = I2C_STATE_IDLE;
}


/* I2C1 error interrupt: overrun / bus error / arbitration loss. */
void I2C1_ER_IRQHandler(void)
{
    if(i2c_flag_get(I2C1, I2C_FLAG_NACK) != RESET) {
        i2c_flag_clear(I2C1, I2C_FLAG_NACK);
        g_ErrCnt.over++;
    }
    if(i2c_flag_get(I2C1, I2C_FLAG_OUERR) != RESET) {
        i2c_flag_clear(I2C1, I2C_FLAG_OUERR);
        g_ErrCnt.over++;
    }
    if(i2c_flag_get(I2C1, I2C_FLAG_BERR) != RESET) {
        i2c_flag_clear(I2C1, I2C_FLAG_BERR);
        g_ErrCnt.err++;
    }
    if(i2c_flag_get(I2C1, I2C_FLAG_LOSTARB) != RESET) {
        i2c_flag_clear(I2C1, I2C_FLAG_LOSTARB);
        g_ErrCnt.err++;
    }
}

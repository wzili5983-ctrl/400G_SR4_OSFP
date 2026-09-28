/***************************************************************************//**
 *   @file   mci.c
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
 * closes the frame and, when the frame carries data bytes, pushes
 * [address, data...] as one record into the 256-byte ring g_Ring;
 * the application drains it with I2cSlaveRead(). Ring overflow drops
 * the whole frame and is counted.
 *
 * Transmit path: at the host read-phase address match the registered
 * read source (I2cSlaveRegisterReadSource) snapshots 128 bytes from
 * the register file; the TI interrupt streams them with 128-byte wrap.
 * Without a source, 0xFF dummy bytes keep the bus moving.
 *
 * SCL clock stretching is enabled, so a late-draining host is throttled
 * by hardware; remaining overruns / bus errors are counted.
 ******************************************************************************/
#include "mci.h"
#include "gd32e511_512.h"
#include "gd32e511_512_i2c.h"
#include "gd32e511_512_rcu.h"
#include "gd32e511_512_gpio.h"

#define I2C_SLAVE_RX_BUF_SIZE   256U                    /* frame staging buffer */
#define I2C_SLAVE_RING_SIZE     256U                    /* completed-frame ring */
#define I2C_SLAVE_RING_MASK     (I2C_SLAVE_RING_SIZE - 1U)
#define I2C_SLAVE_DUMMY_BYTE    0xFFU                   /* host-read filler */

STATIC volatile BYTE g_I2cState = I2C_STATE_IDILE;
STATIC volatile BYTE g_MgmtMemAddr = 0U;                /* Management Memory Map 绝对地址(00h-FFh)，写相位首字节更新，读相位按 128B 半区回卷自增，跨帧保持 */
STATIC volatile BYTE g_RxData = 0U;

STATIC BYTE  g_RxBuf[I2C_SLAVE_RX_BUF_SIZE];            /* frame staging */

/* 已完成帧环形缓冲区：STPDET 把整帧 [地址,数据...] 压入，I2cSlaveRead
 * 在线程上下文取出。head/tail 单调递增，下标 & I2C_SLAVE_RING_MASK 回卷；
 * head 只由 ISR 写，tail 只由线程写，故二者均不必关中断互斥。 */
typedef struct {
    BYTE           buf[I2C_SLAVE_RING_SIZE]; /* 缓冲区 */
    volatile DWORD head;                     /* 生产者索引（ISR 压帧） */
    volatile DWORD tail;                     /* 消费者索引（线程取帧） */
} I2cSlaveRing_t;
STATIC I2cSlaveRing_t g_Ring;
STATIC volatile DWORD g_FrameBytes;                     /* bytes of the frame in progress */
STATIC volatile DWORD g_FrameCnt;                       /* completed frames */

/* 错误/异常计数：ER ISR（NACK/OUERR → over，BERR/LOSTARB → err），
 * over 另含 EV 侧环满丢帧（I2cSlaveRingPushFrame）。单调递增不清零，
 * I2cSlaveErrorCount() 返回 err + over。 */
typedef struct {
    volatile DWORD err;                     /* bus error / arbitration loss */
    volatile DWORD over;                    /* ring full / NACK / OUERR */
} I2cSlaveErrCnt_t;
STATIC I2cSlaveErrCnt_t g_ErrCnt;
STATIC VOID (*g_FrameCb)(DWORD frame_bytes);

/* Host-read snapshot: filled by the registered read source at the
 * read-phase address match, streamed by the TI interrupt. */
#define I2C_SLAVE_TX_BUF_SIZE   128U
STATIC BYTE  g_TxBuf[I2C_SLAVE_TX_BUF_SIZE];
STATIC BYTE  g_TxIdx;
STATIC VOID (*g_ReadSrcCb)(BYTE memaddr, BYTE *buf, DWORD len);

/* STOP 时把整帧 [memaddr, data...] 压入 g_Ring；无数据相位或环满
 * （环满计一次 overrun 并丢帧）时不压。ISR 内调用。 */
STATIC VOID I2cSlaveRingPushFrame(VOID)
{
    BYTE len = g_RxBuf[I2C_RX_LEN];
    BYTE i;

    if(len == 0U) {                             /* 纯读/纯设地址帧：无指令数据 */
        return;
    }
    if((g_Ring.head - g_Ring.tail + (DWORD)len + 1U) > I2C_SLAVE_RING_SIZE) {
        g_ErrCnt.over++;
        return;
    }
    g_Ring.buf[g_Ring.head & I2C_SLAVE_RING_MASK] = g_RxBuf[I2C_RX_ADDR];
    g_Ring.head++;
    for(i = 0U; i < len; i++) {
        g_Ring.buf[g_Ring.head & I2C_SLAVE_RING_MASK] = g_RxBuf[I2C_RX_DATA + i];
        g_Ring.head++;
    }
}

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

    g_I2cState = I2C_STATE_IDILE;
    g_MgmtMemAddr = 0U;
    g_RxBuf[I2C_RX_LEN] = 0U;
    g_Ring.head = 0U;
    g_Ring.tail = 0U;
    g_FrameBytes = 0U;
    g_FrameCnt = 0U;
    g_ErrCnt.err = 0U;
    g_ErrCnt.over = 0U;
    g_FrameCb = NULL;
    g_TxIdx = 0U;
    g_ReadSrcCb = NULL;

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

DWORD I2cSlaveRead(BYTE *buf, DWORD len)
{
    DWORD n = 0U;

    if(buf == NULL) {
        return 0U;
    }
    while((n < len) && (g_Ring.tail != g_Ring.head)) {
        buf[n] = g_Ring.buf[g_Ring.tail & I2C_SLAVE_RING_MASK];
        g_Ring.tail++;
        n++;
    }
    return n;
}

DWORD I2cSlaveRxCount(VOID)
{
    return (g_Ring.head - g_Ring.tail);
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

VOID I2cSlaveRegisterFrameCb(VOID (*callback)(DWORD frame_bytes))
{
    g_FrameCb = callback;
}

VOID I2cSlaveRegisterReadSource(VOID (*callback)(BYTE memaddr, BYTE *buf, DWORD len))
{
    g_ReadSrcCb = callback;
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

        I2cSlaveRingPushFrame();                /* 整帧进环形缓冲区 */
        g_FrameCnt++;
        if(g_FrameCb != NULL) {
            g_FrameCb(g_FrameBytes);
        }
        g_FrameBytes = 0U;
    }
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

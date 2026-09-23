/*!
    \file    gd32e511_512_i3c.h
    \brief   definitions for the i3c

    \version 2025-12-25, V0.2.0, firmware for GD32E511_512
*/

/*
    Copyright (c) 2025, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#ifndef GD32E511_512_I3C_H
#define GD32E511_512_I3C_H

#include "gd32e511_512.h"

/* I3C definitions */
#define I3C                          I3C_BASE                                      /*!< I3C base address */

/* registers definitions */
#define I3C_CTL                      REG32((I3C) + 0x00000000U)                    /*!< I3C control register */
#define I3C_CFG                      REG32((I3C) + 0x00000004U)                    /*!< I3C configuration register */
#define I3C_RDATAB                   REG32((I3C) + 0x00000010U)                    /*!< I3C receive data byte register */
#define I3C_RDATAW                   REG32((I3C) + 0x00000014U)                    /*!< I3C receive data word register */
#define I3C_TDATAB                   REG32((I3C) + 0x00000018U)                    /*!< I3C transmit data byte register */
#define I3C_TDATAW                   REG32((I3C) + 0x0000001CU)                    /*!< I3C transmit data word register */
#define I3C_IBIPLDATA                REG32((I3C) + 0x00000020U)                    /*!< I3C IBI payload data register */
#define I3C_TTCFG                    REG32((I3C) + 0x00000024U)                    /*!< I3C target transmit configuration register */
#define I3C_TSTAT                    REG32((I3C) + 0x00000030U)                    /*!< I3C transmit status register */
#define I3C_ERR                      REG32((I3C) + 0x00000034U)                    /*!< I3C error status register */
#define I3C_RMES                     REG32((I3C) + 0x00000040U)                    /*!< I3C received message register */
#define I3C_STAT                     REG32((I3C) + 0x00000050U)                    /*!< I3C status register */
#define I3C_INTEN                    REG32((I3C) + 0x00000054U)                    /*!< I3C interrupt enable register */
#define I3C_STATC                    REG32((I3C) + 0x00000058U)                    /*!< I3C status clear register */
#define I3C_DEV0                     REG32((I3C) + 0x00000060U)                    /*!< I3C own device characteristics register */
#define I3C_MAXRL                    REG32((I3C) + 0x00000090U)                    /*!< I3C maximum read length register */
#define I3C_MAXWL                    REG32((I3C) + 0x00000094U)                    /*!< I3C maximum write length register */
#define I3C_TIMING1                  REG32((I3C) + 0x000000A4U)                    /*!< I3C timing register */
#define I3C_BCR                      REG32((I3C) + 0x000000C0U)                    /*!< I3C bus characteristics register */
#define I3C_DCR                      REG32((I3C) + 0x000000C4U)                    /*!< I3C device characteristics register */
#define I3C_GETCAP                   REG32((I3C) + 0x000000C8U)                    /*!< I3C get capability register */
#define I3C_GETMXDS                  REG32((I3C) + 0x000000D0U)                    /*!< I3C get max data speed register */
#define I3C_EXTPID                   REG32((I3C) + 0x000000D4U)                    /*!< I3C extended provisioned ID register */

/* bits definitions */
/* I3C_CTL */
#define I3C_CTL_DATACNT              BITS(0,15)                                    /*!< the number of data bytes transmitted */
#define I3C_CTL_MREQT                BITS(27,30)                                   /*!< target message request type */

/* I3C_CFG */
#define I3C_CFG_I3CEN                BIT(0)                                        /*!< I3C enable */
#define I3C_CFG_DMAREN               BIT(8)                                        /*!< RX-FIFO DMA request enable */
#define I3C_CFG_RXFLUSH              BIT(9)                                        /*!< RX-FIFO flush */
#define I3C_CFG_RXFTH                BIT(10)                                       /*!< RX-FIFO threshold */
#define I3C_CFG_DMATEN               BIT(12)                                       /*!< TX-FIFO DMA request enable */
#define I3C_CFG_TXFLUSH              BIT(13)                                       /*!< TX-FIFO flush */
#define I3C_CFG_TXFTH                BIT(14)                                       /*!< TX-FIFO threshold */

/* I3C_RDATAB */
#define I3C_RDATAB_RDATA0            BITS(0,7)                                     /*!< 8-bit received data */

/* I3C_RDATAW */
#define I3C_RDATAW_RDATA0            BITS(0,7)                                     /*!< 8-bit received data */
#define I3C_RDATAW_RDATA1            BITS(8,15)                                    /*!< 8-bit received data */
#define I3C_RDATAW_RDATA2            BITS(16,23)                                   /*!< 8-bit received data */
#define I3C_RDATAW_RDATA3            BITS(24,31)                                   /*!< 8-bit received data */

/* I3C_TDATAB */
#define I3C_TDATAB_TDATA0            BITS(0,7)                                     /*!< 8-bit data to transmit */

/* I3C_TDATAW */
#define I3C_TDATAW_TDATA0            BITS(0,7)                                     /*!< 8-bit transmit data */
#define I3C_TDATAW_TDATA1            BITS(8,15)                                    /*!< 8-bit transmit data */
#define I3C_TDATAW_TDATA2            BITS(16,23)                                   /*!< 8-bit transmit data */
#define I3C_TDATAW_TDATA3            BITS(24,31)                                   /*!< 8-bit transmit data */

/* I3C_IBIPLDATA */
#define I3C_IBIPLDATA_IBIPDATA0      BITS(0,7)                                     /*!< 8-bit IBI payload data */
#define I3C_IBIPLDATA_IBIPDATA1      BITS(8,15)                                    /*!< 8-bit IBI payload data */
#define I3C_IBIPLDATA_IBIPDATA2      BITS(16,23)                                   /*!< 8-bit IBI payload data */
#define I3C_IBIPLDATA_IBIPDATA3      BITS(24,31)                                   /*!< 8-bit IBI payload data */

/* I3C_TTCFG */
#define I3C_TTCFG_PRELDDCNT          BITS(0,15)                                    /*!< the number of bytes preloaded into the TXFIFO */
#define I3C_TTCFG_PRELD              BIT(16)                                       /*!< preload of the TX-FIFO */

/* I3C_TSTAT */
#define I3C_TSTAT_DCNT               BITS(0,15)                                    /*!< the number of transmitted bytes */
#define I3C_TSTAT_DIR                BIT(18)                                       /*!< I3C bus message transmit direction */

/* I3C_ERR */
#define I3C_ERR_ERRCODE              BITS(0,3)                                     /*!< protocol error code/type */
#define I3C_ERR_BERR                 BIT(4)                                        /*!< bus protocol error */
#define I3C_ERR_STALLERR             BIT(5)                                        /*!< SCL stall error */
#define I3C_ERR_OUERR                BIT(6)                                        /*!< RXFIFO overrun or TXFIFO underrun */

/* I3C_RMES */
#define I3C_RMES_RCODE               BITS(8,15)                                    /*!< received CCC code */

/* I3C_STAT */
#define I3C_STAT_TFE                 BIT(1)                                        /*!< TX-FIFO empty flag */
#define I3C_STAT_TFNF                BIT(4)                                        /*!< TX-FIFO not full flag */
#define I3C_STAT_RFNE                BIT(5)                                        /*!< RX-FIFO not empty flag */
#define I3C_STAT_TXLASTF             BIT(6)                                        /*!< TXFIFO last data byte/word flag */
#define I3C_STAT_FRMCF               BIT(9)                                        /*!< private read/write frame completed flag */
#define I3C_STAT_ERR                 BIT(11)                                       /*!< error flag */
#define I3C_STAT_IBITCF              BIT(16)                                       /*!< IBI transmit completed flag */
#define I3C_STAT_WUPF                BIT(21)                                       /*!< wake-up/missed start flag */
#define I3C_STAT_GETF                BIT(22)                                       /*!< get flag */
#define I3C_STAT_GETSTATUSF          BIT(23)                                       /*!< get status flag */
#define I3C_STAT_DAAUPF              BIT(24)                                       /*!< dynamic address update flag */
#define I3C_STAT_SETMWLUPF           BIT(25)                                       /*!< maximum write length update flag */
#define I3C_STAT_SETMRLUPF           BIT(26)                                       /*!< maximum read length update flag */
#define I3C_STAT_RSTF                BIT(27)                                       /*!< reset pattern flag */
#define I3C_STAT_ASUPF               BIT(28)                                       /*!< activity state update flag */
#define I3C_STAT_INTHJUPF            BIT(29)                                       /*!< interrupt/hot-join update flag */

/* I3C_INTEN */
#define I3C_INTEN_TFNFIE             BIT(4)                                        /*!< TX-FIFO not full interrupt enable */
#define I3C_INTEN_RFNEIE             BIT(5)                                        /*!< RX-FIFO not empty interrupt enable */
#define I3C_INTEN_FRMCIE             BIT(9)                                        /*!< frame completed interrupt enable */
#define I3C_INTEN_ERRIE              BIT(11)                                       /*!< error interrupt enable */
#define I3C_INTEN_IBITCIE            BIT(16)                                       /*!< IBI completed interrupt enable */
#define I3C_INTEN_WUPIE              BIT(21)                                       /*!< wake-up interrupt enable */
#define I3C_INTEN_GETIE              BIT(22)                                       /*!< GETxxx CCC interrupt enable */
#define I3C_INTEN_GETSTATUSIE        BIT(23)                                       /*!< GETSTATUS CCC format 1 interrupt enable */
#define I3C_INTEN_DAAUPIE            BIT(24)                                       /*!< ENTDAA/RSTDAA/SETNEWDA CCC interrupt enable */
#define I3C_INTEN_SETMWLUPIE         BIT(25)                                       /*!< SETMWL CCC interrupt enable */
#define I3C_INTEN_SETMRLUPIE         BIT(26)                                       /*!< SETMRL CCC interrupt enable */
#define I3C_INTEN_RSTIE              BIT(27)                                       /*!< reset pattern interrupt enable */
#define I3C_INTEN_ASUPIE             BIT(28)                                       /*!< ENTASx CCC interrupt enable */
#define I3C_INTEN_INTHJUPIE          BIT(29)                                       /*!< ENEC/DISEC CCC interrupt enable */

/* I3C_STATC */
#define I3C_STATC_FRMCFC             BIT(9)                                        /*!< clear FRMCF flag in I3C_STAT register */
#define I3C_STATC_ERRC               BIT(11)                                       /*!< clear ERR flag in I3C_STAT register */
#define I3C_STATC_IBITCFC            BIT(16)                                       /*!< clear IBITCF flag in I3C_STAT register */
#define I3C_STATC_WUPFC              BIT(21)                                       /*!< clear WUPF flag in I3C_STAT register */
#define I3C_STATC_GETFC              BIT(22)                                       /*!< clear GETF flag in I3C_STAT register */
#define I3C_STATC_GETSTATUSFC        BIT(23)                                       /*!< clear GETSTATUSF flag in I3C_STAT register */
#define I3C_STATC_DAAUPFC            BIT(24)                                       /*!< clear DAAUPF flag in I3C_STAT register */
#define I3C_STATC_SETMWLUPFC         BIT(25)                                       /*!< clear SETMWLUPF flag in I3C_STAT register */
#define I3C_STATC_SETMRLUPFC         BIT(26)                                       /*!< clear SETMRLF flag in I3C_STAT register */
#define I3C_STATC_RSTFC              BIT(27)                                       /*!< clear RSTF flag in I3C_STAT register */
#define I3C_STATC_ASUPFC             BIT(28)                                       /*!< clear ASUPF flag in I3C_STAT register */
#define I3C_STATC_INTHJUPFC          BIT(29)                                       /*!< clear INTHJUPF flag in I3C_STAT register */

/* I3C_DEV0 */
#define I3C_DEV0_DADDRVAL            BIT(0)                                        /*!< dynamic address is valid */
#define I3C_DEV0_DADDR               BITS(1,7)                                     /*!< 7-bit dynamic address */
#define I3C_DEV0_IBIEN               BIT(16)                                       /*!< IBI request enable */
#define I3C_DEV0_HJEN                BIT(19)                                       /*!< hot-join request enable */
#define I3C_DEV0_ACTSTATE            BITS(20,21)                                   /*!< activity state */
#define I3C_DEV0_RSTACT              BITS(22,23)                                   /*!< reset action/level on received reset pattern */
#define I3C_DEV0_RSTACTVAL           BIT(24)                                       /*!< reset action is valid */

/* I3C_MAXRL */
#define I3C_MAXRL_MRL                BITS(0,15)                                    /*!< maximum number of bytes for data read length */
#define I3C_MAXRL_IBIPML             BITS(16,18)                                   /*!< maximum number of bytes for IBI payload data */

/* I3C_MAXWL */
#define I3C_MAXWL_MWL                BITS(0,15)                                    /*!< maximum number of bytes for data write length */

/* I3C_TIMING1 */
#define I3C_TIMING1_AVALCLK          BITS(0,7)                                     /*!< number of kernel clock cycles to set a time unit of 1 ��s */

/* I3C_BCR */
#define I3C_BCR_BCR0                 BIT(0)                                        /*!< max data speed limitation */
#define I3C_BCR_BCR2                 BIT(2)                                        /*!< IBI payload */

/* I3C_DCR */
#define I3C_DCR_DCR                  BITS(0,7)                                     /*!< device characteristics ID */

/* I3C_GETCAP */
#define I3C_GETCAP_CAP3PEND          BIT(14)                                       /*!< IBI MDB support for pending read notification */

/* I3C_GETMXDS */
#define I3C_GETMXDS_FMAT             BITS(8,9)                                     /*!< GETMXDS CCC format */
#define I3C_GETMXDS_RDTURN           BITS(16,23)                                   /*!< programmed byte of the 3-byte MaxRdTurn */
#define I3C_GETMXDS_TSCO             BIT(24)                                       /*!< clock-to-data turnaround time (tSCO) */

/* I3C_EXTPID */
#define I3C_EXTPID_MIPIINSTID        BITS(12,15)                                   /*!< 4-bit MIPI instance ID */
#define I3C_EXTPID_IDTYSEL           BIT(16)                                       /*!< provisioned ID type selector */
#define I3C_EXTPID_MIPIMFRID         BITS(17,31)                                   /*!< 15-bit MIPI manufacturer ID */

/* structure definition */
/* i3c target parameter */
typedef struct {
    uint8_t         device_id;                      /*!< specifies the device characteristics ID */
    uint8_t         mipi_id;                        /*!< specifies the bits[15:12] of the 48-bit provisioned ID */
    uint32_t        hot_join_request;               /*!< hot-join request enable or disable */
    uint32_t        ibi_request;                    /*!< ibi request enable or disable */
    uint32_t        ibi_payload;                    /*!< in-band interrupt(ibi) payload */
    uint32_t        ibi_pending_read;               /*!< ibi mdb support for pending read notification */
    uint32_t        ibi_payload_size;               /*!< ibi payload data maximum size in bytes */
    uint16_t        max_data_read_length;           /*!< configure the numbers of data bytes that the target can read at maximum */
    uint16_t        max_data_write_length;          /*!< configure the numbers of data bytes that the target can write at maximum */
    uint32_t        data_turnaround_time;           /*!< configure the time that I3C target clock-to-data */
    uint8_t         max_read_turnaround_byte;       /*!< programmed byte of the 3-byte MaxRdTurn */
    uint32_t        getmxds_ccc_format;             /*!< configure the I3C target returned GETMXDS CCC format */
    uint32_t        max_data_speed_limit;           /*!< configure max data speed limitation */
    uint8_t         bus_available_time;             /*!< number of kernel clock cycles to set a time unit of 1us */
    uint32_t        rx_fifo_threshold;              /*!< configure the rx fifo threshold */
    uint32_t        tx_fifo_threshold;              /*!< configure the tx fifo threshold */
} i3c_target_parameter_struct;

/* enumerator definition */
/* i3c message direction on the i3c bus */
typedef enum {
    I3C_MESSAGE_DIR_WRITE = 0U,                     /*!< the direction is write on the i3c bus */
    I3C_MESSAGE_DIR_READ                            /*!< the direction is read on the i3c bus */
} i3c_message_direction_enum;

/* define the I3C bit position and its register index offset */
#define I3C_REGIDX_BIT(regidx, bitpos)  (((uint32_t)(regidx) << 6) | (uint32_t)(bitpos))
#define I3C_REG_VAL(i3cx, offset)       (REG32((i3cx) + (((uint32_t)(offset) & 0x0000FFFFU) >> 6)))
#define I3C_BIT_POS(val)                ((uint32_t)(val) & 0x0000001FU)
#define I3C_REGIDX_BIT2(regidx, bitpos, regidx2, bitpos2)   (((uint32_t)(regidx2) << 22) | (uint32_t)((bitpos2) << 16)\
                                                            | (((uint32_t)(regidx) << 6) | (uint32_t)(bitpos)))
#define I3C_REG_VAL2(i3cx, offset)      (REG32((i3cx) + ((uint32_t)(offset) >> 22)))
#define I3C_BIT_POS2(val)               (((uint32_t)(val) & 0x001F0000U) >> 16)

/* register offset */
#define I3C_STAT_REG_OFFSET            ((uint32_t)0x00000050U)         /*!< STAT register offset */
#define I3C_ERR_REG_OFFSET             ((uint32_t)0x00000034U)         /*!< ERR register offset */
#define I3C_INTEN_REG_OFFSET           ((uint32_t)0x00000054U)         /*!< INTEN register offset */

/* I3C flags */
typedef enum {
    /* flags in STAT register */
    I3C_FLAG_INTHJUP       = I3C_REGIDX_BIT(I3C_STAT_REG_OFFSET, 29U), /*!< interrupt/hot-join update flag */
    I3C_FLAG_ASUP          = I3C_REGIDX_BIT(I3C_STAT_REG_OFFSET, 28U), /*!< activity state update flag */
    I3C_FLAG_RST           = I3C_REGIDX_BIT(I3C_STAT_REG_OFFSET, 27U), /*!< reset pattern flag */
    I3C_FLAG_SETMRLUP      = I3C_REGIDX_BIT(I3C_STAT_REG_OFFSET, 26U), /*!< maximum read length update flag */
    I3C_FLAG_SETMWLUP      = I3C_REGIDX_BIT(I3C_STAT_REG_OFFSET, 25U), /*!< maximum write length update flag */
    I3C_FLAG_DAAUP         = I3C_REGIDX_BIT(I3C_STAT_REG_OFFSET, 24U), /*!< dynamic address update flag */
    I3C_FLAG_GETSTATUS     = I3C_REGIDX_BIT(I3C_STAT_REG_OFFSET, 23U), /*!< get status flag */
    I3C_FLAG_GET           = I3C_REGIDX_BIT(I3C_STAT_REG_OFFSET, 22U), /*!< get flag */
    I3C_FLAG_WUP           = I3C_REGIDX_BIT(I3C_STAT_REG_OFFSET, 21U), /*!< wake-up flag */
    I3C_FLAG_IBITC         = I3C_REGIDX_BIT(I3C_STAT_REG_OFFSET, 16U), /*!< IBI completed flag */
    I3C_FLAG_ERR           = I3C_REGIDX_BIT(I3C_STAT_REG_OFFSET, 11U), /*!< error flag */
    I3C_FLAG_FRMC          = I3C_REGIDX_BIT(I3C_STAT_REG_OFFSET, 9U),  /*!< frame completed flag */
    I3C_FLAG_TXLAST        = I3C_REGIDX_BIT(I3C_STAT_REG_OFFSET, 6U),  /*!< TX-FIFO last data flag */
    I3C_FLAG_RFNE          = I3C_REGIDX_BIT(I3C_STAT_REG_OFFSET, 5U),  /*!< RX-FIFO not empty flag */
    I3C_FLAG_TFNF          = I3C_REGIDX_BIT(I3C_STAT_REG_OFFSET, 4U),  /*!< TX-FIFO not full flag */
    I3C_FLAG_TFE           = I3C_REGIDX_BIT(I3C_STAT_REG_OFFSET, 1U),  /*!< TX-FIFO empty flag */
    /* flags in ERR register */
    I3C_FLAG_OUERR         = I3C_REGIDX_BIT(I3C_ERR_REG_OFFSET, 6U),   /*!< overrun/underrun error flag */
    I3C_FLAG_STALLERR      = I3C_REGIDX_BIT(I3C_ERR_REG_OFFSET, 5U),   /*!< SCL stall error flag */
    I3C_FLAG_BERR          = I3C_REGIDX_BIT(I3C_ERR_REG_OFFSET, 4U)    /*!< bus protocol error flag */
} i3c_flag_enum;

/* I3C interrupt flags */
typedef enum {
    /* interrupt flags in STAT register */
    I3C_INT_FLAG_INTHJUP   = I3C_REGIDX_BIT2(I3C_INTEN_REG_OFFSET, 29U, I3C_STAT_REG_OFFSET, 29U), /*!< interrupt/hot-join update interrupt flag */
    I3C_INT_FLAG_ASUP      = I3C_REGIDX_BIT2(I3C_INTEN_REG_OFFSET, 28U, I3C_STAT_REG_OFFSET, 28U), /*!< activity state update interrupt flag */
    I3C_INT_FLAG_RST       = I3C_REGIDX_BIT2(I3C_INTEN_REG_OFFSET, 27U, I3C_STAT_REG_OFFSET, 27U), /*!< reset pattern interrupt flag */
    I3C_INT_FLAG_SETMRLUP  = I3C_REGIDX_BIT2(I3C_INTEN_REG_OFFSET, 26U, I3C_STAT_REG_OFFSET, 26U), /*!< SETMRL CCC interrupt flag */
    I3C_INT_FLAG_SETMWLUP  = I3C_REGIDX_BIT2(I3C_INTEN_REG_OFFSET, 25U, I3C_STAT_REG_OFFSET, 25U), /*!< SETMWL CCC interrupt flag */
    I3C_INT_FLAG_DAAUP     = I3C_REGIDX_BIT2(I3C_INTEN_REG_OFFSET, 24U, I3C_STAT_REG_OFFSET, 24U), /*!< ENTDAA/RSTDAA/SETNEWDA CCC interrupt flag */
    I3C_INT_FLAG_GETSTATUS = I3C_REGIDX_BIT2(I3C_INTEN_REG_OFFSET, 23U, I3C_STAT_REG_OFFSET, 23U), /*!< GETSTATUS CCC format 1 interrupt flag */
    I3C_INT_FLAG_GET       = I3C_REGIDX_BIT2(I3C_INTEN_REG_OFFSET, 22U, I3C_STAT_REG_OFFSET, 22U), /*!< GETxxx CCC interrupt flag */
    I3C_INT_FLAG_WUP       = I3C_REGIDX_BIT2(I3C_INTEN_REG_OFFSET, 21U, I3C_STAT_REG_OFFSET, 21U), /*!< wake-up interrupt flag */
    I3C_INT_FLAG_IBITC     = I3C_REGIDX_BIT2(I3C_INTEN_REG_OFFSET, 16U, I3C_STAT_REG_OFFSET, 16U), /*!< IBI completed interrupt flag */
    I3C_INT_FLAG_ERR       = I3C_REGIDX_BIT2(I3C_INTEN_REG_OFFSET, 11U, I3C_STAT_REG_OFFSET, 11U), /*!< error interrupt flag */
    I3C_INT_FLAG_FRMC      = I3C_REGIDX_BIT2(I3C_INTEN_REG_OFFSET, 9U, I3C_STAT_REG_OFFSET, 9U),   /*!< frame completed interrupt flag */
    I3C_INT_FLAG_RFNE      = I3C_REGIDX_BIT2(I3C_INTEN_REG_OFFSET, 5U, I3C_STAT_REG_OFFSET, 5U),   /*!< RX-FIFO not empty interrupt flag */
    I3C_INT_FLAG_TFNF      = I3C_REGIDX_BIT2(I3C_INTEN_REG_OFFSET, 4U, I3C_STAT_REG_OFFSET, 4U),   /*!< TX-FIFO not full interrupt flag */
    /* interrupt flags in ERR register */
    I3C_INT_FLAG_ERR_OUERR = I3C_REGIDX_BIT2(I3C_INTEN_REG_OFFSET, 11U, I3C_ERR_REG_OFFSET, 6U),   /*!< error interrupt and overrun/underrun error flag */
    I3C_INT_FLAG_ERR_STALLERR = I3C_REGIDX_BIT2(I3C_INTEN_REG_OFFSET, 11U, I3C_ERR_REG_OFFSET, 5U), /*!< error interrupt and SCL stall error flag */
    I3C_INT_FLAG_ERR_BERR  = I3C_REGIDX_BIT2(I3C_INTEN_REG_OFFSET, 11U, I3C_ERR_REG_OFFSET, 4U)    /*!< error interrupt and bus protocol error flag */
} i3c_interrupt_flag_enum;

/* constant definition */
/* I3C target error definition */
#define I3C_TARGET_ERROR_TE0                        ((uint32_t)0x00000008U)                         /*!< invalid broadcast address */
#define I3C_TARGET_ERROR_TE1                        ((uint32_t)0x00000009U)                         /*!< target detected a parity error on a ccc code via a parity check */
#define I3C_TARGET_ERROR_TE2                        ((uint32_t)0x0000000AU)                         /*!< target detected a parity error on a write data via a parity check */
#define I3C_TARGET_ERROR_TE3                        ((uint32_t)0x0000000BU)                         /*!< target detected a parity error on the assigned address during dynamic address arbitration */
#define I3C_TARGET_ERROR_TE4                        ((uint32_t)0x0000000CU)                         /*!< detect 0x7E/R missing after Sr during dynamic address arbitration */
#define I3C_TARGET_ERROR_TE5                        ((uint32_t)0x0000000DU)                         /*!< target detected an illegally formatted ccc */
#define I3C_TARGET_ERROR_TE6                        ((uint32_t)0x0000000EU)                         /*!< target detected that transmitted data on the bus is different from expected */

/* I3C activity state */
#define I3C_ACTIVITY_STATE_0                        ((uint32_t)0x00000000U)                         /*!< activity state 0 */
#define I3C_ACTIVITY_STATE_1                        ((uint32_t)0x00000001U)                         /*!< activity state 1 */
#define I3C_ACTIVITY_STATE_2                        ((uint32_t)0x00000002U)                         /*!< activity state 2 */
#define I3C_ACTIVITY_STATE_3                        ((uint32_t)0x00000003U)                         /*!< activity state 3 */

/* functional macro definition */
/* IBI payload data maximum size in bytes */
#define I3C_IBI_PAYLOAD_DATA_MAX_SIZE(regval)       (I3C_MAXRL_IBIPML & ((uint32_t)(regval) << 16U))
#define I3C_IBI_PAYLOAD_DATA_NULL                   I3C_IBI_PAYLOAD_DATA_MAX_SIZE(0)                /*!< null payload data size (only allowed when BCR2=0 in the I3C_BCR register) */
#define I3C_IBI_PAYLOAD_DATA_ONE_BYTE               I3C_IBI_PAYLOAD_DATA_MAX_SIZE(1)                /*!< 1 byte payload data (mandatory data byte) */
#define I3C_IBI_PAYLOAD_DATA_TWO_BYTES              I3C_IBI_PAYLOAD_DATA_MAX_SIZE(2)                /*!< 2 bytes payload data */
#define I3C_IBI_PAYLOAD_DATA_THREE_BYTES            I3C_IBI_PAYLOAD_DATA_MAX_SIZE(3)                /*!< 3 bytes payload data */
#define I3C_IBI_PAYLOAD_DATA_FOUR_BYTES             I3C_IBI_PAYLOAD_DATA_MAX_SIZE(4)                /*!< 4 bytes payload data */

/* clock-to-data turnaround time (tsco) */
#define I3C_DATA_TURNAROUND_TIME_TSCO_LESS_12NS     (0x00000000U)                                   /*!< clock-to-data turnaround time less 12ns */
#define I3C_DATA_TURNAROUND_TIME_TSCO_GREATER_12NS  I3C_GETMXDS_TSCO                                /*!< clock-to-data turnaround time greater 12ns */

/* GETMXDS CCC format */
#define I3C_GETMXDS_CCC_FORMAT(regval)              (I3C_GETMXDS_FMAT & ((uint32_t)(regval) << 8U))
#define I3C_GETMXDS_CCC_FORMAT1                     I3C_GETMXDS_CCC_FORMAT(0)                       /*!< format 1 (2 bytes with MaxWr with no defining byte, MaxRd) */
#define I3C_GETMXDS_CCC_FORMAT2_LSB                 I3C_GETMXDS_CCC_FORMAT(1)                       /*!< format 2 (5 bytes with MaxWr with no defining byte,MaxRd, and the LSB of MaxRdTurn) */
#define I3C_GETMXDS_CCC_FORMAT2_MID                 I3C_GETMXDS_CCC_FORMAT(2)                       /*!< format 2 (5 bytes with MaxWr with no defining byte,MaxRd, and the middle byte of MaxRdTurn) */
#define I3C_GETMXDS_CCC_FORMAT2_MSB                 I3C_GETMXDS_CCC_FORMAT(3)                       /*!< format 2 (5 bytes with MaxWr with no defining byte,MaxRd, and the MSB of MaxRdTurn) */

/* TX-FIFO threshold */
#define I3C_TX_FIFO_THRESHOLD_ONE_BYTE              (0x00000000U)                                   /*!< TX-FIFO threshold is one byte */
#define I3C_TX_FIFO_THRESHOLD_ONE_WORD              I3C_CFG_TXFTH                                   /*!< TX-FIFO threshold is one word */

/* RX-FIFO threshold */
#define I3C_RX_FIFO_THRESHOLD_ONE_BYTE              (0x00000000U)                                   /*!< RX-FIFO threshold is one byte */
#define I3C_RX_FIFO_THRESHOLD_ONE_WORD              I3C_CFG_RXFTH                                   /*!< RX-FIFO threshold is one word */

/* IBI or hot-join request */
#define I3C_TARGET_REQUEST(regval)                  (I3C_CTL_MREQT & ((uint32_t)(regval) << 27U))
#define I3C_TARGET_REQUEST_HOT_JOIN                 I3C_TARGET_REQUEST(0x08)                        /*!< i3c target hot-join request */
#define I3C_TARGET_REQUEST_IBI                      I3C_TARGET_REQUEST(0x0A)                        /*!< i3c target ibi request */

/* hot-join request */
#define I3C_HOT_JOIN_REQ_DISABLE                    (0x00000000U)                                    /*!< hot-join request disabled */
#define I3C_HOT_JOIN_REQ_ENABLE                     I3C_DEV0_HJEN                                    /*!< hot-join request enabled */

/* IBI request */
#define I3C_IBI_REQ_DISABLE                         (0x00000000U)                                    /*!< IBI request disabled */
#define I3C_IBI_REQ_ENABLE                          I3C_DEV0_IBIEN                                   /*!< IBI request enabled */

/* IBI payload */
#define I3C_IBI_PAYLOAD_DISABLE                     (0x00000000U)                                    /*!< IBI payload disabled */
#define I3C_IBI_PAYLOAD_ENABLE                      I3C_BCR_BCR2                                     /*!< IBI payload enabled */

/* Max data speed limit */
#define I3C_MAX_DATA_SPEED_LIMIT_DISABLE            (0x00000000U)                                    /*!< Max data speed limit disabled */
#define I3C_MAX_DATA_SPEED_LIMIT_ENABLE             I3C_BCR_BCR0                                     /*!< Max data speed limit enabled */

/* IBI pending read */
#define I3C_IBI_PENDING_READ_DISABLE                (0x00000000U)                                    /*!< IBI pending read disabled */
#define I3C_IBI_PENDING_READ_ENABLE                 I3C_GETCAP_CAP3PEND                              /*!< IBI pending read enabled */

/* The maximum size of the IBI (In-Band Interrupt) payload that can be transmitted */
#define I3C_IBI_PAYLOAD_TRANSMIT_MAX_SIZE           (0x00000004U)                                    /*!< Maximum IBI payload size in bytes */

/* interrupts in INTEN register */
#define I3C_INT_INTHJUP                             I3C_INTEN_INTHJUPIE                              /*!< interrupt/hot-join update interrupt */
#define I3C_INT_ASUP                                I3C_INTEN_ASUPIE                                 /*!< activity state update interrupt */
#define I3C_INT_RST                                 I3C_INTEN_RSTIE                                  /*!< reset pattern interrupt */
#define I3C_INT_SETMRLUP                            I3C_INTEN_SETMRLUPIE                             /*!< SETMRL CCC interrupt */
#define I3C_INT_SETMWLUP                            I3C_INTEN_SETMWLUPIE                             /*!< SETMWL CCC interrupt */
#define I3C_INT_DAAUP                               I3C_INTEN_DAAUPIE                                /*!< ENTDAA/RSTDAA/SETNEWDA CCC interrupt */
#define I3C_INT_GETSTATUS                           I3C_INTEN_GETSTATUSIE                            /*!< GETSTATUS CCC format 1 interrupt */
#define I3C_INT_GET                                 I3C_INTEN_GETIE                                  /*!< GETxxx CCC interrupt */
#define I3C_INT_WUP                                 I3C_INTEN_WUPIE                                  /*!< wake-up interrupt */
#define I3C_INT_IBITC                               I3C_INTEN_IBITCIE                                /*!< IBI completed interrupt */
#define I3C_INT_ERR                                 I3C_INTEN_ERRIE                                  /*!< error interrupt */
#define I3C_INT_FRMC                                I3C_INTEN_FRMCIE                                 /*!< frame completed interrupt */
#define I3C_INT_RFNE                                I3C_INTEN_RFNEIE                                 /*!< RX-FIFO not empty interrupt */
#define I3C_INT_TFNF                                I3C_INTEN_TFNFIE                                 /*!< TX-FIFO not full interrupt */

/* parameter check definitions */
#ifdef FW_DEBUG_ERR_REPORT
/* The maximum size of the IBI (In-Band Interrupt) payload that can be transmitted */
#define I3C_IBI_PAYLOAD_TRANSMIT_MAX_SIZE           (0x00000004U)                                    /*!< Maximum IBI payload size in bytes */
/* check if the IBI payload count exceeds the maximum allowed size */
#define NOT_I3C_IBI_PLAYLOAD_COUNT(cnt)             ((cnt) > I3C_IBI_PAYLOAD_TRANSMIT_MAX_SIZE)
#endif /* FW_DEBUG_ERR_REPORT */

/* function declarations */
/* initialization functions */
/* reset I3C */
void i3c_deinit(void);
/* initialize struct with the default values */
void i3c_target_struct_para_init(i3c_target_parameter_struct *init_struct);
/* initialize I3C */
void i3c_init(i3c_target_parameter_struct *i3c_target_struct);
/* enable I3C */
void i3c_enable(void);
/* disable I3C */
void i3c_disable(void);

/* configure FIFO */
/* flush RX-FIFO */
void i3c_rx_fifo_flush(void);
/* enable RX-FIFO DMA function */
void i3c_rx_fifo_dma_enable(void);
/* disable RX-FIFO DMA function */
void i3c_rx_fifo_dma_disable(void);
/* flush TX-FIFO */
void i3c_tx_fifo_flush(void);
/* enable TX-FIFO DMA function */
void i3c_tx_fifo_dma_enable(void);
/* disable TX-FIFO DMA function */
void i3c_tx_fifo_dma_disable(void);
/* preload I3C TX-FIFO */
void i3c_tx_fifo_preload(uint16_t preload_num);

/* I3C write and read data */
/* receive data byte */
uint8_t i3c_receive_data_byte(void);
/* receive data word */
uint32_t i3c_receive_data_word(void);
/* transmit data byte */
void i3c_transmit_data_byte(uint8_t data);
/* transmit data word */
void i3c_transmit_data_word(uint32_t data);

/* configure I3C IBI request and hot-join request */
/* request hot-join */
void i3c_hot_join_request(void);
/* enable hot-join request */
void i3c_hot_join_request_enable(void);
/* disable hot-join request */
void i3c_hot_join_request_disable(void);
/* get hot-join request status */
FlagStatus i3c_hot_join_request_status_get(void);
/* request IBI */
void i3c_ibi_request(uint16_t ibi_cnt, uint8_t buf[]);
/* enable IBI request */
void i3c_ibi_request_enable(void);
/* disable IBI request */
void i3c_ibi_request_disable(void);
/* get IBI request status */
FlagStatus i3c_ibi_request_status_get(void);
/* enable IBI payload */
void i3c_ibi_payload_enable(void);
/* disable IBI payload */
void i3c_ibi_payload_disable(void);
/* get IBI payload status */
FlagStatus i3c_ibi_payload_status_get(void);
/* set IBI payload maximum size */
void i3c_ibi_payload_maximum_size_set(uint32_t max_size);
/* get IBI payload maximum size */
uint32_t i3c_ibi_payload_maximum_size_get(void);
/* enable IBI MDB support for pending read notification */
void i3c_ibi_mdb_pending_read_notification_enable(void);
/* disable IBI MDB support for pending read notification */
void i3c_ibi_mdb_pending_read_notification_disable(void);
/* get IBI MDB pending read notification status */
FlagStatus i3c_ibi_mdb_pending_read_notification_status_get(void);

/* I3C bus status */
/* get message direction on the I3C bus */
i3c_message_direction_enum i3c_transmit_direction_get(void);
/* get the number of data bytes on the I3C bus */
uint16_t i3c_transmit_bytes_num_get(void);
/* get I3C received CCC code */
uint32_t i3c_received_ccc_code_get(void);

/* I3C error detect */
/* get I3C error code */
uint32_t i3c_error_code_get(void);

/* I3C CCC */
/* get dynamic address status */
FlagStatus i3c_dynamic_address_status_get(void);
/* get dynamic address */
uint8_t i3c_dynamic_address_get(void);
/* get activity state */
uint32_t i3c_activity_state_get(void);
/* get reset level */
uint32_t i3c_reset_level_get(void);
/* get reset action status */
FlagStatus i3c_reset_action_status_get(void);
/* set the maximum data read length */
void i3c_max_data_read_length_set(uint16_t max_length);
/* get the maximum data read length */
uint16_t i3c_max_data_read_length_get(void);
/* set the maximum data write length */
void i3c_max_data_write_length_set(uint16_t max_length);
/* get the maximum data write length */
uint16_t i3c_max_data_write_length_get(void);
/* set I3C device characteristics ID */
void i3c_device_id_set(uint8_t id);
/* get I3C device characteristics ID */
uint8_t i3c_device_id_get(void);

/* I3C timing */
/* set bus available time */
void i3c_bus_available_time_set(uint8_t timing);
/* get bus available time */
uint8_t i3c_bus_available_time_get(void);
/* enable I3C max data speed limitation */
void i3c_max_data_speed_limitation_enable(void);
/* disable I3C max data speed limitation */
void i3c_max_data_speed_limitation_disable(void);
/* get I3C max data speed limitation status */
FlagStatus i3c_max_data_speed_limitation_status_get(void);
/* set I3C clock-to-data turnaround time */
void i3c_data_turnaround_time_set(uint32_t timing);
/* set I3C max read turnaround byte */
void i3c_max_read_turnaround_byte_set(uint8_t byte_num);
/* set I3C GETMXDS CCC format */
void i3c_getmxds_ccc_format_set(uint32_t format);
/* set I3C MIPI ID */
void i3c_mipi_id_instance_set(uint8_t id);

/* I3C interrupt and flag */
/* get I3C flag */
FlagStatus i3c_flag_get(i3c_flag_enum flag);
/* clear I3C flag */
void i3c_flag_clear(i3c_flag_enum flag);
/* enable the I3C interrupt */
void i3c_interrupt_enable(uint32_t interrupt);
/* disable the I3C interrupt */
void i3c_interrupt_disable(uint32_t interrupt);
/* get I3C interrupt flag */
FlagStatus i3c_interrupt_flag_get(i3c_interrupt_flag_enum int_flag);
/* clear I3C interrupt flag */
void i3c_interrupt_flag_clear(i3c_interrupt_flag_enum int_flag);

#endif /* GD32E511_512_I3C_H */

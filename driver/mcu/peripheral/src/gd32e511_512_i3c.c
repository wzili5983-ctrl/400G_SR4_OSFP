/*!
    \file    gd32e511_512_i3c.c
    \brief   I3C driver

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

#include "gd32e511_512_i3c.h"

/* I3C register bit offset */
#define EXTPID_MIPIID_OFFSET              ((uint32_t)0x0000000CU) /*!< bit offset of MIPIID in I3C_EXTPID */
#define GETMXDSR_RDTURN_OFFSET            ((uint32_t)0x00000010U) /*!< bit offset of RDTURN in GETMXDSR */
#define RMR_RCODE_OFFSET                  ((uint32_t)0x00000008U) /*!< bit offset of RCODE in RMR */
#define DEVR0_DA_OFFSET                   ((uint32_t)0x00000001U) /*!< bit offset of DA in DEVR0 */
#define DEVR0_AS_OFFSET                   ((uint32_t)0x00000014U) /*!< bit offset of AS in DEVR0 */
#define DEVR0_RSTACT_OFFSET               ((uint32_t)0x00000016U) /*!< bit offset of RSTACT in DEVR0 */
#define I3C_IBI_MAX_LENGTH_OFFSET         ((uint32_t)0x00000010U) /*!< bit offset of IBIPML in I3C_MAXRL */
/* I3C register mask */
#define I3C_DEV0_INIT_MASK                ((uint32_t)0xFFF6FFFFU) /*!< I3C device initialization mask */
#define I3C_TARGET_REQUEST_HOT_JOIN_MASK  ((uint32_t)0x87FFFFFFU) /*!< I3C target request hot-join mask */

/*!
    \brief      reset I3C (API_ID(0x0001U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i3c_deinit(void)
{
    rcu_periph_reset_enable(RCU_I3CRST);
    rcu_periph_reset_disable(RCU_I3CRST);
}

/*!
    \brief      initialize I3C init parameter struct with a default value (API_ID(0x0002U))
    \param[in]  none
    \param[out] init_struct: init parameter struct
    \retval     none
*/
void i3c_target_struct_para_init(i3c_target_parameter_struct *init_struct)
{
#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_VALID_POINTER(init_struct)) {
        fw_debug_report_err(I3C_MODULE_ID, API_ID(0x0002U), ERR_PARAM_POINTER);
    } else
#endif /* FW_DEBUG_ERR_REPORT */
    {
        /* set the I3C struct with the default values */
        init_struct->device_id                = 0U;
        init_struct->mipi_id                  = 0U;
        init_struct->hot_join_request         = I3C_HOT_JOIN_REQ_DISABLE;
        init_struct->ibi_request              = I3C_IBI_REQ_DISABLE;
        init_struct->ibi_payload              = I3C_IBI_PAYLOAD_DISABLE;
        init_struct->ibi_pending_read         = I3C_IBI_PENDING_READ_DISABLE;
        init_struct->ibi_payload_size         = I3C_IBI_PAYLOAD_DATA_NULL;
        init_struct->max_data_read_length     = 0xFFU;
        init_struct->max_data_write_length    = 0xFFU;
        init_struct->data_turnaround_time     = I3C_DATA_TURNAROUND_TIME_TSCO_LESS_12NS;
        init_struct->max_read_turnaround_byte = 0U;
        init_struct->getmxds_ccc_format       = I3C_GETMXDS_CCC_FORMAT1;
        init_struct->max_data_speed_limit     = I3C_MAX_DATA_SPEED_LIMIT_DISABLE;
        init_struct->bus_available_time       = 0U;
        init_struct->rx_fifo_threshold        = I3C_RX_FIFO_THRESHOLD_ONE_BYTE;
        init_struct->tx_fifo_threshold        = I3C_TX_FIFO_THRESHOLD_ONE_BYTE;
    }
}

/*!
    \brief      initialize I3C (API_ID(0x0003U))
    \param[in]  i3c_target_struct: I3C parameter initialization struct members of the structure
                            and the member values are shown as below:
                  device_id: 0x00 ~ 0xFF
                  mipi_id: 0x00 ~ 0xFF
                  hot_join_request: I3C_HOT_JOIN_REQ_DISABLE, I3C_HOT_JOIN_REQ_ENABLE
                  ibi_request: I3C_IBI_REQ_DISABLE, I3C_IBI_REQ_ENABLE
                  ibi_payload: I3C_IBI_PAYLOAD_DISABLE, I3C_IBI_PAYLOAD_ENABLE
                  ibi_pending_read: I3C_IBI_PENDING_READ_DISABLE, I3C_IBI_PENDING_READ_ENABLE
                  ibi_payload_size: I3C_IBI_PAYLOAD_DATA_NULL, I3C_IBI_PAYLOAD_DATA_ONE_BYTE,
                                    I3C_IBI_PAYLOAD_DATA_TWO_BYTES, I3C_IBI_PAYLOAD_DATA_THREE_BYTES,
                                    I3C_IBI_PAYLOAD_DATA_FOUR_BYTES
                  max_data_read_length: 0x0000 ~ 0xFFFF
                  max_data_write_length: 0x0000 ~ 0xFFFF
                  data_turnaround_time: I3C_DATA_TURNAROUND_TIME_TSCO_LESS_12NS,
                                        I3C_DATA_TURNAROUND_TIME_TSCO_GREATER_12NS
                  max_read_turnaround_byte: Min_data = 0x00 and Max_data = 0xFF
                  getmxds_ccc_format: I3C_GETMXDS_CCC_FORMAT1, I3C_GETMXDS_CCC_FORMAT2_LSB,
                                      I3C_GETMXDS_CCC_FORMAT2_MID, I3C_GETMXDS_CCC_FORMAT2_MSB
                  max_data_speed_limit: I3C_MAX_DATA_SPEED_LIMIT_DISABLE, I3C_MAX_DATA_SPEED_LIMIT_ENABLE
                  bus_available_time: 0x00 ~ 0xFF
                  rx_fifo_threshold: I3C_RX_FIFO_THRESHOLD_ONE_BYTE, I3C_RX_FIFO_THRESHOLD_ONE_WORD
                  tx_fifo_threshold: I3C_TX_FIFO_THRESHOLD_ONE_BYTE, I3C_TX_FIFO_THRESHOLD_ONE_WORD
    \param[out] none
    \retval     none
*/
void i3c_init(i3c_target_parameter_struct *i3c_target_struct)
{
    /* declare all local variables at the beginning and initialize to 0U */
    uint32_t reg_extpid = 0U;
    uint32_t reg_dev = 0U;
    uint32_t reg_cfg = 0U;

#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_VALID_POINTER(i3c_target_struct)) {
        fw_debug_report_err(I3C_MODULE_ID, API_ID(0x0003U), ERR_PARAM_POINTER);
    } else
#endif /* FW_DEBUG_ERR_REPORT */
    {
        /* configure device characteristics ID */
        I3C_DCR = (uint32_t)i3c_target_struct->device_id;

        /* configure 4-bit MIPI Instance ID */
        reg_extpid = I3C_EXTPID;
        reg_extpid = (reg_extpid & ~I3C_EXTPID_MIPIINSTID) | (((uint32_t)i3c_target_struct->mipi_id << EXTPID_MIPIID_OFFSET) &
                     I3C_EXTPID_MIPIINSTID);
        I3C_EXTPID = reg_extpid;

        /* configure hot-join request and ibi request */
        reg_dev = I3C_DEV0;
        reg_dev &= I3C_DEV0_INIT_MASK;
        reg_dev |= (i3c_target_struct->hot_join_request & I3C_DEV0_HJEN);
        reg_dev |= (i3c_target_struct->ibi_request & I3C_DEV0_IBIEN);
        I3C_DEV0 = reg_dev;

        /* configure BCR */
        I3C_BCR = (i3c_target_struct->ibi_payload & I3C_BCR_BCR2) | (i3c_target_struct->max_data_speed_limit & I3C_BCR_BCR0);

        /* configure IBI MDB pending read notification */
        I3C_GETCAP = i3c_target_struct->ibi_pending_read;

        /* configure IBI payload data max size and max data read length */
        I3C_MAXRL = (i3c_target_struct->ibi_payload_size & I3C_MAXRL_IBIPML) | (i3c_target_struct->max_data_read_length &
                    I3C_MAXRL_MRL);

        /* configure max data write length */
        I3C_MAXWL = (uint32_t)i3c_target_struct->max_data_write_length;

        /* configure getmxdsr */
        I3C_GETMXDS = (i3c_target_struct->data_turnaround_time & I3C_GETMXDS_TSCO) | (
                          ((uint32_t)i3c_target_struct->max_read_turnaround_byte << GETMXDSR_RDTURN_OFFSET) & I3C_GETMXDS_RDTURN) |
                      (i3c_target_struct->getmxds_ccc_format & I3C_GETMXDS_FMAT);

        /* configure bus available time */
        I3C_TIMING1 = (uint32_t)i3c_target_struct->bus_available_time;

        /* configure TX\RX FIFO threshold */
        reg_cfg = I3C_CFG;
        reg_cfg &= ~(I3C_CFG_RXFTH | I3C_CFG_TXFTH);
        reg_cfg |= (i3c_target_struct->rx_fifo_threshold & I3C_CFG_RXFTH) | (i3c_target_struct->tx_fifo_threshold &
                   I3C_CFG_TXFTH);
        I3C_CFG = reg_cfg;
    }
}

/*!
    \brief      enable I3C (API_ID(0x0004U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i3c_enable(void)
{
    I3C_CFG |= I3C_CFG_I3CEN;
}

/*!
    \brief      disable I3C (API_ID(0x0005U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i3c_disable(void)
{
    I3C_CFG &= ~I3C_CFG_I3CEN;
}

/*!
    \brief      flush RX-FIFO (API_ID(0x0006U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i3c_rx_fifo_flush(void)
{
    I3C_CFG |= I3C_CFG_RXFLUSH;
}

/*!
    \brief      enable RX-FIFO DMA function (API_ID(0x0007U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i3c_rx_fifo_dma_enable(void)
{
    I3C_CFG |= I3C_CFG_DMAREN;
}

/*!
    \brief      disable RX-FIFO DMA function (API_ID(0x0008U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i3c_rx_fifo_dma_disable(void)
{
    I3C_CFG &= ~I3C_CFG_DMAREN;
}

/*!
    \brief      flush TX-FIFO (API_ID(0x0009U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i3c_tx_fifo_flush(void)
{
    I3C_CFG |= I3C_CFG_TXFLUSH;
}

/*!
    \brief      enable TX-FIFO DMA function (API_ID(0x000AU))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i3c_tx_fifo_dma_enable(void)
{
    I3C_CFG |= I3C_CFG_DMATEN;
}

/*!
    \brief      disable TX-FIFO DMA function (API_ID(0x000BU))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i3c_tx_fifo_dma_disable(void)
{
    I3C_CFG &= ~I3C_CFG_DMATEN;
}

/*!
    \brief      preload I3C TX-FIFO (API_ID(0x000CU))
    \param[in]  preload_num: preload transmit data counter in bytes, 0x00 ~ 0xFF
    \param[out] none
    \retval     none
*/
void i3c_tx_fifo_preload(uint16_t preload_num)
{
    I3C_TTCFG = (I3C_TTCFG_PRELD | (uint32_t)preload_num);
}

/*!
    \brief      I3C receive data byte (API_ID(0x000DU))
    \param[in]  none
    \param[out] none
    \retval     uint8_t: 0x00-0xFF
*/
uint8_t i3c_receive_data_byte(void)
{
    return (uint8_t)(I3C_RDATAB & I3C_RDATAB_RDATA0);
}

/*!
    \brief      I3C receive data word (API_ID(0x000EU))
    \param[in]  none
    \param[out] none
    \retval     uint32_t: 0x00000000-0xFFFFFFFF
*/
uint32_t i3c_receive_data_word(void)
{
    return I3C_RDATAW;
}

/*!
    \brief      I3C transmit data byte (API_ID(0x000FU))
    \param[in]  data: data to be transmitted
    \param[out] none
    \retval     none
*/
void i3c_transmit_data_byte(uint8_t data)
{
    I3C_TDATAB = (uint32_t)data;
}

/*!
    \brief      I3C transmit data word (API_ID(0x0010U))
    \param[in]  data: data to be transmitted
    \param[out] none
    \retval     none
*/
void i3c_transmit_data_word(uint32_t data)
{
    I3C_TDATAW = data;
}

/*!
    \brief      I3C hot-join request (API_ID(0x0011U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i3c_hot_join_request(void)
{
    uint32_t reg;

    reg = I3C_CTL;
    reg &= I3C_TARGET_REQUEST_HOT_JOIN_MASK;
    reg |= I3C_TARGET_REQUEST_HOT_JOIN;
    I3C_CTL = reg;
}

/*!
    \brief      enable I3C hot-join request (API_ID(0x0012U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i3c_hot_join_request_enable(void)
{
    I3C_DEV0 |= I3C_DEV0_HJEN;
}

/*!
    \brief      disable I3C hot-join request (API_ID(0x0013U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i3c_hot_join_request_disable(void)
{
    I3C_DEV0 &= ~I3C_DEV0_HJEN;
}

/*!
    \brief      get hot-join request status (API_ID(0x0014U))
    \param[in]  none
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus i3c_hot_join_request_status_get(void)
{
    FlagStatus reval = RESET;

    if(RESET != (I3C_DEV0 & I3C_DEV0_HJEN)) {
        reval = SET;
    } else {
        reval = RESET;
    }

    return reval;
}

/*!
    \brief      I3C ibi request (API_ID(0x0015U))
    \param[in]  ibi_cnt: number of bytes of the ibi data payload
    \param[in]  buf: ibi data payload buffer
    \param[out] none
    \retval     none
*/
void i3c_ibi_request(uint16_t ibi_cnt, uint8_t buf[])
{
    uint32_t val = 0U;
    uint8_t i = 0U;
    uint8_t val_array[4] = {0};

#ifdef FW_DEBUG_ERR_REPORT
    if(NOT_VALID_POINTER(buf)) {
        fw_debug_report_err(I3C_MODULE_ID, API_ID(0x0015U), ERR_PARAM_POINTER);
    } else if(NOT_I3C_IBI_PLAYLOAD_COUNT((uint32_t)ibi_cnt)) {
        fw_debug_report_err(I3C_MODULE_ID, API_ID(0x0015U), ERR_PARAM_OUT_OF_RANGE);
    } else
#endif /* FW_DEBUG_ERR_REPORT */
    {
        /* Copy the IBI payload data into the temporary array */
        for(i = 0U; i < ibi_cnt; i++) {
            val_array[i] = buf[i];
        }

        /* Combine the array into a single 32-bit value */
        val = ((uint32_t)val_array[0]) |
              ((uint32_t)val_array[1] << 8U) |
              ((uint32_t)val_array[2] << 16U) |
              ((uint32_t)val_array[3] << 24U);

        /* Configure IBI data payload */
        I3C_IBIPLDATA = val;

        /* Send the IBI request with the specified payload size */
        I3C_CTL = (I3C_TARGET_REQUEST_IBI & I3C_CTL_MREQT) | ((uint32_t)(ibi_cnt) & I3C_CTL_DATACNT);
    }
}

/*!
    \brief      enable I3C ibi request (API_ID(0x0016U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i3c_ibi_request_enable(void)
{
    I3C_DEV0 |= I3C_DEV0_IBIEN;
}

/*!
    \brief      disable I3C ibi request (API_ID(0x0017U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i3c_ibi_request_disable(void)
{
    I3C_DEV0 &= ~I3C_DEV0_IBIEN;
}

/*!
    \brief      get I3C ibi request status (API_ID(0x0018U))
    \param[in]  none
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus i3c_ibi_request_status_get(void)
{
    FlagStatus reval = RESET;

    if(RESET != (I3C_DEV0 & I3C_DEV0_IBIEN)) {
        reval = SET;
    } else {
        reval = RESET;
    }

    return reval;
}

/*!
    \brief      enable I3C ibi payload (API_ID(0x0019U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i3c_ibi_payload_enable(void)
{
    I3C_BCR |= I3C_BCR_BCR2;
}

/*!
    \brief      disable I3C ibi payload (API_ID(0x001AU))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i3c_ibi_payload_disable(void)
{
    I3C_BCR &= ~I3C_BCR_BCR2;
}

/*!
    \brief      I3C ibi payload status get (API_ID(0x001BU))
    \param[in]  none
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus i3c_ibi_payload_status_get(void)
{
    FlagStatus reval = RESET;

    if(RESET != (I3C_BCR & I3C_BCR_BCR2)) {
        reval = SET;
    } else {
        reval = RESET;
    }

    return reval;
}

/*!
    \brief      configure I3C ibi payload maximum size (API_ID(0x001CU))
    \param[in]  max_size: ibi payload maximum size, 0x00 ~0x04
    \param[out] none
    \retval     none
*/
void i3c_ibi_payload_maximum_size_set(uint32_t max_size)
{
    I3C_MAXRL &= ~I3C_MAXRL_IBIPML;
    I3C_MAXRL |= (max_size << I3C_IBI_MAX_LENGTH_OFFSET) & I3C_MAXRL_IBIPML;
}

/*!
    \brief      get I3C ibi payload maximum size (API_ID(0x001DU))
    \param[in]  none
    \param[out] none
    \retval     uint32_t: 0x00 ~0x04
*/
uint32_t i3c_ibi_payload_maximum_size_get(void)
{
    uint32_t size;
    if(((I3C_MAXRL & I3C_MAXRL_IBIPML) >> I3C_IBI_MAX_LENGTH_OFFSET) > I3C_IBI_PAYLOAD_TRANSMIT_MAX_SIZE) {
        size = I3C_IBI_PAYLOAD_TRANSMIT_MAX_SIZE;
    } else {
        size = (uint32_t)((I3C_MAXRL & I3C_MAXRL_IBIPML) >> I3C_IBI_MAX_LENGTH_OFFSET);
    }
    return size;
}

/*!
    \brief      enable I3C ibi mdb pending read (API_ID(0x001EU))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i3c_ibi_mdb_pending_read_notification_enable(void)
{
    I3C_GETCAP |= I3C_GETCAP_CAP3PEND;
}

/*!
    \brief      disable I3C ibi mdb pending read notification (API_ID(0x001FU))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i3c_ibi_mdb_pending_read_notification_disable(void)
{
    I3C_GETCAP &= ~I3C_GETCAP_CAP3PEND;
}

/*!
    \brief      get I3C ibi mdb pending read notification status (API_ID(0x0020U))
    \param[in]  none
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus i3c_ibi_mdb_pending_read_notification_status_get(void)
{
    FlagStatus reval = RESET;

    if(RESET != (I3C_GETCAP & I3C_GETCAP_CAP3PEND)) {
        reval = SET;
    } else {
        reval = RESET;
    }

    return reval;
}

/*!
    \brief      get message direction on the I3C bus (API_ID(0x0021U))
    \param[in]  none
    \param[out] none
    \retval     i3c_message_direction_enum: I3C_MESSAGE_DIR_WRITE, I3C_MESSAGE_DIR_READ
*/
i3c_message_direction_enum i3c_transmit_direction_get(void)
{
    i3c_message_direction_enum reval = I3C_MESSAGE_DIR_WRITE;
    if(RESET != (I3C_TSTAT & I3C_TSTAT_DIR)) {
        reval = I3C_MESSAGE_DIR_READ;
    } else {
        reval = I3C_MESSAGE_DIR_WRITE;
    }
    return reval;
}

/*!
    \brief      get the number of data bytes on the I3C bus (API_ID(0x0022U))
    \param[in]  none
    \param[out] none
    \retval     uint16_t: 0x0000-0xFFFF
*/
uint16_t i3c_transmit_bytes_num_get(void)
{
    return (uint16_t)(I3C_TSTAT & I3C_TSTAT_DCNT);
}

/*!
    \brief      get received ccc code (API_ID(0x0023U))
    \param[in]  none
    \param[out] none
    \retval     uint32_t: 0x00-0xFF
*/
uint32_t i3c_received_ccc_code_get(void)
{
    return ((I3C_RMES & I3C_RMES_RCODE) >> RMR_RCODE_OFFSET);
}

/*!
    \brief      get I3C error code (API_ID(0x0024U))
    \param[in]  none
    \param[out] none
    \retval     uint32_t: I3C_TARGET_ERROR_TE0, I3C_TARGET_ERROR_TE1, I3C_TARGET_ERROR_TE2,
                          I3C_TARGET_ERROR_TE3, I3C_TARGET_ERROR_TE4, I3C_TARGET_ERROR_TE5,
                          I3C_TARGET_ERROR_TE6
*/
uint32_t i3c_error_code_get(void)
{
    return (I3C_ERR & I3C_ERR_ERRCODE);
}

/*!
    \brief      get dynamic address status (API_ID(0x0025U))
    \param[in]  none
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus i3c_dynamic_address_status_get(void)
{
    FlagStatus reval = RESET;

    if(RESET != (I3C_DEV0 & I3C_DEV0_DADDRVAL)) {
        reval = SET;
    } else {
        reval = RESET;
    }

    return reval;
}

/*!
    \brief      get dynamic address (API_ID(0x0026U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
uint8_t i3c_dynamic_address_get(void)
{
    return (uint8_t)((I3C_DEV0 & I3C_DEV0_DADDR) >> DEVR0_DA_OFFSET);
}

/*!
    \brief      get activity state (API_ID(0x0027U))
    \param[in]  none
    \param[out] none
    \retval     uint32_t: I3C_ACTIVITY_STATE_0, I3C_ACTIVITY_STATE_1,
                          I3C_ACTIVITY_STATE_2, I3C_ACTIVITY_STATE_3
*/
uint32_t i3c_activity_state_get(void)
{
    return ((I3C_DEV0 & I3C_DEV0_ACTSTATE) >> DEVR0_AS_OFFSET);
}

/*!
    \brief      get reset level (API_ID(0x0028U))
    \param[in]  none
    \param[out] none
    \retval     uint32_t: 0x00-0x03
*/
uint32_t i3c_reset_level_get(void)
{
    return ((I3C_DEV0 & I3C_DEV0_RSTACT) >> DEVR0_RSTACT_OFFSET);
}

/*!
    \brief      get reset action status (API_ID(0x0029U))
    \param[in]  none
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus i3c_reset_action_status_get(void)
{
    FlagStatus reval = RESET;

    if(RESET != (I3C_DEV0 & I3C_DEV0_RSTACTVAL)) {
        reval = SET;
    } else {
        reval = RESET;
    }

    return reval;
}

/*!
    \brief      set the max data read length (API_ID(0x002AU))
    \param[in]  max_length: 0x0000-0xFFFF
    \param[out] none
    \retval     none
*/
void i3c_max_data_read_length_set(uint16_t max_length)
{
    I3C_MAXRL &= ~I3C_MAXRL_MRL;
    I3C_MAXRL |= (uint32_t)max_length;
}

/*!
    \brief      get the max data read length (API_ID(0x002BU))
    \param[in]  none
    \param[out] none
    \retval     uint16_t: 0x0000-0xFFFF
*/
uint16_t i3c_max_data_read_length_get(void)
{
    return (uint16_t)(I3C_MAXRL & I3C_MAXRL_MRL);
}

/*!
    \brief      set the maximum data write length (API_ID(0x002CU))
    \param[in]  max_length: 0x0000-0xFFFF
    \param[out] none
    \retval     none
*/
void i3c_max_data_write_length_set(uint16_t max_length)
{
    I3C_MAXWL &= ~I3C_MAXWL_MWL;
    I3C_MAXWL |= (uint32_t)max_length;
}

/*!
    \brief      get the maximum data write length (API_ID(0x002DU))
    \param[in]  none
    \param[out] none
    \retval     uint16_t: 0x0000-0xFFFF
*/
uint16_t i3c_max_data_write_length_get(void)
{
    return (uint16_t)(I3C_MAXWL & I3C_MAXWL_MWL);
}

/*!
    \brief      set I3C device id (API_ID(0x002EU))
    \param[in]  id: 0x00-0xFF
    \param[out] none
    \retval     none
*/
void i3c_device_id_set(uint8_t id)
{
    I3C_DCR &= ~I3C_DCR_DCR;
    I3C_DCR |= (uint32_t)id;
}

/*!
    \brief      get I3C device id (API_ID(0x002FU))
    \param[in]  none
    \param[out] none
    \retval     uint8_t: 0x00-0xFF
*/
uint8_t i3c_device_id_get(void)
{
    return (uint8_t)(I3C_DCR & I3C_DCR_DCR);
}

/*!
    \brief      set bus available time (API_ID(0x0030U))
    \param[in]  timing: 0x00-0xFF
    \param[out] none
    \retval     none
*/
void i3c_bus_available_time_set(uint8_t timing)
{
    I3C_TIMING1 &= ~I3C_TIMING1_AVALCLK;
    I3C_TIMING1 |= (uint32_t)timing;
}

/*!
    \brief      get bus available time (API_ID(0x0031U))
    \param[in]  none
    \param[out] none
    \retval     uint8_t: 0x00-0xFF
*/
uint8_t i3c_bus_available_time_get(void)
{
    return (uint8_t)(I3C_TIMING1 & I3C_TIMING1_AVALCLK);
}

/*!
    \brief      enable I3C max data speed limitation (API_ID(0x0032U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i3c_max_data_speed_limitation_enable(void)
{
    I3C_BCR |= I3C_BCR_BCR0;
}

/*!
    \brief      disable I3C max data speed limitation (API_ID(0x0033U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i3c_max_data_speed_limitation_disable(void)
{
    I3C_BCR &= ~I3C_BCR_BCR0;
}

/*!
    \brief      get I3C max data speed limitation status (API_ID(0x0034U))
    \param[in]  none
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus i3c_max_data_speed_limitation_status_get(void)
{
    FlagStatus reval = RESET;

    if(RESET != (I3C_BCR & I3C_BCR_BCR0)) {
        reval = SET;
    } else {
        reval = RESET;
    }

    return reval;
}

/*!
    \brief      set I3C clock-to-data turnaround time (API_ID(0x0035U))
    \param[in]  timing: turnaround time
                only one parameter can be selected which is shown as below:
      \arg        I3C_DATA_TURNAROUND_TIME_TSCO_LESS_12NS
      \arg        I3C_DATA_TURNAROUND_TIME_TSCO_GREATER_12NS
    \param[out] none
    \retval     none
*/
void i3c_data_turnaround_time_set(uint32_t timing)
{
    I3C_GETMXDS &= ~I3C_GETMXDS_TSCO;
    I3C_GETMXDS |= (timing & I3C_GETMXDS_TSCO);
}

/*!
    \brief      set I3C max read turnaround byte (API_ID(0x0036U))
    \param[in]  byte_num: 0x00-0xFF
    \param[out] none
    \retval     none
*/
void i3c_max_read_turnaround_byte_set(uint8_t byte_num)
{
    I3C_GETMXDS &= ~I3C_GETMXDS_RDTURN;
    I3C_GETMXDS |= (uint32_t)((uint32_t)byte_num << GETMXDSR_RDTURN_OFFSET);
}

/*!
    \brief      set I3C getmxds ccc format (API_ID(0x0037U))
    \param[in]  format: getmxds ccc format
                only one parameter can be selected which is shown as below:
      \arg        I3C_GETMXDS_CCC_FORMAT1
      \arg        I3C_GETMXDS_CCC_FORMAT2_LSB
      \arg        I3C_GETMXDS_CCC_FORMAT2_MID
      \arg        I3C_GETMXDS_CCC_FORMAT2_MSB
    \param[out] none
    \retval     none
*/
void i3c_getmxds_ccc_format_set(uint32_t format)
{
    I3C_GETMXDS &= ~I3C_GETMXDS_FMAT;
    I3C_GETMXDS |= (format & I3C_GETMXDS_FMAT);
}

/*!
    \brief      set I3C mipi id (API_ID(0x0038U))
    \param[in]  id: 0x00-0x0F
    \param[out] none
    \retval     none
*/
void i3c_mipi_id_instance_set(uint8_t id)
{
    I3C_EXTPID &= ~I3C_EXTPID_MIPIINSTID;
    I3C_EXTPID |= (((uint32_t)id << EXTPID_MIPIID_OFFSET) & I3C_EXTPID_MIPIINSTID);
}

/*!
    \brief      get I3C flag (API_ID(0x0039U))
    \param[in]  flag: the I3C event flag
                only one parameter can be selected which is shown as below:
      \arg        I3C_FLAG_INTHJUP: interrupt/hot-join update flag
      \arg        I3C_FLAG_ASUP: activity state update flag
      \arg        I3C_FLAG_RST: reset pattern flag
      \arg        I3C_FLAG_SETMRLUP: maximum read length update flag
      \arg        I3C_FLAG_SETMWLUP: maximum write length update flag
      \arg        I3C_FLAG_DAAUP: dynamic address update flag
      \arg        I3C_FLAG_GETSTATUS: get status flag
      \arg        I3C_FLAG_GET: get flag
      \arg        I3C_FLAG_WUP: wake-up flag
      \arg        I3C_FLAG_IBITC: IBI completed flag
      \arg        I3C_FLAG_ERR: error flag
      \arg        I3C_FLAG_FRMC: frame completed flag
      \arg        I3C_FLAG_TXLAST: TX-FIFO last data flag
      \arg        I3C_FLAG_RFNE: RX-FIFO not empty flag
      \arg        I3C_FLAG_TFNF: TX-FIFO not full flag
      \arg        I3C_FLAG_TFE: TX-FIFO empty flag
      \arg        I3C_FLAG_OUERR: overrun/underrun error flag
      \arg        I3C_FLAG_STALLERR: SCL stall error flag
      \arg        I3C_FLAG_BERR: bus protocol error flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus i3c_flag_get(i3c_flag_enum flag)
{
    FlagStatus status;

    if(0U != (I3C_REG_VAL(I3C, flag) & BIT(I3C_BIT_POS(flag)))) {
        status = SET;
    } else {
        status = RESET;
    }

    return status;
}

/*!
    \brief      clear I3C flag (API_ID(0x003AU))
    \param[in]  flag: the I3C event flag
                only one parameter can be selected which is shown as below:
      \arg        I3C_FLAG_INTHJUP: interrupt/hot-join update flag
      \arg        I3C_FLAG_ASUP: activity state update flag
      \arg        I3C_FLAG_RST: reset pattern flag
      \arg        I3C_FLAG_SETMRLUP: maximum read length update flag
      \arg        I3C_FLAG_SETMWLUP: maximum write length update flag
      \arg        I3C_FLAG_DAAUP: dynamic address update flag
      \arg        I3C_FLAG_GETSTATUS: get status flag
      \arg        I3C_FLAG_GET: get flag
      \arg        I3C_FLAG_WUP: wake-up flag
      \arg        I3C_FLAG_IBITC: IBI completed flag
      \arg        I3C_FLAG_ERR: error flag
      \arg        I3C_FLAG_FRMC: frame completed flag
      \arg        I3C_FLAG_TXLAST: TX-FIFO last data flag
      \arg        I3C_FLAG_RFNE: RX-FIFO not empty flag
      \arg        I3C_FLAG_TFNF: TX-FIFO not full flag
      \arg        I3C_FLAG_TFE: TX-FIFO empty flag
      \arg        I3C_FLAG_OUERR: overrun/underrun error flag
      \arg        I3C_FLAG_STALLERR: SCL stall error flag
      \arg        I3C_FLAG_BERR: bus protocol error flag
    \param[out] none
    \retval     none
*/
void i3c_flag_clear(i3c_flag_enum flag)
{
    if((I3C_FLAG_OUERR == flag) || (I3C_FLAG_STALLERR == flag) || (I3C_FLAG_BERR == flag)) {
        I3C_STATC |= BIT(I3C_BIT_POS(I3C_FLAG_ERR));
    } else {
        I3C_STATC |= BIT(I3C_BIT_POS(flag));
    }
}

/*!
    \brief      enable the I3C interrupt (API_ID(0x003BU))
    \param[in]  interrupt:the I3C interrupt
                one or more parameters can be selected which is shown as below:
      \arg        I3C_INT_INTHJUP:interrupt/hot-join update interrupt
      \arg        I3C_INT_ASUP: activity state update interrupt
      \arg        I3C_INT_RST: reset pattern interrupt
      \arg        I3C_INT_SETMRLUP: SETMRL CCC interrupt
      \arg        I3C_INT_SETMWLUP: SETMWL CCC interrupt
      \arg        I3C_INT_DAAUP: ENTDAA/RSTDAA/SETNEWDA CCC interrupt
      \arg        I3C_INT_GETSTATUS: GETSTATUS CCC format 1 interrupt
      \arg        I3C_INT_GET: GETxxx CCC interrupt
      \arg        I3C_INT_WUP: wake-up interrupt
      \arg        I3C_INT_IBITC: IBI completed interrupt
      \arg        I3C_INT_ERR: error interrupt
      \arg        I3C_INT_FRMC: frame completed interrupt
      \arg        I3C_INT_RFNE: RX-FIFO not empty interrupt
      \arg        I3C_INT_TFNF: TX-FIFO not full interrupt
    \param[out] none
    \retval     none
*/
void i3c_interrupt_enable(uint32_t interrupt)
{
    I3C_INTEN |= (uint32_t)interrupt;
}

/*!
    \brief      disable the I3C interrupt (API_ID(0x003CU))
    \param[in]  interrupt: the I3C interrupt
                one or more parameters can be selected which is shown as below:
      \arg        I3C_INT_INTHJUP:interrupt/hot-join update interrupt
      \arg        I3C_INT_ASUP: activity state update interrupt
      \arg        I3C_INT_RST: reset pattern interrupt
      \arg        I3C_INT_SETMRLUP: SETMRL CCC interrupt
      \arg        I3C_INT_SETMWLUP: SETMWL CCC interrupt
      \arg        I3C_INT_DAAUP: ENTDAA/RSTDAA/SETNEWDA CCC interrupt
      \arg        I3C_INT_GETSTATUS: GETSTATUS CCC format 1 interrupt
      \arg        I3C_INT_GET: GETxxx CCC interrupt
      \arg        I3C_INT_WUP: wake-up interrupt
      \arg        I3C_INT_IBITC: IBI completed interrupt
      \arg        I3C_INT_ERR: error interrupt
      \arg        I3C_INT_FRMC: frame completed interrupt
      \arg        I3C_INT_RFNE: RX-FIFO not empty interrupt
      \arg        I3C_INT_TFNF: TX-FIFO not full interrupt
    \param[out] none
    \retval     none
*/
void i3c_interrupt_disable(uint32_t interrupt)
{
    I3C_INTEN &= ~(uint32_t)interrupt;
}

/*!
    \brief      get I3C interrupt flag (API_ID(0x003DU))
    \param[in]  int_flag: the I3C interrupt flag
                only one parameter can be selected which is shown as below:
      \arg        I3C_INT_FLAG_INTHJUP: interrupt/hot-join update interrupt flag
      \arg        I3C_INT_FLAG_ASUP: activity state update interrupt flag
      \arg        I3C_INT_FLAG_RST: reset pattern interrupt flag
      \arg        I3C_INT_FLAG_SETMRLUP: SETMRL CCC interrupt flag
      \arg        I3C_INT_FLAG_SETMWLUP: SETMWL CCC interrupt flag
      \arg        I3C_INT_FLAG_DAAUP: ENTDAA/RSTDAA/SETNEWDA CCC interrupt flag
      \arg        I3C_INT_FLAG_GETSTATUS: GETSTATUS CCC format 1 interrupt flag
      \arg        I3C_INT_FLAG_GET: GETxxx CCC interrupt flag
      \arg        I3C_INT_FLAG_WUP: wake-up interrupt flag
      \arg        I3C_INT_FLAG_IBITC: IBI completed interrupt flag
      \arg        I3C_INT_FLAG_ERR: error interrupt flag
      \arg        I3C_INT_FLAG_FRMC: frame completed interrupt flag
      \arg        I3C_INT_FLAG_RFNE: RX-FIFO not empty interrupt flag
      \arg        I3C_INT_FLAG_TFNF: TX-FIFO not full interrupt flag
      \arg        I3C_INT_FLAG_ERR_OUERR: error interrupt and overrun/underrun error flag
      \arg        I3C_INT_FLAG_ERR_STALLERR: error interrupt and SCL stall error flag
      \arg        I3C_INT_FLAG_ERR_BERR: error interrupt and bus protocol error flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus i3c_interrupt_flag_get(i3c_interrupt_flag_enum int_flag)
{
    FlagStatus reval = RESET;
    uint32_t intenable, flagstatus, errstatus;

    if((I3C_INT_FLAG_ERR_OUERR == int_flag) || (I3C_INT_FLAG_ERR_STALLERR == int_flag) ||
            (I3C_INT_FLAG_ERR_BERR == int_flag)) {
        /* get the interrupt enable bit status */
        intenable = (I3C_REG_VAL(I3C, I3C_INT_FLAG_ERR) & BIT(I3C_BIT_POS(I3C_INT_FLAG_ERR)));

        /* get the corresponding flag bit status */
        flagstatus = (I3C_REG_VAL2(I3C, I3C_INT_FLAG_ERR) & BIT(I3C_BIT_POS2(I3C_INT_FLAG_ERR)));

        /* get the corresponding error flag bit status */
        errstatus = (I3C_REG_VAL2(I3C, int_flag) & BIT(I3C_BIT_POS2(int_flag)));

        if((intenable != RESET) && (flagstatus != RESET) && (errstatus != RESET)) {
            reval = SET;
        }
    } else {
        /* get the interrupt enable bit status */
        intenable = (I3C_REG_VAL(I3C, int_flag) & BIT(I3C_BIT_POS(int_flag)));

        /* get the corresponding flag bit status */
        flagstatus = (I3C_REG_VAL2(I3C, int_flag) & BIT(I3C_BIT_POS2(int_flag)));

        if((intenable != RESET) && (flagstatus != RESET)) {
            reval = SET;
        }
    }

    return reval;
}

/*!
    \brief      I3C interrupt flag clear (API_ID(0x003EU))
    \param[in]  int_flag: the I3C interrupt
                only one parameter can be selected which is shown as below:
      \arg        I3C_INT_FLAG_INTHJUP: interrupt/hot-join update interrupt flag
      \arg        I3C_INT_FLAG_ASUP: activity state update interrupt flag
      \arg        I3C_INT_FLAG_RST: reset pattern interrupt flag
      \arg        I3C_INT_FLAG_SETMRLUP: SETMRL CCC interrupt flag
      \arg        I3C_INT_FLAG_SETMWLUP: SETMWL CCC interrupt flag
      \arg        I3C_INT_FLAG_DAAUP: ENTDAA/RSTDAA/SETNEWDA CCC interrupt flag
      \arg        I3C_INT_FLAG_GETSTATUS: GETSTATUS CCC format 1 interrupt flag
      \arg        I3C_INT_FLAG_GET: GETxxx CCC interrupt flag
      \arg        I3C_INT_FLAG_WUP: wake-up interrupt flag
      \arg        I3C_INT_FLAG_IBITC: IBI completed interrupt flag
      \arg        I3C_INT_FLAG_ERR: error interrupt flag
      \arg        I3C_INT_FLAG_FRMC: frame completed interrupt flag
      \arg        I3C_INT_FLAG_RFNE: RX-FIFO not empty interrupt flag
      \arg        I3C_INT_FLAG_TFNF: TX-FIFO not full interrupt flag
      \arg        I3C_INT_FLAG_ERR_OUERR: error interrupt and overrun/underrun error flag
      \arg        I3C_INT_FLAG_ERR_STALLERR: error interrupt and SCL stall error flag
      \arg        I3C_INT_FLAG_ERR_BERR: error interrupt and bus protocol error flag
    \param[out] none
    \retval     none
*/
void i3c_interrupt_flag_clear(i3c_interrupt_flag_enum int_flag)
{
    if((I3C_INT_FLAG_ERR_OUERR == int_flag) || (I3C_INT_FLAG_ERR_STALLERR == int_flag) ||
            (I3C_INT_FLAG_ERR_BERR == int_flag)) {
        I3C_STATC |= BIT(I3C_BIT_POS2(I3C_INT_ERR));
    } else {
        I3C_STATC |= BIT(I3C_BIT_POS2(int_flag));
    }
}

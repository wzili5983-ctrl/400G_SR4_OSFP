/**
 *
 * @file     tunnel_def.h
 * @author   
 * @date     10/30/2019
 * @version  1.0
 *
 * @property  $Copyright: Copyright 2018 Broadcom INC.
 * This program is the proprietary software of Broadcom INC
 * and/or its licensors, and may only be used, duplicated, modified
 * or distributed pursuant to the terms and conditions of a separate,
 * written license agreement executed between you and Broadcom
 * (an "Authorized License").  Except as set forth in an Authorized
 * License, Broadcom grants no license (express or implied), right
 * to use, or waiver of any kind with respect to the Software, and
 * Broadcom expressly reserves all rights in and to the Software
 * and all intellectual property rights therein.  IF YOU HAVE
 * NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE
 * IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE
 * ALL USE OF THE SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1.     This program, including its structure, sequence and organization,
 * constitutes the valuable trade secrets of Broadcom, and you shall use
 * all reasonable efforts to protect the confidentiality thereof,
 * and to use this information only in connection with your use of
 * Broadcom integrated circuit products.
 *
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS
 * PROVIDED "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
 * REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
 * OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
 * DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
 * NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
 * ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
 * OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL
 * BROADCOM OR ITS LICENSORS BE LIABLE FOR   CONSEQUENTIAL,
 * INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER
 * ARISING OUT OF OR IN ANY WAY RELATING TO YOUR USE OF OR INABILITY
 * TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF
 * THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR USD 1.00,
 * WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING
 * ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.$
 *
 * @brief   This file contains CAPI related types, etc.
 *
 * @section
 * 
 */
/**
 *       Brcm Tunnelig definition file 
 *
*/

#define  TUNNELING_ACCESS 1
typedef enum tunnel_return_result_e {
    TUNNEL_RR_ERROR = -255,          /*!< Error                                               */
    TUNNEL_WRITE_WAIT,               /*!< Tunnel write in process and need to wait            */
    TUNNEL_WRITE_FAILED,             /*!< Tunnel write Failed                                 */
    TUNNEL_READ_WAIT,                /*!< Tunnel read in process and need to wait             */
    TUNNEL_READ_FAILED,              /*!< Tunnel read Failed                                  */
    TUNNEL_RR_SUCCESS = 0            /*!< Success                                             */
                                     /*!< could add more error msg                            */
} tunnel_return_result_t; /*!< Retur R */



/**
 * @brief    tunnel_return_result_t tunnel_b_write(uint32_t start_address, uint16_t len, uint8_t* wr_data_ptr);
 *                               
 * @details  This API is used for tunneling write and it is blocking function 
 *
 * @param[in]  start_address: starting DSP 32bit address for DSP register or memory access 
 * @param[in]  len:           number of bytes to write to DSP via tunneling
 * @param[in]  wr_data_ptr:   pointer to write  data
 * 
 * @return     returns the  result of the function 
 */
tunnel_return_result_t tunnel_b_write(uint32_t start_address, uint16_t len, uint8_t* wr_data_ptr);

/**
 * @brief    tunnel_return_result_t tunnel_write_request(uint32_t start_address, uint16_t len, uint8_t* wr_data_ptr);
 *                               
 * @details  This API is used for issuing tunneling write request and it is non-blocking function 
 *
 * @param[in]  start_address: starting DSP 32bit address for DSP register or memory access 
 * @param[in]  len:           number of bytes to write to DSP via tunneling
 * @param[in]  wr_data_ptr:   pointer to write  data
 * 
 * @return     returns the  result of the function 
 */
tunnel_return_result_t tunnel_write_request(uint32_t start_address, uint16_t len, uint8_t* wr_data_ptr);
/**
 * @brief   tunnel_return_result_t tunnel_write_poll_ack(uint32_t start_address, uint16_t len);
 *                               
 * @details  This API is used for polling tunneling write ack and it is non-blocking function 
 *
 * @param[in]  start_address: starting DSP 32bit address for DSP register or memory access 
 * @param[in]  len:           number of bytes to write to DSP via tunneling
 * 
 * @return     returns the  result of the function whether write action is success, failed or need to wait 
 */
tunnel_return_result_t tunnel_write_poll_ack(uint32_t start_address, uint16_t len);

/**
 * @brief   tunnel_return_result_t tunnel_b_read(uint32_t start_address, uint16_t len, uint8_t* rd_data_ptr);
 *                               
 * @details  This API is used for tunneling read and it is blocking function 
 *
 * @param[in]  start_address: starting DSP 32bit address for DSP register or memory access 
 * @param[in]  len:           number of bytes to read from DSP via tunneling
 * @param[in]  rd_data_ptr:   pointer to read  data
 * 
 * @return     returns the  result of the function 
 */
tunnel_return_result_t tunnel_b_read(uint32_t start_address, uint16_t len, uint8_t* rd_data_ptr);

/**
 * @brief    tunnel_read_request(uint32_t start_address, uint16_t len);
 *                               
 * @details  This API is used for issuing tunneling read request and it is non-blocking function 
 *
 * @param[in]  start_address: starting DSP 32bit address for DSP register or memory access  
 * @param[in]  len:           number of bytes to read from DSP via tunneling
 * 
 * @return     returns the  result of the function 
 */
tunnel_return_result_t tunnel_read_request(uint32_t start_address, uint16_t len);
/**
 * @brief  tunnel_return_result_t tunnel_read_poll_ack(uint32_t start_address, uint16_t len, uint8_t* rd_data_ptr);
 *                               
 * @details  This API is used for polling tunneling read ack and it is non-blocking function 
 *
 * @param[in]  start_address: starting DSP 32bit address for DSP register or memory access 
 * @param[in]  len:           number of bytes to read from DSP via tunneling
 * @param[in]  rd_data_ptr:   pointer to read  data
 * @return     returns the  result of the function whether read action is success, failed or need to wait 
 */
tunnel_return_result_t tunnel_read_poll_ack(uint32_t s32tart_address, uint16_t len, uint8_t* rd_data_ptr);



/**
 *
 * @file     tunnel_access.c
 * @author   BRCM HSIP Firmware Team
 * @date     4/01/2019
 * @version  1.0
 *
 * @property  $Copyright: Copyright 2019 Broadcom INC.
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
 * @brief   This file contains the implementation of PHY register read/write with FTDI USB driver.
 *
 * @section
 * 
 */


#include "MON2_HSIP_ENG.h"
#include "hr_time.h"
#include "logger.h"
#include "reg_access.h"

#include "tunnel_def.h"
#include "common_util.h"


uint32_t passed_port = 0;
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
tunnel_return_result_t tunnel_b_read(uint32_t start_address, uint16_t len, uint8_t* rd_data_ptr){
    uint32_t data =0;
    data =sal_rdEst(passed_port, start_address);
    util_memcpy(rd_data_ptr, &data, 4);
    return TUNNEL_RR_SUCCESS;

}
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
tunnel_return_result_t tunnel_b_write(uint32_t start_address, uint16_t len, uint8_t* wr_data_ptr){

   sal_wrEst(passed_port, start_address, (unsigned int)(*(uint32_t*)wr_data_ptr));

   return TUNNEL_RR_SUCCESS;
}

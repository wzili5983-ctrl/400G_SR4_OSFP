
/**
 *
 * @file ml_cw_rtmr_modes.h
 * @author  
 * @date     09/01/2018
 * @version 1.0
 *
 * $Copyright: Copyright 2018 Broadcom INC.
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
 * 1.      This program, including its structure, sequence and organization,
 * constitutes the valuable trade secrets of Broadcom, and you shall use
 * all reasonable efforts to protect the confidentiality thereof,
 * and to use this information only in connection with your use of
 * Broadcom integrated circuit products.
 * 
 * 2.      TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS
 * PROVIDED "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
 * REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
 * OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
 * DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
 * NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
 * ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
 * OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 * 
 * 3.      TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL
 * BROADCOM OR ITS LICENSORS BE LIABLE FOR  CONSEQUENTIAL,
 * INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER
 * ARISING OUT OF OR IN ANY WAY RELATING TO YOUR USE OF OR INABILITY
 * TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF
 * THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR USD 1.00,
 * WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING
 * ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.$
 *
 * @brief   this file includes CW retimer mode config util functions.
 *
 * @section
 * 
 */


#ifndef ML_CW_RTMR_MODES_H
#define ML_CW_RTMR_MODES_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief          ml_cw_rtmr_is_A1_4p_port(cw_mode_parameter_t* cw_mode_ptr)
 * @details    Is the port is lane 4-7 configured as port independent mode for A1 chip
 * @public        any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]     cw_mode_ptr
 * @return TRUE: port is lane 4-7 configured as port independent mode for A1 chip, FALSE: otherwise 
 */

bool ml_cw_rtmr_is_A1_4p_port(cw_mode_parameter_t* cw_mode_ptr);

/**
 * @brief          ml_cw_rtmr_get_base_addr(cfg_egr_or_igr_t egr_or_igr, cw_mode_parameter_t* cw_mode_ptr)
 * @details    Get CW retimer base addr
 * @public        any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]     egr_or_igr  
 * @param[in]     cw_mode_ptr
 * @return CW retimer base addr
 */

uint32_t ml_cw_rtmr_get_base_addr(cfg_egr_or_igr_t egr_or_igr, cw_mode_parameter_t* cw_mode_ptr);

/**
 * @brief          ml_cw_rtmr_gen_cw_mode_param(cconst cw_port_entry_ex_t *port_entry_ptr, uint8_t port_index, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t * cur_port_config_ptr, cw_mode_parameter_t *cw_mode_ptr)
 * @details    Generate the CW mode param information
 * @public        any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]     port_entry_ptr
 * @param[in]     port_index
 * @param[in]     egr_or_igr
 * @param[out]     cur_port_config_ptr
 * @param[out]     cw_mode_ptr
 * @return void
 */

void ml_cw_rtmr_gen_cw_mode_param(const cw_port_entry_ex_t *port_entry_ptr, uint8_t port_index, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t * cur_port_config_ptr, cw_mode_parameter_t *cw_mode_ptr);


#ifdef __cplusplus
}
#endif

#endif /**< ML_CW_RTMR_MODES_H*/


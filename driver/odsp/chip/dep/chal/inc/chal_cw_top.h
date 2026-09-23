/**
 *
 * @file chal_quad_core.h 
 * @author  
 * @date     9/1/2018
 * @version 1.0
 *
 *  * @property   $Copyright: Copyright 2018 Broadcom INC. 
 *This program is the proprietary software of Broadcom INC
 *and/or its licensors, and may only be used, duplicated, modified
 *or distributed pursuant to the terms and conditions of a separate,
 *written license agreement executed between you and Broadcom
 *(an "Authorized License").  Except as set forth in an Authorized
 *License, Broadcom grants no license (express or implied), right
 *to use, or waiver of any kind with respect to the Software, and
 *Broadcom expressly reserves all rights in and to the Software
 *and all intellectual property rights therein.  IF YOU HAVE
 *NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE
 *IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE
 *ALL USE OF THE SOFTWARE.
 *
 *Except as expressly set forth in the Authorized License,
 *
 *1.     This program, including its structure, sequence and organization,
 *constitutes the valuable trade secrets of Broadcom, and you shall use
 *all reasonable efforts to protect the confidentiality thereof,
 *and to use this information only in connection with your use of
 *Broadcom integrated circuit products.
 *
 *2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS
 *PROVIDED "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
 *REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
 *OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
 *DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
 *NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
 *ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 *CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
 *OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 *3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL
 *BROADCOM OR ITS LICENSORS BE LIABLE FOR  CONSEQUENTIAL,
 *INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER
 *ARISING OUT OF OR IN ANY WAY RELATING TO YOUR USE OF OR INABILITY
 *TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF THE
 *POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF
 *THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR USD 1.00,
 *WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING
 *ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.$
 * @brief   this file includes cHAL function implementation.
 *
 * @section
 * 
 */

#ifndef CHAL_CW_TOP_H
#define CHAL_CW_TOP_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief      chal_cw_tx_data_select(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,
 *                                     cfg_dp_type_t dp_type, cfg_egr_or_igr_t egr_or_igr, uint8_t is_disable) 
 * @details    TX data is muxed between retimer and repeater databus. Choose corresponding data based on repeater or retimer 
 * @public     any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 * @param[in]  phy_info_ptr device base address pointer 
 * @param[in]  cfg_dp_type_t :  repeater or retimer
 * @param[in]  cfg_host_or_line_t :  host or line
 * @return enum return_result_t
 */
return_result_t chal_cw_tx_data_select(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, 
                                       cfg_dp_type_t dp_type, cfg_egr_or_igr_t egr_or_igr, uint8_t is_disable);


/**
 * @brief return_result_t chal_cw_release_rtm_reset(phy_info_t* phy_info_ptr)
 * @details  Release retimer reset
 * @public   
 * @private 
 * @param  phy_info_ptr
 * @return enum return_result_t
 */
return_result_t chal_cw_release_rtm_reset(phy_info_t* phy_info_ptr);

/**
 * @brief return_result_t chal_cw_release_egr_reset(phy_info_t* phy_info_ptr, cfg_dp_type_t dp_type)
 * @details  Release EGR reset
 * @public   
 * @private 
 * @param  phy_info_ptr
 * @param  retimer/repeater datapath
 * @return enum return_result_t
 */
return_result_t chal_cw_release_egr_reset(phy_info_t* phy_info_ptr, cfg_dp_type_t dp_type);

/**
 * @brief return_result_t chal_cw_release_igr_reset(phy_info_t* phy_info_ptr, cfg_dp_type_t dp_type)
 * @details  Release IGR reset
 * @public   
 * @private 
 * @param  phy_info_ptr
 * @param  retimer/repeater datapath
 * @return enum return_result_t
 */
return_result_t chal_cw_release_igr_reset(phy_info_t* phy_info_ptr, cfg_dp_type_t dp_type);

#ifdef __cplusplus
}
#endif

#endif /*CHAL_CW_TOP_H*/



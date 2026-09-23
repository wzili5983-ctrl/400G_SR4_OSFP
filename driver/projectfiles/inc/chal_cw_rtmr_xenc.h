/**
 *
 * @file chal_xenc.h 
 * @author  
 * @date     09/01/2018
 * @version 1.0
 *
  * @property   $Copyright: Copyright 2018 Broadcom INC. 
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
 * @brief   this file includes cHAL function implementation for fec_sync .
 *
 * @section
 * 
 */

#ifndef CHAL_XENC_H
#define CHAL_XENC_H

/**
 * @brief  chal_cw_rtmr_xenc_width_ctrl (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] cur_port_config_ptr
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_xenc_width_ctrl (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr);


/**
 * @brief   chal_cw_rtmr_xenc_gbox_gap (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, uint8_t xenc_gap)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] cur_port_config_ptr
 * @param[in] xenc_gap
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_xenc_gbox_gap (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, uint8_t xenc_gap );

/**
 * @brief    chal_cw_rtmr_xenc_dscr_ctrl (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] cur_port_config_ptr
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_xenc_dscr_ctrl (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr);

/**
 * @brief    chal_cw_rtmr_xenc_gbox_release_reset(phy_info_t* phy_info_ptr,  cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] cur_port_config_ptr
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_xenc_gbox_release_reset(phy_info_t* phy_info_ptr,  cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr);

/**
 * @brief    chal_cw_rtmr_get_xenc_gbox_collision_sts (phy_info_t* phy_info_ptr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @return enum uint8_t int
*/
uint8_t chal_cw_rtmr_get_xenc_gbox_collision_sts (phy_info_t* phy_info_ptr);


/**
 * @brief     chal_cw_rtmr_clr_xenc_gbox_collision_sts (phy_info_t* phy_info_ptr, cfg_rtm_slice_t cur_rtm_slice)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_rtm_slice
 * @return enum return result_t  
*/
return_result_t chal_cw_rtmr_clr_xenc_gbox_collision_sts (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_rtm_slice_t cur_rtm_slice);

/**
 * @brief     uint8_t chal_cw_rtmr_get_xenc_gbox_error_sts (phy_info_t* phy_info_ptr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @return enum return result_t  
*/
uint8_t chal_cw_rtmr_get_xenc_gbox_error_sts (phy_info_t* phy_info_ptr);


/**
 * @brief      chal_cw_rtmr_clr_xenc_gbox_error_sts(phy_info_t* phy_info_ptr, cfg_rtm_slice_t cur_rtm_slice)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_rtm_slice
 * @return enum return result_t  
*/
return_result_t chal_cw_rtmr_clr_xenc_gbox_error_sts (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_rtm_slice_t cur_rtm_slice);

/**
 * @brief      chal_cw_rtmr_xenc_scr_ctrl (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] cur_port_config_ptr
 * @return enum return result_t  
*/
return_result_t chal_cw_rtmr_xenc_scr_ctrl (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr);

#endif

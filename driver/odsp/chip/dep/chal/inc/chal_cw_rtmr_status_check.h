/**
 *
 * @file     chal_cw_rtmr_status_check.h
 * @author   
 * @date     10/2/2020
 * @version  1.0
 *
 * @property 
 * $Copyright: (c) 2020 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * @brief    brief description of the block source file, sequences/flow/etc.
 *
 * @section  description
 *
 */
#ifndef CHAL_CW_RTMR_STATUS_CHECK_H
#define CHAL_CW_RTMR_STATUS_CHECK_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  chal_cw_rtmr_fec_sync_status(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr);
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] cur_port_config_ptr
 * @return enum uint16_t
*/
uint16_t chal_cw_rtmr_fec_sync_status(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr);

/**
 * @brief  chal_cw_rtmr_pcs_sync_get_status(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr);
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] cur_port_config_ptr
 * @return enum uint16_t
*/
uint16_t chal_cw_rtmr_pcs_sync_get_status(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr);

uint16_t chal_cw_rtmr_tmt_pfifo_get_lane_mask(cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr);


#ifdef __cplusplus
}
#endif

#endif /**< CHAL_CW_RTMR_STATUS_CHECK_H */


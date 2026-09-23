/**
 *
 * @file     chal_cw_rtmr_clockrst_mux.h 
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

#ifndef CHAL_CW_RTMR_CLOCKRST_MUX_H
#define CHAL_CW_RTMR_CLOCKRST_MUX_H

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief  chal_cw_rtmr_tmt_pfifo_clk_mux_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] cur_port_config_ptr
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_tmt_pfifo_clk_mux_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief  chal_cw_rtmr_rcv_pfifo_clk_mux_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] cur_port_config_ptr
 * @param[in] direction
 * @return enum return result_t 
*/

return_result_t chal_cw_rtmr_rcv_pfifo_clk_mux_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief  chal_cw_rtmr_fec_rcv_clk_rst_mux_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,  cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] cur_port_config_ptr
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_fec_rcv_clk_rst_mux_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,  cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief  chal_cw_rtmr_fec_rcv_clk66_rst_mux_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,  cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] cur_port_config_ptr
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_fec_rcv_clk66_rst_mux_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,  cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief  chal_cw_rtmr_pcs_rcv_clk_rst_mux_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,  cw_port_config_t* cur_port_config_ptr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] cur_port_config_ptr
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_pcs_rcv_clk_rst_mux_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,  cw_port_config_t* cur_port_config_ptr);

/**
 * @brief  chal_cw_rtmr_fec_tmt_clk_rst_mux_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,  cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] cur_port_config_ptr
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_fec_tmt_clk_rst_mux_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,  cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief  chal_cw_rtmr_pcs_tmt_cdr_clk_rst_mux_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,  cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] cur_port_config_ptr
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_pcs_tmt_cdr_clk_rst_mux_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,  cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief  chal_cw_rtmr_pcs_tmt_cmu_clk_rst_mux_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,  cw_port_config_t* cur_port_config_ptr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] cur_port_config_ptr
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_pcs_tmt_cmu_clk_rst_mux_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,  cw_port_config_t* cur_port_config_ptr);



/**
 * @brief  chal_cw_rtmr_fec_rcv_tmt_clk_mux_cfg(phy_info_t* phy_info_ptr,  cw_mode_parameter_t* cur_mode_parameter_ptr, uint8_t enable, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] cur_port_config_ptr
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_fec_rcv_tmt_clk_mux_cfg(phy_info_t* phy_info_ptr,  cw_mode_parameter_t* cur_mode_parameter_ptr, uint8_t enable, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief  chal_cw_rtmr_pcs_rcv_tmt_clk_mux_cfg(phy_info_t* phy_info_ptr,  cw_mode_parameter_t* cur_mode_parameter_ptr, uint8_t enable, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] cur_port_config_ptr
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_pcs_rcv_tmt_clk_mux_cfg(phy_info_t* phy_info_ptr,  cw_mode_parameter_t* cur_mode_parameter_ptr, uint8_t enable, cfg_egr_or_igr_t egr_or_igr);




/**
 * @brief   chal_cw_rtmr_cmu2cdr_lane_map_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_cmu2cdr_lane_map_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr);


/**
 * @brief   util_get_lowest_index_from_mask (uint16_t lane_mask)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] lane_mask
 * @return uint8
*/
uint8_t util_get_lowest_index_from_mask (uint16_t lane_mask);

/**
 * @brief  chal_cw_rtmr_tmt_pgen_clk_mux_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, bool lane_pgen_en);
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] lane_pgen_en
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_tmt_pgen_clk_mux_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr, bool lane_pgen_en) ;

/**
 * @brief  chal_cw_rtmr_rcv_pmon_clk_mux_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, bool lane_pmon_en);
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] lane_pmon_en
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_rcv_pmon_clk_mux_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr, bool lane_pmon_en);

/**
 * @brief  chal_cw_rtmr_fec_rcv_tmt_clk66_mux_cfg(phy_info_t* phy_info_ptr,  cw_mode_parameter_t* cur_mode_parameter_ptr, uint8_t enable, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] lane_pmon_en
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_fec_rcv_tmt_clk66_mux_cfg(phy_info_t* phy_info_ptr,  cw_mode_parameter_t* cur_mode_parameter_ptr, uint8_t enable, cfg_egr_or_igr_t egr_or_igr);

#ifdef __cplusplus
}
#endif

#endif /* CHAL_CW_RTMR_CLOCKRST_MUX_H */

/**
 *
 * @file     chal_cw_rtmr_clkrst_control.h 
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

#ifndef CHAL_CW_RTMR_CLKRST_CONTROL_H
#define CHAL_CW_RTMR_CLKRST_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  chal_cw_rtmr_pcs_clk_gate_control(phy_info_t* phy_info_ptr,  cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_host_or_line_t host_or_line, cfg_egr_or_igr_t egr_or_igr, enabled_t enable)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] host_or_line
 * @param[in] direction
 * @param[in] enable
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_pcs_clk_gate_control(phy_info_t* phy_info_ptr,  cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_host_or_line_t host_or_line, cfg_egr_or_igr_t egr_or_igr, enabled_t enable);

/**
 * @brief  chal_cw_rtmr_fec_clk_gate_control(phy_info_t* phy_info_ptr,  cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_host_or_line_t host_or_line, cfg_egr_or_igr_t egr_or_igr, enabled_t enable)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] host_or_line
 * @param[in] direction
 * @param[in] enable
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_fec_clk_gate_control(phy_info_t* phy_info_ptr,  cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_host_or_line_t host_or_line, cfg_egr_or_igr_t egr_or_igr, enabled_t enable);

/**
 * @brief  chal_cw_rtmr_pcs_reset_control(phy_info_t* phy_info_ptr,  cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_host_or_line_t host_or_line, cfg_egr_or_igr_t egr_or_igr, rst_assert_t reset)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] host_or_line
 * @param[in] direction
 * @param[in] reset
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_pcs_reset_control(phy_info_t* phy_info_ptr,  cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_host_or_line_t host_or_line, cfg_egr_or_igr_t egr_or_igr, rst_assert_t reset);

/**
 * @brief  chal_cw_rtmr_fec_reset_control(phy_info_t* phy_info_ptr,  cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_host_or_line_t host_or_line, cfg_egr_or_igr_t egr_or_igr, rst_assert_t reset)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] host_or_line
 * @param[in] direction
 * @param[in] reset
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_fec_reset_control(phy_info_t* phy_info_ptr,  cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_host_or_line_t host_or_line, cfg_egr_or_igr_t egr_or_igr, rst_assert_t reset);

/**
 * @brief  chal_cw_rtmr_rcv_gapclk_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] enable
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_rcv_gapclk_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief  chal_cw_rtmr_tmt_gapclk_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] enable
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_tmt_gapclk_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief  chal_cw_rtmr_top_reset_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] reset
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_top_reset_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr);


/**
 * @brief chal_cw_rtmr_rcv_pfifo_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr)
 
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] enable
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_rcv_pfifo_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief chal_cw_rtmr_pcs_sync_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr)
 
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] enable
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_pcs_sync_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief chal_cw_rtmr_fec_sync_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr)
 
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] enable
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_fec_sync_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr);


/**
 * @brief  chal_cw_rtmr_rcv_pfifo_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] reset
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_rcv_pfifo_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief  chal_cw_rtmr_rcv_gbox_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] reset
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_rcv_gbox_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief  chal_cw_rtmr_fec_sync_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] reset
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_fec_sync_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief  chal_cw_rtmr_pcs_sync_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] reset
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_pcs_sync_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief  chal_cw_rtmr_fec_dec_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] reset
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_fec_dec_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief  chal_cw_rtmr_fec_dec_err_cnt_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, uint8_t enable, cfg_egr_or_igr_t egr_or_igr);
 * @details this chal can be used to keep FEC counters when FEC decoder is reset.
            enable = 1: FEC counters are NOT cleared when FEC decoder is reset.
            enable = 0: FEC counters are cleared when FEC decoder is reset.
 * 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] enable
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_fec_dec_err_cnt_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, uint8_t enable, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief chal_cw_rtmr_krkp_gbox_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr)
 
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] enable
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_krkp_gbox_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable);

/**
 * @brief  chal_cw_rtmr_krkp_gbox_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] reset
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_krkp_gbox_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief chal_cw_rtmr_xdec_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr)
 
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] enable
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_xdec_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, enabled_t enable);

/**
 * @brief  chal_cw_rtmr_xdec_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] reset
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_xdec_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief chal_cw_rtmr_xenc_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr)
 
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] enable
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_xenc_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, enabled_t enable);

/**
 * @brief  chal_cw_rtmr_xenc_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] reset
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_xenc_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief chal_cw_rtmr_xenc_am_ins_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr)
 
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] enable
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_xenc_am_ins_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, enabled_t enable);

/**
 * @brief chal_cw_rtmr_fec_enc_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr)
 
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] enable
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_fec_enc_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief chal_cw_rtmr_fec_symb_dist_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr)
 
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] enable
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_fec_symb_dist_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr);


/**
 * @brief chal_cw_rtmr_pcs_tmt_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr)
 
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] enable
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_pcs_tmt_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr);


/**
 * @brief  chal_cw_rtmr_tmt_pfifo_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] reset
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_tmt_pfifo_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief chal_cw_rtmr_tmt_pfifo_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr)
 
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] enable
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_tmt_pfifo_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr);


/**
 * @brief  chal_cw_rtmr_fec_enc_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] reset
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_fec_enc_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief  chal_cw_rtmr_fec_symb_dist_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] reset
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_fec_symb_dist_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief  chal_cw_rtmr_pcs_tmt_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] reset
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_pcs_tmt_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr);



/**
 * @brief  chal_cw_rtmr_tmt_gbox_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] reset
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_tmt_gbox_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr);


/**
 * @brief chal_cw_rtmr_fec_pgen_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr)
 
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] enable
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_fec_pgen_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr);


/**
 * @brief chal_cw_rtmr_fec_pmon_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr)
 
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] enable
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_fec_pmon_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr);



/**
 * @brief chal_cw_rtmr_pcs_pgen_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr)
 
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] enable
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_pcs_pgen_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr);


/**
 * @brief chal_cw_rtmr_pcs_pmon_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr)
 
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] enable
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_pcs_pmon_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr);


/**
 * @brief chal_cw_rtmr_fec_pgen_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,  rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr)
 
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] reset
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_fec_pgen_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,  rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr);


/**
 * @brief chal_cw_rtmr_fec_pmon_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,  rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr)
 
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] reset
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_fec_pmon_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,  rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr);



/**
 * @brief chal_cw_rtmr_pcs_pgen_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,  rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr)
 
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] reset
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_pcs_pgen_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,  rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr);


/**
 * @brief chal_cw_rtmr_pcs_pmon_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,  rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr)
 
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] reset
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_pcs_pmon_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,  rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr);

return_result_t chal_cw_rtmr_fec_dec_err_cnt_asynch_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr);






#ifdef __cplusplus
}
#endif

#endif /* CHAL_CW_RTMR_CLKRST_CONTROL_H */

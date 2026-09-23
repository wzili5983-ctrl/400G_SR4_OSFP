/**
 *
 * @file     chal_cw_rtmr_datapath_cfg.h 
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

#ifndef CHAL_CW_RTMR_DATAPATH_CFG_H
#define CHAL_CW_RTMR_DATAPATH_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   cha_cw_rtmr_din_off(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr,  enabled_t enable)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t cha_cw_rtmr_din_off (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr, enabled_t enable);

/**
 * @brief   chal_cw_rtmr_rcv_pfifo_gbox_corr_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr,  enabled_t enable)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_rcv_pfifo_gbox_corr_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable);

/**
 * @brief   chal_cw_rtmr_tmt_pfifo_gbox_corr_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr,  enabled_t enable)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_tmt_pfifo_gbox_corr_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable);

/**
 * @brief   chal_cw_rtmr_rcv_pfifo_gbox_mode_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_rcv_pfifo_gbox_mode_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr);

return_result_t chal_cw_rtmr_rcv_pfifo_gbox_wrmux_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief   chal_cw_rtmr_rcv_pfifo_gbox_ob_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_rcv_pfifo_gbox_ob_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief   chal_cw_rtmr_rcv_pfifo_gbox_ib_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_rcv_pfifo_gbox_ib_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief   chal_cw_rtmr_rcv_pfifo_gbox_bundle_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_rcv_pfifo_gbox_bundle_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr);


/**
 * @brief   chal_cw_rtmr_tmt_pfifo_gbox_mode_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_tmt_pfifo_gbox_mode_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr);

return_result_t chal_cw_rtmr_tmt_pfifo_gbox_rdmux_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief   chal_cw_rtmr_tmt_pfifo_gbox_ob_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_tmt_pfifo_gbox_ob_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief   chal_cw_rtmr_tmt_pfifo_gbox_ib_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_tmt_pfifo_gbox_ib_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief   chal_cw_rtmr_tmt_pfifo_gbox_bundle_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_tmt_pfifo_gbox_bundle_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief  chal_cw_rtmr_rcv_pfifo_gbox_mode_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_rcv_pfifo_gbox_mode_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief  chal_cw_rtmr_rcv_pfifo_gbox_doutmux_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_rcv_pfifo_gbox_doutmux_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief  chal_cw_rtmr_tmt_pfifo_gbox_mode_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_tmt_pfifo_gbox_mode_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief  chal_cw_rtmr_tmt_pfifo_gbox_dinmux_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr,cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_tmt_pfifo_gbox_dinmux_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief  chal_cw_rtmr_tmt_pfifo_gbox_doutmux_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, uint8_t dsel_pcs_fecb, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] pcs_fecb 
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_tmt_pfifo_gbox_doutmux_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, uint8_t dsel_pcs_fecb, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief  chal_cw_rtmr_fec_rcv_datamux_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_fec_rcv_datamux_t fec_rcv_datamux)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] cur_port_config_ptr
 * @param[in] fec_rcv_datamux
 * @return enum return result_t 
*/
return_result_t  chal_cw_rtmr_fec_rcv_datamux_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_fec_rcv_datamux_t fec_rcv_datamux);

/**
 * @brief  cw_rtmr_fec_rcv_datamux_config_slice(phy_info_t* phy_info_ptr, cfg_fec_rcv_datamux_t cur_fec_rcv_datamux, cfg_egr_or_igr_t egr_or_igr, cfg_rtm_slice_t cur_rtm_slice);
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_fec_rcv_datamux
 * @param[in] egr_or_igr
 * @param[in] cur_rtm_slice
 * @return enum return result_t 
*/
return_result_t cw_rtmr_fec_rcv_datamux_config_slice(phy_info_t* phy_info_ptr, cfg_fec_rcv_datamux_t cur_fec_rcv_datamux, cfg_rtm_slice_t cur_rtm_slice);

/**
 * @brief  chal_cw_rtmr_fec_enc_datamux_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_fec_enc_datamux_t datamux_sel)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] cur_port_config_ptr
 * @param[in] datamux_sel
 * @return enum return result_t 
*/
return_result_t  chal_cw_rtmr_fec_enc_datamux_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_fec_enc_datamux_t datamux_sel);


/**
 * @brief  cw_rtmr_fec_enc_datamux_config_slice(phy_info_t* phy_info_ptr, cfg_fec_enc_datamux_t cur_fec_enc_datamux, cfg_egr_or_igr_t egr_or_igr, cfg_rtm_slice_t cur_rtm_slice);
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_fec_enc_datamux
 * @param[in] egr_or_igr
 * @param[in] cur_rtm_slice
 * @return enum return result_t 
*/
return_result_t cw_rtmr_fec_enc_datamux_config_slice(phy_info_t* phy_info_ptr, cfg_fec_enc_datamux_t cur_fec_enc_datamux,  cfg_rtm_slice_t cur_rtm_slice);

/**
 * @brief  chal_cw_rtmr_xenc_datamux_cfg(phy_info_t* phy_info_ptr,   cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, uint8_t dsel_pcs_xdecb)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] cur_port_config_ptr
 * @param[in] dsel_pcs_xdecb
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_xenc_datamux_cfg(phy_info_t* phy_info_ptr,   cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, uint8_t dsel_pcs_xdecb);

/**
 * @brief  chal_cw_rtmr_fec_mode_kr4_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] direction
 * @return enum return result_t 
*/
return_result_t  chal_cw_rtmr_fec_mode_kr4_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief  chal_cw_rtmr_dp_mode_mux_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr);
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] cur_port_config_ptr
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_dp_mode_mux_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr);

/**
 * @brief  chal_cw_rtmr_predec_bypass_deint_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, uint8_t bypass_fec_deint)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] bypass_fec_deint
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_predec_bypass_deint_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, uint8_t bypass_fec_deint);

/**
 * @brief  chal_cw_rtmr_kxky_gbox_speed_cfg(phy_info_t* phy_info_ptr,   cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] cur_port_config_ptr
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_kxky_gbox_speed_cfg(phy_info_t* phy_info_ptr,   cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr);

/**
 * @brief  chal_cw_rtmr_kxky_gbox_fec_cfg(phy_info_t* phy_info_ptr,   cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, uint8_t kr2kp_mode)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] cur_port_config_ptr
 * @param[in] kr2kp_mode
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_kxky_gbox_fec_cfg(phy_info_t* phy_info_ptr,   cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, uint8_t kr2kp_mode);

/**
 * @brief   chal_cw_rtmr_fec_enc_mux_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, uint8_t enc_bypass_mode)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] cur_port_config_ptr
 * @param[in] enc_bypass_mode
 * @return enum return result_t 
*/
return_result_t  chal_cw_rtmr_fec_enc_mux_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, uint8_t enc_bypass_mode);

/**
 * @brief  chal_cw_rtmr_fec_enc_pass_thru_uncorr_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr,  uint8_t enc_pass_thru_uncorr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] cur_port_config_ptr
 * @param[in] enc_pass_thru_uncorr
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_fec_enc_pass_thru_uncorr_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, uint8_t enc_pass_thru_uncorr);

/**
 * @brief  chal_cw_rtmr_fecdec_data_gating_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, uint8_t din_off)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] cur_port_config_ptr
 * @param[in] din_off
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_fecdec_data_gating_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr,  uint8_t din_off);

/**
 * @brief  chal_cw_rtmr_fecenc_data_gating_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, uint8_t din_off)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] cur_port_config_ptr
 * @param[in] din_off
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_fecenc_data_gating_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, uint8_t din_off);

/**
 * @brief   chal_cw_rtmr_fec_deskew_reorder_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* , cfg_egr_or_igr_t egr_or_igr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_mode_parameter_ptr
 * @param[in] cur_port_config_ptr
 * @param[in] egr_or_igr
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_fec_deskew_reorder_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr);
/**
 * @brief   cchal_cw_rtmr_ovr_am_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr)
 * @details 
 * @public any further information for the public domain 
 * @private any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional) 
 * @param[in] phy_info_ptr: phy info
 * @param[in] cur_port_config_ptr
 * @param[in] enable
 * @return enum return result_t 
*/
return_result_t chal_cw_rtmr_ovr_am_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr);

#ifdef __cplusplus
}
#endif

#endif /* CHAL_CW_RTMR_DATAPATH_CFG_H */

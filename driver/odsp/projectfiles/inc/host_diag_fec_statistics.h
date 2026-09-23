/**
 *
 * @file diag_fec_statistics.h 
 * @author  
 * @date     9/01/2018
 * @version 1.0
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
#ifndef DIAG_FEC_STATISTICS_H
#define DIAG_FEC_STATISTICS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  host_diag_cw_rtmr_kpr4fec_dec_stat_clr_all (const phy_info_t* phy_info_ptr)
 * @detail 
 * @public 
 * @private 
 * 
 * @param[in] phy_info_ptr: device base address
 * @param[in] port:  8-bit parameter for general purpose
 * @return return_result_t: returns result code
 */
return_result_t host_diag_cw_rtmr_kpr4fec_dec_stat_clr_all(const phy_info_t* phy_info_ptr);

/**
 * @brief  host_diag_cw_rtmr_kpr4fec_dec_stat_md_latch_all (const phy_info_t* phy_info_ptr)
 * @detail 
 * @public 
 * @private 
 * 
 * @param[in] phy_info_ptr: device base address
 * @param[in] port:  8-bit parameter for general purpose
 * @return return_result_t: returns result code
 */
return_result_t host_diag_cw_rtmr_kpr4fec_dec_stat_md_latch_all(const phy_info_t* phy_info_ptr);

/**
 * @brief  host_diag_cw_rtmr_kpr4fec_dec_stat_md_clr_all (const phy_info_t* phy_info_ptr)
 * @detail 
 * @public 
 * @private 
 * 
 * @param[in] phy_info_ptr: device base address
 * @param[in] port:  8-bit parameter for general purpose
 * @return return_result_t: returns result code
 */
return_result_t host_diag_cw_rtmr_kpr4fec_dec_stat_md_clr_all(const phy_info_t* phy_info_ptr);

/**
 * @brief  host_diag_cw_rtmr_kpr4fec_dec_stat_enable (const phy_info_t* phy_info_ptr)
 * @detail 
 * @public 
 * @private 
 * 
 * @param[in] phy_info_ptr: device base address
 * @param[in] port:  8-bit parameter for general purpose
 * @return return_result_t: returns result code
 */
return_result_t host_diag_cw_rtmr_kpr4fec_dec_stat_enable(const phy_info_t* phy_info_ptr);

/**
 * @brief  host_diag_cw_rtmr_kpr4fec_dec_stat_disable (const void* phy_info_ptr)
 * @detail 
 * @public 
 * @private 
 * 
     * @param[in] phy_info_ptr: device base address
 * @param[in] port:  8-bit parameter for general purpose
 * @return return_result_t: returns result code
 */
return_result_t host_diag_cw_rtmr_kpr4fec_dec_stat_disable(const phy_info_t* phy_info_ptr);

/**
 * @brief  host_diag_cw_rtmr_get_kpr4fec_dec_stat(const phy_info_t* phy_info_ptr, boolean *is_init)
 * @detail get FEC error cnt enable status
 * @public 
 * @private 
 * 
 * @param[in] bbaddr: device base address
 * @param[in] phy_info_ptr:  phy_info pointer; use KP4_KR4_FEC_DEC for base_addr
 * @return return_result_t: returns result code
 */
return_result_t host_diag_cw_rtmr_get_kpr4fec_dec_stat(const phy_info_t* phy_info_ptr, boolean *is_init);

/**
 * @brief  host_diag_cw_rtmr_kpr4fec_dec_cnt_get(const phy_info_t* phy_info_ptr, kpr4fec_cnt_s* cnt, cw_mode_parameter_t *cw_mode_ptr, uint8_t fec_mon_sampling_enabled, uint16_t* re_read_count_ptr)
 * @detail Gets all the BRCM FEC counter values (tot_frame_cnt, tot_frame_uncorr_cnt, tot_bits_corr_cnt[2], tot_frames_err_cnt[16])
 * @public 
 * @private 
 * 
 * @param[in] phy_info_ptr:  phy_info pointer; use KP4_KR4_FEC_DEC for base_addr
 * @param[in] cnt: reference to kpr4fec_cnt_s
 * @param[in] cw_mode_ptr : reference to cw_mode_parameter_t
 * @param[in] fec_mon_sampling_enabled : if fec monitor sampling is enabled
 * @param[in] re_read_count_ptr : second read count pointer
 * @return return_result_t: returns result code
 */
return_result_t host_diag_cw_rtmr_kpr4fec_dec_cnt_get(const phy_info_t* phy_info_ptr, kpr4fec_cnt_s* cnt, cw_mode_parameter_t *cw_mode_ptr, uint8_t fec_mon_sampling_enabled, uint16_t* re_read_count_ptr);

/**
* @brief      host_diag_cw_get_bbaddr_kp4deca(capi_fec_mode_t fec_mode_ptr, capi_function_mode_t func_mode_ptr, uint16_t port_idx)
* @details    This API is used to calculate the KP4 block based address
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  fec_mode_ptr:  FEC mode type
* @param[in]  func_mode_ptr: FUNCTION mode type
* @param[in]  port_idx: port index
* 
* @return     returns the performance result of the called methode/function
*/
uint32_t host_diag_cw_get_bbaddr_kp4deca(capi_fec_mode_t fec_mode_ptr, capi_function_mode_t func_mode_ptr, uint16_t port_idx, cw_mode_parameter_t* cw_mode_ptr);

/*
* @brief      host_diag_cw_get_chip_config(capi_phy_info_t* phy_info_ptr, capi_config_ex_info_t* config_info_ptr, cw_port_info_t* port_info_ptr)
* @details    This API is used to get chip config information
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  phy_info_ptr:    pointer to phy information
* @param[out]  config_info_ptr: pointer to chip config information
* @param[out]  port_idx: port index
*
* @return     returns the performance result of the called methode/function
*/
return_result_t host_diag_cw_get_chip_config(capi_phy_info_t* phy_info_ptr, capi_config_ex_info_t* config_info_ptr, cw_port_info_t* port_info_ptr);

return_result_t host_diag_cw_get_chip_config_fec_mon(capi_phy_info_t* phy_info_ptr, capi_config_ex_info_t* config_info_ptr, cw_port_info_t* port_info_ptr);

/**
* @brief      host_diag_cw_kp4_gen_port_cfg(capi_function_mode_t func_mode, uint16_t port_index, cw_port_config_t *port_cfg_ptr)
* @details    This funciton is used to decode CW port information
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  func_mode:
* @param[in]  port_index: 
* @param[in]  port_cfg_ptr: 
* 
* @return     returns the performance result of the called methode/function
*/
void host_diag_cw_kp4_gen_port_cfg(capi_function_mode_t func_mode, uint16_t port_index, cw_port_config_t *port_cfg_ptr);

/**
* @brief     host_diag_cw_kp4_gen_cw_mode_cfg(capi_config_ex_info_t* cfg_info, cw_port_info_t *port_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t *port_cfg_ptr,cw_mode_parameter_t *cw_mode_ptr)
* @details    This function is used to decode CW mode configuration
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  func_mode:
* @param[in]  port_index: 
* @param[in]  egr_or_igr: 
* @param[in]  port_cfg_ptr: 
* 
* @return    returns the performance result of the called methode/function
*/
void host_diag_cw_kp4_gen_cw_mode_cfg(capi_config_ex_info_t* cfg_info, cw_port_info_t *port_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t *port_cfg_ptr, cw_mode_parameter_t *cw_mode_ptr);

return_result_t host_diag_set_rptr_fec_monitor(capi_phy_info_t* phy_info_ptr, capi_rptr_fec_st_t* rptr_fec_st_ptr);

/**
 * @brief      host_diag_cw_fec_stat_init (capi_phy_info_t* phy_info_ptr, capi_fec_dump_status_t* capi_fec_dump_status_ptr,  capi_function_mode_t func_mode);
 * @details    This function is used to initialize the FEC statistics monitor
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: phy information structure pointer
 * @param[in]  capi_fec_dump_status_ptr: FEC status dump structure pointer
 * @param[in]  func_mode: FUNCTION mode type
 * 
 * @return       returns the performance result of the called methode/function
 */
return_result_t host_diag_cw_fec_stat_init (capi_phy_info_t* phy_info_ptr, capi_fec_dump_status_t* capi_fec_dump_status_ptr, capi_function_mode_t func_mode);

/**
 * @brief      host_diag_cw_fec_stat_deinit(capi_phy_info_t* phy_info_ptr, capi_fec_dump_status_t* capi_fec_dump_status_ptr,  capi_function_mode_t func_mode);
 * @details    This function is used to release the FEC statistics monitor
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: phy information structure pointer
 * @param[in]  capi_fec_dump_status_ptr: FEC status dump structure pointer
 * @param[in]  func_mode: FUNCTION mode type
 * 
 * @return       returns the performance result of the called methode/function
 */
return_result_t host_diag_cw_fec_stat_deinit (capi_phy_info_t* phy_info_ptr, capi_fec_dump_status_t* capi_fec_dump_status_ptr, capi_function_mode_t func_mode);

/**
 * @brief      host_diag_cw_fec_stat_clear (capi_phy_info_t* phy_info_ptr, capi_fec_dump_status_t* capi_fec_dump_status_ptr, capi_function_mode_t func_mode, boolean clr_st);
 * @details    This function is used to clear FEC statistics status
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: phy information structure pointer
 * @param[in]  capi_fec_dump_status_ptr: FEC status dump structure pointer
 * @param[in]  func_mode: FUNCTION mode type
 * 
 * @return       returns the performance result of the called methode/function
 */
return_result_t host_diag_cw_fec_stat_clear(capi_phy_info_t* phy_info_ptr, capi_fec_dump_status_t* capi_fec_dump_status_ptr, capi_function_mode_t func_mode, boolean clr_st);

/**
 * @brief      host_diag_cw_get_fec_info (capi_phy_info_t* phy_info_ptr, capi_fec_dump_status_t* capi_fec_dump_status_ptr, capi_function_mode_t func_mode);
 * @details    This function is used to get FEC statistics information
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: phy information structure pointer
 * @param[out]  capi_fec_dump_status_ptr: FEC status dump structure pointer
 * @param[in]  func_mode: FUNCTION mode type
 * 
 * @return       returns the performance result of the called methode/function
 */
return_result_t host_diag_cw_get_fec_info(capi_phy_info_t* phy_info_ptr, capi_fec_dump_status_t* capi_fec_dump_status_ptr, capi_function_mode_t func_mode);



/**
 * @brief      _get_cfg_list_array_idx(cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, diag_port_cfg_list_t *cfg_list_ptr);
 * @details    This function is used to get port operation index
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  cur_port_config_ptr: port config pointer
 * @param[in]  cur_mode_parameter_ptr: mode config pointer
 * @param[out]  cfg_list_ptr: config port list information
 * 
 * @return       returns
 */
void _get_cfg_list_array_idx(cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, diag_port_cfg_list_t *cfg_list_ptr);

/**
* @brief      host_diag_cw_calculate_ber_between_latch(capi_phy_info_t *phy_ptr, capi_kp4fec_ber_state_t* fec_ptr)
* @details    This API is used to calculate the min/max/average error frame and BER based on CMIS request
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  capi_fec_dump_status_ptr: this parameter pointer current hardware FEC counter 
* @param[in/out]  fec_ptr: this parameter pointer current fec counter post processing number; 
* 
* @return     returns the performance result of the called methode/function
*/
return_result_t host_diag_cw_calculate_ber_between_latch(capi_phy_info_t *phy_ptr,  capi_fec_dump_status_t* capi_fec_dump_status_ptr, capi_kp4fec_ber_state_t* fec_ptr);

return_result_t host_diag_cw_rptr_fec_mon_config_handler(phy_info_t* phy_info_ptr, capi_rptr_fec_st_t *rptr_fec_st);
uint8_t host_diag_cw_is_port_based_fec_mon(phy_info_t* phy_info_ptr);
return_result_t host_diag_cw_fec_ports_auto_detect_status (capi_phy_info_t* phy_info_ptr, capi_fec_auto_ports_detect_state_t* capi_fec_auto_ports_detect_status_ptr);

#ifdef __cplusplus
}
#endif

#endif

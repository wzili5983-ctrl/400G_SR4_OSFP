/**
 * @file     host_diag.h
 * @author  
 * @date     08-5-2019
 * @version 1.0
 *
 * @property
 * $Copyright: (c) 2019 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * @brief    brief description of the block source file, sequences/flow/etc.
 *
 * @section  description
 *
 */
#ifndef HOST_DIAG_H
#define HOST_DIAG_H


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief      host_lw_get_usr_diagnostics(capi_phy_info_t* phy_info_ptr, capi_usr_diag_info_t* usr_diag_info_ptr)
 * @details    This API is used to get user diag information for Line side <BR>
 *             when core_ip is CORE_IP_LW, the LW will report below parameters: <BR>
 *                 sigma and mu values <BR>
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  usr_diag_info_ptr: a pointer which carries detail user diag information
 *
 * @return     returns the performance result of the called method/function
 */
return_result_t host_lw_get_usr_diagnostics(capi_phy_info_t* phy_info_ptr, capi_usr_diag_info_t* usr_diag_info_ptr);

/**
* @brief      host_lw_get_usr_cmis_diagnostics(capi_phy_info_t* phy_info_ptr, capi_usr_diag_info_t* usr_diag_info_ptr)
* @details    This API is used to get the user cmis diag info
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  usr_diag_info_ptr: this parameter
*
* @return     returns the performance result of the called method/function
*/
return_result_t host_lw_get_usr_cmis_diagnostics(capi_phy_info_t* phy_info_ptr, capi_usr_diag_info_t* usr_diag_info_ptr);


/**
 * @brief      host_set_mpi_config(capi_phy_info_t*    phy_info_ptr, dsp_mpi_cfg_info_t *mpi_cfg_ptr)
 *
 * @details    get mpi configuration information
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      mpi_cfg_ptr      : mpi config data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_set_mpi_config(capi_phy_info_t*    phy_info_ptr, dsp_mpi_cfg_info_t *mpi_cfg_ptr);


/**
 * @brief      host_get_mpi_config(capi_phy_info_t*    phy_info_ptr, dsp_mpi_cfg_info_t *mpi_cfg_ptr)
 *
 * @details    get mpi configuration information
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      mpi_cfg_ptr      : mpi config data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_get_mpi_config(capi_phy_info_t*    phy_info_ptr, dsp_mpi_cfg_info_t *mpi_cfg_ptr);


/**
 * @brief      host_get_mpi_state(capi_phy_info_t*    phy_info_ptr, dsp_mpi_st_info_t *mpi_st_ptr)
 *
 * @details    get mpi state information
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      mpi_st_ptr      : mpi state data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_get_mpi_state(capi_phy_info_t*    phy_info_ptr, dsp_mpi_st_info_t *mpi_st_ptr);

/**
 * @brief      host_set_mpi_dynamic_config(capi_phy_info_t*    phy_info_ptr, 
 *                                          dsp_mission_mpi_cfg_info_t *mpi_cfg_ptr)
 *
 * @details    set mpi dynamic configuration information
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      mpi_cfg_ptr      : mpi dynamic config data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_set_mpi_dynamic_config(capi_phy_info_t*            phy_info_ptr,
                                            dsp_mission_mpi_cfg_info_t *mpi_cfg_ptr);
/**
 * @brief      host_get_mpi_dynamic_config(capi_phy_info_t*    phy_info_ptr, 
 *                                          dsp_mission_mpi_cfg_info_t *mpi_cfg_ptr)
 *
 * @details    get mpi dynamic configuration information
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      mpi_cfg_ptr      : mpi dynamic config data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_get_mpi_dynamic_config(capi_phy_info_t*            phy_info_ptr,
                                            dsp_mission_mpi_cfg_info_t *mpi_cfg_ptr);


/**
 * @brief      host_get_dynamic_mpi_state(capi_phy_info_t*    phy_info_ptr, dsp_mission_mpi_st_info_t *mpi_st_ptr)
 *
 * @details    get dynamic MPI information
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      mpi_st_ptr      : mpi state data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_get_dynamic_mpi_state(capi_phy_info_t*    phy_info_ptr, dsp_mission_mpi_st_info_t *mpi_st_ptr);

/**
 * @brief      host_set_tmsd_config(capi_phy_info_t*    phy_info_ptr, dsp_tmsd_cfg_info_t *tmsd_cfg_ptr)
 *
 * @details    set incoming traffic mode switch from 10XG to 5Xg detect configuration information
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      tmsd_cfg_ptr      : tmsd config data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_set_tmsd_config(capi_phy_info_t*    phy_info_ptr, dsp_tmsd_cfg_info_t *tmsd_cfg_ptr);

/**
 * @brief      host_get_tmsd_config(capi_phy_info_t*    phy_info_ptr, dsp_tmsd_cfg_info_t *tmsd_cfg_ptr)
 *
 * @details    get incoming traffic mode switch from 10XG to 5Xg detect configuration information
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      tmsd_cfg_ptr      : tmsd config data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_get_tmsd_config(capi_phy_info_t*    phy_info_ptr, dsp_tmsd_cfg_info_t *tmsd_cfg_ptr);

/**
 * @brief      host_get_tmsd_state(capi_phy_info_t*    phy_info_ptr, dsp_tmsd_st_t *tmsd_st_ptr)
 *
 * @details    get incoming traffic mode switch from 10XG to 5Xg detect state information
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      tmsd_st_ptr      : tmsd state data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_get_tmsd_state(capi_phy_info_t*    phy_info_ptr, dsp_tmsd_st_t *tmsd_st_ptr);


/**
 * @brief      host_set_mpi_canceller_config(capi_phy_info_t*    phy_info_ptr, 
 *                                          dsp_mpi_canceller_cfg_info_t *mpi_cfg_ptr)
 *
 * @details   set mpi canceller configuration information
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      mpi_canceller_cfg_ptr      : mpi canceller config data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_set_mpi_canceller_config(capi_phy_info_t*            phy_info_ptr,
                                            dsp_mpi_canceller_cfg_info_t *mpi_canceller_cfg_ptr);

/**
 * @brief      host_get_mpi_canceller_config(capi_phy_info_t*            phy_info_ptr,
 *                                            dsp_mpi_canceller_cfg_info_t *mpi_canceller_cfg_ptr)
 *
 * @details    get mpi canceller configuration information
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      mpi_cfg_ptr      : mpi dynamic config data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_get_mpi_canceller_config(capi_phy_info_t*            phy_info_ptr,
                                            dsp_mpi_canceller_cfg_info_t *mpi_canceller_cfg_ptr);

/**
 * @brief      host_get_dsp_debug_dump(capi_phy_info_t*    phy_info_ptr, 
 *                                      capi_dsp_debug_dump_t*      ddump_ptr)
 *
 * @details    Dump dsp debugging information
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      ddump_ptr      : dsp dump data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_get_dsp_debug_dump(capi_phy_info_t*    phy_info_ptr, 
                                        capi_dsp_debug_dump_t*      ddump_ptr);
/**
 * @brief      host_set_hw_gain2_a_config(capi_phy_info_t*    phy_info_ptr, 
 *                                          dsp_hw_gain2_adapt_cfg_info_t *hw_gain2_a_cfg_ptr)
 *
 * @details   set hardware gain2 adaptation configuration information
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      hw_gain2_a_cfg_ptr      : hardware gain2 adaptation config data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_set_hw_gain2_a_config(capi_phy_info_t*            phy_info_ptr,
                                            dsp_hw_gain2_adapt_cfg_info_t *hw_gain2_a_cfg_ptr);
/**
 * @brief      host_get_hw_gain2_a_config(capi_phy_info_t*            phy_info_ptr,
 *                                            dsp_hw_gain2_adapt_cfg_info_t *hw_gain2_a_cfg_ptr)
 *
 * @details    get hardware gain2 adaptation configuration information
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      hw_gain2_a_cfg_ptr      : hardware gain2 adaptation config data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_get_hw_gain2_a_config(capi_phy_info_t*            phy_info_ptr,
                                            dsp_hw_gain2_adapt_cfg_info_t *hw_gain2_a_cfg_ptr);

/**
 * @brief      host_dump_fw_events(capi_phy_info_t* phy_info_ptr, FILE* dump_file_ptr)
 * @details    This API is used to collect firmware events
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: pointer to the capi_phy_info_t struct variable
 * @param[in]  dump_handler: pointer to the file handler of the output
 *
 * @return     returns the performance result of the called method/function
 */
return_result_t host_dump_fw_events(capi_phy_info_t* phy_info_ptr, FILE* dump_file_ptr);

/**
 * @brief      host_set_gain3_a_param_config(capi_phy_info_t*    phy_info_ptr, 
 *                                          dsp_gain3_a_param_cfg_info_t *gain3_a_cfg_ptr)
 *
 * @details   set gain3 manual a parameter configuration information
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      mpi_canceller_cfg_ptr      : mpi canceller config data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_set_gain3_a_param_config(capi_phy_info_t*            phy_info_ptr,
                                            dsp_gain3_a_param_cfg_info_t *gain3_a_cfg_ptr);

/**
 * @brief      host_get_gain3_a_param_config(capi_phy_info_t*    phy_info_ptr, 
 *                                          dsp_gain3_a_param_cfg_info_t *gain3_a_cfg_ptr)
 *
 * @details   get gain3 manual a parameter configuration information
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      mpi_canceller_cfg_ptr      : mpi canceller config data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_get_gain3_a_param_config(capi_phy_info_t*            phy_info_ptr,
                                            dsp_gain3_a_param_cfg_info_t *gain3_a_cfg_ptr);


return_result_t host_dump_dsp_rx_diag_status(capi_phy_info_t *     phy_info_ptr,
                                             dsp_rx_diag_status_t *rx_diag_status_ptr);

#ifdef __cplusplus
}
#endif

#endif /* ifndef HOST_DIAG_H */


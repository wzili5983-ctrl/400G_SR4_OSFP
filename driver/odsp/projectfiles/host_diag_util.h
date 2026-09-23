/**
 *
 * @file capi_diag.h
 * @author
 * @date     12/01/2016
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
#ifndef HOST_DIAG_UTIL_H
#define HOST_DIAG_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#define TXFIR_MAX_VALUE 168

#ifdef  CAPI_CONFIG_CMD_SANITY_CHK_ENABLE
#define CMD_SANITY_CHECK(phy_info_ptr, cmd_info, chk_type)   do { \
            return_result_t retcode = RR_SUCCESS; \
            retcode = host_diag_cmd_sanity_checker(phy_info_ptr, cmd_info, chk_type); \
            if (retcode != RR_SUCCESS) return retcode; \
            }   while(0)
#else
#define CMD_SANITY_CHECK(phy_info_ptr, cmd_info, chk_type)
#endif

typedef enum sanity_chk_e {
    CHK_USR_INPUT = 0,  /**< Perform sanity check on the user input         */
    CHK_FW_OUTPUT = 1   /**< Perform sanity check on the FW provided output */

} sanity_chk_t;

/**
 * @brief      util_check_param_range(int source, int lower_limit, int upper_limit)
 * @details    This API is used to check if a parameter is inside lower and upper limit
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  source: parameter to be checked
 * @param[in]  lower_limit: lower boundary for source
 * @param[in]  upper_limit: upper boundary for source
 *
 * @return     TRUE: source is in lower/upper limit, FALSE: source is outside lower/upper limit
 */

bool util_check_param_range(int source, int lower_limit, int upper_limit);


/**
 * @brief      util_lw_tx_fir_lut_validate_coef_and_lvl(phy_info_t* phy_info_ptr, int16_t *coef, int8_t *lvl)
 * @details    This API is used to check if txfir level shift is valid
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr : phy info
 * @param[in]  coef         : Coeff.
 * @param[in]  lvl          : Tap level
 *
 * @return     RR_SUCCESS: Valid, RR_WARNING_BOUNDS: Out of range
 */
return_result_t util_lw_tx_fir_lut_validate_coef_and_lvl (phy_info_t* phy_info_ptr, int16_t *coef, int8_t *lvl);


/**
 * @brief      diag_lane_rx_info_sanity_checker_set(capi_phy_info_t *phy_info_ptr, lane_rx_info_t *rx_info_ptr)
 * @details    This API is used to sanity check lane config params
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  rx_info_ptr : this parameter
 * @param[in]  phy_info_ptr: this parameter
 *
 * @return     returns the performance result of the called method/function
 */
return_result_t diag_lane_rx_info_sanity_checker_set(capi_phy_info_t *phy_info_ptr, lane_rx_info_t *rx_info_ptr);

/**
 * @brief      diag_lane_rx_info_sanity_checker_get(capi_phy_info_t *phy_info_ptr, lane_rx_info_t *rx_info_ptr, sanity_chk_t chk_type)
 * @details    This API is used to sanity check lane config params
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  rx_info_ptr : this parameter
 * @param[in]  phy_info_ptr: this parameter
 * @param[in]  chk_type    : this parameter
 *
 * @return     returns the performance result of the called method/function
 */
return_result_t diag_lane_rx_info_sanity_checker_get(capi_phy_info_t *phy_info_ptr, lane_rx_info_t *rx_info_ptr, sanity_chk_t chk_type);

/**
 * @brief      diag_lane_config_info_sanity_checker(capi_phy_info_t *phy_info_ptr, capi_lane_config_info_t* lane_config_info_ptr)
 * @details    This API is used to sanity check lane config params
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  lane_config_info_ptr: this parameter
 * @param[in]  phy_info_ptr        : this parameter
 *
 * @return     returns the performance result of the called method/function
 */
return_result_t diag_lane_config_info_sanity_checker(capi_phy_info_t *phy_info_ptr, capi_lane_config_info_t* lane_config_info_ptr);

/**
 * @brief      tx_info_sanity_checker_get(lane_tx_info_t* tx_info_ptr)
 * @details    This API is used to sanity check Tx config params
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: this parameter
 * @param[in]  tx_info_ptr : this parameter
 * @param[in]  chk_type    : specifies user input of FW output check type
 *
 * @return     returns the performance result of the called method/function
 */
return_result_t tx_info_sanity_checker_get(capi_phy_info_t *phy_info_ptr, lane_tx_info_t* tx_info_ptr, sanity_chk_t chk_type);

/**
 * @brief      diag_tx_info_sanity_checker(capi_phy_info_t* phy_info_ptr, lane_tx_info_t* tx_info_ptr)
 * @details    This API is used to sanity check Tx config params
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: this parameter
 * @param[in]  tx_info_ptr : this parameter
 *
 * @return     returns the performance result of the called method/function
 */
return_result_t diag_tx_info_sanity_checker(capi_phy_info_t* phy_info_ptr, lane_tx_info_t* tx_info_ptr);

/**
 * @brief      diag_lnktrn_info_sanity_checker(capi_phy_info_t *phy_info_ptr, capi_lnktrn_info_t* lnktrn_info_ptr, sanity_chk_t chk_type)
 * @details    This API is used to sanity check set lnktrn info params
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: this parameter
 * @param[in]  lnktrn_info_ptr: link training info pointer
 * @param[in]  chk_type: sanity check type, input or output
 *
 * @return     returns the performance result of the called method/function
 */

return_result_t diag_lnktrn_info_sanity_checker(capi_phy_info_t *phy_info_ptr, capi_lnktrn_info_t* lnktrn_info_ptr, sanity_chk_t chk_type);

/**
 * @brief      diag_lane_swap_info_sanity_checker(capi_phy_info_t *phy_info_ptr, lane_swap_info_t* lane_swap_info_ptr, uint8_t request, sanity_chk_t chk_type)
 * @details    This API is used to sanity check lane swap info params
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: this parameter
 * @param[in]  lane_swap_info_ptr: lane swap info pointer
 * @param[in]  request: request type, SET or GET
 * @param[in]  chk_type: sanity check type, input or output
 *
 * @return     returns the performance result of the called method/function
 */
return_result_t diag_lane_swap_info_sanity_checker(capi_phy_info_t *phy_info_ptr, lane_swap_info_t* lane_swap_info_ptr, uint8_t request, sanity_chk_t chk_type);

/**
 * @brief      diag_set_loopback_sanity_checker(capi_phy_info_t *phy_info_ptr, capi_loopback_info_t* loopback_ptr)
 * @details    This API is used to sanity check set loopback params
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: this parameter 
 * @param[in]  loopback_ptr: this parameter 
 * 
 * @return     returns the performance result of the called methode/function
 */
return_result_t diag_set_loopback_sanity_checker(capi_phy_info_t *phy_info_ptr, capi_loopback_info_t* loopback_ptr);

/**
 * @brief      diag_set_loopback_sanity_checker_get(capi_phy_info_t *phy_info_ptr, capi_loopback_info_t* loopback_ptr)
 * @details    This API is used to sanity check get loopback params
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: this parameter 
 * @param[in]  loopback_ptr: this parameter 
 * @param[in]  chk_type: this parameter
 * 
 * @return     returns the performance result of the called methode/function
 */
return_result_t diag_set_loopback_sanity_checker_get(capi_phy_info_t *phy_info_ptr, capi_loopback_info_t* loopback_ptr, sanity_chk_t chk_type);
/**
 * @brief      diag_set_fw_lane_config_sanity_checker(capi_phy_info_t *phy_info_ptr, capi_fw_lane_config_t* fw_lane_config)
 * @details    This API is used to sanity check set FW lane config params
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: this parameter 
 * @param[in]  fw_lane_config_ptr: this parameter 
 * 
 * @return     returns the performance result of the called methode/function
 */
//return_result_t diag_set_fw_lane_config_sanity_checker(capi_phy_info_t *phy_info_ptr, capi_fw_lane_config_t* fw_lane_config_ptr);


/**
 * @brief      lane_ctrl_info_sanity_checker_get(capi_phy_info_t* phy_info_ptr, capi_lane_ctrl_info_t* lane_ctrl_info_ptr)
 * @details    This API is used to sanity check lane control info get parameters
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr     : this parameter 
 * @param[in]  polarity_info_ptr: this parameter 
 * @param[in]  sanity_chk_t     : chk_type - 0: Check user input, 1: Chec kFW output
 * 
 * @return     returns the performance result of the called methode/function
 */
return_result_t lane_ctrl_info_sanity_checker_get(capi_phy_info_t* phy_info_ptr, capi_lane_ctrl_info_t* lane_ctrl_info_ptr, sanity_chk_t chk_type);

/**
 * @brief      lane_ctrl_info_sanity_checker(capi_phy_info_t* phy_info_ptr, capi_lane_ctrl_info_t* lane_ctrl_info_ptr)
 * @details    This API is used to sanity check lane control info parameters
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr      : this parameter 
 * @param[in]  lane_ctrl_info_ptr: this parameter 
 * 
 * @return     returns the performance result of the called methode/function
 */
return_result_t lane_ctrl_info_sanity_checker(capi_phy_info_t* phy_info_ptr, capi_lane_ctrl_info_t* lane_ctrl_info_ptr);



/**
 * @brief      pmd_info_sanity_checker_get(capi_phy_info_t* phy_info_ptr, capi_pmd_info_t* pmd_info_ptr, sanity_chk_t chk_type)
 * @details    This API is used to sanity check pmd info params when doing a get
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr     : this parameter 
 * @param[in]  pmd_info_ptr     : this parameter 
 * @param[in]  sanity_chk_t     : chk_type - 0: Check user input, 1: Chec kFW output
 * 
 * @return     returns the performance result of the called methode/function
 */
return_result_t pmd_info_sanity_checker_get (capi_phy_info_t* phy_info_ptr, capi_pmd_info_t* pmd_info_ptr, sanity_chk_t chk_type);


/**
 * @brief      polarity_info_sanity_checker_get(capi_phy_info_t* phy_info_ptr, capi_polarity_info_t* polarity_info_ptr, sanity_chk_t chk_type)
 * @details    This API is used to sanity check polarity info params when doing a get
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr     : this parameter 
 * @param[in]  polarity_info_ptr: this parameter 
 * @param[in]  sanity_chk_t     : chk_type - 0: Check user input, 1: Chec kFW output
 * 
 * @return     returns the performance result of the called methode/function
 */
return_result_t polarity_info_sanity_checker_get (capi_phy_info_t* phy_info_ptr, capi_polarity_info_t* polarity_info_ptr, sanity_chk_t chk_type);

/**
 * @brief      polarity_info_sanity_checker(capi_phy_info_t* phy_info_ptr, capi_polarity_info_t* polarity_info_ptr)
 * @details    This API is used to sanity check polarity info params
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr     : this parameter 
 * @param[in]  polarity_info_ptr: this parameter 
 * 
 * @return     returns the performance result of the called methode/function
 */
return_result_t polarity_info_sanity_checker(capi_phy_info_t* phy_info_ptr, capi_polarity_info_t* polarity_info_ptr);

/**
 * @brief      diag_set_prbs_config_sanity_checker(capi_phy_info_t *phy_info_ptr, capi_prbs_info_t* prbs_info_ptr)
 * @details    This API is used to sanity check set prbs config params
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: this parameter 
 * @param[in]  prbs_info_ptr: this parameter 
 * 
 * @return     returns the performance result of the called methode/function
 */
return_result_t diag_set_prbs_config_sanity_checker(capi_phy_info_t *phy_info_ptr, capi_prbs_info_t* prbs_info_ptr);

/**
 * @brief      diag_set_prbs_enable_sanity_checker(capi_phy_info_t *phy_info_ptr, capi_prbs_info_t* prbs_info_ptr)
 * @details    This API is used to sanity check set prbs enable params
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: this parameter 
 * @param[in]  prbs_info_ptr: this parameter 
 * 
 * @return     returns the performance result of the called methode/function
 */
return_result_t diag_set_prbs_enable_sanity_checker(capi_phy_info_t *phy_info_ptr, capi_prbs_info_t* prbs_info_ptr);

/**
 * @brief      diag_clear_prbs_status_sanity_checker(capi_phy_info_t *phy_info_ptr, capi_prbs_type_t ptype)
 * @details    This API is used to sanity check clear prbs status params
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: this parameter 
 * @param[in]  ptype: this parameter 
 * 
 * @return     returns the performance result of the called methode/function
 */
return_result_t diag_clear_prbs_status_sanity_checker(capi_phy_info_t *phy_info_ptr);

/**
 * @brief      diag_prbs_inject_error_sanity_checker(capi_phy_info_t *phy_info_ptr, capi_prbs_err_inject_t* prbs_err_inj_ptr)
 * @details    This API is used to sanity check prbs inject error params
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: this parameter 
 * @param[in]  prbs_err_inj_ptr: this parameter 
 * 
 * @return     returns the performance result of the called methode/function
 */
return_result_t diag_prbs_inject_error_sanity_checker(capi_phy_info_t *phy_info_ptr, capi_prbs_err_inject_t* prbs_err_inj_ptr);

/**
 * @brief      lane_status_info_sanity_checker_get(capi_phy_info_t* phy_info_ptr, capi_lane_status_info_t* lane_status_info_ptr, sanity_chk_t chk_type)
 * @details    This API is used to sanity check lane status info params when doing a get
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr             : this parameter 
 * @param[in]  lane_status_info_ptr     : this parameter 
 * @param[in]  sanity_chk_t             : chk_type - 0: Check user input, 1: Chec kFW output
 * 
 * @return     returns the performance result of the called methode/function
 */
//return_result_t lane_status_info_sanity_checker_get (capi_phy_info_t* phy_info_ptr, capi_lane_status_info_t* lane_status_info_ptr, sanity_chk_t chk_type);

/**
 * @brief      diag_fw_lane_config_sanity_checker_get(capi_phy_info_t *phy_info_ptr, capi_fw_lane_config_t* fw_lane_config, sanity_chk_t chk_type)
 * @details    This API is used to sanity check get FW lane config params
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: this parameter 
 * @param[in]  loopback_ptr: this parameter 
 * @param[in]  chk_type: this parameter
 * 
 * @return     returns the performance result of the called methode/function
 */
//return_result_t diag_fw_lane_config_sanity_checker_get(capi_phy_info_t *phy_info_ptr, capi_fw_lane_config_t* fw_lane_config, sanity_chk_t chk_type);

/**
 * @brief      diag_prbs_status_sanity_checker_get(capi_phy_info_t *phy_info_ptr, capi_prbs_info_t* prbs_info_ptr)
 * @details    This API is used to sanity check get prbs status params
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: this parameter 
 * @param[in]  prbs_info_ptr: this parameter 
 * 
 * @return     returns the performance result of the called methode/function
 */
return_result_t diag_prbs_status_sanity_checker_get(capi_phy_info_t *phy_info_ptr, capi_prbs_status_t* prbs_status_ptr, sanity_chk_t chk_type) ;

/**
 * @brief      diag_set_prbs_info_sanity_checker(capi_phy_info_t *phy_info_ptr, capi_prbs_info_t* prbs_info_ptr, sanity_chk_t chk_type)
 * @details    This API is used to sanity check set prbs config params
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: this parameter 
 * @param[in]  prbs_info_ptr: this parameter 
 * 
 * @return     returns the performance result of the called methode/function
 */
return_result_t diag_set_prbs_info_sanity_checker(capi_phy_info_t *phy_info_ptr, capi_prbs_info_t* prbs_info_ptr);

/**
 * @brief      diag_get_prbs_info_sanity_checker(capi_phy_info_t *phy_info_ptr, capi_prbs_info_t* prbs_info_ptr, sanity_chk_t chk_type)
 * @details    This API is used to sanity check get prbs config params
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: this parameter 
 * @param[in]  prbs_info_ptr: this parameter 
 * 
 * @return     returns the performance result of the called methode/function
 */
return_result_t diag_get_prbs_info_sanity_checker(capi_phy_info_t *phy_info_ptr, capi_prbs_info_t* prbs_info_ptr, sanity_chk_t chk_type);


#ifdef CAPI_CONFIG_CMD_SANITY_CHK_ENABLE


return_result_t txfir_sanity_checker(capi_phy_info_t* phy_info_ptr, const txfir_info_t *txfir_ptr);

/**
 * @brief       host_diag_cmd_sanity_checker(capi_phy_info_t *phy_info_ptr, capi_command_info_t *cmd_info_ptr, sanity_chk_t chk_type)
 * @details       This function is used to check the capi cmd sanity
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]
 *
 * @return       returns the performance result of the called methode/function
 */
return_result_t host_diag_cmd_sanity_checker(capi_phy_info_t *phy_info_ptr, capi_command_info_t *cmd_info_ptr, sanity_chk_t chk_type);
#endif



/**
 * @brief      host_get_memory_payload(capi_phy_info_t*    phy_info_ptr, 
 *                                     void*               dest_ptr,
 *                                     memory_info_t*      mem_data_ptr)
 *
 * @details    A fast access API to get response data populated by FW.
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      dest_ptr      : dest data pointer
 * @param      mem_data_ptr :  memory data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_get_memory_payload(capi_phy_info_t* phy_info_ptr, 
                                        void*            dest_ptr,
                                        memory_info_t*   mem_data_ptr);



/**
 * @brief      host_get_memory_data_info(capi_phy_info_t*    phy_info_ptr,
 *                                       memory_data_info_t* memory_data_ptr)
 *
 * @details    A fast access API to get response data populated by FW.
 *
 * @param      phy_info_ptr : phy info pointer
 * @param      memory_data_ptr : memory data information pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_get_memory_data_info(capi_phy_info_t*    phy_info_ptr,
                                          memory_data_info_t* memory_data_ptr);



/**
 * @brief        util_get_number_of_lanes(phy_info_t* phy_info_ptr, unsigned char* number_of_lanes_ptr)
 *               This utility function return the total number of the lanes on either media or host side
 *
 * @param[in]    phy_info_ptr: a reference to the phy information object
 * @param[out]   number_of_lanes_ptr: a reference to the number of lanes object, which needs to be initialized
 *
 * @return       retunes the totoal number of the lanes
 */
return_result_t util_get_number_of_lanes(phy_info_t* phy_info_ptr, unsigned char* number_of_lanes_ptr);

/**
 * @brief       sanity_checker_mpi_config_info(capi_phy_info_t *phy_info_ptr, dsp_mpi_cfg_info_t* mpi_info_ptr, sanity_chk_t chk_type)
 * @details    This API is used to sanity check MPI config info params
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: this parameter
 * @param[in]  mpi_info_ptr: this parameter
 * @param[in]  chk_type: this parameter
 *
 * @return     returns the performance result of the called method/function
 */
return_result_t sanity_checker_mpi_config_info(capi_phy_info_t *phy_info_ptr, dsp_mpi_cfg_info_t* mpi_info_ptr, sanity_chk_t chk_type);


/**
 * @brief      dsp_mission_mpi_cfg_sanity_checker(capi_phy_info_t* phy_info_ptr, dsp_mission_mpi_cfg_info_t* dsp_mission_mpi_cfg_info_ptr, sanity_chk_t chk_type)
 * @details    This API is used to sanity check dynamic MPI configuration
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr     : this parameter 
 * @param[in]  dsp_mission_mpi_cfg_info_ptr: this parameter 
 * 
 * @return     returns the performance result of the called methode/function
 */
return_result_t dsp_mission_mpi_cfg_sanity_checker(capi_phy_info_t* phy_info_ptr, dsp_mission_mpi_cfg_info_t* dsp_mission_mpi_cfg_info_ptr, sanity_chk_t chk_type);

/**
 * @brief      dsp_traffic_mode_switch_detect_cfg_sanity_checker(capi_phy_info_t* phy_info_ptr, dsp_tmsd_cfg_info_t* dsp_tmsd_cfg_info_ptr, sanity_chk_t chk_type)
 * @details    This API is used to sanity check TMSD configuration
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr     : this parameter 
 * @param[in]  dsp_mission_mpi_cfg_info_ptr: this parameter 
 * 
 * @return     returns the performance result of the called methode/function
 */
return_result_t dsp_traffic_mode_switch_detect_cfg_sanity_checker(capi_phy_info_t* phy_info_ptr, dsp_tmsd_cfg_info_t* dsp_tmsd_cfg_info_ptr, sanity_chk_t chk_type);

/**
 * @brief      dsp_mpi_canceller_cfg_sanity_checker(capi_phy_info_t* phy_info_ptr, dsp_mpi_canceller_cfg_info_t* mpi_canceller_cfg_info_ptr, sanity_chk_t chk_type)
 * @details    This API is used to sanity check MPI  canceller configuration
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr     : this parameter 
 * @param[in]  mpi_canceller_cfg_info_ptr: this parameter 
 * 
 * @return     returns the performance result of the called methode/function
 */
return_result_t dsp_mpi_canceller_cfg_sanity_checker(capi_phy_info_t* phy_info_ptr, dsp_mpi_canceller_cfg_info_t* mpi_canceller_cfg_info_ptr, sanity_chk_t chk_type);
/**
 * @brief      dsp_hw_gain2_adapt_cfg_sanity_checker(capi_phy_info_t* phy_info_ptr, dsp_hw_gain2_adapt_cfg_info_t* hw_gain2_a_cfg_info_ptr, sanity_chk_t chk_type)
 * @details    This API is used to sanity check hardware gain2 adaptation configuration
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr     : this parameter 
 * @param[in]  hw_gain2_a_cfg_info_ptr: this parameter 
 * 
 * @return     returns the performance result of the called methode/function
 */
return_result_t dsp_hw_gain2_adapt_cfg_sanity_checker(capi_phy_info_t* phy_info_ptr, dsp_hw_gain2_adapt_cfg_info_t* hw_gain2_a_cfg_info_ptr, sanity_chk_t chk_type);

/**
 * @brief      dsp_gain3_a_param_cfg_sanity_checker(capi_phy_info_t* phy_info_ptr, dsp_gain3_a_param_cfg_info_t* gain3_a_cfg_info_ptr, sanity_chk_t chk_type)
 * @details    This API is used to sanity check MPI  canceller configuration
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr     : this parameter 
 * @param[in]  gain3_a_cfg_info_ptr: this parameter 
 * 
 * @return     returns the performance result of the called methode/function
 */
return_result_t dsp_gain3_a_param_cfg_sanity_checker(capi_phy_info_t* phy_info_ptr, dsp_gain3_a_param_cfg_info_t* gain3_a_cfg_info_ptr, sanity_chk_t chk_type);
#ifdef __cplusplus
}
#endif

#endif /* HOST_DIAG_UTIL_H */

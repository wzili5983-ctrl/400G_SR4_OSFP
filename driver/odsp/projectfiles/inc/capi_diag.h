/**
 * @file     capi_diag.h
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
#ifndef CAPI_DIAG_H
#define CAPI_DIAG_H


#ifdef __cplusplus
extern "C" {
#endif

/**
* @brief      capi_diag_get_lane_status(capi_phy_info_t* phy_info_ptr, capi_diag_lane_status_t * diag_lane_status_ptr)
* @details    This API is used to retrieve client lane state
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  diag_lane_status_ptr: pointer to struct that stores the inquired status
* 
* @return     returns the performance result of the called method/function
*/
return_result_t capi_diag_get_lane_status(capi_phy_info_t* phy_info_ptr, capi_diag_lane_status_t * diag_lane_status_ptr);

/**
* @brief      capi_init_fec_mon(capi_phy_info_t* phy_info_ptr, capi_fec_dump_status_t* capi_fec_dump_status_ptr, boolean enable)
* @details    This API is used to enable/disable FEC statistics
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  capi_fec_dump_status_ptr: this parameter 
* 
* @return     returns the performance result of the called methode/function
*/
return_result_t capi_init_fec_mon (capi_phy_info_t* phy_info_ptr, capi_fec_dump_status_t* capi_fec_dump_status_ptr, boolean enable);


/**
* @brief      capi_clear_fec_mon (capi_phy_info_t* phy_info_ptr, capi_fec_dump_status_t* capi_fec_dump_status_ptr)
* @details    This API is used to clear FEC BRCM counters 
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  fec_info_ptr: this parameter 
* 
* @return     returns the performance result of the called methode/function
*/
return_result_t capi_clear_fec_mon (capi_phy_info_t* phy_info_ptr,capi_fec_dump_status_t* capi_fec_dump_status_ptr);


/**
* @brief      capi_get_fec_info(capi_phy_info_t* phy_info_ptr, capi_fec_dump_status_t* capi_fec_dump_status_ptr)
* @details    This API is used to 
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[out] fec_info_ptr: this parameter 
* 
* @return     returns the performance result of the called methode/function
*/
return_result_t capi_get_fec_info(capi_phy_info_t* phy_info_ptr, capi_fec_dump_status_t* capi_fec_dump_status_ptr);

/**
* @brief      capi_get_usr_diagnostics(capi_phy_info_t* phy_info_ptr, capi_usr_diag_info_t* usr_diag_info_ptr)
* @details    This API is used to get the user diag info
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  usr_diag_info_ptr: this parameter
*
* @return     returns the performance result of the called method/function
*/
return_result_t capi_get_usr_diagnostics(capi_phy_info_t* phy_info_ptr, capi_usr_diag_info_t* usr_diag_info_ptr);

/**
* @brief      capi_fec_ber_cmis_latch_request(capi_phy_info_t *phy_ptr, capi_kp4fec_ber_state_t* fec_ptr)
* @details    This API is used to send the FEC BER latch request;
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  fec_ptr: this parameter pointer point to the BER state;
* 
* @return     returns the performance result of the called methode/function
*/
return_result_t capi_fec_ber_cmis_latch_request(capi_phy_info_t *phy_ptr, capi_kp4fec_ber_state_t* fec_ptr);


/**
* @brief      capi_fec_ber_cmis_latch_release(capi_phy_info_t *phy_ptr, capi_kp4fec_ber_state_t* fec_ptr)
* @details    This API is used to 
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  fec_info_ptr: this parameter 
* 
* @return     returns the performance result of the called methode/function
*/
return_result_t capi_fec_ber_cmis_latch_release(capi_phy_info_t *phy_ptr, capi_kp4fec_ber_state_t* fec_ptr);


/**
 * @brief      capi_diag_command_request_info(capi_phy_info_t* phy_info_ptr, capi_diag_command_request_info_t* diag_cmd_req_info_ptr)
 * @details    Function to send Diag command request
 *
 * @param      phy_info_ptr : phy info pointer
 * @param      cmd_req_ptr  : command request
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t capi_diag_command_request_info(capi_phy_info_t* phy_info_ptr, capi_diag_command_request_info_t* diag_cmd_req_info_ptr);

/**
* @brief       capi_get_diag_dump(capi_phy_info_t* phy_info_ptr, capi_diag_dump_t* diag_dump_ptr)
* @details    This API is used to get DSP diagnostics data from FW memory via burst read.
*
* @param[in]  phy_info_ptr : a reference to the phy information object
* @param[in]   diag_dump_ptr  : a reference to the diag dump object
*
* @return     returns the performance result of the called method/function
*/
return_result_t capi_get_diag_dump(capi_phy_info_t* phy_info_ptr, capi_diag_dump_t* diag_dump_ptr);
 
/**
* @brief      capi_fec_start_stop_auto_ports_detect(capi_phy_info_t* phy_info_ptr, capi_fec_auto_ports_detect_t* capi_fec_auto_ports_detect_ptr)
* @details    This API is used to intiate auto FEC port detection
*
* @param[in]  phy_info_ptr : a reference to the phy information object
* @param[in]  capi_fec_auto_ports_detect_ptr  : a reference to the auto fec port detect structure
*
* @return     returns the performance result of the called method/function
*/ 
return_result_t capi_fec_start_stop_auto_ports_detect(capi_phy_info_t* phy_info_ptr, capi_fec_auto_ports_detect_t* capi_fec_auto_ports_detect_ptr);

/**
* @brief      capi_fec_ports_auto_detect_status (capi_phy_info_t* phy_info_ptr, capi_fec_auto_ports_detect_status_t* capi_fec_auto_ports_detect_status_ptr)
* @details    This API is used to get the auto FEC port detection status
*
* @param[in]  phy_info_ptr : a reference to the phy information object
* @param[in]  get_state_info_ptr  : a reference to the state info to get
*
* @return     returns the performance result of the called method/function
*/ 
return_result_t capi_get_state_info(capi_phy_info_t* phy_info_ptr, capi_state_info_t* state_info_ptr); 
#ifdef __cplusplus
}
#endif

#endif /* ifndef CAPI_TEST_H */


/**
 * @file     capi_test.h
 * @author  
 * @date     03-24-2019
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
#ifndef CAPI_TEST_H
#define CAPI_TEST_H


#ifdef __cplusplus
extern "C" {
#endif


/**
* @brief      capi_set_prbs_info(capi_phy_info_t* phy_info_ptr, capi_prbs_info_t* prbs_info_ptr)
* @details    This API is used to configure the PRBS pattern generator and enable or disable it
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  prbs_info_ptr: this pointer contain PRBS config info
* 
* @return     returns the performance result of the called methode/function
*/
return_result_t capi_set_prbs_info(capi_phy_info_t* phy_info_ptr, capi_prbs_info_t* prbs_info_ptr);



/**
* @brief      capi_get_prbs_info(capi_phy_info_t* phy_info_ptr, capi_prbs_info_t* prbs_info_ptr)
* @details    This API is to the the configuration setting of othe PRBS pattern generator
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[out] prbs_info_ptr: this pointer return PRBS config information
* 
* @return     returns the performance result of the called methode/function
*/
return_result_t capi_get_prbs_info(capi_phy_info_t* phy_info_ptr, capi_prbs_info_t* prbs_info_ptr);

/**
* @brief      capi_get_prbs_status(capi_phy_info_t*    phy_info_ptr,
*                                  capi_prbs_status_t* prbs_st_ptr)
* @details    This API is used to get the PRBS  status
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[out] prbs_st_ptr: this pointer return PRBS status information
* 
* @return     returns the performance result of the called methode/function
*/
return_result_t capi_get_prbs_status(capi_phy_info_t*    phy_info_ptr,
                                     capi_prbs_status_t* prbs_st_ptr);



/**
* @brief      capi_clear_prbs_status(capi_phy_info_t* phy_info_ptr, capi_prbs_type_t ptype)
* @details    This API is used to clear the PRBS  status
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  ptype: this parameter indicate the PRBS type
* 
* @return     returns the performance result of the called methode/function
*/
return_result_t capi_clear_prbs_status(capi_phy_info_t* phy_info_ptr, capi_prbs_status_t* prbs_st_ptr);



/**
* @brief      capi_set_loopback(capi_phy_info_t* phy_info_ptr, capi_loopback_t* loopback_ptr)
* @details    This API is used to set the loopback mode
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  loopback_ptr: this pointer carries the necessary information to set the loopback mode
* 
* @return     returns the performance result of the called method/function
*/
return_result_t capi_set_loopback(capi_phy_info_t* phy_info_ptr, capi_loopback_info_t* loopback_ptr);



/**
 * @brief      capi_get_loopback(capi_phy_info_t* phy_info_ptr, capi_loopback_t* loopback_ptr)
 * @details    This API is used to get the loopback mode <BR>
 *             Note: This function support either Line Side or Client side <BR>
 *                   This function only support single-lane operation. <BR>
 *                   If multi-lane mask is passed in, then first valid lane status will be returned. <BR>
 *             when core_ip is CORE_IP_LW, the "mode" value is: <BR>
 *             CAPI_GLOBAL_LOOPBACK_MODE: digital global loopback from Line side transmitter to receiver <BR>
 *             CAPI_REMOTE_LOOPBACK_MODE: digital remote loopback from line side receiver to transmitter <BR>
 *             when core_ip is CORE_IP_CLIENT, the "mode" value is: <BR>
 *             CAPI_GLOBAL_LOOPBACK_MODE: digital global loopback from System side transmitter to receiver <BR>
 *             CAPI_REMOTE_LOOPBACK_MODE: digital remote loopback from System side receiver to transmitter <BR>
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[out] loopback_ptr: this pointer return loopback mode
 * 
 * @return     returns the performance result of the called method/function
 */
return_result_t capi_get_loopback(capi_phy_info_t* phy_info_ptr, capi_loopback_info_t* loopback_ptr);

/**
 * @brief      capi_test_set_command(capi_phy_info_t*          phy_info_ptr,
 *                                   capi_test_command_info_t* test_cmd_inf_ptr)
 * @details    This API is used to invoke test command.
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  capi_test_command_info_t: a reference to the test command object 
 * @return     returns the performance result of the called method/function
 */
return_result_t capi_test_set_command(capi_phy_info_t*          phy_info_ptr,
                                      capi_test_command_info_t* test_cmd_inf_ptr);

/**
* @brief      capi_prbs_inject_error(capi_phy_info_t* phy_info_ptr, capi_prbs_err_inject_t* prbs_err_inj_ptr)
* @details    This API is used to clear the PRBS  status
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  ptype: this parameter indicate the PRBS type
* 
* @return     returns the performance result of the called methode/function
*/
return_result_t capi_prbs_inject_error(capi_phy_info_t* phy_info_ptr, capi_prbs_err_inject_t* prbs_err_inj_ptr);


#ifdef __cplusplus
}
#endif


#endif /* ifndef CAPI_TEST_H */


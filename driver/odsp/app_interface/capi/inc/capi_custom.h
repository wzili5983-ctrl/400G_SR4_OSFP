/**
 * @file     capi_custome.h
 * @author  
 * @date     02-26-2021
 * @version  1.0
 *
 * @property
 * $Copyright: (c) 2021 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * @brief    brief description of the block source file, sequences/flow/etc.
 *
 * @section  description
 *
 */

#ifndef CAPI_CUSTOME_H
#define CAPI_CUSTOME_H


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief      capi_custom_command_request(capi_phy_info_t*            phy_info_ptr,
 *                                         capi_custom_command_info_t* cmd_inf_ptr)
 * @details    This API is used to invoke test command.
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  cmd_inf_ptr:  a pointer which carries custom command 
 * @return     returns the performance result of the called method/function
 */
return_result_t capi_custom_command_request(capi_phy_info_t*            phy_info_ptr,
                                             capi_custom_command_info_t* cmd_inf_ptr);

#ifdef __cplusplus
}
#endif

#endif /* CAPI_CUSTOME_H */


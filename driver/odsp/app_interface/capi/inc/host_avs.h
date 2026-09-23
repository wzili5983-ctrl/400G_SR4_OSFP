/**
 *
 * @file     host_avs.h
 * @author   
 * @date     9/15/2018
 * @version  1.0
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

#ifndef HOST_AVS_H
#define HOST_AVS_H

#ifdef __cplusplus
extern "C" {
#endif

    /**
 * @brief   host_get_avs_status(capi_phy_info_t*   phy_info_ptr,
 *                              capi_avs_status_t* capi_avs_status_ptr)
 * @details  This API is used to set the AVS configuration
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  capi_avs_status_ptr: this pointer contains AVS mode configuration defined by
 *                                  capi_avs_status_t, also dump avs status if AVS_TRACE_DEBUG is enabled
 * 
 * @return     returns the result of the called method/function
 */
return_result_t host_get_avs_status(capi_phy_info_t*   phy_info_ptr,
                                    capi_avs_status_t* capi_avs_status_ptr);


#ifdef __cplusplus
}
#endif

#endif /**< CAPI_H */


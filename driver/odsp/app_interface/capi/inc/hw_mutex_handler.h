/**
 * @file     hw_mutex_handler.h
 * @author   FW Team
 * @date     10-26-2021
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
#ifndef HW_MUTEX_HANDLER_H
#define HW_MUTEX_HANDLER_H

/**
 * @brief      acquire_hw_mutex(capi_phy_info_t* phy_info_ptr, uint32_t hw_mutex_reg)
 * @details    Acquires given HW mutex, if available.
 *
 * @param      phy_info_ptr : phy info pointer
 * @param      hw_mutex_reg : HW Mutex register
 *
 * @return     return_result_t: RR_SUCCESS, if HW Muxtex acquired
 *                              RR_WARNING_MUTEX_UNAVAILABLE_RETRY_LATER, if not available
 */
return_result_t acquire_hw_mutex(capi_phy_info_t* phy_info_ptr, uint32_t hw_mutex_reg);

/**
 * @brief      release_hw_mutex(capi_phy_info_t* phy_info_ptr, uint32_t hw_mutex_reg)
 * @details    Releases HW mutex
 *
 * @param      phy_info_ptr : Phy info pointer
 * @param      hw_mutex_reg : HW Mutex register
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t release_hw_mutex(capi_phy_info_t* phy_info_ptr, uint32_t hw_mutex_reg);


#endif /* ifndef HW_MUTEX_HANDLER_H */


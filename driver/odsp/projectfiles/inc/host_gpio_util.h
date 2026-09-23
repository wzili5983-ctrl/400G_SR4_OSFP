/**
 *
 * @file    host_gpio_util.h
 * @author
 * @date    11/04/2020
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
#ifndef HOST_GPIO_UTIL_H
#define HOST_GPIO_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief host_set_gpio_info(phy_info_t* phy_info_ptr, capi_gpio_info_t* capi_gpio_info_ptr)
 * @details  Set gpio info 
 * @public   
 * @private 
 * @param  bbaddr device base address 
 * @param  capi_gpio_info pointer 
 * @return enum return_result_t
 */
return_result_t host_set_gpio_info(phy_info_t* phy_info_ptr, capi_gpio_info_t* capi_gpio_info_ptr);

/**
 * @brief host_get_gpio_info(phy_info_t* phy_info_ptr, capi_gpio_info_t* capi_gpio_info_ptr)
 * @details  get gpio info 
 * @public   
 * @private 
 * @param  bbaddr device base address 
 * @param  capi_gpio_info pointer 
 * @return enum return_result_t
 */
return_result_t host_get_gpio_info(phy_info_t* phy_info_ptr, capi_gpio_info_t* capi_gpio_info_ptr);

#ifdef __cplusplus
}
#endif

#endif  /* HOST_POWER_UTIL_H */

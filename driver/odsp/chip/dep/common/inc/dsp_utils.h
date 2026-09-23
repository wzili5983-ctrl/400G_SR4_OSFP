/**
 *
 * @file       dsp_utils.h
 * @author     PLP Firmware Team
 * @date       02/15/2020
 * @version    1.0
 *
 * @property 
 * $Copyright: (c) 2020 Broadcom.
 *             Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * @brief      The CAPI files contain all the CAPIs used by the the module and line card products.
 *
 * @section  description
 *
 */
#ifndef DSP_UTILS_H
#define DSP_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief      lw_util_init_lane_hw_base_addr(capi_phy_info_t* capi_phy_info_ptr,
 *                                            phy_info_t*      phy_info_ptr,
 *                                            uint8_t          lane_index)
 *
 * @details    Returns line lane offset for the requested HW register.
 *             Also updates the base_address on phy_info_ptr
 *
 * @param      capi_phy_info_ptr : reference to the phy info
 * @param      phy_info_ptr      : phy info reference which needs to be initialized
 * @param      lane_index        : lane index
 *
 * @return     void
 */
void lw_util_init_lane_hw_base_addr(capi_phy_info_t* capi_phy_info_ptr,
                                    phy_info_t*      phy_info_ptr,
                                    uint8_t          lane_index);

/**
 * @brief      lw_util_init_lane_config_base_addr(capi_phy_info_t* capi_phy_info_ptr,
 *                                                phy_info_t*      phy_info_ptr,
 *                                                uint8_t          lane_index)
 *
 * @details    Returns line lane offset for the requested CONFIG register.
 *             Also updates the base_address on phy_info_ptr
 *
 * @param      capi_phy_info_ptr : reference to the phy info
 * @param      phy_info_ptr      : phy info reference which needs to be initialized
 * @param      lane_index        : lane index
 *
* @return     void
 */
void lw_util_init_lane_config_base_addr(capi_phy_info_t* capi_phy_info_ptr,
                                        phy_info_t*      phy_info_ptr,
                                        uint8_t          lane_index);

#ifdef __cplusplus
}
#endif

#endif /* DSP_UTILS_H */

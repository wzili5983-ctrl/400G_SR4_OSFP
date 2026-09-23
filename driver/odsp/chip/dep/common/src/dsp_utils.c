/**
 *
 * @file       dsp_utils.c
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
#include <stdio.h>
#include "common_def.h"
#include "regs_common.h"
#include "chip_mode_def.h"
#include "capi_def.h"
#include "common_util.h"
#include "dsp_utils.h"
#include "dsp_config.h"

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
                                    uint8_t          lane_index)
{
    util_memcpy(phy_info_ptr, capi_phy_info_ptr, sizeof(phy_info_t));
    if (phy_info_ptr->core_ip == CORE_IP_HOST_SERDES || phy_info_ptr->core_ip == CORE_IP_HOST_DSP)
        phy_info_ptr->base_addr = dsp_lane_bbaddr[PHY_HOST_SIDE][lane_index];
    else
        phy_info_ptr->base_addr = dsp_lane_bbaddr[PHY_MEDIA_SIDE][lane_index];
    phy_info_ptr->lane_mask = 1<<lane_index;
}

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
                                        uint8_t          lane_index)
{
    util_memcpy(phy_info_ptr, capi_phy_info_ptr, sizeof(phy_info_t));
    if (phy_info_ptr->core_ip == CORE_IP_HOST_SERDES || phy_info_ptr->core_ip == CORE_IP_HOST_DSP)
        phy_info_ptr->base_addr = dsp_lane_config_gpr_bbaddr[PHY_HOST_SIDE][lane_index];
    else
        phy_info_ptr->base_addr = dsp_lane_config_gpr_bbaddr[PHY_MEDIA_SIDE][lane_index];
    phy_info_ptr->lane_mask = 1<<lane_index;
}

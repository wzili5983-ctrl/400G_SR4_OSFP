/**
 *
 * @file     hw_mutex_handler.c
 * @author   FW Team
 * @date     10/26/2021
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
#include "access.h"
#include "common_def.h"
#include "regs_common.h"
#include "chip_common_config_ind.h"
#include "fw_gp_reg_map.h"
#include "common_def.h"
#include "chip_mode_def.h"
#include "cw_def.h"
#include "capi_def.h"
#include "hw_mutex_handler.h"


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
return_result_t acquire_hw_mutex(capi_phy_info_t* phy_info_ptr, uint32_t hw_mutex_reg)
{
    return_result_t ret_code = RR_WARNING_MUTEX_UNAVAILABLE_RETRY_LATER;
    uint16_t mutex_value;

    if (hw_mutex_reg == 0)
        return RR_SUCCESS;
    
    /* If the IPC_MUTEX_REG reads value 1, means the MUTEX acquired */
    phy_info_ptr->base_addr = MGT;
    ERR_HSIP(mutex_value = (uint16_t)hsip_rd_reg_(phy_info_ptr, hw_mutex_reg));

    if ((mutex_value & 0x1) == 1)
        ret_code = RR_SUCCESS;

    return ret_code;
}

/**
 * @brief      release_hw_mutex(capi_phy_info_t* phy_info_ptr, uint32_t hw_mutex_reg)
 * @details    Releases HW mutex
 *
 * @param      phy_info_ptr : Phy info pointer
 * @param      hw_mutex_reg : HW Mutex register
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t release_hw_mutex(capi_phy_info_t* phy_info_ptr, uint32_t hw_mutex_reg)
{
    if (hw_mutex_reg) {
        phy_info_ptr->base_addr = MGT;
        hsip_wr_reg_(phy_info_ptr, hw_mutex_reg, 1);
    }

    return RR_SUCCESS;
}


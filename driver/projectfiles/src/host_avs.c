/**
 *
 * @file       host_avs.c
 * @author     PLP Firmware Team
 * @date       03/15/2020
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
#include "regs_common.h"
#include "chip_mode_def.h"
#include "capi_def.h"
#include "common_util.h"
#include "host_log_util.h"
#include "host_avs.h"
#include "hr_time.h"
/**
* @brief        host_get_avs_status(capi_phy_info_t*   phy_info_ptr,
*                                   capi_avs_status_t* capi_avs_status_ptr)
* @details      This API is used to set the AVS configuration
*
* @param[in]    phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]    capi_avs_status_ptr: this pointer contains AVS mode configuration defined by 
*                                      capi_avs_status_t, also dump avs status if AVS_TRACE_DEBUG is enabled
* 
* @return       returns the result of the called methode/function
*/
return_result_t host_get_avs_status(capi_phy_info_t*   phy_info_ptr,
                                    capi_avs_status_t* capi_avs_status_ptr)
{
    chip_top_avs_mode_config_reg_t chip_top_avs_mode_config;
    uint16_t avs_ready_timer   = 30; /*~3 seconds*/
    uint16_t avs_ready_counter = 0;

    CAPI_LOG_FUNC("host_get_avs_status", phy_info_ptr);

    util_memset((void*)capi_avs_status_ptr, 0, sizeof(capi_avs_status_t));
    util_memset((void*)&chip_top_avs_mode_config, 0, sizeof(chip_top_avs_mode_config_reg_t));

    phy_info_ptr->base_addr = OCTAL_TOP_REGS;
    do {
        ERR_HSIP(chip_top_avs_mode_config.words = (uint16_t) hsip_rd_reg_(phy_info_ptr, CHIP_TOP_CHIP_AVS_MODE_CONFIG_REG));
        capi_avs_status_ptr->avs_result = (avs_result_t) chip_top_avs_mode_config.fields.avs_status;
        capi_avs_status_ptr->avs_enable = (uint8_t) chip_top_avs_mode_config.fields.avs_enable;
        if(chip_top_avs_mode_config.fields.avs_status == CAPI_INIT_DONE_SUCCESS) break;
        delay_ms(100);
    } while(avs_ready_counter++ < avs_ready_timer);
    if(chip_top_avs_mode_config.fields.avs_status == CAPI_INIT_DONE_SUCCESS) {
       return RR_SUCCESS;
    }
    else {
       return RR_ERROR;
    }
}


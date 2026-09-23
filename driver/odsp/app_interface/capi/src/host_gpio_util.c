/**
 *
 * @file    host_gpio_util.c
 * @author  Firmware Team
 * @date    11/04/2020
 * @version 0.2
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
#include "access.h"
#include "common_def.h"
#include "chip_mode_def.h"
#include "capi_def.h"
#include "host_gpio_util.h"
#include "chal_gpio.h"

/**
 * @brief host_set_gpio_info(phy_info_t* phy_info_ptr, capi_gpio_info_t* capi_gpio_info_ptr)
 * @details  Set gpio info 
 * @public   
 * @private 
 * @param  bbaddr device base address 
 * @param  capi_gpio_info pointer 
 * @return enum return_result_t
 */
return_result_t host_set_gpio_info(phy_info_t* phy_info_ptr, capi_gpio_info_t* capi_gpio_info_ptr) {
    chal_gpio_set_dir(phy_info_ptr, capi_gpio_info_ptr->gpio_id,  (capi_gpio_dir_type_t) capi_gpio_info_ptr->dir_type);
    if(capi_gpio_info_ptr->pull_updown_type == CAPI_GPIO_PULL_UP_ACTIVE){
        chal_gpio_set_pulldown(phy_info_ptr, capi_gpio_info_ptr->gpio_id,  0);
        chal_gpio_set_pullup(phy_info_ptr, capi_gpio_info_ptr->gpio_id,  1);
    }
    else {
        chal_gpio_set_pulldown(phy_info_ptr, capi_gpio_info_ptr->gpio_id,  1);
        chal_gpio_set_pullup(phy_info_ptr, capi_gpio_info_ptr->gpio_id,  0);
    }
    if (capi_gpio_info_ptr->dir_type== CAPI_GPIO_OUTPUT){
        chal_gpio_set_output(phy_info_ptr, capi_gpio_info_ptr->gpio_id,capi_gpio_info_ptr->gpio_set_value);
    }

    return RR_SUCCESS;
}

/**
 * @brief host_get_gpio_info(phy_info_t* phy_info_ptr, capi_gpio_info_t* capi_gpio_info_ptr)
 * @details  get gpio info 
 * @public   
 * @private 
 * @param  bbaddr device base address 
 * @param  capi_gpio_info pointer 
 * @return enum return_result_t
 */
return_result_t host_get_gpio_info(phy_info_t* phy_info_ptr, capi_gpio_info_t* capi_gpio_info_ptr){
    uint8_t downactive, upactive;
    chal_gpio_get_dir(phy_info_ptr, capi_gpio_info_ptr->gpio_id, (uint8_t*)&capi_gpio_info_ptr->dir_type);
    if (capi_gpio_info_ptr->dir_type== CAPI_GPIO_INPUT){
        chal_gpio_get_input(phy_info_ptr, capi_gpio_info_ptr->gpio_id, &capi_gpio_info_ptr->gpio_get_value);
    }

    chal_gpio_get_pulldown(phy_info_ptr, capi_gpio_info_ptr->gpio_id, &downactive);
    chal_gpio_get_pullup(phy_info_ptr, capi_gpio_info_ptr->gpio_id, &upactive);

    if(downactive==0 && upactive==1){
        capi_gpio_info_ptr->pull_updown_type=CAPI_GPIO_PULL_UP_ACTIVE;
    }
    else if(downactive==1 && upactive==0){
        capi_gpio_info_ptr->pull_updown_type=CAPI_GPIO_PULL_DOWN_ACTIVE;
    }
    else {
        return RR_ERROR;
    }
    return RR_SUCCESS;
}


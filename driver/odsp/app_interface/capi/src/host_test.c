/**
 *
 * @file     host_test.c
 * @author   
 * @date     05/01/2019
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
#include "type_defns.h"
#include "common_def.h"
#include "access.h"
#include "chip_mode_def.h"
#include "capi_def.h"
#include "capi_test_def.h"
#include "host_test.h"

extern const cw_chip_port_info_t cw_cmode[];

/**
 * @brief        uint16_t host_test_get_valid_port_mask(uint8_t chip_mode)
 * @details     based on chip mode enum,  to get the valid port_mask
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  chip_mode:  chip mode index
 * 
 * @return     returns the valid port mask
 */

uint16_t host_test_get_valid_port_mask(uint8_t chip_mode)
{
    return cw_cmode[chip_mode-1].cw_chip_port_mask;
}

/**
 * @brief        host_test_get_port_lane_mask(uint8_t chip_mode, uint8_t is_lw, uint16_t port_idx)
 * @details     based on chip mode enum, port_idx  to get the valid bh_lane_mask or lw_lane_mask
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  chip_mode:  chip mode index
 * @param[in]  is_lw:  0-client; 1-line
 * @param[in]  port_idx:  port index
 * 
 * @return     returns the lw/bh lane_mask for specified port
 */

uint16_t host_test_get_port_lane_mask(uint8_t chip_mode, uint8_t is_lw, uint16_t port_idx)
{
    if(is_lw)
        return cw_cmode[chip_mode-1].cw_chip_lw_lane_mask[port_idx];
    else
        return cw_cmode[chip_mode-1].cw_chip_bh_lane_mask[port_idx];
}


/**
 * @brief        host_test_check_port_validation(uint8_t chip_mode, uint16_t port_idx)
 * @details     based on chip mode enum, port_idx  validate the port idx validation
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  chip_mode:  chip mode index
 * @param[in]  port_idx:  port index
 * 
 * @return     returns specified port is valid or not
 */

uint8_t host_test_check_port_validation(uint8_t chip_mode, uint16_t port_idx)
{
    return ((cw_cmode[chip_mode-1].cw_chip_port_mask&(1<<port_idx))?1:0);
}


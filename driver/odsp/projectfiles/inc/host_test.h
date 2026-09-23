/**
 *
 * @file host_test.h
 * @author   Firmware Team
 * @date     4/01/2019
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
#ifndef HOST_TEST_H
#define HOST_TEST_H

#ifdef __cplusplus
extern "C" {
#endif


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


uint16_t host_test_get_valid_port_mask(uint8_t chip_mode);

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

uint16_t host_test_get_port_lane_mask(uint8_t chip_mode, uint8_t is_lw, uint16_t port_idx);


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


uint8_t host_test_check_port_validation(uint8_t chip_mode, uint16_t port_idx);


#ifdef __cplusplus
}
#endif

#endif /*HOST_TEST_H*/


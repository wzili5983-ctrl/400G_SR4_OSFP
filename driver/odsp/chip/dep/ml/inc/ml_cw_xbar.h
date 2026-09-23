
/**
 *
 * @file ml_cw_xbar.h
 * @author  
 * @date     09/01/2018
 * @version 1.0
 *
 * $Copyright: Copyright 2018 Broadcom INC.
 * This program is the proprietary software of Broadcom INC
 * and/or its licensors, and may only be used, duplicated, modified
 * or distributed pursuant to the terms and conditions of a separate,
 * written license agreement executed between you and Broadcom
 * (an "Authorized License").  Except as set forth in an Authorized
 * License, Broadcom grants no license (express or implied), right
 * to use, or waiver of any kind with respect to the Software, and
 * Broadcom expressly reserves all rights in and to the Software
 * and all intellectual property rights therein.  IF YOU HAVE
 * NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE
 * IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE
 * ALL USE OF THE SOFTWARE.
 * 
 * Except as expressly set forth in the Authorized License,
 * 
 * 1.      This program, including its structure, sequence and organization,
 * constitutes the valuable trade secrets of Broadcom, and you shall use
 * all reasonable efforts to protect the confidentiality thereof,
 * and to use this information only in connection with your use of
 * Broadcom integrated circuit products.
 * 
 * 2.      TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS
 * PROVIDED "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
 * REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
 * OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
 * DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
 * NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
 * ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
 * OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 * 
 * 3.      TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL
 * BROADCOM OR ITS LICENSORS BE LIABLE FOR  CONSEQUENTIAL,
 * INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER
 * ARISING OUT OF OR IN ANY WAY RELATING TO YOUR USE OF OR INABILITY
 * TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF
 * THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR USD 1.00,
 * WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING
 * ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.$
 *
 * @brief   this file includes CW retimer mode config util functions.
 *
 * @section
 * 
 */


#ifndef ML_CW_XBAR_H
#define ML_CW_XBAR_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief       ml_cw_set_quad_swap(uint8_t *p2l_rx_map, uint8_t *l2p_rx_map, uint8_t *p2l_tx_map,  uint8_t *l2p_tx_map)
 * @details     This function caculate crossbar settingfor mix mode (100G x 4 + 50G x 4) 
 *
 * @param[in/out]   p2l_rx_map:
 * @param[in/out]   l2p_rx_map:
 * @param[in/out]   p2l_tx_map:
 * @param[in/out]   l2p_tx_map:
 *
 * @return
 */
void ml_cw_set_quad_swap(uint8_t *p2l_rx_map, uint8_t *l2p_rx_map, uint8_t *p2l_tx_map,  uint8_t *l2p_tx_map);

/**
 * @brief       ml_cw_set_xbar_m1_gearbox(uint8_t *p2l_rx_map, uint8_t *l2p_rx_map, uint8_t *p2l_tx_map,  uint8_t *l2p_tx_map)
 * @details     This function caculate crossbar settingfor M1 gearbox modes (LW lane 2<->4, 3<->5) for 874xx package
 *
 * @param[in/out]   p2l_rx_map:
 * @param[in/out]   l2p_rx_map:
 * @param[in/out]   p2l_tx_map:
 * @param[in/out]   l2p_tx_map:
 *
 * @return
 */
void ml_cw_set_xbar_m1_gearbox(uint8_t *p2l_rx_map, uint8_t *l2p_rx_map, uint8_t *p2l_tx_map,  uint8_t *l2p_tx_map);

/**
 * @brief       ml_cw_get_quad_swap_enabled(void)
 * @details     This function get quad swap enable status
 *
 * @return      1 - quad swap enabled, 0 - disabled
 */
uint8_t ml_cw_get_quad_swap_enabled(void);

/**
 * @brief       ml_cw_get_m1_modes_lane_swap_enabled(void)
 * @details     This function get M1 chip modes lane swap enable status, (LW lane 2<->4, 3<->5) for 874xx package
 *
 * @return      1 - M1 chip modes lane swap enabled, 0 - disabled
 */
uint8_t ml_cw_get_m1_modes_lane_swap_enabled(void);

/**
 * @brief       ml_cw_get_xbar_logical_lane_mask(uint16_t lane_mask)
 * @details     This function get CW xbar logical lane mask
 *
 * @param[in]   lane_mask:
 *
 * @return      CW xbar logical lane mask
 */
uint16_t ml_cw_get_xbar_logical_lane_mask(uint16_t lane_mask, phy_side_t phy_side);

/**
 * @brief       ml_cw_get_xbar_logical_lane_id(uint8_t lane_id)
 * @details     This function get CW xbar logical lane id
 *
 * @param[in]   lane_id:
 *
 * @return      CW xbar logical lane id
 */
uint8_t ml_cw_get_xbar_logical_lane_id(uint8_t lane_id, phy_side_t phy_side);

#ifdef __cplusplus
}
#endif

#endif /**< ML_CW_XBAR_H*/


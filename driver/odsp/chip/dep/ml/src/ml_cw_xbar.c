/**
 *
 * @file ml_cw_xbar.c
 * @author  
 * @date     03/22/2022
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
 * @brief   this file includes cHAL function implementation.
 *
 * @section
 * 
 */
#include <stdio.h>
#include "regs_common.h"
#include "chip_common_config_ind.h"
#include "common_util.h"
#include "chip_mode_def.h"
#include "capi_def.h"
#include "cw_def.h"
#include "ml_cw_xbar.h"


/**
 * @brief       ml_cw_get_quad_swap_enabled(void)
 * @details     This function get quad swap enable status
 *
 * @return      1 - quad swap enabled, 0 - disabled
 */
uint8_t ml_cw_get_quad_swap_enabled(void)
{
    phy_info_t phy_info = {0};
    
    phy_info.base_addr = APB2_SLV3_REGS;
    return (uint8_t)hsip_rd_field_(&phy_info, FW_INTERNAL_CONFIG_REG_2, QUAD_SWAP_EN);
}

/**
 * @brief       ml_cw_get_m1_modes_lane_swap_enabled(void)
 * @details     This function get M1 chip modes lane swap enable status, (LW lane 2<->4, 3<->5) for 874xx package
 *
 * @return      1 - M1 chip modes lane swap enabled, 0 - disabled
 */
uint8_t ml_cw_get_m1_modes_lane_swap_enabled(void)
{
    phy_info_t phy_info = {0};

    phy_info.base_addr = APB2_SLV3_REGS;
    return (uint8_t)hsip_rd_field_(&phy_info, FW_INTERNAL_CONFIG_REG_2, M1_MODES_LANE_SWAP_EN);
}

/**
 * @brief       ml_cw_get_xbar_logical_lane_mask(uint16_t lane_mask)
 * @details     This function get CW xbar logical lane mask
 *
 * @param[in]   lane_mask:
 *
 * @return      CW xbar logical lane mask
 */
uint16_t ml_cw_get_xbar_logical_lane_mask(uint16_t lane_mask, phy_side_t phy_side)
{    
    if (ml_cw_get_quad_swap_enabled()) {
        // quad is swapped, rotate the lane mask bits to get CW lane mask
        return (lane_mask & 0xF0) ? (lane_mask >> 4) : (lane_mask << 4);
    } else if ((phy_side == PHY_MEDIA_SIDE) && ml_cw_get_m1_modes_lane_swap_enabled()) {
        // (LW lane 2<->4, 3<->5) for 874xx package, rotate the lane mask bits to get CW lane mask
        if (lane_mask & 0xC)
            return (lane_mask << 2);
        else if (lane_mask & 0x30)
            return (lane_mask >> 2);
        else
            return lane_mask;
    } else {
        return lane_mask;
    }    
}


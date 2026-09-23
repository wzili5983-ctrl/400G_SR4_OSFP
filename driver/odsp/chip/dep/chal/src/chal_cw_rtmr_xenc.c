/**
 *
 * @file    chal_xenc.c
 * @author  DE
 * @date    09/2018
 * @version 1.0
 *
 *  * @property   $Copyright: Copyright 2018 Broadcom INC. 
 *This program is the proprietary software of Broadcom INC
 *and/or its licensors, and may only be used, duplicated, modified
 *or distributed pursuant to the terms and conditions of a separate,
 *written license agreement executed between you and Broadcom
 *(an "Authorized License").  Except as set forth in an Authorized
 *License, Broadcom grants no license (express or implied), right
 *to use, or waiver of any kind with respect to the Software, and
 *Broadcom expressly reserves all rights in and to the Software
 *and all intellectual property rights therein.  IF YOU HAVE
 *NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE
 *IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE
 *ALL USE OF THE SOFTWARE.
 *
 *Except as expressly set forth in the Authorized License,
 *
 *1.     This program, including its structure, sequence and organization,
 *constitutes the valuable trade secrets of Broadcom, and you shall use
 *all reasonable efforts to protect the confidentiality thereof,
 *and to use this information only in connection with your use of
 *Broadcom integrated circuit products.
 *
 *2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS
 *PROVIDED "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
 *REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
 *OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
 *DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
 *NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
 *ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 *CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
 *OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 *3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL
 *BROADCOM OR ITS LICENSORS BE LIABLE FOR  CONSEQUENTIAL,
 *INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER
 *ARISING OUT OF OR IN ANY WAY RELATING TO YOUR USE OF OR INABILITY
 *TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF THE
 *POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF
 *THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR USD 1.00,
 *WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING
 *ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.$
 * @brief  This file includes cHAL function implementation for: xencoder gbox and xencoder
 *
 * @section
 * 
 */

#include "regs_common.h"
#include "chip_mode_def.h"
#include "cw_def.h"
#include "chal_cw_rtmr_xenc.h"


// checked RETIMER_IGR or RETIMER_EGR
// xenc gbox257-160 read and write pointer gap. default is 1. Ranging 0-15, the middle number will result in the biggest gap (latency)
return_result_t chal_cw_rtmr_xenc_gbox_gap (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, uint8_t xenc_gap )
{
    switch (cur_mode_parameter_ptr->speed) {
        case (SPEED_100G):
            if (cur_port_config_ptr->port_100g_en[0] == PORT_ON) {  // xencoder only used in 100g P0
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_GAP_CTRL_REG, MD_XENC_GBOX_EXTRA_GAP_M1_0, xenc_gap);
            }
            else if (cur_port_config_ptr->port_100g_en[1]  == PORT_ON) { // xencoder only used in 100g P1
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_GAP_CTRL1_REG, MD_XENC_GBOX_EXTRA_GAP_M1_2, xenc_gap);
            }
            else if (cur_port_config_ptr->port_100g_en[2]  == PORT_ON) { // xencoder only used in 100g P2
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_GAP_CTRL2_REG, MD_XENC_GBOX_EXTRA_GAP_M1_4, xenc_gap);
            }
            else if (cur_port_config_ptr->port_100g_en[3]  == PORT_ON) { // xencoder only used in 100g P3
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_GAP_CTRL3_REG, MD_XENC_GBOX_EXTRA_GAP_M1_6, xenc_gap);
            }
            break;
        case (SPEED_200G):
            if (cur_port_config_ptr->port_200g_en[0] == PORT_ON) {  // xencoder only used in 200g P0
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_GAP_CTRL_REG, MD_XENC_GBOX_EXTRA_GAP_M1_0, xenc_gap);
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_GAP_CTRL1_REG, MD_XENC_GBOX_EXTRA_GAP_M1_2, xenc_gap);
            }
            else if (cur_port_config_ptr->port_200g_en[1]  == PORT_ON) { // xencoder only used in 200g P2
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_GAP_CTRL2_REG, MD_XENC_GBOX_EXTRA_GAP_M1_4, xenc_gap);
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_GAP_CTRL3_REG, MD_XENC_GBOX_EXTRA_GAP_M1_6, xenc_gap);
            }
            break;
        case (SPEED_400G):
            if (cur_port_config_ptr->port_400g_en == PORT_ON) {  // xencoder only used in 400g P0
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_GAP_CTRL_REG, MD_XENC_GBOX_EXTRA_GAP_M1_0, xenc_gap);
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_GAP_CTRL1_REG, MD_XENC_GBOX_EXTRA_GAP_M1_2, xenc_gap);
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_GAP_CTRL2_REG, MD_XENC_GBOX_EXTRA_GAP_M1_4, xenc_gap);
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_GAP_CTRL3_REG, MD_XENC_GBOX_EXTRA_GAP_M1_6, xenc_gap);
            }
            break;
        case (SPEED_50G):
            if (cur_port_config_ptr->port_50g_en[0] == PORT_ON) {  // xencoder only used in 50g P0
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_GAP_CTRL_REG, MD_XENC_GBOX_EXTRA_GAP_M1_0, xenc_gap);
            } else if (cur_port_config_ptr->port_50g_en[1] == PORT_ON) {  // xencoder only used in 50g P1
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_GAP_CTRL_REG, MD_XENC_GBOX_EXTRA_GAP_M1_1, xenc_gap);
            } else if (cur_port_config_ptr->port_50g_en[2] == PORT_ON) {  // xencoder only used in 50g P2
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_GAP_CTRL1_REG, MD_XENC_GBOX_EXTRA_GAP_M1_2, xenc_gap);
            } else if (cur_port_config_ptr->port_50g_en[3] == PORT_ON) {  // xencoder only used in 50g P3
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_GAP_CTRL1_REG, MD_XENC_GBOX_EXTRA_GAP_M1_3, xenc_gap);
            } else if (cur_port_config_ptr->port_50g_en[4] == PORT_ON) {  // xencoder only used in 50g P4
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_GAP_CTRL2_REG, MD_XENC_GBOX_EXTRA_GAP_M1_4, xenc_gap);
            } else if (cur_port_config_ptr->port_50g_en[5] == PORT_ON) {  // xencoder only used in 50g P5
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_GAP_CTRL2_REG, MD_XENC_GBOX_EXTRA_GAP_M1_5, xenc_gap);
            } else if (cur_port_config_ptr->port_50g_en[6] == PORT_ON) {  // xencoder only used in 50g P6
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_GAP_CTRL3_REG, MD_XENC_GBOX_EXTRA_GAP_M1_6, xenc_gap);
            } else if (cur_port_config_ptr->port_50g_en[7] == PORT_ON) {  // xencoder only used in 50g P7
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_GAP_CTRL3_REG, MD_XENC_GBOX_EXTRA_GAP_M1_7, xenc_gap);
            }
            break;
        case (SPEED_25G):
            if (cur_port_config_ptr->port_25g_en[0] == PORT_ON) {  // xencoder only used in 25g P0
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_GAP_CTRL_REG, MD_XENC_GBOX_EXTRA_GAP_M1_0, xenc_gap);
            } else if (cur_port_config_ptr->port_25g_en[1] == PORT_ON) {  // xencoder only used in 25g P1
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_GAP_CTRL_REG, MD_XENC_GBOX_EXTRA_GAP_M1_1, xenc_gap);
            } else if (cur_port_config_ptr->port_25g_en[2] == PORT_ON) {  // xencoder only used in 25g P2
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_GAP_CTRL1_REG, MD_XENC_GBOX_EXTRA_GAP_M1_2, xenc_gap);
            } else if (cur_port_config_ptr->port_25g_en[3] == PORT_ON) {  // xencoder only used in 25g P3
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_GAP_CTRL1_REG, MD_XENC_GBOX_EXTRA_GAP_M1_3, xenc_gap);
            } else if (cur_port_config_ptr->port_25g_en[4] == PORT_ON) {  // xencoder only used in 25g P4
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_GAP_CTRL2_REG, MD_XENC_GBOX_EXTRA_GAP_M1_4, xenc_gap);
            } else if (cur_port_config_ptr->port_25g_en[5] == PORT_ON) {  // xencoder only used in 25g P5
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_GAP_CTRL2_REG, MD_XENC_GBOX_EXTRA_GAP_M1_5, xenc_gap);
            } else if (cur_port_config_ptr->port_25g_en[6] == PORT_ON) {  // xencoder only used in 25g P6
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_GAP_CTRL3_REG, MD_XENC_GBOX_EXTRA_GAP_M1_6, xenc_gap);
            } else if (cur_port_config_ptr->port_25g_en[7] == PORT_ON) {  // xencoder only used in 25g P7
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_GAP_CTRL3_REG, MD_XENC_GBOX_EXTRA_GAP_M1_7, xenc_gap);
            }
            break;
    }

    return RR_SUCCESS;
}

// checked RETIMER_IGR or RETIMER_EGR
// [HSIP] Why 25g and 50g only even ports?
return_result_t chal_cw_rtmr_xenc_width_ctrl (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr)
{
    switch (cur_mode_parameter_ptr->speed) {
        case (SPEED_100G):
            if (cur_port_config_ptr->port_100g_en[0] == PORT_ON) {  // xencoder only used in 100g P0
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_WIDTH_CTRL_REG, MD_160_80B_0, 0x1);
            }
            else if (cur_port_config_ptr->port_100g_en[1]  == PORT_ON) { // xencoder only used in 100g P1
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_WIDTH_CTRL_REG, MD_160_80B_2, 0x1);
            }
            else if (cur_port_config_ptr->port_100g_en[2]  == PORT_ON) { // xencoder only used in 100g P2
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_WIDTH_CTRL_REG, MD_160_80B_4, 0x1);
            }
            else if (cur_port_config_ptr->port_100g_en[3]  == PORT_ON) { // xencoder only used in 100g P3
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_WIDTH_CTRL_REG, MD_160_80B_6, 0x1);
            }
            break;
        case (SPEED_200G):
            if (cur_port_config_ptr->port_200g_en[0] == PORT_ON) {  // xencoder only used in 200g P0
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_WIDTH_CTRL_REG, MD_160_80B_0, 0x1);
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_WIDTH_CTRL_REG, MD_160_80B_2, 0x1);
            }
            else if (cur_port_config_ptr->port_200g_en[1]  == PORT_ON) { // xencoder only used in 200g P2
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_WIDTH_CTRL_REG, MD_160_80B_4, 0x1);
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_WIDTH_CTRL_REG, MD_160_80B_6, 0x1);
            }
            break;
        case (SPEED_400G):
            if (cur_port_config_ptr->port_400g_en == PORT_ON) {  // xencoder only used in 200g P0
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_WIDTH_CTRL_REG, MD_160_80B_0, 0x1);
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_WIDTH_CTRL_REG, MD_160_80B_2, 0x1);
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_WIDTH_CTRL_REG, MD_160_80B_4, 0x1);
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_WIDTH_CTRL_REG, MD_160_80B_6, 0x1);
            }
            break;
        case (SPEED_50G):
            if (cur_port_config_ptr->port_50g_en[0] == PORT_ON) {  // xencoder only used in 50g P0
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_WIDTH_CTRL_REG, MD_160_80B_0, 0x0);
            }
            else if (cur_port_config_ptr->port_50g_en[2] == PORT_ON ) { // xencoder only used in 50g P2
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_WIDTH_CTRL_REG, MD_160_80B_2, 0x0);
            }
            else if (cur_port_config_ptr->port_50g_en[4]  == PORT_ON) { // xencoder only used in 50g P4
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_WIDTH_CTRL_REG, MD_160_80B_4, 0x0);
            }
            else if (cur_port_config_ptr->port_50g_en[6]  == PORT_ON) { // xencoder only used in 50g P6
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_WIDTH_CTRL_REG, MD_160_80B_6, 0x0);
            }
            break;
        case (SPEED_25G):
            if (cur_port_config_ptr->port_25g_en[0] == PORT_ON) {  // xencoder only used in 25g P0
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_WIDTH_CTRL_REG, MD_160_80B_0, 0x0);
            }
            else if (cur_port_config_ptr->port_25g_en[2] == PORT_ON ) { // xencoder only used in 25g P2
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_WIDTH_CTRL_REG, MD_160_80B_2, 0x0);
            }
            else if (cur_port_config_ptr->port_25g_en[4]  == PORT_ON) { // xencoder only used in 25g P4
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_WIDTH_CTRL_REG, MD_160_80B_4, 0x0);
            }
            else if (cur_port_config_ptr->port_25g_en[6]  == PORT_ON) { // xencoder only used in 25g P6
                hsip_wr_field_(phy_info_ptr, XENC_GBOX_WIDTH_CTRL_REG, MD_160_80B_6, 0x0);
            }
            break;
    }  

    return RR_SUCCESS;
}


// Checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_xenc_scr_ctrl (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr)
{
    switch (cur_mode_parameter_ptr->speed) {
        case (SPEED_400G):
            if (cur_port_config_ptr->port_400g_en) {  // xencoder only used in 400g P0
                hsip_wr_field_(phy_info_ptr, SCRAMBLER_CTRL_REG, BYPASS_SCRAMBLER_0, 0x0);
                hsip_wr_field_(phy_info_ptr, SCRAMBLER_CTRL_REG, BYPASS_SCRAMBLER_1, 0x0);
                hsip_wr_field_(phy_info_ptr, SCRAMBLER_CTRL_REG, SCRAMBLER_CKEN_0, 0x1);
            }
            break;
        case (SPEED_200G):
            if (cur_port_config_ptr->port_200g_en[0]) {  // xencoder only used in 200g P0
                 hsip_wr_field_(phy_info_ptr, SCRAMBLER_CTRL_REG, BYPASS_SCRAMBLER_0, 0x0);
                 hsip_wr_field_(phy_info_ptr, SCRAMBLER_CTRL_REG, SCRAMBLER_CKEN_0, 0x1);
            }
            else if (cur_port_config_ptr->port_200g_en[1] ) { // xencoder only used in 200g P1
                 hsip_wr_field_(phy_info_ptr, SCRAMBLER_CTRL_REG, BYPASS_SCRAMBLER_1, 0x0);
                 hsip_wr_field_(phy_info_ptr, SCRAMBLER_CTRL_REG, SCRAMBLER_CKEN_1, 0x1);
            }
            break;
        default :
            hsip_wr_field_(phy_info_ptr, SCRAMBLER_CTRL_REG, BYPASS_SCRAMBLER_0, 0x1);
            hsip_wr_field_(phy_info_ptr, SCRAMBLER_CTRL_REG, BYPASS_SCRAMBLER_1, 0x1);
            hsip_wr_field_(phy_info_ptr, SCRAMBLER_CTRL_REG, SCRAMBLER_CKEN_0, 0x0);
            hsip_wr_field_(phy_info_ptr, SCRAMBLER_CTRL_REG, SCRAMBLER_CKEN_1, 0x0);
            break;
    }

    return RR_SUCCESS;
}

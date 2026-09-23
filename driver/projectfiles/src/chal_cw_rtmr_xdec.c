/**
 *
 * @file    chal_xdec.c
 * @author  DE
 * @date    09/2018
 * @version 1.0
 *
 * @property   $Copyright: Copyright 2018 Broadcom INC. 
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
 *
 * @brief  This file includes cHAL function implementation for: xdecoder gbox and xdecoder
 *
 * @section
 * 
 */

#include "regs_common.h"
#include "chip_mode_def.h"
#include "cw_def.h"
#include "chal_cw_rtmr_xdec.h"

// checked RETIMER_IGR or RETIMER_EGR
// xdec gbox160-257 read and write pointer gap. default is 1. Ranging 0-15, the middle number will result in the biggest gap (latency)
return_result_t chal_cw_rtmr_xdec_gbox_gap (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, uint8_t xdec_gap )
{
    switch (cur_mode_parameter_ptr->speed) {
        case (SPEED_100G):
            if (cur_port_config_ptr->port_100g_en[0] == PORT_ON) {  // xdecoder only used in 100g P0
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_GAP_REG, MD_XDEC_GBOX_EXTRA_GAP_M1_0, xdec_gap);
            }
            else if (cur_port_config_ptr->port_100g_en[1]  == PORT_ON) { // xdecoder only used in 100g P1
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_GAP_1_REG, MD_XDEC_GBOX_EXTRA_GAP_M1_2, xdec_gap);
            }
            else if (cur_port_config_ptr->port_100g_en[2]  == PORT_ON) { // xdecoder only used in 100g P2
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_GAP_2_REG, MD_XDEC_GBOX_EXTRA_GAP_M1_4, xdec_gap);
            }
            else if (cur_port_config_ptr->port_100g_en[3]  == PORT_ON) { // xdecoder only used in 100g P3
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_GAP_3_REG, MD_XDEC_GBOX_EXTRA_GAP_M1_6, xdec_gap);
            }
            break;
        case (SPEED_200G):
            if (cur_port_config_ptr->port_200g_en[0] == PORT_ON) {  // xdecoder only used in 200g P0
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_GAP_REG, MD_XDEC_GBOX_EXTRA_GAP_M1_0, xdec_gap);
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_GAP_1_REG, MD_XDEC_GBOX_EXTRA_GAP_M1_2, xdec_gap);
            }
            else if (cur_port_config_ptr->port_200g_en[1]  == PORT_ON) { // xdecoder only used in 200g P2
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_GAP_2_REG, MD_XDEC_GBOX_EXTRA_GAP_M1_4, xdec_gap);
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_GAP_3_REG, MD_XDEC_GBOX_EXTRA_GAP_M1_6, xdec_gap);
            }
            break;
        case (SPEED_400G):
            if (cur_port_config_ptr->port_400g_en == PORT_ON) {  // xdecoder only used in 400g P0
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_GAP_REG, MD_XDEC_GBOX_EXTRA_GAP_M1_0, xdec_gap);
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_GAP_1_REG, MD_XDEC_GBOX_EXTRA_GAP_M1_2, xdec_gap);
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_GAP_2_REG, MD_XDEC_GBOX_EXTRA_GAP_M1_4, xdec_gap);
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_GAP_3_REG, MD_XDEC_GBOX_EXTRA_GAP_M1_6, xdec_gap);
            }
            break;
        case (SPEED_50G):
            if (cur_port_config_ptr->port_50g_en[0] == PORT_ON) {  // xdecoder only used in 50g P0
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_GAP_REG, MD_XDEC_GBOX_EXTRA_GAP_M1_0, xdec_gap);
            } else if (cur_port_config_ptr->port_50g_en[1] == PORT_ON) {  // xdecoder only used in 50g P1
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_GAP_REG, MD_XDEC_GBOX_EXTRA_GAP_M1_1, xdec_gap);
            } else if (cur_port_config_ptr->port_50g_en[2] == PORT_ON) {  // xdecoder only used in 50g P2
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_GAP_1_REG, MD_XDEC_GBOX_EXTRA_GAP_M1_2, xdec_gap);
            } else if (cur_port_config_ptr->port_50g_en[3] == PORT_ON) {  // xdecoder only used in 50g P3
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_GAP_1_REG, MD_XDEC_GBOX_EXTRA_GAP_M1_3, xdec_gap);
            } else if (cur_port_config_ptr->port_50g_en[4] == PORT_ON) {  // xdecoder only used in 50g P4
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_GAP_2_REG, MD_XDEC_GBOX_EXTRA_GAP_M1_4, xdec_gap);
            } else if (cur_port_config_ptr->port_50g_en[5] == PORT_ON) {  // xdecoder only used in 50g P5
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_GAP_2_REG, MD_XDEC_GBOX_EXTRA_GAP_M1_5, xdec_gap);
            } else if (cur_port_config_ptr->port_50g_en[6] == PORT_ON) {  // xdecoder only used in 50g P6
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_GAP_3_REG, MD_XDEC_GBOX_EXTRA_GAP_M1_6, xdec_gap);
            } else if (cur_port_config_ptr->port_50g_en[7] == PORT_ON) {  // xdecoder only used in 50g P7
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_GAP_3_REG, MD_XDEC_GBOX_EXTRA_GAP_M1_7, xdec_gap);
            }
            break;
        case (SPEED_25G):
            if (cur_port_config_ptr->port_25g_en[0] == PORT_ON) {  // xdecoder only used in 25g P0
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_GAP_REG, MD_XDEC_GBOX_EXTRA_GAP_M1_0, xdec_gap);
            } else if (cur_port_config_ptr->port_25g_en[1] == PORT_ON) {  // xdecoder only used in 25g P1
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_GAP_REG, MD_XDEC_GBOX_EXTRA_GAP_M1_1, xdec_gap);
            } else if (cur_port_config_ptr->port_25g_en[2] == PORT_ON) {  // xdecoder only used in 25g P2
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_GAP_1_REG, MD_XDEC_GBOX_EXTRA_GAP_M1_2, xdec_gap);
            } else if (cur_port_config_ptr->port_25g_en[3] == PORT_ON) {  // xdecoder only used in 25g P3
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_GAP_1_REG, MD_XDEC_GBOX_EXTRA_GAP_M1_3, xdec_gap);
            } else if (cur_port_config_ptr->port_25g_en[4] == PORT_ON) {  // xdecoder only used in 25g P4
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_GAP_2_REG, MD_XDEC_GBOX_EXTRA_GAP_M1_4, xdec_gap);
            } else if (cur_port_config_ptr->port_25g_en[5] == PORT_ON) {  // xdecoder only used in 25g P5
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_GAP_2_REG, MD_XDEC_GBOX_EXTRA_GAP_M1_5, xdec_gap);
            } else if (cur_port_config_ptr->port_25g_en[6] == PORT_ON) {  // xdecoder only used in 25g P6
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_GAP_3_REG, MD_XDEC_GBOX_EXTRA_GAP_M1_6, xdec_gap);
            } else if (cur_port_config_ptr->port_25g_en[7] == PORT_ON) {  // xdecoder only used in 25g P7
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_GAP_3_REG, MD_XDEC_GBOX_EXTRA_GAP_M1_7, xdec_gap);
            }
            break;
    }

    return RR_SUCCESS;
}

// checked RETIMER_IGR or RETIMER_EGR
// [HL] Why 25g and 50g only even ports?
return_result_t chal_cw_rtmr_xdec_width_ctrl (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr)
{
    switch (cur_mode_parameter_ptr->speed) {
        case (SPEED_100G):
            if (cur_port_config_ptr->port_100g_en[0] == PORT_ON) {  // xdecoder only used in 100g P0
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_WIDTH_CTRL_REG, MD_160_80B_0, 0x1);
            }
            else if (cur_port_config_ptr->port_100g_en[1]  == PORT_ON) { // xdecoder only used in 100g P1
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_WIDTH_CTRL_REG, MD_160_80B_2, 0x1);
            }
            else if (cur_port_config_ptr->port_100g_en[2]  == PORT_ON) { // xdecoder only used in 100g P2
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_WIDTH_CTRL_REG, MD_160_80B_4, 0x1);
            }
            else if (cur_port_config_ptr->port_100g_en[3]  == PORT_ON) { // xdecoder only used in 100g P3
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_WIDTH_CTRL_REG, MD_160_80B_6, 0x1);
            }
            break;
        case (SPEED_200G):
            if (cur_port_config_ptr->port_200g_en[0] == PORT_ON) {  // xdecoder only used in 200g P0
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_WIDTH_CTRL_REG, MD_160_80B_0, 0x1);
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_WIDTH_CTRL_REG, MD_160_80B_2, 0x1);
            }
            else if (cur_port_config_ptr->port_200g_en[1]  == PORT_ON) { // xdecoder only used in 200g P2
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_WIDTH_CTRL_REG, MD_160_80B_4, 0x1);
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_WIDTH_CTRL_REG, MD_160_80B_6, 0x1);
            }
            break;
        case (SPEED_400G):
            if (cur_port_config_ptr->port_400g_en == PORT_ON) {  // xdecoder only used in 200g P0
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_WIDTH_CTRL_REG, MD_160_80B_0, 0x1);
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_WIDTH_CTRL_REG, MD_160_80B_2, 0x1);
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_WIDTH_CTRL_REG, MD_160_80B_4, 0x1);
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_WIDTH_CTRL_REG, MD_160_80B_6, 0x1);
            }
            break;
        case (SPEED_50G):
            if (cur_port_config_ptr->port_50g_en[0] == PORT_ON) {  // xdecoder only used in 50g P0
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_WIDTH_CTRL_REG, MD_160_80B_0, 0x0);
            }
            else if (cur_port_config_ptr->port_50g_en[2] == PORT_ON ) { // xdecoder only used in 50g P2
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_WIDTH_CTRL_REG, MD_160_80B_2, 0x0);
            }
            else if (cur_port_config_ptr->port_50g_en[4]  == PORT_ON) { // xdecoder only used in 50g P4
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_WIDTH_CTRL_REG, MD_160_80B_4, 0x0);
            }
            else if (cur_port_config_ptr->port_50g_en[6]  == PORT_ON) { // xdecoder only used in 50g P6
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_WIDTH_CTRL_REG, MD_160_80B_6, 0x0);
            }
            break;
        case (SPEED_25G):
            if (cur_port_config_ptr->port_25g_en[0] == PORT_ON) {  // xdecoder only used in 25g P0
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_WIDTH_CTRL_REG, MD_160_80B_0, 0x0);
            }
            else if (cur_port_config_ptr->port_25g_en[2] == PORT_ON ) { // xdecoder only used in 25g P2
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_WIDTH_CTRL_REG, MD_160_80B_2, 0x0);
            }
            else if (cur_port_config_ptr->port_25g_en[4]  == PORT_ON) { // xdecoder only used in 25g P4
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_WIDTH_CTRL_REG, MD_160_80B_4, 0x0);
            }
            else if (cur_port_config_ptr->port_25g_en[6]  == PORT_ON) { // xdecoder only used in 25g P6
                hsip_wr_field_(phy_info_ptr, XDEC_GBOX_WIDTH_CTRL_REG, MD_160_80B_6, 0x0);
            }
            break;
    }  

    return RR_SUCCESS;
}


// checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_xdec_dscr_ctrl (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr)
{
    switch (cur_mode_parameter_ptr->speed) {
        case (SPEED_400G):
            if (cur_port_config_ptr->port_400g_en == PORT_ON) {  // xdecoder only used in 400g P0
                hsip_wr_field_(phy_info_ptr, XDEC_FEC_TYPE_CTRL_REG, BYPASS_DESCRAMBLER_0, 0x0);
                hsip_wr_field_(phy_info_ptr, XDEC_FEC_TYPE_CTRL_REG, BYPASS_DESCRAMBLER_1, 0x0);
                hsip_wr_field_(phy_info_ptr, XDEC_FEC_TYPE_CTRL_REG, DESCRAMBLER_CKEN_0, 0x1);
            }
            break;
        case (SPEED_200G):
            if (cur_port_config_ptr->port_200g_en[0] == PORT_ON) {  // xdecoder only used in 200g P0
                 hsip_wr_field_(phy_info_ptr, XDEC_FEC_TYPE_CTRL_REG, BYPASS_DESCRAMBLER_0, 0x0);
                 hsip_wr_field_(phy_info_ptr, XDEC_FEC_TYPE_CTRL_REG, DESCRAMBLER_CKEN_0, 0x1);
            }
            else if (cur_port_config_ptr->port_200g_en[1]  == PORT_ON) { // xdecoder only used in 200g P1
                 hsip_wr_field_(phy_info_ptr, XDEC_FEC_TYPE_CTRL_REG, BYPASS_DESCRAMBLER_1, 0x0);
                 hsip_wr_field_(phy_info_ptr, XDEC_FEC_TYPE_CTRL_REG, DESCRAMBLER_CKEN_1, 0x1);
            }
            break;
        default:
            hsip_wr_field_(phy_info_ptr, XDEC_FEC_TYPE_CTRL_REG, BYPASS_DESCRAMBLER_0, 0x1);
            hsip_wr_field_(phy_info_ptr, XDEC_FEC_TYPE_CTRL_REG, BYPASS_DESCRAMBLER_1, 0x1);
            hsip_wr_field_(phy_info_ptr, XDEC_FEC_TYPE_CTRL_REG, DESCRAMBLER_CKEN_0, 0x0);
            hsip_wr_field_(phy_info_ptr, XDEC_FEC_TYPE_CTRL_REG, DESCRAMBLER_CKEN_1, 0x0);
            break;

    }
    return RR_SUCCESS;
}

// checked Top
//return_result_t chal_cw_rtmr_rx_four_lane_pmd_ctrl (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t four_lane_pmd, cfg_egr_or_igr_t egr_or_igr)
//{
//    bool access = ((egr_or_igr==EGR) ? ((cur_mode_parameter_ptr->host_fec_type == CHIP_HOST_FEC_TYPE_RS528) || (cur_mode_parameter_ptr->host_fec_type == CHIP_HOST_FEC_TYPE_RS544)):
//                  ((cur_mode_parameter_ptr->line_fec_type == CHIP_LINE_FEC_TYPE_RS528) || (cur_mode_parameter_ptr->line_fec_type == CHIP_LINE_FEC_TYPE_RS544))) ? TRUE : FALSE;
//    uint8_t val = ((cur_mode_parameter_ptr->speed != SPEED_100G) || (four_lane_pmd != IS_ENABLED) || access == FALSE) ? 0 : 1;
//
//    if(egr_or_igr == EGR) {
//        if (cur_mode_parameter_ptr->fec_slice  & 0x3) {
//            hsip_wr_field_(phy_info_ptr, RSFEC_IEEE_50G_100G_EGR_0_RS_FEC_CONTROL_REGISTER_50G_100_EGR_0, FOUR_LANE_PMD_INDICATION, val);
//        } else if (cur_mode_parameter_ptr->fec_slice & 0xC) {
//            hsip_wr_field_(phy_info_ptr, RSFEC_IEEE_50G_100G_EGR_2_RS_FEC_CONTROL_REGISTER_50G_100_EGR_2, FOUR_LANE_PMD_INDICATION, val);
//        } 
//#if 0
//        else if (cur_mode_parameter_ptr->fec_slice & 0x30) {
//            hsip_wr_field_(phy_info_ptr, RSFEC_IEEE_50G_100G_EGR_4_RS_FEC_CONTROL_REGISTER_50G_100_EGR_4, FOUR_LANE_PMD_INDICATION, val);
//        } else if (cur_mode_parameter_ptr->fec_slice & 0xC0) {
//            hsip_wr_field_(phy_info_ptr, RSFEC_IEEE_50G_100G_EGR_6_RS_FEC_CONTROL_REGISTER_50G_100_EGR_6, FOUR_LANE_PMD_INDICATION, val);
//        }
//#endif
//    } 
//    else {
//        if (cur_mode_parameter_ptr->fec_slice  & 0x3) {
//            hsip_wr_field_(phy_info_ptr, RSFEC_IEEE_50G_100G_IGR_0_RS_FEC_CONTROL_REGISTER_50G_100_IGR_0, FOUR_LANE_PMD_INDICATION, val);
//        } else if (cur_mode_parameter_ptr->fec_slice & 0xC) {
//            hsip_wr_field_(phy_info_ptr, RSFEC_IEEE_50G_100G_IGR_2_RS_FEC_CONTROL_REGISTER_50G_100_IGR_2, FOUR_LANE_PMD_INDICATION, val);
//        } 
//#if 0
//        else if (cur_mode_parameter_ptr->fec_slice & 0x30) {
//            hsip_wr_field_(phy_info_ptr, RSFEC_IEEE_50G_100G_IGR_4_RS_FEC_CONTROL_REGISTER_50G_100_IGR_4, FOUR_LANE_PMD_INDICATION, val);
//        } else if (cur_mode_parameter_ptr->fec_slice & 0xC0) {
//            hsip_wr_field_(phy_info_ptr, RSFEC_IEEE_50G_100G_IGR_6_RS_FEC_CONTROL_REGISTER_50G_100_IGR_6, FOUR_LANE_PMD_INDICATION, val);
//        }
//#endif
//    } 
//    return RR_SUCCESS;
//}
//
//// checked Top
//return_result_t chal_cw_rtmr_tx_four_lane_pmd_ctrl (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t four_lane_pmd, cfg_egr_or_igr_t egr_or_igr) 
//{
//    bool access = ((egr_or_igr==IGR) ? ((cur_mode_parameter_ptr->host_fec_type == CHIP_HOST_FEC_TYPE_RS528) || (cur_mode_parameter_ptr->host_fec_type == CHIP_HOST_FEC_TYPE_RS544)):
//            ((cur_mode_parameter_ptr->line_fec_type == CHIP_LINE_FEC_TYPE_RS528) || (cur_mode_parameter_ptr->line_fec_type == CHIP_LINE_FEC_TYPE_RS544))) ? TRUE : FALSE;
//
//    uint8_t val = ((cur_mode_parameter_ptr->speed != SPEED_100G) || (four_lane_pmd != IS_ENABLED) || access == FALSE) ? 0 : 1;
//    if(egr_or_igr == EGR) {
//        if (cur_mode_parameter_ptr->fec_slice  & 0x3) {
//            hsip_wr_field_(phy_info_ptr, RSFEC_IEEE_50G_100G_EGR_0_RS_FEC_CONTROL_REGISTER_50G_100_EGR_0, FOUR_LANE_PMD_INDICATION, val);
//        } else if (cur_mode_parameter_ptr->fec_slice & 0xC) {
//            hsip_wr_field_(phy_info_ptr, RSFEC_IEEE_50G_100G_EGR_2_RS_FEC_CONTROL_REGISTER_50G_100_EGR_2, FOUR_LANE_PMD_INDICATION, val);
//        } 
//#if 0
//        else if (cur_mode_parameter_ptr->fec_slice & 0x30) {
//            hsip_wr_field_(phy_info_ptr, RSFEC_IEEE_50G_100G_EGR_4_RS_FEC_CONTROL_REGISTER_50G_100_EGR_4, FOUR_LANE_PMD_INDICATION, val);
//        } else if (cur_mode_parameter_ptr->fec_slice & 0xC0) {
//            hsip_wr_field_(phy_info_ptr, RSFEC_IEEE_50G_100G_EGR_6_RS_FEC_CONTROL_REGISTER_50G_100_EGR_6, FOUR_LANE_PMD_INDICATION, val);
//        }
//#endif
//    } 
//    else {
//        if (cur_mode_parameter_ptr->fec_slice  & 0x3) {
//            hsip_wr_field_(phy_info_ptr, RSFEC_IEEE_50G_100G_IGR_0_RS_FEC_CONTROL_REGISTER_50G_100_IGR_0, FOUR_LANE_PMD_INDICATION, val);
//        } else if (cur_mode_parameter_ptr->fec_slice & 0xC) {
//            hsip_wr_field_(phy_info_ptr, RSFEC_IEEE_50G_100G_IGR_2_RS_FEC_CONTROL_REGISTER_50G_100_IGR_2, FOUR_LANE_PMD_INDICATION, val);
//        }
//#if 0
//         else if (cur_mode_parameter_ptr->fec_slice & 0x30) {
//            hsip_wr_field_(phy_info_ptr, RSFEC_IEEE_50G_100G_IGR_4_RS_FEC_CONTROL_REGISTER_50G_100_IGR_4, FOUR_LANE_PMD_INDICATION, val);
//        } else if (cur_mode_parameter_ptr->fec_slice & 0xC0) {
//            hsip_wr_field_(phy_info_ptr, RSFEC_IEEE_50G_100G_IGR_6_RS_FEC_CONTROL_REGISTER_50G_100_IGR_6, FOUR_LANE_PMD_INDICATION, val);
//        }
//#endif
//    } 
//    return RR_SUCCESS;
//}

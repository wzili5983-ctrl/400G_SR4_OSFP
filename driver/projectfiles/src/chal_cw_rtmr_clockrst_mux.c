/**
 *
 * @file     chal_cw_rtmr_clockrst_mux.c
 * @author   
 * @date     10/2/2020
 * @version  1.0
 *
 * @property 
 * $Copyright: (c) 2020 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * @brief    brief description of the block source file, sequences/flow/etc.
 *
 * @section  description
 *
 */
#include "regs_common.h"
#include "common_def.h"
#include "chip_mode_def.h"
#include "cw_def.h"
#include "ml_cw_rtmr_modes.h"
#include "chal_cw_rtmr_clockrst_mux.h"

// Checked RETIMER_IGR or RETIMER_EGR
// FEC_PCS clock and reset muxing
// for example, slice 0,2,4,6 clocks are selected as 4X100G bundle host_rx_gbox read clocks
// for example, but only slice 0 clock will be selected as 1X400G bundle host_rx_gbox read clocks
return_result_t chal_cw_rtmr_fec_tmt_clk_rst_mux_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,  cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr)
{
    cfg_pam_or_nrz_t pam_or_nrz = (egr_or_igr == IGR) ? cur_mode_parameter_ptr->host_pam_or_nrz_type : cur_mode_parameter_ptr->line_pam_or_nrz_type;
    uint16_t lane_mask = (egr_or_igr == IGR) ? cur_mode_parameter_ptr->host_llane_mask : cur_mode_parameter_ptr->line_llane_mask;

    switch (cur_mode_parameter_ptr->speed) {
        case (SPEED_25G):
            if (cur_port_config_ptr->port_25g_en[0] == PORT_ON) {
                // no need for slice_0 
            }
            else if (cur_port_config_ptr->port_25g_en[1] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE1_CLK_RST_MUX, 0x1 );
            }
            else if (cur_port_config_ptr->port_25g_en[2] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE2_CLK_RST_MUX, 0x2 );

            }
            else if (cur_port_config_ptr->port_25g_en[3] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE3_CLK_RST_MUX, 0x2 );
            }
            else if (cur_port_config_ptr->port_25g_en[4] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE4_CLK_RST_MUX, 0x1 );
            }
            else if (cur_port_config_ptr->port_25g_en[5] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE5_CLK_RST_MUX, 0x2 );
            }
            else if (cur_port_config_ptr->port_25g_en[6] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE6_CLK_RST_MUX, 0x3 );
            }
            else if (cur_port_config_ptr->port_25g_en[7] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE7_CLK_RST_MUX, 0x3 );
            }
            break;
        case (SPEED_50G):
            if (cur_port_config_ptr->port_50g_en[0] == PORT_ON) {
                // no need for slice_0 
                if (pam_or_nrz == CW_NRZ) {
                    hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE1_CLK_RST_MUX, 0x0 );
                }
            }
            else if (cur_port_config_ptr->port_50g_en[1] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE1_CLK_RST_MUX, 0x1 );
            }
            else if (cur_port_config_ptr->port_50g_en[2] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE2_CLK_RST_MUX, (lane_mask==0x2)?0x1:0x2 ); // handle H23_L1 
                if (pam_or_nrz == CW_NRZ) {
                    hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE3_CLK_RST_MUX, 0x1 );
                }

            }
            else if (cur_port_config_ptr->port_50g_en[3] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE3_CLK_RST_MUX, 0x2 );
            }
            else if (cur_port_config_ptr->port_50g_en[4] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE4_CLK_RST_MUX, 0x1 );
                if (pam_or_nrz == CW_NRZ) {
                    hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE5_CLK_RST_MUX, 0x1 );
                }
            }
            else if (cur_port_config_ptr->port_50g_en[5] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE5_CLK_RST_MUX, 0x2 );
            }
            else if (cur_port_config_ptr->port_50g_en[6] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE6_CLK_RST_MUX, (lane_mask==0x20)?0x2:0x3 ); // handle H67_L5
                if (pam_or_nrz == CW_NRZ) {
                    hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE7_CLK_RST_MUX, 0x2 );
                }
            }
            else if (cur_port_config_ptr->port_50g_en[7] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE7_CLK_RST_MUX, 0x3 );
            }
            break;
        case (SPEED_100G):
            if (cur_port_config_ptr->port_100g_en[0] == PORT_ON) {
                // no need for slice_0 
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE1_CLK_RST_MUX, 0x0 );
                if (pam_or_nrz == CW_NRZ) {
                    hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE2_CLK_RST_MUX, 0x0 );
                    hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE3_CLK_RST_MUX, 0x0 );
                }
            }
            else if (cur_port_config_ptr->port_100g_en[1] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE2_CLK_RST_MUX, 0x2 );
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE3_CLK_RST_MUX, 0x1 );
            }
            else if (cur_port_config_ptr->port_100g_en[2] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE4_CLK_RST_MUX, 0x1 );
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE5_CLK_RST_MUX, 0x1 );
                if (pam_or_nrz == CW_NRZ) {
                    hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE6_CLK_RST_MUX, 0x1 );
                    hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE7_CLK_RST_MUX, 0x1 );
                }
            }
            else if (cur_port_config_ptr->port_100g_en[3] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE6_CLK_RST_MUX, 0x3 );
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE7_CLK_RST_MUX, 0x2 );
            }
            break;
        case (SPEED_200G):
            if (cur_port_config_ptr->port_200g_en[0] == PORT_ON) {
                // no need for slice_0 
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE1_CLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE2_CLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE3_CLK_RST_MUX, 0x0 );
            }
            else if (cur_port_config_ptr->port_200g_en[1] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE4_CLK_RST_MUX, 0x1 );
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE5_CLK_RST_MUX, 0x1 );
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE6_CLK_RST_MUX, 0x1 );
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE7_CLK_RST_MUX, 0x1 );
            }
            break;
        case (SPEED_400G):
            if (cur_port_config_ptr->port_400g_en == PORT_ON) {
                // no need for slice_0
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE1_CLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE2_CLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE3_CLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE4_CLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE5_CLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE6_CLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, FEC_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_SLICE7_CLK_RST_MUX, 0x0 );
            }
            break;
    }
    return RR_SUCCESS;
}

//Checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_pcs_tmt_cdr_clk_rst_mux_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,  cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr)
{
    switch (cur_mode_parameter_ptr->speed) {
        case (SPEED_25G):
            if (cur_port_config_ptr->port_25g_en[0] == PORT_ON) {
                // no need for slice_0 
            }
            else if (cur_port_config_ptr->port_25g_en[1] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, PCS_TMT_RCLK_RST_MUX_REG, MD_PCS_TMT_SLICE1_RCLK_RST_MUX, 0x1 );
            }
            else if (cur_port_config_ptr->port_25g_en[2] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, PCS_TMT_RCLK_RST_MUX_REG, MD_PCS_TMT_SLICE2_RCLK_RST_MUX, 0x2 );

            }
            else if (cur_port_config_ptr->port_25g_en[3] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, PCS_TMT_RCLK_RST_MUX_REG, MD_PCS_TMT_SLICE3_RCLK_RST_MUX, 0x3 );
            }
            else if (cur_port_config_ptr->port_25g_en[4] == PORT_ON) {
                // no need for slice_4 
            }
            else if (cur_port_config_ptr->port_25g_en[5] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, PCS_TMT_RCLK_RST_MUX_REG, MD_PCS_TMT_SLICE5_RCLK_RST_MUX, 0x1 );
            }
            else if (cur_port_config_ptr->port_25g_en[6] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, PCS_TMT_RCLK_RST_MUX_REG, MD_PCS_TMT_SLICE6_RCLK_RST_MUX, 0x2 );
            }
            else if (cur_port_config_ptr->port_25g_en[7] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, PCS_TMT_RCLK_RST_MUX_REG, MD_PCS_TMT_SLICE7_RCLK_RST_MUX, 0x3 );
            }
            break;
        case (SPEED_50G):
            if (egr_or_igr == EGR) { // EGR
                if (cur_port_config_ptr->port_50g_en[0] == PORT_ON) {
                    // no need for slice_0 
                    hsip_wr_field_(phy_info_ptr, PCS_TMT_RCLK_RST_MUX_REG, MD_PCS_TMT_SLICE1_RCLK_RST_MUX, 0x0 );
                }
                else if (cur_port_config_ptr->port_50g_en[2] == PORT_ON) {
                    if (cur_mode_parameter_ptr->host_llane_mask == 0x2) {   // H1_L23
                        hsip_wr_field_(phy_info_ptr, PCS_TMT_RCLK_RST_MUX_REG, MD_PCS_TMT_SLICE2_RCLK_RST_MUX, 0x1 );
                        hsip_wr_field_(phy_info_ptr, PCS_TMT_RCLK_RST_MUX_REG, MD_PCS_TMT_SLICE3_RCLK_RST_MUX, 0x1 );
                    } else {    // H2_L23
                        hsip_wr_field_(phy_info_ptr, PCS_TMT_RCLK_RST_MUX_REG, MD_PCS_TMT_SLICE2_RCLK_RST_MUX, 0x2 );
                        hsip_wr_field_(phy_info_ptr, PCS_TMT_RCLK_RST_MUX_REG, MD_PCS_TMT_SLICE3_RCLK_RST_MUX, 0x2 );
                    }
                }
                else if (cur_port_config_ptr->port_50g_en[4] == PORT_ON) {
                    // no need for slice_4 
                    hsip_wr_field_(phy_info_ptr, PCS_TMT_RCLK_RST_MUX_REG, MD_PCS_TMT_SLICE5_RCLK_RST_MUX, 0x0 );
                }
                else if (cur_port_config_ptr->port_50g_en[6] == PORT_ON) {
                    if (cur_mode_parameter_ptr->host_llane_mask == 0x20) {   // H5_L67
                        hsip_wr_field_(phy_info_ptr, PCS_TMT_RCLK_RST_MUX_REG, MD_PCS_TMT_SLICE2_RCLK_RST_MUX, 0x1 );
                        hsip_wr_field_(phy_info_ptr, PCS_TMT_RCLK_RST_MUX_REG, MD_PCS_TMT_SLICE3_RCLK_RST_MUX, 0x1 );
                    } else {    // H6_L67
                        hsip_wr_field_(phy_info_ptr, PCS_TMT_RCLK_RST_MUX_REG, MD_PCS_TMT_SLICE2_RCLK_RST_MUX, 0x2 );
                        hsip_wr_field_(phy_info_ptr, PCS_TMT_RCLK_RST_MUX_REG, MD_PCS_TMT_SLICE3_RCLK_RST_MUX, 0x2 );
                    }
                }
            } else {    // IGR
                if (cur_port_config_ptr->port_50g_en[0] == PORT_ON) {
                    // no need for slice_0 
                    hsip_wr_field_(phy_info_ptr, PCS_TMT_RCLK_RST_MUX_REG, MD_PCS_TMT_SLICE1_RCLK_RST_MUX, 0x0 );
                }
                else if (cur_port_config_ptr->port_50g_en[2] == PORT_ON) {
                    if (cur_mode_parameter_ptr->line_llane_mask == 0x2) {   // H1_L23
                        hsip_wr_field_(phy_info_ptr, PCS_TMT_RCLK_RST_MUX_REG, MD_PCS_TMT_SLICE2_RCLK_RST_MUX, 0x1 );
                        hsip_wr_field_(phy_info_ptr, PCS_TMT_RCLK_RST_MUX_REG, MD_PCS_TMT_SLICE3_RCLK_RST_MUX, 0x1 );
                    } else {    // H2_L23
                        hsip_wr_field_(phy_info_ptr, PCS_TMT_RCLK_RST_MUX_REG, MD_PCS_TMT_SLICE2_RCLK_RST_MUX, 0x2 );
                        hsip_wr_field_(phy_info_ptr, PCS_TMT_RCLK_RST_MUX_REG, MD_PCS_TMT_SLICE3_RCLK_RST_MUX, 0x2 );
                    }
                }
                else if (cur_port_config_ptr->port_50g_en[4] == PORT_ON) {
                    // no need for slice_4 
                    hsip_wr_field_(phy_info_ptr, PCS_TMT_RCLK_RST_MUX_REG, MD_PCS_TMT_SLICE5_RCLK_RST_MUX, 0x0 );
                }
                else if (cur_port_config_ptr->port_50g_en[6] == PORT_ON) {
                    if (cur_mode_parameter_ptr->line_llane_mask == 0x20) {   // H5_L67
                        hsip_wr_field_(phy_info_ptr, PCS_TMT_RCLK_RST_MUX_REG, MD_PCS_TMT_SLICE6_RCLK_RST_MUX, 0x1 );
                        hsip_wr_field_(phy_info_ptr, PCS_TMT_RCLK_RST_MUX_REG, MD_PCS_TMT_SLICE7_RCLK_RST_MUX, 0x1 );
                    } else {    // H6_L67
                        hsip_wr_field_(phy_info_ptr, PCS_TMT_RCLK_RST_MUX_REG, MD_PCS_TMT_SLICE6_RCLK_RST_MUX, 0x2 );
                        hsip_wr_field_(phy_info_ptr, PCS_TMT_RCLK_RST_MUX_REG, MD_PCS_TMT_SLICE7_RCLK_RST_MUX, 0x2 );
                    }
                }
            }
            break;
        case (SPEED_100G):
            if (cur_port_config_ptr->port_100g_en[0] == PORT_ON) {
                // no need for slice_0 
                hsip_wr_field_(phy_info_ptr, PCS_TMT_RCLK_RST_MUX_REG, MD_PCS_TMT_SLICE1_RCLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, PCS_TMT_RCLK_RST_MUX_REG, MD_PCS_TMT_SLICE2_RCLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, PCS_TMT_RCLK_RST_MUX_REG, MD_PCS_TMT_SLICE3_RCLK_RST_MUX, 0x0 );
            }
            else if (cur_port_config_ptr->port_100g_en[2] == PORT_ON) {
                // no need for slice_4 
                hsip_wr_field_(phy_info_ptr, PCS_TMT_RCLK_RST_MUX_REG, MD_PCS_TMT_SLICE5_RCLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, PCS_TMT_RCLK_RST_MUX_REG, MD_PCS_TMT_SLICE6_RCLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, PCS_TMT_RCLK_RST_MUX_REG, MD_PCS_TMT_SLICE7_RCLK_RST_MUX, 0x0 );
            }
            break;
        default:
            break;
    }
    return RR_SUCCESS;
}

//Checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_pcs_tmt_cmu_clk_rst_mux_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,  cw_port_config_t* cur_port_config_ptr)
{
    switch (cur_mode_parameter_ptr->speed) {
        case (SPEED_25G):
            if (cur_port_config_ptr->port_25g_en[0] == PORT_ON) {
                // no need for slice_0 
            }
            else if (cur_port_config_ptr->port_25g_en[1] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, PCS_TMT_TCLK_RST_MUX_REG, MD_PCS_TMT_SLICE1_TCLK_RST_MUX, 0x1 );
            }
            else if (cur_port_config_ptr->port_25g_en[2] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, PCS_TMT_TCLK_RST_MUX_REG, MD_PCS_TMT_SLICE2_TCLK_RST_MUX, 0x2 );

            }
            else if (cur_port_config_ptr->port_25g_en[3] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, PCS_TMT_TCLK_RST_MUX_REG, MD_PCS_TMT_SLICE3_TCLK_RST_MUX, 0x3 );
            }
            else if (cur_port_config_ptr->port_25g_en[4] == PORT_ON) {
                // no need for slice_4 
            }
            else if (cur_port_config_ptr->port_25g_en[5] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, PCS_TMT_TCLK_RST_MUX_REG, MD_PCS_TMT_SLICE5_TCLK_RST_MUX, 0x1 );
            }
            else if (cur_port_config_ptr->port_25g_en[6] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, PCS_TMT_TCLK_RST_MUX_REG, MD_PCS_TMT_SLICE6_TCLK_RST_MUX, 0x2 );
            }
            else if (cur_port_config_ptr->port_25g_en[7] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, PCS_TMT_TCLK_RST_MUX_REG, MD_PCS_TMT_SLICE7_TCLK_RST_MUX, 0x3 );
            }
            break;
        case (SPEED_50G):
            if (cur_port_config_ptr->port_50g_en[0] == PORT_ON) {
                // no need for slice_0 
                hsip_wr_field_(phy_info_ptr, PCS_TMT_TCLK_RST_MUX_REG, MD_PCS_TMT_SLICE1_TCLK_RST_MUX, 0x0 );
            }
            else if (cur_port_config_ptr->port_50g_en[2] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, PCS_TMT_TCLK_RST_MUX_REG, MD_PCS_TMT_SLICE2_TCLK_RST_MUX, 0x2 );
                hsip_wr_field_(phy_info_ptr, PCS_TMT_TCLK_RST_MUX_REG, MD_PCS_TMT_SLICE3_TCLK_RST_MUX, 0x2 );
            }
            else if (cur_port_config_ptr->port_50g_en[4] == PORT_ON) {
                // no need for slice_4 
                hsip_wr_field_(phy_info_ptr, PCS_TMT_TCLK_RST_MUX_REG, MD_PCS_TMT_SLICE5_TCLK_RST_MUX, 0x0 );
            }
            else if (cur_port_config_ptr->port_50g_en[6] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, PCS_TMT_TCLK_RST_MUX_REG, MD_PCS_TMT_SLICE2_TCLK_RST_MUX, 0x2 );
                hsip_wr_field_(phy_info_ptr, PCS_TMT_TCLK_RST_MUX_REG, MD_PCS_TMT_SLICE3_TCLK_RST_MUX, 0x2 );
            }
            break;
        case (SPEED_100G):
            if (cur_port_config_ptr->port_100g_en[0] == PORT_ON) {
                // no need for slice_0 
                hsip_wr_field_(phy_info_ptr, PCS_TMT_TCLK_RST_MUX_REG, MD_PCS_TMT_SLICE1_TCLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, PCS_TMT_TCLK_RST_MUX_REG, MD_PCS_TMT_SLICE2_TCLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, PCS_TMT_TCLK_RST_MUX_REG, MD_PCS_TMT_SLICE3_TCLK_RST_MUX, 0x0 );
            }
            else if (cur_port_config_ptr->port_100g_en[2] == PORT_ON) {
                // no need for slice_4 
                hsip_wr_field_(phy_info_ptr, PCS_TMT_TCLK_RST_MUX_REG, MD_PCS_TMT_SLICE5_TCLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, PCS_TMT_TCLK_RST_MUX_REG, MD_PCS_TMT_SLICE6_TCLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, PCS_TMT_TCLK_RST_MUX_REG, MD_PCS_TMT_SLICE7_TCLK_RST_MUX, 0x0 );
            }
            break;
        default:
            break;
    }
    return RR_SUCCESS;
}

// Checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_fec_rcv_clk_rst_mux_cfg_8p(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,  cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr)
{
    cfg_pam_or_nrz_t rcv_pam_or_nrz = (egr_or_igr == EGR) ? cur_mode_parameter_ptr->host_pam_or_nrz_type : cur_mode_parameter_ptr->line_pam_or_nrz_type;
    cfg_pam_or_nrz_t tmt_pam_or_nrz = (egr_or_igr == IGR) ? cur_mode_parameter_ptr->host_pam_or_nrz_type : cur_mode_parameter_ptr->line_pam_or_nrz_type;
    uint16_t rcv_lane_mask = (egr_or_igr == EGR) ? cur_mode_parameter_ptr->host_llane_mask : cur_mode_parameter_ptr->line_llane_mask;
    uint16_t tmt_lane_mask = (egr_or_igr == IGR) ? cur_mode_parameter_ptr->host_llane_mask : cur_mode_parameter_ptr->line_llane_mask;
    bool PAM50G_M2_port2 = ((rcv_pam_or_nrz == CW_PAM) && (tmt_pam_or_nrz == CW_NRZ) &&
                            (rcv_lane_mask == 0x2) && (tmt_lane_mask == 0xC)) ? true : false;
    bool PAM50G_M2_port6 = ((rcv_pam_or_nrz == CW_PAM) && (tmt_pam_or_nrz == CW_NRZ) &&
                            (rcv_lane_mask == 0x20) && (tmt_lane_mask == 0xC0)) ? true : false;

    switch (cur_mode_parameter_ptr->speed) {
        case (SPEED_25G):
            if (cur_port_config_ptr->port_25g_en[0] == PORT_ON) {
                // no need for slice_0 
            }
            else if (cur_port_config_ptr->port_25g_en[1] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE1_CLK_RST_MUX, 0x1 );
            }
            else if (cur_port_config_ptr->port_25g_en[2] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE2_CLK_RST_MUX, 0x2 );
            }
            else if (cur_port_config_ptr->port_25g_en[3] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE3_CLK_RST_MUX, 0x2 );
            }
            else if (cur_port_config_ptr->port_25g_en[4] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE4_CLK_RST_MUX, 0x1 );
            }
            else if (cur_port_config_ptr->port_25g_en[5] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE5_CLK_RST_MUX, 0x2 );
            }
            else if (cur_port_config_ptr->port_25g_en[6] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE6_CLK_RST_MUX, 0x3 );
            }
            else if (cur_port_config_ptr->port_25g_en[7] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE7_CLK_RST_MUX, 0x3 );
            }
            break;
        case (SPEED_50G):
            if (cur_port_config_ptr->port_50g_en[0] == PORT_ON) {
                // no need for slice_0 
                if (rcv_pam_or_nrz == CW_NRZ) {
                    hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE1_CLK_RST_MUX, 0x0 );
                }
            }
            else if (cur_port_config_ptr->port_50g_en[1] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE1_CLK_RST_MUX, 0x1 );
            }
            else if (cur_port_config_ptr->port_50g_en[2] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE2_CLK_RST_MUX, (PAM50G_M2_port2 ? 0x1 : 0x2) );
                if (rcv_pam_or_nrz == CW_NRZ) {
                    hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE3_CLK_RST_MUX, 0x1 );
                }
            }
            else if (cur_port_config_ptr->port_50g_en[3] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE3_CLK_RST_MUX, 0x2 );
            }
            else if (cur_port_config_ptr->port_50g_en[4] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE4_CLK_RST_MUX, 0x1 );
                if (rcv_pam_or_nrz == CW_NRZ) {
                    hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE5_CLK_RST_MUX, 0x1 );
                }
            }
            else if (cur_port_config_ptr->port_50g_en[5] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE5_CLK_RST_MUX, 0x2 );
            }
            else if (cur_port_config_ptr->port_50g_en[6] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE6_CLK_RST_MUX, (PAM50G_M2_port6 ? 0x2 : 0x3) );
                if (rcv_pam_or_nrz == CW_NRZ) {
                    hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE7_CLK_RST_MUX, 0x2 );
                }
            }
            else if (cur_port_config_ptr->port_50g_en[7] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE7_CLK_RST_MUX, 0x3 );
            }
            break;
        case (SPEED_100G):
            if (cur_port_config_ptr->port_100g_en[0] == PORT_ON) {
                // no need for slice_0 
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE1_CLK_RST_MUX, 0x0 );
                if (rcv_pam_or_nrz == CW_NRZ) {
                    hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE2_CLK_RST_MUX, 0x0 );
                    hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE3_CLK_RST_MUX, 0x0 );
                }
            }
            else if (cur_port_config_ptr->port_100g_en[1] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE2_CLK_RST_MUX, 0x2 );
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE3_CLK_RST_MUX, 0x1 );
            }
            else if (cur_port_config_ptr->port_100g_en[2] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE4_CLK_RST_MUX, 0x1 );
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE5_CLK_RST_MUX, 0x1 );
                if (rcv_pam_or_nrz == CW_NRZ) {
                    hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE6_CLK_RST_MUX, 0x1 );
                    hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE7_CLK_RST_MUX, 0x1 );
                }
            }
            else if (cur_port_config_ptr->port_100g_en[3] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE6_CLK_RST_MUX, 0x3 );
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE7_CLK_RST_MUX, 0x2 );
            }
            break;
        case (SPEED_200G):
            if (cur_port_config_ptr->port_200g_en[0] == PORT_ON) {
                // no need for slice_0 
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE1_CLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE2_CLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE3_CLK_RST_MUX, 0x0 );
            }
            else if (cur_port_config_ptr->port_200g_en[1] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE4_CLK_RST_MUX, 0x1 );
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE5_CLK_RST_MUX, 0x1 );
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE6_CLK_RST_MUX, 0x1 );
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE7_CLK_RST_MUX, 0x1 );
            }
            break;
        case (SPEED_400G):
            if (cur_port_config_ptr->port_400g_en == PORT_ON) {
                // no need for slice_0
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE1_CLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE2_CLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE3_CLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE4_CLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE5_CLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE6_CLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE7_CLK_RST_MUX, 0x0 );
            }
            break;
    } // end switch

    return RR_SUCCESS;
}


// Checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_fec_rcv_clk_rst_mux_cfg_4p(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,  cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr)
{
    cfg_pam_or_nrz_t rcv_pam_or_nrz = (egr_or_igr == EGR) ? cur_mode_parameter_ptr->host_pam_or_nrz_type : cur_mode_parameter_ptr->line_pam_or_nrz_type;
    cfg_pam_or_nrz_t tmt_pam_or_nrz = (egr_or_igr == IGR) ? cur_mode_parameter_ptr->host_pam_or_nrz_type : cur_mode_parameter_ptr->line_pam_or_nrz_type;
    uint16_t rcv_lane_mask = (egr_or_igr == EGR) ? cur_mode_parameter_ptr->host_llane_mask : cur_mode_parameter_ptr->line_llane_mask;
    uint16_t tmt_lane_mask = (egr_or_igr == IGR) ? cur_mode_parameter_ptr->host_llane_mask : cur_mode_parameter_ptr->line_llane_mask;
    bool PAM50G_M2_port2 = ((rcv_pam_or_nrz == CW_PAM) && (tmt_pam_or_nrz == CW_NRZ) &&
                            (rcv_lane_mask == 0x2) && (tmt_lane_mask == 0xC)) ? true : false;
    bool PAM50G_M2_port6 = ((rcv_pam_or_nrz == CW_PAM) && (tmt_pam_or_nrz == CW_NRZ) &&
                            (rcv_lane_mask == 0x20) && (tmt_lane_mask == 0xC0)) ? true : false;

    switch (cur_mode_parameter_ptr->speed) {
        case (SPEED_25G):
            if (cur_port_config_ptr->port_25g_en[0] == PORT_ON) {
                // no need for slice_0 
            }
            else if (cur_port_config_ptr->port_25g_en[1] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE1_CLK_RST_MUX, 0x1 );
            }
            else if (cur_port_config_ptr->port_25g_en[2] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE2_CLK_RST_MUX, 0x2 );
            }
            else if (cur_port_config_ptr->port_25g_en[3] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE3_CLK_RST_MUX, 0x2 );
            }
            else if (cur_port_config_ptr->port_25g_en[4] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE4_CLK_RST_MUX, 0x1 );
            }
            else if (cur_port_config_ptr->port_25g_en[5] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE5_CLK_RST_MUX, 0x2 );
            }
            else if (cur_port_config_ptr->port_25g_en[6] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE6_CLK_RST_MUX, 0x3 );
            }
            else if (cur_port_config_ptr->port_25g_en[7] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE7_CLK_RST_MUX, 0x3 );
            }
            break;
        case (SPEED_50G):
            if (cur_port_config_ptr->port_50g_en[0] == PORT_ON) {
                // no need for slice_0 
                if (rcv_pam_or_nrz == CW_NRZ) {
                    hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE1_CLK_RST_MUX, 0x0 );
                }
            }
            else if (cur_port_config_ptr->port_50g_en[1] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE1_CLK_RST_MUX, 0x1 );
            }
            else if (cur_port_config_ptr->port_50g_en[2] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE2_CLK_RST_MUX, (PAM50G_M2_port2 ? 0x1 : 0x2) );
                if (rcv_pam_or_nrz == CW_NRZ) {
                    hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE3_CLK_RST_MUX, 0x1 );
                }
            }
            else if (cur_port_config_ptr->port_50g_en[3] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE3_CLK_RST_MUX, 0x2 );
            }
            else if (cur_port_config_ptr->port_50g_en[4] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE4_CLK_RST_MUX, 0x1 );
                if (rcv_pam_or_nrz == CW_NRZ) {
                    hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE5_CLK_RST_MUX, 0x1 );
                }
            }
            else if (cur_port_config_ptr->port_50g_en[5] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE5_CLK_RST_MUX, 0x2 );
            }
            else if (cur_port_config_ptr->port_50g_en[6] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE6_CLK_RST_MUX, (PAM50G_M2_port6 ? 0x2 : 0x3) );
                if (rcv_pam_or_nrz == CW_NRZ) {
                    hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE7_CLK_RST_MUX, 0x2 );
                }
            }
            else if (cur_port_config_ptr->port_50g_en[7] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE7_CLK_RST_MUX, 0x3 );
            }
            break;
        case (SPEED_100G):
            if (cur_port_config_ptr->port_100g_en[0] == PORT_ON) {
                // no need for slice_0 
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE1_CLK_RST_MUX, 0x0 );
                if (rcv_pam_or_nrz == CW_NRZ || rcv_pam_or_nrz == CW_PAM) {
                    hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE2_CLK_RST_MUX, 0x0 );
                    hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE3_CLK_RST_MUX, 0x0 );
                }
            }
            else if (cur_port_config_ptr->port_100g_en[1] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE2_CLK_RST_MUX, 0x2 );
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE3_CLK_RST_MUX, 0x1 );
            }
            else if (cur_port_config_ptr->port_100g_en[2] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE4_CLK_RST_MUX, 0x1 );
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE5_CLK_RST_MUX, 0x1 );
                if (rcv_pam_or_nrz == CW_NRZ || rcv_pam_or_nrz == CW_PAM) {
                    hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE6_CLK_RST_MUX, 0x1 );
                    hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE7_CLK_RST_MUX, 0x1 );
                }
            }
            else if (cur_port_config_ptr->port_100g_en[3] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE6_CLK_RST_MUX, 0x3 );
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE7_CLK_RST_MUX, 0x2 );
            }
            break;
        case (SPEED_200G):
            if (cur_port_config_ptr->port_200g_en[0] == PORT_ON) {
                // no need for slice_0 
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE1_CLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE2_CLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE3_CLK_RST_MUX, 0x0 );
                if(rcv_pam_or_nrz == CW_PAM) {
                    hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE4_CLK_RST_MUX, 0x0 );
                    hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE5_CLK_RST_MUX, 0x0 );
                    hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE6_CLK_RST_MUX, 0x0 );
                    hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE7_CLK_RST_MUX, 0x0 );
                }                
            }
            else if (cur_port_config_ptr->port_200g_en[1] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE4_CLK_RST_MUX, 0x1 );
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE5_CLK_RST_MUX, 0x1 );
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE6_CLK_RST_MUX, 0x1 );
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE7_CLK_RST_MUX, 0x1 );
            }
            break;
        case (SPEED_400G):
            if (cur_port_config_ptr->port_400g_en == PORT_ON) {
                // no need for slice_0
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE1_CLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE2_CLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE3_CLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE4_CLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE5_CLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE6_CLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, FEC_RCV_CLK_RST_MUX_REG, MD_FEC_RCV_SLICE7_CLK_RST_MUX, 0x0 );
            }
            break;
    } // end switch

    return RR_SUCCESS;
}

return_result_t chal_cw_rtmr_fec_rcv_clk_rst_mux_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,  cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr)
{
    if (ml_cw_rtmr_is_A1_4p_port(cur_mode_parameter_ptr)) {        
        return chal_cw_rtmr_fec_rcv_clk_rst_mux_cfg_4p(phy_info_ptr, cur_mode_parameter_ptr, cur_port_config_ptr, egr_or_igr);
    } else {
        return chal_cw_rtmr_fec_rcv_clk_rst_mux_cfg_8p(phy_info_ptr, cur_mode_parameter_ptr, cur_port_config_ptr, egr_or_igr);
    }
}

//Checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_pcs_rcv_clk_rst_mux_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,  cw_port_config_t* cur_port_config_ptr)
{
    switch (cur_mode_parameter_ptr->speed) {
        case (SPEED_25G):
            if (cur_port_config_ptr->port_25g_en[0] == PORT_ON) {
                // no need for slice_0 
            }
            else if (cur_port_config_ptr->port_25g_en[1] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, PCS_RCV_CLK_RST_MUX_REG, MD_PCS_RCV_SLICE1_CLK_RST_MUX, 0x1 );
            }
            else if (cur_port_config_ptr->port_25g_en[2] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, PCS_RCV_CLK_RST_MUX_REG, MD_PCS_RCV_SLICE2_CLK_RST_MUX, 0x1 );
            }
            else if (cur_port_config_ptr->port_25g_en[3] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, PCS_RCV_CLK_RST_MUX_REG, MD_PCS_RCV_SLICE3_CLK_RST_MUX, 0x2 );
            }
            else if (cur_port_config_ptr->port_25g_en[4] == PORT_ON) {
                // no need for slice_04
            }
            else if (cur_port_config_ptr->port_25g_en[5] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, PCS_RCV_CLK_RST_MUX_REG, MD_PCS_RCV_SLICE5_CLK_RST_MUX, 0x1 );
            }
            else if (cur_port_config_ptr->port_25g_en[6] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, PCS_RCV_CLK_RST_MUX_REG, MD_PCS_RCV_SLICE6_CLK_RST_MUX, 0x1 );
            }
            else if (cur_port_config_ptr->port_25g_en[7] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, PCS_RCV_CLK_RST_MUX_REG, MD_PCS_RCV_SLICE7_CLK_RST_MUX, 0x2 );
            }
            break;
        case (SPEED_50G):
            if (cur_port_config_ptr->port_50g_en[0] == PORT_ON) {
                // no need for slice_0 
                hsip_wr_field_(phy_info_ptr, PCS_RCV_CLK_RST_MUX_REG, MD_PCS_RCV_SLICE1_CLK_RST_MUX, 0x0 );
            }
            else if (cur_port_config_ptr->port_50g_en[2] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, PCS_RCV_CLK_RST_MUX_REG, MD_PCS_RCV_SLICE2_CLK_RST_MUX, 0x1 );
                hsip_wr_field_(phy_info_ptr, PCS_RCV_CLK_RST_MUX_REG, MD_PCS_RCV_SLICE3_CLK_RST_MUX, 0x1 );
            }
            else if (cur_port_config_ptr->port_50g_en[4] == PORT_ON) {
                // no need for slice_4
                hsip_wr_field_(phy_info_ptr, PCS_RCV_CLK_RST_MUX_REG, MD_PCS_RCV_SLICE5_CLK_RST_MUX, 0x0 );
            }
            else if (cur_port_config_ptr->port_50g_en[6] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, PCS_RCV_CLK_RST_MUX_REG, MD_PCS_RCV_SLICE6_CLK_RST_MUX, 0x1 );
                hsip_wr_field_(phy_info_ptr, PCS_RCV_CLK_RST_MUX_REG, MD_PCS_RCV_SLICE7_CLK_RST_MUX, 0x1 );
            }
            break;
        case (SPEED_100G):
            if (cur_port_config_ptr->port_100g_en[0] == PORT_ON) {
                // no need for slice_0 
                hsip_wr_field_(phy_info_ptr, PCS_RCV_CLK_RST_MUX_REG, MD_PCS_RCV_SLICE1_CLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, PCS_RCV_CLK_RST_MUX_REG, MD_PCS_RCV_SLICE2_CLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, PCS_RCV_CLK_RST_MUX_REG, MD_PCS_RCV_SLICE3_CLK_RST_MUX, 0x0 );
            }
            else if (cur_port_config_ptr->port_100g_en[2] == PORT_ON) {
                // no need for slice_4
                hsip_wr_field_(phy_info_ptr, PCS_RCV_CLK_RST_MUX_REG, MD_PCS_RCV_SLICE5_CLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, PCS_RCV_CLK_RST_MUX_REG, MD_PCS_RCV_SLICE6_CLK_RST_MUX, 0x0 );
                hsip_wr_field_(phy_info_ptr, PCS_RCV_CLK_RST_MUX_REG, MD_PCS_RCV_SLICE7_CLK_RST_MUX, 0x0 );
            }
            break;
        default:
            break;
    } // end switch

    return RR_SUCCESS;
}


// Checked RETIMER_IGR or RETIMER_EGR
//[HSIP] Add else
return_result_t chal_cw_rtmr_tmt_pfifo_clk_mux_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t tmt_lane_mask = (egr_or_igr == EGR) ? cur_mode_parameter_ptr->line_llane_mask : cur_mode_parameter_ptr->host_llane_mask;
    uint16_t rcv_lane_mask = (egr_or_igr == IGR) ? cur_mode_parameter_ptr->line_llane_mask : cur_mode_parameter_ptr->host_llane_mask;

    if (cur_mode_parameter_ptr->speed == SPEED_50G) {
        if (cur_port_config_ptr->port_50g_en[2] == PORT_ON) {
            if ((rcv_lane_mask == 0xC) && (tmt_lane_mask == 0x2))     // H23_L1
                hsip_wr_field_(phy_info_ptr, PFIFO_CLK_RST_MUX_REG, MD_TMT_PFIFO_SLICE1_WCLK_RST_MUX, 0x1 );
            else    // H1_L1 
                hsip_wr_field_(phy_info_ptr, PFIFO_CLK_RST_MUX_REG, MD_TMT_PFIFO_SLICE1_WCLK_RST_MUX, 0x0 );
        }
        else if (cur_port_config_ptr->port_50g_en[6] == PORT_ON) {
            if ((rcv_lane_mask == 0xC0) && (tmt_lane_mask == 0x20))   // H67_L5
                hsip_wr_field_(phy_info_ptr, PFIFO_CLK_RST_MUX_REG, MD_TMT_PFIFO_SLICE5_WCLK_RST_MUX, 0x1 );
            else    // H5_L5
                hsip_wr_field_(phy_info_ptr, PFIFO_CLK_RST_MUX_REG, MD_TMT_PFIFO_SLICE5_WCLK_RST_MUX, 0x0 );
        }
    }
    else {
        hsip_wr_field_(phy_info_ptr, PFIFO_CLK_RST_MUX_REG, MD_TMT_PFIFO_SLICE1_WCLK_RST_MUX, 0x0 );
        hsip_wr_field_(phy_info_ptr, PFIFO_CLK_RST_MUX_REG, MD_TMT_PFIFO_SLICE5_WCLK_RST_MUX, 0x0 );
    }

    return RR_SUCCESS;
}

//Checked RETIMER_IGR or RETIMER_EGR
//[HSIP] Add else
return_result_t chal_cw_rtmr_rcv_pfifo_clk_mux_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t rcv_lane_mask = (egr_or_igr == IGR) ? cur_mode_parameter_ptr->line_llane_mask : cur_mode_parameter_ptr->host_llane_mask;
    uint16_t tmt_lane_mask = (egr_or_igr == EGR) ? cur_mode_parameter_ptr->line_llane_mask : cur_mode_parameter_ptr->host_llane_mask;

    if (cur_mode_parameter_ptr->speed == SPEED_50G) {
        if (cur_port_config_ptr->port_50g_en[2] == PORT_ON) {
            if ((rcv_lane_mask == 0x2) && (tmt_lane_mask == 0xC))     // H1_L23
                hsip_wr_field_(phy_info_ptr, PFIFO_CLK_RST_MUX_REG, MD_RCV_PFIFO_SLICE2_WCLK_RST_MUX, 0x1 );
            else    // H2_L23 / H2_L2
                hsip_wr_field_(phy_info_ptr, PFIFO_CLK_RST_MUX_REG, MD_RCV_PFIFO_SLICE2_WCLK_RST_MUX, 0x0 );
        }
        else if (cur_port_config_ptr->port_50g_en[6] == PORT_ON) {
            if ((rcv_lane_mask == 0x20) && (tmt_lane_mask == 0xC0))   // H5_L67
                hsip_wr_field_(phy_info_ptr, PFIFO_CLK_RST_MUX_REG, MD_RCV_PFIFO_SLICE6_WCLK_RST_MUX, 0x1 );
            else    // H6_L67 / H6_L6
                hsip_wr_field_(phy_info_ptr, PFIFO_CLK_RST_MUX_REG, MD_RCV_PFIFO_SLICE6_WCLK_RST_MUX, 0x0 );
        }
    }
    else {
        hsip_wr_field_(phy_info_ptr, PFIFO_CLK_RST_MUX_REG, MD_RCV_PFIFO_SLICE2_WCLK_RST_MUX, 0x0 );
        hsip_wr_field_(phy_info_ptr, PFIFO_CLK_RST_MUX_REG, MD_RCV_PFIFO_SLICE6_WCLK_RST_MUX, 0x0 );
    }
        
    return RR_SUCCESS;
}

// Checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_fec_rcv_tmt_clk_mux_cfg(phy_info_t* phy_info_ptr,  cw_mode_parameter_t* cur_mode_parameter_ptr, uint8_t enable, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t reg_val = 0, lane_mask=0;
    cfg_pam_or_nrz_t rcv_pam_or_nrz = (egr_or_igr == EGR) ? cur_mode_parameter_ptr->host_pam_or_nrz_type : cur_mode_parameter_ptr->line_pam_or_nrz_type;
    cfg_pam_or_nrz_t tmt_pam_or_nrz = (egr_or_igr == IGR) ? cur_mode_parameter_ptr->host_pam_or_nrz_type : cur_mode_parameter_ptr->line_pam_or_nrz_type;
    uint16_t tmt_lane_mask = (egr_or_igr == EGR) ? cur_mode_parameter_ptr->line_llane_mask : cur_mode_parameter_ptr->host_llane_mask;
    uint16_t rcv_lane_mask = (egr_or_igr == IGR) ? cur_mode_parameter_ptr->line_llane_mask : cur_mode_parameter_ptr->host_llane_mask;
    bool PAM50G_M2_port2 = ((tmt_pam_or_nrz == CW_PAM) && (rcv_pam_or_nrz == CW_NRZ) &&
                            (tmt_lane_mask == 0x2)     && (rcv_lane_mask == 0xC)) ? true : false;
    bool PAM50G_M2_port6 = ((tmt_pam_or_nrz == CW_PAM) && (rcv_pam_or_nrz == CW_NRZ) &&
                            (tmt_lane_mask == 0x20)    && (rcv_lane_mask == 0xC0)) ? true : false;

    lane_mask = (PAM50G_M2_port2 || PAM50G_M2_port6) ? (tmt_lane_mask<<1) : tmt_lane_mask;
    reg_val = hsip_rd_field_(phy_info_ptr, FEC_RCV_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_CLK40_RST_MUX_LN);

    if (enable) {
        reg_val |= (lane_mask);
// HSIP: We should only use tx clock for clk40, not clk66. Since xdecoder  and pcs_sync are all using rx clk66. 
//     Only in PCS pattern gen modes we can choose to use tclk66 for xencoder
//        reg_val |= (lane_mask)<<8;
    } else {
        reg_val &= ~(lane_mask);
//        reg_val &= ~(lane_mask<<8);
    }

    hsip_wr_field_(phy_info_ptr, FEC_RCV_TMT_CLK_RST_MUX_REG, MD_FEC_TMT_CLK40_RST_MUX_LN, reg_val);

    return RR_SUCCESS;
}


// Checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_pcs_rcv_tmt_clk_mux_cfg(phy_info_t* phy_info_ptr,  cw_mode_parameter_t* cur_mode_parameter_ptr, uint8_t enable, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t reg_val = 0;
    uint16_t lane_mask = (egr_or_igr == EGR) ? cur_mode_parameter_ptr->line_llane_mask : cur_mode_parameter_ptr->host_llane_mask;

    reg_val = (uint16_t)hsip_rd_reg_(phy_info_ptr, PCS_RCV_TMT_CLK_RST_MUX_REG);

    if (enable) {
        reg_val |= (lane_mask<<8);
    } else {
        reg_val &= ~(lane_mask<<8);
    }

    hsip_wr_reg_(phy_info_ptr, PCS_RCV_TMT_CLK_RST_MUX_REG, reg_val);

    return RR_SUCCESS;
}

//Checked
uint8_t util_get_lowest_index_from_mask (uint16_t lane_mask) {
    uint8_t idx = 16;
    uint8_t lane = 0;

    if (lane_mask) {
        for (lane=0; lane<16; lane++) {
            if ((1<<lane) & lane_mask) {
                idx = lane;
                break;
            }
        }
    }

    return idx;
}

//Checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_cmu2cdr_lane_map_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr)
{
    uint8_t idx = 16;
    uint8_t lane = 0;
    uint16_t wmask, wdata;
    uint16_t src_lane_mask = (egr_or_igr == EGR) ? cur_mode_parameter_ptr->host_llane_mask :  cur_mode_parameter_ptr->line_llane_mask;
    uint16_t dst_lane_mask = (egr_or_igr == IGR) ? cur_mode_parameter_ptr->host_llane_mask :  cur_mode_parameter_ptr->line_llane_mask;

    wmask = wdata = 0;
    for (lane=0; lane<8; lane++) {
        if ((1<<lane) & src_lane_mask) {
            if ((idx = util_get_lowest_index_from_mask(dst_lane_mask)) < 8) {
                wmask |= 0x3 << (lane<<1);
                wdata |= ((uint16_t)((idx<4) ? idx : (idx-4))) << (lane<<1) ;
            }
        }
    }

    if (wmask) {
        hsip_wr_fields(phy_info_ptr, CMU2CDR_LANE_MAP_REG, wmask, wdata);
    }

    return RR_SUCCESS;
}

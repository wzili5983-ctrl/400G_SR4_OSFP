/**
 *
 * @file     chal_cw_rtmr_datapath_cfg.c 
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
#include <stdio.h>
#include "regs_common.h"
#include "chip_mode_def.h"
#include "cw_def.h"
#include "ml_cw_rtmr_modes.h"
#include "chal_cw_rtmr_datapath_cfg.h"
#include "common_util.h"

return_result_t cha_cw_rtmr_din_off (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr, enabled_t enable)
{
    uint16_t reg_val = 0;
    uint16_t lane_mask = 0;

    lane_mask  = (egr_or_igr == EGR) ? cur_mode_parameter_ptr->host_llane_mask : cur_mode_parameter_ptr->line_llane_mask;
    reg_val = hsip_rd_field_(phy_info_ptr, RCV_IFC_CTRL_4_RDB, MD_RTM_RCV_DIN_OFF);
    if (enable == IS_ENABLED)
       reg_val &= lane_mask ^ 0xFF;
    else
       reg_val |= lane_mask & 0xFF;
    hsip_wr_field_(phy_info_ptr, RCV_IFC_CTRL_4_RDB, MD_RTM_RCV_DIN_OFF, reg_val);

    return RR_SUCCESS;
}

return_result_t chal_cw_rtmr_rcv_pfifo_gbox_corr_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable)
{
    (void)cur_mode_parameter_ptr; 
   hsip_wr_reg_(phy_info_ptr, FEC_FIFO_CORR_MODE_REG, 0xAA); 
   if (enable == IS_ENABLED) {
     hsip_wr_reg_(phy_info_ptr, FEC_FIFO_LAT_CTL_REG, 0xF);
   }
   else {
     hsip_wr_reg_(phy_info_ptr, FEC_FIFO_LAT_CTL_REG, 0x0);
   }
   return RR_SUCCESS;
}

return_result_t chal_cw_rtmr_tmt_pfifo_gbox_corr_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable)
{
    (void)cur_mode_parameter_ptr;
   hsip_wr_reg_(phy_info_ptr, TMT_FEC_FIFO_CORR_MODE_REG, 0xAA); 
   if (enable == IS_ENABLED) {
     hsip_wr_reg_(phy_info_ptr, TMT_FEC_FIFO_LAT_CTL_REG, 0xF);
   }
   else {
     hsip_wr_reg_(phy_info_ptr, TMT_FEC_FIFO_LAT_CTL_REG, 0x0);
   }
   return RR_SUCCESS;
}

// Program CW_PAM/CW_NRZ mode for rcv_pfifo_gbox . Just used in Blackhawk modes
return_result_t chal_cw_rtmr_rcv_pfifo_gbox_mode_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t lane_mask = 0;
    cfg_pam_or_nrz_t pam_or_nrz;

    lane_mask  = (egr_or_igr == EGR) ? cur_mode_parameter_ptr->host_llane_mask : cur_mode_parameter_ptr->line_llane_mask;
    pam_or_nrz = (egr_or_igr == EGR) ? cur_mode_parameter_ptr->host_pam_or_nrz_type : cur_mode_parameter_ptr->line_pam_or_nrz_type;
    if (pam_or_nrz == CW_PAM) {  // write 0
        //reg_val &= (lane_mask ^ 0xFF);
        if ( (lane_mask & 0x1 ) == 0x1) {
           hsip_wr_field_(phy_info_ptr, RCV_PAM_NRZ_CTRL_REG, RCV_NRZ_PAM_0, 0x0); 
        }
        if ( (lane_mask & 0x2 ) == 0x2) {
           hsip_wr_field_(phy_info_ptr, RCV_PAM_NRZ_CTRL_REG, RCV_NRZ_PAM_1, 0x0); 
        }
        if ( (lane_mask & 0x4 ) == 0x4) {
           hsip_wr_field_(phy_info_ptr, RCV_PAM_NRZ_CTRL_REG, RCV_NRZ_PAM_2, 0x0); 
        }
        if ( (lane_mask & 0x8 ) == 0x8) {
           hsip_wr_field_(phy_info_ptr, RCV_PAM_NRZ_CTRL_REG, RCV_NRZ_PAM_3, 0x0); 
        }
        if ( (lane_mask & 0x10) == 0x10) {
           hsip_wr_field_(phy_info_ptr, RCV_PAM_NRZ_CTRL_REG, RCV_NRZ_PAM_4, 0x0); 
        }
        if ( (lane_mask & 0x20) == 0x20) {
            hsip_wr_field_(phy_info_ptr, RCV_PAM_NRZ_CTRL_REG, RCV_NRZ_PAM_5, 0x0); 
        }
        if ( (lane_mask & 0x40) == 0x40) {
            hsip_wr_field_(phy_info_ptr, RCV_PAM_NRZ_CTRL_REG, RCV_NRZ_PAM_6, 0x0); 
        }
        if ((lane_mask & 0x80 ) == 0x80) {
            hsip_wr_field_(phy_info_ptr, RCV_PAM_NRZ_CTRL_REG, RCV_NRZ_PAM_7, 0x0); 
        }
    } else if (pam_or_nrz == CW_NRZ) { // NRZ, write 1
        //reg_val |= (lane_mask & 0xFF);
        if ( (lane_mask & 0x1 ) == 0x1) {
           hsip_wr_field_(phy_info_ptr, RCV_PAM_NRZ_CTRL_REG, RCV_NRZ_PAM_0, 0x1); 
        } 
        if ( (lane_mask & 0x2 ) == 0x2) {
           hsip_wr_field_(phy_info_ptr, RCV_PAM_NRZ_CTRL_REG, RCV_NRZ_PAM_1, 0x1); 
        }
        if ( (lane_mask & 0x4 ) == 0x4) {
           hsip_wr_field_(phy_info_ptr, RCV_PAM_NRZ_CTRL_REG, RCV_NRZ_PAM_2, 0x1); 
        }
        if ( (lane_mask & 0x8 ) == 0x8) {
           hsip_wr_field_(phy_info_ptr, RCV_PAM_NRZ_CTRL_REG, RCV_NRZ_PAM_3, 0x1); 
        }
        if ( (lane_mask & 0x10) == 0x10) {
           hsip_wr_field_(phy_info_ptr, RCV_PAM_NRZ_CTRL_REG, RCV_NRZ_PAM_4, 0x1); 
        }
        if ( (lane_mask & 0x20) == 0x20) {
           hsip_wr_field_(phy_info_ptr, RCV_PAM_NRZ_CTRL_REG, RCV_NRZ_PAM_5, 0x1); 
        }
        if ( (lane_mask & 0x40) == 0x40) {
           hsip_wr_field_(phy_info_ptr, RCV_PAM_NRZ_CTRL_REG, RCV_NRZ_PAM_6, 0x1); 
        }
        if ( (lane_mask & 0x80) == 0x80) {
           hsip_wr_field_(phy_info_ptr, RCV_PAM_NRZ_CTRL_REG, RCV_NRZ_PAM_7, 0x1); 
        }
    } else if (pam_or_nrz == CW_PAM100) {
        if ( (lane_mask & 0x3 ) == 0x3) {
           hsip_wr_field_(phy_info_ptr, RCV_PAM_NRZ_CTRL_REG, RCV_NRZ_PAM_0, 0x2); 
        } 
        if ( (lane_mask & 0xC ) == 0xC) {
           hsip_wr_field_(phy_info_ptr, RCV_PAM_NRZ_CTRL_REG, RCV_NRZ_PAM_1, 0x2); 
        }
        if ( (lane_mask & 0x30) == 0x30) {
           hsip_wr_field_(phy_info_ptr, RCV_PAM_NRZ_CTRL_REG, RCV_NRZ_PAM_2, 0x2); 
        }
        if ( (lane_mask & 0xC0) == 0xC0) {
           hsip_wr_field_(phy_info_ptr, RCV_PAM_NRZ_CTRL_REG, RCV_NRZ_PAM_3, 0x2); 
        }
    }
    //hsip_wr_fields(phy_info_ptr, RCV_PAM_NRZ_CTRL_REG, lane_mask, reg_val); 
    return RR_SUCCESS;
}

//wr mux 
return_result_t chal_cw_rtmr_rcv_pfifo_gbox_wrmux_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr)
{
    if ((egr_or_igr == IGR && cur_mode_parameter_ptr->line_pam_or_nrz_type == CW_PAM && cur_mode_parameter_ptr->host_pam_or_nrz_type == CW_NRZ) ||
        (egr_or_igr == EGR && cur_mode_parameter_ptr->host_pam_or_nrz_type == CW_PAM && cur_mode_parameter_ptr->line_pam_or_nrz_type == CW_NRZ)) {
        // Set WR_2_1_MUX and WR_6_5_MUX for PCS/KR to KP4 and KP4 to PCS/KR M2 modes: KP4 side lane 1 and 3
        if (cur_mode_parameter_ptr->speed == SPEED_50G) {
            // no need to set the mux for 50G PCS/KR to KP4
            /*
            if (cur_port_config_ptr->port_50g_en[2])
                hsip_wr_field_(phy_info_ptr,FEC_FIFO_CLK_RST_SEL_REG, WR_2_1_MUX, 1);
            if (cur_port_config_ptr->port_50g_en[6])
                hsip_wr_field_(phy_info_ptr,FEC_FIFO_CLK_RST_SEL_REG, WR_6_5_MUX, 1);
                */
        }
        else if (cur_mode_parameter_ptr->speed == SPEED_100G) {
            if (cur_port_config_ptr->port_100g_en[0])
                hsip_wr_field_(phy_info_ptr,FEC_FIFO_CLK_RST_SEL_REG, WR_2_1_MUX, 1);
            if (cur_port_config_ptr->port_100g_en[2])
                hsip_wr_field_(phy_info_ptr,FEC_FIFO_CLK_RST_SEL_REG, WR_6_5_MUX, 1);
        }
        else if (cur_mode_parameter_ptr->speed == SPEED_200G) {
            hsip_wr_field_(phy_info_ptr,FEC_FIFO_CLK_RST_SEL_REG, WR_2_1_MUX, 1);
            hsip_wr_field_(phy_info_ptr,FEC_FIFO_CLK_RST_SEL_REG, WR_6_5_MUX, 1);
        }
    }
    else {
        hsip_wr_field_(phy_info_ptr,FEC_FIFO_CLK_RST_SEL_REG, WR_2_1_MUX, 0);
        hsip_wr_field_(phy_info_ptr,FEC_FIFO_CLK_RST_SEL_REG, WR_6_5_MUX, 0);            
    }
    return RR_SUCCESS;
}

// ob_sel . 0 = use seperate clk, 1 = use bundle clock
return_result_t chal_cw_rtmr_rcv_pfifo_gbox_ob_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t reg_val = 0;
    uint16_t lane_mask = 0;
    cfg_pam_or_nrz_t pam_or_nrz;

    lane_mask  = (egr_or_igr == EGR) ? cur_mode_parameter_ptr->host_llane_mask : cur_mode_parameter_ptr->line_llane_mask;
    pam_or_nrz = (egr_or_igr == EGR) ? cur_mode_parameter_ptr->host_pam_or_nrz_type : cur_mode_parameter_ptr->line_pam_or_nrz_type;
    reg_val = hsip_rd_field_(phy_info_ptr,FEC_FIFO_CLK_RST_SEL_REG ,FEC_FIFO_OB_SELECT );
    if (pam_or_nrz == CW_PAM || pam_or_nrz == CW_NRZ) {
        if( (lane_mask & 0x1 ) == 0x1 || (lane_mask & 0x2 ) == 0x2) {
            reg_val &= 0x1 ^ 0xF;
        }
        if( (lane_mask & 0x4 ) == 0x4 || (lane_mask & 0x8 ) == 0x8) {
            reg_val &= 0x2 ^ 0xF;
        }
        if( (lane_mask & 0x10) == 0x10 || (lane_mask & 0x20 ) == 0x20) {
            reg_val &= 0x4 ^ 0xF;
        }
        if( (lane_mask & 0x40) == 0x40 || (lane_mask & 0x80 ) == 0x80) {
            reg_val &= 0x8 ^ 0xF;
        }
        hsip_wr_field_(phy_info_ptr,FEC_FIFO_CLK_RST_SEL_REG, FEC_FIFO_OB_SELECT, reg_val);
    } else if (pam_or_nrz == CW_PAM100) {
        if ( (lane_mask & 0x3) == 0x3) {
            reg_val |= 0x1;
        }
        if ( (lane_mask & 0xC) == 0xC) {
            reg_val |= 0x2;
        }
        if ( (lane_mask & 0x30) == 0x30) {
            reg_val |= 0x4;
        }
        if ( (lane_mask & 0xC0) == 0xC0) {
            reg_val |= 0x8;
        }
        hsip_wr_field_(phy_info_ptr,FEC_FIFO_CLK_RST_SEL_REG, FEC_FIFO_OB_SELECT, reg_val);
    }
    return RR_SUCCESS;
}

// ib_sel . 0 = use seperate clk, 1 = use bundle clock
return_result_t chal_cw_rtmr_rcv_pfifo_gbox_ib_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t reg_val = 0;
    uint16_t lane_mask = 0;
    cfg_pam_or_nrz_t pam_or_nrz;

    lane_mask  = (egr_or_igr == EGR) ? cur_mode_parameter_ptr->host_llane_mask : cur_mode_parameter_ptr->line_llane_mask;
    pam_or_nrz = (egr_or_igr == EGR) ? cur_mode_parameter_ptr->host_pam_or_nrz_type : cur_mode_parameter_ptr->line_pam_or_nrz_type;
    reg_val = hsip_rd_field_(phy_info_ptr,FEC_FIFO_CLK_RST_SEL_REG ,FEC_FIFO_IB_SELECT );
    if (pam_or_nrz == CW_PAM || pam_or_nrz == CW_NRZ) {
        // clear bit
        if( (lane_mask & 0x1 ) == 0x1 || (lane_mask & 0x2 ) == 0x2) {
            reg_val &= 0x1 ^ 0xF;
        }
        if( (lane_mask & 0x4 ) == 0x4 || (lane_mask & 0x8 ) == 0x8) {
            reg_val &= 0x2 ^ 0xF;
        }
        if( (lane_mask & 0x10 ) == 0x10 || (lane_mask & 0x20 ) == 0x20) {
            reg_val &= 0x4 ^ 0xF;
        }
        if( (lane_mask & 0x40 ) == 0x40 || (lane_mask & 0x80 ) == 0x80) {
            reg_val &= 0x8 ^ 0xF;
        }
        hsip_wr_field_(phy_info_ptr,FEC_FIFO_CLK_RST_SEL_REG, FEC_FIFO_IB_SELECT, reg_val);
    } else if (pam_or_nrz == CW_PAM100) {
        // set bit
        if ( (lane_mask & 0x3 ) == 0x3) {
            reg_val |= 0x1;
        }
        if ( (lane_mask & 0xC ) == 0xC) {
            reg_val |= 0x2;
        }
        if ( (lane_mask & 0x30 ) == 0x30) {
            reg_val |= 0x4;
        }
        if ( (lane_mask & 0xC0 ) == 0xC0) {
            reg_val |= 0x8;
        }
        hsip_wr_field_(phy_info_ptr,FEC_FIFO_CLK_RST_SEL_REG, FEC_FIFO_IB_SELECT, reg_val);
    }
    return RR_SUCCESS;
}

// bundle select . 0 = use seperate clk, 1 = use bundle clock
return_result_t chal_cw_rtmr_rcv_pfifo_gbox_bundle_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t reg_val = 0;
    uint16_t lane_mask = 0;
    cfg_pam_or_nrz_t pam_or_nrz;

    lane_mask  = (egr_or_igr == EGR) ? cur_mode_parameter_ptr->host_llane_mask : cur_mode_parameter_ptr->line_llane_mask;
    pam_or_nrz = (egr_or_igr == EGR) ? cur_mode_parameter_ptr->host_pam_or_nrz_type : cur_mode_parameter_ptr->line_pam_or_nrz_type;
    reg_val = hsip_rd_field_(phy_info_ptr,FEC_FIFO_BUNDLE_DIS_REG,FEC_FIFO_BUNDLE_DIS );
    if (pam_or_nrz == CW_PAM || pam_or_nrz == CW_NRZ) {
        // set bundle disable
        if((lane_mask & 0x1 ) == 0x1 || (lane_mask & 0x2 ) == 0x2) {
            reg_val |= 0x1;
        }
        if((lane_mask & 0x4 ) == 0x4 || (lane_mask & 0x8 ) == 0x8) {
            reg_val |= 0x2;
        }
        if((lane_mask & 0x10 ) == 0x10 || (lane_mask & 0x20 ) == 0x20) {
            reg_val |= 0x4;
        }
        if((lane_mask & 0x40 ) == 0x40 || (lane_mask & 0x80 ) == 0x80) {
            reg_val |= 0x8;
        }
        hsip_wr_field_(phy_info_ptr,FEC_FIFO_BUNDLE_DIS_REG, FEC_FIFO_BUNDLE_DIS, reg_val);
    } else if (pam_or_nrz == CW_PAM100) {
        // clear bundle disable
        if ((lane_mask & 0x3 ) == 0x3) {
            reg_val &= (0x1 ^ 0xF);
        }
        if ((lane_mask & 0xC ) == 0xC) {
            reg_val &= (0x2 ^ 0xF);
        }
        if ((lane_mask & 0x30 ) == 0x30) {
            reg_val &= (0x4 ^ 0xF);
        }
        if ((lane_mask & 0xC0 ) == 0xC0) {
            reg_val &= (0x8 ^ 0xF);
        }
        hsip_wr_field_(phy_info_ptr,FEC_FIFO_BUNDLE_DIS_REG, FEC_FIFO_BUNDLE_DIS, reg_val);
    }
    return RR_SUCCESS;
}

return_result_t chal_cw_rtmr_rcv_pfifo_gbox_doutmux_cfg_8p (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t lane_mask = 0;
    cfg_pam_or_nrz_t rcv_pam_or_nrz;
    cfg_pam_or_nrz_t tmt_pam_or_nrz;
    bool PAM50G_M2_port2, PAM50G_M2_port6;
    uint8_t i;

    lane_mask  = (egr_or_igr == EGR) ? cur_mode_parameter_ptr->host_llane_mask : cur_mode_parameter_ptr->line_llane_mask;
    rcv_pam_or_nrz = (egr_or_igr == EGR) ? cur_mode_parameter_ptr->host_pam_or_nrz_type : cur_mode_parameter_ptr->line_pam_or_nrz_type;
    tmt_pam_or_nrz = (egr_or_igr == IGR) ? cur_mode_parameter_ptr->host_pam_or_nrz_type : cur_mode_parameter_ptr->line_pam_or_nrz_type;

    PAM50G_M2_port2 = rcv_pam_or_nrz == CW_PAM && tmt_pam_or_nrz == CW_NRZ && 
               (cur_port_config_ptr->port_50g_en[2] == PORT_ON && lane_mask == 0x2);  
    PAM50G_M2_port6 = rcv_pam_or_nrz == CW_PAM && tmt_pam_or_nrz == CW_NRZ && 
                (cur_port_config_ptr->port_50g_en[6] == PORT_ON && lane_mask == 0x20);
             
    if (PAM50G_M2_port2) {
        hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX4,  0x2);
        hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX5,  0x2);
    } else if (PAM50G_M2_port6) {
        hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX12, 0x2);
        hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX13, 0x2);
    } else if (rcv_pam_or_nrz == CW_PAM  || rcv_pam_or_nrz == CW_PAM100 || cur_mode_parameter_ptr->speed == SPEED_25G) {
       if(cur_mode_parameter_ptr->fec_slice ==  0x1)  { // program mux sel to 0 for each fec slices
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX1, 0x0);
       } else if (cur_mode_parameter_ptr->fec_slice == 0x2 ) {
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX2, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX3, 0x0);
       } else if (cur_mode_parameter_ptr->fec_slice == 0x4 ) {
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX4, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX5, 0x0);
       } else if (cur_mode_parameter_ptr->fec_slice == 0x8) {
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX6, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX7, 0x0);
       } else if (cur_mode_parameter_ptr->fec_slice == 0x10 ) {
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX8, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX9, 0x0);
       } else if (cur_mode_parameter_ptr->fec_slice == 0x20 ) {
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX10, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX11, 0x0);
       } else if (cur_mode_parameter_ptr->fec_slice == 0x40 ) {
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX12, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX13, 0x0);
       } else if (cur_mode_parameter_ptr->fec_slice == 0x80 ) {
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX14, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX15, 0x0);
       } else if ( cur_mode_parameter_ptr->fec_slice == 0x3) {
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX1, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX2, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX3, 0x0);
       } else if ( cur_mode_parameter_ptr->fec_slice == 0xC) {
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX4, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX5, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX6, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX7, 0x0);
       } else if ( cur_mode_parameter_ptr->fec_slice == 0x30) {
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX8, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX9, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX10, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX11, 0x0);
       } else if ( cur_mode_parameter_ptr->fec_slice == 0xC0) {
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX12, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX13, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX14, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX15, 0x0);
       } else if ( cur_mode_parameter_ptr->fec_slice == 0xF ) {
#if 1
         hsip_wr_reg_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, 0x0);
#else
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX1, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX2, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX3, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX4, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX5, 0x0);
#endif
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX6, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX7, 0x0);
       } else if ( cur_mode_parameter_ptr->fec_slice == 0xF0) {
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX8, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX9, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX10, 0x0);
#if 1
         hsip_wr_reg_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, 0x0);
#else
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX11, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX12, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX13, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX14, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX15, 0x0);
#endif
      } else if ( cur_mode_parameter_ptr->fec_slice == 0xFF) {
#if 1
        for (i=0; i<3; i++)
            hsip_wr_reg_(phy_info_ptr, RCV_PFIFO_DMUX0_REG+i*4, 0x0);
#else
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX1, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX2, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX3, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX4, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX5, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX6, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX7, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX8, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX9, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX10, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX11, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX12, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX13, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX14, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX15, 0x0);
#endif
      }
    } else if(rcv_pam_or_nrz == CW_NRZ && cur_mode_parameter_ptr->speed == SPEED_50G) { // program mux to 0 for each fec slice
       if (cur_mode_parameter_ptr->fec_slice == 0x1  ) {
           hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX1, 0x1);
           hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX2, 0x1);
           hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX3, 0x1);
       } else if (cur_mode_parameter_ptr->fec_slice == 0x4 ) {
           hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX4, 0x1);
           hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX5, 0x1);
           hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX6, 0x1);
           hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX7, 0x1);
       } else if (cur_mode_parameter_ptr->fec_slice == 0x10) {
           hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX8, 0x1);
           hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX9, 0x1);
           hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX10, 0x1);
           hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX11, 0x1);
       } else if (cur_mode_parameter_ptr->fec_slice == 0x40) {
           hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX12, 0x1);
           hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX13, 0x1);
           hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX14, 0x1);
           hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX15, 0x1);
       }
    } else if(rcv_pam_or_nrz == CW_NRZ && cur_mode_parameter_ptr->speed == SPEED_100G) {
        if(cur_mode_parameter_ptr->fec_slice == 0x3) {
#if 1
            hsip_wr_reg_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, 0x36DB);
#else
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX1, 0x3);
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX2, 0x3);
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX3, 0x3);
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX4, 0x3);
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX5, 0x3);
#endif
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX6, 0x3);
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX7, 0x3);
        } else if(cur_mode_parameter_ptr->fec_slice == 0x30) {
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX8, 0x3);
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX9, 0x3);
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX10, 0x3);
#if 1
            hsip_wr_reg_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, 0x36DB);
#else
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX11, 0x3);
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX12, 0x3);
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX13, 0x3);
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX14, 0x3);
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX15, 0x3);
#endif
    }
    } else if(rcv_pam_or_nrz == CW_NRZ && cur_mode_parameter_ptr->speed == SPEED_200G) {
#if 1
        for (i=0; i<3; i++)
            hsip_wr_reg_(phy_info_ptr, RCV_PFIFO_DMUX0_REG+i*4, 0x4924);
#else
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX1, 0x4);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX2, 0x4);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX3, 0x4);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX4, 0x4);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX5, 0x4);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX6, 0x4);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX7, 0x4);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX8, 0x4);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX9, 0x4);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX10, 0x4);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX11, 0x4);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX12, 0x4);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX13, 0x4);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX14, 0x4);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX15, 0x4);
#endif
    }

    else {};

    return RR_SUCCESS;
}


return_result_t chal_cw_rtmr_rcv_pfifo_gbox_doutmux_cfg_4p (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t lane_mask = 0;
    cfg_pam_or_nrz_t rcv_pam_or_nrz;
    cfg_pam_or_nrz_t tmt_pam_or_nrz;
    bool PAM50G_M2_port2, PAM50G_M2_port6;
    uint8_t i;

    lane_mask  = (egr_or_igr == EGR) ? cur_mode_parameter_ptr->host_llane_mask : cur_mode_parameter_ptr->line_llane_mask;
    rcv_pam_or_nrz = (egr_or_igr == EGR) ? cur_mode_parameter_ptr->host_pam_or_nrz_type : cur_mode_parameter_ptr->line_pam_or_nrz_type;
    tmt_pam_or_nrz = (egr_or_igr == IGR) ? cur_mode_parameter_ptr->host_pam_or_nrz_type : cur_mode_parameter_ptr->line_pam_or_nrz_type;

    PAM50G_M2_port2 = rcv_pam_or_nrz == CW_PAM && tmt_pam_or_nrz == CW_NRZ && 
               (cur_port_config_ptr->port_50g_en[2] == PORT_ON && lane_mask == 0x2);  
    PAM50G_M2_port6 = rcv_pam_or_nrz == CW_PAM && tmt_pam_or_nrz == CW_NRZ && 
                (cur_port_config_ptr->port_50g_en[6] == PORT_ON && lane_mask == 0x20);
             
    if (PAM50G_M2_port2) {
        hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX4,  0x2);
        hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX5,  0x2);
    } else if (PAM50G_M2_port6) {
        hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX12, 0x2);
        hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX13, 0x2);
    } else if ((rcv_pam_or_nrz == CW_PAM && cur_mode_parameter_ptr->speed == SPEED_50G)  || rcv_pam_or_nrz == CW_PAM100 || cur_mode_parameter_ptr->speed == SPEED_25G) {    
       if(cur_mode_parameter_ptr->fec_slice ==  0x1)  { // program mux sel to 0 for each fec slices
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX1, 0x0);
       } else if (cur_mode_parameter_ptr->fec_slice == 0x2 ) {
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX2, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX3, 0x0);
       } else if (cur_mode_parameter_ptr->fec_slice == 0x4 ) {
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX4, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX5, 0x0);
       } else if (cur_mode_parameter_ptr->fec_slice == 0x8) {
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX6, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX7, 0x0);
       } else if (cur_mode_parameter_ptr->fec_slice == 0x10 ) {
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX8, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX9, 0x0);
       } else if (cur_mode_parameter_ptr->fec_slice == 0x20 ) {
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX10, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX11, 0x0);
       } else if (cur_mode_parameter_ptr->fec_slice == 0x40 ) {
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX12, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX13, 0x0);
       } else if (cur_mode_parameter_ptr->fec_slice == 0x80 ) {
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX14, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX15, 0x0);
       } else if ( cur_mode_parameter_ptr->fec_slice == 0x3) {
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX1, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX2, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX3, 0x0);
       } else if ( cur_mode_parameter_ptr->fec_slice == 0xC) {
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX4, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX5, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX6, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX7, 0x0);
       } else if ( cur_mode_parameter_ptr->fec_slice == 0x30) {
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX8, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX9, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX10, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX11, 0x0);
       } else if ( cur_mode_parameter_ptr->fec_slice == 0xC0) {
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX12, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX13, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX14, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX15, 0x0);
       } else if ( cur_mode_parameter_ptr->fec_slice == 0xF ) {
#if 1
        hsip_wr_reg_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, 0x0);
#else
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX1, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX2, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX3, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX4, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX5, 0x0);
#endif
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX6, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX7, 0x0);
       } else if ( cur_mode_parameter_ptr->fec_slice == 0xF0) {
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX8, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX9, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX10, 0x0);
#if 1
        hsip_wr_reg_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, 0x0);
#else
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX11, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX12, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX13, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX14, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX15, 0x0);
#endif
      } else if ( cur_mode_parameter_ptr->fec_slice == 0xFF) {
#if 1
        for (i=0; i<3; i++)
            hsip_wr_reg_(phy_info_ptr, RCV_PFIFO_DMUX0_REG+i*4, 0x0);
#else
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX1, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX2, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX3, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX4, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX5, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX6, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX7, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX8, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX9, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX10, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX11, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX12, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX13, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX14, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX15, 0x0);
#endif
      }
    } else if(rcv_pam_or_nrz == CW_NRZ && cur_mode_parameter_ptr->speed == SPEED_50G) { // program mux to 0 for each fec slice
       if (cur_mode_parameter_ptr->fec_slice == 0x1  ) {
           hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX1, 0x1);
           hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX2, 0x1);
           hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX3, 0x1);
       } else if (cur_mode_parameter_ptr->fec_slice == 0x4 ) {
           hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX4, 0x1);
           hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX5, 0x1);
           hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX6, 0x1);
           hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX7, 0x1);
       } else if (cur_mode_parameter_ptr->fec_slice == 0x10) {
           hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX8, 0x1);
           hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX9, 0x1);
           hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX10, 0x1);
           hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX11, 0x1);
       } else if (cur_mode_parameter_ptr->fec_slice == 0x40) {
           hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX12, 0x1);
           hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX13, 0x1);
           hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX14, 0x1);
           hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX15, 0x1);
       }
    } else if(rcv_pam_or_nrz == CW_NRZ && cur_mode_parameter_ptr->speed == SPEED_100G) {
        if(cur_mode_parameter_ptr->fec_slice == 0x3) {
#if 1
            hsip_wr_reg_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, 0x471B);
#else
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX1, 0x3);
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX2, 0x3);
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX3, 0x4);
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX4, 0x3);
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX5, 0x4);
#endif
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX6, 0x3);
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX7, 0x4);
        } else if(cur_mode_parameter_ptr->fec_slice == 0x30) {
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX8, 0x3);
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX9, 0x3);
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX10, 0x3);
#if 1
            hsip_wr_reg_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, 0x36DB);
#else
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX11, 0x3);
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX12, 0x3);
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX13, 0x3);
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX14, 0x3);
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX15, 0x3);
#endif
        }
    } else if(rcv_pam_or_nrz == CW_PAM && cur_mode_parameter_ptr->speed == SPEED_100G) {
        if(cur_mode_parameter_ptr->fec_slice == 0x3) {
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX1, 0x0);
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX2, 0x3);
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX3, 0x3);
        } else if(cur_mode_parameter_ptr->fec_slice == 0x30) {
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX8, 0x0);
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX9, 0x0);
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX10, 0x3);
            hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX11, 0x3);
	    }         
    } else if(rcv_pam_or_nrz == CW_NRZ && cur_mode_parameter_ptr->speed == SPEED_200G) {
#if 1
        for (i=0; i<3; i++)
            hsip_wr_reg_(phy_info_ptr, RCV_PFIFO_DMUX0_REG+i*4, 0x4924);
#else
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX1, 0x4);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX2, 0x4);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX3, 0x4);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX4, 0x4);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX5, 0x4);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX6, 0x4);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX7, 0x4);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX8, 0x4);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX9, 0x4);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX10, 0x4);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX11, 0x4);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX12, 0x4);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX13, 0x4);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX14, 0x4);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX2_REG, RCV_PFIFO_DOUT_MUX15, 0x4);
#endif
    } else if(rcv_pam_or_nrz == CW_PAM && cur_mode_parameter_ptr->speed == SPEED_200G) {
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX1, 0x0);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX2, 0x3);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX3, 0x3);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX4, 0x3);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX0_REG, RCV_PFIFO_DOUT_MUX5, 0x3);
         
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX6, 0x3);
         hsip_wr_field_(phy_info_ptr, RCV_PFIFO_DMUX1_REG, RCV_PFIFO_DOUT_MUX7, 0x3);
    }    
    else {};

    return RR_SUCCESS;
}

return_result_t chal_cw_rtmr_rcv_pfifo_gbox_doutmux_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr)
{
    if (ml_cw_rtmr_is_A1_4p_port(cur_mode_parameter_ptr)) {        
        return chal_cw_rtmr_rcv_pfifo_gbox_doutmux_cfg_4p(phy_info_ptr, cur_mode_parameter_ptr, cur_port_config_ptr, egr_or_igr);
    } else {
        return chal_cw_rtmr_rcv_pfifo_gbox_doutmux_cfg_8p(phy_info_ptr, cur_mode_parameter_ptr, cur_port_config_ptr, egr_or_igr);
    }
}

return_result_t chal_cw_rtmr_tmt_pfifo_gbox_dinmux_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t lane_mask = 0;
    cfg_pam_or_nrz_t rcv_pam_or_nrz;
    cfg_pam_or_nrz_t tmt_pam_or_nrz;
    bool PAM50G_M2_port2, PAM50G_M2_port6;
    uint8_t i;

    lane_mask  = (egr_or_igr == IGR) ? cur_mode_parameter_ptr->host_llane_mask : cur_mode_parameter_ptr->line_llane_mask;
    rcv_pam_or_nrz = (egr_or_igr == EGR) ? cur_mode_parameter_ptr->host_pam_or_nrz_type : cur_mode_parameter_ptr->line_pam_or_nrz_type;
    tmt_pam_or_nrz = (egr_or_igr == IGR) ? cur_mode_parameter_ptr->host_pam_or_nrz_type : cur_mode_parameter_ptr->line_pam_or_nrz_type;

    PAM50G_M2_port2 = tmt_pam_or_nrz == CW_PAM && rcv_pam_or_nrz == CW_NRZ && 
               (cur_port_config_ptr->port_50g_en[2] == PORT_ON && lane_mask == 0x2);  
    PAM50G_M2_port6 = tmt_pam_or_nrz == CW_PAM && rcv_pam_or_nrz == CW_NRZ && 
                (cur_port_config_ptr->port_50g_en[6] == PORT_ON && lane_mask == 0x20);
             
    if (PAM50G_M2_port2) {
        hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX4,  0x2);
        hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX5,  0x2);
    } else if (PAM50G_M2_port6){
        hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX12, 0x2);
        hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX13, 0x2);
    } else if (tmt_pam_or_nrz == CW_PAM || tmt_pam_or_nrz == CW_PAM100 ||  cur_mode_parameter_ptr->speed == SPEED_25G  ) {
       if(cur_mode_parameter_ptr->fec_slice ==  0x1)  { // program mux sel to 0 for each fec slices
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX1, 0x0);
       } else if (cur_mode_parameter_ptr->fec_slice == 0x2 ) {
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX2, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX3, 0x0);
       } else if (cur_mode_parameter_ptr->fec_slice == 0x4 ) {
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX4, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX5, 0x0);
       } else if (cur_mode_parameter_ptr->fec_slice == 0x8) {
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX6, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX7, 0x0);
       } else if (cur_mode_parameter_ptr->fec_slice == 0x10 ) {
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX8, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX9, 0x0);
       } else if (cur_mode_parameter_ptr->fec_slice == 0x20 ) {
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX10, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX11, 0x0);
       } else if (cur_mode_parameter_ptr->fec_slice == 0x40 ) {
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX12, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX13, 0x0);
       } else if (cur_mode_parameter_ptr->fec_slice == 0x80 ) {
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX14, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX15, 0x0);
       } else if ( cur_mode_parameter_ptr->fec_slice == 0x3) {
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX1, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX2, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX3, 0x0);
       } else if ( cur_mode_parameter_ptr->fec_slice == 0xC) {
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX4, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX5, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX6, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX7, 0x0);
       } else if ( cur_mode_parameter_ptr->fec_slice == 0x30) {
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX8, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX9, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX10, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX11, 0x0);
       } else if ( cur_mode_parameter_ptr->fec_slice == 0xC0) {
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX12, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX13, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX14, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX15, 0x0);
       } else if ( cur_mode_parameter_ptr->fec_slice == 0xF ) {
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX1, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX2, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX3, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX4, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX5, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX6, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX7, 0x0);
       } else if ( cur_mode_parameter_ptr->fec_slice == 0xF0) {
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX8, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX9, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX10, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX11, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX12, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX13, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX14, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX15, 0x0);
      } else if ( cur_mode_parameter_ptr->fec_slice == 0xFF) {
#if 1
        for (i=0; i<3; i++)
            hsip_wr_reg_(phy_info_ptr, TMT_PFIFO_DMUX0_REG+i*4, 0x0);
#else
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX1, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX2, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX3, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX4, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX5, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX6, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX7, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX8, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX9, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX10, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX11, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX12, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX13, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX14, 0x0);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX15, 0x0);
#endif
      }
    } else if(tmt_pam_or_nrz == CW_NRZ && cur_mode_parameter_ptr->speed == SPEED_50G) { // program mux to 0 for each fec slice
       if (cur_mode_parameter_ptr->fec_slice == 0x1  ) {
           hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX1, 0x1);
           hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX2, 0x1);
           hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX3, 0x1);
       } else if (cur_mode_parameter_ptr->fec_slice == 0x4 ) {
           hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX4, 0x1);
           hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX5, 0x1);
           hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX6, 0x1);
           hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX7, 0x1);
       } else if (cur_mode_parameter_ptr->fec_slice == 0x10) {
           hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX8, 0x1);
           hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX9, 0x1);
           hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX10, 0x1);
           hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX11, 0x1);
       } else if (cur_mode_parameter_ptr->fec_slice == 0x40) {
           hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX12, 0x1);
           hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX13, 0x1);
           hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX14, 0x1);
           hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX15, 0x1);
       }
    } else if(tmt_pam_or_nrz == CW_NRZ && cur_mode_parameter_ptr->speed == SPEED_100G) {
        if(cur_mode_parameter_ptr->fec_slice == 0x3) {
            hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX1, 0x3);
            hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX2, 0x3);
            hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX3, 0x3);
            hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX4, 0x3);
            hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX5, 0x3);
            hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX6, 0x3);
            hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX7, 0x3);
        } else if(cur_mode_parameter_ptr->fec_slice == 0x30) {
            hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX8, 0x3);
            hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX9, 0x3);
            hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX10, 0x3);
            hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX11, 0x3);
            hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX12, 0x3);
            hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX13, 0x3);
            hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX14, 0x3);
            hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX15, 0x3);
    }
    } else if(tmt_pam_or_nrz == CW_NRZ && cur_mode_parameter_ptr->speed == SPEED_200G) {
#if 1
        for (i=0; i<3; i++)
            hsip_wr_reg_(phy_info_ptr, TMT_PFIFO_DMUX0_REG+i*4, 0x4924);
#else
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX1, 0x4);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX2, 0x4);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX3, 0x4);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX4, 0x4);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX0_REG, TMT_PFIFO_DIN_MUX5, 0x4);

         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX6, 0x4);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX7, 0x4);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX8, 0x4);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX9, 0x4);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX1_REG, TMT_PFIFO_DIN_MUX10, 0x4);

         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX11, 0x4);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX12, 0x4);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX13, 0x4);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX14, 0x4);
         hsip_wr_field_(phy_info_ptr, TMT_PFIFO_DMUX2_REG, TMT_PFIFO_DIN_MUX15, 0x4);
#endif
    }
    else {};

  return RR_SUCCESS;
}

return_result_t chal_cw_rtmr_tmt_pfifo_gbox_mode_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t lane_mask = 0;
    cfg_pam_or_nrz_t pam_or_nrz;

    lane_mask  = (egr_or_igr == IGR) ? cur_mode_parameter_ptr->host_llane_mask : cur_mode_parameter_ptr->line_llane_mask;
    pam_or_nrz = (egr_or_igr == IGR) ? cur_mode_parameter_ptr->host_pam_or_nrz_type : cur_mode_parameter_ptr->line_pam_or_nrz_type;
    if(pam_or_nrz == CW_PAM) {  // write 0
        //reg_val &= (lane_mask ^ 0xFF);
        if (  (lane_mask& 0x1 ) == 0x1) {
            hsip_wr_field_(phy_info_ptr, TMT_PAM_NRZ_CTRL_REG, TMT_NRZ_PAM_0, 0x0); 
        }
        if ( (lane_mask& 0x2 ) == 0x2) {
           hsip_wr_field_(phy_info_ptr, TMT_PAM_NRZ_CTRL_REG, TMT_NRZ_PAM_1, 0x0); 
        }
        if ( (lane_mask& 0x4 ) == 0x4) {
           hsip_wr_field_(phy_info_ptr, TMT_PAM_NRZ_CTRL_REG, TMT_NRZ_PAM_2, 0x0); 
        }
        if ( (lane_mask& 0x8 ) == 0x8) {
           hsip_wr_field_(phy_info_ptr, TMT_PAM_NRZ_CTRL_REG, TMT_NRZ_PAM_3, 0x0); 
        }
        if ( (lane_mask& 0x10 ) == 0x10) {
           hsip_wr_field_(phy_info_ptr, TMT_PAM_NRZ_CTRL_REG, TMT_NRZ_PAM_4, 0x0); 
        }
        if ( (lane_mask& 0x20 ) == 0x20) {
           hsip_wr_field_(phy_info_ptr, TMT_PAM_NRZ_CTRL_REG, TMT_NRZ_PAM_5, 0x0); 
        }
        if ( (lane_mask& 0x40 ) == 0x40) {
           hsip_wr_field_(phy_info_ptr, TMT_PAM_NRZ_CTRL_REG, TMT_NRZ_PAM_6, 0x0); 
        }
        if ( (lane_mask& 0x80 ) == 0x80) {
           hsip_wr_field_(phy_info_ptr, TMT_PAM_NRZ_CTRL_REG, TMT_NRZ_PAM_7, 0x0); 
        }
    } else if (pam_or_nrz == CW_NRZ) { // NRZ, write 1
        //reg_val |= ( (lane_mask& 0xFF);
        if (  (lane_mask& 0x1 ) == 0x1) {
           hsip_wr_field_(phy_info_ptr, TMT_PAM_NRZ_CTRL_REG, TMT_NRZ_PAM_0, 0x1); 
        }
        if ( (lane_mask& 0x2 ) == 0x2) {
           hsip_wr_field_(phy_info_ptr, TMT_PAM_NRZ_CTRL_REG, TMT_NRZ_PAM_1, 0x1); 
        }
        if ( (lane_mask& 0x4 ) == 0x4) {
           hsip_wr_field_(phy_info_ptr, TMT_PAM_NRZ_CTRL_REG, TMT_NRZ_PAM_2, 0x1); 
        }
        if ( (lane_mask& 0x8 ) == 0x8) {
           hsip_wr_field_(phy_info_ptr, TMT_PAM_NRZ_CTRL_REG, TMT_NRZ_PAM_3, 0x1); 
        }
        if ( (lane_mask& 0x10 ) == 0x10) {
           hsip_wr_field_(phy_info_ptr, TMT_PAM_NRZ_CTRL_REG, TMT_NRZ_PAM_4, 0x1); 
        }
        if ( (lane_mask& 0x20 ) == 0x20) {
           hsip_wr_field_(phy_info_ptr, TMT_PAM_NRZ_CTRL_REG, TMT_NRZ_PAM_5, 0x1); 
        }
        if ( (lane_mask& 0x40 ) == 0x40) {
           hsip_wr_field_(phy_info_ptr, TMT_PAM_NRZ_CTRL_REG, TMT_NRZ_PAM_6, 0x1); 
        }
        if ( (lane_mask& 0x80 ) == 0x80) {
           hsip_wr_field_(phy_info_ptr, TMT_PAM_NRZ_CTRL_REG, TMT_NRZ_PAM_7, 0x1); 
        }
    } else if (pam_or_nrz == CW_PAM100) {
        if (  (lane_mask& 0x3 ) == 0x3) {
           hsip_wr_field_(phy_info_ptr, TMT_PAM_NRZ_CTRL_REG, TMT_NRZ_PAM_0, 0x2); 
        }
        if ( (lane_mask& 0xC ) == 0xC) {
           hsip_wr_field_(phy_info_ptr, TMT_PAM_NRZ_CTRL_REG, TMT_NRZ_PAM_1, 0x2); 
        }
        if ( (lane_mask& 0x30 ) == 0x30) {
           hsip_wr_field_(phy_info_ptr, TMT_PAM_NRZ_CTRL_REG, TMT_NRZ_PAM_2, 0x2); 
        }
        if ( (lane_mask& 0xC0 ) == 0xC0) {
           hsip_wr_field_(phy_info_ptr, TMT_PAM_NRZ_CTRL_REG, TMT_NRZ_PAM_3, 0x2); 
        }
    }
    //hsip_wr_fields(phy_info_ptr, TMT_PAM_NRZ_CTRL_REG, lane_mask, reg_val); 
    return RR_SUCCESS;
}

//rd mux 
return_result_t chal_cw_rtmr_tmt_pfifo_gbox_rdmux_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr)
{
    if ((egr_or_igr == EGR && cur_mode_parameter_ptr->line_pam_or_nrz_type == CW_PAM && cur_mode_parameter_ptr->host_pam_or_nrz_type == CW_NRZ) ||
        (egr_or_igr == IGR && cur_mode_parameter_ptr->host_pam_or_nrz_type == CW_PAM && cur_mode_parameter_ptr->line_pam_or_nrz_type == CW_NRZ)) {
        // Set RD_2_1_MUX and RD_6_5_MUX for PCS/KR to KP4 and KP4 to PCS/KR M2 modes: KP4 side lane 1 and 3
        if (cur_mode_parameter_ptr->speed == SPEED_50G) {
            // clear the mux for 50G PCS/KR to KP4
            if (cur_port_config_ptr->port_50g_en[2])
                hsip_wr_field_(phy_info_ptr,TMT_FEC_FIFO_CLK_RST_SEL_REG, RD_2_1_MUX, 0);
            if (cur_port_config_ptr->port_50g_en[6])
                hsip_wr_field_(phy_info_ptr,TMT_FEC_FIFO_CLK_RST_SEL_REG, RD_6_5_MUX, 0);
        }
        else if (cur_mode_parameter_ptr->speed == SPEED_100G) {
            if (cur_port_config_ptr->port_100g_en[0])
                hsip_wr_field_(phy_info_ptr,TMT_FEC_FIFO_CLK_RST_SEL_REG, RD_2_1_MUX, 1);
            if (cur_port_config_ptr->port_100g_en[2])
                hsip_wr_field_(phy_info_ptr,TMT_FEC_FIFO_CLK_RST_SEL_REG, RD_6_5_MUX, 1);
        }
        else if (cur_mode_parameter_ptr->speed == SPEED_200G) {
            hsip_wr_field_(phy_info_ptr,TMT_FEC_FIFO_CLK_RST_SEL_REG, RD_2_1_MUX, 1);
            hsip_wr_field_(phy_info_ptr,TMT_FEC_FIFO_CLK_RST_SEL_REG, RD_6_5_MUX, 1);
        }
    }
    else {
        hsip_wr_field_(phy_info_ptr,TMT_FEC_FIFO_CLK_RST_SEL_REG, RD_2_1_MUX, 0);
        hsip_wr_field_(phy_info_ptr,TMT_FEC_FIFO_CLK_RST_SEL_REG, RD_6_5_MUX, 0);
    }
    return RR_SUCCESS;
}

// ob_sel . 0 = use seperate clk, 1 = use bundle clock
return_result_t chal_cw_rtmr_tmt_pfifo_gbox_ob_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t reg_val = 0;
    uint16_t lane_mask = 0;
    cfg_pam_or_nrz_t pam_or_nrz;

    lane_mask  = (egr_or_igr == IGR) ? cur_mode_parameter_ptr->host_llane_mask : cur_mode_parameter_ptr->line_llane_mask;
    pam_or_nrz = (egr_or_igr == IGR) ? cur_mode_parameter_ptr->host_pam_or_nrz_type : cur_mode_parameter_ptr->line_pam_or_nrz_type;
    reg_val = hsip_rd_field_(phy_info_ptr,TMT_FEC_FIFO_CLK_RST_SEL_REG ,FEC_FIFO_OB_SELECT );
    if (pam_or_nrz == CW_PAM || pam_or_nrz == CW_NRZ) {
        if(( lane_mask & 0x1 ) == 0x1 || ( lane_mask & 0x2 ) == 0x2) {
            reg_val &= 0x1 ^ 0xF;
        }
        if(( lane_mask & 0x4 ) == 0x4 || ( lane_mask & 0x8 ) == 0x8) {
            reg_val &= 0x2 ^ 0xF;
        }
        if(( lane_mask & 0x10 ) == 0x10 || ( lane_mask & 0x20 ) == 0x20) {
            reg_val &= 0x4 ^ 0xF;
        }
        if(( lane_mask & 0x40 ) == 0x40 || ( lane_mask & 0x80 ) == 0x80) {
            reg_val &= 0x8 ^ 0xF;
        }
        hsip_wr_field_(phy_info_ptr,TMT_FEC_FIFO_CLK_RST_SEL_REG, FEC_FIFO_OB_SELECT, reg_val);
    } else if (pam_or_nrz == CW_PAM100) {
        if (( lane_mask & 0x3 ) == 0x3) {
            reg_val |= 0x1;
        }
        if (( lane_mask & 0xC ) == 0xC) {
            reg_val |= 0x2;
        }
        if (( lane_mask & 0x30 ) == 0x30) {
            reg_val |= 0x4;
        }
        if (( lane_mask & 0xC0 ) == 0xC0) {
            reg_val |= 0x8;
        }
        hsip_wr_field_(phy_info_ptr,TMT_FEC_FIFO_CLK_RST_SEL_REG, FEC_FIFO_OB_SELECT, reg_val);
    }
    return RR_SUCCESS;
}

// ib_sel . 0 = use seperate clk, 1 = use bundle clock
return_result_t chal_cw_rtmr_tmt_pfifo_gbox_ib_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t reg_val = 0;
    uint16_t lane_mask = 0;
    cfg_pam_or_nrz_t pam_or_nrz;

    lane_mask  = (egr_or_igr == IGR) ? cur_mode_parameter_ptr->host_llane_mask : cur_mode_parameter_ptr->line_llane_mask;
    pam_or_nrz = (egr_or_igr == IGR) ? cur_mode_parameter_ptr->host_pam_or_nrz_type : cur_mode_parameter_ptr->line_pam_or_nrz_type;
    reg_val = hsip_rd_field_(phy_info_ptr,TMT_FEC_FIFO_CLK_RST_SEL_REG ,FEC_FIFO_IB_SELECT );
    if (pam_or_nrz == CW_PAM || pam_or_nrz == CW_NRZ) {
        if((lane_mask & 0x1 ) == 0x1 || (lane_mask & 0x2 ) == 0x2) {
            reg_val &= 0x1 ^ 0xF;
        }
        if((lane_mask & 0x4 ) == 0x4 || (lane_mask & 0x8 ) == 0x8) {
            reg_val &= 0x2 ^ 0xF;
        }
        if((lane_mask & 0x10 ) == 0x10 || (lane_mask & 0x20 ) == 0x20) {
            reg_val &= 0x4 ^ 0xF;
        }
        if((lane_mask & 0x40 ) == 0x40 || (lane_mask & 0x80 ) == 0x80) {
            reg_val &= 0x8 ^ 0xF;
        }
        hsip_wr_field_(phy_info_ptr,TMT_FEC_FIFO_CLK_RST_SEL_REG, FEC_FIFO_IB_SELECT, reg_val);
    } else if (pam_or_nrz == CW_PAM100) {
        if ((lane_mask & 0x3 ) == 0x3) {
            reg_val |= 0x1;
        }
        if ((lane_mask & 0xC ) == 0xC) {
            reg_val |= 0x2;
        }
        if ((lane_mask & 0x30 ) == 0x30) {
            reg_val |= 0x4;
        }
        if ((lane_mask & 0xC0 ) == 0xC0) {
            reg_val |= 0x8;
        }
        hsip_wr_field_(phy_info_ptr,TMT_FEC_FIFO_CLK_RST_SEL_REG, FEC_FIFO_IB_SELECT, reg_val);
    }
    return RR_SUCCESS;
}

// bundle select . 0 = use seperate clk, 1 = use bundle clock
return_result_t chal_cw_rtmr_tmt_pfifo_gbox_bundle_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t reg_val = 0;
    uint16_t lane_mask = 0;
    cfg_pam_or_nrz_t pam_or_nrz;

    lane_mask  = (egr_or_igr == IGR) ? cur_mode_parameter_ptr->host_llane_mask : cur_mode_parameter_ptr->line_llane_mask;
    pam_or_nrz = (egr_or_igr == IGR) ? cur_mode_parameter_ptr->host_pam_or_nrz_type : cur_mode_parameter_ptr->line_pam_or_nrz_type;
    reg_val = hsip_rd_field_(phy_info_ptr,TMT_FEC_FIFO_BUNDLE_DIS_REG,FEC_FIFO_BUNDLE_DIS );
    if (pam_or_nrz == CW_PAM || pam_or_nrz == CW_NRZ) {
        // set bundle disable
        if((lane_mask & 0x1) == 0x1 || (lane_mask & 0x2) == 0x2) {
            reg_val |= 0x1;
        }
        if((lane_mask & 0x4) == 0x4 || (lane_mask & 0x8) == 0x8) {
            reg_val |= 0x2;
        }
        if((lane_mask & 0x10) == 0x10 || (lane_mask & 0x20) == 0x20) {
            reg_val |= 0x4;
        }
        if((lane_mask & 0x40) == 0x40 || (lane_mask & 0x80) == 0x80) {
            reg_val |= 0x8;
        }
        hsip_wr_field_(phy_info_ptr,TMT_FEC_FIFO_BUNDLE_DIS_REG, FEC_FIFO_BUNDLE_DIS, reg_val);
    } else if (pam_or_nrz == CW_PAM100) {
        // clear bundle disable
        if ((lane_mask & 0x3) == 0x3) {
            reg_val &= (0x1 ^ 0xF);
        }
        if ((lane_mask & 0xC) == 0xC) {
            reg_val &= (0x2 ^ 0xF);
        }
        if ((lane_mask & 0x30) == 0x30) {
            reg_val &= (0x4 ^ 0xF);
        }
        if ((lane_mask & 0xC0) == 0xC0) {
            reg_val &= (0x8 ^ 0xF);
        }
        hsip_wr_field_(phy_info_ptr,TMT_FEC_FIFO_BUNDLE_DIS_REG, FEC_FIFO_BUNDLE_DIS, reg_val);
    }
    return RR_SUCCESS;
}

return_result_t chal_cw_rtmr_tmt_pfifo_gbox_doutmux_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, uint8_t dsel_pcs_fecb, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t lane_mask = 0;
    lane_mask  = (egr_or_igr == IGR) ? cur_mode_parameter_ptr->host_llane_mask : cur_mode_parameter_ptr->line_llane_mask;

    if (dsel_pcs_fecb) 
        hsip_wr_fields(phy_info_ptr, TMT_FEC_PCS_DATA_MUX_CTRL_REG, lane_mask, lane_mask);
    else
        /* coverity[bit_and_with_zero] */
        hsip_wr_fields(phy_info_ptr, TMT_FEC_PCS_DATA_MUX_CTRL_REG, lane_mask, 0);

    return RR_SUCCESS;
}

// Checked RETIMER_IGR or RETIMER_EGR
// FEC_PCS datapath mux select
return_result_t cw_rtmr_fec_enc_datamux_config_slice(phy_info_t* phy_info_ptr, cfg_fec_enc_datamux_t cur_fec_enc_datamux,  cfg_rtm_slice_t cur_rtm_slice)
{
    switch(cur_rtm_slice) {
        case(SLICE0):
            hsip_wr_field_(phy_info_ptr, FEC_DATA_MUX_SEL0_REG, MD_FEC_ENC_DATA_MUX0, cur_fec_enc_datamux);        
            break;
        case(SLICE1):
            hsip_wr_field_(phy_info_ptr, FEC_DATA_MUX_SEL1_REG, MD_FEC_ENC_DATA_MUX1, cur_fec_enc_datamux);   
            break;
        case(SLICE2):
            hsip_wr_field_(phy_info_ptr, FEC_DATA_MUX_SEL2_REG, MD_FEC_ENC_DATA_MUX2, cur_fec_enc_datamux);        
            break;
        case(SLICE3):
            hsip_wr_field_(phy_info_ptr, FEC_DATA_MUX_SEL3_REG, MD_FEC_ENC_DATA_MUX3, cur_fec_enc_datamux);   
            break;
        case(SLICE4):
            hsip_wr_field_(phy_info_ptr, FEC_DATA_MUX_SEL4_REG, MD_FEC_ENC_DATA_MUX4, cur_fec_enc_datamux);        
            break;
        case(SLICE5):
            hsip_wr_field_(phy_info_ptr, FEC_DATA_MUX_SEL5_REG, MD_FEC_ENC_DATA_MUX5, cur_fec_enc_datamux);   
            break;
        case(SLICE6):
            hsip_wr_field_(phy_info_ptr, FEC_DATA_MUX_SEL6_REG, MD_FEC_ENC_DATA_MUX6, cur_fec_enc_datamux);        
            break; 
        case(SLICE7):
            hsip_wr_field_(phy_info_ptr, FEC_DATA_MUX_SEL7_REG, MD_FEC_ENC_DATA_MUX7, cur_fec_enc_datamux);   
            break; 
    }
    return RR_SUCCESS;
}

// checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_fec_enc_datamux_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_fec_enc_datamux_t datamux_sel)
{
    // 0 = fec prbs, 1 = fec decoder output, 2 = xencoder output
    switch (cur_mode_parameter_ptr->speed) {     
        case (SPEED_400G): 
            if(cur_port_config_ptr->port_400g_en == PORT_ON )  {
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE0);
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE1);
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE2);
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE3);
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE4);
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE5);
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE6);
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE7);
            }
            break;
        case (SPEED_200G):
            if(cur_port_config_ptr->port_200g_en[0] == PORT_ON )  {
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE0);
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE1);
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE2);
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE3);
            } else if (cur_port_config_ptr->port_200g_en[1] == PORT_ON )  {
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE4);
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE5);
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE6);
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE7);
            }
            break;
        case(SPEED_100G):
            if (cur_port_config_ptr->port_100g_en[0] == PORT_ON )  {
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE0);
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE1);
            } else if (cur_port_config_ptr->port_100g_en[1] == PORT_ON )  {
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE2);
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE3);
            } else if (cur_port_config_ptr->port_100g_en[2] == PORT_ON )  {
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE4);
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE5);
            } else if (cur_port_config_ptr->port_100g_en[3] == PORT_ON )  {
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE6);
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE7);
            }
            break;
        case(SPEED_50G):
            if (cur_port_config_ptr->port_50g_en[0] == PORT_ON )  {
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE0);
            } else if (cur_port_config_ptr->port_50g_en[1] == PORT_ON )  {
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE1);
            } else if (cur_port_config_ptr->port_50g_en[2] == PORT_ON )  {
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE2);
            } else if (cur_port_config_ptr->port_50g_en[3] == PORT_ON )  {
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE3);
            } else if (cur_port_config_ptr->port_50g_en[4] == PORT_ON )  {
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE4);
            } else if (cur_port_config_ptr->port_50g_en[5] == PORT_ON )  {
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE5);
            } else if (cur_port_config_ptr->port_50g_en[6] == PORT_ON )  {
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE6);
            } else if (cur_port_config_ptr->port_50g_en[7] == PORT_ON )  {
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE7);
            }
            break;
        case(SPEED_25G):
            if (cur_port_config_ptr->port_25g_en[0] == PORT_ON )  {
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE0);
            } else if (cur_port_config_ptr->port_25g_en[1] == PORT_ON )  {
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE1);
            } else if (cur_port_config_ptr->port_25g_en[2] == PORT_ON )  {
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE2);
            } else if (cur_port_config_ptr->port_25g_en[3] == PORT_ON )  {
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE3);
            } else if (cur_port_config_ptr->port_25g_en[4] == PORT_ON )  {
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE4);
            } else if (cur_port_config_ptr->port_25g_en[5] == PORT_ON )  {
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE5);
            } else if (cur_port_config_ptr->port_25g_en[6] == PORT_ON )  {
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE6);
            } else if (cur_port_config_ptr->port_25g_en[7] == PORT_ON )  {
                cw_rtmr_fec_enc_datamux_config_slice(phy_info_ptr, datamux_sel, SLICE7);
            }
            break;
    }
    return RR_SUCCESS;
}

// checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_kxky_gbox_speed_cfg(phy_info_t* phy_info_ptr,   cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr)
{
    (void)cur_mode_parameter_ptr;
    if(cur_port_config_ptr->port_100g_en[0] == PORT_ON )
        hsip_wr_field_(phy_info_ptr, KX2KY_GBOX_100G0_50G01_CTRL0_REG, MD_KXKY_EN_100G_CH01, 0x1);
    else if ( cur_port_config_ptr->port_100g_en[1] == PORT_ON )
        hsip_wr_field_(phy_info_ptr, KX2KY_GBOX_100G1_50G23_CTRL0_REG, MD_KXKY_EN_100G_CH23, 0x1);
    else if ( cur_port_config_ptr->port_100g_en[2] == PORT_ON )
        hsip_wr_field_(phy_info_ptr, KX2KY_GBOX_100G2_50G45_CTRL0_REG, MD_KXKY_EN_100G_CH45, 0x1);
    else if ( cur_port_config_ptr->port_100g_en[3] == PORT_ON )
        hsip_wr_field_(phy_info_ptr, KX2KY_GBOX_100G3_50G67_CTRL0_REG, MD_KXKY_EN_100G_CH67, 0x1);
    else if ( cur_port_config_ptr->port_50g_en[0] == PORT_ON  ||cur_port_config_ptr->port_50g_en[1] == PORT_ON )
        hsip_wr_field_(phy_info_ptr, KX2KY_GBOX_100G0_50G01_CTRL0_REG, MD_KXKY_EN_100G_CH01, 0x0);
    else if ( cur_port_config_ptr->port_50g_en[2] == PORT_ON || cur_port_config_ptr->port_50g_en[3] == PORT_ON )
        hsip_wr_field_(phy_info_ptr, KX2KY_GBOX_100G1_50G23_CTRL0_REG, MD_KXKY_EN_100G_CH23, 0x0);
    else if ( cur_port_config_ptr->port_50g_en[4] == PORT_ON || cur_port_config_ptr->port_50g_en[5] == PORT_ON )
        hsip_wr_field_(phy_info_ptr, KX2KY_GBOX_100G2_50G45_CTRL0_REG, MD_KXKY_EN_100G_CH45, 0x0);
    else if ( cur_port_config_ptr->port_50g_en[6] == PORT_ON || cur_port_config_ptr->port_50g_en[7] == PORT_ON)
        hsip_wr_field_(phy_info_ptr, KX2KY_GBOX_100G3_50G67_CTRL0_REG, MD_KXKY_EN_100G_CH67, 0x0);

    return RR_SUCCESS;
}

// checked RETIMER_IGR or RETIMER_EGR
// MD_KXKY_EN_KR2KP = 0 for RS544-to-RS528
// MD_KXKY_EN_KR2KP = 1 for RS528-to-RS544
return_result_t chal_cw_rtmr_kxky_gbox_fec_cfg(phy_info_t* phy_info_ptr,   cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, uint8_t kr2kp_mode)
{
    (void)cur_mode_parameter_ptr;
    if(cur_port_config_ptr->port_100g_en[0] == PORT_ON )
        hsip_wr_field_(phy_info_ptr, KX2KY_GBOX_100G0_50G01_CTRL0_REG, MD_KXKY_EN_KR2KP_CH01, kr2kp_mode);
    else if ( cur_port_config_ptr->port_100g_en[1] == PORT_ON )
        hsip_wr_field_(phy_info_ptr, KX2KY_GBOX_100G1_50G23_CTRL0_REG, MD_KXKY_EN_KR2KP_CH23, kr2kp_mode);
    else if ( cur_port_config_ptr->port_100g_en[2] == PORT_ON )
        hsip_wr_field_(phy_info_ptr, KX2KY_GBOX_100G2_50G45_CTRL0_REG, MD_KXKY_EN_KR2KP_CH45, kr2kp_mode);
    else if ( cur_port_config_ptr->port_100g_en[3] == PORT_ON )
        hsip_wr_field_(phy_info_ptr, KX2KY_GBOX_100G3_50G67_CTRL0_REG, MD_KXKY_EN_KR2KP_CH67, kr2kp_mode);
    else if ( cur_port_config_ptr->port_50g_en[0] == PORT_ON  ||cur_port_config_ptr->port_50g_en[1] == PORT_ON )
        hsip_wr_field_(phy_info_ptr, KX2KY_GBOX_100G0_50G01_CTRL0_REG, MD_KXKY_EN_KR2KP_CH01, kr2kp_mode);
    else if ( cur_port_config_ptr->port_50g_en[2] == PORT_ON || cur_port_config_ptr->port_50g_en[3] == PORT_ON )
        hsip_wr_field_(phy_info_ptr, KX2KY_GBOX_100G1_50G23_CTRL0_REG, MD_KXKY_EN_KR2KP_CH23, kr2kp_mode);
    else if ( cur_port_config_ptr->port_50g_en[4] == PORT_ON || cur_port_config_ptr->port_50g_en[5] == PORT_ON )
        hsip_wr_field_(phy_info_ptr, KX2KY_GBOX_100G2_50G45_CTRL0_REG, MD_KXKY_EN_KR2KP_CH45, kr2kp_mode);
    else if ( cur_port_config_ptr->port_50g_en[6] == PORT_ON || cur_port_config_ptr->port_50g_en[7] == PORT_ON)
        hsip_wr_field_(phy_info_ptr, KX2KY_GBOX_100G3_50G67_CTRL0_REG, MD_KXKY_EN_KR2KP_CH67, kr2kp_mode);

    return RR_SUCCESS;
}

// Checked RETIMER_IGR or RETIMER_EGR
return_result_t cw_rtmr_fec_rcv_datamux_config_slice(phy_info_t* phy_info_ptr, cfg_fec_rcv_datamux_t cur_fec_rcv_datamux, cfg_rtm_slice_t cur_rtm_slice)
{
    switch(cur_rtm_slice) {
        case(SLICE0):
            hsip_wr_field_(phy_info_ptr, FEC_DATA_MUX_SEL0_REG, MD_FEC_DEC_DATA_MUX0, cur_fec_rcv_datamux);        
            break;
        case(SLICE1):
            hsip_wr_field_(phy_info_ptr, FEC_DATA_MUX_SEL1_REG, MD_FEC_DEC_DATA_MUX1, cur_fec_rcv_datamux);   
            break;
        case(SLICE2):
            hsip_wr_field_(phy_info_ptr, FEC_DATA_MUX_SEL2_REG, MD_FEC_DEC_DATA_MUX2, cur_fec_rcv_datamux);        
            break;
        case(SLICE3):
            hsip_wr_field_(phy_info_ptr, FEC_DATA_MUX_SEL3_REG, MD_FEC_DEC_DATA_MUX3, cur_fec_rcv_datamux);   
            break;
        case(SLICE4):
            hsip_wr_field_(phy_info_ptr, FEC_DATA_MUX_SEL4_REG, MD_FEC_DEC_DATA_MUX4, cur_fec_rcv_datamux);        
            break;
        case(SLICE5):
            hsip_wr_field_(phy_info_ptr, FEC_DATA_MUX_SEL5_REG, MD_FEC_DEC_DATA_MUX5, cur_fec_rcv_datamux);   
            break;
        case(SLICE6):
            hsip_wr_field_(phy_info_ptr, FEC_DATA_MUX_SEL6_REG, MD_FEC_DEC_DATA_MUX6, cur_fec_rcv_datamux);        
            break; 
        case(SLICE7):
            hsip_wr_field_(phy_info_ptr, FEC_DATA_MUX_SEL7_REG, MD_FEC_DEC_DATA_MUX7, cur_fec_rcv_datamux);   
            break; 
    }
    return RR_SUCCESS;
}

// Checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_fec_rcv_datamux_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_fec_rcv_datamux_t fec_rcv_datamux)
{
    // 1 = kpkr , 2 = rsfec dec output 
    switch (cur_mode_parameter_ptr->speed) {     
        case (SPEED_400G): 
            if(cur_port_config_ptr->port_400g_en == PORT_ON )  {
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE0);
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE1);
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE2);
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE3);
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE4);
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE5);
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE6);
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE7);
            } 
            break;
        case (SPEED_200G):
            if(cur_port_config_ptr->port_200g_en[0] == PORT_ON )  {
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE0);
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE1);
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE2);
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE3);
            } else if (cur_port_config_ptr->port_200g_en[1] == PORT_ON )  {
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE4);
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE5);
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE6);
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE7);
            }
            break;
        case(SPEED_100G):
            if(cur_port_config_ptr->port_100g_en[0] == PORT_ON )  {
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE0);
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE1);
            } else if(cur_port_config_ptr->port_100g_en[1] == PORT_ON )  {
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE2);
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE3);
            } else if(cur_port_config_ptr->port_100g_en[2] == PORT_ON )  {
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE4);
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE5);
            } else if(cur_port_config_ptr->port_100g_en[3] == PORT_ON )  {
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE6);
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE7);
            }
            break;
        case(SPEED_50G):
            if(cur_port_config_ptr->port_50g_en[0] == PORT_ON )  {
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE0);
            } else if(cur_port_config_ptr->port_50g_en[1] == PORT_ON )  {
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE1);
            } else if(cur_port_config_ptr->port_50g_en[2] == PORT_ON )  {
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE2);
            } else if(cur_port_config_ptr->port_50g_en[3] == PORT_ON )  {
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE3);
            } else if(cur_port_config_ptr->port_50g_en[4] == PORT_ON )  {
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE4);
            } else if(cur_port_config_ptr->port_50g_en[5] == PORT_ON )  {
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE5);
            } else if(cur_port_config_ptr->port_50g_en[6] == PORT_ON )  {
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE6);
            } else if(cur_port_config_ptr->port_50g_en[7] == PORT_ON )  {
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE7);
            }
            break;
        case(SPEED_25G):
            if(cur_port_config_ptr->port_25g_en[0] == PORT_ON )  {
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE0);
            } else if(cur_port_config_ptr->port_25g_en[1] == PORT_ON )  {
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE1);
            } else if(cur_port_config_ptr->port_25g_en[2] == PORT_ON )  {
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE2);
            } else if(cur_port_config_ptr->port_25g_en[3] == PORT_ON )  {
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE3);
            } else if(cur_port_config_ptr->port_25g_en[4] == PORT_ON )  {
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE4);
            } else if(cur_port_config_ptr->port_25g_en[5] == PORT_ON )  {
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE5);
            } else if(cur_port_config_ptr->port_25g_en[6] == PORT_ON )  {
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE6);
            } else if(cur_port_config_ptr->port_25g_en[7] == PORT_ON )  {
                cw_rtmr_fec_rcv_datamux_config_slice(phy_info_ptr, fec_rcv_datamux, SLICE7);
            }
            break;
    }
    return RR_SUCCESS;
}

// checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_xenc_datamux_cfg(phy_info_t* phy_info_ptr,   cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, uint8_t dsel_pcs_xdecb)
{ 
    switch (cur_mode_parameter_ptr->speed) {
        case SPEED_25G:
            if (cur_port_config_ptr->port_25g_en[0] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL0_REG, MD_PCS_XDEC_DATA_MUX0, dsel_pcs_xdecb);
            } else if (cur_port_config_ptr->port_25g_en[1] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL1_REG, MD_PCS_XDEC_DATA_MUX1, dsel_pcs_xdecb);
            } else if (cur_port_config_ptr->port_25g_en[2] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL2_REG, MD_PCS_XDEC_DATA_MUX2, dsel_pcs_xdecb);
            } else if (cur_port_config_ptr->port_25g_en[3] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL3_REG, MD_PCS_XDEC_DATA_MUX3, dsel_pcs_xdecb);
            } else if (cur_port_config_ptr->port_25g_en[4] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL4_REG, MD_PCS_XDEC_DATA_MUX4, dsel_pcs_xdecb);
            } else if (cur_port_config_ptr->port_25g_en[5] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL5_REG, MD_PCS_XDEC_DATA_MUX5, dsel_pcs_xdecb);
            } else if (cur_port_config_ptr->port_25g_en[6] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL6_REG, MD_PCS_XDEC_DATA_MUX6, dsel_pcs_xdecb);
            } else if (cur_port_config_ptr->port_25g_en[7] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL7_REG, MD_PCS_XDEC_DATA_MUX7, dsel_pcs_xdecb);
            } 
            break;
        case SPEED_50G:
            if (cur_port_config_ptr->port_50g_en[0] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL0_REG, MD_PCS_XDEC_DATA_MUX0, dsel_pcs_xdecb);
            } else if (cur_port_config_ptr->port_50g_en[1] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL1_REG, MD_PCS_XDEC_DATA_MUX1, dsel_pcs_xdecb);
            } else if (cur_port_config_ptr->port_50g_en[2] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL2_REG, MD_PCS_XDEC_DATA_MUX2, dsel_pcs_xdecb);
            } else if (cur_port_config_ptr->port_50g_en[3] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL3_REG, MD_PCS_XDEC_DATA_MUX3, dsel_pcs_xdecb);
            } else if (cur_port_config_ptr->port_50g_en[4] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL4_REG, MD_PCS_XDEC_DATA_MUX4, dsel_pcs_xdecb);
            } else if (cur_port_config_ptr->port_50g_en[5] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL5_REG, MD_PCS_XDEC_DATA_MUX5, dsel_pcs_xdecb);
            } else if (cur_port_config_ptr->port_50g_en[6] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL6_REG, MD_PCS_XDEC_DATA_MUX6, dsel_pcs_xdecb);
            } else if (cur_port_config_ptr->port_50g_en[7] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL7_REG, MD_PCS_XDEC_DATA_MUX7, dsel_pcs_xdecb);
            } 
            break;
        case SPEED_100G:
            if(cur_port_config_ptr->port_100g_en[0] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL0_REG, MD_PCS_XDEC_DATA_MUX0, dsel_pcs_xdecb);
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL1_REG, MD_PCS_XDEC_DATA_MUX1, dsel_pcs_xdecb);
            } else if(cur_port_config_ptr->port_100g_en[1] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL2_REG, MD_PCS_XDEC_DATA_MUX2, dsel_pcs_xdecb);
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL3_REG, MD_PCS_XDEC_DATA_MUX3, dsel_pcs_xdecb);
            } else if (cur_port_config_ptr->port_100g_en[2] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL4_REG, MD_PCS_XDEC_DATA_MUX4, dsel_pcs_xdecb);
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL5_REG, MD_PCS_XDEC_DATA_MUX5, dsel_pcs_xdecb);
            } else if (cur_port_config_ptr->port_100g_en[3] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL6_REG, MD_PCS_XDEC_DATA_MUX6, dsel_pcs_xdecb);
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL7_REG, MD_PCS_XDEC_DATA_MUX7, dsel_pcs_xdecb);
            } 
            break;
        case SPEED_200G:
            if(cur_port_config_ptr->port_200g_en[0] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL0_REG, MD_PCS_XDEC_DATA_MUX0, dsel_pcs_xdecb);
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL1_REG, MD_PCS_XDEC_DATA_MUX1, dsel_pcs_xdecb);
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL2_REG, MD_PCS_XDEC_DATA_MUX2, dsel_pcs_xdecb);
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL3_REG, MD_PCS_XDEC_DATA_MUX3, dsel_pcs_xdecb);
            } else if (cur_port_config_ptr->port_200g_en[1] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL4_REG, MD_PCS_XDEC_DATA_MUX4, dsel_pcs_xdecb);
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL5_REG, MD_PCS_XDEC_DATA_MUX5, dsel_pcs_xdecb);
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL6_REG, MD_PCS_XDEC_DATA_MUX6, dsel_pcs_xdecb);
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL7_REG, MD_PCS_XDEC_DATA_MUX7, dsel_pcs_xdecb);
            } 
            break;
        case SPEED_400G:
            if(cur_port_config_ptr->port_400g_en == PORT_ON) {
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL0_REG, MD_PCS_XDEC_DATA_MUX0, dsel_pcs_xdecb);
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL1_REG, MD_PCS_XDEC_DATA_MUX1, dsel_pcs_xdecb);
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL2_REG, MD_PCS_XDEC_DATA_MUX2, dsel_pcs_xdecb);
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL3_REG, MD_PCS_XDEC_DATA_MUX3, dsel_pcs_xdecb);
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL4_REG, MD_PCS_XDEC_DATA_MUX4, dsel_pcs_xdecb);
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL5_REG, MD_PCS_XDEC_DATA_MUX5, dsel_pcs_xdecb);
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL6_REG, MD_PCS_XDEC_DATA_MUX6, dsel_pcs_xdecb);
                hsip_wr_field_(phy_info_ptr,FEC_DATA_MUX_SEL7_REG, MD_PCS_XDEC_DATA_MUX7, dsel_pcs_xdecb);
            } 
            break;
    }

    return RR_SUCCESS;
}

// checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_dp_mode_mux_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr)
{
    switch (cur_mode_parameter_ptr->speed) {
        case (SPEED_25G):
            if (cur_port_config_ptr->port_25g_en[0] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_400G,   0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_200G_0, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_100G_0, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_0,  0x0);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_0,  0x1);
            }
            else if (cur_port_config_ptr->port_25g_en[1] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_400G,   0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_200G_0, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_100G_0, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_1,  0x0);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_1,  0x1);
            }
            else if (cur_port_config_ptr->port_25g_en[2] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_400G,   0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_200G_0, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_100G_1, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_2,  0x0);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_2,  0x1);
            }
            else if (cur_port_config_ptr->port_25g_en[3] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_400G,   0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_200G_0, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_100G_1, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_3,  0x0);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_3,  0x1);
            }
            else if (cur_port_config_ptr->port_25g_en[4] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_400G,   0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_200G_1, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_100G_2, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_4,  0x0);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_4,  0x1);
            }
            else if (cur_port_config_ptr->port_25g_en[5] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_400G,   0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_200G_1, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_100G_2, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_5,  0x0);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_5,  0x1);
            }
            else if (cur_port_config_ptr->port_25g_en[6] == PORT_ON) {    
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_400G,   0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_200G_1, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_100G_3, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_6,  0x0);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_6,  0x1);
            }
            else if (cur_port_config_ptr->port_25g_en[7] == PORT_ON) {    
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_400G,   0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_200G_1, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_100G_3, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_7,  0x0);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_7,  0x1);
            }
            break;
        case (SPEED_50G):
            if (cur_port_config_ptr->port_50g_en[0] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_400G,   0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_200G_0, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_100G_0, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_0,  0x1);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_0,  0x0);
            }
            else if (cur_port_config_ptr->port_50g_en[1] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_400G,   0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_200G_0, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_100G_0, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_1,  0x1);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_1,  0x0);
            }
            else if (cur_port_config_ptr->port_50g_en[2] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_400G,   0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_200G_0, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_100G_1, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_2,  0x1);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_2,  0x0);
            }
            else if (cur_port_config_ptr->port_50g_en[3] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_400G,   0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_200G_0, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_100G_1, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_3,  0x1);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_3,  0x0);
            }
            else if (cur_port_config_ptr->port_50g_en[4] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_400G,   0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_200G_1, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_100G_2, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_4,  0x1);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_4,  0x0);
            }
            else if (cur_port_config_ptr->port_50g_en[5] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_400G,   0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_200G_1, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_100G_2, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_5,  0x1);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_5,  0x0);
            }
            else if (cur_port_config_ptr->port_50g_en[6] == PORT_ON) {    
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_400G,   0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_200G_1, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_100G_3, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_6,  0x1);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_6,  0x0);
            }
            else if (cur_port_config_ptr->port_50g_en[7] == PORT_ON) {    
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_400G,   0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_200G_1, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_100G_3, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_7,  0x1);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_7,  0x0);
            }
            break;
        case (SPEED_100G):
            if (cur_port_config_ptr->port_100g_en[0] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_400G,   0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_200G_0, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_100G_0, 0x1);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_0,  0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_1,  0x0);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_0,  0x0);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_1,  0x0);
            }
            else if (cur_port_config_ptr->port_100g_en[1] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_400G,   0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_200G_0, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_100G_1, 0x1);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_2,  0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_3,  0x0);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_2,  0x0);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_3,  0x0);
            }
            else if (cur_port_config_ptr->port_100g_en[2] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_400G,   0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_200G_1, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_100G_2, 0x1);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_4,  0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_5,  0x0);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_4,  0x0);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_5,  0x0);
            }
            else if (cur_port_config_ptr->port_100g_en[3] == PORT_ON) {    
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_400G,   0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_200G_1, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_100G_3, 0x1);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_6,  0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_7,  0x0);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_6,  0x0);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_7,  0x0);
            }
            break;

        case (SPEED_200G):
            if (cur_port_config_ptr->port_200g_en[0] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_400G,   0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_200G_0, 0x1);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_100G_0, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_100G_1, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_0,  0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_1,  0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_2,  0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_3,  0x0);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_0,  0x0);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_1,  0x0);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_2,  0x0);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_3,  0x0);
            }
            else if (cur_port_config_ptr->port_200g_en[1] == PORT_ON) {
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_400G,   0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_200G_1, 0x1);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_100G_2, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_100G_3, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_4,  0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_5,  0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_6,  0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_7,  0x0);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_4,  0x0);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_5,  0x0);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_6,  0x0);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_7,  0x0);
            }
            break;

        case (SPEED_400G):
            if (cur_port_config_ptr->port_400g_en == PORT_ON) {
#if 1
                hsip_wr_reg_(phy_info_ptr, TRAFFIC_SPEED_REG, 0x4000);
#else
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_400G,   0x1);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_200G_0, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_200G_1, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_100G_0, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_100G_1, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_100G_2, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_100G_3, 0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_0,  0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_1,  0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_2,  0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_3,  0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_4,  0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_5,  0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_6,  0x0);
                hsip_wr_field_(phy_info_ptr, TRAFFIC_SPEED_REG, MD_SPEED_50G_7,  0x0);
#endif
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_0,  0x0);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_1,  0x0);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_2,  0x0);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_3,  0x0);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_4,  0x0);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_5,  0x0);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_6,  0x0);
                hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG,   MD_SPEED_25G_7,  0x0);
            }
            break;
    } // end switch

    return RR_SUCCESS;
}

// checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_fec_mode_kr4_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t reg_val = 0;

    reg_val = hsip_rd_field_(phy_info_ptr, FEC_RX_MODE_REG, MD_FEC_MODE_KR4);

    if ((cur_mode_parameter_ptr->host_fec_type==CHIP_HOST_FEC_TYPE_RS528  && egr_or_igr == EGR ) | 
        (cur_mode_parameter_ptr->line_fec_type==CHIP_LINE_FEC_TYPE_RS528  && egr_or_igr == IGR )) {
           reg_val |= (cur_mode_parameter_ptr->fec_slice & 0xFF); 
           hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG, MD_FEC_MODE_KR4,  reg_val);
     }
     else  { 
           reg_val &= (cur_mode_parameter_ptr->fec_slice ^ 0xFF); 
           hsip_wr_field_(phy_info_ptr, FEC_RX_MODE_REG, MD_FEC_MODE_KR4,  reg_val);
     }

    reg_val = hsip_rd_field_(phy_info_ptr, FEC_PCS_TOP_TX_FEC_TX_FEC_MODE_REG, MD_FEC_TX_MODE_KR4);
    if ((cur_mode_parameter_ptr->host_fec_type==CHIP_HOST_FEC_TYPE_RS528  && egr_or_igr == IGR ) | 
        (cur_mode_parameter_ptr->line_fec_type==CHIP_LINE_FEC_TYPE_RS528  && egr_or_igr == EGR )) {

           reg_val |= (cur_mode_parameter_ptr->fec_slice & 0xFF); 
           hsip_wr_field_(phy_info_ptr, FEC_PCS_TOP_TX_FEC_TX_FEC_MODE_REG, MD_FEC_TX_MODE_KR4,  reg_val);
     }
     else  { 
           reg_val &= (cur_mode_parameter_ptr->fec_slice ^ 0xFF); 
           hsip_wr_field_(phy_info_ptr, FEC_PCS_TOP_TX_FEC_TX_FEC_MODE_REG, MD_FEC_TX_MODE_KR4,  reg_val);
     }

   return RR_SUCCESS;
}

// Checked RSFEC_ENC_TOP 0,1,2,3,4,5,6,7 RTMR_RSFEC_ENC_TOP_IGR_0 or RTMR_RSFEC_ENC_TOP_EGR_0
// [HSIP] Removed egr_or_igr 
//phy_info_ptr: block base address RTMR_RSFEC_ENC_TOP_IGR_0 or RTMR_RSFEC_ENC_TOP_EGR_0
return_result_t chal_cw_rtmr_fec_enc_mux_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, uint8_t enc_bypass_mode)
{
    uint8_t gap=0, gap_start=0, gap_num=0;
    phy_info_t chal_phy={0};
    util_memcpy((void *)&chal_phy, phy_info_ptr, sizeof(phy_info_t));
    switch (cur_mode_parameter_ptr->speed) {
        case(SPEED_25G): 
            if(cur_port_config_ptr->port_25g_en[0] == PORT_ON) {
                gap_start = 0;
            } else if(cur_port_config_ptr->port_25g_en[1] == PORT_ON) {
                gap_start = 1;
            } else if(cur_port_config_ptr->port_25g_en[2] == PORT_ON) { 
                gap_start = 2;
            } else if(cur_port_config_ptr->port_25g_en[3] == PORT_ON) { 
                gap_start = 3;
            } else if(cur_port_config_ptr->port_25g_en[4] == PORT_ON) { 
                gap_start = 4;
            } else if(cur_port_config_ptr->port_25g_en[5] == PORT_ON) { 
                gap_start = 5;
            } else if(cur_port_config_ptr->port_25g_en[6] == PORT_ON) { 
                gap_start = 6;
            } else if(cur_port_config_ptr->port_25g_en[7] == PORT_ON) { 
                gap_start = 7;
            }
            gap_num = 1;
            break;
        case(SPEED_50G): 
            if(cur_port_config_ptr->port_50g_en[0] == PORT_ON) {
                gap_start = 0;
            } else if(cur_port_config_ptr->port_50g_en[1] == PORT_ON) {
                gap_start = 1;
            } else if(cur_port_config_ptr->port_50g_en[2] == PORT_ON) { 
                gap_start = 2;
            } else if(cur_port_config_ptr->port_50g_en[3] == PORT_ON) { 
                gap_start = 3;
            } else if(cur_port_config_ptr->port_50g_en[4] == PORT_ON) { 
                gap_start = 4;
            } else if(cur_port_config_ptr->port_50g_en[5] == PORT_ON) { 
                gap_start = 5;
            } else if(cur_port_config_ptr->port_50g_en[6] == PORT_ON) { 
                gap_start = 6;
            } else if(cur_port_config_ptr->port_50g_en[7] == PORT_ON) { 
                gap_start = 7;
            }
            gap_num = 1;
            break;
        case(SPEED_100G): 
            if(cur_port_config_ptr->port_100g_en[0] == PORT_ON) {
                gap_start = 0;
            } else if(cur_port_config_ptr->port_100g_en[1] == PORT_ON) { 
                gap_start = 2;
            } else if(cur_port_config_ptr->port_100g_en[2] == PORT_ON) { 
                gap_start = 4;
            } else if(cur_port_config_ptr->port_100g_en[3] == PORT_ON) { 
                gap_start = 6;
            }
            gap_num = 2;
            break;
        case(SPEED_200G):
            if(cur_port_config_ptr->port_200g_en[0] == PORT_ON) {
                gap_start = 0;
            } else if(cur_port_config_ptr->port_200g_en[1] == PORT_ON) { 
                gap_start = 4;
            }
            gap_num = 4;
            break;
        case(SPEED_400G):
            if(cur_port_config_ptr->port_400g_en == PORT_ON) {
                gap_start = 0;
                gap_num = 8;
            }
            break;
    }
    for(gap=gap_start; gap<(gap_start+gap_num); gap++){
        chal_phy.base_addr = phy_info_ptr->base_addr + RTMR_RSFEC_ENC_TOP_GAP*gap;
        hsip_wr_field_(&chal_phy, MODE_CONTROL_REGISTER_ENCODER, MD_ENC_BYPASS, enc_bypass_mode); 
    }
    return RR_SUCCESS;
}

// Checked RSFEC_ENC_TOP 0,1,2,3,4,5,6,7 RTMR_RSFEC_ENC_TOP_IGR_0 or RTMR_RSFEC_ENC_TOP_EGR_0
// [HSIP] Removed parameter egr_or_igr
//phy_info_ptr: block base address RTMR_RSFEC_ENC_TOP_IGR_0 or RTMR_RSFEC_ENC_TOP_EGR_0
return_result_t chal_cw_rtmr_fec_enc_pass_thru_uncorr_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, uint8_t enc_pass_thru_uncorr)
{
    uint8_t gap=0, gap_start=0, gap_num=0;
    phy_info_t chal_phy={0};
    util_memcpy((void *)&chal_phy, phy_info_ptr, sizeof(phy_info_t));
    switch (cur_mode_parameter_ptr->speed) {
        case(SPEED_25G): 
            if(cur_port_config_ptr->port_25g_en[0] == PORT_ON) {
                gap_start = 0;
            } else if(cur_port_config_ptr->port_25g_en[1] == PORT_ON) {
                gap_start = 1;
            } else if(cur_port_config_ptr->port_25g_en[2] == PORT_ON) {
                gap_start = 2; 
            } else if(cur_port_config_ptr->port_25g_en[3] == PORT_ON) { 
                gap_start = 3;
            } else if(cur_port_config_ptr->port_25g_en[4] == PORT_ON) { 
                gap_start = 4;
            } else if(cur_port_config_ptr->port_25g_en[5] == PORT_ON) { 
                gap_start = 5;
            } else if(cur_port_config_ptr->port_25g_en[6] == PORT_ON) { 
                gap_start = 6;
            } else if(cur_port_config_ptr->port_25g_en[7] == PORT_ON) { 
                gap_start = 7;
            }
            gap_num = 1;
            break;
        case(SPEED_50G): 
            if(cur_port_config_ptr->port_50g_en[0] == PORT_ON) {
                gap_start = 0;
            } else if(cur_port_config_ptr->port_50g_en[1] == PORT_ON) {
                gap_start = 1;
            } else if(cur_port_config_ptr->port_50g_en[2] == PORT_ON) { 
                gap_start = 2;
            } else if(cur_port_config_ptr->port_50g_en[3] == PORT_ON) { 
                gap_start = 3;
            } else if(cur_port_config_ptr->port_50g_en[4] == PORT_ON) { 
                gap_start = 4;
            } else if(cur_port_config_ptr->port_50g_en[5] == PORT_ON) { 
                gap_start = 5;
            } else if(cur_port_config_ptr->port_50g_en[6] == PORT_ON) { 
                gap_start = 6;
            } else if(cur_port_config_ptr->port_50g_en[7] == PORT_ON) { 
                gap_start = 7;
            }
            gap_num = 1;
            break;
        case(SPEED_100G): 
            if(cur_port_config_ptr->port_100g_en[0] == PORT_ON) {
                gap_start = 0;
                gap_num = 2;
            } else if(cur_port_config_ptr->port_100g_en[1] == PORT_ON) { 
                gap_start = 2;
                gap_num = 2;
            } else if(cur_port_config_ptr->port_100g_en[2] == PORT_ON) { 
                gap_start = 4;
                gap_num = 2;
            } else if(cur_port_config_ptr->port_100g_en[3] == PORT_ON) { 
                gap_start = 6;
                gap_num = 2;
            }
            break;
        case(SPEED_200G):
            if(cur_port_config_ptr->port_200g_en[0] == PORT_ON) {
                gap_start = 0;
                gap_num = 4;
            } else if(cur_port_config_ptr->port_200g_en[1] == PORT_ON) { 
                gap_start = 4;
                gap_num = 4;
            }
            break;
        case(SPEED_400G):
            if(cur_port_config_ptr->port_400g_en == PORT_ON) {
                gap_start = 0;
                gap_num = 8;
            }
            break;
    }
    for(gap=gap_start; gap<(gap_start+gap_num); gap++){
        chal_phy.base_addr = phy_info_ptr->base_addr + RTMR_RSFEC_ENC_TOP_GAP*gap;
        hsip_wr_field_(&chal_phy, MODE_CONTROL_REGISTER_ENCODER, MD_ENC_PASS_THRU_UNCORR, enc_pass_thru_uncorr); 
    }
    return RR_SUCCESS;
}

//Checked RSFEC_DEC_TOP 0,1,2,3,4,5,6,7  RTMR_RSFEC_DEC_TOP_IGR_0 or RTMR_RSFEC_DEC_TOP_EGR_0
// [HSIP] Removed parameter egr_or_igr
//phy_info_ptr: block base address RTMR_RSFEC_ENC_TOP_IGR_0 or RTMR_RSFEC_ENC_TOP_EGR_0
return_result_t chal_cw_rtmr_fecdec_data_gating_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, uint8_t din_off)
{
    uint8_t gap=0, gap_start=0, gap_num=0;
    phy_info_t chal_phy={0};
    util_memcpy((void *)&chal_phy, phy_info_ptr, sizeof(phy_info_t));
    // Configure RS-FEC data path input gating
    // disable fec decoder datain when bitmux_symbol_mux mode or host is pcs; otherwise, enable datain
    switch (cur_mode_parameter_ptr->speed) {
        case(SPEED_25G): 
            if(cur_port_config_ptr->port_25g_en[0] == PORT_ON) {
                gap_start = 0;
            } else if(cur_port_config_ptr->port_25g_en[1] == PORT_ON) {
                gap_start = 1;
            } else if(cur_port_config_ptr->port_25g_en[2] == PORT_ON) { 
                gap_start = 2;
            } else if(cur_port_config_ptr->port_25g_en[3] == PORT_ON) { 
                gap_start = 3;
            } else if(cur_port_config_ptr->port_25g_en[4] == PORT_ON) { 
                gap_start = 4;
            } else if(cur_port_config_ptr->port_25g_en[5] == PORT_ON) { 
                gap_start = 5;
            } else if(cur_port_config_ptr->port_25g_en[6] == PORT_ON) { 
                gap_start = 6;
            } else if(cur_port_config_ptr->port_25g_en[7] == PORT_ON) { 
                gap_start = 7;
            }
            gap_num = 1;
            break;
        case(SPEED_50G): 
            if(cur_port_config_ptr->port_50g_en[0] == PORT_ON) {
                gap_start = 0;
            } else if(cur_port_config_ptr->port_50g_en[1] == PORT_ON) {
                gap_start = 1;
            } else if(cur_port_config_ptr->port_50g_en[2] == PORT_ON) { 
                gap_start = 2;
            } else if(cur_port_config_ptr->port_50g_en[3] == PORT_ON) { 
                gap_start = 3;
            } else if(cur_port_config_ptr->port_50g_en[4] == PORT_ON) { 
                gap_start = 4;
            } else if(cur_port_config_ptr->port_50g_en[5] == PORT_ON) { 
                gap_start = 5;
            } else if(cur_port_config_ptr->port_50g_en[6] == PORT_ON) { 
                gap_start = 6;
            } else if(cur_port_config_ptr->port_50g_en[7] == PORT_ON) { 
                gap_start = 7;
            }
            gap_num = 1;
            break;
        case(SPEED_100G): 
            if(cur_port_config_ptr->port_100g_en[0] == PORT_ON) {
                gap_start = 0;
                gap_num = 2;
            } else if(cur_port_config_ptr->port_100g_en[1] == PORT_ON) { 
                gap_start = 2;
                gap_num = 2;
            } else if(cur_port_config_ptr->port_100g_en[2] == PORT_ON) { 
                gap_start = 4;
                gap_num = 2;
            } else if(cur_port_config_ptr->port_100g_en[3] == PORT_ON) {
                gap_start = 6;
                gap_num = 2;
            }
            break;
        case(SPEED_200G):
            if(cur_port_config_ptr->port_200g_en[0] == PORT_ON) {
                gap_start = 0;
                gap_num = 4;
            } else if(cur_port_config_ptr->port_200g_en[1] == PORT_ON) { 
                gap_start = 4;
                gap_num = 4;
            }
            break;
        case(SPEED_400G):
            if(cur_port_config_ptr->port_400g_en == PORT_ON) {
                gap_start = 0;
                gap_num = 8;
            }
            break;
    }
    for(gap=gap_start; gap<(gap_start+gap_num); gap++){
        chal_phy.base_addr = phy_info_ptr->base_addr + RTMR_RSFEC_DEC_TOP_GAP*gap;
        hsip_wr_field_(&chal_phy, MODE_CONTROL_REGISTER, MD_DEC_DIN_OFF, din_off); 
    }

    return RR_SUCCESS;
}

//Checked RSFEC_ENC_TOP 0,1,2,3,4,5,6,7 RTMR_RSFEC_ENC_TOP_IGR_0 or RTMR_RSFEC_ENC_TOP_EGR_0
//[HSIP] Add 25G and 50G
//[HSIP] Removed parameter egr_or_igr
return_result_t chal_cw_rtmr_fecenc_data_gating_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, uint8_t din_off)
{
    uint8_t gap=0, gap_start=0, gap_num=0;
    phy_info_t chal_phy={0};
    util_memcpy((void *)&chal_phy, phy_info_ptr, sizeof(phy_info_t));
    // disable encoder din when in bitmux_symbol_mux mode and fec_dec_fwd mode. Otherwise, enable 
    switch (cur_mode_parameter_ptr->speed) {
        case(SPEED_25G): 
            if(cur_port_config_ptr->port_25g_en[0] == PORT_ON) {
                gap_start = 0;
            } else if(cur_port_config_ptr->port_25g_en[1] == PORT_ON) {
                gap_start = 1;
            } else if(cur_port_config_ptr->port_25g_en[2] == PORT_ON) { 
                gap_start = 2;
            } else if(cur_port_config_ptr->port_25g_en[3] == PORT_ON) { 
                gap_start = 3;
            } else if(cur_port_config_ptr->port_25g_en[4] == PORT_ON) { 
                gap_start = 4;
            } else if(cur_port_config_ptr->port_25g_en[5] == PORT_ON) { 
                gap_start = 5;
            } else if(cur_port_config_ptr->port_25g_en[6] == PORT_ON) { 
                gap_start = 6;
            } else if(cur_port_config_ptr->port_25g_en[7] == PORT_ON) { 
                gap_start = 7;
            }
            gap_num = 1;
            break;
        case(SPEED_50G): 
            if(cur_port_config_ptr->port_50g_en[0] == PORT_ON) {
                gap_start = 0;
            } else if(cur_port_config_ptr->port_50g_en[1] == PORT_ON) {
                gap_start = 1;
            } else if(cur_port_config_ptr->port_50g_en[2] == PORT_ON) { 
                gap_start = 2;
            } else if(cur_port_config_ptr->port_50g_en[3] == PORT_ON) { 
                gap_start = 3;
            } else if(cur_port_config_ptr->port_50g_en[4] == PORT_ON) { 
                gap_start = 4;
            } else if(cur_port_config_ptr->port_50g_en[5] == PORT_ON) { 
                gap_start = 5;
            } else if(cur_port_config_ptr->port_50g_en[6] == PORT_ON) { 
                gap_start = 6;
            } else if(cur_port_config_ptr->port_50g_en[7] == PORT_ON) { 
                gap_start = 7;
            }
            gap_num = 1;
            break;
        case(SPEED_100G): 
            if(cur_port_config_ptr->port_100g_en[0] == PORT_ON) {
                gap_start = 0;
                gap_num = 2;
            } else if(cur_port_config_ptr->port_100g_en[1] == PORT_ON) { 
                gap_start = 2;
                gap_num = 2;
            } else if(cur_port_config_ptr->port_100g_en[2] == PORT_ON) { 
                gap_start = 4;
                gap_num = 2;
            } else if(cur_port_config_ptr->port_100g_en[3] == PORT_ON) { 
                gap_start = 6;
                gap_num = 2;
            }
            break;
        case(SPEED_200G):
            if(cur_port_config_ptr->port_200g_en[0] == PORT_ON) {
                gap_start = 0;
                gap_num = 4;
            } else if(cur_port_config_ptr->port_200g_en[1] == PORT_ON) { 
                gap_start = 4;
                gap_num = 4;
            }
            break;
        case(SPEED_400G):
            if(cur_port_config_ptr->port_400g_en == PORT_ON) { 
                gap_start = 0;
                gap_num = 8;
            }
            break;
    }
    for(gap=gap_start; gap<(gap_start+gap_num); gap++){
        chal_phy.base_addr = phy_info_ptr->base_addr + RTMR_RSFEC_ENC_TOP_GAP*gap;
        hsip_wr_field_(&chal_phy,  MODE_CONTROL_REGISTER_ENCODER, MD_ENC_DIN_OFF, din_off); 
    }

    return RR_SUCCESS;
}

return_result_t chal_cw_rtmr_fec_deskew_reorder_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr)
{
    (void)egr_or_igr;
    uint16_t wmask = 0, wdata = 0;
    uint16_t wmask1 = 0, wdata1 = 0;
    uint16_t wmask2 = 0, wdata2 = 0;
    uint16_t lane=0;

    for (lane=0; lane<8; lane++) {
        if (cur_mode_parameter_ptr->fec_slice & (1 << lane)) 
            wmask1 |= (0x3 << (lane<<1));
    }
    wmask2 = (cur_mode_parameter_ptr->fec_slice);
    wmask = (cur_mode_parameter_ptr->fec_slice) << 8;

    switch (cur_mode_parameter_ptr->speed) {
        case SPEED_25G:
            wdata = wmask;
            wdata1 = 0;
            wdata2 = wmask2;
            break;
        default:
            wdata = 0;
            wdata1 = wmask1;
            wdata2 = 0;
            break;

    }
    hsip_wr_fields(phy_info_ptr, BYPASS_DEINT_REG, wmask, wdata);       /* BYPASS_REORDER */
    hsip_wr_fields(phy_info_ptr, AM_DEBUG0_REG, wmask1, wdata1);        /* REPEAT_AM_CHECK_EN */
    hsip_wr_fields(phy_info_ptr, DISABLE_SKEW_MEM_REG, wmask2, wdata2); /* DISABLE_SKEW_MEM */
    hsip_wr_fields(phy_info_ptr, DESKEW_CTRL_REG, wmask2, wdata2);      /* DISABLE_DESKEW_FSM */

    return RR_SUCCESS;
}

// Checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_predec_bypass_deint_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, uint8_t bypass_fec_deint)
{
    uint16_t wmask = 0, wdata = 0;
    wmask = cur_mode_parameter_ptr->fec_slice;

    if (bypass_fec_deint)
        wdata = wmask;
    else 
        wdata = 0;

    hsip_wr_fields(phy_info_ptr, BYPASS_DEINT_REG, wmask, wdata);

    return RR_SUCCESS;
}

return_result_t chal_cw_rtmr_ovr_am_cfg (phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr)
{
    uint16_t reg_val = hsip_rd_field_(phy_info_ptr, PCS_AM_DEBUG0_REG, OVR_AM_DIST_EN);

    if((cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) ||
        (cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC)){
        switch (cur_mode_parameter_ptr->speed) {
            case(SPEED_50G): 
              if(cur_port_config_ptr->port_50g_en[0] == PORT_ON) {
                  hsip_wr_field_(phy_info_ptr, PCS_AM_DEBUG2_REG, OVR_AM_DIST_0, 0x4FFF);  
                  hsip_wr_field_(phy_info_ptr, PCS_AM_DEBUG0_REG, OVR_AM_DIST_EN, reg_val|0x1);
              }else if(cur_port_config_ptr->port_50g_en[2] == PORT_ON) {
                  hsip_wr_field_(phy_info_ptr, PCS_AM_DEBUG3_REG, OVR_AM_DIST_1, 0x4FFF);  
                  hsip_wr_field_(phy_info_ptr, PCS_AM_DEBUG0_REG, OVR_AM_DIST_EN, reg_val|0x2);
              }else if(cur_port_config_ptr->port_50g_en[4] == PORT_ON) {
                  hsip_wr_field_(phy_info_ptr, PCS_AM_DEBUG4_REG, OVR_AM_DIST_2, 0x4FFF);  
                  hsip_wr_field_(phy_info_ptr, PCS_AM_DEBUG0_REG, OVR_AM_DIST_EN, reg_val|0x4);
              }else if(cur_port_config_ptr->port_50g_en[6] == PORT_ON) {
                  hsip_wr_field_(phy_info_ptr, PCS_AM_DEBUG5_REG, OVR_AM_DIST_3, 0x4FFF);  
                  hsip_wr_field_(phy_info_ptr, PCS_AM_DEBUG0_REG, OVR_AM_DIST_EN, reg_val|0x8);
              }
              break;
            case(SPEED_100G): 
              if(cur_port_config_ptr->port_100g_en[0] == PORT_ON) {  
                hsip_wr_field_(phy_info_ptr, PCS_AM_DEBUG0_REG, OVR_AM_DIST_EN, reg_val&0xC);
              }else if(cur_port_config_ptr->port_100g_en[2] == PORT_ON) {
                  hsip_wr_field_(phy_info_ptr, PCS_AM_DEBUG0_REG, OVR_AM_DIST_EN, reg_val&0x3);
              }
              break;
            default:
              hsip_wr_field_(phy_info_ptr, PCS_AM_DEBUG0_REG, OVR_AM_DIST_EN, 0x0);
            break;
        }
  }
  return RR_SUCCESS;
}

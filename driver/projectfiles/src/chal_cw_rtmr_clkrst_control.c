/**
 *
 * @file     chal_cw_rtmr_clkrst_control.c
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
#include "chip_mode_def.h"
#include "cw_def.h"
#include "ml_cw_rtmr_modes.h"
#include "chal_cw_rtmr_status_check.h"
#include "chal_cw_rtmr_clkrst_control.h"

extern uint8_t util_get_lowest_index_from_mask (uint16_t lane_mask);

//Checked TOP
return_result_t chal_cw_rtmr_fec_clk_gate_control_8p(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_host_or_line_t host_or_line, cfg_egr_or_igr_t egr_or_igr, enabled_t enable)
{
    (void)cur_port_config_ptr;
    // enable fec per lane clock in retimer
    uint16_t reg_val;
    uint16_t lane_mask = 0;
    lane_mask  = (host_or_line == HOST)  ? cur_mode_parameter_ptr->host_llane_mask : cur_mode_parameter_ptr->line_llane_mask;

    if (egr_or_igr == EGR) {
        if (host_or_line == HOST) { // HOST
            reg_val  = hsip_rd_field_(phy_info_ptr, RETIMER_CDR_CLK_CTRL_RDB, MD_RTM_CDR_CKEN);
            if(enable == IS_ENABLED)
                reg_val |= (lane_mask & 0xFF);
            else 
                reg_val &= (lane_mask ^ 0xFF);
            hsip_wr_field_(phy_info_ptr, RETIMER_CDR_CLK_CTRL_RDB, MD_RTM_CDR_CKEN, reg_val);  // cdr is host lane mask
        } else { // 
            reg_val  = hsip_rd_field_(phy_info_ptr, RETIMER_CMU_CLK_CTRL_RDB, MD_RTM_CMU_CKEN);
            if(enable == IS_ENABLED)
                reg_val |= (lane_mask & 0xFF);
            else 
                reg_val &= (lane_mask ^ 0xFF);
            hsip_wr_field_(phy_info_ptr, RETIMER_CMU_CLK_CTRL_RDB, MD_RTM_CMU_CKEN, reg_val);  // cmu is line lane mask
        }
    }
    else {
        if (host_or_line == LINE) { // LINE
            reg_val  = hsip_rd_field_(phy_info_ptr, RETIMER_CDR_CLK_CTRL_RDB, MD_RTM_CDR_CKEN);
            if(enable == IS_ENABLED)
                reg_val |= (lane_mask & 0xFF);
            else 
                reg_val &= (lane_mask ^ 0xFF);
            hsip_wr_field_(phy_info_ptr, RETIMER_CDR_CLK_CTRL_RDB, MD_RTM_CDR_CKEN, reg_val);  // cdr is host lane mask
        } else { // 
            reg_val  = hsip_rd_field_(phy_info_ptr, RETIMER_CMU_CLK_CTRL_RDB, MD_RTM_CMU_CKEN);
            if(enable == IS_ENABLED)
                reg_val |= (lane_mask & 0xFF);
            else 
                reg_val &= (lane_mask ^ 0xFF);
            hsip_wr_field_(phy_info_ptr, RETIMER_CMU_CLK_CTRL_RDB, MD_RTM_CMU_CKEN, reg_val);  // cmu is line lane mask
        }
    }
    return RR_SUCCESS;
}

//Checked TOP
return_result_t chal_cw_rtmr_fec_clk_gate_control_4p(phy_info_t* phy_info_ptr,  cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_host_or_line_t host_or_line, cfg_egr_or_igr_t egr_or_igr, enabled_t enable)
{
    // enable fec per lane clock in retimer
    uint16_t reg_val;
    uint16_t lane_mask = 0;
    cfg_pam_or_nrz_t pam_or_nrz = (egr_or_igr == EGR) ? cur_mode_parameter_ptr->host_pam_or_nrz_type : cur_mode_parameter_ptr->line_pam_or_nrz_type;
    
    lane_mask  = (host_or_line == HOST)  ? cur_mode_parameter_ptr->host_llane_mask : cur_mode_parameter_ptr->line_llane_mask;
    if(cur_mode_parameter_ptr->speed == SPEED_200G && pam_or_nrz == CW_PAM) {
        lane_mask = 0x55;
    } else if(cur_mode_parameter_ptr->speed == SPEED_100G && pam_or_nrz == CW_PAM) {
        if(cur_port_config_ptr->port_100g_en[0]) {
            lane_mask = 0x5;
        } else if(cur_port_config_ptr->port_100g_en[2]) {
            lane_mask = 0x50;
        }
    }  else if(cur_mode_parameter_ptr->speed == SPEED_50G && pam_or_nrz == CW_PAM) {
        if(cur_port_config_ptr->port_50g_en[0]) {
            lane_mask = 0x1;
        } else if(cur_port_config_ptr->port_50g_en[2]) {
            lane_mask = 0x4;
        } else if(cur_port_config_ptr->port_50g_en[4]) {
            lane_mask = 0x10;
        } else if(cur_port_config_ptr->port_50g_en[6]) {
            lane_mask = 0x40;
        }
    }

    if (egr_or_igr == EGR) {
        if (host_or_line == HOST) { // HOST
            reg_val  = hsip_rd_field_(phy_info_ptr, RETIMER_CDR_CLK_CTRL_RDB, MD_RTM_CDR_CKEN);
            if(enable == IS_ENABLED)
                reg_val |= (lane_mask & 0xFF);
            else 
                reg_val &= (lane_mask ^ 0xFF);
            hsip_wr_field_(phy_info_ptr, RETIMER_CDR_CLK_CTRL_RDB, MD_RTM_CDR_CKEN, reg_val);  // cdr is host lane mask
        } else { // 
            reg_val  = hsip_rd_field_(phy_info_ptr, RETIMER_CMU_CLK_CTRL_RDB, MD_RTM_CMU_CKEN);
            if(enable == IS_ENABLED)
                reg_val |= (lane_mask & 0xFF);
            else 
                reg_val &= (lane_mask ^ 0xFF);
            hsip_wr_field_(phy_info_ptr, RETIMER_CMU_CLK_CTRL_RDB, MD_RTM_CMU_CKEN, reg_val);  // cmu is line lane mask
        }
    }
    else {
        if (host_or_line == LINE) { // LINE
            reg_val  = hsip_rd_field_(phy_info_ptr, RETIMER_CDR_CLK_CTRL_RDB, MD_RTM_CDR_CKEN);
            if(enable == IS_ENABLED)
                reg_val |= (lane_mask & 0xFF);
            else 
                reg_val &= (lane_mask ^ 0xFF);
            hsip_wr_field_(phy_info_ptr, RETIMER_CDR_CLK_CTRL_RDB, MD_RTM_CDR_CKEN, reg_val);  // cdr is host lane mask
        } else { // 
            reg_val  = hsip_rd_field_(phy_info_ptr, RETIMER_CMU_CLK_CTRL_RDB, MD_RTM_CMU_CKEN);
            if(enable == IS_ENABLED)
                reg_val |= (lane_mask & 0xFF);
            else 
                reg_val &= (lane_mask ^ 0xFF);
            hsip_wr_field_(phy_info_ptr, RETIMER_CMU_CLK_CTRL_RDB, MD_RTM_CMU_CKEN, reg_val);  // cmu is line lane mask
        }
    }
    return RR_SUCCESS;
}

return_result_t chal_cw_rtmr_fec_clk_gate_control(phy_info_t* phy_info_ptr,  cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_host_or_line_t host_or_line, cfg_egr_or_igr_t egr_or_igr, enabled_t enable)
{
    if (ml_cw_rtmr_is_A1_4p_port(cur_mode_parameter_ptr)) {        
        return chal_cw_rtmr_fec_clk_gate_control_4p(phy_info_ptr, cur_mode_parameter_ptr, cur_port_config_ptr, host_or_line, egr_or_igr, enable);
    } else {
        return chal_cw_rtmr_fec_clk_gate_control_8p(phy_info_ptr, cur_mode_parameter_ptr, cur_port_config_ptr, host_or_line, egr_or_igr, enable);
    }
}

//Checked Top
//[HSIP] Change to support both enable and disable
return_result_t chal_cw_rtmr_pcs_clk_gate_control(phy_info_t* phy_info_ptr,  cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_host_or_line_t host_or_line, cfg_egr_or_igr_t egr_or_igr, enabled_t enable)
{
    // enable pcs per lane clock in retimer
    uint16_t reg_val;
    uint16_t lane_mask;
    uint16_t wmask;

    lane_mask  = (host_or_line == HOST)  ? cur_mode_parameter_ptr->host_llane_mask : cur_mode_parameter_ptr->line_llane_mask;
    wmask = (1 << (util_get_lowest_index_from_mask(lane_mask)));

    if (egr_or_igr == EGR) {	
        if (host_or_line == HOST) { // HOST
            reg_val  = hsip_rd_field_(phy_info_ptr, RETIMER_CDR_CLK_CTRL_RDB,MD_RTM_CDR_PCS_CKEN );
            if(enable == IS_ENABLED)
                reg_val |= (wmask & 0xFF);
            else 
                reg_val &= (wmask ^ 0xFF);
            hsip_wr_field_(phy_info_ptr, RETIMER_CDR_CLK_CTRL_RDB, MD_RTM_CDR_PCS_CKEN,reg_val);
        } else { // 
            reg_val  = hsip_rd_field_(phy_info_ptr, RETIMER_CMU_CLK_CTRL_RDB,MD_RTM_CMU_PCS_CKEN );
            if(enable == IS_ENABLED)
                reg_val |= (wmask & 0xFF);
            else 
                reg_val &= (wmask ^ 0xFF);
            hsip_wr_field_(phy_info_ptr, RETIMER_CMU_CLK_CTRL_RDB, MD_RTM_CMU_PCS_CKEN, reg_val);
        }
    }
    else {
        if (host_or_line == LINE) {
            reg_val  = hsip_rd_field_(phy_info_ptr, RETIMER_CDR_CLK_CTRL_RDB,MD_RTM_CDR_PCS_CKEN );
            if(enable == IS_ENABLED)
                reg_val |= (wmask & 0xFF);
            else 
                reg_val &= (wmask ^ 0xFF);
            hsip_wr_field_(phy_info_ptr, RETIMER_CDR_CLK_CTRL_RDB, MD_RTM_CDR_PCS_CKEN,reg_val);
        } else { // 
            reg_val  = hsip_rd_field_(phy_info_ptr, RETIMER_CMU_CLK_CTRL_RDB,MD_RTM_CMU_PCS_CKEN );
            if(enable == IS_ENABLED)
                reg_val |= (wmask & 0xFF);
            else 
                reg_val &= (wmask ^ 0xFF);
            hsip_wr_field_(phy_info_ptr, RETIMER_CMU_CLK_CTRL_RDB, MD_RTM_CMU_PCS_CKEN, reg_val);
        }
    }

    return RR_SUCCESS;
}

//Checked Top
return_result_t chal_cw_rtmr_fec_reset_control_8p(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_host_or_line_t host_or_line, cfg_egr_or_igr_t egr_or_igr, rst_assert_t reset)
{
    (void)cur_port_config_ptr;
    //  release fec per lane reset
    uint16_t reg_val;
    uint16_t lane_mask = 0;
    lane_mask  = (host_or_line == HOST) ? cur_mode_parameter_ptr->host_llane_mask : cur_mode_parameter_ptr->line_llane_mask;
 
    if (egr_or_igr == EGR) {
        if (host_or_line == HOST) { // HOST
            reg_val  = hsip_rd_field_(phy_info_ptr, RETIMER_CDR_RSTB_CTRL_RDB, MD_RTM_CDR_RSTB);
            if(reset == RST_DEASSERTED)
                reg_val |= (lane_mask & 0xFF);
            else 
                reg_val &= (lane_mask ^ 0xFF);
            hsip_wr_field_(phy_info_ptr, RETIMER_CDR_RSTB_CTRL_RDB, MD_RTM_CDR_RSTB, reg_val);
        } else { // 
            reg_val  = hsip_rd_field_(phy_info_ptr, RETIMER_CMU_RSTB_CTRL_RDB, MD_RTM_CMU_RSTB);
            if(reset == RST_DEASSERTED)
                reg_val |= (lane_mask & 0xFF);
            else 
                reg_val &= (lane_mask ^ 0xFF);
            hsip_wr_field_(phy_info_ptr, RETIMER_CMU_RSTB_CTRL_RDB, MD_RTM_CMU_RSTB, reg_val);
        }
    }
    else {
        if (host_or_line == LINE) {
            reg_val  = hsip_rd_field_(phy_info_ptr, RETIMER_CDR_RSTB_CTRL_RDB, MD_RTM_CDR_RSTB);
            if(reset == RST_DEASSERTED)
                reg_val |= (lane_mask & 0xFF);
            else 
                reg_val &= (lane_mask ^ 0xFF);
            hsip_wr_field_(phy_info_ptr, RETIMER_CDR_RSTB_CTRL_RDB, MD_RTM_CDR_RSTB, reg_val);
        } else { // 
            reg_val  = hsip_rd_field_(phy_info_ptr, RETIMER_CMU_RSTB_CTRL_RDB, MD_RTM_CMU_RSTB);
            if(reset == RST_DEASSERTED)
                reg_val |= (lane_mask & 0xFF);
            else 
                reg_val &= (lane_mask ^ 0xFF);
            hsip_wr_field_(phy_info_ptr, RETIMER_CMU_RSTB_CTRL_RDB, MD_RTM_CMU_RSTB, reg_val);
        }
    }
    return RR_SUCCESS;
}


//Checked Top
return_result_t chal_cw_rtmr_fec_reset_control_4p(phy_info_t* phy_info_ptr,  cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_host_or_line_t host_or_line, cfg_egr_or_igr_t egr_or_igr, rst_assert_t reset)
{
    //  release fec per lane reset
    uint16_t reg_val;
    uint16_t lane_mask = 0;
    cfg_pam_or_nrz_t pam_or_nrz = (egr_or_igr == EGR) ? cur_mode_parameter_ptr->host_pam_or_nrz_type : cur_mode_parameter_ptr->line_pam_or_nrz_type;
    
    lane_mask  = (host_or_line == HOST) ? cur_mode_parameter_ptr->host_llane_mask : cur_mode_parameter_ptr->line_llane_mask;
    if(cur_mode_parameter_ptr->speed == SPEED_200G && pam_or_nrz == CW_PAM) {
       lane_mask = 0x55;
    } else if(cur_mode_parameter_ptr->speed == SPEED_100G && pam_or_nrz == CW_PAM) {
      if(cur_port_config_ptr->port_100g_en[0]) {
         lane_mask = 0x5;
      } else if(cur_port_config_ptr->port_100g_en[2]) {
         lane_mask = 0x50;
      }
    } else if(cur_mode_parameter_ptr->speed == SPEED_50G && pam_or_nrz == CW_PAM) {
        if(cur_port_config_ptr->port_50g_en[0]) {
            lane_mask = 0x1;
        } else if(cur_port_config_ptr->port_50g_en[2]) {
            lane_mask = 0x4;
        } else if(cur_port_config_ptr->port_50g_en[4]) {
            lane_mask = 0x10;
        } else if(cur_port_config_ptr->port_50g_en[6]) {
            lane_mask = 0x40;
        }
    }
    
    if (egr_or_igr == EGR) {
        if (host_or_line == HOST) { // HOST
            reg_val  = hsip_rd_field_(phy_info_ptr, RETIMER_CDR_RSTB_CTRL_RDB, MD_RTM_CDR_RSTB);
            if(reset == RST_DEASSERTED)
                reg_val |= (lane_mask & 0xFF);
            else 
                reg_val &= (lane_mask ^ 0xFF);
            hsip_wr_field_(phy_info_ptr, RETIMER_CDR_RSTB_CTRL_RDB, MD_RTM_CDR_RSTB, reg_val);
        } else { // 
            reg_val  = hsip_rd_field_(phy_info_ptr, RETIMER_CMU_RSTB_CTRL_RDB, MD_RTM_CMU_RSTB);
            if(reset == RST_DEASSERTED)
                reg_val |= (lane_mask & 0xFF);
            else 
                reg_val &= (lane_mask ^ 0xFF);
            hsip_wr_field_(phy_info_ptr, RETIMER_CMU_RSTB_CTRL_RDB, MD_RTM_CMU_RSTB, reg_val);
        }
    }
    else {
        if (host_or_line == LINE) {
            reg_val  = hsip_rd_field_(phy_info_ptr, RETIMER_CDR_RSTB_CTRL_RDB, MD_RTM_CDR_RSTB);
            if(reset == RST_DEASSERTED)
                reg_val |= (lane_mask & 0xFF);
            else 
                reg_val &= (lane_mask ^ 0xFF);
            hsip_wr_field_(phy_info_ptr, RETIMER_CDR_RSTB_CTRL_RDB, MD_RTM_CDR_RSTB, reg_val);
        } else { // 
            reg_val  = hsip_rd_field_(phy_info_ptr, RETIMER_CMU_RSTB_CTRL_RDB, MD_RTM_CMU_RSTB);
            if(reset == RST_DEASSERTED)
                reg_val |= (lane_mask & 0xFF);
            else 
                reg_val &= (lane_mask ^ 0xFF);
            hsip_wr_field_(phy_info_ptr, RETIMER_CMU_RSTB_CTRL_RDB, MD_RTM_CMU_RSTB, reg_val);
        }
    }
    return RR_SUCCESS;
}

return_result_t chal_cw_rtmr_fec_reset_control(phy_info_t* phy_info_ptr,  cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_host_or_line_t host_or_line, cfg_egr_or_igr_t egr_or_igr, rst_assert_t reset)
{
    if (ml_cw_rtmr_is_A1_4p_port(cur_mode_parameter_ptr)) {        
        return chal_cw_rtmr_fec_reset_control_4p(phy_info_ptr, cur_mode_parameter_ptr, cur_port_config_ptr, host_or_line, egr_or_igr, reset);
    } else {
        return chal_cw_rtmr_fec_reset_control_8p(phy_info_ptr, cur_mode_parameter_ptr, cur_port_config_ptr, host_or_line, egr_or_igr, reset);
    }
}

//Checked Top
return_result_t chal_cw_rtmr_pcs_reset_control(phy_info_t* phy_info_ptr,  cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_host_or_line_t host_or_line, cfg_egr_or_igr_t egr_or_igr, rst_assert_t reset)
{
    //  assert pcs per lane reset
    uint16_t reg_val;
    uint16_t lane_mask = 0;
    uint16_t wmask = 0;
 
    lane_mask  = (host_or_line == HOST)  ? cur_mode_parameter_ptr->host_llane_mask : cur_mode_parameter_ptr->line_llane_mask;
    wmask = (1 << (util_get_lowest_index_from_mask(lane_mask)));

    if (egr_or_igr == EGR) {
        if (host_or_line == HOST) { // HOST
            reg_val  = hsip_rd_field_(phy_info_ptr, RETIMER_CDR_RSTB_CTRL_RDB,MD_RTM_CDR_PCS_RSTB );
            if(reset == RST_DEASSERTED)
                reg_val |= (wmask & 0xFF);
            else 
                reg_val &= (wmask ^ 0xFF);
            hsip_wr_field_(phy_info_ptr, RETIMER_CDR_RSTB_CTRL_RDB, MD_RTM_CDR_PCS_RSTB, reg_val);
        } else { 
            reg_val  = hsip_rd_field_(phy_info_ptr, RETIMER_CMU_RSTB_CTRL_RDB, MD_RTM_CMU_PCS_RSTB);
            if(reset == RST_DEASSERTED)
                reg_val |= (wmask & 0xFF);
            else 
                reg_val &= (wmask ^ 0xFF);
            hsip_wr_field_(phy_info_ptr, RETIMER_CMU_RSTB_CTRL_RDB, MD_RTM_CMU_PCS_RSTB, reg_val);
        }
    }
    else {
        if (host_or_line == LINE) {
            reg_val  = hsip_rd_field_(phy_info_ptr, RETIMER_CDR_RSTB_CTRL_RDB,MD_RTM_CDR_PCS_RSTB );
            if(reset == RST_DEASSERTED)
                reg_val |= (wmask & 0xFF);
            else 
                reg_val &= (wmask ^ 0xFF);
            hsip_wr_field_(phy_info_ptr, RETIMER_CDR_RSTB_CTRL_RDB, MD_RTM_CDR_PCS_RSTB, reg_val);
        } else { 
            reg_val  = hsip_rd_field_(phy_info_ptr, RETIMER_CMU_RSTB_CTRL_RDB, MD_RTM_CMU_PCS_RSTB);
            if(reset == RST_DEASSERTED)
                reg_val |= (wmask & 0xFF);
            else 
                reg_val &= (wmask ^ 0xFF);
            hsip_wr_field_(phy_info_ptr, RETIMER_CMU_RSTB_CTRL_RDB, MD_RTM_CMU_PCS_RSTB, reg_val);
        }
    }
    return RR_SUCCESS;
}

//Checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_top_reset_cfg(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t wmask=0, wmask1=0, wmask2=0, wmask3=0;
    uint16_t wdata=0;

    if ((cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC) ||
        (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KRKP) ||
        (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC) ||
        (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_FWD)) {
        wmask1 = (egr_or_igr == EGR) ? (cur_mode_parameter_ptr->host_llane_mask) : (cur_mode_parameter_ptr->line_llane_mask);
        wmask3 = (egr_or_igr == EGR) ? (cur_mode_parameter_ptr->line_llane_mask) : (cur_mode_parameter_ptr->host_llane_mask);
        wmask1 |= (wmask3)<<8;
    }

    if (cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) {
        wmask2  = (egr_or_igr == EGR) ? (cur_mode_parameter_ptr->host_llane_mask) : 0;
        wmask2 |= (egr_or_igr == IGR) ? (cur_mode_parameter_ptr->host_llane_mask)<<8 : 0;
        wmask1 = (egr_or_igr == IGR) ? (cur_mode_parameter_ptr->line_llane_mask) : 0;
        wmask1 |= (egr_or_igr == EGR) ? (cur_mode_parameter_ptr->line_llane_mask)<<8 : 0;
    }

    if (cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) {
        wmask2  = (egr_or_igr == IGR) ? (cur_mode_parameter_ptr->line_llane_mask) : 0;
        wmask2 |= (egr_or_igr == EGR) ? (cur_mode_parameter_ptr->line_llane_mask)<<8 : 0;
        wmask1 |= (egr_or_igr == EGR) ? (cur_mode_parameter_ptr->host_llane_mask) : 0;
        wmask1 |= (egr_or_igr == IGR) ? (cur_mode_parameter_ptr->host_llane_mask)<<8 : 0;
    }

    wmask = (egr_or_igr == IGR) ? (cur_mode_parameter_ptr->host_llane_mask) : (cur_mode_parameter_ptr->line_llane_mask);

    if (wmask) {
        wdata = (reset == RST_ASSERTED) ? 0 : wmask;
        hsip_wr_fields(phy_info_ptr, TOP_RESET_CTRL0_REG, wmask, wdata);
    }

    if (wmask1) {
        wdata = (reset == RST_ASSERTED) ? 0 : wmask1;
        hsip_wr_fields(phy_info_ptr, TOP_RESET_CTRL1_REG, wmask1, wdata);
    }
    if (wmask2) {
        wdata = (reset == RST_ASSERTED) ? 0 : wmask2;
        hsip_wr_fields(phy_info_ptr, TOP_RESET_CTRL2_REG, wmask2, wdata);
    }

    return RR_SUCCESS;
}


//Checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_fec_enc_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr)
{
    (void)egr_or_igr;
    uint16_t wmask = 0, wdata = 0;

    wmask = cur_mode_parameter_ptr->fec_slice;

    if (wmask) {
        wmask = (wmask)<<8;
        wdata = (enable == IS_ENABLED) ? wmask : 0;
        hsip_wr_fields(phy_info_ptr, BLOCK_CLKEN_CTRL3_REG, wmask, wdata);
    }

    return RR_SUCCESS;
}

//Checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_fec_symb_dist_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr)
{
    (void)egr_or_igr;
    uint16_t wmask = 0, wdata = 0;

    wmask = cur_mode_parameter_ptr->fec_slice;

    if (wmask) {
        wdata = (enable == IS_ENABLED) ? wmask : 0;
        hsip_wr_fields(phy_info_ptr, BLOCK_CLKEN_CTRL3_REG, wmask, wdata);
    }

    return RR_SUCCESS;
} 

//Checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_xdec_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, enabled_t enable)
{
    // enable clocks for xdec and xdec_gbox
    uint16_t wmask = 0, wdata = 0;
    uint16_t reg_val = 0;
    switch (cur_mode_parameter_ptr->speed) {
        case (SPEED_400G):
            if (cur_port_config_ptr->port_400g_en) {
                wmask = 0x55;
            }
            break;
        case (SPEED_200G):
            if (cur_port_config_ptr->port_200g_en[0]) {
                wmask = 0x5;
            }
            else if (cur_port_config_ptr->port_200g_en[1]) {
                wmask = 0x50;
            }
            break;
        case (SPEED_100G):
            if (cur_port_config_ptr->port_100g_en[0]) {
                wmask = 0x1;
            }
            else if (cur_port_config_ptr->port_100g_en[1]) {
                wmask = 0x4;
            }
            else if (cur_port_config_ptr->port_100g_en[2]) {
                wmask = 0x10;
            }
            else if (cur_port_config_ptr->port_100g_en[3]) {
                wmask = 0x40;
            }
            break;
        case (SPEED_50G):
        case (SPEED_25G):
            if ((cur_port_config_ptr->port_50g_en[0]) || (cur_port_config_ptr->port_25g_en[0])) {
                wmask = 0x1;
            }
            else if ((cur_port_config_ptr->port_50g_en[1]) || (cur_port_config_ptr->port_25g_en[1])) {
                wmask = 0x2;
            }
            else if ((cur_port_config_ptr->port_50g_en[2]) || (cur_port_config_ptr->port_25g_en[2])) {
                wmask = 0x4;
            }
            else if ((cur_port_config_ptr->port_50g_en[3]) || (cur_port_config_ptr->port_25g_en[3])) {
                wmask = 0x8;
            }
            else if ((cur_port_config_ptr->port_50g_en[4]) || (cur_port_config_ptr->port_25g_en[4])) {
                wmask = 0x10;
            }
            else if ((cur_port_config_ptr->port_50g_en[5]) || (cur_port_config_ptr->port_25g_en[5])) {
                wmask = 0x20;
            }
            else if ((cur_port_config_ptr->port_50g_en[6]) || (cur_port_config_ptr->port_25g_en[6])) {
                wmask = 0x40;
            }
            else if ((cur_port_config_ptr->port_50g_en[7]) || (cur_port_config_ptr->port_25g_en[7])) {
                wmask = 0x80;
            }
            break;
        default:
            wmask = 0x0;
            break;
    }

    if (wmask) {
        reg_val = hsip_rd_field_(phy_info_ptr,  BLOCK_CLKEN_CTRL1_REG, MD_CKE_FEC_XDEC);
        wdata = (enable == IS_ENABLED) ? (reg_val | wmask) : reg_val;
        hsip_wr_field_ (phy_info_ptr, BLOCK_CLKEN_CTRL1_REG, MD_CKE_FEC_XDEC, wdata);
    }

    return RR_SUCCESS;
}

//Checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_xenc_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, enabled_t enable)
{
    // enable clocks for xenc and xenc_gbox
    uint16_t wmask = 0, wdata = 0;
    uint16_t reg_val = 0;
    switch (cur_mode_parameter_ptr->speed) {
        case (SPEED_400G):
            if (cur_port_config_ptr->port_400g_en) {
                wmask = 0x55;
            }
            break;
        case (SPEED_200G):
            if (cur_port_config_ptr->port_200g_en[0]) {
                wmask = 0x5;
            }
            else if (cur_port_config_ptr->port_200g_en[1]) {
                wmask = 0x50;
            }
            break;
        case (SPEED_100G):
            if (cur_port_config_ptr->port_100g_en[0]) {
                wmask = 0x1;
            }
            else if (cur_port_config_ptr->port_100g_en[1]) {
                wmask = 0x4;
            }
            else if (cur_port_config_ptr->port_100g_en[2]) {
                wmask = 0x10;
            }
            else if (cur_port_config_ptr->port_100g_en[3]) {
                wmask = 0x40;
            }
            break;
        case (SPEED_50G):
        case (SPEED_25G):
            if ((cur_port_config_ptr->port_50g_en[0]) || (cur_port_config_ptr->port_25g_en[0])) {
                wmask = 0x1;
            }
            else if ((cur_port_config_ptr->port_50g_en[1]) || (cur_port_config_ptr->port_25g_en[1])) {
                wmask = 0x2;
            }
            else if ((cur_port_config_ptr->port_50g_en[2]) || (cur_port_config_ptr->port_25g_en[2])) {
                wmask = 0x4;
            }
            else if ((cur_port_config_ptr->port_50g_en[3]) || (cur_port_config_ptr->port_25g_en[3])) {
                wmask = 0x8;
            }
            else if ((cur_port_config_ptr->port_50g_en[4]) || (cur_port_config_ptr->port_25g_en[4])) {
                wmask = 0x10;
            }
            else if ((cur_port_config_ptr->port_50g_en[5]) || (cur_port_config_ptr->port_25g_en[5])) {
                wmask = 0x20;
            }
            else if ((cur_port_config_ptr->port_50g_en[6]) || (cur_port_config_ptr->port_25g_en[6])) {
                wmask = 0x40;
            }
            else if ((cur_port_config_ptr->port_50g_en[7]) || (cur_port_config_ptr->port_25g_en[7])) {
                wmask = 0x80;
            }
            break;
        default:
            wmask = 0x0;
            break;
    }

    if (wmask) {
        reg_val = hsip_rd_field_(phy_info_ptr,  BLOCK_CLKEN_CTRL9_REG, MD_CKE_FEC_XENC);
        wdata = (enable == IS_ENABLED) ? (reg_val | wmask) : reg_val;
        hsip_wr_field_ (phy_info_ptr, BLOCK_CLKEN_CTRL9_REG, MD_CKE_FEC_XENC, wdata);
    }

    return RR_SUCCESS;
}

//Checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_xenc_am_ins_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, enabled_t enable)
{
    // enable clocks for AM insertion block of xenc
    uint16_t wmask = 0, wdata = 0;
    uint16_t reg_val = 0;
    switch (cur_mode_parameter_ptr->speed) {
        case (SPEED_400G):
            if (cur_port_config_ptr->port_400g_en) {
                wmask = 0x1;
            }
            break;
        case (SPEED_200G):
            if (cur_port_config_ptr->port_200g_en[0]) {
                wmask = 0x1;
            }
            else if (cur_port_config_ptr->port_200g_en[1]) {
                wmask = 0x10;
            }
            break;
        case (SPEED_100G):
            if (cur_port_config_ptr->port_100g_en[0]) {
                wmask = 0x1;
            }
            else if (cur_port_config_ptr->port_100g_en[1]) {
                wmask = 0x4;
            }
            else if (cur_port_config_ptr->port_100g_en[2]) {
                wmask = 0x10;
            }
            else if (cur_port_config_ptr->port_100g_en[3]) {
                wmask = 0x40;
            }
            break;
        case (SPEED_50G):
        case (SPEED_25G):
            if ((cur_port_config_ptr->port_50g_en[0]) || (cur_port_config_ptr->port_25g_en[0])) {
                wmask = 0x1;
            }
            else if ((cur_port_config_ptr->port_50g_en[1]) || (cur_port_config_ptr->port_25g_en[1])) {
                wmask = 0x2;
            }
            else if ((cur_port_config_ptr->port_50g_en[2]) || (cur_port_config_ptr->port_25g_en[2])) {
                wmask = 0x4;
            }
            else if ((cur_port_config_ptr->port_50g_en[3]) || (cur_port_config_ptr->port_25g_en[3])) {
                wmask = 0x8;
            }
            else if ((cur_port_config_ptr->port_50g_en[4]) || (cur_port_config_ptr->port_25g_en[4])) {
                wmask = 0x10;
            }
            else if ((cur_port_config_ptr->port_50g_en[5]) || (cur_port_config_ptr->port_25g_en[5])) {
                wmask = 0x20;
            }
            else if ((cur_port_config_ptr->port_50g_en[6]) || (cur_port_config_ptr->port_25g_en[6])) {
                wmask = 0x40;
            }
            else if ((cur_port_config_ptr->port_50g_en[7]) || (cur_port_config_ptr->port_25g_en[7])) {
                wmask = 0x80;
            }
            break;
        default:
            wmask = 0x0;
            break;
    }

    if (wmask) {
        reg_val = hsip_rd_field_(phy_info_ptr,  XENC_SLICE_CTRL_REG, XENC_AM_INS_SLICE_CKEN);
        wdata = (enable == IS_ENABLED) ? (reg_val | wmask) : reg_val;
        hsip_wr_field_ (phy_info_ptr, XENC_SLICE_CTRL_REG, XENC_AM_INS_SLICE_CKEN, wdata);
    }

    return RR_SUCCESS;
} 

//Checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_krkp_gbox_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable)
{
    // enable clocks for krkp_gbox 
    uint16_t wmask = 0, wdata = 0;

    wmask = cur_mode_parameter_ptr->fec_slice;

    if (wmask) {
        wdata = (enable == IS_ENABLED) ? wmask : 0;
        hsip_wr_fields(phy_info_ptr, BLOCK_CLKEN_CTRL4_REG, wmask, wdata);
    }

    return RR_SUCCESS;
}


//Checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_pcs_tmt_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr)
{
    (void)egr_or_igr;
    // enable clocks for AM_lock, deskew and reorder
    uint16_t wmask = 0, wdata = 0;

    wmask = cur_mode_parameter_ptr->pcs_slice;

    if (wmask) {
        wmask = (wmask)<<8;
        wdata = (enable == IS_ENABLED) ? wmask : 0;
        hsip_wr_fields(phy_info_ptr, BLOCK_CLKEN_CTRL6_REG, wmask, wdata);
    }

    return RR_SUCCESS;
} 


//Checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_fec_enc_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t wmask = 0, wdata = 0;

    wmask = (egr_or_igr == IGR) ? (cur_mode_parameter_ptr->host_llane_mask) : (cur_mode_parameter_ptr->line_llane_mask);
    wmask = (1 << (util_get_lowest_index_from_mask(wmask)));

    if (wmask) {
        wmask = (wmask)<<8;
        wdata = (reset == RST_DEASSERTED) ? wmask : 0;
        hsip_wr_fields(phy_info_ptr, BLOCK_RESET_CTRL3_REG, wmask, wdata);
    }

    return RR_SUCCESS;
}

//Checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_fec_symb_dist_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t wmask = 0, wdata = 0;

    wmask = (egr_or_igr == IGR) ? (cur_mode_parameter_ptr->host_llane_mask) : (cur_mode_parameter_ptr->line_llane_mask);
    wmask = (1 << (util_get_lowest_index_from_mask(wmask)));

    if (wmask) {
        wdata = (reset == RST_DEASSERTED) ? wmask : 0;
        hsip_wr_fields(phy_info_ptr, BLOCK_RESET_CTRL3_REG, wmask, wdata);
    }

    return RR_SUCCESS;
} 

//Checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_xdec_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr)
{
    // enable clocks for xdec and xdec_gbox
    uint16_t wmask = 0, wdata = 0;

    wmask = (egr_or_igr == EGR) ? (cur_mode_parameter_ptr->host_llane_mask) : (cur_mode_parameter_ptr->line_llane_mask);
    wmask = (1 << (util_get_lowest_index_from_mask(wmask)));

    if (wmask) {
        wmask |= (wmask)<<8;
        wdata = (reset == RST_DEASSERTED) ? wmask : 0;
        hsip_wr_fields(phy_info_ptr, BLOCK_RESET_CTRL1_REG, wmask, wdata);
    }

    return RR_SUCCESS;
}

//Checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_xenc_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr)
{
    // enable clocks for xenc and xenc_gbox
    uint16_t wmask = 0, wdata = 0;

    wmask = (egr_or_igr == IGR) ? (cur_mode_parameter_ptr->host_llane_mask) : (cur_mode_parameter_ptr->line_llane_mask);
    wmask = (1 << (util_get_lowest_index_from_mask(wmask)));

    if (wmask) {
        wmask |= (wmask)<<8;
        wdata = (reset == RST_DEASSERTED) ? wmask : 0;
        hsip_wr_fields(phy_info_ptr, BLOCK_RESET_CTRL9_REG, wmask, wdata);
    }

    return RR_SUCCESS;
} 

//Checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_krkp_gbox_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr)
{
    // enable clocks for krkp_gbox 
    uint16_t wmask = 0, wdata = 0;

    wmask = (egr_or_igr == IGR) ? (cur_mode_parameter_ptr->host_llane_mask) : (cur_mode_parameter_ptr->line_llane_mask);
    wmask = (1 << (util_get_lowest_index_from_mask(wmask)));

    if (wmask) {
        wdata = (reset == RST_DEASSERTED) ? wmask : 0;
        hsip_wr_fields(phy_info_ptr, BLOCK_RESET_CTRL4_REG, wmask, wdata);
    }

    return RR_SUCCESS;
} 


//Checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_pcs_tmt_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t wmask = 0, wdata = 0;

    wmask = (egr_or_igr == IGR) ? (cur_mode_parameter_ptr->host_llane_mask) : (cur_mode_parameter_ptr->line_llane_mask);
    wmask = (1 << (util_get_lowest_index_from_mask(wmask)));

    if (wmask) {
        wmask = (wmask)<<8;
        wdata = (reset == RST_DEASSERTED) ? wmask : 0;
        hsip_wr_fields(phy_info_ptr, BLOCK_RESET_CTRL6_REG, wmask, wdata);
    }

    return RR_SUCCESS;
}


//Checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_tmt_gbox_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t wmask=0, wdata=0;

    wmask = (egr_or_igr == IGR) ? cur_mode_parameter_ptr->host_llane_mask : cur_mode_parameter_ptr->line_llane_mask;

    if (wmask) {
        wdata = (reset == RST_DEASSERTED) ? wmask : 0;
        hsip_wr_fields(phy_info_ptr, TMT_PCS_GBOX_RESETB, wmask, wdata);
    }

    return RR_SUCCESS;
}


//Checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_tmt_pfifo_clk_gate_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t lane, wmask=0, wmask1=0, wdata=0;

    wmask = (egr_or_igr == IGR) ? cur_mode_parameter_ptr->host_llane_mask : cur_mode_parameter_ptr->line_llane_mask;

    // pfifo's one set would have 2 physical pfifo's 
    // in case of PAM4 intf both pfifo's would be used
    // in case of NRZ intf, only one pfifo would be used
    for (lane=0; lane<8; lane++) {
        if ((1<<lane) & wmask) {
            if (egr_or_igr == IGR) {
                wmask1 |= ((cur_mode_parameter_ptr->host_pam_or_nrz_type == CW_NRZ) ? 0x1 : 0x3) << (lane<<1);
            } else {
                wmask1 |= ((cur_mode_parameter_ptr->line_pam_or_nrz_type == CW_NRZ) ? 0x1 : 0x3) << (lane<<1);
            }
        }
    }

    if (wmask1) {
        wdata = (enable == IS_ENABLED) ? wmask1 : 0;
        hsip_wr_fields(phy_info_ptr, TMT_FEC_FIFO_CLK_CTRL_REG, wmask1, wdata);
    }

    return RR_SUCCESS;
}

//Checked RETIMER_IGR or RETIMER_EGR

//Checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_tmt_pfifo_reset_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, rst_assert_t reset, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t wmask=0, wdata=0;

    wmask = chal_cw_rtmr_tmt_pfifo_get_lane_mask(cur_mode_parameter_ptr, egr_or_igr);

    if (wmask) {
        wdata = (reset == RST_DEASSERTED) ? wmask : 0;
        hsip_wr_fields(phy_info_ptr, TMT_FEC_FIFO_RESETB_REG, wmask, wdata);
    }

    return RR_SUCCESS;
}

// Checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_rcv_gapclk_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t wdata=0;
    bool use_gapclk40 = false, use_gapclk64_0 = false;
    uint16_t fec_type = (egr_or_igr == EGR) ? cur_mode_parameter_ptr->host_fec_type : cur_mode_parameter_ptr->line_fec_type;
    uint16_t lane_mask = (egr_or_igr == EGR) ? cur_mode_parameter_ptr->host_llane_mask : cur_mode_parameter_ptr->line_llane_mask;
    
    use_gapclk40 = ((cur_mode_parameter_ptr->speed == SPEED_25G) &&
                    ((cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_FWD) || 
                     (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC) ||
                     (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC) || 
                     ((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) && (egr_or_igr == IGR)))) ? true : false;

    // MD_RCV_CLK40_GAPCLK_RELOCK_ENA_LN @ 15:08
    // RESERVED  @ 07:00
    wdata = (uint16_t)hsip_rd_reg_(phy_info_ptr, RCV_CLK40_GAPCLK_CTRL1_REG);
    if ( use_gapclk40 && (enable == IS_ENABLED)) {
        wdata |= (lane_mask<<8);
    } else {
        wdata &= ~(lane_mask<<8);
    }
    hsip_wr_reg_(phy_info_ptr, RCV_CLK40_GAPCLK_CTRL1_REG, wdata);

    // MD_RCV_CLK40_GAPCLK_BYPASS_LN @ 15:08
    // MD_RCV_CLK40_GAPCLK_RSTB_LN   @ 07:00
    wdata = (uint16_t)hsip_rd_reg_(phy_info_ptr, RCV_CLK40_GAPCLK_CTRL0_REG);
    if ( use_gapclk40 && (enable == IS_ENABLED)) {
        wdata &= ~(lane_mask << 8); wdata |= lane_mask;
    } else {
        wdata |= (lane_mask << 8); wdata &= ~lane_mask;
    }
    hsip_wr_reg_(phy_info_ptr, RCV_CLK40_GAPCLK_CTRL0_REG, wdata);

    // disable gapclk bypass for all modes
    hsip_wr_reg_(phy_info_ptr, RCV_CLK64_GAPCLK_CTRL1_REG, 0);

    use_gapclk64_0 = (fec_type == CHIP_HOST_FEC_TYPE_RS544 || fec_type == CHIP_HOST_FEC_TYPE_CL161) ? true : false;
    hsip_wr_reg_(phy_info_ptr, RCV_CLK64_GAPCLK_CTRL0_REG, use_gapclk64_0 ? 0xFF : 0);    

    return RR_SUCCESS;
}

//Checked RETIMER_IGR or RETIMER_EGR
return_result_t chal_cw_rtmr_tmt_gapclk_control(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, enabled_t enable, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t wdata=0;
    bool use_gapclk40 = false, use_gapclk64_0 = false;

    uint16_t fec_type = (egr_or_igr == IGR) ? cur_mode_parameter_ptr->host_fec_type : cur_mode_parameter_ptr->line_fec_type;
    uint16_t lane_mask = (egr_or_igr == IGR) ? cur_mode_parameter_ptr->host_llane_mask : cur_mode_parameter_ptr->line_llane_mask;

    use_gapclk40 = ((cur_mode_parameter_ptr->speed == SPEED_25G) &&
                    ((cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC) || 
                     ((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) && (egr_or_igr == EGR)))) ? true : false;

    // MD_TMT_CLK40_GAPCLK_BYPASS_LN @ 15:08
    // MD_TMT_CLK40_GAPCLK_RSTB_LN   @ 07:00
    wdata = (uint16_t)hsip_rd_reg_(phy_info_ptr, TMT_CLK40_GAPCLK_CTRL0_REG);
    if ( use_gapclk40 && (enable == IS_ENABLED) ) {
        wdata &= ~(lane_mask << 8); wdata |=  lane_mask;
    } else {
        wdata |=  (lane_mask << 8); wdata &= ~lane_mask;
    }
    hsip_wr_reg_(phy_info_ptr, TMT_CLK40_GAPCLK_CTRL0_REG, wdata);

    // disable gapclk bypass for all modes
    hsip_wr_reg_(phy_info_ptr, TMT_CLK64_GAPCLK_CTRL1_REG, 0);

    // igr, if  cw_mode_ptr->host_fec_type == RS544, set TMT_CLK64_GAPCLK_CTRL0_REG to 1, else set to 0
    // egr, if  cw_mode_ptr->line_fec_type == RS544, set TMT_CLK64_GAPCLK_CTRL0_REG to 1, else set to 0
    use_gapclk64_0 = (fec_type == CHIP_HOST_FEC_TYPE_RS544 || fec_type == CHIP_HOST_FEC_TYPE_CL161) ? true : false;
    hsip_wr_reg_(phy_info_ptr, TMT_CLK64_GAPCLK_CTRL0_REG, use_gapclk64_0 ? 0xFF : 0);    

    return RR_SUCCESS;
}

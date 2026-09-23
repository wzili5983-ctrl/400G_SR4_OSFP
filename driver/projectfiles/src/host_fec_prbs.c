/**
 *
 * @file     host_fec_prbs.c
 * @author   
 * @date     9/21/2020
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
#include <string.h>
#include <time.h>
#include "chip_config_def.h"
#include "hr_time.h"
#include "type_defns.h"
#include "access.h"
#include "regs_common.h"
#include "common_def.h"
#include "chip_common_config_ind.h"
#include "chip_mode_def.h"
#include "capi_def.h"
#include "capi_diag_def.h"
#include "diag_fec_statistics_def.h"
#include "common_util.h"
#include "cw_def.h"
#include "cw_comm_def.h"
#include "host_log_util.h"
#include "pam4_prbs_def.h"
#include "chal_cw_rtmr_kp4prbs.h"
#include "ml_cw_rtmr_modes.h"
#include "host_fec_prbs.h"
#include "capi.h"
#include "chal_cw_rtmr_clkrst_control.h"
#include "chal_cw_rtmr_clockrst_mux.h"
#include "chal_cw_rtmr_status_check.h"
#include "chal_cw_rtmr_datapath_cfg.h"
#include "chal_cw_top.h"
#include "ml_cw_rtmr_handler.h"
#include "host_diag_fec_statistics.h"

extern const uint32_t fec_port_config2_regs[];

const uint32_t fec_port_config2_shadow_regs[] ={
    OCTAL_PORT_MODE_CONFIG_2_SHADOW_REG_0, 
    OCTAL_PORT_MODE_CONFIG_2_SHADOW_REG_1, 
    OCTAL_PORT_MODE_CONFIG_2_SHADOW_REG_2, 
    OCTAL_PORT_MODE_CONFIG_2_SHADOW_REG_3,
    OCTAL_PORT_MODE_CONFIG_2_SHADOW_REG_4, 
    OCTAL_PORT_MODE_CONFIG_2_SHADOW_REG_5, 
    OCTAL_PORT_MODE_CONFIG_2_SHADOW_REG_6, 
    OCTAL_PORT_MODE_CONFIG_2_SHADOW_REG_7,
};

/**
* @brief      host_fec_ignore_fault (capi_phy_info_t* phy_info_ptr, uint8_t lane_mask, capi_direction_t direction, capi_enable_t ignore_fhdlr)
* @details    This API is used to set ignore fault handling in OCW fw per port per ingress/egress
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  lane_mask: line-side lane-mask for selected PORT
* @param[in]  direction: data-path direction
* @param[in]  ignore_fhdlr: enable(1) or disable(0) for ignoring fault handling
* 
* @return     returns the performance result of the called method/function
*/
return_result_t host_fec_ignore_fault(capi_phy_info_t* phy_info_ptr,
                                      uint8_t lane_mask, 
                                      capi_direction_t direction, 
                                      capi_enable_t ignore_fhdlr, 
                                      uint8_t pidx)
{
    uint16_t wdata, wmask;
    uint8_t wcnt=0, pflag=1, chkcnt;
    uint16_t reg_val;

    phy_info_ptr->base_addr = OCTAL_TOP_REGS;
    wmask = (direction == DIR_EGRESS) ? (uint16_t) lane_mask : ((uint16_t)lane_mask) << 8;
    wdata = (ignore_fhdlr == CAPI_ENABLE) ? wmask : 0;
    hsip_wr_fields(phy_info_ptr,  OCW_CONFIG_FLT_PROC_CFG_REG, wmask, wdata);
    chkcnt = (ignore_fhdlr == CAPI_ENABLE) ? 100 : 20;
    do {
        delay_ms(10);
        if(direction==DIR_EGRESS)
            ERR_HSIP(reg_val = (uint16_t)hsip_rd_field_(phy_info_ptr, OCTAL_PORT_MISSION_INFO_REG, EGR_MISSION));
        else
            ERR_HSIP(reg_val = (uint16_t)hsip_rd_field_(phy_info_ptr, OCTAL_PORT_MISSION_INFO_REG, IGR_MISSION));
        reg_val &= (1<<pidx);
        pflag = (ignore_fhdlr==CAPI_ENABLE)?reg_val : (reg_val==0);
    } while (pflag == 0 && wcnt++ < chkcnt);
    return RR_SUCCESS;
}

/**
* @brief      host_fec_reset_port(capi_phy_info_t* phy_info_ptr, uint16_t port_mask)
* @details    This API is used to notify firmware FSM to result per port per ingress/egress
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  port_mask
* 
* @return     returns the performance result of the called method/function
*/
return_result_t host_fec_reset_port(capi_phy_info_t* phy_info_ptr, uint16_t port_mask)
{
    uint8_t wcnt = 0;
    uint16_t reg_val;
    hsip_wr_reg_(phy_info_ptr, OCW_CONFIG_FSM_RESET_CFG_REG, port_mask);
    do {
        delay_ms(10);
        ERR_HSIP(reg_val = (uint16_t) hsip_rd_reg_(phy_info_ptr, OCW_CONFIG_FSM_RESET_CFG_REG));
    } while (reg_val && wcnt++ < 100);
    if (reg_val)
        return RR_ERROR;
    else
        return RR_SUCCESS;
}


static return_result_t kp4_config_rtmr_prbs_pmon(capi_phy_info_t* phy_info_ptr, 
                       cw_mode_parameter_t *cw_mode_ptr, 
                       cw_port_config_t *port_cfg_ptr,
                       capi_prbs_cfg_t* pcfg,
                       cfg_egr_or_igr_t egr_or_igr)
{
    pam4_pmon_misc_cfg_t pmon_misc_struct_inst = {0};

    pmon_misc_struct_inst.polysel = pcfg->poly.fec_pgen_poly;
    pmon_misc_struct_inst.pmon_inv = pcfg->rx_invert;
    pmon_misc_struct_inst.pmon_init = 1;
    pmon_misc_struct_inst.fec_mode_ena = ON;
    /* Set QC_PMON_CFG0_REG:MD_PMON_1PAM4_0NRZ to 1 regardless of NRZ or PAM4 */
    pmon_misc_struct_inst.pam4_mode = ON;
    pmon_misc_struct_inst.nrz_ctrl = 0;

    chal_cw_rtmr_fec_prbs_pmon_config_wrapper(phy_info_ptr, egr_or_igr, &pmon_misc_struct_inst, port_cfg_ptr, cw_mode_ptr);
    return RR_SUCCESS;
}

/**
* @brief      kp4_config_rtmr_prbs_igr_line_pmon(capi_phy_info_t* phy_info_ptr, cw_mode_parameter_t *cw_mode_ptr, cw_port_config_t *port_cfg_ptr, capi_prbs_cfg_t* pcfg)
* @details    This API is used to config PRBS LINE PMON in retimer mode
*
* @param[in]  phy_info_ptr:
* @param[in]  cw_mode_ptr: 
* @param[in]  port_cfg_ptr: 
* @param[in]  pcfg: 
* 
* @return     returns the performance result of the called method/function
*/
static return_result_t kp4_config_rtmr_prbs_igr_line_pmon(capi_phy_info_t* phy_info_ptr, cw_mode_parameter_t *cw_mode_ptr, cw_port_config_t *port_cfg_ptr, capi_prbs_cfg_t* pcfg)
{
    return kp4_config_rtmr_prbs_pmon(phy_info_ptr, cw_mode_ptr, port_cfg_ptr, pcfg, IGR);
}

/**
* @brief      kp4_config_rtmr_prbs_egr_host_pmon(capi_phy_info_t* phy_info_ptr, cw_mode_parameter_t *cw_mode_ptr, cw_port_config_t *port_cfg_ptr, capi_prbs_cfg_t* pcfg)
* @details    This API is used to config PRBS HOST PMON in retimer mode
*
* @param[in]  phy_info_ptr:
* @param[in]  cw_mode_ptr: 
* @param[in]  port_cfg_ptr: 
* @param[in]  pcfg: 
* 
* @return     returns the performance result of the called method/function
*/
static return_result_t kp4_config_rtmr_prbs_egr_host_pmon(capi_phy_info_t* phy_info_ptr, cw_mode_parameter_t *cw_mode_ptr, cw_port_config_t *port_cfg_ptr, capi_prbs_cfg_t* pcfg)
{
    return kp4_config_rtmr_prbs_pmon(phy_info_ptr, cw_mode_ptr, port_cfg_ptr, pcfg, EGR);
}

static return_result_t kp4_config_rtmr_prbs_pgen(capi_phy_info_t* phy_info_ptr, 
                                                 cw_mode_parameter_t *cw_mode_ptr, 
                                                 cw_port_config_t *port_cfg_ptr, 
                                                 capi_prbs_cfg_t* pcfg,
                                                 cfg_egr_or_igr_t egr_or_igr)
{
    pam4_pgen_misc_cfg_t pam4_pgen_misc_struct_inst = {0};
    capi_phy_info_t capi_phy;

    util_memcpy((void *)&capi_phy, phy_info_ptr, sizeof(capi_phy_info_t));
    pam4_pgen_misc_struct_inst.polysel           = pcfg->poly.fec_pgen_poly; 
    pam4_pgen_misc_struct_inst.pgen_inv          = pcfg->tx_invert;
    pam4_pgen_misc_struct_inst.pgen_init         = 1 ;
    pam4_pgen_misc_struct_inst.fec_mode_ena      = ON;
    pam4_pgen_misc_struct_inst.prbs_on_fault_ena = OFF;
    /* Set QC_PGEN_CFG0_REGISTER:MD_PGEN_1PAM4_0NRZ to 1 regardless of NRZ or PAM4 */
    pam4_pgen_misc_struct_inst.pam4_mode         = ON;
    pam4_pgen_misc_struct_inst.nrz_ctrl          =0;

    cw_mode_ptr->line_fec_prbs = PRBS_ON;

    //Reset assertion for all the blocks which are running in CDR clock
    //Clock disable for all blocks which are running in CDR clock - Stops all the clocks
    //
    if ((cw_mode_ptr->fec_dec_enc_mode == FEC_DEC_FWD) ||
         (cw_mode_ptr->fec_dec_enc_mode == FEC_DEC_ENC)) {

             capi_phy.base_addr = OCTAL_TOP_REGS + ml_cw_rtmr_get_base_addr(egr_or_igr, cw_mode_ptr);
             //FEC_ENC/FEC_SYMB
             chal_cw_rtmr_fec_enc_reset_control(&capi_phy, cw_mode_ptr, RST_ASSERTED, egr_or_igr);
             chal_cw_rtmr_fec_enc_clk_gate_control(&capi_phy, cw_mode_ptr, IS_DISABLED, egr_or_igr);
             chal_cw_rtmr_fec_symb_dist_reset_control(&capi_phy, cw_mode_ptr, RST_ASSERTED, egr_or_igr);
             chal_cw_rtmr_fec_symb_dist_clk_gate_control(&capi_phy, cw_mode_ptr, IS_DISABLED, egr_or_igr);
     }
     
     //PFIFO - For FEC output
     if ((cw_mode_ptr->fec_dec_enc_mode == FEC_DEC_FWD) ||
         (cw_mode_ptr->fec_dec_enc_mode == FEC_DEC_ENC)) {
             capi_phy.base_addr = OCTAL_TOP_REGS + ml_cw_rtmr_get_base_addr(egr_or_igr, cw_mode_ptr);
             chal_cw_rtmr_tmt_pfifo_reset_control(&capi_phy, cw_mode_ptr, RST_ASSERTED, egr_or_igr);
             chal_cw_rtmr_tmt_pfifo_clk_gate_control(&capi_phy, cw_mode_ptr, IS_DISABLED, egr_or_igr);
     }

     //FEC output
     if ((cw_mode_ptr->fec_dec_enc_mode == FEC_DEC_FWD) ||
         (cw_mode_ptr->fec_dec_enc_mode == FEC_DEC_ENC)) {
             capi_phy.base_addr = OCTAL_TOP_REGS + ml_cw_rtmr_get_base_addr(egr_or_igr, cw_mode_ptr);
             chal_cw_rtmr_fec_rcv_tmt_clk_mux_cfg(&capi_phy, cw_mode_ptr, 0x1, egr_or_igr); 
     }

      //FEC_ENC/FEC_SYMB
      if ((cw_mode_ptr->fec_dec_enc_mode == FEC_DEC_FWD) ||
      (cw_mode_ptr->fec_dec_enc_mode == FEC_DEC_ENC)) {
             capi_phy.base_addr = OCTAL_TOP_REGS + ml_cw_rtmr_get_base_addr(egr_or_igr, cw_mode_ptr);
             chal_cw_rtmr_fec_enc_clk_gate_control(&capi_phy, cw_mode_ptr, IS_ENABLED, egr_or_igr);
             chal_cw_rtmr_fec_symb_dist_clk_gate_control(&capi_phy, cw_mode_ptr, IS_ENABLED, egr_or_igr);
      }

    //Config input data mux to encoder to take prbs data . Note that this is not perlane register. So needs to be moved elsewhere. 
    capi_phy.base_addr = OCTAL_TOP_REGS + ml_cw_rtmr_get_base_addr(egr_or_igr, cw_mode_ptr);
    chal_pam4_pmon_fec_prbs_fec_ip_data_mux_cfg (&capi_phy, egr_or_igr, port_cfg_ptr, cw_mode_ptr); 

    chal_pam4_pgen_fec_prbs_clk_cfg(&capi_phy, egr_or_igr, port_cfg_ptr, cw_mode_ptr); 

    chal_cw_rtmr_fec_prbs_pgen_config_wrapper(phy_info_ptr, egr_or_igr, &pam4_pgen_misc_struct_inst, port_cfg_ptr, cw_mode_ptr);
    return RR_SUCCESS;
}

/**
* @brief      kp4_config_rtmr_prbs_egr_line_pgen(capi_phy_info_t* phy_info_ptr, cw_mode_parameter_t *cw_mode_ptr, cw_port_config_t *port_cfg_ptr, capi_prbs_cfg_t* pcfg)
* @details    This API is used to config PRBS LINE PGEN in retimer mode
*
* @param[in]  phy_info_ptr:
* @param[in]  cw_mode_ptr: 
* @param[in]  port_cfg_ptr: 
* @param[in]  pcfg: 
* 
* @return     returns the performance result of the called method/function
*/
static return_result_t kp4_config_rtmr_prbs_egr_line_pgen(capi_phy_info_t* phy_info_ptr, cw_mode_parameter_t *cw_mode_ptr, cw_port_config_t *port_cfg_ptr, capi_prbs_cfg_t* pcfg)
{  
    return_result_t return_rslt;
    capi_phy_info_t capi_phy_info;

    // set mux to PRBS data
    util_memcpy(&capi_phy_info, phy_info_ptr, sizeof(capi_phy_info_t));
    capi_phy_info.base_addr += ml_cw_rtmr_get_base_addr(EGR, cw_mode_ptr);
    chal_cw_rtmr_fec_enc_datamux_cfg(&capi_phy_info, cw_mode_ptr, port_cfg_ptr, PRBS_DATA);

    return_rslt = kp4_config_rtmr_prbs_pgen(phy_info_ptr, cw_mode_ptr, port_cfg_ptr, pcfg, EGR);
    return return_rslt;
}

/**
* @brief      kp4_config_rtmr_prbs_igr_host_pgen(capi_phy_info_t* phy_info_ptr, cw_mode_parameter_t *cw_mode_ptr, cw_port_config_t *port_cfg_ptr, capi_prbs_cfg_t* pcfg)
* @details    This API is used to config PRBS HOST PGEN in retimer mode
*
* @param[in]  phy_info_ptr:
* @param[in]  cw_mode_ptr: 
* @param[in]  port_cfg_ptr: 
* @param[in]  pcfg: 
* 
* @return     returns the performance result of the called method/function
*/
static return_result_t kp4_config_rtmr_prbs_igr_host_pgen(capi_phy_info_t* phy_info_ptr, cw_mode_parameter_t *cw_mode_ptr, cw_port_config_t *port_cfg_ptr, capi_prbs_cfg_t* pcfg)
{
    return_result_t return_rslt;
    capi_phy_info_t capi_phy_info;

    // set mux to PRBS data
    util_memcpy(&capi_phy_info, phy_info_ptr, sizeof(capi_phy_info_t));
    capi_phy_info.base_addr += ml_cw_rtmr_get_base_addr(IGR, cw_mode_ptr);
    chal_cw_rtmr_fec_enc_datamux_cfg(&capi_phy_info, cw_mode_ptr, port_cfg_ptr, PRBS_DATA);

    return_rslt = kp4_config_rtmr_prbs_pgen(phy_info_ptr, cw_mode_ptr, port_cfg_ptr, pcfg, IGR);
    return return_rslt;
}

/**
 * @brief       uint16_t kp4_rtmr_egr_get_pcsfec_lock(capi_phy_info_t* phy_info_ptr, cw_mode_parameter_t *cw_mode_ptr, cw_port_config_t *port_cfg_ptr)
 * @details     in retimer egress, check PCSFEC lock
 * @public      any further information for the public domain
 * @private     any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 * @param[in]  port_ptr  port pointer
 * @return void
 */
static uint16_t kp4_rtmr_egr_get_pcsfec_lock(capi_phy_info_t* phy_info_ptr, cw_mode_parameter_t *cw_mode_ptr, cw_port_config_t *port_cfg_ptr)
 {
    capi_phy_info_t capi_phy;

    util_memcpy((void *)(&capi_phy), phy_info_ptr, sizeof(capi_phy_info_t));
    capi_phy.base_addr = OCTAL_TOP_REGS + ml_cw_rtmr_get_base_addr(EGR, cw_mode_ptr);

     //Check FEC or PCS sync status
     if (cw_mode_ptr->fec_dec_enc_mode== PCS_XENC)
        return (chal_cw_rtmr_pcs_sync_get_status(&capi_phy, cw_mode_ptr, port_cfg_ptr));
    else
        return (chal_cw_rtmr_fec_sync_status(&capi_phy, cw_mode_ptr, port_cfg_ptr));
}

/**
* @brief      kp4_enable_rtmr_prbs_egr_host_pmon(capi_phy_info_t* phy_info_ptr, cw_mode_parameter_t *cw_mode_ptr, cw_port_config_t *port_cfg_ptr, uint8_t enable)
* @details    This API is used to enable HOST PMON in retimer mode
*
* @param[in]  phy_info_ptr:  phy info pointer
* @param[in]  cw_mode_ptr:  CW mode config structure pointer
* @param[in]  port_cfg_ptr: Port config structure pointer
* @param[in]  enable:  PRBS enable (1) or disable (0)
* 
* @return     returns the performance result of the called method/function
*/
static return_result_t kp4_enable_rtmr_prbs_egr_host_pmon(capi_phy_info_t* phy_info_ptr, cw_mode_parameter_t *cw_mode_ptr, cw_port_config_t *port_cfg_ptr, uint8_t enable)
{
    uint16_t fec_lock_cnt=0;
    
    /* FEC PRBS Gen/Chk Config */    
    if(enable) {
       /** check fec_lock status*/
        while( kp4_rtmr_egr_get_pcsfec_lock(phy_info_ptr, cw_mode_ptr, port_cfg_ptr)==0 && (fec_lock_cnt++<CW_FEC_LOCK_TIMER_CNT))
            delay_ms(10);
        if(kp4_rtmr_egr_get_pcsfec_lock(phy_info_ptr, cw_mode_ptr, port_cfg_ptr)==0){
            printf("ERROR  --  KP4 HOST FEC is failed to get locked!\r\n");
            return RR_ERROR;
        }
    }

    chal_cw_rtmr_fec_prbs_pmon_enable_disable_wrapper(phy_info_ptr, EGR, port_cfg_ptr, cw_mode_ptr, enable);
    return RR_SUCCESS;
}

static return_result_t kp4_enable_rtmr_prbs_pgen(capi_phy_info_t* phy_info_ptr, 
                                                 cw_mode_parameter_t *cw_mode_ptr, 
                                                 cw_port_config_t *port_cfg_ptr, 
                                                 cfg_egr_or_igr_t egr_or_igr, 
                                                 uint8_t enable)
{
    uint8_t lock_flag;
    capi_phy_info_t capi_phy;
    cw_mode_ptr->line_fec_prbs = PRBS_ON;

    util_memcpy((void *)&capi_phy, phy_info_ptr, sizeof(capi_phy_info_t));
    //Release resets from downstream to upstream
    //Since clocks are changed to CMU
    //PFIFO and GBOX
    if ((cw_mode_ptr->fec_dec_enc_mode == FEC_DEC_FWD) ||
         (cw_mode_ptr->fec_dec_enc_mode == FEC_DEC_ENC)) {
             capi_phy.base_addr = OCTAL_TOP_REGS + ml_cw_rtmr_get_base_addr(egr_or_igr, cw_mode_ptr);
             chal_cw_rtmr_tmt_pfifo_reset_control(&capi_phy, cw_mode_ptr, RST_DEASSERTED, egr_or_igr); 

             //PFIFO - For FEC output
             chal_cw_rtmr_tmt_pfifo_clk_gate_control(&capi_phy, cw_mode_ptr, IS_ENABLED, egr_or_igr);

             //FEC symbol distribution
             chal_cw_rtmr_fec_symb_dist_reset_control(&capi_phy, cw_mode_ptr, RST_DEASSERTED, egr_or_igr);
             //FEC encoder reset release
             chal_cw_rtmr_fec_enc_reset_control(&capi_phy, cw_mode_ptr, RST_DEASSERTED, egr_or_igr);
     }

    lock_flag = chal_cw_rtmr_fec_prbs_pgen_enable_disable_wrapper(phy_info_ptr, egr_or_igr, port_cfg_ptr, cw_mode_ptr, enable);

    if (lock_flag == 0 && enable == CAPI_SWITCH_ON)
        return RR_ERROR;
    else
        return RR_SUCCESS;
}

/**
* @brief      kp4_enable_rtmr_prbs_egr_line_pgen(capi_phy_info_t* phy_info_ptr, cw_mode_parameter_t *cw_mode_ptr, cw_port_config_t *port_cfg_ptr, uint8_t enable)
* @details    This API is used to enable LINE PGEN in retimer mode
*
* @param[in]  phy_info_ptr:  phy info pointer
* @param[in]  cw_mode_ptr:  CW mode config structure pointer
* @param[in]  port_cfg_ptr: Port config structure pointer
* @param[in]  enable:  PRBS enable (1) or disable (0)
* 
* @return     returns the performance result of the called method/function
*/
static return_result_t kp4_enable_rtmr_prbs_egr_line_pgen(capi_phy_info_t* phy_info_ptr, cw_mode_parameter_t *cw_mode_ptr, cw_port_config_t *port_cfg_ptr, uint8_t enable)
{
    return_result_t return_rslt;

    return_rslt = kp4_enable_rtmr_prbs_pgen(phy_info_ptr, cw_mode_ptr, port_cfg_ptr, EGR, enable);
    return return_rslt;
}

/**
* @brief      kp4_enable_rtmr_prbs_igr_host_pgen(capi_phy_info_t* phy_info_ptr, cw_mode_parameter_t *cw_mode_ptr, cw_port_config_t *port_cfg_ptr, uint8_t enable)
* @details    This API is used to enable HOST PGEN in retimer mode
*
* @param[in]  phy_info_ptr:  phy info pointer
* @param[in]  cw_mode_ptr:  CW mode config structure pointer
* @param[in]  port_cfg_ptr: Port config structure pointer
* @param[in]  enable:  PRBS enable (1) or disable (0)
* 
* @return     returns the performance result of the called method/function
*/          
static return_result_t kp4_enable_rtmr_prbs_igr_host_pgen(capi_phy_info_t* phy_info_ptr, cw_mode_parameter_t *cw_mode_ptr, cw_port_config_t *port_cfg_ptr, uint8_t enable)
{
    return_result_t return_rslt;

    return_rslt = kp4_enable_rtmr_prbs_pgen(phy_info_ptr, cw_mode_ptr, port_cfg_ptr, IGR, enable);
    return return_rslt;
}

/**
 * @brief       kp4_rtmr_igr_get_pcsfec_lock(capi_phy_info_t* phy_info_ptr, cw_mode_parameter_t *cw_mode_ptr, cw_port_config_t *port_cfg_ptr)
 * @details     in retimer ingress, check PCSFEC lock
 * @public      any further information for the public domain
 * @private     any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 * @param[in]  port_ptr  port pointer
 * @return void
 */
uint16_t kp4_rtmr_igr_get_pcsfec_lock(capi_phy_info_t* phy_info_ptr, cw_mode_parameter_t *cw_mode_ptr, cw_port_config_t *port_cfg_ptr)
{
   capi_phy_info_t capi_phy;

   util_memcpy((void *)(&capi_phy), phy_info_ptr, sizeof(capi_phy_info_t));
   capi_phy.base_addr = OCTAL_TOP_REGS + ml_cw_rtmr_get_base_addr(IGR, cw_mode_ptr);

   return (chal_cw_rtmr_fec_sync_status(&capi_phy, cw_mode_ptr, port_cfg_ptr));
}

/**
* @brief      kp4_enable_rtmr_prbs_igr_line_pmon(capi_phy_info_t* phy_info_ptr, cw_mode_parameter_t *cw_mode_ptr, cw_port_config_t *port_cfg_ptr, uint8_t enable)
* @details    This API is used to enable LINE PMON in retimer mode
*
* @param[in]  phy_info_ptr:  phy info pointer
* @param[in]  cw_mode_ptr:  CW mode config structure pointer
* @param[in]  port_cfg_ptr: Port config structure pointer
* @param[in]  enable:  PRBS enable (1) or disable (0)
* 
* @return     returns the performance result of the called method/function
*/
static return_result_t kp4_enable_rtmr_prbs_igr_line_pmon(capi_phy_info_t* phy_info_ptr, cw_mode_parameter_t *cw_mode_ptr, cw_port_config_t *port_cfg_ptr, uint8_t enable)
{
    uint16_t fec_lock_cnt = 0;

    /* FEC PRBS Gen/Chk Config */    
    if(enable) {
       /** check fec_lock status*/
        while(kp4_rtmr_igr_get_pcsfec_lock(phy_info_ptr,cw_mode_ptr,  port_cfg_ptr)==0 && (fec_lock_cnt++<CW_FEC_LOCK_TIMER_CNT))
            delay_ms(10);
        if(kp4_rtmr_igr_get_pcsfec_lock(phy_info_ptr,cw_mode_ptr,  port_cfg_ptr)==0){
            printf("ERROR  --  KP4 LINE FEC is failed to get locked!\r\n");
            
            return RR_ERROR;
        }
    }

    chal_cw_rtmr_fec_prbs_pmon_enable_disable_wrapper(phy_info_ptr, IGR, port_cfg_ptr, cw_mode_ptr, enable);
    return RR_SUCCESS;
}

static return_result_t host_diag_cw_rptr_fec_pgen_config_handler(phy_info_t* phy_info_ptr, capi_fec_mode_t fec_mode, capi_enable_t enable)
{
    capi_config_ex_info_t config_info; 
    cw_mode_parameter_t mode_parameter;
    cw_port_config_t port_cfg;
    phy_info_t       phy_info= {0};
    cw_port_info_t port_info;
    cfg_egr_or_igr_t egr_or_igr = EGR;
    
    phy_info.phy_id = phy_info_ptr->phy_id;
    phy_info.base_addr = OCTAL_TOP_REGS;

    phy_info_ptr->lane_mask &= 0xFF;    /*to get rid of other command handling for the 0x8000 flag*/
    util_memset(&config_info, 0, sizeof(capi_config_ex_info_t));

    if (fec_mode == CAPI_FEC_LINE) {
        config_info.line_lane.lane_mask = phy_info_ptr->lane_mask;   /* Using line-side lane_mask for identifying PORT */
        egr_or_igr = EGR;
    } else if (fec_mode == CAPI_FEC_CLIENT) {
        config_info.host_lane.lane_mask = phy_info_ptr->lane_mask;   /* Using host-side lane_mask for identifying PORT */
        egr_or_igr = IGR;
    }
    
    /*retrieve the chip config mode*/
    ERR_FUNC(host_diag_cw_get_chip_config(&phy_info, &config_info, &port_info));

    if (egr_or_igr == EGR)
        config_info.egr_fec_term = CHIP_LANE_FEC_DEC_XDEC_XENC_ENC;
    else
        config_info.igr_fec_term = CHIP_LANE_FEC_DEC_XDEC_XENC_ENC;
    host_diag_cw_kp4_gen_port_cfg((capi_function_mode_t)config_info.func_mode, port_info.port_idx, &port_cfg);
    host_diag_cw_kp4_gen_cw_mode_cfg(&config_info, &port_info, egr_or_igr, &port_cfg, &mode_parameter);

    phy_info.base_addr = OCTAL_TOP_REGS;

    chal_cw_release_rtm_reset(&phy_info);
    if(egr_or_igr==EGR)
        chal_cw_release_egr_reset(&phy_info, RETIMER_PATH);
    else
        chal_cw_release_igr_reset(&phy_info, RETIMER_PATH);
    if (enable == CAPI_ENABLE){
        ml_cw_rtmr_cfg_datapath_init_handler(&phy_info, &mode_parameter, &port_cfg, egr_or_igr, port_info.port_type);
        ml_cw_rtmr_cfg_clock_reset_mux_handler(&phy_info, &mode_parameter, &port_cfg, egr_or_igr);
        ml_cw_rtmr_cfg_lane_clk_handler(&phy_info, &mode_parameter, &port_cfg, IS_ENABLED, egr_or_igr);
        ml_cw_rtmr_cfg_lane_reset_handler(&phy_info, &mode_parameter, &port_cfg, RST_DEASSERTED, egr_or_igr);
        phy_info.base_addr = OCTAL_TOP_REGS + ml_cw_rtmr_get_base_addr(EGR, &mode_parameter);
        ml_cw_rtmr_cfg_trnsmt_func_blk_clk_and_rst_handler(&phy_info, &mode_parameter, &port_cfg, RST_DEASSERTED, egr_or_igr);
    }else{
        phy_info.base_addr = OCTAL_TOP_REGS + ml_cw_rtmr_get_base_addr(EGR, &mode_parameter);
        ml_cw_rtmr_cfg_trnsmt_func_blk_clk_and_rst_handler(&phy_info, &mode_parameter, &port_cfg, RST_ASSERTED, egr_or_igr);
        // do not assert top level clock and reset since it will affect repeater mode FEC monitor if it is enabled
        // ml_cw_rtmr_cfg_lane_reset_handler(&phy_info, &mode_parameter, &port_cfg, RST_ASSERTED, egr_or_igr);
        // ml_cw_rtmr_cfg_lane_clk_handler(&phy_info, &mode_parameter, IS_DISABLED, egr_or_igr);
    }
    return RR_SUCCESS;
}

static return_result_t host_diag_cw_rptr_fec_pmon_config_handler(phy_info_t*   phy_info_ptr, 
    capi_fec_mode_t fec_mode, 
    capi_function_mode_t func_mode,
    uint8_t port_idx,
    cw_mode_parameter_t *cw_mode_ptr,
    capi_enable_t enable)
{
    capi_rptr_fec_st_t rptr_fec_st = {0};
    phy_info_t   phy_info = *phy_info_ptr;
    boolean fec_stats_enabed;
    
    // check if FEC stats monitor is enabled, skip FEC monitor enable/disable if it is on, otherwise it will disrupt FEC stats monitor
    phy_info.base_addr = host_diag_cw_get_bbaddr_kp4deca(fec_mode, func_mode, port_idx, cw_mode_ptr);
    ERR_FUNC(host_diag_cw_rtmr_get_kpr4fec_dec_stat(&phy_info, &fec_stats_enabed));

    if (fec_stats_enabed)
        return RR_SUCCESS;
    
    rptr_fec_st.fec_mode = fec_mode;
    rptr_fec_st.is_enable = (uint8_t) enable;
    rptr_fec_st.sampling_rate = 100;    // 100% sampling rate

     return host_diag_set_rptr_fec_monitor(phy_info_ptr, &rptr_fec_st);
}

/**
* @brief      host_fec_set_prbs_gen(capi_phy_info_t* phy_info_ptr, capi_prbs_info_t* prbs_info_ptr)
* @details    This API is used to configure the KP4 PRBS pattern generator and enable or disable it
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  prbs_info_ptr: this pointer contain PRBS config info
* 
* @return     returns the performance result of the called method/function
*/
return_result_t host_fec_set_prbs_gen(capi_phy_info_t* phy_info_ptr, capi_prbs_info_t* prbs_info_ptr)
{
    capi_config_ex_info_t config_info;
    capi_phy_info_t phy_info, phy_info_dsp;
    cw_mode_parameter_t cw_mode;
    cw_port_config_t port_cfg;
    cw_port_info_t port_info;
    return_result_t rtrn_rslt = RR_SUCCESS;
    capi_lane_ctrl_info_t capi_lane_ctrl_info;
    capi_enable_t enbl;
    cfg_egr_or_igr_t egr_or_igr = (prbs_info_ptr->ptype == CAPI_PRBS_KP4_MEDIA_GENERATOR) ? EGR : IGR;
    uint16_t fec_term;

    util_memcpy((void*)&phy_info, phy_info_ptr, sizeof(capi_phy_info_t)); 
    util_memcpy((void*)&phy_info_dsp, phy_info_ptr, sizeof(capi_phy_info_t)); 
    util_memset(&capi_lane_ctrl_info, 0, sizeof(capi_lane_ctrl_info_t));
    phy_info.base_addr = APB2_SLV0_REGS; 

    if (phy_info_ptr->lane_mask == 0) {
        CAPI_LOG_ERROR("Invalid Lane Mask. %d \r\n", phy_info_ptr->lane_mask);
        return RR_ERROR_INITIALIZATION;
    }
    if (((prbs_info_ptr->ptype != CAPI_PRBS_KP4_HOST_GENERATOR) && (prbs_info_ptr->ptype != CAPI_PRBS_KP4_MEDIA_GENERATOR)) ||
        (phy_info_ptr->core_ip!=CORE_IP_CW)) {
        CAPI_LOG_ERROR("Wrong input for side. %d \r\n", prbs_info_ptr->ptype);
        return RR_ERROR_WRONG_INPUT_VALUE;
    }

    util_memset(&config_info, 0, sizeof(capi_config_ex_info_t));
    if (prbs_info_ptr->ptype == CAPI_PRBS_KP4_MEDIA_GENERATOR) {
        config_info.line_lane.lane_mask = phy_info_ptr->lane_mask;   /* Using line-side lane_mask for identifying PORT */
    } else if (prbs_info_ptr->ptype == CAPI_PRBS_KP4_HOST_GENERATOR) {
        config_info.host_lane.lane_mask = phy_info_ptr->lane_mask;   /* Using host-side lane_mask for identifying PORT */
    }
    /*retrieve the config information*/
    ERR_FUNC(host_diag_cw_get_chip_config(&phy_info, &config_info, &port_info));

    /* FEC PRBS Gen/Chk Config */
    enbl = (prbs_info_ptr->gen_switch) ? CAPI_ENABLE : CAPI_DISABLE;

    fec_term = (egr_or_igr == EGR) ? config_info.egr_fec_term : config_info.igr_fec_term; 

    /*Build the KP4 port information*/
    host_diag_cw_kp4_gen_port_cfg((capi_function_mode_t)config_info.func_mode, port_info.port_idx, &port_cfg);
    host_diag_cw_kp4_gen_cw_mode_cfg(&config_info, &port_info, egr_or_igr, &port_cfg, &cw_mode);

    /* FEC PRBS Gen/Chk Config */
    phy_info_dsp.core_ip = prbs_info_ptr->ptype == CAPI_PRBS_KP4_MEDIA_GENERATOR ? CORE_IP_MEDIA_SERDES : CORE_IP_HOST_SERDES;
    phy_info_dsp.lane_mask = prbs_info_ptr->ptype == CAPI_PRBS_KP4_MEDIA_GENERATOR ? config_info.line_lane.lane_mask : config_info.host_lane.lane_mask;
    if(enbl == CAPI_ENABLE) {
        if(prbs_info_ptr->ptype == CAPI_PRBS_KP4_MEDIA_GENERATOR) {    /*LINE PGEN*/
            if (fec_term == CHIP_PORT_FEC_TERM_BYPASS || fec_term == CHIP_PORT_FEC_DEC_FWD)
                ERR_FUNC(host_diag_cw_rptr_fec_pgen_config_handler(&phy_info, CAPI_FEC_LINE, enbl));
            
            ERR_FUNC(host_fec_ignore_fault (&phy_info, (uint8_t) config_info.line_lane.lane_mask, DIR_EGRESS, enbl, (uint8_t)port_info.port_idx));
        }else {    /*HOST PGEN*/
            if (fec_term == CHIP_PORT_FEC_TERM_BYPASS || fec_term == CHIP_PORT_FEC_DEC_FWD)
                ERR_FUNC(host_diag_cw_rptr_fec_pgen_config_handler(&phy_info, CAPI_FEC_CLIENT, enbl));
        
            ERR_FUNC(host_fec_ignore_fault (&phy_info, (uint8_t) config_info.host_lane.lane_mask, DIR_INGRESS, enbl, (uint8_t)port_info.port_idx));
        }
        //send fault ignore command to LW
        capi_lane_ctrl_info.param.is.ignore_fault = 1;
        capi_lane_ctrl_info.cmd_value.is.ignore_fault = 1;
        rtrn_rslt = capi_set_lane_ctrl_info(&phy_info_dsp, &capi_lane_ctrl_info);
    }
    if (prbs_info_ptr->ptype == CAPI_PRBS_KP4_MEDIA_GENERATOR) {    /*LINE PGEN*/
        rtrn_rslt = kp4_config_rtmr_prbs_egr_line_pgen(&phy_info, &cw_mode, &port_cfg,   &prbs_info_ptr->op.pcfg);
        rtrn_rslt =  kp4_enable_rtmr_prbs_egr_line_pgen(&phy_info, &cw_mode, &port_cfg, (prbs_info_ptr->gen_switch)?1:0);
    } else if(prbs_info_ptr->ptype == CAPI_PRBS_KP4_HOST_GENERATOR) {    /*HOST PGEN*/
        rtrn_rslt =  kp4_config_rtmr_prbs_igr_host_pgen(&phy_info, &cw_mode, &port_cfg, &prbs_info_ptr->op.pcfg);
        rtrn_rslt = kp4_enable_rtmr_prbs_igr_host_pgen(&phy_info, &cw_mode, &port_cfg, (prbs_info_ptr->gen_switch)?1:0);
    }
    if(enbl == CAPI_DISABLE) {
        if(prbs_info_ptr->ptype == CAPI_PRBS_KP4_MEDIA_GENERATOR) {    /*LINE PGEN*/
            if (fec_term == CHIP_PORT_FEC_TERM_BYPASS || fec_term == CHIP_PORT_FEC_DEC_FWD)
                ERR_FUNC(host_diag_cw_rptr_fec_pgen_config_handler(&phy_info, CAPI_FEC_LINE, enbl));
            ERR_FUNC(host_fec_ignore_fault (&phy_info, (uint8_t) config_info.line_lane.lane_mask, DIR_EGRESS, enbl, (uint8_t)port_info.port_idx));
            ERR_FUNC(host_fec_reset_port((&phy_info), (1<<port_info.port_idx) ));            
        }else{    /*HOST PGEN*/
            if (fec_term == CHIP_PORT_FEC_TERM_BYPASS || fec_term == CHIP_PORT_FEC_DEC_FWD)
                ERR_FUNC(host_diag_cw_rptr_fec_pgen_config_handler(&phy_info, CAPI_FEC_CLIENT, enbl));
            ERR_FUNC(host_fec_ignore_fault (&phy_info, (uint8_t) config_info.host_lane.lane_mask, DIR_INGRESS, enbl, (uint8_t)port_info.port_idx));
            ERR_FUNC(host_fec_reset_port((&phy_info), (1<<(port_info.port_idx+8)) ));
        }
        //send fault ignore command to LW
        capi_lane_ctrl_info.param.is.ignore_fault = 1;
        capi_lane_ctrl_info.cmd_value.is.ignore_fault = 0;
        rtrn_rslt = capi_set_lane_ctrl_info(&phy_info_dsp, &capi_lane_ctrl_info);
    }

    return rtrn_rslt;
}

/**
* @brief      host_fec_set_prbs_mon(capi_phy_info_t* phy_info_ptr, capi_prbs_info_t* prbs_info_ptr)
* @details    This API is used to configure the KP4 PRBS pattern monitor and enable or disable it
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  prbs_info_ptr: this pointer contain PRBS config info
* 
* @return     returns the performance result of the called method/function
*/
return_result_t host_fec_set_prbs_mon(capi_phy_info_t* phy_info_ptr, capi_prbs_info_t* prbs_info_ptr)
{
    capi_config_ex_info_t config_info;
    capi_phy_info_t phy_info;
    cw_mode_parameter_t cw_mode;
    cw_port_config_t port_cfg;
    cw_port_info_t port_info;
    return_result_t rtrn_rslt = RR_SUCCESS;
    capi_lane_ctrl_info_t capi_lane_ctrl_info;
    capi_enable_t enbl = (prbs_info_ptr->gen_switch) ? CAPI_ENABLE : CAPI_DISABLE;
    cfg_egr_or_igr_t egr_or_igr = (prbs_info_ptr->ptype == CAPI_PRBS_KP4_HOST_MONITOR) ? EGR : IGR;
    uint16_t fec_term;

    util_memcpy((void*)&phy_info, phy_info_ptr, sizeof(capi_phy_info_t)); 
    util_memset(&capi_lane_ctrl_info, 0, sizeof(capi_lane_ctrl_info_t));
    phy_info.base_addr = APB2_SLV0_REGS; 

    if (phy_info_ptr->lane_mask == 0) {
        CAPI_LOG_ERROR("Invalid Lane Mask. %d \r\n", phy_info_ptr->lane_mask);
        return RR_ERROR_INITIALIZATION;
    }
    if ((prbs_info_ptr->ptype != CAPI_PRBS_KP4_MEDIA_MONITOR) && (prbs_info_ptr->ptype != CAPI_PRBS_KP4_HOST_MONITOR)) {
        CAPI_LOG_ERROR("Wrong input for side. %d \r\n", prbs_info_ptr->ptype);
        return RR_ERROR_WRONG_INPUT_VALUE;
    }

    util_memset(&config_info, 0, sizeof(capi_config_ex_info_t));
    if (prbs_info_ptr->ptype == CAPI_PRBS_KP4_MEDIA_MONITOR) {
        config_info.line_lane.lane_mask = phy_info_ptr->lane_mask;   /* Using line-side lane_mask for identifying PORT */
    } else if (prbs_info_ptr->ptype == CAPI_PRBS_KP4_HOST_MONITOR) {
        config_info.host_lane.lane_mask = phy_info_ptr->lane_mask;   /* Using host-side lane_mask for identifying PORT */
    }
    /*retrieve the config information*/
    ERR_FUNC(host_diag_cw_get_chip_config(&phy_info, &config_info, &port_info));

    /*Build the KP4 port information*/
    host_diag_cw_kp4_gen_port_cfg((capi_function_mode_t)config_info.func_mode, port_info.port_idx, &port_cfg);
    host_diag_cw_kp4_gen_cw_mode_cfg(&config_info, &port_info, egr_or_igr, &port_cfg, &cw_mode);
    
    /* FEC PRBS Gen/Chk Config */
    fec_term = (egr_or_igr == EGR) ? config_info.egr_fec_term : config_info.igr_fec_term; 
    if (prbs_info_ptr->ptype == CAPI_PRBS_KP4_HOST_MONITOR) {    /*HOST PMON*/
        if(fec_term == CHIP_PORT_FEC_TERM_BYPASS)
            ERR_FUNC(host_diag_cw_rptr_fec_pmon_config_handler(&phy_info, CAPI_FEC_CLIENT,(capi_function_mode_t)config_info.func_mode, port_info.port_idx, &cw_mode, enbl));
        ERR_FUNC(kp4_config_rtmr_prbs_egr_host_pmon(&phy_info, &cw_mode, &port_cfg, &prbs_info_ptr->op.pcfg));
        rtrn_rslt = kp4_enable_rtmr_prbs_egr_host_pmon(&phy_info, &cw_mode, &port_cfg, (uint8_t) enbl);
    }
    else if (prbs_info_ptr->ptype == CAPI_PRBS_KP4_MEDIA_MONITOR) {    /*LINE PMON*/
        if (fec_term == CHIP_PORT_FEC_TERM_BYPASS)
            ERR_FUNC(host_diag_cw_rptr_fec_pmon_config_handler(&phy_info, CAPI_FEC_LINE, (capi_function_mode_t)config_info.func_mode, port_info.port_idx, &cw_mode, enbl));
        
        ERR_FUNC(kp4_config_rtmr_prbs_igr_line_pmon(&phy_info, &cw_mode, &port_cfg, &prbs_info_ptr->op.pcfg));        
        rtrn_rslt = kp4_enable_rtmr_prbs_igr_line_pmon(&phy_info, &cw_mode, &port_cfg, (uint8_t) enbl);
    }

    return rtrn_rslt;
}

/**
* @brief      host_fec_get_prbs_gen(capi_phy_info_t* phy_info_ptr, capi_prbs_info_t* prbs_info_ptr)
* @details    This API is to get the configuration setting of FEC PRBS pattern generator
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[out] prbs_info_ptr: this pointer return PRBS config information
* 
* @return     returns the performance result of the called method/function
*/
return_result_t host_fec_get_prbs_gen(capi_phy_info_t* phy_info_ptr, capi_prbs_info_t* prbs_info_ptr)
{
    capi_config_ex_info_t config_info;
    capi_phy_info_t phy_info;
    cw_mode_parameter_t cw_mode;
    cw_port_config_t port_cfg;
    cw_port_info_t port_info;
    cfg_egr_or_igr_t egr_or_igr;

    util_memcpy((void*)&phy_info, phy_info_ptr, sizeof(capi_phy_info_t));
    phy_info.base_addr = APB2_SLV0_REGS; 

    if (phy_info_ptr->lane_mask == 0) {
        CAPI_LOG_ERROR("Invalid Lane Mask. %d \r\n", phy_info_ptr->lane_mask);
        return RR_ERROR_INITIALIZATION;
    }
    if (((prbs_info_ptr->ptype != CAPI_PRBS_KP4_HOST_GENERATOR) && (prbs_info_ptr->ptype != CAPI_PRBS_KP4_MEDIA_GENERATOR)) ||
        (phy_info_ptr->core_ip!=CORE_IP_CW)) {
        CAPI_LOG_ERROR("Wrong input for side. %d \r\n", prbs_info_ptr->ptype);
        return RR_ERROR_WRONG_INPUT_VALUE;
    }
    util_memset(&config_info, 0, sizeof(capi_config_ex_info_t));
    if (prbs_info_ptr->ptype == CAPI_PRBS_KP4_MEDIA_GENERATOR) {
        config_info.line_lane.lane_mask = phy_info_ptr->lane_mask;   /* Using line-side lane_mask for identifying PORT */
    } else if (prbs_info_ptr->ptype == CAPI_PRBS_KP4_HOST_GENERATOR) {
        config_info.host_lane.lane_mask = phy_info_ptr->lane_mask;   /* Using host-side lane_mask for identifying PORT */
    }
    ERR_FUNC(host_diag_cw_get_chip_config(&phy_info, &config_info, &port_info));

    if (prbs_info_ptr->ptype == CAPI_PRBS_KP4_HOST_GENERATOR) 
        egr_or_igr = IGR;
    else
        egr_or_igr = EGR;

    /*Build the KP4 port information*/
    host_diag_cw_kp4_gen_port_cfg(config_info.func_mode, port_info.port_idx, &port_cfg);
    host_diag_cw_kp4_gen_cw_mode_cfg(&config_info, &port_info, egr_or_igr, &port_cfg, &cw_mode);

    /* FEC PRBS Gen/Chk Config */
    prbs_info_ptr->op.pcfg.poly.fec_pgen_poly = chal_cw_rtmr_pgen_get_poly(&phy_info, egr_or_igr, &port_cfg, &cw_mode);
    prbs_info_ptr->op.pcfg.tx_invert =  chal_cw_rtmr_pgen_get_inv(&phy_info, egr_or_igr, &port_cfg, &cw_mode);
    prbs_info_ptr->gen_switch = chal_cw_rtmr_fec_pgen_get_active_status(&phy_info, egr_or_igr, &port_cfg, &cw_mode);
    return RR_SUCCESS;
}

/**
* @brief      host_fec_get_prbs_mon(capi_phy_info_t* phy_info_ptr, capi_prbs_info_t* prbs_info_ptr)
* @details    This API is to get the configuration setting of FEC PRBS pattern monitor
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[out] prbs_info_ptr: this pointer return PRBS config information
* 
* @return     returns the performance result of the called method/function
*/
return_result_t host_fec_get_prbs_mon(capi_phy_info_t* phy_info_ptr, capi_prbs_info_t* prbs_info_ptr)
{
    capi_config_ex_info_t config_info;
    capi_phy_info_t phy_info;
    cw_mode_parameter_t cw_mode;
    cw_port_config_t port_cfg;
    cw_port_info_t port_info;
    cfg_egr_or_igr_t egr_or_igr;

    util_memcpy((void*)&phy_info, phy_info_ptr, sizeof(capi_phy_info_t));
    phy_info.base_addr = APB2_SLV0_REGS; 

    if (phy_info_ptr->lane_mask == 0) {
        CAPI_LOG_ERROR("Invalid Lane Mask. %d \r\n", phy_info_ptr->lane_mask);
        return RR_ERROR_INITIALIZATION;
    }
    if ((prbs_info_ptr->ptype != CAPI_PRBS_KP4_HOST_MONITOR) && (prbs_info_ptr->ptype != CAPI_PRBS_KP4_MEDIA_MONITOR)) {
        CAPI_LOG_ERROR("Wrong input for side. %d \r\n", prbs_info_ptr->ptype);
        return RR_ERROR_WRONG_INPUT_VALUE;
    }

    util_memset(&config_info, 0, sizeof(capi_config_ex_info_t));
    if (prbs_info_ptr->ptype == CAPI_PRBS_KP4_MEDIA_MONITOR) {
        config_info.line_lane.lane_mask = phy_info_ptr->lane_mask;   /* Using line-side lane_mask for identifying PORT */
    } else if (prbs_info_ptr->ptype == CAPI_PRBS_KP4_HOST_MONITOR) {
        config_info.host_lane.lane_mask = phy_info_ptr->lane_mask;   /* Using host-side lane_mask for identifying PORT */
    }
    ERR_FUNC(host_diag_cw_get_chip_config(&phy_info, &config_info, &port_info));

    if (prbs_info_ptr->ptype == CAPI_PRBS_KP4_MEDIA_MONITOR) 
        egr_or_igr = IGR;
    else
        egr_or_igr = EGR;

    /*Build the KP4 port information*/
    host_diag_cw_kp4_gen_port_cfg(config_info.func_mode, port_info.port_idx, &port_cfg);
    host_diag_cw_kp4_gen_cw_mode_cfg(&config_info, &port_info, egr_or_igr, &port_cfg, &cw_mode);

    /* FEC PRBS Gen/Chk Config */
    prbs_info_ptr->op.pcfg.poly.fec_pgen_poly = chal_cw_rtmr_pmon_get_poly(&phy_info, egr_or_igr, &port_cfg, &cw_mode);
    prbs_info_ptr->op.pcfg.rx_invert =  chal_cw_rtmr_pmon_get_inv(&phy_info, egr_or_igr, &port_cfg, &cw_mode);
    prbs_info_ptr->gen_switch = chal_cw_rtmr_fec_pmon_get_active_status(&phy_info, egr_or_igr, &port_cfg, &cw_mode);
    return RR_SUCCESS;
}

/**
* @brief     host_fec_get_prbs_status(capi_phy_info_t* phy_info_ptr, capi_prbs_status_t* prbs_st_ptr)
* @details    This API is used to get PRBS status information 
*
* @param[in]  phy_info_ptr:  phy info pointer
* @param[out]  prbs_st_ptr:  PRBS status structure pointer
* 
* @return     returns the performance result of the called method/function
*/          
return_result_t host_fec_get_prbs_status(capi_phy_info_t* phy_info_ptr, capi_prbs_status_t* prbs_st_ptr)
{
    capi_config_ex_info_t config_info;
    phy_info_t phy_info;
    cw_mode_parameter_t cw_mode;
    cw_port_config_t port_cfg;
#ifdef CAPI_CONFIG_CMODE_CMD_INTF
    octal_port_mode_status2_reg_t pmode_st2;
#endif
    cw_port_info_t port_info;
    cfg_egr_or_igr_t egr_or_igr;
    pam4_pmon_stat_t pam4_pmon_stat = {0};

    util_memcpy((void*)&phy_info, phy_info_ptr, sizeof(phy_info_t));
    phy_info.core_ip = CORE_IP_ALL;
    phy_info.base_addr = APB2_SLV0_REGS; 
    
    if (phy_info_ptr->lane_mask == 0) {
        CAPI_LOG_ERROR("Invalid Lane Mask. %d \r\n", phy_info_ptr->lane_mask);
        return RR_ERROR_INITIALIZATION;
    }
    if ((prbs_st_ptr->prbs_type != CAPI_PRBS_KP4_HOST_MONITOR) && (prbs_st_ptr->prbs_type != CAPI_PRBS_KP4_MEDIA_MONITOR)) {
        CAPI_LOG_ERROR("Wrong input for side. %d \r\n", prbs_st_ptr->prbs_type);
        return RR_ERROR_WRONG_INPUT_VALUE;
    }

    util_memset(&config_info, 0, sizeof(capi_config_ex_info_t));
    if (prbs_st_ptr->prbs_type == CAPI_PRBS_KP4_MEDIA_MONITOR) {
        config_info.line_lane.lane_mask = phy_info_ptr->lane_mask;   /* Using line-side lane_mask for identifying PORT */
    } else if (prbs_st_ptr->prbs_type == CAPI_PRBS_KP4_HOST_MONITOR) {
        config_info.host_lane.lane_mask = phy_info_ptr->lane_mask;   /* Using host-side lane_mask for identifying PORT */
    }
#ifdef CAPI_CONFIG_CMODE_CMD_INTF
    ERR_FUNC(capi_get_config(&phy_info, &config_info));
    ERR_HSIP(pmode_st2.words = (uint16_t)hsip_rd_reg_(&phy_info, OCTAL_PORT_MODE_STATUS_2_REG));
    port_idx = pmode_st2.fields.port_index;
#else    
    ERR_FUNC(host_diag_cw_get_chip_config(&phy_info, &config_info, &port_info));
#endif

    if(prbs_st_ptr->prbs_type == CAPI_PRBS_KP4_HOST_MONITOR) {    /*HOST PMON*/
        egr_or_igr = EGR;
    }else {    /*LINE PMON*/
        egr_or_igr = IGR;
    }

    /*Build the KP4 port information*/
    host_diag_cw_kp4_gen_port_cfg(config_info.func_mode, port_info.port_idx, &port_cfg);
    host_diag_cw_kp4_gen_cw_mode_cfg(&config_info, &port_info, egr_or_igr, &port_cfg, &cw_mode);
    
     /* 4.2.3.11.    Check PRBS mon status */
     chal_cw_rtmr_pmon_get_status(&phy_info, egr_or_igr, &port_cfg, &cw_mode, &pam4_pmon_stat);
     prbs_st_ptr->lock = pam4_pmon_stat.pam4_pmon_lock_flag;
     prbs_st_ptr->lock_loss = pam4_pmon_stat.pam4_pmon_lol_flag;
     prbs_st_ptr->err.ml_err.lsb_err = pam4_pmon_stat.pam4_oflow_even ? 0xFFFFFFFF : pam4_pmon_stat.pam4_pmon_acc_even;
     prbs_st_ptr->err.ml_err.msb_err = pam4_pmon_stat.pam4_oflow_odd ? 0xFFFFFFFF : pam4_pmon_stat.pam4_pmon_acc_odd;
         
     return RR_SUCCESS;
}

/**
* @brief     host_fec_clear_prbs_status(capi_phy_info_t* phy_info_ptr, capi_pattern_gen_mon_type_t ptype)
* @details    This API is used to clear PRBS hardware status 
*
* @param[in]  phy_info_ptr:  phy info pointer
* @param[in]  ptype:  PRBS type: CAPI_PRBS_KP4_LINE or CAPI_PRBS_KP4_CLIENT
* 
* @return     returns the performance result of the called method/function
*/          
return_result_t host_fec_clear_prbs_status(capi_phy_info_t* phy_info_ptr, capi_pattern_gen_mon_type_t ptype)
{
    capi_config_ex_info_t config_info;
    phy_info_t phy_info;
    cw_mode_parameter_t cw_mode;
    cw_port_config_t port_cfg;
#ifdef CAPI_CONFIG_CMODE_CMD_INTF
    octal_port_mode_status2_reg_t pmode_st2;
#endif
    cw_port_info_t port_info;
    cfg_egr_or_igr_t egr_or_igr = (ptype == CAPI_PRBS_KP4_HOST_MONITOR) ? EGR : IGR;

    util_memcpy((void*)&phy_info, phy_info_ptr, sizeof(phy_info_t));
    phy_info.core_ip = CORE_IP_ALL;
    phy_info.base_addr = APB2_SLV0_REGS; 
    
    if (phy_info_ptr->lane_mask == 0) {
        CAPI_LOG_ERROR("Invalid Lane Mask. %d \r\n", phy_info_ptr->lane_mask);
        return RR_ERROR_INITIALIZATION;
    }
    if ((ptype != CAPI_PRBS_KP4_MEDIA_MONITOR) && (ptype != CAPI_PRBS_KP4_HOST_MONITOR)) {
        CAPI_LOG_ERROR("Wrong input for side. %d \r\n", ptype);
        return RR_ERROR_WRONG_INPUT_VALUE;
    }

    util_memset(&config_info, 0, sizeof(capi_config_ex_info_t));
    if (ptype == CAPI_PRBS_KP4_MEDIA_MONITOR) {
        config_info.line_lane.lane_mask = phy_info_ptr->lane_mask;   /* Using line-side lane_mask for identifying PORT */
    } else if (ptype == CAPI_PRBS_KP4_HOST_MONITOR) {
        config_info.host_lane.lane_mask = phy_info_ptr->lane_mask;   /* Using host-side lane_mask for identifying PORT */
    } 

#ifdef CAPI_CONFIG_CMODE_CMD_INTF
    ERR_FUNC(capi_get_config(&phy_info, &config_info));
    ERR_HSIP(pmode_st2.words = (uint16_t)hsip_rd_reg_(&phy_info, OCTAL_PORT_MODE_STATUS_2_REG));
    port_idx = pmode_st2.fields.port_index;
#else    
    ERR_FUNC(host_diag_cw_get_chip_config(&phy_info, &config_info, &port_info));
#endif
    
    /*Build the KP4 port information*/
    host_diag_cw_kp4_gen_port_cfg(config_info.func_mode, port_info.port_idx, &port_cfg);
    host_diag_cw_kp4_gen_cw_mode_cfg(&config_info, &port_info, egr_or_igr, &port_cfg, &cw_mode);

    chal_cw_rtmr_fec_pmon_clr_all(&phy_info, egr_or_igr, &port_cfg, &cw_mode);
    return (RR_SUCCESS);
}



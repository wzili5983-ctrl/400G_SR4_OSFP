/**
 *
 * @file diag_fec_statistics.c 
 * @author  
 * @date    08/09/2018
 * @version 1.0
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
#include <math.h>
#include "chip_config_def.h"
#include "regs_common.h"
#include "string.h"
#include "chip_common_config_ind.h"
#include "chip_mode_def.h"
#include "cw_def.h"
#include "capi_def.h"
#include "capi.h"
#include "capi_test_def.h"
#include "capi_test.h"
#include "common_util.h"
#include "diag_fec_statistics_def.h"
#include "host_diag_fec_statistics.h"
#include "host_log_util.h"
#include "host_test.h"
#include "ml_cw_rtmr_modes.h"
#include "ml_cw_xbar.h"
#include "hr_time.h"

#define KP4_PRBS_ERR_ANALYZER_LINEARITY_ADJUST 1.0
#define FEC_COUNTER_LIMIT                  ((uint32_t)0xFFFFFFFF)

extern const capi_port_entry_t ports[];
/**
 * chip port mode config interface
 */
const uint32_t fec_port_config1_regs[] ={
    OCTAL_PORT_MODE_CONFIG_1_REG_0, 
    OCTAL_PORT_MODE_CONFIG_1_REG_1, 
    OCTAL_PORT_MODE_CONFIG_1_REG_2, 
    OCTAL_PORT_MODE_CONFIG_1_REG_3,
    OCTAL_PORT_MODE_CONFIG_1_REG_4, 
    OCTAL_PORT_MODE_CONFIG_1_REG_5, 
    OCTAL_PORT_MODE_CONFIG_1_REG_6, 
    OCTAL_PORT_MODE_CONFIG_1_REG_7,
};

const uint32_t fec_port_config2_regs[] ={
    OCTAL_PORT_MODE_CONFIG_2_REG_0, 
    OCTAL_PORT_MODE_CONFIG_2_REG_1, 
    OCTAL_PORT_MODE_CONFIG_2_REG_2, 
    OCTAL_PORT_MODE_CONFIG_2_REG_3,
    OCTAL_PORT_MODE_CONFIG_2_REG_4, 
    OCTAL_PORT_MODE_CONFIG_2_REG_5, 
    OCTAL_PORT_MODE_CONFIG_2_REG_6, 
    OCTAL_PORT_MODE_CONFIG_2_REG_7,
};

/** PRBS Error Analyzer Struct used to report Projection data */
typedef struct {
  /** Projected BER values (Equivalent projected post-FEC BER for t=15) */
  double proj_ber;
  /** Projected BER Invalid (0 -> valid, +1 -> BER greater than; -1 -> BER less than) */ 
  int8_t ber_proj_invalid;
  /** Number of Measured BER points available for extrapolation */
  uint8_t delta_n;
} cw_err_analyzer_report_st;

static bool host_diag_cw_rptr_fec_mon_get_sub_sampling(phy_info_t*   phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, uint8_t port_id);

static unsigned int intf_count_set_bits(uint32_t n)
{
    unsigned int count = 0;
    while (n)
    {
      n &= (n-1) ;
      count++;
    }
    return count;
}

uint8_t host_diag_cw_is_port_based_fec_mon(phy_info_t* phy_info_ptr)
{
    capi_phy_info_t phy_info;
    util_memcpy((void *)&phy_info, phy_info_ptr, sizeof(capi_phy_info_t));
    phy_info.base_addr = OCTAL_TOP_REGS;
    return (hsip_rd_reg_(&phy_info, QUAD_CORE_GPCTRL_GPCTRL_282_RDB))?1:0;
}
/**
 * @brief  host_diag_cw_rtmr_kpr4fec_dec_stat_clr_all (const phy_info_t* phy_info_ptr)
 * @detail clears all decoder error cnt to 0 in Decoder clock domain
 * @public 
 * @private 
 * 
 * @param[in] phy_info_ptr:  phy_info pointer; use KP4_KR4_FEC_DEC for base_addr
 * @return return_result_t: returns result code
 */
return_result_t host_diag_cw_rtmr_kpr4fec_dec_stat_clr_all(const phy_info_t* phy_info_ptr)
{
    hsip_wr_field_(phy_info_ptr, FEC_COUNTER_CONTROL_REGISTER, MD_DEC_CLR_ALL, 0x1);
    hsip_wr_field_(phy_info_ptr, FEC_COUNTER_CONTROL_REGISTER, MD_DEC_CLR_ALL, 0x0);
    return RR_SUCCESS;
}

/**
 * @brief  host_diag_cw_rtmr_kpr4fec_dec_stat_md_latch_all (const phy_info_t* phy_info_ptr)
 * @detail in sync, latch all decoder error cnt to MD for status reading
 * @public 
 * @private 
 * 
 * @param[in] phy_info_ptr:  phy_info pointer; use KP4_KR4_FEC_DEC for base_addr
 * @return return_result_t: returns result code
 */
return_result_t host_diag_cw_rtmr_kpr4fec_dec_stat_md_latch_all(const phy_info_t* phy_info_ptr)
{
    hsip_wr_field_(phy_info_ptr, FEC_COUNTER_CONTROL_REGISTER, MD_STAT_LATCH_ALL, 0x1);
    hsip_wr_field_(phy_info_ptr, FEC_COUNTER_CONTROL_REGISTER, MD_STAT_LATCH_ALL, 0x0);
    return RR_SUCCESS;
}

/**
 * @brief  host_diag_cw_rtmr_kpr4fec_dec_stat_md_clr_all (const phy_info_t* phy_info_ptr)
 * @detail clr all decoder error cnt in MD clock domain
 * @public 
 * @private 
 * 
 * @param[in] phy_info_ptr:  phy_info pointer; use KP4_KR4_FEC_DEC for base_addr
 * @return return_result_t: returns result code
 */
return_result_t host_diag_cw_rtmr_kpr4fec_dec_stat_md_clr_all(const phy_info_t* phy_info_ptr)
{
    hsip_wr_field_(phy_info_ptr, FEC_COUNTER_CONTROL_REGISTER, MD_STAT_CLR_ALL, 0x1);
    hsip_wr_field_(phy_info_ptr, FEC_COUNTER_CONTROL_REGISTER, MD_STAT_CLR_ALL, 0x0);
    return RR_SUCCESS;
}

/**
 * @brief  host_diag_cw_rtmr_kpr4fec_dec_stat_enable (const phy_info_t* phy_info_ptr)
 * @detail enable FEC error cnt
 * @public 
 * @private 
 * 
 * @param[in] phy_info_ptr:  phy_info pointer; use KP4_KR4_FEC_DEC for base_addr
 * @return return_result_t: returns result code
 */
return_result_t host_diag_cw_rtmr_kpr4fec_dec_stat_enable(const phy_info_t* phy_info_ptr)
{
    hsip_wr_field_(phy_info_ptr, FEC_COUNTER_CONTROL_REGISTER, MD_ERR_COUNT_SAT_EN, 0x1);
    hsip_wr_field_(phy_info_ptr, FEC_COUNTER_CONTROL_REGISTER, MD_ERR_STAT_EN, 0x1);
    return RR_SUCCESS;
}

/**
 * @brief  host_diag_cw_rtmr_kpr4fec_dec_stat_disable (const phy_info_t* phy_info_ptr)
 * @detail disable FEC error cnt
 * @public 
 * @private 
 * 
 * @param[in] bbaddr: device base address
 * @param[in] phy_info_ptr:  phy_info pointer; use KP4_KR4_FEC_DEC for base_addr
 * @return return_result_t: returns result code
 */
return_result_t host_diag_cw_rtmr_kpr4fec_dec_stat_disable(const phy_info_t* phy_info_ptr)
{
    hsip_wr_field_(phy_info_ptr, FEC_COUNTER_CONTROL_REGISTER, MD_ERR_COUNT_SAT_EN, 0x0);
    hsip_wr_field_(phy_info_ptr, FEC_COUNTER_CONTROL_REGISTER, MD_ERR_STAT_EN, 0x0);
    return RR_SUCCESS;
}

/**
 * @brief  host_diag_cw_rtmr_get_kpr4fec_dec_stat(const phy_info_t* phy_info_ptr, boolean *is_init)
 * @detail get FEC error cnt enable status
 * @public 
 * @private 
 * 
 * @param[in] bbaddr: device base address
 * @param[in] phy_info_ptr:  phy_info pointer; use KP4_KR4_FEC_DEC for base_addr
 * @return return_result_t: returns result code
 */
return_result_t host_diag_cw_rtmr_get_kpr4fec_dec_stat(const phy_info_t* phy_info_ptr, boolean *is_init)
{
    uint16_t err_cnt_stat_en, err_stat_en;
    ERR_HSIP(err_cnt_stat_en = hsip_rd_field_(phy_info_ptr, FEC_COUNTER_CONTROL_REGISTER, MD_ERR_COUNT_SAT_EN));
    ERR_HSIP(err_stat_en = hsip_rd_field_(phy_info_ptr, FEC_COUNTER_CONTROL_REGISTER, MD_ERR_STAT_EN));
    if(err_cnt_stat_en==0 || err_stat_en==0)
       *is_init = FALSE;
    else
       *is_init = TRUE;
    return RR_SUCCESS;
}

/**
 * @brief  host_diag_cw_rtmr_kpr4fec_dec_cnt_get (const phy_info_t* phy_info_ptr, kpr4fec_cnt_s *cnt)
 * @detail Gets all the BRCM FEC counter values (tot_frame_cnt, tot_frame_uncorr_cnt, tot_bits_corr_cnt[2], tot_frames_err_cnt[16])
 * @public 
 * @private 
 * 
 * @param[in] phy_info_ptr:  phy_info pointer; use KP4_KR4_FEC_DEC for base_addr
 * @param[in] cnt: reference to kpr4fec_cnt_s
 * @param[in] cw_mode_ptr : reference to cw_mode_parameter_t
 * @param[in] fec_mon_sampling_enabled : if fec monitor sampling is enabled
 * @param[in] re_read_count_ptr : second read count pointer
 * @return return_result_t: returns result code
 */
return_result_t host_diag_cw_rtmr_kpr4fec_dec_cnt_get(const phy_info_t* phy_info_ptr, kpr4fec_cnt_s* cnt, cw_mode_parameter_t *cw_mode_ptr, uint8_t fec_mon_sampling_enabled, uint16_t* re_read_count_ptr)
{
    uint8_t id;
    uint32_t reglen_id;
    phy_info_t phy_info ;
    kpr4fec_err_cnt_u fec_err_cnt_2;
    kpr4fec_err_cnt_u tot_frame_count;
    uint16_t speed_reg_host, speed_reg_line;
    fec_err_cnt_2.err_cnt = 0;
    tot_frame_count.err_cnt = 0;
    util_memcpy((void *)(&phy_info), (void *)phy_info_ptr, sizeof(phy_info_t));
    *re_read_count_ptr =0;
    do {
    util_memcpy((void *)(&phy_info), (void *)phy_info_ptr, sizeof(phy_info_t));
    /* DO not have to check 48 bits overflow since kpr4fec_tot_bits_corr_cnt will overflow in 2^48 * 1e4 / (24*60*60*56*1e9) = 580 days assuming it corrects 1e-4 channel */
    /* kpr4fec_tot_frame_cnt will overflow in 2^48 * 5440 / (24*60*60*56*1e9) = 316 days */
    ERR_HSIP(cnt->kpr4fec_tot_frame_cnt.err.err_cntr_lo = (uint16_t)hsip_rd_reg_(phy_info_ptr, MD_TOT_FRAMES_RCV_LO_REGISTER));
    ERR_HSIP(cnt->kpr4fec_tot_frame_cnt.err.err_cntr_md = (uint16_t)hsip_rd_reg_(phy_info_ptr, MD_TOT_FRAMES_RCV_MD_REGISTER));
    ERR_HSIP(cnt->kpr4fec_tot_frame_cnt.err.err_cntr_hi = (uint16_t)hsip_rd_reg_(phy_info_ptr, MD_TOT_FRAMES_RCV_HI_REGISTER));

    ERR_HSIP(cnt->kpr4fec_tot_frame_uncorr_cnt.err.err_cntr_lo = (uint16_t)hsip_rd_reg_(phy_info_ptr, MD_TOT_FRAMES_UNCORR_LO_REGISTER));
    ERR_HSIP(cnt->kpr4fec_tot_frame_uncorr_cnt.err.err_cntr_md = (uint16_t)hsip_rd_reg_(phy_info_ptr, MD_TOT_FRAMES_UNCORR_MD_REGISTER));
    ERR_HSIP(cnt->kpr4fec_tot_frame_uncorr_cnt.err.err_cntr_hi = (uint16_t)hsip_rd_reg_(phy_info_ptr, MD_TOT_FRAMES_UNCORR_HI_REGISTER));

    for (id = 0; id < KP4_FEC_TOT_BITS_CORR_NUM; id++) {
       reglen_id = (REGLEN * 3 * id);
       ERR_HSIP(cnt->kpr4fec_tot_bits_corr_cnt[id].err.err_cntr_lo = (uint16_t)hsip_rd_reg_(phy_info_ptr, (MD_TOT_BIT0S_CORR_LO_REGISTER - reglen_id)));
       ERR_HSIP(cnt->kpr4fec_tot_bits_corr_cnt[id].err.err_cntr_md = (uint16_t)hsip_rd_reg_(phy_info_ptr, (MD_TOT_BIT0S_CORR_MD_REGISTER - reglen_id)));
       ERR_HSIP(cnt->kpr4fec_tot_bits_corr_cnt[id].err.err_cntr_hi = (uint16_t)hsip_rd_reg_(phy_info_ptr, (MD_TOT_BIT0S_CORR_HI_REGISTER - reglen_id)));
    }

    /* ToDo: check func_mode is 400 G, need handle base address specially 
      for 400G, the md_tot_bit0 & md_tot_bit1 need be read twice, 1st use the passed-in base address; 2ns use teh passed-in address + LANE_OFFSET_KP4_KR4_FEC_DEC * 2 
      then retunr md_tot_bit0 & md_tot_bit1 value need add these two readout together; */
    /* 
       3. 400G Egress (i.e. host FEC)
         Dec A: BADDR_KP4_KR4_FEC_DEC_HOST
                    BADDR_KP4_KR4_FEC_DEC_HOST + LANE_OFFSET_KP4_KR4_FEC_DEC * 2 (also only for ‘md_tot_bit0/1s_corr’)
         Dec B: BADDR_KP4_KR4_FEC_DEC_HOST + LANE_OFFSET_KP4_KR4_FEC_DEC * 4
                    BADDR_KP4_KR4_FEC_DEC_HOST + LANE_OFFSET_KP4_KR4_FEC_DEC * 6 (also only for ‘md_tot_bit0/1s_corr’)
           
       6. 400G Ingress (i.e. line FEC)
         Dec A: BADDR_KP4_KR4_FEC_DEC_LINE
                    BADDR_KP4_KR4_FEC_DEC_LINE + LANE_OFFSET_KP4_KR4_FEC_DEC * 2 (also only for ‘md_tot_bit0/1s_corr’)
         Dec B: BADDR_KP4_KR4_FEC_DEC_LINE + LANE_OFFSET_KP4_KR4_FEC_DEC * 4
                    BADDR_KP4_KR4_FEC_DEC_LINE + LANE_OFFSET_KP4_KR4_FEC_DEC * 6 (also only for ‘md_tot_bit0/1s_corr’)
    */
    phy_info.base_addr = OCTAL_TOP_REGS + ml_cw_rtmr_get_base_addr(EGR, cw_mode_ptr);
    ERR_HSIP(speed_reg_host = hsip_rd_field_(&phy_info, TRAFFIC_SPEED_REG, MD_SPEED_400G));
    phy_info.base_addr = OCTAL_TOP_REGS + ml_cw_rtmr_get_base_addr(IGR, cw_mode_ptr);
    ERR_HSIP(speed_reg_line = hsip_rd_field_(&phy_info, TRAFFIC_SPEED_REG, MD_SPEED_400G));
    if(speed_reg_host || speed_reg_line) {
       for (id = 0; id < KP4_FEC_TOT_BITS_CORR_NUM; id++) {
          reglen_id = (REGLEN * 3 * id);
          ERR_HSIP(fec_err_cnt_2.err.err_cntr_lo = (uint16_t)hsip_rd_reg_(phy_info_ptr, (LANE_OFFSET_KP4_KR4_FEC_DEC*2 + MD_TOT_BIT0S_CORR_LO_REGISTER - reglen_id)));
          ERR_HSIP(fec_err_cnt_2.err.err_cntr_md = (uint16_t)hsip_rd_reg_(phy_info_ptr, (LANE_OFFSET_KP4_KR4_FEC_DEC*2 + MD_TOT_BIT0S_CORR_MD_REGISTER - reglen_id)));
          ERR_HSIP(fec_err_cnt_2.err.err_cntr_hi = (uint16_t)hsip_rd_reg_(phy_info_ptr, (LANE_OFFSET_KP4_KR4_FEC_DEC*2 + MD_TOT_BIT0S_CORR_HI_REGISTER - reglen_id)));
          cnt->kpr4fec_tot_bits_corr_cnt[id].err_cnt += fec_err_cnt_2.err_cnt;
       }
    }
    for (id = 0; id < KP4_FEC_TOT_FRAMES_ERR_NUM; id++) {
       reglen_id = (REGLEN * 3 * id);
       ERR_HSIP(cnt->kpr4fec_tot_frames_err_cnt[id].err.err_cntr_lo = (uint16_t)hsip_rd_reg_(phy_info_ptr, (MD_TOT_FRAMES_0ERR_LO_REGISTER + reglen_id)));
       ERR_HSIP(cnt->kpr4fec_tot_frames_err_cnt[id].err.err_cntr_md = (uint16_t)hsip_rd_reg_(phy_info_ptr, (MD_TOT_FRAMES_0ERR_MD_REGISTER + reglen_id)));
       ERR_HSIP(cnt->kpr4fec_tot_frames_err_cnt[id].err.err_cntr_hi = (uint16_t)hsip_rd_reg_(phy_info_ptr, (MD_TOT_FRAMES_0ERR_HI_REGISTER + reglen_id)));
    }
    ERR_HSIP(tot_frame_count.err.err_cntr_lo = (uint16_t)hsip_rd_reg_(phy_info_ptr, MD_TOT_FRAMES_RCV_LO_REGISTER));
    ERR_HSIP(tot_frame_count.err.err_cntr_md = (uint16_t)hsip_rd_reg_(phy_info_ptr, MD_TOT_FRAMES_RCV_MD_REGISTER));
    ERR_HSIP(tot_frame_count.err.err_cntr_hi = (uint16_t)hsip_rd_reg_(phy_info_ptr, MD_TOT_FRAMES_RCV_HI_REGISTER));
    if((cnt->kpr4fec_tot_frame_cnt.err.err_cntr_lo == tot_frame_count.err.err_cntr_lo) && 
       (cnt->kpr4fec_tot_frame_cnt.err.err_cntr_md == tot_frame_count.err.err_cntr_md) && 
       (cnt->kpr4fec_tot_frame_cnt.err.err_cntr_hi == tot_frame_count.err.err_cntr_hi) ){
      return RR_SUCCESS;
    }
    (*re_read_count_ptr)++;
  }while(fec_mon_sampling_enabled && (*re_read_count_ptr) <6);
  if((*re_read_count_ptr) >= 6){
    return RR_ERROR;
  }

    return RR_SUCCESS;
}


/**
* @brief     host_diag_cw_get_bbaddr_kp4deca(capi_fec_mode_t fec_mode_ptr, capi_function_mode_t func_mode_ptr, uint16_t port_idx)
* @details    This API is used to calculate the KP4 block based address
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  fec_mode_ptr:  FEC mode type
* @param[in]  func_mode_ptr: FUNCTION mode type
* @param[in]  port_idx: port index
* 
* @return    returns the performance result of the called methode/function
*/
uint32_t host_diag_cw_get_bbaddr_kp4deca(capi_fec_mode_t fec_mode_ptr, capi_function_mode_t func_mode_ptr, uint16_t port_idx, cw_mode_parameter_t* cw_mode_ptr)
{
    uint32_t bbaddr = OCTAL_TOP_REGS;
    
    
    if (fec_mode_ptr == CAPI_FEC_CLIENT) {
       bbaddr += ml_cw_rtmr_get_base_addr(EGR, cw_mode_ptr);
    } else if(fec_mode_ptr == CAPI_FEC_LINE) {
       bbaddr += ml_cw_rtmr_get_base_addr(IGR, cw_mode_ptr);
    }
    else {
       // invalid
       return bbaddr;
    }

    switch(func_mode_ptr){
        case CAPI_MODE_400G:
            if (cw_mode_ptr->line_pam_or_nrz_type == CW_PAM100 &&
                cw_mode_ptr->host_pam_or_nrz_type == CW_PAM100)
               ;  /* port_idx: 0 & 4 */
            else              
               ;  /* port_idx: 0 */
        break;
        
        case CAPI_MODE_200G:
            if (cw_mode_ptr->line_pam_or_nrz_type == CW_PAM100 &&
                cw_mode_ptr->host_pam_or_nrz_type == CW_PAM100)
               bbaddr += (port_idx % 4) * 2 * LANE_OFFSET_KP4_KR4_FEC_DEC;  /* port_idx: 0 & 2 & 4 & 6*/
            else {
               /* for 200G mixed mode (4*50G lane 4-7), lane 4-7 is swapped to lane 0-3 to use port 0 of the 1st quad */
               if (ml_cw_get_quad_swap_enabled())
                  port_idx = 0;
               else if (ml_cw_rtmr_is_A1_4p_port(cw_mode_ptr))
                   port_idx = 0;
               bbaddr += port_idx * LANE_OFFSET_KP4_KR4_FEC_DEC;  /* port_idx: 0 & 4 */
            }
        break;
        
        case CAPI_MODE_100G:
            if (cw_mode_ptr->line_pam_or_nrz_type == CW_PAM100 &&
                 cw_mode_ptr->host_pam_or_nrz_type == CW_PAM100)
               bbaddr += (port_idx % 4) * 2 * LANE_OFFSET_KP4_KR4_FEC_DEC;  /* port_idx: 0 ~ 7, */
            else if (ml_cw_rtmr_is_A1_4p_port(cw_mode_ptr)) {
                /* 100G ports mixed mode at 4p, port_index: 4,6, port_100g_en: 0,2 */
                bbaddr += (port_idx % 4) * 2 * LANE_OFFSET_KP4_KR4_FEC_DEC;
            } else
               bbaddr += port_idx * LANE_OFFSET_KP4_KR4_FEC_DEC;  /* port_idx: 0 ~ 7, */
        break;
        
        case CAPI_MODE_50G:
            /* for 50G mixed mode (1*50G lane 4-7), lane 4-7 is swapped to lane 0-3 to use port 0-3 of the 1st quad */
            if (ml_cw_get_quad_swap_enabled())
                port_idx = (port_idx % 4); 
            else if (ml_cw_rtmr_is_A1_4p_port(cw_mode_ptr)) {
                /* 50G ports mixed mode at 4p, port_index: 4,5,6,7, port_50g_en: 0,2,4,6 */
                port_idx = (port_idx % 4) * 2;
            }
            bbaddr += port_idx * LANE_OFFSET_KP4_KR4_FEC_DEC;  /* port_idx: 0 ~ 7 */
        break;
        
        case CAPI_MODE_25G:
            bbaddr += port_idx * LANE_OFFSET_KP4_KR4_FEC_DEC;  /* port_idx: 0 ~ 7 */
        break;
        
        default:
        break;
    }
    
    return bbaddr;
}
/*
* @brief     host_diag_cw_get_chip_config(capi_phy_info_t* phy_info_ptr, capi_config_ex_info_t* config_info_ptr, cw_port_info_t* port_info_ptr)
* @details    This API is used to get chip config information
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  phy_info_ptr:    pointer to phy information
* @param[out]  config_info_ptr: pointer to chip config information
* @param[out]  port_idx: port index
*
* @return    returns the performance result of the called methode/function
*/
return_result_t host_diag_cw_get_chip_config_fec_mon(capi_phy_info_t* phy_info_ptr, capi_config_ex_info_t* config_info_ptr, cw_port_info_t* port_info_ptr)
{
    capi_phy_info_t capi_phy;
    uint16_t lane_mask;
    uint8_t idx =0;
//    uint16_t pidx, pmode_idx=0;
    octal_port_mode_config1_reg_t pmode_cfg1;
//    octal_port_mode_config2_reg_t pmode_cfg2;
    capi_config_info_t config_info; 
    util_memset(&config_info, 0, sizeof(capi_config_info_t));
    CAPI_LOG_FUNC("_diag_cw_get_chip_config", phy_info_ptr);
    CAPI_LOG_INFO("line lane_mask 0x%x host lane_mask 0x%x\n",\
          config_info_ptr->line_lane.lane_mask, config_info_ptr->host_lane.lane_mask);
    util_memset((void *)&config_info, 0, sizeof(capi_config_info_t));
    util_memset((void *)&capi_phy, 0, sizeof(capi_phy_info_t));
    util_memcpy((void *)&capi_phy, phy_info_ptr, sizeof(capi_phy_info_t));
    capi_phy.phy_id = phy_info_ptr->phy_id;
    capi_phy.base_addr = OCTAL_TOP_REGS;

    /* return error if both line & host lane_mask is non_zero or zero value */
    if((config_info_ptr->host_lane.lane_mask && config_info_ptr->line_lane.lane_mask) ||
       (config_info_ptr->host_lane.lane_mask==0 && config_info_ptr->line_lane.lane_mask==0)){ 
          CAPI_LOG_ERROR("Wrong input line & host lane_mask 0x%x 0x%x \n",config_info_ptr->line_lane.lane_mask, config_info_ptr->host_lane.lane_mask);
          return RR_ERROR_WRONG_INPUT_VALUE;
    }

    if(config_info_ptr->host_lane.lane_mask){
       lane_mask = config_info_ptr->host_lane.lane_mask;
       config_info.host_lane.lane_mask = lane_mask;      
    } else if(config_info_ptr->line_lane.lane_mask) {
       lane_mask = config_info_ptr->line_lane.lane_mask;
       config_info.line_lane.lane_mask =  lane_mask;
    }
    config_info.fec_monitor_port = 1;
    if(capi_get_config(&capi_phy, &config_info) == RR_SUCCESS){
        config_info_ptr->bh_br = config_info.bh_br;
        config_info_ptr->egr_fec_term = config_info.fec_term;
        config_info_ptr->func_mode = config_info.func_mode;
        config_info_ptr->host_fec_type = config_info.host_fec_type;
        config_info_ptr->host_lane.lane_mask =config_info.host_lane.lane_mask;
        config_info_ptr->host_lane.modulation =config_info.host_lane.modulation;
        config_info_ptr->igr_fec_term = config_info.fec_term;
        config_info_ptr->line_fec_type = config_info.line_fec_type;
        config_info_ptr->line_lane.lane_mask =config_info.line_lane.lane_mask;
        config_info_ptr->line_lane.modulation =config_info.line_lane.modulation;
        config_info_ptr->lw_br = config_info.lw_br;
        config_info_ptr->mux_type = config_info.mux_type;
        config_info_ptr->pwd_status = config_info.pwd_status;
        config_info_ptr->ref_clk = config_info.ref_clk;
        if(intf_count_set_bits(config_info.line_lane.lane_mask) > intf_count_set_bits(config_info.host_lane.lane_mask)){
          for(idx=0;idx < CAPI_MAX_PORTS;idx++){
              if(config_info.line_lane.lane_mask & (1<<idx)){
                break;
              }
            }
            port_info_ptr->port_idx = idx;
        }else {
            for(idx=0;idx < CAPI_MAX_PORTS;idx++){
              if(config_info.host_lane.lane_mask & (1<<idx)){
                break;
              }
            }
            port_info_ptr->port_idx = idx;
        }
        ERR_HSIP(pmode_cfg1.words = (uint16_t)hsip_rd_reg_(&capi_phy, fec_port_config1_regs[port_info_ptr->port_idx]));
        config_info_ptr->mixed_mode = pmode_cfg1.fields.mixed_mode;
    }
    
    CAPI_LOG_CONFIG_EX_INFO(config_info_ptr);
    return RR_SUCCESS;
}

/*
* @brief     host_diag_cw_get_chip_config(capi_phy_info_t* phy_info_ptr, capi_config_ex_info_t* config_info_ptr, cw_port_info_t* port_info_ptr)
* @details    This API is used to get chip config information
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  phy_info_ptr:    pointer to phy information
* @param[out]  config_info_ptr: pointer to chip config information
* @param[out]  port_idx: port index
*
* @return    returns the performance result of the called methode/function
*/
return_result_t host_diag_cw_get_chip_config(capi_phy_info_t* phy_info_ptr, capi_config_ex_info_t* config_info_ptr, cw_port_info_t* port_info_ptr)
{
    capi_phy_info_t capi_phy;
    uint16_t lane_mask;
    cfg_host_or_line_t host_or_line;
    uint16_t pidx, pmode_idx=0;
    octal_port_mode_config1_reg_t pmode_cfg1;
    octal_port_mode_config2_reg_t pmode_cfg2;
#if (CAPI_LOG_LEVEL != 0)
    uint16_t regs[8]={0};
#endif
    uint16_t reg_val;

    CAPI_LOG_FUNC("_diag_cw_get_chip_config", phy_info_ptr);
    CAPI_LOG_INFO("line lane_mask 0x%x host lane_mask 0x%x\n",\
          config_info_ptr->line_lane.lane_mask, config_info_ptr->host_lane.lane_mask);
    
    util_memset((void *)&capi_phy, 0, sizeof(capi_phy_info_t));
    util_memcpy((void *)&capi_phy, phy_info_ptr, sizeof(capi_phy_info_t));
    capi_phy.phy_id = phy_info_ptr->phy_id;
    capi_phy.base_addr = OCTAL_TOP_REGS;

    /* return error if both line & host lane_mask is non_zero or zero value */
    if((config_info_ptr->host_lane.lane_mask && config_info_ptr->line_lane.lane_mask) ||
       (config_info_ptr->host_lane.lane_mask==0 && config_info_ptr->line_lane.lane_mask==0)){ 
          CAPI_LOG_ERROR("Wrong input line & host lane_mask 0x%x 0x%x \n",config_info_ptr->line_lane.lane_mask, config_info_ptr->host_lane.lane_mask);
          return RR_ERROR_WRONG_INPUT_VALUE;
    }

    if(config_info_ptr->host_lane.lane_mask){
       lane_mask = config_info_ptr->host_lane.lane_mask;
       host_or_line = HOST;
    } else if(config_info_ptr->line_lane.lane_mask) {
       lane_mask = config_info_ptr->line_lane.lane_mask;
       host_or_line = LINE;
    }

    /*Need fine the port idx*/
    for(pidx=0; pidx<MAX_PORT; pidx++){
       ERR_HSIP(pmode_cfg1.words = (uint16_t)hsip_rd_reg_(&capi_phy, fec_port_config1_regs[pidx]));
       /*add sanity checkup*/    
       if(pmode_cfg1.fields.port_vld==0 || pmode_cfg1.fields.port_mode_enum > CHIP_MODE_MAX_MODE - 1)
          continue;
#if (CAPI_LOG_LEVEL != 0)
       regs[pidx] = pmode_cfg1.words;
#endif
       if((host_or_line==LINE && (host_test_get_port_lane_mask((uint8_t)pmode_cfg1.fields.port_mode_enum, 1, pidx)&lane_mask)) ||
          (host_or_line==HOST && (host_test_get_port_lane_mask((uint8_t)pmode_cfg1.fields.port_mode_enum, 0, pidx)&lane_mask))){
              port_info_ptr->port_idx = (uint8_t)pmode_cfg1.fields.port_idx;
          break;             
       }
    }
    if(pidx==MAX_PORT) {
       
       CAPI_LOG_ERROR("Couldn't find the matched port\n");
       util_memset((void *)config_info_ptr, 0, sizeof(capi_config_ex_info_t));
       CAPI_LOG_INFO("WRONG -- pmode_cfg1 0x%x 0x%x - %x - %x - %x\r\n", capi_phy.base_addr, regs[0], regs[1], regs[2], regs[3]);
       return RR_ERROR;
    }
    ERR_HSIP(pmode_cfg2.words = (uint16_t)hsip_rd_reg_(&capi_phy, fec_port_config2_regs[port_info_ptr->port_idx]));
    
    for(pidx = 0; pidx < CHIP_MODE_MAX_MODE - 1; pidx++){
      if(pmode_cfg1.fields.port_mode_enum == ports[pidx].port_type){
            pmode_idx = pidx;
          break;
         }
    }
    if(pidx == CHIP_MODE_MAX_MODE - 1) {

       CAPI_LOG_ERROR("Couldn't find the matched port mode\n");
       CAPI_LOG_INFO("WRONG -- pmode_cfg1 0x%x 0x%x - %x - %x - %x\r\n", capi_phy.base_addr, regs[0], regs[1], regs[2], regs[3]);
       util_memset((void *)config_info_ptr, 0, sizeof(capi_config_ex_info_t));
       return RR_ERROR;
    }
    port_info_ptr->port_type = ports[pidx].port_type;
    /*build parameters of config structure*/ 

    ERR_HSIP(config_info_ptr->ref_clk = (capi_ref_clk_frq_mode_t)hsip_rd_field_(&capi_phy, CHIP_TOP_CHIP_REFCLK_CONFIG_REG, REF_CLK_FREQ_SELECT));
    config_info_ptr->func_mode = (capi_function_mode_t)ports[pmode_idx].func_mode;
    config_info_ptr->egr_fec_term = (capi_lane_fec_term_type_t)pmode_cfg2.fields.egr_fec_term;
    config_info_ptr->igr_fec_term = (capi_lane_fec_term_type_t)pmode_cfg2.fields.igr_fec_term;
    
    config_info_ptr->lw_br = (capi_lw_baud_rate_t)ports[pmode_idx].lw_cfg.br;
    config_info_ptr->bh_br = (capi_bh_baud_rate_t)ports[pmode_idx].bh_cfg.br;

    config_info_ptr->line_fec_type = (capi_line_fec_type_t)pmode_cfg2.fields.line_fec_type;
    config_info_ptr->host_fec_type = (capi_host_fec_type_t)pmode_cfg2.fields.host_fec_type;
    ERR_HSIP(reg_val = (uint16_t)hsip_rd_reg_(&capi_phy, OCW_CHIP_POWER_DOWN_STATUS_REG));
    config_info_ptr->pwd_status = (((reg_val >>(port_info_ptr->port_idx<<1))&0x3)==0x3)?CAPI_PORT_POWER_DOWN_STATUS_POWER_DOWN:CAPI_PORT_POWER_DOWN_STATUS_POWER_UP;

    config_info_ptr->line_lane.lane_mask = host_test_get_port_lane_mask((uint8_t)pmode_cfg1.fields.port_mode_enum, 1, port_info_ptr->port_idx);
    config_info_ptr->host_lane.lane_mask = host_test_get_port_lane_mask((uint8_t)pmode_cfg1.fields.port_mode_enum, 0, port_info_ptr->port_idx);
    config_info_ptr->line_lane.modulation = (capi_modulation_t)ports[pmode_idx].lw_cfg.mod;
    config_info_ptr->host_lane.modulation = (capi_modulation_t)ports[pmode_idx].bh_cfg.mod;

    config_info_ptr->mixed_mode = pmode_cfg1.fields.mixed_mode;

    CAPI_LOG_CONFIG_EX_INFO(config_info_ptr);

    return RR_SUCCESS;
}


/**
* @brief    host_diag_cw_kp4_gen_port_cfg(capi_function_mode_t func_mode, uint16_t port_index, cw_port_config_t *port_cfg_ptr)
* @details    This funciton is used to decode CW port information
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  func_mode:
* @param[in]  port_index: 
* @param[in]  port_cfg_ptr: 
* 
* @return    returns the performance result of the called methode/function
*/
void host_diag_cw_kp4_gen_port_cfg(capi_function_mode_t func_mode, uint16_t port_index, cw_port_config_t *port_cfg_ptr)
{

    util_memset((void *)port_cfg_ptr, 0, sizeof(cw_port_config_t));
    port_cfg_ptr->port_id = (uint8_t)port_index;
    if(func_mode == CAPI_MODE_25G)
       port_cfg_ptr->port_25g_en[port_index] = PORT_ON;
    else if(func_mode == CAPI_MODE_50G){
       /* for 50G mixed mode (1*50G lane 4-7), lane 4-7 is swapped to lane 0-3 to use port 0-3 of the 1st quad */
       if (ml_cw_get_quad_swap_enabled())
           port_index = (port_index % 4); 
       port_cfg_ptr->port_50g_en[port_index] = PORT_ON;
    }else if(func_mode == CAPI_MODE_100G)
       port_cfg_ptr->port_100g_en[port_index/2] = PORT_ON;
    else if(func_mode == CAPI_MODE_200G) {
       /* for 200G mixed mode (4*50G lane 4-7), lane 4-7 is swapped to lane 0-3 to use port 0 of the 1st quad */
       if (ml_cw_get_quad_swap_enabled())
          port_index = 0;
       port_cfg_ptr->port_200g_en[port_index/4] = PORT_ON;
    } else if(func_mode == CAPI_MODE_400G)
       port_cfg_ptr->port_400g_en = PORT_ON;
}


/**
* @brief     host_diag_cw_kp4_gen_cw_mode_cfg(capi_config_ex_info_t* cfg_info, cw_port_info_t *port_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t *port_cfg_ptr,cw_mode_parameter_t *cw_mode_ptr)
* @details    This function is used to decode CW mode configuration
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  func_mode:
* @param[in]  port_index: 
* @param[in]  egr_or_igr: 
* @param[in]  port_cfg_ptr: 
* 
* @return    returns the performance result of the called methode/function
*/
void host_diag_cw_kp4_gen_cw_mode_cfg(capi_config_ex_info_t* cfg_info, cw_port_info_t *port_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t *port_cfg_ptr, cw_mode_parameter_t *cw_mode_ptr)
{
    cw_port_entry_ex_t cw_port_entry = {0};
    cw_port_entry_ex_t *port_entry_ptr = &cw_port_entry;
    
    port_entry_ptr->lw_cfg.lane_mask = cfg_info->line_lane.lane_mask;
    port_entry_ptr->bh_cfg.lane_mask = cfg_info->host_lane.lane_mask;
    port_entry_ptr->lw_cfg.mod = cfg_info->line_lane.modulation;
    port_entry_ptr->bh_cfg.mod = cfg_info->host_lane.modulation;        
    port_entry_ptr->lw_cfg.br = cfg_info->lw_br;
    port_entry_ptr->bh_cfg.br = cfg_info->bh_br;
    port_entry_ptr->func_mode = (cw_function_mode_t)cfg_info->func_mode;
    port_entry_ptr->host_fec = (cw_host_fec_type_t)cfg_info->host_fec_type;
    port_entry_ptr->line_fec = (cw_line_fec_type_t)cfg_info->line_fec_type;
    port_entry_ptr->egr_fec_term = (cw_port_fec_term_type_t)cfg_info->egr_fec_term;
    port_entry_ptr->igr_fec_term = (cw_port_fec_term_type_t)cfg_info->igr_fec_term;
    port_entry_ptr->mixed_mode = (chip_mixed_mode_t)cfg_info->mixed_mode;

    ml_cw_rtmr_gen_cw_mode_param(port_entry_ptr, port_info_ptr->port_idx, egr_or_igr, port_cfg_ptr, cw_mode_ptr);    
}


void _get_cfg_list_array_idx(cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, diag_port_cfg_list_t *cfg_list_ptr)
{
    
    uint8_t idx;
    switch (cur_mode_parameter_ptr->speed) {
       case (SPEED_100G):
          /*port 0: Lane 0,1 ; port 2:Lane 2,3 ; port 2: Lane 4,5 ; port 3:Lane 6,7 ; */
          for(idx=0; idx<4; idx++){
             if(cur_port_config_ptr->port_100g_en[idx]==0)
                continue;
             cfg_list_ptr->val_idx = idx<<1;
          }
          cfg_list_ptr->gap_num = 2;
          break;
       
       case (SPEED_200G):
          /*port 0: Lane 0,1,2,3 ; port 4:Lane 4,5,6,7 */
          for(idx=0; idx<2; idx++){
             if(cur_port_config_ptr->port_200g_en[idx]==0)
                continue;
             cfg_list_ptr->val_idx = idx<<2;
          }
          cfg_list_ptr->gap_num = 4;
          break;
       case (SPEED_400G):
          /*port 0: Lane 0,1,2,3,4,5,6,7 */
          cfg_list_ptr->val_idx = 0;     
          cfg_list_ptr->gap_num = 8;
          break;
       case (SPEED_50G):
          /*port x: Lane x */
          for(idx=0; idx<8; idx++){
             if(cur_port_config_ptr->port_50g_en[idx]==0)
                continue;
             cfg_list_ptr->val_idx = idx;
          }
          cfg_list_ptr->gap_num = 1;
          break;
       case (SPEED_25G):
          /*port x: Lane x */
          for(idx=0; idx<8; idx++){
             if(cur_port_config_ptr->port_25g_en[idx]==0)
                continue;
             cfg_list_ptr->val_idx = idx;
          }
          cfg_list_ptr->gap_num = 1;
          break;
    }
}

/* Checked RETIMER_IGR or RETIMER_EGR */
uint16_t host_diag_cw_rtmr_fec_sync_status(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr)
{
    /* read fec alignment status. Live status,not sticky*/
    uint16_t reg_val = 0;
    int __hsip_err = 0;
    uint16_t maskval_list[8] = {0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80};
    diag_port_cfg_list_t cfg_list;
    _get_cfg_list_array_idx(cur_port_config_ptr, cur_mode_parameter_ptr, &cfg_list);

    reg_val = (uint16_t)hsip_rd_reg_(phy_info_ptr, AM_LANE_ALIGNMENT_STATUS_REG);
    reg_val &= maskval_list[cfg_list.val_idx];
    (void) __hsip_err;
    return reg_val;
}

static uint16_t host_cw_rtmr_rcv_pfifo_get_lane_mask(cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t wmask;

    wmask = (egr_or_igr == EGR) ? cur_mode_parameter_ptr->host_llane_mask : cur_mode_parameter_ptr->line_llane_mask;

    // Special case handling for 50G (2x25G NRZ to 1x50G PAM mode) rgbox/fgbox mapping, PFIFO selection: NRZ side uses lane mask, PAM side uses FEC slice
    // Chip modes: CHIP_MODE_2X25G_KR4NRZ_TO_1X51G_KR4PAM, CHIP_MODE_2x25G_NRZ_TO_1X53G_KP4PAM, CHIP_MODE_2X26G_KP4NRZ_TO_1X53G_KP4PAM, CHIP_MODE_2X25G_KR4NRZ_TO_1X51G_KR4PAM
    if (cur_mode_parameter_ptr->speed == SPEED_50G) {
        if (egr_or_igr == IGR && cur_mode_parameter_ptr->rpt_type == GEARBOX) {
            // line side rcv pfifo: 0x1, 0x4, 0x10, 0x40
            wmask = cur_mode_parameter_ptr->fec_slice;
        }
        if (egr_or_igr == EGR && cur_mode_parameter_ptr->rpt_type == R_GEARBOX) {
            // client side rcv pfifo: 0x1, 0x4, 0x10, 0x40
            wmask = cur_mode_parameter_ptr->fec_slice;
        }
    }

    // Special case handling for 100G (4x25G NRZ to 2x50G PAM mode) rgbox/fgbox mapping, PFIFO selection: NRZ side uses lane mask, PAM side uses FEC slice
    // chip modes: CHIP_MODE_4x25G_NRZ_TO_2X53G_KP4PAM, CHIP_MODE_4X26G_KP4NRZ_TO_2X53G_KP4PAM
    if (cur_mode_parameter_ptr->speed == SPEED_100G) {
       if (egr_or_igr == IGR && cur_mode_parameter_ptr->rpt_type == GEARBOX && cur_mode_parameter_ptr->line_pam_or_nrz_type == CW_PAM) {
           // line side rcv pfifo: 0x3, 0x30
           wmask = cur_mode_parameter_ptr->fec_slice;
       }
       if (egr_or_igr == EGR && cur_mode_parameter_ptr->rpt_type == R_GEARBOX && cur_mode_parameter_ptr->host_pam_or_nrz_type == CW_PAM) {
           // client side rcv pfifo: 0x3, 0x30
           wmask = cur_mode_parameter_ptr->fec_slice;
       }
    }
       
	return wmask;
}

static uint16_t host_cw_rtmr_tmt_pfifo_get_lane_mask(cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t wmask;

    wmask = (egr_or_igr == IGR) ? cur_mode_parameter_ptr->host_llane_mask : cur_mode_parameter_ptr->line_llane_mask;

    // Special case handling for 50G (2x25G NRZ to 1x50G PAM mode) rgbox/fgbox mapping, PFIFO selection: NRZ side uses lane mask, PAM side uses FEC slice
    // Chip modes: CHIP_MODE_2X25G_KR4NRZ_TO_1X51G_KR4PAM, CHIP_MODE_2x25G_NRZ_TO_1X53G_KP4PAM, CHIP_MODE_2X26G_KP4NRZ_TO_1X53G_KP4PAM, CHIP_MODE_2X25G_KR4NRZ_TO_1X51G_KR4PAM
    if (cur_mode_parameter_ptr->speed == SPEED_50G) {
        if (egr_or_igr == EGR && cur_mode_parameter_ptr->rpt_type == GEARBOX) {
            // line side tx pfifo: 0x1, 0x4, 0x10, 0x40
            wmask = cur_mode_parameter_ptr->fec_slice;
        }
        if (egr_or_igr == IGR && cur_mode_parameter_ptr->rpt_type == R_GEARBOX) {
            // client side tx pfifo: 0x1, 0x4, 0x10, 0x40
            wmask = cur_mode_parameter_ptr->fec_slice;
        }
    }

    // Special case handling for 100G (4x25G NRZ to 2x50G PAM mode) rgbox/fgbox mapping, PFIFO selection: NRZ side uses lane mask, PAM side uses FEC slice
    // chip modes: CHIP_MODE_4x25G_NRZ_TO_2X53G_KP4PAM, CHIP_MODE_4X26G_KP4NRZ_TO_2X53G_KP4PAM
    if (cur_mode_parameter_ptr->speed == SPEED_100G) {
       if (egr_or_igr == EGR && cur_mode_parameter_ptr->rpt_type == GEARBOX && cur_mode_parameter_ptr->line_pam_or_nrz_type == CW_PAM) {
           // line side tx pfifo: 0x3, 0x30
           wmask = cur_mode_parameter_ptr->fec_slice;
       }
       if (egr_or_igr == IGR && cur_mode_parameter_ptr->rpt_type == R_GEARBOX && cur_mode_parameter_ptr->host_pam_or_nrz_type == CW_PAM) {
           // client side tx pfifo: 0x3, 0x30
           wmask = cur_mode_parameter_ptr->fec_slice;
       }
    }

	return wmask;
}

/* Checked RETIMER_IGR or RETIMER_EGR */
return_result_t host_diag_cw_rtmr_rcv_pfifo_clsn_get_fault_status(phy_info_t* phy_info_ptr, uint16_t* pfifo_clsn_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t reg_val = 0x0, wmask;

    wmask = host_cw_rtmr_rcv_pfifo_get_lane_mask(cur_mode_parameter_ptr, egr_or_igr);
    if (wmask) {
       ERR_HSIP(reg_val = hsip_rd_field_(phy_info_ptr,  FIFO_COLLISION_STICKY_STATUS_REG, FIFO_COLLISION_FAULT_STICKY));
       (*pfifo_clsn_ptr) = (reg_val & wmask) & 0xFF;
    } else {
       (*pfifo_clsn_ptr) = 0;
    }

    return RR_SUCCESS;
}


/* Checked RETIMER_IGR or RETIMER_EGR */
return_result_t host_diag_cw_rtmr_rcv_pfifo_clsn_clr_fault_status(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t wmask;

    wmask = host_cw_rtmr_rcv_pfifo_get_lane_mask(cur_mode_parameter_ptr, egr_or_igr);
    if (wmask) 
       hsip_wr_reg_(phy_info_ptr,  FIFO_COLLISION_STICKY_STATUS_REG, wmask<<8);  
      /* write-1 to clear FIFO_COLLISION_FAULT_STICKY 0x0000ff00 */

    return RR_SUCCESS;
}



/* Checked RETIMER_IGR or RETIMER_EGR */
uint16_t host_diag_cw_rtmr_fec_am_los_lock_get_status(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr)
{
    uint16_t reg_val = 0;
    int __hsip_err = 0;
    ERR_HSIP(reg_val = (uint16_t)hsip_rd_reg_(phy_info_ptr,AM_LOSS_LOCK_LANE_STICKY_STATUS_REG ));
    switch (cur_mode_parameter_ptr->speed) {
        case (SPEED_400G):
           reg_val &= 0xFFFF;  
        break;
        case (SPEED_200G):
           if (cur_port_config_ptr->port_200g_en[0] == PORT_ON ) {
             reg_val &= 0x00FF;
           } else if (cur_port_config_ptr->port_200g_en[1] == PORT_ON) {
             reg_val &= 0xFF00;
           } else {
          reg_val = 0x00;
           }
        break;
        case (SPEED_100G):
          if (cur_port_config_ptr->port_100g_en[0] == PORT_ON) {
             reg_val &= 0x000F;
           } else if(cur_port_config_ptr->port_100g_en[1] == PORT_ON) {
             reg_val &= 0x00F0;
           } else if(cur_port_config_ptr->port_100g_en[2] == PORT_ON) {
             reg_val &= 0x0F00;
           } else if(cur_port_config_ptr->port_100g_en[3] == PORT_ON) {
             reg_val &= 0xF000;
           } else {
          reg_val = 0x0;
           }
        break;
        case (SPEED_50G):
          if (cur_port_config_ptr->port_50g_en[0] == PORT_ON) {
             reg_val &= 0x0003;
           } else if(cur_port_config_ptr->port_50g_en[1] == PORT_ON) {
             reg_val &= 0x000C;
           } else if(cur_port_config_ptr->port_50g_en[2] == PORT_ON) {
             reg_val &= 0x0030;
           } else if(cur_port_config_ptr->port_50g_en[3] == PORT_ON) {
             reg_val &= 0x00C0;
           } else if(cur_port_config_ptr->port_50g_en[4] == PORT_ON) {
             reg_val &= 0x0300;
           } else if(cur_port_config_ptr->port_50g_en[5] == PORT_ON) {
             reg_val &= 0x0C00;
           } else if(cur_port_config_ptr->port_50g_en[6] == PORT_ON) {
             reg_val &= 0x3000;
           } else if(cur_port_config_ptr->port_50g_en[7] == PORT_ON) {
             reg_val &= 0xC000;
           } else {
             reg_val = 0x00;
           }
        break;
        case (SPEED_25G):
          if (cur_port_config_ptr->port_25g_en[0] == PORT_ON) {
             reg_val &= 0x01;
           } else if(cur_port_config_ptr->port_25g_en[1] == PORT_ON) {
             reg_val &= 0x04;
           } else if(cur_port_config_ptr->port_25g_en[2] == PORT_ON) {
             reg_val &= 0x10;
           } else if(cur_port_config_ptr->port_25g_en[3] == PORT_ON) {
             reg_val &= 0x40;
           } else if(cur_port_config_ptr->port_25g_en[4] == PORT_ON) {
             reg_val &= 0x100;
           } else if(cur_port_config_ptr->port_25g_en[5] == PORT_ON) {
             reg_val &= 0x400;
           } else if(cur_port_config_ptr->port_25g_en[6] == PORT_ON) {
             reg_val &= 0x1000;
           } else if(cur_port_config_ptr->port_25g_en[7] == PORT_ON) {
             reg_val &= 0x4000;
           } else {
             reg_val = 0x00;
           }
        break;
        default:
        break;
    }    
    (void) __hsip_err;
    return reg_val;
}


/* Checked RETIMER_IGR or RETIMER_EGR */
return_result_t host_diag_cw_rtmr_fec_am_los_lock_clr_status(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr)
{
    int __hsip_err = 0;
    uint16_t reg_val = 0;

    switch (cur_mode_parameter_ptr->speed) {
        case (SPEED_400G):
           reg_val = 0xFFFF;  
        break;
        case (SPEED_200G):
           if (cur_port_config_ptr->port_200g_en[0] == PORT_ON ) {
             reg_val = 0x00FF;
           } else if (cur_port_config_ptr->port_200g_en[1] == PORT_ON) {
             reg_val = 0xFF00;
           } else {
          reg_val = 0x00;
           }
        break;
        case (SPEED_100G):
          if (cur_port_config_ptr->port_100g_en[0] == PORT_ON) {
             reg_val = 0x000F;
           } else if(cur_port_config_ptr->port_100g_en[1] == PORT_ON) {
             reg_val = 0x00F0;
           } else if(cur_port_config_ptr->port_100g_en[2] == PORT_ON) {
             reg_val = 0x0F00;
           } else if(cur_port_config_ptr->port_100g_en[3] == PORT_ON) {
             reg_val = 0xF000;
           } else {
          reg_val = 0x0;
           }
        break;
        case (SPEED_50G):
          if (cur_port_config_ptr->port_50g_en[0] == PORT_ON) {
             reg_val = 0x0003;
           } else if(cur_port_config_ptr->port_50g_en[1] == PORT_ON) {
             reg_val = 0x000C;
           } else if(cur_port_config_ptr->port_50g_en[2] == PORT_ON) {
             reg_val = 0x0030;
           } else if(cur_port_config_ptr->port_50g_en[3] == PORT_ON) {
             reg_val = 0x00C0;
           } else if(cur_port_config_ptr->port_50g_en[4] == PORT_ON) {
             reg_val = 0x0300;
           } else if(cur_port_config_ptr->port_50g_en[5] == PORT_ON) {
             reg_val = 0x0C00;
           } else if(cur_port_config_ptr->port_50g_en[6] == PORT_ON) {
             reg_val = 0x3000;
           } else if(cur_port_config_ptr->port_50g_en[7] == PORT_ON) {
             reg_val = 0xC000;
           } else {
             reg_val = 0x00;
           }
       break;
        case (SPEED_25G):
          if (cur_port_config_ptr->port_25g_en[0] == PORT_ON) {
             reg_val = 0x01;
           } else if(cur_port_config_ptr->port_25g_en[1] == PORT_ON) {
             reg_val = 0x04;
           } else if(cur_port_config_ptr->port_25g_en[2] == PORT_ON) {
             reg_val = 0x10;
           } else if(cur_port_config_ptr->port_25g_en[3] == PORT_ON) {
             reg_val = 0x40;
           } else if(cur_port_config_ptr->port_25g_en[4] == PORT_ON) {
             reg_val = 0x100;
           } else if(cur_port_config_ptr->port_25g_en[5] == PORT_ON) {
             reg_val = 0x400;
           } else if(cur_port_config_ptr->port_25g_en[6] == PORT_ON) {
             reg_val = 0x1000;
           } else if(cur_port_config_ptr->port_25g_en[7] == PORT_ON) {
             reg_val = 0x4000;
           } else {
             reg_val = 0x00;
           }
        break;
        default:
        break;
    }    
    hsip_wr_reg_(phy_info_ptr,AM_LOSS_LOCK_LANE_STICKY_STATUS_REG, reg_val );
    (void) __hsip_err;
    return RR_SUCCESS;
}

/* Checked RETIMER_IGR or RETIMER_EGR */
return_result_t host_diag_cw_rtmr_xdec_gbox_collision_get_fault_status(phy_info_t* phy_info_ptr, uint16_t* xdec_gbox_clsn_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr)
{
    uint16_t reg_val = 0;
    uint16_t wmask = 0;

    switch (cur_mode_parameter_ptr->speed) {
       case (SPEED_400G):
          if (cur_port_config_ptr->port_400g_en) {
             wmask = 0x55;
          } 
          break;
       case (SPEED_200G):
          if (cur_port_config_ptr->port_200g_en[0]) {
             wmask = 0x5;
          } else if (cur_port_config_ptr->port_200g_en[1]) {
             wmask = 0x50;
          }
          break;
       case (SPEED_100G):
          if (cur_port_config_ptr->port_100g_en[0]) {
             wmask = 0x1;
          } else if (cur_port_config_ptr->port_100g_en[1]) {
             wmask = 0x4;
          } else if (cur_port_config_ptr->port_100g_en[2]) {
             wmask = 0x10;
          } else if (cur_port_config_ptr->port_100g_en[3]) {
             wmask = 0x40;
          }
          break;
       default:
          wmask = cur_mode_parameter_ptr->fec_slice;
          break;
    }

    if (wmask) {
       ERR_HSIP(reg_val = hsip_rd_field_(phy_info_ptr, XDEC_GBOX_160_257_IRQ_STICKY_REG, XDECODER_GBOX_160_257_COLLISIION_FAULT_STICKY));
       (*xdec_gbox_clsn_ptr) = reg_val & wmask;
    } else {
       (*xdec_gbox_clsn_ptr) = 0;
    }

    return RR_SUCCESS;
}

/* Checked RETIMER_IGR or RETIMER_EGR */
return_result_t host_diag_cw_rtmr_xdec_gbox_collision_clr_fault_status(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr)
{
    uint16_t wmask = 0;

    switch (cur_mode_parameter_ptr->speed) {
       case (SPEED_400G):
          if (cur_port_config_ptr->port_400g_en) {
             wmask = 0x55;
          } 
          break;
       case (SPEED_200G):
          if (cur_port_config_ptr->port_200g_en[0]) {
             wmask = 0x5;
          } else if (cur_port_config_ptr->port_200g_en[1]) {
             wmask = 0x50;
          }
          break;
       case (SPEED_100G):
          if (cur_port_config_ptr->port_100g_en[0]) {
             wmask = 0x1;
          } else if (cur_port_config_ptr->port_100g_en[1]) {
             wmask = 0x4;
          } else if (cur_port_config_ptr->port_100g_en[2]) {
             wmask = 0x10;
          } else if (cur_port_config_ptr->port_100g_en[3]) {
             wmask = 0x40;
          }
          break;
       default:
          wmask = cur_mode_parameter_ptr->fec_slice;
          break;
    }

    if (wmask) 
       hsip_wr_reg_(phy_info_ptr, XDEC_GBOX_160_257_IRQ_STICKY_REG, wmask<<8); 
       /* write-1 to clear XDECODER_GBOX_160_257_COLLISIION_FAULT_STICKY */

    return RR_SUCCESS;
}

/* Checked RETIMER_IGR or RETIMER_EGR */
return_result_t host_diag_cw_rtmr_xenc_gbox_collision_get_fault_status(phy_info_t* phy_info_ptr, uint16_t* xenc_gbox_clsn_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr)
{
    uint16_t reg_val = 0;
    uint16_t wmask = 0, wmask1 = 0;

    switch (cur_mode_parameter_ptr->speed) {
       case (SPEED_400G):
          if (cur_port_config_ptr->port_400g_en) {
             wmask  = 0x5;
             wmask1 = 0x5;
          } 
          break;
       case (SPEED_200G):
          if (cur_port_config_ptr->port_200g_en[0]) {
             wmask  = 0x5;
          } else if (cur_port_config_ptr->port_200g_en[1]) {
             wmask1 = 0x5;
          }
          break;
       case (SPEED_100G):
          if (cur_port_config_ptr->port_100g_en[0]) {
             wmask  = 0x1;
          } else if (cur_port_config_ptr->port_100g_en[1]) {
             wmask  = 0x4;
          } else if (cur_port_config_ptr->port_100g_en[2]) {
             wmask1 = 0x1;
          } else if (cur_port_config_ptr->port_100g_en[3]) {
             wmask1 = 0x4;
          }
          break;
       default:
          wmask  = (cur_mode_parameter_ptr->fec_slice & 0xF);
          wmask1 = (cur_mode_parameter_ptr->fec_slice & 0xF0)>>4;
          break;
    }

    if (wmask | wmask1) {
       ERR_HSIP(reg_val = hsip_rd_field_(phy_info_ptr, XENC_GBOX_257_160_IRQ_STICKY_REG, XENCODER_GBOX_257_160_COLLISIION_FAULT_STICKY));
       (*xenc_gbox_clsn_ptr) = reg_val & wmask & 0xF;

       ERR_HSIP(reg_val = hsip_rd_field_(phy_info_ptr, XENC_GBOX_257_160_IRQ_STICKY_REG, XENCODER_GBOX_257_160_COLLISIION_FAULT_STICKY_EXTRA));
       (*xenc_gbox_clsn_ptr) |= (reg_val & wmask1 & 0xF) << 4;
    } else {
       (*xenc_gbox_clsn_ptr) = 0;
    }

    return RR_SUCCESS;
}

/* Checked RETIMER_IGR or RETIMER_EGR */
return_result_t host_diag_cw_rtmr_xenc_gbox_collision_clr_fault_status(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr)
{
    uint16_t wmask = 0, wmask1 = 0;

    switch (cur_mode_parameter_ptr->speed) {
       case (SPEED_400G):
          if (cur_port_config_ptr->port_400g_en) {
             wmask  = 0x5;
             wmask1 = 0x5;
          } 
          break;
       case (SPEED_200G):
          if (cur_port_config_ptr->port_200g_en[0]) {
             wmask  = 0x5;
          } else if (cur_port_config_ptr->port_200g_en[1]) {
             wmask1 = 0x5;
          }
          break;
       case (SPEED_100G):
          if (cur_port_config_ptr->port_100g_en[0]) {
             wmask  = 0x1;
          } else if (cur_port_config_ptr->port_100g_en[1]) {
             wmask  = 0x4;
          } else if (cur_port_config_ptr->port_100g_en[2]) {
             wmask1 = 0x1;
          } else if (cur_port_config_ptr->port_100g_en[3]) {
             wmask1 = 0x4;
          }
          break;
       default:
          wmask  = (cur_mode_parameter_ptr->fec_slice & 0xF);
          wmask1 = (cur_mode_parameter_ptr->fec_slice & 0xF0)>>4;
          break;
    }

    if (wmask) 
       hsip_wr_reg_(phy_info_ptr, XENC_GBOX_257_160_IRQ_STICKY_REG, wmask<<4); 
       /*  write-1 to clear XENCODER_GBOX_257_160_COLLISIION_FAULT_STICKY 0x000000f0 */

    if (wmask1) 
       hsip_wr_reg_(phy_info_ptr, XENC_GBOX_257_160_IRQ_STICKY_REG, wmask1<<12);  
       /* write-1 to clear XENCODER_GBOX_257_160_COLLISIION_FAULT_STICKY_EXTRA 0x0000f000*/

    return RR_SUCCESS;
}

/* Checked RETIMER_IGR or RETIMER_EGR */
return_result_t host_diag_cw_rtmr_tmt_pfifo_clsn_get_fault_status(phy_info_t* phy_info_ptr, uint16_t* pfifo_clsn_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t reg_val = 0x0, wmask;

    wmask = host_cw_rtmr_tmt_pfifo_get_lane_mask(cur_mode_parameter_ptr, egr_or_igr);
    if (wmask) {
       ERR_HSIP(reg_val = hsip_rd_field_(phy_info_ptr,  TMT_FIFO_COLLISION_STICKY_STATUS_REG, FIFO_COLLISION_FAULT_STICKY));
       (*pfifo_clsn_ptr) = (reg_val & wmask) & 0xFF;
    } else {
       (*pfifo_clsn_ptr) = 0;
    }

    return RR_SUCCESS;
}

/* Checked RETIMER_IGR or RETIMER_EGR */
return_result_t host_diag_cw_rtmr_tmt_pfifo_clsn_clr_fault_status(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t wmask;

    wmask = host_cw_rtmr_tmt_pfifo_get_lane_mask(cur_mode_parameter_ptr, egr_or_igr);
    if (wmask) 
       hsip_wr_reg_(phy_info_ptr,  TMT_FIFO_COLLISION_STICKY_STATUS_REG, wmask<<8);  
       /* write-1 to clear FIFO_COLLISION_FAULT_STICKY 0x0000ff00*/

    return RR_SUCCESS;
}

uint16_t host_diag_cw_rtmr_fec_los_sync_fault_get_status(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr)
{
    uint16_t reg_val = 0;
    int __hsip_err = 0;
    uint16_t maskval_list[8] = {0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80};
    diag_port_cfg_list_t cfg_list;
    _get_cfg_list_array_idx(cur_port_config_ptr, cur_mode_parameter_ptr, &cfg_list);
    
    ERR_HSIP(reg_val = (uint16_t)hsip_rd_field_(phy_info_ptr, AM_LANE_ALIGNMENT_UNLOCK_IRQ_STICKY_REG, ALIGN_FAIL_FAULT_STICKY ));
    reg_val &= maskval_list[cfg_list.val_idx];
    (void) __hsip_err;
    return reg_val;
}


return_result_t host_diag_cw_rtmr_fec_los_sync_fault_clr_status(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr)
{
    uint16_t reg_val = 0;
    int __hsip_err = 0;
    uint16_t maskval_list[8] = {0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80};
    diag_port_cfg_list_t cfg_list;
    _get_cfg_list_array_idx(cur_port_config_ptr, cur_mode_parameter_ptr, &cfg_list);
    
    reg_val =  maskval_list[cfg_list.val_idx];
    hsip_wr_reg_(phy_info_ptr, AM_LANE_ALIGNMENT_UNLOCK_IRQ_STICKY_REG, reg_val<<8);
    /* write-1 to clear ALIGN_FAIL_FAULT_STICKY 0x0000ff00*/
    (void) __hsip_err;
    return RR_SUCCESS;
}


static return_result_t host_diag_clear_fec_counters(capi_phy_info_t* phy_info_ptr, capi_fec_mode_t fec_mode, uint8_t port_id)
{
    capi_phy_info_t capi_phy;
    uint16_t port_mask, fec_cnt_clear, loops;
    cfg_egr_or_igr_t egr_or_igr=(fec_mode==CAPI_FEC_LINE)?IGR:EGR;

    util_memcpy((void *)&capi_phy, phy_info_ptr, sizeof(capi_phy_info_t));

    port_mask = (egr_or_igr == EGR) ? (1<<port_id) : (1<<(port_id+8));
    capi_phy.base_addr = OCTAL_TOP_REGS;
    fec_cnt_clear = (uint16_t)hsip_rd_reg_(&capi_phy, OCW_FEC_COUNTER_CLEAR_REG);
    fec_cnt_clear |= port_mask;
    hsip_wr_reg_(&capi_phy, OCW_FEC_COUNTER_CLEAR_REG, fec_cnt_clear);
    for (loops = 0; loops < 2000; loops++) {
        delay_ms(1);
        fec_cnt_clear = (uint16_t)hsip_rd_reg_(&capi_phy, OCW_FEC_COUNTER_CLEAR_REG);
        if ((fec_cnt_clear & port_mask) == 0)   // FW should clear OCW_FEC_COUNTER_CLEAR_REG bit
            return RR_SUCCESS;
    }
    CAPI_LOG_ERROR("Failed to clear FEC counters, port_mask %x \n", port_mask);        
    return RR_ERROR_FIRMWARE_NOT_READY;
}


static return_result_t host_diag_clear_am_lol_sticky_status(capi_phy_info_t* phy_info_ptr, capi_fec_mode_t fec_mode, uint8_t port_id)
{
    capi_phy_info_t capi_phy;
    uint16_t port_mask, fec_lol_clear, loops;
    cfg_egr_or_igr_t egr_or_igr=(fec_mode==CAPI_FEC_LINE)?IGR:EGR;
    uint8_t fec_mon_sampling_enabled;

    util_memcpy((void *)&capi_phy, phy_info_ptr, sizeof(capi_phy_info_t));
    port_mask = (egr_or_igr == EGR) ? (1<<port_id) : (1<<(port_id+8));
    capi_phy.base_addr = OCTAL_TOP_REGS;

    fec_mon_sampling_enabled = host_diag_cw_rptr_fec_mon_get_sub_sampling(phy_info_ptr, egr_or_igr, port_id);
    if (!fec_mon_sampling_enabled) {
        uint16_t fec_lol_value = (uint16_t)hsip_rd_reg_(&capi_phy, OCW_FEC_AM_LOL_STICKY_STATUS);
        if (fec_lol_value & port_mask) {
            fec_lol_value &= (~port_mask);
            hsip_wr_reg_(&capi_phy, OCW_FEC_AM_LOL_STICKY_STATUS, fec_lol_value);
        }
        return RR_SUCCESS;
    }

    fec_lol_clear = (uint16_t)hsip_rd_reg_(&capi_phy, OCW_FEC_AM_LOL_STICKY_STATUS_CLEAR);
    fec_lol_clear |= port_mask;
    hsip_wr_reg_(&capi_phy, OCW_FEC_AM_LOL_STICKY_STATUS_CLEAR, fec_lol_clear);
    for (loops = 0; loops < 2000; loops++) {
        delay_ms(1);
        fec_lol_clear = (uint16_t)hsip_rd_reg_(&capi_phy, OCW_FEC_AM_LOL_STICKY_STATUS_CLEAR);
        if ((fec_lol_clear & port_mask) == 0)   // FW should clear OCW_FEC_AM_LOL_STICKY_STATUS_CLEAR bit and OCW_FEC_AM_LOL_STICKY_STATUS bit
            return RR_SUCCESS;
    }
    CAPI_LOG_ERROR("Failed to clear AM LOL status, port_mask %x \n", port_mask);        
    return RR_ERROR_FIRMWARE_NOT_READY;
}


/**
 * @brief      _diag_cw_clear_fec_rtmr_info(capi_phy_info_t* phy_info_ptr, cw_mode_parameter_t *cw_mode_ptr, cw_port_config_t *port_cfg_ptr, capi_fec_mode_t fec_mode)
 * @details    This function is used to clear FEC sticky status
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: phy information structure pointer
 * @param[in]  cw_mode_ptr: CW mode config structure pointer
 * @param[in]  port_cfg_ptr: CW port config structure pointer
 * @param[in]  fec_mode: FEC mode type
 * 
 * @return  returns the performance result of the called methode/function
 */
return_result_t _diag_cw_clear_fec_rtmr_info(capi_phy_info_t* phy_info_ptr, cw_mode_parameter_t *cw_mode_ptr, cw_port_config_t *port_cfg_ptr, capi_fec_mode_t fec_mode)
{
    capi_phy_info_t capi_phy;
    cfg_egr_or_igr_t egr_or_igr=(fec_mode==CAPI_FEC_LINE)?IGR:EGR;

    util_memcpy((void *)(&capi_phy), phy_info_ptr, sizeof(capi_phy_info_t));
    //capi_phy.base_addr = OCTAL_TOP_REGS + ((fec_mode==CAPI_FEC_LINE)?RETIMER_IGR:RETIMER_EGR);
    capi_phy.base_addr = OCTAL_TOP_REGS + ml_cw_rtmr_get_base_addr(egr_or_igr, cw_mode_ptr);

    host_diag_cw_rtmr_rcv_pfifo_clsn_clr_fault_status(&capi_phy, cw_mode_ptr, port_cfg_ptr, egr_or_igr);
    host_diag_cw_rtmr_fec_am_los_lock_clr_status(&capi_phy, cw_mode_ptr, port_cfg_ptr);
    host_diag_cw_rtmr_xdec_gbox_collision_clr_fault_status(&capi_phy, cw_mode_ptr, port_cfg_ptr);
    host_diag_cw_rtmr_xenc_gbox_collision_clr_fault_status(&capi_phy, cw_mode_ptr, port_cfg_ptr);
    host_diag_cw_rtmr_tmt_pfifo_clsn_clr_fault_status(&capi_phy, cw_mode_ptr, port_cfg_ptr, egr_or_igr);

    host_diag_cw_rtmr_fec_los_sync_fault_clr_status(&capi_phy, cw_mode_ptr, port_cfg_ptr); 
     
    return host_diag_clear_am_lol_sticky_status(phy_info_ptr, fec_mode, port_cfg_ptr->port_id);
}

/**
 * @brief     _diag_cw_get_fec_rtmr_info(capi_phy_info_t* phy_info_ptr, cw_mode_parameter_t *cw_mode_ptr, cw_port_config_t *port_cfg_ptr,
                                  capi_fec_mode_t fec_mode, capi_fec_rx_status_t* fec_st_ptr)
 * @details    This function is used to retrieve FEC status pointer
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: phy information structure pointer
 * @param[in]  cw_mode_ptr: CW mode config structure pointer
 * @param[in]  port_cfg_ptr: CW port config structure pointer
 * @param[in]  fec_mode: FEC mode type
 * @param[out]  fec_st_ptr: FEC status pointer
 * 
 * @return  returns the performance result of the called methode/function
 */
return_result_t _diag_cw_get_fec_rtmr_info(capi_phy_info_t* phy_info_ptr, cw_mode_parameter_t *cw_mode_ptr, cw_port_config_t *port_cfg_ptr,
                                  capi_fec_mode_t fec_mode, uint16_t fec_term, uint8_t fec_mon_sampling_enabled, capi_fec_rx_status_t* fec_st_ptr)
{
    boolean link_stat = TRUE;
    uint16_t fec_align_unlock = 0;
    capi_phy_info_t capi_phy;
    cfg_egr_or_igr_t egr_or_igr=(fec_mode==CAPI_FEC_LINE)?IGR:EGR;
    uint16_t fec_lock_value, fec_lol_value, port_mask;

    util_memcpy((void *)(&capi_phy), phy_info_ptr, sizeof(capi_phy_info_t));
    capi_phy.base_addr = OCTAL_TOP_REGS + ml_cw_rtmr_get_base_addr(egr_or_igr, cw_mode_ptr);    //((fec_mode==CAPI_FEC_LINE)?RETIMER_IGR:RETIMER_EGR);

    if ((enum cw_port_fec_term_type_e)fec_term != CHIP_PORT_FEC_TERM_BYPASS && host_diag_cw_is_port_based_fec_mon(&capi_phy) == 0) {
        host_diag_cw_rtmr_rcv_pfifo_clsn_get_fault_status(&capi_phy, &fec_st_ptr->igbox_clsn_sticky, cw_mode_ptr, port_cfg_ptr, egr_or_igr);
        host_diag_cw_rtmr_xdec_gbox_collision_get_fault_status(&capi_phy, &fec_st_ptr->xdec_gbox_clsn_sticky, cw_mode_ptr, port_cfg_ptr);
        host_diag_cw_rtmr_xenc_gbox_collision_get_fault_status(&capi_phy, &fec_st_ptr->xenc_gbox_clsn_sticky, cw_mode_ptr, port_cfg_ptr);
        host_diag_cw_rtmr_tmt_pfifo_clsn_get_fault_status(&capi_phy, &fec_st_ptr->ogbox_clsn_sticky, cw_mode_ptr, port_cfg_ptr, egr_or_igr);
    }

    fec_st_ptr->am_lolock_sticky = host_diag_cw_rtmr_fec_am_los_lock_get_status(&capi_phy, cw_mode_ptr, port_cfg_ptr);

    if (fec_mon_sampling_enabled) {
        // get AM lock and LOL status reported by FW
        capi_phy.base_addr = OCTAL_TOP_REGS;
        port_mask = (egr_or_igr == EGR) ? (1<<port_cfg_ptr->port_id) : (1<<(port_cfg_ptr->port_id+8));
        fec_lock_value = (uint16_t)hsip_rd_reg_(&capi_phy, OCW_FEC_AM_LOCK_STATUS);
        fec_lol_value = (uint16_t)hsip_rd_reg_(&capi_phy, OCW_FEC_AM_LOL_STICKY_STATUS);
        fec_st_ptr->fec_algn_stat = (fec_lock_value & port_mask) ? CAPI_LINK_STATUS_UP : CAPI_LINK_STATUS_DOWN;
        fec_st_ptr->fec_algn_lol_sticky = (fec_lol_value & port_mask) ? TRUE : FALSE;
        fec_st_ptr->am_lolock_sticky = 0;
    } else {
        link_stat = (host_diag_cw_rtmr_fec_sync_status(&capi_phy, cw_mode_ptr, port_cfg_ptr))?TRUE:FALSE;
        fec_st_ptr->fec_algn_stat = (!link_stat) ? CAPI_LINK_STATUS_DOWN : CAPI_LINK_STATUS_UP;
        fec_align_unlock = host_diag_cw_rtmr_fec_los_sync_fault_get_status(&capi_phy, cw_mode_ptr, port_cfg_ptr);
        fec_st_ptr->fec_algn_lol_sticky = fec_align_unlock ? TRUE : FALSE;
    }

    return _diag_cw_clear_fec_rtmr_info(phy_info_ptr, cw_mode_ptr, port_cfg_ptr, fec_mode);    
}



return_result_t host_diag_set_rptr_fec_monitor(capi_phy_info_t* phy_info_ptr, capi_rptr_fec_st_t* rptr_fec_st_ptr)
{
    capi_command_info_t command_info;
    capi_phy_info_t capi_phy;

    util_memcpy((void *)&capi_phy, phy_info_ptr, sizeof(capi_phy_info_t));
    if(rptr_fec_st_ptr->fec_mode==CAPI_FEC_CLIENT)
        capi_phy.lane_mask = phy_info_ptr->lane_mask << 16;

    capi_phy.core_ip = CORE_IP_CW;
    command_info.command_id = COMMAND_ID_SET_CW_RPTR_FEC_MON_CONFIG;
    command_info.type.rptr_fec_st = *rptr_fec_st_ptr;

    return(capi_command_request(&capi_phy, &command_info, sizeof(capi_rptr_fec_st_t), SET_CONFIG));
}


static void host_diag_cw_rptr_fec_mon_set_sub_sampling(phy_info_t*   phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, uint8_t port_id, bool sub_sampling_enabled)
{
    uint16_t fec_mon_sampling_reg_val, port_mask;
    uint16_t port_based_fec_mon = host_diag_cw_is_port_based_fec_mon(phy_info_ptr);
    
    phy_info_ptr->base_addr = OCTAL_TOP_REGS;
    fec_mon_sampling_reg_val = (uint16_t)hsip_rd_reg_(phy_info_ptr, ((port_based_fec_mon)?OCW_FEC_MON_PORT_SUB_SAMPLING_CFG:OCW_FEC_MON_SUB_SAMPLING_CFG));
    port_mask = (egr_or_igr == EGR) ? (1<<port_id) : (1<<(port_id+8));
    if (sub_sampling_enabled)
        fec_mon_sampling_reg_val |= port_mask;
    else
        fec_mon_sampling_reg_val &= (~port_mask);
    hsip_wr_reg_(phy_info_ptr, ((port_based_fec_mon)?OCW_FEC_MON_PORT_SUB_SAMPLING_CFG:OCW_FEC_MON_SUB_SAMPLING_CFG), fec_mon_sampling_reg_val);
}

static bool host_diag_cw_rptr_fec_mon_get_sub_sampling(phy_info_t*   phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, uint8_t port_id)
{
    uint16_t fec_mon_sampling_reg_val, port_mask;
    uint16_t port_based_fec_mon = host_diag_cw_is_port_based_fec_mon(phy_info_ptr);
    
    phy_info_ptr->base_addr = OCTAL_TOP_REGS;
    fec_mon_sampling_reg_val = (uint16_t)hsip_rd_reg_(phy_info_ptr, ((port_based_fec_mon)?OCW_FEC_MON_PORT_SUB_SAMPLING_CFG:OCW_FEC_MON_SUB_SAMPLING_CFG));
    port_mask = (egr_or_igr == EGR) ? (1<<port_id) : (1<<(port_id+8));
    return (fec_mon_sampling_reg_val & port_mask) ? TRUE : FALSE;
}

static return_result_t host_diag_cw_rptr_fec_stats_mon_config_handler(phy_info_t*   phy_info_ptr, capi_fec_dump_status_t* capi_fec_dump_status_ptr, uint8_t port_id, enabled_t enable)
{
    capi_rptr_fec_st_t rptr_fec_st = {0};
    phy_info_t   phy_info = *phy_info_ptr;
    capi_prbs_info_t capi_prbs = {0};
    cfg_egr_or_igr_t egr_or_igr;
    bool sub_sampling_enabled;

    // check if FEC PMON is enabled, skip FEC monitor enable/disable if FEC PMON is on, otherwise it will disrupt FEC PMON
    capi_prbs.ptype = (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_CLIENT) ? CAPI_PRBS_KP4_HOST_MONITOR : CAPI_PRBS_KP4_MEDIA_MONITOR;
    ERR_FUNC(capi_get_prbs_info(&phy_info, &capi_prbs));

    if (capi_prbs.gen_switch)
        return RR_SUCCESS;

    rptr_fec_st.fec_mode = capi_fec_dump_status_ptr->fec_mode;
    rptr_fec_st.is_enable = enable;
    rptr_fec_st.sampling_rate = capi_fec_dump_status_ptr->sampling_rate;
    /* default value 0 is 100% sampling for backward compatible */
    if (rptr_fec_st.sampling_rate == 0 || rptr_fec_st.sampling_rate > 100)
        rptr_fec_st.sampling_rate = 100;

    // set FEC mon sub-sampling register, 1 - sub-sampling enabled, 0 - disabled, 100% FEC mon rate
    egr_or_igr = (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_CLIENT)? EGR : IGR;
    sub_sampling_enabled = (rptr_fec_st.sampling_rate < 100 && enable) ? TRUE : FALSE;
    host_diag_cw_rptr_fec_mon_set_sub_sampling(&phy_info, egr_or_igr, port_id, sub_sampling_enabled);
    if(host_diag_cw_is_port_based_fec_mon(&phy_info)){
        rptr_fec_st.fec_monitor_port = 1;
    }
        
     return host_diag_set_rptr_fec_monitor(phy_info_ptr, &rptr_fec_st);
}

static return_result_t host_diag_cw_clear_fec_stats(
    phy_info_t*              phy_info_ptr,
    capi_fec_dump_status_t*  capi_fec_dump_status_ptr,
    cw_mode_parameter_t*     cur_mode_parameter_ptr,
    uint8_t                  port_id)
{
    capi_phy_info_t capi_phy = *phy_info_ptr;
    capi_phy_info_t capi_phy_fec_stats = *phy_info_ptr;
    capi_gpr_lane_status_t capi_gpr_lane_status = {0};
    uint8_t idx, clr_port_max = 0;
    uint8_t fec_mon_sampling_enabled = 0;
    capi_function_mode_t func_mode = capi_fec_dump_status_ptr->func_mode;
    capi_fec_mode_t      fec_mode  = capi_fec_dump_status_ptr->fec_mode;
    cfg_egr_or_igr_t     egr_or_igr = (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_CLIENT) ? EGR : IGR;
    return_result_t return_result;

    capi_phy.base_addr = OCTAL_TOP_REGS;
    
    /* check if repeater mode FEC monitor sub-sampling (FEC monitor sample rate < 100%) is enabled */
    fec_mon_sampling_enabled = host_diag_cw_rptr_fec_mon_get_sub_sampling(&capi_phy, egr_or_igr, port_id);
    
    if (fec_mon_sampling_enabled) {
        // retrieve CDR lock status
        capi_phy.core_ip = (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_CLIENT) ? CORE_IP_HOST_DSP : CORE_IP_MEDIA_DSP;
        capi_phy.lane_mask = (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_CLIENT) ?
                              cur_mode_parameter_ptr->host_plane_mask :
                              cur_mode_parameter_ptr->line_plane_mask;
        capi_gpr_lane_status.status_type = GPR_LANE_CDR_LOCK_STATUS;
        capi_get_status(&capi_phy, &capi_gpr_lane_status);

        //if (capi_gpr_lane_status.param.cdr.lock_status == capi_phy.lane_mask) {
            // if CDR locks, clear fec counters by FW if fec mon sampling is enabled so it can clear it when clock is on
            // else: do not clear since FEC sampling FSM cannot clear FEC counters with FEC decoder clock off.
            return_result = host_diag_clear_fec_counters(phy_info_ptr, fec_mode, port_id);
            if (return_result != RR_SUCCESS)
                return return_result;
        //}
    } else {
        capi_phy_fec_stats.base_addr = host_diag_cw_get_bbaddr_kp4deca(fec_mode, func_mode, port_id, cur_mode_parameter_ptr);
        if(func_mode == CAPI_MODE_50G || func_mode == CAPI_MODE_25G)
          clr_port_max = 1;
        else if(func_mode == CAPI_MODE_100G)
          clr_port_max = 2;
        else if(func_mode == CAPI_MODE_200G)
          clr_port_max = 4;
        else if(func_mode == CAPI_MODE_400G)
          clr_port_max = 8;

        for(idx=0; idx<clr_port_max; idx++){
            host_diag_cw_rtmr_kpr4fec_dec_stat_clr_all(&capi_phy_fec_stats);
            host_diag_cw_rtmr_kpr4fec_dec_stat_md_clr_all(&capi_phy_fec_stats);
            capi_phy_fec_stats.base_addr += LANE_OFFSET_KP4_KR4_FEC_DEC;
        }
    }
    return RR_SUCCESS;
}

return_result_t host_diag_cw_fec_ports_auto_detect_status (capi_phy_info_t* phy_info_ptr, capi_fec_auto_ports_detect_state_t* capi_fec_auto_ports_detect_status_ptr)
{
    return_result_t ret_result = RR_SUCCESS;
    capi_phy_info_t capi_phy1;
    cw_fec_mon_discover_reg_t discover_reg;

    util_memcpy((void *)&capi_phy1, phy_info_ptr, sizeof(capi_phy_info_t));

    capi_phy1.base_addr = OCTAL_TOP_REGS;

    discover_reg.words = (uint16_t)hsip_rd_reg_(&capi_phy1, QUAD_CORE_CONFIG_CFGVAL_61_RDB);
    if(discover_reg.fields.state == 0){
      capi_fec_auto_ports_detect_status_ptr->auto_detect_status = CAPI_AUTO_DETECT_STATUS_INVALID;
    }else if(discover_reg.fields.state == 1){
      uint8_t port_idx;
      capi_phy_info_t capi_phy;
      capi_config_info_t capi_cfg;
      uint8_t next_valid_port = 0;
       uint16_t valid_lane_mask =0;
       capi_fec_auto_ports_detect_status_ptr->undiscovered_lanemask=0x0000000F;

      util_memcpy((void *)&capi_phy, (void*)phy_info_ptr, sizeof(capi_phy_info_t));
      capi_phy.core_ip = CORE_IP_ALL;

      capi_fec_auto_ports_detect_status_ptr->fec_lock_value = (uint16_t)hsip_rd_reg_(&capi_phy1, OCW_FEC_AM_LOCK_STATUS);
      valid_lane_mask = (uint16_t)hsip_rd_reg_(&capi_phy1, QUAD_CORE_GPCTRL_GPCTRL_273_RDB);
      valid_lane_mask |= (uint16_t)hsip_rd_reg_(&capi_phy1, QUAD_CORE_GPCTRL_GPCTRL_274_RDB);
      valid_lane_mask |= (uint16_t)hsip_rd_reg_(&capi_phy1, QUAD_CORE_GPCTRL_GPCTRL_275_RDB);
      capi_fec_auto_ports_detect_status_ptr->undiscovered_lanemask &= ((uint32_t)(~valid_lane_mask));
      for(port_idx=0; port_idx<MAX_PORT; port_idx++)
      {
          util_memset((void *)&capi_cfg, 0, sizeof(capi_config_info_t));
          util_memset((void *)&capi_fec_auto_ports_detect_status_ptr->capi_config_info[port_idx], 0, sizeof(capi_config_info_t));
          capi_cfg.fec_monitor_port =1;
          capi_cfg.line_lane.lane_mask = 1 << port_idx;
          if(valid_lane_mask& capi_cfg.line_lane.lane_mask){
              if (capi_get_config(&capi_phy, &capi_cfg)==RR_SUCCESS) {
                  if (capi_cfg.func_mode){
                      if(next_valid_port == port_idx) {
                          next_valid_port = port_idx + intf_count_set_bits(capi_cfg.line_lane.lane_mask);
                          capi_fec_auto_ports_detect_status_ptr->valid_ports = (capi_fec_auto_ports_detect_status_ptr->valid_ports|(1<<port_idx));
                      }
                      capi_fec_auto_ports_detect_status_ptr->capi_config_info[port_idx] = capi_cfg;
                  } else {
                      next_valid_port++;
                  }
              }
          }else {
                      next_valid_port++;
          }
      }
      capi_fec_auto_ports_detect_status_ptr->auto_detect_status = CAPI_AUTO_DETECT_STATUS_INPROGRESS;

    }else if(discover_reg.fields.state == 2){  
      uint8_t port_idx;
      capi_phy_info_t capi_phy;
      capi_config_info_t capi_cfg;
      uint8_t next_valid_port = 0;
      uint16_t valid_lane_mask =0;
      capi_fec_auto_ports_detect_status_ptr->undiscovered_lanemask=0x0000000F;

      util_memcpy((void *)&capi_phy, (void*)phy_info_ptr, sizeof(capi_phy_info_t));
      capi_phy.core_ip = CORE_IP_ALL;
      valid_lane_mask = (uint16_t)hsip_rd_reg_(&capi_phy1, QUAD_CORE_GPCTRL_GPCTRL_273_RDB);
      valid_lane_mask |= (uint16_t)hsip_rd_reg_(&capi_phy1, QUAD_CORE_GPCTRL_GPCTRL_274_RDB);
      valid_lane_mask |= (uint16_t)hsip_rd_reg_(&capi_phy1, QUAD_CORE_GPCTRL_GPCTRL_275_RDB);
      capi_fec_auto_ports_detect_status_ptr->undiscovered_lanemask &= ((uint32_t)(~valid_lane_mask));
      capi_fec_auto_ports_detect_status_ptr->fec_lock_value = (uint16_t)hsip_rd_reg_(&capi_phy1, OCW_FEC_AM_LOCK_STATUS);
      for(port_idx=0; port_idx<MAX_PORT; port_idx++)
      {
          util_memset((void *)&capi_cfg, 0, sizeof(capi_config_info_t));
          util_memset((void *)&capi_fec_auto_ports_detect_status_ptr->capi_config_info[port_idx], 0, sizeof(capi_config_info_t));
          capi_cfg.fec_monitor_port =1;
          capi_cfg.line_lane.lane_mask = 1 << port_idx;

          if (capi_get_config(&capi_phy, &capi_cfg)==RR_SUCCESS) {
              if (capi_cfg.func_mode){
                  if(next_valid_port == port_idx) {
                      next_valid_port = port_idx + intf_count_set_bits(capi_cfg.line_lane.lane_mask);
                      capi_fec_auto_ports_detect_status_ptr->valid_ports = (capi_fec_auto_ports_detect_status_ptr->valid_ports|(1<<port_idx));
                  }
                  capi_fec_auto_ports_detect_status_ptr->capi_config_info[port_idx] = capi_cfg;
              } else {
                  next_valid_port++;

              }
          }
      }
      capi_fec_auto_ports_detect_status_ptr->auto_detect_status = CAPI_AUTO_DETECT_STATUS_DONE;
    }
  return ret_result;
}
/**
 * @brief     host_diag_cw_fec_stat_init(capi_phy_info_t* phy_info_ptr, capi_fec_dump_status_t* capi_fec_dump_status_ptr,  capi_function_mode_t func_mode);
 * @details    This function is used to initialize the FEC statistics monitor
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: phy information structure pointer
 * @param[in]  capi_fec_dump_status_ptr: FEC status dump structure pointer
 * @param[in]  func_mode: FUNCTION mode type
 * 
 * @return      returns the performance result of the called methode/function
 */
return_result_t host_diag_cw_fec_stat_init (capi_phy_info_t* phy_info_ptr, capi_fec_dump_status_t* capi_fec_dump_status_ptr, capi_function_mode_t func_mode)
{
    boolean is_init;
    capi_phy_info_t capi_phy;
    capi_phy_info_t capi_phy2, capi_phy3;
    cw_port_info_t port_info;
    capi_config_ex_info_t config_info; 
    cw_mode_parameter_t mode_parameter;
    cw_port_config_t port_cfg;
    uint8_t fec_mon_sampling_enabled;
    cfg_egr_or_igr_t egr_or_igr = (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_CLIENT) ? EGR : IGR;
    uint16_t fec_term;

    if(capi_fec_dump_status_ptr->fec_mode != CAPI_FEC_CLIENT && capi_fec_dump_status_ptr->fec_mode != CAPI_FEC_LINE){
       return RR_ERROR_WRONG_INPUT_VALUE;
    }
    util_memcpy((void *)&capi_phy, phy_info_ptr, sizeof(capi_phy_info_t));
    capi_phy.base_addr = OCTAL_TOP_REGS;
    util_memcpy((void *)&capi_phy2, &capi_phy, sizeof(capi_phy_info_t));
    util_memcpy((void *)&capi_phy3, &capi_phy, sizeof(capi_phy_info_t));
    util_memset(&config_info, 0, sizeof(capi_config_ex_info_t));

    if (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_LINE) {
       config_info.line_lane.lane_mask = phy_info_ptr->lane_mask;   /* Using line-side lane_mask for identifying PORT */
    } else if (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_CLIENT) {
       config_info.host_lane.lane_mask = phy_info_ptr->lane_mask;   /* Using host-side lane_mask for identifying PORT */
    }
    /*retrieve the chip config mode*/
    if(host_diag_cw_is_port_based_fec_mon(&capi_phy)){
        ERR_FUNC(host_diag_cw_get_chip_config_fec_mon(&capi_phy, &config_info, &port_info));
    }else {
    ERR_FUNC(host_diag_cw_get_chip_config(&capi_phy, &config_info, &port_info));
    }
    
    /*Build the KP4 port information*/
    host_diag_cw_kp4_gen_port_cfg((capi_function_mode_t)config_info.func_mode, port_info.port_idx, &port_cfg);
    host_diag_cw_kp4_gen_cw_mode_cfg(&config_info, &port_info, egr_or_igr, &port_cfg, &mode_parameter);

    fec_term = (egr_or_igr == EGR) ? config_info.egr_fec_term : config_info.igr_fec_term; 
    if((enum cw_port_fec_term_type_e)fec_term==CHIP_PORT_FEC_TERM_BYPASS || host_diag_cw_is_port_based_fec_mon(&capi_phy))
        ERR_FUNC(host_diag_cw_rptr_fec_stats_mon_config_handler(phy_info_ptr, capi_fec_dump_status_ptr, port_info.port_idx, 1));

    capi_phy.base_addr = host_diag_cw_get_bbaddr_kp4deca(capi_fec_dump_status_ptr->fec_mode, capi_fec_dump_status_ptr->func_mode, port_info.port_idx, &mode_parameter);

    host_diag_cw_rtmr_get_kpr4fec_dec_stat(&capi_phy, &is_init);
    fec_mon_sampling_enabled = host_diag_cw_rptr_fec_mon_get_sub_sampling(&capi_phy3, egr_or_igr, port_info.port_idx);
    if (is_init == FALSE) {
       /* clear fec stats */
       host_diag_cw_clear_fec_stats(&capi_phy, capi_fec_dump_status_ptr, &mode_parameter, port_info.port_idx);
       
       if(!fec_mon_sampling_enabled){
       /*enable fec stats */
       host_diag_cw_rtmr_kpr4fec_dec_stat_enable(&capi_phy);
    }
    }
    return _diag_cw_clear_fec_rtmr_info(&capi_phy2,  &mode_parameter, &port_cfg, capi_fec_dump_status_ptr->fec_mode);
}

/**
 * @brief     host_diag_cw_fec_stat_deinit(capi_phy_info_t* phy_info_ptr, capi_fec_dump_status_t* capi_fec_dump_status_ptr,  capi_function_mode_t func_mode);
 * @details    This function is used to release the FEC statistics monitor
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: phy information structure pointer
 * @param[in]  capi_fec_dump_status_ptr: FEC status dump structure pointer
 * @param[in]  func_mode: FUNCTION mode type
 * 
 * @return      returns the performance result of the called methode/function
 */
return_result_t host_diag_cw_fec_stat_deinit(capi_phy_info_t* phy_info_ptr, capi_fec_dump_status_t* capi_fec_dump_status_ptr, capi_function_mode_t func_mode)
{
    capi_phy_info_t capi_phy;
    cw_mode_parameter_t mode_parameter;
    cw_port_info_t port_info;
    capi_config_ex_info_t config_info; 
    cw_port_config_t port_cfg;
    cfg_egr_or_igr_t egr_or_igr = (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_CLIENT) ? EGR : IGR;
    uint16_t fec_term;

    if(capi_fec_dump_status_ptr->fec_mode != CAPI_FEC_CLIENT && capi_fec_dump_status_ptr->fec_mode != CAPI_FEC_LINE){
       return RR_ERROR_WRONG_INPUT_VALUE;
    }
    /*parameter initialization*/
    util_memcpy((void *)&capi_phy, phy_info_ptr, sizeof(capi_phy_info_t));
    capi_phy.base_addr = OCTAL_TOP_REGS;
    util_memset(&config_info, 0, sizeof(capi_config_ex_info_t));
    
    if (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_LINE) {
       config_info.line_lane.lane_mask = phy_info_ptr->lane_mask;   /* Using line-side lane_mask for identifying PORT */
    } else if (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_CLIENT) {
       config_info.host_lane.lane_mask = phy_info_ptr->lane_mask;   /* Using host-side lane_mask for identifying PORT */
    }
    /*retrieve the chip config mode*/
    if(host_diag_cw_is_port_based_fec_mon(&capi_phy)){
        ERR_FUNC(host_diag_cw_get_chip_config_fec_mon(&capi_phy, &config_info, &port_info));
    }else {
    ERR_FUNC(host_diag_cw_get_chip_config(&capi_phy, &config_info, &port_info));
    }

    /*Build the KP4 port information*/
    host_diag_cw_kp4_gen_port_cfg((capi_function_mode_t)config_info.func_mode, port_info.port_idx, &port_cfg);
    host_diag_cw_kp4_gen_cw_mode_cfg(&config_info, &port_info, egr_or_igr, &port_cfg, &mode_parameter);

    fec_term = (egr_or_igr == EGR) ? config_info.egr_fec_term : config_info.igr_fec_term; 
    if((enum cw_port_fec_term_type_e)fec_term==CHIP_PORT_FEC_TERM_BYPASS || host_diag_cw_is_port_based_fec_mon(&capi_phy))
        ERR_FUNC(host_diag_cw_rptr_fec_stats_mon_config_handler(phy_info_ptr, capi_fec_dump_status_ptr, port_info.port_idx, 0));
    
    util_memcpy((void *)&capi_phy, phy_info_ptr, sizeof(capi_phy_info_t));
    capi_phy.base_addr = host_diag_cw_get_bbaddr_kp4deca(capi_fec_dump_status_ptr->fec_mode, capi_fec_dump_status_ptr->func_mode, port_info.port_idx, &mode_parameter);

    {
       host_diag_cw_rtmr_kpr4fec_dec_stat_disable(&capi_phy);

       /* clear fec stats */
       host_diag_cw_clear_fec_stats(&capi_phy, capi_fec_dump_status_ptr, &mode_parameter, port_info.port_idx);
       
    }

    util_memcpy((void *)&capi_phy, phy_info_ptr, sizeof(capi_phy_info_t));
    capi_phy.base_addr = OCTAL_TOP_REGS;
    return _diag_cw_clear_fec_rtmr_info(&capi_phy,  &mode_parameter, &port_cfg, capi_fec_dump_status_ptr->fec_mode);
}

/**
 * @brief     host_diag_cw_fec_stat_clear (capi_phy_info_t* phy_info_ptr, capi_fec_dump_status_t* capi_fec_dump_status_ptr, capi_function_mode_t func_mode, boolean clr_st);
 * @details    This function is used to clear FEC statistics status
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: phy information structure pointer
 * @param[in]  capi_fec_dump_status_ptr: FEC status dump structure pointer
 * @param[in]  func_mode: FUNCTION mode type
 * 
 * @return      returns the performance result of the called methode/function
 */
return_result_t host_diag_cw_fec_stat_clear(capi_phy_info_t* phy_info_ptr, capi_fec_dump_status_t* capi_fec_dump_status_ptr, capi_function_mode_t func_mode, boolean clr_st)
{
    capi_phy_info_t capi_phy;
    cw_mode_parameter_t mode_parameter;
    cw_port_info_t port_info;
    capi_config_ex_info_t config_info; 
    cw_port_config_t port_cfg;
    cfg_egr_or_igr_t egr_or_igr = (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_CLIENT) ? EGR : IGR;
    return_result_t return_result;

    if(capi_fec_dump_status_ptr->fec_mode != CAPI_FEC_CLIENT && capi_fec_dump_status_ptr->fec_mode != CAPI_FEC_LINE){
       return RR_ERROR_WRONG_INPUT_VALUE;
    }
    util_memcpy((void *)&capi_phy, phy_info_ptr, sizeof(capi_phy_info_t));
    capi_phy.base_addr = OCTAL_TOP_REGS;

    util_memset(&config_info, 0, sizeof(capi_config_ex_info_t));

    if (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_LINE) {
       config_info.line_lane.lane_mask = phy_info_ptr->lane_mask;   /* Using line-side lane_mask for identifying PORT */
    } else if (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_CLIENT) {
       config_info.host_lane.lane_mask = phy_info_ptr->lane_mask;   /* Using host-side lane_mask for identifying PORT */
    }
    /*retrieve the chip config mode*/
    if(host_diag_cw_is_port_based_fec_mon(&capi_phy)){
        ERR_FUNC(host_diag_cw_get_chip_config_fec_mon(&capi_phy, &config_info, &port_info));
    }else {
    ERR_FUNC(host_diag_cw_get_chip_config(&capi_phy, &config_info, &port_info));

    }
    /*Build the KP4 port information*/
    host_diag_cw_kp4_gen_port_cfg((capi_function_mode_t)config_info.func_mode, port_info.port_idx, &port_cfg);
    host_diag_cw_kp4_gen_cw_mode_cfg(&config_info, &port_info, egr_or_igr, &port_cfg, &mode_parameter);

    /* clear fec stats */
    host_diag_cw_clear_fec_stats(&capi_phy, capi_fec_dump_status_ptr, &mode_parameter, port_info.port_idx);

    if(clr_st){
       util_memcpy((void *)&capi_phy, phy_info_ptr, sizeof(capi_phy_info_t));
       capi_phy.base_addr = OCTAL_TOP_REGS;
       return_result = _diag_cw_clear_fec_rtmr_info(&capi_phy,  &mode_parameter, &port_cfg, capi_fec_dump_status_ptr->fec_mode);
       if (return_result != RR_SUCCESS)
           return return_result;       
    }
    return RR_SUCCESS;
}


/**
 * @brief     host_diag_cw_get_fec_info(capi_phy_info_t* phy_info_ptr, capi_fec_dump_status_t* capi_fec_dump_status_ptr, capi_function_mode_t func_mode);
 * @details    This function is used to get FEC statistics information
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: phy information structure pointer
 * @param[out]  capi_fec_dump_status_ptr: FEC status dump structure pointer
 * @param[in]  func_mode: FUNCTION mode type
 * 
 * @return      returns the performance result of the called methode/function
 */
/**
 * Accumulate KP4 FEC ERROR counter for error projection
 * @param phy device info
 * @param capi_fec_dump_status [in] fec error count structure
 * @param err_analyzer [out] fec error count structure for error projection
 * @return error value;
 */
static void _diag_cw_fec_error_analyzer_accumulate_err_count(capi_fec_dump_status_t *capi_fec_dump_status,
                                           cw_kp4fec_err_cnt_t *err_analyzer)
{
    int i;
    
    if ((capi_fec_dump_status->fec_mode == CAPI_FEC_LINE && capi_fec_dump_status->fec_type.line_fec == CAPI_LINE_FEC_TYPE_RS528) ||
       (capi_fec_dump_status->fec_mode == CAPI_FEC_CLIENT && capi_fec_dump_status->fec_type.host_fec == CAPI_HOST_FEC_TYPE_RS528)) {/* KR4 */
       err_analyzer->fec_frame_size = 5280; /* KP4 is 5440, KR4 is 5280 */
       err_analyzer->uncorr_bits_per_frame = 8;  /* KP4 is 16, KR4 is 8   */
    }
    else {  /* KP4 */
       err_analyzer->fec_frame_size = 5440;
       err_analyzer->uncorr_bits_per_frame = 16;
    }

    /* total frames */
    err_analyzer->tot_frame_cnt = capi_fec_dump_status->fec_a_err_cnt.tot_frame_rev_cnt + capi_fec_dump_status->fec_b_err_cnt.tot_frame_rev_cnt;

    /* correctable bit counts  */
    for (i = 0; i < FEC_TOT_BITS_CORR_NUM; i++) {
       err_analyzer->tot_bits_corr_cnt[i] = capi_fec_dump_status->fec_a_err_cnt.tot_bits_corr_cnt[i] + capi_fec_dump_status->fec_b_err_cnt.tot_bits_corr_cnt[i];
    }
    
    /* uncorrectable frames */
    err_analyzer->tot_frame_uncorr_cnt = capi_fec_dump_status->fec_a_err_cnt.tot_frame_uncorr_cnt + capi_fec_dump_status->fec_b_err_cnt.tot_frame_uncorr_cnt;
    /* last one is uncorrectable frames */
    err_analyzer->prbs_errcnt[err_analyzer->uncorr_bits_per_frame - 1] = err_analyzer->tot_frame_uncorr_cnt;
    /* correctable frames: number of cw with correstable symbol error = i + 1 */
    for (i = 0; i < err_analyzer->uncorr_bits_per_frame - 1; i++) {
       err_analyzer->prbs_errcnt[i] = capi_fec_dump_status->fec_a_err_cnt.tot_frames_err_cnt[i + 1]
          + capi_fec_dump_status->fec_b_err_cnt.tot_frames_err_cnt[i + 1];
    }

#ifdef FEC_ERROR_PROJ_VERBOSE
    for (i=0; i<err_analyzer->uncorr_bits_per_frame; i++) {
       if (i < (err_analyzer->uncorr_bits_per_frame-1)) {
          CAPI_LOG_INFO("\t FEC frames with   %2d Errors = %lu\n", i+1, (long unsigned int)err_analyzer->prbs_errcnt[i]);
       }
       else {
          CAPI_LOG_INFO("\t FEC frames with > %2d Errors = %lu\n", (err_analyzer->uncorr_bits_per_frame-1), (long unsigned int)err_analyzer->prbs_errcnt[i]);
       }
    }
#endif

    /* Calculating cumulative error count for respective values of "t" */
    for (i = (err_analyzer->uncorr_bits_per_frame - 2); i >= 0; i--) {
       /* simulate BH error projection behavior to saturate cumulative error count to 2^48-1 */
       if (err_analyzer->prbs_errcnt[i+1] >= FEC_COUNTER_LIMIT) {   /*changed 0xFFFFFFFFF (32bit) to 48bits;*/
          err_analyzer->prbs_errcnt[i] = FEC_COUNTER_LIMIT;
       }
       else {
          /* Check for saturation while accumulating from histogram bins */
          err_analyzer->prbs_errcnt[i] += err_analyzer->prbs_errcnt[i+1];
          if (err_analyzer->prbs_errcnt[i] >= FEC_COUNTER_LIMIT) {
            err_analyzer->prbs_errcnt[i] = FEC_COUNTER_LIMIT;
          }
       }
    }

    /* Display PRBS Error Analyzer Err_Counts */
    CAPI_LOG_INFO("\n -------------------------------------------------------------\n");
    CAPI_LOG_INFO("  FEC Error Analyzer Error_Counts:\n");
    CAPI_LOG_INFO(" -------------------------------------------------------------\n");
    for (i=0; i<err_analyzer->uncorr_bits_per_frame; i++) {
       if (err_analyzer->prbs_errcnt[i] == FEC_COUNTER_LIMIT)
          CAPI_LOG_INFO("    FEC Frames with > %2d Errors =       MAX \n", i);
       else
          CAPI_LOG_INFO("    FEC Frames with > %2d Errors = %lu \n", i, (long unsigned int)err_analyzer->prbs_errcnt[i]);
    }
    CAPI_LOG_INFO("    FEC uncorrectable frames = %lu \n", (long unsigned int) err_analyzer->tot_frame_uncorr_cnt);
    CAPI_LOG_INFO("    FEC total frames %lu \n", (long unsigned int)err_analyzer->tot_frame_cnt);
    CAPI_LOG_INFO(" -------------------------------------------------------------\n");    
}


/*----------------------------------------------------------------------------------------------------------------*/
/* Following calculations are used for projecting the Bit Error Rate (BER) from the PRBS Error Analyzer result    */
/*  - bit_err_rate    BER  = (num_frame_errors * frame_overlap_ratio) / (total frames * num_bits_per_frame)        */
/*                       where, frame_overlap_ratio = 1 / frame_overlap_factor                         */
/*                                                                                          */
/* Following guidelines are used while computing projected values -                                      */
/*   i) Only "frame_errors" that are > 10 are considered as valid data points for projection */
/*  ii) Need atleast 2 valid data points to generated projected data                                     */
/*----------------------------------------------------------------------------------------------------------------*/
static void _diag_cw_fec_error_analyzer_compute_proj(cw_kp4fec_err_cnt_t *err_analyzer,
    capi_fec_ber_t *fec_ber)
{
    cw_err_analyzer_report_st err_analyzer_rep = {0};
    cw_err_analyzer_report_st *err_analyzer_report = &err_analyzer_rep;
    uint8_t    delta_n;
    int8_t    i;
    uint8_t    start_idx, stop_idx;
    double meas_fec_ber[KP4_FEC_TOT_FRAMES_ERR_NUM + 1];
    double proj_ber[16];
    double x, y;
    double Exy = 0.0;
    double Eyy = 0.0;
    double Exx = 0.0;
    double Ey  = 0.0;
    double Ex  = 0.0;
    double alpha = 0.0;
    double beta = 0.0;
    
    /* Loop to calculate the measured BER based on above mentioned calculations */

    for(i = 0; i < err_analyzer->uncorr_bits_per_frame; i++) {
       meas_fec_ber[i] = (double)err_analyzer->prbs_errcnt[i] / err_analyzer->tot_frame_cnt / err_analyzer->fec_frame_size;
#ifdef FEC_ERROR_PROJ_VERBOSE
       CAPI_LOG_INFO("\t Measured Equivalent BER at 't=%2d' is %0.3e \n", i, meas_fec_ber[i]);
#endif
    }

    /* Calculating number of MEASURED points available for extrapolation */
    /* start_idx - first non-max FEC frame error value; stop_idx - index where FEC frame errors < 10 errors */
    start_idx = 0;
    stop_idx  = 0;
    if (err_analyzer->prbs_errcnt[err_analyzer->uncorr_bits_per_frame-1] != FEC_COUNTER_LIMIT) {
       stop_idx  = err_analyzer->uncorr_bits_per_frame-1;
    }
    for (i = err_analyzer->uncorr_bits_per_frame; i > 0; i--) {
       if (err_analyzer->prbs_errcnt[i - 1] != FEC_COUNTER_LIMIT) {
          start_idx = (uint8_t)(i - 1);
       }
       if (err_analyzer->prbs_errcnt[i - 1] < 10) {
          stop_idx = i - 1;
       }
    }
    delta_n = stop_idx - start_idx;                                        /* Number of MEASURED points available for extrapolation */

    /* Compute covariance and mean for extrapolation */
    if (delta_n >= 2) {                                                  /* There are at least 2 points to trace a line for extrapolation */
       for(i=start_idx; i < stop_idx; i++) {
          x    = (double) (i);
          y    = pow((-log10(meas_fec_ber[i])),KP4_PRBS_ERR_ANALYZER_LINEARITY_ADJUST);
          Exy += ((x * y)/(double)delta_n);
          Eyy += ((y * y)/(double)delta_n);
          Exx += ((x * x)/(double)delta_n);
          Ey  += ((  y  )/(double)delta_n);
          Ex  += ((  x  )/(double)delta_n);
       }
       
       /* Compute fit slope and offset: ber = alpha*frame_err_thresh + beta */
       alpha = (Exy - Ey*Ex) / (Exx - Ex*Ex);
       beta  = Ey - Ex*alpha;
    }

#ifdef FEC_ERROR_PROJ_VERBOSE
    CAPI_LOG_INFO("\n\t << Computing linear fit from Measured Equivalent BER points >>\n");
    CAPI_LOG_INFO("\t Number of measured points used for extrapolation = %d, start=%d, stop=%d\n",delta_n,start_idx,stop_idx);
    CAPI_LOG_INFO("\t Exy=%lf, Eyy=%lf, Exx=%lf, Ey=%lf, Ex=%lf\n",Exy,Eyy,Exx,Ey,Ex);    
    CAPI_LOG_INFO("\t alpha=%lf, beta=%lf\n",alpha,beta);    
    CAPI_LOG_INFO("\n\t=====> DEBUG INFO (end)\n\n");
#endif

    /* Calulate Projected BER => Equivalent projected post-FEC BER for t=15 */
    if (delta_n >= 2) {
       proj_ber[15] = ((alpha * 15) + beta);
       proj_ber[15] = pow(proj_ber[15],1.0/KP4_PRBS_ERR_ANALYZER_LINEARITY_ADJUST);
       proj_ber[15] = pow(10,-proj_ber[15]);
    }else {    
       proj_ber[15] = 0.0;
    }

    /* Populate output structure */ 
    err_analyzer_report->proj_ber  = proj_ber[15];
    err_analyzer_report->ber_proj_invalid  = 0;      /* To be populated: 0 -> valid, +1 -> BER greater than; -1 -> BER less than */
    err_analyzer_report->delta_n   = delta_n;

    /* Displaying Projected BER => Equivalent projected post-FEC BER for t=15 */
    if (err_analyzer_report->delta_n >= 2) {
       CAPI_LOG_INFO("\n  FEC Error Analyzer Projected BER (Equivalent projected post-FEC BER for t=15) = %0.3e\n\n", err_analyzer_report->proj_ber);
    }
    else {
       CAPI_LOG_INFO("\n << WARNING: Not enough valid measured points available for BER Projection! >>\n\n");
    }

    fec_ber->post_fec_ber = ((float)err_analyzer->tot_frame_uncorr_cnt * err_analyzer->uncorr_bits_per_frame) /
       err_analyzer->tot_frame_cnt / err_analyzer->fec_frame_size;
    fec_ber->pre_fec_ber = fec_ber->post_fec_ber + ((float)err_analyzer->tot_bits_corr_cnt[0] + err_analyzer->tot_bits_corr_cnt[1]) /
       err_analyzer->tot_frame_cnt / err_analyzer->fec_frame_size;   /* counter overflow? */
    fec_ber->post_fec_ber_proj = (float)err_analyzer_report->proj_ber;

    CAPI_LOG_INFO("\n pre-fec BER = %0.3e, post-fec BER = %0.3e, projected post-fec BER = %0.3e\n\n", fec_ber->pre_fec_ber,
       fec_ber->post_fec_ber, fec_ber->post_fec_ber_proj);
}
/* Calculate FEC BER and project post-fec BER */
static void _diag_cw_fec_prbs_calculate_ber(capi_fec_dump_status_t *fec_dump_status, uint8_t sampling_enabled) 
{
    cw_kp4fec_err_cnt_t err_analyzer = {0};
    capi_fec_ber_t *fec_ber = &fec_dump_status->fec_ber;

    /* copy FEC error struct to FEC error analyzer struct */
    _diag_cw_fec_error_analyzer_accumulate_err_count(fec_dump_status, &err_analyzer);

    /* If FEC unlock or total frame is 0, force pre&post FEC BER to 1 */
    if ((!sampling_enabled && fec_dump_status->fec_sts.fec_algn_stat == 0) || err_analyzer.tot_frame_cnt == 0) {
       fec_ber->pre_fec_ber = 1;
       fec_ber->post_fec_ber = 1;
       fec_ber->post_fec_ber_proj = 1;
    }else
    {    
       /* Compute and display PRBS Error Analyzer Projection */
       _diag_cw_fec_error_analyzer_compute_proj(&err_analyzer, fec_ber);
    }
}

/**
 * @brief     host_diag_cw_get_fec_info(capi_phy_info_t* phy_info_ptr, capi_fec_dump_status_t* capi_fec_dump_status_ptr, capi_function_mode_t func_mode);
 * @details    This function is used to get FEC statistics information
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: phy information structure pointer
 * @param[out]  capi_fec_dump_status_ptr: FEC status dump structure pointer
 * @param[in]  func_mode: FUNCTION mode type
 * @param[in]  sampling_enabled: is repeater mode FEC monitor sub-sampling enabled? (sampling rate < 100%)
 * 
 * @return       returns the performance result of the called methode/function
 */
return_result_t host_diag_cw_get_fec_info(capi_phy_info_t* phy_info_ptr, capi_fec_dump_status_t* capi_fec_dump_status_ptr, capi_function_mode_t func_mode)
{
    capi_phy_info_t capi_phy, capi_phy_b;
    uint8_t regcnt;
    kpr4fec_cnt_s kpr4fec_cntr, kpr4fec_cntr2;
    boolean enable;
    uint8_t fec_mon_sampling_enabled = 0;
    uint16_t reread_count =0;
    capi_phy_info_t capi_fec_phy;
    capi_config_ex_info_t config_info;
    cw_port_info_t port_info;
    cw_mode_parameter_t cw_mode;
    cw_port_config_t port_cfg;
    uint16_t fec_term;
    cfg_egr_or_igr_t egr_or_igr = (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_CLIENT) ? EGR : IGR;

    capi_fec_rx_err_cnt_t* fec_a_err_cnt     = &(capi_fec_dump_status_ptr->fec_a_err_cnt);
    capi_fec_rx_err_cnt_t* fec_b_err_cnt     = &(capi_fec_dump_status_ptr->fec_b_err_cnt);
    capi_fec_rx_status_t*      fec_sts      = &(capi_fec_dump_status_ptr->fec_sts);    

    /* Initialize status variables - so that only valid ones would get updated with new values */
    util_memset((void*)fec_a_err_cnt, 0, sizeof(capi_fec_rx_err_cnt_t));
    util_memset((void*)fec_b_err_cnt, 0, sizeof(capi_fec_rx_err_cnt_t));

    util_memcpy((void*)&capi_fec_phy, phy_info_ptr, sizeof(capi_phy_info_t));
    capi_fec_phy.base_addr = OCTAL_TOP_REGS; 
    util_memset((void*)fec_sts, 0, sizeof(capi_fec_rx_status_t));
    capi_fec_dump_status_ptr->fec_ber.pre_fec_ber = 1;
    capi_fec_dump_status_ptr->fec_ber.post_fec_ber = 1;
    capi_fec_dump_status_ptr->fec_ber.post_fec_ber_proj = 1;

    if((capi_fec_dump_status_ptr->fec_mode != CAPI_FEC_CLIENT && capi_fec_dump_status_ptr->fec_mode != CAPI_FEC_LINE)){
       return RR_ERROR_WRONG_INPUT_VALUE;
    }
    /*TBF: check the clock/reset is released or not. if under reset, directly return;*/
    util_memset(&config_info, 0, sizeof(capi_config_ex_info_t));
    if (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_LINE) {
       config_info.line_lane.lane_mask = phy_info_ptr->lane_mask;   /* Using line-side lane_mask for identifying PORT */
    } else if (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_CLIENT) {
       config_info.host_lane.lane_mask = phy_info_ptr->lane_mask;   /* Using host-side lane_mask for identifying PORT */
    }
    if(host_diag_cw_is_port_based_fec_mon(&capi_fec_phy)){
        ERR_FUNC(host_diag_cw_get_chip_config_fec_mon(&capi_fec_phy, &config_info, &port_info));
    }else {
    ERR_FUNC(host_diag_cw_get_chip_config(&capi_fec_phy, &config_info, &port_info));
    }
    /*Build the KP4 port information*/
    host_diag_cw_kp4_gen_port_cfg((capi_function_mode_t)config_info.func_mode, port_info.port_idx, &port_cfg);
    host_diag_cw_kp4_gen_cw_mode_cfg(&config_info, &port_info, egr_or_igr, &port_cfg, &cw_mode);
    util_memcpy((void *)&capi_phy, phy_info_ptr, sizeof(capi_phy_info_t));
    /*FEC counter only is avaliable when AM locked;*/
    capi_phy.base_addr = OCTAL_TOP_REGS + ml_cw_rtmr_get_base_addr(
        egr_or_igr, 
        &cw_mode);    // ((capi_fec_dump_status_ptr->fec_mode==CAPI_FEC_LINE)?RETIMER_IGR:RETIMER_EGR);

    capi_fec_dump_status_ptr->fec_sts.fec_algn_stat = (host_diag_cw_rtmr_fec_sync_status(&capi_phy, &cw_mode, &port_cfg))?TRUE:FALSE;

    /* check if repeater mode FEC monitor sub-sampling (FEC monitor sample rate < 100%) is enabled */
    //host_diag_cw_rtmr_get_fec_dec_err_cnt_reset_control(&capi_phy, &cw_mode, egr_or_igr, &fec_mon_sampling_enabled);
    fec_mon_sampling_enabled = host_diag_cw_rptr_fec_mon_get_sub_sampling(&capi_phy, egr_or_igr, port_info.port_idx);
        
    /* read KP4 FEC BRCM cnt (latch and read) */
    util_memset((void*)&kpr4fec_cntr, 0, sizeof(kpr4fec_cntr));
    util_memset((void*)&kpr4fec_cntr2, 0, sizeof(kpr4fec_cntr2));    
    capi_phy.base_addr = host_diag_cw_get_bbaddr_kp4deca(capi_fec_dump_status_ptr->fec_mode, capi_fec_dump_status_ptr->func_mode, port_info.port_idx, &cw_mode);
 
    host_diag_cw_rtmr_get_kpr4fec_dec_stat(&capi_phy, &enable);

    //if(capi_fec_dump_status_ptr->fec_sts.fec_algn_stat || fec_mon_sampling_enabled)
    {
       
       util_memcpy((void *)&capi_phy_b, (void *)&capi_phy, sizeof(capi_phy_info_t));
       if (func_mode == CAPI_MODE_200G)
          capi_phy_b.base_addr +=  LANE_OFFSET_KP4_KR4_FEC_DEC * 2;
       else if(func_mode == CAPI_MODE_400G)
          capi_phy_b.base_addr +=  LANE_OFFSET_KP4_KR4_FEC_DEC * 4;
       else if (func_mode == CAPI_MODE_100G && ((capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_LINE && cw_mode.line_fec_type == CHIP_LINE_FEC_TYPE_CL161) ||
               (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_CLIENT && cw_mode.host_fec_type == CHIP_HOST_FEC_TYPE_CL161))) {
           capi_phy_b.base_addr +=  LANE_OFFSET_KP4_KR4_FEC_DEC;
       }

       if(!fec_mon_sampling_enabled){
       /*latch ALL only is requested once*/
        host_diag_cw_rtmr_kpr4fec_dec_stat_md_latch_all(&capi_phy);
      }
      host_diag_cw_rtmr_kpr4fec_dec_cnt_get(&capi_phy, &kpr4fec_cntr, &cw_mode, fec_mon_sampling_enabled, &reread_count);

       /*200G/400G need read DECB also*/
       if (func_mode == CAPI_MODE_200G || func_mode == CAPI_MODE_400G ||
           (func_mode == CAPI_MODE_100G && ((capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_LINE && cw_mode.line_fec_type == CHIP_LINE_FEC_TYPE_CL161) ||
               (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_CLIENT && cw_mode.host_fec_type == CHIP_HOST_FEC_TYPE_CL161)))) {
          host_diag_cw_rtmr_kpr4fec_dec_cnt_get(&capi_phy_b, &kpr4fec_cntr2, &cw_mode, fec_mon_sampling_enabled, &reread_count);
       }

       /* Primary FEC Counters */
       fec_a_err_cnt->tot_bits_corr_cnt[0] = kpr4fec_cntr.kpr4fec_tot_bits_corr_cnt[0].err_cnt;
       fec_a_err_cnt->tot_bits_corr_cnt[1] = kpr4fec_cntr.kpr4fec_tot_bits_corr_cnt[1].err_cnt;
       for (regcnt = 0; regcnt < KP4_FEC_TOT_FRAMES_ERR_NUM; regcnt++) {
          fec_a_err_cnt->tot_frames_err_cnt[regcnt] = kpr4fec_cntr.kpr4fec_tot_frames_err_cnt[regcnt].err_cnt;
          fec_a_err_cnt->tot_symbols_corr_cnt += regcnt * fec_a_err_cnt->tot_frames_err_cnt[regcnt];
       }
       fec_a_err_cnt->tot_frame_rev_cnt = kpr4fec_cntr.kpr4fec_tot_frame_cnt.err_cnt;
       fec_a_err_cnt->tot_frame_uncorr_cnt = kpr4fec_cntr.kpr4fec_tot_frame_uncorr_cnt.err_cnt;
       fec_a_err_cnt->tot_frame_corr_cnt = fec_a_err_cnt->tot_frame_rev_cnt - fec_a_err_cnt->tot_frame_uncorr_cnt - fec_a_err_cnt->tot_frames_err_cnt[0];  

       /* Secondary FEC Counters for 200G & 400G */
       if (func_mode == CAPI_MODE_200G || func_mode == CAPI_MODE_400G ||
           (func_mode == CAPI_MODE_100G && ((capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_LINE && cw_mode.line_fec_type == CHIP_LINE_FEC_TYPE_CL161) ||
               (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_CLIENT && cw_mode.host_fec_type == CHIP_HOST_FEC_TYPE_CL161)))) {
          fec_b_err_cnt->tot_bits_corr_cnt[0] = kpr4fec_cntr2.kpr4fec_tot_bits_corr_cnt[0].err_cnt;
          fec_b_err_cnt->tot_bits_corr_cnt[1] = kpr4fec_cntr2.kpr4fec_tot_bits_corr_cnt[1].err_cnt;
          for (regcnt = 0; regcnt < KP4_FEC_TOT_FRAMES_ERR_NUM; regcnt++) {
             fec_b_err_cnt->tot_frames_err_cnt[regcnt] = kpr4fec_cntr2.kpr4fec_tot_frames_err_cnt[regcnt].err_cnt;
             fec_b_err_cnt->tot_symbols_corr_cnt += regcnt * fec_b_err_cnt->tot_frames_err_cnt[regcnt];  
          }
          fec_b_err_cnt->tot_frame_rev_cnt = kpr4fec_cntr2.kpr4fec_tot_frame_cnt.err_cnt;
          fec_b_err_cnt->tot_frame_uncorr_cnt = kpr4fec_cntr2.kpr4fec_tot_frame_uncorr_cnt.err_cnt;
          fec_b_err_cnt->tot_frame_corr_cnt = fec_b_err_cnt->tot_frame_rev_cnt - fec_b_err_cnt->tot_frame_uncorr_cnt - fec_b_err_cnt->tot_frames_err_cnt[0];  
       }
    }

    /* FEC Decoder Stats - live and sticky internal*/
    fec_term = (egr_or_igr == EGR) ? config_info.egr_fec_term : config_info.igr_fec_term; 
    _diag_cw_get_fec_rtmr_info(&capi_fec_phy, &cw_mode, &port_cfg, capi_fec_dump_status_ptr->fec_mode, fec_term, fec_mon_sampling_enabled, fec_sts);
    _diag_cw_fec_prbs_calculate_ber(capi_fec_dump_status_ptr, fec_mon_sampling_enabled);
    return RR_SUCCESS;
}

/**
* @brief     host_diag_cw_calculate_ber_between_latch(capi_phy_info_t *phy_ptr, capi_kp4fec_ber_state_t* fec_ptr)
* @details    This API is used to calculate the min/max/average error frame and BER based on CMIS request
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  capi_fec_dump_status_ptr: this parameter pointer current hardware FEC counter 
* @param[in/out]  fec_ptr: this parameter pointer current fec counter post processing number; 
* 
* @return    returns the performance result of the called methode/function
*/
return_result_t host_diag_cw_calculate_ber_between_latch(capi_phy_info_t *phy_ptr,  capi_fec_dump_status_t* capi_fec_dump_status_ptr, capi_kp4fec_ber_state_t* fec_ptr)
{
    return_result_t ret_val = RR_SUCCESS;
    uint16_t total_uncorr_bpf = 16, regcnt;
    int16_t bpf = 5440;
    cw_kp4fec_err_cnt_t hw_err;
#ifdef CAPI_CMIS_FEC_STATS_SMALL_FOOTPRINT
    cw_kp4fec_err_cnt_t cur_err = {0};
    cw_kp4fec_err_cnt_t *cur_err_ptr = &cur_err;
#else
    cw_kp4fec_err_cnt_t *cur_err_ptr = &fec_ptr->cur_err;
#endif

    if(capi_fec_dump_status_ptr==NULL || fec_ptr==NULL){
       return RR_ERROR_WRONG_INPUT_VALUE;
    }
    /*update the frame size if KR4*/
    if ((capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_LINE && capi_fec_dump_status_ptr->fec_type.line_fec == CAPI_LINE_FEC_TYPE_RS528) ||
       (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_CLIENT && capi_fec_dump_status_ptr->fec_type.host_fec == CAPI_HOST_FEC_TYPE_RS528)) {
       bpf = 5280;
       total_uncorr_bpf = 8;
    }
    /*Post processing counter and calculate the FEC BER*/
    for (regcnt = 0; regcnt < total_uncorr_bpf; regcnt++) {
       hw_err.prbs_errcnt[regcnt] = capi_fec_dump_status_ptr->fec_a_err_cnt.tot_frames_err_cnt[regcnt] + capi_fec_dump_status_ptr->fec_b_err_cnt.tot_frames_err_cnt[regcnt];
    }

    for (regcnt = 0; regcnt < KP4_FEC_TOT_BITS_CORR_NUM; regcnt++) {
       hw_err.tot_bits_corr_cnt[regcnt] = capi_fec_dump_status_ptr->fec_a_err_cnt.tot_bits_corr_cnt[regcnt] + capi_fec_dump_status_ptr->fec_b_err_cnt.tot_bits_corr_cnt[regcnt];
    }
    hw_err.tot_frame_cnt = capi_fec_dump_status_ptr->fec_a_err_cnt.tot_frame_rev_cnt + capi_fec_dump_status_ptr->fec_b_err_cnt.tot_frame_rev_cnt;
    hw_err.tot_frame_uncorr_cnt = capi_fec_dump_status_ptr->fec_a_err_cnt.tot_frame_uncorr_cnt + capi_fec_dump_status_ptr->fec_b_err_cnt.tot_frame_uncorr_cnt;

    /*calculate the counter difference, and accumulate counters*/
    for (regcnt = 0; regcnt < total_uncorr_bpf; regcnt++) {
       if (hw_err.prbs_errcnt[regcnt] >= fec_ptr->backup_err.prbs_errcnt[regcnt]){
          cur_err_ptr->prbs_errcnt[regcnt] = (hw_err.prbs_errcnt[regcnt] - fec_ptr->backup_err.prbs_errcnt[regcnt]);
       }else{
          cur_err_ptr->prbs_errcnt[regcnt] = (((uint64_t)1 << 48) - fec_ptr->backup_err.prbs_errcnt[regcnt] + hw_err.prbs_errcnt[regcnt]);
       }
       /* record T-code which is the max symbol errors per frame  */
       if (cur_err_ptr->prbs_errcnt[regcnt])
           cur_err_ptr->t_code = (uint8_t)regcnt;
       if(fec_ptr->only_cur == 0){
          fec_ptr->total_err.prbs_errcnt[regcnt] += cur_err_ptr->prbs_errcnt[regcnt];
       }
    }

    for (regcnt = 0; regcnt < KP4_FEC_TOT_BITS_CORR_NUM; regcnt++) {
       if (hw_err.tot_bits_corr_cnt[regcnt] >= fec_ptr->backup_err.tot_bits_corr_cnt[regcnt]){
          cur_err_ptr->tot_bits_corr_cnt[regcnt] = (hw_err.tot_bits_corr_cnt[regcnt] - fec_ptr->backup_err.tot_bits_corr_cnt[regcnt]);
       }else{
          cur_err_ptr->tot_bits_corr_cnt[regcnt] = (((uint64_t)1 << 48) - fec_ptr->backup_err.tot_bits_corr_cnt[regcnt] +hw_err.tot_bits_corr_cnt[regcnt]);
       }
       if(fec_ptr->only_cur == 0){
          fec_ptr->total_err.tot_bits_corr_cnt[regcnt] += cur_err_ptr->tot_bits_corr_cnt[regcnt];
       }
    }

    if (hw_err.tot_frame_uncorr_cnt >= fec_ptr->backup_err.tot_frame_uncorr_cnt){
       cur_err_ptr->tot_frame_uncorr_cnt = (hw_err.tot_frame_uncorr_cnt - fec_ptr->backup_err.tot_frame_uncorr_cnt);
    }else{
       cur_err_ptr->tot_frame_uncorr_cnt = (((uint64_t)1 << 48) - fec_ptr->backup_err.tot_frame_uncorr_cnt + hw_err.tot_frame_uncorr_cnt);
    }
    fec_ptr->cur_err.tot_frame_uncorr_cnt = cur_err_ptr->tot_frame_uncorr_cnt;
    fec_ptr->cur_err.t_code = cur_err_ptr->t_code;

    if(fec_ptr->only_cur == 0){
       fec_ptr->total_err.tot_frame_uncorr_cnt += cur_err_ptr->tot_frame_uncorr_cnt;
    }

    if (hw_err.tot_frame_cnt >= fec_ptr->backup_err.tot_frame_cnt){
       cur_err_ptr->tot_frame_cnt = (hw_err.tot_frame_cnt - fec_ptr->backup_err.tot_frame_cnt);
    }else{
       cur_err_ptr->tot_frame_cnt = (((uint64_t)1 << 48) - fec_ptr->backup_err.tot_frame_cnt + hw_err.tot_frame_cnt);
    }
    if(fec_ptr->only_cur == 0){
       fec_ptr->total_err.tot_frame_cnt += cur_err_ptr->tot_frame_cnt;
    }

    /*back-up current reading counter for next cycle*/
    fec_ptr->err_latch_times ++;
    for (regcnt = 0; regcnt < total_uncorr_bpf; regcnt++) {
       fec_ptr->backup_err.prbs_errcnt[regcnt] = hw_err.prbs_errcnt[regcnt];
    }
    for (regcnt = 0; regcnt < KP4_FEC_TOT_BITS_CORR_NUM; regcnt++) {
       fec_ptr->backup_err.tot_bits_corr_cnt[regcnt] = hw_err.tot_bits_corr_cnt[regcnt];
    }
    fec_ptr->backup_err.tot_frame_cnt = hw_err.tot_frame_cnt;
    fec_ptr->backup_err.tot_frame_uncorr_cnt = hw_err.tot_frame_uncorr_cnt;

    /* If total frame is 0, force pre&post FEC BER to 1*/
    if (cur_err_ptr->tot_frame_cnt ) {
       fec_ptr->pre_fec_ber.cur_ber = (double)(cur_err_ptr->tot_bits_corr_cnt[0] + cur_err_ptr->tot_bits_corr_cnt[1] + cur_err_ptr->tot_frame_uncorr_cnt * total_uncorr_bpf) / cur_err_ptr->tot_frame_cnt / (double)bpf;
       fec_ptr->post_fec_ber.cur_ber = (double)(cur_err_ptr->tot_frame_uncorr_cnt * total_uncorr_bpf) / cur_err_ptr->tot_frame_cnt / (double)bpf;
       if(fec_ptr->only_cur == 0){
          fec_ptr->pre_fec_ber.total_ber += fec_ptr->pre_fec_ber.cur_ber;
          fec_ptr->post_fec_ber.total_ber += fec_ptr->post_fec_ber.cur_ber;
       }
    } else {
       return RR_ERROR;
    }
    
    if(fec_ptr->only_cur == 0){
       /*total uncorrect frame counter*/
       if(fec_ptr->min_err.tot_frame_uncorr_cnt > cur_err_ptr->tot_frame_uncorr_cnt)
          fec_ptr->min_err.tot_frame_uncorr_cnt = cur_err_ptr->tot_frame_uncorr_cnt;
       if(fec_ptr->max_err.tot_frame_uncorr_cnt < cur_err_ptr->tot_frame_uncorr_cnt)
          fec_ptr->max_err.tot_frame_uncorr_cnt = cur_err_ptr->tot_frame_uncorr_cnt;
       fec_ptr->average_err.tot_frame_uncorr_cnt = fec_ptr->total_err.tot_frame_uncorr_cnt/fec_ptr->err_latch_times;
#ifndef CAPI_CMIS_FEC_STATS_SMALL_FOOTPRINT
       /*calculate the Min/max/average*/
       /*total frame counter*/
       if(fec_ptr->min_err.tot_frame_cnt > cur_err_ptr->tot_frame_cnt)
          fec_ptr->min_err.tot_frame_cnt = cur_err_ptr->tot_frame_cnt;
       if(fec_ptr->max_err.tot_frame_cnt < cur_err_ptr->tot_frame_cnt)
          fec_ptr->max_err.tot_frame_cnt = cur_err_ptr->tot_frame_cnt;
       fec_ptr->average_err.tot_frame_cnt = fec_ptr->total_err.tot_frame_cnt/fec_ptr->err_latch_times;
       /*correctable 0's & 1's counter*/
       for (regcnt = 0; regcnt < KP4_FEC_TOT_BITS_CORR_NUM; regcnt++) {
          if(fec_ptr->min_err.tot_bits_corr_cnt[regcnt] > cur_err_ptr->tot_bits_corr_cnt[regcnt])
             fec_ptr->min_err.tot_bits_corr_cnt[regcnt] = cur_err_ptr->tot_bits_corr_cnt[regcnt];
          if(fec_ptr->max_err.tot_bits_corr_cnt[regcnt] < cur_err_ptr->tot_bits_corr_cnt[regcnt])
             fec_ptr->max_err.tot_bits_corr_cnt[regcnt] = cur_err_ptr->tot_bits_corr_cnt[regcnt];
          fec_ptr->average_err.tot_bits_corr_cnt[regcnt] = fec_ptr->total_err.tot_bits_corr_cnt[regcnt]/fec_ptr->err_latch_times;
       }
       /*correct symbol frame counter*/
       for (regcnt = 0; regcnt < total_uncorr_bpf; regcnt++) {
          if(fec_ptr->min_err.prbs_errcnt[regcnt] > cur_err_ptr->prbs_errcnt[regcnt])
             fec_ptr->min_err.prbs_errcnt[regcnt] = cur_err_ptr->prbs_errcnt[regcnt];
          if(fec_ptr->max_err.prbs_errcnt[regcnt] < cur_err_ptr->prbs_errcnt[regcnt])
             fec_ptr->max_err.prbs_errcnt[regcnt] = cur_err_ptr->prbs_errcnt[regcnt];
          fec_ptr->average_err.prbs_errcnt[regcnt] = fec_ptr->total_err.prbs_errcnt[regcnt]/fec_ptr->err_latch_times;
       }
#endif
       fec_ptr->total_err.t_code += cur_err_ptr->t_code;
       fec_ptr->average_err.t_code = fec_ptr->total_err.t_code/fec_ptr->err_latch_times;

       if(fec_ptr->min_err.t_code > cur_err_ptr->t_code)
          fec_ptr->min_err.t_code = cur_err_ptr->t_code;
       if(fec_ptr->max_err.t_code < cur_err_ptr->t_code)
          fec_ptr->max_err.t_code = cur_err_ptr->t_code;

       /*pre FEC BER*/
       if(fec_ptr->pre_fec_ber.min_ber > fec_ptr->pre_fec_ber.cur_ber)
          fec_ptr->pre_fec_ber.min_ber = fec_ptr->pre_fec_ber.cur_ber;
       if(fec_ptr->pre_fec_ber.max_ber < fec_ptr->pre_fec_ber.cur_ber)
          fec_ptr->pre_fec_ber.max_ber = fec_ptr->pre_fec_ber.cur_ber;
       fec_ptr->pre_fec_ber.average_ber = fec_ptr->pre_fec_ber.total_ber/fec_ptr->err_latch_times;
       /*post FEC BER*/
       if(fec_ptr->post_fec_ber.min_ber > fec_ptr->post_fec_ber.cur_ber)
          fec_ptr->post_fec_ber.min_ber = fec_ptr->post_fec_ber.cur_ber;
       if(fec_ptr->post_fec_ber.max_ber < fec_ptr->post_fec_ber.cur_ber)
          fec_ptr->post_fec_ber.max_ber = fec_ptr->post_fec_ber.cur_ber;
       fec_ptr->post_fec_ber.average_ber = fec_ptr->post_fec_ber.total_ber/fec_ptr->err_latch_times;
    }
    return ret_val;
}

/**
 *
 * @file    host_diag_lw_p.c
 * @author  Firmware Team
 * @date    2/01/2017
 * @version 0.1
 *
 * @property 
 * $Copyright: (c) 2024 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * @brief    brief description of the block source file, sequences/flow/etc.
 *
 * @section  description
 *
 */

/** @file host_diag_lw_p.c
 *  cAPI chal warpper implementation
 */
#include "hr_time.h"
#include "chip_config_def.h"
#include "regs_common.h"
#include "chip_common_config_ind.h"
#include "chip_mode_def.h"
#include "capi_def.h"
#include "capi_test_def.h"
#include "capi_diag_def.h"
#include "capi_custom_def.h"
#include "chal_lw_diag_info.h"
#include "common_util.h"
#include "dsp_utils.h"
#include "host_log_util.h"
#include "host_lw_wrapper.h"
#include "host_diag.h"
#include "host_diag_lw.h"

static uint8_t gray_2_bin(uint8_t input) 
{
    input = input ^ (input >> 4);
    input = input ^ (input >> 2);
    input = input ^ (input >> 1);
    return input;
}

static return_result_t _lw_get_lvl_snr(phy_info_t* phy_info_ptr, uint8_t lane_idx, lw_level_snr_value_t *level_snr_value, float *snr_ptr)
{
    uint32_t mse_val, mseval_lvl[8]={0};
    uint8_t idx;
    uint8_t slicer_mode;
    phy_info_t  lw_phy;
    uint8_t eq2_en, ffe_multiplier;
    dsp_fast_brcm_snr_cfg_t snr_cfg = {0};
    
    lw_util_init_lane_config_base_addr(phy_info_ptr, &lw_phy, lane_idx);

    slicer_mode = ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x000055c4)), (phy_info_ptr)->phy_id) & (0x00000c00)) >> (10));

    snr_cfg.fields.lineid     = lane_idx;
    snr_cfg.fields.slice_mode = slicer_mode;
    snr_cfg.fields.snr_level  = ((slicer_mode== NRZ_MODE)||(slicer_mode== EPAM2_MODE))?2:4;
    snr_cfg.fields.snr_mval   = ((slicer_mode== NRZ_MODE)||(slicer_mode== EPAM2_MODE))?9:5;
    
    do { int __hsip_err; *(&__hsip_err) = 0; (eq2_en = ((rd_reg_ex((((&lw_phy)->base_addr) + ((((0 + 32) + 32) + 32))), (&lw_phy)->phy_id) & (0x00002000)) >> (13))); if (*(&__hsip_err) != 0) { return RR_ERROR_MDIO; } (void)__hsip_err; } while(0);
    do { int __hsip_err; *(&__hsip_err) = 0; (ffe_multiplier = ((rd_reg_ex((((&lw_phy)->base_addr) + ((((((((0 + 32) + 32) + 32) + 32) + 32) + 32) + 32))), (&lw_phy)->phy_id) & (0x00006000)) >> (13))); if (*(&__hsip_err) != 0) { return RR_ERROR_MDIO; } (void)__hsip_err; } while(0);
    
    if(eq2_en){
        chal_lw_diag_rx_dfe_mse_read(phy_info_ptr, &snr_cfg, &mse_val, mseval_lvl);
        if(ffe_multiplier){
            mse_val = mse_val>>2;
            for(idx=1; idx<=mseval_lvl[0]; idx++)
                mseval_lvl[idx] = mseval_lvl[idx] >>2;
        }
    }else
        chal_lw_diag_ffe_mse_get (phy_info_ptr, &snr_cfg, &mse_val, mseval_lvl);
    
    level_snr_value->snr_levels = mseval_lvl[0];    
    for (idx = 0; idx < mseval_lvl[0]; idx++) {
        level_snr_value->snr_value[idx] = (float)chal_lw_diag_calculate_snr(slicer_mode, mseval_lvl[idx+1]);
    }
    *snr_ptr = (float)chal_lw_diag_calculate_snr(slicer_mode, mse_val);
    return RR_SUCCESS;
}

return_result_t host_diag_get_lvl_snr(phy_info_t* phy_info_ptr, uint8_t lane_index, float *level_snr_ptr, float *snr_ptr)
{
    lw_level_snr_value_t level_snr_value;
    uint8_t i;
        
    _lw_get_lvl_snr(phy_info_ptr, lane_index, &level_snr_value, snr_ptr);
    if (level_snr_value.snr_levels > 7)
        level_snr_value.snr_levels = 7;
    for (i = 0; i < level_snr_value.snr_levels; i++)
        level_snr_ptr[i] = level_snr_value.snr_value[i];
    
    return RR_SUCCESS;
}

uint8_t host_diag_get_vga(phy_info_t* phy_info_ptr)
{
    
    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00006000)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00006000)), (phy_info_ptr)->phy_id) & (~(0x00000100))) | (((1) << (8)) & (0x00000100))), (phy_info_ptr)->phy_id)));
    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00006000)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00006000)), (phy_info_ptr)->phy_id) & (~(0x00000100))) | (((0) << (8)) & (0x00000100))), (phy_info_ptr)->phy_id)));
    return (uint8_t)((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00006024)), (phy_info_ptr)->phy_id) & (0x0000007f)) >> (0));
}

return_result_t host_diag_get_phase_bias(phy_info_t* phy_info_ptr, phy_info_t* cfg_phy_info_ptr, int8_t *phase_bias_ptr, int8_t *static_phase_bias_ptr)
{
    *phase_bias_ptr = (int8_t)((((13) == 15) ? (((int16_t)((((rd_reg_ex(((phy_info_ptr)->base_addr) + (0x00004d08), (phy_info_ptr)->phy_id)) & ((1 << ((13) + 1)) - 1)) >> (0)))) >> (0)) : ((((13) == 7) ? (((int8_t)((((rd_reg_ex(((phy_info_ptr)->base_addr) + (0x00004d08), (phy_info_ptr)->phy_id)) & ((1 << ((13) + 1)) - 1)) >> (0)))) >> (0)) : ((signext((((rd_reg_ex(((phy_info_ptr)->base_addr) + (0x00004d08), (phy_info_ptr)->phy_id)) & ((1 << ((13) + 1)) - 1)) >> (0)), (1 << (13)))) >> (0))))) / 80);
    *static_phase_bias_ptr = ((rd_reg_ex((((cfg_phy_info_ptr)->base_addr) + ((((((((((((((((((((((((((((((((((0 + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32))), (cfg_phy_info_ptr)->phy_id) & (0x0000FF00)) >> (8));
    return RR_SUCCESS;
}

return_result_t host_diag_get_ffe_saturation(phy_info_t* phy_info_ptr, uint32_t *tap_under_saturation, uint32_t *tap_over_saturation)
{
    ffe_saturation_t ffe_stat;

    
    chal_lw_diag_ffe_get_saturation_st(phy_info_ptr, &ffe_stat);
    *tap_under_saturation = ((ffe_stat.under_stat_h << 16) | ffe_stat.under_stat_l);
    *tap_over_saturation = ((ffe_stat.over_stat_h << 16) | ffe_stat.over_stat_l);
    chal_lw_diag_ffe_write_clear_saturation_st(phy_info_ptr);        
	return RR_SUCCESS;
}

return_result_t host_diag_get_ffe_taps(phy_info_t* phy_info_ptr, float* ffe_taps_ptr)
{
    int32_t coef[9];
    return chal_lw_diag_ffe_capture_9_ffe_taps(phy_info_ptr, 1, 0, coef, ffe_taps_ptr);
}

uint32_t host_diag_get_adcclip(phy_info_t* phy_info_ptr, uint8_t lane_index, uint8_t th)
{    
    uint32_t det_err;
    phy_info_t lw_phy;
    clipdet_config_t clipdet_cfg;
    uint8_t timer_maxed;
    uint8_t enabled;
        
    lw_util_init_lane_hw_base_addr(phy_info_ptr, &lw_phy, lane_index);
            
    
    clipdet_cfg.adc_clp_exp_timer = 26;
    clipdet_cfg.adc_clp_neg_en = 1;
    clipdet_cfg.adc_clp_sample_sel = 0;
    clipdet_cfg.adc_clp_sample_rrbin = 1;
    clipdet_cfg.adc_clp_pos_en = 1;
    
    clipdet_cfg.adc_clp_neg_thr = -th;
    clipdet_cfg.adc_clp_pos_thr = th;
    
    chal_lw_diag_rx_clipdet_config (&lw_phy, &clipdet_cfg);
    chal_lw_diag_rx_clipdet_get_enable(&lw_phy, &enabled);
    chal_lw_diag_rx_clipdet_clear(&lw_phy, 0);
    chal_lw_diag_rx_clipdet_enable (&lw_phy);

    delay_ms(1000);
    chal_lw_diag_rx_chk_clipdet(&lw_phy, &timer_maxed, &det_err);
    chal_lw_diag_rx_clipdet_disable (&lw_phy);
    return det_err;

}

uint8_t host_diag_get_afe_bw(phy_info_t* phy_info_ptr)
{
    return (uint8_t)((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00004b18)), (phy_info_ptr)->phy_id) & (0x0000007e)) >> (1));
}

uint8_t host_diag_get_gain_boost(phy_info_t* phy_info_ptr)
{
    return gray_2_bin((uint8_t)((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00004a04)), (phy_info_ptr)->phy_id) & (0x0000f800)) >> (11)));
}

uint8_t host_diag_get_peaking(phy_info_t* phy_info_ptr)
{
    return gray_2_bin((uint8_t)((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00004a04)), (phy_info_ptr)->phy_id) & (0x0000001f)) >> (0)));
}

uint8_t host_diag_get_processor_status(phy_info_t* phy_info_ptr)
{
    capi_phy_info_t cfg_phy = *phy_info_ptr;
    uint8_t core_id;
    uint16_t core_status[4];
    uint8_t processor_status = 0;
    
    
    cfg_phy.base_addr = 0x52000000;
    for(core_id=0; core_id<4; core_id++){
        core_status[core_id] = (uint16_t)(rd_reg_ex(((&cfg_phy)->base_addr) + (0x0001c890 + (core_id<<2)), (&cfg_phy)->phy_id));
    }
    delay_ms(100);
    for(core_id=0; core_id<4; core_id++){
        if (core_status[core_id] != (rd_reg_ex(((&cfg_phy)->base_addr) + (0x0001c890 + (core_id<<2)), (&cfg_phy)->phy_id)))
            processor_status |= (1 << core_id);
    }
    return processor_status;
}

uint8_t host_diag_get_cdr_lock_counter(phy_info_t* phy_info_ptr, uint8_t lane_id)
{
    capi_phy_info_t cfg_phy;

    lw_util_init_lane_config_base_addr(phy_info_ptr, &cfg_phy, lane_id);
    return (uint8_t)((rd_reg_ex((((&cfg_phy)->base_addr) + (((((((((((((0 + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32))), (&cfg_phy)->phy_id) & (0x0000000F)) >> (0));
}

return_result_t host_diag_get_ppm(phy_info_t* phy_info_ptr, float *tx_ppm_ptr, float *rx_ppm_ptr)
{
    int32_t integ_val;
    int16_t tx_integ_val = 0;

    integ_val = chal_lw_diag_rx_cdr_get_integ_ppm(phy_info_ptr);
    
    *rx_ppm_ptr = (float)((((float)integ_val) / (1 << 29)) * 1000000.0 / 80.0 / 128.0);

    chal_lw_diag_hsip_jf_rd_integ(phy_info_ptr, &tx_integ_val);    
    *tx_ppm_ptr = (float)tx_integ_val / (2.62144f * 2);
    return RR_SUCCESS;
}

return_result_t host_diag_get_port_status(phy_info_t* phy_info_ptr, uint16_t *port_status_ptr)
{
    capi_phy_info_t cfg_phy = *phy_info_ptr;
    uint8_t tidx;

    for(tidx=0; tidx<8; tidx++){
        cfg_phy.base_addr = 0x52000000 + (tidx<<2);
        port_status_ptr[tidx] = (uint16_t)(rd_reg_ex(((&cfg_phy)->base_addr) + (0x0001ca48), (&cfg_phy)->phy_id));
        (wr_reg_ex((((&cfg_phy)->base_addr) + (0x0001ca48)), 0, (&cfg_phy)->phy_id));
    }
    return RR_SUCCESS;
}

return_result_t host_diag_get_hw_histogram(phy_info_t* phy_info_ptr, phy_info_t* cfg_phy_info_ptr, bool first_time, uint32_t *lw_slicer_histogram)
{
    
    uint8_t i, finished = 0, bin_cap, timer_cap, hist_mode;
    hist_cfg_t hist_cfg = {0};
    int counter = 0;

    if (first_time) {
        chal_lw_diag_rx_hist_en(phy_info_ptr, 1);
        hist_cfg.hist_sel = 0;  
        do { int __hsip_err; *(&__hsip_err) = 0; (bin_cap = (uint8_t)((rd_reg_ex((((cfg_phy_info_ptr)->base_addr) + ((((((((((((((((((((((((((((((0 + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32))), (cfg_phy_info_ptr)->phy_id) & (0x000007C0)) >> (6))); if (*(&__hsip_err) != 0) { return RR_ERROR_MDIO; } (void)__hsip_err; } while(0);
        bin_cap = (bin_cap>27)?27:bin_cap;
        if(bin_cap){
            hist_cfg.bin_cap = (1<<bin_cap)-1;
        }else
             hist_cfg.bin_cap = 0x7FFFFFF; 
        do { int __hsip_err; *(&__hsip_err) = 0; (timer_cap = ((rd_reg_ex((((cfg_phy_info_ptr)->base_addr) + ((((((((((((((((((((((((((((((0 + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32))), (cfg_phy_info_ptr)->phy_id) & (0x0000003F)) >> (0))); if (*(&__hsip_err) != 0) { return RR_ERROR_MDIO; } (void)__hsip_err; } while(0);
        timer_cap = (timer_cap>32)?32:timer_cap;
        if(timer_cap){
            hist_cfg.timer_cap = (timer_cap==32)?0xFFFFFFFF:((1<<timer_cap)-1);
        }else
            hist_cfg.timer_cap = 0xFFFFFFFF; 
        do { int __hsip_err; *(&__hsip_err) = 0; (hist_mode = ((rd_reg_ex((((cfg_phy_info_ptr)->base_addr) + ((((((((((((((((((((((((((((((0 + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32))), (cfg_phy_info_ptr)->phy_id) & (0x00000800)) >> (11))); if (*(&__hsip_err) != 0) { return RR_ERROR_MDIO; } (void)__hsip_err; } while(0);
        if(hist_mode){
            do { int __hsip_err; *(&__hsip_err) = 0; (hist_cfg.hist_mode = ((rd_reg_ex((((cfg_phy_info_ptr)->base_addr) + ((((((((((((((((((((((((((((((0 + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32) + 32))), (cfg_phy_info_ptr)->phy_id) & (0x0003000)) >> (12))); if (*(&__hsip_err) != 0) { return RR_ERROR_MDIO; } (void)__hsip_err; } while(0);
        }else
            hist_cfg.hist_mode = 2; 
        chal_lw_diag_rx_hist_config(phy_info_ptr, &hist_cfg);
    }
    for (i=0; i<1; i++) {
    chal_lw_diag_rx_hist_start(phy_info_ptr, 1);
    do {
        chal_lw_diag_rx_hist_check_finish(phy_info_ptr, &finished);
        delay_ms(10);
        counter++;
    } while ((!finished)&&(counter<1000));
    chal_lw_diag_rx_rd_hist(phy_info_ptr, 0, lw_slicer_histogram);
    chal_lw_diag_rx_hist_start(phy_info_ptr, 0);
    chal_lw_diag_rx_hist_clear(phy_info_ptr, 1);
    chal_lw_diag_rx_hist_clear(phy_info_ptr, 0);
    }
    return RR_SUCCESS;
}

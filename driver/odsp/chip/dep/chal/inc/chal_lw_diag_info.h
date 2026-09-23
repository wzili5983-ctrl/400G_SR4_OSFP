/**
 *        cHAL C Hardware Abstract Layer for Line Wrapper CAPture MEMory IP
 *
 * @file chal_lw_diag_info.c
 * @author 
 * @date 9-5-2018
 * * @property   $Copyright: Copyright 2018 Broadcom INC.
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
 * Except as expressly set forth in the Authorized License,
 *
 * 1.     This program, including its structure, sequence and organization,
 * constitutes the valuable trade secrets of Broadcom, and you shall use
 * all reasonable efforts to protect the confidentiality thereof,
 * and to use this information only in connection with your use of
 * Broadcom integrated circuit products.
 *
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS
 * PROVIDED "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
 * REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
 * OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
 * DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
 * NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
 * ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
 * OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL
 * BROADCOM OR ITS LICENSORS BE LIABLE FOR   CONSEQUENTIAL,
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
#ifndef CHAL_LW_DIAG_INFO_H
#define CHAL_LW_DIAG_INFO_H

#ifdef __cplusplus
extern "C" {
#endif

/*

 * Enumeration of Line Wrapper RX FFE slicer mode  values
 */
enum {
     NRZ_MODE =0,     /*!<  NRZ mode */
     EPAM2_MODE =1,   /*!<  EPAM2 mode */
     PAM4_MODE =2,    /*!<  PAM4 mode */
     EPAM4_MODE =3    /*!<  EPAM4 mode */
}; /*!<  Line Wrapper RX FFE tap value */
typedef uint8_t ffe_slc_mode_t;  // force the enum to 1 bytes

/**
 * Enumeration of Line Wrapper RX FFE  LMS mode values
 */
enum {
     FFE_LMS1_MODE =0,  /*!<  FFE 1  LMS  mode */
     FFE_LMS8_MODE =1   /*!<  FFE 8  LMS  mode */
}; /**<  Line Wrapper RX LMS mode value */
typedef uint8_t ffe_lms_mode_t;  // force the enum to 1 bytes

/**
 * Enumeration of Line Wrapper RX FFE LOAD  values
 */
typedef enum {
     LD_LOW =0,  /*!<  FFE register write low */
     LD_HIGH =1   /*!<  FFE register write high */
} ffe_reg_ld_t; /**<  Line Wrapper RX FFE tap value */

typedef struct clipdet_config_s {

    uint8_t                             adc_clp_exp_timer;    /**<  5 bit */
    uint8_t                             adc_clp_sample_sel;   /**<  4 bit */
    uint8_t                             adc_clp_sample_rrbin; /**<  1 bit */
    uint8_t                             adc_clp_neg_en;       /**<  1 bit */
    uint8_t                             adc_clp_pos_en;       /**<  1 bit */

    uint8_t                             adc_clp_neg_thr;      /**<  7 bit */
    uint8_t                             adc_clp_pos_thr;      /**<  7 bit */

} clipdet_config_t;                                           /**< Clipping Detection Configuration Parameters */

/**
 * Capture Memory Configuration Structure
 */
typedef struct {
  uint8_t    hist_mode;  /**<  mode= 0, stop when sample count reached timer cap. */
                         /**<  mode= 1, stop when when any of the 256 histogram bins reached bin cap. */
                         /**<  mode= 2, stop when sample count reached cap or any of the 256 histogram bins reached bin cap. */
  uint8_t    hist_sel;   /**<  mode= 0, select traffic from FFE. */
                         /**<  mode= 1, select traffic from ADC PP. */
  uint32_t   bin_cap;    /**<  27 bit bin cap, must be less than   2^27 - 1 = 134217727 */
  uint32_t   timer_cap;  /**<  32 bit timer cap, must be less than 2^32 - 1 = 4294967295 */
} hist_cfg_t; /**< LW Capture Memory configuration type */

typedef struct hw_hist_cfg_s {
    uint8_t first_time; /**<  set it to 1 if the command was sent down first time */
    uint8_t source;     /**<  histogram sources, FFE=0, ADC=1 */
    uint8_t hist_mode;  /**<  mode= 0, stop when sample count reached timer cap. */
                        /**<  mode= 1, stop when when any of the 256 histogram bins reached bin cap. */
                        /**<  mode= 2, stop when sample count reached cap or any of the 256 histogram bins reached bin cap. */
    uint8_t bin_cap;    /**<  bin cap, [0-27] in bit unit, 0 will use default setting (27 bits) */
    uint8_t timer_cap;  /**<  timer cap, [0-32] in bit unit, 0 will use default setting (32 bits) */
} hw_hist_cfg_t;

typedef struct ffe_saturation_s {
     uint16_t   over_stat_l;        /*!<  LSB 16 TAP FFE saturation flag */
     uint16_t   under_stat_l;       /*!<  LSB 16 TAP FFE saturation flag */
     uint16_t   over_stat_h;        /*!<  MSB 8/14 TAP FFE saturation flag */
     uint16_t   under_stat_h;       /*!<  MSB 8/14 TAP FFE saturation flag */
} ffe_saturation_t; /**<  Line Wrapper RX FFE tap value */


/**
 * @brief     chal_lw_diag_rx_clipdet_enable (phy_info_t* phy_info_ptr)
 * @details   Enable Clipping Detection
 *
 * @param[in] phy_info_ptr: pointer to PHY info
 * @return enum return_result_t
 */

return_result_t chal_lw_diag_rx_clipdet_enable (phy_info_t* phy_info_ptr);

/**
 * @brief     chal_lw_diag_rx_clipdet_get_enable (phy_info_t* phy_info_ptr, uint8_t *enabled)
 * @details   Get Clipping Detection enabled status
 *
 * @param[in] phy_info_ptr: pointer to PHY info
 * @return enum return_result_t
 */

return_result_t chal_lw_diag_rx_clipdet_get_enable (phy_info_t* phy_info_ptr, uint8_t *enabled);


/**
 * @brief     chal_lw_diag_rx_clipdet_disable (phy_info_t* phy_info_ptr)
 * @details   Disable Clipping Detection
 *
 * @param[in] phy_info_ptr: pointer to PHY info
 * @return enum return_result_t
 */

return_result_t chal_lw_diag_rx_clipdet_disable (phy_info_t* phy_info_ptr);


/**
 * @brief     chal_lw_diag_rx_clipdet_config (phy_info_t* phy_info_ptr, clipdet_config_t* cfg_ptr)
 * @details   Configure Clipping Detection
 *
 * @param[in] phy_info_ptr: pointer to PHY info
 * @param[in] cfg_ptr:      pointer to Clipping Detection configuration info
 * @return enum return_result_t
 */

return_result_t chal_lw_diag_rx_clipdet_config (phy_info_t* phy_info_ptr, clipdet_config_t* cfg_ptr);


/**
 * @brief     chal_lw_diag_rx_chk_clipdet (phy_info_t* phy_info_ptr, uint8_t* adc_clp_timer_maxed_ptr, uint32_t* adc_clp_cnt_ptr)
 * @details   Check ClIPDET Status
 *
 * @param[in] phy_info_ptr:            pointer to PHY info
 * @param[in] adc_clp_timer_maxed_ptr: pointer to CLIPDET Status Register 1 readback
 * @param[in] adc_clp_cnt_ptr:         pointer to CLIPDET Status Register 0 readback
 * @return enum return_result_t
 */

return_result_t chal_lw_diag_rx_chk_clipdet (phy_info_t* phy_info_ptr, uint8_t* adc_clp_timer_maxed_ptr, uint32_t* adc_clp_cnt_ptr);


/**
 * @brief     chal_lw_diag_rx_clipdet_clear (phy_info_t* phy_info_ptr, uint8_t clear)
 * @details   Clear Clipping Detection
 *
 * @param[in] phy_info_ptr: pointer to PHY info
 * @param[in] clear:      1: clear
 * @return enum return_result_t
 */

return_result_t chal_lw_diag_rx_clipdet_clear (phy_info_t* phy_info_ptr, uint8_t clear);

/**
 * @brief  chal_lw_diag_rx_hist_en(phy_info_t* phy_info_ptr, uint8_t enable)
 * @detail enable = 1 - enable hist; enable = 0 - disable hist
 * @public 
 * @private 
 * 
 * @param[in] phy_info_ptr: device base address
 * @param[in] enable 1: enable hist  0: disable hist
 *
 * @return return_result_t: returns result code
*/
return_result_t chal_lw_diag_rx_hist_en(phy_info_t* phy_info_ptr, uint8_t enable);

/**
 * @brief  chal_lw_diag_rx_hist_config(phy_info_t* phy_info_ptr, hist_cfg_t* hist_cfg_ptr)
 * @param[in] phy_info_ptr: device base address
 * @param[in] hist_cfg_ptr->hist_mode;   mode= 0, stop when sample count reached timer cap. 
                                         mode= 1, stop when when any of the 256 histogram bins reached bin cap. 
                                         mode= 2, stop when sample count reached cap or any of the 256 histogram bins reached bin cap. 
 * @param[in] hist_cfg_ptr->hist_sel;    mode= 0, select traffic from FFE. 
                                         mode= 1, select traffic from ADC PP. 
 * @param[in] hist_cfg_ptr->bin_cap;     27 bit bin cap, must be less than   2^27 - 1 = 134217727 
 * @param[in] hist_cfg_ptr->timer_cap;   32 bit timer cap, must be less than 2^32 - 1 = 4294967295 
 * @public 
 * @private 
 * 
 * @return return_result_t: returns result code
*/
return_result_t chal_lw_diag_rx_hist_config(phy_info_t* phy_info_ptr, hist_cfg_t* hist_cfg_ptr);

/**
 * @brief  chal_lw_diag_rx_hist_clear(phy_info_t* phy_info_ptr, uint8_t clear)
 * @detail clear = 1 - clear all counters; clear = 0 -  do not clear counters
 * @public 
 * @private 
 * 
 * @param[in] phy_info_ptr: device base address
 * @param[in] clear 1: clear hist ;   0: disable clear 
 *
 * @return return_result_t: returns result code
*/
return_result_t chal_lw_diag_rx_hist_clear(phy_info_t* phy_info_ptr, uint8_t clear);

/**
 * @brief  chal_lw_diag_rx_hist_start(phy_info_t* phy_info_ptr, uint8_t start)
 * @detail start = 1 - start histogram capture counters; start = 0 - stop histogram capture counters
 * @detail config the hist by call function chal_lw_diag_rx_hist_config before start the counters 
 * @public 
 * @private 
 * 
 * @param[in] phy_info_ptr: device base address
 * @param[in] start 1: start hist ;   0:  stop hist 
 *
 * @return return_result_t: returns result code
*/
return_result_t chal_lw_diag_rx_hist_start(phy_info_t* phy_info_ptr, uint8_t start);

/**
 * @brief  chal_lw_diag_rx_rd_hist(phy_info_t* phy_info_ptr, uint8_t mode, uint32_t* data_ptr)
 * @detail Read all the counter data in histogram into data_ptr array
 * @public 
 * @private 
 * 
 * @param[in] phy_info_ptr: device base address
 * @param[in] mode: 0 for ffe, 1 for adc mode
 * @param[out] data_ptr: 256/128 32-bit word data array, maximum 27 bits used
 *
 * @return return_result_t: returns result code
*/
return_result_t chal_lw_diag_rx_rd_hist(phy_info_t* phy_info_ptr, uint8_t mode, uint32_t* data_ptr);

/**
 * @brief  chal_lw_diag_rx_hist_check_finish(phy_info_t* phy_info_ptr, uint8_t* data_ptr)
 * @detail check if the histogram capture finished  
 * @public 
 * @private 
 * 
 * @param[in] phy_info_ptr: device base address
 * @param[out]  data_ptr:  8-bit word counter_finished 
 *
 * @return return_result_t: returns result code
*/

return_result_t chal_lw_diag_rx_hist_check_finish(phy_info_t* phy_info_ptr, uint8_t* data_ptr);

/**
 * @brief  chal_lw_diag_rx_hist_rd_timer(phy_info_t* phy_info_ptr, uint32_t* data_ptr)
 * @detail read the value in timer register and return to data_ptr, stop the histogram or wait for the histogram stoped before reading the timer counter. 
 * @public 
 * @private 
 * 
 * @param[in] phy_info_ptr: device base address
 * @param[out]  data_ptr:  32-bit word timer 
 *
 * @return return_result_t: returns result code
*/

return_result_t chal_lw_diag_rx_hist_rd_timer(phy_info_t* phy_info_ptr, uint32_t* data_ptr);


/**
 * @brief     chal_lw_diag_rx_dfe_mse_read (phy_info_t* phy_info_ptr, dsp_fast_brcm_snr_cfg_t *snr_cfg_ptr,  uint32_t *mse_val, uint32_t* mseval_lvl)
 *
 * @details  Read DFE MSE Sigma^2 Value
 *
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  mse_opt_sel  :  MSE Output Select (0-8)
 *
 * @return   uint32_t : 32-bit MSE sigma^2 value (LS 4-bits always = 0)
 *                      Note: A return value of 0x00000bad => an invalid mse_opt_sel valid was passed to function..
 *
 */
return_result_t chal_lw_diag_rx_dfe_mse_read (phy_info_t* phy_info_ptr, dsp_fast_brcm_snr_cfg_t *snr_cfg_ptr,  uint32_t *mse_val, uint32_t* mseval_lvl);


/**
 * @brief  return_result_t chal_lw_diag_ffe_mse_get (phy_info_t* phy_info_ptr, dsp_fast_brcm_snr_cfg_t *snr_cfg_ptr, uint32_t *mse_val, uint32_t* mseval_lvl)
 *
 * @details Read FFE MSE Value
 *
 * @param[in] phy_info_ptr: pointer to Phy information
 * @param[in] mse_opt_sel   : MSE Output Select:
 *                       :   Single LMS Mode: 0-6 => MSE Level 0-6 SNR, 7 => Unused, 8 => Total SNR
 *                       :   Octal  LMS Mode: 0-7 => MSE 0-7 SNR, 8 => Total SNR
 * @param[out] mse_val:    32-bit FFE MSE Value
 * 
 * @return return_result_t
 */
return_result_t chal_lw_diag_ffe_mse_get (phy_info_t* phy_info_ptr, dsp_fast_brcm_snr_cfg_t *snr_cfg_ptr, uint32_t *mse_val, uint32_t* mseval_lvl);


/**
 * @brief  double chal_lw_diag_calculate_snr(ffe_slc_mode_t slc_mode, uint32_t mse_val)
 *
 * @details Calculate SNR from MSE Value  SNR = 10 log (signal_value) - 10 log (MSE/2^28)
 *
 * @param[in] slc_mode:    slicer mode
 * @param[in] mse_val:    32-bit MSE Value from FFE or DFE
 * 
 * @return SNR value in double
 */
double chal_lw_diag_calculate_snr(ffe_slc_mode_t slc_mode, uint32_t mse_val);

/**
 * @brief  return_result_t chal_lw_diag_ffe_write_clear_saturation_st (phy_info_t* phy_info_ptr)
 *
 * @details write clear FFE saturation status
 *
 * @param[in] phy_info_ptr: pointer to Phy information
  * @return return_result_t
 */
return_result_t chal_lw_diag_ffe_write_clear_saturation_st (phy_info_t* phy_info_ptr);

/**
 * @brief  chal_lw_diag_ffe_get_saturation_st (phy_info_t* phy_info_ptr, ffe_saturation_t *ffe_stat_ptr)
 *
 * @details read FFE saturation status
 *
 * @param[in] phy_info_ptr: pointer to Phy information
  * @return return_result_t
 */
return_result_t chal_lw_diag_ffe_get_saturation_st (phy_info_t* phy_info_ptr, ffe_saturation_t *ffe_stat_ptr);

/**
 * @brief return_result_t   chal_lw_diag_ffe_capture_9_ffe_taps(phy_info_t* phy_info_ptr, uint8_t acc_sel,uint8_t round_sel, int32_t* coef_ptr, float* float_coef_ptr)
 *
 * @details  read the specified FFE coefficients (signed 32 bits)
 * @details  sequence: set lms_rd_select and lms_snpshot select, toggle lms_strobe
 * @details   write specified address, read ffe coefficients (16bits MSB + 16bits LSB)
 *
 * @param[in]  phy_info_ptr: pointer to Phy information
 * @param[in]  acc_sel: 1- select rd lms coefficients accumulator value; 0- rd lms coefficients shadow values on ffe  (S5.10)
 * @param[in]  round_sel:  1-  read pre round lms coefficients at lms_rd_data;(S5.26)  0- read post round lms coefficients at lms_rd_data (S5.4) 
 * @param[out]  coeff_ptr : pointer to loaction of coeff read from EQ, 32 bits memory 
 * @param[out]  float_coeff_ptr : pointer to loaction of coeff read from EQ,  
 * @return enum return_result_t
 */

return_result_t chal_lw_diag_ffe_capture_9_ffe_taps(phy_info_t* phy_info_ptr, uint8_t acc_sel, uint8_t round_sel, int32_t* coef_ptr, float* float_coef_ptr);

/**
 * @brief  chal_lw_diag_rx_cdr_get_integ_ppm (phy_info_t* phy_info_ptr)
 *
 * Get Integ PPM
 *
 * @param[in]  phy_info_ptr: device info pointer
 * @return int32 integ PPM value
 */
int32_t chal_lw_diag_rx_cdr_get_integ_ppm(phy_info_t* phy_info_ptr );

/**
 * @detail  Latch and read hsip jitter filter signed integ value. <br>
            <b>PPM = integ / 2.62144</b><br>
            The read value is already converted into signed integer. No external conversion is required.
 * @param[in] *integ_12_0: <b>signed</b> integ value. 
 * @return return_result_t always returns RR_SUCCESS
*/
return_result_t chal_lw_diag_hsip_jf_rd_integ(phy_info_t* phy_info_ptr, int16_t* integ_12_0);

#ifdef __cplusplus
}
#endif

#endif

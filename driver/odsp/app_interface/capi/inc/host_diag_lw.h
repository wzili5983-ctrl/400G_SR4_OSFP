/**
 * @file     host_diag_lw.h
 * @author  
 * @date     11-22-2022
 * @version 1.0
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
#ifndef HOST_DIAG_LW_H
#define HOST_DIAG_LW_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  host_diag_get_lvl_snr(phy_info_t* phy_info_ptr, float *level_snr_ptr, float *snr_ptr)
 * Calculate level SNR and total snr value
 * @param[in]  phy_info_ptr  phy_info containing device base address and port
 * @param[out]  level_snr_ptr: store snr level value
 * @param[out]  snr_ptr: store total snr value
 * @return  0: RR_OK; 0xFF:RR_ERROR
 */
return_result_t host_diag_get_lvl_snr(phy_info_t* phy_info_ptr, uint8_t lane_index, float *level_snr_ptr, float *snr_ptr);

/**
 * @brief  host_diag_get_vga(phy_info_t* phy_info_ptr)
 * Return VGA value
 * 
 * @param[in]  phy_info_ptr  phy_info containing device base address and port
 * @return  VGA value
 */
uint8_t host_diag_get_vga(phy_info_t* phy_info_ptr);

/**
 * @brief  host_diag_get_phase_bias(phy_info_t* phy_info_ptr, int8_t *phase_bias_ptr, int8_t *static_phase_bias_ptr)
 * Get phase bias value and static tuning phase bias value
 * 
 * @param[in]  phy_info_ptr  phy_info containing device base address and port
 * @param[in]  cfg_phy_info_ptr  phy_info containing device base address and port
 * @param[out] phase_bias_ptr: 
 * @param[out] static_phase_bias_ptr: 
 * @return  
 */
return_result_t host_diag_get_phase_bias(phy_info_t* phy_info_ptr, phy_info_t* cfg_phy_info_ptr, int8_t *phase_bias_ptr, int8_t *static_phase_bias_ptr);

/**
 * @brief  host_diag_get_ffe_saturation(phy_info_t* phy_info_ptr, uint32_t *tap_under_saturation, uint32_t *tap_over_saturation)
 * Return FFE tap saturation
 * 
 * @param[in]  phy_info_ptr  phy_info containing device base address and port
 * @param[out] tap_under_saturation: 
 * @param[out] tap_over_saturation: 
 * @return  0: RR_OK; 0xFF:RR_ERROR
 */
return_result_t host_diag_get_ffe_saturation(phy_info_t* phy_info_ptr, uint32_t *tap_under_saturation, uint32_t *tap_over_saturation);

/**
 * @brief  host_diag_get_ffe_taps(phy_info_t* phy_info_ptr, float* ffe_taps_ptr)
 * Return FFE main tap and 8 side taps
 * 
 * @param[in]  phy_info_ptr  phy_info containing device base address and port
 * @param[out] ffe_ptr:  piointer to 24*float variants array which will be used to store the read-out FFE coefficients
 * @return  0: RR_OK; 0xFF:RR_ERROR
 */
return_result_t host_diag_get_ffe_taps(phy_info_t* phy_info_ptr, float* ffe_taps_ptr);

/**
 * @brief  cl_lw_get_adcclip(phy_info_t* phy_info_ptr, uint8_t lane_index, uint8_t th)
 * Read adcclip value
  *
 * @param[in]  phy_info_ptr  phy_info containing device base address and port
 * @param[in]  linkup_time: 16 bit ms times
 * @return  0: RR_OK; 0xFF:RR_ERROR
 */
uint32_t host_diag_get_adcclip(phy_info_t* phy_info_ptr, uint8_t lane_index, uint8_t th);

/**
 * @brief  host_diag_get_afe_bw(phy_info_t* phy_info_ptr)
 * 
 * @param[in]  phy_info_ptr  phy_info containing device base address and port
 * @return  AFE BW value
 */
uint8_t host_diag_get_afe_bw(phy_info_t* phy_info_ptr);

/**
 * @brief  host_diag_get_gain_boost(phy_info_t* phy_info_ptr)
 * 
 * @param[in]  phy_info_ptr  phy_info containing device base address and port
 * @return  gain boost
 */
uint8_t host_diag_get_gain_boost(phy_info_t* phy_info_ptr);

/**
 * @brief  host_diag_get_peaking(phy_info_t* phy_info_ptr)
 * 
 * @param[in]  phy_info_ptr  phy_info containing device base address and port
 * @return  peaking filter value
 */
uint8_t host_diag_get_peaking(phy_info_t* phy_info_ptr);

/**
 * @brief  host_diag_get_processor_status(phy_info_t* phy_info_ptr)
 * 
 * @param[in]  phy_info_ptr  phy_info containing device base address and port
 * @return processor status
 */
uint8_t host_diag_get_processor_status(phy_info_t* phy_info_ptr);

/**
 * @brief  host_diag_get_cdr_lock_counter(phy_info_t* phy_info_ptr, uint8_t lane_id)
 * 
 * @param[in]  phy_info_ptr  phy_info containing device base address and port
 * @param[in]  lane_id  
 * @return cdr lock counter
 */
uint8_t host_diag_get_cdr_lock_counter(phy_info_t* phy_info_ptr, uint8_t lane_id);

/**
 * @brief  host_diag_get_ppm(phy_info_t* phy_info_ptr, float *tx_ppm_ptr, float *rx_ppm_ptr)
 * 
 * @param[in]  phy_info_ptr  phy_info containing device base address and port
 * @param[out]  tx_ppm_ptr  
 * @param[out]  rx_ppm_ptr  
 * @return
 */
return_result_t host_diag_get_ppm(phy_info_t* phy_info_ptr, float *tx_ppm_ptr, float *rx_ppm_ptr);

/**
 * @brief  host_diag_get_port_status(phy_info_t* phy_info_ptr, uint16_t port_status_ptr)
 * 
 * @param[in]  phy_info_ptr  phy_info containing device base address and port
 * @param[out]  port_status_ptr  phy_info containing device base address and port
 * @return 
 */
return_result_t host_diag_get_port_status(phy_info_t* phy_info_ptr, uint16_t *port_status_ptr);

/**
 * @brief    lw_internal_get_hw_histogram(phy_info_t* phy_info_ptr, phy_info_t* cfg_phy_info_ptr, bool first_time, uint32_t *lw_slicer_histogram)
 * @details  This API is used to get hw histogram,  slicer mean and sigma values
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  cfg_phy_info_ptr: a pointer which carries detail information necessary to identify the config phy
 * @param[in]  first_time: first time to start HW histogram, 1 - second time
 * @param[out]  lw_slicer_histogram: 256 array output FFE slicer histogram
 * @param[out]  slicer_mean_value: 4 slicer mean values (4 levels)
 * @param[out]  slicer_sigma_value: 4 slicer sigma values (4 levels)
 * @return     returns the result of the called method/function, RR_SUCCESS
 */
return_result_t host_diag_get_hw_histogram(phy_info_t* phy_info_ptr, phy_info_t* cfg_phy_info_ptr, bool first_time, uint32_t *lw_slicer_histogram);

#ifdef __cplusplus
}
#endif

#endif /* HOST_DIAG_LW_H */


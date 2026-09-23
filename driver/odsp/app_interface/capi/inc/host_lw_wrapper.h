/**
 *
 * @file    host_lw_wrapper.h
 * @author  Firmware Team
 * @date    04/30/2019
 * @version 0.1
 *
 * @property 
 * $Copyright: (c) 2019 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * @brief    brief description of the block source file, sequences/flow/etc.
 *
 * @section  description
 *
 */

/** @file host_lw_wrapper.h
 *  cAPI prototype definition
 */

#ifndef HOST_LW_WRAPPER_H
#define HOST_LW_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct lw_level_snr_value_s {
    float snr_value[7]; /**< snr_value */
    uint8_t snr_levels; /**< snr_levels */
} lw_level_snr_value_t; /**< LW level SNR value type */

/**
 * @brief    host_lw_get_lane_modulation(capi_phy_info_t*   phy_info_ptr,
 *                                       uint8_t            lane_index,
 *                                       uint8_t* modulation_ptr)
 * @details  This API is used to get FFE slicer histogram
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  lane_index: Lane ID
 * @param[out] modulation_ptr: lane modulation
  * @return     returns the result of the called method/function, RR_SUCCESS
 */
return_result_t host_lw_get_lane_modulation(capi_phy_info_t*   phy_info_ptr,
                                            uint8_t            lane_index,
                                            capi_modulation_t* modulation_ptr);



/**
 * @brief host_lw_tx_fir_4tap_prog_256_lut (phy_info_t* phy_info_ptr, int8_t *lut_val)
 * @details     Program TXFIR 256 LUT with the given 256 entries
 * @public
 * @private
 *
 *
 * @param[in]  phy_info_ptr         device info pointer
 * @param[in]  *lut_val             256 entries of TXFIR LUT value, the value should be 7 bit signed [-64, 63]
 * @return enum return_result_t
 */
return_result_t host_lw_tx_fir_4tap_prog_256_lut (phy_info_t* phy_info_ptr, int8_t *lut_val);

/**
 * @brief host_lw_tx_fir_4tap_read_256_lut (phy_info_t* phy_info_ptr,  int8_t *lut_val)
 * @details     Read TXFIR 256 LUT values from chip
 * @public
 * @private
 *
 *
 * @param[in]  phy_info_ptr         device info pointer
 * @param[out]  *lut_val            256 entries of TXFIR LUT value read from chip
 * @return enum return_result_t
 */
return_result_t host_lw_tx_fir_4tap_read_256_lut (phy_info_t* phy_info_ptr,  int8_t *lut_val);
/**
 * @brief host_lw_tx_fir_set_floating_13tap (phy_info_t* phy_info_ptr,  capi_txfir_nonlinear_13tap_table_info_t *txfir_tbl_ptr)
 * @details     Read TXFIR 256 LUT values from chip
 * @public
 * @private
 *
 *
 * @param[in]  phy_info_ptr         device info pointer
 * @param[out]  *lut_val            256 entries of TXFIR LUT value read from chip
 * @return enum return_result_t
 */
return_result_t host_lw_tx_fir_set_floating_13tap (phy_info_t* phy_info_ptr,  capi_txfir_nonlinear_13tap_table_info_t *txfir_tbl_ptr);

/**
 * @brief host_lw_tx_fir_get_floating_13tap (phy_info_t* phy_info_ptr,  capi_txfir_nonlinear_13tap_table_info_t *txfir_tbl_ptr)
 * @details     Read TXFIR 256 LUT values from chip
 * @public
 * @private
 *
 *
 * @param[in]  phy_info_ptr         device info pointer
 * @param[out]  *lut_val            256 entries of TXFIR LUT value read from chip
 * @return enum return_result_t
 */
return_result_t host_lw_tx_fir_get_floating_13tap (phy_info_t* phy_info_ptr,  capi_txfir_nonlinear_13tap_table_info_t *txfir_tbl_ptr);
/**
 * @brief    host_lw_get_pll_configuration(capi_phy_info_t* phy_info_ptr, capi_pll_info_t *pll_info_ptr)
 * @details  This API is used to get PLL config information
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  pll_info_ptr: a pointer with pll configuration information
 *
 * @return     returns the result of the called methode/function, RR_SUCCESS
 */
return_result_t host_lw_get_pll_configuration(capi_phy_info_t* phy_info_ptr, capi_pll_info_t *pll_info_ptr);
/**
 * @brief    host_lw_set_snr_threshold(capi_phy_info_t* phy_info_ptr, capi_lw_snr_info_t *snr_info_ptr)
 * @details  This API is used to config snr information
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  snr_info_ptr: a pointer which carries LW snr config information
 *
 * @return     returns the result of the called methode/function, RR_SUCCESS
 */
return_result_t host_lw_set_snr_threshold(capi_phy_info_t* phy_info_ptr, capi_lw_snr_info_t *snr_info_ptr);
/**
 * @brief    host_lw_get_snr_threshold(capi_phy_info_t* phy_info_ptr, capi_lw_snr_info_t *snr_info_ptr)
 * @details  This API is used to retrieve snr configuration information
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  snr_info_ptr: a pointer which carries LW snr config information
 *
 * @return     returns the result of the called methode/function, RR_SUCCESS
 */
return_result_t host_lw_get_snr_threshold(capi_phy_info_t* phy_info_ptr, capi_lw_snr_info_t *snr_info_ptr);


/**
 * @brief    host_dsp_set_pll_fracn_dynamic_adjust(capi_phy_info_t* phy_info_ptr, capi_lw_dynamic_pll_fracn_cfg_info_t *dpll_info_ptr)
 * @details  This API is used to config dynamic PLL fracN information
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  dpll_info_ptr: a pointer which carries dynamic PLL config information
 *
 * @return     returns the result of the called methode/function, RR_SUCCESS
 */
return_result_t host_dsp_set_pll_fracn_dynamic_adjust(capi_phy_info_t* phy_info_ptr, capi_lw_dynamic_pll_fracn_cfg_info_t *dpll_info_ptr);

/**
 * @brief    host_dsp_get_pll_fracn_dynamic_adjust(capi_phy_info_t* phy_info_ptr, capi_lw_dynamic_pll_fracn_cfg_info_t *dpll_info_ptr)
 * @details  This API is used to get dynamic PLL fracN information
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  dpll_info_ptr: a pointer which carries dynamic PLL config information
 *
 * @return     returns the result of the called methode/function, RR_SUCCESS
 */
return_result_t host_dsp_get_pll_fracn_dynamic_adjust(capi_phy_info_t* phy_info_ptr, capi_lw_dynamic_pll_fracn_cfg_info_t *dpll_info_ptr);


/**
 * @brief    host_lw_get_gain3_a_val(capi_phy_info_t* phy_info_ptr, int channel_id, float *gain3_a_val_ptr)
 * @details  This API is used to get gain3 A value
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  channel_id: ADC channel ID
 * @param[in]  gain3_a_val_ptr: point to gain3 a value
 * @return     returns the result of the called method/function, RR_SUCCESS
 */
return_result_t host_lw_get_gain3_a_val(capi_phy_info_t* phy_info_ptr, int channel_id, float *gain3_a_val_ptr);
/**
* @brief    host_set_ingress_power_off(capi_phy_info_t* phy_info_ptr)
* @details  This API is a custome command to turn off the ingress path
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* 
* @return     returns the result of the called methode/function, RR_SUCCESS
*/
return_result_t host_set_ingress_power_off(capi_phy_info_t* phy_info_ptr);



#ifdef __cplusplus
}
#endif

#endif /* HOST_LW_WRAPPER_H */

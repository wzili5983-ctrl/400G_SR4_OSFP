/**
 *
 * @file      host_chip_wrapper.h
 * @author    Firmware Team
 * @date      02/01/2019
 * @version   1.0
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

/** @file host_chip_wrapper.h
 *  cAPI prototype definition
 */
#ifndef HOST_CHIP_WRAPPER_H
#define HOST_CHIP_WRAPPER_H


#ifdef __cplusplus
extern "C" {
#endif

/**
* @brief      util_wait_for_uc_ready(capi_phy_info_t* phy_info_ptr)
* @details    This utility function to pool the FW ready state
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* 
* @return     returns the performance result of the called methode/function
*/
return_result_t util_wait_for_uc_ready(capi_phy_info_t* phy_info_ptr);


/**
 * @brief    host_get_chip_info(capi_phy_info_t* phy_info_ptr, capi_chip_info_t* chip_info_ptr)
 * @details  This API is used to get the Chip information
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[out] chip_info_ptr: this pointer contains chip info defined by capi_chip_info_t, which has 
 *                            chip_id and chip_revision
 * 
 * @return     returns the result of the called methode/function, RR_SUCCESS
 */
return_result_t host_get_chip_info(capi_phy_info_t* phy_info_ptr, capi_chip_info_t* chip_info_ptr);


/**
 * @brief    host_get_lpm_st(capi_phy_info_t* phy_info_ptr, capi_lpm_info_t* lpm_ptr)
 * @details  This API is used to get firmware LPM status: IN LPM(1) or NOT IN LPM(0)
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  lpm_ptr: a pointer for lpm information
 * 
 * @return     returns the result of the called methode/function
 */
return_result_t host_get_lpm_st(capi_phy_info_t* phy_info_ptr, capi_lpm_info_t* lpm_ptr);
#ifdef __cplusplus
}
#endif


/**
 * @brief      host_get_gpr_lane_status(capi_phy_info_t* phy_info_ptr, capi_gpr_lane_status_t* status_ptr)
 * @details    Get lane status from GP registers
 *
 * @param      phy_info_ptr : phy information
 * @param      status_ptr : output gpr lane status
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_get_gpr_lane_status(capi_phy_info_t* phy_info_ptr, capi_gpr_lane_status_t* status_ptr);



/**
 * @brief    host_set_spi_info(capi_phy_info_t* phy_info_ptr, capi_spi_info_t* lpm_ptr)
 * @details  This API is used to get firmware spi information
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  capi_spi_info_ptr: a pointer for spi information
 * 
 * @return     returns the result of the called methode/function
 */
return_result_t host_set_spi_info(capi_phy_info_t* phy_info_ptr, capi_spi_info_t* capi_spi_info_ptr);



/**
 * @brief    host_get_spi_info(capi_phy_info_t* phy_info_ptr, capi_spi_info_t* lpm_ptr)
 * @details  This API is used to get firmware spi information
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  capi_spi_info_ptr: a pointer for spi information
 * 
 * @return     returns the result of the called methode/function
 */
return_result_t host_get_spi_info(capi_phy_info_t* phy_info_ptr, capi_spi_info_t* capi_spi_info_ptr);



/**
 * @brief    host_set_capi_feature_info(capi_phy_info_t* phy_info_ptr, capi_feature_info_t* feature_info_ptr)
 * @details  This API is used to configure the CAPI features
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  feature_info_ptr: a reference to the CAPI feature information object
 * 
 * @return     returns the result of the called methode/function
 */
return_result_t host_set_capi_feature_info(capi_phy_info_t* phy_info_ptr, capi_feature_info_t* feature_info_ptr);



/**
 * @brief    host_get_capi_feature_info(capi_phy_info_t* phy_info_ptr, capi_feature_info_t* feature_info_ptr)
 * @details  This API is used to get the configuration the CAPI features
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  feature_info_ptr: a reference to the CAPI feature information object
 * 
 * @return     returns the result of the called methode/function
 */
return_result_t host_get_capi_feature_info(capi_phy_info_t* phy_info_ptr, capi_feature_info_t* feature_info_ptr);

#endif /**< HOST_CHIP_WRAPPER_H */

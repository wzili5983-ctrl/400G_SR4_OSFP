/**
 *
 * @file  host_fec_prbs.h
 * @author  
 * @date     09/01/2018
 *
 *
 * @file     host_fec_prbs.h
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
#ifndef HOST_FEC_PRBS_H
#define HOST_FEC_PRBS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
* @brief     host_fec_get_prbs_status(capi_phy_info_t* phy_info_ptr, capi_prbs_status_t* prbs_st_ptr)
* @details    This API is used to get PRBS status information 
*
* @param[in]  phy_info_ptr:  phy info pointer
* @param[out]  prbs_st_ptr:  PRBS status structure pointer
* 
* @return     returns the performance result of the called methode/function
*/          
return_result_t host_fec_get_prbs_status(capi_phy_info_t* phy_info_ptr, capi_prbs_status_t* prbs_st_ptr);

/**
* @brief     host_fec_clear_prbs_status(capi_phy_info_t* phy_info_ptr, capi_pattern_gen_mon_type_t ptype)
* @details    This API is used to clear PRBS hardware status 
*
* @param[in]  phy_info_ptr:  phy info pointer
* @param[in]  ptype:  PRBS type: CAPI_PRBS_KP4_LINE or CAPI_PRBS_KP4_CLIENT
* 
* @return     returns the performance result of the called methode/function
*/          
return_result_t host_fec_clear_prbs_status(capi_phy_info_t* phy_info_ptr, capi_pattern_gen_mon_type_t ptype);

/**
* @brief      host_fec_set_prbs_gen(capi_phy_info_t* phy_info_ptr, capi_prbs_info_t* prbs_info_ptr)
* @details    This API is used to configure the KP4 PRBS pattern generator and enable or disable it
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  prbs_info_ptr: this pointer contain PRBS config info
* 
* @return     returns the performance result of the called methode/function
*/
return_result_t host_fec_set_prbs_gen(capi_phy_info_t* phy_info_ptr, capi_prbs_info_t* prbs_info_ptr);

/**
* @brief      host_fec_set_prbs_mon(capi_phy_info_t* phy_info_ptr, capi_prbs_info_t* prbs_info_ptr)
* @details    This API is used to configure the KP4 PRBS pattern monitor and enable or disable it
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  prbs_info_ptr: this pointer contain PRBS config info
* 
* @return     returns the performance result of the called methode/function
*/
return_result_t host_fec_set_prbs_mon(capi_phy_info_t* phy_info_ptr, capi_prbs_info_t* prbs_info_ptr);

/**
* @brief      host_fec_get_prbs_gen(capi_phy_info_t* phy_info_ptr, capi_prbs_info_t* prbs_info_ptr)
* @details    This API is to get the configuration setting of FEC PRBS pattern generator
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[out] prbs_info_ptr: this pointer return PRBS config information
* 
* @return     returns the performance result of the called methode/function
*/
return_result_t host_fec_get_prbs_gen(capi_phy_info_t* phy_info_ptr, capi_prbs_info_t* prbs_info_ptr);

/**
* @brief      host_fec_get_prbs_mon(capi_phy_info_t* phy_info_ptr, capi_prbs_info_t* prbs_info_ptr)
* @details    This API is to get the configuration setting of FEC PRBS pattern monitor
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[out] prbs_info_ptr: this pointer return PRBS config information
* 
* @return     returns the performance result of the called methode/function
*/
return_result_t host_fec_get_prbs_mon(capi_phy_info_t* phy_info_ptr, capi_prbs_info_t* prbs_info_ptr);

/**
* @brief      host_fec_ignore_fault (capi_phy_info_t* phy_info_ptr, uint8_t lane_mask, capi_direction_t direction, capi_enable_t ignore_fhdlr)
* @details    This API is used to set ignore fault handling in OCW fw per port per ingress/egress
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  lane_mask: line-side lane-mask for selected PORT
* @param[in]  direction: data-path direction
* @param[in]  ignore_fhdlr: enable(1) or disable(0) for ignoring fault handling
* 
* @return     returns the performance result of the called methode/function
*/
return_result_t host_fec_ignore_fault(capi_phy_info_t* phy_info_ptr,
                                                    uint8_t lane_mask, 
                                                    capi_direction_t direction, 
                                                    capi_enable_t ignore_fhdlr, 
                                                    uint8_t pidx);

/**
* @brief     host_fec_reset_port (capi_phy_info_t* phy_info_ptr, uint16_t port_mask)
* @details    This API is used to notify firmware FSM to result per port per ingress/egress
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  port_mask
* 
* @return     returns the performance result of the called methode/function
*/
return_result_t host_fec_reset_port(capi_phy_info_t* phy_info_ptr, uint16_t port_mask);

#ifdef __cplusplus
}
#endif

#endif /* HOST_FEC_PRBS_H */


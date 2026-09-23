/**
 *
 * @file     chal_cw_rtmr_kp4prbs.h 
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

#ifndef CHAL_CW_RTMR_KP4PRBS_H
#define CHAL_CW_RTMR_KP4PRBS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  chal_cw_rtmr_fec_prbs_pgen_config_top(phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, onoff_t enable, cw_mode_parameter_t* cur_mode_parameter_ptr) 
 * @detail FEC PRBSGen sequence 
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in] onoff_t : Enable/disable FEC PRBS 
 * @param[in]  cur_mode_parameter_ptr : pointer to mode parameter structure 
 * @return    void:
*/
void chal_cw_rtmr_fec_prbs_pgen_config_top(phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, onoff_t enable, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr) ;

/**
 * @brief  chal_cw_rtmr_fec_prbs_pmon_config_top(phy_info_t* phy_info_ptr, onoff_t enable, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr) 
 * @detail Clear all PMON err-ors and sticky flags
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in] enable : Enable FEC PRBS
 * @param[in] cur_mode_parameter_ptr : pointer to structure  
 * @param[in]  egr_or_igr
 * @return    void:
*/
void chal_cw_rtmr_fec_prbs_pmon_config_top(phy_info_t* phy_info_ptr, onoff_t enable, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief  chal_cw_rtmr_fec_prbs_pgen_config_wrapper( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, pam4_pgen_misc_cfg_t* pam4_pgen_misc_cfg_ptr,
 *                                                    cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
 * @detail Wrapper than decides the speed to decide which PGEN lanes should be active 
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in]  cur_mode_parameter_ptr : pointer to mode parameter structure 
 * @return    void:
*/
void chal_cw_rtmr_fec_prbs_pgen_config_wrapper( phy_info_t* phy_info_ptr, 
                                                cfg_egr_or_igr_t egr_or_igr, 
                                                pam4_pgen_misc_cfg_t* pam4_pgen_misc_cfg_ptr,
                                                cw_port_config_t* cur_port_config_ptr, 
                                                cw_mode_parameter_t* cur_mode_parameter_ptr);

/**
 * @brief  chal_cw_rtmr_fec_prbs_pgen_enable_disable_wrapper( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr,cw_port_config_t* cur_port_config_ptr,  cw_mode_parameter_t* cur_mode_parameter_ptr,onoff_t enable)
 * @detail Fuction that enables or disables the wrapper 
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in]  cur_mode_parameter_ptr : pointer to mode parameter structure 
 * @param[in]  enable : To enable or disable 
 * @return    void:
*/
uint8_t chal_cw_rtmr_fec_prbs_pgen_enable_disable_wrapper( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,onoff_t enable);

/**
 * @brief  chal_cw_rtmr_fec_prbs_pmon_config_wrapper( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, 
            pam4_pmon_misc_cfg_t *pmon_misc_struct_inst_ptr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
 * @detail Wrapper than decides the speed to decide which PMON lanes should be active 
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in]  pmon_misc_struct_inst_ptr : pmon config 
 * @param[in]  cur_port_config_ptr : port config
 * @param[in]  cur_mode_parameter_ptr : pointer to mode parameter structure 
 * @return    void:
*/

void chal_cw_rtmr_fec_prbs_pmon_config_wrapper( phy_info_t* phy_info_ptr, 
                                            cfg_egr_or_igr_t egr_or_igr,
                                            pam4_pmon_misc_cfg_t *pmon_misc_struct_inst_ptr,
                                            cw_port_config_t* cur_port_config_ptr,
                                            cw_mode_parameter_t* cur_mode_parameter_ptr);


/**
 * @brief  chal_cw_rtmr_fec_prbs_pmon_enable_disable_wrapper( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,onoff_t enable)
 * @detail Fuction that enables or disables the PMON instances corresponding to the speed  
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in]  cur_mode_parameter_ptr : pointer to mode parameter structure 
 * @param[in]  enable : To enable or disable 
 * @return    void:
*/

void chal_cw_rtmr_fec_prbs_pmon_enable_disable_wrapper( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,onoff_t enable);



/**
 * @brief  chal_pam4_pgen_lane_prbs_clk_cfg ( (phy_info_t* phy_info_ptr, unint8_t lane_mask) 
 * @detail Choose lanewise clock for lane prbs pgen
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in] lane_mask:Lane mask 
 * @return void
 */

void chal_pam4_pgen_lane_prbs_clk_cfg (phy_info_t* phy_info_ptr, uint8_t lane_mask) ;


/**
 * @brief  chal_pam4_pmon_lane_prbs_clk_cfg ( (phy_info_t* phy_info_ptr, unint8_t lane_mask) 
 * @detail Choose lanewise clock for lane prbs pmon
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in] lane_mask:Lane mask 
 * @return void
 */

void chal_pam4_pmon_lane_prbs_clk_cfg ( phy_info_t* phy_info_ptr, uint8_t lane_mask) ;




/**
 * @brief  chal_pam4_pgen_fec_prbs_clk_cfg( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
 * @detail Function that makes the PGENs get bundled clock 
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in]  cur_mode_parameter_ptr : pointer to mode parameter structure 

 * @return    void:
*/




void chal_pam4_pgen_fec_prbs_clk_cfg (phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr);


/**
 * @brief  chal_pam4_pmon_fec_prbs_clk_cfg( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr,  cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
 * @detail Function that makes the pmons get bundled clock 
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in]  cur_mode_parameter_ptr : pointer to mode parameter structure 

 * @return    void:
*/

void chal_pam4_pmon_fec_prbs_clk_cfg( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr,  cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr);



/**
 * @brief  chal_pam4_pmon_fec_prbs_fec_ip_data_mux_cfg (phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
 * @detail Function that configures the FEC slice inputs to be PGEN data. 
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in]  cur_mode_parameter_ptr : pointer to mode parameter structure 

 * @return    void:
*/


void chal_pam4_pmon_fec_prbs_fec_ip_data_mux_cfg (phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr);


/**
 * @brief  chal_cw_rtmr_fec_pmon_lock_status( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
 * @detail Function that gets the PMON lock status corresponding to the speed  
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in]  cur_port_config_ptr: port config pointer
 * @param[in]  cur_mode_parameter_ptr : pointer to mode parameter structure 
 * @return    uint8_t:
*/


uint8_t chal_cw_rtmr_fec_pmon_lock_status( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr);


/**
 * @brief  chal_cw_rtmr_pmon_get_status( phy_info_t* phy_info_ptr, 
                                    cfg_egr_or_igr_t egr_or_igr, 
                                    cw_port_config_t* cur_port_config_ptr, 
                                    cw_mode_parameter_t* cur_mode_parameter_ptr, 
                                    pam4_pmon_stat_t* pam4_pmon_stat_ptr)
 * @detail Function that gets the PMON  status corresponding to the lane  
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in]  cur_port_config_ptr: port config pointer
 * @param[in]  cur_mode_parameter_ptr : pointer to mode parameter structure 
 * @param[out]  pam4_pmon_stat_ptr: pmon stat pointer. 
 * @return    void:
*/

void chal_cw_rtmr_pmon_get_status( phy_info_t* phy_info_ptr, 
                                    cfg_egr_or_igr_t egr_or_igr, 
                                    cw_port_config_t* cur_port_config_ptr, 
                                    cw_mode_parameter_t* cur_mode_parameter_ptr, 
                                    pam4_pmon_stat_t* pam4_pmon_stat_ptr);


/**
 * @brief  chal_cw_rtmr_fec_pmon_clr_all( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
 * @detail Function that clears the PMON Accumulators, overflow flags, LOL flag corresponding to the speed  
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in]  cur_port_config_ptr: port config pointer
 * @param[in]  cur_mode_parameter_ptr : pointer to mode parameter structure 
 * @return    void:
*/


void chal_cw_rtmr_fec_pmon_clr_all( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr);


/**
 * @brief  chal_cw_rtmr_fec_pgen_get_active_status( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
 * @detail Function that gets the PGEN active status corresponding to the speed  
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in]  cur_port_config_ptr: port config pointer
 * @param[in]  cur_mode_parameter_ptr : pointer to mode parameter structure 
 * @return    uint8_t:
*/

uint8_t chal_cw_rtmr_fec_pgen_get_active_status( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr);



/**
 * @brief  chal_cw_rtmr_fec_pmon_get_active_status( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
 * @detail Function that gets the PGEN active status corresponding to the speed  
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in]  cur_mode_parameter_ptr : pointer to mode parameter structure 
 * @param[in]  cur_port_config_ptr: port config pointer
 * @return    uint8_t:
*/


uint8_t chal_cw_rtmr_fec_pmon_get_active_status( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr);


/**
 * @brief  chal_cw_rtmr_pgen_get_poly( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr,  cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
 * @detail Function that gets the PGEN poly sel for the specified lane
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in]  lane_num : lane number of PGEN instance for which we want to get the poly 
 * @return    uint8_t:
*/


uint8_t chal_cw_rtmr_pgen_get_poly( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr,  cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr);


/**
 * @brief  chal_cw_rtmr_pgen_get_inv( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr,  cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
 * @detail Function that gets the PGEN DOUT INV 
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in]  lane_num : lane number of PGEN instance for which we want to get the poly 
 * @return    uint8_t:
*/

uint8_t chal_cw_rtmr_pgen_get_inv( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr);

/**
 * @brief  chal_cw_rtmr_pmon_get_poly( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_ig, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
 * @detail Function that gets the pmon poly sel for the specified lane
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in]  lane_num : lane number of pmon instance for which we want to get the poly 
 * @return    uint8_t:
*/


uint8_t chal_cw_rtmr_pmon_get_poly( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_ig, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr);


/**
 * @brief  chal_cw_rtmr_pmon_get_inv( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr,  cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
 * @detail Function that gets the PMON DIN INV 
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in]  lane_num : lane number of PGEN instance for which we want to get the poly 
 * @return    uint8_t:
*/

uint8_t chal_cw_rtmr_pmon_get_inv( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr);



/**
 * @brief  chal_cw_rtmr_fec_pgen_get_enable( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
 * @detail Function that gets the PGEN enable value corresponding to the speed  
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in]  cur_port_config_ptr: port config pointer
 * @param[in]  cur_mode_parameter_ptr : pointer to mode parameter structure 
 * @return    uint8_t:
*/


uint8_t chal_cw_rtmr_fec_pgen_get_enable( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr);



/**
 * @brief  chal_cw_rtmr_fec_pmon_get_enable( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
 * @detail Function that gets the PMON enable value corresponding to the speed  
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in]  cur_mode_parameter_ptr : pointer to mode parameter structure 
 * @return    uint8_t:
*/
uint8_t chal_cw_rtmr_fec_pmon_get_enable( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr);

#ifdef __cplusplus
}
#endif

#endif /* CHAL_CW_RTMR_KP4PRBS_H */


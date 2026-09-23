
/**
 *
 * @file ml_cw_rtmr_handler.h
 * @author  
 * @date     09/01/2018
 * @version 1.0
 *
 * $Copyright: Copyright 2018 Broadcom INC.
 * This program is the proprietary software of Broadcom INC
 * and/or its licensors, and may only be used, duplicated, modified
 * or distributed pursuant to the terms and conditions of a separate,
 * written license agreement executed between you and Broadcom
 * (an "Authorized License").  Except as set forth in an Authorized
 * License, Broadcom grants no license (express or implied), right
 * to use, or waiver of any kind with respect to the Software, and
 * Broadcom expressly reserves all rights in and to the Software
 * and all intellectual property rights therein.  IF YOU HAVE
 * NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE
 * IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE
 * ALL USE OF THE SOFTWARE.
 * 
 * Except as expressly set forth in the Authorized License,
 * 
 * 1.      This program, including its structure, sequence and organization,
 * constitutes the valuable trade secrets of Broadcom, and you shall use
 * all reasonable efforts to protect the confidentiality thereof,
 * and to use this information only in connection with your use of
 * Broadcom integrated circuit products.
 * 
 * 2.      TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS
 * PROVIDED "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
 * REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
 * OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
 * DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
 * NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
 * ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
 * OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 * 
 * 3.      TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL
 * BROADCOM OR ITS LICENSORS BE LIABLE FOR  CONSEQUENTIAL,
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


#ifndef ML_CW_RTMR_HANDLER_H
#define ML_CW_RTMR_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief          ml_cw_rtmr_gen_port_cfg(cw_port_profile_t* port_ptr, cw_port_config_t *port_cfg_ptr)
 * @details    Generate the port config information
 * @public        any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]     port_ptr  port pointer
 * @return void
 */

void ml_cw_rtmr_gen_port_cfg(cw_port_profile_t* port_ptr, cw_port_config_t *port_cfg_ptr);


/**
 * @brief          ml_cw_rtmr_gen_cw_mode_cfg(cw_port_profile_t* port_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t *port_cfg_ptr, cw_mode_parameter_t *cw_mode_ptr)
 * @details    Generate the port config information
 * @public        any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]     port_ptr
 * @param[in]     egr_or_igr
 * @param[out]     port_cfg_ptr
 * @param[out]     cw_mode_ptr
 * @return void
 */

void ml_cw_rtmr_gen_cw_mode_cfg(cw_port_profile_t* port_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t *port_cfg_ptr, cw_mode_parameter_t *cw_mode_ptr);


/******************************************************************************************
 *
 * Ingress/Egress retimer port  config and control functions
 *
 */


/**
 * @brief       ml_cw_rtmr_egr_cfg_datapath(cw_port_profile_t* port_ptr)
 * @details    cfg Core Wrapper Egress data path 
 * @public    any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]  port_ptr  port pointer
 * @return void
 */

void ml_cw_rtmr_egr_cfg_datapath(cw_port_profile_t* port_ptr);   
     
 /**
  * @brief          ml_cw_rtmr_egr_cfg_datapath_init(cw_port_profile_t* port_ptr)
  * @details    cfg Core Wrapper retimer egress: Datapath Mux, mode and data gating configuration
  * @public      any further information for the public domain
  * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
  *
  *
  * @param[in]    port_ptr  port pointer
  * @return void
  */
 
 void ml_cw_rtmr_egr_cfg_datapath_init(cw_port_profile_t* port_ptr);

 
     
 /**
  * @brief          ml_cw_rtmr_egr_cfg_clock_reset_mux(cw_port_profile_t* port_ptr)
  * @details    Clock and reset mux configuration
  * @public      any further information for the public domain
  * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
  *
  *
  * @param[in]    port_ptr  port pointer
  * @return void
  */
 
 void ml_cw_rtmr_egr_cfg_clock_reset_mux(cw_port_profile_t* port_ptr);
 
/**
 * @brief         ml_cw_rtmr_egr_cfg_lane_clk(cw_port_profile_t* port_ptr, enabled_t enbl)
 * @details    1Enable Host RX PCS per lane clocks when HOST is PCS. Otherwise, enable FEC lane clock, but not PCS clock (JIRA42)
 * @public    any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]    port_ptr  port pointer
 * @param[in]    enbl  clock enable: IS_DISABLED or IS_ENABLED
 * @return void
 */
 
void ml_cw_rtmr_egr_cfg_lane_clk(cw_port_profile_t* port_ptr, enabled_t enbl);

 

/**
 * @brief       ml_cw_rtmr_egr_cfg_lane_reset(phy_info_t* phy_info_ptr, rstb_ctrl_t rstl)
 * @details    config  Core Wrapper Retimer egress per lane reset
 * @public    any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]  port_ptr  port pointer
 * @param[in]  rst   reset asserted: RST_DEASSERTED or RST_ASSERTED
 * @return void
 */ 
 void ml_cw_rtmr_egr_cfg_lane_reset(cw_port_profile_t* port_ptr, rst_assert_t rst);

 
 /**
  * @brief         ml_cw_rtmr_egr_cfg_rcv_func_blk_clk_and_rst(phy_info_t* phy_info_ptr, rstb_ctrl_t rstl)
  * @details    config    Core Wrapper Retimer egress receiver side functional block level clock and reset control t
  * @public    any further information for the public domain
  * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
  *
  *
  * @param[in]    port_ptr  port pointer
  * @return void
  */ 
  void ml_cw_rtmr_egr_cfg_rcv_func_blk_clk_and_rst(cw_port_profile_t* port_ptr, rst_assert_t rst);



 

/**
 * @brief       ml_cw_rtmr_egr_cfg_trnsmt_func_blk_clk_and_rst(phy_info_t* phy_info_ptr, rstb_ctrl_t rstl)
 * @details    config  Core Wrapper Retimer egress transmiter side functional block level clock and reset control t
 * @public    any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]  port_ptr  port pointer
 * @return void
 */ 
 void ml_cw_rtmr_egr_cfg_trnsmt_func_blk_clk_and_rst(cw_port_profile_t* port_ptr, rst_assert_t rst);
  
 
 /**
  * @brief         ml_cw_rtmr_egr_clr_flt_status(phy_info_t* phy_info_ptr)
  * @details    Clear fault  status  and in case of false fault  during start-up
  * @public    any further information for the public domain
  * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
  *
  *
  * @param[in]    port_ptr  port pointer
  * @return void
  */ 
  void ml_cw_rtmr_egr_clr_flt_status(cw_port_profile_t* port_ptr);
  
  
  
/**
 * @brief       ml_cw_rtmr_egr_get_pcsfec_lock(cw_port_profile_t* port_ptr)
 * @details     in retimer egress, check PCSFEC lock
 * @public    any further information for the public domain
 * @private     any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]  port_ptr  port pointer
 * @return void
 */
 
 uint16_t ml_cw_rtmr_egr_get_pcsfec_lock(cw_port_profile_t* port_ptr);
 
 
 /**
  * @brief         ml_cw_rtmr_egr_clr_pcsfec_igbox_clsn(cw_port_profile_t* port_ptr)
  * @details     in retimer egress, clear pcsfec_igbox collision
  * @public    any further information for the public domain
  * @private     any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
  *
  *
  * @param[in]    port_ptr  port pointer
  * @return void
  */
  
 void ml_cw_rtmr_egr_clr_pcsfec_igbox_clsn(cw_port_profile_t* port_ptr);


/**
 * @brief       ml_cw_rtmr_egr_get_pcsfec_igbox_clsn(cw_port_profile_t* port_ptr, uint8_t am_unlock_chk)
 * @details     in retimer egress, check pcsfec_igbox collision
 * @public    any further information for the public domain
 * @private     any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]  port_ptr  port pointer
 * @return void
 */
 
uint16_t ml_cw_rtmr_egr_get_pcsfec_igbox_clsn(cw_port_profile_t* port_ptr, uint8_t am_unlock_chk);



/**
 * @brief       ml_cw_rtmr_egr_clr_pcsfec_ogbox_clsn(cw_port_profile_t* port_ptr)
 * @details     in retimer egress, clear pcsfec_ogbox collision
 * @public    any further information for the public domain
 * @private     any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]  port_ptr  port pointer
 * @return void
 */
 
void ml_cw_rtmr_egr_clr_pcsfec_ogbox_clsn(cw_port_profile_t* port_ptr);

/**
 * @brief       ml_cw_rtmr_egr_get_pcsfec_ogbox_clsn(cw_port_profile_t* port_ptr)
 * @details     in retimer egress, check pcsfec_ogbox collision
 * @public    any further information for the public domain
 * @private     any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]  port_ptr  port pointer
 * @return void
 */
 
 uint16_t ml_cw_rtmr_egr_get_pcsfec_ogbox_clsn(cw_port_profile_t* port_ptr);


/**
 * @brief       ml_cw_rtmr_egr_get_pcsfec_clk66_ogbox_clsn(cw_port_profile_t* port_ptr)
 * @details     in retimer egress, check pcsfec_clk66_ogbox collision
 * @public    any further information for the public domain
 * @private     any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]  port_ptr  port pointer
 * @return void
 */
 
 uint16_t ml_cw_rtmr_egr_get_pcsfec_clk66_ogbox_clsn(cw_port_profile_t* port_ptr);

/**
* @brief         ml_cw_rtmr_egr_enable(cw_port_profile_t* port_ptr)
* @details   Release retimer mode egress reset
* @public    any further information for the public domain
* @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
*
*
* @param[in]    port_ptr  port pointer
* @return void
*/

void ml_cw_rtmr_egr_rcvr_enable(cw_port_profile_t* port_ptr);

/**
* @brief         ml_cw_rtmr_egr_trnsmt_enable(cw_port_profile_t* port_ptr)
* @details   Release retimer mode egress reset
* @public    any further information for the public domain
* @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
*
*
* @param[in]    port_ptr  port pointer
* @return void
*/

void ml_cw_rtmr_egr_trnsmt_enable(cw_port_profile_t* port_ptr);

/**
* @brief         ml_cw_rtmr_egr_enable(cw_port_profile_t* port_ptr)
* @details   Release retimer mode egress reset
* @public    any further information for the public domain
* @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
*
*
* @param[in]    port_ptr  port pointer
* @return void
*/

void ml_cw_rtmr_egr_enable(cw_port_profile_t* port_ptr);

 
/**
* @brief         ml_cw_rtmr_egr_reset(cw_port_profile_t* port_ptr)
* @details   Put retimer mode egress reset
* @public    any further information for the public domain
* @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
*
*
* @param[in]    port_ptr  port pointer
* @return void
*/

void ml_cw_rtmr_egr_reset(cw_port_profile_t* port_ptr);
 
 
 
/**
* @brief         ml_cw_rtmr_egr_rcvr_reset(cw_port_profile_t* port_ptr)
* @details    Put retimer mode egress reset
* @public    any further information for the public domain
* @private     any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
*
*
* @param[in]    port_ptr  port pointer
* @return void
*/

void ml_cw_rtmr_egr_rcvr_reset(cw_port_profile_t* port_ptr);
 
/**
 * @brief           ml_cw_rtmr_egr_trnsmt_reset(cw_port_profile_t* port_ptr)
 * @details   Put retimer mode egress    transmit reset
 * @public    any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]      port_ptr    port pointer
 * @return void
 */

void ml_cw_rtmr_egr_trnsmt_reset(cw_port_profile_t* port_ptr);

 

/**
 * @brief       ml_cw_rtmr_igr_cfg_datapath(cw_port_profile_t* port_ptr)
 * @details    cfg Core Wrapper Ingress data path 
 * @public    any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]  port_ptr  port pointer
 * @return void
 */

void ml_cw_rtmr_igr_cfg_datapath(cw_port_profile_t* port_ptr);  
     
/**
* @brief          ml_cw_rtmr_igr_cfg_datapath_init(cw_port_profile_t* port_ptr)
* @details    cfg Core Wrapper Repeater ingress :Datapath Mux, mode and data gating configuration
* @public      any further information for the public domain
* @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
*
*
* @param[in]    port_ptr  port pointer
* @return void
*/

void ml_cw_rtmr_igr_cfg_datapath_init(cw_port_profile_t* port_ptr);


     
/**
 * @brief          ml_cw_rtmr_igr_cfg_clock_reset_mux(cw_port_profile_t* port_ptr)
 * @details    Retimer ingress: Clock and reset mux configuration
 * @public      any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]    port_ptr  port pointer
 * @return void
 */

void ml_cw_rtmr_igr_cfg_clock_reset_mux(cw_port_profile_t* port_ptr);


/**
 * @brief         ml_cw_rtmr_egr_cfg_lane_clk(cw_port_profile_t* port_ptr, enabled_t enbl)
 * @details    Retimer Ingress lane clock config
 * @public    any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]    port_ptr  port pointer
 * @param[in]    enbl  clock enable: IS_DISABLED or IS_ENABLED
 * @return void
 */
 
void ml_cw_rtmr_igr_cfg_lane_clk(cw_port_profile_t* port_ptr, enabled_t enbl);
 

 

/**
 * @brief       ml_cw_rtmr_igr_cfg_lane_reset(phy_info_t* phy_info_ptr, rstb_ctrl_t rstl)
 * @details    config  Core Wrapper Retimer ingress per lane reset
 * @public    any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]  port_ptr  port pointer
 * @param[in]  rst   reset asserted: RST_DEASSERTED or RST_ASSERTED
 * @return void
 */ 
 void ml_cw_rtmr_igr_cfg_lane_reset(cw_port_profile_t* port_ptr, rst_assert_t rst);


/**
 * @brief       ml_cw_rtmr_igr_cfg_rcvr_func_blk_clk_and_rst(phy_info_t* phy_info_ptr, rstb_ctrl_t rstl)
 * @details    config  Core Wrapper Retimer ingress receiver functional block level clock and reset control t
 * @public    any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]  port_ptr  port pointer
 * @return void
 */ 
 void ml_cw_rtmr_igr_cfg_rcvr_func_blk_clk_and_rst(cw_port_profile_t* port_ptr, rst_assert_t rst);
  
 
/**
 * @brief       ml_cw_rtmr_igr_cfg_trnsmt_func_blk_clk_and_rst(phy_info_t* phy_info_ptr, rstb_ctrl_t rstl)
 * @details    config  Core Wrapper Retimer ingress transmit functional block level clock and reset control t
 * @public    any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]  port_ptr  port pointer
 * @return void
 */ 
 void ml_cw_rtmr_igr_cfg_trnsmt_func_blk_clk_and_rst(cw_port_profile_t* port_ptr, rst_assert_t rst);

 /**
  * @brief         ml_cw_rtmr_igr_clr_flt_status(phy_info_t* phy_info_ptr)
  * @details    Clear fault  status  and in case of false fault  during start-up
  * @public    any further information for the public domain
  * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
  *
  *
  * @param[in]    port_ptr  port pointer
  * @return void
  */ 
  void ml_cw_rtmr_igr_clr_flt_status(cw_port_profile_t* port_ptr);
  
  
 
 
 
  
/**
 * @brief       ml_cw_rtmr_igr_get_pcsfec_lock(cw_port_profile_t* port_ptr)
 * @details     in retimer ingress, check PCSFEC lock
 * @public    any further information for the public domain
 * @private     any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]  port_ptr  port pointer
 * @return void
 */
 
uint16_t ml_cw_rtmr_igr_get_pcsfec_lock(cw_port_profile_t* port_ptr);



/**
* @brief         ml_cw_rtmr_igr_clr_pcsfec_igbox_clsn(cw_port_profile_t* port_ptr)
* @details     in retimer ingress, clear pcsfec_igbox collision
* @public    any further information for the public domain
* @private     any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
*
*
* @param[in]    port_ptr  port pointer
* @return void
*/
 
void ml_cw_rtmr_igr_clr_pcsfec_igbox_clsn(cw_port_profile_t* port_ptr);;

/**
 * @brief       ml_cw_rtmr_igr_get_pcsfec_igbox_clsn(cw_port_profile_t* port_ptr, uint8_t am_unlock_chk)
 * @details     in retimer ingress, check pcsfec_igbox collision
 * @public    any further information for the public domain
 * @private     any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]  port_ptr  port pointer
 * @return void
 */
 
 uint16_t ml_cw_rtmr_igr_get_pcsfec_igbox_clsn(cw_port_profile_t* port_ptr, uint8_t am_unlock_chk);


/**
 * @brief       ml_cw_rtmr_igr_clr_pcsfec_ogbox_clsn(cw_port_profile_t* port_ptr)
 * @details     in retimer ingress, clear pcsfec_ogbox collision
 * @public    any further information for the public domain
 * @private     any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]  port_ptr  port pointer
 * @return void
 */
 
void ml_cw_rtmr_igr_clr_pcsfec_ogbox_clsn(cw_port_profile_t* port_ptr);

/**
 * @brief       ml_cw_rtmr_igr_get_pcsfec_ogbox_clsn(cw_port_profile_t* port_ptr)
 * @details     in retimer ingress, check pcsfec_ogbox collision
 * @public    any further information for the public domain
 * @private     any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]  port_ptr  port pointer
 * @return void
 */
 
 uint16_t ml_cw_rtmr_igr_get_pcsfec_ogbox_clsn(cw_port_profile_t* port_ptr);


 
/**
 * @brief       ml_cw_rtmr_igr_get_pcsfec_clk66_ogbox_clsn(cw_port_profile_t* port_ptr)
 * @details     in retimer ingress, check pcsfec_clk66_ogbox collision
 * @public    any further information for the public domain
 * @private     any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]  port_ptr  port pointer
 * @return void
 */
 
 uint16_t ml_cw_rtmr_igr_get_pcsfec_clk66_ogbox_clsn(cw_port_profile_t* port_ptr);
  
/**
 * @brief           ml_cw_rtmr_igr_rcvr_enable(cw_port_profile_t* port_ptr)
 * @details   Release retimer mode ingress receiving part reset
 * @public    any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]      port_ptr    port pointer
 * @return void
 */

void ml_cw_rtmr_igr_rcvr_enable(cw_port_profile_t* port_ptr); 
 
 
  
 /**
 * @brief          ml_cw_rtmr_igr_trnsmt_enable(cw_port_profile_t* port_ptr)
 * @details   Release retimer mode ingress    transmit reset
 * @public      any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]     port_ptr  port pointer
 * @return void
 */
 
 void ml_cw_rtmr_igr_trnsmt_enable(cw_port_profile_t* port_ptr);

/**
* @brief         ml_cw_rtmr_igr_enable(cw_port_profile_t* port_ptr)
* @details   Release retimer mode ingress reset
* @public    any further information for the public domain
* @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
*
*
* @param[in]    port_ptr  port pointer
* @return void
*/

void ml_cw_rtmr_igr_enable(cw_port_profile_t* port_ptr);

  
/**
 * @brief         ml_cw_rtmr_igr_rcvr_reset(cw_port_profile_t* port_ptr)
 * @details    Put retimer mode ingress receiving reset
 * @public    any further information for the public domain
 * @private     any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]    port_ptr  port pointer
 * @return void
 */

void ml_cw_rtmr_igr_rcvr_reset(cw_port_profile_t* port_ptr);

  
/**
 * @brief         ml_cw_rtmr_igr_trnsmt_reset(cw_port_profile_t* port_ptr)
 * @details   Put retimer mode ingress transmiter reset
 * @public    any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]    port_ptr  port pointer
 * @return void
 */

void ml_cw_rtmr_igr_trnsmt_reset(cw_port_profile_t* port_ptr);

/**
* @brief         ml_cw_rtmr_igr_reset(cw_port_profile_t* port_ptr)
* @details   Put retimer mode ingress reset
* @public    any further information for the public domain
* @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
*
*
* @param[in]    port_ptr  port pointer
* @return void
*/

void ml_cw_rtmr_igr_reset(cw_port_profile_t* port_ptr);


/**
 * @brief     ml_cw_rtmr_clear_fec_fault_info(cw_port_profile_t* port_ptr, cfg_egr_or_igr_t egr_or_igr )
 * @details   CW clear retimer FEC fault information
 * @public    any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]    port_ptr  port pointer
 * @return void
 */
	
void ml_cw_rtmr_clear_fec_fault_info(cw_port_profile_t* port_ptr, cfg_egr_or_igr_t egr_or_igr );


  
/**
 * @brief     ml_cw_rtmr_get_fec_fault_info(cw_port_profile_t* port_ptr, cfg_egr_or_igr_t egr_or_igr, cw_fec_rx_status_t* fec_st_ptr )
 * @details   CW get retimer FEC fault information
 * @public    any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]    port_ptr  port pointer
 * @return void
 */
void ml_cw_rtmr_get_fec_fault_info(cw_port_profile_t* port_ptr, cfg_egr_or_igr_t egr_or_igr, cw_fec_rx_status_t* fec_st_ptr );



  
/**
 * @brief    ml_cw_rtmr_disable_patg(cw_port_profile_t* port_ptr, cfg_egr_or_igr_t egr_or_igr )
 * @details   CW disable pattern gen
 * @public    any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]    port_ptr  port pointer
 * @return void
 */         

void ml_cw_rtmr_disable_patg(cw_port_profile_t* port_ptr, cfg_egr_or_igr_t egr_or_igr );


 /**
  * @brief          ml_cw_rtmr_cfg_datapath_init_handler_core(cw_port_profile_t* port_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,
  *                                                      cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr,
  *                                                      cw_chip_mode_t chip_mode)
  * @details    cfg Core Wrapper retimer: Datapath Mux, mode and data gating configuration
  * @public      any further information for the public domain
  * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
  *
  *
  * @param[in]    port_ptr  port pointer
  * @return void
  */
 
void ml_cw_rtmr_cfg_datapath_init_handler_core(phy_info_t* phy_info_ptr,
                                            cw_mode_parameter_t* cur_mode_parameter_ptr,
                                            cw_port_config_t* cur_port_config_ptr,
                                            cfg_egr_or_igr_t egr_or_igr,
                                            cw_chip_mode_t chip_mode);
 /**
  * @brief          ml_cw_rtmr_cfg_datapath_init_handler(cw_port_profile_t* port_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,
  *                                                      cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr,
  *                                                      cw_chip_mode_t chip_mode)
  * @details    cfg Core Wrapper retimer: Datapath Mux, mode and data gating configuration
  * @public      any further information for the public domain
  * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
  *
  *
  * @param[in]    port_ptr  port pointer
  * @return void
  */
 
void ml_cw_rtmr_cfg_datapath_init_handler(phy_info_t* phy_info_ptr,
                                            cw_mode_parameter_t* cur_mode_parameter_ptr,
                                            cw_port_config_t* cur_port_config_ptr,
                                            cfg_egr_or_igr_t egr_or_igr,
                                            cw_chip_mode_t chip_mode);
 /**
  * @brief          ml_cw_rtmr_cfg_clock_reset_mux_handler(cw_port_profile_t* port_ptr,
  *                                          cw_mode_parameter_t* cur_mode_parameter_ptr,
  *                                          cw_port_config_t* cur_port_config_ptr,
  *                                          cfg_egr_or_igr_t egr_or_igr)
  * @details    Clock and reset mux configuration
  * @public      any further information for the public domain
  * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
  *
  *
  * @param[in]    port_ptr  port pointer
  * @return void
  */
 
 void ml_cw_rtmr_cfg_clock_reset_mux_handler(phy_info_t* phy_info_ptr,
                                            cw_mode_parameter_t* cur_mode_parameter_ptr,
                                            cw_port_config_t* cur_port_config_ptr,
                                            cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief        ml_cw_rtmr_cfg_lane_clk_handler(cw_port_profile_t* port_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,
 *                                                cw_port_config_t* cur_port_config_ptr, enabled_t enbl, cfg_egr_or_igr_t egr_or_igr)
 * @details     Control per lane clock
 * @public    any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]    port_ptr  port pointer
 * @param[in]    enbl  clock enable: IS_DISABLED or IS_ENABLED
 * @return void
 */
 
void ml_cw_rtmr_cfg_lane_clk_handler(phy_info_t *phy_info_ptr, 
                                        cw_mode_parameter_t* cur_mode_parameter_ptr,
                                        cw_port_config_t* cur_port_config_ptr,
                                        enabled_t enbl,
                                        cfg_egr_or_igr_t egr_or_igr);


/**
 * @brief      ml_cw_rtmr_cfg_lane_reset_handler(cw_port_profile_t* port_ptr,
 *                                                 cw_mode_parameter_t* cur_mode_parameter_ptr,
 *                                                 rst_assert_t rst, 
 *                                                 cfg_egr_or_igr_t egr_or_igr)
 * @details    config  Core Wrapper Retimer egress per lane reset
 * @public    any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]  port_ptr  port pointer
 * @param[in]  rst   reset asserted: RST_DEASSERTED or RST_ASSERTED
 * @return void
 */ 
 void ml_cw_rtmr_cfg_lane_reset_handler(phy_info_t* phy_info_ptr,
                                        cw_mode_parameter_t* cur_mode_parameter_ptr,
                                        cw_port_config_t* cur_port_config_ptr,
                                        rst_assert_t rst, 
                                        cfg_egr_or_igr_t egr_or_igr);
/**
 * @brief     ml_cw_rtmr_cfg_rcvr_func_blk_clk_and_rst_handler(cw_port_profile_t* port_ptr,
 *                                                             cw_mode_parameter_t* cur_mode_parameter_ptr,
 *                                                             cw_port_config_t* cur_port_config_ptr,
 *                                                             rst_assert_t rst,
 *                                                             cfg_egr_or_igr_t egr_or_igr)
 * @details    config  Core Wrapper Retimer receiver side functional block level clock and reset control handler
 * @public    any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]  port_ptr  port pointer
 * @return void
 */ 
 void ml_cw_rtmr_cfg_rcvr_func_blk_clk_and_rst_handler(phy_info_t* phy_info_ptr,
                                                        cw_mode_parameter_t* cur_mode_parameter_ptr,
                                                        cw_port_config_t* cur_port_config_ptr,
                                                        rst_assert_t rst,
                                                        cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief       ml_cw_rtmr_gen_cw_mode_internal(cw_mode_t cur_quad_core_mode, 
 *                                                  cw_port_config_t * cur_port_config_ptr,
 *                                                  cw_mode_parameter_t * cur_mode_parameter_ptr)
 * @details     This function config repeater mode fec_monitor based on the user settings
 *
 * @param[in]   cur_quad_core_mode:   core mode
 * @param[in]   cur_port_config_ptr:    port config pointer
 * @param[in]   cur_mode_parameter_ptr:    mode config pointer
 *
 * @return      returns the performance result of the called method/function
 */
return_result_t ml_cw_rtmr_gen_cw_mode_internal(cw_mode_t cur_quad_core_mode,
                                                    cw_port_config_t * cur_port_config_ptr,
                                                    cw_mode_parameter_t * cur_mode_parameter_ptr);

/**
 * @brief       ml_enable_dsp_tx_clock(cw_port_profile_t* port_ptr,
 *                                    cfg_host_or_line_t host_or_line, uint8_t enable)
 * @details     This function enable/disable LW Tx div64 clock for XENC
 *
 * @param[in]   port_ptr:    port config pointer
 * @param[in]   host_or_line:    host or line side
 * @param[in]   enable:    enable/disable clock
 *
 * @return 
 */
void ml_enable_dsp_tx_clock(cw_port_profile_t* port_ptr, cfg_host_or_line_t host_or_line, uint8_t enable);

/**
 * @brief       ml_enable_dsp_rx_clock(cw_port_profile_t* port_ptr,
 *                                    cfg_host_or_line_t host_or_line, uint8_t enable)
 * @details     This function enable/disable LW Rx div32 clock for XDENC
 *
 * @param[in]   port_ptr:    port config pointer
 * @param[in]   host_or_line:    host or line side
 * @param[in]   enable:    enable/disable clock
 *
 * @return   
 */
void ml_enable_dsp_rx_clock(cw_port_profile_t* port_ptr, cfg_host_or_line_t host_or_line, uint8_t enable);

/**
* @brief		ml_cw_tx_data_select(phy_info_t* phy_info_ptr, cfg_dp_type_t dp_type, cfg_egr_or_igr_t egr_or_igr)
* @details	TX data is muxed between retimer and repeater databus. Choose corresponding data based on repeater or retimer 
* @public 	any further information for the public domain
* @private	any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
*
* @param[in]	phy_info_ptr device base address pointer 
* @param[in]	cfg_dp_type_t :  repeater or retimer
* @param[in]	cfg_egr_or_igr_t :	EGR or IGR
* @return enum return_result_t
*/ 
void ml_cw_tx_data_select(cw_port_profile_t* port_ptr, cfg_dp_type_t dp_type, cfg_egr_or_igr_t egr_or_igr);

/**
* @brief          ml_cw_rtmr_cfg_cl161_datapath_init(cw_port_profile_t* port_ptr,  cfg_egr_or_igr_t egr_or_igr)
* @details    cfg Core Wrapper retimer: Datapath Mux, mode and data gating configuration for CL161
* @public      any further information for the public domain
* @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
*
*
* @param[in]    port_ptr  port pointer
* @return void
*/ 
void ml_cw_rtmr_cfg_cl161_datapath_init(cw_port_profile_t* port_ptr,  cfg_egr_or_igr_t egr_or_igr);

/**
* @brief          ml_cw_rtmr_cfg_cl161_datapath_deinit(cw_port_profile_t* port_ptr,  cfg_egr_or_igr_t egr_or_igr)
* @details    Clear Core Wrapper retimer CL161 configurations
* @public      any further information for the public domain
* @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
*
*
* @param[in]    port_ptr  port pointer
* @return void
*/ 
void ml_cw_rtmr_cfg_cl161_datapath_deinit(cw_port_profile_t* port_ptr,  cfg_egr_or_igr_t egr_or_igr);

void ml_cw_rtmr_cfg_trnsmt_func_blk_clk_and_rst_handler(phy_info_t *cur_phy_info,
                                                            cw_mode_parameter_t* cur_mode_parameter_ptr,
                                                            cw_port_config_t* port_config_ptr,
                                                            rst_assert_t rst,
                                                            cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief      ml_cw_rtmr_fec_dec_reset_control(cw_port_profile_t* port_ptr, rst_assert_t rst, cfg_egr_or_igr_t egr_or_igr)
 * @details    Assert/Deassert FEC decoder reset
 * @public    any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]  port_ptr   port pointer
 * @param[in]  rst        reset asserted: RST_DEASSERTED or RST_ASSERTED
 * @param[in]  egr_or_igr EGR or IGR
 * @return void
 */ 
void ml_cw_rtmr_fec_dec_reset_control(cw_port_profile_t* port_ptr, rst_assert_t rst, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief      ml_cw_rtmr_fec_dec_clock_control(cw_port_profile_t* port_ptr, rst_assert_t rst, cfg_egr_or_igr_t egr_or_igr)
 * @details    turn on/off FEC decoder clock
 * @public    any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]  port_ptr   port pointer
 * @param[in]  rst        reset asserted: RST_DEASSERTED or RST_ASSERTED
 * @param[in]  egr_or_igr EGR or IGR
 * @return void
 */ 
void ml_cw_rtmr_fec_dec_clock_control(cw_port_profile_t* port_ptr, rst_assert_t rst, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief      ml_cw_rtmr_fec_dec_err_cnt_reset_control(cw_port_profile_t* port_ptr, uint8_t enable, cfg_egr_or_igr_t egr_or_igr)
 * @details this chal can be used to keep FEC counters when FEC decoder is reset.
            enable = 1: FEC counters are NOT cleared when FEC decoder is reset.
            enable = 0: FEC counters are cleared when FEC decoder is reset.
 * @public    any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]  port_ptr   port pointer
 * @param[in]  enable     1 : FEC counters are NOT cleared when FEC decoder is reset. 0: FEC counters are cleared when FEC decoder is reset.
 * @param[in]  egr_or_igr EGR or IGR
 * @return void
 */ 
void ml_cw_rtmr_fec_dec_err_cnt_reset_control(cw_port_profile_t* port_ptr, uint8_t enable, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief       ml_cw_rtmr_get_pcsfec_lock(cw_port_profile_t* port_ptr, cfg_egr_or_igr_t egr_or_igr)
 * @details     in retimer egress, check PCSFEC lock
 * @public    any further information for the public domain
 * @private     any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]  port_ptr  port pointer
 * @return void
 */
 
 uint16_t ml_cw_rtmr_get_pcsfec_lock(cw_port_profile_t* port_ptr, cfg_egr_or_igr_t egr_or_igr);

 uint16_t ml_cw_rtmr_get_fec_lock_reg_value(cw_port_profile_t* port_ptr, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief       ml_cw_rtmr_clr_fec_sync_align_unlock_status(cw_port_profile_t* port_ptr, cfg_egr_or_igr_t egr_or_igr)
 * @details     clear fec unlock sticky status
 * @public    any further information for the public domain
 * @private     any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]  port_ptr  port pointer
 * @return void
 */
 
void ml_cw_rtmr_clr_fec_sync_align_unlock_status(cw_port_profile_t* port_ptr, cfg_egr_or_igr_t egr_or_igr);

/**
 * @brief       ml_cw_rtmr_get_fec_sync_align_unlock_status(cw_port_profile_t* port_ptr, cfg_egr_or_igr_t egr_or_igr)
 * @details     in retimer, check fec unlock sticky status
 * @public    any further information for the public domain
 * @private     any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]  port_ptr  port pointer
 * @return void
 */
 
uint16_t ml_cw_rtmr_get_fec_sync_align_unlock_status(cw_port_profile_t* port_ptr, cfg_egr_or_igr_t egr_or_igr);

void ml_cw_rtmr_fec_dec_asynch_clr(cw_port_profile_t* port_ptr, cfg_egr_or_igr_t egr_or_igr);

#ifdef __cplusplus
}
#endif

#endif /**< ML_CW_RTMR_HANDLER_H*/

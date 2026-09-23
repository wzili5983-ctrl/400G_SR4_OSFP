
/**
 *
 * @file chal_cw_prbs.h 
 * @author  
 * @date     2020
 * @version 1.0
 *
 *  * @property   $Copyright: Copyright 2018 Broadcom INC. 
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
 *Except as expressly set forth in the Authorized License,
 *
 *1.     This program, including its structure, sequence and organization,
 *constitutes the valuable trade secrets of Broadcom, and you shall use
 *all reasonable efforts to protect the confidentiality thereof,
 *and to use this information only in connection with your use of
 *Broadcom integrated circuit products.
 *
 *2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS
 *PROVIDED "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
 *REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
 *OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
 *DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
 *NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
 *ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 *CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
 *OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 *3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL
 *BROADCOM OR ITS LICENSORS BE LIABLE FOR  CONSEQUENTIAL,
 *INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER
 *ARISING OUT OF OR IN ANY WAY RELATING TO YOUR USE OF OR INABILITY
 *TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF THE
 *POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF
 *THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR USD 1.00,
 *WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING
 *ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.$
 * @brief   this file includes cHAL function implementation.
 *
 * @section
 * 
 */
 

#ifndef CHAL_CW_PRBS_H
#define CHAL_CW_PRBS_H

#ifdef __cplusplus
extern "C" {
#endif




/**
 * @brief  chal_cw_pgen_read_lock (phy_info_t* phy_info_ptr, uint8_t* lock_flag)
 * @detail Read PGEN lock flag
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr : Device physical information pointer
 * @return    lock_flag     : 0 => Not Locked, 1=> Locked
 */
return_result_t chal_cw_pgen_read_lock (phy_info_t* phy_info_ptr, uint8_t* lock_flag);


/**
 * @brief  uint8_t chal_cw_pgen_enable (phy_info_t* phy_info_ptr, onoff_t onoff)
 * @detail Enable/Disable the PGEN block.
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  onoff        :  ON => Turn on PGEN and enable it, OFF => Turn off PGEN
 * @return     lock_flag    :  0 => PGEN not locked, 1 => PGEN Locked
 */
uint8_t chal_cw_pgen_enable (phy_info_t* phy_info_ptr, onoff_t onoff);



/**
 * @brief  void chal_cw_pgen_init (phy_info_t* phy_info_ptr)
 * @detail Initialize the PGEN block (places the PGEN registers into their power-up reset states). Basic config for raw prbs mode
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @return    void:
 */
void chal_cw_pgen_init (phy_info_t* phy_info_ptr);


/**
 * @brief  void chal_cw_pgen_set_config (phy_info_t* phy_info_ptr, pam4_pgen_cfg_t* pam4_pgen_cfg_ptr)
 * @detail Set PGEN Configuration (copy PGEN config structure into PGEN RDB registers)
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr  :  Device physical information pointer
 * @param[in]  pam4_pgen_cfg :  Pointer to PAM4 PGEN Config Type
 * @return     void:
 */
void chal_cw_pgen_set_config (phy_info_t* phy_info_ptr, pam4_pgen_cfg_t* pam4_pgen_cfg_ptr);




/**
 * @brief  void  chal_cw_pgen_get_config (phy_info_t* phy_info_ptr, pam4_pgen_cfg_t* pam4_pgen_cfg_ptr)
 * @detail Get PGEN Configuration (copy PGEN RDB values into PGEN config structure)
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr      :  Device physical information pointer
 * @param[in]  pam4_pgen_cfg_ptr :  Pointer to PAM4 PGEN Config Type
 * @return     void:
*/
return_result_t chal_cw_pgen_get_config (phy_info_t* phy_info_ptr, pam4_pgen_cfg_t* pam4_pgen_cfg_ptr);


/**
 * @brief  chal_cw_pgen_config (phy_info_t* phy_info_ptr, pam4_pgen_misc_cfg_t* pam4_pgen_misc_cfg_ptr)
 * @detail Configure PRBS Generator, FEC mode, data converter, prbs on fault etc. .
 *    Note: you may want to call chal_cw_pgen_config_pbi() after calling this.
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer

 * @param[in] pam4_pgen_misc_cfg_ptr: structure containing config values. 

 */
void chal_cw_pgen_config (phy_info_t* phy_info_ptr, pam4_pgen_misc_cfg_t* pam4_pgen_misc_cfg_ptr);






/**
 * @brief  chal_cw_pgen_prbs_on_fault_config (phy_info_t* phy_info_ptr,prbs_on_fault_cfg_t* prbs_on_fault_cfg_ptr)
 * @detail PRBS on fault config 

 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in] prbs_on_fault_cfg_ptr: fault mode enablepointer to structure contatining prbs_on_fault config 
 
 * @return return_result (Success => PGEN is locked, Error => PGEN is not locked)
 */

void chal_cw_pgen_prbs_on_fault_config (phy_info_t* phy_info_ptr,prbs_on_fault_cfg_t* prbs_on_fault_cfg_ptr);



/**
 * @brief  void chal_cw_pgen_config_pbi (phy_info_t* phy_info_ptr, onoff_t pbi_enable)
 * @detail Configure PRBS Generator Programmable Bit Invert Feature
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in] pbi_enable: ON => Enable PBI, OFF => Disable PBI
 * @return return_result (Success => PGEN is locked, Error => PGEN is not locked)
 */
void chal_cw_pgen_config_pbi (phy_info_t* phy_info_ptr, onoff_t pbi_enable);





/**
 * @brief  chal_cw_pgen_gen_err (phy_info_t* phy_info_ptr, prbs_gen_err_cfg_t* pgen_gen_err_cfg_ptr ) 
 * @detail Enable PGEN to generate bit err-or(s)
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  pgen_gen_err_cfg_ptr :pointer to pgen error generation structre 
 * @return    void:
*/
void chal_cw_pgen_gen_err (phy_info_t* phy_info_ptr, prbs_gen_err_cfg_t* pgen_gen_err_cfg_ptr );



/**
 * @brief  chal_cw_pgen_cfg_cw_mux (phy_info_t* phy_info_ptr, pam4_pgen_misc_cfg_t* pam4_pgen_misc_cfg_ptr, onoff_t onoff)
 * @detail Configure CW TMT_LANE_PRBS_MODE 
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in] pam4_pmon_misc_cfg_ptr: pointer to pmon misc config structure 
 * @return void
 */
return_result_t chal_cw_pgen_cfg_cw_mux (phy_info_t* phy_info_ptr, pam4_pgen_misc_cfg_t* pam4_pgen_misc_cfg_ptr, onoff_t onoff);


/**
 * @brief  chal_cw_pmon_cfg_cw_mux (phy_info_t* phy_info_ptr, pam4_pmon_misc_cfg_t* pam4_pmon_misc_cfg_ptr, onoff_t onoff)
 * @detail Configure CW RCV_LANE_PRBS_MODE 
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in] pam4_pmon_misc_cfg_ptr: pointer to pmon misc config structure 
 * @return void
 */
return_result_t chal_cw_pmon_cfg_cw_mux (phy_info_t* phy_info_ptr, pam4_pmon_misc_cfg_t* pam4_pmon_misc_cfg_ptr, onoff_t onoff);
/**
 * @brief  return_result_t chal_cw_pmon_read_lock (phy_info_t* phy_info_ptr, uint8_t* lock_flag)
 * @detail 
 *    Read pmon Lock Flag. Note that the pmon lock flag will never
 *    set if link is down or the BER is too high so do not wait forever.
 *    Just read the lock flag and return its value to the calling funtion.
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr : Device physical information pointer
 * @return     lock_flag    : 0 => Not Locked, 1=> Locked
 */
return_result_t chal_cw_pmon_read_lock (phy_info_t* phy_info_ptr, uint8_t* lock_flag);





/**
 * @brief  return_result_t chal_cw_pmon_enable (phy_info_t* phy_info_ptr, onoff_t onoff)
 * @detail Enable/Disable the PMON block.
 * @publics
 * @private
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  onoff        :  ON => Turn on PMON and enable it, OFF => Turn off PMON
 * @return     void
 */

return_result_t chal_cw_pmon_enable (phy_info_t* phy_info_ptr, onoff_t onoff);



/**
 * @brief  return_result_t chal_cw_pmon_get_enable (phy_info_t* phy_info_ptr, uint8_t* enable_ptr)
 * @detail Get PMON block Enable
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @return     enable       :  0 => PMON not enabled, 1 => PMON enabled
 */
return_result_t chal_cw_pmon_get_enable (phy_info_t* phy_info_ptr, uint8_t* enable_ptr);



/**
 * @brief  void chal_cw_pmon_restart (phy_info_t* phy_info_ptr)
 *
 * Restart PMON and start checking PRBS.
 *
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @return void
 */
void chal_cw_pmon_restart (phy_info_t* phy_info_ptr);





/**
 * @brief  void chal_cw_pmon_init (phy_info_t* phy_info_ptr)
 * @detail Initialize the PMON block (places the PMON registers into their power-up reset states)
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @return    void:
 */
void chal_cw_pmon_init (phy_info_t* phy_info_ptr);





/**
 * @brief  void  chal_cw_pmon_set_config (phy_info_t* phy_info_ptr, pam4_pmon_cfg_t* pam4_pmon_cfg_ptr, boolean load_cfg)
 * @detail Set PMON Configuration (copy PMON configuration structure to PMON RDB and datapath CDC registers)
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr      :  Device physical information pointer
 * @param[in]  pam4_pmon_cfg_ptr :  Pointer to PAM4 PMON Config Type
 * @param[in]  load_cfg          :  Pointer to PAM4 PMON Config Type
 * @return     void:
 */
void chal_cw_pmon_set_config (phy_info_t* phy_info_ptr, pam4_pmon_cfg_t* pam4_pmon_cfg_ptr, boolean load_cfg);



/**
 * @brief  void  chal_cw_pmon_get_config (phy_info_t* phy_info_ptr, pam4_pmon_cfg_t* pam4_pmon_cfg_ptr)
 * @detail Get PMON Configuration (copy PMON RDB register values to PMON configuration structure)
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr      :  Device physical information pointer
 * @param[in]  pam4_pmon_cfg_ptr :  Pointer to PAM4 PMON Config Type
 * @return     void
*/
return_result_t chal_cw_pmon_get_config (phy_info_t* phy_info_ptr, pam4_pmon_cfg_t* pam4_pmon_cfg_ptr);





/**
 * @brief  chal_cw_pmon_config (phy_info_t* phy_info_ptr, pam4_pmon_misc_cfg_t* pam4_pmon_misc_cfg_ptr)
 * @detail Configure PMON, data converter, FEC mode etc, 
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in] pam4_pmon_misc_cfg_ptr: pointer to pmon misc config structure 
 * @return void
 */
return_result_t chal_cw_pmon_config (phy_info_t* phy_info_ptr, pam4_pmon_misc_cfg_t* pam4_pmon_misc_cfg_ptr);




/**
 * @brief  chal_cw_pmon_config_acc (phy_info_t* phy_info_ptr, pam4_pmon_config_acc_cfg_t* pam4_pmon_config_acc_cfg_ptr)
 * @detail Configure PMON BER Accumulator
 *    Note: You should call chal_cw_pmon_config() before calling this.
 *    Note: This needs to be done while the PMON is not enabled (prior to it being enabled).
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in] pam4_pmon_config_acc_cfg_ptr  : structure pointer 
 
 * @return     void
 */
void chal_cw_pmon_config_acc (phy_info_t* phy_info_ptr, pam4_pmon_config_acc_cfg_t* pam4_pmon_config_acc_cfg_ptr);








/**
 * @brief  void chal_cw_pmon_config_lock (phy_info_t* phy_info_ptr, pam4_pmon_config_lock_cfg_t* pam4_pmon_config_lock_cfg_ptr)
 * @detail Configure PMON lock/unlock/relock
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in] pam4_pmon_config_lock_cfg_ptr: pointer to structure

 * @return void
 */
void chal_cw_pmon_config_lock (phy_info_t* phy_info_ptr, pam4_pmon_config_lock_cfg_t* pam4_pmon_config_lock_cfg_ptr);





/**
 * @brief  void  chal_cw_pmon_unlock_enable (phy_info_t* phy_info_ptr, uint8_t unlock_enable)
 * @detail Enable/Disable PMON unlock/relock feature
 * @public
 * @private
 * 
 * @param[in]  phy_info_ptr  :  Device physical information pointer
 * @param[in]  unlock_enable :  0=> Disable unlock, 1=> Enable unlock using settings defined below
 * @return     void:
*/
void chal_cw_pmon_unlock_enable (phy_info_t* phy_info_ptr, uint8_t unlock_enable);







/**
 * @brief  chal_cw_pmon_gen_err (phy_info_t* phy_info_ptr, prbs_gen_err_cfg_t* pmon_gen_err_cfg_ptr)
 * @detail Enable PMON to generate bit err-or(s)
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in] pmon_gen_err_cfg_ptr: pointer to structure 
 * @return : void:
*/
void chal_cw_pmon_gen_err (phy_info_t* phy_info_ptr, prbs_gen_err_cfg_t* pmon_gen_err_cfg_ptr);


/**
 * @brief  return_result_t chal_cw_pmon_read_err (phy_info_t* phy_info_ptr, uint8_t errsel, uint32_t *err_cnt)
 * @detail Read selected PMON bit err-or accumulator
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in] errsel:  Err-or accumulator select
 *                        0 = Read LSB/Even Bit Error Accumulator
 *                        1 = Read MSB/Odd  Bit Error Accumulator
 *                        3 = Read Bit Error Accumulator Overflow Flags
 *                        Others = RSVD
 * @return uint32_t:   register read data
*/
return_result_t chal_cw_pmon_read_err (phy_info_t* phy_info_ptr, uint8_t errsel, uint32_t *err_cnt);




/**
 * @brief  return_result_t chal_cw_pmon_get_status (phy_info_t* phy_info_ptr, pam4_pmon_stat_t* pam4_pmon_stat_ptr)
 * @detail Get PAM4 PMON Status
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr      : Device physical information pointer
 * @param[in] pam4_pmon_stat_ptr : Pointer to PAM4 PMON Config Type
 * @return return_result
*/
return_result_t chal_cw_pmon_get_status (phy_info_t* phy_info_ptr, pam4_pmon_stat_t* pam4_pmon_stat_ptr);



/**
 * @brief  void chal_cw_pmon_clr_oflow (phy_info_t* phy_info_ptr)
 * @detail Clear sticky PMON bit err-or accumulator overflow flags
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @return    void:
*/
void chal_cw_pmon_clr_oflow (phy_info_t* phy_info_ptr);



/**
 * @brief  void chal_cw_pmon_clr_accs (phy_info_t* phy_info_ptr)
 * @detail Clear sticky PMON bit err-or accumulators (and their overflow flags)
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @return    void:
*/
void chal_cw_pmon_clr_accs (phy_info_t* phy_info_ptr);



/**
 * @brief  void chal_cw_pmon_clr_lol (phy_info_t* phy_info_ptr)
 * @detail Clear sticky PMON loss-of-lock flag/IRQ
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @return    void:
*/
void chal_cw_pmon_clr_lol (phy_info_t* phy_info_ptr);



/**
 * @brief  void chal_cw_pmon_clr_all (phy_info_t* phy_info_ptr)
 * @detail Clear all PMON err-ors and sticky flags
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @return    void:
*/
void chal_cw_pmon_clr_all (phy_info_t* phy_info_ptr);



#ifdef __cplusplus
}
#endif
#endif


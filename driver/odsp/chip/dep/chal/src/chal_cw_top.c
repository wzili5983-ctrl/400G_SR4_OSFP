/**
 *
 * @file chal_cw_top.c 
 * @author  
 * @date     09/01/2018
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
#include "regs_common.h"
#include "common_def.h"
#include "chip_mode_def.h"
#include "cw_def.h"
#include "chal_cw_utils.h"
#include "chal_cw_top.h"

/**
 * @brief      chal_cw_tx_data_select(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,
 *                                     cfg_dp_type_t dp_type, cfg_egr_or_igr_t egr_or_igr, uint8_t is_disable) 
 * @details    TX data is muxed between retimer and repeater databus. Choose corresponding data based on repeater or retimer 
 * @public     any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 * @param[in]  phy_info_ptr device base address pointer 
 * @param[in]  cfg_dp_type_t :  repeater or retimer
 * @param[in]  cfg_host_or_line_t :  host or line
 * @return enum return_result_t
 */

return_result_t chal_cw_tx_data_select(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, 
                                       cfg_dp_type_t dp_type, cfg_egr_or_igr_t egr_or_igr, uint8_t is_disable)
{
    uint32_t reg_val = 0;
    uint32_t lane_mask = 0; 
    uint32_t rpt_lane_mask = 0; 
                
    // Repeater/RETIMER_PATH: EGR set mux based on line lane mask, IGR set mux based on host lane mask, 
    lane_mask  = (egr_or_igr == EGR) ? cur_mode_parameter_ptr->line_llane_mask : cur_mode_parameter_ptr->host_llane_mask;

    if (dp_type == REPEATER_PATH) {
        if (egr_or_igr == EGR) {
        // if line side is PAM100G, do lane double. use 80x2 muxes
		    rpt_lane_mask = (cur_mode_parameter_ptr->line_pam_or_nrz_type == CW_PAM100) ? duplicate_2(lane_mask) : lane_mask;
        } 
        else 
        {	// Ingress
        // if host side is PAM100G, do lane double. use 80x2 muxes
		  rpt_lane_mask = (cur_mode_parameter_ptr->host_pam_or_nrz_type == CW_PAM100) ? duplicate_2(lane_mask) : lane_mask;
      }
 
      reg_val = hsip_rd_reg_(phy_info_ptr, TMT_IFC_MUX_CTRL_RDB);
      reg_val &= (rpt_lane_mask ^ 0xFF);
      hsip_wr_reg_(phy_info_ptr,  TMT_IFC_MUX_CTRL_RDB, reg_val);
    
    } else {
        reg_val = hsip_rd_reg_(phy_info_ptr, TMT_IFC_MUX_CTRL_RDB);
        if(is_disable)
            reg_val &= (lane_mask ^ 0xFF);
        else
            reg_val |= (lane_mask & 0xFF);
        hsip_wr_reg_(phy_info_ptr, TMT_IFC_MUX_CTRL_RDB, reg_val);
    }
    return RR_SUCCESS;
}


/**
 * @brief return_result_t chal_cw_release_rtm_reset(phy_info_t* phy_info_ptr)
 * @details  Release retimer reset
 * @public   
 * @private 
 * @param  phy_info_ptr
 * @return enum return_result_t
 */
return_result_t chal_cw_release_rtm_reset(phy_info_t* phy_info_ptr)
{
    //uint32_t reg_val = 0;
    
    //reg_val = hsip_rd_reg_(phy_info_ptr, QUAD_CORE_EGRMGT_RDB_QC_SW_RST_RDB);
    //reg_val |= 0x0002;
    hsip_wr_field_(phy_info_ptr, QUAD_CORE_EGRMGT_QC_SW_RST_RDB, MD_RTM_ALL_RSTB, 0x1);
    return RR_SUCCESS;
}

/**
 * @brief return_result_t chal_cw_release_egr_reset(phy_info_t* phy_info_ptr, cfg_dp_type_t dp_type)
 * @details  Release EGR reset
 * @public   
 * @private 
 * @param  phy_info_ptr
 * @param  retimer/repeater datapath
 * @return enum return_result_t
 */
return_result_t chal_cw_release_egr_reset(phy_info_t* phy_info_ptr, cfg_dp_type_t dp_type)
{
    //uint32_t reg_val = 0;
    //reg_val = hsip_rd_reg_(phy_info_ptr, QUAD_CORE_EGRMGT_RDB_QC_DP_RST_RDB);    

    if (dp_type == REPEATER_PATH) {
       // REPEATER_PATH
       // reg_val |= 0x0001;
       hsip_wr_field_(phy_info_ptr, QUAD_CORE_EGRMGT_QC_DP_RST_RDB, MD_RPT_EGR_DP_RSTB, 0x1);
    } else {
        // RETIMER_PATH
      //  reg_val |= 0x0040;
       hsip_wr_field_(phy_info_ptr, QUAD_CORE_EGRMGT_QC_DP_RST_RDB, MD_RTM_EGR_DP_RSTB, 0x1);
    }
    //reg_val = hsip_wr_reg_(phy_info_ptr, QUAD_CORE_EGRMGT_RDB_QC_DP_RST_RDB, reg_val);    
    return RR_SUCCESS;
}

/**
 * @brief return_result_t chal_cw_release_igr_reset(phy_info_t* phy_info_ptr, cfg_dp_type_t dp_type)
 * @details  Release IGR reset
 * @public   
 * @private 
 * @param  phy_info_ptr
 * @param  retimer/repeater datapath
 * @return enum return_result_t
 */
return_result_t chal_cw_release_igr_reset(phy_info_t* phy_info_ptr, cfg_dp_type_t dp_type)
{
    // uint32_t reg_val = 0;
    // 
    // reg_val = hsip_rd_reg_(phy_info_ptr, QUAD_CORE_EGRMGT_RDB_QC_DP_RST_RDB);
    // 
    // if (dp_type == REPEATER_PATH) {
    //     // REPEATER_PATH
    //     reg_val |= 0x0002;
    // } else {
    //     // RETIMER_PATH
    //     reg_val |= 0x0008;
    // }
    // 
    // reg_val = hsip_wr_reg_(phy_info_ptr, QUAD_CORE_EGRMGT_RDB_QC_DP_RST_RDB, reg_val);

    if (dp_type == REPEATER_PATH) {
        // REPEATER_PATH
       hsip_wr_field_(phy_info_ptr, QUAD_CORE_EGRMGT_QC_DP_RST_RDB, MD_RPT_IGR_DP_RSTB, 0x1);
    } else {
       // RETIMER_PATH
       hsip_wr_field_(phy_info_ptr, QUAD_CORE_EGRMGT_QC_DP_RST_RDB, MD_RTM_IGR_DP_RSTB, 0x1);
    }  
    return RR_SUCCESS;
}


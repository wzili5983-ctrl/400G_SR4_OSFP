/**
 *
 * @file     capi_custome.c
 * @author   
 * @date     01/26/2021
 * @version  1.0
 *
 * @property 
 * $Copyright: (c) 2021 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * @brief    brief description of the block source file, sequences/flow/etc.
 *
 * @section  description
 *
 */
#include "access.h"
#include "common_def.h"
#include "regs_common.h"
#include "chip_mode_def.h"
#include "capi_def.h"
#include "capi_custom_def.h"
#include "capi_test_def.h"
#include "dsp_utils.h"
#include "lw_tx_def.h"
#include "capi_custom_def.h"
#include "host_lw_wrapper.h"
#include "capi_custom.h"
#include "lw_common_config_ind.h"

/**
 * @brief      capi_custom_command_request(capi_phy_info_t*            phy_info_ptr,
 *                                         capi_custom_command_info_t* cmd_inf_ptr)
 * @details    This API is used to invoke custom command.
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  cmd_inf_ptr:  a pointer which carries custom command 
 * @return     returns the performance result of the called method/function
 */
return_result_t capi_custom_command_request(capi_phy_info_t*            phy_info_ptr,
                                             capi_custom_command_info_t* cmd_inf_ptr)
{
    return_result_t return_result = RR_ERROR_WRONG_INPUT_VALUE;
    uint8_t lane_index;
    capi_phy_info_t lw_cfg_phy, lw_hw_phy;

    switch (cmd_inf_ptr->command_id) {
        case COMMAND_ID_SET_CUSTOM_TXFIR_TABLE:
            if (phy_info_ptr->core_ip == CORE_IP_HOST_DSP || phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP) {
                for (lane_index = 0; lane_index < MAX_LW_LANES; lane_index++) {
                    if (phy_info_ptr->lane_mask & (0x01 << lane_index)) {
                        lw_util_init_lane_config_base_addr(phy_info_ptr, &lw_cfg_phy, lane_index);
                        lw_util_init_lane_hw_base_addr(phy_info_ptr, &lw_hw_phy, lane_index);
                        //media side non-linear 13tap programming
                        //set the bit to notify FW to skip TXFIR control
                        hsip_wr_field_(&lw_cfg_phy, COMMON_LW_LANE_CONFIG28_REG, CONFIGED_NONLINEAR_13TAP, 1);
                        hsip_wr_field_(&lw_cfg_phy, COMMON_LW_LANE_CONFIG5_REG, LW_TXFIR_TYPE, NONLINWFTAP+1);
                        return_result = host_lw_tx_fir_set_floating_13tap(&lw_hw_phy, &cmd_inf_ptr->type.txfir_nonlinear_13tap_info);
                        
                        hsip_wr_field_(&lw_cfg_phy, COMMON_LW_LANE_CONFIG14_REG, TX_LEVEL_SHIFT_0, 0);
                        hsip_wr_field_(&lw_cfg_phy, COMMON_LW_LANE_CONFIG14_REG, TX_LEVEL_SHIFT_1, 0);
                        hsip_wr_field_(&lw_cfg_phy, COMMON_LW_LANE_CONFIG15_REG, TX_LEVEL_SHIFT_2, 0);
                        hsip_wr_field_(&lw_cfg_phy, COMMON_LW_LANE_CONFIG15_REG, TX_LEVEL_SHIFT_3, 0);
                        
                    }
                }
            }
            break;

        case COMMAND_ID_GET_CUSTOM_TXFIR_TABLE:
            if (phy_info_ptr->core_ip == CORE_IP_HOST_DSP || phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP) {
                for (lane_index = 0; lane_index < MAX_LW_LANES; lane_index++) {
                    if (phy_info_ptr->lane_mask & (0x01 << lane_index)) {
                        lw_util_init_lane_config_base_addr(phy_info_ptr, &lw_cfg_phy, lane_index);
                        lw_util_init_lane_hw_base_addr(phy_info_ptr, &lw_hw_phy, lane_index);
                         //media side non-linear 13tap programming
                         if(hsip_rd_field_(&lw_cfg_phy, COMMON_LW_LANE_CONFIG28_REG, CONFIGED_NONLINEAR_13TAP)==0)
                             return RR_ERROR;
                        return_result = host_lw_tx_fir_get_floating_13tap(&lw_hw_phy, &cmd_inf_ptr->type.txfir_nonlinear_13tap_info);
                        
                    }
                }
            }
            break;

        default:
            break;
    }
    return return_result;
}


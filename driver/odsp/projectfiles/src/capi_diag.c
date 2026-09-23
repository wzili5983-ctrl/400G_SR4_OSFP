/**
 *
 * @file     capi_diag.c
 * @author   
 * @date     8/5/2019
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
#include "chip_config_def.h"
#include "access.h"
#include "common_def.h"
#include "regs_common.h"
#include "chip_common_config_ind.h"
#include "fw_gp_reg_map.h"
#include "common_def.h"
#include "chip_mode_def.h"
#include "capi_def.h"
#include "capi.h"
#include "capi_fw_intf.h"
#include "host_to_chip_ipc.h"
#include "host_diag_util.h"
#include "host_log_util.h"
#include "common_util.h"
#include "cw_def.h"
#include "capi_diag_def.h"
#include "host_diag.h"
#include "capi_diag.h"
#include "diag_fec_statistics_def.h"
#include "host_diag_fec_statistics.h"
#include "host_diag.h"
#include "chip_config_def.h"


static return_result_t _chk_rprt_mode_fec_st_support(capi_fec_dump_status_t* capi_fec_dump_status_ptr, 
                                                        capi_config_ex_info_t* config_info_ptr)
{
    
    /* REPEATER_PATH */
    if (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_LINE) {
        if(config_info_ptr->lw_br==CAPI_LW_BR_53_125 ||
           config_info_ptr->lw_br==CAPI_LW_BR_106_25 ||
           config_info_ptr->lw_br==CAPI_LW_BR_26_5625)                           
            capi_fec_dump_status_ptr->fec_type.line_fec  = CAPI_LINE_FEC_TYPE_RS544;
        else if(config_info_ptr->lw_br==CAPI_LW_BR_25_78125 ||
                config_info_ptr->lw_br==CAPI_LW_BR_51_565 ||
                config_info_ptr->lw_br==CAPI_LW_BR_103_13)                           
            capi_fec_dump_status_ptr->fec_type.line_fec  = CAPI_LINE_FEC_TYPE_RS528;
        else
            return RR_ERROR_FEATURE_NOT_SUPPORTED;
    } else if (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_CLIENT) {
        if(config_info_ptr->bh_br==CAPI_LW_BR_53_125 ||
           config_info_ptr->bh_br==CAPI_LW_BR_106_25 ||
           config_info_ptr->bh_br==CAPI_LW_BR_26_5625)
            capi_fec_dump_status_ptr->fec_type.host_fec  = CAPI_HOST_FEC_TYPE_RS544;
        else if(config_info_ptr->bh_br==CAPI_LW_BR_25_78125 ||
                config_info_ptr->bh_br==CAPI_LW_BR_51_565 ||
                config_info_ptr->lw_br==CAPI_LW_BR_103_13)
            capi_fec_dump_status_ptr->fec_type.host_fec  = CAPI_HOST_FEC_TYPE_RS528;
        else
            return RR_ERROR_FEATURE_NOT_SUPPORTED;
    }
    return RR_SUCCESS;
}

static return_result_t _chk_rtmr_mode_fec_st_support(capi_fec_dump_status_t* capi_fec_dump_status_ptr, 
                                                        capi_config_ex_info_t* config_info_ptr)
{
    /*RETIMER_MODE*/
    if (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_LINE) {
        if((capi_line_fec_type_t)config_info_ptr->line_fec_type!=CAPI_LINE_FEC_TYPE_RS528 &&
            (capi_line_fec_type_t)config_info_ptr->line_fec_type!=CAPI_LINE_FEC_TYPE_CL161 &&
            (capi_line_fec_type_t)config_info_ptr->line_fec_type!=CAPI_LINE_FEC_TYPE_RS544)
            return RR_ERROR_FEATURE_NOT_SUPPORTED;
        capi_fec_dump_status_ptr->fec_type.line_fec  = (capi_line_fec_type_t)config_info_ptr->line_fec_type;
    } else if (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_CLIENT) {
        if((capi_host_fec_type_t)config_info_ptr->host_fec_type!=CAPI_HOST_FEC_TYPE_RS528 &&
            (capi_host_fec_type_t)config_info_ptr->host_fec_type!=CAPI_HOST_FEC_TYPE_CL161 &&
            (capi_host_fec_type_t)config_info_ptr->host_fec_type!=CAPI_HOST_FEC_TYPE_RS544 )
            return RR_ERROR_FEATURE_NOT_SUPPORTED;
        capi_fec_dump_status_ptr->fec_type.host_fec  = (capi_host_fec_type_t)config_info_ptr->host_fec_type;
    }
    return RR_SUCCESS;
}

static return_result_t _chk_chip_mode_fec_st_support(capi_fec_dump_status_t* capi_fec_dump_status_ptr, 
                                                        capi_config_ex_info_t* config_info_ptr)
{
    uint16_t fec_term;

    fec_term = (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_CLIENT) ? config_info_ptr->egr_fec_term : config_info_ptr->igr_fec_term;

    /*
    // FWPLPHSIP-3845 FEC mon support for gearbox repeater modes
    if((enum cw_port_fec_term_type_e)fec_term==CHIP_PORT_FEC_TERM_BYPASS &&
        (config_info_ptr->line_lane.lane_mask != config_info_ptr->host_lane.lane_mask))
        return RR_ERROR_FEATURE_NOT_SUPPORTED;
    */

    /*check current data path type*/
    if((enum cw_port_fec_term_type_e)fec_term==CHIP_PORT_FEC_TERM_BYPASS){
       /* REPEATER_PATH */
        ERR_FUNC(_chk_rprt_mode_fec_st_support(capi_fec_dump_status_ptr, config_info_ptr));
    }else {
       /* RETIMER_PATH */
        ERR_FUNC(_chk_rtmr_mode_fec_st_support(capi_fec_dump_status_ptr, config_info_ptr));
    }
    if ((capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_LINE && 
        (capi_fec_dump_status_ptr->fec_type.line_fec != CAPI_LINE_FEC_TYPE_RS528) &&
        (capi_fec_dump_status_ptr->fec_type.line_fec != CAPI_LINE_FEC_TYPE_CL161) &&
        (capi_fec_dump_status_ptr->fec_type.line_fec != CAPI_LINE_FEC_TYPE_RS544)) || 
        (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_CLIENT && 
        (capi_fec_dump_status_ptr->fec_type.host_fec != CAPI_HOST_FEC_TYPE_RS528) && 
        (capi_fec_dump_status_ptr->fec_type.line_fec != CAPI_LINE_FEC_TYPE_CL161) &&
        (capi_fec_dump_status_ptr->fec_type.host_fec != CAPI_HOST_FEC_TYPE_RS544))) {
            CAPI_LOG_ERROR("Feature Not Supported since no FEC protocol. fec_mode %d host %d line %d \n", 
                capi_fec_dump_status_ptr->fec_mode,
                capi_fec_dump_status_ptr->fec_type.host_fec, capi_fec_dump_status_ptr->fec_type.line_fec);
        return RR_ERROR_FEATURE_NOT_SUPPORTED;
    }
    return RR_SUCCESS;
}

/**
* @brief      capi_init_fec_mon(capi_phy_info_t* phy_info_ptr, capi_fec_dump_status_t* capi_fec_dump_status_ptr, boolean enable)
* @details    This API is used to 
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  fec_info_ptr: this parameter 
* 
* @return     returns the performance result of the called methode/function
*/
return_result_t capi_init_fec_mon (capi_phy_info_t* phy_info_ptr, capi_fec_dump_status_t* capi_fec_dump_status_ptr, boolean enable)
{
    return_result_t ret_val = RR_SUCCESS;
    capi_config_ex_info_t config_info;
    capi_phy_info_t phy_info;
    cw_port_info_t fec_port_info;

    CAPI_LOG_FUNC("capi_init_fec_mon", phy_info_ptr);

    if (phy_info_ptr->lane_mask == 0) {
        CAPI_LOG_ERROR("Invalid Lane Mask. %d \n", phy_info_ptr->lane_mask);
        return RR_ERROR_INITIALIZATION;
    }
    if ((capi_fec_dump_status_ptr->fec_mode != CAPI_FEC_LINE) && (capi_fec_dump_status_ptr->fec_mode != CAPI_FEC_CLIENT)) {
        CAPI_LOG_ERROR("Wrong input for side. %d \n", capi_fec_dump_status_ptr->fec_mode);
        return RR_ERROR_WRONG_INPUT_VALUE;
    }
	capi_fec_dump_status_ptr->enable_time_slice = 1;
    if (capi_fec_dump_status_ptr->enable_time_slice) {
        /*
        *  This is the optimized version of capi_init_fec_mon with the following new features:
        *  1. Move the handler to FW to handle it faster.
        *  2. Support multiple ports. So users can configure all 8 ports (by setting lane_mask=0xFF) in one capi call.
        *  3. To prevent power surge, FW will time slice the VDM init of all 8 ports of EGR and IGR directions, VDM init of each port will separated by 1/16 = 62.5ms.
        */
        capi_command_info_t command_info;
        
        util_memcpy((void *)&phy_info, phy_info_ptr, sizeof(capi_phy_info_t));
        phy_info.core_ip = CORE_IP_CW;
        command_info.command_id = COMMAND_ID_SET_CW_RPTR_FEC_MON_CONFIG;
        command_info.type.rptr_fec_st.fec_mode = capi_fec_dump_status_ptr->fec_mode;
        command_info.type.rptr_fec_st.is_enable = enable;
        command_info.type.rptr_fec_st.sampling_rate = capi_fec_dump_status_ptr->sampling_rate;
        command_info.type.rptr_fec_st.fec_monitor_discover = 0;
        /* default value 0 is 100% sampling for backward compatible */
        if (command_info.type.rptr_fec_st.sampling_rate == 0 || command_info.type.rptr_fec_st.sampling_rate > 100)
            command_info.type.rptr_fec_st.sampling_rate = 100;
        
        if(host_diag_cw_is_port_based_fec_mon(&phy_info)){
            command_info.type.rptr_fec_st.fec_monitor_port = 1;
        }else {
            command_info.type.rptr_fec_st.fec_monitor_port = 0;
        }
        return(capi_command_request(&phy_info, &command_info, sizeof(capi_rptr_fec_st_t), SET_CONFIG));        
    }

    util_memset(&config_info, 0, sizeof(capi_config_ex_info_t));
    if (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_LINE) {
        config_info.line_lane.lane_mask = phy_info_ptr->lane_mask;   /* Using line-side lane_mask for identifying PORT */
    } else if (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_CLIENT) {
        config_info.host_lane.lane_mask = phy_info_ptr->lane_mask;   /* Using host-side lane_mask for identifying PORT */
    }

    util_memcpy((void *)&phy_info, phy_info_ptr, sizeof(capi_phy_info_t));
    phy_info.base_addr = OCTAL_TOP_REGS;

    /*retrieve the config information*/
    if(host_diag_cw_is_port_based_fec_mon(&phy_info)){
        ERR_FUNC(host_diag_cw_get_chip_config_fec_mon(&phy_info, &config_info, &fec_port_info));
    }else {
    ERR_FUNC(host_diag_cw_get_chip_config(&phy_info, &config_info, &fec_port_info));
    }
    
    capi_fec_dump_status_ptr->func_mode = (capi_function_mode_t)config_info.func_mode;
    /*check current chip mode support FEC ST or not*/
    ERR_FUNC(_chk_chip_mode_fec_st_support(capi_fec_dump_status_ptr, &config_info));

    
    if (enable)
        ret_val = host_diag_cw_fec_stat_init (phy_info_ptr, capi_fec_dump_status_ptr, (capi_function_mode_t)config_info.func_mode);
    else 
        ret_val = host_diag_cw_fec_stat_deinit (phy_info_ptr, capi_fec_dump_status_ptr, (capi_function_mode_t)config_info.func_mode);
    return ret_val; 
}

/**
* @brief      capi_clear_fec_mon (capi_phy_info_t* phy_info_ptr, capi_fec_dump_status_t* capi_fec_dump_status_ptr)
* @details    This API is used to clear FEC BRCM counters 
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  fec_info_ptr: this parameter 
* 
* @return     returns the performance result of the called methode/function
*/
return_result_t capi_clear_fec_mon (capi_phy_info_t* phy_info_ptr,capi_fec_dump_status_t* capi_fec_dump_status_ptr)
{
    return_result_t ret_val = RR_SUCCESS;
    capi_config_ex_info_t config_info;
    capi_phy_info_t phy_info;
    cw_port_info_t port_info;

    CAPI_LOG_FUNC("capi_clear_fec_mon", phy_info_ptr);

    if (phy_info_ptr->lane_mask == 0) {
        CAPI_LOG_ERROR("Invalid Lane Mask. %d \n", phy_info_ptr->lane_mask);
        return RR_ERROR_INITIALIZATION;
    }
    if ((capi_fec_dump_status_ptr->fec_mode != CAPI_FEC_LINE) && (capi_fec_dump_status_ptr->fec_mode != CAPI_FEC_CLIENT)) {
        CAPI_LOG_ERROR("Wrong input for side. %d \n", capi_fec_dump_status_ptr->fec_mode);
        return RR_ERROR_WRONG_INPUT_VALUE;
    }

	capi_fec_dump_status_ptr->enable_time_slice = 1;
    util_memset(&config_info, 0, sizeof(capi_config_ex_info_t));
    if (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_LINE) {
        config_info.line_lane.lane_mask = phy_info_ptr->lane_mask;   /* Using line-side lane_mask for identifying PORT */
    } else if (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_CLIENT) {
        config_info.host_lane.lane_mask = phy_info_ptr->lane_mask;   /* Using host-side lane_mask for identifying PORT */
    }

    util_memcpy((void *)&phy_info, phy_info_ptr, sizeof(capi_phy_info_t));
    phy_info.base_addr = OCTAL_TOP_REGS;
    /*retrieve the config information*/
     if(host_diag_cw_is_port_based_fec_mon(&phy_info)){
        ERR_FUNC(host_diag_cw_get_chip_config_fec_mon(&phy_info, &config_info, &port_info));
    }else {
    ERR_FUNC(host_diag_cw_get_chip_config(&phy_info, &config_info, &port_info));
     }
    capi_fec_dump_status_ptr->func_mode = (capi_function_mode_t)config_info.func_mode;

    /*check current chip mode support FEC ST or not*/
    ERR_FUNC(_chk_chip_mode_fec_st_support(capi_fec_dump_status_ptr, &config_info));

    ret_val = host_diag_cw_fec_stat_clear (phy_info_ptr, capi_fec_dump_status_ptr, (capi_function_mode_t)config_info.func_mode, TRUE);
    return ret_val; 
}

/**
* @brief      capi_get_fec_info(capi_phy_info_t* phy_info_ptr, capi_fec_dump_status_t* capi_fec_dump_status_ptr)
* @details    This API is used to 
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[out] fec_info_ptr: this parameter 
* 
* @return     returns the performance result of the called methode/function
*/
return_result_t capi_get_fec_info(capi_phy_info_t* phy_info_ptr, capi_fec_dump_status_t* capi_fec_dump_status_ptr)
{
    return_result_t ret_val = RR_SUCCESS;
    capi_config_ex_info_t config_info;
    capi_phy_info_t phy_info;
    cw_port_info_t port_info;

    CAPI_LOG_FUNC("capi_get_fec_info", phy_info_ptr);

    if (phy_info_ptr->lane_mask == 0) {
        CAPI_LOG_ERROR("Invalid Lane Mask. %d \n", phy_info_ptr->lane_mask);
        return RR_ERROR_INITIALIZATION;
    }

    if ((capi_fec_dump_status_ptr->fec_mode != CAPI_FEC_LINE) && (capi_fec_dump_status_ptr->fec_mode != CAPI_FEC_CLIENT)) {
        CAPI_LOG_ERROR("Wrong input for side. %d \n", capi_fec_dump_status_ptr->fec_mode);
        return RR_ERROR_WRONG_INPUT_VALUE;
    }

	capi_fec_dump_status_ptr->enable_time_slice = 1;
    util_memset(&config_info, 0, sizeof(capi_config_ex_info_t));
    if (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_LINE) {
        config_info.line_lane.lane_mask = phy_info_ptr->lane_mask;   /* Using line-side lane_mask for identifying PORT */
    } else if (capi_fec_dump_status_ptr->fec_mode == CAPI_FEC_CLIENT) {
        config_info.host_lane.lane_mask = phy_info_ptr->lane_mask;   /* Using host-side lane_mask for identifying PORT */
    }
    
    util_memcpy((void *)&phy_info, phy_info_ptr, sizeof(capi_phy_info_t));
    phy_info.base_addr = OCTAL_TOP_REGS;
    /*retrieve the config information*/
    if(host_diag_cw_is_port_based_fec_mon(&phy_info)){
        ERR_FUNC(host_diag_cw_get_chip_config_fec_mon(&phy_info, &config_info, &port_info));
    }else {
    ERR_FUNC(host_diag_cw_get_chip_config(&phy_info, &config_info, &port_info));
    }
    
    capi_fec_dump_status_ptr->func_mode = (capi_function_mode_t)config_info.func_mode;
    

    /*check current chip mode support FEC ST or not*/
    ERR_FUNC(_chk_chip_mode_fec_st_support(capi_fec_dump_status_ptr, &config_info));

    ret_val = host_diag_cw_get_fec_info (phy_info_ptr, capi_fec_dump_status_ptr, (capi_function_mode_t)config_info.func_mode);
    return ret_val; 
}


/**
* @brief      capi_get_usr_diagnostics(capi_phy_info_t* phy_info_ptr, capi_usr_diag_info_t* usr_diag_info_ptr)
* @details    This API is used to get the user diag info
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  usr_diag_info_ptr: this parameter
*
* @return     returns the performance result of the called method/function
*/
return_result_t capi_get_usr_diagnostics(capi_phy_info_t* phy_info_ptr, capi_usr_diag_info_t* usr_diag_info_ptr)
{
    CAPI_LOG_FUNC("capi_get_usr_diagnostics", phy_info_ptr);
    if ((phy_info_ptr->core_ip == CORE_IP_HOST_DSP) ||
        (phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP)) {
        if (usr_diag_info_ptr->diag_info_type == DIAG_INFO_TYPE_CMIS)
            return host_lw_get_usr_cmis_diagnostics(phy_info_ptr, usr_diag_info_ptr);
        else if (usr_diag_info_ptr->diag_info_type == DIAG_INFO_TYPE_BRCM_DSP_SNR)
            return host_lw_get_usr_diagnostics(phy_info_ptr, usr_diag_info_ptr);
        else
            return (RR_ERROR_WRONG_INPUT_VALUE);
    }
    else {
        return (RR_ERROR_WRONG_INPUT_VALUE);
    }
}

/**
* @brief      capi_fec_ber_cmis_latch_request(capi_phy_info_t *phy_ptr, capi_kp4fec_ber_state_t* fec_ptr)
* @details    This API is used to send the FEC BER latch request;
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  fec_ptr: this parameter pointer point to the BER state;
* 
* @return     returns the performance result of the called methode/function
*/
return_result_t capi_fec_ber_cmis_latch_request(capi_phy_info_t *phy_ptr, capi_kp4fec_ber_state_t* fec_ptr)
{
    return_result_t ret_val = RR_SUCCESS;
    bool clr_cnt=fec_ptr->clr_cnt, only_cur=fec_ptr->only_cur;
    capi_fec_mode_t  fec_mode = fec_ptr->fec_mode;
    capi_fec_dump_status_t fec_dump_st;
    uint8_t regcnt, total_uncorr_bpf;
    CAPI_LOG_FUNC("capi_fec_ber_cmis_latch_request", phy_ptr);
    if ((fec_mode != CAPI_FEC_LINE) && (fec_mode != CAPI_FEC_CLIENT)) {
        CAPI_LOG_ERROR("Wrong fec mode. %d \n", fec_ptr->fec_mode);
        return RR_ERROR_WRONG_INPUT_VALUE;
    }
    /*clear the structure in case of any wrong information is brought in*/
    util_memset(fec_ptr, 0, sizeof(capi_kp4fec_ber_state_t));
    fec_ptr->fec_mode = fec_mode;
    fec_ptr->clr_cnt = clr_cnt;
    fec_ptr->only_cur = only_cur;

    util_memset(&fec_dump_st, 0, sizeof(capi_fec_dump_status_t));
    fec_dump_st.fec_mode = fec_mode;

    /*If clr_cnt is TRUE, call clr function; else, read one set of data as start point*/
    if(fec_ptr->clr_cnt){
        ret_val = capi_clear_fec_mon(phy_ptr, &fec_dump_st);
        if ( ret_val!= RR_SUCCESS)
        {
             CAPI_LOG_ERROR("\n  *** capi_clear_fec_mon call failed. \r\n");
             return ret_val;
        }
    }else{
        ret_val = capi_get_fec_info(phy_ptr, &fec_dump_st);
        if ( ret_val!= RR_SUCCESS)
        {
             CAPI_LOG_ERROR("\n  *** capi_get_fec_info call failed. \r\n");
             return ret_val;
        }
        fec_ptr->fec_algn_stat = fec_dump_st.fec_sts.fec_algn_stat;
        fec_ptr->fec_algn_lol_sticky = fec_dump_st.fec_sts.fec_algn_lol_sticky;

        if ((fec_dump_st.fec_mode == CAPI_FEC_LINE && fec_dump_st.fec_type.line_fec == CAPI_LINE_FEC_TYPE_RS528) ||
            (fec_dump_st.fec_mode == CAPI_FEC_CLIENT && fec_dump_st.fec_type.host_fec == CAPI_HOST_FEC_TYPE_RS528)) {
            total_uncorr_bpf = 8;
#ifndef CAPI_CMIS_FEC_STATS_SMALL_FOOTPRINT
            fec_ptr->cur_err.fec_frame_size = 5280;      /* KR4 is 5280 */
            fec_ptr->min_err.fec_frame_size = 5280;
            fec_ptr->max_err.fec_frame_size = 5280;
            fec_ptr->average_err.fec_frame_size = 5280;
            fec_ptr->cur_err.uncorr_bits_per_frame = 8;  /* KR4 is 8   */
            fec_ptr->min_err.uncorr_bits_per_frame = 8;
            fec_ptr->max_err.uncorr_bits_per_frame = 8;
            fec_ptr->average_err.uncorr_bits_per_frame = 8;
#endif
            fec_ptr->total_err.fec_frame_size = 5280;
            fec_ptr->total_err.uncorr_bits_per_frame = 8;
        }
        else { /* KP4 */
            total_uncorr_bpf = 16;
#ifndef CAPI_CMIS_FEC_STATS_SMALL_FOOTPRINT
            fec_ptr->cur_err.fec_frame_size = 5440;      /* KP4 is 5440 */
            fec_ptr->min_err.fec_frame_size = 5440;
            fec_ptr->max_err.fec_frame_size = 5440;
            fec_ptr->average_err.fec_frame_size = 5440;
            fec_ptr->cur_err.uncorr_bits_per_frame = 16; /* KP4 is 16 */
            fec_ptr->min_err.uncorr_bits_per_frame = 16;
            fec_ptr->max_err.uncorr_bits_per_frame = 16;
            fec_ptr->average_err.uncorr_bits_per_frame = 16;
#endif
            fec_ptr->total_err.fec_frame_size = 5440;
            fec_ptr->total_err.uncorr_bits_per_frame = 16;
        }

        /*Store hardware counter into callback pointer*/
        for (regcnt = 0; regcnt < total_uncorr_bpf; regcnt++) {
            fec_ptr->backup_err.prbs_errcnt[regcnt] = fec_dump_st.fec_a_err_cnt.tot_frames_err_cnt[regcnt] + fec_dump_st.fec_b_err_cnt.tot_frames_err_cnt[regcnt];
        }

        for (regcnt = 0; regcnt < KP4_FEC_TOT_BITS_CORR_NUM; regcnt++) {
            fec_ptr->backup_err.tot_bits_corr_cnt[regcnt] = fec_dump_st.fec_a_err_cnt.tot_bits_corr_cnt[regcnt] + fec_dump_st.fec_b_err_cnt.tot_bits_corr_cnt[regcnt];
        }
        fec_ptr->backup_err.tot_frame_cnt = fec_dump_st.fec_a_err_cnt.tot_frame_rev_cnt + fec_dump_st.fec_b_err_cnt.tot_frame_rev_cnt;
        fec_ptr->backup_err.tot_frame_uncorr_cnt = fec_dump_st.fec_a_err_cnt.tot_frame_uncorr_cnt + fec_dump_st.fec_b_err_cnt.tot_frame_uncorr_cnt;
    }
    /* Initialize the minimum error counter and BER */
    if(fec_ptr->only_cur == 0){
        util_memset(&fec_ptr->min_err, 0xFF, sizeof(fec_ptr->min_err));
        fec_ptr->pre_fec_ber.min_ber = (double)0xFFFFFFFFFFFFF;
        fec_ptr->post_fec_ber.min_ber = (double)0xFFFFFFFFFFFFF;
    }
    return ret_val;
}

/**
* @brief      capi_fec_ber_cmis_latch_release(capi_phy_info_t *phy_ptr, capi_kp4fec_ber_state_t* fec_ptr)
* @details    This API is used to 
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  fec_info_ptr: this parameter 
* 
* @return     returns the performance result of the called methode/function
*/
return_result_t capi_fec_ber_cmis_latch_release(capi_phy_info_t *phy_ptr, capi_kp4fec_ber_state_t* fec_ptr)
{
    return_result_t ret_val = RR_SUCCESS;
    capi_fec_dump_status_t fec_dump_st;
    CAPI_LOG_FUNC("capi_fec_ber_cmis_latch_release", phy_ptr);
    if ((fec_ptr->fec_mode != CAPI_FEC_LINE) && (fec_ptr->fec_mode != CAPI_FEC_CLIENT)) {
        CAPI_LOG_ERROR("Wrong fec mode. %d \n", fec_ptr->fec_mode);
        return RR_ERROR_WRONG_INPUT_VALUE;
    }
    util_memset(&fec_dump_st, 0, sizeof(capi_fec_dump_status_t));
    fec_dump_st.fec_mode = fec_ptr->fec_mode;

    ret_val = capi_get_fec_info(phy_ptr, &fec_dump_st);
    if ( ret_val!= RR_SUCCESS)
    {
         CAPI_LOG_ERROR("\n  *** capi_get_fec_info call failed 0x%x \r\n", ret_val);
         return ret_val;
    }
    fec_ptr->fec_algn_stat = fec_dump_st.fec_sts.fec_algn_stat;
    fec_ptr->fec_algn_lol_sticky = fec_dump_st.fec_sts.fec_algn_lol_sticky;

    if ((fec_dump_st.fec_mode == CAPI_FEC_LINE && fec_dump_st.fec_type.line_fec == CAPI_LINE_FEC_TYPE_RS528) ||
        (fec_dump_st.fec_mode == CAPI_FEC_CLIENT && fec_dump_st.fec_type.host_fec == CAPI_HOST_FEC_TYPE_RS528)) { /* KR4 */
#ifndef CAPI_CMIS_FEC_STATS_SMALL_FOOTPRINT
       fec_ptr->cur_err.fec_frame_size = 5280;      /* KR4 is 5280 */
       fec_ptr->min_err.fec_frame_size = 5280;
       fec_ptr->max_err.fec_frame_size = 5280;
       fec_ptr->average_err.fec_frame_size = 5280;
       fec_ptr->cur_err.uncorr_bits_per_frame = 8;  /* KR4 is 8   */
       fec_ptr->min_err.uncorr_bits_per_frame = 8;
       fec_ptr->max_err.uncorr_bits_per_frame = 8;
       fec_ptr->average_err.uncorr_bits_per_frame = 8;
#endif
       fec_ptr->total_err.fec_frame_size = 5280;
       fec_ptr->total_err.uncorr_bits_per_frame = 8;
    }
    else { /* KP4 */
#ifndef CAPI_CMIS_FEC_STATS_SMALL_FOOTPRINT
       fec_ptr->cur_err.fec_frame_size = 5440;      /* KP4 is 5440 */
       fec_ptr->min_err.fec_frame_size = 5440;
       fec_ptr->max_err.fec_frame_size = 5440;
       fec_ptr->average_err.fec_frame_size = 5440;
       fec_ptr->cur_err.uncorr_bits_per_frame = 16;  /* KP4 is 16 */
       fec_ptr->min_err.uncorr_bits_per_frame = 16;
       fec_ptr->max_err.uncorr_bits_per_frame = 16;
       fec_ptr->average_err.uncorr_bits_per_frame = 16;
#endif
       fec_ptr->total_err.fec_frame_size = 5440;
       fec_ptr->total_err.uncorr_bits_per_frame = 16;
    }


    /*if request clear hardware counter, then clear up the backup*/
    if(fec_ptr->clr_cnt)
    {
        ret_val = capi_clear_fec_mon(phy_ptr, &fec_dump_st);
        if ( ret_val!= RR_SUCCESS)
        {
             CAPI_LOG_ERROR("\n  *** capi_clear_fec_mon call failed 0x%x \r\n", ret_val);
             return ret_val;
        }
        util_memset((void *)&fec_ptr->backup_err, 0, sizeof(cw_kp4fec_err_cnt_t));
    }
    ret_val = host_diag_cw_calculate_ber_between_latch(phy_ptr, &fec_dump_st, fec_ptr);
    if ( ret_val!= RR_SUCCESS)
    {
         CAPI_LOG_ERROR("\n  *** _diag_cw_calculate_ber_between_latch call failed 0x%x. \r\n", ret_val);
         return ret_val;
    }
    return ret_val;
}


/**
 * @brief      capi_diag_command_request_info(capi_phy_info_t*                  phy_info_ptr,
 *                                            capi_diag_command_request_info_t* cmd_req_ptr)
 * @details    Function to diag command request
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr : pointer to phy_info
 * @param      cmd_req_ptr  : command request
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t capi_diag_command_request_info(capi_phy_info_t*                  phy_info_ptr,
                                               capi_diag_command_request_info_t* diag_cmd_req_info_ptr)
{
    return_result_t         return_result = RR_SUCCESS;
    capi_command_info_t     command_info;
    capi_phy_info_t         phy_info;

    util_memset((void*)&command_info, 0, sizeof(capi_command_info_t));
    util_memcpy(&command_info.type.diag_cmd_req_info, diag_cmd_req_info_ptr, sizeof(capi_diag_command_request_info_t));
    util_memcpy((void *)&phy_info, phy_info_ptr, sizeof(capi_phy_info_t));

    switch(diag_cmd_req_info_ptr->command_id)
    {
        case COMMAND_ID_START_DIAG_DUMP:
            command_info.command_id = diag_cmd_req_info_ptr->command_id;
            return_result = capi_command_request(&phy_info,
                                                 &command_info,
                                                 sizeof(memory_data_info_t),
                                                 GET_CONFIG);

            /* Copy the memory details to user provided cmd_req structure */
            util_memcpy(&diag_cmd_req_info_ptr->type.dsp_diag_dump.memory_info,
                        &command_info.type.diag_cmd_req_info.type.dsp_diag_dump.memory_info, sizeof(memory_info_t));
            break;

        case COMMAND_ID_GET_DIAG_DUMP:
            {
                capi_diag_dump_t* diag_dump_ptr = &diag_cmd_req_info_ptr->type.dsp_diag_dump;
                dsp_diag_dump_header_t dsp_diag_dump_header;
                uint32_t *dsp_diag_dump_header_ptr = (uint32_t *)&dsp_diag_dump_header;

                if (!diag_dump_ptr->memory_info.ref_address || !diag_dump_ptr->dump_buf_ptr ||
                    diag_dump_ptr->dump_buf_size < diag_dump_ptr->memory_info.ref_data_len)
                    return RR_ERROR_WRONG_INPUT_VALUE;

                if (!diag_dump_ptr->memory_info.ref_address || !diag_dump_ptr->dump_buf_ptr ||
                    diag_dump_ptr->dump_buf_size < diag_dump_ptr->memory_info.ref_data_len)
                    return RR_ERROR_WRONG_INPUT_VALUE;

                // check diag dump header
                phy_info.base_addr = 0;
                *dsp_diag_dump_header_ptr = hsip_rd_reg_(&phy_info, diag_dump_ptr->memory_info.ref_address);
                if (dsp_diag_dump_header.dsp_diag_uuid != DSP_DIAG_DUMP_UUID ||
                    dsp_diag_dump_header.dsp_diag_size != diag_dump_ptr->memory_info.ref_data_len)
                    return RR_ERROR_NOT_INITIALIZED;
                
                /* Read FW memory data info */
                phy_info.base_addr = 0;
                intf_read_memory(&phy_info,
                        (uint32_t*)diag_dump_ptr->dump_buf_ptr,
                        (uint32_t*)(uintptr_t) (diag_dump_ptr->memory_info.ref_address),
                        diag_dump_ptr->memory_info.ref_data_len);
                
            }
            break;
    
        case COMMAND_ID_DIAG_SET_CMIS_SNR_LTP_INFO:
            command_info.command_id = diag_cmd_req_info_ptr->command_id;
            return_result = capi_command_request(&phy_info,
                                                 &command_info,
                                                 sizeof(memory_data_info_t),
                                                 GET_CONFIG);

            /* Copy the memory details to user provided cmd_req structure */
            util_memcpy(&diag_cmd_req_info_ptr->type.cmis_snr_ltp.memory_info,\
                        &command_info.type.diag_cmd_req_info.type.cmis_snr_ltp.memory_info, sizeof(memory_info_t));
            break;

        case COMMAND_ID_DIAG_GET_CMIS_SNR_LTP_CONFIG_INFO:
            command_info.command_id = diag_cmd_req_info_ptr->command_id;
            return_result = capi_command_request(&phy_info,
                                                 &command_info,
                                                 sizeof(memory_data_info_t),
                                                 GET_CONFIG);

             /* Copy the memory details to user provided cmd_req structure */
            util_memcpy(&diag_cmd_req_info_ptr->type.cmis_snr_ltp,\
                        &command_info.type.diag_cmd_req_info.type.cmis_snr_ltp, sizeof(memory_data_info_t));
            break;


        case COMMAND_ID_DIAG_GET_CMIS_SNR_LTP_INFO:   
            return_result = host_get_memory_data_info(phy_info_ptr, &diag_cmd_req_info_ptr->type.cmis_snr_ltp);
            break;

        case COMMAND_ID_DIAG_SET_MEDIA_MPI_CONFIG:
            return_result = sanity_checker_mpi_config_info(phy_info_ptr, &diag_cmd_req_info_ptr->type.mpi_cfg, CHK_USR_INPUT);
            if(return_result!=RR_SUCCESS)
                return return_result;
            return_result = host_set_mpi_config(phy_info_ptr, &diag_cmd_req_info_ptr->type.mpi_cfg);
        break;
        
        case COMMAND_ID_DIAG_GET_MEDIA_MPI_CONFIG:
            return_result = host_get_mpi_config(phy_info_ptr, &diag_cmd_req_info_ptr->type.mpi_cfg);
            if(return_result!=RR_SUCCESS)
                return return_result;
            return_result = sanity_checker_mpi_config_info(phy_info_ptr, &diag_cmd_req_info_ptr->type.mpi_cfg, CHK_FW_OUTPUT);
        break;
        
        case COMMAND_ID_DIAG_GET_MEDIA_MPI_STATE:
            return_result = host_get_mpi_state(phy_info_ptr, &diag_cmd_req_info_ptr->type.mpi_st);
        break;
        
        case COMMAND_ID_DIAG_SET_MEDIA_MISSION_MPI_CONFIG:
            return_result = dsp_mission_mpi_cfg_sanity_checker(phy_info_ptr, &diag_cmd_req_info_ptr->type.mission_mpi_cfg, CHK_USR_INPUT);
            if (return_result != RR_SUCCESS)
                return return_result;
            return_result = host_set_mpi_dynamic_config(phy_info_ptr, &diag_cmd_req_info_ptr->type.mission_mpi_cfg);
        break;
        
        case COMMAND_ID_DIAG_GET_MEDIA_MISSION_MPI_CONFIG:
            return_result = host_get_mpi_dynamic_config(phy_info_ptr, &diag_cmd_req_info_ptr->type.mission_mpi_cfg);
            if(return_result!=RR_SUCCESS)
                return return_result;
            return_result = dsp_mission_mpi_cfg_sanity_checker(phy_info_ptr, &diag_cmd_req_info_ptr->type.mission_mpi_cfg, CHK_FW_OUTPUT);
        break;

        case COMMAND_ID_DIAG_GET_MEDIA_MISSION_MPI_STATE:
            return_result = host_get_dynamic_mpi_state(phy_info_ptr, &diag_cmd_req_info_ptr->type.mission_mpi_st);
        break;
        
        case COMMAND_ID_DIAG_SET_TRAFFIC_SWITCH_DETECT_CONFIG:
            return_result = dsp_traffic_mode_switch_detect_cfg_sanity_checker(phy_info_ptr, &diag_cmd_req_info_ptr->type.tmsd_cfg, CHK_USR_INPUT);
            if(return_result!=RR_SUCCESS)
                return return_result;
            return_result = host_set_tmsd_config(phy_info_ptr, &diag_cmd_req_info_ptr->type.tmsd_cfg);
        break;
        
        case COMMAND_ID_DIAG_GET_TRAFFIC_SWITCH_DETECT_CONFIG:
            return_result = host_get_tmsd_config(phy_info_ptr, &diag_cmd_req_info_ptr->type.tmsd_cfg);
            if(return_result!=RR_SUCCESS)
                return return_result;
            return_result = dsp_traffic_mode_switch_detect_cfg_sanity_checker(phy_info_ptr, &diag_cmd_req_info_ptr->type.tmsd_cfg, CHK_FW_OUTPUT);
        break;
        
        case COMMAND_ID_DIAG_GET_TRAFFIC_SWITCH_DETECT_STATE:
            return_result = host_get_tmsd_state(phy_info_ptr, &diag_cmd_req_info_ptr->type.tmsd_st);
        break;
        
        
        case COMMAND_ID_DIAG_SET_MPI_CANCELLER_CONFIG:
            return_result = dsp_mpi_canceller_cfg_sanity_checker(phy_info_ptr, &diag_cmd_req_info_ptr->type.mpi_canceller_cfg, CHK_USR_INPUT);
            if(return_result!=RR_SUCCESS)
                return return_result;
            return_result = host_set_mpi_canceller_config(phy_info_ptr, &diag_cmd_req_info_ptr->type.mpi_canceller_cfg);
        break;
        
        case COMMAND_ID_DIAG_GET_MPI_CANCELLER_CONFIG:
            return_result = host_get_mpi_canceller_config(phy_info_ptr, &diag_cmd_req_info_ptr->type.mpi_canceller_cfg);
            if(return_result!=RR_SUCCESS)
                return return_result;
            return_result = dsp_mpi_canceller_cfg_sanity_checker(phy_info_ptr, &diag_cmd_req_info_ptr->type.mpi_canceller_cfg, CHK_FW_OUTPUT);
        break;

        case COMMAND_ID_GET_DSP_DEBUG_DUMP:
            {               
                return_result = host_get_dsp_debug_dump(phy_info_ptr, 
                                                        &diag_cmd_req_info_ptr->type.ddump);
            }
            break;
        case COMMAND_ID_DIAG_SET_HW_GAIN2_ADAPT_CONFIG:
            return_result = dsp_hw_gain2_adapt_cfg_sanity_checker(phy_info_ptr, &diag_cmd_req_info_ptr->type.hw_gain2_a_cfg, CHK_USR_INPUT);
            if(return_result!=RR_SUCCESS)
                return return_result;
            return_result = host_set_hw_gain2_a_config(phy_info_ptr, &diag_cmd_req_info_ptr->type.hw_gain2_a_cfg);
        break;
        
        case COMMAND_ID_DIAG_GET_HW_GAIN2_ADAPT_CONFIG:
            return_result = host_get_hw_gain2_a_config(phy_info_ptr, &diag_cmd_req_info_ptr->type.hw_gain2_a_cfg);
            if(return_result!=RR_SUCCESS)
                return return_result;
            return_result = dsp_hw_gain2_adapt_cfg_sanity_checker(phy_info_ptr, &diag_cmd_req_info_ptr->type.hw_gain2_a_cfg, CHK_FW_OUTPUT);
        break;

        
        case COMMAND_ID_SET_GAIN3_A_PARAM:
            return_result = dsp_gain3_a_param_cfg_sanity_checker(phy_info_ptr, &diag_cmd_req_info_ptr->type.gain3_a_cfg, CHK_USR_INPUT);
            if(return_result!=RR_SUCCESS)
                return return_result;
            return_result = host_set_gain3_a_param_config(phy_info_ptr, &diag_cmd_req_info_ptr->type.gain3_a_cfg);
        break;
        
        case COMMAND_ID_GET_GAIN3_A_PARAM:
            return_result = host_get_gain3_a_param_config(phy_info_ptr, &diag_cmd_req_info_ptr->type.gain3_a_cfg);
            if(return_result!=RR_SUCCESS)
                return return_result;
            return_result = dsp_gain3_a_param_cfg_sanity_checker(phy_info_ptr, &diag_cmd_req_info_ptr->type.gain3_a_cfg, CHK_FW_OUTPUT);
        break;

        case COMMAND_ID_DIAG_DISPLAY_FW_EVENT_LOG:
            return_result = host_dump_fw_events(phy_info_ptr, stdout);
        break;

        case COMMAND_ID_GET_LW_RX_DIAG_INFO:
            // get DSP Rx status: cpga, clipping, tap-saturation register reads, static phase bias value, current phase bias
            return_result = host_dump_dsp_rx_diag_status(phy_info_ptr, &diag_cmd_req_info_ptr->type.rx_diag_status);
        break;        

        default:
            return_result = RR_ERROR_WRONG_INPUT_VALUE;
            break;
    }

    return return_result; 
}

/**
* @brief       capi_get_diag_dump(capi_phy_info_t* phy_info_ptr, capi_diag_dump_t* diag_dump_ptr)
* @details    This API is used to get DSP diagnostics data from FW memory via burst read.
*
* @param[in]  phy_info_ptr : a reference to the phy information object
* @param[in]   diag_dump_ptr  : a reference to the diag dump object
*
* @return     returns the performance result of the called method/function
*/
return_result_t capi_get_diag_dump(capi_phy_info_t* phy_info_ptr, capi_diag_dump_t* diag_dump_ptr)
{
    return_result_t         return_result;
    capi_phy_info_t         phy_info = *phy_info_ptr;
    capi_diag_command_request_info_t req_info = {0};

    // start DSP diag dump
    req_info.command_id = COMMAND_ID_START_DIAG_DUMP;
    req_info.type.dsp_diag_dump = *diag_dump_ptr;
    return_result = capi_diag_command_request_info(&phy_info, &req_info);
    if (return_result != RR_SUCCESS)
        return return_result;

    // copy DSP diag dump data to local buffer
    req_info.command_id = COMMAND_ID_GET_DIAG_DUMP;
    return capi_diag_command_request_info(&phy_info, &req_info);
}

return_result_t capi_fec_start_stop_auto_ports_detect(capi_phy_info_t* phy_info_ptr, capi_fec_auto_ports_detect_t* capi_fec_auto_ports_detect_ptr)
{
    capi_command_info_t command_info;
    capi_phy_info_t phy_info;
    uint16_t stop_reg;

    CAPI_LOG_FUNC("capi_fec_initiate_auto_ports_detect", phy_info_ptr);

    if (phy_info_ptr->lane_mask == 0) {
        CAPI_LOG_ERROR("Invalid Lane Mask. %d \n", phy_info_ptr->lane_mask);
        return RR_ERROR_INITIALIZATION;
    }
            
    util_memcpy((void *)&phy_info, phy_info_ptr, sizeof(capi_phy_info_t));
    if(capi_fec_auto_ports_detect_ptr->stop){
        phy_info.base_addr = OCTAL_TOP_REGS;
        stop_reg = (uint16_t)hsip_rd_reg_(&phy_info, QUAD_CORE_CONFIG_CFGVAL_60_RDB);
        stop_reg |= 0x8000;
        hsip_wr_reg_(&phy_info, QUAD_CORE_CONFIG_CFGVAL_60_RDB, stop_reg);
        return RR_SUCCESS;
    }else {
        cw_fec_mon_discover_ports_reg_t ports_list;
        phy_info.base_addr = OCTAL_TOP_REGS;
        ports_list.words = (uint16_t)hsip_rd_reg_(&phy_info, QUAD_CORE_CONFIG_CFGVAL_60_RDB);
        if(capi_fec_auto_ports_detect_ptr->fec_mon_valid_discover_ports_ordered_list[0] == CAPI_FEC_MON_PORT_NULL){/* Default */
            ports_list.fields.port0 = CAPI_FEC_MON_PORT_100G;
            ports_list.fields.port1 = CAPI_FEC_MON_PORT_400G;
            ports_list.fields.port2 = CAPI_FEC_MON_PORT_200G;
        }else{
            ports_list.fields.port0 = capi_fec_auto_ports_detect_ptr->fec_mon_valid_discover_ports_ordered_list[0];
            ports_list.fields.port1 = capi_fec_auto_ports_detect_ptr->fec_mon_valid_discover_ports_ordered_list[1];
            ports_list.fields.port2 = capi_fec_auto_ports_detect_ptr->fec_mon_valid_discover_ports_ordered_list[2];
        }
        ports_list.words &=0x7FFF;
        hsip_wr_reg_(&phy_info, QUAD_CORE_CONFIG_CFGVAL_60_RDB, ports_list.words);
        phy_info.core_ip = CORE_IP_CW;
        command_info.command_id = COMMAND_ID_SET_CW_RPTR_FEC_MON_CONFIG;
        command_info.type.rptr_fec_st.sampling_rate = capi_fec_auto_ports_detect_ptr->sampling_rate;
        /* default value 0 is 100% sampling for backward compatible */
        if (command_info.type.rptr_fec_st.sampling_rate == 0 || command_info.type.rptr_fec_st.sampling_rate > 100)
            command_info.type.rptr_fec_st.sampling_rate = 100;
        command_info.type.rptr_fec_st.is_enable = 1;
        command_info.type.rptr_fec_st.fec_monitor_port = 1;
        command_info.type.rptr_fec_st.fec_monitor_discover = 1;
        /* just to get past the checker */
        command_info.type.rptr_fec_st.fec_mode = CAPI_FEC_CLIENT;
        return(capi_command_request(&phy_info, &command_info, sizeof(capi_rptr_fec_st_t), SET_CONFIG));
    }        
}



/**
* @brief      capi_get_state_info(capi_phy_info_t* phy_info_ptr, capi_get_state_info_t* get_state_info_ptr)
* @details    This API is used to get DSP diagnostics info
*
* @param[in]  phy_info_ptr : a reference to the phy information object
* @param[in]   get_state_info_ptr  : a reference to get the state info
*
* @return     returns the performance result of the called method/function
*/
return_result_t capi_get_state_info(capi_phy_info_t* phy_info_ptr, capi_state_info_t* state_info_ptr)
{
    return_result_t return_result = RR_ERROR_WRONG_INPUT_VALUE;

    if (state_info_ptr->type == CAPI_STATE_TYPE_FEC_AUTO_PORT_DETECT_STATE) {
        return host_diag_cw_fec_ports_auto_detect_status (phy_info_ptr, &state_info_ptr->state.fec_auto_ports_detect);
    }
    
    return return_result;
}


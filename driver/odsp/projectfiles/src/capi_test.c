/**
 *
 * @file     capi_test.c
 * @author   
 * @date     9/15/2018
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
#include "access.h"
#include "common_def.h"
#include "chip_mode_def.h"
#include "chip_config_def.h"
#include "capi_def.h"
#include "capi_test_def.h"
#include "capi_custom_def.h"
#include "host_diag_util.h"
#include "host_log_util.h"
#include "capi.h"
#include "host_test.h"
#include "host_fec_prbs.h"
#include "capi_test.h"

#include "host_lw_wrapper.h"


/**
* @brief      capi_set_prbs_info(capi_phy_info_t* phy_info_ptr, capi_prbs_info_t* prbs_info_ptr)
* @details    This API is used to configure the PRBS pattern generator and enable or disable it
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  prbs_info_ptr: this pointer contain PRBS config info
* 
* @return     returns the performance result of the called methode/function
*/

return_result_t capi_set_prbs_info(capi_phy_info_t* phy_info_ptr, capi_prbs_info_t* prbs_info_ptr)
{
    capi_command_info_t command_info;
    return_result_t ret_result;

    if(prbs_info_ptr->ptype == CAPI_PRBS_KP4_HOST_GENERATOR ||
        prbs_info_ptr->ptype == CAPI_PRBS_KP4_MEDIA_GENERATOR){
        return ( host_fec_set_prbs_gen(phy_info_ptr, prbs_info_ptr));
    }else if(prbs_info_ptr->ptype == CAPI_PRBS_KP4_HOST_MONITOR ||
        prbs_info_ptr->ptype == CAPI_PRBS_KP4_MEDIA_MONITOR){
        return ( host_fec_set_prbs_mon(phy_info_ptr, prbs_info_ptr));
    }

#ifdef ENABLE_CW_PRBS
    if(prbs_info_ptr->ptype == CAPI_PRBS_CW_GENERATOR)
    { return (host_diag_cw_prbs_gen_cfg(phy_info_ptr, prbs_info_ptr)); }
    else if(prbs_info_ptr->ptype == CAPI_PRBS_CW_MONITOR)
    { return (host_diag_cw_prbs_mon_cfg(phy_info_ptr, prbs_info_ptr)); }
#endif

#ifdef ENABLE_PCS_SCRM_IDLE_GEN
    if (prbs_info_ptr->ptype == CAPI_PCS_SCRM_IDLE_GENERATOR) {
        CAPI_LOG_PRBS_INFO(prbs_info_ptr);
        return host_diag_pcs_scrm_idle_generator(phy_info_ptr, prbs_info_ptr);
    }
#endif

    command_info.command_id = COMMAND_ID_SET_PRBS_INFO;
    command_info.type.prbs_info = *prbs_info_ptr;

    if (command_info.type.prbs_info.ptype == CAPI_PRBS_GEN_MON)
    {
        /* Send GEN command first */
        command_info.type.prbs_info.ptype = CAPI_PRBS_GENERATOR;
        ret_result = capi_command_request(phy_info_ptr, &command_info, sizeof(capi_prbs_info_t), SET_CONFIG);
        if (ret_result == RR_SUCCESS) {
            /* Send MON command next */
            command_info.type.prbs_info.ptype = CAPI_PRBS_MONITOR;
            ret_result = capi_command_request(phy_info_ptr, &command_info, sizeof(capi_prbs_info_t), SET_CONFIG);
        }
    } else if (command_info.type.prbs_info.ptype == CAPI_PRBS_SSPRQ_GEN_MON) {
        /* Send GEN command first */
        command_info.type.prbs_info.ptype = CAPI_PRBS_SSPRQ_GENERATOR;
        ret_result = capi_command_request(phy_info_ptr, &command_info, sizeof(capi_prbs_info_t), SET_CONFIG);
        if (ret_result == RR_SUCCESS) {
            /* Send MON command next */
            command_info.type.prbs_info.ptype = CAPI_PRBS_SSPRQ_MONITOR;
            ret_result = capi_command_request(phy_info_ptr, &command_info, sizeof(capi_prbs_info_t), SET_CONFIG);
        }
    } else {
        ret_result = capi_command_request(phy_info_ptr, &command_info, sizeof(capi_prbs_info_t), SET_CONFIG);
    }
    return ret_result;
}

/**
* @brief      capi_get_prbs_info(capi_phy_info_t* phy_info_ptr, capi_prbs_info_t* prbs_info_ptr)
* @details    This API is to the the configuration setting of othe PRBS pattern generator
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[out] prbs_info_ptr: this pointer return PRBS config information
* 
* @return     returns the performance result of the called methode/function
*/
return_result_t capi_get_prbs_info(capi_phy_info_t* phy_info_ptr, capi_prbs_info_t* prbs_info_ptr)
{
    capi_command_info_t command_info;
    return_result_t     return_result = RR_ERROR_WRONG_INPUT_VALUE;

    if(prbs_info_ptr->ptype == CAPI_PRBS_KP4_HOST_GENERATOR ||
        prbs_info_ptr->ptype == CAPI_PRBS_KP4_MEDIA_GENERATOR){
        return ( host_fec_get_prbs_gen(phy_info_ptr, prbs_info_ptr));
    }else if(prbs_info_ptr->ptype == CAPI_PRBS_KP4_HOST_MONITOR ||
        prbs_info_ptr->ptype == CAPI_PRBS_KP4_MEDIA_MONITOR){
        return ( host_fec_get_prbs_mon(phy_info_ptr, prbs_info_ptr));
    }

    command_info.command_id = COMMAND_ID_GET_PRBS_INFO;
    command_info.type.prbs_info = *prbs_info_ptr;


    return_result = capi_command_request(phy_info_ptr, &command_info, sizeof(capi_prbs_info_t), GET_CONFIG);

    *prbs_info_ptr = command_info.type.prbs_info;

    if (return_result == RR_SUCCESS) {
    }
    return(return_result);
}

/**
* @brief      capi_get_prbs_status(capi_phy_info_t*    phy_info_ptr,
*                                 capi_prbs_status_t* prbs_st_ptr)
* @details    This API is used to get the PRBS  status
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[out] prbs_st_ptr: this pointer return PRBS status information
* 
* @return     returns the performance result of the called methode/function
*/
return_result_t capi_get_prbs_status(capi_phy_info_t*    phy_info_ptr,
                                     capi_prbs_status_t* prbs_st_ptr)
{
    return_result_t return_result;
    capi_command_info_t command_info;

    CAPI_LOG_FUNC("capi_get_prbs_status", phy_info_ptr);

    if( phy_info_ptr->core_ip == CORE_IP_KP4_KR4_FEC_DEC)
       return(host_fec_get_prbs_status(phy_info_ptr, prbs_st_ptr));

    command_info.command_id = COMMAND_ID_GET_PRBS_STATUS;
    command_info.type.prbs_status_info = *prbs_st_ptr;


    return_result = capi_command_request(phy_info_ptr, &command_info, sizeof(capi_prbs_status_t), GET_CONFIG);
    *prbs_st_ptr = command_info.type.prbs_status_info;
    if (return_result == RR_SUCCESS) {
    }
    return(return_result);
}

/**
* @brief      capi_clear_prbs_status(capi_phy_info_t* phy_info_ptr, capi_prbs_status_t* prbs_st_ptr)
* @details    This API is used to clear the PRBS  status
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  prbs_st_ptr: this pointer return PRBS status information
* 
* @return     returns the performance result of the called methode/function
*/
return_result_t capi_clear_prbs_status(capi_phy_info_t* phy_info_ptr, capi_prbs_status_t* prbs_st_ptr)
{
    capi_command_info_t command_info;

    CAPI_LOG_FUNC("capi_clear_prbs_status", phy_info_ptr);
    CAPI_LOG_INFO("ptype : %s\n", util_capi_prbs_type_str(prbs_st_ptr->prbs_type));
    if( phy_info_ptr->core_ip == CORE_IP_KP4_KR4_FEC_DEC)
       return host_fec_clear_prbs_status(phy_info_ptr, prbs_st_ptr->prbs_type);

    command_info.command_id = COMMAND_ID_CLEAR_PRBS_STATUS;
    command_info.type.prbs_status_info = *prbs_st_ptr;

    return (capi_command_request(phy_info_ptr, &command_info, sizeof(capi_prbs_status_t), SET_CONFIG));
}

/**
* @brief      capi_set_loopback(capi_phy_info_t* phy_info_ptr, capi_loopback_t* loopback_ptr)
* @details    This API is used to set the loopback mode
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  loopback_ptr: this pointer carries the necessary information to set the loopback mode
* 
* @return     returns the performance result of the called method/function
*/
return_result_t capi_set_loopback(capi_phy_info_t* phy_info_ptr, capi_loopback_info_t* loopback_ptr)
{
    capi_command_info_t command_info;

    command_info.command_id = COMMAND_ID_SET_LOOPBACK_INFO;
    command_info.type.lpbk_ctrl_info = *loopback_ptr;


    return(capi_command_request(phy_info_ptr, &command_info, sizeof(capi_loopback_info_t), SET_CONFIG));
}

/**
 * @brief      capi_get_loopback(capi_phy_info_t* phy_info_ptr, capi_loopback_t* loopback_ptr)
 * @details    This API is used to get the loopback mode <BR>
 *             Note: This function support either Line Side or Client side <BR>
 *                   This function only support single-lane operation. <BR>
 *                   If multi-lane mask is passed in, then first valid lane status will be returned. <BR>
 *             when core_ip is CORE_IP_LW, the "mode" value is: <BR>
 *             CAPI_GLOBAL_LOOPBACK_MODE: digital global loopback from Line side transmitter to receiver <BR>
 *             CAPI_REMOTE_LOOPBACK_MODE: digital remote loopback from line side receiver to transmitter <BR>
 *             when core_ip is CORE_IP_CLIENT, the "mode" value is: <BR>
 *             CAPI_GLOBAL_LOOPBACK_MODE: digital global loopback from System side transmitter to receiver <BR>
 *             CAPI_REMOTE_LOOPBACK_MODE: digital remote loopback from System side receiver to transmitter <BR>
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[out] loopback_ptr: this pointer return loopback mode
 * 
 * @return     returns the performance result of the called method/function
 */
return_result_t capi_get_loopback(capi_phy_info_t* phy_info_ptr, capi_loopback_info_t* loopback_ptr)
{
    return_result_t return_result;
    capi_command_info_t command_info;

    command_info.command_id = COMMAND_ID_GET_LOOPBACK_INFO;
    command_info.type.lpbk_ctrl_info = *loopback_ptr;


    return_result = capi_command_request(phy_info_ptr, &command_info, sizeof(capi_loopback_info_t), GET_CONFIG);
    *loopback_ptr = command_info.type.lpbk_ctrl_info;
    if (return_result == RR_SUCCESS) {
    }
    return(return_result);
}

/**
* @brief      capi_prbs_inject_error(capi_phy_info_t* phy_info_ptr, capi_prbs_err_inject_t* prbs_err_inj_ptr)
* @details    This API is used to clear the PRBS  status
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  ptype: this parameter indicate the PRBS type
* 
* @return     returns the performance result of the called methode/function
*/
return_result_t capi_prbs_inject_error(capi_phy_info_t* phy_info_ptr, capi_prbs_err_inject_t* prbs_err_inj_ptr)
{
    capi_command_info_t command_info;

    command_info.command_id = COMMAND_ID_INJ_PRBS_ERROR;
    command_info.type.prbs_err_inj_info = *prbs_err_inj_ptr;


    return(capi_command_request(phy_info_ptr, &command_info, sizeof(capi_prbs_err_inject_t), SET_CONFIG));
}


/**
 * @brief      capi_test_set_command(capi_phy_info_t*          phy_info_ptr,
 *                                   capi_test_command_info_t* test_cmd_inf_ptr)
 * @details    This API is used to invoke test command.
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  capi_test_command_info_t: a reference to the test command object 
 * @return     returns the performance result of the called method/function
 */
return_result_t capi_test_set_command(capi_phy_info_t*          phy_info_ptr,
                                      capi_test_command_info_t* test_cmd_inf_ptr)
{
    uint16_t            payload_size;
    capi_command_info_t command_info;
    return_result_t     return_result = RR_ERROR_WRONG_INPUT_VALUE;

    command_info.command_id = test_cmd_inf_ptr->command_id;

    switch(test_cmd_inf_ptr->command_id) {
        case COMMAND_ID_SET_RECOVERED_CLOCK_INFO:
            if (phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP){
                command_info.command_id                = COMMAND_ID_SET_RECOVERED_CLOCK_INFO;
                if(test_cmd_inf_ptr->type.recovered_clock_info.command.is.config_analog_pin_0){
                    test_cmd_inf_ptr->type.recovered_clock_info.value.config_analog_pin[0].lane_mask = 0x80;
                    phy_info_ptr->lane_mask = 0x80;
                }else if(test_cmd_inf_ptr->type.recovered_clock_info.command.is.config_vco_pin){
                    phy_info_ptr->lane_mask = 0x80;
                }
                command_info.type.recovered_clock_info = test_cmd_inf_ptr->type.recovered_clock_info;
                payload_size = sizeof(capi_recovered_clock_info_t);
                return(capi_command_request(phy_info_ptr, &command_info, payload_size, SET_CONFIG));
            } else
                return(RR_ERROR_WRONG_INPUT_VALUE);

        case COMMAND_ID_GET_RECOVERED_CLOCK_INFO:
            if (phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP){
                command_info.command_id                = COMMAND_ID_GET_RECOVERED_CLOCK_INFO;
                command_info.type.recovered_clock_info = test_cmd_inf_ptr->type.recovered_clock_info;
                if(test_cmd_inf_ptr->type.recovered_clock_info.command.is.config_analog_pin_0 ||
                    test_cmd_inf_ptr->type.recovered_clock_info.command.is.config_vco_pin){
                    phy_info_ptr->lane_mask = 0x80;
                }
                payload_size = sizeof(capi_recovered_clock_info_t);
                return_result  = capi_command_request(phy_info_ptr, &command_info, payload_size, GET_CONFIG);
                test_cmd_inf_ptr->type.recovered_clock_info = command_info.type.recovered_clock_info;
                return return_result;
            }else
                return(RR_ERROR_WRONG_INPUT_VALUE);
            break;

        case COMMAND_ID_SET_DSP_OPT_LOS_HOST_TX_UNSQUELCH:
            command_info.command_id = COMMAND_ID_SET_DSP_OPT_LOS_HOST_TX_UNSQUELCH;
            command_info.type.dsp_opt_los_host_tx_unsquelch = test_cmd_inf_ptr->type.dsp_opt_los_host_tx_unsquelch;
            //CMD_SANITY_CHECK(phy_info_ptr, &command_info, CHK_USR_INPUT);
            phy_info_ptr->core_ip = CORE_IP_HOST_SERDES;
            payload_size = sizeof(uint8_t);
            return(capi_command_request(phy_info_ptr, &command_info, payload_size, SET_CONFIG));

        case COMMAND_ID_SET_TXPI_OVERRIDE:
            command_info.type.txpi_ovrd = test_cmd_inf_ptr->type.txpi_ovrd;
            payload_size = sizeof(txpi_override_t);
            return(capi_command_request(phy_info_ptr, &command_info, payload_size, SET_CONFIG));

        case COMMAND_ID_SET_LW_SNR_TH_CFG:
            return_result = host_lw_set_snr_threshold(phy_info_ptr, &test_cmd_inf_ptr->type.lw_snr_info);
            break;           
        case COMMAND_ID_GET_LW_SNR_TH_CFG:
            return_result = host_lw_get_snr_threshold(phy_info_ptr, &test_cmd_inf_ptr->type.lw_snr_info);
            break;
        case COMMAND_ID_SET_DSP_PLL_FRACN_ADJUST_ON_THE_FLY:
            return_result = host_dsp_set_pll_fracn_dynamic_adjust(phy_info_ptr, &test_cmd_inf_ptr->type.dsp_dpll_cfg);
            break;
        case COMMAND_ID_GET_DSP_PLL_FRACN_ADJUST_ON_THE_FLY:
            return_result = host_dsp_get_pll_fracn_dynamic_adjust(phy_info_ptr, &test_cmd_inf_ptr->type.dsp_dpll_cfg);
            break;
        case COMMAND_ID_SET_LANE_SR2:
            command_info.command_id = COMMAND_ID_SET_LANE_SR2;
            command_info.type.dsp_lane_sr2 = test_cmd_inf_ptr->type.dsp_lane_sr2;
            payload_size = sizeof(uint8_t);
            return(capi_command_request(phy_info_ptr, &command_info, payload_size, SET_CONFIG));
            break;


        default:
            return(RR_ERROR_WRONG_INPUT_VALUE);
    }
    return return_result;
}


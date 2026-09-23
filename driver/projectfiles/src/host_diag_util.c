/**
 *
 * @file    host_diag_util.c
 * @author  Firmware Team
 * @date    03/01/2019
 * @version 0.2
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
#include <stdio.h>
#include <stdlib.h>
#include "access.h"
#include "hr_time.h"
#include "fw_gp_reg_map.h"
#include "regs_common.h"
#include "common_def.h"
#include "chip_mode_def.h"
#include "chip_config_def.h"
#include "capi_def.h"
#include "capi_test_def.h"
#include "host_log_util.h"
#include "host_chip_wrapper.h"
#include "chip_mode_def.h"
#include "common_util.h"
#include "hw_mutex_handler.h"
#include "chip_common_config_ind.h"
#include "host_to_chip_ipc.h"
#include "dsp_utils.h"
#include "dsp_config.h"
#include "host_diag_util.h"


extern const capi_port_entry_t ports[];
extern uint8_t port_total_entries;
extern const cw_chip_port_info_t cw_cmode[];

/**
 * @brief      util_check_param_range(int source, int lower_limit, int upper_limit)
 * @details    This API is used to check if a parameter is inside lower and upper limit
 *
 * @param[in]  source: parameter to be checked
 * @param[in]  lower_limit: lower boundary for source
 * @param[in]  upper_limit: upper boundary for source
 *
 * @return     TRUE: source is in lower/upper limit, FALSE: source is outside lower/upper limit
 */

bool util_check_param_range(int source, int lower_limit, int upper_limit) {
    if (source < lower_limit) { /* Check if source is less than the lower limit */
        return FALSE; /* Return false */
    }
    if (source > upper_limit) { /* Check if source is more than the upper limit */
        return FALSE; /* Return false */
    }
    return TRUE; /* Otherwise return true */
}

/**
 * @brief      util_lw_tx_fir_lut_validate_coef_and_lvl(phy_info_t* phy_info_ptr, int16_t *coef, int8_t *lvl)
 * @details    This API is used to check if txfir level shift is valid
 *
 * @param[in]  phy_info_ptr : phy info
 * @param[in]  coef         : Coeff.
 * @param[in]  lvl          : Tap level
 *
 * @return     RR_SUCCESS: Valid, RR_WARNING_BOUNDS: Out of range
 */
return_result_t util_lw_tx_fir_lut_validate_coef_and_lvl (phy_info_t* phy_info_ptr, int16_t *coef, int8_t *lvl)

{
    (void)phy_info_ptr;

    uint8_t s0, s1, s2, s3;
    int8_t symbol[4];
    float tap_lvl[4];
    return_result_t return_result = RR_SUCCESS;
    float sum;
    int16_t int_sum;


    symbol[3] = 3;
    symbol[2] = 1;
    symbol[1] = -1;
    symbol[0] = -3;

    tap_lvl[0] = (float)lvl[0] / 21.0f;
    tap_lvl[1] = (float)lvl[1] / 21.0f;
    tap_lvl[2] = (float)lvl[2] / 21.0f;
    tap_lvl[3] = (float)lvl[3] / 21.0f;

    for ( s3 = 0; s3 < 4; s3++ ) {
        for ( s2 = 0; s2 < 4; s2++ ) {
            for ( s1 = 0; s1 < 4; s1++ ) {
                for ( s0 = 0; s0 < 4; s0++ ) {

                    sum =
                        (float)coef[3]/8 * ( symbol[s3] + tap_lvl[s3] ) +
                        (float)coef[2]/8 * ( symbol[s2] + tap_lvl[s2] ) +
                        (float)coef[1]/8 * ( symbol[s1] + tap_lvl[s1] ) +
                        (float)coef[0]/8 * ( symbol[s0] + tap_lvl[s0] ) ;

                    /* rounding */
                    int_sum = sum >= 0 ? (int16_t) (sum + 0.5) : (int16_t) (sum - 0.5);
                    /* DAC value range is [0, 127] */
                    if ( int_sum > 63 || int_sum < -64)
                    {
                        return_result = RR_WARNING_BOUNDS;
                    }
                }
            }
        }
    }

    return return_result;

}

/**
 * @brief      sanity_checker_lane_rx_info(capi_phy_info_t* phy_info_ptr,
 *                                         lane_rx_info_t*  lane_rx_info_ptr,
 *                                         uint8_t          set_get_request,
 *                                         sanity_chk_t     chk_type)
 * @details    This API is used to sanity check lane config params
 *
 * @param[in]  phy_info_ptr: reference to the phy information object
 * @param[in]  lane_rx_info_ptr: reference to the lane rx information object
 * @param[in]  set_get_request: get or set request
 * @param[in]  chk_type: sanity cheker type
 *
 * @return     returns the performance result of the called method/function
 */
static return_result_t sanity_checker_lane_rx_info(capi_phy_info_t* phy_info_ptr,
                                                   lane_rx_info_t*  lane_rx_info_ptr,
                                                   uint8_t          set_get_request,
                                                   sanity_chk_t     chk_type)
{
    return_result_t return_result = RR_SUCCESS;

    if (!lane_rx_info_ptr->param.content) return(RR_ERROR_NOT_INITIALIZED);

    if ((phy_info_ptr->core_ip == CORE_IP_ALL) ||
        (phy_info_ptr->core_ip == CORE_IP_CW)  ||
        (phy_info_ptr->core_ip == CORE_IP_KP4_KR4_FEC_DEC)) {
        return(RR_ERROR_WRONG_CORE_IP_VALUE);
    }

    if ((phy_info_ptr->core_ip == CORE_IP_HOST_DSP) ||
        (phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP)) {
        if (lane_rx_info_ptr->param.is.serdes_lp_has_prec_en ||
            lane_rx_info_ptr->param.is.serdes_unreliable_los ||
            lane_rx_info_ptr->param.is.serdes_scrambling_off ||               
            lane_rx_info_ptr->param.is.serdes_db_loss        ||
            lane_rx_info_ptr->param.is.serdes_pam_er_nr) {
            return(RR_ERROR_WRONG_CORE_IP_VALUE);
        }
    }
    if ((phy_info_ptr->core_ip == CORE_IP_HOST_SERDES) ||
        (phy_info_ptr->core_ip == CORE_IP_MEDIA_SERDES)) {
        if (lane_rx_info_ptr->param.is.dsp_nldet_info                  ||
            lane_rx_info_ptr->param.is.dsp_los_info                    ||
            lane_rx_info_ptr->param.is.dsp_ffe_slicer_multiplier       ||
            lane_rx_info_ptr->param.is.dsp_eq_tap_sel                  ||
            lane_rx_info_ptr->param.is.dsp_kp_kf_info                  ||
            lane_rx_info_ptr->param.is.dsp_phase_bias_auto_tuning_info ||
            lane_rx_info_ptr->param.is.dsp_oplos_ignore                ||
            lane_rx_info_ptr->param.is.dsp_elos_ignore                 ||
            lane_rx_info_ptr->param.is.dsp_outlos_th_idx               ||
            lane_rx_info_ptr->param.is.dsp_inlos_th_idx                ||
            lane_rx_info_ptr->param.is.dsp_los_th_idx                  ||
            lane_rx_info_ptr->param.is.dsp_precode                    ||
            lane_rx_info_ptr->param.is.dsp_autopeaking_en              ||
            lane_rx_info_ptr->param.is.dsp_lms_mode                    ||
            lane_rx_info_ptr->param.is.dsp_gain2_on                    ||
            lane_rx_info_ptr->param.is.dsp_gain3_on                    ||
            lane_rx_info_ptr->param.is.dsp_rx_bw_value                 ||
            lane_rx_info_ptr->param.is.dsp_gain_boost                  ||
            lane_rx_info_ptr->param.is.dsp_dc_wander_mu                ||
            lane_rx_info_ptr->param.is.dsp_graycode ||
            lane_rx_info_ptr->param.is.dsp_eq_hpwr_o ) {
            return(RR_ERROR_WRONG_CORE_IP_VALUE);
        }
    }
    
    if ((lane_rx_info_ptr->param.is.dsp_autopeaking_en) && (phy_info_ptr->core_ip != CORE_IP_HOST_DSP)) {
        CAPI_LOG_ERROR("dsp_autopeaking_en not supported\r\n");
        return(RR_ERROR_WRONG_CORE_IP_VALUE);
    }
    if ((lane_rx_info_ptr->param.is.dsp_gain2_on || lane_rx_info_ptr->param.is.dsp_gain3_on) && (phy_info_ptr->core_ip != CORE_IP_MEDIA_DSP)){
        CAPI_LOG_ERROR("dsp_gain2_on /  dsp_gain3_on not supported\r\n");
        return(RR_ERROR_WRONG_CORE_IP_VALUE);
    }
    
    if ((lane_rx_info_ptr->param.is.dsp_gain2_on && lane_rx_info_ptr->param.is.dsp_gain3_on) && (phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP)){
        if((lane_rx_info_ptr->value.core_ip.dsp.gain2_on == 0  || lane_rx_info_ptr->value.core_ip.dsp.gain2_on==2) && lane_rx_info_ptr->value.core_ip.dsp.gain3_on){
            CAPI_LOG_ERROR("when dsp_gain2_on is 0 or 2,  dsp_gain3_on failed to set to 1\r\n");
            return(RR_ERROR_FEATURE_NOT_SUPPORTED);
        }
    }
    if ((lane_rx_info_ptr->param.is.eq2_info) && (phy_info_ptr->core_ip != CORE_IP_HOST_DSP && phy_info_ptr->core_ip != CORE_IP_MEDIA_DSP )) {
        CAPI_LOG_ERROR("eq2_info not supported\r\n");
        return(RR_ERROR_WRONG_CORE_IP_VALUE);
    }
    if (lane_rx_info_ptr->param.is.dsp_nldet_info) {
        CAPI_LOG_ERROR("dsp_nldet_info not supported\r\n");
        return(RR_ERROR_FEATURE_NOT_SUPPORTED);
    }
    if (lane_rx_info_ptr->param.is.dsp_lms_mode) {
        CAPI_LOG_ERROR("dsp_lms_mode not supported\r\n");
        return(RR_ERROR_FEATURE_NOT_SUPPORTED);
    }
    if (lane_rx_info_ptr->param.is.dsp_los_info) {
        CAPI_LOG_ERROR("dsp_los_info not supported\r\n");
        return(RR_ERROR_FEATURE_NOT_SUPPORTED);
    }

    if ((CHK_USR_INPUT == chk_type) && (set_get_request == GET_CONFIG)) {
        /*  */
    } else {

        if ((CHK_USR_INPUT == chk_type) && ((lane_rx_info_ptr->param.is.serdes_unreliable_los) ||
            (lane_rx_info_ptr->param.is.serdes_scrambling_off))) {
            if (set_get_request == SET_CONFIG) {
               if (lane_rx_info_ptr->param.is.serdes_unreliable_los) {
                    CAPI_LOG_ERROR("Read Only parameter serdes_unreliable_los : %d  \n ",
                                   lane_rx_info_ptr->param.is.serdes_unreliable_los);
                    return_result = RR_ERROR_WRONG_INPUT_VALUE;
                } 
                if (lane_rx_info_ptr->param.is.serdes_scrambling_off){
                    CAPI_LOG_ERROR("Read Only parameter serdes_scrambling_off : %d  \n ",
                                   lane_rx_info_ptr->param.is.serdes_scrambling_off);
                    return_result = RR_ERROR_WRONG_INPUT_VALUE;
                }
            }
        }

    if (lane_rx_info_ptr->param.is.vga) {
        if (phy_info_ptr->core_ip == CORE_IP_HOST_DSP  ||
            phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP) {
            if(!util_check_param_range(lane_rx_info_ptr->value.vga, 0, 31)) {
                CAPI_LOG_ERROR("DSP wrong vga value : %d \r\n", lane_rx_info_ptr->value.vga);
                return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
            }
        } else {
            if(!util_check_param_range(lane_rx_info_ptr->value.vga, 0, 37) || (lane_rx_info_ptr->value.vga > 37) ||
                (set_get_request == SET_CONFIG)) {
                CAPI_LOG_ERROR("SERDES wrong vga value: %d   set_get_request: %d \r\n",
                               lane_rx_info_ptr->value.vga, set_get_request);
                return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
            }
        }
    }        
    
    if (lane_rx_info_ptr->param.is.symbol_swap) {
        if(!util_check_param_range(lane_rx_info_ptr->value.symbol_swap, 0, 1)) {
            CAPI_LOG_ERROR("Wrong symbol_swap : %d \n ", lane_rx_info_ptr->value.symbol_swap);
            return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
        }
    }
    
    if (lane_rx_info_ptr->param.is.media_type) {
        if(!util_check_param_range(lane_rx_info_ptr->value.media_type, CAPI_MEDIA_TYPE_OPTICAL, CAPI_MEDIA_TYPE_COPPER_CABLE)) {
            CAPI_LOG_ERROR("Wrong media_type : %d \n ", lane_rx_info_ptr->value.media_type);
            return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
        }
    }
    
    if (lane_rx_info_ptr->param.is.peaking_filter_info) {
        if ((phy_info_ptr->core_ip == CORE_IP_HOST_SERDES) ||
            (phy_info_ptr->core_ip == CORE_IP_MEDIA_SERDES)) {
            if (set_get_request == SET_CONFIG) {
                CAPI_LOG_ERROR("Set peaking_filter_info isn't allowed for Serdes : %d \r\n ",
                               lane_rx_info_ptr->param.is.peaking_filter_info);
                return_result = RR_ERROR_WRONG_INPUT_VALUE;
            }
        }
        if(!util_check_param_range(lane_rx_info_ptr->value.peaking_filter_info.oven, 0, 1) ||
            !util_check_param_range(lane_rx_info_ptr->value.peaking_filter_info.oval, 0, 31)) {
            CAPI_LOG_ERROR("Wrong Peaking Filter ovrd: %d  oval :  %d \r\n",
                           lane_rx_info_ptr->value.peaking_filter_info.oven, lane_rx_info_ptr->value.peaking_filter_info.oval);
            return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
        }
    }

    if (lane_rx_info_ptr->param.is.eq2_info) {
        if (!util_check_param_range(lane_rx_info_ptr->value.eq2_info.dsp_eq2_adapt_off, 0, 1) ||
            !util_check_param_range(lane_rx_info_ptr->value.eq2_info.dsp_force_baud_rate_eq2, 0, 1)) {
            CAPI_LOG_ERROR("Wrong parameter value dsp_eq2_adapt_off: %d,  dsp_force_baud_rate_eq2 : %d\n ",
                           lane_rx_info_ptr->value.eq2_info.dsp_eq2_adapt_off,
                           lane_rx_info_ptr->value.eq2_info.dsp_force_baud_rate_eq2);
            return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
        }        
        if( !util_check_param_range(lane_rx_info_ptr->value.eq2_info.eq2_on, 0, 1) ||
            !util_check_param_range(lane_rx_info_ptr->value.eq2_info.low_power_eq2_on, 0, 1)) {
            CAPI_LOG_ERROR("Wrong parameter value eq2_on: %d,  low_power_eq2_on : %d\n ",
                           lane_rx_info_ptr->value.eq2_info.eq2_on,
                           lane_rx_info_ptr->value.eq2_info.low_power_eq2_on);
            return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
        }
        if (!util_check_param_range(lane_rx_info_ptr->value.eq2_info.dsp_eq2_type, 0, 1)) {
            CAPI_LOG_ERROR("Wrong parameter value dsp_eq2_type: %d\n ",
                           lane_rx_info_ptr->value.eq2_info.dsp_eq2_type);
            return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
        }       
    }
    
    /*********************************SERDES**********************************/
    if (lane_rx_info_ptr->param.is.serdes_lp_has_prec_en) {
        if(!util_check_param_range(lane_rx_info_ptr->value.core_ip.serdes.lp_has_prec_en, 0, 1)) {
            CAPI_LOG_ERROR("Wrong Serdes lp_has_prec_en : %d \n ",
                           lane_rx_info_ptr->value.core_ip.serdes.lp_has_prec_en);
            return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
        }
    }
    
    if (lane_rx_info_ptr->param.is.serdes_pam_er_nr) {
        if(!util_check_param_range(lane_rx_info_ptr->value.core_ip.serdes.pam_er_nr, 0, 2)) {
            CAPI_LOG_ERROR("Wrong Serdes serdes_pam_er_nr : %d \n ",
                           lane_rx_info_ptr->value.core_ip.serdes.pam_er_nr);
            return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
        }
    }
    
    if (lane_rx_info_ptr->param.is.serdes_db_loss) {
        if(!util_check_param_range(lane_rx_info_ptr->value.core_ip.serdes.db_loss, 0, 35)) {
            CAPI_LOG_ERROR("Wrong Serdes serdes_db_loss : %d \n ",
                           lane_rx_info_ptr->value.core_ip.serdes.db_loss);
            return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
        }
    }
    
    /*********************************DSP**********************************/

    if (lane_rx_info_ptr->param.is.dsp_oplos_ignore) {
        if(!util_check_param_range(lane_rx_info_ptr->value.core_ip.dsp.oplos_ignore, 0, 1)) {
            CAPI_LOG_ERROR("Wrong DSP dsp_oplos_ignore  %d\r\n",
                           lane_rx_info_ptr->value.core_ip.dsp.oplos_ignore);
            return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
        }
    }

    if (lane_rx_info_ptr->param.is.dsp_elos_ignore) {
        if(!util_check_param_range(lane_rx_info_ptr->value.core_ip.dsp.elos_ignore, 0, 1)) {
            CAPI_LOG_ERROR("Wrong DSP dsp_elos_ignore  %d\r\n",
                           lane_rx_info_ptr->value.core_ip.dsp.elos_ignore);
            return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
        }
    }

    if((lane_rx_info_ptr->param.is.dsp_outlos_th_idx != lane_rx_info_ptr->param.is.dsp_inlos_th_idx) && (chk_type == CHK_USR_INPUT)){
        CAPI_LOG_ERROR("dsp_outlos_th_idx and dsp_inlos_th_idx need to be set at the same time\r\n");
        return(RR_ERROR_WRONG_INPUT_VALUE);
    }
    
    if (lane_rx_info_ptr->param.is.dsp_outlos_th_idx) {
        if(!util_check_param_range(lane_rx_info_ptr->value.core_ip.dsp.outlos_th_idx, 0, 9)) {
            CAPI_LOG_ERROR("Wrong DSP dsp_outlos_th_idx  %d\r\n",
                           lane_rx_info_ptr->value.core_ip.dsp.outlos_th_idx);
            return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
        }
    }

    if (lane_rx_info_ptr->param.is.dsp_inlos_th_idx) {
        if(!util_check_param_range(lane_rx_info_ptr->value.core_ip.dsp.inlos_th_idx, 0, 9)) {
            CAPI_LOG_ERROR("Wrong DSP dsp_inlos_th_idx  %d\r\n",
                           lane_rx_info_ptr->value.core_ip.dsp.inlos_th_idx);
            return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
        }
        /*make sure outlos_th_idx should be >= inlos_th_idx*/
        if(lane_rx_info_ptr->value.core_ip.dsp.outlos_th_idx < lane_rx_info_ptr->value.core_ip.dsp.inlos_th_idx){
            CAPI_LOG_ERROR("Wrong DSP dsp_inlos_th_idx  %d dsp_outlos_th_idx  %d\r\n",
                           lane_rx_info_ptr->value.core_ip.dsp.inlos_th_idx, lane_rx_info_ptr->value.core_ip.dsp.outlos_th_idx);
            return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
        }
    }
    
    if (lane_rx_info_ptr->param.is.dsp_los_th_idx) {
        if(!util_check_param_range(lane_rx_info_ptr->value.core_ip.dsp.los_th_idx, 0, 9)) {
            CAPI_LOG_ERROR("Wrong DSP dsp_los_th_idx  %d\r\n",
                           lane_rx_info_ptr->value.core_ip.dsp.los_th_idx);
            return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
        }
    }
    
    if (lane_rx_info_ptr->param.is.dsp_graycode) {
        if(!util_check_param_range(lane_rx_info_ptr->value.core_ip.dsp.graycode, 0, 1)) {
            CAPI_LOG_ERROR("Wrong DSP rx_graycode  %d\r\n",
                           lane_rx_info_ptr->value.core_ip.dsp.graycode);
            return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
        }
    }
    
    
    if (lane_rx_info_ptr->param.is.dsp_dc_wander_mu) {
        if(!util_check_param_range(lane_rx_info_ptr->value.core_ip.dsp.dc_wander_mu, 0, 6)) {
            CAPI_LOG_ERROR("Wrong DSP dc_wander_mu : %d \r\n",
                           lane_rx_info_ptr->value.core_ip.dsp.dc_wander_mu);
            return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
        }
    }
    
    
    if (lane_rx_info_ptr->param.is.dsp_gain_boost) {
        if(!util_check_param_range(lane_rx_info_ptr->value.core_ip.dsp.gain_boost, 0, 31)) {
            CAPI_LOG_ERROR("Wrong DSP gain_boost : %d \r\n",
                           lane_rx_info_ptr->value.core_ip.dsp.gain_boost);
            return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
        }
    }

    if (lane_rx_info_ptr->param.is.dsp_rx_bw_value) {
        uint8_t max_ana_bw = (phy_info_ptr->core_ip == CORE_IP_HOST_DSP)?63:15;
        if(!util_check_param_range(lane_rx_info_ptr->value.core_ip.dsp.rx_bw_value, 0, max_ana_bw) ) {
            CAPI_LOG_ERROR("Wrong DSP rx_bw_value %d\r\n",
                           lane_rx_info_ptr->value.core_ip.dsp.rx_bw_value);
            return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
        }
    }
    if (lane_rx_info_ptr->param.is.dsp_gain2_on) {
        if(!util_check_param_range(lane_rx_info_ptr->value.core_ip.dsp.gain2_on, 0, 2)) {
            CAPI_LOG_ERROR("Wrong DSP gain2_on %d\r\n",
                           lane_rx_info_ptr->value.core_ip.dsp.gain2_on);
            return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
        }
    }
    
    if (lane_rx_info_ptr->param.is.dsp_gain3_on) {
        if(!util_check_param_range(lane_rx_info_ptr->value.core_ip.dsp.gain3_on, 0, 2)) {
            CAPI_LOG_ERROR("Wrong DSP gain3_on %d\r\n",
                           lane_rx_info_ptr->value.core_ip.dsp.gain3_on);
            return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
        }
    }
    if (lane_rx_info_ptr->param.is.dsp_autopeaking_en) {
        if(!util_check_param_range(lane_rx_info_ptr->value.core_ip.dsp.autopeaking_en, 0, 1)) {
            CAPI_LOG_ERROR("Wrong DSP autopeaking_en %d\r\n",
                           lane_rx_info_ptr->value.core_ip.dsp.autopeaking_en);
            return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
        }
    }
    
    if (lane_rx_info_ptr->param.is.dsp_precode) {
        if(!util_check_param_range(lane_rx_info_ptr->value.core_ip.dsp.precode, 0, 2)) {
            CAPI_LOG_ERROR("Wrong DSP precode %d\r\n",
                           lane_rx_info_ptr->value.core_ip.dsp.precode);
            return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
        }
    }
    
    if (lane_rx_info_ptr->param.is.dsp_phase_bias_auto_tuning_info) {
        if (phy_info_ptr->core_ip == CORE_IP_HOST_DSP) {
            CAPI_LOG_ERROR("Host side doesn't support this feature!\r\n");
            return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
            return return_result;
        }
       if ((lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.phase_auto_tune_en > 1) ||
            (lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.dynamic_phase_auto_tune_en > 1)) {
            CAPI_LOG_ERROR("Invalid DSP phase_auto_tune_en %d\r\n",
                           lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.phase_auto_tune_en);
            return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
        } else if (lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.phase_auto_tune_en) {
            if ((lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.max_phase_bias_th <
                 lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.min_phase_bias_th)            ||
                 ((lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.max_phase_bias_th >  31) ||
                  (lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.max_phase_bias_th < -32))   ||
                 ((lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.min_phase_bias_th >  31) ||
                  (lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.min_phase_bias_th < -32))) {
                CAPI_LOG_ERROR("Invalid DSP phase_bias_th=[%d, %d] \r\n",
                                lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.min_phase_bias_th,
                                lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.max_phase_bias_th);
                return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
            }
            if (lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.phase_tune_dwell_time < 0 ||
                 lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.phase_tune_dwell_time > 4) {
                CAPI_LOG_ERROR("Invalid DSP phase_tune_dwell_time %d \r\n",
                                lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.phase_tune_dwell_time);
                return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
            }
            if (lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.phase_tune_step_size < 0 ||
                 lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.phase_tune_step_size > 1) {
                CAPI_LOG_ERROR("Invalid DSP phase_tune_step_size %d \r\n",
                                lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.phase_tune_step_size);
                return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
            }
            if (lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.phase_tune_step_size == 1 && (
                (lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.max_phase_bias_th % 2) != 0 || 
                (lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.min_phase_bias_th % 2) != 0)) {
                CAPI_LOG_ERROR("Invalid DSP phase_bias_th=[%d, %d] step %d \r\n",
                                lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.min_phase_bias_th,
                                lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.max_phase_bias_th,
                                lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.phase_tune_step_size);
                return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
            }  

            if (lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.dynamic_phase_auto_tune_en == 1 && (
                lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.phase_dtune_bias_range < 0 || 
                lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.phase_dtune_bias_range > 10)) {
                CAPI_LOG_ERROR("Invalid DSP phase_dtune_bias_range=%d \r\n",
                                lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.phase_dtune_bias_range);
                return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
            }            

            if (lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.dynamic_phase_auto_tune_en == 1 && (
                lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.phase_dtune_snr_change_th < 0 || 
                lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.phase_dtune_snr_change_th > 10)) {
                CAPI_LOG_ERROR("Invalid DSP phase_dtune_snr_change_th=%d \r\n",
                                lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.phase_dtune_snr_change_th);
                return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
            }            

            if (lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.phase_tune_link_down_snr < 0 || 
                lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.phase_tune_link_down_snr > 7) {
                CAPI_LOG_ERROR("Invalid DSP phase_tune_link_down_snr=%d \r\n",
                                lane_rx_info_ptr->value.core_ip.dsp.phase_bias_auto_tuning_info.phase_tune_link_down_snr);
                return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
            }            
            
        }

    }
  
    if (lane_rx_info_ptr->param.is.dsp_kp_kf_info) {
        if ((lane_rx_info_ptr->value.core_ip.dsp.kp_kf_info.kp_tracking > 7) ||
            (lane_rx_info_ptr->value.core_ip.dsp.kp_kf_info.kf_tracking > 7) ||
            (lane_rx_info_ptr->value.core_ip.dsp.kp_kf_info.kp > 7)          ||
            (lane_rx_info_ptr->value.core_ip.dsp.kp_kf_info.kf > 7)          ||
            (lane_rx_info_ptr->value.core_ip.dsp.kp_kf_info.kp_hlf_stp > 1)  ||
            (lane_rx_info_ptr->value.core_ip.dsp.kp_kf_info.kp_tracking_hlf_stp > 1)) {
            CAPI_LOG_ERROR("Wrong DSP dsp_kp_kf_info : %d, %d, %d, %d, %d, %d\r\n",
                           lane_rx_info_ptr->value.core_ip.dsp.kp_kf_info.kp_tracking,
                           lane_rx_info_ptr->value.core_ip.dsp.kp_kf_info.kf_tracking,
                           lane_rx_info_ptr->value.core_ip.dsp.kp_kf_info.kp,
                           lane_rx_info_ptr->value.core_ip.dsp.kp_kf_info.kf,
                           lane_rx_info_ptr->value.core_ip.dsp.kp_kf_info.kp_hlf_stp,
                           lane_rx_info_ptr->value.core_ip.dsp.kp_kf_info.kp_tracking_hlf_stp);
            return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
        }
    }

    if (lane_rx_info_ptr->param.is.dsp_eq_tap_sel) {
        if(!util_check_param_range(lane_rx_info_ptr->value.core_ip.dsp.eq_tap_sel, 0, 3)) {
            CAPI_LOG_ERROR("Wrong DSP eq_tap_sel;  eq_tap_sel: %d  \n ",
                           lane_rx_info_ptr->value.core_ip.dsp.eq_tap_sel);
            return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
        }
    }
    
    if (lane_rx_info_ptr->param.is.dsp_eq_hpwr_o) {
        if(!util_check_param_range(lane_rx_info_ptr->value.core_ip.dsp.dsp_eq_hpwr_option, 0, 1)) {
            CAPI_LOG_ERROR("Wrong DSP dsp_eq_hpwr_option;  dsp_eq_hpwr_option: %d  \n ",
                           lane_rx_info_ptr->value.core_ip.dsp.dsp_eq_hpwr_option);
            return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
        }
    }

    if (lane_rx_info_ptr->param.is.dsp_ffe_slicer_multiplier) {
        if(!util_check_param_range(lane_rx_info_ptr->value.core_ip.dsp.ffe_slicer_multiplier, 0, 1)) {
            CAPI_LOG_ERROR("Wrong DSP ffe_slicer_multiplier;  ffe_slicer_multiplier: %d \n ",
                           lane_rx_info_ptr->value.core_ip.dsp.ffe_slicer_multiplier);
            return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
        }
    }
    
        /*
    if (lane_rx_info_ptr->param.is.dsp_los_info) {
        CAPI_LOG_ERROR("dsp_los_info not supported\r\n");
        return(RR_ERROR_FEATURE_NOT_SUPPORTED);
        if ((lane_rx_info_ptr->value.core_ip.dsp.los_info.los_th_idx  > 9) ||
            (lane_rx_info_ptr->value.core_ip.dsp.los_info.los_bypass     > 1) ||
            (lane_rx_info_ptr->value.core_ip.dsp.los_info.los_bypass_val > 1)) {
            CAPI_LOG_ERROR("Wrong dsp_los_info:   th_idx:%d   bypass:%d    bypass_val:%d \r\n",
                           lane_rx_info_ptr->value.core_ip.dsp.los_info.los_th_idx,
                           lane_rx_info_ptr->value.core_ip.dsp.los_info.los_bypass,
                           lane_rx_info_ptr->value.core_ip.dsp.los_info.los_bypass_val);
            return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
        }
        }
        */
    }
    return(return_result);
}

/**
 * @brief      sanity_checker_loopback(capi_phy_info_t*      phy_info_ptr,
 *                                              capi_loopback_info_t* loopback_ptr)
 * @details    This API is used to sanity check set loopback params
 *
 * @param[in]  phy_info_ptr: reference to the phy information object
 * @param[in]  loopback_ptr: reference to the loopback information object
 *
 * @return     returns the performance result of the called methode/function
 */
return_result_t sanity_checker_loopback(capi_phy_info_t*      phy_info_ptr,
                                        capi_loopback_info_t* loopback_ptr)
{
    if ((phy_info_ptr->core_ip == CORE_IP_ALL) ||
        (phy_info_ptr->core_ip == CORE_IP_CW)  ||
        (phy_info_ptr->core_ip == CORE_IP_KP4_KR4_FEC_DEC)) {
        CAPI_LOG_ERROR("Wrong core ip : %d\n", phy_info_ptr->core_ip);
        return(RR_ERROR_WRONG_CORE_IP_VALUE);
    }

    if ((loopback_ptr->mode != CAPI_GLOBAL_LOOPBACK_MODE) &&
        (loopback_ptr->mode != CAPI_REMOTE_LOOPBACK_MODE)) {
        CAPI_LOG_ERROR("Invalid loopback mode: %d\r\n", loopback_ptr->mode);
        return(RR_ERROR_WRONG_INPUT_VALUE);
    }

    if (loopback_ptr->enable > 1) {
        CAPI_LOG_ERROR("Invalid enable value: %d\r\n", loopback_ptr->enable);
        return(RR_ERROR_WRONG_INPUT_VALUE);
    }

    return(RR_SUCCESS);
}

/**
 * @brief      sanity_checker_set_prbs_info(capi_phy_info_t *phy_info_ptr, capi_prbs_info_t* prbs_info_ptr)
 * @details    This API is used to sanity check set prbs config params
 *
 * @param[in]  phy_info_ptr: this parameter
 * @param[in]  prbs_info_ptr: this parameter
 *
 * @return     returns the performance result of the called methode/function
 */
return_result_t sanity_checker_set_prbs_info(capi_phy_info_t *phy_info_ptr, capi_prbs_info_t* prbs_info_ptr)
{

    if(!util_check_param_range(prbs_info_ptr->gen_switch, 0 , 1)) {
        CAPI_LOG_ERROR("Invalid switch status: %d\r\n", prbs_info_ptr->gen_switch);
        return RR_ERROR_WRONG_INPUT_VALUE;
    }

    if ((phy_info_ptr->core_ip == CORE_IP_HOST_DSP) || (phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP)) {
        if( prbs_info_ptr->gen_switch == CAPI_SWITCH_OFF ){
            return RR_SUCCESS;
        }else{
            if ((prbs_info_ptr->ptype == CAPI_PRBS_MONITOR)  ||
                (prbs_info_ptr->ptype == CAPI_PRBS_GENERATOR)) {
                switch(prbs_info_ptr->op.pcfg.poly.poly_type) {
                    case CAPI_PRBS_POLY_7:
                    case CAPI_PRBS_POLY_9  :
                    case CAPI_PRBS_POLY_10 :
                    case CAPI_PRBS_POLY_11 :
                    case CAPI_PRBS_POLY_13 :
                    case CAPI_PRBS_POLY_15 :
                    case CAPI_PRBS_POLY_20 :
                    case CAPI_PRBS_POLY_23 :
                    case CAPI_PRBS_POLY_31 :
                    case CAPI_PRBS_POLY_49 :
                    case CAPI_PRBS_POLY_58 :
                        break;
                    default:
                        CAPI_LOG_ERROR("Invalid LW poly: %d\r\n", prbs_info_ptr->op.pcfg.poly.poly_type);
                        return RR_ERROR_WRONG_INPUT_VALUE;
                }
                if (prbs_info_ptr->ptype == CAPI_PRBS_GENERATOR) {
                    if(!util_check_param_range(prbs_info_ptr->op.pcfg.tx_invert, 0 , 1)) {
                        CAPI_LOG_ERROR("Invalid TX invert: %d\r\n",prbs_info_ptr->op.pcfg.tx_invert);
                        return RR_ERROR_WRONG_INPUT_VALUE;
                    }
                }
                if (prbs_info_ptr->ptype == CAPI_PRBS_MONITOR) {
                    if(!util_check_param_range(prbs_info_ptr->op.pcfg.rx_invert, 0 , 1)) {
                        CAPI_LOG_ERROR("Invalid RX invert: %d\r\n",prbs_info_ptr->op.pcfg.rx_invert);
                        return RR_ERROR_WRONG_INPUT_VALUE;
                    }
                }
            }else if ((CAPI_PRBS_SSPRQ_GENERATOR == prbs_info_ptr->ptype) || (CAPI_PRBS_SSPRQ_MONITOR == prbs_info_ptr->ptype)) {
                if (util_check_param_range(prbs_info_ptr->op.ssprq.modulation, 0 , 1) &&
                    util_check_param_range(prbs_info_ptr->op.ssprq.bit_swap  , 0 , 1) &&
                    util_check_param_range(prbs_info_ptr->op.ssprq.gray_code , 0 , 1)) {
                } else {
                    return RR_ERROR_WRONG_INPUT_VALUE;
                }
            } else if (CAPI_PRBS_SQUARE_WAVE_GENERATOR == prbs_info_ptr->ptype) {
                if (util_check_param_range(prbs_info_ptr->op.sqr_wave.ptrn_wave_type, 0, (SQR_WAVE_PATTEN_MAX-1))) {
                } else {
                    return RR_ERROR_WRONG_INPUT_VALUE;
                }
           /* } else if (CAPI_PRBS_Q_PRBS_13_GENERATOR == prbs_info_ptr->ptype) {
                if (util_check_param_range(prbs_info_ptr->op.qprbs13.invert_bits, 0, 1) &&
                    util_check_param_range(prbs_info_ptr->op.qprbs13.training_frame, 0, 1)) {
                } else {
                    return RR_ERROR_WRONG_INPUT_VALUE;
                }
            } else if (CAPI_PRBS_TX_LINEARITY_GENERATOR == prbs_info_ptr->ptype) {
                return RR_SUCCESS; */
            } else if (CAPI_PCS_SCRM_IDLE_GENERATOR == prbs_info_ptr->ptype) {
                return RR_SUCCESS;
            } else if (CAPI_PRBS_SHARED_TX_PATTERN_GENERATOR == prbs_info_ptr->ptype) {
                if (!util_check_param_range(prbs_info_ptr->op.shared_tx_ptrn.length, 0, 32)) {
                    CAPI_LOG_ERROR("Invalid pattern length: %d\r\n", prbs_info_ptr->op.shared_tx_ptrn.length);
                    return RR_ERROR_WRONG_INPUT_VALUE;
                } 
            } else {
                CAPI_LOG_ERROR("Invalid ptype: %d\r\n", prbs_info_ptr->ptype);
                return RR_ERROR_WRONG_INPUT_VALUE;
            }
        }
    } 
    else if ((phy_info_ptr->core_ip == CORE_IP_HOST_SERDES) || (phy_info_ptr->core_ip == CORE_IP_MEDIA_SERDES)) {
        if ((prbs_info_ptr->ptype == CAPI_PRBS_MONITOR)  ||
            (prbs_info_ptr->ptype == CAPI_PRBS_GENERATOR)) {
            switch(prbs_info_ptr->op.pcfg.poly.bh_poly) {
                case CAPI_BH_PRBS_POLY_7:
                case CAPI_BH_PRBS_POLY_9:
                case CAPI_BH_PRBS_POLY_11:
                case CAPI_BH_PRBS_POLY_15:
                case CAPI_BH_PRBS_POLY_23:
                case CAPI_BH_PRBS_POLY_31:
                case CAPI_BH_PRBS_POLY_58:
                case CAPI_BH_PRBS_POLY_49:
                case CAPI_BH_PRBS_POLY_10:
                case CAPI_BH_PRBS_POLY_20:
                case CAPI_BH_PRBS_POLY_13:
                case CAPI_BH_PRBS_USER_40_BIT_REPEAT:
                case CAPI_BH_PRBS_PRBS_AUTO_DETECT:
                  break;
                default:
                  CAPI_LOG_ERROR("Invalid BH poly: %d\r\n", prbs_info_ptr->op.pcfg.poly.bh_poly);
                  return RR_ERROR_WRONG_INPUT_VALUE;
            }
            if (prbs_info_ptr->ptype == CAPI_PRBS_MONITOR) {
                if(!util_check_param_range(prbs_info_ptr->op.pcfg.rx_invert, 0 , 1)) {
                    CAPI_LOG_ERROR("Invalid RX invert: %d\r\n",prbs_info_ptr->op.pcfg.rx_invert);
                    return RR_ERROR_WRONG_INPUT_VALUE;
                }
            }
            if (prbs_info_ptr->ptype == CAPI_PRBS_GENERATOR) {
                if(!util_check_param_range(prbs_info_ptr->op.pcfg.tx_invert, 0 , 1)) {
                    CAPI_LOG_ERROR("Invalid TX invert: %d\r\n",prbs_info_ptr->op.pcfg.tx_invert);
                    return RR_ERROR_WRONG_INPUT_VALUE;
                }
           }
       } else if ((CAPI_PRBS_SSPRQ_GEN_MON == prbs_info_ptr->ptype) ||
                  (CAPI_PRBS_SSPRQ_GENERATOR == prbs_info_ptr->ptype)     ||
                  (CAPI_PRBS_SSPRQ_MONITOR == prbs_info_ptr->ptype)) {
            if (util_check_param_range(prbs_info_ptr->op.ssprq.modulation, 0 , 1) && /* Modulation not used in ML? Remove? */
                util_check_param_range(prbs_info_ptr->op.ssprq.bit_swap  , 0 , 1) && /* None of these used in ML */
                util_check_param_range(prbs_info_ptr->op.ssprq.gray_code , 0 , 1)) {
            } else {
                return RR_ERROR_WRONG_INPUT_VALUE;
            }
      /*  } else if (CAPI_PRBS_Q_PRBS_13_GENERATOR == prbs_info_ptr->ptype) {
            if (util_check_param_range(prbs_info_ptr->op.qprbs13.invert_bits, 0, 1) &&
                util_check_param_range(prbs_info_ptr->op.qprbs13.training_frame, 0, 1)) {
            } else {
                return RR_ERROR_WRONG_INPUT_VALUE;
            } */
        } else if (CAPI_PRBS_SQUARE_WAVE_GENERATOR == prbs_info_ptr->ptype) {
            if (util_check_param_range(prbs_info_ptr->op.sqr_wave.ptrn_wave_type, 0, (SQR_WAVE_PATTEN_MAX-1))) {
            } else {
                return RR_ERROR_WRONG_INPUT_VALUE;
            }
     /*   } else if (CAPI_PRBS_TX_LINEARITY_GENERATOR == prbs_info_ptr->ptype) {
            return RR_SUCCESS; */
        } else if (CAPI_PRBS_SHARED_TX_PATTERN_GENERATOR == prbs_info_ptr->ptype) {
            if (!util_check_param_range(prbs_info_ptr->op.shared_tx_ptrn.length, 0, 32)) {
                CAPI_LOG_ERROR("Invalid pattern length: %d\r\n", prbs_info_ptr->op.shared_tx_ptrn.length);
                return RR_ERROR_WRONG_INPUT_VALUE;
            } 
        } else {
            CAPI_LOG_ERROR("Invalid ptype: %d\r\n", prbs_info_ptr->ptype);
            return RR_ERROR_WRONG_INPUT_VALUE;
        }
    } else {
        CAPI_LOG_ERROR("Invalid core_ip: %d\r\n", phy_info_ptr->core_ip);
        return RR_ERROR_WRONG_CORE_IP_VALUE;
    }

    return RR_SUCCESS;
}

/**
 * @brief      sanity_checker_get_prbs_info(capi_phy_info_t *phy_info_ptr, capi_prbs_info_t* prbs_info_ptr, sanity_chk_t chk_type)
 * @details    This API is used to sanity check get prbs config params
 *
 * @param[in]  phy_info_ptr: this parameter
 * @param[in]  prbs_info_ptr: this parameter
 *
 * @return     returns the performance result of the called methode/function
 */
return_result_t sanity_checker_get_prbs_info(capi_phy_info_t *phy_info_ptr, capi_prbs_info_t* prbs_info_ptr, sanity_chk_t chk_type)
{
    return_result_t return_result = RR_ERROR_WRONG_INPUT_VALUE;

    if (CHK_USR_INPUT == chk_type) {
        if ((phy_info_ptr->core_ip == CORE_IP_HOST_DSP)    ||
            (phy_info_ptr->core_ip == CORE_IP_HOST_SERDES) ||
            (phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP)   ||
            (phy_info_ptr->core_ip == CORE_IP_MEDIA_SERDES)) {
            return_result = RR_SUCCESS;
        } else {
            CAPI_LOG_ERROR("Invalid core_ip: %d\r\n", phy_info_ptr->core_ip);
            return RR_ERROR_WRONG_CORE_IP_VALUE;
        }
        /*TODO: Revisit with KP4 PRBS, KP4 PRBS not using sanity checker*/
        /*Commented QPRBS, TX linearity and shared pattern in case of future support */
        if ((prbs_info_ptr->ptype == CAPI_PRBS_GENERATOR)                  ||
            (prbs_info_ptr->ptype == CAPI_PRBS_MONITOR)                    ||
            (prbs_info_ptr->ptype == CAPI_PRBS_SSPRQ_GENERATOR)            ||
            (prbs_info_ptr->ptype == CAPI_PRBS_SSPRQ_MONITOR)              ||
            (prbs_info_ptr->ptype == CAPI_PRBS_SQUARE_WAVE_GENERATOR)   /* ||
            (prbs_info_ptr->ptype == CAPI_PRBS_Q_PRBS_13_GENERATOR)        ||
            (prbs_info_ptr->ptype == CAPI_PRBS_TX_LINEARITY_GENERATOR)     ||
            (prbs_info_ptr->ptype == CAPI_PRBS_SHARED_TX_PATTERN_GENERATOR)*/ ) {
            return_result = RR_SUCCESS;
        } else {
            CAPI_LOG_ERROR("Invalid pattern type: %d\r\n", prbs_info_ptr->ptype);
            return RR_ERROR_WRONG_INPUT_VALUE;
        }
    } else if (CHK_FW_OUTPUT == chk_type) {
        return_result = sanity_checker_set_prbs_info(phy_info_ptr, prbs_info_ptr);
    }
    
    return(return_result);

}


/**
 * @brief      sanity_checker_prbs_status_get(capi_phy_info_t *phy_info_ptr, capi_prbs_info_t* prbs_info_ptr)
 * @details    This API is used to sanity check get prbs status params
 *
 * @param[in]  phy_info_ptr: this parameter
 * @param[in]  prbs_info_ptr: this parameter
 *
 * @return     returns the performance result of the called methode/function
 */
return_result_t sanity_checker_prbs_status_get(capi_phy_info_t *phy_info_ptr, capi_prbs_status_t* prbs_status_ptr, sanity_chk_t chk_type)
{
    if (CHK_USR_INPUT == chk_type) {
        if((((prbs_status_ptr->prbs_type == CAPI_PRBS_MONITOR) 
             || (prbs_status_ptr->prbs_type == CAPI_PRBS_SSPRQ_MONITOR)) &&
            ((phy_info_ptr->core_ip == CORE_IP_HOST_DSP)    ||
             (phy_info_ptr->core_ip == CORE_IP_HOST_SERDES) ||
             (phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP)   ||
             (phy_info_ptr->core_ip == CORE_IP_MEDIA_SERDES))) || 
          (((prbs_status_ptr->prbs_type == CAPI_PRBS_KP4_MEDIA_MONITOR) ||
            (prbs_status_ptr->prbs_type == CAPI_PRBS_KP4_HOST_MONITOR)) &&
            (phy_info_ptr->core_ip == CORE_IP_KP4_KR4_FEC_DEC))) { /*TODO: Revisit when KP4 PRBS is implemented, KP4 PRBS not using sanity checker*/
            return RR_SUCCESS;
        } else {
            return RR_ERROR_WRONG_INPUT_VALUE;
        }
    } else if (CHK_FW_OUTPUT == chk_type) {
        if (util_check_param_range(prbs_status_ptr->lock, 0, 1) &&
            util_check_param_range(prbs_status_ptr->lock_loss, 0, 1)) {
            return RR_SUCCESS;
        } else {
            return RR_ERROR_WRONG_OUTPUT_VALUE;
        }
    } else {
        return RR_ERROR;
    }
}

/**
 * @brief      sanity_checker_clear_prbs_status(capi_phy_info_t *phy_info_ptr,  capi_prbs_status_t* prbs_st_ptr)
 * @details    This API is used to sanity check clear prbs status params
 *
 * @param[in]  phy_info_ptr: this parameter
 * @param[in]  ptype: this parameter
 *
 * @return     returns the performance result of the called methode/function
 */
return_result_t sanity_checker_clear_prbs_status(capi_phy_info_t *phy_info_ptr,  capi_prbs_status_t* prbs_st_ptr)
{
    if (((phy_info_ptr->core_ip != CORE_IP_HOST_DSP)    &&
        (phy_info_ptr->core_ip != CORE_IP_HOST_SERDES) &&
        (phy_info_ptr->core_ip != CORE_IP_MEDIA_DSP)   &&
        (phy_info_ptr->core_ip != CORE_IP_MEDIA_SERDES)) || ((prbs_st_ptr->prbs_type != CAPI_PRBS_MONITOR) && (prbs_st_ptr->prbs_type != CAPI_PRBS_SSPRQ_MONITOR))) {
        CAPI_LOG_ERROR("Invalid core_ip: %d\r\n", phy_info_ptr->core_ip);
        return RR_ERROR_WRONG_CORE_IP_VALUE;
    }
    return RR_SUCCESS;
}

/**
 * @brief      sanity_checker_set_txpi_override(capi_phy_info_t *phy_info_ptr,  txpi_override_t* txpi_ovrd)
 * @details    This API is used to sanity check txpi override parameters
 *
 * @param[in]  phy_info_ptr: this parameter
 * @param[in]  txpi_ovrd   : this parameter
 *
 * @return     returns the performance result of the called methode/function
 */
return_result_t sanity_checker_set_txpi_override(capi_phy_info_t *phy_info_ptr,  txpi_override_t* txpi_ovrd)
{
    if (/*(phy_info_ptr->core_ip != CORE_IP_HOST_DSP)    && */
        (phy_info_ptr->core_ip != CORE_IP_HOST_SERDES) &&
        /*(phy_info_ptr->core_ip != CORE_IP_MEDIA_DSP)   && */
        (phy_info_ptr->core_ip != CORE_IP_MEDIA_SERDES)) {
        CAPI_LOG_ERROR("Invalid core_ip: %d\r\n", phy_info_ptr->core_ip);
        return RR_ERROR_WRONG_CORE_IP_VALUE;
    }

    if (!util_check_param_range(txpi_ovrd->enable, 0, 1)) {
        return RR_ERROR_WRONG_INPUT_VALUE;
    }

    return RR_SUCCESS;
}


/**
 * @brief      sanity_checker_prbs_inject_error(capi_phy_info_t *phy_info_ptr, capi_prbs_err_inject_t* prbs_err_inj_ptr)
 * @details    This API is used to sanity check prbs inject error params
 *
 * @param[in]  phy_info_ptr: this parameter
 * @param[in]  prbs_err_inj_ptr: this parameter
 *
 * @return     returns the performance result of the called methode/function
 */
return_result_t sanity_checker_prbs_inject_error(capi_phy_info_t *phy_info_ptr, capi_prbs_err_inject_t* prbs_err_inj_ptr)
{

    if ((phy_info_ptr->core_ip != CORE_IP_HOST_DSP)    &&
        (phy_info_ptr->core_ip != CORE_IP_HOST_SERDES) &&
        (phy_info_ptr->core_ip != CORE_IP_MEDIA_DSP)   &&
        (phy_info_ptr->core_ip != CORE_IP_MEDIA_SERDES)) {
        CAPI_LOG_ERROR("Invalid core ip: %d\r\n", phy_info_ptr->core_ip);
        return RR_ERROR_WRONG_INPUT_VALUE;
    }
    if(prbs_err_inj_ptr->inject_err_num == 0) {
         CAPI_LOG_ERROR("Invalid error number: %d\r\n",prbs_err_inj_ptr->inject_err_num);
         return RR_ERROR_WRONG_INPUT_VALUE;
    }

    return RR_SUCCESS;
}

#ifdef CAPI_CONFIG_CMD_SANITY_CHK_ENABLE
/**
 * @brief      sanity_checker_get_lane_info(capi_phy_info_t*  phy_info_ptr,
 *                                          capi_lane_info_t* lane_info_ptr,
 *                                          sanity_chk_t      chk_type)
 * @details    This API is used to sanity check lane status info params when doing a get
 *
 * @param[in]  phy_info_ptr  : reference to the phy information object 
 * @param[in]  lane_info_ptr : reference to the lane information object 
 * @param[in]  sanity_chk_t  : chk_type - 0: Check user input, 1: Chec kFW output
 * 
 * @return     returns the performance result of the called methode/function
 */
static return_result_t sanity_checker_get_lane_info(capi_phy_info_t*  phy_info_ptr,
                                                    capi_lane_info_t* lane_info_ptr,
                                                    sanity_chk_t      chk_type)
{
    if (!lane_info_ptr->param.content) return(RR_ERROR_NOT_INITIALIZED);

    if ((phy_info_ptr->core_ip == CORE_IP_CW) ||
        (phy_info_ptr->core_ip == CORE_IP_KP4_KR4_FEC_DEC)) {
        if ((lane_info_ptr->param.is.lane_status)            ||
            (lane_info_ptr->param.is.lnktrn_status)          ||
#ifdef EST2_AUTONEG_ENABLED
            (lane_info_ptr->param.is.an_status)              ||
#endif
            (lane_info_ptr->param.is.diag_status)            ||
            (lane_info_ptr->param.is.sticky_loss_electrical) ||
            (lane_info_ptr->param.is.sticky_loss_optical)    ||
            (lane_info_ptr->param.is.sticky_loss_link)) {
            return(RR_ERROR_WRONG_CORE_IP_VALUE);
        }

        if (phy_info_ptr->core_ip == CORE_IP_KP4_KR4_FEC_DEC) {
            if (lane_info_ptr->param.is.cw_status) return(RR_ERROR_WRONG_CORE_IP_VALUE);
        } else if (!lane_info_ptr->param.is.cw_status) {
            return(RR_ERROR_WRONG_CORE_IP_VALUE);
        }
    } else if (phy_info_ptr->core_ip == CORE_IP_ALL) {
        return(RR_ERROR_WRONG_CORE_IP_VALUE);
    } else {
        if (lane_info_ptr->param.is.cw_status) {
            return(RR_ERROR_WRONG_CORE_IP_VALUE);
        }
    }

    if (CHK_FW_OUTPUT == chk_type) {
        ;
        /*We might need to do some Sanity checking for the FW returned value in future*/
    }

    return(RR_SUCCESS);
}
#endif /* CAPI_CONFIG_CMD_SANITY_CHK_ENABLE */

/**
 * @brief      sanity_checker_get_polarity_info(capi_phy_info_t* phy_info_ptr, capi_polarity_info_t* polarity_info_ptr, sanity_chk_t chk_type)
 * @details    This API is used to sanity check polarity info params when doing a get
 *
 * @param[in]  phy_info_ptr     : this parameter
 * @param[in]  polarity_info_ptr: this parameter
 * @param[in]  sanity_chk_t     : chk_type - 0: Check user input, 1: Check FW output
 *
 * @return     returns the performance result of the called methode/function
 */
return_result_t sanity_checker_get_polarity_info (capi_phy_info_t* phy_info_ptr, capi_polarity_info_t* polarity_info_ptr, sanity_chk_t chk_type)
{
    return_result_t return_result = RR_ERROR_WRONG_INPUT_VALUE;
    uint32_t num_user_lanes = 0, i;
    unsigned char total_lanes;

    if (CHK_USR_INPUT == chk_type) {
        /* Sanity check the input by the user */
        if (((phy_info_ptr->core_ip == CORE_IP_HOST_DSP)    ||
             (phy_info_ptr->core_ip == CORE_IP_HOST_SERDES) ||
             (phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP)   ||
             (phy_info_ptr->core_ip == CORE_IP_MEDIA_SERDES)) &&
            ((DIR_INGRESS == polarity_info_ptr->direction) || (DIR_EGRESS == polarity_info_ptr->direction))) {
            return_result = RR_SUCCESS;
        } /* If not LW or BH and not ingress of egress, return wrong input */

        /* Check the number of lanes entered by the user.
         * capi_get_polarity does not support more than one lane.
         * If more than one lane is selected, return error
         */
        util_get_number_of_lanes(phy_info_ptr, &total_lanes);
        for (i = 0; i < total_lanes; i++)
        {
            if(phy_info_ptr->lane_mask & (1 << i))
                num_user_lanes++;
        }
        if(num_user_lanes > 1)
        {
            CAPI_LOG_ERROR("Only one lane allowed for capi_get_polarity.\r\n");
            return_result = RR_ERROR_WRONG_INPUT_VALUE;
        }
    } else if (CHK_FW_OUTPUT == chk_type) {
        /* Sanity check the output by the FW */
        if ((POLARITY_SWAP_CURRENT_SETTING == polarity_info_ptr->action)   ||
            (POLARITY_INVERT_DEFAULT_SETTING == polarity_info_ptr->action) ||
            (POLARITY_DEFAULT_SETTING == polarity_info_ptr->action)) {
            return_result = RR_SUCCESS;
        } else {
            /* If FW doesn't return swap_current, invert_default or default, return wrong output */
            return_result = RR_ERROR_WRONG_OUTPUT_VALUE;
        }
    } else {
        return_result = RR_ERROR; /* What is the correct error type in this case? */
    }
    return return_result;
}

/**
 * @brief      sanity_checker_set_polarity_info(capi_phy_info_t* phy_info_ptr, capi_polarity_info_t* polarity_info_ptr)
 * @details    This API is used to sanity check polarity info params
 *
 * @param[in]  phy_info_ptr     : this parameter
 * @param[in]  polarity_info_ptr: this parameter
 *
 * @return     returns the performance result of the called methode/function
 */
return_result_t sanity_checker_set_polarity_info(capi_phy_info_t* phy_info_ptr, capi_polarity_info_t* polarity_info_ptr)
{
    return_result_t return_result = RR_SUCCESS;

    if ((phy_info_ptr->core_ip == CORE_IP_HOST_DSP)    ||
        (phy_info_ptr->core_ip == CORE_IP_HOST_SERDES) ||
        (phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP)   ||
        (phy_info_ptr->core_ip == CORE_IP_MEDIA_SERDES)) {
        if ((polarity_info_ptr->action == POLARITY_DEFAULT_SETTING) || (polarity_info_ptr->action == POLARITY_INVERT_DEFAULT_SETTING) || (polarity_info_ptr->action == POLARITY_SWAP_CURRENT_SETTING)){
            return_result = RR_SUCCESS;
        } else {
            CAPI_LOG_ERROR("Invalid polarity action value: %d\r\n", polarity_info_ptr->action);
            return_result = RR_ERROR_WRONG_INPUT_VALUE;
        }
    } else {
        CAPI_LOG_ERROR("Invalid core ip value: %d\r\n", phy_info_ptr->core_ip);
        return_result = RR_ERROR_WRONG_CORE_IP_VALUE;
    }

    return return_result;
}

/**
 * @brief      sanity_checker_get_lane_ctrl_info(capi_phy_info_t* phy_info_ptr, capi_lane_ctrl_info_t* lane_ctrl_info_ptr)
 * @details    This API is used to sanity check lane control info get parameters
 *
 * @param[in]  phy_info_ptr      : this parameter
 * @param[in]  lane_ctrl_info_ptr: this parameter
 * @param[in]  sanity_chk_t      : chk_type - 0: Check user input, 1: Chec kFW output
 *
 * @return     returns the performance result of the called methode/function
 */
return_result_t sanity_checker_get_lane_ctrl_info(capi_phy_info_t* phy_info_ptr, capi_lane_ctrl_info_t* lane_ctrl_info_ptr, sanity_chk_t chk_type)
{
    return_result_t return_result = RR_SUCCESS;

    if (phy_info_ptr->core_ip == CORE_IP_HOST_SERDES || phy_info_ptr->core_ip == CORE_IP_MEDIA_SERDES ||
        phy_info_ptr->core_ip == CORE_IP_HOST_DSP || phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP) {
        if (CHK_USR_INPUT == chk_type)  {
            if ((lane_ctrl_info_ptr->param.is.rx_datapath_power)  ||
                (lane_ctrl_info_ptr->param.is.tx_datapath_power)  ||
                (lane_ctrl_info_ptr->param.is.rx_data_path)       ||
                (lane_ctrl_info_ptr->param.is.tx_data_path)       ||
                (lane_ctrl_info_ptr->param.is.rx_lane_cfg_reset)  ||
                (lane_ctrl_info_ptr->param.is.tx_lane_cfg_reset)  ||
                (lane_ctrl_info_ptr->param.is.rx_squelch)         ||
                (lane_ctrl_info_ptr->param.is.tx_squelch)         ||
                (lane_ctrl_info_ptr->param.is.tx_electric_idle)   ||
                (lane_ctrl_info_ptr->param.is.rx_suspend_resume)  ||
                (lane_ctrl_info_ptr->param.is.tx_suspend_resume)  ||
                (lane_ctrl_info_ptr->param.is.rx_afe_power)       ||
                (lane_ctrl_info_ptr->param.is.tx_afe_power)       ||
                (lane_ctrl_info_ptr->param.is.ignore_fault))       
            {
                return_result = RR_SUCCESS;
            }
            else {
                CAPI_LOG_ERROR("Unsupported parameters\r\n");
                return_result = RR_ERROR_WRONG_INPUT_VALUE;
            }
        } else if (CHK_FW_OUTPUT == chk_type) {
            if ((util_check_param_range(lane_ctrl_info_ptr->cmd_value.is.rx_datapath_power, 0, 1))  || /* Can't return TOGGLE */
                (util_check_param_range(lane_ctrl_info_ptr->cmd_value.is.tx_datapath_power, 0, 1))  || /* Can't return TOGGLE */
                (util_check_param_range(lane_ctrl_info_ptr->cmd_value.is.rx_data_path, 0, 1))       ||
                (util_check_param_range(lane_ctrl_info_ptr->cmd_value.is.tx_data_path, 0, 1))       ||
                (util_check_param_range(lane_ctrl_info_ptr->cmd_value.is.rx_lane_cfg_reset, 0, 1))  ||
                (util_check_param_range(lane_ctrl_info_ptr->cmd_value.is.tx_lane_cfg_reset, 0, 1))  ||
                (util_check_param_range(lane_ctrl_info_ptr->cmd_value.is.rx_squelch,        0, 1))  ||
                (util_check_param_range(lane_ctrl_info_ptr->cmd_value.is.tx_squelch,        0, 1))  ||
                (util_check_param_range(lane_ctrl_info_ptr->cmd_value.is.tx_electric_idle,  0, 1))  ||
                (util_check_param_range(lane_ctrl_info_ptr->cmd_value.is.rx_suspend_resume, 0, 1))  ||
                (util_check_param_range(lane_ctrl_info_ptr->cmd_value.is.tx_suspend_resume, 0, 1))  ||
                (util_check_param_range(lane_ctrl_info_ptr->cmd_value.is.rx_afe_power,      0, 1))  || /* Can't return TOGGLE */
                (util_check_param_range(lane_ctrl_info_ptr->cmd_value.is.tx_afe_power,      0, 1))  || /* Can't return TOGGLE */
                (util_check_param_range(lane_ctrl_info_ptr->cmd_value.is.ignore_fault,      0, 1)))       
            {
                return_result = RR_SUCCESS;
            }
            else {
                CAPI_LOG_ERROR("Unsupported parameters\r\n");
                return_result = RR_ERROR_WRONG_OUTPUT_VALUE;
            }
        } else {
            return_result = RR_ERROR; /* What is the correct error type in this case? */
        }
    } else {
        CAPI_LOG_ERROR("Core ip is incorrect: %d\r\n", phy_info_ptr->core_ip);
        return_result = RR_ERROR_WRONG_CORE_IP_VALUE;
    }

    return return_result;
}

/**
 * @brief      sanity_checker_set_lane_ctrl_info(capi_phy_info_t* phy_info_ptr, capi_lane_ctrl_info_t* lane_ctrl_info_ptr)
 * @details    This API is used to sanity check lane control info parameters
 *
 * @param[in]  phy_info_ptr         : this parameter
 * @param[in]  lane_ctrl_info_t: this parameter
 *
 * @return     returns the performance result of the called methode/function
 */
return_result_t sanity_checker_set_lane_ctrl_info(capi_phy_info_t* phy_info_ptr, capi_lane_ctrl_info_t* lane_ctrl_info_ptr)
{
    return_result_t return_result = RR_SUCCESS;

    if (phy_info_ptr->core_ip == CORE_IP_HOST_SERDES || phy_info_ptr->core_ip == CORE_IP_MEDIA_SERDES ||
        phy_info_ptr->core_ip == CORE_IP_HOST_DSP || phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP) {
            if ((lane_ctrl_info_ptr->param.is.ignore_fault)       ||
                (lane_ctrl_info_ptr->param.is.tx_electric_idle)   ||
                (lane_ctrl_info_ptr->param.is.rx_datapath_power)  ||
                (lane_ctrl_info_ptr->param.is.tx_datapath_power)  ||
                (lane_ctrl_info_ptr->param.is.rx_data_path)       ||
                (lane_ctrl_info_ptr->param.is.tx_data_path)       ||
                (lane_ctrl_info_ptr->param.is.rx_lane_cfg_reset)  ||
                (lane_ctrl_info_ptr->param.is.tx_lane_cfg_reset)  ||
                (lane_ctrl_info_ptr->param.is.rx_squelch)         ||
                (lane_ctrl_info_ptr->param.is.tx_squelch)         ||
                (lane_ctrl_info_ptr->param.is.rx_suspend_resume)  ||
                (lane_ctrl_info_ptr->param.is.tx_suspend_resume)  ||
                (lane_ctrl_info_ptr->param.is.rx_afe_power)       ||
                (lane_ctrl_info_ptr->param.is.tx_afe_power))
            {
                return_result = RR_SUCCESS;
            }
            else {
                CAPI_LOG_ERROR("Unsupported parameters\r\n");
                return_result = RR_ERROR_WRONG_INPUT_VALUE;
            }
    } 
    else {
        CAPI_LOG_ERROR("Core ip is incorrect: %d\r\n", phy_info_ptr->core_ip);
        return_result = RR_ERROR_WRONG_CORE_IP_VALUE;
    }
    return return_result;
}

#ifdef CAPI_CONFIG_CMD_SANITY_CHK_ENABLE
return_result_t txfir_sanity_checker(capi_phy_info_t* phy_info_ptr, const txfir_info_t *txfir_ptr)
{
    int16_t txfir_sum;
    int16_t txfir_max_value = 168;
    return_result_t return_result = RR_ERROR_WRONG_OUTPUT_VALUE;
    core_ip_t core_ip = phy_info_ptr->core_ip;
    switch (txfir_ptr->numb_of_taps) {
        /* coverity[unterminated_case] */
        case TXFIR_TAPS_NRZ_LP_3TAP:
            txfir_max_value = 127;
        case TXFIR_TAPS_PAM4_LP_3TAP:
            if ((core_ip == CORE_IP_HOST_SERDES) ||
                (core_ip == CORE_IP_MEDIA_SERDES)) {                
                txfir_sum = abs(txfir_ptr->tap[1]) +
                            abs(txfir_ptr->tap[2]) +
                            abs(txfir_ptr->tap[3]);

                if ((abs(txfir_ptr->tap[1]) <= txfir_max_value) &&
                    (abs(txfir_ptr->tap[2]) <= txfir_max_value) &&
                    (abs(txfir_ptr->tap[3]) <= txfir_max_value) &&
                    (txfir_sum              <= txfir_max_value)) 
                {
                    return_result = RR_SUCCESS;
                }
            }
            break;
            /* coverity[unterminated_case] */
        case TXFIR_TAPS_NRZ_6TAP:
            txfir_max_value = 127;
        case TXFIR_TAPS_PAM4_6TAP:
            if ((core_ip == CORE_IP_HOST_SERDES) ||
                (core_ip == CORE_IP_MEDIA_SERDES)) {  
                txfir_sum = abs(txfir_ptr->tap[0]) +
                            abs(txfir_ptr->tap[1]) +
                            abs(txfir_ptr->tap[2]) +
                            abs(txfir_ptr->tap[3]) +
                            abs(txfir_ptr->tap[4]) +
                            abs(txfir_ptr->tap[5]);
        
                if ((abs(txfir_ptr->tap[0])  <= txfir_max_value)                         &&
                    (abs(txfir_ptr->tap[1])  <= txfir_max_value)                         &&
                    (abs(txfir_ptr->tap[2])  <= txfir_max_value)                         &&
                    (abs(txfir_ptr->tap[3])  <= txfir_max_value)                         &&
                    (abs(txfir_ptr->tap[4])  <= txfir_max_value)                         &&
                    (abs(txfir_ptr->tap[5])  <= txfir_max_value)                         &&
                    (txfir_sum               <= txfir_max_value)) 
                {
                    return_result = RR_SUCCESS;
                }
            }                        
            break;
        case TXFIR_TAPS_4TAP:
            if ((core_ip == CORE_IP_HOST_DSP) ||
                (core_ip == CORE_IP_MEDIA_DSP)) {  
                txfir_sum = abs(txfir_ptr->tap[1]) +
                    abs(txfir_ptr->tap[2]) +
                    abs(txfir_ptr->tap[3]) +
                    abs(txfir_ptr->tap[4]);
        
                if ((abs(txfir_ptr->tap[1])  <= txfir_max_value)                         &&
                    (abs(txfir_ptr->tap[2])  <= txfir_max_value)                         &&
                    (abs(txfir_ptr->tap[3])  <= txfir_max_value)                         &&
                    (abs(txfir_ptr->tap[4])  <= txfir_max_value)                         &&
                    (txfir_sum               <= txfir_max_value)) 
                {
                    return_result =  RR_SUCCESS;
                }
            }
            break;
        case TXFIR_TAPS_7TAP:
            if ((core_ip == CORE_IP_HOST_DSP) ||
                (core_ip == CORE_IP_MEDIA_DSP)) {  
                txfir_sum = abs(txfir_ptr->tap[0]) +
                            abs(txfir_ptr->tap[1]) +
                            abs(txfir_ptr->tap[2]) +
                            abs(txfir_ptr->tap[3]) +
                            abs(txfir_ptr->tap[4]) +
                            abs(txfir_ptr->tap[5]) +
                            abs(txfir_ptr->tap[6]);
        
                if ((abs(txfir_ptr->tap[0])  <= txfir_max_value)                         &&
                    (abs(txfir_ptr->tap[1])  <= txfir_max_value)                         &&
                    (abs(txfir_ptr->tap[2])  <= txfir_max_value)                         &&
                    (abs(txfir_ptr->tap[3])  <= txfir_max_value)                         &&
                    (abs(txfir_ptr->tap[4])  <= txfir_max_value)                         &&
                    (abs(txfir_ptr->tap[5])  <= txfir_max_value)                         &&
                    (abs(txfir_ptr->tap[6])  <= txfir_max_value)                         &&
                    (txfir_sum               <= txfir_max_value)) 
                {
                    return_result =  RR_SUCCESS;
                }
            }              
            break;
        case TXFIR_TAPS_LIN10TAP:
        case TXFIR_TAPS_NONLIN10TAP:
        case TXFIR_TAPS_NONLINWFTAP:
        {
            capi_phy_info_t capi_phy = {0};
            uint8_t txfir_idx;
            
            util_memcpy((void *)&capi_phy, phy_info_ptr, sizeof(capi_phy_info_t));
            txfir_max_value = 168;
            txfir_sum = 0;
            for(txfir_idx=0; txfir_idx<13; txfir_idx++){
                txfir_sum += abs(txfir_ptr->tap[txfir_idx]);
                if(abs(txfir_ptr->tap[txfir_idx]) > txfir_max_value)
                    break;
            }
            if(txfir_sum               <= txfir_max_value)
                return_result =  RR_SUCCESS;

        }
            break;
        default:
            break;
    }
    return return_result;
}

/**
 * @brief      sanity_checker_lane_tx_info(capi_phy_info_t* phy_info_ptr,
 *                                         lane_tx_info_t*  lane_tx_info_ptr,
 *                                         uint8_t          set_get_request,
 *                                         sanity_chk_t     chk_type)
 * @details    This API is used to sanity check lane Tx information params
 *
 * @param[in]  phy_info_ptr: reference to the phy information object
 * @param[in]  lane_tx_info_ptr : reference to the lane Tx information object
 * @param[in]  set_get_request: set or get capi request
 * @param[in]  chk_type    : specifies user input of FW output check type
 *
 * @return     returns the performance result of the called methode/function
 */
static return_result_t sanity_checker_lane_tx_info(capi_phy_info_t* phy_info_ptr,
                                                   lane_tx_info_t*  lane_tx_info_ptr,
                                                   uint8_t          set_get_request,
                                                   sanity_chk_t     chk_type)
{
    return_result_t return_result = RR_SUCCESS;

    if (!lane_tx_info_ptr->param.content) return(RR_ERROR_NOT_INITIALIZED);

    if ((phy_info_ptr->core_ip == CORE_IP_ALL) ||
        (phy_info_ptr->core_ip == CORE_IP_CW)  ||
        (phy_info_ptr->core_ip == CORE_IP_KP4_KR4_FEC_DEC)) {
        CAPI_LOG_ERROR("Wrong Core IP : %d  \r\n", RR_ERROR_WRONG_CORE_IP_VALUE);
        return(RR_ERROR_WRONG_CORE_IP_VALUE);
    }

    if ((phy_info_ptr->core_ip == CORE_IP_HOST_SERDES) ||
        (phy_info_ptr->core_ip == CORE_IP_MEDIA_SERDES)) {
        if (lane_tx_info_ptr->param.is.dsp_level_shift ||
            lane_tx_info_ptr->param.is.dsp_graycode    ||
            lane_tx_info_ptr->param.is.dsp_high_swing  ||
            lane_tx_info_ptr->param.is.dsp_precode) {
            CAPI_LOG_ERROR("Wrong Core IP : %d  \r\n", RR_ERROR_WRONG_CORE_IP_VALUE);
            return(RR_ERROR_WRONG_CORE_IP_VALUE);
        }
    }

    if ((phy_info_ptr->core_ip == CORE_IP_HOST_DSP) ||
        (phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP)) {
        if (lane_tx_info_ptr->param.is.txfir != lane_tx_info_ptr->param.is.dsp_level_shift) {
            CAPI_LOG_ERROR("DSP TXFIR and LEVEL SHIFT must be accessed same time : %d \r\n", RR_ERROR_WRONG_INPUT_VALUE);
            return(RR_ERROR_WRONG_INPUT_VALUE);
        }
    }

    if ((CHK_USR_INPUT == chk_type) && (set_get_request == GET_CONFIG)) {
        /*  */
    } else {
        if (lane_tx_info_ptr->param.is.txfir) {
            if (SET_CONFIG == set_get_request) { /*User input txfir during a get is empty*/
                if (txfir_sanity_checker(phy_info_ptr, &lane_tx_info_ptr->value.txfir)) {
                        CAPI_LOG_ERROR("Wrong txfir! \r\n");
                    return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
                }
            }
        }

        if (lane_tx_info_ptr->param.is.hs_dac_cur) {
            if (!(util_check_param_range(lane_tx_info_ptr->value.hs_dac_cur, 0, 4)) && phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP) {
                CAPI_LOG_ERROR("Wrong parameter hs_dac_cur : %d core_ip %d \r\n",
                                lane_tx_info_ptr->value.hs_dac_cur, phy_info_ptr->core_ip);
                return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
            }
        }

        if (lane_tx_info_ptr->param.is.hs_ap) {
            if (!(util_check_param_range(lane_tx_info_ptr->value.hs_ap, 0, 15) && phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP)) {
                CAPI_LOG_ERROR("Wrong parameter hs_ap : %d core_ip %d \r\n",
                                lane_tx_info_ptr->value.hs_ap, phy_info_ptr->core_ip);
                return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
            }
        }
        if (lane_tx_info_ptr->param.is.hs_apb) {
            if (!(util_check_param_range(lane_tx_info_ptr->value.hs_apb, 0, 15) && phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP)) {
                CAPI_LOG_ERROR("Wrong parameter hs_apb : %d core_ip %d \r\n",
                                lane_tx_info_ptr->value.hs_apb, phy_info_ptr->core_ip);
                return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
            }
        }
        if (lane_tx_info_ptr->param.is.hs_cp) {
            if (!(util_check_param_range(lane_tx_info_ptr->value.hs_cp, 0, 15) && phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP)) {
                CAPI_LOG_ERROR("Wrong parameter hs_cp : %d core_ip %d \r\n",
                                lane_tx_info_ptr->value.hs_cp, phy_info_ptr->core_ip);
                return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
            }
        }
        if (lane_tx_info_ptr->param.is.hs_cpb) {
            if (!(util_check_param_range(lane_tx_info_ptr->value.hs_cpb, 0, 15) && phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP)) {
                CAPI_LOG_ERROR("Wrong parameter hs_cpb : %d core_ip %d \r\n",
                                lane_tx_info_ptr->value.hs_cpb, phy_info_ptr->core_ip);
                return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
            }
        }
        if (lane_tx_info_ptr->param.is.hs_pon_dac) {
            if (!(util_check_param_range(lane_tx_info_ptr->value.hs_pon_dac, 0, 7) && phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP)) {
                CAPI_LOG_ERROR("Wrong parameter hs_pon_dac : %d core_ip %d \r\n",
                                lane_tx_info_ptr->value.hs_pon_dac, phy_info_ptr->core_ip);
                return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
            }
        }

        if (lane_tx_info_ptr->param.is.symbol_swap) {
            if (!util_check_param_range(lane_tx_info_ptr->value.symbol_swap, 0, 1)) {
                CAPI_LOG_ERROR("Wrong parameter symbol_swap : %d \r\n",
                                lane_tx_info_ptr->value.symbol_swap);
                return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
            }
        }

        if (lane_tx_info_ptr->param.is.dsp_level_shift) {
            #if CAPI_CONFIG_ARCHIVED
            int16_t coef[24];
            if (!util_lw_tx_fir_lut_validate_coef_and_lvl(phy_info_ptr,
                                                            coef,
                                                            lane_tx_info_ptr->value.level_shift)) {
                CAPI_LOG_ERROR("Wrong parameter dsp_level_shift : %d \r\n",
                                lane_tx_info_ptr->value.dsp_level_shift);
                return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
            }
            #endif
        }

        if (lane_tx_info_ptr->param.is.dsp_graycode) {
            if (!util_check_param_range(lane_tx_info_ptr->value.dsp_graycode, 0, 1)) {
                CAPI_LOG_ERROR("Wrong parameter dsp_graycode : %d \r\n",
                                lane_tx_info_ptr->value.dsp_graycode);
                return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
            }
        }

        if (lane_tx_info_ptr->param.is.dsp_high_swing) {
            if (!util_check_param_range(lane_tx_info_ptr->value.dsp_high_swing, 0, 1)) {
                CAPI_LOG_ERROR("Wrong parameter dsp_high_swing : %d \r\n",
                                lane_tx_info_ptr->value.dsp_high_swing);
                return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
            }
        }

        if (lane_tx_info_ptr->param.is.dsp_precode) {
            if (!util_check_param_range(lane_tx_info_ptr->value.dsp_precode,
                                        CAPI_LW_INDEP_TX_PRECODE_DEFAULT,
                                        CAPI_LW_INDEP_TX_PRECODE_ON)) {
                CAPI_LOG_ERROR("Wrong parameter dsp_precode : %d \r\n",
                                lane_tx_info_ptr->value.dsp_precode);
                return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
            }
        }
    
        if (lane_tx_info_ptr->param.is.tx_clk_filter_bw) {
            if (!(util_check_param_range(lane_tx_info_ptr->value.dsp_tx_clk_filter_bw, 0, 16))) {
                CAPI_LOG_ERROR("Wrong parameter dsp_tx_clk_filter_bw : %d  \r\n",
                                lane_tx_info_ptr->value.dsp_tx_clk_filter_bw);
                return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
            }
        }
        if (lane_tx_info_ptr->param.is.tx_clk_filter2) {
            if (!(util_check_param_range(lane_tx_info_ptr->value.dsp_tx_clk_filter2.tx_clk_filter2_bw, 0, 16)) ||
                !(util_check_param_range(lane_tx_info_ptr->value.dsp_tx_clk_filter2.enable, 0, 1))) {
                CAPI_LOG_ERROR("Wrong parameter dsp_tx_clk_filter2 : enable %d tx_clk_filter2_bw %d \r\n",
                                lane_tx_info_ptr->value.dsp_tx_clk_filter2.enable,
                                lane_tx_info_ptr->value.dsp_tx_clk_filter2.tx_clk_filter2_bw);
                return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
            }
        }
        if (lane_tx_info_ptr->param.is.tx_bias) {
            if (!(util_check_param_range(lane_tx_info_ptr->value.dsp_tx_bias.tx_bias_reg1, 0, 15)) ||
                !(util_check_param_range(lane_tx_info_ptr->value.dsp_tx_bias.tx_bias_reg2, 0, 15)) ||
                !(util_check_param_range(lane_tx_info_ptr->value.dsp_tx_bias.tx_bias_reg3, 0, 15)) ||
                !(util_check_param_range(lane_tx_info_ptr->value.dsp_tx_bias.tx_bias_reg4, 0, 15)) ) {
                CAPI_LOG_ERROR("Wrong parameter tx_bias : tx_bias_reg1 %d tx_bias_reg2 %d  tx_bias_reg3 %d tx_bias_reg4 %d\r\n",
                                lane_tx_info_ptr->value.dsp_tx_bias.tx_bias_reg1,
                                lane_tx_info_ptr->value.dsp_tx_bias.tx_bias_reg2,
                                lane_tx_info_ptr->value.dsp_tx_bias.tx_bias_reg3,
                                lane_tx_info_ptr->value.dsp_tx_bias.tx_bias_reg4);
                return_result = (chk_type == CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE;
            }
            if( phy_info_ptr->core_ip != CORE_IP_MEDIA_DSP && chk_type == CHK_USR_INPUT ){
                CAPI_LOG_ERROR("Wrong core IP %d, feature is not supported \r\n", phy_info_ptr->core_ip);
                return_result  = RR_ERROR_WRONG_INPUT_VALUE;
            }
        }
    }
    return(return_result);
}
#endif /* CAPI_CONFIG_CMD_SANITY_CHK_ENABLE */

static unsigned int intf_count_set_bits(uint32_t n)
{
    unsigned int count = 0;
    while (n)
    {
      n &= (n-1) ;
      count++;
    }
    return count;
}


static bool validate_ports(capi_phy_info_t* phy_info_ptr, capi_config_info_t* config_info_ptr, cw_chip_mode_t port_type)
{
    bool port_valid = false;
    int i;
    uint8_t port_line_mask, port_host_mask;
    uint8_t mode_table_index = port_type -1;
    /* Total no.of ports in this mode */
    int total_ports_in_mode = intf_count_set_bits(cw_cmode[mode_table_index].cw_chip_port_mask);
    int port_count = total_ports_in_mode;
    uint8_t total_line_mask =0, total_host_mask=0;
    /* If total no. of ports are between 1 and max ports else invalid port */
    if( total_ports_in_mode >= 1 && total_ports_in_mode <= CAPI_MAX_PORTS ){
        /* Iterate through the port slot for this mode entry in the mode table */
        for(i=0; i < CAPI_MAX_PORTS;i++){
            /* Extract the port from the user input */
            port_line_mask = cw_cmode[mode_table_index].cw_chip_lw_lane_mask[i] & config_info_ptr->line_lane.lane_mask;
            port_host_mask = cw_cmode[mode_table_index].cw_chip_bh_lane_mask[i] & config_info_ptr->host_lane.lane_mask;

            /* If this slot contains the port mask else go to the next one */
            if(!(cw_cmode[mode_table_index].cw_chip_lw_lane_mask[i] == 0 &&
                 cw_cmode[mode_table_index].cw_chip_bh_lane_mask[i] == 0)){

                /* user config wants to skip this port */
                if(!(port_line_mask == 0 && port_host_mask == 0)){
                    /* If the user input contains this port then it is valid else not valid */
                    if( port_host_mask == cw_cmode[mode_table_index].cw_chip_bh_lane_mask[i] &&
                        port_line_mask == cw_cmode[mode_table_index].cw_chip_lw_lane_mask[i]){
                        total_line_mask |= port_line_mask;
                        total_host_mask |= port_host_mask;
                        port_valid = true;
                       } else { /* encountered a bad port so no point to continue */
                         port_valid = false;
                         break;
                    }
                }
                /* Decrement the port count here */
                port_count--;
                /* Are we done going over all the ports */
                if(!port_count){
                    break;
                }
            }
        }
    }
    if(port_valid){
        /* The user mask is not the same as the sum total of all the port masks for this mode */
       if( total_line_mask != config_info_ptr->line_lane.lane_mask ||
          total_host_mask != config_info_ptr->host_lane.lane_mask ){
          port_valid = false;
        }
    }
    (void)phy_info_ptr;
    return port_valid;
}



/**
 * @brief      sanity_checker_get_config_info(capi_phy_info_t *phy_info_ptr, capi_config_info_t* config_info_ptr, sanity_chk_t chk_type)
 * @details    This API is used to sanity check get config info params
 *
 * @param[in]  phy_info_ptr: this parameter
 * @param[in]  config_info_ptr: this parameter
 * @param[in]  chk_type: this parameter
 *
 * @return     returns the performance result of the called method/function
 */
return_result_t sanity_checker_get_config_info(capi_phy_info_t *phy_info_ptr, capi_config_info_t* config_info_ptr, sanity_chk_t chk_type)
{
    uint8_t port_idx;
    cw_chip_mode_t port_type = CHIP_MODES_NONE;

    /* Validate phy info and config parameter*/
    /* Do not check fw output since fw will zero out user input per request */
    if (chk_type == CHK_USR_INPUT) {
        if (!(config_info_ptr->host_lane.lane_mask || config_info_ptr->line_lane.lane_mask)) {
            CAPI_LOG_ERROR("Invalid lane mask. Host lane mask %x, Line lane mask %x", config_info_ptr->host_lane.lane_mask, config_info_ptr->line_lane.lane_mask);
            return RR_ERROR_WRONG_INPUT_VALUE;
        }
    } else {
        if (CAPI_PORT_CONFIG_STATUS_SUCCESS == config_info_ptr->status) {
            for (port_idx=0; port_idx < port_total_entries; port_idx++)
            {
                if ((ports[port_idx].func_mode         == config_info_ptr->func_mode) &&
                    (ports[port_idx].lw_cfg.mod == config_info_ptr->line_lane.modulation) &&
                    (ports[port_idx].bh_cfg.mod == config_info_ptr->host_lane.modulation) &&
                    (ports[port_idx].lw_cfg.br  == config_info_ptr->lw_br) &&
                    (ports[port_idx].bh_cfg.br  == config_info_ptr->bh_br) &&
                    validate_ports(phy_info_ptr, config_info_ptr, ports[port_idx].port_type))
                {
                   port_type = ports[port_idx].port_type;
                   break;
                }
            }
            if (CHIP_MODES_NONE == port_type) {
                return RR_ERROR;
            }
        }
    }
    return RR_SUCCESS;
}

/**
 * @brief      sanity_checker_set_config_info(capi_phy_info_t *phy_info_ptr, capi_config_info_t* config_info_ptr, sanity_chk_t chk_type)
 * @details    This API is used to sanity check set config info params
 *
 * @param[in]  phy_info_ptr: this parameter
 * @param[in]  config_info_ptr: this parameter
 * @param[in]  chk_type: this parameter
 *
 * @return     returns the performance result of the called method/function
 */
return_result_t sanity_checker_set_config_info(capi_phy_info_t *phy_info_ptr, capi_config_info_t* config_info_ptr, sanity_chk_t chk_type)
{
    (void)chk_type;
    uint8_t port_idx;
    cw_chip_mode_t port_type = CHIP_MODES_NONE;

    /*validate phy info and config parameter*/
    if (!(config_info_ptr->host_lane.lane_mask || config_info_ptr->line_lane.lane_mask)) {
        CAPI_LOG_ERROR("Invalid lane mask. Host lane mask %x, Line lane mask %x", config_info_ptr->host_lane.lane_mask, config_info_ptr->line_lane.lane_mask);
        return RR_ERROR_WRONG_INPUT_VALUE;
    }

    /* only do this for set config */
    if (config_info_ptr->ref_clk != CAPI_REF_CLK_FRQ_156_25_MHZ_ETHERNET)
    {
        CAPI_LOG_ERROR("Wrong input ref clock %d \n", config_info_ptr->ref_clk);
        return(RR_ERROR_WRONG_INPUT_VALUE);
    }

    for (port_idx=0; port_idx < port_total_entries; port_idx++)
    {
        if ((ports[port_idx].func_mode         == config_info_ptr->func_mode) &&
            (ports[port_idx].lw_cfg.mod == config_info_ptr->line_lane.modulation) &&
            (ports[port_idx].bh_cfg.mod == config_info_ptr->host_lane.modulation) &&
            (ports[port_idx].lw_cfg.br  == config_info_ptr->lw_br) &&
            (ports[port_idx].bh_cfg.br  == config_info_ptr->bh_br) &&
            validate_ports(phy_info_ptr, config_info_ptr, ports[port_idx].port_type))
        {
            port_type = ports[port_idx].port_type;
            break;
        }
    }

    /* do not validate config info input for small CAPI to reduce code size */
    if (port_type == CHIP_MODES_NONE){
       CAPI_LOG_ERROR("Wrong configure info for the mode \n");
       return RR_ERROR_WRONG_INPUT_VALUE;
    }

    return RR_SUCCESS;
}

/**
 * @brief      sanity_checker_set_port_info(capi_phy_info_t *phy_info_ptr, capi_port_info_t* port_info_ptr, sanity_chk_t chk_type)
 * @details    This API is used to sanity check set config info params
 *
 * @param[in]  phy_info_ptr: this parameter
 * @param[in]  config_info_ptr: this parameter
 * @param[in]  chk_type: this parameter
 *
 * @return     returns the performance result of the called method/function
 */
static return_result_t sanity_checker_set_port_info(capi_phy_info_t *phy_info_ptr, capi_port_info_t* port_info_ptr, sanity_chk_t chk_type)
{
    (void)chk_type;
    uint8_t port_idx;
    capi_config_info_t config_info;

    if (port_info_ptr->param.is.add_port) {
        config_info.host_lane.lane_mask = (port_info_ptr->lane_mask >> 16); 
        config_info.line_lane.lane_mask = (port_info_ptr->lane_mask & 0xFF); 

        if (!(config_info.host_lane.lane_mask || config_info.line_lane.lane_mask)) {
            CAPI_LOG_ERROR("Invalid lane mask. Host lane mask %x, Line lane mask %x", config_info.host_lane.lane_mask, config_info.line_lane.lane_mask);
            return RR_ERROR_WRONG_INPUT_VALUE;
        }
        if (port_info_ptr->param.is.phy_side != PHY_BOTH_SIDES)
        {
            CAPI_LOG_ERROR("Wrong phy_side %d \n", port_info_ptr->param.is.phy_side);
            return(RR_ERROR_WRONG_INPUT_VALUE);
        }
        if (port_info_ptr->port.ref_clk != CAPI_REF_CLK_FRQ_156_25_MHZ_ETHERNET)
        {
            CAPI_LOG_ERROR("Wrong ref clock %d \n", port_info_ptr->port.ref_clk);
            return(RR_ERROR_WRONG_INPUT_VALUE);
        }
        for (port_idx=0; port_idx < port_total_entries; port_idx++) {
            if (ports[port_idx].port_type  == port_info_ptr->port.mode)
               break;
        }
        if (port_idx == port_total_entries){
           CAPI_LOG_ERROR("Wrong chip mode %d \n", port_info_ptr->port.mode);
           return RR_ERROR_WRONG_INPUT_VALUE;
        }
        if (!validate_ports(phy_info_ptr, &config_info, port_info_ptr->port.mode)){
           CAPI_LOG_ERROR("Wrong port info to add the port: mode %d lane_mask 0x%x 0x%x \n", port_info_ptr->port.mode, config_info.host_lane.lane_mask, config_info.line_lane.lane_mask);
           return RR_ERROR_WRONG_INPUT_VALUE;
        }
        if (port_info_ptr->port.egr_fec_mode >= CAPI_FORWARD_ERROR_CORR_MODE_MAX ||  port_info_ptr->port.igr_fec_mode >= CAPI_FORWARD_ERROR_CORR_MODE_MAX){
           CAPI_LOG_ERROR("Wrong FEC mode %d %d\n", port_info_ptr->port.egr_fec_mode, port_info_ptr->port.igr_fec_mode);
           return RR_ERROR_WRONG_INPUT_VALUE;
        }
    }

    return RR_SUCCESS;
}


/**
 * @brief      sanity_checker_get_port_info(capi_phy_info_t *phy_info_ptr, capi_port_info_t* port_info_ptr, sanity_chk_t chk_type)
 * @details    This API is used to sanity check get port info params
 *
 * @param[in]  phy_info_ptr: this parameter
 * @param[in]  port_info_ptr: this parameter
 * @param[in]  chk_type: this parameter
 *
 * @return     returns the performance result of the called method/function
 */
static return_result_t sanity_checker_get_port_info(capi_phy_info_t *phy_info_ptr, capi_port_info_t* port_info_ptr, sanity_chk_t chk_type)
{
    capi_config_info_t config_info;

    if (chk_type == CHK_USR_INPUT) {
        if (port_info_ptr->lane_mask == 0) {
            CAPI_LOG_ERROR("Invalid lane mask 0x%x", port_info_ptr->lane_mask);
            return RR_ERROR_WRONG_INPUT_VALUE;
        }
    } else if (chk_type == CHK_FW_OUTPUT) {    
        if (CAPI_PORT_CONFIG_STATUS_SUCCESS == port_info_ptr->port.status) {
            if (port_info_ptr->port.mode == CHIP_MODES_NONE || 
                port_info_ptr->port.mode >= CHIP_MODE_MAX_MODE ||
                port_info_ptr->port.egr_fec_mode >= CAPI_FORWARD_ERROR_CORR_MODE_MAX ||
                port_info_ptr->port.igr_fec_mode >= CAPI_FORWARD_ERROR_CORR_MODE_MAX) {
                CAPI_LOG_ERROR("Wrong port info, mode %d egr_fec_mode %d igr_fec_mode %d\n", 
                    port_info_ptr->port.mode,
                    port_info_ptr->port.egr_fec_mode,
                    port_info_ptr->port.igr_fec_mode);
                return RR_ERROR_WRONG_INPUT_VALUE;
            }
            config_info.host_lane.lane_mask = (port_info_ptr->lane_mask >> 16); 
            config_info.line_lane.lane_mask = (port_info_ptr->lane_mask & 0xFF); 
            if (!validate_ports(phy_info_ptr, &config_info, port_info_ptr->port.mode)) {
               CAPI_LOG_ERROR("Wrong port info, lane mask 0x%x mode %d\n", port_info_ptr->lane_mask, port_info_ptr->port.mode);
               return RR_ERROR_WRONG_INPUT_VALUE;
            }
        }
    }
    return RR_SUCCESS;
}



/**
 * @brief       sanity_checker_mpi_config_info(capi_phy_info_t *phy_info_ptr, dsp_mpi_cfg_info_t* mpi_info_ptr, sanity_chk_t chk_type)
 * @details    This API is used to sanity check MPI config info params
 *
 * @param[in]  phy_info_ptr: this parameter
 * @param[in]  mpi_info_ptr: this parameter
 * @param[in]  chk_type: this parameter
 *
 * @return     returns the performance result of the called method/function
 */
return_result_t sanity_checker_mpi_config_info(capi_phy_info_t *phy_info_ptr, dsp_mpi_cfg_info_t* mpi_info_ptr, sanity_chk_t chk_type)
{
    if(phy_info_ptr->core_ip != CORE_IP_MEDIA_DSP ||
        util_check_param_range(mpi_info_ptr->mpi_enable, 0, 1) == FALSE ||
        util_check_param_range(mpi_info_ptr->mpi_interval, 0, 15) == FALSE ||
        (mpi_info_ptr->ratio_threshold!=0 && (mpi_info_ptr->ratio_threshold<0)))
        return ((chk_type== CHK_USR_INPUT)? RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE);
    return RR_SUCCESS;
}



/**
 * @brief      sanity_checker_set_lw_rclk_info(capi_phy_info_t *phy_info_ptr, capi_recovered_clock_info_t* rclk_info_ptr, sanity_chk_t chk_type)
 * @details    This API is used to sanity check set lw rclk info params
 *
 * @param[in]  phy_info_ptr: this parameter
 * @param[in]  rclk_info_ptr: this parameter
 * @param[in]  chk_type: this parameter
 *
 * @return     returns the performance result of the called method/function
 */
static return_result_t sanity_checker_set_lw_rclk_info(capi_phy_info_t *phy_info_ptr, capi_recovered_clock_info_t* rclk_info_ptr, sanity_chk_t chk_type)
{
    /*validate phy info and rclk_info parameter*/
    if (chk_type == CHK_USR_INPUT) {
        /* only brings out Lane 7 Recovered Clock */
        if(rclk_info_ptr->command.is.config_analog_pin_0){
            if (phy_info_ptr->lane_mask != 0x80
                || rclk_info_ptr->value.config_analog_pin[0].div_ratio > CAPI_RCLK_DIV_RATIO_128
                || (rclk_info_ptr->value.config_analog_pin[0].rclk_type != CAPI_RCLK_80T && rclk_info_ptr->value.config_analog_pin[0].rclk_type != CAPI_RCLK_64T)
                || util_check_param_range(rclk_info_ptr->value.config_analog_pin[0].enable, 0, 1)==FALSE)
                return (RR_ERROR_WRONG_INPUT_VALUE);
        }
        if(rclk_info_ptr->command.is.config_vco_pin){
            if (util_check_param_range(rclk_info_ptr->value.config_vco_pin.enable, 0, 1)==FALSE)
                return (RR_ERROR_WRONG_INPUT_VALUE);
        }
    }
    return RR_SUCCESS;
}



/**
 * @brief      sanity_checker_get_lw_rclk_info(capi_phy_info_t *phy_info_ptr, capi_recovered_clock_info_t* rclk_info_ptr, sanity_chk_t chk_type)
 * @details    This API is used to sanity check get lw rclk info params
 *
 * @param[in]  phy_info_ptr: this parameter
 * @param[in]  rclk_info_ptr: this parameter
 * @param[in]  chk_type: this parameter
 *
 * @return     returns the performance result of the called method/function
 */
static return_result_t sanity_checker_get_lw_rclk_info(capi_phy_info_t *phy_info_ptr, capi_recovered_clock_info_t* rclk_info_ptr, sanity_chk_t chk_type)
{
    /*validate phy info and rclk_info parameter*/
    if (chk_type == CHK_USR_INPUT) {
        /* only brings out Lane 3 Recovered Clock */
        if (rclk_info_ptr->command.is.config_analog_pin_0 && phy_info_ptr->lane_mask != 0x80)
            return (RR_ERROR_WRONG_INPUT_VALUE);
    }
    if (chk_type == CHK_FW_OUTPUT) {
        /* only brings out Lane 7 Recovered Clock */
        if(rclk_info_ptr->command.is.config_analog_pin_0){
            if (phy_info_ptr->lane_mask != 0x80
                || rclk_info_ptr->value.config_analog_pin[0].lane_mask != 0x80
                || rclk_info_ptr->value.config_analog_pin[0].div_ratio > CAPI_RCLK_DIV_RATIO_128
                || (rclk_info_ptr->value.config_analog_pin[0].rclk_type != CAPI_RCLK_80T && rclk_info_ptr->value.config_analog_pin[0].rclk_type != CAPI_RCLK_64T)
                || util_check_param_range(rclk_info_ptr->value.config_analog_pin[0].enable, 0, 1)==FALSE)
                return (RR_ERROR_WRONG_INPUT_VALUE);
        }
        if(rclk_info_ptr->command.is.config_vco_pin){
            if (util_check_param_range(rclk_info_ptr->value.config_vco_pin.enable, 0, 1)==FALSE)
                return (RR_ERROR_WRONG_INPUT_VALUE);
        }
    }
    return RR_SUCCESS;
}


#ifdef CAPI_CONFIG_CMD_SANITY_CHK_ENABLE
/**
 * @brief      sanity_checker_lnktrn_info(capi_phy_info_t*    phy_info_ptr,
 *                                        capi_lnktrn_info_t* lnktrn_info_ptr,
 *                                        uint8_t             set_get_request,
 *                                        sanity_chk_t        chk_type)
 * @details    This API is used to sanity check set lnktrn info params
 *
 * @param[in]  phy_info_ptr: reference to the phy information objecct
 * @param[in]  lnktrn_info_ptr: referencce to the link training info object
 * @param[in]  set_get_request: set or get capi request
 * @param[in]  chk_type: sanity check type, input or output
 *
 * @return     returns the performance result of the called method/function
 */
static return_result_t sanity_checker_lnktrn_info(capi_phy_info_t*    phy_info_ptr,
                                                  capi_lnktrn_info_t* lnktrn_info_ptr,
                                                  uint8_t             set_get_request,
                                                  sanity_chk_t        chk_type)
{
    return_result_t return_result = RR_SUCCESS;

    if (!lnktrn_info_ptr->param.content) return(RR_ERROR_NOT_INITIALIZED);

    if ((phy_info_ptr->core_ip == CORE_IP_ALL) ||
        (phy_info_ptr->core_ip == CORE_IP_CW)  ||
        (phy_info_ptr->core_ip == CORE_IP_KP4_KR4_FEC_DEC)) {
        CAPI_LOG_ERROR("Wrong Core IP : %d  \r\n", RR_ERROR_WRONG_CORE_IP_VALUE);
        return(RR_ERROR_WRONG_CORE_IP_VALUE);
    }
    
        if ((phy_info_ptr->core_ip == CORE_IP_HOST_SERDES) ||
            (phy_info_ptr->core_ip == CORE_IP_MEDIA_SERDES)) {
            if (lnktrn_info_ptr->param.is.dsp_linktrn_timer                ||
                lnktrn_info_ptr->param.is.dsp_disable_lt_rxflt_restart_tx  ||
                lnktrn_info_ptr->param.is.dsp_disable_lt_rxlos_restart_tx  ||               
                lnktrn_info_ptr->param.is.dsp_cl72_auto_pol_en             ||
                lnktrn_info_ptr->param.is.dsp_lnktrn_type                  ||
                lnktrn_info_ptr->param.is.dsp_enable_cl72_cl93_preset_req  ||               
                lnktrn_info_ptr->param.is.dsp_enable_802_3cd_preset_req    ||
                lnktrn_info_ptr->param.is.dsp_lnktrn_cfg) {
                CAPI_LOG_ERROR("Wrong Core IP : %d  \r\n", RR_ERROR_WRONG_CORE_IP_VALUE);
                return(RR_ERROR_WRONG_CORE_IP_VALUE);
            }
        }

        if ((phy_info_ptr->core_ip == CORE_IP_HOST_DSP) ||
            (phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP)) {
            if (lnktrn_info_ptr->param.is.serdes_lnktrn_restart_timeout ||
                lnktrn_info_ptr->param.is.serdes_lnktrn_type) {
                CAPI_LOG_ERROR("Wrong Core IP : %d  \r\n", RR_ERROR_WRONG_CORE_IP_VALUE);
                return(RR_ERROR_WRONG_CORE_IP_VALUE);
            }
        }
        
    if ((CHK_USR_INPUT == chk_type) && (set_get_request == GET_CONFIG)) {
        /*  */
    } else {
        if (lnktrn_info_ptr->param.is.opposite_cdr_first) {
            if (!util_check_param_range(lnktrn_info_ptr->opposite_cdr_first, 0, 1)) {
                CAPI_LOG_ERROR("Invalid opposite_cdr_first value: %d. Should be 0 or 1.\r\n",
                               lnktrn_info_ptr->opposite_cdr_first);
                return_result = RR_ERROR_WRONG_INPUT_VALUE;
            }
        }

        if (lnktrn_info_ptr->param.is.lnktrn_en) {
            if (!util_check_param_range(lnktrn_info_ptr->lnktrn_en, 0, 1)) {
                CAPI_LOG_ERROR("Invalid serdes_lnktrn_en value: %d. Should be 0 or 1.\r\n",
                               lnktrn_info_ptr->lnktrn_en);
                return_result = RR_ERROR_WRONG_INPUT_VALUE;
            }
        }
        
        if (lnktrn_info_ptr->param.is.serdes_lnktrn_restart_timeout) {
            if (!util_check_param_range(lnktrn_info_ptr->value.serdes.lnktrn_restart_timeout, 0, 1)) {
                CAPI_LOG_ERROR("Invalid serdes_lnktrn_restart_timeout value: %d. Should be 0 or 1.\r\n",
                               lnktrn_info_ptr->value.serdes.lnktrn_restart_timeout);
                return_result = RR_ERROR_WRONG_INPUT_VALUE;
            }
        }
        
        if (lnktrn_info_ptr->param.is.serdes_lnktrn_type) {
            if ((set_get_request == SET_CONFIG) ||
                !util_check_param_range(lnktrn_info_ptr->value.serdes.lnktrn_type,
                                        LNKTRN_CL72,
                                        LNKTRN_802_3CD)) {
                CAPI_LOG_ERROR("Invalid serdes_lnktrn_type value: %d    set_get_request: %d \r\n",
                               lnktrn_info_ptr->value.serdes.lnktrn_type,
                               set_get_request);
                return_result = RR_ERROR_WRONG_INPUT_VALUE;
            }
        }

#if CAPI_DSP_PLANNED
        if (lnktrn_info_ptr->param.is.dsp_linktrn_timer) {
            if (!util_check_param_range(lnktrn_info_ptr->value.dsp.linktrn_timer.disable_lt_timeout, 0, 1)) {
                CAPI_LOG_ERROR("Invalid dsp_linktrn_timer value: %d. Should be 0 or 1.\r\n",
                               lnktrn_info_ptr->value.dsp.linktrn_timer.disable_lt_timeout);
                return_result = RR_ERROR_WRONG_INPUT_VALUE;
            }
        }
        
        if (lnktrn_info_ptr->param.is.dsp_restart) {
            if (!util_check_param_range(lnktrn_info_ptr->value.dsp.restart, 0, 1)) {
                CAPI_LOG_ERROR("Invalid dsp_restart value: %d. Should be 0 or 1.\r\n",
                               lnktrn_info_ptr->value.dsp.restart);
                return_result = RR_ERROR_WRONG_INPUT_VALUE;
            }
        }
        
        if (lnktrn_info_ptr->param.is.dsp_disable_lt_rxflt_restart_tx) {
            if (!util_check_param_range(lnktrn_info_ptr->value.dsp.disable_lt_rxflt_restart_tx, 0, 1)) {
                CAPI_LOG_ERROR("Invalid dsp_disable_lt_rxflt_restart_tx value: %d. Should be 0 or 1.\r\n",
                               lnktrn_info_ptr->value.dsp.disable_lt_rxflt_restart_tx);
                return_result = RR_ERROR_WRONG_INPUT_VALUE;
            }
        }
        
        if (lnktrn_info_ptr->param.is.dsp_disable_lt_rxlos_restart_tx) {
            if (!util_check_param_range(lnktrn_info_ptr->value.dsp.disable_lt_rxlos_restart_tx, 0, 1)) {
                CAPI_LOG_ERROR("Invalid dsp_disable_lt_rxlos_restart_tx value: %d. Should be 0 or 1.\r\n",
                               lnktrn_info_ptr->value.dsp.disable_lt_rxlos_restart_tx);
                return_result = RR_ERROR_WRONG_INPUT_VALUE;
            }
        }
        
        if (lnktrn_info_ptr->param.is.dsp_cl72_auto_pol_en) {
            if (!util_check_param_range(lnktrn_info_ptr->value.dsp.cl72_auto_pol_en, 0, 1)) {
                CAPI_LOG_ERROR("Invalid dsp_cl72_auto_pol_en value: %d. Should be 0 or 1.\r\n",
                               lnktrn_info_ptr->value.dsp.cl72_auto_pol_en);
                return_result = RR_ERROR_WRONG_INPUT_VALUE;
            }
        }
        
        if (lnktrn_info_ptr->param.is.dsp_cl72_rest_to) {
            if (!util_check_param_range(lnktrn_info_ptr->value.dsp.cl72_rest_to, 0, 1)) {
                CAPI_LOG_ERROR("Invalid dsp_cl72_rest_to value: %d. Should be 0 or 1.\r\n",
                               lnktrn_info_ptr->value.dsp.cl72_rest_to);
                return_result = RR_ERROR_WRONG_INPUT_VALUE;
            }
        }
        
        if (lnktrn_info_ptr->param.is.dsp_enable_cl72_cl93_preset_req) {
            if (!util_check_param_range(lnktrn_info_ptr->value.dsp.enable_cl72_cl93_preset_req, 0, 1)) {
                CAPI_LOG_ERROR("Invalid dsp_enable_cl72_cl93_preset_req value: %d. Should be 0 or 1.\r\n",
                               lnktrn_info_ptr->value.dsp.enable_cl72_cl93_preset_req);
                return_result = RR_ERROR_WRONG_INPUT_VALUE;
            }
        }
        
        if (lnktrn_info_ptr->param.is.dsp_enable_802_3cd_preset_req) {
            if (!util_check_param_range(lnktrn_info_ptr->value.dsp.enable_802_3cd_preset_req, 0, 1)) {
                CAPI_LOG_ERROR("Invalid dsp_enable_802_3cd_preset_req value: %d. Should be 0 or 1.\r\n",
                               lnktrn_info_ptr->value.dsp.enable_802_3cd_preset_req);
                return_result = RR_ERROR_WRONG_INPUT_VALUE;
            }
        }
        
        if (lnktrn_info_ptr->param.is.dsp_lnktrn_cfg) {
            /*
            if (!util_check_param_range(lnktrn_info_ptr->value.dsp.lnktrn_cfg.pam_802_3, 0, 1)) {
                CAPI_LOG_ERROR("Invalid dsp_lnktrn_cfg value: %d. Should be 0 or 1.\r\n",
                               lnktrn_info_ptr->value.dsp.lnktrn_cfg);
                return_result = RR_ERROR_WRONG_INPUT_VALUE;
            }
            */
        }
#endif

    }
    return(return_result);
}
#endif /* CAPI_CONFIG_CMD_SANITY_CHK_ENABLE */



/**
 * @brief      diag_lane_swap_info_sanity_checker(capi_phy_info_t *phy_info_ptr, lane_swap_info_t* lane_swap_info_ptr, uint8_t request, sanity_chk_t chk_type)
 * @details    This API is used to sanity check lane swap info params
 *
 * @param[in]  phy_info_ptr: this parameter
 * @param[in]  lane_swap_info_ptr: lane swap info pointer
 * @param[in]  request: request type, SET or GET
 * @param[in]  chk_type: sanity check type, input or output
 *
 * @return     returns the performance result of the called method/function
 */
return_result_t diag_lane_swap_info_sanity_checker(capi_phy_info_t *phy_info_ptr, lane_swap_info_t* lane_swap_info_ptr, uint8_t request, sanity_chk_t chk_type)
{
    (void)phy_info_ptr;
    return_result_t err_code;
    uint8_t i, map_rx[8] = {0}, map_tx[8] = {0};

    if (request == GET_CONFIG)
        return RR_SUCCESS;

    if (chk_type == CHK_USR_INPUT)
        err_code = RR_ERROR_WRONG_INPUT_VALUE;
    else if (chk_type == CHK_FW_OUTPUT)
        err_code = RR_ERROR_WRONG_OUTPUT_VALUE;
    else
        return RR_ERROR_WRONG_INPUT_VALUE;

    if (!util_check_param_range(lane_swap_info_ptr->num_of_lanes, 1, 8)) {
        CAPI_LOG_ERROR("Invalid num of lanes value: %d. Should be between [1, 8].\r\n", lane_swap_info_ptr->num_of_lanes);
        return err_code;
    }

    for (i=0; i<8; i++) {
        if (!util_check_param_range(lane_swap_info_ptr->rx_lane_list[i], 0 ,7)) {
            CAPI_LOG_ERROR("Invalid rx_lane_list[%d] value: %d. Should be [0, 7].\r\n", i, lane_swap_info_ptr->rx_lane_list[i]);
            return err_code;
        }
        map_rx[lane_swap_info_ptr->rx_lane_list[i]]++;
    }

    for (i=0; i<8; i++) {
        if (!util_check_param_range(lane_swap_info_ptr->tx_lane_list[i], 0 ,7)) {
            CAPI_LOG_ERROR("Invalid tx_lane_list[%d] value: %d. Should be [0, 7].\r\n", i, lane_swap_info_ptr->tx_lane_list[i]);
            return err_code;
        }
        map_tx[lane_swap_info_ptr->tx_lane_list[i]]++;
    }
    for (i=0; i<8; i++) {
        if (map_rx[i] != 1) {
            CAPI_LOG_ERROR("Overlap lanes in rx_lane_list.\r\n");
            return err_code;
        }
        if (map_tx[i] != 1) {
            CAPI_LOG_ERROR("Overlap lanes in tx_lane_list.\r\n");
            return err_code;
            }
        }
    
    return RR_SUCCESS;
}



#ifdef CAPI_CONFIG_CMD_SANITY_CHK_ENABLE
/**
 * @brief       host_diag_cmd_sanity_checker(capi_phy_info_t *phy_info_ptr, capi_command_info_t *cmd_info_ptr, sanity_chk_t chk_type)
 * @details       This function is used to check the capi cmd sanity
 *
 * @param[in]
 *
 * @return       returns the performance result of the called methode/function
 */
return_result_t host_diag_cmd_sanity_checker(capi_phy_info_t*     phy_info_ptr, 
                                             capi_command_info_t* cmd_info_ptr, 
                                             sanity_chk_t         chk_type)
{
    return_result_t     return_result = RR_SUCCESS;
    capi_feature_info_t capi_feature_info;

    if (phy_info_ptr == NULL || cmd_info_ptr == NULL)
        return RR_ERROR_WRONG_INPUT_VALUE;

    host_get_capi_feature_info(phy_info_ptr, &capi_feature_info);

    if (capi_feature_info.value.is.disable_sanity_checker) {
        return return_result;
    }

    switch(cmd_info_ptr->command_id) {
        case COMMAND_ID_SET_POLARITY:
            CAPI_LOG_POLARITY_INFO(&cmd_info_ptr->type.polarity_info);
            return_result = sanity_checker_set_polarity_info(phy_info_ptr,
                                                             &cmd_info_ptr->type.polarity_info);
            break;

        case COMMAND_ID_GET_POLARITY:
            CAPI_LOG_POLARITY_INFO(&cmd_info_ptr->type.polarity_info);
            return_result = sanity_checker_get_polarity_info(phy_info_ptr,
                                                             &cmd_info_ptr->type.polarity_info, chk_type);
            break;

        case COMMAND_ID_SET_LANE_CONFIG_INFO:
            if (cmd_info_ptr->type.lane_config_info.lane_config_type == LANE_CONFIG_TYPE_LANE_RX_INFO) {
                CAPI_LOG_RX_INFO(&cmd_info_ptr->type.lane_config_info.type.lane_rx_info);
                return_result = sanity_checker_lane_rx_info(phy_info_ptr,
                                                            &cmd_info_ptr->type.lane_config_info.type.lane_rx_info,
                                                            SET_CONFIG,
                                                            chk_type);
            } else if (cmd_info_ptr->type.lane_config_info.lane_config_type == LANE_CONFIG_TYPE_LANE_TX_INFO) {
                CAPI_LOG_TX_INFO(&cmd_info_ptr->type.lane_config_info.type.lane_tx_info);
                return_result = sanity_checker_lane_tx_info(phy_info_ptr,
                                                            &cmd_info_ptr->type.lane_config_info.type.lane_tx_info,
                                                            SET_CONFIG,
                                                            chk_type);
            } else if (cmd_info_ptr->type.lane_config_info.lane_config_type == LANE_CONFIG_TYPE_LANE_LINK_TRAINING_INFO) {
                CAPI_LOG_LNKTRN_INFO(&cmd_info_ptr->type.lane_config_info.type.lane_lnktrn_info);
                return_result = sanity_checker_lnktrn_info(phy_info_ptr,
                                                           &cmd_info_ptr->type.lane_config_info.type.lane_lnktrn_info,
                                                           SET_CONFIG,
                                                           chk_type);
            } else if (cmd_info_ptr->type.lane_config_info.lane_config_type == LANE_CONFIG_TYPE_LANE_SWAP_INFO) {
                CAPI_LOG_LANE_SWAP_INFO(&cmd_info_ptr->type.lane_config_info.type.lane_swap_info);
                return_result = diag_lane_swap_info_sanity_checker(phy_info_ptr,
                                                                   &cmd_info_ptr->type.lane_config_info.type.lane_swap_info,
                                                                   SET_CONFIG,
                                                                   chk_type);
            }
            break;

        case COMMAND_ID_GET_LANE_CONFIG_INFO:
            if (cmd_info_ptr->type.lane_config_info.lane_config_type == LANE_CONFIG_TYPE_LANE_RX_INFO) {
                CAPI_LOG_RX_INFO(&cmd_info_ptr->type.lane_config_info.type.lane_rx_info);
                return_result = sanity_checker_lane_rx_info(phy_info_ptr,
                                                            &cmd_info_ptr->type.lane_config_info.type.lane_rx_info,
                                                            GET_CONFIG,
                                                            chk_type);
            } else if (cmd_info_ptr->type.lane_config_info.lane_config_type == LANE_CONFIG_TYPE_LANE_TX_INFO) {
                CAPI_LOG_TX_INFO(&cmd_info_ptr->type.lane_config_info.type.lane_tx_info);
                return_result = sanity_checker_lane_tx_info(phy_info_ptr,
                                                            &cmd_info_ptr->type.lane_config_info.type.lane_tx_info,
                                                            GET_CONFIG,
                                                            chk_type);
            } else if (cmd_info_ptr->type.lane_config_info.lane_config_type == LANE_CONFIG_TYPE_LANE_LINK_TRAINING_INFO) {
                CAPI_LOG_LNKTRN_INFO(&cmd_info_ptr->type.lane_config_info.type.lane_lnktrn_info);
                return_result = sanity_checker_lnktrn_info(phy_info_ptr,
                                                           &cmd_info_ptr->type.lane_config_info.type.lane_lnktrn_info,
                                                           GET_CONFIG,
                                                           chk_type);
            } else if (cmd_info_ptr->type.lane_config_info.lane_config_type == LANE_CONFIG_TYPE_LANE_SWAP_INFO) {
                CAPI_LOG_LANE_SWAP_INFO(&cmd_info_ptr->type.lane_config_info.type.lane_swap_info);
                return_result = diag_lane_swap_info_sanity_checker(phy_info_ptr,
                                                                   &cmd_info_ptr->type.lane_config_info.type.lane_swap_info,
                                                                   GET_CONFIG,
                                                                   chk_type);
            }
            break;

        case COMMAND_ID_GET_LANE_CTRL_INFO:
            CAPI_LOG_LANE_CTRL_INFO(&cmd_info_ptr->type.lane_ctrl_info);
            return_result = sanity_checker_get_lane_ctrl_info(phy_info_ptr,
                                                              &cmd_info_ptr->type.lane_ctrl_info,
                                                              chk_type);
            break;

        case COMMAND_ID_SET_LANE_CTRL_INFO:
            CAPI_LOG_LANE_CTRL_INFO(&cmd_info_ptr->type.lane_ctrl_info);
            return_result = sanity_checker_set_lane_ctrl_info(phy_info_ptr,
                                                          &cmd_info_ptr->type.lane_ctrl_info);
            break;

        case COMMAND_ID_GET_LANE_INFO:
            CAPI_LOG_LANE_INFO(&cmd_info_ptr->type.lane_info);
            return_result = sanity_checker_get_lane_info(phy_info_ptr,
                                                         &cmd_info_ptr->type.lane_info,
                                                         chk_type);
            break;

        case COMMAND_ID_GET_CONFIG_INFO:
            CAPI_LOG_CONFIG_INFO(&cmd_info_ptr->type.config_info);
            return_result = sanity_checker_get_config_info(phy_info_ptr, &cmd_info_ptr->type.config_info, chk_type);
            break;

        case COMMAND_ID_SET_CONFIG_INFO:
            CAPI_LOG_CONFIG_INFO(&cmd_info_ptr->type.config_info);
            return_result = sanity_checker_set_config_info(phy_info_ptr, &cmd_info_ptr->type.config_info, chk_type);
            break;

        case COMMAND_ID_SET_RECOVERED_CLOCK_INFO:
            return_result = sanity_checker_set_lw_rclk_info(phy_info_ptr, (capi_recovered_clock_info_t*)&cmd_info_ptr->type, chk_type);
            break;
            
        case COMMAND_ID_GET_RECOVERED_CLOCK_INFO:
            return_result = sanity_checker_get_lw_rclk_info(phy_info_ptr, (capi_recovered_clock_info_t*)&cmd_info_ptr->type, chk_type);            
            break;

        case COMMAND_ID_DIAG_LANE_STATUS:
            break;

        case COMMAND_ID_SET_ARCHIVE_INFO:
            break;

        case COMMAND_ID_GET_ARCHIVE_INFO:
            break;

        case COMMAND_ID_SET_LOOPBACK_INFO:
            CAPI_LOG_LPBK_INFO(&cmd_info_ptr->type.lpbk_ctrl_info);
            return_result = sanity_checker_loopback(phy_info_ptr,
                                                    &cmd_info_ptr->type.lpbk_ctrl_info);
            break;

        case COMMAND_ID_GET_LOOPBACK_INFO:
            CAPI_LOG_LPBK_INFO(&cmd_info_ptr->type.lpbk_ctrl_info);
            return_result = sanity_checker_loopback(phy_info_ptr,
                                                    &cmd_info_ptr->type.lpbk_ctrl_info);
            break;

        case COMMAND_ID_SET_PRBS_INFO: /* Need confirmation on pattern values */
            return_result = sanity_checker_set_prbs_info(phy_info_ptr, &cmd_info_ptr->type.prbs_info);
            break;

        case COMMAND_ID_GET_PRBS_INFO:
            return_result = sanity_checker_get_prbs_info(phy_info_ptr, &cmd_info_ptr->type.prbs_info, chk_type);
            break;

        case COMMAND_ID_GET_PRBS_STATUS:
            CAPI_LOG_PRBS_STATUS(&cmd_info_ptr->type.prbs_status_info);
            return_result = sanity_checker_prbs_status_get(phy_info_ptr, &cmd_info_ptr->type.prbs_status_info, chk_type);
            break;

        case COMMAND_ID_CLEAR_PRBS_STATUS:
            return_result = sanity_checker_clear_prbs_status(phy_info_ptr, &cmd_info_ptr->type.prbs_status_info);
            break;

        case COMMAND_ID_INJ_PRBS_ERROR:
            return_result = sanity_checker_prbs_inject_error(phy_info_ptr, &cmd_info_ptr->type.prbs_err_inj_info);
            break;

        case COMMAND_ID_SET_TXPI_OVERRIDE:
            return_result = sanity_checker_set_txpi_override(phy_info_ptr, &cmd_info_ptr->type.txpi_ovrd);
            break;
            
        case COMMAND_ID_SET_CW_RPTR_FEC_MON_CONFIG:
            if (cmd_info_ptr->type.rptr_fec_st.fec_mode != CAPI_FEC_CLIENT && cmd_info_ptr->type.rptr_fec_st.fec_mode != CAPI_FEC_LINE) 
                return_result = RR_ERROR_WRONG_INPUT_VALUE;
            if (cmd_info_ptr->type.rptr_fec_st.sampling_rate > 100)
                return_result = RR_ERROR_WRONG_INPUT_VALUE;
            break;

        case COMMAND_ID_GET_CMIS_INFO:
        case COMMAND_ID_GET_DSP_DEBUG_DUMP:
            break;

        case COMMAND_ID_SET_DSP_OPT_LOS_HOST_TX_UNSQUELCH:
            //CAPI_LOG_LANE_INFO(&cmd_info_ptr->type.dsp_opt_los_host_tx_unsquelch);
            return_result = RR_SUCCESS;
            break;

        case COMMAND_ID_START_LINKCAT:
        case COMMAND_ID_STOP_LINKCAT:
            return_result = RR_SUCCESS;
            break;
            
        case COMMAND_ID_SET_PORT_INFO:
            return_result = sanity_checker_set_port_info(phy_info_ptr, &cmd_info_ptr->type.port_info, chk_type);
            break;

        case COMMAND_ID_GET_PORT_INFO:
            return_result = sanity_checker_get_port_info(phy_info_ptr, &cmd_info_ptr->type.port_info, chk_type);
            break;

        case COMMAND_ID_DIAG_SET_CMIS_SNR_LTP_INFO:
        case COMMAND_ID_DIAG_GET_CMIS_SNR_LTP_CONFIG_INFO:
        case COMMAND_ID_DIAG_GET_CMIS_SNR_LTP_INFO:
        case COMMAND_ID_DIAG_GET_MEDIA_MPI_STATE:
        case COMMAND_ID_DIAG_GET_MEDIA_MISSION_MPI_STATE:
        case COMMAND_ID_SET_LANE_SR2:
            break;
        
        case COMMAND_ID_DIAG_SET_MEDIA_MPI_CONFIG:
        case COMMAND_ID_DIAG_GET_MEDIA_MPI_CONFIG:
        case COMMAND_ID_DIAG_SET_MEDIA_MISSION_MPI_CONFIG:
        case COMMAND_ID_DIAG_GET_MEDIA_MISSION_MPI_CONFIG:
            break;

        case COMMAND_ID_DIAG_SET_TRAFFIC_SWITCH_DETECT_CONFIG:
        case COMMAND_ID_DIAG_GET_TRAFFIC_SWITCH_DETECT_CONFIG:
        case COMMAND_ID_DIAG_GET_TRAFFIC_SWITCH_DETECT_STATE:
        case COMMAND_ID_START_DIAG_DUMP:            
            break;
        case COMMAND_ID_SET_PLL_CFG:
        if ((cmd_info_ptr->type.pll_info.enable_pll_dmode != CAPI_PLL1 && 
            cmd_info_ptr->type.pll_info.enable_pll_dmode != CAPI_PLL2 ) ||
            cmd_info_ptr->type.pll_info.pll_bandwidth >= CAPI_PLL_BW_MAX)
            return_result = RR_ERROR_WRONG_INPUT_VALUE;
        break;
        default:
            return_result = RR_ERROR_WRONG_INPUT_VALUE;
            break;
    }
    return return_result;
}
#endif



/**
 * @brief      host_get_memory_payload(capi_phy_info_t*    phy_info_ptr, 
 *                                     void*               dest_ptr,
 *                                     memory_info_t*      mem_data_ptr)
 *
 * @details    A fast access API to get response data populated by FW.
 *
 * @param      phy_info_ptr : phy info pointer
 * @param      dest_ptr     : dest data pointer
 * @param      mem_data_ptr : memory data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_get_memory_payload(capi_phy_info_t*    phy_info_ptr, 
                                        void*               dest_ptr,
                                        memory_info_t*      mem_data_ptr)
{
    return_result_t     return_result = RR_SUCCESS;
    capi_phy_info_t     phy_info;
    uint16_t            timeout_ms;

    if (!mem_data_ptr->ref_address) return RR_ERROR_WRONG_INPUT_VALUE;

    util_memcpy((void *)&phy_info, phy_info_ptr, sizeof(capi_phy_info_t));
    phy_info.base_addr = 0;

    /* Wait up to 100ms to acquire hw mutex before reading FW memory */
    for (timeout_ms = 0; timeout_ms < 100; timeout_ms++) {

        return_result = acquire_hw_mutex(&phy_info, mem_data_ptr->hw_mutex);
        
        if (return_result == RR_SUCCESS) {
            /* Read FW memory data info */
            intf_read_memory(&phy_info,
                    (uint32_t*) dest_ptr,
                    (uint32_t*)(uintptr_t) (mem_data_ptr->ref_address),
                    mem_data_ptr->ref_data_len);
            release_hw_mutex(&phy_info, mem_data_ptr->hw_mutex);
            break;
        }
        delay_ms(1);
    }
    if (return_result != RR_SUCCESS)
        CAPI_LOG_ERROR("host_get_memory_payload: failed to acquire hw mutex");
    return return_result;
}


/**
 * @brief      host_get_memory_data_info(capi_phy_info_t*    phy_info_ptr,
 *                                       memory_data_info_t* memory_data_ptr)
 *
 * @details    A fast access API to get response data populated by FW.
 *
 * @param      phy_info_ptr : phy info pointer
 * @param      memory_data_ptr : memory data information pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_get_memory_data_info(capi_phy_info_t*    phy_info_ptr,
                                          memory_data_info_t* memory_data_ptr)
{

    uint8_t              lane_index;
    uint16_t             timeout_ms;
    uint32_t             gpr_reg_address;
    uint16_t             snr_ltp_ready_lane_mask;
    return_result_t      return_result = RR_SUCCESS;
    capi_phy_info_t      phy_info;
    capi_phy_info_t      temp_phy_info;
    memory_info_t*       memmory_info_ptr = (memory_info_t*)&memory_data_ptr->memory_info;
    capi_cmis_snr_ltp_t* dest_snr_ltp_ptr = memory_data_ptr->param.lane_data;
    capi_cmis_snr_ltp_t* src_snr_ltp_ptr  = (capi_cmis_snr_ltp_t*)memory_data_ptr->memory_info.ref_address;

    if (!memmory_info_ptr->ref_address) return RR_ERROR_WRONG_INPUT_VALUE;

    util_memcpy((void*)&phy_info,      phy_info_ptr, sizeof(capi_phy_info_t));
    util_memset((void*)&temp_phy_info, 0x00,         sizeof(capi_phy_info_t));

    phy_info.base_addr      = MGT;
    temp_phy_info.base_addr = OCTAL_TOP_REGS;

    if (phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP) {
        gpr_reg_address = MEDIA_SIDE_LANE_SNR_LTP_READY_STATUS;
    } else {
        gpr_reg_address = HOST_SIDE_LANE_SNR_LTP_READY_STATUS;
    }

    /* Wait up to 100ms to acquire hw mutex before reading FW memory */
    for (timeout_ms = 0; timeout_ms < 500; timeout_ms++) {

        return_result = acquire_hw_mutex(&phy_info, memmory_info_ptr->hw_mutex);

        if (return_result == RR_SUCCESS) {

            ERR_HSIP(snr_ltp_ready_lane_mask = (uint16_t)hsip_rd_reg_(&temp_phy_info, gpr_reg_address));

            hsip_wr_reg_(&temp_phy_info, gpr_reg_address, (snr_ltp_ready_lane_mask & ~phy_info_ptr->lane_mask));

            /* Read FW memory data info */
            intf_read_memory(&phy_info,
                             (uint32_t*) dest_snr_ltp_ptr,
                             (uint32_t*) src_snr_ltp_ptr,
                             memmory_info_ptr->ref_data_len);

            release_hw_mutex(&phy_info, memmory_info_ptr->hw_mutex);
            break;
        }

        delay_ms(1);
    }

    dest_snr_ltp_ptr = memory_data_ptr->param.lane_data;

    for (lane_index = 0; lane_index < DSP_LANE_NUM_MAX; lane_index++) {
        if (!(phy_info_ptr->lane_mask & (0x01<<lane_index))) {
            util_memset((void*)dest_snr_ltp_ptr, 0x00, sizeof(capi_cmis_snr_ltp_t));
        }
        dest_snr_ltp_ptr++;
    }


    if (return_result != RR_SUCCESS)
        CAPI_LOG_ERROR("host_get_memory_payload: failed to acquire hw mutex");

    return return_result;
}



/**
 * @brief        util_get_number_of_lanes(phy_info_t* phy_info_ptr, uint8_t * number_of_lanes_ptr)
 *               This utility function return the total number of the lanes on either media or host side
 *
 * @param[in]    phy_info_ptr: a reference to the phy information object
 * @param[out]   number_of_lanes_ptr: a reference to the number of lanes object, which needs to be initialized
 *
 * @return       retunes the totoal number of the lanes
 */
return_result_t util_get_number_of_lanes(phy_info_t* phy_info_ptr, uint8_t * number_of_lanes_ptr)
{
    return_result_t return_result = RR_SUCCESS;
    if ((phy_info_ptr->core_ip == CORE_IP_HOST_SERDES) ||
        (phy_info_ptr->core_ip == CORE_IP_HOST_DSP)) {

        *number_of_lanes_ptr = HOST_SIDE_NUMBER_OF_LANE;
    } else if ((phy_info_ptr->core_ip == CORE_IP_MEDIA_SERDES) ||
               (phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP)) {

        *number_of_lanes_ptr = MEDIA_SIDE_NUMBER_OF_LANE;
    }
    else {
        return_result = RR_ERROR_WRONG_INPUT_VALUE;
    }
    return return_result;
}

/**
 * @brief      dsp_mission_mpi_cfg_sanity_checker(capi_phy_info_t* phy_info_ptr, dsp_mission_mpi_cfg_info_t* dsp_mission_mpi_cfg_info_ptr, sanity_chk_t chk_type)
 * @details    This API is used to sanity check dynamic MPI configuration
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr     : this parameter 
 * @param[in]  dsp_mission_mpi_cfg_info_ptr: this parameter 
 * 
 * @return     returns the performance result of the called methode/function
 */
return_result_t dsp_mission_mpi_cfg_sanity_checker(capi_phy_info_t* phy_info_ptr, dsp_mission_mpi_cfg_info_t* dsp_mission_mpi_cfg_info_ptr, sanity_chk_t chk_type)
{
    /* MPI detect is line side only. 
     dynamic MPI check counte default is 512, settable option are 128, 256, 512, 1024
     dynamic MPI check threshold, default is 11, with 9,10,11,12,13 and 14 as settable options */
    if ((CORE_IP_MEDIA_DSP == phy_info_ptr->core_ip) &&
        ((128 == dsp_mission_mpi_cfg_info_ptr->mpi_chk_cnt) || (256 == dsp_mission_mpi_cfg_info_ptr->mpi_chk_cnt) || (512 == dsp_mission_mpi_cfg_info_ptr->mpi_chk_cnt) || (1024 == dsp_mission_mpi_cfg_info_ptr->mpi_chk_cnt)) &&
        (dsp_mission_mpi_cfg_info_ptr->mpi_chk_th >= 9 && dsp_mission_mpi_cfg_info_ptr->mpi_chk_th <= 14)) {
        return RR_SUCCESS;
    }
    else
        return ((chk_type==CHK_USR_INPUT)?RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE);
}

/**
 * @brief      dsp_traffic_mode_switch_detect_cfg_sanity_checker(capi_phy_info_t* phy_info_ptr, dsp_tmsd_cfg_info_t* dsp_tmsd_cfg_info_ptr, sanity_chk_t chk_type)
 * @details    This API is used to sanity check TMSD configuration
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr     : this parameter 
 * @param[in]  dsp_mission_mpi_cfg_info_ptr: this parameter 
 * 
 * @return     returns the performance result of the called methode/function
 */
return_result_t dsp_traffic_mode_switch_detect_cfg_sanity_checker(capi_phy_info_t* phy_info_ptr, dsp_tmsd_cfg_info_t* dsp_tmsd_cfg_info_ptr, sanity_chk_t chk_type)
{
    /* tmsd_enable;          incoming traffic mode switch from 10XG to 5Xg detect check enable/disable [0, 1] 0-disable, 1; 1-enable
    */
    if ((CORE_IP_MEDIA_DSP == phy_info_ptr->core_ip || CORE_IP_HOST_DSP == phy_info_ptr->core_ip) &&
        util_check_param_range(dsp_tmsd_cfg_info_ptr->tmsd_enable, 0, 1)) {
        return RR_SUCCESS;
    }
    else
        return ((chk_type==CHK_USR_INPUT)?RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE);
}


/**
 * @brief      dsp_mpi_canceller_cfg_sanity_checker(capi_phy_info_t* phy_info_ptr, dsp_mpi_canceller_cfg_info_t* mpi_canceller_cfg_info_ptr, sanity_chk_t chk_type)
 * @details    This API is used to sanity check MPI  canceller configuration
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr     : this parameter 
 * @param[in]  mpi_canceller_cfg_info_ptr: this parameter 
 * 
 * @return     returns the performance result of the called methode/function
 */
return_result_t dsp_mpi_canceller_cfg_sanity_checker(capi_phy_info_t* phy_info_ptr, dsp_mpi_canceller_cfg_info_t* mpi_canceller_cfg_info_ptr, sanity_chk_t chk_type)
{
    /* MPI canceller is line side only. 
      MPI canceller is enabled or disabled;*/
    if ((CORE_IP_MEDIA_DSP == phy_info_ptr->core_ip) &&
        (( mpi_canceller_cfg_info_ptr->mpi_canceller_enable == 0) ||
            (  mpi_canceller_cfg_info_ptr->mpi_canceller_enable == 1)))  {
        return RR_SUCCESS;
    }
    else
        return ((chk_type==CHK_USR_INPUT)?RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE);
}

/**
 * @brief      dsp_hw_gain2_adapt_cfg_sanity_checker(capi_phy_info_t* phy_info_ptr, dsp_hw_gain2_adapt_cfg_info_t* hw_gain2_a_cfg_info_ptr, sanity_chk_t chk_type)
 * @details    This API is used to sanity check hardware gain2 adaptation configuration
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr     : this parameter 
 * @param[in]  hw_gain2_a_cfg_info_ptr: this parameter 
 * 
 * @return     returns the performance result of the called methode/function
 */
return_result_t dsp_hw_gain2_adapt_cfg_sanity_checker(capi_phy_info_t* phy_info_ptr, dsp_hw_gain2_adapt_cfg_info_t* hw_gain2_a_cfg_info_ptr, sanity_chk_t chk_type)
{
    /* Hardware GAIN2 adaptation is line side only: enabled or disabled;*/
    if ((CORE_IP_MEDIA_DSP == phy_info_ptr->core_ip) &&
        (( hw_gain2_a_cfg_info_ptr->hw_gain2_adapt_enable == 0) ||
        (  hw_gain2_a_cfg_info_ptr->hw_gain2_adapt_enable == 1)))  {
        return RR_SUCCESS;
    }
    else
        return ((chk_type==CHK_USR_INPUT)?RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE);
}



/**
 * @brief      dsp_gain3_a_param_cfg_sanity_checker(capi_phy_info_t* phy_info_ptr, dsp_gain3_a_param_cfg_info_t* gain3_a_cfg_info_ptr, sanity_chk_t chk_type)
 * @details    This API is used to sanity check MPI  canceller configuration
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr     : this parameter 
 * @param[in]  gain3_a_cfg_info_ptr: this parameter 
 * 
 * @return     returns the performance result of the called methode/function
 */
return_result_t dsp_gain3_a_param_cfg_sanity_checker(capi_phy_info_t* phy_info_ptr, dsp_gain3_a_param_cfg_info_t* gain3_a_cfg_info_ptr, sanity_chk_t chk_type)
{
    /* GAIN3 manual a parameter is line side only*/
    if ((CORE_IP_MEDIA_DSP == phy_info_ptr->core_ip) &&
        (( gain3_a_cfg_info_ptr->gain3_a_override == 0) ||
            (  gain3_a_cfg_info_ptr->gain3_a_override == 1)) &&
        util_check_param_range(gain3_a_cfg_info_ptr->gain3_a_oval, -64, 63))  {
        return RR_SUCCESS;
    }
    else
        return ((chk_type==CHK_USR_INPUT)?RR_ERROR_WRONG_INPUT_VALUE : RR_ERROR_WRONG_OUTPUT_VALUE);
}

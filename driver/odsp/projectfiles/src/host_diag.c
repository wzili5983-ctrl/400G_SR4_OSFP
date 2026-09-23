/**
 *
 * @file     capi_diag.c
 * @author   
 * @date     8/5/2019
 * @version  1.0
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
#include <math.h>
#include <float.h>
#include "chip_config_def.h"
#include "regs_common.h"
#include "chip_mode_def.h"
#include "capi_def.h"
#include "hr_time.h"
#include "capi.h"
#include "capi_fw_intf.h"
#include "chip_common_config_ind.h"
#include "lw_common_config_ind.h"
#include "fw_event_log.h"
#include "host_to_chip_ipc.h"
#include "host_log_util.h"
#include "capi_diag_def.h"
#include "dsp_utils.h"
#include "host_diag_util.h"
#include "common_util.h"
#include "capi_custom_def.h"
#include "host_diag_lw.h"
#include "capi_test_def.h"
#include "host_lw_wrapper.h"

typedef union mem_brcm_snr_info_s {
    uint32_t raw[44];
    capi_fast_brcm_snr_info_t snr_info;                                                            /**< SNR configuation items                         */
} mem_brcm_snr_info_t;

return_result_t host_lw_get_snr_from_gp(phy_info_t* phy_info_ptr, float* snr_ptr, float *level_snr_ptr)
{
    uint8_t lane_index, idx;
    phy_info_t   top_phy = {0};
    uint32_t     lo_addr, hi_addr, brcm_snr_info_bbaddr, lane_abase;
    uint32_t     lane_gap = sizeof(capi_fast_brcm_snr_info_t);
    dsp_fast_brcm_snr_cfg_t snr_cfg;
    mem_brcm_snr_info_t brcm_snr;
    memory_info_t mem_info;
    return_result_t return_result;
#if 0
    uint32_t didx;
#endif
    
    util_memcpy((void *)&top_phy, phy_info_ptr, sizeof(phy_info_t));
    util_memset(&snr_cfg, 0, sizeof(dsp_fast_brcm_snr_cfg_t));
    top_phy.base_addr    = OCTAL_TOP_REGS;
    ERR_HSIP(lo_addr     = (uint32_t)hsip_rd_reg_(&top_phy, DSP_BRCM_SNR_LOG_LO_REG));
    ERR_HSIP(hi_addr     = (uint32_t)hsip_rd_reg_(&top_phy, DSP_BRCM_SNR_LOG_HI_REG));
    brcm_snr_info_bbaddr = (hi_addr<<16)|lo_addr;
    if(phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP)
        brcm_snr_info_bbaddr += lane_gap*MAX_LW_LANES;

    for (lane_index = 0; lane_index < MAX_LW_LANES; lane_index++) {
        if (phy_info_ptr->lane_mask & (0x01 << lane_index)) {
            if (phy_info_ptr->core_ip == CORE_IP_HOST_DSP || phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP) {
                top_phy.base_addr    = 0;
                lane_abase = brcm_snr_info_bbaddr+lane_index*lane_gap;
#if 0
                for(didx=0; didx<lane_gap/4; didx++){
                    brcm_snr.raw[didx] = (uint32_t)hsip_rd_reg_(&top_phy, (lane_abase+didx*cfg_gap));
                }
#else
                mem_info.hw_mutex = HW_MUTEX_INVALID;
                mem_info.ref_address = lane_abase;
                mem_info.ref_data_len = 24;
                return_result = host_get_memory_payload(&top_phy, (void *)&brcm_snr.raw[0], &mem_info);
#endif
                snr_cfg.words    = brcm_snr.snr_info.snr_cfg.words;
                *snr_ptr         = brcm_snr.snr_info.average_snr;
                 *level_snr_ptr  = (float)snr_cfg.fields.snr_level;
                for(idx=0; idx<snr_cfg.fields.snr_level; idx++){   
                    *(level_snr_ptr+idx+1) = brcm_snr.snr_info.lvl_snr[idx];
                }
                return return_result;
            }
        }
    }
    return RR_ERROR_WRONG_INPUT_VALUE;
}

return_result_t host_lw_get_asnr_from_gp(phy_info_t* phy_info_ptr, float* snr_ptr)
{    
    uint8_t lane_index;
    phy_info_t   top_phy = {0};
    uint32_t     lo_addr, hi_addr, brcm_snr_info_bbaddr, lane_abase;
    uint32_t     lane_gap = sizeof(capi_fast_brcm_snr_info_t);
    mem_brcm_snr_info_t brcm_snr;
    memory_info_t mem_info;
    return_result_t return_result;
#if 0
    uint32_t didx;
#endif
    
    util_memcpy((void *)&top_phy, phy_info_ptr, sizeof(phy_info_t));
    util_memset(&brcm_snr, 0, sizeof(mem_brcm_snr_info_t));
    top_phy.base_addr    = OCTAL_TOP_REGS;
    ERR_HSIP(lo_addr     = (uint32_t)hsip_rd_reg_(&top_phy, DSP_BRCM_SNR_LOG_LO_REG));
    ERR_HSIP(hi_addr     = (uint32_t)hsip_rd_reg_(&top_phy, DSP_BRCM_SNR_LOG_HI_REG));
    brcm_snr_info_bbaddr = (hi_addr<<16)|lo_addr;
    if(phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP)
        brcm_snr_info_bbaddr += lane_gap*MAX_LW_LANES;

    for (lane_index = 0; lane_index < MAX_LW_LANES; lane_index++) {
        if (phy_info_ptr->lane_mask & (0x01 << lane_index)) {
            if (phy_info_ptr->core_ip == CORE_IP_HOST_DSP || phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP) {
                top_phy.base_addr    = 0;
                lane_abase = brcm_snr_info_bbaddr+lane_index*lane_gap;
#if 0
                for(didx=0; didx<lane_gap/4; didx++){
                    brcm_snr.raw[didx] = (uint32_t)hsip_rd_reg_(&top_phy, (lane_abase+didx*cfg_gap));
                }
#else
                mem_info.hw_mutex = HW_MUTEX_INVALID;
                mem_info.ref_address = lane_abase;
                mem_info.ref_data_len = 8;
                return_result = host_get_memory_payload(&top_phy, (void *)&brcm_snr.raw[0], &mem_info);
#endif
                *snr_ptr         = brcm_snr.snr_info.average_snr;
                return return_result;
            }
        }
    }
    return RR_ERROR_WRONG_INPUT_VALUE;
}


/**
 * @brief      host_lw_get_usr_diagnostics(capi_phy_info_t* phy_info_ptr, capi_usr_diag_info_t* usr_diag_info_ptr)
 * @details    This API is used to get user diag information for Line side <BR>
 *             when core_ip is CORE_IP_LW, the LW will report below parameters: <BR>
 *                  snr: SNR value <BR>
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  usr_diag_info_ptr: a pointer which carries detail user diag information
 *
 * @return     returns the performance result of the called method/function
 */
return_result_t host_lw_get_usr_diagnostics(capi_phy_info_t* phy_info_ptr, capi_usr_diag_info_t* vdm_info_ptr)
{
    return_result_t return_result = RR_ERROR_WRONG_INPUT_VALUE;
    if (vdm_info_ptr == NULL)
        return return_result;
    if (vdm_info_ptr->diag_info_type == DIAG_INFO_TYPE_BRCM_DSP_SNR) {
            return_result = host_lw_get_snr_from_gp(phy_info_ptr, &vdm_info_ptr->type.diag_brcm_dsp_snr.brcm_snr_value, 
                                &vdm_info_ptr->type.diag_brcm_dsp_snr.brcm_snr_level[0]);
    }else if (vdm_info_ptr->diag_info_type == DIAG_INFO_TYPE_BRCM_DSP_SNR_AVRG_ONLY) {
            return_result = host_lw_get_asnr_from_gp(phy_info_ptr, &vdm_info_ptr->type.diag_brcm_dsp_snr.brcm_snr_value);
    }
    return (return_result);
}

/**
* @brief      host_lw_get_usr_cmis_diagnostics(capi_phy_info_t* phy_info_ptr, capi_usr_diag_info_t* usr_diag_info_ptr)
* @details    This API is used to get the user cmis diag info
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  usr_diag_info_ptr: this parameter
*
* @return     returns the performance result of the called method/function
*/
return_result_t host_lw_get_usr_cmis_diagnostics(capi_phy_info_t* phy_info_ptr, capi_usr_diag_info_t* usr_diag_info_ptr)
{
    return_result_t return_result = RR_ERROR_WRONG_INPUT_VALUE;
    capi_command_info_t command_info;
    capi_cmis_info_t  lw_cmis_info;
    capi_cmis_info_t *lw_cmis_info_ptr = &lw_cmis_info;
    float snr_lvl[3]={0}, ltp_lvl[3]={0}, snr_min=0, ltp_min=FLT_MAX;
    uint8_t v_valid=1, level;

    if(usr_diag_info_ptr->type.diag_cmis_info.param.is.dsp_snr_ltp==0)
        return RR_ERROR_WRONG_INPUT_VALUE;

    util_memset(lw_cmis_info_ptr, 0x0, sizeof(capi_cmis_info_t));

    if (((phy_info_ptr->core_ip == CORE_IP_HOST_DSP) || (phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP)) &&
            (usr_diag_info_ptr->diag_info_type==DIAG_INFO_TYPE_CMIS)){
       command_info.command_id = COMMAND_ID_GET_CMIS_INFO;
       command_info.type.cmis_diag_info = *(lw_cmis_info_ptr);


       return_result = capi_command_request(phy_info_ptr, &command_info, sizeof(capi_usr_diag_info_t), GET_CONFIG);

       if (return_result != RR_SUCCESS) {
           return return_result;
       }

       *(lw_cmis_info_ptr) = command_info.type.cmis_diag_info;

       for(level = 0; level < 4; level++){
           usr_diag_info_ptr->type.diag_cmis_info.value.dsp_snr_ltp.slicer_target[level]      = lw_cmis_info_ptr->slicer_target[level];
           usr_diag_info_ptr->type.diag_cmis_info.value.dsp_snr_ltp.slicer_mean_value[level]  = lw_cmis_info_ptr->slicer_mean_value[level];
           usr_diag_info_ptr->type.diag_cmis_info.value.dsp_snr_ltp.slicer_sigma_value[level] = (float)sqrt(lw_cmis_info_ptr->slicer_sigma_sqr_value[level]);
           usr_diag_info_ptr->type.diag_cmis_info.value.dsp_snr_ltp.slicer_p_value[level]     = lw_cmis_info_ptr->slicer_p_value[level];
           usr_diag_info_ptr->type.diag_cmis_info.value.dsp_snr_ltp.slicer_p_location[level]  = lw_cmis_info_ptr->slicer_p_location[level];
       }

       for(level = 0; level < 3; level++){
           usr_diag_info_ptr->type.diag_cmis_info.value.dsp_snr_ltp.slicer_threshold[level]   = lw_cmis_info_ptr->slicer_threshold[level];
           usr_diag_info_ptr->type.diag_cmis_info.value.dsp_snr_ltp.slicer_v_value[level]     = lw_cmis_info_ptr->slicer_v_value[level];
           usr_diag_info_ptr->type.diag_cmis_info.value.dsp_snr_ltp.slicer_v_location[level]  = lw_cmis_info_ptr->slicer_v_location[level];
       }
       /* SNR = 10* log10(min{SNR0, SNR1, SNR2 }) where SNRi= (mean(i+1)- mean(i))/( sigma(i+1) +sigma(i)), expressed in 1/256 dB units
          LTP = 10* log10(min{LTP0, LTP1, LTP2 }) where LTPi= (P(i+1) + P(i))/( 2V(i)), expressed in 1/256 dB units*/
       for(level = 0; level < 3; level++) {
           if(usr_diag_info_ptr->type.diag_cmis_info.value.dsp_snr_ltp.slicer_sigma_value[level]+usr_diag_info_ptr->type.diag_cmis_info.value.dsp_snr_ltp.slicer_sigma_value[level+1]){
               snr_lvl[level] = (float)((lw_cmis_info_ptr->slicer_mean_value[level+1]-lw_cmis_info_ptr->slicer_mean_value[level])/
                                  (usr_diag_info_ptr->type.diag_cmis_info.value.dsp_snr_ltp.slicer_sigma_value[level+1]+usr_diag_info_ptr->type.diag_cmis_info.value.dsp_snr_ltp.slicer_sigma_value[level]));
               snr_min = (level==0)?snr_lvl[0]:((snr_lvl[level]<snr_min)?snr_lvl[level]:snr_min);
            }
            ltp_lvl[level] = 0xFFFF;
            if(lw_cmis_info_ptr->slicer_v_value[level]){
                ltp_lvl[level] = (float)((lw_cmis_info_ptr->slicer_p_value[level+1]+lw_cmis_info_ptr->slicer_p_value[level])/2/lw_cmis_info_ptr->slicer_v_value[level]);
                ltp_min = (level==0)?ltp_lvl[0]:((ltp_lvl[level]<ltp_min)?ltp_lvl[level]:ltp_min);
            }else
                v_valid&=0;
        }
        if (snr_min <= 0)
            snr_min = (float)1.0;
        usr_diag_info_ptr->type.diag_cmis_info.value.dsp_snr_ltp.cmis_snr = (float)(10 * log10(snr_min)*256);
        usr_diag_info_ptr->type.diag_cmis_info.value.dsp_snr_ltp.cmis_ltp = (v_valid==0)?0xFFFF:((float)(10*log10(ltp_min)*256));


        CAPI_LOG_INFO("FFE config: slicer threshold:  %d %d %d, slicer target  %d %d %d %d\n",
            lw_cmis_info_ptr->slicer_threshold[0], lw_cmis_info_ptr->slicer_threshold[1], lw_cmis_info_ptr->slicer_threshold[2],
            lw_cmis_info_ptr->slicer_target[0], lw_cmis_info_ptr->slicer_target[1],
            lw_cmis_info_ptr->slicer_target[2], lw_cmis_info_ptr->slicer_target[3]);
        CAPI_LOG_INFO("FFE slicer mean values: %f %f %f %f\n",
            lw_cmis_info_ptr->slicer_mean_value[0], lw_cmis_info_ptr->slicer_mean_value[1],
            lw_cmis_info_ptr->slicer_mean_value[2], lw_cmis_info_ptr->slicer_mean_value[3]);
        CAPI_LOG_INFO("FFE slicer sigma values: %f %f %f %f\n",
            usr_diag_info_ptr->type.diag_cmis_info.value.dsp_snr_ltp.slicer_sigma_value[0], usr_diag_info_ptr->type.diag_cmis_info.value.dsp_snr_ltp.slicer_sigma_value[1],
            usr_diag_info_ptr->type.diag_cmis_info.value.dsp_snr_ltp.slicer_sigma_value[2], usr_diag_info_ptr->type.diag_cmis_info.value.dsp_snr_ltp.slicer_sigma_value[3]);
        CAPI_LOG_INFO("FFE slicer P values: \t%d(%d) \t%d(%d) \t%d(%d) \t%d(%d)\n",
            lw_cmis_info_ptr->slicer_p_value[0], lw_cmis_info_ptr->slicer_p_location[0],
            lw_cmis_info_ptr->slicer_p_value[1], lw_cmis_info_ptr->slicer_p_location[1],
            lw_cmis_info_ptr->slicer_p_value[2], lw_cmis_info_ptr->slicer_p_location[2],
            lw_cmis_info_ptr->slicer_p_value[3], lw_cmis_info_ptr->slicer_p_location[3]);
        CAPI_LOG_INFO("FFE slicer V values: \t%d(%d) \t%d(%d) \t%d(%d)\n",
            lw_cmis_info_ptr->slicer_v_value[0], lw_cmis_info_ptr->slicer_v_location[0],
            lw_cmis_info_ptr->slicer_v_value[1], lw_cmis_info_ptr->slicer_v_location[1],
            lw_cmis_info_ptr->slicer_v_value[2], lw_cmis_info_ptr->slicer_p_location[2]);
        CAPI_LOG_INFO("\nSNR level values: %f %f %f; \tSNR value %f\n",
            snr_lvl[0], snr_lvl[1], snr_lvl[2], usr_diag_info_ptr->type.diag_cmis_info.value.dsp_snr_ltp.cmis_snr);
        CAPI_LOG_INFO("LTP level values: %f %f %f; \tLTP value %f\n",
            ltp_lvl[0], ltp_lvl[1], ltp_lvl[2], usr_diag_info_ptr->type.diag_cmis_info.value.dsp_snr_ltp.cmis_ltp);

    }

    return return_result;
}


/**
 * @brief      host_set_mpi_config(capi_phy_info_t*    phy_info_ptr, dsp_mpi_cfg_info_t *mpi_cfg_ptr)
 *
 * @details    get mpi configuration information
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      mpi_cfg_ptr      : mpi config data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_set_mpi_config(capi_phy_info_t*    phy_info_ptr, dsp_mpi_cfg_info_t *mpi_cfg_ptr)
{
    return_result_t     return_result = RR_ERROR_WRONG_INPUT_VALUE;
    uint8_t             lane_index;
    uint16_t            ratio_threshold_int;
    uint16_t            lane_mask = phy_info_ptr->lane_mask;
    phy_info_t          lw_phy_cfg;
    
   for (lane_index = 0; lane_index < MAX_LW_LANES; ++lane_index) {
       if ((lane_mask & (1 << lane_index)) == 0)
           continue;
       phy_info_ptr->lane_mask = (1 << lane_index);
       lw_util_init_lane_config_base_addr(phy_info_ptr, &lw_phy_cfg, lane_index);
       hsip_wr_field_(&lw_phy_cfg, COMMON_LW_LANE_CONFIG1C_REG, LW_DISABLE_FFE_HISTOGRAM_MPI_DETECT,  mpi_cfg_ptr->mpi_enable?0:1);
       hsip_wr_field_(&lw_phy_cfg, COMMON_LW_LANE_CONFIG28_REG, FFE_SLICER_TH_MPI_INTERVAL_TIMER,  mpi_cfg_ptr->mpi_interval);
       ratio_threshold_int = (uint16_t)((mpi_cfg_ptr->ratio_threshold+0.005) * 100);
       hsip_wr_field_(&lw_phy_cfg, COMMON_LW_LANE_CONFIG23_REG, FFE_SLICER_CMIS_MPI_METRIC_RATIO_TH,  ratio_threshold_int);
       return_result = RR_SUCCESS;
   }
    return return_result;
}

/**
 * @brief      host_get_mpi_config(capi_phy_info_t*    phy_info_ptr, dsp_mpi_cfg_info_t *mpi_cfg_ptr)
 *
 * @details    get mpi configuration information
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      mpi_cfg_ptr      : mpi config data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_get_mpi_config(capi_phy_info_t*    phy_info_ptr, dsp_mpi_cfg_info_t *mpi_cfg_ptr)
{
    return_result_t     return_result = RR_ERROR_WRONG_INPUT_VALUE;
    uint8_t             lane_index;
    uint16_t            lane_mask = phy_info_ptr->lane_mask;
    phy_info_t          lw_phy_cfg;
    
   for (lane_index = 0; lane_index < MAX_LW_LANES; ++lane_index) {
       if ((lane_mask & (1 << lane_index)) == 0)
           continue;
       phy_info_ptr->lane_mask = (1 << lane_index);
       lw_util_init_lane_config_base_addr(phy_info_ptr, &lw_phy_cfg, lane_index);
       ERR_HSIP( mpi_cfg_ptr->mpi_enable  = !((uint16_t) hsip_rd_field_(&lw_phy_cfg, COMMON_LW_LANE_CONFIG1C_REG, LW_DISABLE_FFE_HISTOGRAM_MPI_DETECT)));
       ERR_HSIP( mpi_cfg_ptr->mpi_interval = (uint16_t) hsip_rd_field_(&lw_phy_cfg, COMMON_LW_LANE_CONFIG28_REG, FFE_SLICER_TH_MPI_INTERVAL_TIMER));
       ERR_HSIP( mpi_cfg_ptr->ratio_threshold = (float) hsip_rd_field_(&lw_phy_cfg, COMMON_LW_LANE_CONFIG23_REG, FFE_SLICER_CMIS_MPI_METRIC_RATIO_TH)/100);
       return_result =  RR_SUCCESS;
   }
    return return_result;
}

/**
 * @brief      host_get_mpi_state(capi_phy_info_t*    phy_info_ptr, dsp_mpi_st_info_t *mpi_st_ptr)
 *
 * @details    get mpi state information
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      mpi_st_ptr      : mpi state data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_get_mpi_state(capi_phy_info_t*    phy_info_ptr, dsp_mpi_st_info_t *mpi_st_ptr)
{
    return_result_t     return_result = RR_ERROR_WRONG_INPUT_VALUE;
    uint8_t             lane_index, sidx;
    uint16_t            lane_mask = phy_info_ptr->lane_mask;
    phy_info_t          lw_phy_cfg;
    float               b11, b21=0,  mpi_normalized_s_sqr[4];

    if(phy_info_ptr->core_ip != CORE_IP_MEDIA_DSP ||
        util_check_param_range(mpi_st_ptr->read_clear, 0, 1) == FALSE)
        return RR_ERROR_WRONG_INPUT_VALUE;

   for (lane_index = 0; lane_index < MAX_LW_LANES; ++lane_index) {
       if ((lane_mask & (1 << lane_index)) == 0)
           continue;
       phy_info_ptr->lane_mask = (1 << lane_index);
       lw_util_init_lane_config_base_addr(phy_info_ptr, &lw_phy_cfg, lane_index);
       ERR_HSIP( mpi_st_ptr->dust_indicator         = (uint8_t)hsip_rd_field_(&lw_phy_cfg, COMMON_LW_LANE_CONFIG1F_REG, MPI_HW_HISTOGRAM_RATIO_INDICATOR));
       ERR_HSIP( mpi_st_ptr->dust_indicator_sticky  = (uint8_t)hsip_rd_field_(&lw_phy_cfg, COMMON_LW_LANE_CONFIG1F_REG, MPI_HW_HISTOGRAM_RATIO_INDICATOR_STICKY));
       if(mpi_st_ptr->read_clear)
           hsip_wr_field_(&lw_phy_cfg, COMMON_LW_LANE_CONFIG1F_REG, MPI_HW_HISTOGRAM_RATIO_INDICATOR_STICKY, 0);
       /*Normalized standard deviation of level-0~3 */
       ERR_HSIP(mpi_st_ptr->mpi_normalized_s[0] = (float)sqrt(hsip_rd_field_(&lw_phy_cfg,  COMMON_LW_LANE_CONFIG2E_REG, FFE_SLICER_CMIS_SIGMA_0))/10);
       ERR_HSIP(mpi_st_ptr->mpi_normalized_s[1] = (float)sqrt(hsip_rd_field_(&lw_phy_cfg,  COMMON_LW_LANE_CONFIG2E_REG, FFE_SLICER_CMIS_SIGMA_1))/10);
       ERR_HSIP(mpi_st_ptr->mpi_normalized_s[2] = (float)sqrt(hsip_rd_field_(&lw_phy_cfg,  COMMON_LW_LANE_CONFIG2F_REG, FFE_SLICER_CMIS_SIGMA_2))/10);
       ERR_HSIP(mpi_st_ptr->mpi_normalized_s[3] = (float)sqrt(hsip_rd_field_(&lw_phy_cfg,  COMMON_LW_LANE_CONFIG2F_REG, FFE_SLICER_CMIS_SIGMA_3))/10);
       
       for(sidx=0; sidx<4; sidx++){
            mpi_normalized_s_sqr[sidx ]  = (mpi_st_ptr->mpi_normalized_s[sidx]) * (mpi_st_ptr->mpi_normalized_s[sidx]);
            b21 += mpi_normalized_s_sqr[sidx ];
       }


       /* B11 = s1+2*s2+3*s3
          B21 = s0+s1+s2+s3
          Slope is = 0.2*B11 - 0.3*B21
          Intercept is = -0.3*B11 + 0.7*B21
          Then calculate MPI metric as: new_slope + 0.25*new_c0
       */
       b11                        = mpi_normalized_s_sqr[1] + 2*mpi_normalized_s_sqr[2] + 3*mpi_normalized_s_sqr[3];
       mpi_st_ptr->mpi_slope      = (float)(0.2*b11 - 0.3*b21);
       mpi_st_ptr->mpi_intercept  = (float)(-0.3*b11 + 0.7*b21);
       ERR_HSIP(  mpi_st_ptr->mpi_metric = (float)hsip_rd_reg_(&lw_phy_cfg,  COMMON_LW_LANE_CONFIG2B_REG)/1000000);
       return RR_SUCCESS;
   }
    return return_result;
}

/**
 * @brief      host_set_mpi_dynamic_config(capi_phy_info_t*    phy_info_ptr, 
 *                                          dsp_mission_mpi_cfg_info_t *mpi_cfg_ptr)
 *
 * @details    get mpi configuration information
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      mpi_cfg_ptr      : mpi config data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_set_mpi_dynamic_config(capi_phy_info_t*            phy_info_ptr,
                                            dsp_mission_mpi_cfg_info_t *mpi_cfg_ptr)
{
    return_result_t     return_result = RR_ERROR_WRONG_INPUT_VALUE;
    uint8_t             lane_index;
    uint16_t            lane_mask = phy_info_ptr->lane_mask;
    phy_info_t          lw_phy_cfg;
    
   for (lane_index = 0; lane_index < MAX_LW_LANES; ++lane_index) {
       if ((lane_mask & (1 << lane_index)) == 0)
           continue;
       phy_info_ptr->lane_mask = (1 << lane_index);
       lw_util_init_lane_config_base_addr(phy_info_ptr, &lw_phy_cfg, lane_index);
       hsip_wr_field_(&lw_phy_cfg, COMMON_LW_LANE_CONFIG20_REG, MISSION_MPI_BLW_ST_CHK_CNT, mpi_cfg_ptr->mpi_chk_cnt);
       hsip_wr_field_(&lw_phy_cfg, COMMON_LW_LANE_CONFIG27_REG, MISSION_MPI_DETECT_SUM_TH, mpi_cfg_ptr->mpi_chk_th);
       hsip_wr_field_(&lw_phy_cfg, COMMON_LW_LANE_CONFIG1C_REG, LW_DISABLE_MISSION_MPI_DETECT,  mpi_cfg_ptr->mpi_enable?0:1);
       return_result = RR_SUCCESS;
   }
    return return_result;
}

/**
 * @brief      host_get_mpi_dynamic_config(capi_phy_info_t*    phy_info_ptr, 
 *                                          dsp_mission_mpi_cfg_info_t *mpi_cfg_ptr)
 *
 * @details    get mpi dynamic configuration information
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      mpi_cfg_ptr      : mpi dynamic config data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_get_mpi_dynamic_config(capi_phy_info_t*            phy_info_ptr,
                                            dsp_mission_mpi_cfg_info_t *mpi_cfg_ptr)
{
    return_result_t     return_result = RR_ERROR_WRONG_INPUT_VALUE;
    uint8_t             lane_index;
    uint16_t            lane_mask = phy_info_ptr->lane_mask;
    phy_info_t          lw_phy_cfg;
    
   for (lane_index = 0; lane_index < MAX_LW_LANES; ++lane_index) {
       if ((lane_mask & (1 << lane_index)) == 0)
           continue;
       phy_info_ptr->lane_mask = (1 << lane_index);
       lw_util_init_lane_config_base_addr(phy_info_ptr, &lw_phy_cfg, lane_index);
       ERR_HSIP( mpi_cfg_ptr->mpi_enable  = !((uint16_t) hsip_rd_field_(&lw_phy_cfg, COMMON_LW_LANE_CONFIG1C_REG, LW_DISABLE_MISSION_MPI_DETECT)));
       ERR_HSIP( mpi_cfg_ptr->mpi_chk_cnt = (uint16_t) hsip_rd_field_(&lw_phy_cfg, COMMON_LW_LANE_CONFIG20_REG, MISSION_MPI_BLW_ST_CHK_CNT));
       ERR_HSIP( mpi_cfg_ptr->mpi_chk_th  = (uint16_t) hsip_rd_field_(&lw_phy_cfg, COMMON_LW_LANE_CONFIG27_REG, MISSION_MPI_DETECT_SUM_TH));
       return  RR_SUCCESS;
   }
   return return_result;
}

/**
 * @brief      host_get_dynamic_mpi_state(capi_phy_info_t*    phy_info_ptr, dsp_mission_mpi_st_info_t *mpi_st_ptr)
 *
 * @details    get dynamic mpi state information
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      mpi_st_ptr      : mpi state data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_get_dynamic_mpi_state(capi_phy_info_t*    phy_info_ptr, dsp_mission_mpi_st_info_t *mpi_st_ptr)
{
    return_result_t     return_result = RR_ERROR_WRONG_INPUT_VALUE;
    uint8_t             lane_index;
    uint16_t            lane_mask = phy_info_ptr->lane_mask;
    phy_info_t          lw_phy_cfg;
    
    if(phy_info_ptr->core_ip != CORE_IP_MEDIA_DSP ||
        util_check_param_range(mpi_st_ptr->read_clear, 0, 1) == FALSE)
        return RR_ERROR_WRONG_INPUT_VALUE;

   for (lane_index = 0; lane_index < MAX_LW_LANES; ++lane_index) {
       if ((lane_mask & (1 << lane_index)) == 0)
           continue;
       phy_info_ptr->lane_mask = (1 << lane_index);
       lw_util_init_lane_config_base_addr(phy_info_ptr, &lw_phy_cfg, lane_index);
       
       ERR_HSIP( mpi_st_ptr->dust_indicator         = (uint8_t)hsip_rd_field_(&lw_phy_cfg, COMMON_LW_LANE_CONFIG1F_REG, MISSION_MPI_DUST_INDICATOR));
       ERR_HSIP( mpi_st_ptr->dust_indicator_sticky  = (uint8_t)hsip_rd_field_(&lw_phy_cfg, COMMON_LW_LANE_CONFIG1F_REG, MISSION_MPI_DUST_INDICATOR_STICKY));
       if(mpi_st_ptr->read_clear)
           hsip_wr_field_(&lw_phy_cfg, COMMON_LW_LANE_CONFIG1F_REG, MISSION_MPI_DUST_INDICATOR_STICKY, 0);
       ERR_HSIP( mpi_st_ptr->average_record         = (uint8_t)hsip_rd_field_(&lw_phy_cfg, COMMON_LW_LANE_CONFIG26_REG, MISSION_MPI_DETECT_AVERAGE_CNT));
       return RR_SUCCESS;
   }
    return return_result;
}

/**
 * @brief      host_set_tmsd_config(capi_phy_info_t*    phy_info_ptr, dsp_tmsd_cfg_info_t *tmsd_cfg_ptr)
 *
 * @details    set incoming traffic mode switch from 10XG to 5Xg detect configuration information
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      tmsd_cfg_ptr      : tmsd config data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_set_tmsd_config(capi_phy_info_t*    phy_info_ptr, dsp_tmsd_cfg_info_t *tmsd_cfg_ptr)
{
    return_result_t     return_result = RR_ERROR_WRONG_INPUT_VALUE;
    uint8_t             lane_index;
    uint16_t            lane_mask = phy_info_ptr->lane_mask;
    phy_info_t          lw_phy_cfg;
    
   for (lane_index = 0; lane_index < MAX_LW_LANES; ++lane_index) {
       if ((lane_mask & (1 << lane_index)) == 0)
           continue;
       phy_info_ptr->lane_mask = (1 << lane_index);
       lw_util_init_lane_config_base_addr(phy_info_ptr, &lw_phy_cfg, lane_index);
       
       hsip_wr_field_(&lw_phy_cfg, COMMON_LW_LANE_CONFIG2C_REG, 
                                DISABLE_TRAFFIC_SWITCH_DETECT_BASED_ADC, tmsd_cfg_ptr->tmsd_enable?0:1);
       return_result = RR_SUCCESS;
   }
    return return_result;
}

/**
 * @brief      host_get_tmsd_config(capi_phy_info_t*    phy_info_ptr, dsp_tmsd_cfg_info_t *tmsd_cfg_ptr)
 *
 * @details    get incoming traffic mode switch from 10XG to 5Xg detect configuration information
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      tmsd_cfg_ptr      : tmsd config data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_get_tmsd_config(capi_phy_info_t*    phy_info_ptr, dsp_tmsd_cfg_info_t *tmsd_cfg_ptr)
{
    return_result_t     return_result = RR_ERROR_WRONG_INPUT_VALUE;
    uint8_t             lane_index;
    uint16_t            lane_mask = phy_info_ptr->lane_mask;
    phy_info_t          lw_phy_cfg;
    
   for (lane_index = 0; lane_index < MAX_LW_LANES; ++lane_index) {
       if ((lane_mask & (1 << lane_index)) == 0)
           continue;
       phy_info_ptr->lane_mask = (1 << lane_index);
       lw_util_init_lane_config_base_addr(phy_info_ptr, &lw_phy_cfg, lane_index);
       ERR_HSIP( tmsd_cfg_ptr->tmsd_enable  = !((uint8_t) hsip_rd_field_(&lw_phy_cfg, COMMON_LW_LANE_CONFIG2C_REG, 
                                DISABLE_TRAFFIC_SWITCH_DETECT_BASED_ADC)));
       return_result =  RR_SUCCESS;
   }
    return return_result;
}

/**
 * @brief      host_get_tmsd_state(capi_phy_info_t*    phy_info_ptr, dsp_tmsd_st_t *tmsd_st_ptr)
 *
 * @details    get incoming traffic mode switch from 10XG to 5Xg detect state information
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      tmsd_st_ptr      : tmsd state data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_get_tmsd_state(capi_phy_info_t*    phy_info_ptr, dsp_tmsd_st_t *tmsd_st_ptr)
{
    return_result_t     return_result = RR_ERROR_WRONG_INPUT_VALUE;
    uint8_t             lane_index;
    uint16_t            lane_mask = phy_info_ptr->lane_mask;
    phy_info_t          lw_phy_cfg;
    
    if((phy_info_ptr->core_ip != CORE_IP_MEDIA_DSP && phy_info_ptr->core_ip != CORE_IP_HOST_DSP )||
        util_check_param_range(tmsd_st_ptr->read_clear, 0, 1) == FALSE)
        return RR_ERROR_WRONG_INPUT_VALUE;

   for (lane_index = 0; lane_index < MAX_LW_LANES; ++lane_index) {
       if ((lane_mask & (1 << lane_index)) == 0)
           continue;
       phy_info_ptr->lane_mask = (1 << lane_index);
       lw_util_init_lane_config_base_addr(phy_info_ptr, &lw_phy_cfg, lane_index);     
       ERR_HSIP( tmsd_st_ptr->tmsd_detected              = (uint8_t)hsip_rd_field_(&lw_phy_cfg, COMMON_LW_LANE_CONFIG24_REG,
                                                                                                TRAFFIC_SWITCH_DETECT_BASED_ADC_DETECTED));
       ERR_HSIP( tmsd_st_ptr->tmsd_detected_sticky  = (uint8_t)hsip_rd_field_(&lw_phy_cfg, COMMON_LW_LANE_CONFIG1D_REG, 
                                                                                                TRAFFIC_SWITCH_DETECT_BASED_ADC_DETECTED_STICKY));
       if(tmsd_st_ptr->read_clear)
           hsip_wr_field_(&lw_phy_cfg, COMMON_LW_LANE_CONFIG1D_REG, TRAFFIC_SWITCH_DETECT_BASED_ADC_DETECTED_STICKY, 0);
       return RR_SUCCESS;
   }
    return return_result;
}

/**
 * @brief      host_set_mpi_canceller_config(capi_phy_info_t*    phy_info_ptr, 
 *                                          dsp_mpi_canceller_cfg_info_t *mpi_cfg_ptr)
 *
 * @details   set mpi canceller configuration information
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      mpi_canceller_cfg_ptr      : mpi canceller config data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_set_mpi_canceller_config(capi_phy_info_t*            phy_info_ptr,
                                            dsp_mpi_canceller_cfg_info_t *mpi_canceller_cfg_ptr)
{
    return_result_t     return_result = RR_ERROR_WRONG_INPUT_VALUE;
    uint8_t             lane_index;
    uint16_t            lane_mask = phy_info_ptr->lane_mask;
    phy_info_t          lw_phy;
    
   for (lane_index = 0; lane_index < MAX_LW_LANES; ++lane_index) {
       if ((lane_mask & (1 << lane_index)) == 0)
           continue;
       phy_info_ptr->lane_mask = (1 << lane_index);
       lw_util_init_lane_hw_base_addr(phy_info_ptr, &lw_phy, lane_index);
       hsip_wr_field_(&lw_phy, FFE_EQ_CTRL_1, DC_WANDER_ENABLE, mpi_canceller_cfg_ptr->mpi_canceller_enable);
       return_result = RR_SUCCESS;
   }
    return return_result;
}

/**
 * @brief      host_get_mpi_canceller_config(capi_phy_info_t*            phy_info_ptr,
 *                                            dsp_mpi_canceller_cfg_info_t *mpi_canceller_cfg_ptr)
 *
 * @details    get mpi canceller configuration information
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      mpi_cfg_ptr      : mpi dynamic config data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_get_mpi_canceller_config(capi_phy_info_t*            phy_info_ptr,
                                            dsp_mpi_canceller_cfg_info_t *mpi_canceller_cfg_ptr)
{
    return_result_t     return_result = RR_ERROR_WRONG_INPUT_VALUE;
    uint8_t             lane_index;
    uint16_t            lane_mask = phy_info_ptr->lane_mask;
    phy_info_t          lw_phy;
    
   for (lane_index = 0; lane_index < MAX_LW_LANES; ++lane_index) {
       if ((lane_mask & (1 << lane_index)) == 0)
           continue;
       phy_info_ptr->lane_mask = (1 << lane_index);
       lw_util_init_lane_hw_base_addr(phy_info_ptr, &lw_phy, lane_index);
       ERR_HSIP( mpi_canceller_cfg_ptr->mpi_canceller_enable  = (uint16_t) hsip_rd_field_(&lw_phy, FFE_EQ_CTRL_1, DC_WANDER_ENABLE));
       return  RR_SUCCESS;
   }
   return return_result;
}

/**
 * @brief      host_get_dsp_debug_dump(capi_phy_info_t*    phy_info_ptr, 
 *                                      capi_dsp_debug_dump_t*      ddump_ptr)
 *
 * @details    Dump dsp debugging information
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      ddump_ptr      : dsp dump data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_get_dsp_debug_dump(capi_phy_info_t*    phy_info_ptr, 
                                        capi_dsp_debug_dump_t*      ddump_ptr)
{
    capi_phy_info_t     phy_info = {0}, dsp_cfg_phy={0};
    uint32_t            lo_addr, hi_addr, dsp_dump_baddr;
    uint8_t             lgap_len = sizeof(capi_dsp_debug_dump_info_t)*CAPI_DSP_DUMP_STAGE_MAX;
    uint8_t                ridx, sflag, lane_id = ddump_ptr->lane_id;
    uint32_t            *rd_ptr = (uint32_t *)ddump_ptr->dbg_dump;

    util_memcpy((void *)&phy_info, phy_info_ptr, sizeof(capi_phy_info_t));
    util_memset((void *)ddump_ptr, 0, sizeof(capi_dsp_debug_dump_t));
    phy_info.base_addr = OCTAL_TOP_REGS;
    ERR_HSIP(lo_addr     = (uint32_t)hsip_rd_reg_(&phy_info, LW_DSP_DEBUG_ADDR_LO_REG));
    ERR_HSIP(hi_addr     = (uint32_t)hsip_rd_reg_(&phy_info, LW_DSP_DEBUG_ADDR_HI_REG));
    dsp_dump_baddr       = (hi_addr<<16)|lo_addr;
    if(phy_info_ptr->core_ip == CORE_IP_HOST_DSP)
        dsp_dump_baddr        += lgap_len*lane_id;
    else
        dsp_dump_baddr        += lgap_len*(lane_id + MAX_LW_LANES);
    
    phy_info.base_addr = 0;
    for (ridx = 0; ridx < lgap_len/4; ridx++) {     
        *rd_ptr = hsip_rd_reg_(&phy_info, (dsp_dump_baddr + ridx * 4));
        rd_ptr++;
    }
    
    ddump_ptr->lane_id = lane_id;
    lw_util_init_lane_config_base_addr(phy_info_ptr, &dsp_cfg_phy, lane_id);
    sflag = (uint8_t)hsip_rd_field_(&dsp_cfg_phy, COMMON_LW_LANE_CONFIG1F_REG, DSP_DBG_DUMP_STAGE_FLAG);
    ddump_ptr->dbg_stage_1_flag = sflag & 0x1;
    ddump_ptr->dbg_stage_2_flag = (sflag & 0x2)?1:0;
    ddump_ptr->dbg_stage_3_flag = (sflag & 0x4)?1:0;

    return RR_SUCCESS;
}


/**
 * @brief      host_set_hw_gain2_a_config(capi_phy_info_t*    phy_info_ptr, 
 *                                          dsp_hw_gain2_adapt_cfg_info_t *hw_gain2_a_cfg_ptr)
 *
 * @details   set hardware gain2 adaptation configuration information
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      hw_gain2_a_cfg_ptr      : hardware gain2 adaptation config data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_set_hw_gain2_a_config(capi_phy_info_t*            phy_info_ptr,
                                            dsp_hw_gain2_adapt_cfg_info_t *hw_gain2_a_cfg_ptr)
{
    return_result_t     return_result = RR_ERROR_WRONG_INPUT_VALUE;
    uint8_t             lane_index;
    uint16_t            lane_mask = phy_info_ptr->lane_mask;
    phy_info_t          lw_cfg_phy;
    
   for (lane_index = 0; lane_index < MAX_LW_LANES; ++lane_index) {
       if ((lane_mask & (1 << lane_index)) == 0)
           continue;
       phy_info_ptr->lane_mask = (1 << lane_index);
       lw_util_init_lane_config_base_addr(phy_info_ptr, &lw_cfg_phy, lane_index);
       hsip_wr_field_(&lw_cfg_phy, COMMON_LW_LANE_CONFIG21_REG, LW_DISABLE_HW_GAIN2_A_VAL_ADAPT, 
                                (hw_gain2_a_cfg_ptr->hw_gain2_adapt_enable)?0:1);
       return_result = RR_SUCCESS;
   }
    return return_result;
}

/**
 * @brief      host_get_hw_gain2_a_config(capi_phy_info_t*            phy_info_ptr,
 *                                            dsp_hw_gain2_adapt_cfg_info_t *hw_gain2_a_cfg_ptr)
 *
 * @details    get hardware gain2 adaptation configuration information
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      hw_gain2_a_cfg_ptr      : hardware gain2 adaptation config data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_get_hw_gain2_a_config(capi_phy_info_t*            phy_info_ptr,
                                            dsp_hw_gain2_adapt_cfg_info_t *hw_gain2_a_cfg_ptr)
{
    return_result_t     return_result = RR_ERROR_WRONG_INPUT_VALUE;
    uint8_t             lane_index;
    uint16_t            lane_mask = phy_info_ptr->lane_mask;
    phy_info_t          lw_cfg_phy;
    
   for (lane_index = 0; lane_index < MAX_LW_LANES; ++lane_index) {
       if ((lane_mask & (1 << lane_index)) == 0)
           continue;
       phy_info_ptr->lane_mask = (1 << lane_index);
       lw_util_init_lane_config_base_addr(phy_info_ptr, &lw_cfg_phy, lane_index);
       ERR_HSIP( hw_gain2_a_cfg_ptr->hw_gain2_adapt_enable  = 
                    (uint16_t) !hsip_rd_field_(&lw_cfg_phy, COMMON_LW_LANE_CONFIG21_REG, LW_DISABLE_HW_GAIN2_A_VAL_ADAPT));
       return  RR_SUCCESS;
   }
   return return_result;
}

static char* host_get_event_type_string(fw_event_type_t fw_event_type)
{    
    switch(fw_event_type){
        case FW_EVENT_TYPE_LW_RX_FAULT: return "LW_RX_FAULT";
        case FW_EVENT_TYPE_LW_HANDLE_TX_FAULT: return "LW_HANDLE_TX_FAULT";
        case FW_EVENT_TYPE_LW_HANDLE_CAPI_CMD: return "LW_HANDLE_CAPI_CMD";
        case FW_EVENT_TYPE_LW_HANDLE_CAPI_CMD_EX: return "LW_HANDLE_CAPI_CMD_EX";
        case FW_EVENT_TYPE_BH_HANDLE_CAPI_CMD: return "BH_HANDLE_CAPI_CMD";
        case FW_EVENT_TYPE_BH_HANDLE_CAPI_CMD_EX: return "BH_HANDLE_CAPI_CMD_EX";
        case FW_EVENT_TYPE_CW_HANDLE_CAPI_CMD: return "CW_HANDLE_CAPI_CMD";
        case FW_EVENT_TYPE_LW_PTUNE_SNR: return "LW_PTUNE_SNR";
        case FW_EVENT_TYPE_LW_PTUNE_BIAS: return "LW_PTUNE_BIAS";
        case FW_EVENT_TYPE_MEDIA_RX_FSM_STATE: return "MEDIA_RX_FSM_STATE";
        case FW_EVENT_TYPE_MEDIA_TX_FSM_STATE: return "MEDIA_TX_FSM_STATE";
        case FW_EVENT_TYPE_HOST_RX_FSM_STATE: return "HOST_RX_FSM_STATE";
        case FW_EVENT_TYPE_HOST_TX_FSM_STATE: return "HOST_TX_FSM_STATE";         
        case FW_EVENT_TYPE_HOST_FEC_DEC_ENABLE: return "HOST_FEC_DEC_ENABLE";
        case FW_EVENT_TYPE_MEDIA_FEC_DEC_ENABLE: return "MEDIA_FEC_DEC_ENABLE";         
        case FW_EVENT_TYPE_HOST_FEC_TOT_FRAMES: return "HOST_FEC_TOT_FRAMES";
        case FW_EVENT_TYPE_MEDIA_FEC_TOT_FRAMES: return "MEDIA_FEC_TOT_FRAMES"; 
        case FW_EVENT_TYPE_HOST_FEC_LOCK: return "HOST_FEC_LOCK";
        case FW_EVENT_TYPE_MEDIA_FEC_LOCK: return "MEDIA_FEC_LOCK";
        case FW_EVENT_TYPE_CW_RX_FSM_STATE: return "CW_RX_FSM";
        case FW_EVENT_TYPE_CW_TX_FSM_STATE: return "CW_TX_FSM";
        case FW_EVENT_TYPE_LX_RX_FSM_STATE: return "HOST_RX";
        case FW_EVENT_TYPE_LW_PFTUNE: return "HOST_RX_PFTUNE";
        case FW_EVENT_TYPE_SET_FEC_LOL_STICKY: return "SET_FEC_LOL_STICKY";
        case FW_EVENT_TYPE_LW_DYNAMIC_TUNE: return "LW_DYNAMIC_TUNE";
        case FW_EVENT_TYPE_LW_DYNAMIC_TUNE_CAL_GRADIEN: return "LW_DYNAMIC_TUNE_CAL_GRADIEN";
        default: break;
    }
    return "UNKNOWN_EVENT";
}

/**
 * @brief  double host_lw_calculate_pam_snr(uint32_t mse_val)
 *
 * @details Calculate SNR from MSE Value at PAM4 mode  SNR = 10 log (signal_value) - 10 log (MSE/2^28)
 *
 * @param[in] slc_mode:    slicer mode
 * @param[in] mse_val:    32-bit MSE Value from FFE or DFE
 * 
 * @return SNR value in double
 */
static double host_lw_calculate_pam_snr(uint32_t mse_val)
{
    double mval, mse_int;

    mval = 5;   // PAM4 only, NRZ mode is 9.
    mse_int  = ((double)mse_val) / (1 << 28);
    if (mse_int == 0)
        mse_int = 0.0000001;

    return (10 * log10(mval) - 10 * log10(mse_int));
}


return_result_t host_dump_dsp_rx_diag_status(capi_phy_info_t* phy_info_ptr,
                                             dsp_rx_diag_status_t *rx_diag_status_ptr)
{
    return_result_t              return_result = RR_SUCCESS;
    uint8_t                      lane_index;
    uint16_t                     lane_mask = phy_info_ptr->lane_mask;
    phy_info_t                   lw_phy, lw_phy_cfg;
    capi_lane_ctrl_info_t        capi_lane_ctrl_info = {0};
    
    for (lane_index = 0; lane_index < MAX_LW_LANES; ++lane_index) {
        if ((lane_mask & (1 << lane_index)) == 0)
            continue;
        
        phy_info_ptr->lane_mask = (1 << lane_index);
        lw_util_init_lane_hw_base_addr(phy_info_ptr, &lw_phy, lane_index);
        lw_util_init_lane_config_base_addr(phy_info_ptr, &lw_phy_cfg, lane_index);

        /*suspend LW RX*/
        capi_lane_ctrl_info.param.is.rx_suspend_resume = 1;
        capi_lane_ctrl_info.cmd_value.is.rx_suspend_resume = 1;
        return_result |= capi_set_lane_ctrl_info(phy_info_ptr, &capi_lane_ctrl_info);

        // vga
        rx_diag_status_ptr->vga = host_diag_get_vga(&lw_phy);

        // phase bias, static phase bias
        host_diag_get_phase_bias(&lw_phy, &lw_phy_cfg, &rx_diag_status_ptr->c_ph_bias, &rx_diag_status_ptr->s_ph_bias);

        // adc clipping
        rx_diag_status_ptr->adc_clip_cnt = host_diag_get_adcclip(&lw_phy, lane_index, 63);

        // ffe tap saturation 
        host_diag_get_ffe_saturation(&lw_phy, &rx_diag_status_ptr->tap_under_saturation, &rx_diag_status_ptr->tap_over_saturation);

        // get level snr and total snr
        host_diag_get_lvl_snr(&lw_phy, lane_index, rx_diag_status_ptr->level_snr, &rx_diag_status_ptr->snr);

        // get ffe taps
        host_diag_get_ffe_taps(&lw_phy, rx_diag_status_ptr->ffe_taps);

        rx_diag_status_ptr->afe_bw = host_diag_get_afe_bw(&lw_phy);

        rx_diag_status_ptr->gain_boost = host_diag_get_gain_boost(&lw_phy);

        rx_diag_status_ptr->peaking = host_diag_get_peaking(&lw_phy);

        rx_diag_status_ptr->processor_status = host_diag_get_processor_status(&lw_phy);

        rx_diag_status_ptr->cdr_lock_counter = host_diag_get_cdr_lock_counter(&lw_phy, lane_index);

        host_diag_get_ppm(&lw_phy, &rx_diag_status_ptr->tx_ppm, &rx_diag_status_ptr->rx_ppm);

        host_diag_get_port_status(&lw_phy, rx_diag_status_ptr->port_status);
        
        // gain3 a param
        if(phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP)
            host_lw_get_gain3_a_val(&lw_phy, lane_index, &(rx_diag_status_ptr->gain3_a));
        else
            rx_diag_status_ptr->gain3_a = 0.0;

        /*resume LW RX*/
        capi_lane_ctrl_info.param.is.rx_suspend_resume = 1;
        capi_lane_ctrl_info.cmd_value.is.rx_suspend_resume = 0;
        return_result |= capi_set_lane_ctrl_info(phy_info_ptr, &capi_lane_ctrl_info) ;
        
        return return_result;
    }
    
    return RR_ERROR_WRONG_INPUT_VALUE;
}

static uint8_t fsm_wo_lt_cu_rstr[][50] = {
    "RX_CU_INIT              ", /*0*/
    "RX_CU_ANALOG_CFG        ", /*1*/
    "RX_CU_DIGITAL_CFG       ", /*2*/
    "RX_CU_DIGITAL_EN        ", /*3*/
    "RX_CU_CHK_LOS           ", /*4*/
    "RX_CU_PATH_START        ", /*5*/
    "RX_CU_SKEW_START        ", /*6*/
    "RX_CU_COPPER_TRAIN      ", /*7*/
    "RX_CU_TR_START          ", /*8*/
    "RX_CU_EQ_SWITCH         ", /*9*/
    "RX_CU_CHK_SNR           ", /*A*/
    "RX_CU_CHK_PPM           ", /*B*/
    "RX_CU_TRACKING          ", /*C*/
    "RX_CU_PHASE_TUNE        ", /*D*/
    "RX_CU_MISSION           ", /*E*/
    "RX_CU_GLOBAL_LPBK       ", /*F*/
};
static void _lw_decode_fw_cu_rx_event(FILE* dump_file_ptr, uint32_t log_val)
{
    uint8_t cu_rx_fsm   = (uint8_t)(log_val >>28);
    uint8_t pk_bin      = (uint8_t)((log_val >> 23) & 0x1F);
    uint8_t pgac        = (uint8_t)((log_val >> 17) & 0x3F);
    float   snr         = (float)((log_val >> 7) & 0x3FF)/10;
    int8_t  bias        = (int8_t)signext((log_val & 0x7F), 1<<6);
    fprintf(dump_file_ptr, "%s pk_bin %d pgac %d snr %f bias %d", fsm_wo_lt_cu_rstr[cu_rx_fsm], pk_bin, pgac, snr, bias);
}
/**
 * @brief      host_dump_fw_events(capi_phy_info_t* phy_info_ptr, FILE* dump_file_ptr)
 * @details    This API is used to collect firmware events
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: pointer to the capi_phy_info_t struct variable
 * @param[in]  dump_handler: pointer to the file handler of the output
 *
 * @return     returns the performance result of the called method/function
 */
return_result_t host_dump_fw_events(capi_phy_info_t* phy_info_ptr, FILE* dump_file_ptr)
{
    phy_info_t fw_phy;
    uint32_t baseAddr, offset, eventBufferSize;
    firmware_event_log_buffer_t fw_event_buffer;
    uint32_t *fw_event_buffer_ptr = (uint32_t *)&fw_event_buffer;
    uint16_t event_log_filter_lsb, event_log_filter_msb; // event_log_filter;

    fprintf(dump_file_ptr, "Firmware event dump:\n");
    
    fw_phy.phy_id = phy_info_ptr->phy_id;
    fw_phy.base_addr = OCTAL_TOP_REGS;
    ERR_HSIP(baseAddr = (hsip_rd_reg_(&fw_phy, FW_EVENTS_ADDR_HI_REG) << 16) | (hsip_rd_reg_(&fw_phy, FW_EVENTS_ADDR_LO_REG) & 0xFFFF));
    if (baseAddr == 0) {
        CAPI_LOG_WARN("Event log is disabled in firmware\n ");
        return RR_WARNING_SYSTEM_UNAVAILABLE;        
    }
    fw_phy.base_addr = baseAddr;
    for (offset = 0; offset < 8; offset += 4) {
        ERR_HSIP(*fw_event_buffer_ptr = hsip_rd_reg_(&fw_phy, offset));
        fw_event_buffer_ptr++;            
    }

    // fw_event_buffer_size is size of event buffer in words (32 bits)
    eventBufferSize = fw_event_buffer.fw_event_buffer_size * 4;
    if (fw_event_buffer.fw_event_uuid != FM_EVENT_UUID || eventBufferSize != sizeof(firmware_event_log_buffer_t)) {
        fprintf(dump_file_ptr, "firmware event buffer is corrupted, UUID=0x%8X, Size=%u != %zu\n", fw_event_buffer.fw_event_uuid, eventBufferSize,
            sizeof(firmware_event_log_buffer_t));
    }
    else {
        uint8_t id;

        // pause FW event logging
        fw_phy.base_addr = OCTAL_TOP_REGS;
//        event_log_filter = (uint16_t)hsip_rd_reg_(&fw_phy, COMMON_FW_EVENT_LOGGING_FILTER_REG);
//        hsip_wr_reg_(&fw_phy, COMMON_FW_EVENT_LOGGING_FILTER_REG, 0xFFFF);        
        event_log_filter_lsb = (uint16_t)hsip_rd_reg_(&fw_phy, COMMON_FW_EVENT_LOGGING_FILTER_LSB_REG);
        event_log_filter_msb = (uint16_t)hsip_rd_reg_(&fw_phy, COMMON_FW_EVENT_LOGGING_FILTER_MSB_REG);
        hsip_wr_reg_(&fw_phy, COMMON_FW_EVENT_LOGGING_FILTER_LSB_REG, 0xFFFF);
        hsip_wr_reg_(&fw_phy, COMMON_FW_EVENT_LOGGING_FILTER_MSB_REG, 0xFFFF);

        fw_event_buffer_ptr = (uint32_t *)&fw_event_buffer;
        fw_phy.base_addr = baseAddr;
        for (offset = 0; offset < sizeof(firmware_event_log_buffer_t); offset += 4) {
            ERR_HSIP(*fw_event_buffer_ptr = hsip_rd_reg_(&fw_phy, offset));
            fw_event_buffer_ptr++;
        }

        // resume FW event logging
        fw_phy.base_addr = OCTAL_TOP_REGS;
//        hsip_wr_reg_(&fw_phy, COMMON_FW_EVENT_LOGGING_FILTER_REG, event_log_filter);
        hsip_wr_reg_(&fw_phy, COMMON_FW_EVENT_LOGGING_FILTER_LSB_REG, event_log_filter_lsb);
        hsip_wr_reg_(&fw_phy, COMMON_FW_EVENT_LOGGING_FILTER_MSB_REG, event_log_filter_msb);

        fprintf(dump_file_ptr, "Firmware events logged: 0x%x, event_id: %d - %d \n",
            fw_event_buffer.number_of_events,
            fw_event_buffer.oldest_event_id,
            fw_event_buffer.latest_event_id);

        for (id = 0; id < fw_event_buffer.number_of_events; id++) {
            uint8_t event_id = (fw_event_buffer.oldest_event_id + id) % MAX_FW_EVENTS;
            firmware_event_log_t *event = &fw_event_buffer.event_log[event_id];

            fprintf(dump_file_ptr, "Event: type %d %s, lane %u, ms_since %d, data 0x%x ",
                event->event_type,
                host_get_event_type_string((fw_event_type_t)event->event_type),
                event->lane_id,
                event->time_since_last_event,
                event->event_data);

            //log snr*10 bit[0,12]  phase bias [13:18] tune st [28:30] static/dynamic BIT [31]
            if (event->event_type == FW_EVENT_TYPE_LW_PTUNE_BIAS && event->event_data) {
                printf( ", %s PTUNE_STATE %d BIAS %d SNR %f", 
                                 (event->event_data & 0x80000000)?"Dynamic":"Static",
                                (event->event_data>>28)&0x7, 
                                (int8_t)signext((event->event_data>>13)&0x3F, 1<<5),
                                (float)(event->event_data & 0x1fFF)/100);
            }

            if (event->event_type == FW_EVENT_TYPE_LW_PTUNE_SNR && event->event_data) {
                double snr = host_lw_calculate_pam_snr(event->event_data);
                fprintf(dump_file_ptr, ", SNR %f", snr);
            }
      
            if (event->event_type == FW_EVENT_TYPE_LX_RX_FSM_STATE && event->event_data) {
                _lw_decode_fw_cu_rx_event(dump_file_ptr, event->event_data);
            }
             //log snr*10 bit[0,8]  is_pam[11] pgac [12:16] adcclip_st [17] phase bias [18:23] peaking [24:28] tune st [29:31] 
            if (event->event_type == FW_EVENT_TYPE_LW_PFTUNE && event->event_data) { 
                if(event->event_data&0x80000000)
                    //log snr*10 bit[0,9]  tune_idx[10:11] pgac [12:16] phase bias [17:22] peaking [23:27] tune st [28:30] step_jump[31]
                    fprintf(dump_file_ptr, ", MinorTune ST %d pf %d bias %d pgac %d une_idx %d SNR %f ", 
                                    (event->event_data>>28)&0x7, 
                                    (event->event_data>>23)&0x1F,
                                    (int8_t)signext((event->event_data>>17)&0x3F, 1<<5),  
                                    (event->event_data>>12)&0x1F,   
                                    ((event->event_data>>10)&0x3), 
                                    (float)(event->event_data & 0x3FF)/10);
                else //log snr*10 bit[0,9]  is_pam[10] pgac [11:15] adcclip_st [16] phase bias [17:22] peaking [23:27] tune st [28:30] 
                    fprintf(dump_file_ptr, ", DirectJump ST %d pf %d bias %d pgac %d %s SNR %f adcclip_probability %d", 
                                    (event->event_data>>28)&0x7, 
                                    (event->event_data>>23)&0x1F,
                                    (int8_t)signext((event->event_data>>17)&0x3F, 1<<5),  
                                    (event->event_data>>11)&0x1F,   
                                    ((event->event_data>>10)&0x1)?"PAM":"NRZ", 
                                    (float)(event->event_data & 0x3FF)/10,
                                    (event->event_data>>16)&0x1);
            }
            if (event->event_type == FW_EVENT_TYPE_MEDIA_RX_FSM_STATE && event->event_data) {
                if(event->event_data&0x80000000)
                    printf( ", cnt %d, stage %d, snr_th %f, ppm %d", (event->event_data>>25)&0x3F, 
                                                (event->event_data>>23)&0x3, 
                                                ((float)((event->event_data>>20)&0x7))/2.0, 
                                                (int16_t)signext((event->event_data&0xFFF), 1<<11));
            }
            //log snr*100 bit[0,12]  phase bias [13:18] tune st [19:21] dtune_type [22:23]
            if (event->event_type == FW_EVENT_TYPE_LW_DYNAMIC_TUNE && event->event_data) {
                fprintf(dump_file_ptr, ", tune_idx %d DTUNE %s st %d tuned_val %d SNR %f adcclip_probability %d",
                                (event->event_data>>24)&0x3,
                                ((event->event_data>>22)&0x3) == 0 ? "APB" : "AP", 
                                (event->event_data>>19)&0x7,  
                                (int8_t)signext((event->event_data>>13)&0x3F, 1<<5),
                                (float)(event->event_data & 0x1FFF)/100,
                                (event->event_data>>26)&0x1);
            }
            //log snr_diff*100 bit[0,9]  adcclip_hit [10:12] tune idx [13:14] tune_idx_bak [15:16] dtune_type [22:23] 
            if (event->event_type == FW_EVENT_TYPE_LW_DYNAMIC_TUNE_CAL_GRADIEN && event->event_data) {
                fprintf(dump_file_ptr, ", DTUNE %s tune_idx_bak %d tune_idx %d adcclip_probability %d %d %d  gradient %f", 
                                ((event->event_data>>22)&0x3) == 0 ? "APB" : "AP", 
                                (event->event_data>>15)&0x3,   
                                (event->event_data>>13)&0x3,   
                                (event->event_data>>10)&0x1,    
                                (event->event_data>>11)&0x1,    
                                (event->event_data>>12)&0x1,   
                                (float)(event->event_data & 0x3FF)/100.0);
            }
            
            //  fsm [0:3] falut [4:8] integ 12bit [9:20] log snr*10 bit[21,30] flag BIT [31]
            if ((event->event_type == FW_EVENT_TYPE_MEDIA_RX_FSM_STATE ||event->event_type == FW_EVENT_TYPE_HOST_RX_FSM_STATE)  && (event->event_data&0x80000000) ) {
                if(event->event_data&0x100)
                    fprintf(dump_file_ptr, " SNR %f integ %f sub_evt %d RX FSM 0x%x", 
                                (float)((event->event_data>>21) & 0x3FF)/10,
                                ((float)(signext((event->event_data>>9)&0xFFF, 1<<11))/10),
                                (event->event_data>>4)&0xF,
                                event->event_data&0xF);
                else
                    fprintf(dump_file_ptr, " SNR %f integ %f fault %d RX FSM 0x%x", 
                                (float)((event->event_data>>21) & 0x3FF)/10,
                                ((float)(signext((event->event_data>>9)&0xFFF, 1<<11))/10),
                                (event->event_data>>4)&0xF,
                                event->event_data&0xF);
            }
            
            fprintf(dump_file_ptr, "\n");
        }
    }
    return RR_SUCCESS;
}

/**
 * @brief      host_set_gain3_a_param_config(capi_phy_info_t*    phy_info_ptr, 
 *                                          dsp_gain3_a_param_cfg_info_t *gain3_a_cfg_ptr)
 *
 * @details   set gain3 manual a parameter configuration information
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      mpi_canceller_cfg_ptr      : mpi canceller config data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_set_gain3_a_param_config(capi_phy_info_t*            phy_info_ptr,
                                            dsp_gain3_a_param_cfg_info_t *gain3_a_cfg_ptr)
{
    return_result_t     return_result = RR_ERROR_WRONG_INPUT_VALUE;
    uint8_t             lane_index;
    uint16_t            lane_mask = phy_info_ptr->lane_mask;
    phy_info_t          lw_phy;
    
    for (lane_index = 0; lane_index < MAX_LW_LANES; ++lane_index) {
        if ((lane_mask & (1 << lane_index)) == 0)
            continue;
        phy_info_ptr->lane_mask = (1 << lane_index);
        lw_util_init_lane_config_base_addr(phy_info_ptr, &lw_phy, lane_index);
        hsip_wr_field_(&lw_phy, COMMON_LW_LANE_CONFIG18_REG, VA_PARAM_A_MANUAL_VAL, gain3_a_cfg_ptr->gain3_a_oval);
        hsip_wr_field_(&lw_phy, COMMON_LW_LANE_CONFIG18_REG, VA_PARAM_A_MANUAL_EN, gain3_a_cfg_ptr->gain3_a_override);
        hsip_wr_field_(&lw_phy, COMMON_LW_LANE_CONFIG19_REG, RX_RESTART_PROFILE, 1);
        return_result = RR_SUCCESS;
    }
    return return_result;
}

/**
 * @brief      host_get_gain3_a_param_config(capi_phy_info_t*    phy_info_ptr, 
 *                                          dsp_gain3_a_param_cfg_info_t *gain3_a_cfg_ptr)
 *
 * @details   get gain3 manual a parameter configuration information
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr  : phy info pointer
 * @param      mpi_canceller_cfg_ptr      : mpi canceller config data pointer
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_get_gain3_a_param_config(capi_phy_info_t*            phy_info_ptr,
                                            dsp_gain3_a_param_cfg_info_t *gain3_a_cfg_ptr)
{
    return_result_t     return_result = RR_ERROR_WRONG_INPUT_VALUE;
    uint8_t             lane_index;
    uint16_t            lane_mask = phy_info_ptr->lane_mask;
    phy_info_t          lw_phy;
    
   for (lane_index = 0; lane_index < MAX_LW_LANES; ++lane_index) {
       if ((lane_mask & (1 << lane_index)) == 0)
           continue;
       phy_info_ptr->lane_mask = (1 << lane_index);
       lw_util_init_lane_config_base_addr(phy_info_ptr, &lw_phy, lane_index);
       ERR_HSIP( gain3_a_cfg_ptr->gain3_a_oval  = (int8_t) hsip_rd_field_(&lw_phy, COMMON_LW_LANE_CONFIG18_REG, VA_PARAM_A_MANUAL_VAL));
       ERR_HSIP( gain3_a_cfg_ptr->gain3_a_override  = (uint8_t) hsip_rd_field_(&lw_phy, COMMON_LW_LANE_CONFIG18_REG, VA_PARAM_A_MANUAL_EN));
       return  RR_SUCCESS;
   }
   return return_result;
}



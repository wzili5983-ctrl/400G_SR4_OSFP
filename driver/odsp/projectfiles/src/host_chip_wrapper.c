/**
 *
 * @file    host_chip_wrapper.c
 * @author  Firmware Team
 * @date    03/24/2017
 * @version 0.1
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
#include "capi_def.h"
#include "lw_common_config_ind.h"
#include "hr_time.h"
#include "host_log_util.h"
#include "fw_gp_reg_map.h"
#include "regs_common.h"
#include "common_util.h"
#include "host_chip_wrapper.h"
#include "chip_common_config_ind.h"
#include "dsp_utils.h"

/**
* @brief    host_get_chip_info(capi_phy_info_t* phy_info_ptr, capi_chip_info_t* chip_info_ptr)
* @details  This API is used to get the Chip information
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[out] chip_info_ptr: this pointer contains chip info defined by capi_chip_info_t, which has 
*                            chip_id and chip_revision
* 
* @return     returns the result of the called methode/function, RR_SUCCESS
*/
return_result_t host_get_chip_info(capi_phy_info_t* phy_info_ptr, capi_chip_info_t* chip_info_ptr)
{
    uint32_t reg_val;
    uint32_t chip_id_msb;
    uint32_t chip_id_lsb;

    chip_info_ptr->chip_id  = 0;
    phy_info_ptr->base_addr = OCTAL_TOP_REGS;

    ERR_HSIP(reg_val = hsip_rd_reg_(phy_info_ptr, QUAD_CORE_CONFIG_CFG_CHIPID_0_RDB));
    chip_id_lsb = reg_val & QUAD_CORE_CONFIG_CFG_CHIPID_0_RDB_CFG_CHIPID_15_0_MASK;

    ERR_HSIP(chip_id_msb = hsip_rd_field_(phy_info_ptr, QUAD_CORE_CONFIG_CFG_CHIPID_1_RDB, CFG_CHIPID_19_16));

    chip_info_ptr->chip_id = chip_id_msb << 16 | chip_id_lsb;
    ERR_HSIP(chip_info_ptr->chip_revision = hsip_rd_field_(phy_info_ptr, QUAD_CORE_CONFIG_CFG_CHIPID_1_RDB, CFG_REVID));

    ERR_HSIP(chip_info_ptr->fw_major_version = (uint16_t) hsip_rd_reg_(phy_info_ptr, CHIP_TOP_CHIP_FIRMWARE_VERSION_REG));
    ERR_HSIP(chip_info_ptr->fw_minor_version = (uint16_t) hsip_rd_reg_(phy_info_ptr, CHIP_TOP_CHIP_FIRMWARE_VERSION_MINOR_REG_ADDR));

    ERR_HSIP(chip_info_ptr->capi_major_version = (uint16_t) hsip_rd_reg_(phy_info_ptr, CHIP_CAPI_MAJOR_VERSION_REG));
    ERR_HSIP(chip_info_ptr->capi_minor_version = (uint16_t) hsip_rd_reg_(phy_info_ptr, CHIP_CAPI_MINOR_VERSION_REG));
    ERR_HSIP(chip_info_ptr->upgrade_fw_major_version = (uint16_t) hsip_rd_reg_(phy_info_ptr, CHIP_STANDBY_FW_MAJOR_VERSION_REG));
    ERR_HSIP(chip_info_ptr->upgrade_fw_minor_version = (uint16_t) hsip_rd_reg_(phy_info_ptr, CHIP_STANDBY_FW_MINOR_VERSION_REG));
    return (RR_SUCCESS);
}

/**
* @brief      util_wait_for_uc_ready(capi_phy_info_t* phy_info_ptr)
* @details    This utility function to pool the FW ready state
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* 
* @return     returns the performance result of the called methode/function
*/
return_result_t util_wait_for_uc_ready(capi_phy_info_t* phy_info_ptr)
{
    uint16_t core_rdy_mask=0, core_rdy_time=0, reg_uc_rdy;

    capi_phy_info_t capi_phy;

    util_memset((void *)&capi_phy, 0, sizeof(capi_phy_info_t));
    util_memcpy((void *)&capi_phy, phy_info_ptr, sizeof(capi_phy_info_t));
    capi_phy.base_addr = OCTAL_TOP_REGS;

    core_rdy_mask = 0x07;

    /*Check and Wait the cores are ready to receive new config*/
    ERR_HSIP(reg_uc_rdy = (uint16_t) hsip_rd_reg_(&capi_phy, UC_READY_REG));
    while(((reg_uc_rdy&core_rdy_mask) != core_rdy_mask) &&  (core_rdy_time<CORE_READY_TIMER)) {
        delay_ms(10);
        core_rdy_time++;
        ERR_HSIP(reg_uc_rdy = (uint16_t) hsip_rd_reg_(&capi_phy, UC_READY_REG));
    }

    if(core_rdy_time>=CORE_READY_TIMER){
        PRINT_DIAG_OUT(("Fail: Core isn't ready 0x%lx \r\n", hsip_rd_reg_(&capi_phy, UC_READY_REG) ));
        return  RR_ERROR;
    }
    else

        return RR_SUCCESS;
}

/**
 * @brief    host_get_lpm_st(capi_phy_info_t* phy_info_ptr, capi_lpm_info_t* lpm_ptr)
 * @details  This API is used to get firmware LPM status: IN LPM(1) or NOT IN LPM(0)
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  lpm_ptr: a pointer for lpm information
 * 
 * @return     returns the result of the called methode/function
 */
return_result_t host_get_lpm_st(capi_phy_info_t* phy_info_ptr, capi_lpm_info_t* lpm_ptr)
{
    uint16_t lpm_st, pre_lpm_st,  lpm_cnt=0;

    phy_info_ptr->base_addr = OCTAL_TOP_REGS;
    ERR_HSIP(lpm_st = hsip_rd_field_(phy_info_ptr, OCW_CHIP_LPM_FW_REG, CHIP_IN_LPM));

    do {
        pre_lpm_st = lpm_st;
        ERR_HSIP(lpm_st = hsip_rd_field_(phy_info_ptr, OCW_CHIP_LPM_FW_REG, CHIP_IN_LPM));

        if(pre_lpm_st==lpm_st)
            lpm_cnt++;
        else
            lpm_cnt = 0;
    } while(lpm_cnt<5);

    lpm_ptr->lpm_en = (lpm_st?CAPI_ENABLE:CAPI_DISABLE);

    return RR_SUCCESS;
}

/**
 * @brief      host_get_gpr_lane_status(capi_phy_info_t* phy_info_ptr, capi_gpr_lane_status_t* status_ptr)
 * @details    Get lane status from GP registers
 *
 * @param      phy_info_ptr : phy information
 * @param      status_ptr : output gpr lane status
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t host_get_gpr_lane_status(capi_phy_info_t* phy_info_ptr, capi_gpr_lane_status_t* status_ptr)
{
    uint8_t         media_side;
    uint8_t         lane_idx      = 0;
    uint16_t        reg_value     = 0;
    uint32_t        gpr_address   = 0;
    return_result_t return_result = RR_SUCCESS;
    capi_phy_info_t phy_info;

    if ((phy_info_ptr->core_ip == CORE_IP_HOST_DSP) || (phy_info_ptr->core_ip == CORE_IP_HOST_SERDES) ) {
		media_side = 0;
	}
	else if ((phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP) || (phy_info_ptr->core_ip == CORE_IP_MEDIA_SERDES)) {
		media_side = 1;
	} else {
        return(RR_ERROR_WRONG_CORE_IP_VALUE);
    }

    util_memcpy(&phy_info, phy_info_ptr, sizeof(capi_phy_info_t));

    phy_info.base_addr = OCTAL_TOP_REGS;


    switch((capi_lane_status_type_t)status_ptr->status_type)
    {
        case GPR_LANE_CDR_LOCK_STATUS:
            gpr_address = HOST_LANE_CDR_LOCK_STATUS_GPREG;

            if (media_side) {
                gpr_address = MEDIA_LANE_CDR_LOCK_STATUS_GPREG;
            }
            ERR_HSIP(reg_value = (uint16_t)hsip_rd_reg_(&phy_info, gpr_address));
            status_ptr->param.cdr.lock_status = reg_value & phy_info.lane_mask;
            break;


        case GPR_LANE_LOL_LOS_STATUS:
            {
                uint16_t  loop_counter = 0;
                uint16_t  cdr_lol_sticky, los_lol_sticky, cdr_lol;
                uint16_t  los_reg_value, cdr_reg_val;
                uint32_t  lol_status_ack_gpr_address      = HOST_LANE_LOL_STATUS_ACK_GPREG;
                uint32_t  lol_los_status_ack_gpr_address  = HOST_LANE_LOL_LOS_STATUS_GPREG;
                uint32_t  cdr_lock_status_ack_gpr_address = HOST_LANE_CDR_LOCK_STATUS_GPREG;

                if (media_side) {
                    lol_status_ack_gpr_address      = MEDIA_LANE_LOL_STATUS_ACK_GPREG;
                    lol_los_status_ack_gpr_address  = MEDIA_LANE_LOL_LOS_STATUS_GPREG;
                    cdr_lock_status_ack_gpr_address = MEDIA_LANE_CDR_LOCK_STATUS_GPREG;
                }

                /*read ACK GP first, LOS_lol sticky LSB 8 bits and CDR_lol sticky MSB 8 bits;*/
                do{
                    ERR_HSIP(reg_value = (uint16_t)hsip_rd_reg_(&phy_info, lol_status_ack_gpr_address));
                    if (loop_counter++ > 1000) {
                        return_result = RR_WARNING_BUSY_TRY_LATER;
                        break;
                     }
                }while((reg_value & (phy_info.lane_mask<<8)) || (reg_value & phy_info.lane_mask));

                /**< Lane los status of host or media lanes : LSB 8bits sticky status; MSB 8bits real-time status                */
                ERR_HSIP(los_reg_value = (uint16_t)hsip_rd_reg_(&phy_info, lol_los_status_ack_gpr_address));  //LSB: real time status; MSB: sticky status
                los_lol_sticky = ((los_reg_value>>8)&phy_info.lane_mask);
                status_ptr->param.los.los_status = (((los_reg_value & phy_info.lane_mask)<<8 ) | los_lol_sticky);

                /**< Lane CDR lol status of host or media lanes: LSB 8bits sticky status; MSB 8bits real-time status               */
                ERR_HSIP(cdr_reg_val = (uint16_t)hsip_rd_reg_(&phy_info, cdr_lock_status_ack_gpr_address));  //LSB: real time status; MSB: sticky status
                cdr_lol_sticky = ((cdr_reg_val>>8)&phy_info.lane_mask);
                cdr_lol        = (~cdr_reg_val)&0xFF;
                status_ptr->param.los.lol_sticky = (((cdr_lol & phy_info.lane_mask)<<8 ) | cdr_lol_sticky);

                /* based on JIRA6469, los_lol follows with real signal _detect state */
                hsip_wr_reg_(&phy_info, lol_status_ack_gpr_address, (reg_value|los_lol_sticky|(cdr_lol_sticky<<8)));
            }
            break;

        case GPR_LANE_SIGDET_STATUS:
            gpr_address = HOST_LANE_LOL_LOS_STATUS_GPREG;

            if (media_side) {
                gpr_address = MEDIA_LANE_LOL_LOS_STATUS_GPREG;
            }

            ERR_HSIP(reg_value = (uint16_t)hsip_rd_reg_(&phy_info, gpr_address));
            reg_value = ~reg_value;
            reg_value &= 0x00FF;
            status_ptr->param.lane_sigdet_status = reg_value & phy_info.lane_mask;
            break;

        case GPR_LANE_TX_SQUELCH_STATUS:
            gpr_address = HOST_LANE_TX_SQUELCH_STATUS_GPREG;

            if (media_side) {
                gpr_address = MEDIA_LANE_TX_SQUELCH_STATUS_GPREG;
            }

            ERR_HSIP(status_ptr->param.lane_tx_squelch_status = (uint16_t)hsip_rd_reg_(&phy_info, gpr_address));
            status_ptr->param.lane_tx_squelch_status &= phy_info.lane_mask;
            break;

        case GPR_LANE_RX_OUTPUT_STATUS:
			if (media_side) {
				return(RR_ERROR_WRONG_CORE_IP_VALUE);
			}

            gpr_address = HOST_LANE_RX_OUTPUT_STATUS_GPREG;
            status_ptr->param.lane_rx_output_status = 0;

            ERR_HSIP(status_ptr->param.lane_rx_output_status = (uint16_t)hsip_rd_reg_(&phy_info, gpr_address));
            status_ptr->param.lane_rx_output_status &= phy_info.lane_mask;
            break;

        case GPR_LANE_RX_OUTPUT_LATCH_STATUS:
			if (media_side) {
				return(RR_ERROR_WRONG_CORE_IP_VALUE);
			}
			
			gpr_address = HOST_LANE_RX_OUTPUT_LATCH_STATUS_GPREG;

            status_ptr->param.lane_rx_output_latch_status = 0;

            ERR_HSIP(reg_value = (uint16_t)hsip_rd_reg_(&phy_info, gpr_address));

            status_ptr->param.lane_rx_output_latch_status = reg_value;
            status_ptr->param.lane_rx_output_latch_status &= phy_info.lane_mask;
            reg_value &= ~phy_info_ptr->lane_mask;
            hsip_wr_reg_(&phy_info, gpr_address, reg_value);
            break;

        case GPR_LANE_CDR_RESTART_COUNTER:
            {
                phy_info_t lw_phy;
                status_ptr->param.lane_cdr_restart_counter = 0;
                reg_value = 0;
                for (lane_idx = 0; lane_idx < MAX_LW_LANES; lane_idx++) {
                    if (phy_info_ptr->lane_mask & (1 << lane_idx)) {
                        lw_util_init_lane_config_base_addr(&phy_info, &lw_phy, lane_idx);
                        ERR_HSIP(status_ptr->param.lane_cdr_restart_counter =
                                hsip_rd_field_(&lw_phy, COMMON_LW_LANE_CONFIG1F_REG, CDR_RETRY_CNT));
                        break;
                    }
                }
                printf("CDR_RESTART_COUNT =  %d\n", status_ptr->param.lane_cdr_restart_counter);
            }
            break;

        case GPR_LANE_CDR_LOCK_COUNTER:
            {
                phy_info_t lw_phy;
                status_ptr->param.lane_cdr_restart_counter = 0;
                reg_value = 0;
                for (lane_idx = 0; lane_idx < MAX_LW_LANES; lane_idx++) {
                    if (phy_info_ptr->lane_mask & (1 << lane_idx)) {
                        lw_util_init_lane_config_base_addr(&phy_info, &lw_phy, lane_idx);
                        ERR_HSIP(reg_value =
                                hsip_rd_field_(&lw_phy, COMMON_LW_LANE_CONFIGD_REG, CDR_LOCK_COUNTER));
                        status_ptr->param.lane_cdr_lock_counter = reg_value;
                        break;
                    }
                }
                printf("CDR_LOCK_COUNT =  %d\n", reg_value);
            }
            break;

        case GPR_LANE_RX_MPI_STICKY_STATUS:
            {
                uint16_t  loop_counter = 0;
                
                /*read ACK GP first, MSB reflect the LOS LOL status is retreived or not*/
                do{
                    ERR_HSIP(reg_value = (uint16_t)hsip_rd_reg_(&phy_info, DSP_MEDIA_MPI_STICKY_ACK_REG));
                    if (loop_counter++ > 1000) {
                        return_result = RR_WARNING_IN_PROCESS_TRY_LATER;
                        break;
                    }
                }while(reg_value & phy_info.lane_mask);

                ERR_HSIP(reg_value = (uint16_t)hsip_rd_reg_(&phy_info, DSP_MEDIA_MPI_STICKY_ST_REG));
                status_ptr->param.lane_rx_mpi_status = (reg_value & 0x00FF) & phy_info.lane_mask;
                ERR_HSIP(reg_value = (uint16_t)hsip_rd_reg_(&phy_info, DSP_MEDIA_MPI_STICKY_ACK_REG));
                hsip_wr_reg_(&phy_info, DSP_MEDIA_MPI_STICKY_ACK_REG, reg_value|status_ptr->param.lane_rx_mpi_status);
            }
            break;

        case GPR_LANE_SNR_LTP_READY_STATUS:
            {
                capi_phy_info_t temp_phy_info;

                util_memset((void *)&temp_phy_info, 0x00, sizeof(capi_phy_info_t));

                if (phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP) {
                    gpr_address = MEDIA_SIDE_LANE_SNR_LTP_READY_STATUS;
                } else {
                    gpr_address = HOST_SIDE_LANE_SNR_LTP_READY_STATUS;
                }
                
                temp_phy_info.base_addr = OCTAL_TOP_REGS;

                ERR_HSIP(status_ptr->param.lane_snr_ltp_ready_status = (uint16_t)hsip_rd_reg_(&phy_info, gpr_address));
            }
            break;

        default:
            return_result = RR_ERROR_WRONG_INPUT_VALUE;
            CAPI_LOG_ERROR("ERROR: Invalid status_type\n");
            break;
    }

    return return_result;
}

/**
 * @brief    host_get_spi_info(capi_phy_info_t* phy_info_ptr, capi_spi_info_t* lpm_ptr)
 * @details  This API is used to get firmware spi information
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  capi_spi_info_ptr: a pointer for spi information
 * 
 * @return     returns the result of the called methode/function
 */
return_result_t host_get_spi_info(capi_phy_info_t* phy_info_ptr, capi_spi_info_t* capi_spi_info_ptr)
{
    capi_phy_info_t capi_phy={0};
    util_memcpy((void *)&capi_phy, phy_info_ptr, sizeof(capi_phy_info_t));
    capi_phy.base_addr = SPIF;
    ERR_HSIP(capi_spi_info_ptr->spi_ctrl_enable =(capi_spi_ctrl_enable_t)(hsip_rd_field_(&capi_phy, SPIF_CTRL, SPIFEN)));
    return(RR_SUCCESS);
}


/**
 * @brief    host_set_spi_info(capi_phy_info_t* phy_info_ptr, capi_spi_info_t* lpm_ptr)
 * @details  This API is used to get firmware spi information
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  capi_spi_info_ptr: a pointer for spi information
 * 
 * @return     returns the result of the called methode/function
 */
return_result_t host_set_spi_info(capi_phy_info_t* phy_info_ptr, capi_spi_info_t* capi_spi_info_ptr)
{
    capi_phy_info_t capi_phy={0};
    util_memcpy((void *)&capi_phy, phy_info_ptr, sizeof(capi_phy_info_t));
    capi_phy.base_addr = SPIF;
    hsip_wr_field_(&capi_phy, SPIF_CTRL, SPIFEN, capi_spi_info_ptr->spi_ctrl_enable);
    return(RR_SUCCESS);
}



/**
 * @brief    host_set_capi_feature_info(capi_phy_info_t* phy_info_ptr, capi_feature_info_t* feature_info_ptr)
 * @details  This API is used to configure the CAPI features
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  feature_info_ptr: a reference to the CAPI feature information object
 * 
 * @return     returns the result of the called methode/function
 */
return_result_t host_set_capi_feature_info(capi_phy_info_t* phy_info_ptr, capi_feature_info_t* feature_info_ptr)
{
    capi_phy_info_t     capi_phy_info         = *phy_info_ptr;
    capi_feature_info_t current_feature_info;

    capi_phy_info.base_addr = OCTAL_TOP_REGS;

    ERR_HSIP(current_feature_info.value.content = (uint16_t) hsip_rd_reg_(&capi_phy_info, CAPI_FEATURE_INFORMATION));

    if (feature_info_ptr->param.is.disable_sanity_checker) {
        current_feature_info.value.is.disable_sanity_checker = feature_info_ptr->value.is.disable_sanity_checker;
    }

    hsip_wr_reg_(&capi_phy_info, CAPI_FEATURE_INFORMATION, current_feature_info.value.content);

    return RR_SUCCESS;
}


/**
 * @brief    host_get_capi_feature_info(capi_phy_info_t* phy_info_ptr, capi_feature_info_t* feature_info_ptr)
 * @details  This API is used to get the configuration the CAPI features
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  feature_info_ptr: a reference to the CAPI feature information object
 * 
 * @return     returns the result of the called methode/function
 */
return_result_t host_get_capi_feature_info(capi_phy_info_t* phy_info_ptr, capi_feature_info_t* feature_info_ptr)
{
    capi_phy_info_t capi_phy_info = *phy_info_ptr;
    capi_phy_info.base_addr = OCTAL_TOP_REGS;

    ERR_HSIP(feature_info_ptr->value.content = (uint16_t) hsip_rd_reg_(&capi_phy_info, CAPI_FEATURE_INFORMATION));

    return RR_SUCCESS;
}


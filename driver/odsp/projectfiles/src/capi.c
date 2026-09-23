/**
 *
 * @file       capi.c
 * @author     PLP Firmware Team
 * @date       03/15/2020
 * @version    1.0
 *
 * @property 
 * $Copyright: (c) 2020 Broadcom.
 *             Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * @brief      The CAPI files contain all the CAPIs used by the the module and line card products.
 *
 * @section  description
 *
 */
#include "hr_time.h"
#include "chip_config_def.h"
#include "access.h"
#include "whole_image_sram.h"
#include "whole_image_spi.h"
#include "regs_common.h"
#include "chip_common_config_ind.h"
#include "ipc_regs.h"
#include "chip_mode_def.h"
#include "capi_def.h"
#include "host_diag_util.h"                  /* CMD_SANITY_CHECK() */
#include "common_util.h"
#include "host_diag_util.h"
#include "host_power_util.h"
#include "host_log_util.h"
#include "host_chip_wrapper.h"
#include "capi_test_def.h"
#include "capi_custom_def.h"
#include "host_lw_wrapper.h"
#include "host_download_util.h"
#include "host_to_chip_ipc.h"
#include "host_gpio_util.h"
#include "host_avs.h"
#include "chip_config_def.h"
#include "capi.h"

static return_result_t _capi_drop_xbar_ports(capi_phy_info_t* phy_info_ptr, uint8_t enable_xbar);

/************************************************************************************************************************/
/****************************************************Read Register*******************************************************/
/************************************************************************************************************************/
/**
* @brief         capi_read_register(capi_phy_info_t* phy_info_ptr, capi_reg_info_t* reg_info_ptr)
* @details       This API is used to read register/s
*
* @param[in]     phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in/out] reg_info_ptr: when calling, give the 32 bit reg_address element defined in
*                the capi_reg_info_t and return content element defined in  the capi_reg_info_t
* 
* @return        returns the result of the called methode/function
*/
return_result_t capi_read_register(capi_phy_info_t* phy_info_ptr, capi_reg_info_t* reg_info_ptr)
{
    /*Shall be move to Sanity Checker (Jian Guo)*/
    /*CAPI_LOG_FUNC("capi_read_register", phy_info_ptr);*/
    /*CAPI_LOG_INFO("register addr: 0x%x\n", reg_info_ptr->reg_address);*/

    phy_info_ptr->base_addr = 0x0;
    ERR_HSIP(reg_info_ptr->content  = hsip_rd_reg_(phy_info_ptr, reg_info_ptr->reg_address));
    return (RR_SUCCESS);
}

/************************************************************************************************************************/
/****************************************************Write Register******************************************************/
/************************************************************************************************************************/
/**
* @brief        capi_write_register(capi_phy_info_t* phy_info_ptr, capi_reg_info_t* reg_info_ptr)
* @details      This API is used to read register/s
*
* @param[in]    phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]    reg_info_ptr: this pointer contains the information to write to the register
*               give the 32 bit reg_address element defined in  the capi_reg_info_t
*               and register value, content element defined in  the capi_reg_info_t
* 
* @return       returns the result of the called methode/function
*/
return_result_t capi_write_register(capi_phy_info_t* phy_info_ptr, capi_reg_info_t* reg_info_ptr)
{
    /*Shall be move to Sanity Checker c/src/capi.c*/
    /*CAPI_LOG_FUNC("capi_write_register", phy_info_ptr);*/
    /*CAPI_LOG_INFO("register addr: 0x%x value 0x%x\n", reg_info_ptr->reg_address, reg_info_ptr->content);*/

    phy_info_ptr->base_addr = 0x0;
    hsip_wr_reg_(phy_info_ptr, reg_info_ptr->reg_address, reg_info_ptr->content);
    return (RR_SUCCESS);
}


/************************************************************************************************************************/
/****************************************************Get Chip Info*******************************************************/
/************************************************************************************************************************/
/**
* @brief        capi_get_chip_info(capi_phy_info_t* phy_info_ptr, capi_chip_info_t* chip_info_ptr)
* @details      This API is used to get the Chip information
*
* @param[in]    phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[out]   chip_info_ptr: this pointer contains chip info defined by capi_chip_info_t, which has 
*                              chip_id and chip_revision
* 
* @return       returns the result of the called methode/function, RR_SUCCESS
*/
return_result_t capi_get_chip_info(capi_phy_info_t* phy_info_ptr, capi_chip_info_t* chip_info_ptr)
{
    return(host_get_chip_info(phy_info_ptr, chip_info_ptr));
}

/**
* @brief      capi_get_chip_status(capi_phy_info_t*         phy_info_ptr,
*                                  capi_chip_status_info_t* chip_status_info_ptr)
* @details    This API is used to get the chip status
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  chip_status_info_ptr: pointer to capi_chip_status_info_t
* @return     returns the performance result of the called method/function
*/
return_result_t capi_get_chip_status(capi_phy_info_t* phy_info_ptr, capi_chip_status_info_t* chip_status_info_ptr)
{
    return_result_t return_result = RR_ERROR_WRONG_INPUT_VALUE;

    if (chip_status_info_ptr->param.is.download_done) {
        capi_download_info_t download_info;
        phy_static_config_t  static_config;

        util_memset(&download_info, 0, sizeof(capi_download_info_t));
        util_memset(&static_config, 0, sizeof(phy_static_config_t));

        download_info.mode                  = CAPI_DOWNLOAD_MODE_MDIO_SRAM;
        download_info.image_info.image_ptr  = (uint32_t *) NULL;
        download_info.phy_static_config_ptr = &static_config;

        return_result = capi_get_download_status(phy_info_ptr, &download_info);

        if (return_result) return(return_result);

        if (download_info.status.result == 0x1) {
             chip_status_info_ptr->value.download_done = 1;
        } else {
            return RR_ERROR_DOWNLOAD_FAILED;
        }
    }

    if (chip_status_info_ptr->param.is.avs_done) {
        capi_avs_status_t avs_status;
        util_memset(&avs_status, 0, sizeof(capi_avs_status_t));
        return_result = host_get_avs_status(phy_info_ptr, &avs_status);

        if (avs_status.avs_result == CAPI_INIT_DONE_SUCCESS) {
            chip_status_info_ptr->value.avs_done = 1;
        } else {
            chip_status_info_ptr->value.avs_done = 0;
            return(RR_ERROR_AVS_INITIALIZATION_TIME_OUT);
        }
        if (return_result) return(return_result);

    }

    if (chip_status_info_ptr->param.is.uc_ready) {
        if (util_wait_for_uc_ready(phy_info_ptr) == RR_SUCCESS) {
            chip_status_info_ptr->value.uc_ready = 1;
            return_result = RR_SUCCESS;
        } else {
            chip_status_info_ptr->value.uc_ready = 0;
            return_result = RR_ERROR_FIRMWARE_NOT_READY;
        }
    }

    if (chip_status_info_ptr->param.is.spi_dev_ready) {
        return RR_ERROR_WRONG_INPUT_VALUE;
    }
    return return_result;
}

/************************************************************************************************************************/
/****************************************************SET AND GET POLARITY************************************************/
/************************************************************************************************************************/
/**
* @brief        capi_set_polarity(capi_phy_info_t* phy_info_ptr, capi_polarity_info_t* polarity_info_ptr)
* @details      This API is used to set the polarity configuration 
*
* @param[in]    phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]    polarity_info_ptr: this pointer carries the necessary information to set polarity
* 
* @return       returns the performance result of the called methode/function
*/
return_result_t capi_set_polarity(capi_phy_info_t* phy_info_ptr, capi_polarity_info_t* polarity_info_ptr)
{
    capi_command_info_t command_info;

    command_info.command_id = COMMAND_ID_SET_POLARITY;
    command_info.type.polarity_info = *polarity_info_ptr;

    return(capi_command_request(phy_info_ptr, &command_info, sizeof(capi_polarity_info_t), SET_CONFIG));
}

/**
* @brief        capi_get_polarity(capi_phy_info_t* phy_info_ptr, capi_polarity_info_t* config_info_ptr)
* @details      This API is used to get the configuration information
*
* @param[in]    phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[out]   polarity_info_ptr: this pointer return polarity information
* 
* @return       returns the performance result of the called methode/function
*/
return_result_t capi_get_polarity(capi_phy_info_t* phy_info_ptr, capi_polarity_info_t* polarity_info_ptr)
{
    return_result_t     return_result = RR_SUCCESS;
    capi_command_info_t command_info;

    command_info.command_id = COMMAND_ID_GET_POLARITY;
    command_info.type.polarity_info = *polarity_info_ptr;

    return_result = capi_command_request(phy_info_ptr, &command_info, sizeof(capi_polarity_info_t), GET_CONFIG);
    *polarity_info_ptr = command_info.type.polarity_info;

    return(return_result);
}

/************************************************************************************************************************/
/****************************************************SET AND GET LANE CONFIG INFO*************************************************/
/************************************************************************************************************************/
/**
* @brief        capi_set_lane_config_info(capi_phy_info_t*         phy_info_ptr,
*                                         capi_lane_config_info_t* lane_config_info_ptr)
* @details      This API is used to
*
* @param[in]    phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]    lane_config_info_ptr: this parameter
*
* @return       returns the performance result of the called method/function
*/
return_result_t capi_set_lane_config_info(capi_phy_info_t*         phy_info_ptr,
                                          capi_lane_config_info_t* lane_config_info_ptr)
{
    capi_command_info_t command_info;
    return_result_t return_rslt;
    capi_phy_info_t capi_phy={0};
    uint8_t dport_st, chk_cnt=0;
    capi_chip_info_t  chip_info;
    
    /*auto fill the unused lanes' lane swap information*/
    util_memcpy((void *)&capi_phy, phy_info_ptr, sizeof(capi_phy_info_t));
    capi_get_chip_info(&capi_phy, &chip_info);
    util_memcpy((void *)&capi_phy, phy_info_ptr, sizeof(capi_phy_info_t));
    capi_phy.base_addr = OCTAL_TOP_REGS;
    if(lane_config_info_ptr->lane_config_type == LANE_CONFIG_TYPE_LANE_SWAP_INFO){
      /* Notify port drop */
      hsip_wr_reg_(&capi_phy, OCW_CHIP_POWER_DOWN_FW_CMD_REG, 1);
      /* check FW LPM status is set */             
        do {
            delay_ms(50);
            ERR_HSIP(dport_st = hsip_rd_field_(&capi_phy, OCW_CHIP_LPM_FW_REG, CHIP_IN_LPM));
        }while(chk_cnt++<100 && dport_st==0);
   }
    command_info.command_id = COMMAND_ID_SET_LANE_CONFIG_INFO;
    command_info.type.lane_config_info = *lane_config_info_ptr;

    return_rslt = capi_command_request(phy_info_ptr, &command_info, sizeof(capi_lane_config_info_t), SET_CONFIG);

   if(lane_config_info_ptr->lane_config_type == LANE_CONFIG_TYPE_LANE_SWAP_INFO ){
      
      /* Restore the dropped port */
      hsip_wr_reg_(&capi_phy, OCW_CHIP_POWER_DOWN_FW_CMD_REG, 0);

      /* check FW LPM status is clear */
      chk_cnt = 0;             
        do {
            delay_ms(50);
            ERR_HSIP(dport_st = hsip_rd_field_(&capi_phy, OCW_CHIP_LPM_FW_REG, CHIP_IN_LPM));
        }while(chk_cnt++<100 && dport_st);
      
      if(return_rslt!=RR_SUCCESS)
         return return_rslt;

      if(chk_cnt>=100 || dport_st )
         return RR_ERROR;
      else
         return RR_SUCCESS;
   }else
      return return_rslt;
}

/**
* @brief        capi_get_lane_config_info(capi_phy_info_t*         phy_info_ptr,
                                          capi_lane_config_info_t* lane_config_info_ptr)
* @details      This API is used to
*
* @param[in]    phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[out]   lane_config_info_ptr: this parameter
*
* @return       returns the performance result of the called method/function
*/
return_result_t capi_get_lane_config_info(capi_phy_info_t*         phy_info_ptr,
                                          capi_lane_config_info_t* lane_config_info_ptr)
{
    return_result_t return_result;
    capi_command_info_t command_info;

    command_info.command_id = COMMAND_ID_GET_LANE_CONFIG_INFO;
    command_info.type.lane_config_info = *lane_config_info_ptr;

    return_result = capi_command_request(phy_info_ptr, &command_info, sizeof(capi_lane_config_info_t), GET_CONFIG);
    *lane_config_info_ptr = command_info.type.lane_config_info;
    
    return(return_result);
}

/************************************************************************************************************************/
/**************************************************SET AND GET LANE CTRL INFO********************************************/
/************************************************************************************************************************/
/**
* @brief        capi_set_lane_ctrl_info(capi_phy_info_t*       phy_info_ptr,
*                                       capi_lane_ctrl_info_t* lane_ctrl_info_ptr)
* @details      This API is used to set the lane control information
*
* @param[in]    phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[out]   lane_ctrl_info_ptr: this parameter 
* 
* @return       returns the performance result of the called methode/function
*/
return_result_t capi_set_lane_ctrl_info(capi_phy_info_t*       phy_info_ptr,
                                        capi_lane_ctrl_info_t* lane_ctrl_info_ptr)
{
    capi_command_info_t command_info;

    command_info.command_id = COMMAND_ID_SET_LANE_CTRL_INFO;
    command_info.type.lane_ctrl_info = *lane_ctrl_info_ptr;

    return(capi_command_request(phy_info_ptr, &command_info, sizeof(capi_lane_ctrl_info_t), SET_CONFIG));
}

/**
* @brief        capi_get_lane_ctrl_info(capi_phy_info_t*       phy_info_ptr,
*                                       capi_lane_ctrl_info_t* lane_ctrl_info_ptr)
* @details      This API is used to get the lane control information
*
* @param[in]    phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]    lane_ctrl_info_ptr: this parameter 
* 
* @return       returns the performance result of the called method/function
*/
return_result_t capi_get_lane_ctrl_info(capi_phy_info_t*       phy_info_ptr,
                                        capi_lane_ctrl_info_t* lane_ctrl_info_ptr)
{
    return_result_t return_result;
    capi_command_info_t command_info;

    command_info.command_id = COMMAND_ID_GET_LANE_CTRL_INFO;
    command_info.type.lane_ctrl_info = *lane_ctrl_info_ptr;

    return_result = capi_command_request(phy_info_ptr, &command_info, sizeof(capi_lane_ctrl_info_t), GET_CONFIG);
    *lane_ctrl_info_ptr = command_info.type.lane_ctrl_info;

    return(return_result);
}

/************************************************************************************************************************/
/****************************************************GET LANE INFO*******************************************************/
/************************************************************************************************************************/
/**
* @brief        capi_get_lane_info(capi_phy_info_t* phy_info_ptr, capi_lane_info_t* lane_info_ptr)
* @details      This API is used to get the lane info
*
* @param[in]    phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[out]   lane_info_ptr: this pointer return lane status, refer to capi_lane_info_t
*
* @return       returns the performance result of the called method/function
*/
return_result_t capi_get_lane_info(capi_phy_info_t* phy_info_ptr, capi_lane_info_t* lane_info_ptr)
{
    return_result_t return_result;
    capi_command_info_t command_info;

    command_info.command_id = COMMAND_ID_GET_LANE_INFO;
    command_info.type.lane_info = *lane_info_ptr;

    return_result = capi_command_request(phy_info_ptr, &command_info, sizeof(capi_lane_info_t), GET_CONFIG);

    *lane_info_ptr = command_info.type.lane_info;

    return(return_result);
}



/************************************************************************************************************************/
/****************************************************SET AND GET Config**************************************************/
/************************************************************************************************************************/
/**
* @brief        capi_set_config(capi_phy_info_t* phy_info_ptr_in, capi_config_info_t* config_info_ptr)
* @details      This API is used to set the configuration information
*
* @param[in]    phy_info_ptr_in: a pointer which carries detail information necessary to identify the Phy
* @param[in]    config_info_ptr: this pointer carries the necessary information for configuration procedure
*   
* @return       returns the performance result of the called methode/function
*/
return_result_t capi_set_config(capi_phy_info_t* phy_info_ptr_in, capi_config_info_t* config_info_ptr)
{
    return_result_t     return_result = RR_SUCCESS;
    capi_command_info_t command_info;
    uint8_t             lpm_st = 0;
    uint8_t             counter_mode_config_in_progress = 0;
    uint16_t            is_chip_mode_config_in_progress = 1;
    uint32_t            port_mode_cfg_log;
    capi_phy_info_t     capi_phy = {0};
    capi_phy_info_t *   phy_info_ptr = &capi_phy;

    util_memcpy((void *)&capi_phy, phy_info_ptr_in, sizeof(capi_phy_info_t));

    /* drop all ports if crossbar is enabled */
    _capi_drop_xbar_ports(&capi_phy, 0);

    phy_info_ptr->core_ip = CORE_IP_CW;
    if (config_info_ptr->line_lane.lane_mask)
        phy_info_ptr->lane_mask = config_info_ptr->line_lane.lane_mask;
    else if (config_info_ptr->host_lane.lane_mask)
        phy_info_ptr->lane_mask = config_info_ptr->host_lane.lane_mask << 16;

    command_info.command_id = COMMAND_ID_SET_CONFIG_INFO;
    command_info.type.config_info = *config_info_ptr;
    
    return_result = capi_command_request(phy_info_ptr, &command_info, sizeof(capi_config_info_t), SET_CONFIG);

    if (return_result != RR_SUCCESS) return return_result;

    if(config_info_ptr->fec_monitor_port){
        return return_result;
    }
    phy_info_ptr->base_addr = OCTAL_TOP_REGS;

    while (is_chip_mode_config_in_progress) {
        delay_ms(10);
        counter_mode_config_in_progress++;
        ERR_HSIP(is_chip_mode_config_in_progress = (uint16_t)hsip_rd_reg_(phy_info_ptr, CHIP_MODE_FAST_CONFIG_COMMAND_REG));
        if (counter_mode_config_in_progress >= 200)
            return(RR_WARNING_BUSY_TRY_LATER);
    }

    ERR_HSIP(port_mode_cfg_log = hsip_rd_reg_(phy_info_ptr, OCTAL_PORT_MODE_CFG_LOG));
    CAPI_DEBUG_PRINTF("\n\nFIRMWRE CHIP MODE CONFIG LOG 0x%x\r\n", port_mode_cfg_log);
    ERR_HSIP(port_mode_cfg_log = hsip_rd_field_(phy_info_ptr,OCTAL_PORT_MODE_CFG_LOG, CFG_RESULT));

    if (port_mode_cfg_log != 0xF){
        CAPI_LOG_ERROR("FW report the CHIP MODE CONFIG fail \r\n");
        return RR_ERROR;
    }

    ERR_HSIP(lpm_st = (uint8_t)hsip_rd_field_(phy_info_ptr, OCW_CHIP_LPM_FW_REG, CHIP_IN_LPM));

    if (lpm_st == 0) {
        uint8_t lane_idx;
        uint16_t line_lane_cfg_done=0, host_lane_cfg_done=0;
        uint8_t line_lane_cfg_rdy=1, host_lane_cfg_rdy=1;
        uint8_t timeout, line_max_lane = 8, host_max_lane = 8, lane_cfg_rdy, lane_cw_cfg_rdy;
        uint8_t line_cfg_st[16] = {0}, host_cfg_st[16] = {0};
        /*check if all related lane get the done*/
        for (timeout=0; timeout<PORT_CFG_LANE_RDY_TIMEOUT; timeout++) {
            delay_ms(1);

            for (lane_idx=0; lane_idx<line_max_lane;lane_idx++) {
                /*if no valid lane or lane has responsed status, SKIP*/
                if ((config_info_ptr->line_lane.lane_mask&(1<<lane_idx))==0 || (line_lane_cfg_done&(1<<lane_idx)))
                    continue;
                phy_info_ptr->base_addr = OCTAL_TOP_REGS+(lane_idx<<2);
                ERR_HSIP(lane_cfg_rdy = hsip_rd_field_(phy_info_ptr, INTF_LWTOCW_COMMANDSTATUS_LANE0, LANE_CFG_STATUS));
                line_lane_cfg_done |= ((lane_cfg_rdy==INTF_LANE_ST_READY || lane_cfg_rdy==INTF_LANE_ST_FAIL)<<lane_idx);

                if (lane_cfg_rdy==INTF_LANE_ST_FAIL)
                    line_lane_cfg_rdy = 0;
                line_cfg_st[lane_idx] = lane_cfg_rdy;
            }

            for (lane_idx=0; lane_idx<host_max_lane;lane_idx++) {
                /*if no valid lane or lane has responsed status, SKIP*/
                if ((config_info_ptr->host_lane.lane_mask&(1<<lane_idx))==0 || (host_lane_cfg_done&(1<<lane_idx)))
                    continue;
                phy_info_ptr->base_addr = OCTAL_TOP_REGS+(lane_idx<<2);
                ERR_HSIP(lane_cfg_rdy = hsip_rd_field_(phy_info_ptr, INTF_BHTOCW_COMMANDSTATUS_LANE0, LANE_CFG_STATUS));
                host_lane_cfg_done |= ((lane_cfg_rdy==INTF_LANE_ST_READY || lane_cfg_rdy==INTF_LANE_ST_FAIL)<<lane_idx);
                if (lane_cfg_rdy==INTF_LANE_ST_FAIL)
                    host_lane_cfg_rdy = 0;
                host_cfg_st[lane_idx] = lane_cfg_rdy;
            }
        
            /*if(line_lane_cfg_done==cmode_lane_mask[port_entry.port_type-1].lw_lane_mask && host_lane_cfg_done==cmode_lane_mask[port_entry.port_type-1].bh_lane_mask)*/
            if (line_lane_cfg_done==config_info_ptr->line_lane.lane_mask && host_lane_cfg_done==config_info_ptr->host_lane.lane_mask)
                break;
        }
        /*Check and Wait the cores are ready to confirm the new config is IN*/
        if (timeout>=PORT_CFG_LANE_RDY_TIMEOUT || line_lane_cfg_rdy==0 || host_lane_cfg_rdy==0){
            CAPI_LOG_ERROR("Set config didn't get all lanes ready\n");
            return_result =  RR_ERROR;
        }

        /* LW lane calibration done takes >10ms *2*4*2 = 160ms */
        delay_ms(200);
        
        /*Check DW LW & BH lane config rdy status*/
        phy_info_ptr->base_addr = OCTAL_TOP_REGS;

        for (lane_idx=0; lane_idx<line_max_lane;lane_idx++) {
            ERR_HSIP(lane_cw_cfg_rdy =((hsip_rd_reg_(phy_info_ptr, OCW_LW_LANE_CONFIG_ST))>>(lane_idx<<1))&0x3);
            if(lane_cw_cfg_rdy!=line_cfg_st[lane_idx] && (config_info_ptr->line_lane.lane_mask&(1<<lane_idx)))
                return_result = RR_ERROR;
        }

        for(lane_idx=0; lane_idx<host_max_lane;lane_idx++) {
            ERR_HSIP(lane_cw_cfg_rdy = ((hsip_rd_reg_(phy_info_ptr, OCW_BH1_LANE_CONFIG_ST))>>((lane_idx%8)<<1))&0x3);
            if(lane_cw_cfg_rdy!=host_cfg_st[lane_idx]&& (config_info_ptr->host_lane.lane_mask&(1<<lane_idx)))
                return_result = RR_ERROR;
        }

#if (CAPI_LOG_LEVEL & CAPI_LOG_INFO_LEVEL)
        CAPI_LOG_INFO("Line side lane config status:");
        for(lane_idx=0; lane_idx<line_max_lane;lane_idx++) {
            /*if no valid lane or lane has responsed status, SKIP*/
            if(config_info_ptr->line_lane.lane_mask&(1<<lane_idx))
                CAPI_LOG_DATA("lane%d-%d;", lane_idx, line_cfg_st[lane_idx]);
        }

        CAPI_LOG_DATA("\n");
        CAPI_LOG_INFO("host side lane config status:");

        for (lane_idx=0; lane_idx<host_max_lane; lane_idx++) {
            /*if no valid lane or lane has responsed status, SKIP*/
            if (config_info_ptr->host_lane.lane_mask&(1<<lane_idx))
               CAPI_LOG_DATA("lane%d-%d;", lane_idx, host_cfg_st[lane_idx]);
        }

        CAPI_LOG_DATA("\n");
        CAPI_LOG_INFO("DW lane config status:\n");
        CAPI_LOG_INFO(" BH 0~7  0x%x \n", hsip_rd_reg_(phy_info_ptr, OCW_BH1_LANE_CONFIG_ST));
        CAPI_LOG_INFO(" LW 0~4  0x%x \n", hsip_rd_reg_(phy_info_ptr, OCW_LW_LANE_CONFIG_ST));
#endif
    }

    return return_result;
}

/**
* @brief        capi_get_config(capi_phy_info_t* phy_info_ptr_in, capi_config_info_t* config_info_ptr)
* @details      This API is used to get the configuration information
*
* @param[in]    phy_info_ptr_in: a pointer which carries detail information necessary to identify the Phy
* @param[out]   config_info_ptr: this pointer return configuration information
* 
* @return       returns the performance result of the called methode/function
*/
return_result_t capi_get_config(capi_phy_info_t* phy_info_ptr_in, capi_config_info_t* config_info_ptr)
{
    return_result_t     return_result = RR_SUCCESS;
    capi_command_info_t command_info;
    capi_phy_info_t     capi_phy = {0};
    capi_phy_info_t *   phy_info_ptr = &capi_phy;

    util_memcpy((void *)&capi_phy, phy_info_ptr_in, sizeof(capi_phy_info_t));

    phy_info_ptr->core_ip = CORE_IP_CW;
    if (config_info_ptr->line_lane.lane_mask)
        phy_info_ptr->lane_mask = config_info_ptr->line_lane.lane_mask;
    else if (config_info_ptr->host_lane.lane_mask)
        phy_info_ptr->lane_mask = config_info_ptr->host_lane.lane_mask << 16;

    command_info.command_id = COMMAND_ID_GET_CONFIG_INFO;
    command_info.type.config_info = *config_info_ptr;

    return_result = capi_command_request(phy_info_ptr, &command_info, sizeof(capi_config_info_t), GET_CONFIG);

    if (return_result != RR_SUCCESS) return return_result;

    *config_info_ptr = command_info.type.config_info;

    return(return_result);
}


/**
* @brief        capi_set_port(capi_phy_info_t* phy_info_ptr, capi_port_info_t* port_info_ptr)
* @details      This API drops the port/s
*
* @param[in]    phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]    port_info_ptr: reference to the port information object
*   
* @return       returns the performance result of the called methode/function
*/
static return_result_t _capi_set_port(capi_phy_info_t* phy_info_ptr, capi_port_info_t* port_info_ptr)
{
    capi_command_info_t command_info;
    return_result_t  return_result;
    uint16_t chk_loop=0, drop_in_progress;
    capi_phy_info_t  capi_phy;
    
    phy_info_ptr->core_ip = CORE_IP_CW;
    if(port_info_ptr->param.is.phy_side==PHY_HOST_SIDE)
        phy_info_ptr->lane_mask = (port_info_ptr->lane_mask << 16);
    else
        phy_info_ptr->lane_mask = port_info_ptr->lane_mask;

    command_info.command_id     = COMMAND_ID_SET_PORT_INFO;
    command_info.type.port_info = *port_info_ptr;

    return_result = capi_command_request(phy_info_ptr, &command_info, sizeof(capi_port_info_t), SET_CONFIG);
    if(return_result != RR_SUCCESS)
        return return_result;

    /*check drop port status*/
    util_memcpy((void *)&capi_phy, phy_info_ptr, sizeof(capi_phy_info_t));
    capi_phy.base_addr = OCTAL_TOP_REGS;
    do{
        ERR_HSIP(drop_in_progress = (uint16_t)hsip_rd_reg_(&capi_phy, CHIP_MODE_FAST_CONFIG_COMMAND_REG));
        chk_loop++;
        delay_ms(1);
    }while(drop_in_progress && chk_loop<3000);
    if(drop_in_progress)
        return RR_ERROR;
    else
        return RR_SUCCESS;
}

static bool _capi_is_mixed_mode_xbar_required(capi_port_info_t* port_info_ptr)
{
    uint8_t chip_mode = port_info_ptr->port.mode;
    uint16_t lw_lane_mask = (port_info_ptr->lane_mask & 0xFF);

    // have to enable quad swap in these mixed mode (4x100G port0 4x50G port1, 1x100G port0-3 1x50G port4-7, 1x100G port0-3 4x50G port4)
    if ((chip_mode == CHIP_MODE_4X106G_KP4PAM_TO_4X106G_KP4PAM && lw_lane_mask == 0xF) ||        
        (chip_mode == CHIP_MODE_4X53G_KP4PAM_TO_4X53G_KP4PAM && lw_lane_mask == 0xF0) ||
        (chip_mode == CHIP_MODE_1X106G_KP4PAM_TO_1X106G_KP4PAM && (lw_lane_mask & 0xF)) ||
        (chip_mode == CHIP_MODE_1X53G_KP4PAM_TO_1X53G_KP4PAM && (lw_lane_mask & 0xF0)))
        return TRUE;
    else
        return FALSE;
}

return_result_t capi_set_port(capi_phy_info_t* phy_info_ptr, capi_port_info_t* port_info_ptr)
{
    if (port_info_ptr->param.is.add_port) {
        uint8_t          enable_xbar;
        return_result_t  return_result;

        /* enable crossbar (0<->4, 1<->5, 2<->6, 3<->7) to swap CW quad 0 and 1 for some mix modes */
        if (port_info_ptr->param.is.mixed_mode == CAPI_MIXED_MODE_PORT_DEPENDENT && _capi_is_mixed_mode_xbar_required(port_info_ptr)) {
            enable_xbar = 1;
        } else {
            enable_xbar = 0;        
        }

        /* drop all ports if we have to change crossbar settings */
        return_result = _capi_drop_xbar_ports(phy_info_ptr, enable_xbar);
        if (return_result != RR_SUCCESS)
            return return_result;
    }
    
    return _capi_set_port(phy_info_ptr, port_info_ptr);
}

static return_result_t _capi_drop_xbar_ports(capi_phy_info_t* phy_info_ptr, uint8_t enable_xbar)
{
    uint8_t          xbar_enabled;
    capi_chip_info_t chip_info;
    capi_port_info_t drop_port_info = {0};
    return_result_t return_result = RR_SUCCESS;

    /* read existing crossbar setting */
    phy_info_ptr->base_addr = OCTAL_TOP_REGS;
    xbar_enabled = (uint8_t)hsip_rd_field_(phy_info_ptr, FW_INTERNAL_CONFIG_REG_2, QUAD_SWAP_EN);
    
    /* drop all ports if need to change crossbar settings */
    if (xbar_enabled != enable_xbar) {
        return_result_t return_result = host_get_chip_info(phy_info_ptr, &chip_info);
        if(return_result != RR_SUCCESS)
            return return_result;

        CAPI_LOG_INFO("drop all ports to change xbar\n");
        drop_port_info.param.is.drop_port = 1;
        drop_port_info.lane_mask = ALL_PORT_MASK;
        return_result = _capi_set_port(phy_info_ptr, &drop_port_info);
    }
    return return_result;
}

/**
* @brief        capi_get_port_info(capi_phy_info_t* phy_info_ptr, capi_port_info_t* port_info_ptr)
* @details      This API get the port config status
*
* @param[in]    phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in/out]    port_info_ptr: reference to the port information object
*   
* @return       returns the performance result of the called methode/function
*/
return_result_t capi_get_port_info(capi_phy_info_t* phy_info_ptr, capi_port_info_t* port_info_ptr)
{
    capi_command_info_t command_info;
    return_result_t  return_result;

    phy_info_ptr->core_ip = CORE_IP_CW;    
    if(port_info_ptr->param.is.phy_side==PHY_HOST_SIDE)
        phy_info_ptr->lane_mask = (port_info_ptr->lane_mask << 16);
    else
        phy_info_ptr->lane_mask = port_info_ptr->lane_mask;

    command_info.command_id     = COMMAND_ID_GET_PORT_INFO;
    command_info.type.port_info = *port_info_ptr;

    return_result = capi_command_request(phy_info_ptr, &command_info, sizeof(capi_port_info_t), GET_CONFIG);
    if(return_result != RR_SUCCESS)
        return return_result;

    *port_info_ptr = command_info.type.port_info;
    return(return_result);
}


/************************************************************************************************************************/
/****************************************************COMMAND REQIEST*****************************************************/
/************************************************************************************************************************/
/**
 * @brief      capi_command_request(capi_phy_info_t*     phy_info_ptr,
 *                                  capi_command_info_t* cmd_inf_ptr,
 *                                  uint16_t             payload_size)
 * @details    This API is used to send command to interface.
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  cmd_inf_ptr: a pointer which carries command 
 * @param[in]  payload_size: size of the payload type
 * @param[in]  config_cmd: the command configuration flag for set or get configuration
 * @return     returns the performance result of the called methode/function
 */
return_result_t capi_command_request(capi_phy_info_t*     phy_info_ptr,
                                     capi_command_info_t* cmd_inf_ptr,
                                     uint16_t             payload_size,
                                     uint8_t              config_cmd)
{
    return_result_t return_result = RR_SUCCESS;

    CAPI_LOG_FUNC("capi_command_request()", phy_info_ptr);
    CAPI_LOG_COMMAND_INFO(cmd_inf_ptr, config_cmd);
    CMD_SANITY_CHECK(phy_info_ptr, cmd_inf_ptr, CHK_USR_INPUT);

    return_result = intf_capi2fw_command_Handler(phy_info_ptr,
                                                 cmd_inf_ptr->command_id,
                                                 (uint8_t*)&cmd_inf_ptr->type.payload,
                                                 payload_size,
                                                 config_cmd);

    if (config_cmd == GET_CONFIG && return_result == RR_SUCCESS) {
        CMD_SANITY_CHECK(phy_info_ptr, cmd_inf_ptr, CHK_FW_OUTPUT);
    }

    return (return_result);
}

/************************************************************************************************************************/
/****************************************************Download Related CAPIs**********************************************/
/************************************************************************************************************************/


/************************************************************************************************************************/
/************************************************************RESET*******************************************************/
/************************************************************************************************************************/

/**
* @brief        capi_reset(capi_phy_info_t* phy_info_ptr, capi_reset_mode_t* reset_mode_ptr)
* @details      This API is used to reset the chip
*
* @param[in]    phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]    reset_mode_ptr: this pointer carries the necessary information for reseting procedure
*                               defined by capi_reset_mode_t
*
* @return       returns the  result of the called methode/function
*/
return_result_t capi_reset(capi_phy_info_t* phy_info_ptr, capi_reset_mode_t* reset_mode_ptr)
{
    uint32_t    i = 0;
    capi_phy_info_t fw_phy_core_top_reg;
    return_result_t ret_code = RR_SUCCESS;

    CAPI_LOG_FUNC("capi_reset", phy_info_ptr);
    CAPI_LOG_INFO("reset mode : %s\n", util_capi_reset_mode_str(*reset_mode_ptr));

    util_memcpy(&fw_phy_core_top_reg, phy_info_ptr, sizeof(phy_info_t));
    phy_info_ptr->base_addr = MGT;
    fw_phy_core_top_reg.base_addr = OCTAL_TOP_REGS;

    switch(*reset_mode_ptr) {

        /* boot_en_in pin is set */
        case CAPI_SOFT_RESET_MODE:

            hsip_wr_field_(&fw_phy_core_top_reg, REG_BOOT_EN_CTRL, BOOT_EN_VAL, 0x0);
            hsip_wr_field_(&fw_phy_core_top_reg, REG_BOOT_EN_CTRL, BOOT_EN_OVR, 0x1);

            hsip_wr_reg_(phy_info_ptr, PCR_RST_CTR,
                                       PCR_RST_CTR_M0P3_SRST_EN_MASK |
                                       PCR_RST_CTR_M0P2_SRST_EN_MASK |
                                       PCR_RST_CTR_M0P1_SRST_EN_MASK |
                                       PCR_RST_CTR_M0P0_SRST_EN_MASK);

            for (i=0; i<GP_CFG_NUM; i++) {
                hsip_wr_reg_(&fw_phy_core_top_reg, QUAD_CORE_CONFIG_CFGVAL_0_RDB + i*4, 0);
            }
            phy_info_ptr->base_addr = 0;
            for (i=0; i<((SRAM_GP_REGION_END-SRAM_GP_REGION_START)/4); i++){
                hsip_wr_reg_(phy_info_ptr, (SRAM_GP_REGION_START + i*4), 0);
            }
            phy_info_ptr->base_addr = MGT;
            hsip_wr_field_(&fw_phy_core_top_reg, QUAD_CORE_EGRMGT_QC_CHIP_SW_RST_RDB, MD_CHIP_SW_RST, 0x1);
            delay_ms(1);

            /* Wait for 1ms. */
            delay_ms(1);

            hsip_wr_reg_(phy_info_ptr, PCR_RST_CTR,
                                       PCR_RST_CTR_M0P3_SRST_EN_MASK |
                                       PCR_RST_CTR_M0P2_SRST_EN_MASK |
                                       PCR_RST_CTR_M0P1_SRST_EN_MASK |
                                       PCR_RST_CTR_M0P0_SRST_EN_MASK);
            hsip_wr_reg_(phy_info_ptr, COM_REMAP, 0x0);    /* disable remap */

            hsip_wr_reg_(phy_info_ptr, PCR_RST_CTR,
                                       PCR_RST_CTR_M0P3_SRST_EN_MASK |
                                       PCR_RST_CTR_M0P2_SRST_EN_MASK |
                                       PCR_RST_CTR_M0P1_SRST_EN_MASK);
            break;

        /* boot_en_in pin is set */
        case  CAPI_HARD_RESET_MODE:
            hsip_wr_reg_(phy_info_ptr, PCR_RST_CTR,
                                       PCR_RST_CTR_M0P3_SRST_EN_MASK |
                                       PCR_RST_CTR_M0P2_SRST_EN_MASK |
                                       PCR_RST_CTR_M0P1_SRST_EN_MASK |
                                       PCR_RST_CTR_M0P0_SRST_EN_MASK);

            hsip_wr_field_(&fw_phy_core_top_reg, REG_BOOT_EN_CTRL, BOOT_EN_VAL, 0x1);
            hsip_wr_field_(&fw_phy_core_top_reg, REG_BOOT_EN_CTRL, BOOT_EN_OVR, 0x1);

            /* clear all gpcfg regs for a clean env for now */
            for (i=0; i<GP_CFG_NUM; i++) {
                hsip_wr_reg_(&fw_phy_core_top_reg, QUAD_CORE_CONFIG_CFGVAL_0_RDB + i*4, 0);
            }
            phy_info_ptr->base_addr = 0;
            for (i=0; i<((SRAM_GP_REGION_END-SRAM_GP_REGION_START)/4); i++){
                hsip_wr_reg_(phy_info_ptr, (SRAM_GP_REGION_START + i*4), 0);
            }
            phy_info_ptr->base_addr = MGT;
            hsip_wr_field_(&fw_phy_core_top_reg, QUAD_CORE_EGRMGT_QC_CHIP_SW_RST_RDB, MD_CHIP_SW_RST, 0x1);
            delay_ms(1);

            break;

        default:
            ret_code = RR_ERROR_WRONG_INPUT_VALUE;
            break;
    }
    return (ret_code);
}

#ifndef CAPI_IMAGE_DOWNLOAD_DISABLED

/************************************************************************************************************************/
/**********************************************************DOWNLOAD******************************************************/
/************************************************************************************************************************/
/**
* @brief        capi_download(capi_phy_info_t*      phy_info_ptr,
*                             capi_download_info_t* download_info_ptr)
* @details      This API is used to download firmware 
*
* @param[in]    phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]    download_info_ptr: This pointer did not return download status because this API only download firmware
*               please use capi_get_download_status to get download status.
* 
* @return       returns the result of the called methode/function, either RR_ERROR or RR_SUCCESS
*/
return_result_t capi_download(capi_phy_info_t*      phy_info_ptr,
                              capi_download_info_t* download_info_ptr)
{
    return_result_t ret = RR_SUCCESS;
    capi_chip_info_t capi_chip_info;

    CAPI_LOG_FUNC("capi_download", phy_info_ptr);
    CAPI_LOG_INFO("mode : %s\n", util_capi_download_mode_str(download_info_ptr->mode));

    if (host_get_chip_info(phy_info_ptr, &capi_chip_info) != RR_SUCCESS) {
        CAPI_LOG_ERROR("get chip info failed\n");
        return RR_ERROR;
    }

    switch (download_info_ptr->mode) {

    case CAPI_DOWNLOAD_MODE_MDIO_EEPROM:
    case CAPI_DOWNLOAD_MODE_I2C_EEPROM:
        if((download_info_ptr->image_info.image_ptr == NULL) || (download_info_ptr->image_info.image_size == 0)) {
             download_info_ptr->image_info.image_ptr=(uint32_t*) &wholeimage_spi[0];
             download_info_ptr->image_info.image_size=sizeof(wholeimage_spi);
        }
        if (download_info_ptr->spi_program_mode==CAPI_SPI_PROGRAM_FAST)
            ret = host_download_spi_internal_fast(phy_info_ptr, download_info_ptr);
        else
            ret = host_download_spi_internal(phy_info_ptr, download_info_ptr);
        break;

    case CAPI_DOWNLOAD_MODE_MDIO_SRAM:
    case CAPI_DOWNLOAD_MODE_I2C_SRAM:

           host_pre_sram_download(phy_info_ptr);
           phy_info_ptr->base_addr = MGT;
           hsip_wr_reg_(phy_info_ptr, PCR_RST_CTR,
                                      PCR_RST_CTR_M0P3_SRST_EN_MASK |
                                      PCR_RST_CTR_M0P2_SRST_EN_MASK |
                                      PCR_RST_CTR_M0P1_SRST_EN_MASK |
                                      PCR_RST_CTR_M0P0_SRST_EN_MASK);

        if((download_info_ptr->image_info.image_ptr == NULL) || (download_info_ptr->image_info.image_size==0)) {
            download_info_ptr->image_info.image_ptr=(uint32_t*) &wholeimage_sram[0];
            download_info_ptr->image_info.image_size=sizeof(wholeimage_sram);

        }
        if (download_info_ptr->sram_program_mode==CAPI_SRAM_PROGRAM_FAST)
            ret = host_download_sram_fast(phy_info_ptr, download_info_ptr);
        else
            ret = host_download_sram(phy_info_ptr, download_info_ptr->image_info.image_ptr, download_info_ptr->image_info.image_size, download_info_ptr->phy_static_config_ptr);

        break;
    default:
        return (RR_ERROR_WRONG_INPUT_VALUE);
    }
    return ret;
}

/**
* @brief        capi_get_download_status(capi_phy_info_t*      phy_info_ptr,
*                                        capi_download_info_t* download_info_ptr)
* @details      This API is used to get firmware download status
*
* @param[in]    phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]    download_info_ptr: this pointer carries the necessary information for downloading status
*                                  defined by capi_download_info_t
* 
* @return       returns the  result of the called methode/function, either RR_ERROR or RR_SUCCESS
*/
return_result_t capi_get_download_status(capi_phy_info_t*      phy_info_ptr,
                                         capi_download_info_t* download_info_ptr)
{
    uint32_t read_version = 0, read_version_minor = 0;
    capi_phy_info_t  fw_phy;

    CAPI_LOG_FUNC("capi_get_download_status", phy_info_ptr);
    CAPI_LOG_INFO("download mode = %s\n", util_capi_download_mode_str(download_info_ptr->mode));

    download_info_ptr->status.crc_check = 0;

    util_memcpy(&fw_phy, phy_info_ptr, sizeof(capi_phy_info_t));

    download_info_ptr->status.result = TRUE;
    phy_info_ptr->base_addr = OCTAL_TOP_REGS;
    ERR_HSIP(read_version = hsip_rd_reg_(phy_info_ptr, CHIP_TOP_CHIP_FIRMWARE_VERSION_REG));
    ERR_HSIP(read_version_minor = hsip_rd_reg_(phy_info_ptr, CHIP_TOP_CHIP_FIRMWARE_VERSION_MINOR_REG_ADDR));

    download_info_ptr->status.version     = read_version;
    download_info_ptr->status.sub_version = read_version_minor;
   
    fw_phy.base_addr = OCTAL_TOP_REGS;  
    ERR_HSIP(download_info_ptr->status.crc_check = hsip_rd_reg_(&fw_phy, CHIP_TOP_CHIP_FIRMWARE_CRC_RET_REG));

    if (download_info_ptr->status.crc_check != 0x600d){
        download_info_ptr->status.result = FALSE;
        CAPI_LOG_ERROR(" CHIP CRC failed\n");
        return (RR_ERROR_CRC32_MISMATCH);
    }

    download_info_ptr->status.result = TRUE;

    CAPI_LOG_INFO("get download status success\n");
    return (RR_SUCCESS);
}

/**
* @brief    capi_get_download_crc_status(capi_phy_info_t*      phy_info_ptr)
* @details  This API is used to get firmware download crc status
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* 
* @return     returns the  result of the called methode/function, either RR_ERROR or RR_SUCCESS
*/
return_result_t capi_get_download_crc_status(capi_phy_info_t*  phy_info_ptr)
{
    uint32_t read_crc = 0; 
    capi_phy_info_t fw_phy;

    CAPI_LOG_FUNC("capi_get_download_crc", phy_info_ptr);

    util_memset((void *)&fw_phy, 0, sizeof(capi_phy_info_t));

    fw_phy.phy_id =phy_info_ptr->phy_id;
    fw_phy.base_addr = OCTAL_TOP_REGS;  

    ERR_HSIP(read_crc = hsip_rd_reg_(&fw_phy, CHIP_TOP_CHIP_FIRMWARE_CRC_RET_REG));

    if (read_crc != 0x600d){
        CAPI_LOG_ERROR(" CHIP CRC failed\n");
        return (RR_ERROR_CRC32_MISMATCH);
    }

    CAPI_LOG_INFO("get download crc status success\n");
    return (RR_SUCCESS);
}

/**
* @brief    capi_get_firmware_status(capi_phy_info_t* phy_info_ptr, capi_status_info_t* status_info_ptr)
* @details  This API is used checking the firmware status
*
* @param[in]    phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[out]   status_info_ptr: this pointer retuns the firmware status info defined by capi_status_info_t
* 
* @return    returns the result of the called methode/function, either RR_ERROR or RR_SUCCESS
*/
return_result_t capi_get_firmware_status(capi_phy_info_t* phy_info_ptr, capi_status_info_t* status_info_ptr)
{
    capi_phy_info_t              fw_phy;
    uint32_t major_version, minior_version;
    uint32_t read_crc = 0;

    CAPI_LOG_FUNC("capi_get_firmware_status", phy_info_ptr);

    util_memcpy(&fw_phy, phy_info_ptr, sizeof(phy_info_t));
    fw_phy.base_addr = OCTAL_TOP_REGS;

    fw_phy.phy_id = phy_info_ptr->phy_id;
    ERR_HSIP(major_version = hsip_rd_reg_(&fw_phy, CHIP_TOP_CHIP_FIRMWARE_VERSION_REG));
    ERR_HSIP(minior_version = hsip_rd_reg_(&fw_phy, CHIP_TOP_CHIP_FIRMWARE_VERSION_MINOR_REG_ADDR));
    status_info_ptr->version     = major_version;
    status_info_ptr->sub_version = minior_version;

    fw_phy.base_addr = OCTAL_TOP_REGS;  
   
    ERR_HSIP(read_crc = hsip_rd_reg_(&fw_phy, CHIP_TOP_CHIP_FIRMWARE_CRC_RET_REG));
    status_info_ptr->crc_check = read_crc;   
       
    if (read_crc != 0x600d){
        status_info_ptr->result = false;
        CAPI_LOG_ERROR(" CHIP CRC failed\n");
        return (RR_ERROR_CRC32_MISMATCH);
    }

    status_info_ptr->result = TRUE; 

    CAPI_LOG_INFO("_capi_get_firmware_status(version = %x, crc = 0x%x)\n", status_info_ptr->version, status_info_ptr->crc_check);
    return (RR_SUCCESS);
}

/************************************************************************************************************************/
/************************************************************************************************************************/
/************************************************************************************************************************/
#endif

/************************************************************************************************************************/
/****************************************************GET TEMPERTURE******************************************************/
/************************************************************************************************************************/
/**
* @brief        capi_get_temperture_status(capi_phy_info_t*         phy_info_ptr,
*                                          capi_temp_status_info_t* temp_status_info_ptr)
* @details      This API is used obtin the temperture in degree C
*
* @param[in]    phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]    capi_temp_status_info_ptr: this parameter pointed to the temperature infor defined by 
*                                          capi_temp_status_info_t
* 
* @return       returns the result of the called methode/function
*/
return_result_t capi_get_temperture_status(capi_phy_info_t*         phy_info_ptr,
                                           capi_temp_status_info_t* capi_temp_status_info_ptr)
{
    return_result_t ret = RR_ERROR;
    chip_top_chip_temp_reading_reg_t chip_top_chip_temp_reading_reg;
    uint32_t count = 200;

    CAPI_LOG_FUNC("capi_get_temperture_status", phy_info_ptr);
    phy_info_ptr->base_addr = OCTAL_TOP_REGS;
   
    switch(capi_temp_status_info_ptr->check_type) {

    case CAPI_CLEAR_WAIT_READ:
        {
            hsip_wr_reg_(phy_info_ptr, CHIP_TOP_CHIP_TEMP_READING_REG, 0x0);
            do {
                ERR_HSIP(chip_top_chip_temp_reading_reg.words = (uint16_t)hsip_rd_reg_(phy_info_ptr, CHIP_TOP_CHIP_TEMP_READING_REG));
                if (chip_top_chip_temp_reading_reg.fields.temp_reading_valid == 1)
                    break;
                delay_ms(10);
                count--;
            } while (count > 0);
        }
        break;

    case CAPI_ONLY_READ:
        {
            ERR_HSIP(chip_top_chip_temp_reading_reg.words = (uint16_t)hsip_rd_reg_(phy_info_ptr, CHIP_TOP_CHIP_TEMP_READING_REG));
        }
        break;

    case CAPI_CLEAR_ONLY:
        {
            hsip_wr_reg_(phy_info_ptr, CHIP_TOP_CHIP_TEMP_READING_REG, 0x0);
            return RR_SUCCESS;
        }
        break;

    default:
        {
            return RR_ERROR_WRONG_INPUT_VALUE;
        }

    }

    CAPI_LOG_INFO("capi_get_tempernure(valid: %d, Temp:%d) \n", chip_top_chip_temp_reading_reg.fields.temp_reading_valid, chip_top_chip_temp_reading_reg.fields.temp_reading);
    capi_temp_status_info_ptr->temperature = 0;
    if (chip_top_chip_temp_reading_reg.fields.temp_reading_valid == 1) {
        ret = RR_SUCCESS;
         if ((chip_top_chip_temp_reading_reg.fields.temp_reading & 0x7000) != 0x7000) {
            capi_temp_status_info_ptr->temperature = (int16_t)(chip_top_chip_temp_reading_reg.fields.temp_reading);
        } else {
            capi_temp_status_info_ptr->temperature = (int16_t)(chip_top_chip_temp_reading_reg.words);
        }
    }

    return ret;
}

/************************************************************************************************************************/
/****************************************************SET AND GET AVS*****************************************************/
/************************************************************************************************************************/
/**
* @brief        capi_set_avs_config(capi_phy_info_t*             phy_info_ptr,
*                                   capi_avs_mode_config_info_t* avs_config_ptr)
* @details      This API is used to set the AVS configuration
*
* @param[in]    phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]    avs_config_ptr: this pointer contains AVS mode configuration defined by capi_avs_mode_config_info_t
* 
* @return       returns the result of the called methode/function
*/
return_result_t capi_set_avs_config(capi_phy_info_t*             phy_info_ptr,
                                    capi_avs_mode_config_info_t* avs_config_ptr)
{
    chip_top_avs_mode_config_reg_t chip_top_avs_mode_config;
    CAPI_LOG_FUNC("capi_set_avs_config", phy_info_ptr);
    phy_info_ptr->base_addr = OCTAL_TOP_REGS;
    chip_top_avs_mode_config.words = (uint16_t)hsip_rd_reg_(phy_info_ptr, CHIP_TOP_CHIP_AVS_MODE_CONFIG_REG);
    if (chip_top_avs_mode_config.fields.avs_enable == avs_config_ptr->avs) {
        /* do not enable/disable if avs is already enabled/disabled */
        CAPI_LOG_ERROR("AVS already %s!\n", avs_config_ptr->avs ? "enabled" : "disabled");
        return RR_ERROR_WRONG_INPUT_VALUE;
    }
    chip_top_avs_mode_config.words = 0;
    chip_top_avs_mode_config.fields.avs_enable = avs_config_ptr->avs;
    chip_top_avs_mode_config.fields.disable_type = avs_config_ptr->disable_type;
    chip_top_avs_mode_config.fields.avs_regulator_control_mode = avs_config_ptr->avs_ctrl;
    chip_top_avs_mode_config.fields.num_of_phy = avs_config_ptr->package_share;
    chip_top_avs_mode_config.fields.avs_dc_margin = avs_config_ptr->board_dc_margin;
    chip_top_avs_mode_config.fields.type_of_regulator = avs_config_ptr->type_of_regulator;

    hsip_wr_reg_(phy_info_ptr, CHIP_TOP_CHIP_AVS_REGULATOR_I2C_ADDRESS_REG, (avs_config_ptr->regulator_i2c_address & 0xff));
    hsip_wr_reg_(phy_info_ptr, CHIP_TOP_CHIP_AVS_MODE_CONFIG_REG, chip_top_avs_mode_config.words);
    CAPI_LOG_INFO("capi_set_avs_config:%x\n", chip_top_avs_mode_config.words);
    return (RR_SUCCESS);
}

/**
* @brief        capi_get_avs_config(capi_phy_info_t*             phy_info_ptr,
*                                   capi_avs_mode_config_info_t* avs_config_ptr)
* @details      This API is used to set the AVS configuration
*
* @param[in]    phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]    avs_config_ptr: this pointer contains AVS mode configuration defined by 
*                               capi_avs_mode_config_info_t
* 
* @return       returns the result of the called methode/function
*/
return_result_t capi_get_avs_config(capi_phy_info_t*             phy_info_ptr,
                                    capi_avs_mode_config_info_t* avs_config_ptr)
{
    chip_top_avs_mode_config_reg_t chip_top_avs_mode_config;
    CAPI_LOG_FUNC("capi_get_avs_config", phy_info_ptr);

    phy_info_ptr->base_addr = OCTAL_TOP_REGS;
    ERR_HSIP(chip_top_avs_mode_config.words = (uint16_t) hsip_rd_reg_(phy_info_ptr, CHIP_TOP_CHIP_AVS_MODE_CONFIG_REG));
    CAPI_LOG_INFO("capi_get_avs_config:%x\n", chip_top_avs_mode_config.words);
    util_memset((void*)avs_config_ptr, 0, sizeof(capi_avs_mode_config_info_t));

    avs_config_ptr->avs = (capi_enable_t)chip_top_avs_mode_config.fields.avs_enable;

    avs_config_ptr->package_share     = (capi_avs_pack_share_t)chip_top_avs_mode_config.fields.num_of_phy;
    avs_config_ptr->board_dc_margin   = (capi_board_dc_margin_t)chip_top_avs_mode_config.fields.avs_dc_margin;
    avs_config_ptr->avs_ctrl          = (capi_avs_ctrl_t)chip_top_avs_mode_config.fields.avs_regulator_control_mode;
    avs_config_ptr->type_of_regulator = (capi_type_of_regulator_t)chip_top_avs_mode_config.fields.type_of_regulator;
    avs_config_ptr->disable_type      = (capi_avs_disable_type_t)chip_top_avs_mode_config.fields.disable_type;
    return (RR_SUCCESS);
}

/**
* @brief    capi_cleanup_avs_config_status(capi_phy_info_t*             phy_info_ptr)
* @details  This API is used to cleanup the AVS configuration and status information when
*           calling capi_set_avs_config() fails, for example, due to wrong i2c address
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* 
* @return     returns the result of the called method/function
*/
return_result_t capi_cleanup_avs_config_status(capi_phy_info_t*             phy_info_ptr)
{
    chip_top_avs_mode_config_reg_t chip_top_avs_mode_config;
    CAPI_LOG_FUNC("capi_cleanup_avs_config_status", phy_info_ptr);
    phy_info_ptr->base_addr = OCTAL_TOP_REGS;
    chip_top_avs_mode_config.words = (uint16_t)hsip_rd_reg_(phy_info_ptr, CHIP_TOP_CHIP_AVS_MODE_CONFIG_REG);
    chip_top_avs_mode_config.fields.avs_enable = ~chip_top_avs_mode_config.fields.avs_enable;
    chip_top_avs_mode_config.fields.avs_status = 0;
    hsip_wr_reg_(phy_info_ptr, CHIP_TOP_CHIP_AVS_MODE_CONFIG_REG, chip_top_avs_mode_config.words);
    CAPI_LOG_INFO("capi_cleanup_avs_config_status:%x\n", chip_top_avs_mode_config.words);
    return (RR_SUCCESS);
}

/**
 * @brief       capi_set_voltage_config(capi_phy_info_t*                  phy_info_ptr,
 *                                      capi_fixed_voltage_config_info_t* fixed_config_ptr)
 * @details     This API is used to set the fixed volatge 
 *
 * @param[in]   phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]   fixed_config_ptr: this pointer contains configuration defined by capi_fixed_voltage_config_info_t
 * 
 * @return      returns the result of the called methode/function
 */
return_result_t capi_set_voltage_config(capi_phy_info_t*                  phy_info_ptr,
                                        capi_fixed_voltage_config_info_t* fixed_config_ptr)
{
    chip_top_fixed_voltage_config_reg_t chip_top_fixed_voltage_config_reg;
    uint16_t value = 0;
    capi_phy_info_t fw_phy;
    uint32_t tries = 0;

    util_memcpy(&fw_phy, phy_info_ptr, sizeof(capi_phy_info_t));
    fw_phy.base_addr = OCTAL_TOP_REGS;
    chip_top_fixed_voltage_config_reg.words = 0;
    chip_top_fixed_voltage_config_reg.fields.fixed_enable = fixed_config_ptr->fixed_voltage_enable;
    chip_top_fixed_voltage_config_reg.fields.type_of_regulator = fixed_config_ptr->type_of_regulator;

    if((fixed_config_ptr->fixed_voltage == 0) || (fixed_config_ptr->fixed_voltage < 500)) {
        value = 500;
    }
    else {
        value = fixed_config_ptr->fixed_voltage;
    }
    if (fixed_config_ptr->capi_set_voltage_with_fw == CAPI_SET_VOLTAGE_WITH_FIRMWARE) {
        hsip_wr_reg_(&fw_phy, CHIP_TOP_CHIP_FIXED_VOLTAGE_INPUT_REG, value);
        hsip_wr_reg_(&fw_phy, CHIP_TOP_CHIP_AVS_REGULATOR_I2C_ADDRESS_REG, (fixed_config_ptr->regulator_i2c_address));
        hsip_wr_reg_(&fw_phy, CHIP_TOP_CHIP_FIXED_VOLTAGE_CONFIG_REG,  chip_top_fixed_voltage_config_reg.words);
        CAPI_DEBUG_PRINTF("capi_set_fixed voltage_config:%x\n", chip_top_fixed_voltage_config_reg.words);

        do  {
           ERR_HSIP(chip_top_fixed_voltage_config_reg.words = (uint16_t)hsip_rd_reg_(&fw_phy, CHIP_TOP_CHIP_FIXED_VOLTAGE_CONFIG_REG));
           if ((chip_top_fixed_voltage_config_reg.fields.fixed_enable==0) && (chip_top_fixed_voltage_config_reg.fields.set_status==1)){
               hsip_wr_reg_(&fw_phy, CHIP_TOP_CHIP_FIXED_VOLTAGE_CONFIG_REG,  0x0);
              return(RR_SUCCESS);

           }
           delay_ms(100);
           tries++;
        } while(tries < 100);

        if(tries >=100) {
            fw_phy.base_addr = OCTAL_TOP_REGS;
            hsip_wr_reg_(&fw_phy, CHIP_TOP_CHIP_FIXED_VOLTAGE_CONFIG_REG,  0x0);
            return(RR_ERROR);
        }

        hsip_wr_reg_(&fw_phy, CHIP_TOP_CHIP_FIXED_VOLTAGE_CONFIG_REG,  0x0);
    }
    else{
        fw_phy.base_addr = MGT;
        chal_ana_avs_set_volt(&fw_phy, (value* 1000));
    }
    return(RR_SUCCESS);
}

/**
* @brief        capi_get_voltage_status(capi_phy_info_t*            phy_info_ptr,
*                                       capi_voltage_status_info_t* capi_voltage_status_info_ptr)
* @details      This API is used obtin the voltage in MV
*
* @param[in]    phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]    capi_voltage_status_info_ptr: this parameter pointed to the volatge infor defined by 
*                                               capi_voltage_status_info_t
*
* @return       returns the result of the called methode/function
*/
return_result_t capi_get_voltage_status(capi_phy_info_t*                     phy_info_ptr,
                                                                     capi_voltage_status_info_t*  voltage_info_ptr)
{
    uint8_t             iteration_index   = 0;
    return_result_t     return_result     = RR_SUCCESS;
    chip_top_chip_volt_reading_reg_t chip_voltage_info = {0};

    phy_info_ptr->base_addr = OCTAL_TOP_REGS;

    /*Set to dafault value*/
    voltage_info_ptr->voltage = 0;

    if (voltage_info_ptr->check_type == CAPI_CLEAR_WAIT_READ) {
        /*First clear the voltage info register*/
        hsip_wr_reg_(phy_info_ptr, CHIP_TOP_CHIP_VOLT_READING_REG, 0x0);

        while (iteration_index++ < VOLTAGE_MAX_ITERATION) {
            delay_ms(VOLTAGE_WAIT_TIME_MS);     /* 10 ms wait time */
            ERR_HSIP(chip_voltage_info.words = (uint16_t)hsip_rd_reg_(phy_info_ptr, CHIP_TOP_CHIP_VOLT_READING_REG));
            if (chip_voltage_info.fields.voltage_reading_valid == 1) {
                voltage_info_ptr->voltage = chip_voltage_info.fields.volatge_reading;
                break;
            }

            if (iteration_index >= VOLTAGE_MAX_ITERATION) {
                return(RR_WARNING_WRONG_VOLTAGE_READING);
            }
        }
    }
    else {      /*CAPI_ONLY_READ*/
        ERR_HSIP(chip_voltage_info.words = (uint16_t)hsip_rd_reg_(phy_info_ptr, CHIP_TOP_CHIP_VOLT_READING_REG));
        if (chip_voltage_info.fields.voltage_reading_valid == 0) {
            return(RR_WARNING_WRONG_VOLTAGE_READING);
        }
        voltage_info_ptr->voltage = chip_voltage_info.fields.volatge_reading;
    }

    return(return_result);
}


return_result_t capi_set_internal_regulator_voltage(capi_phy_info_t*                   phy_info_ptr,
                                                    capi_internal_regulator_voltage_t* int_reg_voltage_ptr)
{
     return(host_set_internal_regulator_voltage(phy_info_ptr,int_reg_voltage_ptr));
}


/**
 * @brief    capi_disable_avs_slaves(capi_phy_info_t*                  phy_info_ptr,
                                              capi_disable_avs_slave_info__t* capi_disable_avs_slave_info_ptr )
 * @details  This API is used to disable avs slaves
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  capi_disable_avs_slave_info__ptr: this pointer contains avs slave defined
 *             by capi_disable_avs_slave_info__t
 * 
 * @return     returns the result of the called method/function
 */
return_result_t capi_disable_avs_slaves(capi_phy_info_t*                  phy_info_ptr,
                                        capi_disable_avs_slave_info__t*  capi_disable_avs_slave_info_ptr )
{

    uint32_t read_value = 0;
    uint32_t firmware_flag = 0;
    capi_phy_info_t fw_phy;
    capi_phy_info_t fw_phy_core_top_reg;
    return_result_t ret = RR_SUCCESS;

    uint32_t try_counter =0;

    util_memcpy(&fw_phy, phy_info_ptr, sizeof(phy_info_t));
    util_memcpy(&fw_phy_core_top_reg, phy_info_ptr, sizeof(phy_info_t));

    fw_phy_core_top_reg.base_addr = OCTAL_TOP_REGS;
    fw_phy_core_top_reg.phy_id = phy_info_ptr->phy_id;

    if (capi_disable_avs_slave_info_ptr->capi_set_voltage_with_fw == CAPI_SET_VOLTAGE_WITH_FIRMWARE) {
        hsip_wr_reg_(&fw_phy_core_top_reg, CHIP_TOP_CHIP_FIRMWARE_STATUS_P0_REG, 0); 

        do {
            ERR_HSIP(read_value = hsip_rd_reg_(&fw_phy_core_top_reg, CHIP_TOP_CHIP_FIRMWARE_STATUS_P1_REG));
            delay_ms(1);
        } while((read_value==0x0) && (try_counter++ < 10));

        if(read_value==0x0) {
            firmware_flag = 0;
        }
        else {
            firmware_flag = 1;
        }
        if (firmware_flag ==1) {
            ret = top_supspend_resume(phy_info_ptr, 1);
            if(ret!= RR_SUCCESS) return RR_ERROR;
        }
    }
    fw_phy.base_addr = MGT;
    switch(capi_disable_avs_slave_info_ptr->slave_enum) {
        case CAPI_DISABLE_1_SLAVE:
            chal_ana_reg_disable_avs_1_slaves(&fw_phy);
            break;
        case CAPI_DISABLE_2_SLAVE:
            chal_ana_reg_disable_avs_2_slaves(&fw_phy);
            break;
        case CAPI_DISABLE_3_SLAVE:
            chal_ana_reg_disable_avs_3_slaves(&fw_phy);
            break;
        case CAPI_DISABLE_ALL_SLAVE:
            chal_ana_reg_disable_avs_slaves(&fw_phy);
            break;
    }
    if (capi_disable_avs_slave_info_ptr->capi_set_voltage_with_fw == CAPI_SET_VOLTAGE_WITH_FIRMWARE) {
        if (firmware_flag ==1) {
            ret = top_supspend_resume(phy_info_ptr, 0);
            if(ret!= RR_SUCCESS) return RR_ERROR;
        }
    }
    return(RR_SUCCESS);
}

/**
 * @brief    capi_enable_avs_slaves(capi_phy_info_t*                  phy_info_ptr,
                                    capi_enable_avs_slave_info__t* capi_disable_avs_slave_info_ptr )
 * @details  This API is used to enable avs slaves
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  capi_enable_avs_slave_info__ptr: this pointer contains avs slave defined
 *             by capi_enable_avs_slave_info__t
 * 
 * @return     returns the result of the called method/function
 */
return_result_t capi_enable_avs_slaves(capi_phy_info_t*                  phy_info_ptr,
                                       capi_enable_avs_slave_info__t* capi_enable_avs_slave_info_ptr )
{

    uint32_t read_value =0;
    uint32_t firmware_flag = 0;
    capi_phy_info_t fw_phy;
    capi_phy_info_t fw_phy_core_top_reg;
    return_result_t ret = 0;

    uint32_t try_counter = 0;

    util_memcpy(&fw_phy, phy_info_ptr, sizeof(phy_info_t));
    util_memcpy(&fw_phy_core_top_reg, phy_info_ptr, sizeof(phy_info_t));

    fw_phy_core_top_reg.base_addr = OCTAL_TOP_REGS;
    fw_phy_core_top_reg.phy_id = phy_info_ptr->phy_id;

    if (capi_enable_avs_slave_info_ptr->capi_set_voltage_with_fw == CAPI_SET_VOLTAGE_WITH_FIRMWARE) {
        hsip_wr_reg_(&fw_phy_core_top_reg, CHIP_TOP_CHIP_FIRMWARE_STATUS_P0_REG, 0); 

        do {
            ERR_HSIP(read_value = hsip_rd_reg_(&fw_phy_core_top_reg, CHIP_TOP_CHIP_FIRMWARE_STATUS_P1_REG));
            delay_ms(1);
        } while((read_value==0x0) && (try_counter++ < 10));

        if(read_value==0x0) {
            firmware_flag = 0;
        }
        else {
            firmware_flag = 1;
        }
        if (firmware_flag ==1) {
            ret = top_supspend_resume(phy_info_ptr, 1);
            if(ret!= RR_SUCCESS) return RR_ERROR;
        }
    }
    fw_phy.base_addr = MGT;

    
    fw_phy.base_addr = MGT;
    switch(capi_enable_avs_slave_info_ptr->slave_enum) {
        case CAPI_ENAABLE_1_SLAVE:
            chal_ana_reg_enable_avs_slave1(&fw_phy);
            break;
        case CAPI_ENAABLE_2_SLAVE:
            chal_ana_reg_enable_avs_slave2(&fw_phy);
            break;
        case CAPI_ENAABLE_3_SLAVE:
            chal_ana_reg_enable_avs_slave3(&fw_phy);
            break;
        case CAPI_ENAABLE_4_SLAVE:
             chal_ana_reg_enable_avs_slave4(&fw_phy);
             break;
        case CAPI_ENABLE_ALL_SLAVE:
            chal_ana_reg_enable_avs_slaves(&fw_phy);
            break;
    }
    if (capi_enable_avs_slave_info_ptr->capi_set_voltage_with_fw == CAPI_SET_VOLTAGE_WITH_FIRMWARE) {
        if (firmware_flag ==1) {
            ret = top_supspend_resume(phy_info_ptr, 0);
            if(ret!= RR_SUCCESS) return RR_ERROR;
        }
    }
    return(RR_SUCCESS);
}

/**
 * @brief    capi_disable_vddm_slave(capi_phy_info_t*                  phy_info_ptr,
                                              capi_disable_vddm_slave_info__t* capi_disable_vddm_slave_info_ptr )
 * @details  This API is used to disable vddm slaves
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  capi_disable_vddm_slave_info__ptr: this pointer contains vddm slave defined
 *             by capi_disable_vddm_slave_info__t
 * 
 * @return     returns the result of the called method/function
 */
return_result_t capi_disable_vddm_slave(capi_phy_info_t*                  phy_info_ptr,
                                        capi_disable_vddm_slave_info__t* capi_disable_vddm_slave_info_ptr )
{
    capi_phy_info_t fw_phy;
    uint32_t read_value =0;
    capi_phy_info_t fw_phy_core_top_reg;
    return_result_t ret = RR_SUCCESS;
    uint32_t firmware_flag =0;

    uint32_t try_counter =0;

    util_memset(&fw_phy, 0, sizeof(phy_info_t));
    util_memcpy(&fw_phy, phy_info_ptr, sizeof(phy_info_t));
    util_memcpy(&fw_phy_core_top_reg, phy_info_ptr, sizeof(phy_info_t));

    fw_phy_core_top_reg.base_addr = OCTAL_TOP_REGS;
    fw_phy_core_top_reg.phy_id = phy_info_ptr->phy_id;

    if (capi_disable_vddm_slave_info_ptr->capi_set_voltage_with_fw == CAPI_SET_VOLTAGE_WITH_FIRMWARE) {
        hsip_wr_reg_(&fw_phy_core_top_reg, CHIP_TOP_CHIP_FIRMWARE_STATUS_P0_REG, 0); 

        do {
            ERR_HSIP(read_value = hsip_rd_reg_(&fw_phy_core_top_reg, CHIP_TOP_CHIP_FIRMWARE_STATUS_P1_REG));
            delay_ms(1);
        } while((read_value==0x0) && (try_counter++ < 10));

        if(read_value==0x0) {
            firmware_flag = 0;
        }
        else {
            firmware_flag = 1;
        }
        if (firmware_flag ==1) {
            ret = top_supspend_resume(phy_info_ptr, 1);
            if(ret!= RR_SUCCESS) return RR_ERROR;
        }
    }
    fw_phy.base_addr = MGT;
  
    if (capi_disable_vddm_slave_info_ptr->disable_or_eable==0){
        chal_ana_reg_disable_vddm_slave(&fw_phy);
    }
    else {
        chal_ana_reg_enable_vddm_slave(&fw_phy);
    }
        
    if (capi_disable_vddm_slave_info_ptr->capi_set_voltage_with_fw == CAPI_SET_VOLTAGE_WITH_FIRMWARE) {
        if (firmware_flag ==1) {
            ret = top_supspend_resume(phy_info_ptr, 0);
            if(ret!= RR_SUCCESS) return RR_ERROR;
        }
    }
    return(RR_SUCCESS);
}


/**
 * @brief    capi_disable_avdd_slave(capi_phy_info_t*                  phy_info_ptr,
                                              capi_disable_avdd_slave_info__t* capi_disable_avdd_slave_info_ptr )
 * @details  This API is used to disable avdd slaves
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  capi_disable_avdd_slave_info__ptr: this pointer contains avdd slave defined
 *             by capi_disable_avdd_slave_info__t
 * 
 * @return     returns the result of the called method/function
 */
return_result_t capi_disable_avdd_slave(capi_phy_info_t*                  phy_info_ptr,
                                        capi_disable_avdd_slave_info__t* capi_disable_avdd_slave_info_ptr )
{
    capi_phy_info_t fw_phy;
    uint32_t read_value =0;
    capi_phy_info_t fw_phy_core_top_reg;
    return_result_t ret = RR_SUCCESS;
    uint32_t firmware_flag =0;

    uint32_t try_counter =0;

    util_memset(&fw_phy, 0, sizeof(phy_info_t));
    util_memcpy(&fw_phy, phy_info_ptr, sizeof(phy_info_t));
    util_memcpy(&fw_phy_core_top_reg, phy_info_ptr, sizeof(phy_info_t));

    fw_phy_core_top_reg.base_addr = OCTAL_TOP_REGS;
    fw_phy_core_top_reg.phy_id = phy_info_ptr->phy_id;

    if (capi_disable_avdd_slave_info_ptr->capi_set_voltage_with_fw == CAPI_SET_VOLTAGE_WITH_FIRMWARE) {
        hsip_wr_reg_(&fw_phy_core_top_reg, CHIP_TOP_CHIP_FIRMWARE_STATUS_P0_REG, 0); 

        do {
            ERR_HSIP(read_value = hsip_rd_reg_(&fw_phy_core_top_reg, CHIP_TOP_CHIP_FIRMWARE_STATUS_P1_REG));
            delay_ms(1);
        } while((read_value==0x0) && (try_counter++ < 10));

        if(read_value==0x0) {
            firmware_flag = 0;
        }
        else {
            firmware_flag = 1;
        }
        if (firmware_flag ==1) {
            ret = top_supspend_resume(phy_info_ptr, 1);
            if(ret!= RR_SUCCESS) return RR_ERROR;
        }
    }
    fw_phy.base_addr = MGT;
  
    if (capi_disable_avdd_slave_info_ptr->disable_or_eable==0){
        chal_ana_reg_disable_avdd_slave(&fw_phy);
    }
    else {
        chal_ana_reg_enable_avdd_slave(&fw_phy);
    }
        
    if (capi_disable_avdd_slave_info_ptr->capi_set_voltage_with_fw == CAPI_SET_VOLTAGE_WITH_FIRMWARE) {
        if (firmware_flag ==1) {
            ret = top_supspend_resume(phy_info_ptr, 0);
            if(ret!= RR_SUCCESS) return RR_ERROR;
        }
    }
    return(RR_SUCCESS);
}
/**
 * @brief      capi_set_chip_command(capi_phy_info_t*          phy_info_ptr,
 *                                   capi_chip_command_info_t* chip_cmd_inf_ptr)
 * @details    This API is used to invoke chip set command
 *
 * @param[in]  phy_info_ptr: a reference to the phy information object
 * @param[in]  chip_cmd_inf_ptr: a reference to the chip command object 
 * @return     returns the performance result of the called method/function
 */
return_result_t capi_set_chip_command(capi_phy_info_t*          phy_info_ptr,
                                      capi_chip_command_info_t* chip_cmd_info_ptr)
{
    capi_command_info_t command_info;
    command_info.command_id = chip_cmd_info_ptr->command_id;

    switch(chip_cmd_info_ptr->command_id) 
    {
        case COMMAND_ID_SET_GPIO_INFO:
            return (host_set_gpio_info(phy_info_ptr, (capi_gpio_info_t*)&chip_cmd_info_ptr->type.gpio_info));

        case COMMAND_ID_SET_SPI_INFO:
            return (host_set_spi_info(phy_info_ptr, (capi_spi_info_t*)&chip_cmd_info_ptr->type.spi_info));

        case COMMAND_ID_SET_CAPI_FEATURE_INFO:
            return (host_set_capi_feature_info(phy_info_ptr, (capi_feature_info_t*) &chip_cmd_info_ptr->type.capi_feature_info));
         
        case COMMAND_ID_SET_PLL_CFG:      
            command_info.type.pll_info = chip_cmd_info_ptr->type.pll_info;
            return (capi_command_request(phy_info_ptr, &command_info, sizeof(capi_pll_info_t), SET_CONFIG));
            
        default:
            return RR_ERROR_WRONG_INPUT_VALUE;
    }
}

/**
 * @brief      capi_get_chip_command(capi_phy_info_t*          phy_info_ptr,
 *                                   capi_chip_command_info_t* chip_cmd_info_ptr)
 * @details    This API is used to invoke chip get command
 *
 * @param[in]  phy_info_ptr: a reference to the phy information object
 * @param[in]  chip_cmd_inf_ptr: a reference to the chip command object 
 * @return     returns the performance result of the called method/function
 */
return_result_t capi_get_chip_command(capi_phy_info_t*          phy_info_ptr,
                                      capi_chip_command_info_t* chip_cmd_info_ptr)
{
    switch(chip_cmd_info_ptr->command_id) 
    {
        case COMMAND_ID_GET_GPIO_INFO:
            return(host_get_gpio_info(phy_info_ptr, (capi_gpio_info_t*) &chip_cmd_info_ptr->type.gpio_info));

        case COMMAND_ID_GET_LOW_POWER_MODE:
            return(host_get_lpm_st(phy_info_ptr, &chip_cmd_info_ptr->type.lpm_info));
            
        case COMMAND_ID_GET_SPI_INFO:
            return(host_get_spi_info(phy_info_ptr, &chip_cmd_info_ptr->type.spi_info));

        case COMMAND_ID_GET_CAPI_FEATURE_INFO:
            return (host_get_capi_feature_info(phy_info_ptr, (capi_feature_info_t*) &chip_cmd_info_ptr->type.capi_feature_info));
            
        case COMMAND_ID_GET_PLL_CFG:      
            return( host_lw_get_pll_configuration(phy_info_ptr, &chip_cmd_info_ptr->type.pll_info));
            
        default:
            return RR_ERROR_WRONG_INPUT_VALUE;
    }
}

/**
* @brief    capi_set_fw_download_info(capi_phy_info_t* phy_info_ptr,
*                           capi_fw_download_info_t* fw_download_info_ptr)
* @details  This API is used to download/upgrade firmware in the flash
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  fw_download_info_ptr: This pointer which carries fw download/upgrade info
*
* @return     returns the result of the called method/function, either RR_ERROR or RR_SUCCESS
*/
return_result_t capi_set_fw_download_info(capi_phy_info_t* phy_info_ptr, capi_fw_download_info_t* fw_download_info_ptr)
{
    return_result_t ret = RR_SUCCESS;
    capi_phy_info_t capi_phy;

    util_memcpy((void *)&capi_phy, phy_info_ptr, sizeof(capi_phy_info_t));

    CAPI_LOG_FUNC("capi_set_fw_download_info", &capi_phy);

    if(fw_download_info_ptr->image.src_ptr == NULL || fw_download_info_ptr->image.size == 0) {
        fw_download_info_ptr->image.src_ptr = (uint32_t*) &wholeimage_spi[0];
        fw_download_info_ptr->image.size = sizeof(wholeimage_spi);
        CAPI_LOG_INFO("capi_set_fw_download_info: default wholeimage_spi and size are used!!!\n");
    }
    switch (fw_download_info_ptr->memory_type) {
        case MEMORY_TYPE_FLASH_HITLESS:
            fw_download_info_ptr->image.crc = IMAGE_UPGRADE_CRC;
            ret = host_flash_hitless_fw_upgrade(&capi_phy, fw_download_info_ptr);
            break;

        case MEMORY_TYPE_SRAM:
            //ret = host_sram_fw_download(&capi_phy, fw_download_info_ptr); /* TBD: normal SRAM download */
            ret = RR_ERROR_FEATURE_NOT_SUPPORTED;
            break;

        case MEMORY_TYPE_FLASH_VIA_SRAM:
            //ret = host_flash_fw_upgrade_sram_trial(&capi_phy, fw_download_info_ptr); /* TBD: non hitless via sram for trial run cases */
            ret = RR_ERROR_FEATURE_NOT_SUPPORTED;
            break;

        case MEMORY_TYPE_FLASH:
            //ret = host_flash_fw_download(&capi_phy, fw_download_info_ptr); /* TBD: Normal flash download */
            ret = RR_ERROR_FEATURE_NOT_SUPPORTED;
            break;

        default:
            ret = RR_WARNING_WRONG_INPUT_VALUE;
            break;
    }
   if (ret != RR_SUCCESS){
       CAPI_LOG_ERROR("capi_set_fw_download_info failed: error %d\n", ret);
   }
   return ret;
}

/************************************************************************************************************************/
/******************************************************* Get Status Info ************************************************/
/************************************************************************************************************************/
/**
 * @brief        capi_get_status_info(capi_phy_info_t* phy_info_ptr, capi_status_info_tt* status_info_ptr)
 * @details      This API retrieves the status information
 *
 * @param[in]    phy_info_ptr: a reference to the phy information object
 * @param[out]   status_info_ptr: a reference to the module status information object
 *
 * @return       returns the result of the called method/function, RR_SUCCESS
 */
return_result_t capi_get_status_info(capi_phy_info_t* phy_info_ptr, capi_status_info_tt* status_info_ptr)
{
    return_result_t     return_result = RR_SUCCESS;
    capi_command_info_t command_info;
    CAPI_LOG_FUNC("capi_get_status_info", phy_info_ptr);
    switch (status_info_ptr->status_info_type) {
        case STATUS_INFO_TYPE_FW_IMAGE_STATUS_INFO:
            return(host_fw_image_status(phy_info_ptr, &status_info_ptr->type.fw_image_status));
    }

    command_info.command_id = COMMAND_ID_GET_STATUS_INFO;
    command_info.type.status_info = *status_info_ptr;
    return_result = capi_command_request(phy_info_ptr, &command_info, sizeof(capi_status_info_t), GET_CONFIG);
    *status_info_ptr = command_info.type.status_info;
    return(return_result);
}

/**
* @brief    capi_override_upgrade_status(capi_phy_info_t* phy_info_ptr,
*                                        firmware_image_status_t* upgrade_status_ptr)
* @details  This API is used to override the firmware upgrade status in the flash
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  upgrade_status_ptr: a pointer which carries the override status
*
* @return     returns the result of the called method/function, either RR_ERROR or RR_SUCCESS
*/
return_result_t capi_override_upgrade_status(capi_phy_info_t* phy_info_ptr, firmware_image_status_t* upgrade_status_ptr)
{
    return_result_t ret = RR_SUCCESS;
    capi_phy_info_t capi_phy;

    util_memcpy((void *)&capi_phy, phy_info_ptr, sizeof(capi_phy_info_t));

    CAPI_LOG_FUNC("capi_override_upgrade_status", &capi_phy);

    ret = host_override_upgrade_status(&capi_phy, upgrade_status_ptr);

   if (ret != RR_SUCCESS){
       CAPI_LOG_ERROR("capi_override_upgrade_status failed: error %d\n", ret);
   }
   return ret;
}

/**
 * @brief      capi_get_status(capi_phy_info_t* phy_info_ptr, capi_gpr_lane_status_t* status_ptr)
 * @details    Get lane status from GP registers
 *
 * @param      phy_info_ptr : phy information
 * @param      status_ptr : output gpr lane status
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t capi_get_status(capi_phy_info_t* phy_info_ptr, capi_gpr_lane_status_t* status_ptr)
{
    return(host_get_gpr_lane_status(phy_info_ptr, status_ptr));
}

/**
 * @brief   capi_set_regulator_info(capi_phy_info_t*  phy_info_ptr, capi_regulator_info_t* regulator_info_ptr)
 * @details  This API is used to disable all regulator
 *
 * @param[in]  phy_info_ptr: a reference to the phy information object
 * @param[in]  regulator_info_ptr: a reference to the regulator information object
 * 
 * @return     returns the result of the called method/function
 */
return_result_t capi_set_regulator_info(capi_phy_info_t*  phy_info_ptr, capi_regulator_info_t* regulator_info_ptr)
{   phy_info_t phy_info_local;
	capi_regulator_info_t regulator_info;
    util_memcpy((void *)&phy_info_local, phy_info_ptr, sizeof(capi_phy_info_t));
    util_memset((void *)&regulator_info, 0, sizeof(capi_regulator_info_t));
    phy_info_local.base_addr = MGT;
    if(regulator_info_ptr->command == REGULATOR_CMD_DISABLE){
	   phy_info_local.base_addr = MGT;
       return(host_util_ana_disable_all_regs(&phy_info_local,regulator_info_ptr));
    }
    else {
       return(RR_ERROR_WRONG_INPUT_VALUE);
    }
}

/**
 * @brief    capi_disable_all_regs(capi_phy_info_t* phy_info_ptr)
 * @details  This API is used to disable all regulator
 *
 * @param[in]  phy_info_ptr: a reference to the phy information object
 * 
 * @return     returns the result of the called method/function
 */
return_result_t capi_disable_all_regs(capi_phy_info_t*  phy_info_ptr ){
    capi_phy_info_t phy_info_local;
    capi_regulator_info_t regulator_info;
    util_memcpy((void *)&phy_info_local, phy_info_ptr, sizeof(capi_phy_info_t));
    util_memset((void *)&regulator_info, 0, sizeof(capi_regulator_info_t));
    phy_info_local.base_addr = MGT;
    regulator_info.component = REGULATOR_COMPONENT_MASTER_ALL;
    return ( host_util_ana_disable_all_regs(phy_info_ptr, &regulator_info ) );
}


/**
 *
 * @file    host_to_chip_ipc.c
 * @author  Firmware Team
 * @date    01/03/2019
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
#include "hr_time.h"
#include "regs_common.h"
#include "chip_mode_def.h"
#include "capi_def.h"
#include "chip_common_config_ind.h"
#include "host_log_util.h"
#include "common_util.h"
#include "capi_fw_intf.h"
#include "host_chip_mem_map.h"
#include "host_to_chip_ipc.h"

//#define BURST_READ

static uint16_t cmd_tocken = 0;

#define IPC_PAYLOAD_OPTIMIZATION

#ifdef IPC_PAYLOAD_OPTIMIZATION
/**
 * @brief      void intf_util_get_optimized_req_payload_size(uint8_t   command_id,
 *                                                           uint8_t*  payload_ptr,
 *                                                           uint16_t* req_payload_sz_ptr)
 * @details    Optimizes payload size based on the command ID
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      command_id           : command ID of the payload
 * @param      payload_ptr          : pointer to payload
 * @param      req_payload_sz_ptr   : request payload size, updated if needed
 * @param      rsp_payload_sz_ptr   : response payload size, updated if needed
 */
static void intf_util_get_optimized_req_payload_size(uint8_t   command_id,
                                                     uint8_t*  payload_ptr,
                                                     uint16_t* req_payload_sz_ptr)
{
    (void)payload_ptr;
    switch ((phy_command_id_t)command_id)
    {
        case COMMAND_ID_GET_LANE_CONFIG_INFO:
            *req_payload_sz_ptr = sizeof(uint32_t) + /* sizeof(lane_config_type_t) */
                                  sizeof(uint32_t);  /* sizeof(param.content) */
            break;

        case COMMAND_ID_GET_LANE_INFO:
            *req_payload_sz_ptr = sizeof(uint32_t);  /* sizeof(param.content) */
            break;

        case COMMAND_ID_DIAG_LANE_STATUS:
            *req_payload_sz_ptr = sizeof(uint32_t);  /* sizeof(param.content) */
            break;

        case COMMAND_ID_GET_PRBS_INFO:
        case COMMAND_ID_GET_PRBS_STATUS:
        case COMMAND_ID_CLEAR_PRBS_STATUS:
            *req_payload_sz_ptr = sizeof(uint32_t);  /* sizeof(ptype) */
            break;

        default:
            /* Dont update request payload size */
            break;
    }
}
#endif


/*****************************************************************************************************/
/***************************************************IPC METHODE***************************************/
/*****************************************************************************************************/
/**
 * @brief      intf_util_assign_tocken(uint16_t* cmd_tocken_ptr)
 *
 * @details    This utility incrementally generate a new token
 *
 * @param[in]  dst_ptr: destination address (i.e. reference to the component object)
 * @param[in]  src_ptr: source address in SRAM
 * @param[in]  src_byte_length: source length in byte/s
 * 
 * @return     returns the result of the called methode/function
 */void intf_util_assign_tocken(uint16_t* cmd_tocken_ptr)
{
    cmd_tocken++;
    *cmd_tocken_ptr = cmd_tocken;
}

/**
 * @brief      intf_util_read_sram(capi_phy_info_t* phy_info_ptr,
 *                                 uint32_t* dst_ptr,
 *                                 uint32_t* src_ptr,
 *                                 uint16_t  src_byte_length)
 *
 * @details    This utility is to retrieve the command response payload from the FW via SRAM
 *
 * @param[in]  dst_ptr: destination address (i.e. reference to the component object)
 * @param[in]  src_ptr: source address in SRAM
 * @param[in]  src_byte_length: source length in byte/s
 * 
 * @return     returns the result of the called methode/function
 */
static return_result_t intf_util_read_sram(capi_phy_info_t* phy_info_ptr, uint32_t* dst_ptr, uint32_t* src_ptr, uint16_t src_byte_length)
{
    uint32_t data_index;
    uint8_t  fraction = src_byte_length % 4;
    uint32_t length   = src_byte_length / 4;

    phy_info_ptr->base_addr = 0;
    if (length) {
#ifdef IPC_BURST
        burst_read_chip_sram((uint32_t)src_ptr, length, dst_ptr, phy_info_ptr->phy_id);
        src_ptr += length;
        dst_ptr += length;
#else
        for (data_index = 0; data_index < length; data_index++) {
            ERR_HSIP(*dst_ptr++ = hsip_rd_reg_(phy_info_ptr, ((long)src_ptr++)));
        }
#endif
    }
    
    if (fraction) {
        ERR_HSIP(*dst_ptr = hsip_rd_reg_(phy_info_ptr, (long)src_ptr));
    }

    return RR_SUCCESS;
}

#ifdef BURST_READ
/**
 * @brief      intf_util_burst_read(capi_phy_info_t* phy_info_ptr, uint32_t* dst_ptr, uint32_t starting_address, uint32_t src_byte_length)
 * @details    Perform fast read of memory using incremental MDIO
 *
 * @param      phy_info_ptr     : pointer to phy info
 * @param      dst_ptr          : destination buffer
 * @param      starting_address : memory address to read
 * @param      src_byte_length  : length to read
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t intf_util_burst_read(capi_phy_info_t* phy_info_ptr, uint32_t* dst_ptr, uint32_t starting_address, uint32_t src_byte_length)
{
    uint8_t fraction = src_byte_length % 4;
    uint32_t length   = src_byte_length / 4;

    if (fraction)
        length++;
    return burst_read_chip_sram(starting_address, length, dst_ptr, phy_info_ptr->phy_id);
}
#endif

/**
 * @brief      intf_read_memory(capi_phy_info_t* phy_info_ptr, uint32_t* dst_ptr, uint32_t* src_ptr, uint16_t src_byte_length)
 * @details    Read data from given memory address
 *
 * @param      phy_info_ptr     : pointer to phy info
 * @param      dst_ptr          : destination buffer
 * @param      src_ptr          : memory address
 * @param      src_byte_length  : length
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t intf_read_memory(capi_phy_info_t* phy_info_ptr, uint32_t* dst_ptr, uint32_t* src_ptr, uint16_t src_byte_length)
{
#ifdef BURST_READ
    return intf_util_burst_read(phy_info_ptr, dst_ptr, (uint32_t)src_ptr, src_byte_length);
#else
    return intf_util_read_sram(phy_info_ptr, dst_ptr, src_ptr, src_byte_length);
#endif
}

/**
 * @brief      intf_util_read_cmd_rsp_sram(capi_phy_info_t* phy_info_ptr,
 *                                         uint8_t*  payload_ptr,
 *                                         uint16_t  payload_length,
 *                                         uint16_t* cmd_tocken_ptr)
 *
 * @details    This utility is to retrieve the response payload from the FW via SRAM
 *
 * @param[in]  payload_ptr: reference to the command payload
 * @param[in]  payload_length: payload length
 * @param[in]  cmd_tocken_ptr: reference to the commmand tocken
 * 
 * @return     returns the result of the called methode/function
 */
static return_result_t intf_util_read_cmd_rsp_sram(capi_phy_info_t* phy_info_ptr,
                                                   uint8_t*  payload_ptr,
                                                   uint16_t  payload_length,
                                                   uint16_t* cmd_tocken_ptr)
{
    uint32_t* src_ptr = (uint32_t*)INTF_FW2CAPI_CMD_RSP_DATA_PAYLOAD_ADDRESS(phy_info_ptr);
    intf_capi2fw_cmd_payload_info_t payload_info;
    uint32_t save_base_addr_ptr = (uint32_t)phy_info_ptr->base_addr;

    phy_info_ptr->base_addr = 0x0;
    ERR_HSIP(payload_info.content = (uint32_t)hsip_rd_reg_(phy_info_ptr, ((long)src_ptr++)));

    if (payload_info.fields.cmd_tocken != *cmd_tocken_ptr) {
        phy_info_ptr->base_addr = save_base_addr_ptr;
        return(RR_ERROR_IPC_HANDSHAKE_FAILED);
    }

    intf_util_read_sram(phy_info_ptr, (uint32_t*)payload_ptr, src_ptr, payload_info.fields.cmd_length);
    phy_info_ptr->base_addr = save_base_addr_ptr;
    (void)payload_length;
    return(RR_SUCCESS);
}

/**
 * @brief      intf_util_write_sram(capi_phy_info_t* phy_info_ptr,
 *                                  uint32_t*        dst_ptr,
 *                                  uint32_t*        src_ptr,
 *                                  uint16_t         src_byte_length)
 *
 * @details    This utility is to send the command payload to the FW via SRAM
 *
 * @param[in]  dst_ptr: destination address in SRAM
 * @param[in]  src_ptr: source address in SRAM
 * @param[in]  src_byte_length: source length in byte/s
 * 
 * @return     returns the result of the called methode/function
 */
static return_result_t intf_util_write_sram(capi_phy_info_t* phy_info_ptr,
                                 uint32_t*        dst_ptr,
                                 uint32_t*        src_ptr,
                                 uint16_t         src_byte_length)
{
    uint8_t  data_index;
    uint8_t  length     = src_byte_length / 4;
    uint8_t  fraction   = src_byte_length % 4;
    uint32_t last_data  = 0;

    if (length) {
#ifdef IPC_BURST
        burst_write_chip_sram((uint32_t)dst_ptr, length, src_ptr, phy_info_ptr->phy_id);
        dst_ptr += length;
        src_ptr += length;

        src_ptr--;
#else
        for (data_index = 0; data_index < length; data_index++) {
            hsip_wr_reg_(phy_info_ptr, ((long)dst_ptr++), (*src_ptr++));
        }

        src_ptr--;
#endif
    }

    if (fraction) {
        if (length) {
            switch(fraction) {
                case 1:
                    last_data = *src_ptr & 0xFF000000;
                    break;
                case 2:
                    last_data = *src_ptr & 0xFFFF0000;
                    break;
                case 3:
                    last_data = *src_ptr & 0xFFFFFF00;
                    break;
                default:
                    break;
            }
        } else {
            last_data = *src_ptr;
        }

        hsip_wr_reg_(phy_info_ptr, (long)dst_ptr, last_data);
    }

    return RR_SUCCESS;
}


/**
 * @brief      intf_util_write_cmd_req_payload_sram(capi_phy_info_t* phy_info_ptr,
 *                                                  uint8_t*         payload_ptr,
 *                                                  uint16_t         payload_length,
 *                                                  uint16_t*        cmd_tocken_ptr)
 *
 * @details    This utility is to send the command payload to the FW via SRAM
 *
 * @param[in]  payload_ptr: Payload reference pointer
 * @param[in]  payload_length: Payload length
 * @param[in]  cmd_tocken_ptr: Reference to the tocken
 * 
 * @return     returns the result of the called methode/function
 */
static return_result_t intf_util_write_cmd_req_payload_sram(capi_phy_info_t* phy_info_ptr,
                                                 uint8_t*         payload_ptr,
                                                 uint16_t         payload_length,
                                                 uint16_t*        tocken_ptr)
{
    uint32_t* base_addr          = (uint32_t*)INTF_CAPI2FW_CMD_REQ_DATA_PAYLOAD_ADDRESS(phy_info_ptr);
    uint32_t save_base_addr_ptr = (uint32_t)phy_info_ptr->base_addr;
    intf_capi2fw_cmd_payload_info_t payload_info;

    phy_info_ptr->base_addr        = 0x00;
    payload_info.fields.cmd_tocken = *tocken_ptr;
    payload_info.fields.cmd_length = payload_length;

    hsip_wr_reg_(phy_info_ptr, ((long)base_addr++), payload_info.content);
    intf_util_write_sram(phy_info_ptr, base_addr, (uint32_t*)payload_ptr, payload_length);

    phy_info_ptr->base_addr = save_base_addr_ptr;

    return RR_SUCCESS;
}

/**
 * @brief      intf_capi2fw_command_response(capi_phy_info_t* phy_info_ptr,
 *                                           uint8_t*         payload_ptr,
 *                                           uint16_t         payload_length,
 *                                           uint16_t*        cmd_tocken_ptr)
 *
 * @details    This API is used to the command response from the FW via SRAM
 *
 * @param[in]      phy_info_ptr: a pointer to the the Phy Info object
 * @param[in/out]  payload_ptr: Payload reference pointer
 * @param[in]      payload_length: Payload length
 * @param[in]      cmd_tocken_ptr: Reference to the tocken
 * 
 * @return     returns the result of the called methode/function
 */
static return_result_t intf_capi2fw_command_response(capi_phy_info_t* phy_info_ptr,
                                                     uint8_t*         payload_ptr,
                                                     uint16_t         payload_length,
                                                     uint16_t*        cmd_tocken_ptr)
{
    uint16_t        timeout;
    uint16_t        rsp_lane_mask;
    uint16_t        cmd_lane_mask;
    uint32_t        cmd_response_address = 0;
    uint32_t        rsp_lane_mask_address = 0;
    return_result_t return_result = RR_ERROR_IPC_HANDSHAKE_FAILED;
    intf_fw2capi_command_response_gpreg_t cmd_response;

    phy_info_ptr->base_addr = OCTAL_TOP_REGS;
    cmd_lane_mask           = phy_info_ptr->lane_mask >> 16;

    switch(phy_info_ptr->core_ip) {
        case CORE_IP_HOST_DSP:
        case CORE_IP_HOST_SERDES:
            cmd_lane_mask         = (uint16_t)(phy_info_ptr->lane_mask >> 16);
            cmd_response_address  = INTF_CAPI2FW_HOST_CMD_RESPONSE_GPREG;
            rsp_lane_mask_address = INTF_CAPI2FW_HOST_RSP_LANE_MASK_GPREG;
            break;

        case CORE_IP_MEDIA_DSP:
        case CORE_IP_MEDIA_SERDES:
            cmd_lane_mask         = (uint16_t)(phy_info_ptr->lane_mask & 0xFF);
            cmd_response_address  = INTF_CAPI2FW_MEDIA_CMD_RESPONSE_GPREG;
            rsp_lane_mask_address = INTF_CAPI2FW_MEDIA_RSP_LANE_MASK_GPREG;
            break;

        case CORE_IP_CW:
            cmd_lane_mask = phy_info_ptr->lane_mask & LW_LANE_MASK;
            /* recover bh mask if lw mask is not valid */
            if (!cmd_lane_mask) {
                cmd_lane_mask = (phy_info_ptr->lane_mask >> 16) & CLIENT_LANE_MASK;
            }
            cmd_response_address  = INTF_CAPI2FW_CW_CMD_RESPONSE_GPREG;
            rsp_lane_mask_address = INTF_CAPI2FW_CW_RSP_LANE_MASK_GPREG;
            break;

        case CORE_IP_ALL:
            break;

        default:
            return_result = RR_ERROR_WRONG_INPUT_VALUE;
            break;
    }

    for (timeout = 0; timeout < IPC_COMMAND_TIMEOUT_MS; timeout++) {
        delay_ms(1);
        ERR_HSIP(cmd_response.words = (uint16_t)hsip_rd_reg_(phy_info_ptr, cmd_response_address));

        /* Check whether the FW has processed the command */
        if (cmd_response.fields.cmd_response) {

            return_result = (return_result_t)cmd_response.fields.return_result;

            if (return_result != RR_SUCCESS) {
                break;
            }

            /* This is optional since the tocken verification is already completed */
            ERR_HSIP(rsp_lane_mask = (uint16_t) hsip_rd_reg_(phy_info_ptr, rsp_lane_mask_address));
            if (rsp_lane_mask & 0x8000) cmd_lane_mask |= 0x8000;
            if (cmd_lane_mask != rsp_lane_mask) {
                return_result = RR_ERROR_IPC_HANDSHAKE_FAILED;
                break;
            }

            if (payload_ptr) {
                return_result = intf_util_read_cmd_rsp_sram(phy_info_ptr, payload_ptr, payload_length, cmd_tocken_ptr);
            }

            break;
        }
    }

    hsip_wr_reg_(phy_info_ptr, INTF_CAPI2FW_HOST_CMD_REQUEST_GPREG,  0x00);
    hsip_wr_reg_(phy_info_ptr, INTF_CAPI2FW_MEDIA_CMD_REQUEST_GPREG,  0x00);
    hsip_wr_reg_(phy_info_ptr, INTF_CAPI2FW_CW_CMD_REQUEST_GPREG,  0x00);
    hsip_wr_reg_(phy_info_ptr, cmd_response_address, 0x00);
    hsip_wr_reg_(phy_info_ptr, rsp_lane_mask_address, 0x00);

    if (timeout >= IPC_COMMAND_TIMEOUT_MS) {
        return_result = RR_ERROR_IPC_TIME_OUT;
    }

    return(return_result);
}


/**
 * @brief      intf_capi2fw_command_request(capi_phy_info_t* phy_info_ptr,
 *                                          uint8_t          command_id,
 *                                          uint8_t*         payload_ptr,
 *                                          uint16_t         payload_length,
 *                                          uint16_t*        cmd_tocken_ptr)
 *
 * @details    This API is used to send CAPI command to the firmware via SRAM
 *
 * @param[in]      phy_info_ptr: a pointer to the the Phy Info object
 * @param[in]      command_id: Command ID
 * @param[in/out]  payload_ptr: Payload reference pointer
 * @param[in]      payload_length: Payload length
 * @param[in]      cmd_tocken_ptr: reference to the tocken
 * 
 * @return     returns the result of the called methode/function
 */
static return_result_t intf_capi2fw_command_request(capi_phy_info_t* phy_info_ptr,
                                                    uint8_t          command_id,
                                                    uint8_t*         payload_ptr,
                                                    uint16_t         payload_length,
                                                    uint16_t*        cmd_tocken_ptr)
{
    uint16_t                             lane_mask;
    return_result_t                      return_result = RR_ERROR_WRONG_INPUT_VALUE;
    intf_capi2fw_command_request_gpreg_t cmd_request;

    cmd_request.words = 0;
    phy_info_ptr->base_addr = OCTAL_TOP_REGS;

    if ((phy_info_ptr->core_ip == CORE_IP_HOST_DSP) ||
        (phy_info_ptr->core_ip == CORE_IP_HOST_SERDES) ||
        (phy_info_ptr->core_ip == CORE_IP_ALL)) {
        ERR_HSIP(cmd_request.words = (uint16_t)hsip_rd_reg_(phy_info_ptr, INTF_CAPI2FW_HOST_CMD_REQUEST_GPREG));

        /*Check whether the FW is currently processing the previous command*/
        if (cmd_request.fields.cmd_request) {
            return(RR_WARNING_BUSY_TRY_LATER);
        }

        lane_mask = (phy_info_ptr->lane_mask >> 16) & CLIENT_LANE_MASK;
    
        if (lane_mask) {
            intf_util_assign_tocken(cmd_tocken_ptr);

            if (payload_ptr) {
                intf_util_write_cmd_req_payload_sram(phy_info_ptr, payload_ptr, payload_length, cmd_tocken_ptr);
            }

            cmd_request.fields.command_id  = command_id;
            /*cmd_request.fields.cmd_tocken  = (*cmd_tocken_ptr%0x7F);*/
            cmd_request.fields.cmd_request = 1;          

            hsip_wr_reg_(phy_info_ptr, INTF_CAPI2FW_HOST_CMD_LANE_MASK_GPREG, lane_mask);
            hsip_wr_reg_(phy_info_ptr, INTF_CAPI2FW_HOST_CMD_REQUEST_GPREG,   cmd_request.words);
        } else {
            return(RR_ERROR_WRONG_INPUT_VALUE);
        }
        return_result = RR_SUCCESS;
    }

    if ((phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP) ||
        (phy_info_ptr->core_ip == CORE_IP_MEDIA_SERDES) ||
        (phy_info_ptr->core_ip == CORE_IP_ALL)) {
        ERR_HSIP(cmd_request.words = (uint16_t)hsip_rd_reg_(phy_info_ptr, INTF_CAPI2FW_MEDIA_CMD_REQUEST_GPREG));

        /*Check whether the FW is currently processing the previous command*/
        if (cmd_request.fields.cmd_request) {
            return(RR_WARNING_BUSY_TRY_LATER);
        }

        lane_mask = phy_info_ptr->lane_mask & LW_LANE_MASK;
    
        if (lane_mask) {
            intf_util_assign_tocken(cmd_tocken_ptr);

            if (payload_ptr) {
                /* To make sure not to overwrite it! */
                if (phy_info_ptr->core_ip != CORE_IP_ALL) {
                    intf_util_write_cmd_req_payload_sram(phy_info_ptr, payload_ptr, payload_length, cmd_tocken_ptr);
                }
            }

            cmd_request.fields.command_id  = command_id;
            /*cmd_request.fields.cmd_tocken  = (*cmd_tocken_ptr % 0x7F);*/
            cmd_request.fields.cmd_request = 1;
            hsip_wr_reg_(phy_info_ptr, INTF_CAPI2FW_MEDIA_CMD_LANE_MASK_GPREG,   lane_mask);
            hsip_wr_reg_(phy_info_ptr, INTF_CAPI2FW_MEDIA_CMD_REQUEST_GPREG, cmd_request.words);
        } else {
            return(RR_ERROR_WRONG_INPUT_VALUE);
        }
        return_result = RR_SUCCESS;
    }

    if ((phy_info_ptr->core_ip == CORE_IP_CW) || (phy_info_ptr->core_ip == CORE_IP_ALL)) {
        ERR_HSIP(cmd_request.words = (uint16_t)hsip_rd_reg_(phy_info_ptr, INTF_CAPI2FW_CW_CMD_REQUEST_GPREG));

        /*Check whether the FW is currently processing the previous command*/
        if (cmd_request.fields.cmd_request) {
            return(RR_WARNING_BUSY_TRY_LATER);
        }
    
        if (phy_info_ptr->lane_mask) {
            intf_util_assign_tocken(cmd_tocken_ptr);

            if (payload_ptr) {
                /* To make sure not to overwrite it! */
                if (phy_info_ptr->core_ip != CORE_IP_ALL) {
                    intf_util_write_cmd_req_payload_sram(phy_info_ptr, payload_ptr, payload_length, cmd_tocken_ptr);
                }
            }

            lane_mask = phy_info_ptr->lane_mask & LW_LANE_MASK;

            if (!lane_mask) {
                lane_mask = (phy_info_ptr->lane_mask >> 16) & CLIENT_LANE_MASK;
            } else {
                lane_mask |= 0x8000; /* To identify that this is LW lane mask */
            }

            cmd_request.fields.command_id = command_id;
            /*cmd_request.fields.cmd_tocken  = (*cmd_tocken_ptr % 0x7F);*/
            cmd_request.fields.cmd_request = 1;
            hsip_wr_reg_(phy_info_ptr, INTF_CAPI2FW_CW_CMD_LANE_MASK_GPREG, lane_mask);
            hsip_wr_reg_(phy_info_ptr, INTF_CAPI2FW_CW_CMD_REQUEST_GPREG,   cmd_request.words);
        } else {
            return(RR_ERROR_WRONG_INPUT_VALUE);
        }
        return_result = RR_SUCCESS;
    }

    return(return_result);
}

/**
 * @brief      intf_capi2fw_command_Handler(capi_phy_info_t* phy_info_ptr,
 *                                          uint8_t          command_id,
 *                                          uint8_t*         payload_ptr,
 *                                          uint16_t         payload_length,
 *                                          uint8_t          config_cmd)
 *
 * @details    This API is used to send CAPI command to firmware and retrieve the response
 *
 * @param[in]      phy_info_ptr: a pointer to the the Phy Info object
 * @param[in]      command_id: Command ID
 * @param[in/out]  payload_ptr: Payload reference pointer
 * @param[in]      payload_length: Payload length
 * @param[in]      config_cmd: configuration command type (get/set)
 * 
 * @return     returns the result of the called methode/function
 */
return_result_t intf_capi2fw_command_Handler(capi_phy_info_t* phy_info_ptr,
                                             uint8_t          command_id,
                                             uint8_t*         payload_ptr,
                                             uint16_t         payload_length,
                                             uint8_t          config_cmd)
{
    uint16_t        cmd_tok = 0;
    return_result_t return_result;
    uint32_t        lane_mask = phy_info_ptr->lane_mask;
	uint16_t        req_payload_len = payload_length;

    if ((phy_info_ptr->core_ip == CORE_IP_HOST_DSP) ||
        (phy_info_ptr->core_ip == CORE_IP_HOST_SERDES)) {
        phy_info_ptr->lane_mask = phy_info_ptr->lane_mask << 16;
    }

#ifdef IPC_PAYLOAD_OPTIMIZATION
    intf_util_get_optimized_req_payload_size(command_id, payload_ptr, &req_payload_len);
#endif

    return_result = intf_capi2fw_command_request(phy_info_ptr,
                                                 command_id,
                                                 payload_ptr,
                                                 req_payload_len,
                                                 &cmd_tok);

    if (return_result != RR_SUCCESS) {
        CAPI_LOG_ERROR("intf_capi2fw_command_request(Command Id: %u   Return Result: %u)\n", command_id, return_result);
        return(return_result);
    }

    /* wait for command is accepted and get lane status for single lane command */
    return_result = intf_capi2fw_command_response(phy_info_ptr,
                                                  config_cmd ? payload_ptr : (uint8_t*)NULL,
                                                  payload_length,
                                                  &cmd_tok);


    if (return_result != RR_SUCCESS) {
        CAPI_LOG_ERROR("intf_capi2fw_command_response(Command Id: %u   Return Result: %u)\n",
                        command_id, return_result);
    }

    phy_info_ptr->lane_mask = lane_mask;

    return(return_result);
}



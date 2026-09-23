/**
 *
 * @file     host_to_chip_ipc.h
 * @author   Firmware Team
 * @date     01/03/2019
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
#ifndef HOST_TO_CHIP_IPC_H
#define HOST_TO_CHIP_IPC_H

#ifdef __cplusplus
extern "C" {
#endif


uint8_t util_get_max_lanes(phy_info_t* phy_info_ptr);

/**
 * @brief      intf_send_fw_command(capi_phy_info_t* phy_info_ptr,
 *                                  uint16_t         bh_lane_mask,
 *                                  uint16_t         lw_lane_mask,
 *                                  uint16_t         command_id)
 *
 * @details    This API is used to send CAPI command to the firmware
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  bh_lane_mask: client lane mask
 * @param[in]  lw_lane_mask: LW lane mask
 * @param[in]  command_id: command ID
 * 
 * @return     returns the result of the called methode/function
 */
return_result_t intf_send_fw_command(capi_phy_info_t* phy_info_ptr,
                                     uint16_t         bh_lane_mask,
                                     uint16_t         lw_lane_mask,
                                     uint16_t         command_id);

/**
 * @brief      intf_send_fw_lane_command(capi_phy_info_t*          phy_info_ptr,
 *                                       uint8_t                   lane_id,
 *                                       uint16_t                  command_id,
 *                                       common_lane_status_reg_t* lane_status_ptr)
 * @details    This API is used to send CAPI lane command to firmware and get lane status
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  lane_id: lane ID
 * @param[in]  lw_lane_mask: LW lane mask
 * @param[in]  command_id: command ID
 * @param[out] lane_status: lane status output
 * 
 * @return     returns the result of the called methode/function
 */
return_result_t intf_send_fw_lane_command(capi_phy_info_t*          phy_info_ptr,
                                          uint8_t                   lane_id,
                                          uint16_t                  command_id,
                                          common_lane_status_reg_t* lane_status_ptr);

/**
 * @brief      intf_capi2fw_command_Handler(capi_phy_info_t* phy_info_ptr,
 *                                          uint8_t          command_id,
 *                                          uint8_t*         payload_ptr,
 *                                          uint16_t         payload_length,
 *                                          uint8_t          config_cmd)
 *
 * @details    This API is used to send CAPI lane command to firmware and get lane status
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
                                             uint8_t          config_cmd);

/**
 * @brief      intf_read_memory(capi_phy_info_t* phy_info_ptr,uint32_t* dst_ptr, uint32_t* src_ptr, uint16_t src_byte_length)
 * @details    Read data from given memory address
 *
 * @param      phy_info_ptr     : phy_info pointer
 * @param      dst_ptr          : destination buffer pointer
 * @param      src_ptr          : SRAM location
 * @param      src_byte_length  : length to be copied
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t intf_read_memory(capi_phy_info_t* phy_info_ptr,
                                 uint32_t*        dst_ptr,
                                 uint32_t*        src_ptr,
                                 uint16_t         src_byte_length);

#ifdef __cplusplus
}
#endif

#endif //HOST_TO_CHIP_IPC_H

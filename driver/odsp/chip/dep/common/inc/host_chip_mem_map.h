/****************************************************************************
*
*     Copyright (c) 2021 Broadcom Limited
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom Limited 
*           1320 Ridder Park Dr.
*           San Jose, California 95131
*           United States
*
*     All information contained in this document is Broadcom Limited 
*     company private, proprietary, and trade secret.
*
****************************************************************************/

/**
  *        Host Chip Memory Map Header File
  * @file   host_chip_mem_map.h
  * @author HSIP FW team
  * @date 10-18-2021
  * @brief
  */

#ifndef HOST_CHIP_MEM_MAP_H
#define HOST_CHIP_MEM_MAP_H

#ifdef __cplusplus
extern "C" {
#endif

#define CHIP_MEMMAP_CAPI2FW_IPC_MEM_SIZE                    1024            /**< CHIP memmory map CAPI/FW IPC memory size                                 */

/**
 * A structure represents CAPI/FW IPC memory
 */
typedef struct capi_fw_ipc_buf_s {
    uint8_t capi_fw_ipc_req[CHIP_MEMMAP_CAPI2FW_IPC_MEM_SIZE];              /**< 1K space allocated in the SRAM for CAPI2FW data payload    */
    uint8_t capi_fw_ipc_resp[CHIP_MEMMAP_CAPI2FW_IPC_MEM_SIZE];             /**< 1K space allocated in the SRAM for FW2CAPI data payload    */
} capi_fw_ipc_buf_t;        /**< CAPI/FW IPC memory info */

/**
 * A structure represents SPI programming info
 */
typedef struct spi_program_info_s {
    uint32_t spi_program_command;               /**< SPI programming command            */
    uint32_t spi_program_buffer_1_location;     /**< SPI programming buffer 1 location  */
    uint32_t spi_program_buffer_2_location;     /**< SPI programming buffer 2 location  */
    uint32_t spi_program_block_size;            /**< SPI programming block size         */
    uint32_t spi_program_buffer_1_status;       /**< SPI programming buffer 1 status    */
    uint32_t spi_program_buffer_2_status;       /**< SPI programming buffer 2 status    */
    uint32_t reserved[2];                       /**< Resered for internal use only      */
} spi_program_info_t;       /**< SPI programming info */

/**
 * A structure represents host chip memomry map info
 */
typedef struct host_chip_memmap_info_s {
    capi_fw_ipc_buf_t    capi_fw_ipc_buf;                                 /**< CAPI-FW IPC buf, 2048b            */
    spi_program_info_t   spi_program_info;                                /**< SPI program info, 32b             */
} host_chip_memmap_info_t;                                                /**< Host Chip memory map info          */

extern host_chip_memmap_info_t *host_chip_memmap_get_data_ptr(phy_info_t *phy_info_ptr);

/**
 * Chip memory map macro names used to access individual memory mapped data
 */
#define CHIP_MEMMAP_CAPI2FW_IPC_REQ_DATA(phy_info_ptr)               (uintptr_t)&(host_chip_memmap_get_data_ptr(phy_info_ptr)->capi_fw_ipc_buf.capi_fw_ipc_req)
#define CHIP_MEMMAP_CAPI2FW_IPC_RSP_DATA(phy_info_ptr)               (uintptr_t)&(host_chip_memmap_get_data_ptr(phy_info_ptr)->capi_fw_ipc_buf.capi_fw_ipc_resp)

#define CHIP_MEMMAP_SPI_PROGRAM_COMMAND_REG(phy_info_ptr)             (uintptr_t)&(host_chip_memmap_get_data_ptr(phy_info_ptr)->spi_program_info.spi_program_command)
#define CHIP_MEMMAP_SPI_PROGRAM_BUFFER_1_LOCATION_REG(phy_info_ptr)   (uintptr_t)&(host_chip_memmap_get_data_ptr(phy_info_ptr)->spi_program_info.spi_program_buffer_1_location)
#define CHIP_MEMMAP_SPI_PROGRAM_BUFFER_2_LOCATION_REG(phy_info_ptr)   (uintptr_t)&(host_chip_memmap_get_data_ptr(phy_info_ptr)->spi_program_info.spi_program_buffer_2_location)
#define CHIP_MEMMAP_SPI_PROGRAM_BLOCK_SIZE_REG(phy_info_ptr)          (uintptr_t)&(host_chip_memmap_get_data_ptr(phy_info_ptr)->spi_program_info.spi_program_block_size)
#define CHIP_MEMMAP_SPI_PROGRAM_BUFFER_1_STATUS_REG(phy_info_ptr)     (uintptr_t)&(host_chip_memmap_get_data_ptr(phy_info_ptr)->spi_program_info.spi_program_buffer_1_status)
#define CHIP_MEMMAP_SPI_PROGRAM_BUFFER_2_STATUS_REG(phy_info_ptr)     (uintptr_t)&(host_chip_memmap_get_data_ptr(phy_info_ptr)->spi_program_info.spi_program_buffer_2_status)
#define CHIP_MEMMAP_SPI_PROGRAM_RESERVED_0_REG(phy_info_ptr)          (uintptr_t)&(host_chip_memmap_get_data_ptr(phy_info_ptr)->spi_program_info.reserved[0])
#define CHIP_MEMMAP_SPI_PROGRAM_RESERVED_1_REG(phy_info_ptr)          (uintptr_t)&(host_chip_memmap_get_data_ptr(phy_info_ptr)->spi_program_info.reserved[1])

#define FW_UPGRADE_START_ADDR                          0x050000
#define FW_UPGRADE_START_ADDR_B                        0x007000
#define FW_UPGRADE_SECTOR_SIZE                         0x1000
#define CHIP_TOP_SPI_PROGRAM_FW_UPGRADE_CRC_REG        0x5fccc
#define CHIP_TOP_SPI_BLOCK_SIZE                        0x200
#define CHIP_TOP_SPI_BUFFER1_LOCATION                  0x5f800
#define CHIP_TOP_SPI_BUFFER2_LOCATION                  (CHIP_TOP_SPI_BUFFER1_LOCATION+CHIP_TOP_SPI_BLOCK_SIZE)

#define CHIP_TOP_SPI_PROGRAM_COMMAND_REG               CHIP_MEMMAP_SPI_PROGRAM_COMMAND_REG
#define CHIP_TOP_SPI_PROGRAM_BUFFER_1_LOCATION_REG     CHIP_MEMMAP_SPI_PROGRAM_BUFFER_1_LOCATION_REG
#define CHIP_TOP_SPI_PROGRAM_BUFFER_2_LOCATION_REG     CHIP_MEMMAP_SPI_PROGRAM_BUFFER_2_LOCATION_REG
#define CHIP_TOP_SPI_PROGRAM_BLOCK_SIZE_REG            CHIP_MEMMAP_SPI_PROGRAM_BLOCK_SIZE_REG
#define CHIP_TOP_SPI_PROGRAM_BUFFER_1_STATUS_REG       CHIP_MEMMAP_SPI_PROGRAM_BUFFER_1_STATUS_REG
#define CHIP_TOP_SPI_PROGRAM_BUFFER_2_STATUS_REG       CHIP_MEMMAP_SPI_PROGRAM_BUFFER_2_STATUS_REG
#define CHIP_TOP_SPI_PROGRAM_AUTH_REG                  CHIP_MEMMAP_SPI_PROGRAM_RESERVED_0_REG

#define INTF_CAPI2FW_CMD_REQ_DATA_PAYLOAD_ADDRESS      CHIP_MEMMAP_CAPI2FW_IPC_REQ_DATA   /**< 1K space allocated in the SRAM for CAPI2FW data payload    */
#define INTF_FW2CAPI_CMD_RSP_DATA_PAYLOAD_ADDRESS      CHIP_MEMMAP_CAPI2FW_IPC_RSP_DATA   /**< 1K space allocated in the SRAM for FW2CAPI data payload    */

#ifdef __cplusplus
}
#endif

#endif  /* HOST_CHIP_MEM_MAP_H */



/**
 *
 * @file host_download_util.h
 * @author
 * @date     12/11/2017
 * @version 1.0
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
#ifndef HOST_DOWNLOAD_UTIL_H
#define HOST_DOWNLOAD_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#define SPIF_INDIRECT_BASE                 0x21000000
#define CONFIG_DATA_FLASH_LOCATION         0x70000
#define SPI_BLOCK_SIZE                     64
#define IMAGE_DOWNLOAD_BASE                0x00000000
#define I2C_BLOCK_WRITE_SIZE               0x2000
#define UPGRADE_AUTHENTIC_REG              0xD000
#define IMAGE_DOWNLOAD_EACH_BLOCK_SIZE    (64*1024)

#define SPIF_WFIFO_BASE               SPIF_INDIRECT_BASE
#define SPIF_RFIFO_BASE               SPIF_INDIRECT_BASE

#define SPI_RDID                      0x9F
#define SPI_RDSR                      0x05
#define SPI_WRSR                      0x01
#define SPI_READ                      0x03
#define SPI_WRITE                     0x02
#define SPI_WREN                      0x06
#define SPI_WRDI                      0x04
#define SPI_CE                        0xC7
#define SPI_BE                        0xD8
#define SPI_SE                        0x20
#define SPI_RDID_LEN                  0x03

#define QSPI_STOP                     0x0A
#define QSPI_START                    0x0B
#define QSPI_READ                     0xEB
#define QSPI_WRITE                    0x38

#ifdef MODE_QSPI
#define STOP_CMD                      QSPI_STOP
#define START_CMD                     QSPI_START
#define READ_CMD                      QSPI_READ
#else
#define STOP_CMD                      SPI_STOP
#define START_CMD                     SPI_START
#define READ_CMD                      SPI_READ
#define WRITE_CMD                     SPI_WRITE
#endif


/**
 * The structure contains flash device info
 */
typedef struct spi_flash_device_info_s {
    uint32_t jid;          /**< JEDEC ID                   */
    uint32_t capacity;     /**< in KB                      */
    uint8_t  name[32];     /**< Device name                */
} spi_flash_device_info_t; /**< SPI flash device info type */


/**
 * @brief      host_spi_eeprom_ready(capi_phy_info_t* phy_info_ptr)
 * @details    This API returns whether the spi device is ready
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr
 *
 * @return     returns the result of the called method/function
 */
return_result_t host_spi_eeprom_ready(capi_phy_info_t* phy_info_ptr);

/**
 * @brief      host_spi_flash_erase(capi_phy_info_t* phy_info_ptr)
 * @details    This API erases the flash device
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr
 *
 * @return     returns the result of the called method/function
 */
return_result_t host_spi_flash_erase(capi_phy_info_t* phy_info_ptr);

/**
 * @brief      host_program_config_to_spi(capi_phy_info_t* phy_info_ptr,
 *                                        uint32_t*        config_data_ptr,
 *                                        uint32_t         size)
 * @details    This API programs user configuration data to flash device
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr
 * @param      a pointer to the configuration data
 * @param      size of data
 *
 * @return     returns the result of the called method/function
 */
return_result_t  host_program_config_to_spi(capi_phy_info_t* phy_info_ptr,
                                                    uint32_t*        config_data_ptr,
                                                    uint32_t         size);

/**
* @brief    host_download_sram(capi_phy_info_t*     phy_info_ptr,
*                              const uint32_t*      wholeimage_sram_ptr,
*                              uint32_t             size,
*                              uint32_t             offset,
*                              phy_static_config_t* phy_static_config_ptr);
*
* @details  This API is used to download firmware to sram
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  wholeimage_sram_ptr: a pointer to the fw image
* @param[in]  size: size of the fw image
* @param[in]  offset: offset of the image
* @param[in]  phy_static_config_ptr: a pointer to phy configuration
*
* @return     returns the result of the called method/function, either RR_ERROR or RR_SUCCESS
*/
return_result_t host_download_sram(capi_phy_info_t*     phy_info_ptr,
                                    const uint32_t*      wholeimage_sram_ptr,
                                    uint32_t             size,                                   
                                    phy_static_config_t* phy_static_config_ptr);


/**
* @brief    return_result_t host_download_spi_internal(capi_phy_info_t* phy_info_ptr,
                                           capi_download_info_t* download_info_ptr)    
* @details  This API is used to download firmware to spi device
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  download_info_ptr: a pointer to  the capi_download_info_t
*
* @return     returns the result of the called method/function, either RR_ERROR or RR_SUCCESS
*/
return_result_t host_download_spi_internal(capi_phy_info_t* phy_info_ptr,  capi_download_info_t* download_info_ptr);
                                         

/**
 * @brief    host_download_mem_reset(capi_phy_info_t* phy_info_ptr) 
 * @details  
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * 
 * @return     returns the result of the called method/function
 */
return_result_t host_download_mem_reset(capi_phy_info_t* phy_info_ptr);

/**
 * @brief      host_pre_sram_download(capi_phy_info_t* phy_info_ptr)
 * @details    operations before SRAM download 
 *
 * @property   None
 * @public     None
 * @private    None
 * @example    None
 *
 * @param      phy_info_ptr
 *
 * @return     returns the result of the called methode/function
 */
return_result_t host_pre_sram_download(capi_phy_info_t* phy_info_ptr);

/**
* @brief    return_result_t host_download_spi_internal_fast(capi_phy_info_t* phy_info_ptr,
                                           capi_download_info_t* download_info_ptr)
* @details  This API is used to download firmware to spi device
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  download_info_ptr: a pointer to  the capi_download_info_t
*
* @return     returns the result of the called method/function, either RR_ERROR or RR_SUCCESS
*/
return_result_t host_download_spi_internal_fast(capi_phy_info_t* phy_info_ptr,  capi_download_info_t* download_info_ptr);
return_result_t host_download_sram_fast(capi_phy_info_t*     phy_info_ptr,
                                 capi_download_info_t* download_info_ptr);

/**
 * @brief      host_flash_hitless_fw_upgrade(capi_phy_info_t* phy_info_ptr, capi_fw_download_info_t *fw_download_info_ptr)
 * @details
 *
 * @param[in]      phy_info_ptr: pointer to the phy
 * @param[in]      fw_download_info_ptr: pointer to download info
 *
 * @return     returns the result of the called method/function
 */
return_result_t host_flash_hitless_fw_upgrade(capi_phy_info_t* phy_info_ptr, capi_fw_download_info_t *fw_download_info_ptr);

/**
 * @brief      host_override_upgrade_status(capi_phy_info_t* phy_info_ptr, firmware_image_status_t *fw_image_status_ptr)
 * @details
 *
 * @param[in]      phy_info_ptr: pointer to the phy
 * @param[in]      fw_image_status_ptr: pointer to firmware image status
 *
 * @return     returns the result of the called method/function
 */
return_result_t host_override_upgrade_status(capi_phy_info_t* phy_info_ptr, firmware_image_status_t *fw_image_status_ptr);

/**
 * @brief       host_fw_image_status(capi_phy_info_t* phy_info_ptr, firmware_image_status_t* fw_image_status_ptr)
 * @details
 *
 * @param[in]   phy_info_ptr: a reference the the phy information object
 * @param[in]   fw_image_status_ptr: a reference to the firmware image status object
 *
 * @return      returns the result of the called method/function
 */
return_result_t host_fw_image_status(capi_phy_info_t* phy_info_ptr, firmware_image_status_t* fw_image_status_ptr);


#ifdef __cplusplus
}
#endif

#endif  /* HOST_DOWNLOAD_UTIL_H */

/**
 *
 * @file    host_download_util.c
 * @author  Firmware Team
 * @date    12/11/2017
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
#include "hr_time.h"
#include "access.h"
#include "regs_common.h"
#include "chip_config_def.h"
#include "chip_mode_def.h"
#include "capi_def.h"
#include "capi_test_def.h"
#include "host_log_util.h"
#include "common_util.h"
#include "host_download_util.h"
#include "capi.h"
#ifdef CAPI_CONFIG_FAST_SPI_PROGRAM_WITH_BLOCK_WRITE
#include "phy_spi_program_fast.h"
#endif
#include "host_chip_mem_map.h"

extern void wr8_reg_ex(uint32_t addr, uint32_t data, uint32_t phy_id);
extern uint32_t rd8_reg_ex(uint32_t addr, uint32_t phy_id);
extern bool burst_write_i2c(uint32_t starting_address, uint32_t size, uint32_t* data);
extern bool burst_write_mdio(uint32_t starting_address, uint32_t size, uint32_t* data, int prtad);
return_result_t read_status(capi_phy_info_t* phy_info_ptr, uint8_t command_id, uint8_t* status);
bool burst_write(capi_phy_info_t* phy_info_ptr, capi_download_info_t* download_info_ptr,  uint32_t starting_address, uint32_t size, uint32_t* data);

/* basic info of the flash devices */
static spi_flash_device_info_t hsip_flash_devs[] ={
    /* #jid       #cap      #name      */
    {0x00BF258D,  512,      "SST25VF040B"},
    {0x00C22815,  2048,     "MX25R1635F"},
    {0x00C22812,  256,      "MX25R2035F"},
    {0x009d7014,  1024,     "IS25WP080D"}, /* 1.65v~1.95v */
    {0x009d6014,  1024,     "IS25LP080D"}, /* 2.30v~3.60v */
    {0x00620613,  512,      "SST25PF040C"},
    {0x00C22813,  512,      "MX25R4035F"},
    {0x00C22814,  1024,     "MX25R8035F"},
    {0x001f8401,  512,      "AT25SF041"},
    {0x001f8501,  1024,      "AT25SF081B"}, /* TBD */
    {0x00EF4014,  1024,     "W25Q80DV/DL"},
};

static return_result_t read_flash_device_id(capi_phy_info_t* phy_info_ptr, uint8_t* fid_ptr)
{
    uint32_t retry = 0;
    uint32_t reg_val_depth, i;

    phy_info_ptr->base_addr = SPIF;
    hsip_wr_reg_(phy_info_ptr, SPIF_RLEN, SPI_RDID_LEN);
    wr8_reg_ex(SPIF_WFIFO_BASE, SPI_RDID, phy_info_ptr->phy_id);
    do {
        ERR_HSIP(reg_val_depth = hsip_rd_reg_(phy_info_ptr, SPIF_RFIFO_DEPTH));
        retry++;
    } while ((reg_val_depth != SPI_RDID_LEN) && (retry < 100));

    if (reg_val_depth != SPI_RDID_LEN)
        return RR_ERROR;
    for (i=0; i<SPI_RDID_LEN; i++)
        *fid_ptr++ = rd8_reg_ex(SPIF_RFIFO_BASE, phy_info_ptr->phy_id);
    return RR_SUCCESS;
}


return_result_t read_status(capi_phy_info_t* phy_info_ptr, uint8_t command_id, uint8_t* status_ptr){
     uint32_t retry = 0;
     uint32_t reg_val_depth;  

     phy_info_ptr->base_addr = SPIF;
     hsip_wr_field_(phy_info_ptr, SPIF_CTRL, SPIFEN, 0x0);    

     phy_info_ptr->base_addr = SPIF;
     hsip_wr_reg_(phy_info_ptr, SPIF_RLEN, 0x1);

     wr8_reg_ex(0x21000000, command_id, phy_info_ptr->phy_id);

     phy_info_ptr->base_addr = SPIF;
     hsip_wr_field_(phy_info_ptr, SPIF_CTRL, SPIFEN, 0x1);

    phy_info_ptr->base_addr = 0x00;
    do {
          ERR_HSIP(reg_val_depth = hsip_rd_reg_(phy_info_ptr, 0x21000084));
          delay_ms(10);
          //CAPI_DELAY_MS(10);
          retry++;
    }
    while ((reg_val_depth !=0x1)&& (retry < 1000));

    if (reg_val_depth !=0x1) 
        return RR_ERROR;
    *status_ptr = rd8_reg_ex(SPIF_RFIFO_BASE, phy_info_ptr->phy_id);
    return RR_SUCCESS;
}

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
                                    phy_static_config_t* phy_static_config_ptr)
{
    return_result_t ret = RR_SUCCESS;
    uint32_t data_write = 0;
    uint32_t base_addr = 0;
    uint32_t i = 0;
    uint32_t write_size = 0;
    CAPI_LOG_INFO("host_download_sram() - download\n");

    phy_info_ptr->base_addr = MGT;
    hsip_wr_reg_(phy_info_ptr, PCR_RST_CTR,
                               PCR_RST_CTR_M0P3_SRST_EN_MASK |
                               PCR_RST_CTR_M0P2_SRST_EN_MASK |
                               PCR_RST_CTR_M0P1_SRST_EN_MASK |
                               PCR_RST_CTR_M0P0_SRST_EN_MASK);

    hsip_wr_reg_(phy_info_ptr, COM_REMAP, 0x0);    /* disable remap */


    phy_info_ptr->base_addr = 0x00;
   
    write_size = size >> 2;
    for (i = 0; i < write_size; i++) {
        data_write = wholeimage_sram_ptr[i];
        hsip_wr_reg_(phy_info_ptr, (base_addr + i * 4), data_write);
    }

   

    phy_info_ptr->base_addr = MGT;
    hsip_wr_reg_(phy_info_ptr, PCR_RST_CTR,
                               PCR_RST_CTR_M0P3_SRST_EN_MASK |
                               PCR_RST_CTR_M0P2_SRST_EN_MASK |
                               PCR_RST_CTR_M0P1_SRST_EN_MASK |
                               PCR_RST_CTR_M0P0_SRST_EN_MASK);

   

    hsip_wr_reg_(phy_info_ptr, PCR_RST_CTR,
                               PCR_RST_CTR_M0P3_SRST_EN_MASK |
                               PCR_RST_CTR_M0P2_SRST_EN_MASK |
                               PCR_RST_CTR_M0P1_SRST_EN_MASK);
    return ret;
}

return_result_t write_a_section(capi_phy_info_t* phy_info_ptr, capi_download_info_t* download_info_ptr, uint32_t base_addr, uint32_t write_size, uint32_t offset) 
{
    uint32_t total_block_num = 0;
    uint32_t transfer_size = 0;
    uint32_t image_start_in_sram  = 0;
    uint32_t block_write_size = 0;
    uint32_t* wholeimage_sram_ptr;
    uint32_t i = 0;
    uint32_t last_block_transfer_size = 0 ;

    wholeimage_sram_ptr = download_info_ptr->image_info.image_ptr;
    if (phy_info_ptr->i2c_block_write_size == 0) 
       block_write_size = I2C_BLOCK_WRITE_SIZE;
    else
       block_write_size  = phy_info_ptr->i2c_block_write_size;

    if (write_size <= block_write_size) {
        total_block_num = 1;
        transfer_size = write_size;  
        last_block_transfer_size = transfer_size;
    }
    else {
        if (( write_size % block_write_size) ==0){
            total_block_num = write_size / block_write_size;
            transfer_size = block_write_size;   
            last_block_transfer_size = transfer_size;
        }
        else {
            total_block_num = write_size / block_write_size;
            transfer_size = block_write_size;  
            last_block_transfer_size = write_size- (write_size / block_write_size) * block_write_size;
        }
    }

    for (i = 0; i < total_block_num ; i++) {
        image_start_in_sram = base_addr + i * block_write_size;      
        if (i==(total_block_num-1)) {
            transfer_size = last_block_transfer_size;
        }
        if (burst_write(phy_info_ptr, download_info_ptr, image_start_in_sram, transfer_size, (uint32_t*) &wholeimage_sram_ptr[offset+i*block_write_size/4])){
                 return  RR_ERROR_SRAM_BURST_WRITE_ERROR;                
        }
    }
    return RR_SUCCESS;


}
return_result_t host_download_sram_fast(capi_phy_info_t*     phy_info_ptr,
                                   capi_download_info_t* download_info_ptr)
{
    return_result_t ret = RR_SUCCESS;   
    uint32_t base_addr = 0;
    uint32_t write_size = 0;
    uint32_t offset =0;
    CAPI_LOG_INFO("host_download_sram() - download\n");

    phy_info_ptr->base_addr = MGT;
    hsip_wr_reg_(phy_info_ptr, PCR_RST_CTR,
                               PCR_RST_CTR_M0P3_SRST_EN_MASK |
                               PCR_RST_CTR_M0P2_SRST_EN_MASK |
                               PCR_RST_CTR_M0P1_SRST_EN_MASK |
                               PCR_RST_CTR_M0P0_SRST_EN_MASK);

    hsip_wr_reg_(phy_info_ptr, COM_REMAP, 0x0);    /* disable remap */


    phy_info_ptr->base_addr = 0x00;

    offset = 0x00;   
    base_addr =0x0000;
    write_size = download_info_ptr->image_info.image_size ;
    ret = write_a_section(phy_info_ptr, download_info_ptr, base_addr, write_size, offset);
    if(ret!=RR_SUCCESS) return ret;

    phy_info_ptr->base_addr = MGT;
    hsip_wr_reg_(phy_info_ptr, PCR_RST_CTR,
                               PCR_RST_CTR_M0P3_SRST_EN_MASK |
                               PCR_RST_CTR_M0P2_SRST_EN_MASK |
                               PCR_RST_CTR_M0P1_SRST_EN_MASK |
                               PCR_RST_CTR_M0P0_SRST_EN_MASK);

    hsip_wr_reg_(phy_info_ptr, PCR_RST_CTR,
                               PCR_RST_CTR_M0P3_SRST_EN_MASK |
                               PCR_RST_CTR_M0P2_SRST_EN_MASK |
                               PCR_RST_CTR_M0P1_SRST_EN_MASK);
    return ret;
}

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
return_result_t host_download_spi_internal(capi_phy_info_t* phy_info_ptr,
                                           capi_download_info_t* download_info_ptr)
{
    return_result_t ret = RR_SUCCESS;
    uint32_t data_write = 0;    
    uint32_t i = 0, j = 0, counter = 0;    
    uint32_t image_read_size =0;
    uint8_t phy_num = 0;
    uint32_t transfer_size = 0;
    uint32_t temp_var = 0;
    uint32_t IMAGE_SIZE_DOWNLOAD_BLOCKS =0;
    uint32_t image_start_in_sram = 0x8000;
    uint32_t*  wholeimage_spi_ptr;
    uint32_t size =0;

    uint32_t num_of_phy = 1;
    uint32_t original_phy_id = phy_info_ptr->phy_id;
    chip_top_spi_program_command_t chip_top_spi_program_command;

    wholeimage_spi_ptr = download_info_ptr->image_info.image_ptr;
    size = download_info_ptr->image_info.image_size;

    original_phy_id = phy_info_ptr->phy_id;

    phy_info_ptr->base_addr = OCTAL_TOP_REGS;
    hsip_wr_field_(phy_info_ptr, REG_BOOT_EN_CTRL, BOOT_EN_VAL, 0x0);
    hsip_wr_field_(phy_info_ptr, REG_BOOT_EN_CTRL, BOOT_EN_OVR, 0x0);

    phy_info_ptr->base_addr = MGT;
    for (phy_num = 0; phy_num <  num_of_phy; phy_num++){
       phy_info_ptr->phy_id = original_phy_id + phy_num;
       hsip_wr_reg_(phy_info_ptr, PCR_RST_CTR,
                                  PCR_RST_CTR_M0P3_SRST_EN_MASK |
                                  PCR_RST_CTR_M0P2_SRST_EN_MASK |
                                  PCR_RST_CTR_M0P1_SRST_EN_MASK |
                                  PCR_RST_CTR_M0P0_SRST_EN_MASK);
    }
    phy_info_ptr->base_addr = MGT;
    hsip_wr_reg_(phy_info_ptr, COM_REMAP, 0x0);
    delay_ms(10);
    phy_info_ptr->base_addr = 0;   
    hsip_wr_reg_(phy_info_ptr, 0x6ffe4, 0x668899AA); 
    hsip_wr_reg_(phy_info_ptr, 0x6ffe0, 0);   

    for (phy_num = 0; phy_num <  num_of_phy; phy_num++){
        phy_info_ptr->base_addr = SPIF;
        phy_info_ptr->phy_id = original_phy_id + phy_num;
        hsip_wr_field_(phy_info_ptr,  SPIF_CTRL, SPIFEN, 0x0);
    }

    phy_info_ptr->phy_id = original_phy_id;
    phy_info_ptr->base_addr = MGT;
    hsip_wr_reg_(phy_info_ptr, COM_REMAP, 0x0);

    for (i = 0; i < sizeof(spi_program_fast) / 4; i++) {
        data_write = spi_program_fast[i];
        phy_info_ptr->base_addr = 0x00;
        hsip_wr_reg_(phy_info_ptr, ( i * 4), data_write);
    }

    phy_info_ptr->base_addr = MGT;

    hsip_wr_reg_(phy_info_ptr, PCR_RST_CTR,
                                    PCR_RST_CTR_M0P3_SRST_EN_MASK |
                                    PCR_RST_CTR_M0P2_SRST_EN_MASK |
                                    PCR_RST_CTR_M0P1_SRST_EN_MASK);

    IMAGE_SIZE_DOWNLOAD_BLOCKS = (size / IMAGE_DOWNLOAD_EACH_BLOCK_SIZE) + 1;
    for (j = 0 ; j < IMAGE_SIZE_DOWNLOAD_BLOCKS ; j++)
    {
         phy_info_ptr->base_addr = 0x00;
         if(j==0) hsip_wr_reg_(phy_info_ptr, 0x6ffd4, 1);
         else hsip_wr_reg_(phy_info_ptr, 0x6ffd4, 0);

        if (j != (IMAGE_SIZE_DOWNLOAD_BLOCKS-1)) {
             transfer_size = 0x10000;
             hsip_wr_reg_(phy_info_ptr, 0x6ffdc, (0x0 + j * transfer_size));
         } else {
             transfer_size = size - j * 0x10000;
             hsip_wr_reg_(phy_info_ptr, 0x6ffdc, (0x0 + j * 0x10000));
         }

         hsip_wr_reg_(phy_info_ptr, 0x6ffd8, (0x0 + transfer_size));
         hsip_wr_reg_(phy_info_ptr, 0x6ffe8, image_start_in_sram);
         
         temp_var = transfer_size >> 2;
         image_read_size =  temp_var;
         phy_info_ptr->base_addr = 0x00;
         if (j!= (IMAGE_SIZE_DOWNLOAD_BLOCKS-1)) {
             for (i = 0; i < image_read_size; i++) {
                 data_write = wholeimage_spi_ptr[i+j*( temp_var)];
                 hsip_wr_reg_(phy_info_ptr, (image_start_in_sram + i * 4), data_write);
             }
         } else {
             for (i = 0; i < image_read_size; i++) {
                 data_write = wholeimage_spi_ptr[i+j*0x10000/4];
                 hsip_wr_reg_(phy_info_ptr, (image_start_in_sram + i * 4), data_write);
             }
         }
         phy_info_ptr->base_addr = 0x00;
         hsip_wr_reg_(phy_info_ptr, 0x6ffe0, 0);
         chip_top_spi_program_command.words =0;
         chip_top_spi_program_command.fields.command_change=1;
         hsip_wr_reg_(phy_info_ptr, 0x6ffe0,  chip_top_spi_program_command.words);
         phy_info_ptr->base_addr = SPIF;
         hsip_wr_field_(phy_info_ptr, SPIF_CTRL, SPIFEN, 0x1);

         counter = 0;
         phy_info_ptr->base_addr = 0x00;
         do {
            ERR_HSIP(chip_top_spi_program_command.words = hsip_rd_reg_(phy_info_ptr,  0x6ffe0));
            if((chip_top_spi_program_command.fields.command_change ==0) && (chip_top_spi_program_command.fields.error_status==1)){            
                break;
            }
            counter++;
            delay_ms(100);
         } while  (counter < 3000);

         if((chip_top_spi_program_command.fields.command_change !=0) || (chip_top_spi_program_command.fields.error_status!=1)){      
             return RR_ERROR_SPI_PROGRAM_VERIFY_FAILED;              
         }
    }

    phy_info_ptr->base_addr = MGT;
    hsip_wr_reg_(phy_info_ptr, PCR_RST_CTR,
                               PCR_RST_CTR_M0P3_SRST_EN_MASK |
                               PCR_RST_CTR_M0P2_SRST_EN_MASK |
                               PCR_RST_CTR_M0P1_SRST_EN_MASK |
                               PCR_RST_CTR_M0P0_SRST_EN_MASK);
    hsip_wr_reg_(phy_info_ptr, COM_REMAP, 0x1);
    phy_info_ptr->base_addr = OCTAL_TOP_REGS;
    hsip_wr_field_(phy_info_ptr, REG_BOOT_EN_CTRL, BOOT_EN_VAL, 0x1);
    hsip_wr_field_(phy_info_ptr, REG_BOOT_EN_CTRL, BOOT_EN_OVR, 0x1);
    phy_info_ptr->base_addr = OCTAL_TOP_REGS;
    hsip_wr_field_(phy_info_ptr, QUAD_CORE_EGRMGT_QC_CHIP_SW_RST_RDB, MD_CHIP_SW_RST, 0x1);
    delay_ms(1);
    phy_info_ptr->phy_id = original_phy_id ;
    return ret;
}


bool burst_write(capi_phy_info_t* phy_info_ptr, capi_download_info_t* download_info_ptr,  uint32_t starting_address, uint32_t size, uint32_t* data) {
    if (download_info_ptr->burst_write_mode == CAPI_BURST_WRITE_MODE_I2C) {
        return burst_write_i2c( starting_address,  size,  data);
    }
    else {
        return burst_write_mdio( starting_address,  size, data, phy_info_ptr->phy_id);
    }
}

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
return_result_t host_download_spi_internal_fast(capi_phy_info_t* phy_info_ptr, capi_download_info_t* download_info_ptr)                                            
{
    return_result_t ret = RR_SUCCESS;
    uint32_t i = 0, j = 0, counter = 0;
    uint8_t phy_num = 0;
    uint32_t transfer_size = 0;
    uint32_t temp_var = 0;
    uint32_t image_start_in_sram = 0x8000;
    uint32_t num_of_phy = 1;
    uint32_t original_phy_id = phy_info_ptr->phy_id;
    chip_top_spi_program_command_t chip_top_spi_program_command;
    uint32_t buffer_id =0;
    uint32_t next_buffer_id =0;
    uint32_t next_image_start_in_sram = 0;
    uint32_t total_i2c_block_num = 0;
    uint32_t i2c_block_write_size = 0;
    uint32_t IMAGE_SIZE_DOWNLOAD_BLOCKS = 0;
    uint32_t*  wholeimage_spi_ptr;
    uint32_t size = 0;

    wholeimage_spi_ptr = download_info_ptr->image_info.image_ptr;
    size = download_info_ptr->image_info.image_size;

    original_phy_id = phy_info_ptr->phy_id;
    if(phy_info_ptr->i2c_block_write_size==0) 
       i2c_block_write_size = I2C_BLOCK_WRITE_SIZE;
    else 
       i2c_block_write_size  = phy_info_ptr->i2c_block_write_size;

    phy_info_ptr->base_addr = OCTAL_TOP_REGS;
    hsip_wr_field_(phy_info_ptr, REG_BOOT_EN_CTRL, BOOT_EN_VAL, 0x0);
    hsip_wr_field_(phy_info_ptr, REG_BOOT_EN_CTRL, BOOT_EN_OVR, 0x0);

    phy_info_ptr->base_addr = MGT;
    for (phy_num = 0; phy_num <  num_of_phy; phy_num++) {
        phy_info_ptr->phy_id = original_phy_id + phy_num;
        hsip_wr_reg_(phy_info_ptr, PCR_RST_CTR,
                                   PCR_RST_CTR_M0P3_SRST_EN_MASK |
                                   PCR_RST_CTR_M0P2_SRST_EN_MASK |
                                   PCR_RST_CTR_M0P1_SRST_EN_MASK |
                                   PCR_RST_CTR_M0P0_SRST_EN_MASK);
    }

    phy_info_ptr->base_addr = MGT;
    hsip_wr_reg_(phy_info_ptr, COM_REMAP, 0x0);
    delay_ms(10);
    phy_info_ptr->base_addr = 0;   
    hsip_wr_reg_(phy_info_ptr, 0x6ffe4, 0x668899AA);
    hsip_wr_reg_(phy_info_ptr, 0x6ffe0, 0);    

    for (phy_num = 0; phy_num <  num_of_phy; phy_num++){
        phy_info_ptr->base_addr = SPIF;
        phy_info_ptr->phy_id = original_phy_id + phy_num;
        hsip_wr_field_(phy_info_ptr,  SPIF_CTRL, SPIFEN, 0x0);
    }

    phy_info_ptr->phy_id = original_phy_id;

    IMAGE_SIZE_DOWNLOAD_BLOCKS = (size / IMAGE_DOWNLOAD_EACH_BLOCK_SIZE) + 1;
    if (((sizeof(spi_program_fast) % i2c_block_write_size) !=0) && ((sizeof(spi_program_fast) > i2c_block_write_size))) {
        return RR_ERROR_SPI_SIZE_INCORRECT;
    }

    if (sizeof(spi_program_fast) <= i2c_block_write_size) {
        total_i2c_block_num = 1;
        transfer_size = sizeof(spi_program_fast);
    }
    else {
        total_i2c_block_num = sizeof(spi_program_fast) / i2c_block_write_size;
        transfer_size = i2c_block_write_size;  
    }

    /* download Flash programming code to SRAM */
    for (i = 0; i < total_i2c_block_num ; i++) {
        image_start_in_sram = i * i2c_block_write_size;
        if (burst_write(phy_info_ptr, download_info_ptr, image_start_in_sram, transfer_size, (uint32_t*) &spi_program_fast[i*i2c_block_write_size/4])){
                 return  RR_ERROR_SPI_BURST_WRITE_ERROR;
        }
    }

    phy_info_ptr->base_addr = MGT;

    hsip_wr_reg_(phy_info_ptr, PCR_RST_CTR,
                               PCR_RST_CTR_M0P3_SRST_EN_MASK |
                               PCR_RST_CTR_M0P2_SRST_EN_MASK |
                               PCR_RST_CTR_M0P1_SRST_EN_MASK);

    /* Download firmware image to SRAM and Program Flash */
    for (j = 0 ; j < IMAGE_SIZE_DOWNLOAD_BLOCKS ; j++)
    {
         buffer_id = j%2;
         image_start_in_sram = 0x8000 + buffer_id * 0x10000; /* block size*/
         phy_info_ptr->base_addr = 0x00;
         if(j==0) 
             hsip_wr_reg_(phy_info_ptr, 0x6ffd4, 1);
         else 
             hsip_wr_reg_(phy_info_ptr, 0x6ffd4, 0);

         if (j!= (IMAGE_SIZE_DOWNLOAD_BLOCKS-1)) {
             transfer_size = 0x10000;
             hsip_wr_reg_(phy_info_ptr, 0x6ffdc, (0x0 + j * transfer_size));
         }
         else {
             transfer_size = size - j* 0x10000;
             hsip_wr_reg_(phy_info_ptr, 0x6ffdc, (0x0 + j * 0x10000));
         }
         hsip_wr_reg_(phy_info_ptr, 0x6ffd8, (0x0 + transfer_size));
         hsip_wr_reg_(phy_info_ptr, 0x6ffe8, image_start_in_sram);

         temp_var = transfer_size >> 2;
         phy_info_ptr->base_addr = 0x00;

         if( j==0) {
            /* First Segment in first bufffer */
            total_i2c_block_num = transfer_size / i2c_block_write_size;
            for (i = 0; i < total_i2c_block_num; i++) {                  
                if (burst_write(phy_info_ptr, download_info_ptr, image_start_in_sram + i*i2c_block_write_size, i2c_block_write_size, (uint32_t*) &wholeimage_spi_ptr[j*( temp_var) + (i*i2c_block_write_size/4)])){
                    return  RR_ERROR_SPI_BURST_WRITE_ERROR;                
                }
            }
         }
         phy_info_ptr->base_addr = 0x00;
         hsip_wr_reg_(phy_info_ptr, 0x6ffe0, 0);
         chip_top_spi_program_command.words =0;
         chip_top_spi_program_command.fields.command_change=1;
         hsip_wr_reg_(phy_info_ptr, 0x6ffe0,  chip_top_spi_program_command.words);
         phy_info_ptr->base_addr = SPIF;
         hsip_wr_field_(phy_info_ptr, SPIF_CTRL, SPIFEN, 0x1);

         /* check ACK of receving the image */
         counter = 0;
         phy_info_ptr->base_addr = 0x00;
         do {
             ERR_HSIP(chip_top_spi_program_command.words = hsip_rd_reg_(phy_info_ptr,  0x6ffe0));
             if (chip_top_spi_program_command.fields.command_change == 0) {
                 break;
             }
             counter++;
             delay_ms(10);
         } while  (counter < 3000);
         if (chip_top_spi_program_command.fields.command_change !=0) 
             return RR_ERROR_SPI_PROGRAM_NOT_ACK;

         /* fill other buffer */
         if (j < (IMAGE_SIZE_DOWNLOAD_BLOCKS-1)) {
             next_buffer_id = (j+1)%2;
             next_image_start_in_sram = 0x8000 + next_buffer_id * 0x10000; /* block size*/

             if (j == (IMAGE_SIZE_DOWNLOAD_BLOCKS-2)) {
                transfer_size = size - (j+1)* 0x10000;
             }
             if (transfer_size <= i2c_block_write_size) {
                total_i2c_block_num = 1;
             }
             else  if(( transfer_size % i2c_block_write_size) !=0){
                total_i2c_block_num = transfer_size / i2c_block_write_size +1;
             }
             else total_i2c_block_num = transfer_size / i2c_block_write_size;
             for (i = 0; i < total_i2c_block_num; i++) {
                 if (burst_write(phy_info_ptr, download_info_ptr, next_image_start_in_sram + i*i2c_block_write_size, i2c_block_write_size, (uint32_t*)&wholeimage_spi_ptr[(j+1)*( temp_var) + (i*i2c_block_write_size/4)])) {
                     return  RR_ERROR_SPI_BURST_WRITE_ERROR;
                }
             }
         }
         /* check ACK of Flash programming */
         counter = 0;
         phy_info_ptr->base_addr = 0x00;
         do {
             ERR_HSIP(chip_top_spi_program_command.words = hsip_rd_reg_(phy_info_ptr,  0x6ffe0));
             if((chip_top_spi_program_command.fields.command_change ==0) && (chip_top_spi_program_command.fields.error_status==1)){            
                 break;
             }
             counter++;
             delay_ms(10);
         } while  (counter < 3000);
         if (chip_top_spi_program_command.fields.error_status !=1)
             return RR_ERROR_SPI_PROGRAM_VERIFY_FAILED;
    }
    if (ret==RR_SUCCESS) {
        phy_info_ptr->base_addr = OCTAL_TOP_REGS;
        hsip_wr_field_(phy_info_ptr, QUAD_CORE_EGRMGT_QC_CHIP_SW_RST_RDB, MD_CHIP_SW_RST, 0x1);
        delay_ms(1);

        phy_info_ptr->base_addr = MGT;
        hsip_wr_reg_(phy_info_ptr, PCR_RST_CTR,
                                   PCR_RST_CTR_M0P3_SRST_EN_MASK |
                                   PCR_RST_CTR_M0P2_SRST_EN_MASK |
                                   PCR_RST_CTR_M0P1_SRST_EN_MASK |
                                   PCR_RST_CTR_M0P0_SRST_EN_MASK);
        hsip_wr_reg_(phy_info_ptr, COM_REMAP, 0x1);
        hsip_wr_reg_(phy_info_ptr, PCR_RST_CTR,
                                   PCR_RST_CTR_M0P3_SRST_EN_MASK |
                                   PCR_RST_CTR_M0P2_SRST_EN_MASK |
                                   PCR_RST_CTR_M0P1_SRST_EN_MASK);
    }

    phy_info_ptr->phy_id = original_phy_id ;
    return ret;
}

/**
 * @brief      host_pre_mdio_download(capi_phy_info_t* phy_info_ptr)
 * @details    
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
return_result_t host_pre_sram_download(capi_phy_info_t* phy_info_ptr)
{
    return_result_t ret_val = RR_SUCCESS;
    phy_info_t fw_phy; 
    /*capi_chip_info_t capi_chip_info;*/
    uint32_t i;

    CAPI_LOG_FUNC("capi_pre_mdio_download", phy_info_ptr);

    util_memset(&fw_phy, 0, sizeof(phy_info_t));
    util_memcpy(&fw_phy, phy_info_ptr, sizeof(phy_info_t));

    fw_phy.base_addr = OCTAL_TOP_REGS;
    hsip_wr_field_(&fw_phy, REG_BOOT_EN_CTRL, BOOT_EN_VAL, 0x0);
    hsip_wr_field_(&fw_phy, REG_BOOT_EN_CTRL, BOOT_EN_OVR, 0x1);

    fw_phy.base_addr = 0x00;
    hsip_wr_reg_(&fw_phy, 0x40000000, 0x1803); /* assert all m0 in reset */

   
    /* clear all gpcfg regs for a clean env for now */
#define GP_CFG_NUM 128
#define GP_CFG_START (OCTAL_TOP_REGS + QUAD_CORE_CONFIG_CFGVAL_0_RDB)
    fw_phy.base_addr = 0x00;
    for (i=0; i<GP_CFG_NUM; i++) {
        if(i==23 || i==38) /*skip top internal config register FW_INTERNAL_CONFIG_REG_3, OCW_DEFAULT_MODE_REG*/
            continue;
        hsip_wr_reg_(&fw_phy, GP_CFG_START + i*4, 0);
    }

    fw_phy.base_addr = OCTAL_TOP_REGS;
    hsip_wr_field_(&fw_phy, QUAD_CORE_EGRMGT_QC_CHIP_SW_RST_RDB, MD_CHIP_SW_RST, 0x1);
    delay_ms(1);

    return ret_val;
}

/**
 * @brief      host_spi_eeprom_ready_for_upgrade(capi_phy_info_t* phy_info_ptr)
 * @details    This API returns whether the spi device is ready for fw upgrade
 *
 * @param      phy_info_ptr
 *
 * @return     returns the result of the called method/function
 */
static return_result_t host_spi_eeprom_ready_for_upgrade(capi_phy_info_t* phy_info_ptr)
{
    uint32_t reg_val = 0;
    return_result_t ret = RR_SUCCESS;
    uint32_t saved_base_addr = phy_info_ptr->base_addr;
    uint32_t i = 0, jid = 0;
    uint8_t dev_id[3], items;
    util_memset(&dev_id[0], 0, sizeof(dev_id));

    ret = read_status(phy_info_ptr, SPI_RDSR, (uint8_t *)&reg_val);

    if (ret == RR_SUCCESS) {
        CAPI_LOG_INFO("SPI DEVICE is ready:%x ", reg_val);
        read_flash_device_id(phy_info_ptr, (uint8_t *)&dev_id);

        items = sizeof(hsip_flash_devs)/sizeof(spi_flash_device_info_t);
        jid = dev_id[0]<<16 | dev_id[1]<<8 | dev_id[2];

        for (i=0; i<items; i++) {
            if (jid == hsip_flash_devs[i].jid) {
                CAPI_LOG_INFO("Flash id=0x%x capacity=%dKB name=%s\n",
                    jid, hsip_flash_devs[i].capacity, hsip_flash_devs[i].name);
                if (hsip_flash_devs[i].capacity < 1024) {
                    CAPI_LOG_INFO("name=%s does not meet capacity requirement\n",hsip_flash_devs[i].name);
                    ret = RR_ERROR_SPI_SIZE_INCORRECT;
                }
                break;
            }
        }
        if (i == items) {
           CAPI_LOG_INFO("Unknown Device id=0x%x\n", jid);
           ret = RR_ERROR_UNKNOWN;
        }
        phy_info_ptr->base_addr = saved_base_addr;
        return ret;
    } else {
        CAPI_LOG_ERROR("SPI DEVICE is not ready:%x\n", reg_val);
        phy_info_ptr->base_addr = saved_base_addr;
        return RR_ERROR_EEPROM_NOT_READY;
    }
}

// code under flag BLOCK_BASED_TRANSFER is reference code for some customers  
// whose mcu mem is limited. Enabling this flag to use 512 bytes data buffer,
// instead of whole image
//#define BLOCK_BASED_TRANSFER
#ifdef BLOCK_BASED_TRANSFER
return_result_t host_mcu_read_flash(uint32_t *start_addr, uint32_t *buf, uint32_t size)
{
    uint32_t i;
    for (i=0; i<size; i++)
        *buf++ = start_addr[i];
    return RR_SUCCESS;
}
#endif

/**
 * @brief      host_flash_hitless_fw_upgrade(capi_phy_info_t* phy_info_ptr, capi_fw_download_info_t *upgrade_info_ptr)
 * @details
 *
 * @param[in]      phy_info_ptr: pointer to the phy
 * @param[in]      upgrade_info_ptr: pointer to firmware upgrade info
 *
 * @return     returns the result of the called method/function
 */
return_result_t host_flash_hitless_fw_upgrade(capi_phy_info_t* phy_info_ptr, capi_fw_download_info_t *upgrade_info_ptr)
{
    uint32_t data_write = 0;
    uint32_t i = 0, counter = 0, j = 0, blk_number;
    uint32_t buf_number = FW_UPGRADE_SECTOR_SIZE/CHIP_TOP_SPI_BLOCK_SIZE-1;
    uint32_t image_read_size = 0;
    return_result_t ret = RR_SUCCESS;
    uint8_t num_of_phy = 1, phy_num;
    uint32_t original_phy_id = phy_info_ptr->phy_id;
    uint32_t block_size = CHIP_TOP_SPI_BLOCK_SIZE;
    uint32_t buffer_id, next_buffer_id;
    uint32_t buffer_location[2];
    uint32_t buffer_status[2];
    chip_top_spi_upgrade_buffer_status_t chip_top_spi_program_buffer_status;
    chip_top_spi_upgrade_command_f_t chip_top_spi_program_command;
    uint32_t whole_size = upgrade_info_ptr->image.size - FW_UPGRADE_START_ADDR_B;
    uint32_t auth;
#ifdef BLOCK_BASED_TRANSFER
    // mcu needs to make sure read data starting at offset of FW_UPGRADE_START_ADDR_B/4 in
    // the image array: const unsigned int wholeimage_spi[]
    // fill the data into this data_buf which is 512 bytes
    uint32_t *start_addr = upgrade_info_ptr->image.src_ptr+FW_UPGRADE_START_ADDR_B/4;
    static uint32_t data_buf[CHIP_TOP_SPI_BLOCK_SIZE/4];
#else
    const uint32_t* wholeimage_spi_ptr = upgrade_info_ptr->image.src_ptr+FW_UPGRADE_START_ADDR_B/4;
#endif
    uint32_t fw_upgrade_start_addr = FW_UPGRADE_START_ADDR;

    /* make sure both spi and flash size are ready */
    ret = host_spi_eeprom_ready_for_upgrade(phy_info_ptr);
    if(ret != RR_SUCCESS) return ret;

    original_phy_id = phy_info_ptr->phy_id;

    buffer_location[0] = CHIP_TOP_SPI_BUFFER1_LOCATION;
    buffer_location[1] = CHIP_TOP_SPI_BUFFER2_LOCATION;
    buffer_status[0] = CHIP_TOP_SPI_PROGRAM_BUFFER_1_STATUS_REG(phy_info_ptr);
    buffer_status[1] = CHIP_TOP_SPI_PROGRAM_BUFFER_2_STATUS_REG(phy_info_ptr);

    for (phy_num = 0; phy_num <  num_of_phy; phy_num++){
        phy_info_ptr->base_addr = SPIF;
        phy_info_ptr->phy_id = original_phy_id + phy_num * 2;
        hsip_wr_field_(phy_info_ptr, COM_COM_CTRL, BOOT_EN_OUT, 0x0);
        hsip_wr_field_(phy_info_ptr,  SPIF_CTRL, SPIFEN, 0x0);
    }

     phy_info_ptr->phy_id = original_phy_id;

    phy_info_ptr->base_addr = 0x00;
    hsip_wr_reg_(phy_info_ptr, 0x40001080, 0x0);

    phy_info_ptr->base_addr = SPIF;
    hsip_wr_field_(phy_info_ptr,  SPIF_CTRL, SPIFEN, 0x1);

    phy_info_ptr->base_addr = SPIF;
    hsip_wr_field_(phy_info_ptr,  SPIF_CTRL, SPIFEN, 0x0);

    CAPI_DEBUG_PRINTF("whole_size :%x\n", whole_size);
    phy_info_ptr->base_addr = 0x00;
    hsip_wr_reg_(phy_info_ptr, CHIP_TOP_SPI_PROGRAM_FW_UPGRADE_CRC_REG, upgrade_info_ptr->image.crc);

    blk_number = (whole_size&(block_size-1))? whole_size/block_size+1:whole_size/block_size;
    image_read_size = block_size/4;
    if (upgrade_info_ptr->image_bank.location == 1)
        fw_upgrade_start_addr = FW_UPGRADE_START_ADDR_B;
#ifdef BLOCK_BASED_TRANSFER
    // make sure first 512 bytes data is ready from mcu
    // add mcu function to send 512 bytes from mcu flash to data_buf
    host_mcu_read_flash(start_addr, data_buf, image_read_size);
    start_addr+=image_read_size;
#endif
    if (blk_number) {
        for (i = 0; i < image_read_size; i++) {
            phy_info_ptr->base_addr = 0x00;
#ifdef BLOCK_BASED_TRANSFER
            data_write = data_buf[i];
#else
            data_write = wholeimage_spi_ptr[i];
#endif
            hsip_wr_reg_(phy_info_ptr, (buffer_location[0] + i * 4), data_write);
        }
        auth = hsip_rd_reg_(phy_info_ptr, CHIP_TOP_SPI_PROGRAM_AUTH_REG(phy_info_ptr));
    }
    for (j = 0 ; j < blk_number; j++) {
        buffer_id = j&1;

         phy_info_ptr->base_addr = SPIF;
         hsip_wr_field_(phy_info_ptr, SPIF_CTRL, SPIFEN, 0x1);
         phy_info_ptr->base_addr = MGT;
         hsip_wr_field_(phy_info_ptr, COM_COM_CTRL, BOOT_EN_OUT, 0x0);

         chip_top_spi_program_command.words = 0;
         chip_top_spi_program_command.fields.block_id = j&buf_number;
         chip_top_spi_program_command.fields.buffer_id = buffer_id;
         chip_top_spi_program_command.fields.start_addr = fw_upgrade_start_addr + j*block_size;
         chip_top_spi_program_command.fields.command_change = 0x1;

         chip_top_spi_program_buffer_status.words = 0;
         if (j == blk_number-1) {
             chip_top_spi_program_buffer_status.fields.data_ready = 0x2;
             chip_top_spi_program_buffer_status.fields.commit = upgrade_info_ptr->image_bank.commit? 1: 0;
             chip_top_spi_program_buffer_status.fields.mode = upgrade_info_ptr->image_bank.trial_run? 1: 0;
         }
         else
             chip_top_spi_program_buffer_status.fields.data_ready = 0x1;
         phy_info_ptr->base_addr = 0x00;
         if (chip_top_spi_program_command.fields.block_id == 0)
             hsip_wr_reg_(phy_info_ptr, UPGRADE_AUTHENTIC_REG, auth);
         hsip_wr_reg_(phy_info_ptr, buffer_status[buffer_id], chip_top_spi_program_buffer_status.words );
         hsip_wr_reg_(phy_info_ptr, CHIP_TOP_SPI_PROGRAM_COMMAND_REG(phy_info_ptr), chip_top_spi_program_command.words );
         counter = 0;
         /* poll command change =0? */
          do {
            phy_info_ptr->base_addr = 0;
            ERR_HSIP(chip_top_spi_program_command.words = hsip_rd_reg_(phy_info_ptr, CHIP_TOP_SPI_PROGRAM_COMMAND_REG(phy_info_ptr)));
            counter++;
            delay_ms(1);
         } while ((( chip_top_spi_program_command.fields.command_change) != 0) && (counter < 30000));

          if(chip_top_spi_program_command.fields.command_change != 0) {
              CAPI_LOG_ERROR("host_flash_hitless_fw_upgrade failed\n");
              return (RR_ERROR);
          }
         if ((j+1)*block_size < whole_size) {
             next_buffer_id = (buffer_id+1)&1;
#ifdef BLOCK_BASED_TRANSFER
            // make sure 512 bytes data is ready from mcu
            // add mcu function to send 512 bytes from mcu flash to data_buf
            host_mcu_read_flash(start_addr, data_buf, image_read_size);
            start_addr+=image_read_size;
#endif
             for (i = 0; i < image_read_size; i++) {
                phy_info_ptr->base_addr = 0x00;
#ifdef BLOCK_BASED_TRANSFER
                data_write = data_buf[i];
#else
                data_write = wholeimage_spi_ptr[i+(j+1)*(block_size/4)];
#endif
                hsip_wr_reg_(phy_info_ptr, (buffer_location[next_buffer_id] + i * 4), data_write);
             }
         }
        counter = 0;
        do {
            phy_info_ptr->base_addr = 0;
            ERR_HSIP(chip_top_spi_program_buffer_status.words = hsip_rd_reg_(phy_info_ptr, buffer_status[buffer_id]));
            counter++;
            delay_ms(1);
         } while (((chip_top_spi_program_buffer_status.fields.data_ready) != 0) && (counter < 30000));
         if((chip_top_spi_program_buffer_status.fields.data_ready!=0)|| (chip_top_spi_program_buffer_status.fields.result!=1)){
              CAPI_LOG_ERROR("Upgrade failed result:%x, data_ready:%x\n", chip_top_spi_program_buffer_status.fields.result, chip_top_spi_program_buffer_status.fields.data_ready);
              return (RR_ERROR);
         }
    }
    phy_info_ptr->phy_id = original_phy_id ;
    return ret;
}

/**
 * @brief      host_override_upgrade_status(capi_phy_info_t* phy_info_ptr, firmware_image_status_t *upgrade_status_ptr)
 * @details
 *
 * @param[in]      phy_info_ptr: pointer to the phy
 * @param[in]      upgrade_status_ptr: pointer to firmware upgrade status
 *
 * @return     returns the result of the called method/function
 */
return_result_t host_override_upgrade_status(capi_phy_info_t* phy_info_ptr, firmware_image_status_t *upgrade_status_ptr)
{
    uint32_t data_write = 0;
    uint32_t counter = 0;
    return_result_t ret = RR_SUCCESS;
    uint8_t num_of_phy = 1, phy_num;
    uint32_t original_phy_id = phy_info_ptr->phy_id;
    uint32_t buffer_location[2];
    uint32_t buffer_status[2];
    chip_top_spi_upgrade_buffer_status_t chip_top_spi_program_buffer_status;
    chip_top_spi_upgrade_command_f_t chip_top_spi_program_command;

    /* make sure both spi and flash size are ready */
    ret = host_spi_eeprom_ready_for_upgrade(phy_info_ptr);
    if(ret != RR_SUCCESS) return ret;
    original_phy_id = phy_info_ptr->phy_id;

    buffer_location[0] = CHIP_TOP_SPI_BUFFER1_LOCATION;
    buffer_location[1] = CHIP_TOP_SPI_BUFFER2_LOCATION;
    buffer_status[0] = CHIP_TOP_SPI_PROGRAM_BUFFER_1_STATUS_REG(phy_info_ptr);
    buffer_status[1] = CHIP_TOP_SPI_PROGRAM_BUFFER_2_STATUS_REG(phy_info_ptr);

    for (phy_num = 0; phy_num <  num_of_phy; phy_num++){
        phy_info_ptr->base_addr = SPIF;
        phy_info_ptr->phy_id = original_phy_id + phy_num * 2;
        hsip_wr_field_(phy_info_ptr, COM_COM_CTRL, BOOT_EN_OUT, 0x0);
        hsip_wr_field_(phy_info_ptr,  SPIF_CTRL, SPIFEN, 0x0);
    }

    phy_info_ptr->phy_id = original_phy_id;

    phy_info_ptr->base_addr = 0x00;
    hsip_wr_reg_(phy_info_ptr, 0x40001080, 0x0);

    phy_info_ptr->base_addr = SPIF;
    hsip_wr_field_(phy_info_ptr,  SPIF_CTRL, SPIFEN, 0x1);

    phy_info_ptr->base_addr = SPIF;
    hsip_wr_field_(phy_info_ptr,  SPIF_CTRL, SPIFEN, 0x0);

    CAPI_DEBUG_PRINTF("upgrade status :%x\n", upgrade_status_ptr->words);
    phy_info_ptr->base_addr = 0x00;
    data_write = upgrade_status_ptr->words;
    hsip_wr_reg_(phy_info_ptr, buffer_location[0], data_write);

    phy_info_ptr->base_addr = SPIF;
    hsip_wr_field_(phy_info_ptr, SPIF_CTRL, SPIFEN, 0x1);
    phy_info_ptr->base_addr = MGT;
    hsip_wr_field_(phy_info_ptr, COM_COM_CTRL, BOOT_EN_OUT, 0x0);

    chip_top_spi_program_command.words = 0;
    chip_top_spi_program_command.fields.command_change = 0x1;
    chip_top_spi_program_command.fields.status_ctrl = 0x1;
    chip_top_spi_program_buffer_status.words = 0;
    phy_info_ptr->base_addr = 0x00;
    data_write = hsip_rd_reg_(phy_info_ptr, CHIP_TOP_SPI_PROGRAM_AUTH_REG(phy_info_ptr));
    hsip_wr_reg_(phy_info_ptr, UPGRADE_AUTHENTIC_REG, data_write);
    hsip_wr_reg_(phy_info_ptr, buffer_status[0], chip_top_spi_program_buffer_status.words );
    hsip_wr_reg_(phy_info_ptr, CHIP_TOP_SPI_PROGRAM_COMMAND_REG(phy_info_ptr), chip_top_spi_program_command.words );

    counter = 0;
    /* poll command change = 0? */
    do {
        phy_info_ptr->base_addr = 0;
        ERR_HSIP(chip_top_spi_program_command.words = hsip_rd_reg_(phy_info_ptr, CHIP_TOP_SPI_PROGRAM_COMMAND_REG(phy_info_ptr)));
        counter++;
        delay_ms(1);
     } while ((( chip_top_spi_program_command.fields.command_change) != 0) && (counter < 30000));

    if(chip_top_spi_program_command.fields.command_change != 0) {
        CAPI_LOG_ERROR("host_flash_hitless_fw_upgrade failed\n");
        return (RR_ERROR);
    }

    counter = 0;
    do {
        phy_info_ptr->base_addr = 0;
        ERR_HSIP(chip_top_spi_program_buffer_status.words = hsip_rd_reg_(phy_info_ptr, buffer_status[0]));
        counter++;
        delay_ms(1);
    } while (((chip_top_spi_program_buffer_status.fields.data_ready) != 0) && (counter < 30000));
    if((chip_top_spi_program_buffer_status.fields.data_ready!=0)|| (chip_top_spi_program_buffer_status.fields.result!=1)){
        CAPI_LOG_ERROR("host_flash_hitless_fw_upgrade failed result:%x, data_ready:%x\n", chip_top_spi_program_buffer_status.fields.result, chip_top_spi_program_buffer_status.fields.data_ready);
        return (RR_ERROR);
    }

    phy_info_ptr->phy_id = original_phy_id ;
    return ret;
}

/**
 * @brief       host_fw_image_status(capi_phy_info_t* phy_info_ptr, firmware_image_status_t* fw_image_status_ptr)
 * @details
 *
 * @param[in]   phy_info_ptr: a reference the the phy information object
 * @param[in]   fw_image_status_ptr: a reference to the firmware image status object
 *
 * @return      returns the result of the called method/function
 */
return_result_t host_fw_image_status(capi_phy_info_t* phy_info_ptr, firmware_image_status_t* fw_image_status_ptr)
{
    uint32_t data_write = 0;
    uint32_t counter = 0;
    return_result_t ret = RR_SUCCESS;
    uint8_t num_of_phy = 1, phy_num;
    uint32_t original_phy_id = phy_info_ptr->phy_id;
    uint32_t buffer_location[2];
    uint32_t buffer_status[2];
    chip_top_spi_upgrade_buffer_status_t chip_top_spi_program_buffer_status;
    chip_top_spi_upgrade_command_f_t chip_top_spi_program_command;

    /* make sure both spi and flash size are ready */
    ret = host_spi_eeprom_ready_for_upgrade(phy_info_ptr);
    if(ret != RR_SUCCESS) return ret;

    buffer_location[0] = CHIP_TOP_SPI_BUFFER1_LOCATION;
    buffer_location[1] = CHIP_TOP_SPI_BUFFER2_LOCATION;
    buffer_status[0] = CHIP_TOP_SPI_PROGRAM_BUFFER_1_STATUS_REG(phy_info_ptr);
    buffer_status[1] = CHIP_TOP_SPI_PROGRAM_BUFFER_2_STATUS_REG(phy_info_ptr);

    for (phy_num = 0; phy_num <  num_of_phy; phy_num++){
        phy_info_ptr->base_addr = SPIF;
        phy_info_ptr->phy_id = original_phy_id + phy_num * 2;
        hsip_wr_field_(phy_info_ptr, COM_COM_CTRL, BOOT_EN_OUT, 0x0);
        hsip_wr_field_(phy_info_ptr,  SPIF_CTRL, SPIFEN, 0x0);
    }

    phy_info_ptr->phy_id = original_phy_id;
    phy_info_ptr->base_addr = 0x00;
    hsip_wr_reg_(phy_info_ptr, 0x40001080, 0x0);

    phy_info_ptr->base_addr = SPIF;
    hsip_wr_field_(phy_info_ptr,  SPIF_CTRL, SPIFEN, 0x1);

    phy_info_ptr->base_addr = SPIF;
    hsip_wr_field_(phy_info_ptr,  SPIF_CTRL, SPIFEN, 0x0);

    CAPI_DEBUG_PRINTF("host_fw_image_status... \n");
    phy_info_ptr->base_addr = 0x00;
    data_write = 0xFF;
    hsip_wr_reg_(phy_info_ptr, buffer_location[0], data_write);

    phy_info_ptr->base_addr = SPIF;
    hsip_wr_field_(phy_info_ptr, SPIF_CTRL, SPIFEN, 0x1);
    phy_info_ptr->base_addr = MGT;
    hsip_wr_field_(phy_info_ptr, COM_COM_CTRL, BOOT_EN_OUT, 0x0);

    chip_top_spi_program_command.words = 0;
    chip_top_spi_program_command.fields.command_change = 0x1;
    chip_top_spi_program_command.fields.status_ctrl = 0x1;
    chip_top_spi_program_buffer_status.words = 0;
    phy_info_ptr->base_addr = 0x00;
    hsip_wr_reg_(phy_info_ptr, buffer_status[0], chip_top_spi_program_buffer_status.words );
    hsip_wr_reg_(phy_info_ptr, CHIP_TOP_SPI_PROGRAM_COMMAND_REG(phy_info_ptr), chip_top_spi_program_command.words );

    counter = 0;
    /* poll command change = 0? */
    do {
        phy_info_ptr->base_addr = 0;
        ERR_HSIP(chip_top_spi_program_command.words = hsip_rd_reg_(phy_info_ptr, CHIP_TOP_SPI_PROGRAM_COMMAND_REG(phy_info_ptr)));
        counter++;
        delay_ms(1);
     } while ((( chip_top_spi_program_command.fields.command_change) != 0) && (counter < 30000));

    if(chip_top_spi_program_command.fields.command_change != 0) {
        CAPI_LOG_ERROR("host_fw_image_status failed\n");
        return (RR_ERROR);
    }

    counter = 0;
    do {
        phy_info_ptr->base_addr = 0;
        ERR_HSIP(chip_top_spi_program_buffer_status.words = hsip_rd_reg_(phy_info_ptr, buffer_status[0]));
        counter++;
        delay_ms(1);
    } while (((chip_top_spi_program_buffer_status.fields.data_ready) != 0) && (counter < 30000));
    if((chip_top_spi_program_buffer_status.fields.data_ready!=0)|| (chip_top_spi_program_buffer_status.fields.result!=1)){
        CAPI_LOG_ERROR("host_fw_image_status failed result:%x, data_ready:%x\n",
                       chip_top_spi_program_buffer_status.fields.result,
                       chip_top_spi_program_buffer_status.fields.data_ready);
        return (RR_ERROR);
    }
    fw_image_status_ptr->words = hsip_rd_reg_(phy_info_ptr, buffer_location[0]);
    phy_info_ptr->phy_id = original_phy_id ;
    return ret;
}


/**
 *
 * @file     reg_access.h
 * @author  BRCM HSIP Firmware Team
 * @date     9/15/2018
 * @version  1.0
 *
 * @property  $Copyright: Copyright 2018 Broadcom INC.
 * This program is the proprietary software of Broadcom INC
 * and/or its licensors, and may only be used, duplicated, modified
 * or distributed pursuant to the terms and conditions of a separate,
 * written license agreement executed between you and Broadcom
 * (an "Authorized License").  Except as set forth in an Authorized
 * License, Broadcom grants no license (express or implied), right
 * to use, or waiver of any kind with respect to the Software, and
 * Broadcom expressly reserves all rights in and to the Software
 * and all intellectual property rights therein.  IF YOU HAVE
 * NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE
 * IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE
 * ALL USE OF THE SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1.     This program, including its structure, sequence and organization,
 * constitutes the valuable trade secrets of Broadcom, and you shall use
 * all reasonable efforts to protect the confidentiality thereof,
 * and to use this information only in connection with your use of
 * Broadcom integrated circuit products.
 *
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS
 * PROVIDED "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
 * REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
 * OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
 * DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
 * NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
 * ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
 * OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL
 * BROADCOM OR ITS LICENSORS BE LIABLE FOR   CONSEQUENTIAL,
 * INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER
 * ARISING OUT OF OR IN ANY WAY RELATING TO YOUR USE OF OR INABILITY
 * TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF
 * THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR USD 1.00,
 * WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING
 * ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.$
 *
 * @brief   this file includes platform dependent PHY register access functions
 *
 * @section
 * 
 */
 
#ifndef REG_ACCESS_H
#define REG_ACCESS_H

#include "type_defns.h"

#ifdef __cplusplus
extern "C" {
#endif

/* #define BURST_MDIO  1 */
/**
 * @brief      rd_reg_ex(long address, long port)
 * @details    This API is used to read 32 bits data from PHY register/memory
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in] address: PHY register/memory address
 * @param[in] port: PHYAD
 * 
 * @return     returns 32 bits data
 */
long rd_reg_ex(long address, long port);

/**
 * @brief      rd_reg_ex(long address, long port)
 * @details    This API is used to read 16bits data from PHY register/memory
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in] address: PHY register/memory address
 * @param[in] port: PHYAD
 * 
 * @return     returns 16 bits data
 */
int rd16_reg_ex(long address, long port);

/**
 * @brief      rd8_reg_ex(uint32_t address, uint32_t port)
 * @details    This API is used to read 8 bits data from PHY register/memory
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in] address: PHY register/memory address
 * @param[in] port: PHYAD
 * 
 * @return     returns 8 bits data
 */
uint32_t rd8_reg_ex(uint32_t address, uint32_t port);

/**
 * @brief      rd_qsfp_reg_ex(long address, long port)
 * @details    This API is used to read 8 bits data from QSFP PHY register/memory
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in] address: PHY register/memory address
 * @param[in] port: PHYAD
 * 
 * @return     returns 8 bits data
 */
unsigned char rd_qsfp_reg_ex(long address, long port);

/**
 * @brief      wr_reg_ex(long address, long data, long port)
 * @details    This API is used to write 32 bits data to PHY register/memory
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in] address: PHY register/memory address
 * @param[in] data: 32 bits data
 * @param[in] port: PHYAD
 * 
 * @return     none
 */
void wr_reg_ex(long address, long data, long port);

/**
 * @brief      wr16_reg_ex(long address, long data, long port)
 * @details    This API is used to write 16 bits data to PHY register/memory
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in] address: PHY register/memory address
 * @param[in] data: 16 bits data
 * @param[in] port: PHYAD
 * 
 * @return     none
 */
void wr16_reg_ex(long address, long data, long port);

/**
 * @brief      wr8_reg_ex(uint32_t address, uint32_t data, uint32_t port)
 * @details    This API is used to write 8 bits data to PHY register/memory
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in] address: PHY register/memory address
 * @param[in] data: 8 bits data
 * @param[in] port: PHYAD
 * 
 * @return     none
 */
void wr8_reg_ex(uint32_t address, uint32_t data, uint32_t port);

/**
 * @brief      wr_qsfp_reg_ex(long address, long data, long port)
 * @details    This API is used to write 8 bits data to QSFP PHY register/memory
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in] address: PHY register/memory address
 * @param[in] data: 8 bits data
 * @param[in] port: PHYAD
 * 
 * @return     none
 */
void wr_qsfp_reg_ex(long address, long data, long port);

void display_settings_view(unsigned int view_on);

/**
 * @brief      set_usb_com(int port)
 * @details    This API is used to set USB COM port
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in] port: USB COM port
 * 
 * @return     none
 */
void set_usb_com(int port);

/**
 * @brief     set_i2c_driver_target_address(uint8_t i2c_target)
 * @details    This API is used to set i2c target addrees in host/MCU i2c master driver 
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in] uint8_t qsfp_i2c_address
 * 
 * @return void
 */
void set_i2c_driver_target_address(uint8_t i2c_target);

// FPGA register used in Version D2xx of power board 3
#define ADDR_FPGA_DUT_RST_CTRL                       0x00
#define ADDR_FPGA_DUT_RST_CNT_MSB                    0x01
#define ADDR_FPGA_DUT_RST_CNT_LSB                    0x02
#define ADDR_FPGA_GPIO                               0x03
#define ADDR_FPGA_MDIO_STA_CTRL                      0x04
#define ADDR_FPGA_MDIO_REG_TYPE                      0x05
#define ADDR_FPGA_MDIO_REG_DATA                      0x06
#define ADDR_FPGA_MDIO_DATA                          0x07
#define ADDR_FPGA_I2C_CTRL                           0x08
#define ADDR_FPGA_I2C_DEV_ADDR                       0x09
#define ADDR_FPGA_I2C_REG_TYPE                       0x0A
#define ADDR_FPGA_I2C_REG_DATA                       0x0B
#define ADDR_FPGA_I2C_DATA                           0x0C
#define ADDR_FPGA_I2C_STATUS                         0x0D
#define ADDR_FPGA_MDC_RATE                           0x0E
#define ADDR_FPGA_SCL_RATE                           0x0F
#define ADDR_FPGA_CHIP_ID2                           0x1B
#define ADDR_FPGA_CHIP_ID1                           0x1C
#define ADDR_FPGA_CHIP_ID0                           0x1D
#define ADDR_FPGA_VERSION                            0x1E   //FPGA version register
#define ADDR_FPGA_SUB_VER                            0x1F
#define I2C_REG_LEN_LSB                              0x0
#define I2C_REG_LEN_MID                              0x1
#define I2C_REG_LEN_MSB                              0x2
#define I2C_REG_QSFP_ADDR                            0x3
#define I2C_REG_IND_ADDR0                            0x4
#define I2C_REG_IND_ADDR1                            0x5
#define I2C_REG_IND_ADDR2                            0x6
#define I2C_REG_IND_ADDR3                            0x7
#define I2C_REG_IND_CTRL                             0x8
#define I2C_MODE_BRCM                                0x0
#define DUT_I2C_ADDR                                 0x50
#define I2C_FIFO_LENGTH                              8192  //FPGA FIFO for I2C is dual 8K bytes
#define MDIO_FIFO_LENGTH                             8192  //FPGA FIFO for MDIO is 8K bytes

#define MDIO_BLK_REG_LEN_LSB                         0x0
#define MDIO_BLK_REG_LEN_MID                         0x1
#define MDIO_BLK_REG_LEN_MSB                         0x2
#define MDIO_BLK_REG_PRTAD                           0x3
#define MDIO_BLK_REG_DEVAD                           0x4
#define MDIO_BLK_REG_IDLE_LEN                        0x5
#define MDIO_BLK_HOST_SUM_LSB                        0x06
#define MDIO_BLK_HOST_SUM_MID                        0x07
#define MDIO_BLK_HOST_SUM_MSB                        0x08
#define MDIO_BLK_WR_LEN_LSB                          0x10
#define MDIO_BLK_WR_LEN_MID                          0x11
#define MDIO_BLK_WR_LEN_MSB                          0x12
#define MDIO_BLK_WR_DATA                             0x13
#define MDIO_BLK_RD_DATA                             0x14
#define MDIO_BLK_WR_ADDR                             0x15
#define MDIO_MDIO_REG_7_0                            0x16
#define MDIO_MDIO_REG_15_8                           0x17
#define MDIO_MDIO_REG_23_16                          0x18
#define MDIO_MDIO_REG_31_24                          0x19
#define MDIO_MDIO_BLK_REG_7_0                        0x1A
#define MDIO_MDIO_BLK_REG_15_8                       0x1B
#define MDIO_MDIO_BLK_REG_23_16                      0x1C
#define MDIO_MDIO_BLK_REG_31_24                      0x1D

bool burst_write_mdio( uint32_t starting_address, uint32_t size, uint32_t* data, int prtad);
bool burst_write_i2c( uint32_t starting_address, uint32_t size, uint32_t* data);
/**
* @brief    burst_read_chip_sram(uint32_t starting_address, uint32_t size, uint32_t* data, long port) {
* @details  This API is used to do MDIO/I2C burst read
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  starting_address: staring address in SRAM 
* @param[in]  size: size of this block burst write
* @param[in]  data: pointer to data buffer
* @param[port]
*
* @return     returns the result of the called method/function, either RR_ERROR or RR_SUCCESS
*/
return_result_t burst_read_chip_sram(uint32_t starting_address, uint32_t size, uint32_t* data, long port);

/**
* @brief    burst_write_chip_sram(uint32_t starting_address, uint32_t size, uint32_t* data, long port) {
* @details  This API is used to do MDIO burst write
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  starting_address: staring address in SRAM 
* @param[in]  size: size of this block burst write
* @param[in]  data: pointer to data buffer
* @param[port]
*
* @return     returns the result of the called method/function, either RR_ERROR or RR_SUCCESS
*/
return_result_t burst_write_chip_sram(uint32_t starting_address, uint32_t size, uint32_t* data, long port);
#ifdef __cplusplus
}
#endif

#endif

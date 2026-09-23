/**
 *
 * @file     reg_access.c
 * @author   BRCM HSIP Firmware Team
 * @date     4/01/2019
 * @version  1.0
 *
 * @property  $Copyright: Copyright 2019 Broadcom INC.
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
 * @brief   This file contains the implementation of PHY register read/write with FTDI USB driver.
 *
 * @section
 * 
 */


#include "MON2_HSIP_ENG.h"
#include "hr_time.h"
#include "logger.h"
#include "reg_access.h"

#include "tunnel_def.h"
#include "common_util.h"

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

extern  uint32_t passed_port ;
long rd_reg_ex(long address, long port)
{
    uint32_t data =0;;
    passed_port  = port;
    tunnel_b_read(address, 4, (uint8_t*) &data);
    return ((uint32_t) data);
}


void wr_reg_ex(long address, long data, long port)
{
   uint32_t data_list = data;
   passed_port  = port;
   tunnel_b_write(address, 4, (uint8_t*) &data_list);
}

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
void wr16_reg_ex(long address, long data, long port)
{
     unsigned char dev_addr = (address >> 16) & 0x1F;

     sal_wrmdio(port, dev_addr, (address & 0xffff), data);
}

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
int rd16_reg_ex(long address, long port)
{
    unsigned char dev_addr = (address >> 16) & 0x1F;
    
    return sal_rdmdio(port, dev_addr, (address & 0xffff));
}

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
uint32_t rd8_reg_ex(uint32_t address, uint32_t port)
{
   uint32_t addr_l;
   uint32_t addr_h;
   uint32_t data;

   if (sal_getRcmEst()) {
       data = sal_readAhbI2cByte(sal_getI2CDevEst(), address);
   } else {
       addr_l = address & 0xFFFF;                //sys.GetField32(addr, 15, 0)
       addr_h = (address>>16) & 0xFFFF;                 //sys.GetField32(addr, 31, 16)

       sal_wrmdio(port, 0x1F, 0x0004, 0x0000);               // app.WriteMdio prt, &H1F&, &H0004&, &H0000&      'CTRL = 8bit access
       sal_wrmdio(port, 0x1F, 0x0000, addr_l);                //app.WriteMdio prt, &H1F&, &H0000&, addr_l 'ADDR_L
       sal_wrmdio(port, 0x1F, 0x0001, addr_h);              //app.WriteMdio prt, &H1F&, &H0001&, addr_h 'ADDR_H

       data = sal_rdmdio(port, 0x1F, 0x0002);  //ReadEst8bit = app.ReadMdio(prt, &H1F&, &H0002&) 'DATA_L
   }
   return data;

}

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
void wr8_reg_ex(uint32_t address, uint32_t data, uint32_t port)
{
   uint32_t addr_l;
   uint32_t addr_h;

   if (sal_getRcmEst()) {
       sal_writeAhbI2cByte(sal_getI2CDevEst(), address, data);
   } else {
       addr_l = address & 0xFFFF;                //sys.GetField32(addr, 15, 0)
       addr_h = (address>>16) & 0xFFFF;                 //sys.GetField32(addr, 31, 16)

       sal_wrmdio(port, 0x1F, 0x0004, 0x0000);               // app.WriteMdio prt, &H1F&, &H0004&, &H0000&      'CTRL = 8bit access
       sal_wrmdio(port, 0x1F, 0x0000, addr_l);                //app.WriteMdio prt, &H1F&, &H0000&, addr_l 'ADDR_L
       sal_wrmdio(port, 0x1F, 0x0001, addr_h);              //app.WriteMdio prt, &H1F&, &H0001&, addr_h 'ADDR_H

       sal_wrmdio(port, 0x1F, 0x0002, data);    //app.WriteMdio prt, &H1F&, &H0002&, data         'DATA_L
   }
}

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
unsigned char rd_qsfp_reg_ex(long address, long port)
{
    return sal_readI2c(port, address);
}

/**
 * @brief     set_qsfp_i2c_address(uint8_t qsfp_i2c_address)
 * @details    This API is used to set i2c slave address that Host or MCU will be usded to communciated with the chip
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in] uint8_t qsfp_i2c_address
 * 
 * @return     void
 */
void set_i2c_driver_target_address(uint8_t i2c_target)
{   
     sal_setI2CDevEst(i2c_target);
}


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
void wr_qsfp_reg_ex(long address, long data, long port)
{
    sal_writeI2c(port, address, (unsigned int)data);
}

void display_settings_view(unsigned int view_on) {
    sal_settingView(view_on);
}

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
void set_usb_com(int port)
{
    set_com(port);
}

void addrmdio45(uint32_t phy_id, uint32_t address){
    sal_addrmdio45(phy_id, 0x1f, address);
}

void writemdio45(uint32_t phy_id, uint32_t value)
{
    sal_writemdio45(phy_id, 0x1f, value);
}

int readmdio45(int phyad, int devad)
{
   return sal_readmdio45(phyad, devad);
}

/**
* @brief    bool burst_write_mdio(uint32_t starting_address, uint32_t size, uint32_t* data, int prtad) {
* @details  This API is used to do MDIO burst write
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  starting_address: staring address in SRAM 
* @param[in]  size: size of this block burst write
* @param[in]  data: pointer to data 
*
* @return     returns the result of the called method/function, either RR_ERROR or RR_SUCCESS
*/
bool burst_write_mdio(uint32_t starting_address, uint32_t size, uint32_t* data, int prtad) {
    uint32_t i =0;
    uint32_t data_write, data_0 , data_1;
    uint32_t iter = 2;
    uint8_t fpga_rd_data, mdio_ctrl_reg;
    uint32_t loop_cnt, total_uP_code_wr;
    char odata[I2C_FIFO_LENGTH*2];
    char idata[1];
    uint32_t ocnt = 0;
    uint32_t icnt = 0;
    uint32_t mdio_blk_chk_cnt;
    uint32_t mdio_blk_wr_chk_sum;
    uint32_t mdio_blk_rd_chk_sum;

    addrmdio45(prtad, 0x04);
    writemdio45(prtad, 0x50);

    addrmdio45(prtad, 0x00);
    writemdio45(prtad, starting_address & 0x0000ffff);

    addrmdio45(prtad, 0x01);
    writemdio45(prtad, (starting_address & 0xffff0000)>>16);

    addrmdio45(prtad, 0x02);
    if (0) {
       iter =  size >> 2;
       for (i = 0; i < iter; i++) {
           data_write = data[i];
           data_0 = data_write & 0xffff;
           data_1 = (data_write & 0xffff0000) >> 16;
           writemdio45(prtad, data_0);
           writemdio45(prtad, data_1);
       }
       return RR_SUCCESS;
    } 
    else {  // MDIO burst write from BCM9_I2C_MDIO FPGA board
       //fpga_rd_data = sal_rdFPGA(ADDR_FPGA_VERSION);
       //if ((fpga_rd_data == 0) || (fpga_rd_data == 0xFF)) {
       //   //CAPI_DEBUG_PRINTF("FPGA version is illegal: %x\n", fpga_rd_data);
       //   dprintf("ERROR: FPGA version is illegal: %x\r\n", fpga_rd_data);
       //   return RR_ERROR;
       //}
       //else if ((fpga_rd_data == 0xD2) || (fpga_rd_data == 0xD1))  {
       //   dprintf("WARNING: FPGA version is 0x%X, and this FPGA does not support MDIO burst write\r\n", fpga_rd_data);
       //   return RR_ERROR;
       //}
       //else if ( (fpga_rd_data == 0xD3)) {
       //  /* dprintf("MDIO burst write of %d-byte to starting_address 0x%X\n", size, starting_address);*/
       //  /* dprintf("The first word of the uP code is 0x%X\n", data[0]);*/
       //}
       dprintf("INFO: %d bytes of MDIO burst write from addrss 0x%X\r\n", size, starting_address);
       mdio_ctrl_reg = 0x4;  // reset MDIO BLK write
       sal_wrFPGA(ADDR_FPGA_MDIO_STA_CTRL, mdio_ctrl_reg);
       sal_wrFPGA(ADDR_FPGA_MDIO_REG_TYPE, MDIO_BLK_REG_PRTAD); // set MDIO 5-bit PHY addr
       sal_wrFPGA(ADDR_FPGA_MDIO_REG_DATA, prtad);  // PHY ID
       sal_wrFPGA(ADDR_FPGA_MDIO_REG_TYPE, MDIO_BLK_REG_DEVAD); // set MDIO 5-bit DEV addr
       sal_wrFPGA(ADDR_FPGA_MDIO_REG_DATA, 0x1F);
       sal_wrFPGA(ADDR_FPGA_MDIO_REG_TYPE, MDIO_BLK_REG_LEN_LSB);   // set MDIO burst write length
       sal_wrFPGA(ADDR_FPGA_MDIO_REG_DATA, size & 0xFF);        // size is for 32-bit data array
       sal_wrFPGA(ADDR_FPGA_MDIO_REG_TYPE, MDIO_BLK_REG_LEN_MID);
       sal_wrFPGA(ADDR_FPGA_MDIO_REG_DATA, (size >> 8) & 0xFF);
       sal_wrFPGA(ADDR_FPGA_MDIO_REG_TYPE, MDIO_BLK_REG_LEN_MSB);
       sal_wrFPGA(ADDR_FPGA_MDIO_REG_DATA, 0);
       sal_wrFPGA(ADDR_FPGA_MDIO_REG_TYPE, MDIO_BLK_REG_IDLE_LEN);
       sal_wrFPGA(ADDR_FPGA_MDIO_REG_DATA, 0x0); // 0-bis MDIO preamble
       mdio_ctrl_reg = 0x0;  // clear MDIO BLK start bit
       sal_wrFPGA(ADDR_FPGA_MDIO_STA_CTRL, mdio_ctrl_reg); 
       total_uP_code_wr = 0;
       do {
          ocnt = 0;
          mdio_blk_wr_chk_sum = 0;
          if ((size - total_uP_code_wr) >= MDIO_FIFO_LENGTH) {  // Fill the whole 8K MDIO FIFO: 
                                                                // 8K WR MDIO_DATA registe CMD, 8K MDIO uP: 16K odata from PC to FPGA
             for (loop_cnt = 0; loop_cnt < MDIO_FIFO_LENGTH/4; loop_cnt++) {
                 odata[ocnt]   = 0xE0 | (ADDR_FPGA_MDIO_DATA & 0x1F);
                 odata[ocnt+2] = 0xE0 | (ADDR_FPGA_MDIO_DATA & 0x1F);
                 odata[ocnt+4] = 0xE0 | (ADDR_FPGA_MDIO_DATA & 0x1F);
                 odata[ocnt+6] = 0xE0 | (ADDR_FPGA_MDIO_DATA & 0x1F);
                 odata[ocnt+1] = (data[total_uP_code_wr/4 + loop_cnt] >> 8) & 0xFF;
                 odata[ocnt+3] =  data[total_uP_code_wr/4 + loop_cnt] & 0xFF;
                 odata[ocnt+5] = (data[total_uP_code_wr/4 + loop_cnt] >> 24) & 0xFF;
                 odata[ocnt+7] = (data[total_uP_code_wr/4 + loop_cnt] >> 16) & 0xFF;
                 mdio_blk_wr_chk_sum += ((data[total_uP_code_wr/4 + loop_cnt] >> 8) & 0xFF) +
                                        (data[total_uP_code_wr/4 + loop_cnt] & 0xFF) +
                                        ((data[total_uP_code_wr/4 + loop_cnt] >> 24) & 0xFF) +
                                        ((data[total_uP_code_wr/4 + loop_cnt] >> 16) & 0xFF);
                 ocnt = ocnt + 8;
             }
             total_uP_code_wr = total_uP_code_wr + MDIO_FIFO_LENGTH;
          }
          else {  //last block of MDIO data
             for (loop_cnt = 0; loop_cnt < (size - total_uP_code_wr)/4; loop_cnt++) {
                 odata[ocnt]   = 0xE0 | (ADDR_FPGA_MDIO_DATA & 0x1F);
                 odata[ocnt+2] = 0xE0 | (ADDR_FPGA_MDIO_DATA & 0x1F);
                 odata[ocnt+4] = 0xE0 | (ADDR_FPGA_MDIO_DATA & 0x1F);
                 odata[ocnt+6] = 0xE0 | (ADDR_FPGA_MDIO_DATA & 0x1F);
                 odata[ocnt+1] = (data[total_uP_code_wr/4 + loop_cnt] >> 8) & 0XFF;
                 odata[ocnt+3] =  data[total_uP_code_wr/4 + loop_cnt] & 0XFF;
                 odata[ocnt+5] = (data[total_uP_code_wr/4 + loop_cnt] >> 24) & 0XFF;
                 odata[ocnt+7] = (data[total_uP_code_wr/4 + loop_cnt] >> 16) & 0XFF;
                 mdio_blk_wr_chk_sum += ((data[total_uP_code_wr/4 + loop_cnt] >> 8) & 0xFF) +
                                        (data[total_uP_code_wr/4 + loop_cnt] & 0xFF) +
                                        ((data[total_uP_code_wr/4 + loop_cnt] >> 24) & 0xFF) +
                                        ((data[total_uP_code_wr/4 + loop_cnt] >> 16) & 0xFF);
                 ocnt = ocnt + 8;
             }
             total_uP_code_wr = size;
          }
          sal_send2usb(ocnt, (unsigned char *) odata, icnt, (unsigned char *) idata);  //MDIO data FIFO is filled
          mdio_ctrl_reg = 0x2;
          sal_wrFPGA(ADDR_FPGA_MDIO_STA_CTRL, mdio_ctrl_reg);  //Start MDIO BLK write
          sal_wrFPGA(ADDR_FPGA_MDIO_REG_TYPE, MDIO_BLK_HOST_SUM_LSB);    // set MDIO burst write length
          sal_wrFPGA(ADDR_FPGA_MDIO_REG_DATA, mdio_blk_wr_chk_sum & 0xFF);  // mdio_blk_wr_chk_sum is 24-bit data check sum for MDIO block write
          sal_wrFPGA(ADDR_FPGA_MDIO_REG_TYPE, MDIO_BLK_HOST_SUM_MID);
          sal_wrFPGA(ADDR_FPGA_MDIO_REG_DATA, (mdio_blk_wr_chk_sum >> 8) & 0xFF);
          sal_wrFPGA(ADDR_FPGA_MDIO_REG_TYPE, MDIO_BLK_HOST_SUM_MSB);
          sal_wrFPGA(ADDR_FPGA_MDIO_REG_DATA, (mdio_blk_wr_chk_sum >> 16)  & 0xFF);          //polling_fpga_mdio_bk_wr_done
          mdio_blk_chk_cnt = 0;
          do {
             delay_ms(1);
             fpga_rd_data = sal_rdFPGA(ADDR_FPGA_MDIO_STA_CTRL);
             mdio_blk_chk_cnt++;
             if (mdio_blk_chk_cnt > 1000) {
                total_uP_code_wr = size;  // break the mail while check of total_uP_code_wr < size
                break;
             }
          } while ((fpga_rd_data & 0x04) == 0);  // D2 = mdio_blk_done
          sal_wrFPGA(ADDR_FPGA_MDIO_STA_CTRL, 0x00); // prepare for the next frame
          sal_wrFPGA(ADDR_FPGA_MDIO_REG_TYPE, MDIO_BLK_REG_IDLE_LEN);
          sal_wrFPGA(ADDR_FPGA_MDIO_REG_DATA, 0x10); // 16-bits MDIO preamble

          if ((fpga_rd_data & 0x30) != 0) {  // D5:mdio_blk_total_mismatch
                                             // D4:mdio_blk_chk_sum_mismatch
             sal_wrFPGA(ADDR_FPGA_MDIO_REG_TYPE, MDIO_BLK_HOST_SUM_LSB);
             fpga_rd_data = sal_rdFPGA(ADDR_FPGA_MDIO_REG_DATA);
             mdio_blk_rd_chk_sum = fpga_rd_data;
             sal_wrFPGA(ADDR_FPGA_MDIO_REG_TYPE, MDIO_BLK_HOST_SUM_MID);
             fpga_rd_data = sal_rdFPGA(ADDR_FPGA_MDIO_REG_DATA);
             mdio_blk_rd_chk_sum = mdio_blk_rd_chk_sum + (fpga_rd_data << 8);
             sal_wrFPGA(ADDR_FPGA_MDIO_REG_TYPE, MDIO_BLK_HOST_SUM_MSB);
             fpga_rd_data = sal_rdFPGA(ADDR_FPGA_MDIO_REG_DATA);
             mdio_blk_rd_chk_sum = mdio_blk_rd_chk_sum + (fpga_rd_data << 16);
             if (mdio_blk_rd_chk_sum != mdio_blk_wr_chk_sum)
                dprintf("ERROR - Write check sum is 0x%X, but read back is 0x%X \r\n).",  mdio_blk_wr_chk_sum, mdio_blk_rd_chk_sum);
             else
                 dprintf("INFO -- Write check sum 0x%X matched read back check sum 0x%X \r\n).",  mdio_blk_wr_chk_sum, mdio_blk_rd_chk_sum);
         }
       } while (total_uP_code_wr < size);  // size is for 4-byte uP code
       
       //MDIO block readback check
       if (0) {
          //MDIO read back check
          addrmdio45(prtad, 0x04);
          writemdio45(prtad, 0x60);
          
          addrmdio45(prtad, 0x00);
          writemdio45(prtad, starting_address & 0x0000ffff);
          
          addrmdio45(prtad, 0x01);
          writemdio45(prtad, (starting_address & 0xffff0000)>>16);

          for  (loop_cnt = 0; loop_cnt < size/4; loop_cnt++) {
              addrmdio45(prtad, 0x02);
              data_0 = readmdio45(0, 0x1F);
              addrmdio45(prtad, 0x03);
              data_1 = readmdio45(0, 0x1F);
              if ((data_1 << 16) + data_0  != data[loop_cnt]) {
                 dprintf("ERROR - Write uP code 0x%X, but readback is 0x%X in location of 0x%0X\r\n).",  data[loop_cnt],  (data_1 << 16) + data_0, starting_address+loop_cnt);
                 mdio_blk_chk_cnt = 1001;  // return error
              }
          }
       }

       if (mdio_blk_chk_cnt > 1000) {
          dprintf("ERROR - MDIO blcok write times out.\r\n");
          return RR_ERROR;
       }
       else
          return RR_SUCCESS;       
    }
}

bool burst_write_i2c(uint32_t starting_address, uint32_t size, uint32_t* data) {
    uint8_t fpga_rd_data, i2c_ctrl_reg, i2c_rw_error = 0;
    uint32_t loop_cnt, total_uP_code_wr;
    char odata[I2C_FIFO_LENGTH*2];
    char idata[1];
    uint32_t ocnt = 0;
    uint32_t icnt = 0;
    uint32_t i2c_chk_cnt;
    
    fpga_rd_data = sal_rdFPGA(ADDR_FPGA_VERSION);
    if ((fpga_rd_data == 0) || (fpga_rd_data == 0xFF)) {
       //CAPI_DEBUG_PRINTF("FPGA version is illegal: %x\n", fpga_rd_data);
       dprintf("FPGA version is illegal: %x\r\n", fpga_rd_data);
       return RR_ERROR;
    }
    else if (fpga_rd_data == 0xD1)  {
       dprintf("FPGA version is 0x%X, and this FPGA does not support MDIO burst write\r\n", fpga_rd_data);
       return RR_ERROR;
    }
    else if ((fpga_rd_data == 0xD2) || (fpga_rd_data == 0xD3)) {
      /* dprintf("I2C burst write of %d-byte to starting_address 0x%X\n", size, starting_address);*/
      /* dprintf("The first word of the uP code is 0x%X\n", data[0]);*/
    }
    //Set QSFP page register to 0xFF
    //sal_writeI2c(0x50, 0x7F, 0xFF);

    dprintf("INFO: %d bytes of I2C burst write from addrss 0x%X\r\n", size, starting_address);
    
    //sal_wrFPGA(ADDR_FPGA_SCL_RATE, 0x1);     // set SCL to 100Khz
    //sal_wrFPGA(ADDR_FPGA_SCL_RATE, 0x2);     // set SCL to 400Khz
    sal_wrFPGA(ADDR_FPGA_I2C_CTRL, 0xFC);    //clear I2C state
    sal_wrFPGA(ADDR_FPGA_I2C_CTRL, 0x00);
    sal_wrFPGA(ADDR_FPGA_I2C_DEV_ADDR, DUT_I2C_ADDR);      // set I2C addr
    sal_wrFPGA(ADDR_FPGA_I2C_REG_TYPE, I2C_REG_QSFP_ADDR); // set QSFP ADDDR of 0x80 for IND ADDR0
    sal_wrFPGA(ADDR_FPGA_I2C_REG_DATA, 0x80);
    sal_wrFPGA(ADDR_FPGA_I2C_REG_TYPE, I2C_REG_IND_ADDR0); // set IND ADDR0
    sal_wrFPGA(ADDR_FPGA_I2C_REG_DATA, starting_address & 0xFF);
    sal_wrFPGA(ADDR_FPGA_I2C_REG_TYPE, I2C_REG_IND_ADDR1); // set IND ADDR1
    sal_wrFPGA(ADDR_FPGA_I2C_REG_DATA, (starting_address >> 8) & 0xFF);
    sal_wrFPGA(ADDR_FPGA_I2C_REG_TYPE, I2C_REG_IND_ADDR2); // set IND ADDR2
    sal_wrFPGA(ADDR_FPGA_I2C_REG_DATA, (starting_address >> 16) & 0xFF);
    sal_wrFPGA(ADDR_FPGA_I2C_REG_TYPE, I2C_REG_IND_ADDR3); // set IND ADDR3
    sal_wrFPGA(ADDR_FPGA_I2C_REG_DATA, (starting_address >> 24) & 0xFF);
    sal_wrFPGA(ADDR_FPGA_I2C_REG_TYPE, I2C_REG_LEN_LSB);   // set I2C burst write length
    sal_wrFPGA(ADDR_FPGA_I2C_REG_DATA, size & 0xFF);
    sal_wrFPGA(ADDR_FPGA_I2C_REG_TYPE, I2C_REG_LEN_MID);
    sal_wrFPGA(ADDR_FPGA_I2C_REG_DATA, (size >> 8) & 0xFF);
    sal_wrFPGA(ADDR_FPGA_I2C_REG_TYPE, I2C_REG_LEN_MSB);
    sal_wrFPGA(ADDR_FPGA_I2C_REG_DATA, (size >> 16) & 0xFF);
    sal_wrFPGA(ADDR_FPGA_I2C_REG_TYPE, I2C_REG_IND_CTRL); // set IND_CTRL
    sal_wrFPGA(ADDR_FPGA_I2C_REG_DATA, 0x03);

    // prepare first batch of data to write to DUT in burst mode
    total_uP_code_wr = 0;
    ocnt = 0;
    if (size >= I2C_FIFO_LENGTH) {
       for (loop_cnt = 0; loop_cnt < I2C_FIFO_LENGTH/4; loop_cnt++) {
           odata[ocnt] = 0xE0 | (ADDR_FPGA_I2C_DATA & 0x1F);
           odata[ocnt+2] = 0xE0 | (ADDR_FPGA_I2C_DATA & 0x1F);
           odata[ocnt+4] = 0xE0 | (ADDR_FPGA_I2C_DATA & 0x1F);
           odata[ocnt+6] = 0xE0 | (ADDR_FPGA_I2C_DATA & 0x1F);
           odata[ocnt+1] = data[loop_cnt] & 0XFF;
           odata[ocnt+3] = (data[loop_cnt] >> 8) & 0XFF;
           odata[ocnt+5] = (data[loop_cnt] >> 16) & 0XFF;
           odata[ocnt+7] = (data[loop_cnt] >> 24) & 0XFF;
           ocnt = ocnt + 8;
       }
       total_uP_code_wr = total_uP_code_wr + I2C_FIFO_LENGTH;
    }
    else {
       for (loop_cnt = 0; loop_cnt < size/4; loop_cnt++) {
           odata[ocnt] = 0xE0 | (ADDR_FPGA_I2C_DATA & 0x1F);
           odata[ocnt+2] = 0xE0 | (ADDR_FPGA_I2C_DATA & 0x1F);
           odata[ocnt+4] = 0xE0 | (ADDR_FPGA_I2C_DATA & 0x1F);
           odata[ocnt+6] = 0xE0 | (ADDR_FPGA_I2C_DATA & 0x1F);
           odata[ocnt+1] = data[loop_cnt] & 0XFF;
           odata[ocnt+3] = (data[loop_cnt] >> 8) & 0XFF;
           odata[ocnt+5] = (data[loop_cnt] >> 16) & 0XFF;
           odata[ocnt+7] = (data[loop_cnt] >> 24) & 0XFF;
           ocnt = ocnt + 8;
       }
       total_uP_code_wr = total_uP_code_wr + size;
    }
    sal_send2usb(ocnt, (unsigned char *) odata, icnt, (unsigned char *) idata);  //Fill out first I2C FIFO
   
    // prepare 2nd batch of data to write to DUT in burst mode
    ocnt = 0;
    if (size - total_uP_code_wr >= I2C_FIFO_LENGTH) {
       for (loop_cnt = 0; loop_cnt < I2C_FIFO_LENGTH/4; loop_cnt++) {
           odata[ocnt] = 0xE0 | (ADDR_FPGA_I2C_DATA & 0x1F);
           odata[ocnt+2] = 0xE0 | (ADDR_FPGA_I2C_DATA & 0x1F);
           odata[ocnt+4] = 0xE0 | (ADDR_FPGA_I2C_DATA & 0x1F);
           odata[ocnt+6] = 0xE0 | (ADDR_FPGA_I2C_DATA & 0x1F);
           odata[ocnt+1] = data[total_uP_code_wr/4 + loop_cnt] & 0XFF;
           odata[ocnt+3] = (data[total_uP_code_wr/4 + loop_cnt] >> 8) & 0XFF;
           odata[ocnt+5] = (data[total_uP_code_wr/4 + loop_cnt] >> 16) & 0XFF;
           odata[ocnt+7] = (data[total_uP_code_wr/4 + loop_cnt] >> 24) & 0XFF;
           ocnt = ocnt + 8;
       }
       total_uP_code_wr = total_uP_code_wr + I2C_FIFO_LENGTH;
       sal_send2usb(ocnt, (unsigned char *) odata, icnt, (unsigned char *) idata);  //Fill out 2nd I2C FIFO
    }
    else if (size - total_uP_code_wr > 0) {
       for (loop_cnt = 0; loop_cnt < size/4; loop_cnt++) {
           odata[ocnt] = 0xE0 | (ADDR_FPGA_I2C_DATA & 0x1F);
           odata[ocnt+2] = 0xE0 | (ADDR_FPGA_I2C_DATA & 0x1F);
           odata[ocnt+4] = 0xE0 | (ADDR_FPGA_I2C_DATA & 0x1F);
           odata[ocnt+6] = 0xE0 | (ADDR_FPGA_I2C_DATA & 0x1F);
           odata[ocnt+1] = data[total_uP_code_wr/4 + loop_cnt] & 0XFF;
           odata[ocnt+3] = (data[total_uP_code_wr/4 + loop_cnt] >> 8) & 0XFF;
           odata[ocnt+5] = (data[total_uP_code_wr/4 + loop_cnt] >> 16) & 0XFF;
           odata[ocnt+7] = (data[total_uP_code_wr/4 + loop_cnt] >> 24) & 0XFF;
           ocnt = ocnt + 8;
       }
       total_uP_code_wr = total_uP_code_wr + size;
       sal_send2usb(ocnt, (unsigned char *) odata, icnt, (unsigned char *) idata);  //Fill out 2nd I2C FIFO
    }

    i2c_ctrl_reg = I2C_MODE_BRCM*16 +0x2;  //i2c_fifo_in_sel to FIFO_0 first
    sal_wrFPGA(ADDR_FPGA_I2C_CTRL, i2c_ctrl_reg);  //start I2C write

    do {
       //polling_i2c_FIFO_availabe
       i2c_chk_cnt = 0;
       do {
          fpga_rd_data = sal_rdFPGA(ADDR_FPGA_I2C_STATUS);
          i2c_chk_cnt++;
          if (i2c_chk_cnt > 10000) {
             break;
          }
       } while ((fpga_rd_data & 3) == 0);
       if (i2c_chk_cnt > 10000) {
         i2c_rw_error++;
         break;
       }
       ocnt = 0;
       if (size - total_uP_code_wr >= I2C_FIFO_LENGTH) {
          for (loop_cnt = 0; loop_cnt < I2C_FIFO_LENGTH/4; loop_cnt++) {
              odata[ocnt] = 0xE0 | (ADDR_FPGA_I2C_DATA & 0x1F);
              odata[ocnt+2] = 0xE0 | (ADDR_FPGA_I2C_DATA & 0x1F);
              odata[ocnt+4] = 0xE0 | (ADDR_FPGA_I2C_DATA & 0x1F);
              odata[ocnt+6] = 0xE0 | (ADDR_FPGA_I2C_DATA & 0x1F);
              odata[ocnt+1] = data[total_uP_code_wr/4 + loop_cnt] & 0XFF;
              odata[ocnt+3] = (data[total_uP_code_wr/4 + loop_cnt] >> 8) & 0XFF;
              odata[ocnt+5] = (data[total_uP_code_wr/4 + loop_cnt] >> 16) & 0XFF;
              odata[ocnt+7] = (data[total_uP_code_wr/4 + loop_cnt] >> 24) & 0XFF;
              ocnt = ocnt + 8;
          }
          total_uP_code_wr = total_uP_code_wr + I2C_FIFO_LENGTH;
          sal_send2usb(ocnt, (unsigned char *) odata, icnt, (unsigned char *) idata);  //Fill out 2nd I2C FIFO
       }
       else if (size - total_uP_code_wr > 0) {
          for (loop_cnt = 0; loop_cnt < (size - total_uP_code_wr)/4; loop_cnt++) {
              odata[ocnt] = 0xE0 | (ADDR_FPGA_I2C_DATA & 0x1F);
              odata[ocnt+2] = 0xE0 | (ADDR_FPGA_I2C_DATA & 0x1F);
              odata[ocnt+4] = 0xE0 | (ADDR_FPGA_I2C_DATA & 0x1F);
              odata[ocnt+6] = 0xE0 | (ADDR_FPGA_I2C_DATA & 0x1F);
              odata[ocnt+1] = data[total_uP_code_wr/4 + loop_cnt] & 0XFF;
              odata[ocnt+3] = (data[total_uP_code_wr/4 + loop_cnt] >> 8) & 0XFF;
              odata[ocnt+5] = (data[total_uP_code_wr/4 + loop_cnt] >> 16) & 0XFF;
              odata[ocnt+7] = (data[total_uP_code_wr/4 + loop_cnt] >> 24) & 0XFF;
              ocnt = ocnt + 8;
          }
          total_uP_code_wr = total_uP_code_wr + size;
          sal_send2usb(ocnt, (unsigned char *) odata, icnt, (unsigned char *) idata);  //Fill out 2nd I2C FIFO
       }
    } while (total_uP_code_wr < size);
    
    if (i2c_chk_cnt > 10000)
       i2c_rw_error++;
    else {
       //polling_fpga_i2c_done
       i2c_chk_cnt = 0;
       do {
          delay_ms(10);
          fpga_rd_data = sal_rdFPGA(ADDR_FPGA_I2C_STATUS);
          i2c_chk_cnt++;
          if (i2c_chk_cnt > 1000) {
             break;
          }
       } while ((fpga_rd_data & 0x80) == 0);  // D7 = i2c_rw_done
       if (i2c_chk_cnt > 1000) {
          dprintf("ERROR - I2C blcok write times out.");
          i2c_rw_error++;
       }
       else {
          if ((fpga_rd_data & 0x40) == 0x40) {
             dprintf("ERROR - I2C blcok write has error.");
             i2c_rw_error++;
          }
       }
    }
    sal_wrFPGA(ADDR_FPGA_I2C_CTRL, 0x00);
    if (i2c_rw_error == 0) {
       //dprintf("I2C burst write of %d-byte to starting_address 0x%X is done successfully\n", size, starting_address);
       return RR_SUCCESS;
    }
    else
       return RR_ERROR;

}

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
return_result_t burst_read_chip_sram(uint32_t starting_address, uint32_t size, uint32_t* data, long port) {
    return (sal_readEstInc(port, starting_address, size, data) == 0) ? RR_SUCCESS : RR_ERROR_MDIO;
}

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
return_result_t burst_write_chip_sram(uint32_t starting_address, uint32_t size, uint32_t* data, long port) {
    return (sal_writeEstInc(port, starting_address, size, data) == 0) ? RR_SUCCESS : RR_ERROR_MDIO;
}


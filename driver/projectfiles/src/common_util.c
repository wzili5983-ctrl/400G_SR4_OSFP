/**
 *
 * @file     common_util.c
 * @author   Firmware Team
 * @date     04/04/2018
 * @version  1.0
 *
 * @property $ Copyright: (c) 2018 Broadcom Limited All Rights Reserved $
 *           No portions of this material may be reproduced in any form
 *           without the written permission of: 
 *           Broadcom Limited
 *           1320 Ridder Park Drive
 *           San Jose, California 95131
 *           United States
 * All information contained in this document/file is Broadcom Limit company
 * private proprietary, trade secret, and remains the property of Broadcom
 * Limited. The intellectual and technical concepts contained herein are
 * proprietary to Broadcom Limited and may be covered by U.S. and Foreign Patents,
 * patents in process, and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material is strictly
 * forbidden unless prior written permission is obtained from Bloadcom Limited.
 *
 * @brief   This file includes all the common utilities
 *
 * @section
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "regs_common.h"
#include "access.h"
#include "common_def.h"
#include "common_util.h"


/**
 * @brief      util_memset(void *src_ptr, int content, size_t count)
 * @details    This utility function is created to initialize the memory of address src_ptr
 *             with the content value for the length of count
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  src_ptr: pointer to the source address
 * @param[in]  content: the override value
 * @param[in]  count:   the number of the bytes to be overriden
 * 
 * @return     returns the initial address of the srouce
 */
void* util_memset(void *src_ptr, int content, size_t count)
{
    char *xsrc_ptr = (char *)src_ptr;

    while (count--)
        *xsrc_ptr++ = content;

    return(src_ptr);
}

/**
 * @brief      util_memcpy(void *dest_ptr, void *src_ptr, size_t count)
 * @details    This utility function is created to initialize the memory of address src_ptr
 *             with the content value for the length of count
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  dest_ptr: pointer to the destination address
 * @param[in]  src_ptr: pointer to the source address
 * @param[in]  count:   the number of the bytes to be overriden
 * 
 * @return     returns void
 */
void util_memcpy(void *dest_ptr, void *src_ptr, size_t count)
{
   unsigned int i=0;
   char* xdest_ptr = (char *)dest_ptr;
   char* xsrc_ptr = (char *)src_ptr;
 
   while (i++<count)
       *xdest_ptr++ = *xsrc_ptr++;
}

/**
 * @brief      util_memcmp(void *dest_ptr, void *src_ptr, size_t count)
 * @details    This utility function is created to compare src_ptr
 *             with the content value with dest_ptr content for the length of count
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  dest_ptr: pointer to the destination address
 * @param[in]  src_ptr: pointer to the source address
 * @param[in]  count:   the number of the bytes to be overriden
 * 
 * @return     returns void
 */
unsigned int util_memcmp(void *dest_ptr, void *src_ptr, size_t count)
{
   unsigned int i=0, err_cnt=0;
   char* xdest_ptr = (char *)dest_ptr;
   char* xsrc_ptr = (char *)src_ptr;
 
   while (i++<count){
       if(*xdest_ptr != *xsrc_ptr){
           err_cnt++;
       }
       xdest_ptr++;
       xsrc_ptr++;
   }
   return err_cnt;
}
uint32_t fw_read_package_id(void)
{
    uint32_t package_id = 0;
    uint32_t pkg_id_msb, pkg_id_lsb;

    ERR_HSIP(pkg_id_msb = rd_field_(OCTAL_TOP_REGS, QUAD_CORE_CONFIG_CFG_PKGID_1_RDB, CFG_PKGID_19_16, 0));
    ERR_HSIP(pkg_id_lsb = rd_reg_(OCTAL_TOP_REGS, QUAD_CORE_CONFIG_CFG_PKGID_0_RDB, 0));
    package_id = pkg_id_msb << 16 | pkg_id_lsb;

    return package_id;
}

uint32_t fw_read_chip_id(void)
{
    uint32_t chip_id = 0;
    uint32_t chip_id_msb, chip_id_lsb;

    ERR_HSIP(chip_id_msb = rd_field_(OCTAL_TOP_REGS, QUAD_CORE_CONFIG_CFG_CHIPID_1_RDB, CFG_CHIPID_19_16, 0));
    ERR_HSIP(chip_id_lsb = rd_reg_(OCTAL_TOP_REGS, QUAD_CORE_CONFIG_CFG_CHIPID_0_RDB, 0));
    chip_id = chip_id_msb << 16 | chip_id_lsb;

    return chip_id;

}

uint8_t fw_read_rev_id(void)
{
    uint32_t rev_id = 0;

    ERR_HSIP(rev_id = rd_field_(OCTAL_TOP_REGS, QUAD_CORE_CONFIG_CFG_CHIPID_1_RDB, CFG_REVID, 0));

    return (uint8_t)rev_id;

}

long signext(long val, long msb_mask)
{
    return (val - ((val & msb_mask) << 1));
}

/**
 * @brief  wr_reg_field(long address, long msb, long lsb, long data, long port)
 *
 * write regiser field funciton
 *
 * @param address 32 bit register address
 * @param  msb  field msb bit position
 * @param  lsb  field lsb bit position
 * @param data register field written value
 * @param  port  MDIO i2c device ID
 * @return none
 */
return_result_t wr_reg_field(long address, long msb, long lsb, long data, long port)
{
    long reg_val;
    phy_info_t phy_info;
    phy_info.base_addr = 0;
    phy_info.phy_id    = port;

    ERR_HSIP(reg_val = hsip_rd_reg_(&phy_info, address));
    reg_val &= ~(((1 << (msb - lsb + 1)) - 1) << lsb);
    reg_val |= (((long)((data) & ((1 << (msb - lsb + 1)) - 1))) << lsb);
    hsip_wr_reg_(&phy_info, address, reg_val);

    return RR_SUCCESS;
}




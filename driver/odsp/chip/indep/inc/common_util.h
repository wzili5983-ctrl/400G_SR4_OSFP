/**
 *
 * @file     common_util.h
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
#ifndef COMMON_UTIL_H
#define COMMON_UTIL_H


#ifdef __cplusplus
extern "C" {
#endif



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
void* util_memset(void *src_ptr, int content, size_t count);

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
void util_memcpy(void *dest_ptr, void *src_ptr, size_t count);

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
unsigned int util_memcmp(void *dest_ptr, void *src_ptr, size_t count);
uint32_t fw_read_package_id(void);

uint32_t fw_read_chip_id(void);

uint8_t fw_read_rev_id(void);

long signext(long val, long msb_mask);

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
return_result_t wr_reg_field(long address, long msb, long lsb, long data, long port);




#ifdef __cplusplus
}
#endif

#endif /**< COMMON_UTIL_H */

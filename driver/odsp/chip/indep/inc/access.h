/****************************************************************************
*
*     Copyright (c) 2016 Broadcom Limited
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom Limited 
*           1320 Ridder Park Dr.
*			San Jose, California 95131
*			United States
*
*     All information contained in this document is Broadcom Limited 
*     company private, proprietary, and trade secret.
*
****************************************************************************/
/**
 *        Access header file 
 *
 * @file access.h
 * @author firmware team
 * @date 12-6-2015
 * @version 1.0
 * @brief This header file includes register access header 

*/
#ifndef ACCESS_H
#define ACCESS_H

#define FW_DEFAULT_PORTAD 0 /**< default PORTAD for register access */

#define __HSIP_ERR  &__hsip_err

/* register access definition switch between firmware and external host */

#ifndef __linux
#include "reg_access.h"
#include "logger.h"
#else
#include "centenario_reg_access.h"
#endif /* LINUX_CLI */ 

#define writereg32(a, d, port)                        wr_reg_ex(a, d, port)
#define readreg32(a, port)                            rd_reg_ex(a, port)

#define writereg16(a, d, port)                        wr16_reg_ex(a, d, port)
#define readreg16(a, port)                            rd16_reg_ex(a, port)

#define writereg8(a, d, port)                         wr8_reg_ex(a, d, port)
#define readreg8(a, port)                             rd8_reg_ex(a, port)

#define writereg8_qsfp(a, d, port)                    wr_qsfp_reg_ex(a, d, port)
#define readreg8_qsfp(a, port)                        rd_qsfp_reg_ex(a, port)

#define WriteReg32(a, d, port)                        wr_reg_ex(a, d, port)
#define ReadReg32(a, port)                            rd_reg_ex(a, port)


/* Register access definition switch between FPGA and chip */
#ifdef PLATFORM_SIMULATION

#define rd_reg16_(bbAddr, REG, port) \
        (dprintf("rd_reg16_ %s  val 0x%x \r\n", REG))
#define wr_reg16_(bbAddr, REG, value, port) \
        (dprintf("wr_reg16_ %s  val 0x%x \r\n", REG, value))
#define rd_reg_(bbAddr, REG, port) \
        (dprintf("rd_reg_ %s  val 0x%x \r\n", REG))
#define wr_reg_(bbAddr, REG, value, port) \
        (dprintf("wr_reg_ %s  val 0x%x \r\n", REG, value))
#define update_reg_(bbAddr, REG, value, port) \
        (dprintf("update_reg_ %s  val 0x%x \r\n", REG, value))

#define rd_field_(bbAddr, REG, FIELD, port) \
        (dprintf("rd_field_ %s  \r\n", REG##_##FIELD##_MASK))
#define wr_field_(bbAddr, REG, FIELD, value, port) \
        (dprintf("wr_field_ %s  val 0x%x \r\n", REG##_##FIELD##_MASK, value))

#define hsip_rd_reg_(phy_info_ptr, REG) \
        (dprintf("hsip_rd_reg_ %s  val 0x%x \r\n", REG))
#define hsip_wr_reg_(phy_info_ptr, REG, value)  \
        (dprintf("hsip_wr_reg_ %s  val 0x%x \r\n", REG, value))
#define hsip_update_reg_(phy_info_ptr, REG, value) \
        (dprintf("hsip_update_reg_ %s  val 0x%x \r\n", REG, value))

#define hsip_rd_field_(phy_info_ptr, REG, FIELD) \
        (rd_field_ex(REG##_##FIELD##_MASK))
#define hsip_wr_field_(phy_info_ptr, REG, FIELD, value) \
        (dprintf("hsip_wr_field_ %s  val 0x%x \r\n", REG##_##FIELD##_MASK, value))

#else /* PLATFORM_SIMULATION */

#define readreg32field(REG, SHIFT, MASK, port)                 (readreg32(REG, port) & (MASK)) >> (SHIFT)
#define writereg32field(REG, SHIFT, MASK, VALUE, port)         (writereg32(REG, ((readreg32(REG, port) & (~(MASK))) | (((VALUE) << (SHIFT)) & (MASK))), port))

#define rd_reg16_(bbAddr, REG, port)                           (readreg16((bbAddr) + (REG), port))
#define wr_reg16_(bbAddr, REG, value, port)                    (writereg16(((bbAddr) + (REG)), value, port))
#define rd_reg_(bbAddr, REG, port)                             (readreg32((bbAddr) + (REG), port))
#define wr_reg_(bbAddr, REG, value, port)                      (writereg32(((bbAddr) + (REG)), value, port))
#define update_reg_(bbAddr, REG, value, port)                  (wr_reg_(bbAddr, REG, value, port))

#define rd_field_(bbAddr, REG, FIELD, port)                    (readreg32field(((bbAddr) + (REG)), REG##_##FIELD##_SHIFT, REG##_##FIELD##_MASK, port))
#define wr_field_(bbAddr, REG, FIELD, value, port)             (writereg32field(((bbAddr) + (REG)), REG##_##FIELD##_SHIFT, REG##_##FIELD##_MASK, value, port))

#define hsip_rd_reg_(phy_info_ptr, REG)                        rd_reg_((phy_info_ptr)->base_addr, REG, (phy_info_ptr)->phy_id)
#define hsip_wr_reg_(phy_info_ptr, REG, value)                 wr_reg_((phy_info_ptr)->base_addr, REG, value, (phy_info_ptr)->phy_id)
#define hsip_update_reg_(phy_info_ptr, REG, value)             update_reg_((phy_info_ptr)->base_addr, REG, value, (phy_info_ptr)->phy_id)

#define hsip_rd_field_(phy_info_ptr, REG, FIELD)               (readreg32field((((phy_info_ptr)->base_addr) + (REG)), REG##_##FIELD##_SHIFT, REG##_##FIELD##_MASK, (phy_info_ptr)->phy_id))
#define hsip_wr_field_(phy_info_ptr, REG, FIELD, value)        (writereg32field((((phy_info_ptr)->base_addr) + (REG)), REG##_##FIELD##_SHIFT, REG##_##FIELD##_MASK, value, (phy_info_ptr)->phy_id))

#endif /* PLATFORM_SIMULATION */

#define set_field32(reg_value, shift, mask, field_value, port)       (reg_value = (((reg_value) & ~(mask)) | (((field_value) << (shift)) & (mask))))
#define set_field_(bbaddr, REG, FIELD, reg_value, field_value, port) (set_field32(reg_value, REG##_##FIELD##_SHIFT, REG##_##FIELD##_MASK, field_value, port))        

#define hsip_set_field_(phy_info_ptr, REG, FIELD, REG_VALUE, FIELD_VALUE) \
        (set_field32(REG_VALUE, REG##_##FIELD##_SHIFT, REG##_##FIELD##_MASK, FIELD_VALUE, phy_info_ptr))
#define hsip_wr_reg16_(phy_info_ptr, REG, value)  \
        wr_reg16_((phy_info_ptr)->base_addr, REG, value, (phy_info_ptr)->phy_id)
#define hsip_rd_reg16_(phy_info_ptr, REG) \
        rd_reg16_((phy_info_ptr)->base_addr, REG, (phy_info_ptr)->phy_id)

#define ERR_HSIP(expr)  \
    do  { \
        int __hsip_err; \
        *(__HSIP_ERR) = 0; \
        (expr); \
        if (*(__HSIP_ERR) != 0) \
        {   return  RR_ERROR_MDIO; }\
        (void)__hsip_err; \
    }   while(0)

#define ERR_HSIP_NO_CHK(expr)  \
    do  { \
        int __hsip_err; \
        *(__HSIP_ERR) = 0; \
        (expr); \
        (void)__hsip_err; \
    }   while(0)

/**
 * @brief rd_fields(bbaddr,reg,fmask,port)
 *
 * @details Interface function read multi-feild value specified the mask from specified register   <BR>
 *              (each IP block's base address, register name inside of each IP block)
 * @public   
 * @private 
 * @param[in]  bbaddr device base address 
 * @param[in]  reg       register token which is a combination of IP block name + register name.   <BR>
 *                                This token shall be defined as the register offset address in the related header file.
 * @param[in]  fmask   bit field mask to indicate the operation field.    <BR>
 *                                Such as: 0x00000F0E indicate the operating field of field_val will be bit [11:8] and bit[3:1]
 * @param[in]  port      8bit parameter for general purpose
 * @return  specified multi-feild value
 */
#define rd_fields(bbaddr, reg, fmask, port) (readreg32((bbaddr) + (reg), port) & (fmask))
#define hsip_rd_fields(phy_info_ptr, reg, fmask)  \
        rd_fields((phy_info_ptr)->base_addr, reg, fmask, (phy_info_ptr)->phy_id) 

/**
 * @brief wr_fields(bbaddr,reg,fmask,fval,port)
 *
 * @details Interface function writes multi-field value into specified register    <BR>
 *              (each IP block's base address, register name inside of each IP block)
 * @public   
 * @private 
 * @param[in]  bbaddr device base address 
 * @param[in]  reg       register token which is a combination of IP block name + register name.   <BR>
 *                                This token shall be defined as the register offset address in the related header file.
 * @param[in]  fmask   bit field mask to indicate the operation field.    <BR>
 *                                Such as: 0x00000F0E indicate the operating field of field_val will be bit [11:8] and bit[3:1]
 * @param[in] fval       field value to be stored into register.                 <BR>
 *                               Please make sure the field value should be put at the correlated location as field_mask.  <BR>
 *                               Such as: field_value = 0x87654321 & field_mask = 0x00000F0E, then register bit 11~8 will be updated to 0x04,  <BR>
 *                               and bit 3~1 will be updated 0. Other bits of field_value will be discarded and be kept as original read-out field value.
 * @param[in]  port      8bit parameter for general purpose
 * @return  void
 */
#define wr_fields(bbaddr, reg, fmask, fval, port) (writereg32((bbaddr) + (reg), ((readreg32((bbaddr) + (reg), port) & (~(fmask))) | ((fval) & (fmask))), port))
#define get_field_(bbaddr, REG, FIELD, reg_value, port) (((reg_value)&REG##_##FIELD##_MASK) >> REG##_##FIELD##_SHIFT)

#define hsip_wr_fields(phy_info_ptr, reg, fmask, fval) \
        wr_fields((phy_info_ptr)->base_addr, reg, fmask, fval, (phy_info_ptr)->phy_id)
#define hsip_get_field_(phy_info_ptr, REG, FIELD, reg_value) \
        (((reg_value)&REG##_##FIELD##_MASK) >> REG##_##FIELD##_SHIFT)

/**
 * @brief rd_field(bbaddr, addr, msb, lsb)
 *
 * read specified field <msb, lsb> from specified register 
 *
 * @param  bbaddr: device information (chan_num, sys_or_line, inf_type)
 * @param  addr: register physical address
 * @param  msb: MSB of the field
 * @param  lsb: LSB of the field
 * @return the specified field stored value as unsigned integer value [(msb-lsb) �0]
 */
#define up_mask(msb) ((1 << ((msb) + 1)) - 1)
#define msbmask(msb) (1 << (msb))

#define rd_field(bbaddr, reg, msb, lsb, port) ((rd_reg_(bbaddr, reg, port) & up_mask(msb)) >> (lsb))

#define hsip_rd_field(phy_info_ptr, reg, msb, lsb) \
        rd_field((phy_info_ptr)->base_addr, reg, msb, lsb, (phy_info_ptr)->phy_id)

/**
 * @brief rd_field_signed(bbaddr, addr, msb, lsb)
 *
 * read specified field <msb, lsb> from specified register as integer value 
 *
 * @param  bbaddr: device information (chan_num, sys_or_line, inf_type)
 * @param  addr: register physical address
 * @param  msb: MSB of the field
 * @param  lsb: LSB of the field
 * @return the specified field stored value as signed integer value [(msb-lsb) � 0]
 */
#define rd_field_signed(bbaddr, addr, msb, lsb, port) (((msb) == 15) ? (((int16_t)(rd_field(bbaddr, addr, msb, 0, port))) >> (lsb))                 \
                                                                     : ((((msb) == 7) ? (((int8_t)(rd_field(bbaddr, addr, msb, 0, port))) >> (lsb)) \
                                                                                      : ((signext(rd_field(bbaddr, addr, msb, 0, port), msbmask(msb))) >> (lsb)))))

#define wr_field(bbaddr, addr, msb, lsb, val, port)           \
    do {                                                      \
        wr_reg_field((bbaddr) + (addr), msb, lsb, val, port); \
    } while (0)


#define hsip_rd_field_signed(phy_info_ptr, addr, msb, lsb) \
        rd_field_signed((phy_info_ptr)->base_addr, addr, msb, lsb, (phy_info_ptr)->phy_id)
#define hsip_wr_field(phy_info_ptr, addr, msb, lsb, val) \
        wr_field((phy_info_ptr)->base_addr, addr, msb, lsb, val, (phy_info_ptr)->phy_id)

long signext(long val, long msb_mask);

#endif /* ACCESS_H */


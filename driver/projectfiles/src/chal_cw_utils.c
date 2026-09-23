/**
 *
 * @file       chal_cw_utils.c
 * @author     HSIP Firmware Team
 * @date       03/25/2022
 * @version    1.0
 *
 * @property 
 * $Copyright: (c) 2022 Broadcom.
 *             Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * @brief      The CAPI files contain all the CAPIs used by the the module and line card products.
 *
 * @section  description
 *
 */
#include "regs_common.h"
#include "chal_cw_utils.h"

uint8_t get_num_bits (uint16_t data)
{
    uint8_t i, num_bits = 0;

    for (i=0; i<16; ++i)
    {    
        if ((data & (1 << i)) != 0)
            num_bits++;
    }

    return num_bits;
}  

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief      dup2_even - duplicte even bit
 * @details    b0 > b0, b1; b2 > b2, b3; b4 > b4, b5; b6 > b6, b7
 * @public     any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 * @param[in]  ln_in : ln_in
 * @return uint16_t
 */
uint16_t dup2_even (uint16_t ln_in)
{
    uint16_t dup2_even = 0x0;
    uint16_t tmp;
    int i;

    for (i=0; i<4; ++i)
    {    
        tmp = ln_in & (0x1 << 2*i);
        dup2_even |= tmp | tmp << 1;
    }
    return dup2_even;
}  

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief      duplicate_2 - duplicate every bit two times
 * @details    b0 > b0, b1; b1 > b2, b3; .. b7 > b14, b15
 * @public     any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 * @param[in]  ln_in : ln_in
 * @return uint16_t
 */
uint16_t duplicate_2(uint16_t ln_in)
{
    uint16_t dup2 = 0x0;
    uint16_t tmp;
    int i;

    for (i=0; i<8; ++i)
    {    
        tmp = ln_in & (0x1 << i);
        dup2 |= tmp << i | tmp << (i+1);
    }
    return dup2;
}  


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief      dup4_even - duplicte even bit four times
 * @details    b0 > b[3:0]; b2> b[7:4]; b4 > b[11:8]; b6 > b[15:12]
 * @public     any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 * @param[in]  ln_in : ln_in
 * @return uint16_t
 */
uint16_t dup4_even (uint16_t ln_in)
{
    uint16_t dup4_even = 0x0;
    uint16_t tmp;
    int i;

    for (i=0; i<4; ++i)
    {    
        tmp = ln_in & (0x1 << 2*i);
        dup4_even |= tmp << (2*i) | tmp << (2*i+1) | tmp << (2*i+2) | tmp << (2*i+3);
    }
    return dup4_even;
}  


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief      dup4 - duplicate every bit four times
 * @details    b0 > b[3:0], b1 > b[7:4]
 * @public     any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 * @param[in]  ln_in : ln_in
 * @return uint16_t
 */
uint16_t dup4 (uint16_t ln_in)
{
    uint16_t dup4 = 0x0;
    uint16_t tmp;
    int i;

    for (i=0; i<2; ++i)
    {    
        tmp = ln_in & (0x1 << i);
        dup4 |= tmp << (3*i) | tmp << (3*i+1) | tmp << (3*i+2) | tmp << (3*i+3);
    }
    return dup4;
}

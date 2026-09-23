/**
 *
 * @file       chal_cw_utils.h
 * @author     PLP Firmware Team
 * @date       03/24/2022
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
#ifndef CHAL_CW_UTILS_H
#define CHAL_CW_UTILS_H
 
#ifdef __cplusplus
extern "C" {
#endif
 
uint8_t get_num_bits (uint16_t data);

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
uint16_t dup2_even (uint16_t ln_in);

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
uint16_t duplicate_2(uint16_t ln_in);

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
uint16_t dup4_even(uint16_t ln_in);

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
uint16_t dup4(uint16_t ln_in);

#ifdef __cplusplus
}
#endif

#endif /* CHAL_CW_UTILS_H */

/**
 *
 * @file common_def.h
 * @author  
 * @version 1.0
 *
 * @property   $Copyright: Copyright 2018 Broadcom INC. 
 *This program is the proprietary software of Broadcom INC
 *and/or its licensors, and may only be used, duplicated, modified
 *or distributed pursuant to the terms and conditions of a separate,
 *written license agreement executed between you and Broadcom
 *(an "Authorized License").  Except as set forth in an Authorized
 *License, Broadcom grants no license (express or implied), right
 *to use, or waiver of any kind with respect to the Software, and
 *Broadcom expressly reserves all rights in and to the Software
 *and all intellectual property rights therein.  IF YOU HAVE
 *NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE
 *IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE
 *ALL USE OF THE SOFTWARE.
 *
 *Except as expressly set forth in the Authorized License,
 *
 *1.     This program, including its structure, sequence and organization,
 *constitutes the valuable trade secrets of Broadcom, and you shall use
 *all reasonable efforts to protect the confidentiality thereof,
 *and to use this information only in connection with your use of
 *Broadcom integrated circuit products.
 *
 *2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS
 *PROVIDED "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
 *REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
 *OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
 *DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
 *NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
 *ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 *CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
 *OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 *3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL
 *BROADCOM OR ITS LICENSORS BE LIABLE FOR  CONSEQUENTIAL,
 *INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER
 *ARISING OUT OF OR IN ANY WAY RELATING TO YOUR USE OF OR INABILITY
 *TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF THE
 *POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF
 *THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR USD 1.00,
 *WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING
 *ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.$
 *
 * @brief   this file includes cHAL function implementation.
 *
 * @section
 * 
 */


/**
 *       chip common header file
 *
 * @file common_def.h
 * @author $Id$
 * @date 6-28-2018
 * @brief This file includes common type defines
 */

#ifndef COMMON_DEF_H
#define COMMON_DEF_H
#include "type_defns.h"
#ifdef __cplusplus
extern "C" {
#endif

#if 0
#ifndef BURNIN_DVT_TEST
#define BURNIN_DVT_TEST                              /**< Flag to support DVT Burning Test */
#endif
#endif
#define ATE_DR1_TEST                                   /**< Flag to support ATE DR1 Test */


#define SQUELCH                        0               /**< Client Squelch  0   */
#define UN_SQUELCH                     1               /**< Client Squelch  1   */

#define TURN_OFF                       0               /**< Turn Off            */
#define TURN_ON                        1               /**< Turn On             */
       
#define RESET_RELEASE                  0               /**< Reset RElease       */
#define RESET                          1               /**< Reset               */

#define NIBBLE_SHIFT                   8               /**< 8 bits shift to get upper nibble      */
#define UPPER_NIBBLE_MASK         0xFF00               /**< upper nibble mask                     */
#define LOWER_NIBBLE_MASK           0xFF               /**< lower nibble mask                     */

/**
 * The enumeration of Core IPs
 */
typedef enum core_ip_e {
    CORE_IP_ALL = 0,                                    /*!< Core IP ALL                          */
    CORE_IP_CW,                                         /*!< Core IP Core Wrapper                 */
    CORE_IP_MEDIA_DSP,                                  /*!< Core IP Media Side Handled by DSP    */
    CORE_IP_MEDIA_SERDES,                               /*!< Core IP Media Side Handled by SERDES */
    CORE_IP_HOST_DSP,                                   /*!< Core IP Host Side Handled by DSP     */
    CORE_IP_HOST_SERDES,                                /*!< Core IP Host Side Handled by SERDES  */
    CORE_IP_KP4_KR4_FEC_DEC,                            /*!< Core IP KP4, KR4 Encoder Decoder     */
    CORE_IP_AVS,                                        /*!< Core IP Avs                          */
    CORE_IP_MAX
} core_ip_t;                                            /*!< Core IP type                         */

/**
 * A structure to represent Phy information
 */
typedef struct phy_info_s {
    uint32_t  phy_id;                                   /**< This element identifies the Phy Id */
    core_ip_t core_ip;                                  /**< This element identifies the core IP (FW, LW, DW, etc. ) */
    uint32_t  lane_mask;                                /**< This element identifiesspecific lanes bitmap: */
                                                        /**< Represents the Lane mapping of a port */
                                                        /**< LSB Bit 0 represents lane 0 */
                                                        /**< LSB Bit 1 represents lane 1 */
                                                        /**< Fore instance: */
                                                        /**< 0x3 represents lane 0 and 1 */
                                                        /**< 0xF represents lane 0 to lane 3 */
    void*     ref_ptr;                                  /**< Courtesy pointer for the API client to use */
    uint32_t  base_addr;                                /**< This element identifies the register's base address */
    uint32_t  chip_id;                                  /**< When chip ID is populated by upper layer, cAPI functions
                                                             can use it for enabling chip specific functionality without
                                                             reading the register (for better performance) */
   
    uint8_t   i2c_address;                              /**< this is for setting i2c slave address to differen one, 
                                                             rather than default 0x50. By default, this should be 0x0 */    
    uint32_t  i2c_block_write_size;                     /**< this is for fast spi program with i2c block write,
                                                             i2c_block_write_size must be divisable by 256 bytes */
} phy_info_t; /**< Phy Information Type */

/**
 * The enumeration of enabled type
 */
typedef enum enabled_e {
    IS_DISABLED = 0,       /*!< operation disable   */
    IS_ENABLED  = 1        /*!< operation enable    */
} enabled_t;               /*!< Enable/Disable type */

/**
 * The enumeration of assert type
 */
typedef enum assert_e {
    RST_DEASSERTED  = 0,    /*!< reset de-assert    */
    RST_ASSERTED = 1        /*!< reset assert   */
} rst_assert_t;                /*!< Assert/Deassert type */

/**
 * The enumeration to choose upper or lower nibble
 */
typedef enum nibble_e {
    LOWER_NIBBLE = 0,      /**< Use lower nibble bits */
    UPPER_NIBBLE = 1       /**< Use upper nibble bits */
} nibble_t;

#ifdef __cplusplus
}
#endif

#endif /*COMMON_DEF_H*/


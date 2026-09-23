/**
 *
 * @file     capi_fw_intf.h
 * @author   
 * @date     10/28/2018
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
 * @brief   This file contains the definitions of CAPI-firmware interface
 *
 * @section
 * 
 */
#ifndef CAPI_FW_INTF_H
#define CAPI_FW_INTF_H

//#include "common_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Same as phy_info_t
 
typedef phy_info_t capi_phy_info_t;
*/

/**
 * The enumeration of user command
 */
typedef enum capi_fw_cmd_e {
    CAPI_FW_CMD_SET_ALL_MODES = 0x0,      /**< Set All Modes (Apply globally, Set lane, Blackhawk and line modes together) */
    CAPI_FW_CMD_SET_PER_PLL_MODES = 0x1,  /**< Set All Modes (Apply globally, apply on per PLL range, such as: lane0 & lane1; lane2 & lane3*/

    CAPI_FW_CMD_SET_BLACKHAWK_MODE = 0x3, /**< Set Blackhawk Mode (Apply only to Blackhawk Side) */
    CAPI_FW_CMD_SET_LINE_MODE = 0x4,      /**< Set Line Mode (Apply only to Line side) */

    CAPI_FW_CMD_ASSERT_RESET_BLACKHAWK_LANE = 0x7,   /**< Assert Reset Blackhawk Lane (Rx & Tx) */
    CAPI_FW_CMD_RELEASE_RESET_BLACKHAWK_LANE = 0x8,  /**< Release Reset Blackhawk Lane (Rx & Tx) */
    CAPI_FW_CMD_ASSERT_RESET_LINE_LANE = 0x9,        /**< Assert Reset Line Lane (Rx & Tx) */
    CAPI_FW_CMD_RELEASE_RESET_LINE_LANE = 0xA,       /**< Release Reset Line Lane (Rx & Tx) */

    CAPI_FW_CMD_READ_STATUS_BLACKHAWK_LANE = 0xB,    /**< Read Status Blackhawk Lane (LOS, CDR Lock, PLL Lock, SW State) */
    CAPI_FW_CMD_READ_STATUS_LINE_LANE = 0xC,         /**< Read Status Line Lane (LOS, CDR Lock, PLL Lock, SW State) */

    CAPI_FW_CMD_SUSPEND_LINE_LANE_INGRESS = 0xD,
    CAPI_FW_CMD_RESUME_LINE_LANE_INGRESS = 0xE,
    CAPI_FW_CMD_SUSPEND_LINE_LANE_EGRESS = 0xF,
    CAPI_FW_CMD_RESUME_LINE_LANE_EGRESS = 0x10,

    CAPI_FW_CMD_SUSPEND_BH_LANE_INGRESS = 0x15,
    CAPI_FW_CMD_RESUME_BH_LANE_INGRESS = 0x16,
    CAPI_FW_CMD_SUSPEND_BH_LANE_EGRESS = 0x17,
    CAPI_FW_CMD_RESUME_BH_LANE_EGRESS = 0x18,

    CAPI_FW_CMD_ENABLE_LW_BH_IGNORE_FAULT = 0x19,
    CAPI_FW_CMD_ENABLE_BH_IGNORE_FAULT = 0x20,
    CAPI_FW_CMD_ENABLE_LW_IGNORE_FAULT = 0x21,

    CAPI_FW_CMD_DISABLE_LW_BH_IGNORE_FAULT = 0x22,
    CAPI_FW_CMD_DISABLE_BH_IGNORE_FAULT = 0x23,
    CAPI_FW_CMD_DISABLE_LW_IGNORE_FAULT = 0x24,

    CAPI_FW_CMD_GET_LW_STICKY_STATUS = 0x2E,

    CAPI_FW_CMD_SET_PORT_FUNC_MODE = 0x30,
    CAPI_FW_CMD_ADD_PORT = 0x31,
    CAPI_FW_CMD_DROP_PORT = 0x32,
    CAPI_FW_CMD_DROP_ALL_PORTS = 0x33,
    CAPI_FW_CMD_ASSERT_PORT_RESET = 0x34,
    CAPI_FW_CMD_RELEASE_PORT_RESET = 0x35,
    CAPI_FW_CMD_ASSERT_PORT_POWER_DOWN = 0x36,
    CAPI_FW_CMD_RELEASE_PORT_POWER_DOWN = 0x37,
    CAPI_FW_CMD_GET_PORT = 0x38,
    CAPI_FW_CMD_ADD_PORT_LPM = 0x39,
    CAPI_FW_CMD_ADD_PORT_EX = 0x3A,

    CAPI_FW_CMD_LAST_CMD
} capi_fw_cmd_t; /**< FW-CAPI lane command type */


#define IPC_COMMAND_TIMEOUT_MS 1000
#define COMMAND_TIMEOUT_MS 1000
#define LW_LANE_MASK     0xFF                        /**< Line Wrapper Lane Mask                 */
#define CLIENT_LANE_MASK 0xFF                        /**< Client Lane Mask                       */

/*************************************************************************************************
 * The union used for command payload for capi to fw interface  
 *************************************************************************************************/
typedef union intf_capi2fw_cmd_payload_info_u {
    uint32_t content;                                /**< capi to fw comand payload             */

    struct {
        #ifdef CAPI_IS_BIG_ENDIAN
        uint16_t cmd_tocken : 16;                    /**< Command tocken reference              */
        uint16_t cmd_length : 16;                    /**< command payload length                */
        #else  /*CAPI_IS_LITTLE_ENDIAN*/
        uint16_t cmd_length : 16;                    /**< command payload length                */
        uint16_t cmd_tocken : 16;                    /**< Command tocken reference              */
        #endif /*CAPI_IS_BIG/LITTLE_ENDIAN*/
    } fields;
} intf_capi2fw_cmd_payload_info_t;                   /**< capi to fw comand payload information */


/**************************************************************************************************
 * INTF_CAPI2FW_TOCKEN_DATA_LENGTH 
 *************************************************************************************************/
typedef union capi_fw_cmd_tocken_length_u {
    uint16_t words;
    struct {
        #ifdef CAPI_IS_BIG_ENDIAN
        uint16_t cmd_tocken : 8;                     /*!< command tocken                        */
        uint16_t cmd_length : 8;                     /*!< command length                        */
        #else /* LITTLE ENDIAN */
        uint16_t cmd_length : 8;                     /*!< command length                        */
        uint16_t cmd_tocken : 8;                     /*!< command tocken                        */
        #endif
    } fields;
} capi_fw_cmd_tocken_length_t;                       /*!< CAPI FW command tocken lenght type    */


/*************************************************************************************************
 * top_pam_amba :: INTF_CAPI2FW_CLIENT_CMD_REQUEST_GPREG 
 *                 INTF_CAPI2FW_LW_CMD_REQUEST_GPREG
 *************************************************************************************************/
typedef union intf_capi2fw_command_request_gpreg_u {
    uint16_t words;
    struct {
        #ifdef CAPI_IS_BIG_ENDIAN
        uint16_t cmd_request : 1;                    /*!< command request                      */
        uint16_t cmd_tocken  : 7;                    /*!< command tocken                       */
        uint16_t command_id  : 8;                    /*!< command Id                           */
        #else /* LITTLE ENDIAN */       
        uint16_t command_id  : 8;                    /*!< command Id                           */
        uint16_t cmd_tocken  : 7;                    /*!< command tocken                       */
        uint16_t cmd_request : 1;                    /*!< command request                      */
        #endif
    } fields;
} intf_capi2fw_command_request_gpreg_t;


/**************************************************************************************************
 * top_pam_amba :: INTF_CAPI2FW_CLIENT_CMD_RESPONSE_GPREG 
 *************************************************************************************************/
typedef union intf_fw2capi_command_response_gpreg_u {
    uint16_t words;
    struct {
        #ifdef CAPI_IS_BIG_ENDIAN
        uint16_t cmd_response  : 1;                  /*!< Command Response                      */
        uint16_t return_result : 8;                  /*!< Return Result                         */
        uint16_t reserved      : 7;                  /*!< Reserved                              */
        #else /* LITTLE ENDIAN  */     
        uint16_t reserved      : 7;                  /*!< Reserved                              */
        uint16_t return_result : 8;                  /*!< Return Result                         */
        uint16_t cmd_response  : 1;                  /*!< command response                      */
        #endif
    } fields;
} intf_fw2capi_command_response_gpreg_t;             /*!< CAPI-FW Interface Command Response    */


/**************************************************************************************************
 * top_pam_amba :: INTF_CAPI2FW_CMD_POLARITY_GPREG 
 *************************************************************************************************/
typedef union intf_capi2fw_cmd_polarity_gpreg_u {
    uint16_t words;
    struct {
        #ifdef CAPI_IS_BIG_ENDIAN
        uint16_t action    :  3;                  /*!< Action (Default/Invert Default/ Invert) */
        uint16_t direction :  3;                  /*!< Direction (Ingress/Egress/Both)         */
        uint16_t reserved  : 10;                  /*!< Reserved                                */
        #else /* LITTLE ENDIAN  */     
        uint16_t reserved  : 10;                  /*!< Reserved                                */
        uint16_t direction :  3;                  /*!< Direction (Ingress/Egress/Both)         */
        uint16_t action    :  3;                  /*!< Action (Default/Invert Default/ Invert) */
        #endif
    } fields;
} intf_capi2fw_cmd_polarity_gpreg_t;              /*!< CAPI-FW Interface polarity req pazyload  */

#ifdef __cplusplus
}
#endif

#endif /**< CAPI_FW_INTF_H  */

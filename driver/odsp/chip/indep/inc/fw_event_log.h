/**
 *
 * @file    fw_event_log.h
 * @author  Firmware Team
 * @date     5/1/2018
 * @version 0.1
 *
 * @property $Copyright: Copyright 2018 Broadcom INC.
 *           This program is the proprietary software of Broadcom INC
 *           and/or its licensors, and may only be used, duplicated, modified
 *           or distributed pursuant to the terms and conditions of a separate,
 *           written license agreement executed between you and Broadcom
 *           (an "Authorized License").  Except as set forth in an Authorized
 *           License, Broadcom grants no license (express or implied), right
 *           to use, or waiver of any kind with respect to the Software, and
 *           Broadcom expressly reserves all rights in and to the Software
 *           and all intellectual property rights therein.  IF YOU HAVE
 *           NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE
 *           IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE
 *           ALL USE OF THE SOFTWARE.
 *
 *           Except as expressly set forth in the Authorized License,
 *
 *           1.     This program, including its structure, sequence and organization,
 *           constitutes the valuable trade secrets of Broadcom, and you shall use
 *           all reasonable efforts to protect the confidentiality thereof,
 *           and to use this information only in connection with your use of
 *           Broadcom integrated circuit products.
 *
 *           2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS
 *           PROVIDED "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
 *           REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
 *           OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
 *           DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
 *           NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
 *           ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 *           CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
 *           OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 *           3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL
 *           BROADCOM OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL,
 *           INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER
 *           ARISING OUT OF OR IN ANY WAY RELATING TO YOUR USE OF OR INABILITY
 *           TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF THE
 *           POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF
 *           THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR USD 1.00,
 *           WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING
 *           ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.$
 *
 * @brief   this file includes estoque firmware event logging.
 *
 * @section
 * 
 */

#ifndef FIRMWARE_EVENT_LOG_H
#define FIRMWARE_EVENT_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *
 * Definition -- enum, structure
 *
******************************************************************************/

#define FM_EVENT_UUID  0x1ca4

#pragma pack(push)  /* push current alignment to stack */
#pragma pack(2)     /* set alignment to 2 byte boundary since ARM M0 is 2 bytes aligned at Thumb mode */

typedef struct {
    uint8_t lane_id;   /*!<lane_id: 0-7 */
    uint8_t event_type;    /*!<event_type: fw_event_type_t */
    uint16_t time_since_last_event; /**< time (ms) since the previous event */
    uint32_t event_data; /*!<event_data */
} firmware_event_log_t;

/* event log buffer is 0x208 - 0x8 = 0x200 bytes, each event is 4 bytes,
  so it can hold up to 0x200/4=128 events */
#define MAX_FW_EVENTS 128
//#define MAX_FW_EVENTS ((FM_EVENT_MAX_SIZE - 8)/ sizeof(firmware_event_log_t))

typedef struct {
    uint16_t fw_event_uuid; /**< UUID to check memory corruption */
    uint16_t fw_event_buffer_size; /**< size of firmware event log in words (32 bits) */
    uint8_t total_number_of_events; /**< total snumber of events logged */
    uint8_t number_of_events; /**< number of events logged */
    uint8_t oldest_event_id; /**< oldest event id */
    uint8_t latest_event_id; /**< latest event id */
    firmware_event_log_t event_log[MAX_FW_EVENTS]; /**< ring buffer contains fw event logs */
} firmware_event_log_buffer_t;

#pragma pack(pop)   /* restore original alignment from stack */

/**
 * The enumeration of FW event type
 */
typedef enum {
    FW_EVENT_TYPE_LW_RX_FAULT           = 0,      /**< LW Rx fault   */
    FW_EVENT_TYPE_LW_HANDLE_TX_FAULT    = 1,   /**< LW Tx fault is handled   */
    FW_EVENT_TYPE_LW_HANDLE_CAPI_CMD    = 2,   /**< CAPI LW command is handled   */
    FW_EVENT_TYPE_LW_HANDLE_CAPI_CMD_EX = 3,    /**< CAPI LW IPC is handled   */    
    FW_EVENT_TYPE_BH_HANDLE_CAPI_CMD    = 4,   /**< CAPI BH command is handled   */
    FW_EVENT_TYPE_BH_HANDLE_CAPI_CMD_EX = 5,    /**< CAPI BH IPC is handled   */  
    FW_EVENT_TYPE_CW_HANDLE_CAPI_CMD    = 6,    /**< CAPI CW command is handled   */
    FW_EVENT_TYPE_LW_PTUNE_SNR          = 7,    /**< LW phase tuning SNR value   */
    FW_EVENT_TYPE_LW_PTUNE_BIAS         = 8,   /**< LW phase tuning phase bias value   */    
    FW_EVENT_TYPE_MEDIA_RX_FSM_STATE    = 9,   
    FW_EVENT_TYPE_MEDIA_TX_FSM_STATE    = 10,   
    FW_EVENT_TYPE_HOST_RX_FSM_STATE     = 11,   
    FW_EVENT_TYPE_HOST_TX_FSM_STATE     = 12,
    FW_EVENT_TYPE_HOST_FEC_DEC_ENABLE   = 13,
    FW_EVENT_TYPE_MEDIA_FEC_DEC_ENABLE  = 14,
    FW_EVENT_TYPE_HOST_FEC_TOT_FRAMES   = 15,
    FW_EVENT_TYPE_MEDIA_FEC_TOT_FRAMES  = 16,
    FW_EVENT_TYPE_HOST_FEC_LOCK         = 17,
    FW_EVENT_TYPE_MEDIA_FEC_LOCK        = 18,
    FW_EVENT_TYPE_CW_RX_FSM_STATE       = 19,
    FW_EVENT_TYPE_CW_TX_FSM_STATE       = 20,
    FW_EVENT_TYPE_LX_RX_FSM_STATE       = 21,
    FW_EVENT_TYPE_CW_DROP_PORT          = 22,
    FW_EVENT_TYPE_CW_ADD_PORT           = 23,
    FW_EVENT_TYPE_ENABLE_DSP_TX_CLK     = 24,
    FW_EVENT_TYPE_LW_PFTUNE             = 25,
    FW_EVENT_TYPE_SET_FEC_LOL_STICKY    = 26,
    FW_EVENT_TYPE_FEC_COUNTER_CLEAR     = 27,
    FW_EVENT_TYPE_LW_DYNAMIC_TUNE       = 28,    
    FW_EVENT_TYPE_LW_DYNAMIC_TUNE_CAL_GRADIEN = 29,
} fw_event_type_t;

typedef enum {
    FW_EVENT_CLIENT = 0,
    FW_EVENT_DW,
    FW_EVENT_LW,
    FW_EVENT_CORE
} fw_event_source_t;

/*****************************************************************************
 *
 * Definition -- function prototype
 *
******************************************************************************/

/**
 * @brief      fw_event_log_init()
 * @details     This function is used to init firmware event logging
 *
 * @ property   None
 * @ public     None
 * @ private    None
 * @example     None
 * 
 */
void fw_event_log_init(void);

/**
 * @brief       void fw_log_event(fw_event_type_t event_type, uint8_t lane_id, uint32_t event_data);
 
 * @details     This function is used to log firmware event
 *
 * @ property   None
 * @ public     None
 * @ private    None
 * @example     None
 * 
 */
void fw_log_event(fw_event_type_t event_type, uint8_t lane_id, uint32_t event_data);

/**
 * @brief       fw_event_log_load(void)
 * @details     This function is used to load firmware logs from external memory
 *
 * @ property   None
 * @ public     None
 * @ private    None
 * @example     None
 *
 * 
 * @return  returns the performance result of the called methode/function
 */
return_result_t fw_event_log_load(uint32_t phy_id);

//#define FW_EVENT_LOG_DISABLED

#ifdef FW_EVENT_LOG_DISABLED
#define FW_EVENT_LOG_INIT()
#define FW_LOG_EVENT(event_type, lane_id, event_data)
#define FW_EVENT_LOG_LOAD()
#else
#define FW_EVENT_LOG_INIT() fw_event_log_init()
#define FW_LOG_EVENT(event_type, lane_id, event_data)  fw_log_event(event_type, lane_id, event_data)
#define FW_EVENT_LOG_LOAD(phy_id) fw_event_log_load(phy_id)
#endif

#ifdef __cplusplus
}
#endif

#endif /*FIRMWARE_EVENT_LOG_H*/


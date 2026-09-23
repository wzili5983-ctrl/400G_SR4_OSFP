/**
 *
 * @file cw_comm_def.h
 * @author  
 * @date     09/01/2018
 * @version 1.0
 *
 * $Copyright: Copyright 2018 Broadcom INC.
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
 * 1.      This program, including its structure, sequence and organization,
 * constitutes the valuable trade secrets of Broadcom, and you shall use
 * all reasonable efforts to protect the confidentiality thereof,
 * and to use this information only in connection with your use of
 * Broadcom integrated circuit products.
 * 
 * 2.      TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS
 * PROVIDED "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
 * REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
 * OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
 * DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
 * NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
 * ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
 * OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 * 
 * 3.      TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL
 * BROADCOM OR ITS LICENSORS BE LIABLE FOR  CONSEQUENTIAL,
 * INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER
 * ARISING OUT OF OR IN ANY WAY RELATING TO YOUR USE OF OR INABILITY
 * TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF
 * THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR USD 1.00,
 * WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING
 * ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.$
 *
 * @brief   this file includes cHAL function implementation.
 *
 * @section
 * 
 */


#ifndef CW_COMM_DEF_H
#define CW_COMM_DEF_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_NOOF_PORTS 4   /**< PORT NUMBER */
#define FULL_PORT_MASK 0x0F
#define MAX_BH_LANES 4   /**< Maximum supported BH lane NUMBER */
#define FULL_BH_LANE_MASK  0x0F
#define FULL_LW_LANE_MASK  0x0F

#define CW_PORT_IN_LOW_POWER      0x3
// The handlers are setup based on the port type table and other settings
// when port add command is give by the user

// Following handlers are executed by the top level CW loop
// processing loop that will drive the egress, ingress state machine and process other port commands
typedef return_result_t (*cw_port_process)(const void* cw_profile_ptr_void, void* cw_port_profile_ptr_void);

// Is called after a new port is added by the user command
typedef return_result_t (*cw_port_init)(const void* cw_profile_ptr_void, void* cw_port_profile_ptr_void);
// Is called when a port is deleted by the user command
typedef return_result_t (*cw_port_deinit)(const void* cw_profile_ptr_void, void* cw_port_profile_ptr_void);

// Tx side port state machine processing
typedef return_result_t (*cw_port_process_tx)(const void* cw_profile_ptr_void, void* cw_port_profile_ptr_void, void* cw_port_profile_tx_ptr_void);

typedef return_result_t (*cw_port_init_tx)(const void* cw_profile_ptr_void, void* cw_port_profile_ptr_void, void* cw_port_profile_tx_ptr_void);

typedef return_result_t (*cw_port_deinit_tx)(const void* cw_profile_ptr_void, void* cw_port_profile_ptr_void, void* cw_port_profile_tx_ptr_void);

// Rx side port state machine processing
typedef return_result_t (*cw_port_process_rx)(const void* cw_profile_ptr_void, void* cw_port_profile_ptr_void, void* cw_port_profile_rx_ptr_void);

typedef return_result_t (*cw_port_init_rx)(const void* cw_profile_ptr_void, void* cw_port_profile_ptr_void, void* cw_port_profile_rx_ptr_void);

typedef return_result_t (*cw_port_deinit_rx)(const void* cw_profile_ptr_void, void* cw_port_profile_ptr_void, void* cw_port_profile_rx_ptr_void);


/*CW default mode*/
typedef enum cw_default_mode_e {
    CW_DEFAULT_MODE_1_REPEATER = 1,  /**< CHIP_MODE_1X53G_KP4PAM_TO_1X106G_KP4PAM  CHIP_HOST_FEC_TYPE_NA    CHIP_LINE_FEC_TYPE_NA  CHIP_PORT_FEC_TERM_BYPASS  CHIP_PORT_MUX_BIT_MUX*/
    CW_DEFAULT_MODE_2_REPEATER = 2,  /**< CHIP_MODE_1X53G_KP4PAM_TO_1X53G_KP4PAM  CHIP_HOST_FEC_TYPE_NA    CHIP_LINE_FEC_TYPE_NA  CHIP_PORT_FEC_TERM_BYPASS  CHIP_PORT_MUX_BIT_MUX*/
//    CW_DEFAULT_MODE_3_REPEATER = 3,  /**< CHIP_MODE_8X53G_KP4PAM_TO_4X106G_KP4PAM  CHIP_HOST_FEC_TYPE_NA    CHIP_LINE_FEC_TYPE_NA  CHIP_PORT_FEC_TERM_BYPASS  CHIP_PORT_MUX_BIT_MUX*/
    CW_DEFAULT_MODE_MAX = 3
#if 0
    CW_DEFAULT_MODE_1_KP4_KP4  = 2,  /**< CHIP_MODE_2X53G_KP4PAM_TO_1X106G_KP4PAM  CHIP_HOST_FEC_TYPE_RS544    CHIP_LINE_FEC_TYPE_RS544  CHIP_PORT_FEC_DEC_ENC  CHIP_PORT_MUX_BIT_MUX*/
    CW_DEFAULT_MODE_5_PCS_KP4  = 3,  /**< CHIP_MODE_4x25G_NRZ_TO_1X106G_KP4PAM  CHIP_HOST_FEC_TYPE_PCS    CHIP_LINE_FEC_TYPE_RS544  CHIP_LANE_PCS_XENC  CHIP_PORT_MUX_BIT_MUX*/
    CW_DEFAULT_MODE_5_KR4_KP4  = 4,  /**< CHIP_MODE_4x25G_NRZ_TO_1X106G_KP4PAM  CHIP_HOST_FEC_TYPE_RS528    CHIP_LINE_FEC_TYPE_RS544  CHIP_PORT_FEC_DEC_ENC  CHIP_PORT_MUX_BIT_MUX*/
    CW_DEFAULT_MODE_4_NRZ_NRZ_REPEATER  = 5,  /**< CHIP_MODE_4x25G_KR4NRZ_TO_4x25G_KR4NRZ  CHIP_HOST_FEC_TYPE_NA    CHIP_HOST_FEC_TYPE_NA  CHIP_PORT_FEC_TERM_BYPASS  CHIP_PORT_MUX_BIT_MUX*/
    CW_DEFAULT_MODE_7_REPEATER = 6,  /**< CHIP_MODE_8x53G_KP4PAM_TO_4X106G_KP4PAM  CHIP_HOST_FEC_TYPE_NA    CHIP_LINE_FEC_TYPE_NA  CHIP_PORT_FEC_TERM_BYPASS  CHIP_PORT_MUX_BIT_MUX*/
    CW_DEFAULT_MODE_7_KP4_KP4 = 7,  /**< CHIP_MODE_8x53G_KP4PAM_TO_4X106G_KP4PAM  CHIP_HOST_FEC_TYPE_RS544    CHIP_HOST_FEC_TYPE_RS544  CHIP_PORT_FEC_DEC_ENC  CHIP_PORT_MUX_BIT_MUX*/
    CW_DEFAULT_MODE_MAX  = 8,
#endif
} cw_default_mode_e;

/* FSM state machine state definition */
#ifdef FSM_SIMULATION
typedef enum cw_fsm_state_e {
    CW_FSM_STATE_INIT_DPATH = 0,
    CW_FSM_STATE_WAIT_FOR_CDR_RESTART = 1,
    CW_FSM_STATE_WAIT_FOR_TXPI_RESTART = 2,
    CW_FSM_STATE_WAIT_FOR_CDR_LOCK = 3,
    CW_FSM_STATE_WAIT_FOR_FECPCS_LOCK = 4, 
    CW_FSM_STATE_WAIT_FOR_TXPI_CONVERGED= 5, 
    CW_FSM_STATE_PENDING_MISSION_MODE = 6,
    CW_FSM_STATE_MISSION_MODE = 7 
} cw_fsm_state_t;
#else
typedef enum cw_fsm_state_e {
    CW_FSM_STATE_INIT_DPATH = 1<<0,
    CW_FSM_STATE_WAIT_FOR_CDR_RESTART =1<<1,
    CW_FSM_STATE_WAIT_FOR_TXPI_RESTART = 1<<2,
    CW_FSM_STATE_WAIT_FOR_CDR_LOCK = 1<<3,
    CW_FSM_STATE_WAIT_FOR_FECPCS_LOCK = 1<<4, 
    CW_FSM_STATE_WAIT_FOR_TXPI_CONVERGED=    1<<5, 
    CW_FSM_STATE_PENDING_MISSION_MODE = 1<<6,
    CW_FSM_STATE_MISSION_MODE = 1<<7 
} cw_fsm_state_t;
#endif

typedef enum cw_port_config_status_e {
    CW_PORT_CONFIG_STATUS_NOT_CONFIGURED=0,    /*No port configured */
    CW_PORT_CONFIG_STATUS_IN_PROGRESS=1,       /*Configuration is in progress*/
    CW_PORT_CONFIG_STATUS_SUCCESS=2,           /*Port configured successfully and ready to run*/
    CW_PORT_CONFIG_STATUS_FAILED=3,            /*Port configured but failed */
} cw_port_config_status_t;



typedef struct cw_rptr_tx_s {
    int gbox_fault_count;
    uint32_t pending_mission_ts;
}cw_rptr_tx_t;

typedef struct cw_pcsfec_tx_s {

    int txpi_restart_count;
    uint32_t pending_mission_ts;
    int igbox_reset_count;
}cw_rtmr_tx_t;

typedef union cw_port_tx_u{
    // Context needed for different functionality like repeater
    // To be added
    cw_rtmr_tx_t pcsfec;
    cw_rptr_tx_t rptr;
} cw_port_tx_t;

typedef struct cw_rptr_rx_s {
    int gbox_fault_count;
    uint32_t pending_mission_ts;
}cw_rptr_rx_t;

typedef struct cw_pcsfec_rx_s {
    int txpi_restart_count;
    uint32_t pending_mission_ts;
    int igbox_reset_count;
}cw_rtmr_rx_t;


typedef union cw_port_rx_u{
    // Context needed for different functionality like repeater
    // To be added
    cw_rtmr_rx_t pcsfec;
    cw_rptr_rx_t rptr;
} cw_port_rx_t;

/* FEC monitor sampleing info */
typedef struct cw_fec_monitor_sampling_info_s {
    uint64_t decoder_start_time;    // FEC monitor decoder start time since FW init
    uint64_t start_time_since_init;    // scheduled FEC monitor start time since FW init
    uint32_t enable_time_since_init;    // FEC monitor enable time since FW init
    uint32_t decoder_reset_times;    // How many times decoder reset is toggled   
    uint8_t sampling_rate;  // FEC monitor sampling rate in percentage
    uint8_t sub_sampling_enabled:1;      // FEC monitor sub-sampling is enabled by CAPI
    uint8_t fec_decoder_on:1;      // FEC decoder is on, FEC monitor is active running
    uint8_t fec_lock:1;            // FEC locks
    uint8_t fec_counter_clear_req:1;            // received FEC counter clear request from CAPI
    uint8_t rsvd:4;
} cw_fec_monitor_sampling_info_t;


/* Port profile */
typedef struct cw_port_profile_rx_s {

    //handler defintion;
    cw_port_process_rx rx_hnd;
    cw_port_init_rx rx_init;
    cw_port_deinit_rx rx_deinit;
    //data path port definition
    cfg_dp_type_t rx_dpath;
    cw_port_rx_t ctx_rx;
    cw_fsm_state_t state;               //RX FSM state
    uint8_t txpi_restart_count:4;
    uint8_t cdr_restart_count:4;
    uint8_t mission_bit_notsynch:1; //=0 consumed, =1 not consumed
    uint8_t mission_edge_up:1; // if not consumed, value to write
    uint8_t pgen_en:1;
    uint8_t rsvd:5;
    cw_fec_monitor_sampling_info_t fec_monitor_sampling_info;  // FEC monitor sampling info
} cw_port_profile_rx_t;

typedef struct cw_port_profile_tx_s {
    //handler defintion;
    cw_port_process_tx tx_hnd;
    cw_port_init_tx tx_init;
    cw_port_deinit_tx tx_deinit;
    //data path port definition
    cfg_dp_type_t tx_dpath;
    cw_port_tx_t ctx_tx;
    cw_fsm_state_t state;             //TX FSM state
    uint8_t txpi_restart_count:4;
    uint8_t cdr_restart_count:4;
    uint8_t mission_bit_notsynch:1; //=0 consumed, =1 not consumed
    uint8_t mission_edge_up:1; // if not consumed, value to write
    uint8_t pgen_en:1;
    uint8_t rsvd:5;
    cw_fec_monitor_sampling_info_t fec_monitor_sampling_info;  // FEC monitor sampling info
} cw_port_profile_tx_t;

typedef struct cw_port_hnd_s {
    cw_port_init port_init_hnd;
    cw_port_deinit port_deinit_hnd;
    cw_port_process port_process_hnd;
} cw_port_hnd_t;

typedef enum cw_cmd_e {
    CW_NO_CMD=0x0,
    CW_CMD_SET_FUNCTIONAL_MODE=0x30,// – Set Functional Mode for PORTs (Apply globally – Drop all existing PORTs followed by Add PORTs)
    CW_CMD_ADD_NEW_PORTS=0x31,// – Add new PORTs (Apply globally – proceed only if not colliding with existing PORTs – not forced)
    CW_CMD_DROP_EXISTING_PORTS=0x32,// – Drop existing PORTs (Apply globally – proceed only if PORTs exists – not forced)
    CW_CMD_DROP_ALL_EXISTING_PORTS=0x33,// – Drop all existing PORTs (Apply globally)
    CW_CMD_ASSERT_DATA_RESET_PORTS=0x34,// – Assert data_reset for PORTs
    CW_CMD_DEASSERT_DATA_RESET_PORTS=0x35,// – de-Assert data_reset for PORTs
    CW_CMD_ASSESRT_POWERDOWN_PORTS=0x36,// – Assert power down for PORTs
    CW_CMD_DEASSESRT_POWERDOWN_PORTS=0x37,// – de-assert power down for PORTs 
    CW_CMD_GET_PORT_STATUS=0x38,
    CW_CMD_ADD_NEW_PORTS_LPM=0x39,
    CW_CMD_ADD_PORT_EX=0x3A
}cw_cmd_t;

typedef enum cw_port_cmd_state_e {
    CW_PORT_CMD_STATE_START_SEND,
    CW_PORT_CMD_STATE_SENT,
    CW_PORT_CMD_STATE_ACKED,
    CW_PORT_CMD_STATE_WAIT_TIME,
    CW_PORT_CMD_STATE_DONE
} cw_port_cmd_state_t;



typedef enum cw_mode_config_state_e {
    CW_MODE_CONFIG_STATE_DONE,
    CW_MODE_CONFIG_STATE_DROP_PORTS,
    CW_MODE_CONFIG_STATE_ADD_PORTS
} cw_mode_config_state_t;


typedef struct cw_port_profile_s {
    octal_port_mode_config1_reg_t cfg1;
    octal_port_mode_config2_reg_t cfg2;
    uint8_t port_vld;
    cw_port_entry_ex_t entry;
    
    cw_port_profile_rx_t rx;
    cw_port_profile_tx_t tx;
    const cw_port_hnd_t* port_hnd;
    cw_cmd_t cmd;
    uint8_t port_index;
    uint32_t cw_bbaddr;

    uint32_t egress_fsm_ts;
    uint32_t ingress_fsm_ts;

} cw_port_profile_t;


typedef struct cw_fec_dbg_s {

    uint16_t host_fifo_flt;
    uint16_t host_pcs_gbox_flt;
    uint16_t fec_sync_algn_unlock;
    uint16_t pcs_sync_flt;
    uint16_t xdec_gbox_clsn_flt;
    uint16_t xdec_err_flt;
    uint16_t xenc_gbox_clsn_flt;
    uint16_t xenc_err_flt;
    uint16_t kr2kp_gbox_clsn;
    uint16_t kp2kr_gbox_clsn;
    uint16_t inner_gbox168to160_full;
    uint16_t inner_gbox168to160_empty;
    uint16_t line_pfifo_flt;
    uint16_t inner_pfifo162_flt;
} cw_fec_dbg_t;
typedef union ocw_config_pcs_fec_flt_proc_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t igr_dis_pcs_fec_flt_proc : 8; /*!< disable fault processing for ingress based on line_lane_mask */
        uint16_t egr_dis_pcs_fec_flt_proc : 8; /*!< disable fault processing for egress based on line_lane_mask */
#else /* LITTLE ENDIAN */
        uint16_t egr_dis_pcs_fec_flt_proc : 8; /*!< disable fault processing for egress based on line_lane_mask */
        uint16_t igr_dis_pcs_fec_flt_proc : 8; /*!< disable fault processing for ingress based on line_lane_mask */
#endif
    } fields;
} ocw_config_pcs_fec_flt_proc_reg_t;


#ifdef __cplusplus
}
#endif

#endif /**< CW_COMM_DEF_H*/

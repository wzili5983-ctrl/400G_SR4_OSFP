
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
*            San Jose, California 95131
*            United States
*
*     All information contained in this document is Broadcom Limited 
*     company private, proprietary, and trade secret.
*
****************************************************************************/

/**
 *       Centenario chip common header file
 *
 * @file chip_common_confirg_ind.h
 * @author $Id$
 * @date 12-8-2016
 * @brief This file includes Centenario chip level common header
 */

#ifndef CHIP_COMMON_IND_H
#define CHIP_COMMON_IND_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 * blackhawk_estoque_com_amba :: CHIP_TOP_CHIP_REFCLK_CONFIG_REG 
 ***************************************************************************/
typedef union chip_top_chip_refclk_config_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t ref_clk_valid : 1; /*!<ref_clk_valid */
        uint16_t reserved0 : 10; /*!<reserved0 */
        uint16_t ref_clk_freq_select : 5; /*!<ref_clk_freq_select */
#else /* LITTLE ENDIAN */
        uint16_t ref_clk_freq_select : 5; /*!<ref_clk_freq_select */
        uint16_t reserved0 : 10; /*!<reserved0 */
        uint16_t ref_clk_valid : 1; /*!<ref_clk_valid */
#endif
    } fields;
} chip_top_chip_refclk_config_reg_t;

/****************************************************************************
 * top_pam_amba :: COMMON_LANE_MODE_REG 
 ***************************************************************************/
typedef union common_lane_mode_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t blackhawk_loopback_alt_mode : 1; /*!<blackhawk_loopback_alt_mode */
        uint16_t function_mode : 6; /*!<function_mode */
        uint16_t fec_mode : 2; /*!<fec_mode */
        uint16_t rate_mode : 7; /*!<rate_mode */
#else /* LITTLE ENDIAN */
        uint16_t rate_mode : 7; /*!<rate_mode */
        uint16_t fec_mode : 2; /*!<fec_mode */
        uint16_t function_mode : 6; /*!<function_mode */
        uint16_t blackhawk_loopback_alt_mode : 1; /*!<blackhawk_loopback_alt_mode */
#endif
    } fields;
} common_lane_mode_reg_t;


/****************************************************************************
 * top_pam_amba :: COMMON_LANE_COMMAND_REG 
 ***************************************************************************/
typedef union common_lane_command_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t command_change : 1; /*!<command_change */
        uint16_t reserved0 : 7; /*!<reserved0 */
        uint16_t command : 8; /*!<command */
#else /* LITTLE ENDIAN */
        uint16_t command : 8; /*!<command */
        uint16_t reserved0 : 7; /*!<reserved0 */
        uint16_t command_change : 1; /*!<command_change */
#endif
    } fields;
} common_lane_command_reg_t;

/****************************************************************************
 * top_pam_amba :: COMMON_LANE_STATUS_REG, COMMON_BH_LANE_COMMAND_STATUS_REG_0 to 7, LW_LANE_STATUS_REG_0 
 ***************************************************************************/

enum cmd_status_error_code_u{
    CMD_STATUS_NO_ERROR = 0,
    CMD_STATUS_ERROR_NO_SUPPORT = 1,
};
typedef union common_lane_status_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t command_accepted : 1; /*!<command_accepted */
        uint16_t ready_for_command : 1; /*!<ready_for_command */
        uint16_t error_code : 6; /*!<error_code */
        uint16_t status_return : 8; /*!<status_return */
#else /* LITTLE ENDIAN */
        uint16_t status_return : 8; /*!<status_return */
        uint16_t error_code : 6; /*!<error_code */
        uint16_t ready_for_command : 1; /*!<ready_for_command */
        uint16_t command_accepted : 1; /*!<command_accepted */
#endif
    } fields;
} common_lane_status_reg_t;

typedef union cw_fec_mon_discover_ports_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t dont_loop :1;  
        uint16_t rsvd  : 3;   
        uint16_t port2 : 4; 
        uint16_t port1 : 4;
        uint16_t port0 : 4;  
#else /* LITTLE ENDIAN */
        uint16_t port0 : 4;   
        uint16_t port1 : 4; 
        uint16_t port2 : 4;
        uint16_t rsvd  : 3;
        uint16_t dont_loop :1;  
#endif
    } fields;
} cw_fec_mon_discover_ports_reg_t;

typedef union cw_fec_mon_discover_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t rsvd  : 2;   
        uint16_t state : 2;
        uint16_t port2 : 4; 
        uint16_t port1 : 4;
        uint16_t port0 : 4;  
#else /* LITTLE ENDIAN */
        uint16_t port0 : 4;   
        uint16_t port1 : 4; 
        uint16_t port2 : 4;
        uint16_t state : 2;
        uint16_t rsvd  : 2;  
#endif
    } fields;
} cw_fec_mon_discover_reg_t;


/****************************************************************************
 *  FW_INTERNAL_CONFIG_REG_1 QUAD_CORE_GPCTRL_GPCTRL_164_RDB, CAPI write
 
 ***************************************************************************/
#define FW_INTERNAL_CONFIG_REG_1_SUPER_CMODE_CFG_EN_MASK                0x00000001 /*!< SUPER_CMODE_CFG_EN*/
#define FW_INTERNAL_CONFIG_REG_1_SUPER_CMODE_CFG_EN_SHIFT               0
#define FW_INTERNAL_CONFIG_REG_1_SUPER_CMODE_IGNORE_PLLCLSN_EN_MASK     0x00000002 /*!< SUPER_CMODE_IGNORE_PLLCLSN_EN*/
#define FW_INTERNAL_CONFIG_REG_1_SUPER_CMODE_IGNORE_PLLCLSN_EN_SHIFT    1

#define FW_INTERNAL_CONFIG_REG_1_ADCCAL_RECFG_TIMES_MASK                0x0000008C /*!< ADCCAL_RECFG_TIMES*/
#define FW_INTERNAL_CONFIG_REG_1_ADCCAL_RECFG_TIMES_SHIFT               2
#define FW_INTERNAL_CONFIG_REG_1_PLL_RECFG_TIMES_MASK                   0x000000E0 /*!< PLL_RECFG_TIMES*/
#define FW_INTERNAL_CONFIG_REG_1_PLL_RECFG_TIMES_SHIFT                  5

#define FW_INTERNAL_CONFIG_REG_1_QSFP_FORCE_EN_MASK                     0x00000100 /*!< QSFP_FORCE_EN*/
#define FW_INTERNAL_CONFIG_REG_1_QSFP_FORCE_EN_SHIFT                    8
#define FW_INTERNAL_CONFIG_REG_1_QSFP_FORCE_VAL_MASK                    0x00000200 /*!< QSFP_FORCE_VAL*/
#define FW_INTERNAL_CONFIG_REG_1_QSFP_FORCE_VAL_SHIFT                   9

#define FW_INTERNAL_CONFIG_REG_1_PLL_FAKE_FRACTION_MASK                 0x00000400 /*!< PLL_FAKE_FRACTION*/
#define FW_INTERNAL_CONFIG_REG_1_PLL_FAKE_FRACTION_SHIFT                10
#define FW_INTERNAL_CONFIG_REG_1_DISABLE_PLL_AUTO_FRACN_MODE_MASK       0x00000800 /*!< DISABLE_PLL_AUTO_FRACN_MODE*/
#define FW_INTERNAL_CONFIG_REG_1_DISABLE_PLL_AUTO_FRACN_MODE_SHIFT      11

#define FW_INTERNAL_CONFIG_REG_1_ENABLE_ADCCAL_VCODE_CHK_MASK           0x00002000 /*!< ENABLE_ADCCAL_VCODE_CHK*/
#define FW_INTERNAL_CONFIG_REG_1_ENABLE_ADCCAL_VCODE_CHK_SHIFT          13

#define FW_INTERNAL_CONFIG_REG_1_DISABLE_CW_CLK66_RST_MASK              0x00004000 /*!< DISABLE_CW_CLK66_RST*/
#define FW_INTERNAL_CONFIG_REG_1_DISABLE_CW_CLK66_RST_SHIFT             14

#define FW_INTERNAL_CONFIG_REG_1_ENABLE_CW_FLT_CLR_MASK                 0x00008000 /*!< ENABLE_CW_FLT_CLR*/
#define FW_INTERNAL_CONFIG_REG_1_ENABLE_CW_FLT_CLR_SHIFT                15

/****************************************************************************
 *  FW_INTERNAL_CONFIG_REG_2 QUAD_CORE_GPCTRL_GPCTRL_165_RDB, FW write
 
 ***************************************************************************/
#define FW_INTERNAL_CONFIG_REG_2_M1_MODES_LANE_SWAP_EN_MASK 0x00008000 /*!< M1_MODES_LANE_SWAP_EN*/
#define FW_INTERNAL_CONFIG_REG_2_M1_MODES_LANE_SWAP_EN_SHIFT 15

#define FW_INTERNAL_CONFIG_REG_2_QUAD_SWAP_EN_MASK 0x00004000 /*!< QUAD_SWAP_EN*/
#define FW_INTERNAL_CONFIG_REG_2_QUAD_SWAP_EN_SHIFT 14

#define FW_INTERNAL_CONFIG_REG_2_DSP_SNR_LMS_RAND_DIS_ISR_TIMER_MASK 0x00003FE0 /*!< DSP_SNR_LMS_RAND_DIS_ISR_TIMER*/
#define FW_INTERNAL_CONFIG_REG_2_DSP_SNR_LMS_RAND_DIS_ISR_TIMER_SHIFT 5
#define FW_INTERNAL_CONFIG_REG_2_DSP_SNR_LMS_RAND_DIS_MASK 0x00000010 /*!< DSP_SNR_LMS_RAND_DIS*/
#define FW_INTERNAL_CONFIG_REG_2_DSP_SNR_LMS_RAND_DIS_SHIFT 4

#define FW_INTERNAL_CONFIG_REG_2_ENABLE_DSP_PLL_FRAC_MODE_DYNM_ADJUST_MASK 0x00000002 /*!< ENABLE_DSP_PLL_FRAC_MODE_DYNM_ADJUST */
#define FW_INTERNAL_CONFIG_REG_2_ENABLE_DSP_PLL_FRAC_MODE_DYNM_ADJUST_SHIFT 1

#define FW_INTERNAL_CONFIG_REG_2_CW_FEC_MON_SAMPLING_CDR_CTRL_DISABLED_MASK 0x00000001 /*!< CW_FEC_MON_SAMPLING_CDR_CTRL_DISABLED */
#define FW_INTERNAL_CONFIG_REG_2_CW_FEC_MON_SAMPLING_CDR_CTRL_DISABLED_SHIFT 0


/****************************************************************************
 *  FW_INTERNAL_CONFIG_REG_3 QUAD_CORE_CONFIG_CFGVAL_35_RDB
 
 ***************************************************************************/
#define FW_INTERNAL_CONFIG_REG_3_EX_DFT_CMODE_CFG_EN_MASK                        0x00000001 /*!< EX_DFT_CMODE_CFG_EN*/
#define FW_INTERNAL_CONFIG_REG_3_EX_DFT_CMODE_CFG_EN_SHIFT                       0

#define FW_INTERNAL_CONFIG_REG_3_DSP_SNR_LMS_RAND_DIS_ENABLED_MASK                0x00000002 /*!< DSP_SNR_LMS_RAND_DIS_ENABLED*/
#define FW_INTERNAL_CONFIG_REG_3_DSP_SNR_LMS_RAND_DIS_ENABLED_SHIFT               1

#define FW_INTERNAL_CONFIG_REG_3_PHASE_ERROR_WR_DWELL_US_MASK       0x00007C /*!< PHASE_ERROR_WR_DWELL_US*/
#define FW_INTERNAL_CONFIG_REG_3_PHASE_ERROR_WR_DWELL_US_SHIFT      2

#define FW_INTERNAL_CONFIG_REG_3_DSP_DISABLE_FULL_DIAG_DUMP_MASK                   0x00000800 /*!< DSP_DISABLE_FULL_DIAG_DUMP*/
#define FW_INTERNAL_CONFIG_REG_3_DSP_DISABLE_FULL_DIAG_DUMP_SHIFT                  11

#define FW_INTERNAL_CONFIG_REG_3_ENABLE_ELECTRIC_MODE_MASK                          0x00002000 /*!< EANBLE_ELECTRIC_MODE*/
#define FW_INTERNAL_CONFIG_REG_3_ENABLE_ELECTRIC_MODE_SHIFT                         13   
    
#define FW_INTERNAL_CONFIG_REG_3_DSP_ENABLE_BRCM_FEC_ST_CLR_MASK                0x00004000 /*!< DSP_ENABLE_BRCM_FEC_ST_CLR*/
#define FW_INTERNAL_CONFIG_REG_3_DSP_ENABLE_BRCM_FEC_ST_CLR_SHIFT               14

#define FW_INTERNAL_CONFIG_REG_3_LW_PTUNE_DISABLE_MODE_CHECK_MASK                   0x00008000 /*!< LW_PTUNE_DISABLE_MODE_CHECK */
#define FW_INTERNAL_CONFIG_REG_3_LW_PTUNE_DISABLE_MODE_CHECK_SHIFT                  15

#define FW_INTERNAL_CONFIG_REG_3_FW_LOS_STOP_TESTING_MASK 0x00001000 /*!< FW_LOS_STOP_TESTING */
#define FW_INTERNAL_CONFIG_REG_3_FW_LOS_STOP_TESTING_SHIFT 12

typedef union octal_fw_internal_cfg_3_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t lw_ptune_disable_mode_check:1;/*!<LW phase tuning: disable chip mode check */
        uint16_t rsvd:14;
        uint16_t ex_dft_cmode_cfg_en:1;/*!<fw backdoor to enable default chip mode from external cAPI */
#else /* LITTLE ENDIAN */
        uint16_t ex_dft_cmode_cfg_en:1;/*!<fw backdoor to enable default chip mode from external cAPI */
        uint16_t rsvd:14;
        uint16_t lw_ptune_disable_mode_check:1;/*!<LW phase tuning: disable chip mode check */
#endif
    } fields;
} octal_fw_internal_cfg_3_t;

/****************************************************************************
 * PORT Mode cAPI Conifg1 Registers:  QUAD_CORE_CONFIG_RDB_CFGVAL_6_RDB
 
 ***************************************************************************/
#define PORT_MODE_CAPI_CONFIG_1_REG_PORT_VLD_MASK 0x00008000 /*!< PORT Valid*/
#define PORT_MODE_CAPI_CONFIG_1_REG_PORT_VLD_SHIFT 15
#define PORT_MODE_CAPI_CONFIG_1_REG_PORT_INDEX_MASK 0x00007000 /*!< PORT_INDEX*/
#define PORT_MODE_CAPI_CONFIG_1_REG_PORT_INDEX_SHIFT 12
#define PORT_MODE_CAPI_CONFIG_1_REG_MIXED_MODE_MASK 0x00000C00 /*!< MIXED_MODE*/
#define PORT_MODE_CAPI_CONFIG_1_REG_MIXED_MODE_SHIFT 10
#define PORT_MODE_CAPI_CONFIG_1_REG_RSVD_MASK 0x00000300 /*!< RSVD*/
#define PORT_MODE_CAPI_CONFIG_1_REG_RSVD_SHIFT 8
#define PORT_MODE_CAPI_CONFIG_1_REG_PORT_MODE_MASK 0x000000FF /*!< PORT MODE*/
#define PORT_MODE_CAPI_CONFIG_1_REG_PORT_MODE_SHIFT 0


/****************************************************************************
 * PORT Mode Conifg1 Registers: OCTAL_PORT_MODE_CONFIG_1_REG_0
 ***************************************************************************/
#define OCTAL_PORT_MODE_CONFIG_1_REG_0_PORT_VLD_MASK 0x00008000 /*!< PORT Valid*/
#define OCTAL_PORT_MODE_CONFIG_1_REG_0_PORT_VLD_SHIFT 15
#define OCTAL_PORT_MODE_CONFIG_1_REG_0_PORT_INDEX_MASK 0x00007000 /*!< PORT_INDEX*/
#define OCTAL_PORT_MODE_CONFIG_1_REG_0_PORT_INDEX_SHIFT 12
#define OCTAL_PORT_MODE_CONFIG_1_REG_0_MIXED_MODE_MASK 0x00000C00 /*!< MIXED_MODE*/
#define OCTAL_PORT_MODE_CONFIG_1_REG_0_MIXED_MODE_SHIFT 10
#define OCTAL_PORT_MODE_CONFIG_1_REG_0_RSVD_MASK 0x00000300 /*!< RSVD*/
#define OCTAL_PORT_MODE_CONFIG_1_REG_0_RSVD_SHIFT 8
#define OCTAL_PORT_MODE_CONFIG_1_REG_0_PORT_MODE_MASK 0x000000FF /*!< PORT MODE*/
#define OCTAL_PORT_MODE_CONFIG_1_REG_0_PORT_MODE_SHIFT 0


/****************************************************************************
 * PORT Mode Conifg1 Registers: octal_core.GP0_REGS [8,10,12,14,16,18,20,22]
 ***************************************************************************/
typedef union octal_port_mode_config1_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t port_vld:1;   /*!<0 - PORT config1/2 not valid; 1 - PORT config1/2 valid */
        uint16_t port_idx:3; /*!<HW/FW port idx */
        uint16_t mixed_mode:2; /*!<Mixed mode, 0 - Non-mixed mode; 1 - mixed mode for repeater and retimer; 2 - mixed mode repeater only */
        uint16_t rsvd:2;
        uint16_t port_mode_enum:8; /*!<HW/FW port config modes */
#else /* LITTLE ENDIAN */
        uint16_t port_mode_enum:8; /*!<HW/FW port config modes */
        uint16_t rsvd:2;
        uint16_t mixed_mode:2; /*!<Mixed mode, 0 - Non-mixed mode; 1 - mixed mode for repeater and retimer; 2 - mixed mode repeater only */
        uint16_t port_idx:3; /*!<HW/FW port idx */
        uint16_t port_vld:1;   /*!<0 - PORT config1/2 not valid; 1 - PORT config1/2 valid */
#endif
    } fields;
} octal_port_mode_config1_reg_t;


/****************************************************************************
 * PORT Mode Conifg2 Registers: octal_core.GP0_REGS [9,11,13,15,17,19,21,23]
 ***************************************************************************/
typedef union octal_port_mode_config2_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t independent_fec : 1; /*!<Independent FEC (0-EGR/IGR FEC type is same;  1-EGR and IGR FEC type is different) */
        uint16_t igr_fec_term:4; /*!<0-NO_FEC; 1--FEC_DEC_FWD; 2 -- FEC_DEC_ENC; 3--PCS_XENC; 4--FEC_DEC_XDEC_XENC_ENC*/
        uint16_t egr_fec_term:4; /*!<0-NO_FEC; 1--FEC_DEC_FWD; 2 -- FEC_DEC_ENC; 3--PCS_XENC; 4--FEC_DEC_XDEC_XENC_ENC*/
        uint16_t line_symbol_mux:1;
        uint16_t line_fec_type:3; /*!<line FEC type (0-NO_FEC;  1-RS528; 2-RS544; 3-INNER) */
        uint16_t host_fec_type:3; /*!<Host FEC Type (0-NO_FEC; 1-RS528; 2-RS544; 3-PCS ) */
#else /* LITTLE ENDIAN */
        uint16_t host_fec_type:3; /*!<Host FEC Type (0-NO_FEC; 1-RS528; 2-RS544; 3-PCS ) */
        uint16_t line_fec_type:3;  /*!<line FEC type (0-NO_FEC;  1-RS528; 2-RS544; 3-INNER) */
        uint16_t line_symbol_mux:1;       
        uint16_t egr_fec_term:4; /*!<0-NO_FEC; 1--FEC_DEC_FWD; 2 -- FEC_DEC_ENC; 3--PCS_XENC; 4--FEC_DEC_XDEC_XENC_ENC*/
        uint16_t igr_fec_term:4; /*!<0-NO_FEC; 1--FEC_DEC_FWD; 2 -- FEC_DEC_ENC; 3--PCS_XENC; 4--FEC_DEC_XDEC_XENC_ENC*/
        uint16_t independent_fec : 1; /*!<Independent FEC (0-EGR/IGR FEC type is same;  1-EGR and IGR FEC type is different) */
#endif
    } fields;
} octal_port_mode_config2_reg_t;

/****************************************************************************
 * PORT Mode Status1 Registers: OCTAL_PORT_MODE_STATUS_1_REG
 ***************************************************************************/
#define OCTAL_PORT_MODE_STATUS_1_REG_PORT_VLD_MASK 0x00008000 /*!< PORT Valid*/
#define OCTAL_PORT_MODE_STATUS_1_REG_PORT_VLD_SHIFT 15
#define OCTAL_PORT_MODE_STATUS_1_REG_RSVD_MASK 0x00007800 /*!< RSVD*/
#define OCTAL_PORT_MODE_STATUS_1_REG_RSVD_SHIFT 8
#define OCTAL_PORT_MODE_STATUS_1_REG_CONFIG_STATUS_MASK 0x00000700 /*!< Port config status*/
#define OCTAL_PORT_MODE_STATUS_1_REG_CONFIG_STATUS_SHIFT 8
#define OCTAL_PORT_MODE_STATUS_1_REG_PORT_MODE_MASK 0x000000FF /*!< PORT MODE*/
#define OCTAL_PORT_MODE_STATUS_1_REG_PORT_MODE_SHIFT 0


/****************************************************************************
 * PORT Mode Conifg2 Registers: OCTAL_PORT_MODE_STATUS_2_REG
 ***************************************************************************/
#define OCTAL_PORT_MODE_STATUS_2_REG_RSVD_MASK 0x0000C000 /*!< RSVD*/
#define OCTAL_PORT_MODE_STATUS_2_REG_RSVD_SHIFT 14
#define OCTAL_PORT_MODE_STATUS_2_REG_POWER_DOWN_ST_MASK 0x000002000 /*!< POWER_DOWN_ST*/
#define OCTAL_PORT_MODE_STATUS_2_REG_POWER_DOWN_ST_SHIFT 13
#define OCTAL_PORT_MODE_STATUS_2_REG_PORT_IDX_MASK 0x00001C00 /*!< PORT_IDX*/
#define OCTAL_PORT_MODE_STATUS_2_REG_PORT_IDX_SHIFT 10
#define OCTAL_PORT_MODE_STATUS_2_REG_FEC_TERM_MASK 0x000003c0 /*!< FEC_TERM*/
#define OCTAL_PORT_MODE_STATUS_2_REG_FEC_TERM_SHIFT 6
#define OCTAL_PORT_MODE_STATUS_2_REG_LINE_FEC_TYPE_MASK 0x00000038 /*!< LINE_FEC_TYPE*/
#define OCTAL_PORT_MODE_STATUS_2_REG_LINE_FEC_TYPE_SHIFT 3
#define OCTAL_PORT_MODE_STATUS_2_REG_HOST_FEC_TYPE_MASK 0x00000007 /*!< HOST_FEC_TYPE*/
#define OCTAL_PORT_MODE_STATUS_2_REG_HOST_FEC_TYPE_SHIFT 0



/****************************************************************************
 * PORT Mode Conifg3 Registers: OCTAL_PORT_MODE_STATUS_3_REG
 ***************************************************************************/
#define OCTAL_PORT_MODE_STATUS_3_REG_HOST_LANE_MASK_MASK 0x0000FFFF /*!< Host lane mask*/
#define OCTAL_PORT_MODE_STATUS_3_REG_HOST_LANE_MASK_SHIFT 0


/****************************************************************************
 * PORT Mode Conifg4 Registers: OCTAL_PORT_MODE_STATUS_4_REG
 ***************************************************************************/
#define OCTAL_PORT_MODE_STATUS_3_REG_LINE_LANE_MASK_MASK 0x0000FFFF /*!< Line lane mask*/
#define OCTAL_PORT_MODE_STATUS_3_REG_LINE_LANE_MASK_SHIFT 0


/****************************************************************************
 * PORT Mode Status1 Registers: OCTAL_PORT_MODE_CFG_LOG
 ***************************************************************************/
#define OCTAL_PORT_MODE_CFG_LOG_CFG_RESULT_MASK             0xF000
#define OCTAL_PORT_MODE_CFG_LOG_CFG_RESULT_SHIFT            12
#define OCTAL_PORT_MODE_CFG_LOG_ADD_PORT_TIME_OUT_MASK      0x200
#define OCTAL_PORT_MODE_CFG_LOG_ADD_PORT_TIME_OUT_SHIFT     9
#define OCTAL_PORT_MODE_CFG_LOG_CMD_STATUS_MASK             0x1E0
#define OCTAL_PORT_MODE_CFG_LOG_CMD_STATUS_SHIFT            5
#define OCTAL_PORT_MODE_CFG_LOG_TOP_STATUS_MASK             0x1F
#define OCTAL_PORT_MODE_CFG_LOG_TOP_STATUS_SHIFT            0
/****************************************************************************
 * PORT Mode Status1 Registers: OCTAL_PORT_MODE_STATUS_1_REG
 ***************************************************************************/
typedef union octal_port_mode_status1_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t port_vld:1;   /*!<0 - PORT config1/2 not valid; 1 - PORT config1/2 valid */
        uint16_t rsvd:4; 
        uint16_t port_config_status:3; /*!< 0- not configured, 1- config in progress, 2- config done, 3-Config failed */
        uint16_t port_mode_enum:8; /*!<HW/FW port config modes */
            
#else /* LITTLE ENDIAN */
        uint16_t port_mode_enum:8; /*!<HW/FW port config modes */
        uint16_t port_config_status:3; /*!< 0- not configured, 1- config in progress, 2- config done, 3-Config failed */
         uint16_t rsvd:4; 
         uint16_t port_vld:1;    /*!<0 - PORT config1/2 not valid; 1 - PORT config1/2 valid */
           
#endif
    } fields;
} octal_port_mode_status1_reg_t;
    


/****************************************************************************
 * PORT Mode Status2 Registers: OCTAL_PORT_MODE_STATUS_2_REG
 ***************************************************************************/
typedef union octal_port_mode_status2_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t rsvd:1;
        uint16_t line_symbol_mux:1;
        uint16_t power_down_status:1;
        uint16_t port_index:3;
        uint16_t fec_term:4; /*!<0-NO_FEC; 1--FEC_DEC_FWD; 2 -- FEC_DEC_ENC; 3--PCS_XENC; 4--FEC_DEC_XDEC_XENC_ENC*/
        uint16_t line_fec_type:3; /*!<line FEC type (0-NO_FEC;  1-RS528; 2-RS544; 3-INNER) */
        uint16_t host_fec_type:3; /*!<Host FEC Type (0-NO_FEC; 1-RS528; 2-RS544; 3-PCS ) */
#else /* LITTLE ENDIAN */
        uint16_t host_fec_type:3; /*!<Host FEC Type (0-NO_FEC; 1-RS528; 2-RS544; 3-PCS ) */
        uint16_t line_fec_type:3; /*!<line FEC type (0-NO_FEC;  1-RS528; 2-RS544; 3-INNER) */
        uint16_t fec_term :4; /*!<0-NO_FEC; 1--FEC_DEC_FWD; 2 -- FEC_DEC_ENC; 3--PCS_XENC; 4--FEC_DEC_XDEC_XENC_ENC*/
        uint16_t port_index:3;
        uint16_t power_down_status:1;
        uint16_t line_symbol_mux:1;
        uint16_t rsvd:1; 
#endif
    } fields;
} octal_port_mode_status2_reg_t;

/****************************************************************************
 * PORT Mode Status3 Registers: OCTAL_PORT_MODE_STATUS_3_REG
 ***************************************************************************/
typedef union octal_port_mode_status3_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t host_lane_mask:16; /*!<host lane mask */
#else /* LITTLE ENDIAN */
        uint16_t host_lane_mask:16; /*!<host lane mask */
#endif
    } fields;
} octal_port_mode_status3_reg_t;

/****************************************************************************
 * PORT Mode Status4 Registers: OCTAL_PORT_MODE_STATUS_4_REG
 ***************************************************************************/
typedef union octal_port_mode_status4_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t rsvd:8;
        uint16_t line_lane_mask:8;
#else /* LITTLE ENDIAN */
        uint16_t line_lane_mask:8;
        uint16_t rsvd:8;
#endif
    } fields;
} octal_port_mode_status4_reg_t;


typedef struct config_mode_st_s {
    octal_port_mode_status1_reg_t pmode_st1;
    octal_port_mode_status2_reg_t pmode_st2;
    octal_port_mode_status3_reg_t pmode_st3;
    octal_port_mode_status4_reg_t pmode_st4;
}config_mode_st_t;

/****************************************************************************
 * blackhawk_estoque_com_amba :: CHIP_TOP_CHIP_FIRMWARE_INTERNAL1_REG 
 ***************************************************************************/
typedef union chip_top_chip_firmware_interal1_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t fw_sub_ver_num : 4;
        uint16_t tdb : 8; /*!<TDB */
        uint16_t dw_pending : 1; /*!<dw_pending */
        uint16_t bh_pending : 1; /*!<bh_pending */
        uint16_t lw_pending : 1; /*!<lw_pending */
        uint16_t single_or_dual : 1; /*!<single or dual m0 */
#else /* LITTLE ENDIAN */
        uint16_t single_or_dual : 1; /*!<single or dual m0 */
        uint16_t lw_pending : 1; /*!<lw_pending */
        uint16_t bh_pending : 1; /*!<bh_pending */
        uint16_t dw_pending : 1; /*!<dw_pending */
        uint16_t tdb : 8; /*!<TDB */
        uint16_t fw_sub_ver_num : 4;
#endif
    } fields;
} chip_top_chip_firmware_interal1_reg_t;

typedef union ocw_default_mode_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t tdb : 1; /*!<TDB */
        uint16_t port_mask : 8; /*!< 0 or 0xFF -ALL ports, 0x1- port starting on lw0, 0x2- on lw1, 0x4- lw2, 8-on lw3 */
        uint16_t no_surge:1;
        uint16_t avs_enable : 1; /*! enable AVS or not */
        uint16_t mode_config : 3; /*!<config default mode (  1 : CW_DEFAULT_MODE_1_REPEATER 2: CW_DEFAULT_MODE_1_KP4_KP4 3:CW_DEFAULT_MODE_5_PCS_KP4 4:CW_DEFAULT_MODE_5_KR4_KP4 5:CW_DEFAULT_MODE_4_NRZ_NRZ_REPEATER */
        uint16_t pre_programmed : 1; /*!<no used. use defafult config from pre-programmed config1 and config2 registers. */
        uint16_t skip : 1; /*!<Skip default mode */
#else /* LITTLE ENDIAN */
        uint16_t skip : 1; /*!<Skip default mode */
        uint16_t pre_programmed : 1; /*!<use defafult config from pre-programmed config1 and config2 registers. */
        uint16_t mode_config : 3; /*!<config default mode (  1 : CW_DEFAULT_MODE_1_REPEATER 2: CW_DEFAULT_MODE_1_KP4_KP4 3:CW_DEFAULT_MODE_5_PCS_KP4 4:CW_DEFAULT_MODE_5_KR4_KP4 5:CW_DEFAULT_MODE_4_NRZ_NRZ_REPEATER */
        uint16_t avs_enable : 1; /*! enable AVS or not */
        uint16_t no_surge:1;
        uint16_t port_mask : 8; /*!< 0 or 0xFF -ALL ports, 0x1- port starting on lw0, 0x2- on lw1, 0x4- lw2, 8-on lw3 */
        uint16_t tdb : 1; /*!<TDB */
#endif
    } fields;
} ocw_default_mode_reg_t;

typedef union uc_ready_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t tdb : 12; /*!<TDB */
        uint16_t bh0_3 : 1; /*!<Host side 0 to 3 */
        uint16_t lw2_3 : 1; /*!<Line side 2 to 3 */
        uint16_t lw0_1 : 1; /*!<Line side 0 to 1 */
        uint16_t oc : 1; /*!<Octal Core ready */
#else /* LITTLE ENDIAN */
        uint16_t oc : 1; /*!<Octal Core ready */
        uint16_t lw0_1 : 1; /*!<Line side 0 to 1 */
        uint16_t lw2_3 : 1; /*!<Line side 2 to 3 */
        uint16_t bh0_3 : 1; /*!<Host side 0 to 3 */
        uint16_t tdb : 12; /*!<TDB */
#endif
    } fields;
} uc_ready_reg_t;



/****************************************************************************
 * #define OCTAL_COMMON_TEST_REG   QUAD_CORE_CONFIG_CFGVAL_46_RDB
 ***************************************************************************/
#define OCTAL_COMMON_TEST_REG_START_ST_CLEAN_MASK  0x00004000/*!< START_ENABLE_ST_CLEAN*/
#define OCTAL_COMMON_TEST_REG_START_ST_CLEAN_SHIFT 14

#define OCTAL_COMMON_TEST_REG_ATE_LINE_SINGLE_LANE_TEST_MASK 0x00000FF0/*!< ATE_LINE_SINGLE_LANE_TEST*/
#define OCTAL_COMMON_TEST_REG_ATE_LINE_SINGLE_LANE_TEST_SHIFT 4

#define OCTAL_COMMON_TEST_REG_ATE_DISABLE_DSP_TASK_MASK 0x00000004/*!< ATE_DISABLE_DSP_TASK*/
#define OCTAL_COMMON_TEST_REG_ATE_DISABLE_DSP_TASK_SHIFT 2
#define OCTAL_COMMON_TEST_REG_ATE_VMIN_TEST_MASK 0x00000002/*!< ATE_VMIN_TEST*/
#define OCTAL_COMMON_TEST_REG_ATE_VMIN_TEST_SHIFT 1

#define OCTAL_COMMON_TEST_REG_ATE_TEST_MASK 0x00000001/*!< ATE_TEST*/
#define OCTAL_COMMON_TEST_REG_ATE_TEST_SHIFT 0

/****************************************************************************
 * #define OCTAL_COMMON_TEST_ATE_CFG_REG   QUAD_CORE_CONFIG_CFGVAL_47_RDB
 ***************************************************************************/
#define OCTAL_COMMON_TEST_ATE_CFG_REG_CFG_ENABLE_ST_CLEAN_MASK      0x00008000/*!< CFG_ENABLE_ST_CLEAN*/
#define OCTAL_COMMON_TEST_ATE_CFG_REG_CFG_ENABLE_ST_CLEAN_SHIFT     15

#define OCTAL_COMMON_TEST_ATE_CFG_REG_BER_WAIT_TIMER_MASK           0x00007000/*!< BER_WAIT_TIMER*/
#define OCTAL_COMMON_TEST_ATE_CFG_REG_BER_WAIT_TIMER_SHIFT          12
#define OCTAL_COMMON_TEST_ATE_CFG_REG_CDR_WAIT_TIMER_MASK           0x00000F00/*!< CDR_WAIT_TIMER*/
#define OCTAL_COMMON_TEST_ATE_CFG_REG_CDR_WAIT_TIMER_SHIFT          8

#define OCTAL_COMMON_TEST_ATE_CFG_REG_DISABLE_NEG_PPM_TEST_MASK     0x00000002/*!< DISABLE_NEG_PPM_TEST*/
#define OCTAL_COMMON_TEST_ATE_CFG_REG_DISABLE_NEG_PPM_TEST_SHIFT    1
#define OCTAL_COMMON_TEST_ATE_CFG_REG_DISABLE_POS_PPM_TEST_MASK     0x00000001/*!< DISABLE_POS_PPM_TEST*/
#define OCTAL_COMMON_TEST_ATE_CFG_REG_DISABLE_POS_PPM_TEST_SHIFT    0

/****************************************************************************
 * #define OCTAL_COMMON_TEST_LINE_PPM_VALUE_CFG   QUAD_CORE_CONFIG_CFGVAL_48_RDB
 ***************************************************************************/
#define OCTAL_COMMON_TEST_LINE_PPM_VALUE_CFG_TX_FORCE_PPM_VALUE_MASK 0x000003FF/*!< TX_FORCE_PPM_VALUE*/
#define OCTAL_COMMON_TEST_LINE_PPM_VALUE_CFG_TX_FORCE_PPM_VALUE_SHIFT 0

#define OCTAL_COMMON_TEST_HOST_PPM_VALUE_CFG_TX_FORCE_PPM_VALUE_MASK 0x000003FF/*!< TX_FORCE_PPM_VALUE*/
#define OCTAL_COMMON_TEST_HOST_PPM_VALUE_CFG_TX_FORCE_PPM_VALUE_SHIFT 0

/****************************************************************************
 * #define OCTAL_COMMON_TEST_ATE_ST_REG   QUAD_CORE_CONFIG_CFGVAL_50_RDB
 ***************************************************************************/
#define OCTAL_COMMON_TEST_ATE_ST_REG_ATE_FSM_STATUS_MASK 0x00000FF00/*!< ATE_FSM_STATUS*/
#define OCTAL_COMMON_TEST_ATE_ST_REG_ATE_FSM_STATUS_SHIFT 8

#define OCTAL_COMMON_TEST_ATE_ST_REG_ATE_VMIN_DONE_FLAG_MASK 0x00000010/*!< ATE_VMIN_DONE_FLAG*/
#define OCTAL_COMMON_TEST_ATE_ST_REG_ATE_VMIN_DONE_FLAG_SHIFT 4

#define OCTAL_COMMON_TEST_ATE_ST_REG_ATE_RESULT_MASK 0x0000000F/*!< ATE_RESULT*/
#define OCTAL_COMMON_TEST_ATE_ST_REG_ATE_RESULT_SHIFT 0


/****************************************************************************
 * #define OCTAL_COMMON_TEST_LINE_PRBS_POS_ST0       QUAD_CORE_CONFIG_CFGVAL_51_RDB
 ***************************************************************************/
#define OCTAL_COMMON_TEST_LINE_PRBS_POS_ST0_PORT3_LW_ATE_STATUS_MASK 0x0000F000/*!< PORT3_ATE_STATUS*/
#define OCTAL_COMMON_TEST_LINE_PRBS_POS_ST0_PORT3_LW_ATE_STATUS_SHIFT 12
#define OCTAL_COMMON_TEST_LINE_PRBS_POS_ST0_PORT2_LW_ATE_STATUS_MASK 0x00000F00/*!< PORT2_ATE_STATUS*/
#define OCTAL_COMMON_TEST_LINE_PRBS_POS_ST0_PORT2_LW_ATE_STATUS_SHIFT 8
#define OCTAL_COMMON_TEST_LINE_PRBS_POS_ST0_PORT1_LW_ATE_STATUS_MASK 0x000000F0/*!< PORT1_ATE_STATUS*/
#define OCTAL_COMMON_TEST_LINE_PRBS_POS_ST0_PORT1_LW_ATE_STATUS_SHIFT 4
#define OCTAL_COMMON_TEST_LINE_PRBS_POS_ST0_PORT0_LW_ATE_STATUS_MASK 0x0000000F/*!< PORT0_ATE_STATUS*/
#define OCTAL_COMMON_TEST_LINE_PRBS_POS_ST0_PORT0_LW_ATE_STATUS_SHIFT 0

/****************************************************************************
 * #define OCTAL_COMMON_TEST_LINE_PRBS_POS_ST1       QUAD_CORE_CONFIG_CFGVAL_52_RDB
 ***************************************************************************/
#define OCTAL_COMMON_TEST_LINE_PRBS_POS_ST1_PORT7_LW_ATE_STATUS_MASK 0x0000F000/*!< PORT7_ATE_STATUS*/
#define OCTAL_COMMON_TEST_LINE_PRBS_POS_ST1_PORT7_LW_ATE_STATUS_SHIFT 12
#define OCTAL_COMMON_TEST_LINE_PRBS_POS_ST1_PORT6_LW_ATE_STATUS_MASK 0x00000F00/*!< PORT6_ATE_STATUS*/
#define OCTAL_COMMON_TEST_LINE_PRBS_POS_ST1_PORT6_LW_ATE_STATUS_SHIFT 8
#define OCTAL_COMMON_TEST_LINE_PRBS_POS_ST1_PORT5_LW_ATE_STATUS_MASK 0x000000F0/*!< PORT5_ATE_STATUS*/
#define OCTAL_COMMON_TEST_LINE_PRBS_POS_ST1_PORT5_LW_ATE_STATUS_SHIFT 4
#define OCTAL_COMMON_TEST_LINE_PRBS_POS_ST1_PORT4_LW_ATE_STATUS_MASK 0x0000000F/*!< PORT4_ATE_STATUS*/
#define OCTAL_COMMON_TEST_LINE_PRBS_POS_ST1_PORT4_LW_ATE_STATUS_SHIFT 0

/****************************************************************************
 * #define OCTAL_COMMON_TEST_LINE_PRBS_NEG_ST0       QUAD_CORE_CONFIG_CFGVAL_53_RDB
 ***************************************************************************/
#define OCTAL_COMMON_TEST_LINE_PRBS_NEG_ST0_PORT3_LW_ATE_STATUS_MASK 0x0000F000/*!< PORT3_ATE_STATUS*/
#define OCTAL_COMMON_TEST_LINE_PRBS_NEG_ST0_PORT3_LW_ATE_STATUS_SHIFT 12
#define OCTAL_COMMON_TEST_LINE_PRBS_NEG_ST0_PORT2_LW_ATE_STATUS_MASK 0x00000F00/*!< PORT2_ATE_STATUS*/
#define OCTAL_COMMON_TEST_LINE_PRBS_NEG_ST0_PORT2_LW_ATE_STATUS_SHIFT 8
#define OCTAL_COMMON_TEST_LINE_PRBS_NEG_ST0_PORT1_LW_ATE_STATUS_MASK 0x000000F0/*!< PORT1_ATE_STATUS*/
#define OCTAL_COMMON_TEST_LINE_PRBS_NEG_ST0_PORT1_LW_ATE_STATUS_SHIFT 4
#define OCTAL_COMMON_TEST_LINE_PRBS_NEG_ST0_PORT0_LW_ATE_STATUS_MASK 0x0000000F/*!< PORT0_ATE_STATUS*/
#define OCTAL_COMMON_TEST_LINE_PRBS_NEG_ST0_PORT0_LW_ATE_STATUS_SHIFT 0

/****************************************************************************
 * #define OCTAL_COMMON_TEST_LINE_PRBS_NEG_ST1       QUAD_CORE_CONFIG_CFGVAL_54_RDB
 ***************************************************************************/
#define OCTAL_COMMON_TEST_LINE_PRBS_NEG_ST1_PORT7_LW_ATE_STATUS_MASK 0x0000F000/*!< PORT7_ATE_STATUS*/
#define OCTAL_COMMON_TEST_LINE_PRBS_NEG_ST1_PORT7_LW_ATE_STATUS_SHIFT 12
#define OCTAL_COMMON_TEST_LINE_PRBS_NEG_ST1_PORT6_LW_ATE_STATUS_MASK 0x00000F00/*!< PORT6_ATE_STATUS*/
#define OCTAL_COMMON_TEST_LINE_PRBS_NEG_ST1_PORT6_LW_ATE_STATUS_SHIFT 8
#define OCTAL_COMMON_TEST_LINE_PRBS_NEG_ST1_PORT5_LW_ATE_STATUS_MASK 0x000000F0/*!< PORT5_ATE_STATUS*/
#define OCTAL_COMMON_TEST_LINE_PRBS_NEG_ST1_PORT5_LW_ATE_STATUS_SHIFT 4
#define OCTAL_COMMON_TEST_LINE_PRBS_NEG_ST1_PORT4_LW_ATE_STATUS_MASK 0x0000000F/*!< PORT4_ATE_STATUS*/
#define OCTAL_COMMON_TEST_LINE_PRBS_NEG_ST1_PORT4_LW_ATE_STATUS_SHIFT 0

/****************************************************************************
 * #define OCTAL_COMMON_TEST_HOST_PRBS_POS_ST0       QUAD_CORE_CONFIG_CFGVAL_55_RDB
 ***************************************************************************/
#define OCTAL_COMMON_TEST_HOST_PRBS_POS_ST0_PORT3_BH_ATE_STATUS_MASK 0x0000F000/*!< PORT3_ATE_STATUS*/
#define OCTAL_COMMON_TEST_HOST_PRBS_POS_ST0_PORT3_BH_ATE_STATUS_SHIFT 12
#define OCTAL_COMMON_TEST_HOST_PRBS_POS_ST0_PORT2_BH_ATE_STATUS_MASK 0x00000F00/*!< PORT2_ATE_STATUS*/
#define OCTAL_COMMON_TEST_HOST_PRBS_POS_ST0_PORT2_BH_ATE_STATUS_SHIFT 8
#define OCTAL_COMMON_TEST_HOST_PRBS_POS_ST0_PORT1_BH_ATE_STATUS_MASK 0x000000F0/*!< PORT1_ATE_STATUS*/
#define OCTAL_COMMON_TEST_HOST_PRBS_POS_ST0_PORT1_BH_ATE_STATUS_SHIFT 4
#define OCTAL_COMMON_TEST_HOST_PRBS_POS_ST0_PORT0_BH_ATE_STATUS_MASK 0x0000000F/*!< PORT0_ATE_STATUS*/
#define OCTAL_COMMON_TEST_HOST_PRBS_POS_ST0_PORT0_BH_ATE_STATUS_SHIFT 0


/****************************************************************************
 * #define OCTAL_COMMON_TEST_HOST_PRBS_POS_ST1       QUAD_CORE_CONFIG_CFGVAL_56_RDB
 ***************************************************************************/
#define OCTAL_COMMON_TEST_HOST_PRBS_POS_ST1_PORT7_LW_ATE_STATUS_MASK 0x0000F000/*!< PORT7_ATE_STATUS*/
#define OCTAL_COMMON_TEST_HOST_PRBS_POS_ST1_PORT7_LW_ATE_STATUS_SHIFT 12
#define OCTAL_COMMON_TEST_HOST_PRBS_POS_ST1_PORT6_LW_ATE_STATUS_MASK 0x00000F00/*!< PORT6_ATE_STATUS*/
#define OCTAL_COMMON_TEST_HOST_PRBS_POS_ST1_PORT6_LW_ATE_STATUS_SHIFT 8
#define OCTAL_COMMON_TEST_HOST_PRBS_POS_ST1_PORT5_LW_ATE_STATUS_MASK 0x000000F0/*!< PORT5_ATE_STATUS*/
#define OCTAL_COMMON_TEST_HOST_PRBS_POS_ST1_PORT5_LW_ATE_STATUS_SHIFT 4
#define OCTAL_COMMON_TEST_HOST_PRBS_POS_ST1_PORT4_LW_ATE_STATUS_MASK 0x0000000F/*!< PORT4_ATE_STATUS*/
#define OCTAL_COMMON_TEST_HOST_PRBS_POS_ST1_PORT4_LW_ATE_STATUS_SHIFT 0

/****************************************************************************
 * #define OCTAL_COMMON_TEST_HOST_PRBS_NEG_ST0       QUAD_CORE_CONFIG_CFGVAL_57_RDB
 ***************************************************************************/
#define OCTAL_COMMON_TEST_HOST_PRBS_NEG_ST0_PORT3_BH_ATE_STATUS_MASK 0x0000F000/*!< PORT3_ATE_STATUS*/
#define OCTAL_COMMON_TEST_HOST_PRBS_NEG_ST0_PORT3_BH_ATE_STATUS_SHIFT 12
#define OCTAL_COMMON_TEST_HOST_PRBS_NEG_ST0_PORT2_BH_ATE_STATUS_MASK 0x00000F00/*!< PORT2_ATE_STATUS*/
#define OCTAL_COMMON_TEST_HOST_PRBS_NEG_ST0_PORT2_BH_ATE_STATUS_SHIFT 8
#define OCTAL_COMMON_TEST_HOST_PRBS_NEG_ST0_PORT1_BH_ATE_STATUS_MASK 0x000000F0/*!< PORT1_ATE_STATUS*/
#define OCTAL_COMMON_TEST_HOST_PRBS_NEG_ST0_PORT1_BH_ATE_STATUS_SHIFT 4
#define OCTAL_COMMON_TEST_HOST_PRBS_NEG_ST0_PORT0_BH_ATE_STATUS_MASK 0x0000000F/*!< PORT0_ATE_STATUS*/
#define OCTAL_COMMON_TEST_HOST_PRBS_NEG_ST0_PORT0_BH_ATE_STATUS_SHIFT 0


/****************************************************************************
 * #define OCTAL_COMMON_TEST_HOST_PRBS_NEG_ST1       QUAD_CORE_CONFIG_CFGVAL_58_RDB
 ***************************************************************************/
#define OCTAL_COMMON_TEST_HOST_PRBS_NEG_ST1_PORT7_LW_ATE_STATUS_MASK 0x0000F000/*!< PORT7_ATE_STATUS*/
#define OCTAL_COMMON_TEST_HOST_PRBS_NEG_ST1_PORT7_LW_ATE_STATUS_SHIFT 12
#define OCTAL_COMMON_TEST_HOST_PRBS_NEG_ST1_PORT6_LW_ATE_STATUS_MASK 0x00000F00/*!< PORT6_ATE_STATUS*/
#define OCTAL_COMMON_TEST_HOST_PRBS_NEG_ST1_PORT6_LW_ATE_STATUS_SHIFT 8
#define OCTAL_COMMON_TEST_HOST_PRBS_NEG_ST1_PORT5_LW_ATE_STATUS_MASK 0x000000F0/*!< PORT5_ATE_STATUS*/
#define OCTAL_COMMON_TEST_HOST_PRBS_NEG_ST1_PORT5_LW_ATE_STATUS_SHIFT 4
#define OCTAL_COMMON_TEST_HOST_PRBS_NEG_ST1_PORT4_LW_ATE_STATUS_MASK 0x0000000F/*!< PORT4_ATE_STATUS*/
#define OCTAL_COMMON_TEST_HOST_PRBS_NEG_ST1_PORT4_LW_ATE_STATUS_SHIFT 0

/****************************************************************************
#define OCTAL_COMMON_TEST_LINE_BER_CNT_CFG             QUAD_CORE_CONFIG_CFGVAL_59_RDB
 ***************************************************************************/
#define OCTAL_COMMON_TEST_LINE_BER_CNT_CFG_LINE_BER_TH_MASK 0x0000FFFF/*!< LINE_BER_TH*/
#define OCTAL_COMMON_TEST_LINE_BER_CNT_CFG_LINE_BER_TH_SHIFT 0

/****************************************************************************
#define OCTAL_COMMON_TEST_HOST_BER_CNT_CFG             QUAD_CORE_CONFIG_CFGVAL_60_RDB
 ***************************************************************************/
#define OCTAL_COMMON_TEST_HOST_BER_CNT_CFG_HOST_BER_TH_MASK 0x0000FFFF/*!< HOST_BER_TH*/
#define OCTAL_COMMON_TEST_HOST_BER_CNT_CFG_HOST_BER_TH_SHIFT 0

/****************************************************************************
#define OCTAL_COMMON_VMIN_CTRL                           QUAD_CORE_CONFIG_CFGVAL_64_RDB 
 ***************************************************************************/
#define OCTAL_COMMON_VMIN_CTRL_MANUAL_CTRL_MASK            0x0008000/*!< MANUAL_FLAG*/
#define OCTAL_COMMON_VMIN_CTRL_MANUAL_CTRL_SHIFT           15
#define OCTAL_COMMON_VMIN_CTRL_MANUAL_FLAG_MASK            0x0004000/*!< MANUAL_FLAG*/
#define OCTAL_COMMON_VMIN_CTRL_MANUAL_FLAG_SHIFT           14
#define OCTAL_COMMON_VMIN_CTRL_DISABLE_TRAFFIC_MASK        0x0001000/*!< DISABLE_TRAFFIC*/
#define OCTAL_COMMON_VMIN_CTRL_DISABLE_TRAFFIC_SHIFT       12
#define OCTAL_COMMON_VMIN_CTRL_DROP_MAX_STEP_MASK          0x000F80/*!< DROP_MAX_STEP*/
#define OCTAL_COMMON_VMIN_CTRL_DROP_MAX_STEP_SHIFT         7
#define OCTAL_COMMON_VMIN_CTRL_CFG_VOLTAGE_DIFF_TOL_MASK   0x000070/*!< CFG_VOLTAGE_DIFF_TOL*/
#define OCTAL_COMMON_VMIN_CTRL_CFG_VOLTAGE_DIFF_TOL_SHIFT  4

/****************************************************************************
#define OCTAL_COMMON_VMIN_ST                           QUAD_CORE_CONFIG_CFGVAL_65_RDB 
 ***************************************************************************/
#define OCTAL_COMMON_VMIN_ST_TEST_FAIL_REASON_MASK       0x0000700/*!< TEST_FAIL_REASON*/
#define OCTAL_COMMON_VMIN_ST_TEST_FAIL_REASON_SHIFT      8
#define OCTAL_COMMON_VMIN_ST_TEST_CYCLE_MASK             0x000000FF/*!< TEST_CYCLE*/
#define OCTAL_COMMON_VMIN_ST_TEST_CYCLE_SHIFT            0

/****************************************************************************
 * #define OCW_EGRESS_FSM_SUSPEND_CFG                  QUAD_CORE_GPCTRL_RDB_GPCTRL_364_RDB
 ***************************************************************************/
#define OCW_EGRESS_FSM_SUSPEND_CFG_PORT3_SUSPEND_ST_MASK  0xE000
#define OCW_EGRESS_FSM_SUSPEND_CFG_PORT3_SUSPEND_ST_SHIFT  13
#define OCW_EGRESS_FSM_SUSPEND_CFG_PORT3_SUSPEND_MASK  0x1000
#define OCW_EGRESS_FSM_SUSPEND_CFG_PORT3_SUSPEND_SHIFT  12
#define OCW_EGRESS_FSM_SUSPEND_CFG_PORT2_SUSPEND_ST_MASK  0xE00
#define OCW_EGRESS_FSM_SUSPEND_CFG_PORT2_SUSPEND_ST_SHIFT  9
#define OCW_EGRESS_FSM_SUSPEND_CFG_PORT2_SUSPEND_MASK  0x100
#define OCW_EGRESS_FSM_SUSPEND_CFG_PORT2_SUSPEND_SHIFT  8
#define OCW_EGRESS_FSM_SUSPEND_CFG_PORT1_SUSPEND_ST_MASK  0xE0
#define OCW_EGRESS_FSM_SUSPEND_CFG_PORT1_SUSPEND_ST_SHIFT  5
#define OCW_EGRESS_FSM_SUSPEND_CFG_PORT1_SUSPEND_MASK  0x10
#define OCW_EGRESS_FSM_SUSPEND_CFG_PORT1_SUSPEND_SHIFT  4
#define OCW_EGRESS_FSM_SUSPEND_CFG_PORT0_SUSPEND_ST_MASK  0xE
#define OCW_EGRESS_FSM_SUSPEND_CFG_PORT0_SUSPEND_ST_SHIFT  1
#define OCW_EGRESS_FSM_SUSPEND_CFG_PORT0_SUSPEND_MASK  0x1
#define OCW_EGRESS_FSM_SUSPEND_CFG_PORT0_SUSPEND_SHIFT  0

/****************************************************************************
 * #define OCW_EGRESS_FSM_SUSPEND_CFG                  QUAD_CORE_GPCTRL_RDB_GPCTRL_364_RDB
 ***************************************************************************/
#define OCW_EGRESS47_FSM_SUSPEND_CFG_PORT7_SUSPEND_ST_MASK  0xE000
#define OCW_EGRESS47_FSM_SUSPEND_CFG_PORT7_SUSPEND_ST_SHIFT  13
#define OCW_EGRESS47_FSM_SUSPEND_CFG_PORT7_SUSPEND_MASK  0x1000
#define OCW_EGRESS47_FSM_SUSPEND_CFG_PORT7_SUSPEND_SHIFT  12
#define OCW_EGRESS47_FSM_SUSPEND_CFG_PORT6_SUSPEND_ST_MASK  0xE00
#define OCW_EGRESS47_FSM_SUSPEND_CFG_PORT6_SUSPEND_ST_SHIFT  9
#define OCW_EGRESS47_FSM_SUSPEND_CFG_PORT6_SUSPEND_MASK  0x100
#define OCW_EGRESS47_FSM_SUSPEND_CFG_PORT6_SUSPEND_SHIFT  8
#define OCW_EGRESS47_FSM_SUSPEND_CFG_PORT5_SUSPEND_ST_MASK  0xE0
#define OCW_EGRESS47_FSM_SUSPEND_CFG_PORT5_SUSPEND_ST_SHIFT  5
#define OCW_EGRESS47_FSM_SUSPEND_CFG_PORT5_SUSPEND_MASK  0x10
#define OCW_EGRESS47_FSM_SUSPEND_CFG_PORT5_SUSPEND_SHIFT  4
#define OCW_EGRESS47_FSM_SUSPEND_CFG_PORT4_SUSPEND_ST_MASK  0xE
#define OCW_EGRESS47_FSM_SUSPEND_CFG_PORT4_SUSPEND_ST_SHIFT  1
#define OCW_EGRESS47_FSM_SUSPEND_CFG_PORT4_SUSPEND_MASK  0x1
#define OCW_EGRESS47_FSM_SUSPEND_CFG_PORT4_SUSPEND_SHIFT  0


/****************************************************************************
 * #define OCW_INGRESS_FSM_SUSPEND_CFG                  QUAD_CORE_GPCTRL_RDB_GPCTRL_365_RDB
  ***************************************************************************/
#define OCW_INGRESS_FSM_SUSPEND_CFG_PORT3_SUSPEND_ST_MASK  0xE000
#define OCW_INGRESS_FSM_SUSPEND_CFG_PORT3_SUSPEND_ST_SHIFT  13
#define OCW_INGRESS_FSM_SUSPEND_CFG_PORT3_SUSPEND_MASK  0x1000
#define OCW_INGRESS_FSM_SUSPEND_CFG_PORT3_SUSPEND_SHIFT  12
#define OCW_INGRESS_FSM_SUSPEND_CFG_PORT2_SUSPEND_ST_MASK  0xE00
#define OCW_INGRESS_FSM_SUSPEND_CFG_PORT2_SUSPEND_ST_SHIFT  9
#define OCW_INGRESS_FSM_SUSPEND_CFG_PORT2_SUSPEND_MASK  0x100
#define OCW_INGRESS_FSM_SUSPEND_CFG_PORT2_SUSPEND_SHIFT  8
#define OCW_INGRESS_FSM_SUSPEND_CFG_PORT1_SUSPEND_ST_MASK  0xE0
#define OCW_INGRESS_FSM_SUSPEND_CFG_PORT1_SUSPEND_ST_SHIFT  5
#define OCW_INGRESS_FSM_SUSPEND_CFG_PORT1_SUSPEND_MASK  0x10
#define OCW_INGRESS_FSM_SUSPEND_CFG_PORT1_SUSPEND_SHIFT  4
#define OCW_INGRESS_FSM_SUSPEND_CFG_PORT0_SUSPEND_ST_MASK  0xE
#define OCW_INGRESS_FSM_SUSPEND_CFG_PORT0_SUSPEND_ST_SHIFT  1
#define OCW_INGRESS_FSM_SUSPEND_CFG_PORT0_SUSPEND_MASK  0x1
#define OCW_INGRESS_FSM_SUSPEND_CFG_PORT0_SUSPEND_SHIFT  0


/****************************************************************************
 * #define OCW_PORT0_FSM_FAULT_FLAG      QUAD_CORE_GPCTRL_RDB_GPCTRL_320_RDB
  ***************************************************************************/
#define OCW_PORT0_FSM_FAULT_FLAG_IGR_FIFO_CLSN_MASK  0x8000
#define OCW_PORT0_FSM_FAULT_FLAG_IGR_FIFO_CLSN_SHIFT  15 
#define OCW_PORT0_FSM_FAULT_FLAG_IGR_TXCLK66_FLT_MASK  0x2000
#define OCW_PORT0_FSM_FAULT_FLAG_IGR_TXCLK66_FLT_SHIFT  13
#define OCW_PORT0_FSM_FAULT_FLAG_IGR_TXPI_CONV_FAIL_MASK  0x1000
#define OCW_PORT0_FSM_FAULT_FLAG_IGR_TXPI_CONV_FAIL_SHIFT  12
#define OCW_PORT0_FSM_FAULT_FLAG_IGR_OGBOX_CLSN_MASK  0x800
#define OCW_PORT0_FSM_FAULT_FLAG_IGR_OGBOX_CLSN_SHIFT  11
#define OCW_PORT0_FSM_FAULT_FLAG_IGR_FEC_LOL_MASK  0x400
#define OCW_PORT0_FSM_FAULT_FLAG_IGR_FEC_LOL_SHIFT  10
#define OCW_PORT0_FSM_FAULT_FLAG_IGR_IGBOX_CLSN_MASK  0x200
#define OCW_PORT0_FSM_FAULT_FLAG_IGR_IGBOX_CLSN_SHIFT  9
#define OCW_PORT0_FSM_FAULT_FLAG_IGR_LW_CDR_LOL_MASK  0x100
#define OCW_PORT0_FSM_FAULT_FLAG_IGR_LW_CDR_LOL_SHIFT  8

#define OCW_PORT0_FSM_FAULT_FLAG_EGR_FIFO_CLSN_MASK  0x80
#define OCW_PORT0_FSM_FAULT_FLAG_EGR_FIFO_CLSN_SHIFT  7
#define OCW_PORT0_FSM_FAULT_FLAG_EGR_RXCLK66_FLT_MASK  0x20
#define OCW_PORT0_FSM_FAULT_FLAG_EGR_RXCLK66_FLT_SHIFT  5
#define OCW_PORT0_FSM_FAULT_FLAG_EGR_TXPI_CONV_FAIL_MASK  0x10
#define OCW_PORT0_FSM_FAULT_FLAG_EGR_TXPI_CONV_FAIL_SHIFT  4
#define OCW_PORT0_FSM_FAULT_FLAG_EGR_OGBOX_CLSN_MASK  0x8
#define OCW_PORT0_FSM_FAULT_FLAG_EGR_OGBOX_CLSN_SHIFT  3
#define OCW_PORT0_FSM_FAULT_FLAG_EGR_FEC_LOL_MASK  0x4
#define OCW_PORT0_FSM_FAULT_FLAG_EGR_FEC_LOL_SHIFT  2
#define OCW_PORT0_FSM_FAULT_FLAG_EGR_IGBOX_CLSN_MASK  0x2
#define OCW_PORT0_FSM_FAULT_FLAG_EGR_IGBOX_CLSN_SHIFT  1
#define OCW_PORT0_FSM_FAULT_FLAG_EGR_BH_CDR_LOL_MASK  0x1
#define OCW_PORT0_FSM_FAULT_FLAG_EGR_BH_CDR_LOL_SHIFT  0


/****************************************************************************
 * #define OCTAL_PORT_STATUS_LOG    QUAD_CORE_CONFIG_RDB_CFGVAL_22_RDB
  ***************************************************************************/
  
#define OCTAL_PORT_STATUS_LOG_CW_DEFAULT_MODE_NO_SURGE_MASK  0x8000
#define OCTAL_PORT_STATUS_LOG_CW_DEFAULT_MODE_NO_SURGE_SHIFT  15
#define OCTAL_PORT_STATUS_LOG_CW_DEFAULT_MODE_NO_SURGE_IN_HANDLE_MASK  0x4000
#define OCTAL_PORT_STATUS_LOG_CW_DEFAULT_MODE_NO_SURGE_IN_HANDLE_SHIFT  14

#define OCTAL_PORT_STATUS_LOG_CW_PORT_MISSION_MASK  0x1
#define OCTAL_PORT_STATUS_LOG_CW_PORT_MISSION_SHIFT  0


/****************************************************************************
 * #define OCTAL_PORT_MISSION_INFO_REG                    QUAD_CORE_CONFIG_CFGVAL_50_RDB
***************************************************************************/
#define OCTAL_PORT_MISSION_INFO_REG_IGR_MISSION_MASK  0xFF00
#define OCTAL_PORT_MISSION_INFO_REG_IGR_MISSION_SHIFT  8

#define OCTAL_PORT_MISSION_INFO_REG_EGR_MISSION_MASK  0xFF
#define OCTAL_PORT_MISSION_INFO_REG_EGR_MISSION_SHIFT  0

/****************************************************************************
 * #define OCW_CHIP_LPM_FW_REG                       QUAD_CORE_GPCTRL_RDB_GPCTRL_346_RDB
  ***************************************************************************/

#define OCW_CHIP_LPM_FW_REG_CHIP_IN_LPM_MASK  0x1
#define OCW_CHIP_LPM_FW_REG_CHIP_IN_LPM_SHIFT  0

/****************************************************************************
 * #define OCTAL_CHIP_MODE_PORT_INFO_REG                  QUAD_CORE_CONFIG_CFGVAL_49_RDB
  ***************************************************************************/
#define OCTAL_CHIP_MODE_PORT_INFO_REG_PORT_IDX_MASK  0xF
#define OCTAL_CHIP_MODE_PORT_INFO_REG_PORT_IDX_SHIFT  0

/****************************************************************************
 * #define HOST_PLL_STATUS_REG                            QUAD_CORE_GPCTRL_GPCTRL_220_RDB
  ***************************************************************************/
#define HOST_PLL_STATUS_REG_HOST_PLL_LOCKED_MASK  0x1
#define HOST_PLL_STATUS_REG_HOST_PLL_LOCKED_SHIFT  0
/****************************************************************************
 * #define HOST_PLL_CONTROL_REG                           QUAD_CORE_GPCTRL_GPCTRL_221_RDB
  ***************************************************************************/
#define HOST_PLL_CONTROL_REG_LINE_PLL_RESTART_MASK  0x1
#define HOST_PLL_CONTROL_REG_LINE_PLL_RESTART_SHIFT  0

/****************************************************************************
  * #define CHIP_TOP_CHIP_AVS_SLAVE_MISC_REG              QUAD_CORE_GPCTRL_GPCTRL_60_RDB  
  ***************************************************************************/	
#define CHIP_TOP_CHIP_AVS_SLAVE_MISC_REG_REGULATOR_SLAVE_ENABLE_MASK  0x1
#define CHIP_TOP_CHIP_AVS_SLAVE_MISC_REG_REGULATOR_SLAVE_ENABLE_SHIFT  0

/****************************************************************************
  * #define CHIP_TOP_CHIP_INTERNAL_CONTROL                QUAD_CORE_GPCTRL_GPCTRL_6_RDB  
  ***************************************************************************/	
#define CHIP_TOP_CHIP_INTERNAL_CONTROL_DISABLE_AUTO_RESTORE_WITH_LANE_SWAP_MASK  0x10
#define CHIP_TOP_CHIP_INTERNAL_CONTROL_DISABLE_AUTO_RESTORE_WITH_LANE_SWAP_SHIFT  4

#ifdef __cplusplus
}
#endif


#endif /*CHIP_COMMON_H*/

/**
 * @file     capi_test_def.h
 * @author  
 * @date     03-24-2019
 * @version 1.0
 *
 * @property    $ Copyright: (c) 2020 Broadcom Limited All Rights Reserved $
 *       No portions of this material may be reproduced in any form without the
 *       written permission of: 
 *               Broadcom Limited
 *               1320 Ridder Park Drive
 *               San Jose, California 95131
 *               United States
 * All information contained in this document/file is Broadcom Limit company
 * private proprietary, trade secret, and remains the property of Broadcom
 * Limited. The intellectual and technical concepts contained herein are
 * proprietary to Broadcom Limited and may be covered by U.S. and Foreign
 * Patents, patents in process, and are protected by trade secret or copyright
 * law. Dissemination of this information or reproduction of this material is
 * strictly forbidden unless prior written permission is obtained from Bloadcom
 * Limited.
 *
 * @brief   
 *
 * @section
 * 
 */
#ifndef CAPI_TEST_DEF_H
#define CAPI_TEST_DEF_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct capi_ngen_err_inject_s {
    capi_enable_t enable;       /**< NGEN enable info */
    uint32_t inject_err_num;    /**< NGEN err */
} capi_ngen_err_inject_t;

typedef struct capi_ngen_err_cnt_s {
    uint64_t ngen_tot_bit0s_inj_cnt;
    uint64_t ngen_tot_bit1s_inj_cnt;
    uint64_t ngen_tot_frames_inj_cnt;
    double ngen_ber;
    uint8_t clr;
} capi_ngen_err_cnt_t;

/**
 * The clock divider enum definition of LW recovered clock 
 */
typedef enum {
    CAPI_RCLK_DIV_RATIO_NONE = -1, /**< Invalid Clock divider ratio */
    CAPI_RCLK_DIV_RATIO_1 = 0,  /**< Clock divider ratio is 1 */
    CAPI_RCLK_DIV_RATIO_2 = 1,  /**< Clock divider ratio is 2 */
    CAPI_RCLK_DIV_RATIO_4 = 2,  /**< Clock divider ratio is 4 */
    CAPI_RCLK_DIV_RATIO_8 = 3,  /**< Clock divider ratio is 8 */
    CAPI_RCLK_DIV_RATIO_16 = 4, /**< Clock divider ratio is 16 */
    CAPI_RCLK_DIV_RATIO_32 = 5, /**< Clock divider ratio is 32 */
    CAPI_RCLK_DIV_RATIO_64 = 6, /**< Clock divider ratio is 64 */
    CAPI_RCLK_DIV_RATIO_128 = 7  /**< Clock divider ratio is 128 */
} capi_rclk_div_ratio_t;

/**
 * The enum definition of LW recovered clock base frequency 
 */
typedef enum {
    CAPI_RCLK_64T = 0,  /**< base frequency is VCO divided by 64 */
    CAPI_RCLK_80T = 1   /**< base frequency is VCO divided by 80 */
} capi_rclk_t;

/**
 * The structure of CAPI SNR configuration information 
 */
typedef struct capi_lw_snr_info_s {
    uint8_t cdr_snr_threshold;      /**< LW CDR locked SNR threshold: 0-firmware default(17db); 1:13db; 2:14db; 3:15db; 4:16db; 5:17db; 6:18db; 7:19db */
} capi_lw_snr_info_t;               /**< LW SNR configuration Type                                              */

/**
 * The structure of CAPI Dynamic PLL fracN configuration information 
 */
typedef struct capi_lw_dynamic_pll_fracn_cfg_info_s {
    uint8_t enable_dynamic_adjust;      /**< enable/disable feature: 0-disable feature, firmware default setting; 1 enable feature */
    uint8_t adjust_step_0p1_ppm;        /**< per step, adjust value (unit: 0.1ppm) ; firmware default setting is 10 */
    uint8_t adjust_gap_timer_10us;       /**< timer gap between each step (unit: 10us) ; firmware default setting is 1, 10us */
} capi_lw_dynamic_pll_fracn_cfg_info_t;               /**< LW dynamic_pll_fracn_cfg_info                                              */



/**
 * The structure of test command information 
 */
typedef struct capi_test_command_info_s {
    uint32_t                         command_id;               /**< Command Identifier, refer phy_command_id_t*/

    union {
        txpi_override_t              txpi_ovrd;                /**< TxPI Override                             */
        uint8_t                      dsp_opt_los_host_tx_unsquelch;/**< option/feature to unsquelch client TX upon deassertion of OPRXLOS.
                                                                        0 : feature disabled    1 : feature enabled */
        capi_dsp_mode_type_t         dsp_mode;                 /**< DSP mode (optical vs electrical)          */
        capi_recovered_clock_info_t  recovered_clock_info;     /**< DSP mode (optical vs electrical)          */
        capi_lw_snr_info_t           lw_snr_info;                       /**< LW SNR Information            */
        capi_lw_dynamic_pll_fracn_cfg_info_t  dsp_dpll_cfg; /**< LW  Dynamic PLL fracN configuration information            */
        uint8_t                     dsp_lane_sr2;   /**< option/feature to set lane to SR2
                                                                        0 : disable SR2   1 : enable SR2 */
    } type;                                               /**< Payload/data type of the command Id       */

} capi_test_command_info_t;                               /**< Command Information type                  */



/**
 * The enum definition of KP4 FEC Error Injection Symbols 
 */
typedef enum kp4_fec_symbol_num_type_s {
    CAPI_KP4_FEC_SYMBOL_NUM_0  = 0x0,      /*!< KP4 FEC Error Injection Symbol 0 */
    CAPI_KP4_FEC_SYMBOL_NUM_1  = 0x1,      /*!< KP4 FEC Error Injection Symbol 1 */
    CAPI_KP4_FEC_SYMBOL_NUM_2  = 0x2,      /*!< KP4 FEC Error Injection Symbol 2 */
    CAPI_KP4_FEC_SYMBOL_NUM_3  = 0x3,      /*!< KP4 FEC Error Injection Symbol 3 */
    CAPI_KP4_FEC_SYMBOL_NUM_4  = 0x4,      /*!< KP4 FEC Error Injection Symbol 4 */
    CAPI_KP4_FEC_SYMBOL_NUM_5  = 0x5,      /*!< KP4 FEC Error Injection Symbol 5 */
    CAPI_KP4_FEC_SYMBOL_NUM_6  = 0x6,      /*!< KP4 FEC Error Injection Symbol 6 */
    CAPI_KP4_FEC_SYMBOL_NUM_7  = 0x7,      /*!< KP4 FEC Error Injection Symbol 7 */
    CAPI_KP4_FEC_SYMBOL_NUM_8  = 0x8,      /*!< KP4 FEC Error Injection Symbol 8 */
    CAPI_KP4_FEC_SYMBOL_NUM_9  = 0x9,      /*!< KP4 FEC Error Injection Symbol 9 */
    CAPI_KP4_FEC_SYMBOL_NUM_A  = 0xA,      /*!< KP4 FEC Error Injection Symbol 10 */
    CAPI_KP4_FEC_SYMBOL_NUM_B  = 0xB,      /*!< KP4 FEC Error Injection Symbol 11 */
    CAPI_KP4_FEC_SYMBOL_NUM_C  = 0xC,      /*!< KP4 FEC Error Injection Symbol 12 */
    CAPI_KP4_FEC_SYMBOL_NUM_D  = 0xD,      /*!< KP4 FEC Error Injection Symbol 13 */
    CAPI_KP4_FEC_SYMBOL_NUM_E  = 0xE,      /*!< KP4 FEC Error Injection Symbol 14 */
    CAPI_KP4_FEC_SYMBOL_NUM_F  = 0xF,      /*!< KP4 FEC Error Injection Symbol 15 */
    CAPI_KP4_FEC_SYMBOL_NUM_10 = 0x10,     /*!< KP4 FEC Error Injection Symbol 16 */
} kp4_fec_symbol_num_type_e;                   


#ifdef __cplusplus
}
#endif



#endif /* ifndef CAPI_TEST_DEF_H */


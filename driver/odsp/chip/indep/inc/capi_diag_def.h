/**
 * @file     capi_diag_def.h
 * @author  
 * @date     03-25-2019
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
#ifndef CAPI_DIAG_DEF_H
#define CAPI_DIAG_DEF_H


#ifdef __cplusplus
extern "C" {
#endif

#define DSP_DIAG_DUMP_UUID 0x75ce

#define CW_FEC_LOCK_TIMER_CNT   500   /**< CW Check FEC lock timer counter      */

/*used for diagnostic */
typedef struct client_jf_stat_s {
    int16_t integ;
} client_jf_stat_t;


typedef enum capi_dump_type_e {
    CAPI_DUMP_DEFAULT,
    CAPI_DUMP_FSM
} capi_dump_type_t;

typedef struct dsp_diag_dump_header_s {
    uint16_t dsp_diag_uuid; /**< UUID to check memory corruption */
    uint16_t dsp_diag_size; /**< size of diag dump buffer */    
} dsp_diag_dump_header_t;

/*
#define DUMP_START_ADDRESS 0x2C000
#define DUMP_SIZE 0xF00
*/

typedef struct capi_dump_info_s {
    capi_dump_type_t dump_type;
} capi_dump_info_t;

/* Used by capi_display_diag_data() to select diag_level             */
/* example:                                                           */
/*  diag_level = CAPI_DIAG_LANE | CAPI_DIAG_CORE | CAPI_DIAG_EVENT    */
enum capi_diag_level_enum {
    CAPI_DIAG_MINIMAL  = 0,
    CAPI_DIAG_LANE     = 1,
    CAPI_DIAG_CORE     = 1 << 1,
    CAPI_DIAG_EVENT    = 1 << 2,
    CAPI_DIAG_EYE      = 1 << 3,
    CAPI_DIAG_REG_CORE = 1 << 4,
    CAPI_DIAG_REG_LANE = 1 << 5,
    CAPI_DIAG_UC_CORE  = 1 << 6,
    CAPI_DIAG_UC_LANE  = 1 << 7,
    CAPI_DIAG_LANE_DEBUG = 1 << 8,
    CAPI_DIAG_BER_VERT   = 1 << 9,
    CAPI_DIAG_BER_HORZ   = 1 << 10,
    CAPI_DIAG_EVENT_SAFE = 1 << 11,
    CAPI_DIAG_TIMESTAMP  = 1 << 12,
    CAPI_DIAG_PRBS_PROJ  = 1 << 13,
    CAPI_DIAG_PRBS_PROJ_30S = 1 << 14,
    CAPI_DIAG_INFO_TABLE    = 1 << 15,
    CAPI_DIAG_LEVEL_1    = CAPI_DIAG_LANE     | CAPI_DIAG_CORE     | CAPI_DIAG_EVENT,
    CAPI_DIAG_LEVEL_2    = CAPI_DIAG_LEVEL_1  | CAPI_DIAG_REG_CORE | CAPI_DIAG_REG_LANE |
                           CAPI_DIAG_UC_CORE  | CAPI_DIAG_UC_LANE,
    CAPI_DIAG_LEVEL_3    = CAPI_DIAG_LEVEL_2  | CAPI_DIAG_EYE,
    CAPI_DIAG_INTERNAL   = CAPI_DIAG_LANE     | CAPI_DIAG_EVENT
};



/* Used by capi_diag_access() to select type of access */
enum capi_diag_access_enum {
    CAPI_REG_READ,
    CAPI_REG_RMW,
    CAPI_CORE_RAM_READ_BYTE,
    CAPI_CORE_RAM_RMW_BYTE,
    CAPI_CORE_RAM_READ_WORD,
    CAPI_CORE_RAM_RMW_WORD,
    CAPI_LANE_RAM_READ_BYTE,
    CAPI_LANE_RAM_RMW_BYTE,
    CAPI_LANE_RAM_READ_WORD,
    CAPI_LANE_RAM_RMW_WORD,
    CAPI_GLOB_RAM_READ_BYTE,
    CAPI_GLOB_RAM_RMW_BYTE,
    CAPI_GLOB_RAM_READ_WORD,
    CAPI_GLOB_RAM_RMW_WORD,
    CAPI_PROG_RAM_READ_BYTE,
    CAPI_PROG_RAM_READ_WORD,
    CAPI_UC_CMD,
    CAPI_EN_BREAKPOINT,
    CAPI_GOTO_BREAKPOINT,
    CAPI_RD_BREAKPOINT,
    CAPI_DIS_BREAKPOINT,
    CAPI_BER_PROJ_DATA
};

/* Used by capi_dsc_dump() to select diagnostics dump filter             */
/* example:                                                           */
/*  dsc_dump_filter = CAPI_DSC_DUMP_REGS | CAPI_DSC_DUMP_PROFILES | CAPI_DSC_DUMP_DIAG | CAPI_DSC_DUMP_TRACEBUF   */
enum capi_dsc_dump_filter_enum {
    CAPI_DSC_DUMP_REGS = 1,             /**< Dump registers   */
    CAPI_DSC_DUMP_PROFILES = 1 << 1,    /**< Dump firmware profiles   */
    CAPI_DSC_DUMP_EVENTS = 1 << 2        /**< Dump firmware event log   */
};

/**
 * DSC dump configuration
 */
typedef struct capi_dsc_dump_cfg_s {
    enum capi_dsc_dump_filter_enum dump_filter; /**< DSC dump filter mask */
    bool pause_firmware;
} capi_dsc_dump_cfg_t;

/**
 * This Enum contains capi eyescan modes
 */
typedef enum capi_eyescan_mode_e {
    CAPI_EYESCAN_MODE_FAST = 0,         /**< Fast eyescan resoultion. */
    CAPI_EYESCAN_MODE_LOW_BER,          /**< Allow more aquirate BER resolution */
    CAPI_EYESCAN_MODE_BER_PROJ,         /**< Do BER projection */
    CAPI_EYESCAN_MODE_LAST              /**< Last Element Counter */
} capi_eyescan_mode_t;                  /**< Eyescan Mode type */

/**
 * The structure contains capi eyescan options
 */
typedef struct capi_eyescan_option_s {
    uint32_t linerate_in_khz;       /**< */
    uint16_t timeout_in_ms;         /**< */
    int8_t horz_max;                /**< */
    int8_t horz_min;                /**< */
    int8_t hstep;                   /**< */
    int8_t vert_max;                /**< */
    int8_t vert_min;                /**< */
    int8_t vstep;                   /**< */
    uint8_t ber_proj_scan_mode;     /**< For BER projection Controls the direction and polarity of the test. */
    uint8_t ber_proj_timer_cnt;     /**< BER proj total measurement time in units of ~1.3 seconds. */
    uint8_t ber_proj_err_cnt;       /**< BER proj the error threshold it uses to step to next measurement in units of 16. */
    int8_t mode;                    /**< */
} capi_eyescan_option_t;            /**< Eyescan Option type */


/** Eye margin projection BER mode settings **/
typedef enum capi_eye_diag_ber_mode_enum {
    CAPI_EYE_DIAG_BER_POS  = 0,
    CAPI_EYE_DIAG_BER_NEG  = 1,
    CAPI_EYE_DIAG_BER_VERT = 0,
    CAPI_EYE_DIAG_BER_HORZ = 1<<1,
    CAPI_EYE_DIAG_BER_PASS = 0,
    CAPI_EYE_DIAG_BER_INTR = 1<<2,
    CAPI_EYE_DIAG_BER_P1_NARROW = 1<<3,
    CAPI_EYE_DIAG_BER_P1_WIDE = 0,
    CAPI_EYE_DIAG_BER_FAST = 1<<6
} capi_eye_diag_ber_mode_t;



/**
 * The structure contains capi eyescan information
 */
typedef struct capi_eyescan_info_s {
    uint32_t flags;                 /**< flags???? */
    capi_eyescan_mode_t mode;       /**< mode */
    capi_eyescan_option_t option;   /**< option */
} capi_eyescan_info_t;              /**< Eyescan Information type */

/**
 * The structure contains capi eye margin projection info
 */
typedef struct capi_eye_margin_proj_info_s {
    uint64_t rate;              /**< data rate in Hz */
    uint8_t ber_scan_mode;      /**< the type of test used to take the data(use #capi_eye_diag_ber_mode_t) */
    uint8_t timer_control;      /**< the total test time in units of ~1.31 seconds */
    uint8_t max_error_control;  /**< the error threshold for test in units of 16.(4=64 errors) */
} capi_eye_margin_proj_info_t;  /**< Eyescan Information type */


#define LW_RX_ADC_COUNT 80
#define LW_RX_CAPMEM_BUF_SIZE 960
#define LW_RX_CHANNEL_ID_COUNT 8
#define LW_RX_SLICER_HISTOGRAM_SIZE 256

typedef struct lw_cfg_override_s {
    uint8_t enable; /**< enable =1 indicate the config value is valid */
    uint8_t valid; /**< enable =1 indicate the config value is valid during "GET"*/
    int32_t value; /**<  */
} lw_cfg_override_t; /**< Value Override type */


/**
 * The structure of LW lane config: related both RX/TX data path
 */
typedef struct lw_lane_config_s {

    uint8_t lt_disable_timer; /**< lt_disable_timer */
    uint8_t lt_1st_ppm_step_converge; /**< lt_1st_ppm_step_converge */
    uint8_t tx_ppm_converge_compare_en; /**< tx_ppm_converge_compare_en */
    uint8_t disable_lt_bh_txfir_lmt; /**< disable_lt_bh_txfir_lmt */
    uint8_t disable_lt_rxflt_restart_tx; /**< disable_lt_rxflt_restart_tx */
    uint8_t disable_lt_rxlos_restart_tx; /**< disable_lt_rxlos_restart_tx */
} lw_lane_config_t; /**< LW Lane Config type */



/**
 * The structure of LW RX lane config: related both RX/TX data path
 */
typedef struct lw_lane_rx_config_s {

    lw_cfg_override_t snr_threshold; /**< snr_threshold:0- Don’t swap the MSB and LSB within a symbol ; 1- swap the MSB and LSB within a symbol */
    lw_cfg_override_t b4_sapbtune_snr_threshold; /**< b4_sapbtune_snr_threshold: SNR threshold before static APB tune*/
    lw_cfg_override_t cdr_lol_snr_threshold; /**< cdr_lol_snr_threshold: SNR threshold after CDR enter mission state for 1s*/

    lw_cfg_override_t mtap_coefficient; /**< mtap_coefficient: S5.2 format */
    lw_cfg_override_t pp_mtap_coefficient; /**< pp_mtap_coefficient:  S5.2 format*/
    
    lw_cfg_override_t cfg_cu_dcoffset_skew_ctrl; /**< cfg_cu_dcoffset_skew_ctrl: This parameter only apply for NRZ Copper mode both direct link and link training
                                              0-in default, all NRZ copper will enable integ_ppm_tune; 
                                              1-all NRZ copper will force skew & dc_offset OFF
                                              2-all NRZ copper will force skew & dc_offset ON
                                              3-back_compatible original design (2*20G or 2*25G Consortium enable integ_ppm_tune; 25G/eslicer turn SKEW&DC_OFFSET ON; other mode turn SKEW&DC_OFFSET OFF*/
    lw_cfg_override_t phbias_tune_max; /**< phase_bias_max: 0 – Default setting. Firmware will take the range 10; None zero value: will be maximum phase bias value for phase bias tune algorithm */
    lw_cfg_override_t phbias_tune_min; /**< phase_bias_min: 0 – Default setting. Firmware will take the range -10; None zero value: will be minimum phase bias value for phase bias tune algorithm */
    lw_cfg_override_t disable_static_auto_ph_tune; /**< disable_static_auto_ph_tune: 0 – Firmware will run static phase bias tuning algorithm automatically; 1 – Disable static phase bias tuning (default value)*/
    lw_cfg_override_t disable_dynamic_auto_ph_tune; /**< disable_dynamic_auto_ph_tune: Note: disable_dynamic_auto_ph_tune is valid only if disable_static_auto_ph_tune is set to 0; 0- enable 1-disable */
    lw_cfg_override_t dynamic_auto_ph_tune_type; /**< dynamic_auto_ph_tune_type: Note: dynamic_auto_ph_tune_type is valid only if both disable_static_auto_ph_tune and disable_dynamic_auto_ph_tune are set to 0; 0-threshold compare; 1--calculation & adjust*/
    lw_cfg_override_t dynamic_auto_ph_tune_shift; /**< dynamic_auto_ph_tune_shift: Note: dynamic_auto_ph_tune_shift is valid only if dynamic_auto_ph_tune_type is 0. But cAPI don't reject this config and firmware will not take it in action*/
} lw_lane_rx_config_t; /**< LW Lane Config type */


/**
 * The structure of LW lane config: related TX data path only
 */
typedef struct lw_lane_tx_config_s {
    
    lw_cfg_override_t tx_prbs_or_squelch; /**< tx_prbs_or_squelch */
    lw_cfg_override_t tx_datapath_sel; /**< tx_datapath_sel */
    lw_cfg_override_t tx_datapath_prbs; /**< tx_datapath_prbs */
} lw_lane_tx_config_t; /**< LW Lane Config type */

typedef struct lw_adc_cal_status_s {
    uint8_t exceed_positive_num; /**< number of vcode exceed maximum threshold */
    uint8_t exceed_negative_num; /**< number of vcode exceed minimum threshold */
    uint8_t vcode[LW_RX_ADC_COUNT]; /*! The vcode for each sub adc */    
} lw_adc_cal_status_t; /**< LW level SNR value type */

typedef struct lw_dbg_info_s {

    uint8_t s_elos;
    uint8_t s_oplos;
    uint8_t s_cdrlol;
    uint8_t s_gplos;

} lw_dbg_info_t; /**< PMD Information type */

typedef struct lw_copper_ap_cfg_s {
    uint8_t enable;
} lw_copper_ap_cfg_t; /**< LW copper link training config */

typedef struct lw_lnktrn_cfg_s {
    uint8_t opposite_cdr_first; /**< (i) 1:wait for opposite cdr_lock is ready before link training start 0: start link training firstly, then check opposite cdr_lock status */
} lw_lnktrn_cfg_t;


/**
 * The structure contains capi slicer offset
 */
typedef struct capi_diag_slicer_offset_s {
    uint32_t offset_pe;
    uint32_t offset_ze;
    uint32_t offset_me;
    uint32_t offset_po;
    uint32_t offset_zo;
    uint32_t offset_mo;
} capi_diag_slicer_offset_t; /**< Diagnostic Slicer type */

/**
 * The structure contains capi diagnostisc eyescan
 */
typedef struct capi_diag_eyescan_s {
    uint32_t heye_left;
    uint32_t heye_right;
    uint32_t veye_upper;
    uint32_t veye_lower;
} capi_diag_eyescan_t; /**< Diagnostic Eyescan type */

/**
 * The structure contains capi diagnostic information
 */
typedef struct capi_phy_diag_info_s {
    uint32_t signal_detect;
    uint32_t vga_bias_reduced;
    uint32_t postc_metric;
    uint8_t osr_mode;
    uint8_t pmd_mode;
    uint32_t rx_lock;
    int32_t rx_ppm;
    int32_t tx_ppm;
    uint32_t clk90_offset;
    uint32_t clkp1_offset;
    uint32_t p1_lvl;
    uint32_t m1_lvl;
    uint32_t dfe1_dcd;
    uint32_t dfe2_dcd;
    uint32_t slicer_target;
    capi_diag_slicer_offset_t slicer_offset;
    capi_diag_eyescan_t eyescan;
    uint32_t state_machine_status;
    uint32_t link_time;
    int pf_main;
    int pf_hiz;
    int pf_bst;
    int pf_low;
    int pf2_ctrl;
    int vga;
    int dc_offset;
    int p1_lvl_ctrl;
    int dfe1;
    int dfe2;
    int dfe3;
    int dfe4;
    int dfe5;
    int dfe6;
    int txfir_pre2;
    int txfir_pre1;
    int txfir_main;
    int txfir_post1;
    int txfir_post2;
    int txfir_post3;
    int txfir_post4;
    int8_t txfir_level_shift[4];
    int tx_amp_ctrl;
    uint8_t br_pd_en;
    uint32_t state_mfg_diag_inst;   /**< Specfic device block */
    int state_mfg_diag_op_type;     /**< Operation type */
    int state_mfg_diag_op_cmd;      /**< Operation type */
    void* state_mfg_diag_arg;       /**< Argument pointer */
    int pf3_ctrl;
    int ffe_enable;
    int ffe1;
    int ffe2;
    int8_t lane_reset_state;
    int8_t tx_reset_state;
    int8_t uc_stop_state;
    int8_t ucv_lane_status;
    int8_t tx_pll_select;
    int8_t rx_pll_select;

    float rx_ppm_real;
    float tx_ppm_real;
    uint8_t modulation;
    uint8_t snr_levels;
    float level_snr[7];
    float snr;
    int8_t dc_offset_array[LW_RX_ADC_COUNT];
    uint32_t slice_historgram[LW_RX_SLICER_HISTOGRAM_SIZE];
    int32_t mean_hist_value[4];
    float sigma_hist_value[4];
    float ffe_real[24];
    uint8_t tr_skew[8];
    uint8_t peaking_filter;
    int8_t capture_mem_adc[LW_RX_CAPMEM_BUF_SIZE];
    int8_t capture_mem_eq[LW_RX_CAPMEM_BUF_SIZE];
    int16_t tr_phase;
    float dc_offset_real[LW_RX_ADC_COUNT];
    int8_t dpga_array[LW_RX_ADC_COUNT];
    float dpga_real[LW_RX_ADC_COUNT];
    int32_t channel_id[LW_RX_CHANNEL_ID_COUNT];
    uint32_t adcclip_cnt;
    uint8_t gain2;
    uint8_t gain3;
    uint8_t gain3_a_ovrd_info;
    float gain3_a;
} capi_diag_info_t; /**< Diagnostic Information type */


/*
 * Enumeration of Line Wrapper RX FFE slicer mode  values
 */
enum {
     LW_NRZ_MODE =0,     /*!<  NRZ mode */
     LW_EPAM2_MODE =1,   /*!<  EPAM2 mode */
     LW_PAM4_MODE =2,    /*!<  PAM4 mode */
     LW_EPAM4_mode =3    /*!<  EPAM4 mode */
}; /*!<  Line Wrapper RX FFE tap value */

/**
 * diag info type enumeration
 */
typedef enum diag_info_type_e {
    DIAG_INFO_TYPE_NONE = 0,                 /**< Diag info type none                    */
    DIAG_INFO_TYPE_BRCM_DSP_SNR,             /**< Diag info type Broadcom DSP SNR        */
    DIAG_INFO_TYPE_CMIS,                     /**< Diag info type CMIS                    */
    DIAG_INFO_TYPE_BRCM_DSP_SNR_AVRG_ONLY,   /**< Diag info type Broadcom DSP average SNR only       */
} diag_info_type_t;                          /**< Diag info type                         */

/**
 * The structure contains lw CMIS internal information
 */
typedef struct dsp_cmis_info_s {
    float    cmis_snr;                        /**< CMIS SNR value */
    float    cmis_ltp;                        /**< CMIS LTP value */
        
    int16_t  slicer_threshold[3];             /**< slicer threshold */
    int16_t  slicer_target[4];                /**< slicer symbol value */
    float  slicer_mean_value[4];            /**< slicer mean value */    
    float    slicer_sigma_value[4];           /**< slicer sigma value */
    uint32_t slicer_p_value[4];               /**< slicer P value */
    int16_t  slicer_p_location[4];            /**< slicer P location */
    uint32_t slicer_v_value[3];               /**< slicer V value */
    int16_t  slicer_v_location[3];            /**< slicer V location */
} dsp_cmis_info_t; /**< Core Info type */

/**
 * The structure contains Broadcom DSP SNR information
 */
typedef struct diag_brcm_dsp_snr_s{
    float brcm_snr_value;    /**< Broadcom SNR value           */
    float brcm_snr_level[8]; /**< Broadcom SNR levels          */
} diag_brcm_dsp_snr_t;       /**< User LW level snr value type */

/**
 * The structure contains serdes cmis snr and ltp information
 */
typedef struct serdes_snr_ltp_s {
    uint32_t snr_value; /**< SNR value           */
    uint32_t ltp_value; /**< LTP value           */
} serdes_snr_ltp_t;     /**< Serdes snr ltp type */

/**
 * The structure contains serdes cmis information
 */
typedef struct serdes_cmis_info_s {
    double fec_ber;                    /**< fec_ber, input from user */
    serdes_snr_ltp_t snr_ltp;          /**< snr, ltp, output to user */
} serdes_cmis_info_t;                  /**< Serdes cmis info type    */

/**
 * The structure contains CMIS information
 */
typedef struct diag_cmis_info_s {
    union {
        uint32_t content;
        struct {
#ifdef CAPI_IS_BIG_ENDIAN
            uint32_t reserved                   :30;        /**< reserved                     */
            uint32_t serdes_snr_ltp             : 1;        /**< SerDes CMIS Information, N/A */
            uint32_t dsp_snr_ltp                : 1;        /**< DSP CMIS Information         */
#else  /* CAPI_IS_LITTLE_ENDIAN */
            uint32_t dsp_snr_ltp                : 1;        /**< DSP CMIS Information         */
            uint32_t serdes_snr_ltp             : 1;        /**< SerDes CMIS Information, N/A */
            uint32_t reserved                   :30;        /**< reserved                     */
#endif /* CAPI_IS_BIG_ENDIAN */
        } is;
    } param;

    union {
        dsp_cmis_info_t          dsp_snr_ltp;               /**< DSP CMIS Information         */
        serdes_cmis_info_t       serdes_snr_ltp;            /**< SerDes CMIS Information, N/A */
    } value;
} diag_cmis_info_t; /**< diag cmis info type */

/**
 * The structure contains capi usr diag information
 */
typedef struct capi_usr_diag_info_t {
    diag_info_type_t                  diag_info_type;    /**< Diag info type                         */
    union {
        diag_brcm_dsp_snr_t           diag_brcm_dsp_snr; /**< Diag Broadcom DSP SNR                  */
        diag_cmis_info_t              diag_cmis_info;    /**< Diag info CMIS                         */
    } type;
} capi_usr_diag_info_t;                                  /**< User Diag Info type                    */

#define FW_LINKCAT_UUID 0x97ea
#define PRBS_DATA_SIZE 2047      // PRBS9 data size is 2047 bytes
#define PI_STEPS_MAX 32         // 32 PI steps

typedef struct diag_dsp_linkcat_s {
    uint16_t linkcat_uuid;                          /**< (i) UUID */
    uint8_t lane_id;                                /**< (i) lane ID */
    uint8_t fsm_state;                              /**< (i) linkcat FSM state */
    uint32_t pi_steps_fw;                            /**< (i) PI steps FW catpures ADC data at */
    uint32_t pi_steps_host;                          /**< (i) PI steps host reads ADC data at */
    int16_t  ipc_buf[PRBS_DATA_SIZE + 1];                /**< (i) FW stores ADC data into this IPC buffer for host to read  */
    int16_t adc_data_sum[PRBS_DATA_SIZE + 1];           /**< (i) FW captures 511 bytes ADC data 16 times and get sum */
    int8_t  adc_data[PRBS_DATA_SIZE + 1];                          /**< (i) ADC data buffer */
    uint8_t phy_side;                                /**< (i) phy side */
    uint8_t pgac_code;
} diag_dsp_linkcat_t;

#ifdef __cplusplus
}
#endif

#endif /* ifndef CAPI_DIAG_DEF_H */


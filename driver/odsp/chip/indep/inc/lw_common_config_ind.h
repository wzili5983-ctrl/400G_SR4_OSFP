
#ifndef LW_COMMON_H
#define LW_COMMON_H

#define DSP_ENABLE_DEBUG_DUMP
typedef enum lw_lane_id_e {
    CHIP_LW_LANE_0 = 0,
    CHIP_LW_LANE_1 = 1,
    CHIP_LW_LANE_2 = 2,
    CHIP_LW_LANE_3 = 3,
    CHIP_LW_LANE_MAX,
} lw_lane_id_t;

typedef enum lw_ffe_tap_config_e {
    LW_FFE_TAP_DEFAULT = 0,
    LW_FFE_TAP_24 = 1,
    LW_FFE_TAP_15 = 2,
    LW_FFE_TAP_9 = 3
} lw_ffe_tap_config_t;

/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_STATUS_REG 
 ***************************************************************************/
typedef union common_lw_lane_status_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t command_accepted : 1; /*!<command_accepted */
        uint16_t reserved0 : 1; /*!<reserved0 */
        uint16_t error_code : 6; /*!<error_code */
        uint16_t status_return : 8; /*!<status_return */
#else /* LITTLE ENDIAN */
        uint16_t status_return : 8; /*!<status_return */
        uint16_t error_code : 6; /*!<error_code */
        uint16_t reserved0 : 1; /*!<reserved0 */
        uint16_t command_accepted : 1; /*!<command_accepted */
#endif
    } fields;
} common_lw_lane_status_reg_t;


/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIG1_REG 
 ***************************************************************************/
typedef union common_lw_lane_config1_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t lw_lnk_trn_syscdr_first : 1; /*!<lw_lnk_trn_syscdr_first */
        uint16_t lw_lnk_trn_dis_timer : 1; /*!<lw_lnk_trn_dis_timer */
        uint16_t lw_lnk_trn_restart : 1; /*!<lw_lnk_trn_restart */
        uint16_t lw_lnk_trn_init_cond : 2; /*!<lw_lnk_trn_init_cond */
        uint16_t lw_auto_neg : 1; /*!<lw_auto_neg */
        uint16_t lw_lnk_train_frame_size : 1; /*!<lw_lnk_train_frame_size */
        uint16_t lw_link_training : 3; /*!<lw_link_training */
        uint16_t lw_gloop : 1; /*!<lw_gloop */
        uint16_t lw_rloop : 1; /*!<lw_rloop */
        uint16_t lw_extd_mode : 2; /*!<lw_extd_mode */
        uint16_t lw_optical_mode : 1; /*!<lw_optical_mode */
#else /* LITTLE ENDIAN */
        uint16_t lw_optical_mode : 1; /*!<lw_optical_mode */
        uint16_t lw_extd_mode : 2; /*!<lw_extd_mode */
        uint16_t lw_rloop : 1; /*!<lw_rloop */
        uint16_t lw_gloop : 1; /*!<lw_gloop */
        uint16_t lw_link_training : 3; /*!<lw_link_training */
        uint16_t lw_lnk_train_frame_size : 1; /*!<lw_lnk_train_frame_size */
        uint16_t lw_auto_neg : 1; /*!<lw_auto_neg */
        uint16_t lw_lnk_trn_init_cond : 2; /*!<lw_lnk_trn_init_cond */
        uint16_t lw_lnk_trn_restart : 1; /*!<lw_lnk_trn_restart */
        uint16_t lw_lnk_trn_dis_timer : 1; /*!<lw_lnk_trn_dis_timer */
        uint16_t lw_lnk_trn_syscdr_first : 1; /*!<lw_lnk_trn_syscdr_first */
#endif
    } fields;
} common_lw_lane_config1_reg_t;

/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIG2_REG 
 ***************************************************************************/
typedef union common_lw_lane_config2_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t lw_rx_disable : 1; /*!<lw_rx_disable */
        uint16_t lw_kp_track_hlf_stp : 1; /*!<lw_kp_track_hlf_stp */
        uint16_t lw_kp_hlf_stp : 1; /*!<lw_kp_hlf_stp */
        uint16_t lw_peaking : 5; /*!<lw_peaking */
        uint16_t lw_peaking_en : 1; /*!<lw_peaking_en */
        uint16_t lw_los_ignore : 1; /*!<lw_los_ignore */
        uint16_t lw_snr_lvl : 3; /*!<lw_snr_lvl */
        uint16_t lw_rx_graycode : 1; /*!<lw_rx_graycode */
        uint16_t lw_rx_symbol_swap : 1; /*!<lw_rx_symbol_swap */
        uint16_t lw_invert_rx : 1; /*!<lw_invert_rx */
#else /* LITTLE ENDIAN */
        uint16_t lw_invert_rx : 1; /*!<lw_invert_rx */
        uint16_t lw_rx_symbol_swap : 1; /*!<lw_rx_symbol_swap */
        uint16_t lw_rx_graycode : 1; /*!<lw_rx_graycode */
        uint16_t lw_snr_lvl : 3; /*!<lw_snr_lvl */
        uint16_t lw_los_ignore : 1; /*!<lw_los_ignore */
        uint16_t lw_peaking_en : 1; /*!<lw_peaking_en */
        uint16_t lw_peaking : 5; /*!<lw_peaking */
        uint16_t lw_kp_hlf_stp : 1; /*!<lw_kp_hlf_stp */
        uint16_t lw_kp_track_hlf_stp : 1; /*!<lw_kp_track_hlf_stp */
        uint16_t lw_rx_disable : 1; /*!<lw_rx_disable */
#endif
    } fields;
} common_lw_lane_config2_reg_t;

/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIG3_REG 
 ***************************************************************************/
typedef union common_lw_lane_config3_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t lw_kf_tracking : 3; /*!<lw_kf_tracking */
        uint16_t lw_kf_tracking_override : 1; /*!<lw_kf_tracking_override */
        uint16_t lw_kf : 3; /*!<lw_kf */
        uint16_t lw_kf_override : 1; /*!<lw_kf_override */
        uint16_t lw_kp_tracking : 3; /*!<lw_kp_tracking */
        uint16_t lw_kp_tracking_override : 1; /*!<lw_kp_tracking_override */
        uint16_t lw_kp : 3; /*!<lw_kp */
        uint16_t lw_kp_override : 1; /*!<lw_kp_override */
#else /* LITTLE ENDIAN */
        uint16_t lw_kp_override : 1; /*!<lw_kp_override */
        uint16_t lw_kp : 3; /*!<lw_kp */
        uint16_t lw_kp_tracking_override : 1; /*!<lw_kp_tracking_override */
        uint16_t lw_kp_tracking : 3; /*!<lw_kp_tracking */
        uint16_t lw_kf_override : 1; /*!<lw_kf_override */
        uint16_t lw_kf : 3; /*!<lw_kf */
        uint16_t lw_kf_tracking_override : 1; /*!<lw_kf_tracking_override */
        uint16_t lw_kf_tracking : 3; /*!<lw_kf_tracking */
#endif
    } fields;
} common_lw_lane_config3_reg_t;

/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIG4_REG 
 ***************************************************************************/
typedef union common_lw_lane_config4_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t lw_gain2_ovrd_val : 1; /*!<lw_gain2_ovrd_val */
        uint16_t lw_gain2_ovrd_en : 1; /*!<lw_gain2_ovrd_en */
        uint16_t lw_eq2_enable : 1; /*!<lw_eq2_enable */
        uint16_t lw_eq_tap_sel : 2; /*!<lw_eq_tap_sel, defined in lw_ffe_tap_config_t */
        uint16_t lw_eq2_type : 1; /*!<lw_eq2_type */
        uint16_t lw_phd_mode : 1; /*!<lw_phd_mode */
        uint16_t lw_phd_mode_override : 1; /*!<lw_phd_mode_override */
        uint16_t lw_ffe_mu_tracking : 3; /*!<lw_ffe_mu_tracking */
        uint16_t lw_ffe_mu_tracking_override : 1; /*!<lw_ffe_mu_tracking_override */
        uint16_t lw_ffe_mu : 3; /*!<lw_ffe_mu */
        uint16_t lw_ffe_mu_override : 1; /*!<lw_ffe_mu_override */
#else /* LITTLE ENDIAN */
        uint16_t lw_ffe_mu_override : 1; /*!<lw_ffe_mu_override */
        uint16_t lw_ffe_mu : 3; /*!<lw_ffe_mu */
        uint16_t lw_ffe_mu_tracking_override : 1; /*!<lw_ffe_mu_tracking_override */
        uint16_t lw_ffe_mu_tracking : 3; /*!<lw_ffe_mu_tracking */
        uint16_t lw_phd_mode_override : 1; /*!<lw_phd_mode_override */
        uint16_t lw_phd_mode : 1; /*!<lw_phd_mode */
        uint16_t lw_eq2_type : 1; /*!<lw_eq2_type */
        uint16_t lw_eq_tap_sel : 2; /*!<lw_eq_tap_sel */
        uint16_t lw_eq2_enable : 1; /*!<lw_eq2_enable */
        uint16_t lw_gain2_ovrd_en : 1; /*!<lw_gain2_ovrd_en */
        uint16_t lw_gain2_ovrd_val : 1; /*!<lw_gain2_ovrd_val */
#endif
    } fields;
} common_lw_lane_config4_reg_t;

/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIG5_REG 
 ***************************************************************************/
typedef union common_lw_lane_config5_reg_u {
    uint16_t words;
    struct {        
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t lw_tx_squelch : 1; /*!<lw_tx_squelch */
        uint16_t lw_tx_disable : 1; /*!<lw_tx_disable */
        uint16_t lw_tx_user_override : 1; /*!<lw_tx_user_override */
        uint16_t lw_tx_high_swing : 1; /*!<lw_tx_high_swing */
        uint16_t lw_txfir_7tap_en : 1; /*!<lw_txfir_7tap_en */
        uint16_t lw_tx_precode_indep : 2; /*!<lw_tx_precode_indep */
        uint16_t lw_tx_prbs_or_squelch : 1; /*!<lw_tx_prbs_or_squelch */
        uint16_t lw_tx_graycode : 1; /*!<lw_tx_graycode */
        uint16_t lw_prbs_sel : 3; /*!<lw_prbs_sel */
        uint16_t lw_tx_datapath_sel : 2; /*!<lw_tx_datapath_sel */
        uint16_t lw_tx_symbol_swap : 1; /*!<lw_tx_symbol_swap */
        uint16_t lw_invert_tx : 1; /*!<lw_invert_tx */
#else /* LITTLE ENDIAN */
        uint16_t lw_invert_tx : 1; /*!<lw_invert_tx */
        uint16_t lw_tx_symbol_swap : 1; /*!<lw_tx_symbol_swap */
        uint16_t lw_tx_datapath_sel : 2; /*!<lw_tx_datapath_sel */
        uint16_t lw_prbs_sel : 3; /*!<lw_prbs_sel */
        uint16_t lw_tx_graycode : 1; /*!<lw_tx_graycode */
        uint16_t lw_tx_prbs_or_squelch : 1; /*!<lw_tx_prbs_or_squelch */
        uint16_t lw_tx_precode_indep : 2; /*!<lw_tx_precode_indep */
        uint16_t lw_txfir_7tap_en : 1; /*!<lw_txfir_7tap_en */
        uint16_t lw_tx_high_swing : 1; /*!<lw_tx_high_swing */        
        uint16_t lw_tx_user_override : 1; /*!<lw_tx_user_override */
        uint16_t lw_tx_disable : 1; /*!<lw_tx_disable */
        uint16_t lw_tx_squelch : 1; /*!<lw_tx_squelch */
#endif
    } fields;
} common_lw_lane_config5_reg_t;

/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIG6_REG 
 ***************************************************************************/
typedef union common_lw_lane_config6_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t lw_pp_mtap_coef : 8; /*!<lw_pp_mtap_coef */
        uint16_t lw_mtap_coef : 8; /*!<lw_mtap_coef */
#else /* LITTLE ENDIAN */
        uint16_t lw_mtap_coef : 8; /*!<lw_mtap_coef */
        uint16_t lw_pp_mtap_coef : 8; /*!<lw_pp_mtap_coef */
#endif
    } fields;
} common_lw_lane_config6_reg_t;

/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIG7_REG 
 ***************************************************************************/
typedef union common_lw_lane_config7_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t reserved : 1; /*!<reserved */
        uint16_t lw_phbias_step_size : 1; /*!<lw_phbias_step_size */
        uint16_t lw_disable_d_phbias : 1; /*!<lw_disable_d_phbias */
        uint16_t lw_disable_s_phbias : 1; /*!<lw_disable_s_phbias */
        uint16_t lw_phbias_max : 6; /*!<lw_phbias_max */
        uint16_t lw_phbias_min : 6; /*!<lw_phbias_min */
#else /* LITTLE ENDIAN */
        uint16_t lw_phbias_min : 6; /*!<lw_phbias_min */
        uint16_t lw_phbias_max : 6; /*!<lw_phbias_max */
        uint16_t lw_disable_s_phbias : 1; /*!<lw_disable_s_phbias */
        uint16_t lw_disable_d_phbias : 1; /*!<lw_disable_d_phbias */
        uint16_t lw_phbias_step_size : 1; /*!<lw_phbias_step_size */
        uint16_t reserved : 1; /*!<reserved */
#endif
    } fields;
} common_lw_lane_config7_reg_t;

/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIG8_REG 
 ***************************************************************************/
typedef union common_lw_lane_config8_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t rx_low_bw_en         : 1; /*!<rx_low_bw_en */
        uint16_t ffe_slicer_multiplier : 2; /*!<ffe_slicer_multiplier */
        uint16_t lw_eq2_adapt_disable : 1; /*!<lw_eq2_adapt_disable */
        uint16_t lw_special_optical_cfg   : 1; /*!<lw_special_optical_cfg */
        uint16_t lw_los_th_cfg   : 4; /*!<lw_los_th_cfg */
        uint16_t lw_dc_wander_mu : 3; /*!<lw_dc_wander_mu */
        uint16_t lw_dc_wander_mu_override : 1; /*!<lw_dc_wander_mu_override */
        uint16_t lw_eq_maintap : 3; /*!<lw_eq_maintap */
#else /* LITTLE ENDIAN */
        uint16_t lw_eq_maintap : 3; /*!<lw_eq_maintap */
        uint16_t lw_dc_wander_mu_override : 1; /*!<lw_dc_wander_mu_override */
        uint16_t lw_dc_wander_mu : 3; /*!<lw_dc_wander_mu */
        uint16_t lw_los_th_cfg   : 4; /*!<lw_los_th_cfg */
        uint16_t lw_special_optical_cfg   : 1; /*!<lw_special_optical_cfg */
        uint16_t lw_eq2_adapt_disable : 1; /*!<lw_eq2_adapt_disable */
        uint16_t ffe_slicer_multiplier : 2; /*!<ffe_slicer_multiplier */
        uint16_t rx_low_bw_en         : 1; /*!<rx_low_bw_en */
#endif
    } fields;
} common_lw_lane_config8_reg_t;

/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIG9_REG 
 ***************************************************************************/
typedef union common_lw_lane_config9_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
      uint16_t los_bypass_val        : 1; /*!<los_bypass_val        */
      uint16_t los_bypass            : 1; /*!<los_bypass            */
      uint16_t phase_tune_dwell_time : 3; /*!<phase_tune_dwell_time */
      uint16_t phase_tune_link_down_snr  : 3; /*!<phase_tune_link_down_snr */
      uint16_t phase_dtune_snr_change_th : 4; /*!<phase_dtune_snr_change_th */
      uint16_t phase_dtune_bias_range    : 4; /*!<phase_dtune_bias_range */
#else /* LITTLE ENDIAN */
      uint16_t phase_dtune_bias_range    : 4; /*!<phase_dtune_bias_range */
      uint16_t phase_dtune_snr_change_th : 4; /*!<phase_dtune_snr_change_th */
      uint16_t phase_tune_link_down_snr  : 3; /*!<phase_tune_link_down_snr */
      uint16_t phase_tune_dwell_time : 3; /*!<phase_tune_dwell_time */
      uint16_t los_bypass            : 1; /*!<los_bypass            */
      uint16_t los_bypass_val        : 1; /*!<los_bypass_val        */
#endif
    } fields;
} common_lw_lane_config9_reg_t;

/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIGA_REG 
 ***************************************************************************/
typedef union common_lw_lane_configa_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t copper_trn_en : 1; /*!<copper_trn_en */
        uint16_t txpi_ppm_en : 1; /*!<txpi_ppm_en */
        uint16_t rx_gain_boost : 5; /*!<rx_gain_boost */
        uint16_t disable_rxflt_rst_tx : 1;   /*!<disable_rxflt_rst_tx */
        uint16_t disable_rxlos_rst_tx : 1;   /*!<disable_rxlos_rst_tx */
        uint16_t enable_cl72_cl93_preset_req : 1;   /*!<enable_cl72_cl93_preset_req */
        uint16_t enable_channelid_autopk : 1;   /*!<enable_channelid_autopk */
        uint16_t enable_cl136_preset1 : 1;   /*!<enable_cl136_preset1 */
        uint16_t disable_lnktrn_bh_txfir_lmt: 1;    /*!<disable_lnktrn_bh_txfir_lmt */
        uint16_t lnktrn_auto_restart: 1;    /*!<lnktrn_auto_restart */
        uint16_t berlinetta_compatible : 1; /*!<berlinetta_compatible */ 
        uint16_t ppm_coverge_en : 1; /*!<ppm_coverge_en */
#else /* LITTLE ENDIAN */        
        uint16_t ppm_coverge_en : 1; /*!<ppm_coverge_en */ 
        uint16_t berlinetta_compatible : 1; /*!<berlinetta_compatible */ 
        uint16_t lnktrn_auto_restart: 1;    /*!<lnktrn_auto_restart */
        uint16_t disable_lnktrn_bh_txfir_lmt: 1;    /*!<disable_lnktrn_bh_txfir_lmt */
        uint16_t enable_cl136_preset1 : 1;   /*!<enable_cl136_preset1 */
        uint16_t enable_channelid_autopk : 1;   /*!<enable_channelid_autopk */
        uint16_t enable_cl72_cl93_preset_req : 1;   /*!<enable_cl72_cl93_preset_req */
        uint16_t disable_rxlos_rst_tx : 1;   /*!<disable_rxlos_rst_tx */
        uint16_t disable_rxflt_rst_tx : 1;   /*!<disable_rxflt_rst_tx */
        uint16_t rx_gain_boost : 5; /*!<rx_gain_boost */
        uint16_t txpi_ppm_en : 1; /*!<txpi_ppm_en */
        uint16_t copper_trn_en : 1; /*!<copper_trn_en */
#endif
    } fields;
} common_lw_lane_configa_reg_t;



/****************************************************************************
* top_pam_amba :: COMMON_LW_LANE_CONFIGC_REG 
***************************************************************************/

/****************************************************************************
* top_pam_amba :: COMMON_LW_LANE_CONFIGC_REG 
***************************************************************************/
typedef union common_lw_lane_configc_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t ignore_maintap_th_chk : 1; /*!<ignore_maintap_th_chk */
        uint16_t ignore_maintap_shapre_chk : 1; /*!<ignore_maintap_shapre_chk */
        uint16_t ignore_fsm_snr_chk: 1;    /*!<ignore_fsm_snr_chk */
        uint16_t ignore_snr_chk : 1; /*!<ignore_snr_chk */ 
        uint16_t ignore_los_chk : 1; /*!<ignore_los_chk */ 
        uint16_t ignore_los_chk_in_mission : 1; /*!<ignore_los_chk_in_mission */ 
        uint16_t ignore_elos_check : 1; /*!<ignore_elos_check */ 
        uint16_t ignore_oplos_check : 1; /*!<ignore_oplos_check */ 
        uint16_t reserve : 2; /*!<reserve */ 
        uint16_t txpi_ovrd_val : 1; /*!<txpi_ovrd_val */ 
        uint16_t txpi_ovrd_en : 1; /*!<txpi_ovrd_en  */ 
        uint16_t lt_pam_preset_type : 2; /*!<lt_pam_preset_type  */ 
        uint16_t fault_ignore_val : 1; /*!<fault_ignore_val  */ 
        uint16_t fault_ignore_en : 1; /*!<fault_ignore_en  */ 
#else /* LITTLE ENDIAN */        
        uint16_t fault_ignore_en : 1; /*!<fault_ignore_en  */ 
        uint16_t fault_ignore_val : 1; /*!<fault_ignore_val  */ 
        uint16_t lt_pam_preset_type : 2; /*!<lt_pam_preset_type  */
        uint16_t txpi_ovrd_val : 1; /*!<txpi_ovrd_val */ 
        uint16_t txpi_ovrd_en : 1; /*!<txpi_ovrd_en  */ 
        uint16_t reserve : 2; /*!<reserve */ 
        uint16_t ignore_oplos_check : 1; /*!<ignore_oplos_check */ 
        uint16_t ignore_elos_check : 1; /*!<ignore_elos_check */ 
        uint16_t ignore_los_chk_in_mission : 1; /*!<ignore_los_chk_in_mission */ 
        uint16_t ignore_los_chk : 1; /*!<ignore_los_chk */ 
        uint16_t ignore_snr_chk : 1; /*!<ignore_snr_chk */ 
        uint16_t ignore_fsm_snr_chk: 1;    /*!<ignore_fsm_snr_chk */
        uint16_t ignore_maintap_shapre_chk : 1; /*!<ignore_maintap_shapre_chk */
        uint16_t ignore_maintap_th_chk : 1; /*!<ignore_maintap_th_chk */
#endif
    } fields;
} common_lw_lane_configc_reg_t;




/****************************************************************************
* top_pam_amba :: COMMON_LW_LAND_CONFIGC_REG 
***************************************************************************/
typedef union common_lw_lane_configd_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN 
        uint16_t reserve: 11;    /*!<reserve */
        uint16_t fault_ignore : 1; /*!<fault_ignore */ 
        uint16_t cdr_lock_times : 4; /*!<cdr_lock_times */ 
#else /* LITTLE ENDIAN */        
        uint16_t cdr_lock_times : 4; /*!<cdr_lock_times */ 
        uint16_t fault_ignore : 1; /*!<fault_ignore */ 
        uint16_t reserve: 11;    /*!<reserve */
#endif
    } fields;
} common_lw_lane_configd_reg_t;


/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIGE_REG 
 ***************************************************************************/
typedef union common_lw_lane_confige_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t lw_rx_fsm : 8; /*!<lw_rx_fsm */
        uint16_t lw_tx_fsm : 8; /*!<lw_tx_fsm */
#else /* LITTLE ENDIAN */
        uint16_t lw_tx_fsm : 8; /*!<lw_tx_fsm */
        uint16_t lw_rx_fsm : 8; /*!<lw_rx_fsm */
#endif
    } fields;
} common_lw_lane_confige_reg_t;



/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIGF_REG 
 ***************************************************************************/
typedef union common_lw_lane_configf_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t lw_lnktrn_rx_fsm : 8; /*!<lw_lnktrn_rx_fsm */
        uint16_t lw_lnktrn_tx_fsm : 8; /*!<lw_lnktrn_tx_fsm */
#else /* LITTLE ENDIAN */
        uint16_t lw_lnktrn_tx_fsm : 8; /*!<lw_lnktrn_tx_fsm */
        uint16_t lw_lnktrn_rx_fsm : 8; /*!<lw_lnktrn_rx_fsm */
#endif
    } fields;
} common_lw_lane_configf_reg_t;


/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIG1B_REG 
 ***************************************************************************/
typedef union common_lw_lane_config1b_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN      
        uint16_t lw_gain_boost_user_override : 1; /*!<lw_gain_boost_user_override */ 
        uint16_t lw_ap_init_val : 5; /*!<lw_ap_init_val */ 
        uint16_t lw_outlos_th_cfg : 5; /*!<lw_outlos_th_cfg */
        uint16_t lw_inlos_th_cfg : 5; /*!<lw_inlos_th_cfg */
#else /* LITTLE ENDIAN */
        uint16_t lw_inlos_th_cfg : 5; /*!<lw_inlos_th_cfg */
        uint16_t lw_outlos_th_cfg : 5; /*!<lw_outlos_th_cfg */  
        uint16_t lw_ap_init_val : 5; /*!<lw_ap_init_val */ 
        uint16_t lw_gain_boost_user_override : 1; /*!<lw_gain_boost_user_override */ 
#endif
    } fields;
} common_lw_lane_config1b_reg_t;

typedef union common_lw_default_config_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t lw_lane7_ext_mode : 1; /*!<lw_lane7_ext_mode */
        uint16_t lw_lane7_copper : 1; /*!<lw_lane7_copper */
        uint16_t lw_lane6_ext_mode : 1; /*!<lw_lane6_ext_mode */
        uint16_t lw_lane6_copper : 1; /*!<lw_lane6_copper */
        uint16_t lw_lane5_ext_mode : 1; /*!<lw_lane5_ext_mode */
        uint16_t lw_lane5_copper : 1; /*!<lw_lane5_copper */
        uint16_t lw_lane4_ext_mode : 1; /*!<lw_lane4_ext_mode */
        uint16_t lw_lane4_copper : 1; /*!<lw_lane4_copper */
        uint16_t lw_lane3_ext_mode : 1; /*!<lw_lane3_ext_mode */
        uint16_t lw_lane3_copper : 1; /*!<lw_lane3_copper */
        uint16_t lw_lane2_ext_mode : 1; /*!<lw_lane2_ext_mode */
        uint16_t lw_lane2_copper : 1; /*!<lw_lane2_copper */
        uint16_t lw_lane1_ext_mode : 1; /*!<lw_lane1_ext_mode */
        uint16_t lw_lane1_copper : 1; /*!<lw_lane1_copper */
        uint16_t lw_lane0_ext_mode : 1; /*!<lw_lane0_ext_mode */
        uint16_t lw_lane0_copper : 1; /*!<lw_lane0_copper */
#else /* LITTLE ENDIAN */
        uint16_t lw_lane0_copper : 1; /*!<lw_lane0_copper */
        uint16_t lw_lane0_ext_mode : 1; /*!<lw_lane0_ext_mode */
        uint16_t lw_lane1_copper : 1; /*!<lw_lane1_copper */
        uint16_t lw_lane1_ext_mode : 1; /*!<lw_lane1_ext_mode */
        uint16_t lw_lane2_copper : 1; /*!<lw_lane2_copper */
        uint16_t lw_lane2_ext_mode : 1; /*!<lw_lane2_ext_mode */
        uint16_t lw_lane3_copper : 1; /*!<lw_lane3_copper */
        uint16_t lw_lane3_ext_mode : 1; /*!<lw_lane3_ext_mode */
        uint16_t lw_lane4_copper : 1; /*!<lw_lane4_copper */
        uint16_t lw_lane4_ext_mode : 1; /*!<lw_lane4_ext_mode */
        uint16_t lw_lane5_copper : 1; /*!<lw_lane5_copper */
        uint16_t lw_lane5_ext_mode : 1; /*!<lw_lane5_ext_mode */
        uint16_t lw_lane6_copper : 1; /*!<lw_lane6_copper */
        uint16_t lw_lane6_ext_mode : 1; /*!<lw_lane6_ext_mode */
        uint16_t lw_lane7_copper : 1; /*!<lw_lane7_copper */
        uint16_t lw_lane7_ext_mode : 1; /*!<lw_lane7_ext_mode */
#endif
    } fields;
} common_lw_default_config_reg_t;

/****************************************************************************
 * top_pam_amba :: COMMON_LW_CONFIG1_REG 
 ***************************************************************************/
typedef union common_lw_config1_reg_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t reserve         : 11;
        uint16_t lw_lms_mode  : 1; /*!<lw_lms_mode */
        uint16_t lw_enable_prbs  : 1; /*!<lw_enable_prbs */
        uint16_t lw_delay_start  : 1; /*!<lw_delay_start */
        uint16_t lw_default_txfir_setting  : 2; /*!<lw_default_txfir_setting */
#else /* LITTLE ENDIAN */
        uint16_t lw_default_txfir_setting  : 2; /*!<lw_default_txfir_setting */
        uint16_t lw_delay_start  : 1; /*!<lw_delay_start */
        uint16_t lw_enable_prbs  : 1; /*!<lw_enable_prbs */
        uint16_t lw_lms_mode  : 1; /*!<lw_lms_mode */
        uint16_t reserve         : 11;
#endif
    } fields;
} common_lw_config1_reg_t;

/****************************************************************************
 * RX config struct in uC RAM accessible by host 
 ***************************************************************************/
typedef struct common_lw_lane_config_s {
    uint8_t rx_config_change;            /**< CAPI set this flag to indicate RX config is changed */
    uint8_t tx_config_change;            /**< CAPI set this flag to indicate TX config is changed */
    uint16_t reserved;                   /**< add 2 bytes padding to make it word aligned because of 4 bytes MDIO read */
    
    struct {
        int16_t    slicer_thres_0;       /**< Slicer Threshold 0 (S3.5) */
        int16_t    slicer_thres_1;       /**< Slicer Threshold 0 (S3.5) */
        int16_t    slicer_thres_2;       /**< Slicer Threshold 0 (S3.5) */
        int16_t    slicer_thres_3;       /**< Slicer Threshold 0 (S3.5) */
        int16_t    slicer_thres_4;       /**< Slicer Threshold 0 (S3.5) */
        int16_t    slicer_thres_5;       /**< Slicer Threshold 0 (S3.5) */

        int16_t    slicer_sym0_val;     /**< Slicer Symbol 0 Value (S3.5) */
        int16_t    slicer_sym1_val;     /**< Slicer Symbol 0 Value (S3.5) */
        int16_t    slicer_sym2_val;     /**< Slicer Symbol 0 Value (S3.5) */
        int16_t    slicer_sym3_val;     /**< Slicer Symbol 0 Value (S3.5) */
        int16_t    slicer_sym4_val;     /**< Slicer Symbol 0 Value (S3.5) */
        int16_t    slicer_sym5_val;     /**< Slicer Symbol 0 Value (S3.5) */
        int16_t    slicer_sym6_val;     /**< Slicer Symbol 0 Value (S3.5) */
        uint8_t    usr_slicer_config;   /**< CAPI set this flag to indicate the FFE config is changed by users */
    } ffe_config;
} common_lw_lane_config_t;

/****************************************************************************
 * LW_TXFIR0_LANE0
 ***************************************************************************/
typedef union common_lw_txfir_config_reg_u {
    int16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        int16_t level_shift : 7; /*!<level_shift */
        int16_t txfir : 9; /*!<txfir */
#else /* LITTLE ENDIAN */
        int16_t txfir : 9; /*!<txfir */
        int16_t level_shift : 7; /*!<level_shift */
#endif
    } fields;
} common_lw_txfir_config_reg_t;

#define DBG_LOG_SIZE  1024
#define FSM_LOG_EGR_OR_IGR_MASK      0x8000
#define FSM_LOG_EGR_OR_IGR_SHIFT     15
#define FSM_LOG_FSM_ST_MASK          0x7C00
#define FSM_LOG_FSM_ST_SHIFT         10
#define FSM_LOG_TIMESTAMP_MASK       0x03FE
#define FSM_LOG_TIMESTAMP_SHIFT      1
#define FSM_LOG_TYPE_MASK            0x0001
#define FSM_LOG_TYPE_SHIFT           0

#define FSM_LOG_WILD_FLAG           0x3F
#define FSM_LOG_WILD_FLAG_MASK      0xFC00
#define FSM_LOG_WILD_FLAG_SHIFT     10
#define FSM_LOG_FSM_TYPE_MASK       0x0003
#define FSM_LOG_FSM_TYPE_SHIFT      0

#define FSM_DIRECT_TS_RATE          2
#define FSM_DIRECT_CU_TS_RATE       2
#define FSM_LT_TS_RATE              5
typedef union lw_fsm_log_s {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
       uint16_t    egr_or_igr : 1;   /*  FSM egress or ingress */
       uint16_t    state      : 5;   /*  FSM log state */
       uint16_t    ts         : 9;  /*  FSM log timer stamp 2ms/5ms uint */
       uint16_t    type       : 1;  /*  FSM log type: top FSM or LT spec */
#else /* LITTLE ENDIAN */
       uint16_t    type       : 1;  /*  FSM log type: top FSM or LT spec */
       uint16_t    ts         : 9;  /*  FSM log timer stamp 2ms/5ms uint */
       uint16_t    state      : 5;   /*  FSM log state */
       uint16_t    egr_or_igr : 1;   /*  FSM egress or ingress */
#endif
    } fields;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
       uint16_t    egr_or_igr : 1;   /*  FSM egress or ingress */
       uint16_t    ts         : 9;   /*  FSM egress or ingress */        
       uint16_t    state      : 5;   /*  FSM log state */
       uint16_t    type       : 1;  /*  FSM log type: top FSM or LT spec */
#else /* LITTLE ENDIAN */
       uint16_t    type       : 1;  /*  FSM log type: top FSM or LT spec */
       uint16_t    ts         : 9;  /*  FSM egress or ingress */     
       uint16_t    state      : 5;   /*  FSM log state */
       uint16_t    egr_or_igr : 1;   /*  FSM egress or ingress */
#endif
    } lt_fields;
    uint16_t      lt_ctrl;
    uint16_t      lt_state;
}lw_fsm_log_t;


typedef union lw_fsm_log_wcfg_s {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
       uint16_t    wldflg     : 6;   /*  FSM wild flag */
       uint16_t    rsrv       : 8;   /*  reserved*/
       uint16_t    fsmt       : 2;  /*  fsm type*/
#else /* LITTLE ENDIAN */
       uint16_t    fsmt       : 2;  /*  fsm type*/
       uint16_t    rsrv       : 8;   /*  reserved*/
       uint16_t    wldflg     : 6;   /*  FSM wild flag */
#endif
    } fields;
}lw_fsm_log_wcfg_t;



/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_STATUS_REG 
 ***************************************************************************/
#define COMMON_LW_LANE_STATUS_REG_COMMAND_ACCEPTED_MASK 0x00008000 /*!< COMMAND_ACCEPTED */
#define COMMON_LW_LANE_STATUS_REG_COMMAND_ACCEPTED_SHIFT 15
#define COMMON_LW_LANE_STATUS_REG_RESERVED0_MASK 0x00004000 /*!< RESERVED0 */
#define COMMON_LW_LANE_STATUS_REG_RESERVED0_SHIFT 14
#define COMMON_LW_LANE_STATUS_REG_ERROR_CODE_MASK 0x00003f00 /*!< ERROR_CODE */
#define COMMON_LW_LANE_STATUS_REG_ERROR_CODE_SHIFT 8
#define COMMON_LW_LANE_STATUS_REG_STATUS_RETURN_MASK 0x000000ff /*!< STATUS_RETURN */
#define COMMON_LW_LANE_STATUS_REG_STATUS_RETURN_SHIFT 0

/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIG1_REG  GP0
 ***************************************************************************/

#define COMMON_LW_LANE_CONFIG1_REG_LW_LNK_TRN_SYSCDR_FIRST_MASK 0x00004000 /*!< LW_LNK_TRN_SYSCDR_FIRST */
#define COMMON_LW_LANE_CONFIG1_REG_LW_LNK_TRN_SYSCDR_FIRST_SHIFT 14
#define COMMON_LW_LANE_CONFIG1_REG_LW_LNK_TRN_DISABLE_TIMER_MASK 0x00002000 /*!< LW_LNK_TRN_DISABLE_TIMER */
#define COMMON_LW_LANE_CONFIG1_REG_LW_LNK_TRN_DISABLE_TIMER_SHIFT 13
#define COMMON_LW_LANE_CONFIG1_REG_LW_LNK_TRN_RESTART_MASK 0x00001000 /*!< LW_LNK_TRN_RESTART */
#define COMMON_LW_LANE_CONFIG1_REG_LW_LNK_TRN_RESTART_SHIFT 12
#define COMMON_LW_LANE_CONFIG1_REG_LW_LNK_TRN_INIT_COND_MASK 0x00000c00 /*!< LW_LNK_TRN_INIT_COND */
#define COMMON_LW_LANE_CONFIG1_REG_LW_LNK_TRN_INIT_COND_SHIFT 10
#define COMMON_LW_LANE_CONFIG1_REG_LW_AUTO_NEG_MASK 0x00000200 /*!< LW_AUTO_NEG */
#define COMMON_LW_LANE_CONFIG1_REG_LW_AUTO_NEG_SHIFT 9
#define COMMON_LW_LANE_CONFIG1_REG_LW_LNK_TRN_FRAME_SIZE_MASK 0x00000100 /*!< LW_LNK_TRN_FRAME_SIZE */
#define COMMON_LW_LANE_CONFIG1_REG_LW_LNK_TRN_FRAME_SIZE_SHIFT 8
#define COMMON_LW_LANE_CONFIG1_REG_LW_LINK_TRAINING_MASK 0x000000e0 /*!< LW_LINK_TRAINING */
#define COMMON_LW_LANE_CONFIG1_REG_LW_LINK_TRAINING_SHIFT 5
#define COMMON_LW_LANE_CONFIG1_REG_LW_GLOOP_MASK 0x00000010 /*!< LW_GLOOP */
#define COMMON_LW_LANE_CONFIG1_REG_LW_GLOOP_SHIFT 4
#define COMMON_LW_LANE_CONFIG1_REG_LW_RLOOP_MASK 0x00000008 /*!< LW_RLOOP */
#define COMMON_LW_LANE_CONFIG1_REG_LW_RLOOP_SHIFT 3
#define COMMON_LW_LANE_CONFIG1_REG_LW_EXTD_MODE_MASK 0x00000006 /*!< LW_EXTD_MODE */
#define COMMON_LW_LANE_CONFIG1_REG_LW_EXTD_MODE_SHIFT 1
#define COMMON_LW_LANE_CONFIG1_REG_LW_OPTICAL_MODE_MASK 0x00000001 /*!< LW_OPTICAL_MODE */
#define COMMON_LW_LANE_CONFIG1_REG_LW_OPTICAL_MODE_SHIFT 0

/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIG2_REG  GP1
 ***************************************************************************/

#define COMMON_LW_LANE_CONFIG2_REG_LW_RX_DISABLE_MASK 0x00008000 /*!< LW_RX_DISABLEE */
#define COMMON_LW_LANE_CONFIG2_REG_LW_RX_DISABLE_SHIFT 15
#define COMMON_LW_LANE_CONFIG2_REG_LW_KP_TRACKING_HALF_STEP_MASK 0x00004000 /*!< LW_KP_TRACKING_HALF_STEP */
#define COMMON_LW_LANE_CONFIG2_REG_LW_KP_TRACKING_HALF_STEP_SHIFT 14
#define COMMON_LW_LANE_CONFIG2_REG_LW_KP_HALF_STEP_MASK 0x00002000 /*!< LW_KP_HALF_STEP */
#define COMMON_LW_LANE_CONFIG2_REG_LW_KP_HALF_STEP_SHIFT 13
#define COMMON_LW_LANE_CONFIG2_REG_LW_PEAKING_MASK 0x00001f00 /*!< LW_PEAKING */
#define COMMON_LW_LANE_CONFIG2_REG_LW_PEAKING_SHIFT 8
#define COMMON_LW_LANE_CONFIG2_REG_LW_PEAKING_EN_MASK 0x00000080 /*!< LW_PEAKING_EN */
#define COMMON_LW_LANE_CONFIG2_REG_LW_PEAKING_EN_SHIFT 7
#define COMMON_LW_LANE_CONFIG2_REG_LW_LOG_IGNORE_MASK 0x00000040 /*!< LW_LOS_IGNORE */
#define COMMON_LW_LANE_CONFIG2_REG_LW_LOG_IGNORE_SHIFT 6
#define COMMON_LW_LANE_CONFIG2_REG_LW_SNR_LVL_MASK 0x00000038 /*!< LW_SNR_LVL */
#define COMMON_LW_LANE_CONFIG2_REG_LW_SNR_LVL_SHIFT 3
#define COMMON_LW_LANE_CONFIG2_REG_LW_RX_GRAYCODE_MASK 0x00000004 /*!< LW__RX_GRAYCODE */
#define COMMON_LW_LANE_CONFIG2_REG_LW_RX_GRAYCODE_SHIFT 2
#define COMMON_LW_LANE_CONFIG2_REG_LW_RX_SYMBOL_SWAP_MASK 0x00000002 /*!< LW_RX_SYMBOL_SWAP */
#define COMMON_LW_LANE_CONFIG2_REG_LW_RX_SYMBOL_SWAP_SHIFT 1
#define COMMON_LW_LANE_CONFIG2_REG_LW_INVERT_RX_MASK 0x00000001 /*!< LW_INVERT_RX */
#define COMMON_LW_LANE_CONFIG2_REG_LW_INVERT_RX_SHIFT 0

/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIG3_REG  GP2
 ***************************************************************************/
#define COMMON_LW_LANE_CONFIG3_REG_LW_KF_TRACKING_MASK 0x0000e000 /*!<LW_ KF_TRACKING */
#define COMMON_LW_LANE_CONFIG3_REG_LW_KF_TRACKING_SHIFT 13
#define COMMON_LW_LANE_CONFIG3_REG_LW_KF_OVERRIDE_TRACKING_MASK 0x00001000 /*!<LW_ KF_TRACKING_OVERRIDE */
#define COMMON_LW_LANE_CONFIG3_REG_LW_KF_OVERRIDE_TRACKING_SHIFT 12
#define COMMON_LW_LANE_CONFIG3_REG_LW_KF_MASK 0x00000e00 /*!< LW_KF */
#define COMMON_LW_LANE_CONFIG3_REG_LW_KF_SHIFT 9
#define COMMON_LW_LANE_CONFIG3_REG_LW_KF_OVERRIDE_MASK 0x00000100 /*!< LW_KF_OVERRIDE */
#define COMMON_LW_LANE_CONFIG3_REG_LW_KF_OVERRIDE_SHIFT 8
#define COMMON_LW_LANE_CONFIG3_REG_LW_KP_TRACKING_MASK 0x000000e0 /*!< LW_KP_TRACKING */
#define COMMON_LW_LANE_CONFIG3_REG_LW_KP_TRACKING_SHIFT 5
#define COMMON_LW_LANE_CONFIG3_REG_LW_KP_TRACKING_OVERRIDE_MASK 0x00000010 /*!< LW_KP_TRACKING_OVERRIDE */
#define COMMON_LW_LANE_CONFIG3_REG_LW_KP_TRACKING_OVERRIDE_SHIFT 4
#define COMMON_LW_LANE_CONFIG3_REG_LW_KP_MASK 0x0000000e /*!< LW_KP */
#define COMMON_LW_LANE_CONFIG3_REG_LW_KP_SHIFT 1
#define COMMON_LW_LANE_CONFIG3_REG_LW_KP_OVERRIDE_MASK 0x00000001 /*!< LW_KP_OVERRIDE */
#define COMMON_LW_LANE_CONFIG3_REG_LW_KP_OVERRIDE_SHIFT 0

/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIG4_REG  GP3
 ***************************************************************************/
#define COMMON_LW_LANE_CONFIG4_REG_LW_GAIN2_OVRD_VAL_MASK 0x00018000 /*!< LW_GAIN2_OVRD_VAL */
#define COMMON_LW_LANE_CONFIG4_REG_LW_GAIN2_OVRD_VAL_SHIFT 15
#define COMMON_LW_LANE_CONFIG4_REG_LW_GAIN2_OVRD_EN_MASK 0x00004000 /*!< LW_GAIN2_OVRD_EN*/
#define COMMON_LW_LANE_CONFIG4_REG_LW_GAIN2_OVRD_EN_SHIFT 14
#define COMMON_LW_LANE_CONFIG4_REG_LW_EQ2_ENABLE_MASK 0x00002000 /*!< LW_EQ2_ENABLE */
#define COMMON_LW_LANE_CONFIG4_REG_LW_EQ2_ENABLE_SHIFT 13
#define COMMON_LW_LANE_CONFIG4_REG_LW_EQ_TAP_SEL_MASK 0x0001800 /*!< LW_EQ_TAP_SEL */
#define COMMON_LW_LANE_CONFIG4_REG_LW_EQ_TAP_SEL_SHIFT 11
#define COMMON_LW_LANE_CONFIG4_REG_LW_EQ2_TYPE_MASK 0x0000400 /*!< LW_EQ2_TYPE */
#define COMMON_LW_LANE_CONFIG4_REG_LW_EQ2_TYPE_SHIFT 10
#define COMMON_LW_LANE_CONFIG4_REG_LW_PHD_MODE_MASK 0x00000200 /*!< LW_PHD_MODE */
#define COMMON_LW_LANE_CONFIG4_REG_LW_PHD_MODE_SHIFT 9
#define COMMON_LW_LANE_CONFIG4_REG_LW_PHD_MODE_OVERRIDE_MASK 0x00000100 /*!< LW_PHD_MODE_OVERRIDE */
#define COMMON_LW_LANE_CONFIG4_REG_LW_PHD_MODE_OVERRIDE_SHIFT 8
#define COMMON_LW_LANE_CONFIG4_REG_LW_FFE_MU_TRACKING_MASK 0x000000e0 /*!< LW_FFE_MU_TRACKING */
#define COMMON_LW_LANE_CONFIG4_REG_LW_FFE_MU_TRACKING_SHIFT 5
#define COMMON_LW_LANE_CONFIG4_REG_LW_FFE_MU_TRACKING_OVERRIDE_MASK 0x00000010 /*!< LW_FFE_MU_TRACKING OVERRIDE */
#define COMMON_LW_LANE_CONFIG4_REG_LW_FFE_MU_TRACKING_OVERRIDE_SHIFT 4
#define COMMON_LW_LANE_CONFIG4_REG_LW_FFE_MU_MASK 0x0000000e /*!< LW_FFE_MU */
#define COMMON_LW_LANE_CONFIG4_REG_LW_FFE_MU_SHIFT 1
#define COMMON_LW_LANE_CONFIG4_REG_LW_FFE_MU_OVERRIDE_MASK 0x00000001 /*!< LW_FFE_MU OVERRIDE*/
#define COMMON_LW_LANE_CONFIG4_REG_LW_FFE_MU_OVERRIDE_SHIFT 0

/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIG5_REG GP4
 ***************************************************************************/

#define COMMON_LW_LANE_CONFIG5_REG_LW_TXFIR_TYPE_MASK 0x00070000 /*!< LW_TXFIR_TYPE */
#define COMMON_LW_LANE_CONFIG5_REG_LW_TXFIR_TYPE_SHIFT 16

#define COMMON_LW_LANE_CONFIG5_REG_LW_TX_SQUELCH_MASK 0x00008000 /*!< LW_TX_ELEC_IDLE*/
#define COMMON_LW_LANE_CONFIG5_REG_LW_TX_SQUELCH_SHIFT 15
#define COMMON_LW_LANE_CONFIG5_REG_LW_TX_DISABLE_MASK 0x00004000 /*!< LW_TX_DISABLE*/
#define COMMON_LW_LANE_CONFIG5_REG_LW_TX_DISABLE_SHIFT 14
#define COMMON_LW_LANE_CONFIG5_REG_LW_TX_USER_OVERRIDE_MASK 0x00002000 /*!< LW_TX_USER_OVERRIDE*/
#define COMMON_LW_LANE_CONFIG5_REG_LW_TX_USER_OVERRIDE_SHIFT 13
#define COMMON_LW_LANE_CONFIG5_REG_LW_TX_HIGH_SWING_MASK 0x00001000 /*!< LW_TX_HIGH_SWING */
#define COMMON_LW_LANE_CONFIG5_REG_LW_TX_HIGH_SWING_SHIFT 12
#define COMMON_LW_LANE_CONFIG5_REG_LW_TXFIR_7TAP_EN_MASK 0x00000800 /*!< LW_TXFIR_7TAP_EN */
#define COMMON_LW_LANE_CONFIG5_REG_LW_TXFIR_7TAP_EN_SHIFT 11
#define COMMON_LW_LANE_CONFIG5_REG_LW_TX_PRECODE_INDEP_MASK 0x00000600 /*!< LW_TX_PRECODE_INDEP */
#define COMMON_LW_LANE_CONFIG5_REG_LW_TX_PRECODE_INDEP_SHIFT 9

#define COMMON_LW_LANE_CONFIG5_REG_LW_TX_PRBS_OR_SQUELCH_MASK 0x00000100 /*!< LW_TX_PRBS_OR_SQUELCH */
#define COMMON_LW_LANE_CONFIG5_REG_LW_TX_PRBS_OR_SQUELCH_SHIFT 8
#define COMMON_LW_LANE_CONFIG5_REG_LW_TX_GRAYCODE_MASK 0x00000080 /*!< LW_TX_GRAYCODE */
#define COMMON_LW_LANE_CONFIG5_REG_LW_TX_GRAYCODE_SHIFT 7
#define COMMON_LW_LANE_CONFIG5_REG_LW_PRBS_SELECT_MASK 0x00000070 /*!< LW_PRBS_SELECT */
#define COMMON_LW_LANE_CONFIG5_REG_LW_PRBS_SELECT_SHIFT 4
#define COMMON_LW_LANE_CONFIG5_REG_LW_TX_DATAPATH_SEL_MASK 0x0000000c /*!< LW_TX_DATAPATH_SEL */
#define COMMON_LW_LANE_CONFIG5_REG_LW_TX_DATAPATH_SEL_SHIFT 2
#define COMMON_LW_LANE_CONFIG5_REG_LW_TX_SYMBOL_SWAP_MASK 0x00000002 /*!< LW_TX_SYMBOL_SWAP */
#define COMMON_LW_LANE_CONFIG5_REG_LW_TX_SYMBOL_SWAP_SHIFT 1
#define COMMON_LW_LANE_CONFIG5_REG_LW_INVERT_TX_MASK 0x00000001 /*!< LW_INVERT_TX */
#define COMMON_LW_LANE_CONFIG5_REG_LW_INVERT_TX_SHIFT 0


/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIG6_REG GP5
 ***************************************************************************/
#define COMMON_LW_LANE_CONFIG6_REG_LW_PP_MTAP_COEF_MASK 0x0000ff00 /*!< LW_PP_MTAP_COEF */
#define COMMON_LW_LANE_CONFIG6_REG_LW_PP_MTAP_COEF_SHIFT 8
#define COMMON_LW_LANE_CONFIG6_REG_LW_MTAP_COEF_MASK 0x000000ff /*!< LW_MTAP_COEF */
#define COMMON_LW_LANE_CONFIG6_REG_LW_MTAP_COEF_SHIFT 0

/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIG7_REG  GP6
 ***************************************************************************/ 
#define COMMON_LW_LANE_CONFIG7_REG_LW_PHBIAS_ENABLE_DTUNE_LIMIT_MASK 0x00008000 /*!< LW_PHBIAS_ENABLE_DTUNE_LIMIT  */
#define COMMON_LW_LANE_CONFIG7_REG_LW_PHBIAS_ENABLE_DTUNE_LIMIT_SHIFT 15
#define COMMON_LW_LANE_CONFIG7_REG_LW_PHBIAS_STEP_SIZE_MASK 0x00004000 /*!< LW_PHBIAS_STEP_SIZE  */
#define COMMON_LW_LANE_CONFIG7_REG_LW_PHBIAS_STEP_SIZE_SHIFT 14
#define COMMON_LW_LANE_CONFIG7_REG_LW_DISABLE_D_PHBIAS_MASK 0x00002000 /*!< LW_DISABLE_D_PHBIAS */
#define COMMON_LW_LANE_CONFIG7_REG_LW_DISABLE_D_PHBIAS_SHIFT 13
#define COMMON_LW_LANE_CONFIG7_REG_LW_DISABLE_S_PHBIAS_MASK 0x00001000 /*!< LW_DISABLE_S_PHBIAS */
#define COMMON_LW_LANE_CONFIG7_REG_LW_DISABLE_S_PHBIAS_SHIFT 12
#define COMMON_LW_LANE_CONFIG7_REG_LW_PHBIAS_MAX_MASK 0x00000fc0 /*!< LW_PHBIAS_MAX */
#define COMMON_LW_LANE_CONFIG7_REG_LW_PHBIAS_MAX_SHIFT 6
#define COMMON_LW_LANE_CONFIG7_REG_LW_PHBIAS_MIN_MASK 0x0000003f /*!< LW_PHBIAS_MIN */
#define COMMON_LW_LANE_CONFIG7_REG_LW_PHBIAS_MIN_SHIFT 0

/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIG8_REG  GP7
 ***************************************************************************/
#define COMMON_LW_LANE_CONFIG8_REG_LW_MAINTAP_EXT_MASK 0x001F0000 /*!< LW_MAINTAP_EXT */
#define COMMON_LW_LANE_CONFIG8_REG_LW_MAINTAP_EXT_SHIFT 16
#define COMMON_LW_LANE_CONFIG8_REG_RX_LOW_BW_EN_MASK 0x00008000 /*!< RX_LOW_BW_EN*/
#define COMMON_LW_LANE_CONFIG8_REG_RX_LOW_BW_EN_SHIFT 15
#define COMMON_LW_LANE_CONFIG8_REG_FFE_SLICER_MULTIPLIER_MASK 0x00006000 /*!< FFE_SLICER_MULTIPLIER*/
#define COMMON_LW_LANE_CONFIG8_REG_FFE_SLICER_MULTIPLIER_SHIFT 13
#define COMMON_LW_LANE_CONFIG8_REG_LW_EQ2_ADAPT_DISABLE_MASK 0x00001000 /*!< LW_EQ2_ADAPT_DISABLE*/
#define COMMON_LW_LANE_CONFIG8_REG_LW_EQ2_ADAPT_DISABLE_SHIFT 12
#define COMMON_LW_LANE_CONFIG8_REG_LW_SPECIAL_OPTICAL_CFG_MASK 0x00000800 /*!< LW_SPECIAL_OPTICAL_CFG*/
#define COMMON_LW_LANE_CONFIG8_REG_LW_SPECIAL_OPTICAL_CFG_SHIFT 11
#define COMMON_LW_LANE_CONFIG8_REG_LW_LOS_TH_CFG_MASK 0x00000780 /*!< LW_LOS_TH_CFG*/
#define COMMON_LW_LANE_CONFIG8_REG_LW_LOS_TH_CFG_SHIFT 7
#define COMMON_LW_LANE_CONFIG8_REG_LW_DC_WANDER_MU_MASK 0x00000070 /*!< LW_DC_WANDER_MU*/
#define COMMON_LW_LANE_CONFIG8_REG_LW_DC_WANDER_MU_SHIFT 4
#define COMMON_LW_LANE_CONFIG8_REG_LW_DC_WANDER_MU_OVERRIDE_MASK 0x00000008 /*!< LW_DC_WANDER_MU_OVERRIDE*/
#define COMMON_LW_LANE_CONFIG8_REG_LW_DC_WANDER_MU_OVERRIDE_SHIFT 3
#define COMMON_LW_LANE_CONFIG8_REG_LW_MAINTAP_MASK 0x00000007 /*!< LW_MAINTAP */
#define COMMON_LW_LANE_CONFIG8_REG_LW_MAINTAP_SHIFT 0

/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIG9_REG GP8
 ***************************************************************************/
#define COMMON_LW_LANE_CONFIG9_REG_LOS_BYPASS_VAL_MASK 0x00008000 /*!< LOS_BYPASS_VAL */
#define COMMON_LW_LANE_CONFIG9_REG_LOS_BYPASS_VAL_SHIFT 15
#define COMMON_LW_LANE_CONFIG9_REG_LOS_BYPASS_MASK 0x00004000 /*!< LOS_BYPASS */
#define COMMON_LW_LANE_CONFIG9_REG_LOS_BYPASS_SHIFT 14
#define COMMON_LW_LANE_CONFIG9_REG_PHASE_TUNE_DWELL_TIME_MASK 0x00003800 /*!< PHASE_TUNE_DWELL_TIME */
#define COMMON_LW_LANE_CONFIG9_REG_PHASE_TUNE_DWELL_TIME_SHIFT 11

#define COMMON_LW_LANE_CONFIG9_REG_PHASE_TUNE_LINK_DOWN_SNR_MASK 0x00000700 /*!< PHASE_TUNE_LINK_DOWN_SNR */
#define COMMON_LW_LANE_CONFIG9_REG_PHASE_TUNE_LINK_DOWN_SNR_SHIFT 8
#define COMMON_LW_LANE_CONFIG9_REG_PHASE_DTUNE_SNR_CHANGE_TH_MASK 0x000000F0 /*!< PHASE_DTUNE_SNR_CHANGE_TH */
#define COMMON_LW_LANE_CONFIG9_REG_PHASE_DTUNE_SNR_CHANGE_TH_SHIFT 4
#define COMMON_LW_LANE_CONFIG9_REG_PHASE_DTUNE_BIAS_RANGE_MASK 0x0000000F /*!< PHASE_DTUNE_BIAS_RANGE */
#define COMMON_LW_LANE_CONFIG9_REG_PHASE_DTUNE_BIAS_RANGE_SHIFT 0


/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIGA_REG  GP9
 ***************************************************************************/

#define COMMON_LW_LANE_CONFIGA_REG_LW_EN_COPPER_TRN_MASK 0x00008000 /*!< LW_EN_COPPER_TRN */
#define COMMON_LW_LANE_CONFIGA_REG_LW_EN_COPPER_TRN_SHIFT 15
#define COMMON_LW_LANE_CONFIGA_REG_LW_EN_TXPI_PPM_MASK 0x00004000 /*!< LW_EN_TXPI_PPM */
#define COMMON_LW_LANE_CONFIGA_REG_LW_EN_TXPI_PPM_SHIFT 14
#define COMMON_LW_LANE_CONFIGA_REG_RX_GAIN_BOOST_MASK 0x00003E00 /*!< RX_GAIN_BOOST */
#define COMMON_LW_LANE_CONFIGA_REG_RX_GAIN_BOOST_SHIFT 9
#define COMMON_LW_LANE_CONFIGA_REG_LW_LT_DIS_RXFLT_RESTART_TX_MASK 0x00000100 /*!< LW_LT_DIS_RXFLT_RESTART_TX */
#define COMMON_LW_LANE_CONFIGA_REG_LW_LT_DIS_RXFLT_RESTART_TX_SHIFT 8
#define COMMON_LW_LANE_CONFIGA_REG_LW_LT_DIS_RXLOS_RESTART_TX_MASK 0x00000080 /*!< LW_LT_DIS_RXLOS_RESTART_TX */
#define COMMON_LW_LANE_CONFIGA_REG_LW_LT_DIS_RXLOS_RESTART_TX_SHIFT 7
#define COMMON_LW_LANE_CONFIGA_REG_LW_NRZ_TRN_REQ_PRESET_MASK 0x00000040 /*!< LW_NRZ_TRN_REQ_PRESET */
#define COMMON_LW_LANE_CONFIGA_REG_LW_NRZ_TRN_REQ_PRESET_SHIFT 6
#define COMMON_LW_LANE_CONFIGA_REG_LW_EN_CHANNEL_ID_AUTOPK_MASK 0x00000020 /*!< LW_EN_CHANNEL_ID_AUTOPK */
#define COMMON_LW_LANE_CONFIGA_REG_LW_EN_CHANNEL_ID_AUTOPK_SHIFT 5
#define COMMON_LW_LANE_CONFIGA_REG_LW_PAM4_TRN_USE_PRESET1_MASK 0x00000010 /*!< LW_PAM4_TRN_USE_PRESET1 */
#define COMMON_LW_LANE_CONFIGA_REG_LW_PAM4_TRN_USE_PRESET1_SHIFT 4
#define COMMON_LW_LANE_CONFIGA_REG_LW_DISABLE_LNKTRN_BH_TXFIR_LMT_MASK 0x00000008 /*!< LW_DISABLE_LNKTRN_BH_TXFIR_LMT */
#define COMMON_LW_LANE_CONFIGA_REG_LW_DISABLE_LNKTRN_BH_TXFIR_LMT_SHIFT 3
#define COMMON_LW_LANE_CONFIGA_REG_LW_LNKTRN_AUTO_RESTART_MASK 0x00000004 /*!< LW_LNKTRN_AUTO_RESTART */
#define COMMON_LW_LANE_CONFIGA_REG_LW_LNKTRN_AUTO_RESTART_SHIFT 2
#define COMMON_LW_LANE_CONFIGA_REG_LW_EN_CHANNEL_ID_AP_IN_MISSION_MASK 0x00000002 /*!< LW_EN_CHANNEL_ID_AP_IN_MISSION */
#define COMMON_LW_LANE_CONFIGA_REG_LW_EN_CHANNEL_ID_AP_IN_MISSION_SHIFT 1
#define COMMON_LW_LANE_CONFIGA_REG_LW_EN_LNKTRN_PPM_CONVERGE_MASK 0x00000001 /*!< LW_EN_LNKTRN_PPM_CONVERGE */
#define COMMON_LW_LANE_CONFIGA_REG_LW_EN_LNKTRN_PPM_CONVERGE_SHIFT 0


/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIGB_REG  GP10
 ***************************************************************************/
/*Link up time */



/****************************************************************************
* top_pam_amba :: COMMON_LW_LANE_CONFIGC_REG  GP11
***************************************************************************/
#define COMMON_LW_LANE_CONFIGC_REG_IGNORE_MAINTAP_TH_CHK_MASK 0x00008000
#define COMMON_LW_LANE_CONFIGC_REG_IGNORE_MAINTAP_TH_CHK_SHIFT 15
#define COMMON_LW_LANE_CONFIGC_REG_IGNORE_MAINTAP_SHAPE_CHK_MASK 0x00004000
#define COMMON_LW_LANE_CONFIGC_REG_IGNORE_MAINTAP_SHAPE_CHK_SHIFT 14
#define COMMON_LW_LANE_CONFIGC_REG_IGNORE_FSM_SNR_CHK_MASK 0x00002000
#define COMMON_LW_LANE_CONFIGC_REG_IGNORE_FSM_SNR_CHK_SHIFT 13
#define COMMON_LW_LANE_CONFIGC_REG_IGNORE_SNR_CHK_MASK 0x00001000
#define COMMON_LW_LANE_CONFIGC_REG_IGNORE_SNR_CHK_SHIFT 12
#define COMMON_LW_LANE_CONFIGC_REG_IGNORE_LOS_CHK_MASK 0x00000800
#define COMMON_LW_LANE_CONFIGC_REG_IGNORE_LOS_CHK_SHIFT 11
#define COMMON_LW_LANE_CONFIGC_REG_IGNORE_LOS_CHK_MISSION_MASK 0x00000400
#define COMMON_LW_LANE_CONFIGC_REG_IGNORE_LOS_CHK_MISSION_SHIFT 10
#define COMMON_LW_LANE_CONFIGC_REG_IGNORE_PPM_CHK_MASK 0x000000200
#define COMMON_LW_LANE_CONFIGC_REG_IGNORE_PPM_CHK_SHIFT 9

#define COMMON_LW_LANE_CONFIGC_REG_IGNORE_ELOS_CHK_MASK 0x000000080
#define COMMON_LW_LANE_CONFIGC_REG_IGNORE_ELOS_CHK_SHIFT 7
#define COMMON_LW_LANE_CONFIGC_REG_IGNORE_OPLOS_CHK_MASK 0x00000040
#define COMMON_LW_LANE_CONFIGC_REG_IGNORE_OPLOS_CHK_SHIFT 6


#define COMMON_LW_LANE_CONFIGC_REG_CFG_TXPI_EN_OVERRIDE_VAL_MASK 0x000000020
#define COMMON_LW_LANE_CONFIGC_REG_CFG_TXPI_EN_OVERRIDE_VAL_SHIFT 5
#define COMMON_LW_LANE_CONFIGC_REG_CFG_TXPI_EN_OVERRIDE_MASK 0x000000010
#define COMMON_LW_LANE_CONFIGC_REG_CFG_TXPI_EN_OVERRIDE_SHIFT 4


#define COMMON_LW_LANE_CONFIGC_REG_FAULT_IGNORE_MVAL_MASK 0x00000002
#define COMMON_LW_LANE_CONFIGC_REG_FAULT_IGNORE_MVAL_SHIFT 1
#define COMMON_LW_LANE_CONFIGC_REG_FAULT_IGNORE_OVERRIDE_MASK 0x00000001
#define COMMON_LW_LANE_CONFIGC_REG_FAULT_IGNORE_OVERRIDE_SHIFT 0


/****************************************************************************
* top_pam_amba :: COMMON_LW_LANE_CONFIGD_REG  GP12
***************************************************************************/

#define COMMON_LW_LANE_CONFIGD_REG_LW_UC_READY_MASK 0x000008000
#define COMMON_LW_LANE_CONFIGD_REG_LW_UC_READY_SHIFT 15
#define COMMON_LW_LANE_CONFIGD_REG_CU_DCOSKEW_ON_MASK    0x00004000
#define COMMON_LW_LANE_CONFIGD_REG_CU_DCOSKEW_ON_SHIFT   14

#define COMMON_LW_LANE_CONFIGD_REG_ADCCAL_VCODE_EXCEED_MASK    0x00002000
#define COMMON_LW_LANE_CONFIGD_REG_ADCCAL_VCODE_EXCEED_SHIFT   13

#define COMMON_LW_LANE_CONFIGD_REG_INTEG_PPM_ADJ_MASK    0x00001800
#define COMMON_LW_LANE_CONFIGD_REG_INTEG_PPM_ADJ_SHIFT   11

#define COMMON_LW_LANE_CONFIGD_REG_FAST_CMIS_SNR_ST_MASK 0x0000400
#define COMMON_LW_LANE_CONFIGD_REG_FAST_CMIS_SNR_ST_SHIFT 10

#define COMMON_LW_LANE_CONFIGD_REG_PLL_RECAL_STICKY_MASK 0x0000200
#define COMMON_LW_LANE_CONFIGD_REG_PLL_RECAL_STICKY_SHIFT 9
#define COMMON_LW_LANE_CONFIGD_REG_ADCCAL_RECAL_STICKY_MASK 0x0000100
#define COMMON_LW_LANE_CONFIGD_REG_ADCCAL_RECAL_STICKY_SHIFT 8

#define COMMON_LW_LANE_CONFIGD_REG_TX_SQUELCH_MASK 0x0000080
#define COMMON_LW_LANE_CONFIGD_REG_TX_SQUELCH_SHIFT 7

#define COMMON_LW_LANE_CONFIGD_REG_RX_SUSPEND_MASK 0x000000040
#define COMMON_LW_LANE_CONFIGD_REG_RX_SUSPEND_SHIFT 6
#define COMMON_LW_LANE_CONFIGD_REG_TX_SUSPEND_MASK 0x000000020
#define COMMON_LW_LANE_CONFIGD_REG_TX_SUSPEND_SHIFT 5
#define COMMON_LW_LANE_CONFIGD_REG_FAULT_IGNORE_MASK 0x000000010
#define COMMON_LW_LANE_CONFIGD_REG_FAULT_IGNORE_SHIFT 4
#define COMMON_LW_LANE_CONFIGD_REG_CDR_LOCK_COUNTER_MASK 0x0000000F
#define COMMON_LW_LANE_CONFIGD_REG_CDR_LOCK_COUNTER_SHIFT 0


/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIGE_REG  GP13
 ***************************************************************************/
#define COMMON_LW_LANE_CONFIGE_REG_RX_FSM_STATE_MASK 0x0000ff00
#define COMMON_LW_LANE_CONFIGE_REG_RX_FSM_STATE_SHIFT 8
#define COMMON_LW_LANE_CONFIGE_REG_TX_FSM_STATE_MASK 0x000000ff
#define COMMON_LW_LANE_CONFIGE_REG_TX_FSM_STATE_SHIFT 0

/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIGF_REG  GP14
 ***************************************************************************/
#define COMMON_LW_LANE_CONFIGF_REG_RX_LNKTRN_FSM_STATE_MASK 0x0000ff00
#define COMMON_LW_LANE_CONFIGF_REG_RX_LNKTRN_FSM_STATE_SHIFT 8
#define COMMON_LW_LANE_CONFIGF_REG_TX_LNKTRN_FSM_STATE_MASK 0x000000ff
#define COMMON_LW_LANE_CONFIGF_REG_TX_LNKTRN_FSM_STATE_SHIFT 0

/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIG10_REG  GP15
 ***************************************************************************/

/*Fault log*/
#define COMMON_LW_LANE_CONFIG10_REG_RX_LT_LOST_SYNC_RMT_MASK 0x00008000
#define COMMON_LW_LANE_CONFIG10_REG_RX_LT_LOST_SYNC_RMT_SHIFT 15
#define COMMON_LW_LANE_CONFIG10_REG_RX_LT_LOST_SYNC_LOCAL_MASK 0x00004000
#define COMMON_LW_LANE_CONFIG10_REG_RX_LT_LOST_SYNC_LOCAL_SHIFT 14
#define COMMON_LW_LANE_CONFIG10_REG_RX_LT_FSM_SNR_LOW_MASK 0x00002000
#define COMMON_LW_LANE_CONFIG10_REG_RX_LT_FSM_SNR_LOW_SHIFT 13
#define COMMON_LW_LANE_CONFIG10_REG_RX_LT_SNR_LOW_MASK 0x00001000
#define COMMON_LW_LANE_CONFIG10_REG_RX_LT_SNR_LOW_SHIFT 12
#define COMMON_LW_LANE_CONFIG10_REG_RX_LT_LOS_MASK 0x00000800
#define COMMON_LW_LANE_CONFIG10_REG_RX_LT_LOS_SHIFT 11
#define COMMON_LW_LANE_CONFIG10_REG_RX_LT_CDR_LOL_MASK 0x00000400
#define COMMON_LW_LANE_CONFIG10_REG_RX_LT_CDR_LOL_SHIFT 10
#define COMMON_LW_LANE_CONFIG10_REG_RX_LT_FRAME_LOCK_TIMEOUT_MASK 0x00000200
#define COMMON_LW_LANE_CONFIG10_REG_RX_LT_FRAME_LOCK_TIMEOUT_SHIFT 9
#define COMMON_LW_LANE_CONFIG10_REG_RX_USER_RECFG_MASK 0x00000100
#define COMMON_LW_LANE_CONFIG10_REG_RX_USER_RECFG_SHIFT 8

#define COMMON_LW_LANE_CONFIG10_REG_RX_AP_LONG_CH_MASK 0x00000080
#define COMMON_LW_LANE_CONFIG10_REG_RX_AP_LONG_CH_SHIFT 7
#define COMMON_LW_LANE_CONFIG10_REG_RX_AP_FAULT_MASK 0x00000040
#define COMMON_LW_LANE_CONFIG10_REG_RX_AP_FAULT_SHIFT 6
#define COMMON_LW_LANE_CONFIG10_REG_RX_PRECDR_PEAKING_SWAP_MASK 0x00000020
#define COMMON_LW_LANE_CONFIG10_REG_RX_PRECDR_PEAKING_SWAP_SHIFT 5

#define COMMON_LW_LANE_CONFIG10_REG_TX_LT_FRAME_LOCK_TIMEOUT_MASK 0x00000010
#define COMMON_LW_LANE_CONFIG10_REG_TX_LT_FRAME_LOCK_TIMEOUT_SHIFT 4
#define COMMON_LW_LANE_CONFIG10_REG_TX_LT_TIMEOUT_MASK 0x00000008
#define COMMON_LW_LANE_CONFIG10_REG_TX_LT_TIMEOUT_SHIFT 3
#define COMMON_LW_LANE_CONFIG10_REG_TX_PPM_CONV_FAIL_MASK 0x00000004
#define COMMON_LW_LANE_CONFIG10_REG_TX_PPM_CONV_FAIL_SHIFT 2
#define COMMON_LW_LANE_CONFIG10_REG_TX_DW_EGRESS_FLT_MASK 0x00000002
#define COMMON_LW_LANE_CONFIG10_REG_TX_DW_EGRESS_FLT_SHIFT 1
#define COMMON_LW_LANE_CONFIG10_REG_TX_USER_RECFG_MASK 0x00000001
#define COMMON_LW_LANE_CONFIG10_REG_TX_USER_RECFG_SHIFT 0


/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIG11_REG  GP16
 ***************************************************************************/


#define COMMON_LW_LANE_CONFIG11_REG_APCHID_SNR_LOW_ES_MASK 0x00008000
#define COMMON_LW_LANE_CONFIG11_REG_APCHID_SNR_LOW_ES_SHIFT 15
#define COMMON_LW_LANE_CONFIG11_REG_APCHID_SNR_LOW_RS_MASK 0x00004000
#define COMMON_LW_LANE_CONFIG11_REG_APCHID_SNR_LOW_RS_SHIFT 14
#define COMMON_LW_LANE_CONFIG11_REG_TR_PPM_UNLOCK_MASK 0x00002000
#define COMMON_LW_LANE_CONFIG11_REG_TR_PPM_UNLOCK_SHIFT 13
#define COMMON_LW_LANE_CONFIG11_REG_TR_SNR_LOW_MASK 0x00001000
#define COMMON_LW_LANE_CONFIG11_REG_TR_SNR_LOW_SHIFT 12
#define COMMON_LW_LANE_CONFIG11_REG_LARGE_LP_TX_AMP_MASK 0x00000800
#define COMMON_LW_LANE_CONFIG11_REG_LARGE_LP_TX_AMP_SHIFT 11


#define COMMON_LW_LANE_CONFIG11_REG_NRZ_SPIN_PF_MASK 0x0000700
#define COMMON_LW_LANE_CONFIG11_REG_NRZ_SPIN_PF_SHIFT 8
/*flt forward*/
#define COMMON_LW_LANE_CONFIG11_REG_RX_FLT_FORWARD_MASK 0x0000f8
#define COMMON_LW_LANE_CONFIG11_REG_RX_FLT_FORWARD_SHIFT 3
#define COMMON_LW_LANE_CONFIG11_REG_TX_FLT_FORWARD_MASK 0x0000007
#define COMMON_LW_LANE_CONFIG11_REG_TX_FLT_FORWARD_SHIFT 0

/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIG12_REG   GP17
 ***************************************************************************/

#define COMMON_LW_LANE_CONFIG12_REG_CFG_LT_PAM_TXFIR_LATER_MASK 0x00008000
#define COMMON_LW_LANE_CONFIG12_REG_CFG_LT_PAM_TXFIR_LATER_SHIFT 15
#define COMMON_LW_LANE_CONFIG12_REG_CFG_LT_AP_CHK_DISABLE_MASK 0x0000400
#define COMMON_LW_LANE_CONFIG12_REG_CFG_LT_AP_CHK_DISABLE_SHIFT 14
#define COMMON_LW_LANE_CONFIG12_REG_CFG_LT_NRZ_AP_SNR_TH_MASK 0x00003000
#define COMMON_LW_LANE_CONFIG12_REG_CFG_LT_NRZ_AP_SNR_TH_SHIFT 12
#define COMMON_LW_LANE_CONFIG12_REG_CFG_LT_NRZ_AP_PF_TH_MASK 0x00000C00
#define COMMON_LW_LANE_CONFIG12_REG_CFG_LT_NRZ_AP_PF_TH_SHIFT 10
#define COMMON_LW_LANE_CONFIG12_REG_CFG_LT_TIMER_MASK 0x000003C0
#define COMMON_LW_LANE_CONFIG12_REG_CFG_LT_TIMER_SHIFT 6

#define COMMON_LW_LANE_CONFIG12_REG_CFG_LT_NRZ_NOSKIP_MTAP_INC_MASK 0x00000020
#define COMMON_LW_LANE_CONFIG12_REG_CFG_LT_NRZ_NOSKIP_MTAP_INC_SHIFT 5

#define COMMON_LW_LANE_CONFIG12_REG_CFG_LT_PRESET_DSTEP_MASK 0x0000001F
#define COMMON_LW_LANE_CONFIG12_REG_CFG_LT_PRESET_DSTEP_SHIFT 0

/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIG13_REG    GP18
 *****************************************************************************/
#define COMMON_LW_LANE_CONFIG13_REG_DISABLE_DAP_INTEG_PPM_FRZ_MASK              0x40000000     /*!< DISABLE_DAP_INTEG_PPM_FRZ */
#define COMMON_LW_LANE_CONFIG13_REG_DISABLE_DAP_INTEG_PPM_FRZ_SHIFT             30

#define COMMON_LW_LANE_CONFIG13_REG_DPAM_WO_AP_PF_JUMP_TRY_TH_MASK              0x38000000     /*!< DPAM_WO_AP_PF_JUMP_TRY_TH */
#define COMMON_LW_LANE_CONFIG13_REG_DPAM_WO_AP_PF_JUMP_TRY_TH_SHIFT             27
#define COMMON_LW_LANE_CONFIG13_REG_DPAM_WO_AP_PF_JUMP_DWELL_TIME_MASK          0x07F80000     /*!< DPAM_WO_AP_PF_JUMP_DWELL_TIME */
#define COMMON_LW_LANE_CONFIG13_REG_DPAM_WO_AP_PF_JUMP_DWELL_TIME_SHIFT         19
#define COMMON_LW_LANE_CONFIG13_REG_DPAM_WO_AP_PF_JUMP_ENABLE_MASK              0x00040000     /*!< DPAM_WO_AP_PF_JUMP_ENABLE */
#define COMMON_LW_LANE_CONFIG13_REG_DPAM_WO_AP_PF_JUMP_ENABLE_SHIFT             18

#define COMMON_LW_LANE_CONFIG13_REG_DISABLE_DAPB_INTEG_PPM_FRZ_MASK             0x00010000     /*!< DISABLE_DAPB_INTEG_PPM_FRZ */
#define COMMON_LW_LANE_CONFIG13_REG_DISABLE_DAPB_INTEG_PPM_FRZ_SHIFT            16
#define COMMON_LW_LANE_CONFIG13_REG_FSM_LOG_START_MASK                          0x00008000
#define COMMON_LW_LANE_CONFIG13_REG_FSM_LOG_START_SHIFT                         15
#define COMMON_LW_LANE_CONFIG13_REG_FSM_LOG_RX_EN_MASK                          0x00004000
#define COMMON_LW_LANE_CONFIG13_REG_FSM_LOG_RX_EN_SHIFT                         14
#define COMMON_LW_LANE_CONFIG13_REG_FSM_LOG_TX_EN_MASK                          0x00002000
#define COMMON_LW_LANE_CONFIG13_REG_FSM_LOG_TX_EN_SHIFT                         13

#define COMMON_LW_LANE_CONFIG13_REG_CFG_DPAM_PF_INIT_SWEEP_EN_MASK              0x00000400
#define COMMON_LW_LANE_CONFIG13_REG_CFG_DPAM_PF_INIT_SWEEP_EN_SHIFT             10
#define COMMON_LW_LANE_CONFIG13_REG_CFG_FORCE_SKEW_ON_MASK                      0x00000200
#define COMMON_LW_LANE_CONFIG13_REG_CFG_FORCE_SKEW_ON_SHIFT                     9

#define COMMON_LW_LANE_CONFIG13_REG_PCB_BP_MODE_MASK                            0x00000100
#define COMMON_LW_LANE_CONFIG13_REG_PCB_BP_MODE_SHIFT                           0x8

#define COMMON_LW_LANE_CONFIG13_REG_LW_TX_SKEWP_MASK                            0x000000E0
#define COMMON_LW_LANE_CONFIG13_REG_LW_TX_SKEWP_SHIFT                           0x5

#define COMMON_LW_LANE_CONFIG13_REG_ENABLE_LOS_ADCDIV_RST_MASK                  0x00000010
#define COMMON_LW_LANE_CONFIG13_REG_ENABLE_LOS_ADCDIV_RST_SHIFT                 0x4

#define COMMON_LW_LANE_CONFIG13_REG_CFG_LT_PAM_AP_ADCCLIP_TH_MASK               0x0000000C
#define COMMON_LW_LANE_CONFIG13_REG_CFG_LT_PAM_AP_ADCCLIP_TH_SHIFT              0x2
#define COMMON_LW_LANE_CONFIG13_REG_CFG_LT_PAM_AP_MIXED_MASK                    0x00000002
#define COMMON_LW_LANE_CONFIG13_REG_CFG_LT_PAM_AP_MIXED_SHIFT                   0x1
#define COMMON_LW_LANE_CONFIG13_REG_CFG_LT_PAM_AP_DISABLE_PGAC_CHK_MASK         0x00000001
#define COMMON_LW_LANE_CONFIG13_REG_CFG_LT_PAM_AP_DISABLE_PGAC_CHK_SHIFT        0x0

/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIG14_REG    GP19
 *****************************************************************************/
#define COMMON_LW_LANE_CONFIG14_REG_LW_CTLE_CHID6_DEC_LEFT_SHIFT_FACTOR_MASK      0x00F00000   /*LW_CTLE_CHID6_DEC_LEFT_SHIFT_FACTOR*/
#define COMMON_LW_LANE_CONFIG14_REG_LW_CTLE_CHID6_DEC_LEFT_SHIFT_FACTOR_SHIFT     20
#define COMMON_LW_LANE_CONFIG14_REG_LW_CTLE_CHID5_DEC_LEFT_SHIFT_FACTOR_MASK      0x000F0000   /*LW_CTLE_CHID5_DEC_LEFT_SHIFT_FACTOR*/
#define COMMON_LW_LANE_CONFIG14_REG_LW_CTLE_CHID5_DEC_LEFT_SHIFT_FACTOR_SHIFT     16

#define COMMON_LW_LANE_CONFIG14_REG_TX_LEVEL_SHIFT_0_MASK    0x0000FF00
#define COMMON_LW_LANE_CONFIG14_REG_TX_LEVEL_SHIFT_0_SHIFT   0x8
#define COMMON_LW_LANE_CONFIG14_REG_TX_LEVEL_SHIFT_1_MASK    0x000000FF
#define COMMON_LW_LANE_CONFIG14_REG_TX_LEVEL_SHIFT_1_SHIFT   0x0

/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIG15_REG    GP20
 *****************************************************************************/
#define COMMON_LW_LANE_CONFIG15_REG_LW_CTLE_CHID5_INC_LEFT_SHIFT_LOCKED_FACTOR_MASK      0xF0000000   /*LW_CTLE_CHID5_INC_LEFT_SHIFT_LOCKED_FACTOR*/
#define COMMON_LW_LANE_CONFIG15_REG_LW_CTLE_CHID5_INC_LEFT_SHIFT_LOCKED_FACTOR_SHIFT     28
#define COMMON_LW_LANE_CONFIG15_REG_LW_CTLE_CHID5_INC_LEFT_SHIFT_UNLOCK_FACTOR_MASK      0x0F000000   /*LW_CTLE_CHID5_INC_LEFT_SHIFT_UNLOCK_FACTOR*/
#define COMMON_LW_LANE_CONFIG15_REG_LW_CTLE_CHID5_INC_LEFT_SHIFT_UNLOCK_FACTOR_SHIFT     24
#define COMMON_LW_LANE_CONFIG15_REG_LW_CTLE_CHID3_INC_COND2_LEFT_SHIFT_FACTOR_MASK       0x00F00000   /*LW_CTLE_CHID4_INC_COND2_LEFT_SHIFT_FACTOR*/
#define COMMON_LW_LANE_CONFIG15_REG_LW_CTLE_CHID3_INC_COND2_LEFT_SHIFT_FACTOR_SHIFT      20
#define COMMON_LW_LANE_CONFIG15_REG_LW_CTLE_CHID3_INC_COND1_LEFT_SHIFT_FACTOR_MASK       0x000F0000   /*LW_CTLE_CHID4_INC_COND1_LEFT_SHIFT_FACTOR*/
#define COMMON_LW_LANE_CONFIG15_REG_LW_CTLE_CHID3_INC_COND1_LEFT_SHIFT_FACTOR_SHIFT      16

#define COMMON_LW_LANE_CONFIG15_REG_TX_LEVEL_SHIFT_2_MASK    0x0000FF00
#define COMMON_LW_LANE_CONFIG15_REG_TX_LEVEL_SHIFT_2_SHIFT   0x8
#define COMMON_LW_LANE_CONFIG15_REG_TX_LEVEL_SHIFT_3_MASK    0x000000FF
#define COMMON_LW_LANE_CONFIG15_REG_TX_LEVEL_SHIFT_3_SHIFT   0x0

/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIG16_REG    GP21
 *****************************************************************************/
#define COMMON_LW_LANE_CONFIG16_REG_DSP_B4_SPHTUNE_SNR_TH_MASK                  0xE0000000   /*!< DSP_B4_SPHTUNE_SNR_TH */
#define COMMON_LW_LANE_CONFIG16_REG_DSP_B4_SPHTUNE_SNR_TH_SHIFT                 29
#define COMMON_LW_LANE_CONFIG16_REG_DSP_DROP_CDR_LOCK_SNR_TH_MASK               0x1C000000   /*!< DSP_DROP_CDR_LOCK_SNR_TH */
#define COMMON_LW_LANE_CONFIG16_REG_DSP_DROP_CDR_LOCK_SNR_TH_SHIFT              26

#define COMMON_LW_LANE_CONFIG16_REG_DPAM_CDR_RESET_REMOVE_ADCDIV_RST_MASK       0x02000000     /*!< DPAM_CDR_RESET_REMOVE_ADCDIV_RST */
#define COMMON_LW_LANE_CONFIG16_REG_DPAM_CDR_RESET_REMOVE_ADCDIV_RST_SHIFT      25
#define COMMON_LW_LANE_CONFIG16_REG_DPAM_CDR_RESET_REMOVE_PHS_FRZ_MASK          0x01000000     /*!< DPAM_CDR_RESET_REMOVE_PHS_FRZ */
#define COMMON_LW_LANE_CONFIG16_REG_DPAM_CDR_RESET_REMOVE_PHS_FRZ_SHIFT         24
//bit 23 idle
#define COMMON_LW_LANE_CONFIG16_REG_LW_HOST_FAKE_AP_CAPI_INF_MASK               0x00400000   /*!< LW_HOST_FAKE_AP_CAPI_INF */
#define COMMON_LW_LANE_CONFIG16_REG_LW_HOST_FAKE_AP_CAPI_INF_SHIFT              22

#define COMMON_LW_LANE_CONFIG16_REG_PHASE_TUNE_STOP_SNR_TH_X10_MASK             0x003F0000   /*!< PHASE_TUNE_STOP_SNR_TH_X10 */
#define COMMON_LW_LANE_CONFIG16_REG_PHASE_TUNE_STOP_SNR_TH_X10_SHIFT            16

#define COMMON_LW_LANE_CONFIG16_REG_GET_FFE_SLICE_HIST_MASK                     0x000008000
#define COMMON_LW_LANE_CONFIG16_REG_GET_FFE_SLICE_HIST_SHIFT                    15
#define COMMON_LW_LANE_CONFIG16_REG_FFE_HISTOGRAM_CAPTURE_EN_MASK               0x000004000
#define COMMON_LW_LANE_CONFIG16_REG_FFE_HISTOGRAM_CAPTURE_EN_SHIFT              14
#define COMMON_LW_LANE_CONFIG16_REG_CAP_MEM_CNT_MASK                            0x00000FFF
#define COMMON_LW_LANE_CONFIG16_REG_CAP_MEM_CNT_SHIFT                           0x0

/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIG17_REG   GP22 FW write
 *****************************************************************************/
#define COMMON_LW_LANE_CONFIG17_REG_LW_TR_START_PPM_SWEEP_ST_RECORD_MASK                0xF0000000
#define COMMON_LW_LANE_CONFIG17_REG_LW_TR_START_PPM_SWEEP_ST_RECORD_SHIFT               28
#define COMMON_LW_LANE_CONFIG17_REG_LW_DPKJUMP_AFT_SAPB_PF_RECORD_MASK                  0x0F800000
#define COMMON_LW_LANE_CONFIG17_REG_LW_DPKJUMP_AFT_SAPB_PF_RECORD_SHIFT                 23

#define COMMON_LW_LANE_CONFIG17_REG_LW_DAP_ADCCLIP_PROBABILITY_RECORD_P1_MASK           0x00400000
#define COMMON_LW_LANE_CONFIG17_REG_LW_DAP_ADCCLIP_PROBABILITY_RECORD_P1_SHIFT          22
#define COMMON_LW_LANE_CONFIG17_REG_LW_DAP_ADCCLIP_PROBABILITY_RECORD_CUR_MASK          0x00200000
#define COMMON_LW_LANE_CONFIG17_REG_LW_DAP_ADCCLIP_PROBABILITY_RECORD_CUR_SHIFT         21
#define COMMON_LW_LANE_CONFIG17_REG_LW_DAP_ADCCLIP_PROBABILITY_RECORD_N1_MASK           0x00100000
#define COMMON_LW_LANE_CONFIG17_REG_LW_DAP_ADCCLIP_PROBABILITY_RECORD_N1_SHIFT          20

#define COMMON_LW_LANE_CONFIG17_REG_LW_DPKJUMP_PF_RECORD_MASK               0x00F8000
#define COMMON_LW_LANE_CONFIG17_REG_LW_DPKJUMP_PF_RECORD_SHIFT              15
#define COMMON_LW_LANE_CONFIG17_REG_LW_SNR_BASED_DAPB_INITIAED_FLAG_MASK    0x0004000
#define COMMON_LW_LANE_CONFIG17_REG_LW_SNR_BASED_DAPB_INITIAED_FLAG_SHIFT   14
#define COMMON_LW_LANE_CONFIG17_REG_LW_SNR_BASED_DAP_INITIAED_FLAG_MASK     0x0002000
#define COMMON_LW_LANE_CONFIG17_REG_LW_SNR_BASED_DAP_INITIAED_FLAG_SHIFT    13

#define COMMON_LW_LANE_CONFIG17_REG_LT_DISABLE_INIT_SETTING_MASK 0x0001000
#define COMMON_LW_LANE_CONFIG17_REG_LT_DISABLE_INIT_SETTING_SHIFT 12

#define COMMON_LW_LANE_CONFIG17_REG_LT_TIMER_MASK 0x0000800
#define COMMON_LW_LANE_CONFIG17_REG_LT_TIMER_SHIFT 11

#define COMMON_LW_LANE_CONFIG17_REG_ACTIVE_PORT_MASK 0x000000400
#define COMMON_LW_LANE_CONFIG17_REG_ACTIVE_PORT_SHIFT 10
#define COMMON_LW_LANE_CONFIG17_REG_ADC_CAL_DONE_MASK 0x000000200
#define COMMON_LW_LANE_CONFIG17_REG_ADC_CAL_DONE_SHIFT 9
#define COMMON_LW_LANE_CONFIG17_REG_PLL_LOCKED_MASK 0x000000100
#define COMMON_LW_LANE_CONFIG17_REG_PLL_LOCKED_SHIFT 8

#define COMMON_LW_LANE_CONFIG17_REG_MODULATION_MASK 0x000000080
#define COMMON_LW_LANE_CONFIG17_REG_MODULATION_SHIFT 7
#define COMMON_LW_LANE_CONFIG17_REG_OSR_MASK 0x000000070
#define COMMON_LW_LANE_CONFIG17_REG_OSR_SHIFT 4
#define COMMON_LW_LANE_CONFIG17_REG_PLL_MULTIPLIER_MASK    0x0000000F
#define COMMON_LW_LANE_CONFIG17_REG_PLL_MULTIPLIER_SHIFT   0x0

/****************************************************************************
 * top_pam_amba :: COMMON_LW_LANE_CONFIG18_REG    GP23
 *****************************************************************************/

#define COMMON_LW_LANE_CONFIG18_REG_VA_PARAM_A_MANUAL_VAL_MASK                  0xFF000000 /*!< VA_PARAM_A_MANUAL_VAL */
#define COMMON_LW_LANE_CONFIG18_REG_VA_PARAM_A_MANUAL_VAL_SHIFT                 24
#define COMMON_LW_LANE_CONFIG18_REG_VA_PARAM_A_MANUAL_EN_MASK                   0x00800000 /*!< VA_PARAM_A_MANUAL_EN */
#define COMMON_LW_LANE_CONFIG18_REG_VA_PARAM_A_MANUAL_EN_SHIFT                  23


#define COMMON_LW_LANE_CONFIG18_REG_ENABLE_AP_DBG_1_MASK                        0x00400000
#define COMMON_LW_LANE_CONFIG18_REG_ENABLE_AP_DBG_1_SHIFT                       22

#define COMMON_LW_LANE_CONFIG18_REG_PEAKING_DTUNE_SNR_CHANGE_TH_MASK            0x003C0000 /*!< PEAKING_DTUNE_SNR_CHANGE_TH */
#define COMMON_LW_LANE_CONFIG18_REG_PEAKING_DTUNE_SNR_CHANGE_TH_SHIFT           18

#define COMMON_LW_LANE_CONFIG18_REG_LW_CHID_AP_PGAC_HIGH_TH_OVRD_EN_MASK        0x00020000 /*!< LW_CHID_AP_PGAC_HIGH_TH_OVRD_EN */
#define COMMON_LW_LANE_CONFIG18_REG_LW_CHID_AP_PGAC_HIGH_TH_OVRD_EN_SHIFT       17
#define COMMON_LW_LANE_CONFIG18_REG_LW_CHID_AP_PGAC_LOW_TH_OVRD_EN_MASK         0x00010000 /*!< LW_CHID_AP_PGAC_LOW_TH_OVRD_EN */
#define COMMON_LW_LANE_CONFIG18_REG_LW_CHID_AP_PGAC_LOW_TH_OVRD_EN_SHIFT        16

#define COMMON_LW_LANE_CONFIG18_REG_TURN_OFF_LEVEL_EQUALIZER_MASK               0x0000C000
#define COMMON_LW_LANE_CONFIG18_REG_TURN_OFF_LEVEL_EQUALIZER_SHIFT              14


#define COMMON_LW_LANE_CONFIG18_REG_LW_CHID_AP_PGAC_HIGH_TH_MASK 0x0003800 /*!< LW_CHID_AP_PGAC_HIGH_TH */
#define COMMON_LW_LANE_CONFIG18_REG_LW_CHID_AP_PGAC_HIGH_TH_SHIFT 11
#define COMMON_LW_LANE_CONFIG18_REG_LW_CHID_AP_PGAC_LOW_TH_MASK 0x00000700 /*!< LW_CHID_AP_PGAC_LOW_TH */
#define COMMON_LW_LANE_CONFIG18_REG_LW_CHID_AP_PGAC_LOW_TH_SHIFT 8

#define COMMON_LW_LANE_CONFIG18_REG_LW_DPAM_ENABLE_PGAC_AP_MASK 0x00000080 /*!< LW_DPAM_ENABLE_PGAC_AP */
#define COMMON_LW_LANE_CONFIG18_REG_LW_DPAM_ENABLE_PGAC_AP_SHIFT 7

#define COMMON_LW_LANE_CONFIG18_REG_RX_BW_VALUE_MASK 0x00000007E /*!< RX_BW_VALUE */
#define COMMON_LW_LANE_CONFIG18_REG_RX_BW_VALUE_SHIFT 1  
#define COMMON_LW_LANE_CONFIG18_REG_RX_BW_VALUE_OVERRIDE_MASK 0x00000001    /*!< RX_BW_VALUE_OVERRIDE */
#define COMMON_LW_LANE_CONFIG18_REG_RX_BW_VALUE_OVERRIDE_SHIFT 0

/*********************************************************************
#define COMMON_LW_LANE_CONFIG19_REG    GP24
 **********************************************************************/
#define COMMON_LW_LANE_CONFIG19_REG_RX_RESTART_PROFILE_MASK 0x0000008000
#define COMMON_LW_LANE_CONFIG19_REG_RX_RESTART_PROFILE_SHIFT 15
#define COMMON_LW_LANE_CONFIG19_REG_RX_RESTART_FSM_MASK 0x0000004000
#define COMMON_LW_LANE_CONFIG19_REG_RX_RESTART_FSM_SHIFT 14
#define COMMON_LW_LANE_CONFIG19_REG_RX_SUSPEND_ST_MASK 0x000003E00
#define COMMON_LW_LANE_CONFIG19_REG_RX_SUSPEND_ST_SHIFT 9
#define COMMON_LW_LANE_CONFIG19_REG_RX_SUSPEND_EN_MASK 0x0000000100
#define COMMON_LW_LANE_CONFIG19_REG_RX_SUSPEND_EN_SHIFT 8

#define COMMON_LW_LANE_CONFIG19_REG_TX_RESTART_PROFILE_MASK 0x0000000080
#define COMMON_LW_LANE_CONFIG19_REG_TX_RESTART_PROFILE_SHIFT 7
#define COMMON_LW_LANE_CONFIG19_REG_TX_RESTART_FSM_MASK 0x0000000040
#define COMMON_LW_LANE_CONFIG19_REG_TX_RESTART_FSM_SHIFT 6
#define COMMON_LW_LANE_CONFIG19_REG_TX_SUSPEND_ST_MASK 0x00000003E
#define COMMON_LW_LANE_CONFIG19_REG_TX_SUSPEND_ST_SHIFT 1
#define COMMON_LW_LANE_CONFIG19_REG_TX_SUSPEND_EN_MASK 0x0000000001
#define COMMON_LW_LANE_CONFIG19_REG_TX_SUSPEND_EN_SHIFT 0


/*********************************************************************
#define COMMON_LW_LANE_CONFIG1A_REG    GP25
 **********************************************************************/
#define COMMON_LW_LANE_CONFIG1A_REG_ENABLE_APB_DYNAMIC_EVENT_LOG_MASK   0x80000000
#define COMMON_LW_LANE_CONFIG1A_REG_ENABLE_APB_DYNAMIC_EVENT_LOG_SHIFT  31

#define COMMON_LW_LANE_CONFIG1A_REG_LW_AP_ADCCLIP_PROBABILITY_TH_CNT_MASK  0x7F800000    
#define COMMON_LW_LANE_CONFIG1A_REG_LW_AP_ADCCLIP_PROBABILITY_TH_CNT_SHIFT 23
#define COMMON_LW_LANE_CONFIG1A_REG_LW_AP_ADCCLIP_CAP_TIMER_MASK        0x007F0000    
#define COMMON_LW_LANE_CONFIG1A_REG_LW_AP_ADCCLIP_CAP_TIMER_SHIFT       16

#define COMMON_LW_LANE_CONFIG1A_REG_OSR_MODE_OVR_VAL_MASK               0x0000C000
#define COMMON_LW_LANE_CONFIG1A_REG_OSR_MODE_OVR_VAL_SHIFT              14
#define COMMON_LW_LANE_CONFIG1A_REG_OSR_MODE_OVR_EN_MASK                0x0002000
#define COMMON_LW_LANE_CONFIG1A_REG_OSR_MODE_OVR_EN_SHIFT               13

#define COMMON_LW_LANE_CONFIG1A_REG_LW_ENABLE_DPAM_INIT_SWEEP_MASK      0x0001000
#define COMMON_LW_LANE_CONFIG1A_REG_LW_ENABLE_DPAM_INIT_SWEEP_SHIFT     12

#define COMMON_LW_LANE_CONFIG1A_REG_LW_DISABLE_DC_WANDER_MASK           0x0000800
#define COMMON_LW_LANE_CONFIG1A_REG_LW_DISABLE_DC_WANDER_SHIFT          11

#define COMMON_LW_LANE_CONFIG1A_REG_LW_EANBLE_LEGACY_NRZ2PAM_MASK       0x0000400
#define COMMON_LW_LANE_CONFIG1A_REG_LW_EANBLE_LEGACY_NRZ2PAM_SHIFT      10
#define COMMON_LW_LANE_CONFIG1A_REG_LW_DISABLE_DPAM_DCO_FREEZE_MASK     0x0000200
#define COMMON_LW_LANE_CONFIG1A_REG_LW_DISABLE_DPAM_DCO_FREEZE_SHIFT    9

#define COMMON_LW_LANE_CONFIG1A_REG_CFG_NRZC_DCOFF_SKEW_CTRL_MASK       0x0000180
#define COMMON_LW_LANE_CONFIG1A_REG_CFG_NRZC_DCOFF_SKEW_CTRL_SHIFT      7

#define COMMON_LW_LANE_CONFIG1A_REG_LW_DISABLE_MISSION_AP_MASK          0x00000040 /*!< LW_DISABLE_MISSION_AP */
#define COMMON_LW_LANE_CONFIG1A_REG_LW_DISABLE_MISSION_AP_SHIFT         6
#define COMMON_LW_LANE_CONFIG1A_REG_LW_DPAM_ESLICE_PROG_FST_MASK        0x0000020 /*!< LW_DPAM_ESLICE_PROG_FST */
#define COMMON_LW_LANE_CONFIG1A_REG_LW_DPAM_ESLICE_PROG_FST_SHIFT       5

#define COMMON_LW_LANE_CONFIG1A_REG_DISABLE_DPAM_CMA_FFE_ADJUST_MASK    0x00000010
#define COMMON_LW_LANE_CONFIG1A_REG_DISABLE_DPAM_CMA_FFE_ADJUST_SHIFT   4

#define COMMON_LW_LANE_CONFIG1A_REG_CFG_DPAM_AP_ADCCLIP_TH_MASK         0x0000000C
#define COMMON_LW_LANE_CONFIG1A_REG_CFG_DPAM_AP_ADCCLIP_TH_SHIFT        0x2
#define COMMON_LW_LANE_CONFIG1A_REG_CFG_DPAM_AP_ADCCLIP_EN_MASK         0x00000002
#define COMMON_LW_LANE_CONFIG1A_REG_CFG_DPAM_AP_ADCCLIP_EN_SHIFT        1

#define COMMON_LW_LANE_CONFIG1A_REG_LANE_RESTART_PROFILE_MASK           0x0000000001
#define COMMON_LW_LANE_CONFIG1A_REG_LANE_RESTART_PROFILE_SHIFT          0


/*********************************************************************
#define COMMON_LW_LANE_CONFIG1B_REG    GP26
 **********************************************************************/
#define COMMON_LW_LANE_CONFIG1B_REG_AP_ENABLE_AFT_SAPB_PK_JUMP_MASK             0x40000000 /*!< AP_ENABLE_AFT_SAPB_PK_JUMP */
#define COMMON_LW_LANE_CONFIG1B_REG_AP_ENABLE_AFT_SAPB_PK_JUMP_SHIFT            30
#define COMMON_LW_LANE_CONFIG1B_REG_AP_DISABLE_ADCCLIP_PROBABILITY_MASK         0x20000000 /*!< AP_DISABLE_ADCCLIP_PROBABILITY */
#define COMMON_LW_LANE_CONFIG1B_REG_AP_DISABLE_ADCCLIP_PROBABILITY_SHIFT        29

#define COMMON_LW_LANE_CONFIG1B_REG_LW_AP_DTUNE_3_STATES_MASK                   0x10000000 /*!< LW_AP_DTUNE_3_STATES */
#define COMMON_LW_LANE_CONFIG1B_REG_LW_AP_DTUNE_3_STATES_SHIFT                  28

#define COMMON_LW_LANE_CONFIG1B_REG_ENABLE_DSP_FFE_DEBUG_DUMP_ALWAYS_MASK       0x08000000   /*!< ENABLE_DSP_FFE_DEBUG_DUMP_ALWAYS */
#define COMMON_LW_LANE_CONFIG1B_REG_ENABLE_DSP_FFE_DEBUG_DUMP_ALWAYS_SHIFT      27
#define COMMON_LW_LANE_CONFIG1B_REG_DISABLE_DSP_DEBUG_DUMP_MASK                 0x04000000   /*!< DISABLE_DSP_DEBUG_DUMP */
#define COMMON_LW_LANE_CONFIG1B_REG_DISABLE_DSP_DEBUG_DUMP_SHIFT                26

#define COMMON_LW_LANE_CONFIG1B_REG_LW_GAIN_BOOST_USER_OVERRIDE_MASK  0x00008000 /*!< LW_GAIN_BOOST_USER_OVERRIDE */
#define COMMON_LW_LANE_CONFIG1B_REG_LW_GAIN_BOOST_USER_OVERRIDE_SHIFT 15
#define COMMON_LW_LANE_CONFIG1B_REG_LW_AP_INIT_VAL_MASK  0x00007C00 /*!< LW_AP_INIT_VAL */
#define COMMON_LW_LANE_CONFIG1B_REG_LW_AP_INIT_VAL_SHIFT 10
#define COMMON_LW_LANE_CONFIG1B_REG_LW_OUTLOS_TH_CFG_MASK 0x000003E0 /*!< LW_OUTLOS_TH_CFG */
#define COMMON_LW_LANE_CONFIG1B_REG_LW_OUTLOS_TH_CFG_SHIFT 5
#define COMMON_LW_LANE_CONFIG1B_REG_LW_INLOS_TH_CFG_MASK 0x0000001F /*!< LW_INLOS_TH_CFG */
#define COMMON_LW_LANE_CONFIG1B_REG_LW_INLOS_TH_CFG_SHIFT 0


/*********************************************************************
#define COMMON_LW_LANE_CONFIG1C_REG    GP27
**********************************************************************/
#define COMMON_LW_LANE_CONFIG1B_REG_PEAKING_TUNE_MIN_SNR_LVL_MASK                   0x00020000   /*!< PEAKING_TUNE_MIN_SNR_LVL */
#define COMMON_LW_LANE_CONFIG1B_REG_PEAKING_TUNE_MIN_SNR_LVL_SHIFT                  17
#define COMMON_LW_LANE_CONFIG1B_REG_PHASE_TUNE_MIN_SNR_LVL_MASK                     0x00010000   /*!< PHASE_TUNE_MIN_SNR_LVL */
#define COMMON_LW_LANE_CONFIG1B_REG_PHASE_TUNE_MIN_SNR_LVL_SHIFT                    16

#define COMMON_LW_LANE_CONFIG1C_REG_LW_DISABLE_MISSION_MPI_DETECT_MASK              0x00008000   /*LW_DISABLE_MISSION_MPI_DETECT*/
#define COMMON_LW_LANE_CONFIG1C_REG_LW_DISABLE_MISSION_MPI_DETECT_SHIFT             15
#define COMMON_LW_LANE_CONFIG1C_REG_LW_DISABLE_FFE_HISTOGRAM_MPI_DETECT_MASK        0x00004000   /*LW_DISABLE_FFE_HISTOGRAM_MPI_DETECT*/
#define COMMON_LW_LANE_CONFIG1C_REG_LW_DISABLE_FFE_HISTOGRAM_MPI_DETECT_SHIFT       14
//idle bit 13
#define COMMON_LW_LANE_CONFIG1C_REG_LW_RESTART_ADCCLIP_PROBABILITY_MASK             0x000001000
#define COMMON_LW_LANE_CONFIG1C_REG_LW_RESTART_ADCCLIP_PROBABILITY_SHIFT            12

#define COMMON_LW_LANE_CONFIG1C_REG_LW_EQ_24TAP_MTAP_6_MASK                         0x000000800
#define COMMON_LW_LANE_CONFIG1C_REG_LW_EQ_24TAP_MTAP_6_SHIFT                        11

#define COMMON_LW_LANE_CONFIG1C_REG_LW_ENABLE_5XPAM_SUBTAP_MASK                     0x000000400
#define COMMON_LW_LANE_CONFIG1C_REG_LW_ENABLE_5XPAM_SUBTAP_SHIFT                    10

#define COMMON_LW_LANE_CONFIG1C_REG_LW_LT_ENABLE_PPTAP_DEC_MASK                     0x000000200
#define COMMON_LW_LANE_CONFIG1C_REG_LW_LT_ENABLE_PPTAP_DEC_SHIFT                    9
#define COMMON_LW_LANE_CONFIG1C_REG_LW_LT_ENABLE_PRESET2_POST1_NO_EQ_MASK           0x000000100
#define COMMON_LW_LANE_CONFIG1C_REG_LW_LT_ENABLE_PRESET2_POST1_NO_EQ_SHIFT          8
#define COMMON_LW_LANE_CONFIG1C_REG_LW_LT_ENABLE_PRESET2_PRE1_NO_EQ_MASK            0x000000080
#define COMMON_LW_LANE_CONFIG1C_REG_LW_LT_ENABLE_PRESET2_PRE1_NO_EQ_SHIFT           7


#define COMMON_LW_LANE_CONFIG1C_REG_CFG_LT_PAM_PRESET_TYPE_MASK                     0x00000007
#define COMMON_LW_LANE_CONFIG1C_REG_CFG_LT_PAM_PRESET_TYPE_SHIFT                    0


/*********************************************************************
#define COMMON_LW_LANE_CONFIG1D_REG    GP28  FW write
 **********************************************************************/
#define COMMON_LW_LANE_CONFIG1D_REG_LW_TX_JITTER_FILTER2_OVRD_VAL_MASK          0x3E000000 /*!< LW_TX_JITTER_FILTER2_OVRD_VAL */
#define COMMON_LW_LANE_CONFIG1D_REG_LW_TX_JITTER_FILTER2_OVRD_VAL_SHIFT         25
#define COMMON_LW_LANE_CONFIG1D_REG_LW_TX_JITTER_FILTER2_OVRD_EN_MASK           0x01000000 /*!< LW_TX_JITTER_FILTER2_OVRD_EN */
#define COMMON_LW_LANE_CONFIG1D_REG_LW_TX_JITTER_FILTER2_OVRD_EN_SHIFT          24
#define COMMON_LW_LANE_CONFIG1D_REG_LW_TX_JITTER_FILTER2_EN_MASK                0x00800000 /*!< LW_TX_JITTER_FILTER2_EN */
#define COMMON_LW_LANE_CONFIG1D_REG_LW_TX_JITTER_FILTER2_EN_SHIFT               23

#define COMMON_LW_LANE_CONFIG1D_REG_LW_TX_JITTER_FILTER_OVRD_VAL_MASK           0x007C0000 /*!< LW_TX_JITTER_FILTER_OVRD_VAL */
#define COMMON_LW_LANE_CONFIG1D_REG_LW_TX_JITTER_FILTER_OVRD_VAL_SHIFT          18
#define COMMON_LW_LANE_CONFIG1D_REG_LW_TX_JITTER_FILTER_OVRD_EN_MASK            0x00020000 /*!< LW_TX_JITTER_FILTER_OVRD_EN */
#define COMMON_LW_LANE_CONFIG1D_REG_LW_TX_JITTER_FILTER_OVRD_EN_SHIFT           17

#define COMMON_LW_LANE_CONFIG1D_REG_TRAFFIC_SWITCH_DETECT_BASED_ADC_DETECTED_STICKY_MASK 0x00010000 /*!< TRAFFIC_SWITCH_DETECT_BASED_ADC_DETECTED_STICKY */
#define COMMON_LW_LANE_CONFIG1D_REG_TRAFFIC_SWITCH_DETECT_BASED_ADC_DETECTED_STICKY_SHIFT 16

#define COMMON_LW_LANE_CONFIG1D_REG_LW_PAM_1XXG_MASK 0x00008000 /*!< LW_PAM_1XXG */
#define COMMON_LW_LANE_CONFIG1D_REG_LW_PAM_1XXG_SHIFT 15
#define COMMON_LW_LANE_CONFIG1D_REG_LW_PAM_LT_MSG_MASK 0x00007000 /*!< LW_PAM_LT_MSG */
#define COMMON_LW_LANE_CONFIG1D_REG_LW_PAM_LT_MSG_SHIFT 12

#define COMMON_LW_LANE_CONFIG1D_REG_LW_PAM_LT_DSTEP_MASK 0x000FC0 /*!< LW_PAM_LT_DSTEP */
#define COMMON_LW_LANE_CONFIG1D_REG_LW_PAM_LT_DSTEP_SHIFT 6

#define COMMON_LW_LANE_CONFIG1D_REG_LW_PAM_LT_PREPK_ST_MASK 0x000000038 /*!< LW_PAM_LT_PREPK_ST */
#define COMMON_LW_LANE_CONFIG1D_REG_LW_PAM_LT_PREPK_ST_SHIFT 3
#define COMMON_LW_LANE_CONFIG1D_REG_LW_PAM_LT_PREPK_FRM_LOCK_MASK 0x00000007 /*!< LW_PAM_LT_PREPK_FRM_LOCK */
#define COMMON_LW_LANE_CONFIG1D_REG_LW_PAM_LT_PREPK_FRM_LOCK_SHIFT 0


/*********************************************************************
#define COMMON_LW_LANE_CONFIG1E_REG    GP29  cAPI config
 **********************************************************************/

#define COMMON_LW_LANE_CONFIG1E_REG_LW_PHTUNE_ADJUSTED_MAX_MASK                 0x3FC00000 /*!< LW_PHTUNE_ADJUSTED_MAX */
#define COMMON_LW_LANE_CONFIG1E_REG_LW_PHTUNE_ADJUSTED_MAX_SHIFT                22
#define COMMON_LW_LANE_CONFIG1E_REG_LW_PHTUNE_ADJUSTED_MIN_MASK                 0x003FC000 /*!< LW_PHTUNE_ADJUSTED_MIN */
#define COMMON_LW_LANE_CONFIG1E_REG_LW_PHTUNE_ADJUSTED_MIN_SHIFT                14

#define COMMON_LW_LANE_CONFIG1E_REG_LW_HW_HISTOGRAM_HIST_MODE_OVRD_VAL_MASK     0x0003000 /*!< HIST_MODE_OVRD_VAL */
#define COMMON_LW_LANE_CONFIG1E_REG_LW_HW_HISTOGRAM_HIST_MODE_OVRD_VAL_SHIFT    12
#define COMMON_LW_LANE_CONFIG1E_REG_LW_HW_HISTOGRAM_HIST_MODE_OVRD_EN_MASK      0x00000800 /*!< HIST_MODE_OVRD_EN */
#define COMMON_LW_LANE_CONFIG1E_REG_LW_HW_HISTOGRAM_HIST_MODE_OVRD_EN_SHIFT     11
#define COMMON_LW_LANE_CONFIG1E_REG_LW_HW_HISTOGRAM_BIN_CAP_BITS_MASK           0x000007C0 /*!< LW_HW_HISTOGRAM_BIN_CAP_BITS */
#define COMMON_LW_LANE_CONFIG1E_REG_LW_HW_HISTOGRAM_BIN_CAP_BITS_SHIFT          6
#define COMMON_LW_LANE_CONFIG1E_REG_LW_HW_HISTOGRAM_TIMER_CAP_BITS_MASK         0x0000003F /*!< LW_HW_HISTOGRAM_TIMER_CAP_BITS */
#define COMMON_LW_LANE_CONFIG1E_REG_LW_HW_HISTOGRAM_TIMER_CAP_BITS_SHIFT        0

/*********************************************************************
#define COMMON_LW_LANE_CONFIG1F_REG    GP30  FW write
 **********************************************************************/
#define COMMON_LW_LANE_CONFIG1F_REG_FW_ENTER_MISSION_SNR_TH_MASK                      0x7FC00000 /*!< FW_ENTER_MISSION_SNR_TH */
#define COMMON_LW_LANE_CONFIG1F_REG_FW_ENTER_MISSION_SNR_TH_SHIFT                     22

#define COMMON_LW_LANE_CONFIG1F_REG_DSP_DBG_DUMP_STAGE_3_FLAG_MASK                    0x00200000 /*!< DSP_DBG_DUMP_STAGE_3_FLAG */
#define COMMON_LW_LANE_CONFIG1F_REG_DSP_DBG_DUMP_STAGE_3_FLAG_SHIFT                   21
#define COMMON_LW_LANE_CONFIG1F_REG_DSP_DBG_DUMP_STAGE_2_FLAG_MASK                    0x00100000 /*!< DSP_DBG_DUMP_STAGE_2_FLAG */
#define COMMON_LW_LANE_CONFIG1F_REG_DSP_DBG_DUMP_STAGE_2_FLAG_SHIFT                   20
#define COMMON_LW_LANE_CONFIG1F_REG_DSP_DBG_DUMP_STAGE_1_FLAG_MASK                    0x00080000 /*!< DSP_DBG_DUMP_STAGE_1_FLAG */
#define COMMON_LW_LANE_CONFIG1F_REG_DSP_DBG_DUMP_STAGE_1_FLAG_SHIFT                   19
#define COMMON_LW_LANE_CONFIG1F_REG_DSP_DBG_DUMP_STAGE_FLAG_MASK                      0x00380000 /*!< DSP_DBG_DUMP_STAGE_FLAG */
#define COMMON_LW_LANE_CONFIG1F_REG_DSP_DBG_DUMP_STAGE_FLAG_SHIFT                     19
#define COMMON_LW_LANE_CONFIG1F_REG_DPAM_WO_AP_PF_BEST_VAL_MASK                       0x0007C000 /*!< DPAM_WO_AP_PF_BEST_VAL */
#define COMMON_LW_LANE_CONFIG1F_REG_DPAM_WO_AP_PF_BEST_VAL_SHIFT                      14

#define COMMON_LW_LANE_CONFIG1F_REG_MPI_HW_HISTOGRAM_RATIO_INDICATOR_STICKY_MASK      0x00002000 /*!< MPI_HW_HISTOGRAM_RATIO_INDICATOR_STICKY */
#define COMMON_LW_LANE_CONFIG1F_REG_MPI_HW_HISTOGRAM_RATIO_INDICATOR_STICKY_SHIFT     13
#define COMMON_LW_LANE_CONFIG1F_REG_MPI_HW_HISTOGRAM_RATIO_INDICATOR_MASK             0x00001000 /*!< MPI_HW_HISTOGRAM_RATIO_INDICATOR */
#define COMMON_LW_LANE_CONFIG1F_REG_MPI_HW_HISTOGRAM_RATIO_INDICATOR_SHIFT            12

#define COMMON_LW_LANE_CONFIG1F_REG_MISSION_MPI_DUST_INDICATOR_STICKY_MASK      0x00000800 /*!< MISSION_MPI_DUST_INDICATOR_STICKY */
#define COMMON_LW_LANE_CONFIG1F_REG_MISSION_MPI_DUST_INDICATOR_STICKY_SHIFT     11
#define COMMON_LW_LANE_CONFIG1F_REG_MISSION_MPI_DUST_INDICATOR_MASK             0x00000400 /*!< MISSION_MPI_DUST_INDICATOR */
#define COMMON_LW_LANE_CONFIG1F_REG_MISSION_MPI_DUST_INDICATOR_SHIFT            10

#define COMMON_LW_LANE_CONFIG1F_REG_CDR_RETRY_CNT_MASK                          0x000000FF /*!< CDR_RETRY_CNT */
#define COMMON_LW_LANE_CONFIG1F_REG_CDR_RETRY_CNT_SHIFT                         0

/*********************************************************************
#define COMMON_LW_LANE_CONFIG20_REG    GP31 cAPI config
 **********************************************************************/
#define COMMON_LW_LANE_CONFIG20_REG_DSP_DAP_INTEG_FRZ_TIMER_MASK                0xF8000000     /*DSP_DAP_INTEG_FRZ_TIMER*/
#define COMMON_LW_LANE_CONFIG20_REG_DSP_DAP_INTEG_FRZ_TIMER_SHIFT               27
#define COMMON_LW_LANE_CONFIG20_REG_DSP_LVL_SNR_TH_MON_EN_MASK                  0x04000000     /*DSP_LVL_SNR_TH_MON_EN*/
#define COMMON_LW_LANE_CONFIG20_REG_DSP_LVL_SNR_TH_MON_EN_SHIFT                 26

#define COMMON_LW_LANE_CONFIG20_REG_MISSION_MPI_BLW_ST_CHK_CNT_MASK             0x03FFC000 /*!< MISSION_MPI_BLW_ST_CHK_CNT */
#define COMMON_LW_LANE_CONFIG20_REG_MISSION_MPI_BLW_ST_CHK_CNT_SHIFT            14
//bit 11, 12,13 idle
#define COMMON_LW_LANE_CONFIG20_REG_LW_5XG_PAM_PK_TUNE_SNR_TH_MASK              0x000007C0 /*!< LW_5XG_PAM_PK_TUNE_SNR_TH */
#define COMMON_LW_LANE_CONFIG20_REG_LW_5XG_PAM_PK_TUNE_SNR_TH_SHIFT             6

#define COMMON_LW_LANE_CONFIG20_REG_ANA_BW_DELAY_AFTER_TRACKING_MASK            0x0000003F /*!< ANA_BW_DELAY_AFTER_TRACKING */
#define COMMON_LW_LANE_CONFIG20_REG_ANA_BW_DELAY_AFTER_TRACKING_SHIFT           0

/*********************************************************************
#define COMMON_LW_LANE_CONFIG21_REG    GP32 cAPI confg
 **********************************************************************/
#define COMMON_LW_LANE_CONFIG21_REG_DSP_LVL_SNR_TH_MON_DIFF_MASK               0xFE000000     /*DSP_LVL_SNR_TH_MON_DIFF*/
#define COMMON_LW_LANE_CONFIG21_REG_DSP_LVL_SNR_TH_MON_DIFF_SHIFT              25

#define COMMON_LW_LANE_CONFIG21_REG_FFEX2_MAINTAP_COEF_TH_MASK                  0x01E00000   /*!< FFEX2_MAINTAP_COEF_TH */
#define COMMON_LW_LANE_CONFIG21_REG_FFEX2_MAINTAP_COEF_TH_SHIFT                 21

#define COMMON_LW_LANE_CONFIG21_REG_PTUNE_STATIC_SNR_TH_VAL_MASK                0x001F0000 /*!< PTUNE_STATIC_SNR_TH_VAL */
#define COMMON_LW_LANE_CONFIG21_REG_PTUNE_STATIC_SNR_TH_VAL_SHIFT               16
#define COMMON_LW_LANE_CONFIG21_REG_PTUNE_STATIC_SNR_TH_OVRD_MASK               0x00008000 /*!< PTUNE_STATIC_SNR_TH_OVRD */
#define COMMON_LW_LANE_CONFIG21_REG_PTUNE_STATIC_SNR_TH_OVRD_SHIFT              15

#define COMMON_LW_LANE_CONFIG21_REG_DSP_DBG_DUMP_SNR_TH_X2_MASK                 0x00007000     /*DSP_DBG_DUMP_PPM_EN*/
#define COMMON_LW_LANE_CONFIG21_REG_DSP_DBG_DUMP_SNR_TH_X2_SHIFT                12

#define COMMON_LW_LANE_CONFIG21_REG_HS_DAC_CUR_OVERRIDE_MASK                    0x00000800 /*!< HS_DAC_CUR_OVERRIDE */
#define COMMON_LW_LANE_CONFIG21_REG_HS_DAC_CUR_OVERRIDE_SHIFT                   11
#define COMMON_LW_LANE_CONFIG21_REG_HS_DAC_CUR_VAL_MASK                         0x00000700 /*!< HS_DAC_CUR_VAL */
#define COMMON_LW_LANE_CONFIG21_REG_HS_DAC_CUR_VAL_SHIFT                        8

#define COMMON_LW_LANE_CONFIG21_REG_LW_DISABLE_HW_MLSE_A_VAL_ADAPT_MASK         0x00000080     /*!< LW_DISABLE_HW_MLSE_A_VAL_ADAPT */
#define COMMON_LW_LANE_CONFIG21_REG_LW_DISABLE_HW_MLSE_A_VAL_ADAPT_SHIFT        7
#define COMMON_LW_LANE_CONFIG21_REG_LW_DISABLE_HW_GAIN2_A_VAL_ADAPT_MASK         0x00000080     /*!< LW_DISABLE_HW_GAIN2_A_VAL_ADAPT */
#define COMMON_LW_LANE_CONFIG21_REG_LW_DISABLE_HW_GAIN2_A_VAL_ADAPT_SHIFT        7

#define COMMON_LW_LANE_CONFIG21_REG_LW_5XG_PAM_OVR_25DB_EN_DAP_MASK              0x00000004 /*!< LW_5XG_PAM_OVR_25DB_EN_DAP */
#define COMMON_LW_LANE_CONFIG21_REG_LW_5XG_PAM_OVR_25DB_EN_DAP_SHIFT             2
#define COMMON_LW_LANE_CONFIG21_REG_LW_5XG_PAM_EN_PK_MINOR_TUNE_AFTER_SAPB_MASK  0x00000002 /*!< LW_5XG_PAM_EN_PK_MINOR_TUNE_AFTER_SAPB */
#define COMMON_LW_LANE_CONFIG21_REG_LW_5XG_PAM_EN_PK_MINOR_TUNE_AFTER_SAPB_SHIFT 1
#define COMMON_LW_LANE_CONFIG21_REG_LW_5XG_PAM_EN_PK_DIRECT_JUMP_MASK           0x00000001 /*!< LW_5XG_PAM_EN_PK_DIRECT_JUMP */
#define COMMON_LW_LANE_CONFIG21_REG_LW_5XG_PAM_EN_PK_DIRECT_JUMP_SHIFT          0

/*********************************************************************
#define COMMON_LW_LANE_CONFIG22_REG    GP33   FW debug
 **********************************************************************/
#define COMMON_LW_LANE_CONFIG22_REG_MIN_GAIN2_A_VAL_RECORD_MASK             0xFF000000 /*!< MAX_GAIN2_A_VAL_RECORD */
#define COMMON_LW_LANE_CONFIG22_REG_MIN_GAIN2_A_VAL_RECORD_SHIFT            24
#define COMMON_LW_LANE_CONFIG22_REG_MAX_GAIN2_A_VAL_RECORD_MASK             0x00FF0000 /*!< MIN_GAIN2_A_VAL_RECORD */
#define COMMON_LW_LANE_CONFIG22_REG_MAX_GAIN2_A_VAL_RECORD_SHIFT            16

#define COMMON_LW_LANE_CONFIG22_REG_PTUNE_STATIC_BIAS_VAL_MASK                  0x0000FF00 /*!< PTUNE_STATIC_BIAS_VAL */
#define COMMON_LW_LANE_CONFIG22_REG_PTUNE_STATIC_BIAS_VAL_SHIFT                 8

#define COMMON_LW_LANE_CONFIG22_REG_DSP_FAIL_LOCK_DUE_LVL_SNR_CNT_MASK          0x00000E0 /*!< DSP_FAIL_LOCK_DUE_LVL_SNR_CNT */
#define COMMON_LW_LANE_CONFIG22_REG_DSP_FAIL_LOCK_DUE_LVL_SNR_CNT_SHIFT         5

#define COMMON_LW_LANE_CONFIG22_REG_LW_SNR_THRESHOLD_MASK                       0x0000001C /*!< LW_SNR_THRESHOLD */
#define COMMON_LW_LANE_CONFIG22_REG_LW_SNR_THRESHOLD_SHIFT                      2

#define COMMON_LW_LANE_CONFIG22_REG_LW_MAX_GAIN2_A_VAL_STICKY_MASK              0x00000002 /*!< LW_MAX_GAIN2_A_VAL_STICKY */
#define COMMON_LW_LANE_CONFIG22_REG_LW_MAX_GAIN2_A_VAL_STICKY_SHIFT             1
#define COMMON_LW_LANE_CONFIG22_REG_LW_MIN_GAIN2_A_VAL_STICKY_MASK              0x00000001 /*!< LW_MIN_GAIN2_A_VAL_STICKY */
#define COMMON_LW_LANE_CONFIG22_REG_LW_MIN_GAIN2_A_VAL_STICKY_SHIFT             0
/*********************************************************************
#define COMMON_LW_LANE_CONFIG23_REG    GP34
 **********************************************************************/
#define COMMON_LW_LANE_CONFIG23_REG_FFE_SLICER_CMIS_MPI_METRIC_RATIO_TH_MASK       0xFFC00000 /*!< FFE_SLICER_CMIS_MPI_METRIC_RATIO_TH */
#define COMMON_LW_LANE_CONFIG23_REG_FFE_SLICER_CMIS_MPI_METRIC_RATIO_TH_SHIFT      22

#define COMMON_LW_LANE_CONFIG23_REG_DSP_DAPB_INTEG_FRZ_TIMER_MASK                   0x001F0000     /*DSP_DAP_INTEG_FRZ_TIMER*/
#define COMMON_LW_LANE_CONFIG23_REG_DSP_DAPB_INTEG_FRZ_TIMER_SHIFT                  16

#define COMMON_LW_LANE_CONFIG23_REG_DISABLE_SPTUNE_FFE_RELOCK_MASK       0x00008000 /*!< DISABLE_SPTUNE_FFE_RELOCK */
#define COMMON_LW_LANE_CONFIG23_REG_DISABLE_SPTUNE_FFE_RELOCK_SHIFT      15
#define COMMON_LW_LANE_CONFIG23_REG_DISABLE_SPTUNE_PPM_FORCE_MASK       0x00004000 /*!< DISABLE_SPTUNE_PPM_FORCE */
#define COMMON_LW_LANE_CONFIG23_REG_DISABLE_SPTUNE_PPM_FORCE_SHIFT      14

#define COMMON_LW_LANE_CONFIG23_REG_PHASE_ERR_OVRD_MASK       0x00003FFF /*!< PHASE_ERR_OVRD */
#define COMMON_LW_LANE_CONFIG23_REG_PHASE_ERR_OVRD_SHIFT      0


/*********************************************************************
#define COMMON_LW_LANE_CONFIG24_REG    GP35  FW debug
 **********************************************************************/
#define COMMON_LW_LANE_CONFIG24_REG_DPAM_ELOS_LOL_STICKY_CNT_MASK                       0xF0000000 /*!<  DPAM_ELOS_LOL_STICKY_CNT */
#define COMMON_LW_LANE_CONFIG24_REG_DPAM_ELOS_LOL_STICKY_CNT_SHIFT                      28

#define COMMON_LW_LANE_CONFIG24_REG_AP_END_CDR_RESET_FLAG_MASK                          0x08000000 /*!< AP_END_CDR_RESET_FLAG */
#define COMMON_LW_LANE_CONFIG24_REG_AP_END_CDR_RESET_FLAG_SHIFT                         27 

#define COMMON_LW_LANE_CONFIG24_REG_TRAFFIC_SWITCH_DETECT_ADCC_RECORD_MASK              0x07FE0000 /*!< TRAFFIC_SWITCH_DETECT_ADCC_RECORD */
#define COMMON_LW_LANE_CONFIG24_REG_TRAFFIC_SWITCH_DETECT_ADCC_RECORD_SHIFT             17 
#define COMMON_LW_LANE_CONFIG24_REG_TRAFFIC_SWITCH_DETECT_ADCC_TIMING_MS_MASK           0x0001FE00 /*!< TRAFFIC_SWITCH_DETECT_ADCC_TIMING_MS */
#define COMMON_LW_LANE_CONFIG24_REG_TRAFFIC_SWITCH_DETECT_ADCC_TIMING_MS_SHIFT          9    
#define COMMON_LW_LANE_CONFIG24_REG_TRAFFIC_SWITCH_DETECT_BASED_ADC_DETECTED_MASK       0x00000010 /*!< TRAFFIC_SWITCH_DETECT_BASED_ADC_DETECTED */
#define COMMON_LW_LANE_CONFIG24_REG_TRAFFIC_SWITCH_DETECT_BASED_ADC_DETECTED_SHIFT      4

#define COMMON_LW_LANE_CONFIG24_REG_FAIL_FFE_SHAPE_MASK             0x00000004 /*!< FAIL_FFE_SHAPE */
#define COMMON_LW_LANE_CONFIG24_REG_FAIL_FFE_SHAPE_SHIFT            2
#define COMMON_LW_LANE_CONFIG24_REG_FAIL_PPM_CHK_MASK               0x00000002 /*!< FAIL_PPM_CHK */
#define COMMON_LW_LANE_CONFIG24_REG_FAIL_PPM_CHK_SHIFT              1
#define COMMON_LW_LANE_CONFIG24_REG_FAIL_SNR_TH_MASK                0x00000001 /*!< FAIL_SNR_TH */
#define COMMON_LW_LANE_CONFIG24_REG_FAIL_SNR_TH_SHIFT               0


/*********************************************************************
#define COMMON_LW_LANE_CONFIG25_REG    GP36  FW debug
 **********************************************************************/
#define COMMON_LW_LANE_CONFIG25_REG_DPAM_WO_AP_PF_TUNE_SWP_MAX_MASK                 0x0001F000     /*!<  DPAM_WO_AP_PF_TUNE_SWP_MAX */
#define COMMON_LW_LANE_CONFIG25_REG_DPAM_WO_AP_PF_TUNE_SWP_MAX_SHIFT                12
#define COMMON_LW_LANE_CONFIG25_REG_DPAM_WO_AP_PF_TUNE_SWP_MIN_MASK                 0x00000F80     /*!<  DPAM_WO_AP_PF_TUNE_SWP_MIN */
#define COMMON_LW_LANE_CONFIG25_REG_DPAM_WO_AP_PF_TUNE_SWP_MIN_SHIFT                7
#define COMMON_LW_LANE_CONFIG25_REG_DPAM_WO_AP_PF_TUNE_DWELL_TIME_MASK              0x0000007E     /*!< DPAM_WO_AP_PF_TUNE_DWELL_TIME */
#define COMMON_LW_LANE_CONFIG25_REG_DPAM_WO_AP_PF_TUNE_DWELL_TIME_SHIFT             1
#define COMMON_LW_LANE_CONFIG25_REG_DPAM_WO_AP_PF_TUNE_ENABLE_MASK                  0x00000001     /*!< DPAM_WO_AP_PF_TUNE_ENABLE */
#define COMMON_LW_LANE_CONFIG25_REG_DPAM_WO_AP_PF_TUNE_ENABLE_SHIFT                 0

/*********************************************************************
#define COMMON_LW_LANE_CONFIG26_REG    GP37  FW debug
 **********************************************************************/
#define COMMON_LW_LANE_CONFIG26_REG_PTUNE_ORG_STATIC_BIAS_VAL_MASK                  0xFF000000 /*!< PTUNE_STATIC_BIAS_VAL */
#define COMMON_LW_LANE_CONFIG26_REG_PTUNE_ORG_STATIC_BIAS_VAL_SHIFT                 24

#define COMMON_LW_LANE_CONFIG26_REG_RECORD_HOST_ANA_RX_AMP_TOL_PGAC_HGH_2_MASK      0x00FF0000 /*!< RECORD_HOST_ANA_RX_AMP_TOL_PGAC_HGH_2 */
#define COMMON_LW_LANE_CONFIG26_REG_RECORD_HOST_ANA_RX_AMP_TOL_PGAC_HGH_2_SHIFT     16
#define COMMON_LW_LANE_CONFIG26_REG_RECORD_HOST_ANA_RX_AMP_TOL_PGAC_LOW_2_MASK      0x0000FF00 /*!< RECORD_HOST_ANA_RX_AMP_TOL_PGAC_LOW_2 */
#define COMMON_LW_LANE_CONFIG26_REG_RECORD_HOST_ANA_RX_AMP_TOL_PGAC_LOW_2_SHIFT     8
#define COMMON_LW_LANE_CONFIG26_REG_MISSION_MPI_DETECT_AVERAGE_CNT_MASK             0x000000FF /*!< MISSION_MPI_DETECT_AVERAGE_CNT */
#define COMMON_LW_LANE_CONFIG26_REG_MISSION_MPI_DETECT_AVERAGE_CNT_SHIFT            0


/*********************************************************************
#define COMMON_LW_LANE_CONFIG27_REG    GP38  cAPI setting
 **********************************************************************/
#define COMMON_LW_LANE_CONFIG27_REG_PTUNE_STATIC_REPICK_SNR_TH_MASK                   0x3F000000 /*!< PTUNE_STATIC_REPICK_SNR_TH */
#define COMMON_LW_LANE_CONFIG27_REG_PTUNE_STATIC_REPICK_SNR_TH_SHIFT                  24

#define COMMON_LW_LANE_CONFIG27_REG_MAX_GAIN2_A_VAL_TH_MASK                          0x00FF0000 /*!< MAX_GAIN2_A_VAL_TH */
#define COMMON_LW_LANE_CONFIG27_REG_MAX_GAIN2_A_VAL_TH_SHIFT                         16
#define COMMON_LW_LANE_CONFIG27_REG_MIN_GAIN2_A_VAL_TH_MASK                          0x0000FF00 /*!< MIN_GAIN2_A_VAL_TH */
#define COMMON_LW_LANE_CONFIG27_REG_MIN_GAIN2_A_VAL_TH_SHIFT                         8

/*MPI detect sum delta threshold value signed*/
#define COMMON_LW_LANE_CONFIG27_REG_MISSION_MPI_DETECT_SUM_TH_MASK                  0x00000003F /*!< MISSION_MPI_DETECT_SUM_TH */
#define COMMON_LW_LANE_CONFIG27_REG_MISSION_MPI_DETECT_SUM_TH_SHIFT                 0

/*********************************************************************
#define COMMON_LW_LANE_CONFIG28_REG    GP39  cAPI setting
 **********************************************************************/
#define COMMON_LW_LANE_CONFIG28_REG_DSP_LOS_CHK_IGNORE_TIMER_MASK                   0xF8000000 /*!< DSP_LOS_CHK_IGNORE_TIMER */
#define COMMON_LW_LANE_CONFIG28_REG_DSP_LOS_CHK_IGNORE_TIMER_SHIFT                  27

#define COMMON_LW_LANE_CONFIG28_REG_DISABLE_ADCCLIP_CNT_CHK_MASK                    0x04000000 /*!< DISABLE_ADCCLIP_CNT_CHK */
#define COMMON_LW_LANE_CONFIG28_REG_DISABLE_ADCCLIP_CNT_CHK_SHIFT                   26

#define COMMON_LW_LANE_CONFIG28_REG_CONFIGED_NONLINEAR_13TAP_MASK                   0x02000000 /*!< CONFIGED_NONLINEAR_13TAP */
#define COMMON_LW_LANE_CONFIG28_REG_CONFIGED_NONLINEAR_13TAP_SHIFT                  25
#define COMMON_LW_LANE_CONFIG28_REG_DFFE_SLICER_TH_DISABLE_FFESTH_UPDATE_MASK       0x01000000/*!< DFFE_SLICER_TH_DISABLE_FFESTH_UPDATE */
#define COMMON_LW_LANE_CONFIG28_REG_DFFE_SLICER_TH_DISABLE_FFESTH_UPDATE_SHIFT      24
#define COMMON_LW_LANE_CONFIG28_REG_DFFE_SLICER_TH_ADJUST_FSTH_SETTLE_TIMER_MASK    0x00FFF000 /*!< DFFE_SLICER_TH_ADJUST_FSTH_SETTLE_TIMER */
#define COMMON_LW_LANE_CONFIG28_REG_DFFE_SLICER_TH_ADJUST_FSTH_SETTLE_TIMER_SHIFT   12
#define COMMON_LW_LANE_CONFIG28_REG_DFFE_SLICER_TH_ADJUST_BASED_ON_ADC_MASK         0x00000800 /*!< DFFE_SLICER_TH_ADJUST_BASED_ON_ADC */
#define COMMON_LW_LANE_CONFIG28_REG_DFFE_SLICER_TH_ADJUST_BASED_ON_ADC_SHIFT        11

#define COMMON_LW_LANE_CONFIG28_REG_FFE_SLICER_TH_MPI_INTERVAL_TIMER_MASK           0x000000F0 /*!< FFE_SLICER_TH_MPI_INTERVAL_TIMER */
#define COMMON_LW_LANE_CONFIG28_REG_FFE_SLICER_TH_MPI_INTERVAL_TIMER_SHIFT          4


#define COMMON_LW_LANE_CONFIG28_REG_DSIABLE_DFFE_STH_DUE_HW_HISTORGRAM_MASK         0x00000008 /*!< DSIABLE_DFFE_STH_DUE_HW_HISTORGRAM */
#define COMMON_LW_LANE_CONFIG28_REG_DSIABLE_DFFE_STH_DUE_HW_HISTORGRAM_SHIFT        3
#define COMMON_LW_LANE_CONFIG28_REG_RESTART_DFFE_STH_DUE_HIST_CAPI_MASK             0x00000004 /*!< RESTART_DFFE_STH_DUE_HIST_CAPI */
#define COMMON_LW_LANE_CONFIG28_REG_RESTART_DFFE_STH_DUE_HIST_CAPI_SHIFT            2
#define COMMON_LW_LANE_CONFIG28_REG_DSIABLE_DFFE_STH_DUE_CMIS_SNR_MASK              0x00000002 /*!< DSIABLE_DFFE_STH_DUE_CMIS_SNR */
#define COMMON_LW_LANE_CONFIG28_REG_DSIABLE_DFFE_STH_DUE_CMIS_SNR_SHIFT             1
#define COMMON_LW_LANE_CONFIG28_REG_DISABLE_ADJUST_DYNAMIC_FFE_SLICER_TH_MASK       0x00000001 /*!< DISABLE_ADJUST_DYNAMIC_FFE_SLICER_TH */
#define COMMON_LW_LANE_CONFIG28_REG_DISABLE_ADJUST_DYNAMIC_FFE_SLICER_TH_SHIFT      0

/*********************************************************************
#define COMMON_LW_LANE_CONFIG29_REG    GP40  FW write
 **********************************************************************/
#define COMMON_LW_LANE_CONFIG29_REG_DYNAMIC_FFE_SLICER_TH_P_LOCATION_3_MASK       0xFF000000 /*!< DYNAMIC_FFE_SLICER_TH_P_LOCATION_3 */
#define COMMON_LW_LANE_CONFIG29_REG_DYNAMIC_FFE_SLICER_TH_P_LOCATION_3_SHIFT      24
#define COMMON_LW_LANE_CONFIG29_REG_DYNAMIC_FFE_SLICER_TH_P_LOCATION_2_MASK       0x00FF0000 /*!< DYNAMIC_FFE_SLICER_TH_P_LOCATION_2 */
#define COMMON_LW_LANE_CONFIG29_REG_DYNAMIC_FFE_SLICER_TH_P_LOCATION_2_SHIFT      16
#define COMMON_LW_LANE_CONFIG29_REG_DYNAMIC_FFE_SLICER_TH_P_LOCATION_1_MASK       0x0000FF00 /*!< DYNAMIC_FFE_SLICER_TH_P_LOCATION_1 */
#define COMMON_LW_LANE_CONFIG29_REG_DYNAMIC_FFE_SLICER_TH_P_LOCATION_1_SHIFT      8
#define COMMON_LW_LANE_CONFIG29_REG_DYNAMIC_FFE_SLICER_TH_P_LOCATION_0_MASK       0x000000FF /*!< DYNAMIC_FFE_SLICER_TH_P_LOCATION_0 */
#define COMMON_LW_LANE_CONFIG29_REG_DYNAMIC_FFE_SLICER_TH_P_LOCATION_0_SHIFT      0


/*********************************************************************
#define COMMON_LW_LANE_CONFIG2A_REG    GP41  FW write
 **********************************************************************/
#define COMMON_LW_LANE_CONFIG2A_REG_RECORD_HOST_ANA_RX_AMP_TOL_PGAC_LOW_1_MASK    0xFF000000 /*!< RECORD_HOST_ANA_RX_AMP_TOL_PGAC_LOW_1 */
#define COMMON_LW_LANE_CONFIG2A_REG_RECORD_HOST_ANA_RX_AMP_TOL_PGAC_LOW_1_SHIFT   24
#define COMMON_LW_LANE_CONFIG2A_REG_DYNAMIC_FFE_SLICER_TH_V_LOCATION_2_MASK       0x00FF0000 /*!< DYNAMIC_FFE_SLICER_TH_V_LOCATION_2 */
#define COMMON_LW_LANE_CONFIG2A_REG_DYNAMIC_FFE_SLICER_TH_V_LOCATION_2_SHIFT      16
#define COMMON_LW_LANE_CONFIG2A_REG_DYNAMIC_FFE_SLICER_TH_V_LOCATION_1_MASK       0x0000FF00 /*!< DYNAMIC_FFE_SLICER_TH_V_LOCATION_1 */
#define COMMON_LW_LANE_CONFIG2A_REG_DYNAMIC_FFE_SLICER_TH_V_LOCATION_1_SHIFT      8
#define COMMON_LW_LANE_CONFIG2A_REG_DYNAMIC_FFE_SLICER_TH_V_LOCATION_0_MASK       0x000000FF /*!< DYNAMIC_FFE_SLICER_TH_V_LOCATION_0 */
#define COMMON_LW_LANE_CONFIG2A_REG_DYNAMIC_FFE_SLICER_TH_V_LOCATION_0_SHIFT      0

/*********************************************************************
#define COMMON_LW_LANE_CONFIG2B_REG    GP42  FW Recode
 **********************************************************************/
//line side
#define COMMON_LW_LANE_CONFIG2B_REG_FFE_SLICER_CMIS_MPI_METRIC_MASK           0xFFFFFFFF /*!< FFE_SLICER_CMIS_MPI_METRIC */
#define COMMON_LW_LANE_CONFIG2B_REG_FFE_SLICER_CMIS_MPI_METRIC_SHIFT          0
//host side
#define COMMON_LW_LANE_CONFIG2B_REG_LW_DAP_ADCCLIP_CNT_MASK                   0xFFFFFFFF /*!< LW_DAP_ADCCLIP_CNT */
#define COMMON_LW_LANE_CONFIG2B_REG_LW_DAP_ADCCLIP_CNT_SHIFT                  0
/*********************************************************************
#define COMMON_LW_LANE_CONFIG2C_REG    GP43  FW debugging
 **********************************************************************/
#define COMMON_LW_LANE_CONFIG2C_REG_LW_FORCE_LANE_POWER_RESET_MASK                     0x08000000     /*!< LW_FORCE_LANE_POWER_RESET */
#define COMMON_LW_LANE_CONFIG2C_REG_LW_FORCE_LANE_POWER_RESET_SHIFT                    27
#define COMMON_LW_LANE_CONFIG2C_REG_LW_FORCE_ADC_CAL_RESTART_MASK                      0x04000000     /*!< LW_FORCE_ADC_CAL_RESTART */
#define COMMON_LW_LANE_CONFIG2C_REG_LW_FORCE_ADC_CAL_RESTART_SHIFT                     26

#define COMMON_LW_LANE_CONFIG2C_REG_TRAFFIC_SWITCH_DETECT_THRESHOLD_MASK               0x03FE0000 /*!< TRAFFIC_SWITCH_DETECT_THRESHOLD */
#define COMMON_LW_LANE_CONFIG2C_REG_TRAFFIC_SWITCH_DETECT_THRESHOLD_SHIFT              17
#define COMMON_LW_LANE_CONFIG2C_REG_TRAFFIC_SWITCH_DETECT_100MS_INTERVAL_MASK          0x0001FE00 /*!< TRAFFIC_SWITCH_DETECT_100MS_INTERVAL */
#define COMMON_LW_LANE_CONFIG2C_REG_TRAFFIC_SWITCH_DETECT_100MS_INTERVAL_SHIFT         9
#define COMMON_LW_LANE_CONFIG2C_REG_TRAFFIC_SWITCH_DETECT_CHK_CNT_MASK                 0x000001F0 /*!< TRAFFIC_SWITCH_DETECT_CHK_CNT */
#define COMMON_LW_LANE_CONFIG2C_REG_TRAFFIC_SWITCH_DETECT_CHK_CNT_SHIFT                4

#define COMMON_LW_LANE_CONFIG2C_REG_RESTART_TRAFFIC_SWITCH_DETECT_DUE_CAPI_ADC_MASK    0x00000004 /*!< RESTART_TRAFFIC_SWITCH_DETECT_DUE_CAPI_ADC */
#define COMMON_LW_LANE_CONFIG2C_REG_RESTART_TRAFFIC_SWITCH_DETECT_DUE_CAPI_ADC_SHIFT   2
#define COMMON_LW_LANE_CONFIG2C_REG_DISABLE_TRAFFIC_SWITCH_DETECT_DUE_CAPI_ADC_MASK    0x00000002 /*!< DISABLE_TRAFFIC_SWITCH_DETECT_DUE_CAPI_ADC */
#define COMMON_LW_LANE_CONFIG2C_REG_DISABLE_TRAFFIC_SWITCH_DETECT_DUE_CAPI_ADC_SHIFT   1
#define COMMON_LW_LANE_CONFIG2C_REG_DISABLE_TRAFFIC_SWITCH_DETECT_BASED_ADC_MASK       0x00000001 /*!< DISABLE_TRAFFIC_SWITCH_DETECT_BASED_ADC */
#define COMMON_LW_LANE_CONFIG2C_REG_DISABLE_TRAFFIC_SWITCH_DETECT_BASED_ADC_SHIFT      0


/*********************************************************************
#define COMMON_LW_LANE_CONFIG2D_REG    GP44
 **********************************************************************/

#define COMMON_LW_LANE_CONFIG2D_REG_CFG_HOST_ANA_RX_AMP_TOL_PGAC_HGH_TH_MASK            0x001F0000 /*!< CFG_HOST_ANA_RX_AMP_TOL_PGAC_HGH_TH */
#define COMMON_LW_LANE_CONFIG2D_REG_CFG_HOST_ANA_RX_AMP_TOL_PGAC_HGH_TH_SHIFT           16
#define COMMON_LW_LANE_CONFIG2D_REG_DIS_HOST_ANA_RX_AMP_TOL_PGAC_HGH_CHK_MASK           0x00008000 /*!< DIS_HOST_ANA_RX_AMP_TOL_PGAC_HGH_CHK */
#define COMMON_LW_LANE_CONFIG2D_REG_DIS_HOST_ANA_RX_AMP_TOL_PGAC_HGH_CHK_SHIFT          15

#define COMMON_LW_LANE_CONFIG2D_REG_DIS_HOST_ANA_RX_AMP_TOL_PGAC_LOW_CHK_MASK           0x00004000 /*!< DIS_HOST_ANA_RX_AMP_TOL_PGAC_LOW_CHK */
#define COMMON_LW_LANE_CONFIG2D_REG_DIS_HOST_ANA_RX_AMP_TOL_PGAC_LOW_CHK_SHIFT          14
#define COMMON_LW_LANE_CONFIG2D_REG_CFG_HOST_ANA_RX_AMP_TOL_PGAC_LOW_TH_MASK            0x00003C00 /*!< CFG_HOST_ANA_RX_AMP_TOL_PGAC_LOW_TH */
#define COMMON_LW_LANE_CONFIG2D_REG_CFG_HOST_ANA_RX_AMP_TOL_PGAC_LOW_TH_SHIFT           10

#define COMMON_LW_LANE_CONFIG2D_REG_EN_HOST_5XG_ANA_SPEC_TEST_MASK                      0x00000200 /*!< EN_HOST_5XG_ANA_SPEC_TEST */
#define COMMON_LW_LANE_CONFIG2D_REG_EN_HOST_5XG_ANA_SPEC_TEST_SHIFT                     9

#define COMMON_LW_LANE_CONFIG2D_REG_HOST_SKIP_ANA_AFE_PBIAS100U_SET_MASK                0x00000100 /*!< HOST_SKIP_ANA_AFE_PBIAS100U_SET */
#define COMMON_LW_LANE_CONFIG2D_REG_HOST_SKIP_ANA_AFE_PBIAS100U_SET_SHIFT               8
#define COMMON_LW_LANE_CONFIG2D_REG_HOST_SKIP_ANA_GM_BIAS_SET_MASK                      0x00000080 /*!< HOST_SKIP_ANA_GM_BIAS_SET */
#define COMMON_LW_LANE_CONFIG2D_REG_HOST_SKIP_ANA_GM_BIAS_SET_SHIFT                     7
#define COMMON_LW_LANE_CONFIG2D_REG_HOST_INIT_ANA_BW_VALUE_MASK                         0x0000007E /*!< HOST_INIT_ANA_BW_VALUE */
#define COMMON_LW_LANE_CONFIG2D_REG_HOST_INIT_ANA_BW_VALUE_SHIFT                        1
#define COMMON_LW_LANE_CONFIG2D_REG_HOST_INIT_ANA_BW_VALUE_EN_MASK                      0x00000001 /*!< HOST_INIT_ANA_BW_VALUE_EN */
#define COMMON_LW_LANE_CONFIG2D_REG_HOST_INIT_ANA_BW_VALUE_EN_SHIFT                     0
/*********************************************************************
#define COMMON_LW_LANE_CONFIG2E_REG    GP45
 **********************************************************************/
#define COMMON_LW_LANE_CONFIG2E_REG_FFE_SLICER_CMIS_SIGMA_1_MASK           0xFFFF0000 /*!< FFE_SLICER_CMIS_SIGMA_1 */
#define COMMON_LW_LANE_CONFIG2E_REG_FFE_SLICER_CMIS_SIGMA_1_SHIFT          16
#define COMMON_LW_LANE_CONFIG2E_REG_FFE_SLICER_CMIS_SIGMA_0_MASK           0x0000FFFF /*!< FFE_SLICER_CMIS_SIGMA_0 */
#define COMMON_LW_LANE_CONFIG2E_REG_FFE_SLICER_CMIS_SIGMA_0_SHIFT          0

/*********************************************************************
#define COMMON_LW_LANE_CONFIG2F_REG    GP46
 **********************************************************************/
#define COMMON_LW_LANE_CONFIG2F_REG_FFE_SLICER_CMIS_SIGMA_3_MASK           0xFFFF0000 /*!< FFE_SLICER_CMIS_SIGMA_3 */
#define COMMON_LW_LANE_CONFIG2F_REG_FFE_SLICER_CMIS_SIGMA_3_SHIFT          16
#define COMMON_LW_LANE_CONFIG2F_REG_FFE_SLICER_CMIS_SIGMA_2_MASK           0x0000FFFF /*!< FFE_SLICER_CMIS_SIGMA_2 */
#define COMMON_LW_LANE_CONFIG2F_REG_FFE_SLICER_CMIS_SIGMA_2_SHIFT          0
 
/*********************************************************************
#define COMMON_LW_LANE_CONFIG30_REG    GP47
 **********************************************************************/
#define COMMON_LW_LANE_CONFIG30_REG_MAX_SNR_RECORD_MASK                     0xFFFF0000 /*!< MIN_SNR_RECORD */
#define COMMON_LW_LANE_CONFIG30_REG_MAX_SNR_RECORD_SHIFT                    16
#define COMMON_LW_LANE_CONFIG30_REG_MIN_SNR_RECORD_MASK                     0x0000FFFF /*!< MIN_SNR_RECORD */
#define COMMON_LW_LANE_CONFIG30_REG_MIN_SNR_RECORD_SHIFT                    0

/****************************************************************************
#define HOST_DSP_CONFIG_REG                                   QUAD_CORE_CONFIG_CFGVAL_79_RDB
 ***************************************************************************/
#define HOST_DSP_CONFIG_REG_SPHASE_TUNE_PRECHECK_SNR_TH_MASK        0x0000003E /*!< SPHASE_TUNE_PRECHECK_SNR_TH*/
#define HOST_DSP_CONFIG_REG_SPHASE_TUNE_PRECHECK_SNR_TH_SHIFT       1
#define HOST_DSP_CONFIG_REG_ENABLE_SPHASE_TUNE_PRECHECK_MASK        0x000000001 /*!< ENABLE_SPHASE_TUNE_PRECHECK*/
#define HOST_DSP_CONFIG_REG_ENABLE_SPHASE_TUNE_PRECHECK_SHIFT       0

/****************************************************************************
 * top_pam_amba :: COMMON_LW_CONFIG1_REG 
 * #define HOST_DSP_CONFIG1_REG               QUAD_CORE_CONFIG_CFGVAL_80_RDB
 * #define MEDIA_DSP_CONFIG1_REG              QUAD_CORE_CONFIG_CFGVAL_81_RDB
 ***************************************************************************/
#define COMMON_LW_CONFIG1_REG_FORCE_PLL_RECFG_MASK                  0x00008000 /*!< FORCE_PLL_RECFG*/
#define COMMON_LW_CONFIG1_REG_FORCE_PLL_RECFG_SHIFT                 15
#define COMMON_LW_CONFIG1_REG_SUSPEND_DSP_TASK_MASK                 0x00004000 /*!< SUSPEND_DSP_TASK*/
#define COMMON_LW_CONFIG1_REG_SUSPEND_DSP_TASK_SHIFT                14

#define COMMON_LW_CONFIG1_REG_CFG_INTEG_PLLX2_BW_MASK               0x00003000 /*!< CFG_INTEG_PLLX2_BW*/
#define COMMON_LW_CONFIG1_REG_CFG_INTEG_PLLX2_BW_SHIFT              12

#define COMMON_LW_CONFIG1_REG_ENABLE_PPM_CHK_WITH_AP_DONE_MASK      0x00000800 /*!< ENABLE_PPM_CHK_WITH_AP_DONE*/
#define COMMON_LW_CONFIG1_REG_ENABLE_PPM_CHK_WITH_AP_DONE_SHIFT     11

#define COMMON_LW_CONFIG1_REG_CFG_REG_DIVIDER_RATIO_MASK            0x00000600 /*!< CFG_REG_DIVIDER_RATIO*/
#define COMMON_LW_CONFIG1_REG_CFG_REG_DIVIDER_RATIO_SHIFT           9


#define COMMON_LW_CONFIG1_REG_LW_PLL_REFCLK_X2_ENABLE_MASK          0x00000020 /*!< LW_PLL_REFCLK_X2_ENABLE */
#define COMMON_LW_CONFIG1_REG_LW_PLL_REFCLK_X2_ENABLE_SHIFT         5 

#define COMMON_LW_CONFIG1_REG_LW_LMS_MODE_MASK                      0x00000010 /*!< LW_LMS_MODE*/
#define COMMON_LW_CONFIG1_REG_LW_LMS_MODE_SHIFT                     4 
#define COMMON_LW_CONFIG1_REG_LW_ENABLE_PRBS_MASK                   0x00000008 /*!< LW_ENABLE_PRBS*/
#define COMMON_LW_CONFIG1_REG_LW_ENABLE_PRBS_SHIFT                  3
#define COMMON_LW_CONFIG1_REG_LW_DELAY_START_MASK                   0x00000004 /*!< LW_DELAY_START*/
#define COMMON_LW_CONFIG1_REG_LW_DELAY_START_SHIFT                  2

#define COMMON_LW_CONFIG1_REG_ENABLE_SAPB_MEDIUM_PICK_MASK          0x00000002 /*!< ENABLE_PGAC_85_LOAD*/
#define COMMON_LW_CONFIG1_REG_ENABLE_SAPB_MEDIUM_PICK_SHIFT         1
#define COMMON_LW_CONFIG1_REG_LW_ENABLE_SAPB_REPICK_MASK            0x00000001 /*!< LW_ENABLE_SAPB_REPICK*/
#define COMMON_LW_CONFIG1_REG_LW_ENABLE_SAPB_REPICK_SHIFT           0 
/****************************************************************************
 * top_pam_amba :: COMMON_LW_CONFIG2_REG         //FW write
 * #define HOST_DSP_CONFIG2_REG               QUAD_CORE_CONFIG_CFGVAL_100_RDB
 * #define MEDIA_DSP_CONFIG2_REG              QUAD_CORE_CONFIG_CFGVAL_101_RDB
 ***************************************************************************/
#define COMMON_LW_CONFIG2_REG_APB_TUNE_MSE_ACCU_NUM_MASK            0x00003E00 /*!< APB_TUNE_MSE_ACCU_NUM */
#define COMMON_LW_CONFIG2_REG_APB_TUNE_MSE_ACCU_NUM_SHIFT           9
#define COMMON_LW_CONFIG2_REG_LW_APB_BIO_DTUNE_3_STATES_MASK        0x00000100 /*!< LW_APB_BIO_DTUNE_3_STATES */
#define COMMON_LW_CONFIG2_REG_LW_APB_BIO_DTUNE_3_STATES_SHIFT       8
#define COMMON_LW_CONFIG2_REG_LW_LPM_PER_LANE_FLAG_MASK             0x000000FF /*!< LW_LPM_PER_LANE_FLAG*/
#define COMMON_LW_CONFIG2_REG_LW_LPM_PER_LANE_FLAG_SHIFT            0


/****************************************************************************
 * top_pam_amba :: COMMON_LW_CONFIG3_REG         //FW write
 * #define HOST_DSP_CONFIG3_REG               QUAD_CORE_CONFIG_CFGVAL_121_RDB
 * #define MEDIA_DSP_CONFIG3_REG              QUAD_CORE_CONFIG_CFGVAL_122_RDB
 ***************************************************************************/
#define COMMON_LW_CONFIG3_REG_AP_TUNE_MSE_ACCU_NUM_MASK             0x0000003E /*!< AP_TUNE_MSE_ACCU_NUM */
#define COMMON_LW_CONFIG3_REG_AP_TUNE_MSE_ACCU_NUM_SHIFT            1

/****************************************************************************
 * top_pam_amba    cAPI config FW read
#define COMMON_FW_DSP_PLL_MEDIA_FRACN_CFG                     QUAD_CORE_CONFIG_CFGVAL_97_RDB
 ***************************************************************************/
 
#define COMMON_FW_DSP_PLL_MEDIA_FRACN_CFG_DYNAMIC_PLL_RATIO_SHIFT_DELAY_10US_MASK   0x0000F800 /*!< DYNAMIC_PLL_RATIO_SHIFT_DELAY_10US*/
#define COMMON_FW_DSP_PLL_MEDIA_FRACN_CFG_DYNAMIC_PLL_RATIO_SHIFT_DELAY_10US_SHIFT  11

#define COMMON_FW_DSP_PLL_MEDIA_FRACN_CFG_DYNAMIC_PLL_RATIO_SHIFT_STEP_0P1_MASK     0x000007C0 /*!< DYNAMIC_PLL_RATIO_SHIFT_STEP_0P1*/
#define COMMON_FW_DSP_PLL_MEDIA_FRACN_CFG_DYNAMIC_PLL_RATIO_SHIFT_STEP_0P1_SHIFT    6

#define COMMON_FW_DSP_PLL_MEDIA_FRACN_CFG_PPM_VARIANTION_MASK                       0x0000003F /*!< PPM_VARIANTION*/
#define COMMON_FW_DSP_PLL_MEDIA_FRACN_CFG_PPM_VARIANTION_SHIFT                      0


/****************************************************************************
 * top_pam_amba    HOST write LINE read
#define COMMON_FW_DSP_PLL_MEDIA_FRACN_REQ                     QUAD_CORE_CONFIG_CFGVAL_98_RDB
 ***************************************************************************/
#define COMMON_FW_DSP_PLL_MEDIA_FRACN_REQ_ENABLE_FRACN_MODE_MASK         0x00008000 /*!< ENABLE_FRACN_MODE*/
#define COMMON_FW_DSP_PLL_MEDIA_FRACN_REQ_ENABLE_FRACN_MODE_SHIFT        15

#define COMMON_FW_DSP_PLL_MEDIA_FRACN_REQ_INTEG_PPM_MASK                 0x00001FFF /*!< INTEG_PPM*/
#define COMMON_FW_DSP_PLL_MEDIA_FRACN_REQ_INTEG_PPM_SHIFT                0
/****************************************************************************
 * top_pam_amba    HOST read LINE write
#define COMMON_FW_DSP_PLL_MEDIA_FRACN_ST                      QUAD_CORE_CONFIG_CFGVAL_99_RDB
 ***************************************************************************/
 
#define COMMON_FW_DSP_PLL_MEDIA_FRACN_ST_DYNAMIC_PLL_RATIO_SHIFT_ON_MASK          0x00000100 /*!< DYNAMIC_PLL_RATIO_SHIFT_ON*/
#define COMMON_FW_DSP_PLL_MEDIA_FRACN_ST_DYNAMIC_PLL_RATIO_SHIFT_ON_SHIFT         8

#define COMMON_FW_DSP_PLL_MEDIA_FRACN_ST_FRC_PLL_RLOCK_CNT_MASK          0x000000FE /*!< FRC_PLL_RLOCK_CNT*/
#define COMMON_FW_DSP_PLL_MEDIA_FRACN_ST_FRC_PLL_RLOCK_CNT_SHIFT         1

#define COMMON_FW_DSP_PLL_MEDIA_FRACN_ST_ENABLED_FRACN_MODE_MASK         0x00000001 /*!< ENABLED_FRACN_MODE*/
#define COMMON_FW_DSP_PLL_MEDIA_FRACN_ST_ENABLED_FRACN_MODE_SHIFT        0
#endif


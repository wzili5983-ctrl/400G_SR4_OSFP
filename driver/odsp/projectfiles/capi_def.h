/**
 *
 * @file     capi_def.h
 * @author   
 * @date     11/11/2018
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
 * @brief   This file contains CAPI related types, etc.
 *
 * @section
 * 
 */
#ifndef CAPI_DEF_H
#define CAPI_DEF_H

#ifdef __cplusplus
extern "C" {
#endif

#define HOST_SIDE_NUMBER_OF_LANE         4      /**<  Host Side Maximum Number of Lanes            */
#define MEDIA_SIDE_NUMBER_OF_LANE        4      /**<  Media side Maximum Number of Lanes           */
  
#define SET_CONFIG                       0      /**< Set the configuration within the firmware     */
#define GET_CONFIG                       1      /**< Get the configuration of the firmware setting */
#define CAPI_VERSION                     0xFFFF /**< CAPI Version                                  */
#define CAPI_MAX_PORTS                   4      /**< Maximum number of ports                       */
#define CORE_READY_TIMER                 400    /**< 4sec for bringup stage                        */

#define PORT_CFG_LANE_RDY_TIMEOUT        200    /*< Port Configuration Ready Timeout               */
#define QSFP_I2C_SLAVE_ADDRESS_DEFAULT   0x50   /*< default qsfp i2c slave address                 */

#define GP_CFG_NUM                       128    /**< Number of GP Config registers                 */
#define RST_DELAY_MS                     2000   /**< Reset Delay in milli-sec                      */

#define VOLTAGE_MAX_ITERATION            200    /**< Maximum iterations for reading chip voltage   */
#define VOLTAGE_WAIT_TIME_MS             10     /**< Maximum wait time in milli seconds            */

/**
 * Same as phy_info_t
 */
typedef phy_info_t capi_phy_info_t;

/**
 * The enumeration for Phy Side selection
 */
typedef enum capi_phy_side_e {
    PHY_HOST_SIDE  = 0,                  /**< Phy Host Side  : The valud shall not be changed */
    PHY_MEDIA_SIDE = 1,                  /**< Phy Media Side : The valud shall not be changed */
    PHY_BOTH_SIDES                       /**< Phy Both Side                                   */
} phy_side_t;                            /**< Phy Side type                                   */

/**
 * The enumeration for Transceiver direction
 */
typedef enum transceiver_direction_e {
    RX_DIRECTION  = 0,          /**< Rx Direction : This value shall not be changed */
    TX_DIRECTION  = 1,          /**< Tx Direction : This value shall not be changed */
    BOTH_DIRECTIONS             /**< Both Rx & Tx directions                        */
} transceiver_direction_t;      /**< Transceiver direction Type                     */

/**
 * The enumeration for enable/disable
 */
typedef enum capi_enable_e {
    CAPI_DISABLE,                        /*!< operation disable      */
    CAPI_ENABLE                          /*!< operation enable       */
} capi_enable_t;                         /*!< Enable/Disable type    */

/**
 * The enumeration of switch types
 */
typedef enum capi_switch_e {
    CAPI_SWITCH_OFF,                     /**< Switch off             */
    CAPI_SWITCH_ON,                      /**< Switch on              */
    CAPI_SWITCH_TOGGLE                   /**< Switch toggle off/on   */
} capi_switch_t;                         /**< Switch type            */

/**
 * The enumeration of voltage types
 */
typedef enum capi_voltage_e {
    CAPI_ZERO,                           /**< 0 Voltage              */
    CAPI_ZERO_P_SEVEN_FIVE_MLV,          /**< 0.75 mlV               */
    CAPI_ZERO_P_NINE_MLV,                /**< 0.9 mlV                */
    CAPI_ONE_P_TWO_MLV,                  /**< 1.2 mlV                */
    CAPI_DEFAULT                         /**< Default Voltage        */
} capi_voltage_t;                        /**< Voltage types          */


/**
 * The structure for Tx Driver
 */
typedef struct tx_driver_s {
    uint8_t voltage;                     /**< Direction: refer to capi_voltage_t    */
} tx_driver_t;                           /**< Tx Driver type                        */

/**
 * The enumeration for polarity action (default/invert default/swap current)
 */
typedef enum capi_polarity_action_e {
    POLARITY_DEFAULT_SETTING,            /**< Default Polarity       */
    POLARITY_INVERT_DEFAULT_SETTING,     /**< Inver Default Polarity */
    POLARITY_SWAP_CURRENT_SETTING        /**< Swap Current Polarity  */
} capi_polarity_action_t;                /**< Polarity catio type    */

/**
 * The enumeration for data direction (ingress/egress/duplex)
 */
typedef enum capi_direction_e {
    DIR_INGRESS,                         /**< Ingress: Switch <-  HOST <-  CW <-  MEDIA <-  Fiber/Optical */
    DIR_EGRESS,                          /**< Egress : Switch  -> HOST  -> CW  -> MEDIA  -> Fiber/Optical */
    DIR_BOTH                             /**< Both   : Switch <-> HOST <-> CW <-> MEDIA <-> Fiber/Optical */
} capi_direction_t;                      /**< Direction type                                              */

/**
 * The structure for polarity info
 */
typedef struct capi_polarity_info_s {
    uint8_t direction;                   /**< Direction: refer to capi_direction_t                         */
    uint8_t action;                      /**< Polarity setting: refer to capi_polarity_action_t            */
} capi_polarity_info_t;                  /**< Polarity Info type                                           */

/**
 * The enumeration for command type
 */
typedef enum capi_command_type_s {
    SET,                                /**< Set Config                                                    */
    GET,                                /**< Get Config                                                    */
    RESP
} capi_command_type_t;                  /**< Command type                                                  */


/**
 * The enumeration of line lock status
 */
typedef enum line_lock_e {
    CAPI_LINE_UNLOCKED,                  /**< Line Un-locked     */
    CAPI_LINE_LOCKED                     /**< Line Locked        */
} capi_line_lock_t;                      /**< Line Locked type   */

/**
 * The enumeration of CDR lock
 */
typedef enum cdr_lock_e {
    CAPI_CDR_UNLOCKED,                    /**< CDR Locked        */
    CAPI_CDR_LOCKED                       /**< CDR Unlocked      */
} capi_cdr_lock_t;                        /**< CDR Locked type   */

/**
 * The enumeration of lose of signal
 */
typedef enum lose_of_signal_e {
    CAPI_NO_LOSE_OF_SIGNAL,               /**< No Lose of Signal   */
    CAPI_LOSE_OF_SIGNAL                   /**< Lose of Signal      */
} capi_los_t;                             /**< Lose of Signal type */

/**
 * The enumeration of pll lock
 */
typedef enum pll_lock_e {
    CAPI_PLL_OUT_OF_LOCK,                  /**< PLL out of lock    */
    CAPI_PLL_LOCKED                        /**< PLL Locked         */
} capi_pll_lock_t;                         /**< PLL Lock type      */

/**
 * The enum definition of line side link training type
 */
typedef enum {
    LNKTRN_CL72,                            /*!< 0 - CL72   */
    LNKTRN_CL93,                            /*!< 1 - CL93   */
    LNKTRN_802_3CD,                         /*!< 2 - 802.3CD*/
    LNKTRN_TYPE_MAX                         /**< Link Training Type max element */
} capi_lnktrn_type_t;                       /**< Link Training type */

/**
 * The enum definition of line side link training frame size
 */
typedef enum {
    LNKTRN_FRM_SIZE_4K,                     /*!< 0 - frame size 4K */
    LNKTRN_FRM_SIZE_16K,                    /*!<1 - frame size 16K */
    LNKTRN_FRM_SIZE_MAX                     /**< Link Training Frame Size max element */
} capi_lnktrn_frm_size_t;                   /**< Link Training Frame Size type */

/**
 * The enum definition of line side link training initial condition for 802.3cd
 */
typedef enum {
    LNKTRN_INIT_COND_NO,                     /*!< 0 - No link training init condition   */
    LNKTRN_INIT_COND_NO_EQ,                  /*!< 1 - Link training init condition Preset1 :no EQ    */
    LNKTRN_INIT_COND_MAX                     /**< Link Training Init Cond max element */
} capi_lnktrn_init_cond_t;                   /**< Link Training Init Cond type */

/**
 * The enum definition of line side link training initial condition for CL72/CL93 NRZ mode
 */
typedef enum {
    LNKTRN_CL_INIT_NORMAL,                   /*!< 0 - No link training init condition   */
    LNKTRN_CL_INIT_EN,                       /*!< 1 - Link training init condition enable  */
    LNKTRN_CL_INIT_MAX                       /**< Link Training Clause Init type */
} capi_lnktrn_cl72_cl93_init_t;

/**
 * The enum definition of line side link training preset for CL72/CL93 NRZ mode
 */
typedef enum {
    LNKTRN_CL_PRESET_NORMAL,                 /*!< 0 - No link training PRESET, normal operation  */
    LNKTRN_CL_PRESET_EN,                     /*!< 1 - Link training PRESET enable: Preset coeffiecients    */
    LNKTRN_CL_PRESET_MAX                     /**< Link Training Clause preset max element */
} capi_lnktrn_cl72_cl93_preset_t;            /**< Link Training Clause Preset type */

/**
 * The structure of line side link training cfg for 802.3cd
 */
typedef struct capi_lnktrn_802_3cd_cfg_s {
    capi_lnktrn_frm_size_t lnktrn_frm_size;    /**< Line side link training frame size, this is only valid for 802.3cd rather than CL72 & CL93 */
    capi_lnktrn_init_cond_t lnktrn_init_cond;  /**< Line side link training init condition type for 802.3cd */
} capi_lnktrn_802_3cd_cfg_t;                   /**< Line Side Link Training Config information for 802.3cd type */

/**
 * The structure of line side link training cfg for CL72 and CL93
 */
typedef struct capi_lnktrn_cl72_cl93_cfg_s {
    capi_lnktrn_cl72_cl93_init_t lnktrn_cl_init;       /**< Line side link training init condition type for CL72 & CL93 */
    capi_lnktrn_cl72_cl93_preset_t lnktrn_cl_preset;   /**< Line side link training preset type for CL72 & CL93 */
} capi_lnktrn_cl72_cl93_cfg_t;


/**
 * The enumeration of line side link training timeout
 */
typedef enum capi_lt_timer_e {
    CAPI_LT_TIMER_DEFAULT = 0,                         /**< Link Training Timer default value: firmware default*/
    CAPI_LT_TIMER_500MS = 1,                           /**< Link Training Timer value: 500ms   */
    CAPI_LT_TIMER_1S = 2,                              /**< Link Training Timer value: 1s      */
    CAPI_LT_TIMER_1S_500MS = 3,                        /**< Link Training Timer value: 1.5s    */
    CAPI_LT_TIMER_2S = 4,                              /**< Link Training Timer value: 2s      */
    CAPI_LT_TIMER_2S_500MS = 5,                        /**< Link Training Timer value: 2.5s    */
    CAPI_LT_TIMER_3S = 6,                              /**< Link Training Timer value: 3s      */
    CAPI_LT_TIMER_3S_500MS = 7,                        /**< Link Training Timer value: 3.5s    */
    CAPI_LT_TIMER_4S = 8,                              /**< Link Training Timer value: 4s      */
    CAPI_LT_TIMER_4S_500MS = 9,                        /**< Link Training Timer value: 4.5s    */
    CAPI_LT_TIMER_5S = 10,                             /**< Link Training Timer value: 5s      */
    CAPI_LT_TIMER_5S_500MS = 11,                       /**< Link Training Timer value: 5.5s    */
    CAPI_LT_TIMER_6S = 12,                             /**< Link Training Timer value: 6s      */
    CAPI_LT_TIMER_6S_500MS = 13,                       /**< Link Training Timer value: 6.5s    */
    CAPI_LT_TIMER_7S = 14,                             /**< Link Training Timer value: 7s      */
    CAPI_LT_TIMER_7S_500MS = 15,                       /**< Link Training Timer value: 7.5s    */
 } capi_lt_timer_t;                                    /**< Link Training Timer type           */

/**
 * A structure to represent the link training timer
 */
typedef struct capi_link_training_timer_s {
    capi_enable_t   disable_lt_timeout;                /**< CAPI_ENABLE: disable LT timer; CAPI_DISABLE: enable LT timer (in default) */ 
    capi_lt_timer_t lt_timer_duration;                 /**< LT Timer Duration which only is valid when  disable_lt_timeout=CAPI_DISABLE */
} capi_link_training_timer_t;                          /**< Link Training Timer type */

/**
 * The enumeration of link training error code
 */
typedef enum capi_lnktrn_err_e {
    LNKTRN_ERR_NONE,                                   /*!< 0 no error  */
    LNKTRN_ERR_FRM_LCK,                                /*!< 1 check frame lock timeout   */
    LNKTRN_ERR_SNR,                                    /*!< 2 SNR is lower than threshold */
    LNKTRN_ERR_TIMEOUT,                                /*!< 3 link training timeout */
    LNKTRN_ERR_MAX                                     /*!< Max Link training error element */
} capi_lnktrn_err_t;                                   /**< Link Training Error type */

/**
 * The enumeration of linked training done
 */
typedef enum linktrn_done_s {
    CAPI_LINKED_TRAINING_NOT_COMPLETED,             /**< Linked Training Not Completed */
    CAPI_LINKED_TRAINING_SUCCESSFULLU_COMPLETED     /**< Linked Training Successfully Completed */
} capi_linktrn_done_t;                              /**< Linked Training Done type*/

/**
 * The structure of line side link training status
 */
typedef struct link_training_status_s {
    uint8_t enabled;                  /**< 1:Link training enabled;                 0:Link training disabled        */
    uint8_t signal_detect;            /**< 1:Link training FSM in SEND_DATA state;  0:Link in training state        */
    uint8_t failure_detected;         /**< 1:Training failure detected;             0:Training failure not detected */
    uint8_t training_status;          /**< 1:Training protocol complete;         0:Training protocol in progress;    */
    uint8_t receiver_status;          /**< 1:Receiver trained and ready to receive; 0:Receiver training             */
    uint8_t receiver_lock;            /**< 1:Receiver locked;                       0:Receiver not lock             */
    uint8_t frame_lock;               /**< 1:Frame locked;                          0:Frame not lock                */
} link_training_status_t;             /**< Link Training Status type                                                */

/**
 * The enumeration of lose of signal
 */
typedef enum adc_sig_clipping_e {
    CAPI_NO_ADC_SIGNAL_CLIPPING_DETECTED,   /**< No ADC signal clipping detected */
    CAPI_ADC_SIGNAL_CLIPPING_DETECTED       /**< ADC signal clipping detected */
} capi_adc_sig_clipping_t;                  /**< ADC Signal Clipping Detection type*/

/**
 * The enumeration of PLL unlock sticky
 */
typedef enum client_pll_unlock_sticky_e {
    CAPI_PLL_UNLOCK_STICKY_NOT_LOST_LOCK,   /**< Not lost lock after first lock up  */
    CAPI_PLL_UNLOCK_STICKY_LOST_LOCK        /**< Lost lock after first lock up  */
} capi_client_pll_unlock_sticky_t;          /**< client pll unlock sticky type*/

/**
 * The enumeration of client Bh2bh fifo status
 */
typedef enum bh2bh_fifo_status_e {
    CAPI_BH2BH_FIFO_STATUS_NO_COLLISION,    /**< No collision */
    CAPI_BH2BH_FIFO_STATUS_IN_COLLISION     /**< In collision */
} capi_client_bh2bh_fifo_status_t;          /**< bh2bh Fifo Status type*/

/**
 * The enumeration of bh txfir tap setting
 */
typedef enum capi_bh_txfir_tap_enable_e {
    CAPI_NRZ_LP_3TAP,                                /**<BH TX fir nrz 3 tap                                        */
    CAPI_NRZ_6TAP,                                   /**<BH TX fir nrz 6 tap                                        */
    CAPI_PAM4_LP_3TAP,                               /**<BH TX fir pam4 3 tap                                       */
    CAPI_PAM4_6TAP                                   /**<BH TX fir pam4 6 tap                                       */
} capi_bh_txfir_tap_enable_t;                        /**<BH TX fir tap enable type                                  */


#define Min_DSP_LEVEL_SHIFT         -17              /**< DSP Level Shift minimum value -16                         */
#define MAX_DSP_LEVEL_SHIFT          16              /**< DSP Level Shift maximum value  16                         */

/**
 * The enumeration of DSP skew P
 */
typedef enum capi_dsp_skew_p_e {
    CAPI_DSP_SKEW_P_0,                               /**<DSP Skew P value 0                                         */
    CAPI_DSP_SKEW_P_1,                               /**<DSP Skew P value 1                                         */
    CAPI_DSP_SKEW_P_2,                               /**<DSP Skew P value 2                                         */
    CAPI_DSP_SKEW_P_3,                               /**<DSP Skew P value 3                                         */
    CAPI_DSP_SKEW_P_4,                               /**<DSP Skew P value 4                                         */
    CAPI_DSP_SKEW_P_5,                               /**<DSP Skew P value 5                                         */
    CAPI_DSP_SKEW_P_6,                               /**<DSP Skew P value 6                                         */
    CAPI_DSP_SKEW_P_7                                /**<DSP Skew P value 7                                         */
} capi_dsp_skew_p_t;                                 /**<DSP Skew P type                                            */

/**
 * The enumeration of lw tx precode setting
 */
typedef enum capi_lw_tx_precode_indep_e {
    CAPI_LW_INDEP_TX_PRECODE_DEFAULT = 0,            /**<LW TX precode default                                      */
    CAPI_LW_INDEP_TX_PRECODE_OFF     = 1,            /**<LW TX precode off                                          */
    CAPI_LW_INDEP_TX_PRECODE_ON      = 2             /**<LW TX precode on                                           */
} capi_lw_tx_precode_indep_t;                        /**<LW TX precode type                                         */

/**
 * The structure of tx bias information
 */
typedef struct dsp_tx_bias_info_s {
    uint8_t tx_bias_reg1;                                 /**< TX BIAS reg1 [0:15]        */
    uint8_t tx_bias_reg2;                                 /**< TX BIAS reg2  [0:15]        */
    uint8_t tx_bias_reg3;                                 /**< TX BIAS reg3 [0:15]        */
    uint8_t tx_bias_reg4;                                 /**< TX BIAS reg4 [0:15]        */
} dsp_tx_bias_info_t;                              /**< TX bias information type                                                */


/**
 * The structure of TX clock filter 2 info
 */
typedef struct tx_clk_filter2_info_s {
    uint8_t      enable;                         /**< [0:1] 0-Disable clk_filter2; 1-Enabled clk_filter2                   */
    uint8_t      tx_clk_filter2_bw;              /**< [0:16] clk_filter2 BW setting;   */
} tx_clk_filter2_info_t;                         /**< TX clk_filter2 information definition                                             */


/**
 * The structure of lane tx info
 */
typedef struct lane_tx_info_s {
    union {
        uint32_t content;
        struct {
#ifdef CAPI_IS_BIG_ENDIAN
            uint32_t reserved                     : 16;               /**< reserved                                   */
            uint32_t hs_pon_dac                   : 1;                /**< tx hs_pon_dac                              */
            uint32_t hs_cpb                       : 1;                /**< tx hs_cpb                                  */
            uint32_t hs_cp                        : 1;                /**< tx hs_cp                                   */
            uint32_t hs_apb                       : 1;                /**< tx hs_apb                                  */
            uint32_t hs_ap                        : 1;                /**< tx hs_ap                                   */
            uint32_t tx_bias                      : 1;                /**< tx bias                                   */
            uint32_t tx_clk_filter2               : 1;                /**< tx clock filter 2                         */
            uint32_t tx_clk_filter_bw             : 1;                /**< tx clock filter bandwidth                 */
            uint32_t hs_dac_cur                   : 1;                /**< high swing DAC current, line side          */
            uint32_t txfir                        : 1;                /**< txfir, need set dsp_level_shift too        */
            uint32_t symbol_swap                  : 1;                /**< symbol_swap                                */
            uint32_t dsp_level_shift              : 1;                /**< dsp TxFIR level shift, need set txfir too  */
            uint32_t dsp_graycode                 : 1;                /**< dsp graycode                               */
            uint32_t dsp_high_swing               : 1;                /**< dsp high_swing                             */
            uint32_t dsp_skewp                    : 1;                /**< dsp skewp                                  */
            uint32_t dsp_precode                  : 1;                /**< dsp precode                                */
#else /* CAPI_IS_LITTLE_ENDIAN */
            uint32_t dsp_precode                  : 1;                /**< dsp precode                                */
            uint32_t dsp_skewp                    : 1;                /**< dsp skewp                                  */
            uint32_t dsp_high_swing               : 1;                /**< dsp high_swing                             */
            uint32_t dsp_graycode                 : 1;                /**< dsp graycode                               */
            uint32_t dsp_level_shift              : 1;                /**< dsp TxFIR level shift, need set txfir too  */
            uint32_t symbol_swap                  : 1;                /**< symbol_swap                                */
            uint32_t txfir                        : 1;                /**< txfir, need set dsp_level_shift too        */
            uint32_t hs_dac_cur                   : 1;                /**< Tx high swing DAC current                     */
            uint32_t tx_clk_filter_bw             : 1;                /**< tx clock filter bandwidth                 */
            uint32_t tx_clk_filter2               : 1;                /**< tx clock filter 2                         */
            uint32_t tx_bias                      : 1;                /**< tx bias                                   */
            uint32_t hs_ap                        : 1;                /**< tx hs_ap                                   */
            uint32_t hs_apb                       : 1;                /**< tx hs_apb                                  */
            uint32_t hs_cp                        : 1;                /**< tx hs_cp                                   */
            uint32_t hs_cpb                       : 1;                /**< tx hs_cpb                                  */
            uint32_t hs_pon_dac                   : 1;                /**< tx hs_pon_dac                              */
            uint32_t reserved                     : 16;               /**< reserved                                   */
#endif
        } is;
    } param;

    struct {
        /**< The following parameters are commonly used for SERDES and DSP */
        txfir_info_t        txfir;                                         /**<  TXFIR, need set dsp_level_shift too                */
        uint8_t             hs_dac_cur;                                    /**<  Tx high swing DAC current, line side, valid range is [0, 4] */
        uint8_t             symbol_swap;                                   /**<  TX_info symbol_swap: 0- No swap 1- Swap            */

        /**< The following parameters are only used for DSP */
        int8_t              dsp_level_shift[4];                            /**<DSP : TxFIR level shift, need set TXFIR too          */
        uint8_t             dsp_graycode;                                  /**<DSP : gray code enable: 0- Disable; 1- Enable        */
        uint8_t             dsp_high_swing;                                /**<DSP : high swing enable: 0- Disable; 1- Enable       */
        uint8_t             dsp_skewp;                                     /**<DSP : skew P value, 3 bits value, [0, 7]             */
        uint8_t             dsp_precode;                                   /**<DSP : precode: refer to capi_lw_tx_precode_indep_t   */
        uint8_t             dsp_tx_clk_filter_bw;                          /**<DSP : TX clock filter bandwidth [0,16]  */
        tx_clk_filter2_info_t dsp_tx_clk_filter2;                          /**<DSP : TX clock filter 2                              */
 
        dsp_tx_bias_info_t  dsp_tx_bias;                                   /**<DSP : TX bias setting: reg1~reg4 */

        uint8_t             hs_ap;                                         /**<DSP : TX hs_ap setting: 4bits value [0,15] */
        uint8_t             hs_apb;                                        /**<DSP : TX hs_apb setting: 4bits value [0,15] */
        uint8_t             hs_cp;                                         /**<DSP : TX hs_cp setting: 4bits value [0,15] */
        uint8_t             hs_cpb;                                        /**<DSP : TX hs_cpb setting: 4bits value [0,15] */
        uint8_t             hs_pon_dac;                                    /**<DSP : TX hs_ap setting: 3bits value [0,7] */
    } value;
} lane_tx_info_t;                                                    /**<Lane Tx Info type                                    */

#define CAPI_NUM_DFE_TAPS 14 /*CENTENARIO_NUM_DFE_TAPS         EPDM parameter */

/**
 * The enumeration of media type
 */
typedef enum capi_media_type_e {
    CAPI_MEDIA_TYPE_OPTICAL,                             /**< Optical Media                                                  */
    CAPI_MEDIA_TYPE_PCB_TRACE_BACK_PLANE,                /**< PCB Trace back plane Media                                     */
    CAPI_MEDIA_TYPE_COPPER_CABLE                         /**< Copper Cable Media                                             */
} capi_media_type_t;                                     /**< Media type                                                     */


#define DSP_RX_INFO_VGA_LOW_RANGE                 0      /**< DSP    : Rx Information VGA low threshold/range                */
#define DSP_RX_INFO_VGA_HIGH_RANGE               46      /**< DSP    : Rx Information VGA high threshold/range               */
#define SERDES_RX_INFO_VGA_LOW_RANGE              0      /**< SERDES : Rx Information VGA low threshold/range                */
#define SERDES_RX_INFO_VGA_HIGH_RANGE            32      /**< SERDES : Rx Information VGA high threshold/range               */

#define DSP_RX_INFO_PEAKING_FILTER_LOW_RANGE   0         /**< DSP   : Rx Information peaking filter low range                */
#define DSP_RX_INFO_PEAKING_FILTER_HIGH_RANGE 31         /**< DSP   : Rx Information peaking filter high range               */
//#define SERDES_RX_INFO_PEAKING_FILTER_LOW_RANGE   0      /**< SERDES: Rx Information peaking filter low range                */
//#define SERDES_RX_INFO_PEAKING_FILTER_HIGH_RANGE 46      /**< SERDES: Rx Information peaking filter high range               */


/**
 * The enumeration of symbol swap
 */
typedef enum capi_symbol_swap_e {
    CAPI_SYMBOL_SWAP_NONE,                                /**< Do not swap the MSB and LSB within a symbol      */
    CAPI_SYMBOL_SWAP_MSB_AND_LSB                          /**< Swap the MSB and LSB within a symbol             */
} capi_symbol_swap_t;                                     /**< Symbol Swap type                                 */


/**
 * The enumeration of symbol swap
 */
typedef enum capi_equalizer2_e {
    CAPI_EQUALIZER_2_TYPE_1A,                             /**< Equalizer 2 type 1A                              */
    CAPI_EQUALIZER_2_TYPE_10A                             /**< Equalizer 2 type 10 A                            */
} capi_equalizer2_t;                                      /**< Equalizer 2 type                                 */

/**
 * The enumeration of optical los reliability
 */
typedef enum capi_optical_los_reliability_e {
    CAPI_UNRELIABLE_OPTICAL_LOS,                          /**< Unreliable optical LOS                           */
    CAPI_RELIABLE_OPTICAL_LOS                             /**< Reliable optical LOS                             */
} capi_optical_los_reliability_t;                         /**< Optical LOS Reliability type                     */


/**
 * The enumeration of scrambling
 */
typedef enum capi_scrambling_e {
    CAPI_SCRAMBLING_OFF,                                  /**< Scrambling is not used                           */
    CAPI_SCRAMBLING_ON,                                   /**< Scrambling is on                                 */
    CAPI_SCRAMBLING_UNKNOWN                               /**< Unknow : RX data may have repeating data patterns*/
} capi_scrambling_t;                                      /**< Scrambling type                                  */


/**
 * The enumeration of PAM RX PRECODE type
 */
typedef enum capi_pam_rx_precode_e {
    CAPI_PAM_RX_PRECODE_FW_DEFAULT,                       /**< PAM rx precode, FW default setting            */
    CAPI_PAM_RX_PRECODE_FORCE_OFF,                        /**< PAM rx precode off                            */
    CAPI_PAM_RX_PRECODE_FORCE_ON,                        /**< PAM rx precode on                            */
} capi_pam_rx_precode_t;                                      /**< PAM rx precode type                                   */


/**
 * The enumeration of channel los
 */
typedef enum capi_channel_los_e {
    CAPI_CHANNEL_LOS_DEFAULT_0_DBM,                       /**< default: forces microcode to figure out th best optimization */
    CAPI_CHANNEL_LOS_VSR_1_TO_10_DBM,                     /**< VSR Channel 1-10                                             */
    CAPI_CHANNEL_LOS_MR_11_TO_20_DBM,                     /**< MR Channel  11-20                                            */
    CAPI_CHANNEL_LOS_LR_20_PLUS_DBM                       /**< LR Channel  20+                                              */
} capi_channel_los_t;                                     /**< Channel LOS type                                             */


/**
 * The enumeration of DC wander mu
 */
typedef enum capi_dc_wander_mu_e {
    CAPI_DC_WANDER_MU_0,                                   /**< DC wander mu 0                                   */
    CAPI_DC_WANDER_MU_1,                                   /**< DC wander mu 1                                   */
    CAPI_DC_WANDER_MU_2,                                   /**< DC wander mu 2                                   */
    CAPI_DC_WANDER_MU_3,                                   /**< DC wander mu 3                                   */
    CAPI_DC_WANDER_MU_4,                                   /**< DC wander mu 4                                   */
    CAPI_DC_WANDER_MU_5,                                   /**< DC wander mu 5                                   */
    CAPI_DC_WANDER_MU_6,                                   /**< DC wander mu 6                                   */
    CAPI_DC_WANDER_MU_7                                    /**< DC wander mu 7                                   */
} capi_dc_wander_mu_t;                                     /**< DC Wander MU type                                */


/**
 * The enumeration of gain boost
 */
typedef enum capi_gain_boost_e {
    CAPI_GAIN_BOOST_0_DBM,                                 /**< Gain boost 0 dbm                                 */
    CAPI_GAIN_BOOST_1_DBM,                                 /**< Gain boost 1 dbm                                 */
    CAPI_GAIN_BOOST_2_DBM,                                 /**< Gain boost 2 dbm                                 */
    CAPI_GAIN_BOOST_3_DBM,                                 /**< Gain boost 3 dbm                                 */
    CAPI_GAIN_BOOST_4_DBM,                                 /**< Gain boost 4 dbm                                 */
    CAPI_GAIN_BOOST_5_DBM,                                 /**< Gain boost 5 dbm                                 */
    CAPI_GAIN_BOOST_6_DBM                                  /**< Gain boost 6 dbm                                 */
} capi_gain_boost_t;                                       /**< Gain Boost type                                  */


/**
 * The enumeration of LMS mode
 */
typedef enum capi_lms_mode_e {
    CAPI_LMS_MODE_1,                                       /**< LMS mode 1                                       */
    CAPI_LMS_MODE_8                                        /**< LMS mode 8                                       */
} capi_lms_mode_t;                                         /**< LMS Mode type                                    */


/**
 * The enumeration of los threshold mapping index
 */
typedef enum capi_los_threshold_mapping_index_e {
    CAPI_LOS_THRESHOLD_MAPPING_INDEX_0,                   /**< LOS threshold mapping index 0                    */
    CAPI_LOS_THRESHOLD_MAPPING_INDEX_1,                   /**< LOS threshold mapping index 1                    */
    CAPI_LOS_THRESHOLD_MAPPING_INDEX_2,                   /**< LOS threshold mapping index 2                    */
    CAPI_LOS_THRESHOLD_MAPPING_INDEX_3,                   /**< LOS threshold mapping index 3                    */
    CAPI_LOS_THRESHOLD_MAPPING_INDEX_4,                   /**< LOS threshold mapping index 4                    */
    CAPI_LOS_THRESHOLD_MAPPING_INDEX_5,                   /**< LOS threshold mapping index 5                    */
    CAPI_LOS_THRESHOLD_MAPPING_INDEX_6,                   /**< LOS threshold mapping index 6                    */
    CAPI_LOS_THRESHOLD_MAPPING_INDEX_7,                   /**< LOS threshold mapping index 7                    */
    CAPI_LOS_THRESHOLD_MAPPING_INDEX_8,                   /**< LOS threshold mapping index 8                    */
    CAPI_LOS_THRESHOLD_MAPPING_INDEX_9,                   /**< LOS threshold mapping index 9                    */
    CAPI_LOS_THRESHOLD_MAPPING_INDEX_10,                  /**< LOS threshold mapping index 10                   */
    CAPI_LOS_THRESHOLD_MAPPING_INDEX_11,                  /**< LOS threshold mapping index 11                   */
    CAPI_LOS_THRESHOLD_MAPPING_INDEX_12                   /**< LOS threshold mapping index 12                   */
} capi_los_threshold_mapping_index_t;                     /**< LOS threshold mapping index type                 */


/**
 * The enumeration of inlos threshold mapping index
 */
typedef enum capi_inlos_threshold_mapping_index_e {
    CAPI_INLOS_THRESHOLD_MAPPING_INDEX_0,                 /**< INLOS threshold mapping index 0                  */
    CAPI_INLOS_THRESHOLD_MAPPING_INDEX_1,                 /**< INLOS threshold mapping index 1                  */
    CAPI_INLOS_THRESHOLD_MAPPING_INDEX_2,                 /**< INLOS threshold mapping index 2                  */
    CAPI_INLOS_THRESHOLD_MAPPING_INDEX_3,                 /**< INLOS threshold mapping index 3                  */
    CAPI_INLOS_THRESHOLD_MAPPING_INDEX_4,                 /**< INLOS threshold mapping index 4                  */
    CAPI_INLOS_THRESHOLD_MAPPING_INDEX_5,                 /**< INLOS threshold mapping index 5                  */
    CAPI_INLOS_THRESHOLD_MAPPING_INDEX_6,                 /**< INLOS threshold mapping index 6                  */
    CAPI_INLOS_THRESHOLD_MAPPING_INDEX_7,                 /**< INLOS threshold mapping index 7                  */
    CAPI_INLOS_THRESHOLD_MAPPING_INDEX_8,                 /**< INLOS threshold mapping index 8                  */
    CAPI_INLOS_THRESHOLD_MAPPING_INDEX_9                  /**< INLOS threshold mapping index 9                  */
} capi_inlos_threshold_mapping_index_t;                   /**< INLOS threshold mapping index type               */


/**
 * The enumeration of outlos threshold mapping index
 */
typedef enum capi_outlos_threshold_mapping_index_e {
    CAPI_OUTLOS_THRESHOLD_MAPPING_INDEX_0,                 /**< OUTLOS threshold mapping index 0                 */
    CAPI_OUTLOS_THRESHOLD_MAPPING_INDEX_1,                 /**< OUTLOS threshold mapping index 1                 */
    CAPI_OUTLOS_THRESHOLD_MAPPING_INDEX_2,                 /**< OUTLOS threshold mapping index 2                 */
    CAPI_OUTLOS_THRESHOLD_MAPPING_INDEX_3,                 /**< OUTLOS threshold mapping index 3                 */
    CAPI_OUTLOS_THRESHOLD_MAPPING_INDEX_4,                 /**< OUTLOS threshold mapping index 4                 */
    CAPI_OUTLOS_THRESHOLD_MAPPING_INDEX_5,                 /**< OUTLOS threshold mapping index 5                 */
    CAPI_OUTLOS_THRESHOLD_MAPPING_INDEX_6,                 /**< OUTLOS threshold mapping index 6                 */
    CAPI_OUTLOS_THRESHOLD_MAPPING_INDEX_7,                 /**< OUTLOS threshold mapping index 7                 */
    CAPI_OUTLOS_THRESHOLD_MAPPING_INDEX_8,                 /**< OUTLOS threshold mapping index 8                 */
    CAPI_OUTLOS_THRESHOLD_MAPPING_INDEX_9                  /**< OUTLOS threshold mapping index 9                 */
} capi_outlos_threshold_mapping_index_t;                   /**< OUTLOS threshold mapping index type              */



/**
 * The enumeration of equalizer tab select
 */
typedef enum capi_equalizer_tab_select_e {
    CAPI_EQUALIZER_TAB_SELECT_DEFAULT_SETTING,             /**< Equalizer Tab Select: firmware default setting  */
    CAPI_EQUALIZER_TAB_SELECT_HIGH_POWER,                  /**< Equalizer Tab Select: high power                */
    CAPI_EQUALIZER_TAB_SELECT_MEDIUM_POWER,                /**< Equalizer Tab Select: medium power              */
    CAPI_EQUALIZER_TAB_SELECT_LOW_POWER                    /**< Equalizer Tab Select: low power                 */
} capi_equalizer_tab_select_t;                             /**< Equalizer Tab Select type                       */



/**
 * The enumeration of FFE slicer multiplier
 */
typedef enum capi_ffe_slicer_multiplier_e {
    CAPI_FFE_SLICER_MULTIPLIER_1,                           /**< FFE slicer multiplier 1                        */
    CAPI_FFE_SLICER_MULTIPLIER_2                            /**< FFE slicer multiplier 2                        */
} capi_ffe_slicer_multiplier_t;                             /**< FFE slicer multiplier type                     */


/**
 * The enumeration of phase bias threshold
 */
typedef enum capi_phase_bias_threshold_e {
    CAPI_PHASE_BIAS_THRESHOLD_MINUS_16,                 /**< Phase bias threshold -16                         */
    CAPI_PHASE_BIAS_THRESHOLD_MINUS_15,                 /**< Phase bias threshold -15                         */
    CAPI_PHASE_BIAS_THRESHOLD_MINUS_14,                 /**< Phase bias threshold -14                         */
    CAPI_PHASE_BIAS_THRESHOLD_MINUS_13,                 /**< Phase bias threshold -13                         */
    CAPI_PHASE_BIAS_THRESHOLD_MINUS_12,                 /**< Phase bias threshold -12                         */
    CAPI_PHASE_BIAS_THRESHOLD_MINUS_11,                 /**< Phase bias threshold -11                         */
    CAPI_PHASE_BIAS_THRESHOLD_MINUS_10,                 /**< Phase bias threshold -10                         */
    CAPI_PHASE_BIAS_THRESHOLD_MINUS_9,                  /**< Phase bias threshold -9,                         */
    CAPI_PHASE_BIAS_THRESHOLD_MINUS_8,                  /**< Phase bias threshold -8,                         */
    CAPI_PHASE_BIAS_THRESHOLD_MINUS_7,                  /**< Phase bias threshold -7,                         */
    CAPI_PHASE_BIAS_THRESHOLD_MINUS_6,                  /**< Phase bias threshold -6,                         */
    CAPI_PHASE_BIAS_THRESHOLD_MINUS_5,                  /**< Phase bias threshold -5,                         */
    CAPI_PHASE_BIAS_THRESHOLD_MINUS_4,                  /**< Phase bias threshold -4,                         */
    CAPI_PHASE_BIAS_THRESHOLD_MINUS_3,                  /**< Phase bias threshold -3,                         */
    CAPI_PHASE_BIAS_THRESHOLD_MINUS_2,                  /**< Phase bias threshold -2,                         */
    CAPI_PHASE_BIAS_THRESHOLD_MINUS_1,                  /**< Phase bias threshold -1,                         */
    CAPI_PHASE_BIAS_THRESHOLD_0,                        /**< Phase bias threshold  0,                         */
    CAPI_PHASE_BIAS_THRESHOLD_PLUS_1,                   /**< Phase bias threshold  1,                         */
    CAPI_PHASE_BIAS_THRESHOLD_PLUS_2,                   /**< Phase bias threshold  2,                         */
    CAPI_PHASE_BIAS_THRESHOLD_PLUS_3,                   /**< Phase bias threshold  3,                         */
    CAPI_PHASE_BIAS_THRESHOLD_PLUS_4,                   /**< Phase bias threshold  4,                         */
    CAPI_PHASE_BIAS_THRESHOLD_PLUS_5,                   /**< Phase bias threshold  5,                         */
    CAPI_PHASE_BIAS_THRESHOLD_PLUS_6,                   /**< Phase bias threshold  6,                         */
    CAPI_PHASE_BIAS_THRESHOLD_PLUS_7,                   /**< Phase bias threshold  7,                         */
    CAPI_PHASE_BIAS_THRESHOLD_PLUS_8,                   /**< Phase bias threshold  8,                         */
    CAPI_PHASE_BIAS_THRESHOLD_PLUS_9,                   /**< Phase bias threshold  9,                         */
    CAPI_PHASE_BIAS_THRESHOLD_PLUS_10,                  /**< Phase bias threshold  10                         */
    CAPI_PHASE_BIAS_THRESHOLD_PLUS_11,                  /**< Phase bias threshold  11                         */
    CAPI_PHASE_BIAS_THRESHOLD_PLUS_12,                  /**< Phase bias threshold  12                         */
    CAPI_PHASE_BIAS_THRESHOLD_PLUS_13,                  /**< Phase bias threshold  13                         */
    CAPI_PHASE_BIAS_THRESHOLD_PLUS_14,                  /**< Phase bias threshold  14                         */
    CAPI_PHASE_BIAS_THRESHOLD_PLUS_15                   /**< Phase bias threshold  15                         */
} capi_phase_bias_threshold_t;                          /**< Phase bias treshold type                         */



/**
 * The enumeration of dynamic auto phase tune
 */
typedef enum capi_dynamic_auto_phase_tune_e {
    CAPI_DYNAMIC_AUTO_PATH_TUNE_TYPE_THRESHOLD_COMPARE,     /**< 0: threshold compare                         */
    CAPI_DYNAMIC_AUTO_PATH_TUNE_TYPE_RECURSIVE              /**< 1: calculation & adjust                      */
} capi_dynamic_auto_phase_tune_t;                           /**< Dynamic auto path tune type                  */



/**
 * The enumeration of data mode select
 */
typedef enum capi_data_mode_select_e {
    CAPI_DATA_MODE_SELECT_FIR_USES_1_AND_2_SAMPLES,         /**< 0: FIR uses 1st & 2nd samples                */
    CAPI_DATA_MODE_SELECT_FIR_USES_2_AND_3_SAMPLES          /**< 1: FIR uses 2nd & 3rd samples                */
} capi_data_mode_select_t;                                  /**< Data Mode Select type                        */


/**
 * The enumeration of coefficient select
 */
typedef enum capi_coefficient_select_e {
    CAPI_COEFFICIENT_SELECT_0,                                 /**< 0: FIR a = 0                                 */
    CAPI_COEFFICIENT_SELECT_1_OVER_16,                         /**< 1: FIR a = 1/16                              */
    CAPI_COEFFICIENT_SELECT_2_OVER_16,                         /**< 2: FIR a = 2/16                              */
    CAPI_COEFFICIENT_SELECT_3_OVER_16,                         /**< 3: FIR a = 3/16                              */
    CAPI_COEFFICIENT_SELECT_4_OVER_16,                         /**< 4: FIR a = 4/16                              */
    CAPI_COEFFICIENT_SELECT_5_OVER_16,                         /**< 5: FIR a = 5/16                              */
    CAPI_COEFFICIENT_SELECT_6_OVER_16,                         /**< 6: FIR a = 6/16                              */
    CAPI_COEFFICIENT_SELECT_7_OVER_16,                         /**< 7: FIR a = 7/16                              */
    CAPI_COEFFICIENT_SELECT_8_OVER_16                          /**< 8: FIR a = 8/16                              */
} capi_coefficient_select_t;                                   /**< Coefficient Select type                      */


/**
 * The enumeration of least median square
 */
typedef enum capi_least_median_square_e {
    CAPI_LEAST_MEDIAN_SQUARE_MU_2_POWER_OF_MINUS_11,           /**< 0: LMS mu = 2^-11                            */
    CAPI_LEAST_MEDIAN_SQUARE_MU_2_POWER_OF_MINUS_9,            /**< 1: LMS mu = 2^-9                             */
    CAPI_LEAST_MEDIAN_SQUARE_MU_2_POWER_OF_MINUS_7,            /**< 2: LMS mu = 2^-7                             */
    CAPI_LEAST_MEDIAN_SQUARE_MU_2_POWER_OF_MINUS_5,            /**< 3: LMS mu = 2^-5                             */
    CAPI_LEAST_MEDIAN_SQUARE_MU_RESERVED,                      /**< 4: LMS mu Reserved                           */
    CAPI_LEAST_MEDIAN_SQUARE_MU_0,                             /**< 5: LMS mu = 0                                */
    CAPI_LEAST_MEDIAN_SQUARE_MU_18_H1FFFF,                     /**< 6: LMS mu = 18'h1FFFF                        */
    CAPI_LEAST_MEDIAN_SQUARE_MU_18_H2000                       /**< 7: LMS mu = 18'h2000                         */
} capi_least_median_square_t;                                  /**< Least Median Square type                     */



/**
 * The enumeration of mean square error
 */
typedef enum capi_mean_square_error_e {
    CAPI_MEAN_SQUARE_ERROR_2_POWER_OF_MINUS_9,                 /**< 0: MSE mu = 2^-9                             */
    CAPI_MEAN_SQUARE_ERROR_2_POWER_OF_MINUS_18,                /**< 1: MSE mu = 2^-18                            */
    CAPI_MEAN_SQUARE_ERROR_2_POWER_OF_MINUS_12                 /**< 2: MSE mu = 2^-12                            */
} capi_mean_square_error_t;                                    /**< Mean Square Error type                       */



/**
 * The enumeration of Rx bandwidth
 */
typedef enum capi_rx_bandwidth_e {
    CAPI_RX_BANDWIDTH_0,                                        /**< Rx Bandwidth 0                               */
    CAPI_RX_BANDWIDTH_1,                                        /**< Rx Bandwidth 1                               */
    CAPI_RX_BANDWIDTH_2,                                        /**< Rx Bandwidth 2                               */
    CAPI_RX_BANDWIDTH_3,                                        /**< Rx Bandwidth 3                               */
    CAPI_RX_BANDWIDTH_4,                                        /**< Rx Bandwidth 4                               */
    CAPI_RX_BANDWIDTH_5,                                        /**< Rx Bandwidth 5                               */
    CAPI_RX_BANDWIDTH_6,                                        /**< Rx Bandwidth 6                               */
    CAPI_RX_BANDWIDTH_7,                                        /**< Rx Bandwidth 7                               */
    CAPI_RX_BANDWIDTH_8,                                        /**< Rx Bandwidth 8                               */
    CAPI_RX_BANDWIDTH_9,                                        /**< Rx Bandwidth 9,                              */
    CAPI_RX_BANDWIDTH_10,                                       /**< Rx Bandwidth 10,                             */
    CAPI_RX_BANDWIDTH_11,                                       /**< Rx Bandwidth 11,                             */
    CAPI_RX_BANDWIDTH_12,                                       /**< Rx Bandwidth 12,                             */
    CAPI_RX_BANDWIDTH_13,                                       /**< Rx Bandwidth 13,                             */
    CAPI_RX_BANDWIDTH_14,                                       /**< Rx Bandwidth 14,                             */
    CAPI_RX_BANDWIDTH_15,                                       /**< Rx Bandwidth 15,                             */
} capi_rx_bandwidth_t;                                          /**< Rx Bandwidth type                            */


/**
 * The structure of lane rx info
 */
typedef struct lane_rx_info_s {
    union {
        uint32_t content;

        struct {
            #ifdef CAPI_IS_BIG_ENDIAN
            uint32_t dsp_gain3_on                    : 1; /**< dsp : gain3_on                                                */
            uint32_t dsp_eq_hpwr_o                   : 1; /**< dsp: FFE high power option                                    */
            uint32_t media_type                      : 1; /**< media type    : refer to capi_media_type_t                    */
            uint32_t symbol_swap                     : 1; /**< symbol swap   : refer tp capi_symbol_swap_t                   */
            uint32_t vga                             : 1; /**< vga                                                           */
            uint32_t peaking_filter_info             : 1; /**< peaking_filter_info (Serdes: just get(), DSP: get()/set())    */
            uint32_t eq2_info                        : 1; /**< EQ2 Information                                               */
            uint32_t serdes_lp_has_prec_en           : 1; /**< serdes : lp has prec en                                       */
            uint32_t serdes_unreliable_los           : 1; /**< serdes : unreliable los   : Get() Not Supported               */
            uint32_t serdes_scrambling_off           : 1; /**< serdes : scrambling off   : Get() Not Supported               */
            uint32_t serdes_db_loss                  : 1; /**< serdes : db loss                                              */
            uint32_t serdes_pam_er_nr                : 1; /**< serdes : pam_er_nr                                            */
            uint32_t dsp_graycode                    : 1; /**< dsp : graycode                                                */
            uint32_t dsp_dc_wander_mu                : 1; /**< dsp : dc_wander_mu                                            */
            uint32_t dsp_gain_boost                  : 1; /**< dsp : gain_boost                                              */
            uint32_t dsp_low_bw_en                   : 1; /**< dsp : Low Bandwidth enable                                    */
            uint32_t dsp_gain2_on                    : 1; /**< dsp : gain2_on                                                */
            uint32_t dsp_lms_mode                    : 1; /**< dsp : lms mode                                                */
            uint32_t dsp_autopeaking_en              : 1; /**< dsp : autopeaking_en                                          */
            uint32_t dsp_precode                     : 1; /**< dsp : Rx precode                                              */
            uint32_t dsp_los_th_idx                  : 1; /**< dsp : los_th_idx                                              */
            uint32_t dsp_inlos_th_idx                : 1; /**< dsp : inlos_th_idx                                            */
            uint32_t dsp_outlos_th_idx               : 1; /**< dsp : outlos_th_idx                                           */
            uint32_t dsp_elos_ignore                 : 1; /**< dsp : elos_ignore                                             */
            uint32_t dsp_oplos_ignore                : 1; /**< dsp : oplos_ignore                                            */
            uint32_t dsp_eq_tap_sel                  : 1; /**< dsp : equalizer tap select                                    */
            uint32_t dsp_ffe_slicer_multiplier       : 1; /**< dsp : ffe slicer multiplier                                   */
            uint32_t dsp_phase_bias_auto_tuning_info : 1; /**< dsp : phase bias auto tuning info                             */
            uint32_t dsp_kp_kf_info                  : 1; /**< dsp : kp_kf_info                                              */
            uint32_t dsp_los_info                    : 1; /**< dsp : los info                                                */
            uint32_t dsp_nldet_info                  : 1; /**< dsp : nldet_info                                              */
            uint32_t dsp_rx_bw_value                 : 1; /**< dsp : rx bw value                                             */
            #else  /* CAPI_IS_LITTLE_ENDIAN */
            uint32_t dsp_rx_bw_value                 : 1; /**< dsp : rx bw value                                             */
            uint32_t dsp_nldet_info                  : 1; /**< dsp : nldet info                                              */
            uint32_t dsp_los_info                    : 1; /**< dsp : los info                                                */
            uint32_t dsp_kp_kf_info                  : 1; /**< dsp : kp_kf_info                                              */
            uint32_t dsp_phase_bias_auto_tuning_info : 1; /**< dsp : phase bias auto tuning info                             */
            uint32_t dsp_ffe_slicer_multiplier       : 1; /**< dsp : ffe slicer multiplier                                   */
            uint32_t dsp_eq_tap_sel                  : 1; /**< dsp : equalizer tap select                                    */
            uint32_t dsp_oplos_ignore                : 1; /**< dsp : oplos_ignore                                            */
            uint32_t dsp_elos_ignore                 : 1; /**< dsp : elos_ignore                                             */
            uint32_t dsp_outlos_th_idx               : 1; /**< dsp : outlos_th_idx                                           */
            uint32_t dsp_inlos_th_idx                : 1; /**< dsp : inlos_th_idx                                            */
            uint32_t dsp_los_th_idx                  : 1; /**< dsp : los_th_idx                                              */
            uint32_t dsp_precode                     : 1; /**< dsp : Rx precode                                              */
            uint32_t dsp_autopeaking_en              : 1; /**< dsp : autopeaking_en                                          */
            uint32_t dsp_lms_mode                    : 1; /**< dsp : lms mode                                                */
            uint32_t dsp_gain2_on                    : 1; /**< dsp : gain2_on                                                */
            uint32_t dsp_low_bw_en                   : 1; /**< dsp : Low Bandwidth enable                                    */
            uint32_t dsp_gain_boost                  : 1; /**< dsp : gain_boost                                              */
            uint32_t dsp_dc_wander_mu                : 1; /**< dsp : dc_wander_mu                                            */
            uint32_t dsp_graycode                    : 1; /**< dsp : graycode                                                */
            uint32_t serdes_pam_er_nr                : 1; /**< serdes : pam_er_nr                                            */
            uint32_t serdes_db_loss                  : 1; /**< serdes : db loss                                              */
            uint32_t serdes_scrambling_off           : 1; /**< serdes : scrambling off   : Get() Not Supported               */
            uint32_t serdes_unreliable_los           : 1; /**< serdes : unreliable los   : Get() Not Supported               */
            uint32_t serdes_lp_has_prec_en           : 1; /**< serdes : lp has prec en                                       */
            uint32_t eq2_info                        : 1; /**< EQ2 Information                                               */
            uint32_t peaking_filter_info             : 1; /**< peaking_filter_info (Serdes: just get(), DSP: get()/set())    */
            uint32_t vga                             : 1; /**< vga                                                           */
            uint32_t symbol_swap                     : 1; /**< symbol swap    : refer tp capi_symbol_swap_t                  */
            uint32_t media_type                      : 1; /**< media type     : refer to capi_media_type_t                   */
            uint32_t dsp_eq_hpwr_o                   : 1; /**< dsp: FFE high power option                                    */
            uint32_t dsp_gain3_on                    : 1; /**< dsp : gain3_on                                                */
            #endif /* CAPI_IS_BIG_ENDIAN */
        } is;
    } param;

    struct {
        /**< The following parameters are commonly used for SERDES and DSP */
        uint8_t media_type;                  /**< Media Type. 0: optical, 1: PCB trace or backplane, 2: copper                                    */
        int8_t  symbol_swap;                 /**< symbol_swap:0- Do not swap; 1- swap the MSB and LSB : refer to capi_symbol_swap_t               */
        uint8_t vga;                         /**< VGA : SERDES (Read Only): range[0, 37].   DSP range [0,31]                                      */

        struct {
            int8_t oven;                     /**< peaking value override_en;0-override disabled; 1-oval is valid                                  */
            int8_t oval;                     /**< peaking value in Binary coded, SERDES side is read only, DSP can set and get, DSP range [0,31]  */
            int8_t low_freq;                 /**< read only, SERDES only                                                                          */
            int8_t high_freq;                /**< read only, SERDES only                                                                          */
        } peaking_filter_info;               /**< peaking_filter_info                                                                             */

        struct {
            uint8_t eq2_on;                  /**< Enable EQ2. 0 : disable EQ2, 1 : enable EQ2                                                     */
            uint8_t num_of_eq2_taps;         /**< Number of elements in EQ2 array, read only, SERDES currently support 4                          */
            int8_t  eq2[CAPI_NUM_DFE_TAPS];  /**< EQ2 taps : SERDES (Read Only)                                                                   */
            uint8_t low_power_eq2_on;        /**< Enable low power EQ2. 0 : EQ2 operates in normal power mode, 1 : EQ2 operates in low power mode */
            uint8_t dsp_eq2_adapt_off;       /**< DSP : Disable EQ2 adaptation. 0 : disable EQ2 adaptation, 1 : enable EQ2 adaptation             */
            uint8_t dsp_force_baud_rate_eq2; /**< DSP : Force Baud rate EQ2, not used.                                                            */
            uint8_t dsp_eq2_type;            /**< DSP : EQ2 type. 0-type 1a; 1-type 10a  : refer to capi_equalizer2_t                             */
        } eq2_info;                          /**< DSP : EQ2 information                                                                           */

        union {
            /**< The following parameters are only used for SERDES */
            struct {
                uint8_t lp_has_prec_en;     /**< SERDES : Link partner TX has precoder enabled (non-LT mode): refer to capi_enable_t                                    */
                                            /**<          0: LP precoder disabled, 1: LP precoder enabled, Client only                                                  */
                uint8_t unreliable_los;     /**< SERDES : Unreliable LOS. 0 : LOS is reliable, 1 : LOS cannot be trusted,                                               */
                                            /**<          for optical modules with missing optical LOS                                                                  */
                uint8_t scrambling_off;     /**< SERDES : Not used. 0: RX input is scrambled, 1: RX input may have repeating data patterns. 
                                                          refer to : capi_optical_los_reliability_t                                                                     */
                uint8_t pam_er_nr;          /**< SERDES : Rx Normal or Extended Slicer Mode; 0: Slicer auto select; 1: Normal Slicer, 2: Extended Slicer.   Default: 0  */
                                            /**<          refer to: capi_pam_slicer_t                                                                                   */
                                            /**<          force_es: force Rx to use ES mode.  force_ns: force Rx to use NS mode                                         */
                                            /**<          Force Rx to use ES mode (longer channels). This will be in effect even if link                                */
                                            /**<          training is used. Cannot be set at same time as forced Normal Slicer.                                         */
                                            /**<          During Link Training; force RX to use NS mode (short channel). This will be in effect                         */
                                            /**<          even if link training is used. Cannot be set at same time as forced Extended Slicer                           */
                uint8_t db_loss;            /**< SERDES : Estimate of the channel loss in DB, Valid range 0-35, Client side                                             */
                                            /**<          Refer to : capi_channel_los_t                                                                                 */
                                            /**<          0 is default and forces microcode to figure out best optimization.                                            */
                                            /**<          VSR Channel 1-10                                                                                              */
                                            /**<          MR Channel  11-20                                                                                             */
                                            /**<          LR Channel  20+                                                                                               */
             } serdes;

            /**< The following parameters are only used for DSP */
            struct {
                int8_t  graycode;                        /**< DSP: gray code enable: 1-enable; 0-disable                                                                */
                int8_t  dc_wander_mu;                    /**< DSP: 3'b000:  mu = 2^-123'b001:  mu = 2^-113'b010:  mu = 2^-103'b011:  mu = 2^-93'b100:  mu = 2^-8        */
                int8_t  gain_boost;                      /**< DSP: Coarse PGA gain boost code to provide addition gain up to 6dB, Binary coded                          */
                int8_t  low_bw_en;                       /**< DSP: Media side only; 0 - default (regular bandiwdth) 1 - low bandwidth                                   */
                uint8_t gain2_on;                        /**< DSP: Enable GAIN2. 0 : disable GAIN2, 1: enable gain2; 2: enable half-gain2                                                    */
                uint8_t lms_mode;                        /**< DSP: LMS mode. 0: 1 LMS, 1: 8 LMS                                                                         */
                uint8_t autopeaking_en;                  /**< DSP: 0: disable auto peaking algorithm in copper mode link training;
                                                                   1: enable auto peaking algorithm in copper mode link training                                        */
                int8_t  precode;                         /**< DSP: 0: firmware use default(refer to dsp_precoder)
                                                                   1: precode force off
                                                                   2: precode force on                                                                                  */
                uint8_t los_th_idx;                      /**< DSP: [0, 9] line side LOS threshold mapping index                                                         */
                uint8_t inlos_th_idx;                    /**< DSP: [0, 9] line side INLOS threshold mapping index                                                       */
                uint8_t outlos_th_idx;                   /**< DSP: [0, 9] line side OUTLOS threshold mapping index                                                      */
                uint8_t elos_ignore;                     /**< DSP: [0, 1] line side ignore ELOS no matter electrical LOS or optical LOS                                 */
                uint8_t oplos_ignore;                    /**< DSP: [0, 1] line side ignore OPLOS no matter electrical LOS or optical LOS                                */
                int8_t  eq_tap_sel;                      /**< DSP: FFE power:
                                                                       0: firmware default setting; 
                                                                       1: high power; 
                                                                       2: medium power
                                                                       3: low power                                                                                     */
                int8_t  ffe_slicer_multiplier;           /**< DSP: ffe_slicer_multiplier:0: *1; 1 : *2;                                                                 */

                struct {
                    int8_t phase_auto_tune_en;           /**< DSP: enable/disable phase auto tuning algorithm [static tuning]                                           */
                    int8_t dynamic_phase_auto_tune_en;   /**< DSP: line side enable dynamic auto phase tuning,                                                          */
                    int8_t max_phase_bias_th;            /**< DSP: line side maximum phase bias threshold, los_th_idx. max_bias_th valid range is [-32~31]              */
                    int8_t min_phase_bias_th;            /**< DSP: line side minimum phase bias threshold, los_th_idx. max_bias_th valid range is [-32~31]              */
                    int8_t phase_tune_step_size;         /**< DSP: static phase tuning step size
                                                                   Note: minimum and maximum threshold should be even number if step size is double step. 
                                                                   0: single step
                                                                   1: double step                                                                                       */
                    int8_t phase_tune_dwell_time;        /**< DSP: phase tuning dwell time each tuning step
                                                                   0: 10 ms, 1: 15ms, 2: 20 ms, 3: 25ms, 4: 30ms                                                        */

 
                    int8_t phase_dtune_bias_range;       /**< DSP: phase bias range of dynamic tuning. default is 4, [0, 10]                                            */
                    int8_t phase_dtune_snr_change_th;    /**< DSP: SNR change threshold parameter for dynamic tuning. default is 3(0.1db), [0, 10]. (0.025, 0.05, 0.075, 0.1....)     */
                    int8_t phase_tune_link_down_snr;     /**< DSP: phase static tuning link down SNR threshold, 0: 12db, 1: 13db, 2: 14db (default) ... 7: 19db         */
                                                                   
                } phase_bias_auto_tuning_info;           /**< DSP: phase auto tuning config                                                                             */

                struct {
                    int8_t kp_tracking;                  /**< DSP: kp parameters applicable after link up            */
                    int8_t kf_tracking;                  /**< DSP: kf parameters applicable after link up            */
                    int8_t kp;                           /**< DSP: kp parameters applicable during link              */
                    int8_t kf;                           /**< DSP: kf parameters applicable during link              */
                    int8_t kp_hlf_stp;                   /**< DSP: kp half step parameters applicable during link    */
                    int8_t kp_tracking_hlf_stp;          /**< DSP: kp half step parameters applicable after link up  */
                } kp_kf_info;                            /**< DSP: kp kg parameters, LW only                         */

                struct {
                    int8_t los_th_idx;                   /**< DSP: line side LOS threshold mapping index                */
                    int8_t los_bypass;                   /**< DSP: 0 - LOS bypass disabled 1 - LOS bypass enabled       */
                    int8_t los_bypass_val;               /**< DSP: LOS value if LOS bypass enabled  0 - No LOS, 1 - LOS */
                } los_info;                              /**< DSP: LOS config, LW only                                  */

                struct {
                    uint8_t nldet_en;                   /**< DSP: [0,1] enable/disable Non-Linear Detection in PAM4 mode                                                                                             */
                    int8_t  dsel;                       /**< DSP: [0,1] 0 => FIR uses 1st & 2nd samples, 1 => FIR uses 2nd & 3rd samples.                                                                            */
                    int8_t  casel;                      /**< DSP: [0~8] 0 => FIR a = 0, 1 => FIR a = 1/16, 2 => FIR a = 2/16 = 1/8, 3 => FIR a = 3/16, 4 => FIR a = 4/16 = 1/4, ... , 8 = 8/16 = 1/2                 */
                    int8_t  lms_mu;                     /**< DSP: [0,7] 0 => LMS mu = 2^-11, 1 => mu = 2^-9,  2 => mu = 2^-7,  3 => mu = 2^-5, 4 => Reserved, 5 => mu = 0, 6 => mu = 18'h1FFFF, 7 => mu = 18'h2000   */
                    int8_t  mse_mu;                     /**< DSP: [0,2] 0 => MSE mu = 2^-9,  1 => mu = 2^-18, 2 => mu = 2^-12..                                                                                      */
                } nldet_info;                           /**< DSP: NLDET config                                                                                                                                       */

                uint8_t rx_bw_value;                    /**< DSP: RX bandwidth value  0~15 in line side; 0~63 in host side                                                                                   */
                uint8_t dsp_eq_hpwr_option;             /**< DSP: in high FFE power, maintap location option: 0-default (location 5), 1-location 6                           */ 
                uint8_t gain3_on;                        /**< DSP: Enable GAIN3. 0 : disable, 1 : enable; 2: enable half gain3                                                    */
            } dsp;
        } core_ip;
    } value;
} lane_rx_info_t;                                        /**< Lane Rx Info type                                                                                                                                      */

/**
 * The structure of line side link training cfg for 802.3cd
 */
typedef struct lane_lnktrn_802_3cd_cfg_s {
    uint8_t lnktrn_frm_size;                             /**< Line side link training frame size, this is only valid for 802.3cd rather than CL72 & CL93 */
    uint8_t lnktrn_init_cond;                            /**< Line side link training init condition type for 802.3cd */
} lane_lnktrn_802_3cd_cfg_t;                             /**< Line Side Link Training Config information for 802.3cd type */

/**
 * The structure of line side link training cfg for CL72 and CL93
 */
typedef struct lane_lnktrn_cl72_cl93_cfg_s {
    uint8_t lnktrn_cl_init;                              /**< Line side link training init condition type for CL72 & CL93 */
    uint8_t lnktrn_cl_preset;                            /**< Line side link training preset type for CL72 & CL93 */
} lane_lnktrn_cl72_cl93_cfg_t;

/**
 * A structure to represent the link training timer
 */
typedef struct lane_link_training_timer_s {
    uint8_t   disable_lt_timeout;                        /**< CAPI_ENABLE: disable LT timer; CAPI_DISABLE: enable LT timer (in default) */
    uint8_t   lt_timer_duration;                         /**< LT Timer Duration which only is valid when  disable_lt_timeout=CAPI_DISABLE */
} lane_link_training_timer_t;                            /**< Link Training Timer type */

/**
 * A structure to represent the link training information
 */
typedef struct lane_lnktrn_info_s {
    union {
        uint16_t content;
        struct {
            #ifdef CAPI_IS_BIG_ENDIAN
            uint16_t reserved                         : 2;  /**< Reservedv                            */
            uint16_t opposite_cdr_first               : 1;  /**< opposite_cdr_first                   */
            uint16_t lnktrn_en                        : 1;  /**< lnktrn_en                            */
            uint16_t serdes_lnktrn_restart_timeout    : 1;  /**< SERDES : lnktrn_restart_timeout      */
            uint16_t serdes_lnktrn_type               : 1;  /**< SERDES : lnktrn_type : Read Only     */
            uint16_t dsp_linktrn_timer                : 1;  /**< DSP : linktrn_timer                  */
            uint16_t dsp_lnktrn_type                  : 1;  /**< DSP : lnktrn_type : Read Only        */
            uint16_t dsp_restart                      : 1;  /**< DSP : restart                        */
            uint16_t dsp_auto_restart                 : 1;  /**< DSP : Auto restart                   */
            uint16_t dsp_disable_lt_rxflt_restart_tx  : 1;  /**< DSP : disable_lt_rxflt_restart_tx    */
            uint16_t dsp_disable_lt_rxlos_restart_tx  : 1;  /**< DSP : disable_lt_rxlos_restart_tx    */
            uint16_t dsp_cl72_auto_pol_en             : 1;  /**< DSP : cl72_auto_pol_en               */
            uint16_t dsp_enable_cl72_cl93_preset_req  : 1;  /**< DSP : enable_cl72_cl93_preset_req    */
            uint16_t dsp_enable_802_3cd_preset_req    : 1;  /**< DSP : enable_802_3cd_preset_req      */
            uint16_t dsp_lnktrn_cfg                   : 1;  /**< DSP : lnktrn_cfg                     */
            #else  /* CAPI_IS_LITTLE_ENDIAN */
            uint16_t dsp_lnktrn_cfg                   : 1;  /**< DSP : lnktrn_cfg                     */
            uint16_t dsp_enable_802_3cd_preset_req    : 1;  /**< DSP : enable_802_3cd_preset_req      */
            uint16_t dsp_enable_cl72_cl93_preset_req  : 1;  /**< DSP : enable_cl72_cl93_preset_req    */
            uint16_t dsp_cl72_auto_pol_en             : 1;  /**< DSP : cl72_auto_pol_en               */
            uint16_t dsp_disable_lt_rxlos_restart_tx  : 1;  /**< DSP : disable_lt_rxlos_restart_tx    */
            uint16_t dsp_disable_lt_rxflt_restart_tx  : 1;  /**< DSP : disable_lt_rxflt_restart_tx    */
            uint16_t dsp_auto_restart                 : 1;  /**< DSP : Auto restart                   */
            uint16_t dsp_restart                      : 1;  /**< DSP : restart                        */
            uint16_t dsp_lnktrn_type                  : 1;  /**< DSP : lnktrn_type : Read Only        */
            uint16_t dsp_linktrn_timer                : 1;  /**< DSP : linktrn_timer                  */
            uint16_t serdes_lnktrn_type               : 1;  /**< SERDES : lnktrn_type                 */
            uint16_t serdes_lnktrn_restart_timeout    : 1;  /**< SERDES : lnktrn_restart_timeout      */
            uint16_t lnktrn_en                        : 1;  /**< lnktrn_en                            */
            uint16_t opposite_cdr_first               : 1;  /**< opposite_cdr_first                   */
            uint16_t reserved                         : 2;  /**< Reserved                             */
            #endif /* CAPI_IS_BIG_ENDIAN */
        } is;
    } param;

    uint8_t lnktrn_en;                                      /**< link training enable/disable                                        */
    uint8_t opposite_cdr_first;                             /**< 0: start link training firstly, then check opposite cdr_lock status
                                                                 1: wait for opposite cdr_lock is ready before link training start   */
    union {
        struct {
            uint8_t lnktrn_restart_timeout;                 /**< SERDES : Restart side link training after timeout                   */
            uint8_t lnktrn_type;                            /**< SERDES : link training type, refer to capi_lnktrn_type_t            */
        } serdes;

        struct {
            lane_link_training_timer_t linktrn_timer;       /**< DSP : Line side link training timer                                 */
            uint8_t lnktrn_type;                            /**< DSP : link training type                                            */
            uint8_t linktrn_restart_timeout_en;             /**< DSP : Restart side link training after timeout                      */
            uint8_t restart;                                /**< DSP : Link training restart                                         */
            uint8_t auto_restart;                           /**< DSP : link training auto-restart                                    */
            uint8_t disable_lt_rxflt_restart_tx;            /**< DSP : 0: LT RX fault to restart tx in training procedure; 
                                                                       1: disable LT RX fault to restart tx in training procedure,   */
            uint8_t disable_lt_rxlos_restart_tx;            /**< DSP : 0: LT RX los to restart tx in training procedure; 
                                                                       1: disable LT RX los to restart tx in training procedure      */
            uint8_t cl72_auto_pol_en;                       /**< DSP : CL72 auto polarity selection (not used). 
                                                                       0: no auto polarity selection
                                                                       1: toggle polarity if no framelock within 1ms during CL72. TODO: remove it */
            uint8_t enable_cl72_cl93_preset_req;            /**< DSP : 0: disable LT RX send cl72/cl93 preset request (non-IEEE compatible mode); 
                                                                       1: enable LT RX send cl72/cl93 preset request (IEEE compatible mode, default value) */
            uint8_t enable_802_3cd_preset_req;              /**< DSP : 0: disable LT RX send 802.3cd preset1 request (non-IEEE compatible mode); 
                                                                       1: enable LT RX send 802.3cd preset1 request (IEEE compatible mode, default value)  */
            union {
                lane_lnktrn_802_3cd_cfg_t pam_802_3;        /**< DSP : Line side link training cfg for 802.3cd                       */
                lane_lnktrn_cl72_cl93_cfg_t nrz_cl;         /**< DSP : Line side link training cfg for CL72 & CL93                   */
            } lnktrn_cfg;
        } dsp;
    } value;
} lane_lnktrn_info_t;

typedef lane_lnktrn_info_t capi_lnktrn_info_t;

typedef enum {
    CAPI_AUTO_NEG_MODE_INCOMPATIBLE                  = 0,     /**< Auto Neg Mode Incompatible                    */
    CAPI_AUTO_NEG_MODE_IEEE_10G_KR                   = 1,     /**< Auto Neg Mode IEEE 10G KR                     */
    CAPI_AUTO_NEG_MODE_IEEE_40G_KR4                  = 2,     /**< Auto Neg Mode IEEE 40G KR4                    */
    CAPI_AUTO_NEG_MODE_IEEE_40G_CR4                  = 3,     /**< Auto Neg Mode IEEE 40G CR4                    */
    CAPI_AUTO_NEG_MODE_IEEE_100G_KR4                 = 4,     /**< Auto Neg Mode IEEE 100G KR4                   */
    CAPI_AUTO_NEG_MODE_IEEE_100G_CR4                 = 5,     /**< Auto Neg Mode IEEE 100G CR4                   */
    CAPI_AUTO_NEG_MODE_IEEE_25G_CRSKRS               = 6,     /**< Auto Neg Mode IEEE 25G CRSKRS                 */
    CAPI_AUTO_NEG_MODE_IEEE_25G_CR1KR1               = 7,     /**< Auto Neg Mode IEEE 25G CR1KR1                 */
    CAPI_AUTO_NEG_MODE_IEEE_50GKRCR                  = 8,     /**< Auto Neg Mode IEEE 50G KRCR                   */
    CAPI_AUTO_NEG_MODE_IEEE_100GKR2CR2               = 9,     /**< Auto Neg Mode IEEE 100G KR2CR2                */
    CAPI_AUTO_NEG_MODE_IEEE_200GKR4CR4               = 10,    /**< Auto Neg Mode IEEE 200G KR4CR4                */
    CAPI_AUTO_NEG_MODE_BAM_50G_BRCM_FEC_544_CR2_KR2  = 11,    /**< Auto Neg Mode BAM 50G  BRCM FEC 544 CR2-KR2   */
    CAPI_AUTO_NEG_MODE_BAM_200G_BRCM_KR4_CR4         = 12,    /**< Auto Neg Mode BAM 200G BRCM KR4 CR4           */
    CAPI_AUTO_NEG_MODE_BAM_100G_BRCM_KR4_CR4         = 13,    /**< Auto Neg Mode BAM 100G BRCM KR4 CR4           */
    CAPI_AUTO_NEG_MODE_BAM_100G_BRCM_FEC_528_KR2_CR2 = 14,    /**< Auto Neg Mode BAM 100G BRCM FEC 528 KR2-CR2   */
    CAPI_AUTO_NEG_MODE_BAM_100G_BRCM_NO_FEC_KR2_CR2  = 15,    /**< Auto Neg Mode BAM 100G BRCM NO_FEC KR2-CR2    */
    CAPI_AUTO_NEG_MODE_BAM_100G_BRCM_NO_FEC_X4       = 16,    /**< Auto Neg Mode BAM 100G BRCM NO_FEC X4         */
    CAPI_AUTO_NEG_MODE_C50G_25G_CR1KR1               = 17,    /**< Auto Neg Mode C50G 25G CR1KR1                 */
    CAPI_AUTO_NEG_MODE_C50G_50G_CR2KR2               = 18,    /**< Auto Neg Mode C50G 50G CR2KR2                 */
    CAPI_AUTO_NEG_MODE_C50G_50G_X8                   = 19     /**< Auto Neg Mode C50G 50G X8                     */
} capi_auto_neg_mode_t;                                       /**< Auto Neg Mode type                            */

/*
 * Lane auto-neg structure
 */
typedef struct lane_an_info_s {
    union {
        uint16_t content;
        struct {
            #ifdef CAPI_IS_BIG_ENDIAN
            uint16_t reserved                    : 4; /**< Reservedv                                 */
            uint16_t auto_neg_on                 : 1; /**< Autoneg                                   */
            uint16_t an_req                      : 1; /**< Auto-negotiation request                  */
            uint16_t an_fec                      : 1; /**< Auto-negotiation FEC ability              */
            uint16_t an_opposite_side_cdrlock    : 1; /**< opposite side cdr lock                    */
            uint16_t an_master_lane              : 1; /**< Line id for the master lane               */
            uint16_t an_only_endis               : 1; /**< AN only                                   */
            uint16_t linkt_training_dis          : 1; /**< Link training disable                     */
            uint16_t pause                       : 1; /**< Pause                                     */
            uint16_t other_die_line_lanemask     : 1; /**< multi-die port other die line lanemask    */
            uint16_t other_die_client_lanemask   : 1; /**< multi-die port other die client lanemask  */
            uint16_t die_no                      : 1; /**< Die number                                */
            uint16_t an_6s_timer                 : 1; /**< AN inhibit timer                          */
            #else  /* CAPI_IS_LITTLE_ENDIAN */
            uint16_t an_6s_timer                 : 1; /**< AN inhibit timer                          */
            uint16_t die_no                      : 1; /**< Die number                                */
            uint16_t other_die_client_lanemask   : 1; /**< multi-die port other die client lanemask  */
            uint16_t other_die_line_lanemask     : 1; /**< multi-die port other die line lanemask    */
            uint16_t pause                       : 1; /**< Pause                                     */
            uint16_t linkt_training_dis          : 1; /**< Link training disable                     */
            uint16_t an_only_endis               : 1; /**< AN only                                   */
            uint16_t an_master_lane              : 1; /**< Line id for the master lane               */
            uint16_t an_opposite_side_cdrlock    : 1; /**< opposite side cdr lock                    */
            uint16_t an_fec                      : 1; /**< Auto-negotiation FEC ability              */
            uint16_t an_req                      : 1; /**< Auto-negotiation request                  */
            uint16_t auto_neg_on                 : 1; /**< Autoneg                                   */
            uint16_t reserved                    : 4; /**< Reservedv                                 */
            #endif /* CAPI_IS_BIG_ENDIAN */
        } is;
    } param;

    struct {
        uint8_t auto_neg_on;               /**< Autoneg                                                                     */
        uint8_t an_req;                    /**< Auto-negotiation request, refer to capi_lw_an_req_t                         */
        uint8_t an_fec;                    /**< Auto-negotiation FEC ability                                                */
        uint8_t an_opposite_side_cdrlock;  /**<1=cdr_lock, 0=cdr not locked                                                 */
        uint8_t an_master_lane;            /**<Line id for the master lane                                                  */
        uint8_t an_only_endis;             /**< 1- will not write the ability, enable/disable an based on auto_neg_on field */
        uint8_t linkt_training_dis;        /**< 0- AN+LT, 1-AN only                                                         */
        uint8_t pause;                     /**< pause, refer to capi_an73_pause_t                                           */
        uint8_t other_die_line_lanemask;   /**< multi-die port other die line lanemask                                      */
        uint8_t other_die_client_lanemask; /**< multi-die port other die client lanemask                                    */
        uint8_t die_no;                    /**< refter to capi_die_no_t                                                     */
        uint8_t an_6s_timer;               /**< 0:IEEE standard 3s link inhibit timer, 1:6s  link inhibit timer             */
    } value;
} lane_an_info_t;


/**
 * The structure of Lane Swap Information type
 */
typedef struct capi_lane_swap_info_s {
    uint32_t num_of_lanes;                 /**< Number of elements in lane_map_rx/tx arrays                       */
    uint8_t  rx_lane_list[8];              /**< rx_lane_list[Package/die (physical) Rx lane id] = schematic (logical)  Rx lane id  */ 
    uint8_t  tx_lane_list[8];              /**< tx_lane_list[Package/die (physical)  Tx lane id] = schematic (logical) Tx lane id  */
} lane_swap_info_t;                        /**< Lane Swap Information type                                        */


/**
 * Lane config type enumeration
 */
typedef enum lane_config_type_e {
    LANE_CONFIG_TYPE_NONE = 0,                           /**< Lane config type none                    */
    LANE_CONFIG_TYPE_LANE_RX_INFO,                       /**< Lane config type lane rx info            */
    LANE_CONFIG_TYPE_LANE_TX_INFO,                       /**< Lane config type lane tx info            */
    LANE_CONFIG_TYPE_LANE_LINK_TRAINING_INFO,            /**< Lane config type link training info      */
    LANE_CONFIG_TYPE_AUTO_NEG_INFO,                      /**< Lane config type auto neg info           */
    LANE_CONFIG_TYPE_LANE_SWAP_INFO                      /**< Lane config type lane swap info          */
} lane_config_type_t;                                    /**< Lane config type                         */

/**
 * The structure of command information
 */
typedef struct capi_lane_config_info_s {
    uint32_t                          lane_config_type;  /**< Lane config type, refer lane_config_type_t */
    union {
        lane_rx_info_t                lane_rx_info;      /**< Lane rx info                             */
        lane_tx_info_t                lane_tx_info;      /**< Lane tx info                             */
        lane_lnktrn_info_t            lane_lnktrn_info;  /**< Lane lnktrn info                         */
        lane_an_info_t                lane_an_info;      /**< Lane an info                             */
        lane_swap_info_t              lane_swap_info;    /**< Lane swap info                           */
    } type;
} capi_lane_config_info_t;                               /**< Lane config Info                         */


/**
 * capi lane control structure definition
 */
typedef struct capi_lane_ctrl_info_s {
    union {
        uint32_t content;                               /**< The command options                */

        struct {
            #ifdef CAPI_IS_BIG_ENDIAN
            uint32_t reserved                   : 17;          /**< reserved                            */
            uint32_t rx_datapath_power          : 1;           /**< Rx Data Path Power Switch           */
            uint32_t tx_datapath_power          : 1;           /**< Tx Data Path Power Switch           */
            uint32_t rx_lane_cfg_reset          : 1;           /**< Rx Reset Lane configuration         */
            uint32_t tx_lane_cfg_reset          : 1;           /**< Tx Reset Lane configuration         */
            uint32_t rx_squelch                 : 1;           /**< Rx path Squelch Switch              */
            uint32_t tx_squelch                 : 1;           /**< Tx path Squelch Switch              */
            uint32_t rx_data_path               : 1;           /**< Traffic Path: Off:0, On:1           */
            uint32_t tx_data_path               : 1;           /**< Traffic Path: Off:0, On:1           */
            /*uint32_t rx_electric_idle         : 1;*/         /**< Rx Electrical Idle                  */
            uint32_t tx_electric_idle           : 1;           /**< Tx Electrical Idle                  */
            uint32_t rx_suspend_resume          : 1;           /**< Rx Suspend Resume switch            */
            uint32_t tx_suspend_resume          : 1;           /**< Tx Suspend Resume switch            */
            uint32_t rx_afe_power               : 1;           /**< Rx AFE power switch                 */
            uint32_t tx_afe_power               : 1;           /**< Tx AFE power switch                 */
            uint32_t ignore_fault               : 1;           /**< Ignore Fault                        */
            uint32_t serdes_eye_margin_switch   : 1;           /**< serdes : eye_margin_switch                                    */
            #else  /* CAPI_IS_LITTLE_ENDIAN */
            uint32_t serdes_eye_margin_switch   : 1;           /**< serdes : eye_margin_switch                                    */
            uint32_t ignore_fault               : 1;           /**< Ignore Fault                        */
            uint32_t tx_afe_power               : 1;           /**< Tx AFE power switch                 */
            uint32_t rx_afe_power               : 1;           /**< Rx AFE power switch                 */
            uint32_t tx_suspend_resume          : 1;           /**< Tx Suspend Resume switch            */
            uint32_t rx_suspend_resume          : 1;           /**< Rx Suspend Resume switch            */
            uint32_t tx_electric_idle           : 1;           /**< Tx Electrical Idle                  */
            /*uint32_t rx_electric_idle         : 1;*/         /**< Rx Electrical Idle                  */
            uint32_t tx_data_path               : 1;           /**< Traffic Path: Off:0, On:1           */
            uint32_t rx_data_path               : 1;           /**< Traffic Path: Off:0, On:1           */
            uint32_t tx_squelch                 : 1;           /**< Tx path Squelch Switch              */
            uint32_t rx_squelch                 : 1;           /**< Rx path Squelch Switch              */
            uint32_t tx_lane_cfg_reset          : 1;           /**< Tx Reset Lane configuration         */
            uint32_t rx_lane_cfg_reset          : 1;           /**< Rx Reset Lane configuration         */
            uint32_t tx_datapath_power          : 1;           /**< Tx Data Path Power Switch           */
            uint32_t rx_datapath_power          : 1;           /**< Rx Data Path Power Switch           */
            uint32_t reserved                   : 17;          /**< reserved                            */
            #endif /* CAPI_IS_BIG_ENDIAN */
        } is;                                                  /**< Parameter is <name>                 */
    } param;                                                   /**< The lane control parameter          */

    union {
        uint32_t content;                                      /**< The command payload                 */

        struct {
            #ifdef CAPI_IS_BIG_ENDIAN
            uint32_t reserved           :13;     /**< Reserved for later use                                      */
            uint32_t rx_datapath_power  : 2;     /**< Rx Power: Off:0, On:1, Toggle:2                             */
            uint32_t tx_datapath_power  : 2;     /**< Tx Power: Off:0, On:1, Toggle:2                             */
            uint32_t rx_lane_cfg_reset  : 1;     /**< Rx Power: 1:reset, 0:no action                              */
            uint32_t tx_lane_cfg_reset  : 1;     /**< Tx Power: 1:reset, 0:no action                              */
            uint32_t rx_squelch         : 1;     /**< Rx Squelch: Off:0, On:1                                     */
            uint32_t tx_squelch         : 1;     /**< Tx Squelch: Off:0, On:1                                     */
            uint32_t rx_data_path       : 1;     /**< Traffic Path: Off:0, On:1                                   */
            uint32_t tx_data_path       : 1;     /**< Traffic Path: Off:0, On:1                                   */
            /*uint32_t rx_electric_idle : 1; */  /**< Rx Electrical Idle: Off:0, On:1                             */
            uint32_t tx_electric_idle   : 1;     /**< Tx Electrical Idle: Off:0, On:1                             */
            uint32_t rx_suspend_resume  : 1;     /**< Rx Suspend: 1 Resume: 0                                     */
            uint32_t tx_suspend_resume  : 1;     /**< Tx Suspend: 1 Resume: 0                                     */
            uint32_t rx_afe_power       : 2;     /**< Rx AFE power: Off:0, On:1, Toggle:2                         */
            uint32_t tx_afe_power       : 2;     /**< Tx AFE power: Off:0, On:1, Toggle:2                         */
            uint32_t ignore_fault       : 1;     /**< Ignore Fault: Off:0, On:1                                   */
            uint32_t eye_margin_switch  : 1;     /**< SERDES : Enable/disable serdes eye margin estimation in PAM */
                                                 /**<          mode. Default: 0 (disabled).  In order to get the  */
                                                 /**<          eye margin in get_dig_lane_status() the application*/
                                                 /**<          client shall turn on the eye margin switch and wait*/
                                                 /**<          1 second before calling capi_get_dig_lane_status() */
            #else  /* CAPI_IS_LITTLE_ENDIAN */
            uint32_t eye_margin_switch  : 1;     /**< SERDES : Enable/disable serdes eye margin estimation in PAM */
                                                 /**<          mode. Default: 0 (disabled).  In order to get the  */
                                                 /**<          eye margin in get_dig_lane_status() the application*/
                                                 /**<          client shall turn on the eye margin switch and wait*/
                                                 /**<          1 second before calling capi_get_dig_lane_status() */
            uint32_t ignore_fault       : 1;     /**< Ignore Fault: Off:0, On:1                                   */
            uint32_t tx_afe_power       : 2;     /**< Tx AFE power: Off:0, On:1, Toggle:2                         */
            uint32_t rx_afe_power       : 2;     /**< Rx AFE power: Off:0, On:1, Toggle:2                         */
            uint32_t tx_suspend_resume  : 1;     /**< Tx Suspend: 1 Resume: 0                                     */
            uint32_t rx_suspend_resume  : 1;     /**< Rx Suspend: 1 Resume: 0                                     */
            uint32_t tx_electric_idle   : 1;     /**< Tx Electrical Idle: Off:0, On:1                             */
            /*uint32_t rx_electric_idle : 1; */  /**< Rx Electrical Idle: Off:0, On:1                             */
            uint32_t tx_data_path       : 1;     /**< Traffic Path: Off:0, On:1                                   */
            uint32_t rx_data_path       : 1;     /**< Traffic Path: Off:0, On:1                                   */
            uint32_t tx_squelch         : 1;     /**< Tx Squelch: Off:0, On:1                                     */
            uint32_t rx_squelch         : 1;     /**< Rx Squelch: Off:0, On:1                                     */
            uint32_t tx_lane_cfg_reset  : 1;     /**< Tx Power: 1:reset, 0:no action                              */
            uint32_t rx_lane_cfg_reset  : 1;     /**< Rx Power: 1:reset, 0:no action                              */
            uint32_t tx_datapath_power  : 2;     /**< Tx Power: Off:0, On:1, Toggle:2                             */
            uint32_t rx_datapath_power  : 2;     /**< Rx Power: Off:0, On:1, Toggle:2                             */
            uint32_t reserved           :13;     /**< Reserved for later use                                      */
            #endif /* CAPI_IS_BIG_ENDIAN */
        } is;                                    /**< Parameter is <name>                                         */
    } cmd_value;                                 /**< The lane control command value                              */
} capi_lane_ctrl_info_t;                         /**< Lane Control Information type                               */
                    


/**
 * The structure of pmd info
 */
typedef struct pmd_info_s {
    uint32_t pll_lock;      /**< pll lock      */
    uint32_t cdr_lock;      /**< cdr lock      */
    uint32_t los;           /**< LOS           */
    uint32_t tx_squelch;    /**< tx_squelch    */
} capi_pmd_info_t;          /**< PMD Information type */


/**
 * The enumeration of loopback modes
 */
typedef enum loopback_mode_e {
    CAPI_GLOBAL_LOOPBACK_MODE,              /**< Global loopback mode */
    CAPI_REMOTE_LOOPBACK_MODE,              /**< Remote loopback mode */
    CAPI_LAST_LOOPBACK_MODE                 /**< Last loopback mode   */
} capi_loopback_mode_t;                     /**< Loopback Mode type   */

/**
 * The structure of loopback
 */
typedef struct capi_loopback_s {
    uint8_t    mode;                        /**< Loopback mode: refer to capi_loopback_mode_t */
    uint8_t    enable;                      /**< Loopback enable                              */
} capi_loopback_info_t;



/**
 * The enumeration for pattern generator and monitor types
 */
typedef enum capi_pattern_gen_mon_type_e {
    CAPI_PRBS_GENERATOR = 0x01,                   /**< PRBS Generator                          */
    CAPI_PRBS_MONITOR,                            /**< PRBS Monitor                            */
    CAPI_PRBS_GEN_MON,                            /**< PRBS Generator and Monitor              */
    CAPI_PRBS_SSPRQ_GENERATOR,                    /**< PRBS SSPRQ Generator                    */
    CAPI_PRBS_SSPRQ_MONITOR,                      /**< PRBS SSPRQ Monitor                      */
    CAPI_PRBS_SSPRQ_GEN_MON,                      /**< PRBS SSPRQ Generator and Monitor        */
    CAPI_PRBS_Q_PRBS_13_GENERATOR,                /**< PRBS Q PRBS 13 Generator                */
    CAPI_PRBS_SQUARE_WAVE_GENERATOR,              /**< PRBS SQUARE Wave Generator              */
    CAPI_PRBS_TX_LINEARITY_GENERATOR,             /**< PRBS Tx Linearity Generator             */
    CAPI_PRBS_SHARED_TX_PATTERN_GENERATOR,        /**< PRBS Shared Tx Generator                */
    CAPI_PRBS_KP4_HOST_GENERATOR,                 /**< PRBS KP4 Host Generator                 */
    CAPI_PRBS_KP4_HOST_MONITOR,                   /**< PRBS KP4 Host Monitor                   */
    CAPI_PRBS_KP4_HOST_GEN_MON,                   /**< PRBS KP4 Host Generator and Monitor     */
    CAPI_PRBS_KP4_MEDIA_GENERATOR,                /**< PRBS KP4 Media Generator                */
    CAPI_PRBS_KP4_MEDIA_MONITOR,                  /**< PRBS KP4 Media Monitor                  */
    CAPI_PRBS_KP4_MEDIA_GEN_MON,                  /**< PRBS KP4 Media Generator and Monitor    */
    CAPI_PRBS_JP03B,                            /**< PRBS Shared Tx Pattern     */
    CAPI_PRBS_STAIRCASE_PATTERN,                 /**< PRBS Shared Tx Pattern     */
    CAPI_PRBS_CW_GENERATOR,                       /**< PRBS CW Generator                       */
    CAPI_PRBS_CW_MONITOR,                         /**< PRBS CW Monitor                         */
    CAPI_PCS_SCRM_IDLE_GENERATOR                  /**< PCS scramble idle generator             */
} capi_pattern_gen_mon_type_t;                    /**< PRBS Pattern Generator Monitor type     */


/**
 * The enumeration of PRBS polynomial type
 */
typedef enum capi_bh_prbs_poly_e {
    CAPI_BH_PRBS_POLY_7 = 0,                    /**< BH PRBS Polynomial 7       */
    CAPI_BH_PRBS_POLY_9 = 1,                    /**< BH PRBS Polynomial 9       */
    CAPI_BH_PRBS_POLY_11 = 2,                   /**< BH PRBS Polynomial 11      */
    CAPI_BH_PRBS_POLY_15 = 3,                   /**< BH PRBS Polynomial 15      */
    CAPI_BH_PRBS_POLY_23 = 4,                   /**< BH PRBS Polynomial 23      */
    CAPI_BH_PRBS_POLY_31 = 5,                   /**< BH PRBS Polynomial 31      */
    CAPI_BH_PRBS_POLY_58 = 6,                   /**< BH PRBS Polynomial 58      */
    CAPI_BH_PRBS_POLY_49 = 7,                   /**< BH PRBS Polynomial 49      */
    CAPI_BH_PRBS_POLY_10 = 8,                   /**< BH PRBS Polynomial 10      */
    CAPI_BH_PRBS_POLY_20 = 9,                   /**< BH PRBS Polynomial 20      */
    CAPI_BH_PRBS_POLY_13 = 10,                  /**< BH PRBS Polynomial 13      */
    CAPI_BH_PRBS_USER_40_BIT_REPEAT = 15,       /**< BH PRBS User 40 BIT Repeat */
    CAPI_BH_PRBS_PRBS_AUTO_DETECT = 20,         /**< BH PRBS Auto Detect        */
    CAPI_BH_PCS_PRBS_7 = 100,                   /**< BH PCS PRBS Polynomial 7   */
    CAPI_BH_PCS_PRBS_15 = 101,                  /**< BH PCS PRBS Polynomial 15  */
    CAPI_BH_PCS_PRBS_23 = 102,                  /**< BH PCS PRBS Polynomial 23  */
    CAPI_BH_PCS_PRBS_31 = 103,                  /**< BH PCS PRBS Polynomial 31  */
    CAPI_BH_PRBS_PRBS_UNKNOWN = 255             /**< BH PRBS Unknown            */
} capi_bh_prbs_poly_t;                          /**< BH PRBS Polynomial type    */


/**
 * The enumeration of PRBS Checker Mode
 */
typedef enum capi_bh_prbs_checker_mode_e {
    CAPI_BH_PRBS_SELF_SYNC_HYSTERESIS = 0,
    CAPI_BH_PRBS_INITIAL_SEED_HYSTERESIS = 1,
    CAPI_BH_PRBS_INITIAL_SEED_NO_HYSTERESIS = 2
} capi_bh_prbs_checker_mode_t;

/**
 * The enumeration of FEC PRBS polynomial type
 */
typedef enum capi_fec_prbs_gen_poly_e {
    CAPI_FEC_PRBS_GEN_POLY_7  = 7,  /**< FEC PRBS GEN polynomial 7 */
    CAPI_FEC_PRBS_GEN_POLY_9  = 9,  /**< FEC PRBS GEN polynomial 9 */
    CAPI_FEC_PRBS_GEN_POLY_10 = 10, /**< FEC PRBS GEN polynomial 10 */
    CAPI_FEC_PRBS_GEN_POLY_11 = 11, /**< FEC PRBS GEN polynomial 11 */
    CAPI_FEC_PRBS_GEN_POLY_13 = 13, /**< FEC PRBS GEN polynomial 13 */
    CAPI_FEC_PRBS_GEN_POLY_15 = 15, /**< FEC PRBS GEN polynomial 15 */
    CAPI_FEC_PRBS_GEN_POLY_20 = 20, /**< FEC PRBS GEN polynomial 20 */
    CAPI_FEC_PRBS_GEN_POLY_23 = 23, /**< FEC PRBS GEN polynomial 23 */
    CAPI_FEC_PRBS_GEN_POLY_31 = 31, /**< FEC PRBS GEN polynomial 31 */
    CAPI_FEC_PRBS_GEN_POLY_49 = 49, /**< FEC PRBS GEN polynomial 49 */
    CAPI_FEC_PRBS_GEN_POLY_58 = 58, /**< FEC PRBS GEN polynomial 58 */
    CAPI_FEC_PRBS_GEN_POLY_COUNT    /**<FEC PRBS GEN polynomial count */
} capi_fec_prbs_gen_poly_t;


/**
 * The enumeration of CW PRBS polynomial type
 */
typedef enum capi_prbs_poly_type_e {
    CAPI_PRBS_POLY_7  = 7,  /**< PRBS polynomial 7 */
    CAPI_PRBS_POLY_9  = 9,  /**< PRBS polynomial 9 */
    CAPI_PRBS_POLY_10 = 10, /**< PRBS polynomial 10 */
    CAPI_PRBS_POLY_11 = 11, /**< PRBS polynomial 11 */
    CAPI_PRBS_POLY_13 = 13, /**< PRBS polynomial 13 */
    CAPI_PRBS_POLY_15 = 15, /**< PRBS polynomial 15 */
    CAPI_PRBS_POLY_20 = 20, /**< PRBS polynomial 20 */
    CAPI_PRBS_POLY_23 = 23, /**< PRBS polynomial 23 */
    CAPI_PRBS_POLY_31 = 31, /**< PRBS polynomial 31 */
    CAPI_PRBS_POLY_49 = 49, /**< PRBS polynomial 49 */
    CAPI_PRBS_POLY_58 = 58, /**< PRBS polynomial 58 */
    CAPI_PRBS_POLY_COUNT    /**< PRBS polynomial count */
} capi_prbs_poly_type_t;

/**
 * The structure of PRBS polynomial types
 */
typedef struct capi_prbs_poly_s {
    uint8_t poly_type;              /**< PRBS polynomial: capi_prbs_poly_t     */
    uint8_t bh_poly;                /**< BH PRBS polynomial: refer capi_bh_prbs_poly_t         */
    uint8_t fec_pgen_poly;          /**< FEC PRBS GEN polynomial: capi_fec_prbs_gen_poly_t     */
} capi_prbs_poly_t;                 /**< PRBS poly type                                        */


/**
 * The Enumeratio for Square Wave Pattern Mode
 */
typedef enum square_wave_pattern_e
{
    ONE_1_CONSECUTIVE  = 0,                 /**<  1 consecutive 1s        */
    ONE_2_CONSECUTIVE  = 1,                 /**<  2 consecutive 1s        */
    ONE_4_CONSECUTIVE  = 2,                 /**<  4 consecutive 1s        */
    ONE_8_CONSECUTIVE  = 3,                 /**<  8 consecutive 1s        */
    ONE_16_CONSECUTIVE = 4,                  /**< 16 consecutive 1s        */
    SQR_WAVE_PATTEN_MAX
} square_wave_pattern_t;                    /**< Square Wave Pattern type */


/**
 * The structure of PRBS configuration
 */
typedef struct capi_prbs_cfg_s {
    capi_prbs_poly_t            poly;               /**< polynomial                                      */
    uint32_t                    bh_checker_mode;    /**< checker mode: refer capi_bh_prbs_checker_mode_t */
    uint32_t                    rx_invert;          /**< RX Invert                                       */
    uint32_t                    tx_invert;          /**< TX Invert                                       */
} capi_prbs_cfg_t;                                  /**< PRBS configuration type                         */

/**
 * The structure SSPRQ pattern type
 */
typedef struct capi_prbs_ssprq_s {
    uint8_t   modulation;       /**< Modulation Mode;   1:Normal/Full for PAM4 mode  /  0:NRZ mode       */
    uint8_t   gray_code;        /**< Gray Code;         0:Do Gray Code  /  1:Do Not Gray Code            */
    uint8_t   bit_swap;         /**< Bit Swap;          0:Do Bit Swap   /  1:Do Not Bit Swap             */
} capi_prbs_ssprq_t;            /**< PRBS SSPRQ type                                                     */

/**
 * The structure Q PRBS 13 pattern type
 */
typedef struct capi_q_prbs_13_s {
    uint32_t   training_frame;        /**< 0:Generate 31096 bit long training frame  /  1:Don't Generate */
    uint32_t   invert_bits;           /**< 0:Invert PRBS data every 8191 bits  /  1:Do not Invert Bits   */
} capi_q_prbs_13_t;                   /**< Q PRBS 13 type                                                */

/**
 * The structure square wave pattern type
 */
typedef struct capi_prbs_square_wave_s {
    uint32_t   ptrn_wave_type;           /**< Refer to square_wave_pattern_t                             */
} capi_prbs_square_wave_t;               /**< PRBS Square Wave type                                      */

/** The structure shared Tx pattern type
 */
typedef struct capi_prbs_shared_tx_ptrn_s {
    uint8_t  length;                        /**< Pattern length                                             */
    uint8_t  pattern[32];                  /**< Pattern content                                            */
} capi_prbs_shared_tx_ptrn_t;               /**< PRBS Shared Tx Pattern type                                */


/**
 * The structure staircase pattern type
 */
typedef struct capi_prbs_staircase_pat_s {
    uint8_t   amplitude;           /**< amplitude [0,168]                                   */
} capi_prbs_staircase_pat_t;               /**< TX Staircase Pattern GEN type                                      */

/**
 * The structure for pcs scramble config
 */
typedef struct capi_pcs_scramble_s {
    uint32_t   txpi_on;                     /**< internal usage                                             */
} capi_pcs_scramble_t;


/**
 * The structure of PRBS info
 */
typedef struct prbs_info_s {
    uint32_t ptype;                                 /**< PRBS type: refer capi_pattern_gen_mon_type_t                       */
    uint8_t  gen_switch;                            /**< Pattern Generator; 0:OFF  /  1:ON  refer capi_switch_t  */    

    union {
        capi_prbs_cfg_t            pcfg;            /**< PRBS config info, used when ptype is CAPI_PRBS_LINE_WRAPPER or CAPI_PRBS_BLACKHAWK              */
        capi_prbs_ssprq_t          ssprq;           /**< PRBS SSPRQ Pattern type : CAPI_PRBS_SSPRQ, valid only for line                                  */
        capi_q_prbs_13_t           qprbs13;         /**< Q PRBS 13 Pattern type : CAPI_PRBS_Q_PRBS_13, not supported by neither client nor line          */
        capi_prbs_square_wave_t    sqr_wave;        /**< PRBS Square Wave Pattern type : CAPI_PRBS_SQUARE_WAVE, not supported by neither client nor line */
        capi_prbs_shared_tx_ptrn_t shared_tx_ptrn;  /**< PRBS Shared Tx Pattern : CAPI_PRBS_SHARED_TX_PATTERN, valid only for client                     */
        capi_prbs_staircase_pat_t  staircase_pat;   /**< Staircase Pattern GEN type :  output a stepping PAM4 pattern, that does this:  0 , 1, 2, 3, 0, 1, 2, 3,....... */
        capi_pcs_scramble_t        pcs_scramble_idle; /**< PCS scramble configuration                              */
    } op;
} capi_prbs_info_t;                                   /**< PRBS Information type                                   */

/*
 * Enumeration for inject PRBS error type
 */
typedef enum capi_prbs_err_inj_type_s{
    PRBS_ERR_INJ_LSB = 0x1,                          /**< Inject LSB error counter */
    PRBS_ERR_INJ_MSB = 0x2,                          /**< Inject MSB error counter */

} capi_prbs_err_inj_type_t;

/**
 * The structure of PRBS error inject
 */
typedef struct capi_prbs_err_inject_s {
    uint32_t ptype;                                 /**< PRBS type: refer   capi_pattern_gen_mon_type_t     */
    uint32_t enable;                                /**< PRBS enable info: refer capi_enable_t */
    uint32_t inject_err_num;                        /**< PRBS err                              */
    uint32_t err_type;                              /**< PRBS error bitwise type:  bit 0: LSB err; bit 1:MSB err */
} capi_prbs_err_inject_t;                           /**< PRBS error injection type             */

/**
 * The structure of PRBS error
 */
typedef struct prbs_ml_err_s {
    #ifdef CAPI_IS_BIG_ENDIAN
    uint32_t msb_err;
    uint32_t lsb_err;
    #else
    uint32_t lsb_err;
    uint32_t msb_err;
    #endif
} prbs_ml_err_t;

/**
 * The structure of PRBS status
 */
typedef struct prbs_status_s {
    uint32_t prbs_type;         /**< refer to capi_pattern_gen_mon_type_t */
    uint32_t lock;              /**< Whether PRBS is currently locked */
    uint32_t lock_loss;         /**< Whether PRBS was unlocked since last call */

    union {
        uint64_t s_err;
        prbs_ml_err_t ml_err;
    } err;                      /**< PRBS errors count */
} capi_prbs_status_t;           /**< PRBS status type */

/*
 * Enumeration for GPR lane status type
 */
typedef enum capi_lane_status_type_e
{
    GPR_LANE_CDR_LOCK_STATUS        = 0,             /**< Lane CDR Lock status type to retrieve                  */
    GPR_LANE_LOL_LOS_STATUS         = 1,             /**< Lane los and cdr lol status                            */
    GPR_LANE_SIGDET_STATUS          = 2,             /**< Lane signal detect status                              */
    GPR_LANE_TX_SQUELCH_STATUS      = 3,             /**< Lane Tx Squelch status                                 */
    GPR_LANE_RX_OUTPUT_STATUS       = 4,             /**< Lane Rx Output status type to retrieve                 */
    GPR_LANE_RX_OUTPUT_LATCH_STATUS = 5,             /**< Lane Rx Output Latch status type to retrieve           */
    GPR_LANE_RX_MPI_STICKY_STATUS   = 6,             /**< Lane Rx MPI ready status                               */
    GPR_LANE_SNR_LTP_READY_STATUS   = 7,             /**< Lane Rx MPI ready status                               */
    GPR_LANE_CDR_RESTART_COUNTER    = 8,             /**< Lane Rx output CDR Restart counter                     */
    GPR_LANE_CDR_LOCK_COUNTER       = 9              /**< Lane Rx output CDR Lock counter                        */
} capi_lane_status_type_t;                           /**< Lane CDR Lock status type                              */

/**
 * The structure of GP register based lane status
 */
typedef struct capi_gpr_lane_status_s{

    uint32_t status_type;                            /**< lane status type: refer capi_lane_status_type_t        */

    union {
        struct {
            uint16_t    lock_status;                 /**< Lane CDR lock live status of host or media lanes       */
        } cdr;                                       /**< CDR Lock                                               */

        struct {
            uint16_t    lol_sticky;                  /**< Lane CDR lol status of host or media lanes:            */
                                                     /**< bit [7:0]:cdr_lol sticky status, level triggered;      */
                                                     /**< bit [15:8]: cdr_lol live status;                       */
            uint16_t    los_status;                  /**< Lane los status of host or media lanes :               */
                                                     /**< bit [7:0]:los_lol sticky status, level triggered;      */
                                                     /**< bit [15:8]: los live status;                           */
        } los;                                       /**< LOS                                                    */

        uint16_t        lane_sigdet_status;          /**< NOT SUPPORTED Cable signal detect status               */
        uint16_t        lane_tx_squelch_status;      /**< Lane Tx Squelch status                                 */
        uint16_t        lane_rx_output_status;       /**< Lane Rx Output status of host or media lanes           */
        uint16_t        lane_rx_output_latch_status; /**< Lane Rx Output Latch status of host or media lanes     */
        uint16_t        lane_rx_mpi_status;          /**< Lane Rx MPI ready status                               */
        uint16_t        lane_snr_ltp_ready_status;   /**< Lane SNR LTP ready status                              */
        uint16_t        lane_cdr_restart_counter;    /**< NOT SUPPORTED Lane CDR lock restart counter            */
        uint16_t        lane_cdr_lock_counter;       /**< Lane CDR lock counter                                  */
    } param;                                         /**< Parameter                                              */

} capi_gpr_lane_status_t;                            /**< GPR lane status                                        */



/**
 * Phy Command Id Enumeration
 */
typedef enum phy_command_id_e {
    COMMAND_ID_ALL = 0,                              /**< Command Id; applies to all the modules */
    COMMAND_ID_SET_TXPI_OVERRIDE,                    /**< TxPI Override                          */
    COMMAND_ID_SET_LOW_POWER_MODE,                   /**< Set Low Power Mode : LPM               */
    COMMAND_ID_GET_LOW_POWER_MODE,                   /**< Get Low Power Mode : LPM               */
    COMMAND_ID_SET_POLARITY,                         /**< Configure Polarity                     */
    COMMAND_ID_GET_POLARITY,                         /**< Get Lane Egress Polarity               */
    COMMAND_ID_SET_LANE_CTRL_INFO,                   /**< Set lane control info                  */
    COMMAND_ID_GET_LANE_CTRL_INFO,                   /**< Get lane control info                  */
    COMMAND_ID_SET_CONFIG_INFO,                      /**< Set Configuration Information          */
    COMMAND_ID_GET_CONFIG_INFO,                      /**< Get Configuration Information          */
    COMMAND_ID_SET_PORT_INFO,                        /**< Set Port Information                   */
    COMMAND_ID_GET_PORT_INFO,                        /**< Get Port Information                   */
    COMMAND_ID_SET_LOOPBACK_INFO,                    /**< Set loopback info                      */
    COMMAND_ID_GET_LOOPBACK_INFO,                    /**< Get loopback info                      */
    COMMAND_ID_SET_PRBS_INFO,                        /**< Set PRBS pattern generator info        */
    COMMAND_ID_GET_PRBS_INFO,                        /**< Get PRBS pattern generator info        */
    COMMAND_ID_CLEAR_PRBS_STATUS,                    /**< Clear prbs status                      */
    COMMAND_ID_GET_PRBS_STATUS,                      /**< Get prbs status                        */
    COMMAND_ID_INJ_PRBS_ERROR,                       /**< Inject prbs error                      */
    COMMAND_ID_DIAG_LANE_STATUS,                     /**< Lane Status Diagnostic                 */
    COMMAND_ID_SET_LANE_CONFIG_INFO,                 /**< Set lane config info                   */
    COMMAND_ID_GET_LANE_CONFIG_INFO,                 /**< Get lane config info                   */
    COMMAND_ID_GET_LANE_INFO,                        /**< Get lane info                          */
    COMMAND_ID_SET_ARCHIVE_INFO,                     /**< Set archive info in SRAM               */
    COMMAND_ID_GET_ARCHIVE_INFO,                     /**< Get archive from SRAM                  */
    COMMAND_ID_GET_CMIS_INFO,                        /**< Get cmis info from SRAM                */

    COMMAND_ID_CLIENT_SIDE       = 40,               /**< Client related command Ids             */
    COMMAND_ID_SET_CLIENT_TX_DRIVER_VOLTAGE,         /**< Client Tx driver voltage               */
    COMMAND_ID_SET_CUSTOM_TXFIR_TABLE,               /**< Set Custom Txfir Table                 */
    COMMAND_ID_GET_CUSTOM_TXFIR_TABLE,               /**< Get Custom Txfir Table                 */

    COMMAND_ID_LW_SIDE = 45,                         /**< Line wrapper related command Ids       */
    COMMAND_ID_SET_DSP_OPT_LOS_HOST_TX_UNSQUELCH=46, /**< Set DSP Optical Loss Host Tx Squelch   */
    COMMAND_ID_SET_LW_DSP_MODE=47,                   /**< Set LW DSP optical / electrical mode   */
    COMMAND_ID_GET_LW_DSP_MODE=48,                   /**< Set LW DSP optical / electrical mode   */
    COMMAND_ID_GET_DSP_DEBUG_DUMP= 49,               /**< Get DSP_DEBUG_DUMP                    */

    COMMAND_ID_CW_SIDE = 50,                         /**< Core wrapper related command Ids       */
    COMMAND_ID_SET_CW_RPTR_FEC_MON_CONFIG,           /**< Set FEC Monitor config in repeater mode*/
    COMMAND_ID_SET_RECOVERED_CLOCK_INFO,             /**< Set Recovered Clock information        */
    COMMAND_ID_GET_RECOVERED_CLOCK_INFO,             /**< Get Recovered Clock information        */
    COMMAND_ID_SET_GPIO_INFO,                        /**< Set GPIO information        */
    COMMAND_ID_GET_GPIO_INFO,                        /**< Get GPIO information        */
    COMMAND_ID_START_LINKCAT,                        /**< Start Linkcat                          */
    COMMAND_ID_STOP_LINKCAT,                         /**< Stop Linkcat                           */
    COMMAND_ID_SET_SPI_INFO,                         /**< SPI info Set related command Ids       */
    COMMAND_ID_GET_SPI_INFO,                         /**< SPI info Get related command Ids       */
    COMMAND_ID_SET_CAPI_FEATURE_INFO,                /**< CAPI feature info command              */
    COMMAND_ID_GET_CAPI_FEATURE_INFO,                /**< CAPI feature info command              */

    COMMAND_ID_DIAG_SET_CMIS_SNR_LTP_INFO = 70,      /**< Diagnostic: CMIS SNR LTP Information              */
    COMMAND_ID_DIAG_GET_CMIS_SNR_LTP_CONFIG_INFO = 71,    /**< Diagnostic: CMIS SNR LTP Configuration Information*/
    COMMAND_ID_DIAG_GET_CMIS_SNR_LTP_INFO = 72,           /**< Diagnostic: CMIS SNR LTP Payload Information      */

    COMMAND_ID_DIAG_SET_MEDIA_MPI_CONFIG = 73,                       /**< Diagnostic: media side config MPI Information      */
    COMMAND_ID_DIAG_GET_MEDIA_MPI_CONFIG = 74,                       /**< Diagnostic: media side get MPI config Information      */
    COMMAND_ID_DIAG_GET_MEDIA_MPI_STATE = 75,                          /**< Diagnostic: media side get MPI Information      */
    COMMAND_ID_DIAG_SET_MEDIA_MISSION_MPI_CONFIG = 76,        /**< Diagnostic: media side config dynamic MPI Information */
    COMMAND_ID_DIAG_GET_MEDIA_MISSION_MPI_CONFIG = 77,        /**< Diagnostic: media side get dynamic MPI config Information */
    COMMAND_ID_DIAG_GET_MEDIA_MISSION_MPI_STATE = 78,           /**< Diagnostic: media side get dynamic MPI data*/

    COMMAND_ID_DIAG_SET_TRAFFIC_SWITCH_DETECT_CONFIG = 79,/**< Diagnostic: both sides config traffic mode switch from 10XG to 5XG detect */
    COMMAND_ID_DIAG_GET_TRAFFIC_SWITCH_DETECT_CONFIG = 80,/**< Diagnostic: both sides get traffic mode switch from 10XG to 5XG detect config*/
    COMMAND_ID_DIAG_GET_TRAFFIC_SWITCH_DETECT_STATE= 81,   /**< Diagnostic: both sides get traffic mode switch from 10XG to 5XG detect state*/

    COMMAND_ID_DIAG_SET_MPI_CANCELLER_CONFIG = 82,        /**< Diagnostic: media side set MPI canceller information  */
    COMMAND_ID_DIAG_GET_MPI_CANCELLER_CONFIG = 83,        /**< Diagnostic:  media side get MPI canceller information */
    COMMAND_ID_SET_PLL_CFG = 84,                          /**< Set PLL configuration info                            */
    COMMAND_ID_GET_PLL_CFG = 85,                          /**< Get PLL configuration info                            */
    COMMAND_ID_DIAG_SET_HW_GAIN2_ADAPT_CONFIG = 86,       /**< Diagnostic: media side set hardware GAIN2 adaptation  */
    COMMAND_ID_DIAG_GET_HW_GAIN2_ADAPT_CONFIG = 87,       /**< Diagnostic:  media side get hardware GAIN2 adaptation */
    
    COMMAND_ID_SET_LW_SNR_TH_CFG = 90,                    /**< Set LW snr threshold info                             */
    COMMAND_ID_GET_LW_SNR_TH_CFG = 91,                    /**< Get LW snr threshold info                             */
    COMMAND_ID_SET_DSP_PLL_FRACN_ADJUST_ON_THE_FLY = 92,  /**< Config dsp pll fracN mode ratio adjust on the fly     */
    COMMAND_ID_GET_DSP_PLL_FRACN_ADJUST_ON_THE_FLY = 93,  /**< Get dsp pll fracN mode ratio adjust on the fly        */
    COMMAND_ID_DIAG_DISPLAY_FW_EVENT_LOG=97,            /**< Diagnostic: display FW event log                */
    COMMAND_ID_GET_STATUS_INFO = 98,                      /**< Get status info                        */

    COMMAND_ID_SET_LANE_SR2 = 99,            /**< Set lane to SR2         */
    
    COMMAND_ID_START_DIAG_DUMP = 100,        /**< Diagnostic: start DSP diagnostics dump          */
    COMMAND_ID_GET_DIAG_DUMP = 101,          /**< Diagnostic: get DSP diagnostics dump data       */

    COMMAND_ID_SET_GAIN3_A_PARAM = 105,               /**<  media side set gain3_a_param information  */
    COMMAND_ID_GET_GAIN3_A_PARAM = 106,               /**<  media side get gain3_a_param information */
    COMMAND_ID_GET_LW_RX_DIAG_INFO = 112,            /**< Get LW Rx diagnostics info             */
    COMMAND_ID_LAST                                  /**< Last Command Identifier                */
} phy_command_id_t;                                  /**< Phy Command Id type                    */


/**
 * The structure to represent eye margin estimate
 */
typedef struct eye_margin_estimate_s {
    uint16_t left_eye_mui;                         /**< Left eye mUI                            */
    uint16_t right_eye_mui;                        /**< Right eye mUI                           */
    uint16_t upper_eye_mv;                         /**< Upper eye mUI                           */
    uint16_t lower_eye_mv;                         /**< Lower eye mUI                           */
} eye_margin_estimate_t;                            /**< Eye Margin Estimate type                */



/**
 * The structure of diagnostic lane status
 */
typedef struct capi_diag_lane_status_s {
    union {
        uint32_t content;                            /**< The command payload                     */
        struct {
#ifdef CAPI_IS_BIG_ENDIAN
            uint32_t reserved            : 30;      /**< Reserved for later use                  */
            uint32_t eye_margin_estimate : 1;       /**< Serdes : Eye Margin Estimate            */
            uint32_t dc_offset           : 1;       /**< DC Offset                               */
#else  /* CAPI_IS_LITTLE_ENDIAN */
            uint32_t dc_offset           : 1;       /**< DC Offset                               */
            uint32_t eye_margin_estimate : 1;       /**< Serdes : Eye Margin Estimate            */
            uint32_t reserved            : 30;      /**< Reserved for later use                  */
#endif /* CAPI_IS_BIG_ENDIAN */
        } is;                                       /**< Parameter is <name>                     */
    } param;

    union {
        eye_margin_estimate_t eye_margin_est;       /**< Eye Margin Est; eye_margin_estimate_t   */
        int8_t                dc_offset;            /**< DC Offset value                         */
        int8_t                payload[8];           /**< Padding                                 */
    } value;
} capi_diag_lane_status_t;                          /**< Client Lane Status Information type     */


/**
 * The structure of firmware reset
 */
typedef struct reset_s {
    uint8_t direction;                           /**< Ingress/Egress/Both:  refer to capi_direction_t  */
    uint8_t on_off;                              /**< Switch On/Off/Toggle: refer to capi_switch_t     */
} reset_t;                                       /**< Reset type                                       */

/**
 * The structure of Low Power Mode
 */
typedef struct lpm_s {
    uint8_t on_off;                               /**< Switch On/Off/Toggle: refer to capi_switch_t */
} lpm_t;                                          /**< Low Power Mode type                          */

/**
 * The structure of TxPI override
 */
typedef struct txpi_override_s {
    uint8_t enable;                               /**< txpi override, refer to capi_enable_t */
    int16_t ppm;                                  /**< PPM value                             */
} txpi_override_t;                                /**< TxPI Override Type                    */

/**
 * The enumeration of Host FEC Type
 */
typedef enum capi_host_fec_type_e {
    CAPI_HOST_FEC_TYPE_NA = 0,          /**< No FEC Mode */
    CAPI_HOST_FEC_TYPE_RS528 = 1,       /**< RS528 KR4*/
    CAPI_HOST_FEC_TYPE_RS544 = 2,       /**< RS544 KP4 */
    CAPI_HOST_FEC_TYPE_PCS = 3,         /**< PCS */
    CAPI_HOST_FEC_TYPE_CL161 = 4,       /**< CL161 FEC */
    CAPI_HOST_FEC_TYPE_MAX
} capi_host_fec_type_t;                 /**< FEC type definition */

/**
 * The enumeration of Line FEC Type
 */
typedef enum capi_line_fec_type_e {
    CAPI_LINE_FEC_TYPE_NA = 0,          /**< No FEC Mode */
    CAPI_LINE_FEC_TYPE_RS528 = 1,       /**< RS528 KR4*/
    CAPI_LINE_FEC_TYPE_RS544 = 2,       /**< RS544 KP4 */
    CAPI_LINE_FEC_TYPE_PCS = 3,       /**< PCS */
    CAPI_LINE_FEC_TYPE_CL161 = 4,       /**< CL161 FEC */
    CAPI_LINE_FEC_TYPE_MAX
} capi_line_fec_type_t;                 /**< FEC type definition */

/**
 * The enumeration of reference clock frequency mode
 */
typedef enum ref_clk_frq_mode_e {
    CAPI_REF_CLK_FRQ_156_25_MHZ_ETHERNET = 0,       /**< 156.25 Mhz (Ethernet) */
    CAPI_REF_CLK_FRQ_625_MHZ_ETHERNET = 1,          /**< 625 Mhz (Ethernet)    */
    CAPI_REF_CLK_FRQ_166_15625_MHZ_ETHERNET = 2,    /**< 166.15625 Mhz         */
    CAPI_REF_CLK_FRQ_MAX
} capi_ref_clk_frq_mode_t;                          /**< Reference clock frequency mode type */

/**
 * The enumeration of lane mux Type
 */
typedef enum capi_lane_mux_e {
    CAPI_LANE_MUX_BIT_MUX = 0,          /**< Bit mux type */
    CAPI_LANE_MUX_SYMBOL_MUX = 1,       /**< Symbol mux type  */
    CAPI_LANE_MUX_MAX
} capi_lane_mux_t;                      /**< Chip lane mux type */

/**
 * The enumeration of Path Type
 */
typedef enum {
    CAPI_REPEATER_PATH,         /*!< Repeater */
    CAPI_RETIMER_PATH,          /*!< Retimer */
    CAPI_PATH_MAX
} capi_cfg_dp_type_t;           /*!< Chip data path type */

/**
 * The enumeration of lane FEC term Type
 */
typedef enum capi_lane_fec_term_type_e {
    CAPI_LANE_FEC_TERM_BYPASS = 0,          /**< FEC_TERM_BYPASS */
    CAPI_LANE_FEC_DEC_FWD = 1,              /**< FEC_DEC_FWD */
    CAPI_LANE_FEC_DEC_ENC = 2,              /**< FEC_DEC_ENC */
    CAPI_LANE_PCS_XENC = 3,                 /**< PCS_XENC */
    CAPI_LANE_FEC_DEC_XDEC_XENC_ENC = 4,    /**< FEC_DEC_XDEC_XENC_ENC */
    CAPI_LANE_FEC_TERM_MAX
} capi_lane_fec_term_type_t;                /**< Chip lane fec term type */

/**
 * The enumeration of Line Wrapper Baud Rate
 */
typedef enum capi_lw_baud_rate_e {
    CAPI_LW_BR_106_25 = 0,                  /**< LW baud rate 106.25G    */
    CAPI_LW_BR_103_13,                      /**< LW baud rate 103.13G    */
    CAPI_LW_BR_25_78125,                    /**< LW baud rate 25.78125G  */
    CAPI_LW_BR_111_57,                      /**< LW baud rate 111.57G    */    
    CAPI_LW_BR_53_125,
    CAPI_LW_BR_51_565,
    CAPI_LW_BR_26_5625,
    CAPI_LW_BR_MAX
} capi_lw_baud_rate_t;                      /**< LW baud rates           */

/**
 * The enumeration of BH Baud Rate
 */
typedef enum capi_bh_baud_rate_e {
    CAPI_BH_BR_106_25 = 0,                  /**< LW baud rate 106.25G    */
    CAPI_BH_BR_53_125,                      /**< BH baud rate 53.125G    */
    CAPI_BH_BR_51_563,                      /**< BH baud rate 51.563G    */
    CAPI_BH_BR_25_78125,                    /**< BH baud rate 25.78125G  */
    CAPI_BH_BR_26_5625,                     /**< BH baud rate 26.5625G   */
    CAPI_BH_BR_MAX
} capi_bh_baud_rate_t;                      /**< BH baud rates           */

/**
 * The enumeration of chip function mode
 */
typedef enum capi_function_mode_e {
    CAPI_MODE_NONE = 0,                     /**< No suitable func mode   */
    CAPI_MODE_400G = 1,                     /**< 400G mode               */
    CAPI_MODE_200G = 2,                     /**< 200G mode               */
    CAPI_MODE_100G = 3,                     /**< 100G mode               */
    CAPI_MODE_50G  = 4,                     /**< 50G mode                */
    CAPI_MODE_25G  = 5,                     /**< 25G mode                */
    CAPI_MODE_800G = 6,                     /**< 800G mode               */
    CAPI_MODE_MAX
} capi_function_mode_t;                     /**< Chip function mode type */

/**
 * The enumeration of modulation type
 */
typedef enum capi_modulation_e {
    CAPI_MODULATION_NRZ = 0,                /**< NRZ modulation type */
    CAPI_MODULATION_PAM4 = 1,               /**< PAM4 modulation type */
} capi_modulation_t;                        /**< Chip modulation type list*/

/**
 * A structure to represent lane configuration information
 */
typedef struct config_lane_mod_s {
    uint16_t modulation;                    /**< modulation type; refer to capi_modulation_t   */
    uint16_t lane_mask;                     /**< lane mask related to specified port_mode_enum */
    /* capi_lane_data_rate_t data_rate;*/   /**< data rate enum type                           */
    /*capi_an_config_t an_config; */        /**< AN configuration parameters                   */
    /*capi_an_status_info_t an_status; */   /**< AN status parameters                          */
} capi_lane_config_mod_t;                  /**< Lane configuration information type           */

/**
 * A structure to represent port configuration status
 */
typedef enum capi_port_config_status_e {
    CAPI_PORT_CONFIG_STATUS_NOT_CONFIGURED = 0,     /**< Port not configured               */
    CAPI_PORT_CONFIG_STATUS_IN_PROGRESS = 1,        /**< Port configurtion is in progress  */
    CAPI_PORT_CONFIG_STATUS_SUCCESS = 2,            /**< Port configuration success        */
    CAPI_PORT_CONFIG_STATUS_FAILED = 3,             /**< Port configuration failed         */
} capi_port_config_status_t;                        /**< Port config status type           */

/**
 * A structure to represent port power down status
 */
typedef enum capi_port_power_down_status_e {
    CAPI_PORT_POWER_DOWN_STATUS_POWER_UP=0,         /**< Port is powered up          */
    CAPI_PORT_POWER_DOWN_STATUS_POWER_DOWN=1        /**< Port is powered down        */
} capi_port_power_down_status_t;                     /**< Port power down status type */


/**
 * The structure to represent chip configuration information
 */
typedef struct config_info_s {
    uint16_t                ref_clk;        /**< Reference clock; refer to capi_ref_clk_frq_mode_t               */
    uint16_t                func_mode;      /**< Function Mode; refer to capi_function_mode_t                    */
    uint16_t                mux_type;       /**< MUX mode; refer to capi_lane_mux_t                              */
    uint16_t                fec_term;       /**< fec term type; refer to capi_lane_fec_term_type_t               */
    uint16_t                line_fec_type;  /**< Line side FEC config info; refer to capi_line_fec_type_t        */
    uint16_t                host_fec_type;  /**< Host side FEC config info; refer to capi_host_fec_type_t        */
    uint16_t                lw_br;          /**< Line side bud rate; refer to capi_lw_baud_rate_t                */
    uint16_t                bh_br;          /**< Client side bud rate; refer to capi_lw_baud_rate_t              */
    capi_lane_config_mod_t  line_lane;      /**< line side lane config                                           */
    capi_lane_config_mod_t  host_lane;      /**< host side lane config                                           */
    uint16_t                status;         /**< Port configuration status refer to capi_port_config_status_t    */
    uint16_t                pwd_status;     /**< Port power down status; refer to capi_port_power_down_status_t  */
    uint32_t                fec_monitor_port;/**< =0 regular port configuration =0 fec monitor port cofiguration */
} capi_config_info_t;                       /**< Configuration info type                                         */

/**
 * The structure to represent chip configuration extended information (EGR and IGR FEC termination)
 */
typedef struct config_ex_info_s {
    uint16_t                ref_clk;        /**< Reference clock; refer to capi_ref_clk_frq_mode_t               */
    uint16_t                func_mode;      /**< Function Mode; refer to capi_function_mode_t                    */
    uint16_t                mux_type;       /**< MUX mode; refer to capi_lane_mux_t                              */
    uint16_t                egr_fec_term;   /**< EGR fec term type; refer to capi_lane_fec_term_type_t               */
    uint16_t                igr_fec_term;   /**< IGR fec term type; refer to capi_lane_fec_term_type_t               */
    uint16_t                line_fec_type;  /**< Line side FEC config info; refer to capi_line_fec_type_t        */
    uint16_t                host_fec_type;  /**< Host side FEC config info; refer to capi_host_fec_type_t        */
    uint16_t                lw_br;          /**< Line side bud rate; refer to capi_lw_baud_rate_t                */
    uint16_t                bh_br;          /**< Client side bud rate; refer to capi_bh_baud_rate_t              */
    capi_lane_config_mod_t  line_lane;      /**< line side lane config                                           */
    capi_lane_config_mod_t  host_lane;      /**< host side lane config                                           */
    uint16_t                status;         /**< Port configuration status refer to capi_port_config_status_t    */
    uint16_t                pwd_status;     /**< Port power down status; refer to capi_port_power_down_status_t  */
    uint16_t                mixed_mode;     /**< Mixed mode, refer to capi_mixed_mode_t  */
} capi_config_ex_info_t;                       /**< Configuration info type                                         */

/**
 * The enumeration of FEC mode Type
 */
typedef enum capi_forward_error_correction_mode_e {
    HOST_NONE_TERM_BYPASS_MEDIA_NONE = 0,          /**< repeater mode                          */       
    HOST_RS544_TERM_DEC_FWD_MEDIA_RS544 = 1,           /**< Host RS544 : DEC_FWD   : MEDIA RS544   */
    HOST_RS544_TERM_DEC_ENC_MEDIA_RS544 = 2,           /**< Host RS544 : DEC_ENC   : MEDIA RS544   */
    HOST_RS544_TERM_XDEC_XENC_MEDIA_RS544 = 3,         /**< Host RS544 : XDEC_XENC : MEDIA RS544   */
    HOST_RS528_TERM_DEC_FWD_MEDIA_RS528 = 4,           /**< Host RS528 : DEC_FWD   : MEDIA RS528   */
    HOST_RS528_TERM_DEC_ENC_MEDIA_RS528 = 5,           /**< Host RS528 : DEC_ENC   : MEDIA RS528   */
    HOST_RS528_TERM_XDEC_XENC_MEDIA_RS528 = 6,         /**< Host RS528 : XDEC_XENC : MEDIA RS528   */
    HOST_RS528_TERM_DEC_ENC_MEDIA_RS544 = 7,           /**< Host RS528 : DEC_ENC   : MEDIA RS544   */
    HOST_RS528_TERM_XDEC_XENC_MEDIA_RS544 = 8,         /**< Host RS528 : XDEC_XENC : MEDIA RS544   */
    HOST_RS544_TERM_DEC_ENC_MEDIA_RS528 = 9,           /**< Host RS544 : DEC_ENC   : MEDIA RS528   */
    HOST_RS544_TERM_XDEC_XENC_MEDIA_RS528 = 10,         /**< Host RS544 : XDEC_XENC : MEDIA RS528   */
    HOST_PCS_TERM_PCS_XENC_MEDIA_RS528 = 11,            /**< Host PCS   : PCS_XENC  : MEDIA RS528   */
    HOST_PCS_TERM_PCS_XENC_MEDIA_RS544 = 12,            /**< Host PCS   : PCS_XENC  : MEDIA RS544   */
    CAPI_FORWARD_ERROR_CORR_MODE_MAX
} capi_forward_error_correction_mode_t;            /**< FEC Mode type                                                        */

/**
 * The enumeration of mixed mode mode Type
 */
typedef enum capi_mixed_mode_e {
    CAPI_MIXED_MODE_NONE             = 0,         /**< none mixed mode: data rate is same for all lanes                                       */       
    CAPI_MIXED_MODE_PORT_DEPENDENT   = 1,         /**< port dependent mixed mode: config one port may disrupt traffic on the other ports      */
    CAPI_MIXED_MODE_PORT_INDEPENDENT = 2,         /**< port in-dependent mixed mode: config one port won't disrupt traffic on the other ports */
} capi_mixed_mode_t;            /**< Mixed Mode type */

/**
 * The structure to represent port information
 */
typedef struct port_info_s {
    uint32_t lane_mask;                       /**< Lane mask of either host or media side                          */
                                              /**< If (phy_side = 0 : PHY_HOST_SIDE ) lane_mask lower nibble       */
                                              /**< If (phy_side = 1 : PHY_MEDIA_SIDE) lane_mask lower nibble       */
                                              /**< If (phy_side = 2 : PHY_BOTH_SIDES)                              */
                                              /**<    lane_mask lower  nibble : media side lane mask               */
                                              /**<    lane_mask higher nibble : host  side lane mask               */

    union {
        uint32_t content;                     /**< The content                                                     */

        struct{
            #ifdef CAPI_IS_BIG_ENDIAN
            uint32_t phy_side   :  2;         /**< Phy side : Refer to phy_side_t;                                 */
            uint32_t add_port   :  1;         /**< 1 : Configure the port                                          */
            uint32_t drop_port  :  1;         /**< 0 : do not drop port   1 : drop port                            */
            uint32_t symbol_mux :  1;         /**< Symbol Mux, 0 - bit mux, 1 - symbol mux                         */
            uint32_t mixed_mode :  2;         /**< Mixed mode, refer to capi_mixed_mode_t                          */
            uint32_t reserved   : 25;         /**< Reserved                                                        */
            #else  /*CAPI_IS_BIG_ENDIAN*/
            uint32_t reserved   : 25;         /**< Reserved                                                        */
            uint32_t mixed_mode :  2;         /**< Mixed mode, refer to capi_mixed_mode_t                          */
            uint32_t symbol_mux :  1;         /**< Symbol Mux, 0 - bit mux, 1 - symbol mux                         */
            uint32_t drop_port  :  1;         /**< 0 : do not drop port   1 : drop port                            */
            uint32_t add_port   :  1;         /**< 1 : Configure the port                                          */
            uint32_t phy_side   :  2;         /**< Phy side : Refer to phy_side_t;                                 */
            #endif /*CAPI_IS_BIG_ENDIAN*/
        } is;
    } param;

    struct {
        uint8_t  ref_clk;                     /**< Rerefence clock; refer to ref_clk_freq_t                        */
        uint8_t  mode;                        /**< Port channel mode; refer to the mode table                      */
        uint8_t  egr_fec_mode;                /**< Egress FEC Mode; refer to capi_forward_error_correction_mode_t  */
        uint8_t  igr_fec_mode;                /**< Ingress FEC Mode; refer to capi_forward_error_correction_mode_t */
        uint8_t  status;                      /**< Port configuration status refer to capi_port_config_status_t    */
        uint8_t  pwd_status;                  /**< Port power down status; refer to capi_port_power_down_status_t  */
    } port;

} capi_port_info_t;                           /**< Port Information type                                           */




/** Changes brought in from capi.h  **/
#define CAPI_NUM_DFE_TAPS 14 /*CENTENARIO_NUM_DFE_TAPS         EPDM parameter */
#define PURELY_CAPI_SET_VDDM   1

#define CHIP_VOLTAGE_SET_FLAG    0x6001
#define CHIP_VOLTAGE_GET_STATUS  0x7000

extern uint8_t util_get_max_lanes(phy_info_t* phy_info_ptr);

#define MAX_CORES                   1   /*!< Max cores in the chip */
#define MAX_PRTAD                   8   /*!< Max port_address supported      */
#define MAX_LW_LANES                4   /*!< Max LW lanes supported          */
#define MAX_PORT                    4
#define MAX_BH_LANES                4   /*!< Max BH lanes supported          */
#define MAX_LANES(x)                util_get_max_lanes(x)
#define ALL_LW_LANES_MASK           ((1<<MAX_LW_LANES)-1)
#define ALL_PORT_MASK               ((1<<MAX_PORT)-1)
/*
#define NUM_OF_TRACE_POINT_PER_GENERAL 16
#define TRACE_BUFFER_BASE_GENERAL 0x2FA00
*/

/* #define INCREMENTAL_MDIO */
#define __HSIP_FUNC_ERR &(__hsip_func_err)

/*
 * A enumaratoin to represent register type
 */
typedef enum register_type_s {
    CAPI_CHIP_REF_CLOCK_CONFIG_REGISTER,        /**< Chip Reference Clock Configuration Register */
    CAPI_CHIP_TEMP_STATUS_REGISTER,             /**< Chip Temperture Status Register */
    CAPI_CHIP_VOLTAGE_STATUS_REGISTER,          /**< Chip Voltage Status Register */
    CAPI_LANE_MODE_REGISTER,                    /**< Lane Mode Register */
    CAPI_LANE_COMMAND_REGISTER,                 /**< Lane Command Register */
    CAPI_LANE_STATUS_REGISTER,                  /**< Lane Status Register */
    CAPI_CLIENT_LANE_MODE_REGISTER,             /**< Client Lain Mode Register */
    CAPI_LINE_WRAPPER_MODE_REGISTER             /**< Line Wrapper Mode Register */
} capi_register_t;                              /**< Register type */

/**
 * The union of chip reference clock configuration register
 */
typedef union chip_ref_clk_conf_reg_u {
    uint16_t content;                       /** Chip Reference Clock Configuration Register content */

    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t ref_clk_valid : 1;         /** Rererence Clock Valid */
        uint16_t reserved : 10;             /** Reserved */
        uint16_t ref_clock_freq_mode : 5;   /** Reference Clock Frequency Mode */
#else
        uint16_t ref_clock_freq_mode : 5;   /** Reference Clock Frequency Mode */
        uint16_t reserved : 10;             /** Reserved */
        uint16_t ref_clk_valid : 1;         /** Rererence Clock Valid */
#endif
    } is;
} capi_chip_ref_clk_conf_reg_t;             /** Reference Clock Config Rerigster type */

/**
 * The enumeration of reference clock valid
 */
typedef enum ref_clk_valid_s {
    CAPI_REF_CLOCK_NOT_VALID,   /**< Reference clock not valid */
    CAPI_REF_CLOCK_VALID        /**< Reference clock valid */
} capi_ref_clk_valid_t;         /**< Reference Clock VAlid type*/

/**
 * The enumeration of check type
 */
typedef enum capi_check_type_e {
    CAPI_ONLY_READ,            /**< only read currently */
    CAPI_CLEAR_WAIT_READ,      /**< clear to 0, wait time and read again  */
    CAPI_CLEAR_ONLY            /**< Only clear to 0     */
} capi_check_type_t;           /**< checkType */

/**
 * The union of chip temperature status register
 */
typedef struct chip_temp_status_reg_s {
    int16_t temperature;                         /**< Tempurture reported in celcius                */
                                                 /**< Normal Range  : -10C to 105C                  */
                                                 /**< Exterim Range : -40C to 125C  Not recommended */ 
    capi_check_type_t check_type;
} capi_temp_status_info_t;                       /**< Tempurture Status Information type            */



/**
 * The union of lane mode register
 */
typedef union lane_mode_reg_u {
    uint16_t content; /** Lane Mode Register content*/

    struct {
        #ifdef CAPI_IS_BIG_ENDIAN
        uint16_t client_loopback_alt_mode : 1;  /** Client Loopback Alt Mode */
        uint16_t function_mode            : 6;  /** Function Mode            */
        uint16_t fec_mode                 : 2;  /** FEC Mode                 */
        uint16_t rate_mode                : 7;  /** Rate Mode                */
        #else
        uint16_t rate_mode                : 7;  /** Rate Mode                */
        uint16_t fec_mode                 : 2;  /** FEC Mode                 */
        uint16_t function_mode            : 6;  /** Function Mode            */
        uint16_t client_loopback_alt_mode : 1;  /** Client Loopback Alt Mode */
        #endif
    } is;
} capi_lane_mode_reg_t;                         /** Lane Mode Register type */


/**
 * The enumeration of Black Hawk Loopback Alt Mode
 */
typedef enum bh_loopback_alt_mode_e {
    CAPI_BH_LOOPBACK_ALT_MODE_OFF,      /**< Blackhawk loopback ALT mode off */
    CAPI_BH_LOOPBACK_ALT_MODE_ON        /**< Blackhawk loopback ALT mode on */
} capi_bh_loopback_alt_mode_t;          /**< BH Loopback ALT Mode type */

/**
 * The union of lane command register
 */
typedef union lane_command_reg_u {
    uint16_t content; /** Lane Command Register content*/

    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t command_change : 1;    /** command Change */
        uint16_t reserved : 7;          /** Reserved */
        uint16_t command : 8;           /** Command */
#else
        uint16_t command : 8;           /** Command */
        uint16_t reserved : 7;          /** Reserved */
        uint16_t command_change : 1;    /** command Change */
#endif
    } is;
} capi_lane_command_reg_t;              /** Lane Command Register type */


/**
 * The enumeration of command change
 */
typedef enum command_change_e {
    CAPI_COMMAND_NOT_CHANGED,   /**< Command Not Changed */
    CAPI_COMMAND_CHANGED        /**< Command Changed */
} capi_command_change_t;        /**< Command Change type */

/**
 * The union of lane status register
 */
typedef union lane_status_reg_u {
    uint16_t content;                       /** Lane Status Register content*/

    #ifdef CAPI_IS_BIG_ENDIAN
    struct {
        uint16_t command_accepted   : 1;    /** Command Accepted                     */
        uint16_t line_lock          : 1;    /** Line Lock                            */
        uint16_t error_code         : 6;    /** Error Code                           */
        uint16_t linktrn_error_code : 2;    /** Link Training Error Code             */
        uint16_t linktrn_done       : 1;    /** Successfully Completed Link Training */
        uint16_t cdr_lock           : 2;    /** CDR Lock                             */
        uint16_t adc_clip_detect    : 1;    /** Detected Clipping of ADC Signal      */
        uint16_t los                : 1;    /** Lose of Signal                       */
        uint16_t pll_lock           : 1;    /** PLL Lock                             */
    } lw;

    struct {
        uint16_t command_accepted   : 1;     /** Command Accepted                    */
        uint16_t line_lock          : 1;     /** Line Lock                           */
        uint16_t error_code         : 6;     /** Error Code                          */
        uint16_t reserved2          : 1;     /** Reserved                            */
        uint16_t pll_unlock_sticky  : 2;     /** PLL unlock sticky                   */
        uint16_t bh2bh_fifo_status  : 2;     /** Bh2bh fifo status                   */
        uint16_t cdr_lock           : 1;     /** CDR Lock                            */
        uint16_t los                : 1;     /** Lose of Signal                      */
        uint16_t pll_lock           : 1;     /** PLL Lock                            */
    } client;

    #else /*CAPI_IS_LITTLE_ENDIAN*/

    struct {
        uint16_t pll_lock           : 1;     /** PLL Lock                             */
        uint16_t los                : 1;     /** Lose of Signal                       */
        uint16_t adc_clip_detect    : 1;     /** Detected Clipping of ADC Signal      */
        uint16_t cdr_lock           : 2;     /** CDR Lock                             */
        uint16_t linktrn_done       : 1;     /** Successfully Completed Link Training */
        uint16_t linktrn_error_code : 2;     /** Link Training Error Code             */
        uint16_t error_code         : 6;     /** Error Code                           */
        uint16_t line_lock          : 1;     /** Line Lock                            */
        uint16_t command_accepted   : 1;     /** Command Accepted                     */
    } lw;

    struct {
        uint16_t pll_lock           : 1;     /** PLL Lock                             */
        uint16_t los                : 1;     /** Lose of Signal                       */
        uint16_t cdr_lock           : 1;     /** CDR Lock                             */
        uint16_t bh2bh_fifo_status  : 2;     /** Bh2bh fifo status                    */
        uint16_t pll_unlock_sticky  : 2;     /** PLL unlock sticky                    */
        uint16_t reserved2          : 1;     /** Reserved                             */
        uint16_t error_code         : 6;     /** Error Code                           */
        uint16_t line_lock          : 1;     /** Line Lock                            */
        uint16_t command_accepted   : 1;     /** Command Accepted                     */
    } client;
    #endif /*CAPI_IS_BIG_ENDIAN*/
} capi_lane_status_reg_t;                    /** Line Wrapper and Client Lane Status Register type */


typedef union chip_top_spi_program_command_t_u {
    uint32_t words;
    struct {
        #ifdef CAPI_IS_BIG_ENDIAN
        uint32_t reserved       :12;         /*!< reserved       */ 
        uint32_t block_id       :16;         /*!< block_id       */
        uint32_t buffer_id      : 1;         /*!< buffer_id      */         
        uint32_t error_status   : 2;         /*!< error_status   */       
        uint32_t command_change : 1;         /*!< command_change */
        #else /* LITTLE ENDIAN */        
        uint32_t command_change : 1;         /*!< command_change */        
        uint32_t error_status   : 2;         /*!< error_status   */ 
        uint32_t buffer_id      : 1;         /*!< buffer_id      */   
        uint32_t block_id       :16;         /*!< block_id       */
        uint32_t reserved       :12;         /*!< reserved       */
        #endif /*CAPI_IS_BIG_ENDIAN*/
    } fields;
} chip_top_spi_program_command_t;



/**
 * The enumeration of error code
 */
typedef enum capi_err_code_e {
    CAPI_NO_ERROR       /**< No Error Code */
} capi_err_code_t;      /**< Error Code type */

/**
 * The enumeration of command accepted
 */
typedef enum command_accepted_e {
    CAPI_COMMAND_NOT_ACCEPTED,  /**< Command Not Accepted */
    CAPI_COMMAND_ACCEPTED       /**< Command Accepted */
} capi_command_accepted_t;      /**< Command Accepted type */


/**
 * The union of BH lane mode register
 */

typedef union bh_lane_config_reg_u {
    uint16_t content; /**< BH Lane Mode Register content */

    struct {
        #ifdef CAPI_IS_BIG_ENDIAN
        uint16_t rx            : 1;    /**< Rx disable    */
        uint16_t tx            : 1;    /**< Tx disable    */
        uint16_t link_training : 1;    /**< Link training */
        uint16_t eq2           : 1;    /**< EQ2           */
        uint16_t precoder      : 1;    /**< Precoder      */
        uint16_t ns_es         : 1;    /**< NS/ES         */
        uint16_t medium        : 2;    /**< Medium        */
        uint16_t lane_mode     : 2;    /**< Lane Mode     */
        uint16_t db_lost       : 6;    /**< DB Lost       */
        #else /*CAPI_IS_LITTLE_ENDIAN*/
        uint16_t db_lost       : 6;    /**< DB Lost       */
        uint16_t lane_mode     : 2;    /**< Lane Mode     */
        uint16_t medium        : 2;    /**< Medium        */
        uint16_t ns_es         : 1;    /**< NS/ES         */
        uint16_t precoder      : 1;    /**< Precoder      */
        uint16_t eq2           : 1;    /**< EQ2           */
        uint16_t link_training : 1;    /**< Link training */
        uint16_t tx            : 1;    /**< Tx disable    */
        uint16_t rx            : 1;    /**< Rx disable    */
        #endif /*CAPI_IS_BIG_ENDIAN*/
    } is;
} bh_lane_config_reg_t;             /**< Black Hawk Lane Config Register type */

/**
 * The enumeration of BH DB lost
 */
typedef enum bh_db_lost_e {
    CAPI_BH_DB_LOST_0_9_VSR,        /**< DB lost 0-9 VSR */
    CAPI_BH_DB_LOST_10_19_MR,       /**< DB lost 10-19 MR */
    CAPI_BH_DB_LOST_20_29_LR        /**< DB lost 20-29 LR */
} capi_bh_db_lost_t;                /**< Black Hawk DB Lost type */


/**
 * The enumeration of BH medium
 */
typedef enum bh_medium_e {
    CAPI_BH_MEDIUM_PCB,             /**< PCB medium */
    CAPI_BH_MEDIUM_COPPER_WIRE,     /**< Copper wire medium */
    CAPI_BH_MEDIUM_OPTICAL          /**< Optical medium */
} capi_bh_medium_t;                 /**< BH Medium type */

/**
 * The enumeration of BH Tx
 */
typedef enum bh_tx_e {
    CAPI_BH_tx_ENABLE,      /**< Tx enable*/
    CAPI_BH_tx_DISABLE      /**< Tx disable*/
} capi_bh_tx_t;             /**< Tx type */

/**
 * The enumeration of BH Rx
 */
typedef enum bh_rx_e {
    CAPI_BH_rx_ENABLE,      /**< Rx enable*/
    CAPI_BH_rx_DISABLE      /**< Rx disable*/
} capi_bh_rx_t;             /**< Rx type */

/**
 * The union of line wrapper mode register
 */
typedef union lw_mode_reg_u {
    uint16_t content; /**< Line Wrapper Mode Register content */

    struct {
        #ifdef CAPI_IS_BIG_ENDIAN
        uint16_t optical_mode  : 1;     /**< Optical mode  */
        uint16_t phd_mode      : 3;     /**< Phd Mode      */
        uint16_t eq_tap_sel    : 2;     /**< EQ tap sel    */
        uint16_t eqpp          : 1;     /**< EQPP          */
        uint16_t nldet         : 1;     /**< NLDet         */
        uint16_t eq2           : 1;     /**< EQ2           */
        uint16_t extended_mode : 1;     /**< Extended mode */
        uint16_t r_loop        : 1;     /**< R loop        */
        uint16_t g_loop        : 1;     /**< G loop        */
        uint16_t link_training : 3;     /**< Link training */
        uint16_t auto_neg      : 1;     /**< AutoCL73      */
        #else
        uint16_t auto_neg      : 1;     /**< AutoCL73      */
        uint16_t link_training : 3;     /**< Link training */
        uint16_t g_loop        : 1;     /**< G loop        */
        uint16_t r_loop        : 1;     /**< R loop        */
        uint16_t extended_mode : 1;     /**< Extended mode */
        uint16_t eq2           : 1;     /**< EQ2           */
        uint16_t nldet         : 1;     /**< NLDet         */
        uint16_t eqpp          : 1;     /**< EQPP          */
        uint16_t eq_tap_sel    : 2;     /**< EQ tap sel    */
        uint16_t phd_mode      : 3;     /**< Phd Mode      */
        uint16_t optical_mode  : 1;     /**< Optical mode  */
        #endif
    } is;
} lw_mode_reg_t; /**< Line Wrapper mode Register type */

/**
 * The enumeration of LW optical mode
 */
typedef enum lw_optical_mode_e {
    CAPI_LW_OPTICAL_MODE,   /**< Optical mode */
    CAPI_LW_COPPER_MODE     /**< Copper mode */
} capi_lw_optical_mode_t;   /**< Optical Mode type */

/**
 * The enumeration of LW Phd mode
 */
typedef enum lw_phd_mode_e {
    CAPI_LW_PHD_MODE_PRE_CURSOR,            /**< Pre-cursor */
    CAPI_LW_PHD_MODE_MMSE,                  /**< MMSE */
    CAPI_LW_PHD_MODE_SYMMETRICAL,           /**< Sumetrical */
    CAPI_LW_PHD_MODE_SYMETRICAL_W_PRECODER  /**< Symetrical with precoder */
} capi_lw_phd_mode_t;                       /**< Phd Mode type */

/**
 * The enumeration of EQPP
 */
typedef enum lw_eqpp_e {
    CAPI_LW_EQPP_ENABLE, /**< EQPP enable*/
    CAPI_LW_EQPP_DISABLE /**< EQPP disable*/
} capi_lw_eqpp_t; /**< EQPP type */

/**
 * The enumeration of NLDet
 */
typedef enum lw_nldet_e {
    CAPI_LW_NLDET_ENABLE, /**< NLDet enable*/
    CAPI_LW_NLDET_DISABLE /**< NLDet disable*/
} capi_lw_nldet_t; /**< NLDet type */

/**
 * The enumeration of LW EQ2
 */
typedef enum lw_eq2_e {
    CAPI_LW_EQ2_ENABLE, /**< EQ2 enable*/
    CAPI_LW_EQ2_DISABLE /**< EQ2 disable*/
} capi_lw_eq2_t; /**< EQ2 type */

/**
 * The enumeration of BH slicer mode
 */
typedef enum capi_bh_slicer_mode_e {
    CAPI_BH_SLICER_MODE_NS, /**< NS slicer mode */
    CAPI_BH_SLICER_MODE_ES /**< ES slicer mode */
} capi_bh_slicer_mode_t; /**< slicer mode type */

/**
 * The enumeration of BH precoder
 */
typedef enum bh_precoder_e {
    CAPI_BH_PRECODER_ENABLE, /**< Precoder enable */
    CAPI_BH_PRECODER_DISABLE /**< Precoder disable */
} capi_bh_precoder_t; /**< Precoder type */

/**
 * The enumeration of LW extended mode
 */
typedef enum lw_extended_e {
    CAPI_LW_EXTENDED_MODE_NO_OVERRIDE,      /**< No override */
    CAPI_LW_EXTENDED_MODE_SLICER,           /**< Slicer */
    CAPI_LW_EXTENDED_MODE_REGULAR_SLICER    /**< Regular sliver */
} capi_lw_extended_mode_t;                  /**< Extended Mode type */

/**
 * The enumeration of LW G loop
 */
typedef enum lw_g_loop_e {
    CAPI_LW_G_LOOP_ENABLE, /**< G loop enable */
    CAPI_LW_G_LOO_DISABLE /**< G loop disable */
} capi_lw_g_loop_t; /**< G Loop type */

/**
 * The enumeration of LW R loop
 */
typedef enum lw_r_loop_e {
    CAPI_LW_R_LOOP_ENABLE, /**< R loop enable */
    CAPI_LW_R_LOOP_DISABLE /**< R loop disable */
} capi_lw_r_loop_t; /**< R Loop type */

/**
 * The enumeration of LW link training
 */
typedef enum lw_link_training_e {
    CAPI_LW_LINK_TRAINING_NONE,                 /**< Link training none */
    CAPI_LW_LINK_TRAINING_CL73,                 /**< Link training CL73 */
    CAPI_LW_LINK_TRAINING_802_3CD_NRZ,          /**< Link training 802.3cd NRZ */
    CAPI_LW_LINK_TRAINING_802_3CD_PAM4,         /**< Link training 802.3cd PAM4 */
    CAPI_LW_LINK_TRAINING_802_3CD_PAM4_PRECODER /**< Link training 802.3cd PAM4 precoder */
} capi_lw_link_training_t;                      /**< Link Training type */

/**
 * The enumeration of LW CL73
 */
typedef enum lw_cl73_e {
    CAPI_BH_CL73_ENABLE,    /**< CL73 enable */
    CAPI_BH_CL73_DISABLE    /**< CL73 disable */
} capi_lw_cl73_t;           /**< CL73 type */

/**
 * The enumeration of data rates
 */
typedef enum data_rate_e {
    CAPI_1G_DATA_RATE,      /**< 1 Gbps data rate */
    CAPI_2G_DATA_RATE,      /**< 2 Gbps data rate */
    CAPI_10G_DATA_RATE,     /**< 25 Gbps data rate */
    CAPI_25G_DATA_RATE,     /**< 50 Gbps data rate */
    CAPI_50G_DATA_RATE,     /**< 100 Gbps data rate */
    CAPI_200G_DATA_RATE,    /**< 200 Gbps data rate */
    CAPI_400G_DATA_RATE,    /**< 400 Gbps data rate */
    CAPI_600G_DATA_RATE,    /**< 600 Gbps data rate */
    CAPI_800G_DATA_RATE,    /**< 800 Gbps data rate */
    CAPI_1000G_DATA_RATE,   /**< 1 Tbps data rate */
    CAPI_1200G_DATA_RATE,   /**< 1.2 Tbps data rate */
    CAPI_1600G_DATA_RATE,   /**< 1.6 Tbps data rate */
    CAPI_2000G_DATA_RATE    /**< 2 Tbps data rate */
} capi_data_rate_t;         /**< Data Rate type */

/**
 * The enumeration of firmware image type
 */
typedef enum capi_firmware_image_type_s {
    FIRMWARE_IMAGE_DEFAULT,         /**< default image */
    FIRMWARE_IMAGE_J                /**< image */
} capi_firmware_image_type_t;       /**< fimrware image type */

/**
 * A structure to represent each phy element of the phy list
 */
typedef struct capi_load_phy_list_s {
    uint32_t addr;                          /**< PHY address (PHYAD) used by this PHY */
    capi_firmware_image_type_t image_type;  /**< firmware image type for this PHY */
} capi_load_phy_info_t;                     /**< represent each phy element of the phy list */

/**
 * A structure to represent phy list
 */
typedef struct capi_firmware_load_phy_list_s {
    capi_load_phy_info_t* phy_info;         /**< pointer to the phy list*/
    uint32_t num_of_phy;                    /**< the number of phys in the phy list*/
} capi_firmware_load_phy_list_t;            /**< PHY list */

/**
 * The enumeration of Downloading Mechanism
 */
typedef enum capi_download_mode_e {
    CAPI_DOWNLOAD_MODE_NONE,        /**< Download Mode None */
    CAPI_DOWNLOAD_MODE_MDIO_SRAM,   /**< Download via MDIO to SRAM */
    CAPI_DOWNLOAD_MODE_I2C_SRAM,    /**< Download via I2C to SRAM*/
    CAPI_DOWNLOAD_MODE_MDIO_EEPROM, /**< Download via MDIO to EEPRM*/
    CAPI_DOWNLOAD_MODE_I2C_EEPROM,  /**< Download via I2C to EEPRM*/
} capi_download_mode_t;             /**< Downloading Mode type */

/**
 * The enumeration of SPI Program mode
 */
typedef enum capi_spi_program_mode_e {
    CAPI_SPI_PROGRAM_STANDARD,    /**< stanadard */
    CAPI_SPI_PROGRAM_FAST,        /**< FAST*/
    
} capi_spi_program_mode_t;             /**< spi program mode */

/**
 * The enumeration of SRAM Program mode
 */
typedef enum capi_sram_program_mode_e {
    CAPI_SRAM_PROGRAM_STANDARD,    /**< stanadard */
    CAPI_SRAM_PROGRAM_FAST,        /**< FAST*/
    
} capi_sram_program_mode_t;             /**< sram program mode */
/**
 * A structure to represent the status
 */
typedef struct status_info_s {
    uint32_t version;           /**< Version */
    uint32_t sub_version;       /**< Sub Version */
    uint32_t crc_check;         /**< CRC check */
    boolean  result;            /**< Download Result */
} capi_status_info_t;           /**< Download Status type */

/**
 * A structure to represent firmware image
 */
typedef struct capi_image_info_s {
    uint32_t* image_ptr;
    uint32_t  image_size;
    uint32_t  image_crc;
} capi_image_info_t; /**< firmware info type */


typedef enum capi_default_mode_e {
    CAPI_NO_DEFAULT_MODE = 0,
    CAPI_DEFAULT_MODE_1_REPEATER = 1,  /*CHIP_MODE_2X53G_KP4PAM_TO_1X106G_KP4PAM  CHIP_HOST_FEC_TYPE_NA    CHIP_LINE_FEC_TYPE_NA  CHIP_PORT_FEC_TERM_BYPASS  CHIP_PORT_MUX_BIT_MUX*/
    CAPI_DEFAULT_MODE_1_KP4_KP4  = 2,  /*CHIP_MODE_2X53G_KP4PAM_TO_1X106G_KP4PAM  CHIP_HOST_FEC_TYPE_RS544    CHIP_LINE_FEC_TYPE_RS544  CHIP_PORT_FEC_DEC_ENC  CHIP_PORT_MUX_BIT_MUX*/
    CAPI_DEFAULT_MODE_5_PCS_KP4  = 3,  /*CHIP_MODE_4x25G_NRZ_TO_1X106G_KP4PAM  CHIP_HOST_FEC_TYPE_PCS    CHIP_LINE_FEC_TYPE_RS544  CHIP_LANE_PCS_XENC  CHIP_PORT_MUX_BIT_MUX*/
    CAPI_DEFAULT_MODE_5_KR4_KP4  = 4,  /*CHIP_MODE_4x25G_NRZ_TO_1X106G_KP4PAM  CHIP_HOST_FEC_TYPE_RS528    CHIP_LINE_FEC_TYPE_RS544  CHIP_PORT_FEC_DEC_ENC  CHIP_PORT_MUX_BIT_MUX*/
    CAPI_DEFAULT_MODE_4_NRZ_NRZ_REPEATER  = 5,  /*CHIP_MODE_4x25G_KR4NRZ_TO_4x25G_KR4NRZ  CHIP_HOST_FEC_TYPE_NA    CHIP_HOST_FEC_TYPE_NA  CHIP_PORT_FEC_TERM_BYPASS  CHIP_PORT_MUX_BIT_MUX*/
    CAPI_DEFAULT_MODE_7_REPEATER = 6,  /*CHIP_MODE_8x53G_KP4PAM_TO_4X106G_KP4PAM  CHIP_HOST_FEC_TYPE_NA    CHIP_LINE_FEC_TYPE_NA  CHIP_PORT_FEC_TERM_BYPASS  CHIP_PORT_MUX_BIT_MUX*/
    CAPI_DEFAULT_MODE_7_KP4_KP4 = 7,  /*CHIP_MODE_8x53G_KP4PAM_TO_4X106G_KP4PAM  CHIP_HOST_FEC_TYPE_RS544    CHIP_HOST_FEC_TYPE_RS544  CHIP_PORT_FEC_DEC_ENC  CHIP_PORT_MUX_BIT_MUX*/
    CAPI_DEFAULT_MODE_MAX  = 8,
} capi_default_mode_t;
/**
 * The struct of static_config
 */
typedef struct phy_static_config_s {
    bool avs_enable;
    capi_default_mode_t chip_default_mode;
} phy_static_config_t;

/**
 * Thecapi_burst_write_mode
 */
typedef enum capi_burst_write_mode_e {
    CAPI_BURST_WRITE_MODE_I2C,      
    CAPI_BURST_WRITE_MODE_MDIO,   
    CAPI_BURST_WRITE_MODE_I2C_OPT,
} capi_burst_write_mode_t;         

/**
 * A structure to represent download information
 */
typedef struct download_info_s {
    capi_status_info_t status;                  /**< Download Status */
    capi_download_mode_t mode;                  /**< Download Mode */
    phy_static_config_t* phy_static_config_ptr; /**< static_config */
    capi_image_info_t image_info;
    capi_spi_program_mode_t spi_program_mode;     /**< 0: spi program standard mode 1: fast mode with I2c block write*/
    capi_burst_write_mode_t  burst_write_mode;
    capi_sram_program_mode_t sram_program_mode;     /**< 0: sram program standard mode 1: fast mode with I2c block write*/
} capi_download_info_t;                         /**< Download info type */

/**
 * An enumation to represent download media type
 */
typedef enum download_media_e {
    DOWNLOAD_MEDIA_TYPE_NONE,        /**< Download Media Type None */
    DOWNLOAD_MEDIA_TYPE_I2C,         /**< Download Media Type I2C  */
    DOWNLOAD_MEDIA_TYPE_MDIO,        /**< Download Media Type MDIO */
    DOWNLOAD_MEDIA_TYPE_SPI,         /**< Download Media Type SPI  */
} download_media_t;                  /**< Download Media type      */

/**
 * An enumeration to represent download mode
 */
typedef enum download_mode_e {
    DOWNLOAD_MODE_STANDARD,          /**< Download Mode Standard   */
    DOWNLOAD_MODE_BURST              /**< Download Mode Burst      */
} download_mode_t;                   /**< Download Mode type       */

/**
 * An enumeration to represent download media type
 */
typedef enum memory_type_e {
    MEMORY_TYPE_NONE,                /**< Memory Type None                        */
    MEMORY_TYPE_SRAM,                /**< Memory Type Internal SRAM               */
    MEMORY_TYPE_FLASH,               /**< Memory Type External FLASH              */
    MEMORY_TYPE_FLASH_HITLESS,       /**< Memory Type External FLASH with HITLESS */
    MEMORY_TYPE_FLASH_VIA_SRAM,      /**< Memory Type External FLASH VIA SRAM     */
    MEMORY_TYPE_EEPROM,              /**< Memory Type External EEPROM; NA         */
} memory_type_t;                     /**< Memory type                             */


/**
 * A structure to represent firmware download information
 */
typedef struct capi_fw_download_info_s {
    uint8_t  mode;            /**< Mode; refer to download_mode_t           */
    uint8_t  media_type;      /**< Media type; refer to download_media_t    */
    uint16_t memory_type;     /**< Memory type; refer to memory_type_t      */

    struct {
        uint32_t* src_ptr;    /**< pointer to the upgraded image            */
        uint32_t  size;       /**< image size, in bytes                     */
        uint32_t  crc;        /**< image crc                                */
    } image;                  /**< Image information                        */

    struct {
        uint8_t   location;   /**< 0: bank A, 1: bank B                     */
        uint8_t   commit;     /**< 0: not commit, 1: commit                 */
        uint16_t  trial_run;  /**< 0: normal, 1: trial run                  */
    } image_bank;             /**< Bank information                         */

} capi_fw_download_info_t;    /**< Firmware Download Info type              */

#if 0
/**
 * A structure to represent firmware download information
 */
typedef struct capi_fw_download_info_s {
    uint32_t* image_ptr;        /**< pointer to the upgraded image  */
    uint32_t  image_size;       /**< image size, in bytes           */
    uint32_t  image_crc;        /**< image crc                      */
    uint32_t  image_bank;       /**< 0: bank A, 1: bank B           */
    uint32_t  commit;           /**< 0: not commit, 1: commit       */
    uint32_t mode;              /**< 0: normal, 1: trial run        */
} capi_fw_download_info_t;   /**< Firmware Download Info type       */
#endif

/**
 * enumeration to represent the cmis image administrative status
 */
typedef enum capi_cmis_ad_status_e {
    AD_STATUS_UNCOMMITED = 0,
    AD_STATUS_COMMITTED  = 1,
} capi_cmis_ad_status_t; /**< CAPI cmis ad status type */

/**
 * enumeration to represent the cmis image operational status
 */
typedef enum capi_cmis_op_status_e {
    OP_STATUS_NOT_RUNNING = 0,
    OP_STATUS_RUNNING     = 1,
} capi_cmis_op_status_t; /**< CAPI cmis op status type */

/**
 * enumeration to represent the cmis image validity status
 */
typedef enum capi_cmis_va_status_e {
    VA_STATUS_VALID    = 0,
    VA_STATUS_INVALID  = 1,
} capi_cmis_va_status_t; /**< CAPI cmis va status type */

/**
 * The enumeration to represent firmware status bank
 */
typedef enum capi_firmware_status_bank_e {
    FIRMWARE_STAUS_NONE_BANK_0X00_X_X_AB = 0x00,                                 /**< This may be returned by a module that does not
                                                                                      support firmware update and only has the factory image.
                                                                                      A module that supports image A and B may return these
                                                                                      codes when the module’s boot control block that defines
                                                                                      which image Bank to boot is damaged or when a default
                                                                                      factory image is running.                                                    */
    FIRMWARE_STAUS_NONE_BANK_0X40_X_X_AX = 0x40,                                 /**< A module that supports image A, B, or A and B may return
                                                                                      these codes when something has damaged the modules boot
                                                                                      A module that supports image A and B may return these
                                                                                      control block defining the image Bank to boot, or a default
                                                                                      factory image is running                                                     */
    FIRMWARE_STAUS_NONE_BANK_0X04_X_X_XB = 0x04,                                 /**< A module that supports image A, B, or A and B may return
                                                                                      these codes when something has damaged the modules boot
                                                                                      A module that supports image A and B may return these
                                                                                      control block defining the image Bank to boot, or a default
                                                                                      factory image is running                                                    */
    FIRMWARE_STAUS_NONE_BANK_0X44_X_X_XX = 0x44,                                 /**< A module that supports image A, B, or A and B may return
                                                                                      these codes when something has damaged the modules boot
                                                                                      A module that supports image A and B may return these
                                                                                      control block defining the image Bank to boot, or a default
                                                                                      factory image is running                                                    */
    FIRMWARE_STAUS_SNGL_BANK_0X44_X_X_XX = FIRMWARE_STAUS_NONE_BANK_0X44_X_X_XX, /**< Factory image or boot loader running, no valid image available              */
    FIRMWARE_STAUS_SNGL_BANK_0X40_X_X_AX = FIRMWARE_STAUS_NONE_BANK_0X40_X_X_AX, /**< Factory image or boot loader running, valid uncommitted image available     */
    FIRMWARE_STAUS_SNGL_BANK_0X42_X_A_AX = 0x42,                                 /**< Factory image or boot loader running, valid committed image available       */
    FIRMWARE_STAUS_SNGL_BANK_0X41_A_X_AX = 0x41,                                 /**< Running an uncommitted image                                                */
    FIRMWARE_STAUS_SNGL_BANK_0X43_A_A_AX = 0x43,                                 /**< Running a committed image                                                   */
    FIRMWARE_STAUS_DUAL_BANK_0X03_A_A_AB = 0x03,                                 /**< Normal Operation. A running and A committed.
                                                                                      Image B valid, can be activated by Run B command                            */
    FIRMWARE_STAUS_DUAL_BANK_0X43_A_A_AX = FIRMWARE_STAUS_SNGL_BANK_0X43_A_A_AX, /**< Normal Operation. A running and A committed. Image B update is in progress,
                                                                                       has failed or has been aborted. There is no valid image in Bank B.         */
    FIRMWARE_STAUS_DUAL_BANK_0X12_B_A_AB = 0x12,                                 /**< Trial run of image in Bank B                                                */
    FIRMWARE_STAUS_DUAL_BANK_0X16_B_A_XB = 0x16,                                 /**< Errored Mode (Bank A found invalid). Image in Bank A is committed but
                                                                                      invalid. The module is running the valid image in Bank B.                   */
    FIRMWARE_STAUS_DUAL_BANK_0X30_B_B_AB = 0x30,                                 /**< Normal Operation. B running and B committed. Image A valid,
                                                                                      can be activated by Run A command.                                          */
    FIRMWARE_STAUS_DUAL_BANK_0X34_B_B_XB = 0x34,                                 /**< Normal Operation. B running and B committed. Image A update in progress,
                                                                                      failed or aborted. There is no valid image in Bank A                        */
    FIRMWARE_STAUS_DUAL_BANK_0X21_A_B_AB = 0x21,                                 /**< Trial run of image in Bank A.                                               */
    FIRMWARE_STAUS_DUAL_BANK_0X61_A_B_AX = 0x61,                                 /**< Errored Mode (Bank B found invalid). Image in Bank B is committed
                                                                                      but invalid. The module is running the valid image in Bank A.               */
} capi_firmware_status_bank_t;                                                   /**< Firmware Status Bank type                                                   */

/**
 * A structure to represent firmware image status
 */
typedef union firmware_image_status_u {
    uint32_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint32_t reserved   : 21; /**< reserved                                          */
        uint32_t va_status_f: 1;  /**< validity status: 0 invalid, 1 valid               */
        uint32_t ad_status_f: 1;  /**< administrative status: 0 not commited, 1 commited */
        uint32_t op_status_f: 1;  /**< operational status: 0 not running, 1 running      */
        uint32_t trial_run_b: 1;  /**< trial run status: 0 default, 1 trial run          */
        uint32_t va_status_b: 1;  /**< validity status: 0 invalid, 1 valid               */
        uint32_t ad_status_b: 1;  /**< administrative status: 0 not commited, 1 commited */
        uint32_t op_status_b: 1;  /**< operational status: 0 not running, 1 running      */
        uint32_t trial_run_a: 1;  /**< trial run status: 0 default, 1 trial run          */
        uint32_t va_status_a: 1;  /**< validity status: 0 invalid, 1 valid               */
        uint32_t ad_status_a: 1;  /**< administrative status: 0 not commited, 1 commited */
        uint32_t op_status_a: 1;  /**< operational status: 0 not running, 1 running      */
#else /* LITTLE ENDIAN */
        uint32_t op_status_a: 1;  /**< operational status: 0 not running, 1 running      */
        uint32_t ad_status_a: 1;  /**< administrative status: 0 not commited, 1 commited */
        uint32_t va_status_a: 1;  /**< validity status: 0 invalid, 1 valid               */
        uint32_t trial_run_a: 1;  /**< trial run status: 0 default, 1 trial run          */
        uint32_t op_status_b: 1;  /**< operational status: 0 not running, 1 running      */
        uint32_t ad_status_b: 1;  /**< administrative status: 0 not commited, 1 commited */
        uint32_t va_status_b: 1;  /**< validity status: 0 invalid, 1 valid               */
        uint32_t trial_run_b: 1;  /**< trial run status: 0 default, 1 trial run          */
        uint32_t op_status_f: 1;  /**< operational status: 0 not running, 1 running      */
        uint32_t ad_status_f: 1;  /**< administrative status: 0 not commited, 1 commited */
        uint32_t va_status_f: 1;  /**< validity status: 0 invalid, 1 valid               */
        uint32_t reserved   : 21; /**< reserved                                          */
#endif
    } fields;
} firmware_image_status_t;

typedef struct firmware_image_info_s {
    uint32_t version;           /**< Version */
    uint32_t sub_version;       /**< Sub Version */
    uint32_t crc_check;         /**< CRC check */
    boolean  result;            /**< Download Result */
} firmware_image_version_t;           /**< Download Status type */

/**
 * Status information type enumeration
 */
typedef enum status_info_type_e {
    STATUS_INFO_TYPE_NONE = 0,                               /**< Status Information Type None                                   */
    STATUS_INFO_TYPE_FW_IMAGE_STATUS_INFO,                   /**< Status Information Type FW Image Status                        */
} status_info_type_t;                                        /**< Status Information type                                        */

/**
 * The structure of module status information
 */
typedef struct capi_status_info_s {
    uint32_t   status_info_type;                             /**< Status Info type, refer status_info_type_t                      */
    union {
        firmware_image_status_t   fw_image_status;           /**< Firmware Image Status                                           */
        firmware_image_version_t  fw_image_version;          /**< Firmware Image Version                                          */
    } type;                                                  /**< Type                                                            */
} capi_status_info_tt;                                       /**< CAPI Module Status Information type: temporary                  */

/**
 * A structure to represent firmware upgrade interface
 */
typedef union chip_top_spi_upgrade_command_f_u {
    uint32_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint32_t status_ctrl    : 1; /**< status control */
        uint32_t start_addr     : 24;/**< start address */
        uint32_t block_id       : 3; /**< block_id*/
        uint32_t error_status   : 2; /**< error_status */
        uint32_t buffer_id      : 1; /**< buffer_id */
        uint32_t command_change : 1; /**< command_change */
#else /* LITTLE ENDIAN */
        uint32_t command_change : 1; /**<command_change */
        uint32_t buffer_id      : 1; /**< buffer_id */
        uint32_t error_status   : 2; /**< error_status */
        uint32_t block_id       : 3; /**< block_id */
        uint32_t start_addr     : 24;/**< start address */
        uint32_t status_ctrl    : 1; /**< status control */
#endif
    } fields;
} chip_top_spi_upgrade_command_f_t;

/**
 * A structure to represent firmware upgrade interface
 */
typedef union chip_top_spi_upgrade_buffer_status_t_u {
    uint32_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint32_t reserved   : 24; /**< reserved */
        uint32_t mode       : 1;  /**< mode: 0 normal, 1 trial run */
        uint32_t commit     : 1;  /**< commit: 0 not commit, 1 commit */
        uint32_t result     : 4;  /**< result: 1 successful, 0 failed */
        uint32_t data_ready : 2;  /**< data_ready: 0 not rdy, 1 rdy, 2 terminating data */
#else /* LITTLE ENDIAN */
        uint32_t data_ready : 2;  /**< data_ready: 0 not rdy, 1 rdy, 2 terminating data */
        uint32_t result     : 4;  /**< result: 1 successful, 0 failed */
        uint32_t commit     : 1;  /**< commit: 0 not commit, 1 commit */
        uint32_t mode       : 1;  /**< mode: 0 normal, 1 trial run */
        uint32_t reserved   : 24; /**< reserved*/
#endif
    } fields;
} chip_top_spi_upgrade_buffer_status_t;

/**
 * The enumeration of access bus interface
 */
typedef enum capi_bus_intf_e {
    CAPI_ACCESS_MODE_MDIO = 0,
    CAPI_ACCESS_MODE_I2C
} capi_bus_intf_t;

typedef enum broadcast_enable_e {
    BROADCAST_DISABLE = 0,
    BROADCAST_ENABLE = 1,
} broadcast_enable_t;

typedef struct capi_access_intf_s {
    capi_bus_intf_t bus;
    broadcast_enable_t enable;
} capi_broadcast_intf_t;

/**
 * The enumeration of reset modes
 */
typedef enum reset_mode_e {
    CAPI_HARD_RESET_MODE,       /**< Hard reset mode */
    CAPI_SOFT_RESET_MODE,        /**< Soft reset mode */
    CAPI_RESET_BRING_UP_MODE,    /**< reset bringup mode */
    CAPI_HARD_RESET_MODE_IMMEDIATE  /**< Hard reset mode without delay */
} capi_reset_mode_t;            /**< Reset mode type */

/**
 * The enumeration of interface modes
 */
typedef enum interface_mode_e {
    CAPI_CR_INTERFACE_TYPE,         /**< CR interface type */
    CAPI_KR_INTERFACE_TYPE,         /**< KR interface type */
    CAPI_XX_INTERFACE_TYPE          /**< Count reset type */
} capi_interface_t;                 /**< Interface type */

/**
 * The enumeration of interface mode
 */
typedef enum capi_interface_mode_e {
    CAPI_IEEE_INTERFACE_MODE,       /**< IEEE interface mode */
    CAPI_OTN_INTERFACE_MODE,        /**< OTN interface mode */
    CAPI_HIGIG_INTERFACE_MODE       /**< HIGIG interface mode */
} capi_interface_mode_t;            /**< Interface Mode type */


typedef struct capi_reg_info_s {
    uint32_t reg_address;       /**< Register Address */
    uint32_t content;           /**< Register Value */
} capi_reg_info_t;

/*GP interface interface*/
typedef union capi_lane_status_u {
    uint8_t bytes;

    #ifdef CAPI_IS_BIG_ENDIAN
    struct {
        uint8_t linktrn_err       : 2;   /**< (i) LW channel status: 1-link training error         */
        uint8_t linktrn_done      : 1;   /**< (i) LW channel status: 1-link training done          */
        uint8_t cdrlock           : 1;   /**< (i) LW channel status: 1-CDR locked                  */
        uint8_t tx_ready          : 1;   /**< (i) LW channel status: TX ready                      */
        uint8_t adcclip           : 1;   /**< (i) LW channel status: 1-Dectect the clipping of ADC */
        uint8_t los               : 1;   /**< (i) LW channel status: 1-LOS                         */
        uint8_t plllock           : 1;   /**< (i) LW channel status: 1-PLL locked                  */
    } lw;

    struct {
        uint8_t reserved          : 3;   /**< Reserved                                             */
        uint8_t bh2bh_fifo_status : 1;   /**< BH 2 BH FIFO Status                                  */
        uint8_t pll_unlock_sticky : 1;   /**< PLL Unlock Sticky                                    */
        uint8_t cdr_lock          : 1;   /**< (i) CLIENT channel status: 1-CDR locked              */
        uint8_t los               : 1;   /**< (i) CLIENT channel status: 1-LOS                     */
        uint8_t pll_lock          : 1;   /**< (i) CLIENT channel status: 1-PLL locked              */
    } client;

    #else /* CAPI_IS_LITTLE_ENDIAN */

    struct {
        uint8_t plllock           : 1;   /**< (i) LW channel status: 1-PLL locked                  */
        uint8_t los               : 1;   /**< (i) LW channel status: 1-LOS                         */
        uint8_t adcclip           : 1;   /**< (i) LW channel status: 1-Dectect the clipping of ADC */
        uint8_t tx_ready          : 1;   /**< (i) LW channel status: TX ready                      */
        uint8_t cdrlock           : 1;   /**< (i) LW channel status: 1-CDR locked                  */
        uint8_t linktrn_done      : 1;   /**< (i) LW channel status: 1-link training done          */
        uint8_t linktrn_err       : 2;   /**< (i) LW channel status: 1-link training error         */
    } lw;

    struct {
        uint8_t pll_lock          : 1;   /**< (i) CLIENT channel status: 1-PLL locked              */
        uint8_t los               : 1;   /**< (i) CLIENT channel status: 1-LOS                     */
        uint8_t cdr_lock          : 1;   /**< (i) CLIENT channel status: 1-CDR locked              */
        uint8_t pll_unlock_sticky : 1;   /**< BH 2 BH FIFO Status                                  */
        uint8_t bh2bh_fifo_status : 1;   /**< PLL Unlock Sticky                                    */
        uint8_t reserved          : 3;   /**< Reserved                                             */
    } client;
    #endif /* CAPI_IS_BIG_ENDIAN */
} capi_lane_status_t;

/**
 * The structure contain the chip level information
 */
typedef struct capi_chip_info_s {
    uint32_t chip_id;               /**< Identification                     */
    uint32_t chip_revision;         /**< Revision                           */
    uint32_t rev_id;                /**< Rev ID                             */
    uint16_t capi_major_version;    /**< cAPI major version                 */
    uint16_t capi_minor_version;    /**< cAPI minor version                 */
    uint16_t fw_major_version;      /**< fw major version loaded on the phy */
    uint16_t fw_minor_version;      /**< fw minor version loaded on the phy */
    uint16_t upgrade_fw_major_version; /**< Upgrade fw major version standby */
    uint16_t upgrade_fw_minor_version; /**< Upgrade fw minor version standby */
} capi_chip_info_t;                 /**< Chip Information type */

/**
 * The enumeration of core version
 */
typedef enum capi_core_version_e {
    CORE_VERSION_A0,        /**< A0 Version */
    CORE_VERSION_NONE
} capi_core_version_t;      /**< Core Version type */

/**
 * The structure of core info
 */
typedef struct capi_core_info_s {
    capi_core_version_t core_version;   /* core version */
    uint32_t serdes_id;                 /* serdes_id */
    uint32_t phy_id0;                   /* phy_id0 */
    uint32_t phy_id1;                   /* phy_id1 */
    char name[30];
} capi_core_info_t;                     /**< Core Info type */


/**
 * The structure of chip status information
 */
typedef struct capi_chip_status_info_s {
    union {
        uint8_t content;                         /**< The content                  */
        struct{
            uint8_t uc_ready      : 1;           /**< Micro Code Ready flag        */
            uint8_t avs_done      : 1;           /**< AVS Ready flag               */
            uint8_t download_done : 1;           /**< Download flag                */
            uint8_t spi_dev_ready : 1;           /**< SPI Device Ready flag        */
            uint8_t reserved      : 4;           /**< Reserved                     */
        } is;
    }param;

    struct{
        uint8_t uc_ready      : 1;               /**< Micro Code Ready value       */
        uint8_t avs_done      : 1;               /**< AVS Ready value              */
        uint8_t download_done : 1;               /**< Download value               */
        uint8_t spi_dev_ready : 1;               /**< SPI Device Ready value       */
        uint8_t reserved      : 4;               /**< Reserved                     */
    } value;
} capi_chip_status_info_t;                       /**< Chip Status Information type */


typedef enum client_rx_tx_e {
    CLIENT_RX,
    CLIENT_TX,
    CLIENT_MAX
} client_rx_tx_t;

typedef enum bh_rx_tx_e {
    LW_RX,
    LW_TX,
    LW_MAX
} lw_rx_tx_t;

/**
 * The enumeration of operation modes
 */
typedef enum {
    CAPI_R_INCOMPATIBLE=0,
    CAPI_R_IEEE_10G_KR=1,
    CAPI_R_IEEE_40G_KR4=2,
    CAPI_R_IEEE_40G_CR4=3,
    CAPI_R_IEEE_100G_KR4=4,
    CAPI_R_IEEE_100G_CR4=5,
    CAPI_R_IEEE_25G_CRSKRS=6,
    CAPI_R_IEEE_25G_CR1KR1=7,
    CAPI_R_C50G_25G_CR1KR1=8,
    CAPI_R_C50G_50G_CR2KR2=9,
    CAPI_R_BRCM_50G_KP1=10,
    CAPI_R_BRCM_100G_KP2=11,
    CAPI_R_BRCM_200G_KP4=13
} capi_lw_an_req_t;

/**
 * The enumeration of AVS Control
 */
typedef enum capi_avs_ctrl_e {
    CAPI_INTERNAL_AVS_CONTROL,      /**< Internal AVS Control */
    CAPI_EXTERNAL_AVS_CONTROL       /**< External Chip controlls the AVS */
} capi_avs_ctrl_t;                  /**< AVS Control Type */

/**
 * The enumeration of status
 */
typedef enum avs_result_e {
    CAPI_INIT_NOT_STARTED = 0,
    CAPI_INIT_DONE_SUCCESS = 1,     /**< 1 : init success */
    CAPI_INIT_ERROR = 2,            /**< 2 : int fail  */
} avs_result_t;                     /**<  periodical success faile */


/**
 * The enumeration of status
 */
typedef struct capi_avs_status_e {
    avs_result_t avs_result;       /**< AVS status result    */
    uint8_t      avs_enable;       /**< AVS Enabled/Disabled */
} capi_avs_status_t;               /**< AVS Status type      */

/**
 * The structure of AVS Voltage information
 */
typedef struct capi_avs_voltage_info_s {
    uint32_t voltage;               /**< AVS voltage controller    */
} capi_avs_voltage_info_t;          /**< AVS mode config info type */

/**
 * The enumeration of set volatge with or without fw
 */
typedef enum capi_set_voltage_with_fw_e {
    CAPI_SET_VOLTAGE_WITH_FIRMWARE = 0,     /**< Set Voltage with FW      */
    CAPI_SET_VOLTAGE_NO_FIRMWARE = 1        /**< Set Voltage No FW        */
} capi_set_voltage_with_fw_t;               /**< Set Voltage with FW type */


/**
 * The structure of fixed vddm mode configuration information
 */
typedef struct capi_vddm_voltage_config_info_s {
    capi_enable_t              fixed_voltage_enable;      /**< Fixed AVS voltage enable/disable     */
    uint16_t                   fixed_voltage_set;         /**< Fixed AVS voltage setting            */
    uint16_t                   fixed_voltage_read;        /**< Fixed AVS voltage reading            */
    capi_set_voltage_with_fw_t capi_set_voltage_with_fw;  /**< Set voltage with fw                  */ 
} capi_other_voltage_config_info_t;                       /**< Fixed vddm _voltage config info type */

/**
 * The enumeration of slave enum
 */
typedef enum capi_disable_slave_enum_e {
    CAPI_DISABLE_1_SLAVE = 0,     /**< disable 1 slave      */
    CAPI_DISABLE_2_SLAVE = 1,        /**< disable 2 slave        */
    CAPI_DISABLE_3_SLAVE = 2,        /**< disable 3 slave        */
    CAPI_DISABLE_ALL_SLAVE = 3        /**< disable 4 slave        */
} capi_disable_slave_enum_t;               /**< disable slaves */

/**
 * The enumeration of slave enum
 */
typedef enum capi_enable_slave_enum_e {
    CAPI_ENAABLE_1_SLAVE = 0,     /**< enable 1 slave      */
    CAPI_ENAABLE_2_SLAVE = 1,        /**< enable 2 slave        */
    CAPI_ENAABLE_3_SLAVE = 2,        /**<  enable 3 slave        */
    CAPI_ENAABLE_4_SLAVE = 3,        /**< enable 4 slave        */
    CAPI_ENABLE_ALL_SLAVE = 4        /**< enable 4 slave        */
} capi_enable_slave_enum_t;               /**< enable slaves */




/**
 * The structure of disable_avs slave information
 */
typedef struct capi_disable_avs_slave_info_s {
    capi_disable_slave_enum_t slave_enum;
    capi_set_voltage_with_fw_t capi_set_voltage_with_fw;  /**< Set voltage with fw                  */ 
} capi_disable_avs_slave_info__t;                       /**< Fixed vddm _voltage config info type */

/**
 * The structure of avdd slave information
 */
typedef struct capi_disable_avdd_info_s {    
    uint32_t disable_or_eable ;                            /**< 0: disable 1: enable                  */ 
    capi_set_voltage_with_fw_t capi_set_voltage_with_fw;  /**< Set voltage with fw                  */ 
} capi_disable_avdd_slave_info__t;                       /**< Fixed vddm _voltage config info type */

/**
 * The structure of vddm slave information
 */
typedef struct capi_disable_vddm_info_s {    
    uint32_t disable_or_eable ;                            /**< 0: disable 1: enable                  */ 
    capi_set_voltage_with_fw_t capi_set_voltage_with_fw;  /**< Set voltage with fw                  */ 
} capi_disable_vddm_slave_info__t;                       /**< Fixed vddm _voltage config info type */

/**
 * The structure of enable_avs slave information
 */
typedef struct capi_enable_avs_slave_info_s {
    capi_set_voltage_with_fw_t capi_set_voltage_with_fw;  /**< Set voltage with fw                  */ 
    capi_enable_slave_enum_t slave_enum;
} capi_enable_avs_slave_info__t;                       /**< Fixed vddm _voltage config info type */




/**
 * The union of chip voltage status register
 */
typedef union chip_voltage_status_reg_u {
    uint16_t voltage;                      /**< AVS Voltage                         */
    capi_check_type_t check_type;
} capi_voltage_status_info_t;              /**< AVS Voltage Status Information type */


/**< AVS mode config info type */
/**
 * The enumeration of AVS number of packages share AVS
 */
typedef enum capi_avs_pack_share_e {
    CAPI_1_PACKAGE_SHARE_AVS,   /**< 1 : Package share AVS */
    CAPI_2_PACKAGE_SHARE_AVS,   /**< 2 : Packages share AVS */
    CAPI_3_PACKAGE_SHARE_AVS,   /**< 3 : Packages share AVS */
    CAPI_4_PACKAGE_SHARE_AVS,   /**< 4 : Packages share AVS */
    CAPI_5_PACKAGE_SHARE_AVS,   /**< 5 : Packages share AVS */
    CAPI_6_PACKAGE_SHARE_AVS,   /**< 6 : Packages share AVS */
    CAPI_7_PACKAGE_SHARE_AVS,   /**< 7 : Packages share AVS */
    CAPI_8_PACKAGE_SHARE_AVS    /**< 8 : Packages share AVS */
} capi_avs_pack_share_t;        /**< AVS Control Type */

/**
 * The enumeration of AVS board DC margin
 */
typedef enum capi_board_dc_margin_e {
    CAPI_0_MV_MARGIN_ADDED_ON_TOP, /**<  0 :  0 mV margin added on top for board IR drop */
    CAPI_1_MV_MARGIN_ADDED_ON_TOP, /**<  1 :  5 mV margin added on top for board IR drop */
    CAPI_2_MV_MARGIN_ADDED_ON_TOP, /**<  2 : 10 mV margin added on top for board IR drop */
    CAPI_3_MV_MARGIN_ADDED_ON_TOP, /**<  3 : 15 mV margin added on top for board IR drop */
    CAPI_4_MV_MARGIN_ADDED_ON_TOP, /**<  4 : 20 mV margin added on top for board IR drop */
    CAPI_5_MV_MARGIN_ADDED_ON_TOP, /**<  5 : 25 mV margin added on top for board IR drop */
    CAPI_6_MV_MARGIN_ADDED_ON_TOP, /**<  6 : 30 mV margin added on top for board IR drop */
    CAPI_7_MV_MARGIN_ADDED_ON_TOP, /**<  7 : 35 mV margin added on top for board IR drop */
    CAPI_8_MV_MARGIN_ADDED_ON_TOP, /**<  8 : 40 mV margin added on top for board IR drop */
    CAPI_9_MV_MARGIN_ADDED_ON_TOP, /**<  9 : 45 mV margin added on top for board IR drop */
    CAPI_10_MV_MARGIN_ADDED_ON_TOP, /**< 10 : 50 mV margin added on top for board IR drop */
    CAPI_11_MV_MARGIN_ADDED_ON_TOP, /**< 11 : 55 mV margin added on top for board IR drop */
    CAPI_12_MV_MARGIN_ADDED_ON_TOP, /**< 12 : 60 mV margin added on top for board IR drop */
    CAPI_13_MV_MARGIN_ADDED_ON_TOP, /**< 13 : 65 mV margin added on top for board IR drop */
    CAPI_14_MV_MARGIN_ADDED_ON_TOP, /**< 14 : 70 mV margin added on top for board IR drop */
    CAPI_15_MV_MARGIN_ADDED_ON_TOP, /**< 15 : 75 mV margin added on top for board IR drop */
   
} capi_board_dc_margin_t;           /**< AVS Board DC Margin type */

/**
 * The enumeration of AVS number of packages share AVS
 */
typedef enum capi_voltage_ctrl_e {
    CAPI_VOLTAGE_INCREASE = -1,         /**< 1 : Increase voltage */
    CAPI_VOLTAGE_NO_CHANGE = 0,         /**< 2 : No change */
    CAPI_VOLTAGE_DECREASE = 1           /**< 3 : Decrease voltage */
} capi_voltage_ctrl_t;                  /**< Voltage Controller type */

/**< AVS regulator type */
/**
 * The enumeration of AVS regulator type
 */
typedef enum capi_type_of_regulator_e {
    CAPI_REGULATOR_MAX77812 = 0,        /**< 0 :CAPI_REGULATOR_MAX77812 */
    CAPI_REGULATOR_ISL91212 = 1,        /**< 1 :CAPI_REGULATOR_ISL91212 */
    CAPI_REGULATOR_TPS6287  = 2,        /**< 2 :CAPI_REGULATOR_TPS6287  */
    CAPI_REGULATOR_INTERNAL = 7,        /**< 7: CAPI_REGULATOR_INTERNAL */
} capi_type_of_regulator_t;             /**< AVS Regulator Type         */

/**
 * The enumeration of avs disable
 */
typedef enum capi_avs_disable_type_e {
    CAPI_AVS_DISABLE_FIRMWARE_CONTROL = 0,              /*!< firmware set back volatge */
    CAPI_AVS_DISABLE_NO_FIRMWARE_CONTROL = 1            /*!<no firmware */
} capi_avs_disable_type_t;                              /*!< Enable/Disable type */

/**
 * The enumeration of external control step
 */
typedef enum capi_avs_external_ctrl_step_e {
    CAPI_AVS_EXTERNAL_CONTROL_STEP_1 = 0,       /*!< step 1*/
    CAPI_AVS_EXTERNAL_CONTROL_STEP_N = 1        /*!<step N */
} capi_avs_external_ctrl_step_t;                /*!< external control step */

/**
 * The structure of AVS mode configuration information
 */
typedef struct capi_avs_mode_config_info_s {
    capi_enable_t avs;                                  /**< AVS enable/disable */
    capi_avs_disable_type_t disable_type;               /**< AVS disable either no firmware control or firmware control*/
    capi_avs_ctrl_t avs_ctrl;                           /**< AVS control */
    capi_avs_pack_share_t package_share;                /**< AVS number of package share AVS */
    capi_board_dc_margin_t board_dc_margin;             /**< Board DC Margin */
    capi_type_of_regulator_t type_of_regulator;         /**< different type of regulator */
    uint8_t   i2c_slave_address[3];                     /**< i2c slave addresses */
    capi_avs_external_ctrl_step_t external_ctrl_step;   /**< avs external ctrl step */
    uint8_t regulator_i2c_address;                      /**< regulator i2c address */
} capi_avs_mode_config_info_t;                          /**< AVS mode config info type */


/**
 * The struct of QSFP I2c slave address 
 */
typedef struct capi_i2c_slave_info_u {
    uint8_t i2c_address;                      /**< i2c_address                         */
} capi_i2c_info_t;              /**< i2c_address info */


typedef struct lane_txpi_override_s {
    uint8_t txpi_override_en;       /**< txpi_override_en */
    int16_t ppm;                    /**< ppm value */
} lane_txpi_override_t;             /**< LANE TXPI OVERRIDE structure */

/**
 * Interface Lane Status Enumeration
 */
typedef enum {
    INTF_LANE_ST_IDLE,                           /**< No port configured                                                  */
    INTF_LANE_ST_IN_PROGRESS,                    /**< Configuration is in progress                                        */
    INTF_LANE_ST_READY,                          /**< Port configured successfully and ready to run                       */
    INTF_LANE_ST_FAIL                            /**< Port configured but failed with either PLL_LOL or ADC_CAL_DONE_FAIL */
} capi_intf_lane_st_t;                           /**< Interface Lane Status type                                          */


typedef enum {
    CAPI_TWENTYFIVEGBASE_KRCR = 1<<13,
    CAPI_TWENTYFIVEGBASE_KRSCRS = 1<<12,
    CAPI_ONEHUNDREDGBASE_CR4 = 1<<11,
    CAPI_ONEHUNDREDGBASE_KR4 = 1<<10,
    CAPI_ONEHUNDREDGBASE_KP4 = 1<<9,
    CAPI_ONEHUNDREDGBASE_CR10 = 1<<8,
    CAPI_RSFEC_NEGOTIATED = 1<<7,
    CAPI_FORTYGBASE_CR4 = 1<<6,
    CAPI_FORTYGBASE_KR4 = 1<<5,
    CAPI_KRFEC_NEGOTIATED = 1<<4,
    CAPI_TENGBASE_KR = 1<<3,
    CAPI_TENGBASE_KX4 = 1<<2,
    CAPI_GIGBASE_KX = 1<<1,
    CAPI_BP_AN_ABILITY = 1<<0
} capi_an_ieee_mode_t;

typedef enum {
    CAPI_C50G_25G_CR1KR1 = 1 << 0,
    CAPI_C50G_50G_CR2KR2 = 1 << 1,
    CAPI_BRCM_50G_KP1 = 1 << 2,
    CAPI_BRCM_100G_KP2 = 1 << 3,
    CAPI_BRCM_200G_CR8 = 1 << 4,
    CAPI_BRCM_200G_KP4 = 1 << 5
} capi_an_c50G_brcm_mode;

typedef enum {
    CAPI_AN_FEC_NO_FEC = 0,
    CAPI_AN_FEC_IEEE10G40G_CAPABLE = 1<<2,
    CAPI_AN_FEC_IEEE10G40G_REQUESTED = 1<<3,
    CAPI_AN_FEC_IEEE25G_RSFEC_REQUESTED = 1<<0,
    CAPI_AN_FEC_IEEE25G_BASERFEC_REQUESTED = 1<<1,
    CAPI_AN_FEC_CG50_RSFEC_CAPABLE = 1<<0,
    CAPI_AN_FEC_CG50_FIRECODEFEC_CAPABLE = 1<<1,
    CAPI_AN_FEC_CG50_RSFEC_REQUESTED = 1<<2,
    CAPI_AN_FEC_CG50_FIRECODEFEC_REQUESED = 1<<3,
    CAPI_AN_FEC_BRCM_RS_544_512_REQUESTED = 1<<2,
    CAPI_AN_FEC_BRCM_RS_544_512_CAPABLE = 1<<3,
} capi_an_fec_t;

typedef enum capi_phy_interface_e {
    CAPI_INTERFACE_Bypass = 0,
    CAPI_INTERFACE_SR,
    CAPI_INTERFACE_SR4,
    CAPI_INTERFACE_KX,
    CAPI_INTERFACE_KX4,
    CAPI_INTERFACE_KR,
    CAPI_INTERFACE_KR2,
    CAPI_INTERFACE_KR4,
    CAPI_INTERFACE_CX,
    CAPI_INTERFACE_CX2,
    CAPI_INTERFACE_CX4,
    CAPI_INTERFACE_CR,
    CAPI_INTERFACE_CR2,
    CAPI_INTERFACE_CR4,
    CAPI_INTERFACE_CR10,
    CAPI_INTERFACE_XFI,
    CAPI_INTERFACE_SFI,
    CAPI_INTERFACE_SFPDAC,
    CAPI_INTERFACE_XGMII,
    CAPI_INTERFACE_1000X,
    CAPI_INTERFACE_SGMII,
    CAPI_INTERFACE_XAUI,
    CAPI_INTERFACE_RXAUI,
    CAPI_INTERFACE_X2,
    CAPI_INTERFACE_XLAUI,
    CAPI_INTERFACE_XLAUI2,
    CAPI_INTERFACE_CAUI,
    CAPI_INTERFACE_QSGMII,
    CAPI_INTERFACE_LR4,
    CAPI_INTERFACE_LR,
    CAPI_INTERFACE_LR2,
    CAPI_INTERFACE_ER,
    CAPI_INTERFACE_ER2,
    CAPI_INTERFACE_ER4,
    CAPI_INTERFACE_SR2,
    CAPI_INTERFACE_SR10,
    CAPI_INTERFACE_CAUI4,
    CAPI_INTERFACE_VSR,
    CAPI_INTERFACE_LR10,
    CAPI_INTERFACE_KR10,
    CAPI_INTERFACE_CAUI4_C2C,
    CAPI_INTERFACE_CAUI4_C2M,
    CAPI_INTERFACE_ZR,
    CAPI_INTERFACE_LRM,
    CAPI_INTERFACE_XLPPI,
    CAPI_INTERFACE_OTN,
    CAPI_INTERFACE_Count
} capi_phy_interface_t;

typedef enum capi_link_sts_e {
    CAPI_LINK_STATUS_DOWN    = 0,
    CAPI_LINK_STATUS_UP
} capi_link_sts_t;

typedef struct capi_txfir_level_shift_s {
    uint8_t tx_level_n3;    /**< (i) LW TXFIR level -3 value, valid range [0~127]; */
    uint8_t tx_level_n1;    /**< (i) LW TXFIR level -1 value, valid range [0~127]; */
    uint8_t tx_level_p1;    /**< (i) LW TXFIR level +1 value, valid range [0~127]; */
    uint8_t tx_level_p3;    /**< (i) LW TXFIR level +3 value, valid range [0~127]; */
} capi_txfir_level_shift_t; /**< LW copper link training config */

typedef struct cw_chip_port_info_s{
    uint8_t cw_chip_port_mask;
    uint8_t cw_chip_bh_lane_mask[8];
    uint8_t cw_chip_lw_lane_mask[8];
    uint16_t cw_chip_serdes_mask;
    uint16_t cw_chip_media_mask;
} cw_chip_port_info_t;

typedef struct cw_chip_port_info_for_mode_s{
    uint8_t cw_chip_mode_t;
    uint8_t cw_chip_port_mask;
    uint8_t cw_chip_bh_lane_mask[8];
    uint8_t cw_chip_lw_lane_mask[8];
} cw_chip_port_info_for_mode_t;

/**
 * Test pattern gen type
 */
typedef enum capi_test_pattern_type_e {
    SSPRQ           = 0,        /**< SSPRQ pattern gen   */
    QPRBS13         = 1,        /**< QPRBS13 pattern gen */
    SQUARE_WAVE     = 2,        /**< SQUARE WAVE pattern gen */
    PCS_SCRM_IDLE   = 3,        /**< PCS Scrambled IDLE pattern */
    KP4_PRBS        = 4,        /**< KP4 PRBS pattern */
    NO_TEST_PATTERN
} capi_test_pattern_type_t;

/**
 * Test pattern configuration structure
 */
typedef struct capi_test_pattern_cfg_s {
    capi_enable_t            enable;        /**< test pattern gen enable. 1 - enable and 0 - disable */
    capi_test_pattern_type_t type;          /**< test pattern type - SSPRQ/QPRBS13 */
    square_wave_pattern_t    sq_patt_mode;  /**< square wave pattern mode */
    uint8_t                  txpi_on;       /**< internal usage*/
} capi_test_pattern_cfg_t;

/*
 * Structure of phy power
 */
typedef struct capi_phy_power_s {
    capi_switch_t rx;      /**< receiver on/off/toggle control    */
    capi_switch_t tx;      /**< transmitter on/off/toggle control */
} capi_phy_power_t;

/**
 * The structure of fixed mode configuration information
 */
typedef struct capi_fixed_voltage_config_info_s {
    capi_enable_t fixed_voltage_enable; /**< enable/disable */    
    capi_type_of_regulator_t type_of_regulator; /**< different type of regulator */
    uint16_t fixed_voltage;   
    avs_result_t set_status;
    capi_set_voltage_with_fw_t capi_set_voltage_with_fw;  /**< Set voltage with fw                  */ 
    uint8_t   regulator_i2c_address;
} capi_fixed_voltage_config_info_t; /**< fixed_voltage config info type */

/**
 * The structure of chip voltage
 */
typedef struct capi_chip_voltage_config_info_s {  
    uint16_t voltage_dvdd;   
    uint16_t voltage_075pv; /*vddm */
    uint16_t voltage_09pv;  /*Avdd */
} capi_chip_voltage_config_info_t; /**< fixed_voltage config info type */

/**
 * The enumeration of FEC Type
 */
typedef union capi_fec_type_u {
   capi_line_fec_type_t line_fec;
   capi_host_fec_type_t host_fec;
} capi_fec_type_t;                      /**< FEC Count */

typedef enum capi_fec_mode_e {
    CAPI_FEC_INGRESS,
    CAPI_FEC_EGRESS,
    CAPI_FEC_LINE,
    CAPI_FEC_CLIENT,
    CAPI_FEC_MODE_MAX
} capi_fec_mode_t;

#define FEC_TOT_BITS_CORR_NUM 2
#define FEC_TOT_FRAMES_ERR_NUM 16

/**
 * The struct of fec status to support both Kp4 and jfec
 */
typedef struct fec_rx_err_cnt_s {
    uint64_t tot_frame_rev_cnt;
    uint64_t tot_frame_corr_cnt;
    uint64_t tot_frame_uncorr_cnt;
    uint64_t tot_symbols_corr_cnt;                          /*!< based on tot_frames_err_cnt[0..15] */
    uint64_t tot_bits_corr_cnt[FEC_TOT_BITS_CORR_NUM];      /*!< [0] for correctable 0's; [1] for correctable 1's */
    uint64_t tot_frames_err_cnt[FEC_TOT_FRAMES_ERR_NUM];    /*!< [n] for number of cw with correstable symbol error=n */
} capi_fec_rx_err_cnt_t;

typedef struct fec_rx_status_s {
    uint16_t igbox_clsn_sticky;      /*!< intf-gbox clsn per fec port - read followed by clear */
    uint16_t am_lolock_sticky;       /*!< am lol per fec lane - read followed by clear */
    uint16_t xdec_gbox_clsn_sticky;  /*!< xdec gearbox collison per fec port - read followed by clear */
    uint16_t xenc_gbox_clsn_sticky;  /*!< xenc gearbox collison per fec port - read followed by clear */
    uint16_t ogbox_clsn_sticky;      /*!< xdec gearbox collison per fec port - read followed by clear */    
    uint16_t fec_algn_lol_sticky;    /*!< alignment lol sticky per fec port - read followed by clear */
    uint16_t fec_algn_stat;          /*!< alignment lock live status */
} capi_fec_rx_status_t;

/**
 * The struct of CW KP4 FEC Error Counter
 */
typedef struct cw_kp4fec_err_cnt_s{
    uint64_t tot_frame_cnt;                              /**<  fec total frame count*/
    uint64_t tot_frame_uncorr_cnt;                       /**<  fec uncorrected frame count*/
    uint64_t tot_bits_corr_cnt[FEC_TOT_BITS_CORR_NUM];   /**<  fec corrected bit0 & bit1 count*/
    uint64_t prbs_errcnt[FEC_TOT_FRAMES_ERR_NUM];        /**<  fec corrected frame count*/
    uint32_t t_code;                                     /**<  T-code is the max symbol errors per frame  */
    uint16_t fec_frame_size;                             /**<  KP4 is 5440, KR4 is 5280 */
    uint8_t uncorr_bits_per_frame;                       /**<  KP4 is 16, KR4 is 8   */
} cw_kp4fec_err_cnt_t;

/**
 * The struct of CW KP4 FEC Uncorrected Error Counter
 */
typedef struct cw_kp4fec_uncorr_err_s {
    uint64_t tot_frame_uncorr_cnt;                       /**<  fec uncorrected frame count*/
    uint32_t t_code;                                     /**<  T-code is the max symbol errors per frame  */
} cw_kp4fec_uncorr_err_t;

typedef struct capi_fec_ber_s {
    float pre_fec_ber;              /*!< pre fec BER */
    float post_fec_ber;             /*!< post fec BER */
    float post_fec_ber_proj;        /*!< projected post fec BER */
} capi_fec_ber_t;

/**
 * The struct for dump status
 */
typedef struct fec_dump_status_u {
    capi_fec_rx_status_t      fec_sts;              /*!< output: fec status */
    capi_fec_rx_err_cnt_t     fec_a_err_cnt;        /*!< output: fec err counter struct - should latch/clear/update counters separately */
    capi_fec_rx_err_cnt_t     fec_b_err_cnt;        /*!< output: fec err counter struct - only for 200g / 400g */
    capi_function_mode_t      func_mode;            /*!< output: Function Mode      */
    capi_fec_type_t           fec_type;             /*!< output: FEC protocol type */
    capi_fec_mode_t           fec_mode;             /*!< input : Client / Line side selection */
    uint8_t                   sampling_rate;        /*!< input : FEC monitor sampling rate in percentage, valid value is 0-100, value 0 or 100 is 100% sampling
                                                         Users can set it to 1 if they want 1% sampling rate to save power,
                                                         DSP FW will enable FEC monitor for 10ms, then disable it for 990ms, then repeat
                                                         It is supported in repeater mode only */
    uint8_t                   enable_time_slice;   /*!< input: enable fec monitor time slice to reduce power surge when sub-sampling (sampling_rate<100%) is on,
                                                         0 - disable; 1 - enable */
    capi_fec_ber_t            fec_ber;              /*!< output: FEC BER */

} capi_fec_dump_status_t;

/**
 * The enumeration of valid auto FEC port detection list
 */
typedef enum capi_fec_mon_valid_discover_ports_list_e {
    CAPI_FEC_MON_PORT_NULL = 0,                                         /*!< End the ordered search list */
    CAPI_FEC_MON_PORT_100G = CHIP_MODE_1X106G_KP4PAM_TO_1X106G_KP4PAM,  /*!< The 100g fec monitor port */
    CAPI_FEC_MON_PORT_200G = CHIP_MODE_2X106G_KP4PAM_TO_2X106G_KP4PAM,  /*!< The 200g fec monitor port */
    CAPI_FEC_MON_PORT_400G = CHIP_MODE_4X106G_KP4PAM_TO_4X106G_KP4PAM   /*!< The 400g fec monitor port */
}capi_fec_mon_valid_discover_ports_list_t;

#define CAPI_MAX_FEC_MON_PORT_LIST  3


/**
 * The structure for auto FEC port detection command
 * Refer capi_fec_start_stop_auto_ports_detect
 */
typedef struct capi_fec_auto_ports_detect_s {
    uint8_t  sampling_rate;/*!< input : FEC monitor sampling rate in percentage, valid value is 0-100, value 0 or 100 is 100% sampling
                                                         Users can set it to 1 if they want 1% sampling rate to save power,
                                                         DSP FW will enable FEC monitor for 10ms, then disable it for 990ms, then repeat
                                                         It is supported in repeater mode only */
    uint8_t stop;         /*!< stop :  =1 stop =0 start*/
    capi_fec_mon_valid_discover_ports_list_t  fec_mon_valid_discover_ports_ordered_list[CAPI_MAX_FEC_MON_PORT_LIST];/*!< ordered list of valid fec mon search ports */
} capi_fec_auto_ports_detect_t;

/**
 * The enumeration of auto FEC port detection status
 */
typedef enum capi_auto_detect_status_e {
    CAPI_AUTO_DETECT_STATUS_INVALID,   /*!< auto FEC port detection was never initiated */
    CAPI_AUTO_DETECT_STATUS_INPROGRESS,/*!< auto FEC port detection is in progress */
    CAPI_AUTO_DETECT_STATUS_DONE       /*!< auto FEC port detection is done */
} capi_auto_detect_status_t;


/**
 * The structure for auto FEC port detection status
 */
typedef struct capi_fec_auto_ports_detect_state_s {
    uint8_t   auto_detect_status;                        /*!< please refer capi_auto_detect_status_t */
    uint8_t   valid_ports;                               /*!< is aggregation of all valid detected FEC ports */
    uint16_t  fec_lock_value;                            /*!< is aggregation of the egress and ingress FEC status  */
    uint32_t undiscovered_lanemask;                      /*!< lanemask on which no fec mon ports were discovered */
    capi_config_info_t  capi_config_info[CAPI_MAX_PORTS];/*!< all the discovered FEC ports */
} capi_fec_auto_ports_detect_state_t;

/**
 * The enumeration for CAPI state information
 */
typedef enum capi_state_type_e {
    CAPI_STATE_TYPE_NONE,                                                 /**< CAPI State Type : None                                         */
    CAPI_STATE_TYPE_FEC_AUTO_PORT_DETECT_STATE,                           /**< CAPI State Type : FEC Auto Port Detection State                */
} capi_state_type_t;

/**
 * The structure for CAPI statue information
 */
typedef struct capi_state_info_s {
    uint32_t   type;                                                        /**< State Info type, refer capi_state_type_t                       */
    union {
        capi_fec_auto_ports_detect_state_t   fec_auto_ports_detect;        /**< Auto port detect status                                         */
    } state;                                                               /**< Status                                                          */
} capi_state_info_t;                                                       /**< CAPI State type                                                 */
/**
 * The struct of CW KP4 FEC BER
 */
typedef struct cw_kp4fec_ber_s {
   double cur_ber; /**<  current FEC BER */
   double min_ber; /**<  minimum FEC BER */
   double max_ber; /**<  maximum FEC BER */
   double average_ber; /**<  average FEC BER */ 
   double total_ber; /**<  total FEC BER for average calculation*/
} cw_kp4fec_ber_t;

/**
 * The struct of CW KP4 FEC BER & Error COunter information
 */
typedef struct cw_kp4fec_ber_state_s {
    capi_fec_mode_t fec_mode;                               /**< input : Client / Line side selection                                           */
    bool clr_cnt;                                           /**< input : FALSE:don't clear hardware FEC counter;
                                                                         TRUE:clear hardware FEC counter                                        */
    bool only_cur;                                          /**< input : 0:cAPI will return cur & min & max & average value;
                                                                         1:cAPI will only return cur BER and error count                        */
    uint32_t err_latch_times;                               /**< input & output: capi retrieve_fec_ber calling times which will be used to
                                                                                 calculate the average value;                                   */
    cw_kp4fec_ber_t pre_fec_ber;                            /**< input & output: pre FEC BER                                                    */
    cw_kp4fec_ber_t post_fec_ber;                           /**< input & output: post FEC BER                                                   */

    cw_kp4fec_err_cnt_t backup_err;                         /**< input & output: backup last FEC error count info                               */
#ifdef CAPI_CMIS_FEC_STATS_SMALL_FOOTPRINT
    cw_kp4fec_uncorr_err_t cur_err;                         /**< input & output: minimum FEC uncorrected error count and T-code during interval */
    cw_kp4fec_uncorr_err_t min_err;                         /**< input & output: minimum FEC uncorrected error count and T-code during interval */
    cw_kp4fec_uncorr_err_t max_err;                         /**< input & output: maximum FEC uncorrected error count and T-code during interval */
    cw_kp4fec_uncorr_err_t average_err;                     /**< input & output: average FEC uncorrected error count and T-code during interval */
#else
    cw_kp4fec_err_cnt_t cur_err;                            /**< input & output: current FEC error count info and T-code during interval        */
    cw_kp4fec_err_cnt_t min_err;                            /**< input & output: minimum FEC error count info and T-code during interval        */
    cw_kp4fec_err_cnt_t max_err;                            /**< input & output: maximum FEC error count info and T-code during interval        */
    cw_kp4fec_err_cnt_t average_err;                        /**< input & output: average FEC error count info and T-code during interval        */
#endif
    cw_kp4fec_err_cnt_t total_err;                          /**< input & output: total FEC error count info                                     */
    uint16_t fec_algn_lol_sticky;                           /**< alignment loss of lock sticky status                                           */
    uint16_t fec_algn_stat;                                 /**< alignment lock live status                                                     */
} capi_kp4fec_ber_state_t;


/**
 * The struct of LPM information
 */
typedef struct capi_lpm_info_s {
    capi_enable_t                   lpm_en;            /*!< input : LPM enable or disable status */
} capi_lpm_info_t;

/**
 * The enumeration of lane data rate Type
 */
typedef enum capi_lane_data_rate_e {
    CAPI_LANE_DATA_RATE_NONE = 0,     /**< Line data rate: None */
    CAPI_LANE_DATA_RATE_1P25 = 1,     /**< Line data rate: 1.25G */
    CAPI_LANE_DATA_RATE_10P3125 = 2,  /**< Line data rate: 10.3125G  */
    CAPI_LANE_DATA_RATE_20P625 = 3,   /**< Line data rate: 20.625G */
    CAPI_LANE_DATA_RATE_25P78125 = 4, /**< Line data rate: 25.78125G  */
    CAPI_LANE_DATA_RATE_26P5625 = 5,  /**< Line data rate: 26.5625G */
#if 0
    CAPI_LANE_DATA_RATE_27P95 = 6,    /**< Line data rate: 27.95G  */
#endif
    CAPI_LANE_DATA_RATE_28P125 = 7,   /**< Line data rate: 28.125G */
    CAPI_LANE_DATA_RATE_51P5625 = 8,  /**< Line data rate: 51.5625G */
    CAPI_LANE_DATA_RATE_53P125 = 9,   /**< Line data rate: 53.125 */
#if 0
    CAPI_LANE_DATA_RATE_54 = 10,      /**< Line data rate: 54G ? */
#endif
    CAPI_LANE_DATA_RATE_56P25 = 11,   /**< Line data rate: 56.25G */
#if 0
    CAPI_LANE_DATA_RATE_57 = 12,      /**< Line data rate: 57G ? */
#endif
    CAPI_LANE_DATA_RATE_MAX = 13

} capi_lane_data_rate_t;              /**< Chip Lane Data Rate type*/

/* Refer to cw_lw_pll_multiplier_t */
typedef enum capi_pll_ratio_list_e {
    CAPI_CW_LW_PLL_MULTIPLIER_170 = 0,   /**< PLL ratio 170 */
    CAPI_CW_LW_PLL_MULTIPLIER_165 = 1,   /**< PLL ratio 165 */
    CAPI_CW_LW_PLL_MULTIPLIER_168 = 2,   /**< PLL ratio 168 */
    CAPI_CW_LW_PLL_TYPE_MAX 
} capi_pll_ratio_type_t;

/*Refer to cw_bh_pll_multiplier_t */
typedef enum capi_bh_pll_ratio_list_e {
    CAPI_CW_BH_PLL_MULTIPLIER_170 = 0,   /*PLL ratio 170*/
    CAPI_CW_BH_PLL_MULTIPLIER_165 = 1,   /*PLL ratio 165*/
    CAPI_CW_BH_PLL_MULTIPLIER_160 = 2,   /*PLL ratio 160*/
    CAPI_CW_BH_PLL_TYPE_MAX
} capi_bh_pll_ratio_type_t;

typedef enum capi_egr_port_type_e {
    EGR_PAM4_RPTR =1,
    EGR_NRZ_RPTR =2,
    EGR_P2N_RGB_BM =3,
    EGR_N2P_FGB_BM =4,
    EGR_N2N_RGB_BM =5,
    EGR_N2N_FGB_BM =6,
    EGR_P2N_RGB_FEC =7,
    EGR_N2P_FGB_FEC =8,
    EGR_PAM4_HMUX =9,
    EGR_NRZ_HMUX =10,
    EGR_P2N_RGB_HMUX =11,
    EGR_N2N_RGB_HMUX =12,
} capi_egr_port_type_t;

typedef enum capi_igr_port_type_e {
    IGR_PAM4_RPTR =1,
    IGR_NRZ_RPTR =2,
    IGR_N2P_FGB_BM =3,
    IGR_P2N_RGB_BM =4,
    IGR_N2N_FGB_BM =5,
    IGR_N2N_RGB_BM =6,
    IGR_N2P_FGB_FEC =7,
    IGR_P2N_RGB_FEC =8,
    IGR_PAM4_RPTR_BCST =9,
    IGR_NRZ_RPTR_BCSM =10,
    IGR_N2P_FGB_FEC_BCST =11,
    IGR_N2N_FGB_BM_BCST =12,
    IGR_NRZ_RPTR_BCST =13/* ?? */
} capi_igr_port_type_t;

typedef enum capi_host_osr_e {
    CAPI_CW_BH_OSR_1 = 1
} capi_host_osr_t;

typedef enum capi_line_osr_e {
    CAPI_CW_LW_OSR_1 = 1,
    CAPI_CW_LW_OSR_2 = 2
} capi_line_osr_t;

typedef enum capi_q0q1_e {
    CW_QUAD_Q0,
    CW_QUAD_Q1
} capi_q0q1_t;

typedef enum capi_host_inst_e {
    CW_INST_HOST_0,
    CW_INST_HOST_1,
} capi_host_inst_t;

typedef enum capi_hostline_e {
    CW_INST_LINE = 0, 
    CW_INST_HOST = 1
} capi_hostline_t;

typedef enum capi_ch_e {
    CW_FEC_CORE_CH0A,
    CW_FEC_CORE_CH0B,
    CW_FEC_CORE_CH1A,
    CW_FEC_CORE_CH1B,
} capi_ch_t;

typedef enum capi_rm_ovrd_type_e {
     CAPI_RM_OVRD_NA = 0,  /*Rate mode overridden is not supported*/
     CAPI_RM_OVRD_NO = 1,  /*Rate mode overridden is not.*/
     CAPI_RM_OVRD_YES = 2,  /*Rate mode overridden .*/
} capi_rm_ovrd_type_t;

typedef enum return_stat_e {
   PORT_NO_MATCH = 0,
   PORT_MATCH_FOUND = 1,
   PORT_MATCH_MULTI = 2,
} return_stat_t;

/**
 * The structure to represent fec monitor info
 */
typedef struct capi_rptr_fec_st_s {
    uint8_t           fec_mode;           /*!< input : Client / Line side selection */
    uint8_t           is_enable;          /*!< input : enable or disable repeater FEC monitor */
    uint8_t           func_mode;          /*!< output: Function Mode      */
    uint8_t           fec_type;           /*!< output: FEC protocol type */
    uint16_t          fec_monitor_port;   /*!< input : =0 not port based fec monitor */
    uint8_t           sampling_rate;      /*!< input:  FEC monitor sampling rate in percentage */
    uint8_t           fec_monitor_discover;      /*!< input:  FEC monitor sampling rate in percentage */
} capi_rptr_fec_st_t;                     /*!< Repeater mode FEC monitor type */

/**
 * The structure of lane status
 */
typedef struct lane_status_s{
    uint8_t cdr_lock;                    /**< CDR Lock;       1:locked       0:unlocked        */
    uint8_t los;                         /**< Lose of Signal; 1:Signal lost  0:Signal not lost */
    uint8_t tx_squelch;                  /**< tx_squelch      1:Squelched    0:Unsquelched     */
    uint8_t tx_mission;                  /**< tx_mission      1:In Mission   0:Not in Mission  */
    uint8_t rx_mission;                  /**< rx_mission      1:In Mission   0:Not in Mission  */
    uint8_t pll_lock;                    /**< PLL Lock;       1:locked       0:unlocked        */
} lane_status_t;                         /**< Lane status                                      */

/**
 * The structure of Auto Neg Status Information type
 */
typedef struct an_status_info_s {
    uint8_t  auto_neg_en;                /**< =1 Enable AutoNeg, =0 Disable AutoNeg */
    uint8_t  neg_hcd_mode;               /**< Negotiated AN mode ability, refer to capi_lw_an_req_t */
    uint8_t  neg_fec;                    /**< Negotiated Auto-negotiation FEC ability, refer to capi_an_neg_fec_t */
    uint8_t  an_state;                   /**< Current CL73 AN SM states, refer to capi_an_neg_fec_t */
    uint16_t lp_ability1;                /**< one or more capi_an73_ability1_t LP abilities */
    uint16_t lp_ability2;                /**< one or more capi_an73_ability2_t LP abilities */
    uint16_t lp_ability3;                /**< one or more capi_an73_ability3_t LP abilities */
    uint16_t lp_fec_ability;             /**< FEC LP ability, refer to capi_an73_pause_t */
    uint8_t  lp_pause_ability;           /**< LP Pause ability                           */
} an_status_info_t;                      /**< AutoNeg Status Information type                     */

/**
 * The structure of Diagnostics Status type
 */
typedef struct diag_status_s{
    uint8_t adc_sig_clipping;            /**< ADC Signal Clipping Detected, line side, refer to capi_adc_sig_clipping_t */
    uint8_t pll_unlock_sticky;           /**< PLL unlock sticky, client side, refer to capi_client_pll_unlock_sticky_t*/
    uint8_t bh2bh_fifo_status;           /**< Bh2bh fifo status, refer to capi_client_bh2bh_fifo_status_t  */
} diag_status_t;                         /**< Diagnostics Status type                                      */

/**
 * The structure of Core Wrapper status type
 */
typedef struct cw_status_s{
    uint16_t egress_sticky;              /**< Egress Sticky status                                    */
    uint16_t ingress_sticky;             /**< Ingress Sticky status                                   */
} cw_status_t;                           /**< Core Wrapper Status type                                */

/**
 * The structure of Sticky Loss status type
 */
typedef struct sticky_loss_s{
    uint8_t  electrical;                                 /**< Electrical Loss Sticky Status            */
    uint8_t  optical;                                    /**< Optical Loss Sticky Status   DSP only    */
    uint8_t  link;                                       /**< Loss of Link (i.e. lol) Sticky status    */
} sticky_loss_t;                                         /**< Sticky Loss type                         */

/**
 * The struct of lane information
 */
typedef struct capi_lane_info_s {
    union {
        uint16_t content;

        struct {
            #ifdef CAPI_IS_BIG_ENDIAN
            uint16_t reserved                      : 8;  /**< Reserved                                 */
            uint16_t lane_status                   : 1;  /**< Lane status (i.e. locks, tx-squelch)     */
            uint16_t lnktrn_status                 : 1;  /**< lnktrn status                            */
            uint16_t an_status                     : 1;  /**< an status                                */
            uint16_t diag_status                   : 1;  /**< diag status                              */
            uint16_t cw_status                     : 1;  /**< CW : FIFO sticky                         */
            uint16_t sticky_loss_electrical         : 1;  /**< Electrical loss sticky:sticky_los_t       */
            uint16_t sticky_loss_optical            : 1;  /**< Optical loss sticky:sticky_los_t DSP only */
            uint16_t sticky_loss_link              : 1;  /**< Loss of Link (lol) sticky:sticky_los_t   */
            #else  /* CAPI_IS_LITTLE_ENDIAN */
            uint16_t sticky_loss_link              : 1;  /**< Loss of Link (lol) sticky:sticky_los_t   */
            uint16_t sticky_loss_optical           : 1;  /**< Optical loss sticky:sticky_los_t DSP only */
            uint16_t sticky_loss_electrical        : 1;  /**< Electrical loss sticky:sticky_los_t       */
            uint16_t cw_status                     : 1;  /**< CW status                                */
            uint16_t diag_status                   : 1;  /**< diag status                              */
            uint16_t an_status                     : 1;  /**< an status                                */
            uint16_t lnktrn_status                 : 1;  /**< lnktrn status                            */
            uint16_t lane_status                   : 1;  /**< Lane status (i.e. locks, tx-squelch)     */
            uint16_t reserved                      : 8;  /**< Reserved                                 */
            #endif /* CAPI_IS_BIG_ENDIAN */
        } is;
    } param;

    union {
        lane_status_t          lane_status;              /**< Lane Status   (i.e. locks, tx-squelch)   */
        link_training_status_t lnktrn_status;            /**< Link Training Status                     */
        an_status_info_t       an_status;                /**< Auto Negotiations Status                 */
        diag_status_t          diag_status;              /**< Diagnostic Status                        */
        cw_status_t            cw_status;                /**< CW Status                                */
        sticky_loss_t          sticky_loss;              /**< Sticky Loss status : sticky_loss_t       */
    } value;
} capi_lane_info_t;                                      /**< Lane Information type                    */


/**
 * The struct for Recovered Clock Information
 */
typedef struct capi_recovered_clock_info_s {
    
    uint16_t  phy_side;                                  /**< Phy Side selection; refer to capi_phy_side_t :both side not valid   */ 

    union {
        uint16_t   content;                              /**< Flag Content                                                        */

        struct {
            #ifdef CAPI_IS_BIG_ENDIAN
            uint16_t reserved              :11;          /**< Reserved                                                            */
            uint16_t config_pin_0          : 1;          /**< Configure Digital Pin 0                                             */
            uint16_t config_pin_1          : 1;          /**< Configure Digital Pin 1                                             */
            uint16_t config_pin_2          : 1;          /**< Configure Digital Pin 2                                             */
            uint16_t config_analog_pin_0   : 1;          /**< Configure Analog  Pin 0                                             */
            uint16_t config_vco_pin        : 1;          /**< Configure VCO/16 clock  Pin                                          */
            #else  /* CAPI_IS_LITTLE_ENDIAN */
            uint16_t config_vco_pin        : 1;          /**< Configure VCO/16 clock  Pin                                          */
            uint16_t config_analog_pin_0   : 1;          /**< Configure Analog  Pin 0                                             */
            uint16_t config_pin_2          : 1;          /**< Configure Digital Pin 2                                             */
            uint16_t config_pin_1          : 1;          /**< Configure Digital Pin 1                                             */
            uint16_t config_pin_0          : 1;          /**< Configure Digital Pin 0                                             */
            uint16_t reserved              :11;          /**< Reserved                                                            */
            #endif /* CAPI_IS_BIG_ENDIAN */
        } is;
    } command;

    union {
        uint32_t   content[3];                           /**< Flag Content                                                        */

        struct {
            #ifdef CAPI_IS_BIG_ENDIAN
            uint32_t reserved              :18;          /**< Reserved                                                            */
            uint32_t enable                : 1;          /**< Recovered Clock Enable:  0: Disable     1: Enable                   */
            uint32_t squelch               : 1;          /**< 0: Unsquelch 1: Squelch                                             */
            uint32_t div_ratio             : 4;          /**< Division Ratio used for control recovered clock ratio : range[0,7], *
                                                          **  0 : Clock divider ratio is 1                                        *
                                                          **  1 : Clock divider ratio is 2                                        *
                                                          **  2 : Clock divider ratio is 4                                        *
                                                          **  3 : Clock divider ratio is 8                                        *
                                                          **  4 : Clock divider ratio is 16                                       *
                                                          **  5 : Clock divider ratio is 32                                       *
                                                          **  6 : Clock divider ratio is 64                                       *
                                                          **  7 : Clock divider ratio is 128                                      *
                                                          **  Irrelevant in the disabled case                                     */
            uint32_t lane_mask             : 8;          /**< Lane to be configured                                               */
            #else  /* CAPI_IS_LITTLE_ENDIAN */
            uint32_t lane_mask             : 8;          /**< Lane to be configured                                               */
            uint32_t div_ratio             : 4;          /**< Division Ratio used for control recovered clock ratio : range[0,7] 
                                                          **  0 : Clock divider ratio is 1                                        *
                                                          **  1 : Clock divider ratio is 2                                        *
                                                          **  2 : Clock divider ratio is 4                                        *
                                                          **  3 : Clock divider ratio is 8                                        *
                                                          **  4 : Clock divider ratio is 16                                       *
                                                          **  5 : Clock divider ratio is 32                                       *
                                                          **  6 : Clock divider ratio is 64                                       *
                                                          **  7 : Clock divider ratio is 128                                      *
                                                          **  Irrelevant in the disabled case                                     */
            uint32_t squelch               : 1;          /**< 0: Unsquelch 1: Squelch                                             */
            uint32_t enable                : 1;          /**< Recovered Clock Enable:  0: Disable     1: Enable                   */
            uint32_t reserved              :18;          /**< Reserved                                                            */
            #endif /* CAPI_IS_BIG_ENDIAN */
        } config_pin[3];                                 /**< Configuration of Pin[0-2]                                           */
        struct {
            #ifdef CAPI_IS_BIG_ENDIAN
            uint32_t reserved              :18;          /**< Reserved                                                            */
            uint32_t enable                : 1;          /**< Recovered Clock Enable:  0: Disable     1: Enable                   */
            uint32_t rclk_type             : 1;          /**< 0: base frequence is VCO divided by 32 1: base frequence is VCO divided by 80 */
            uint32_t div_ratio             : 4;          /**< Division Ratio used for control recovered clock ratio : range[0,7], *
                                                          **  0 : Clock divider ratio is 1                                        *
                                                          **  1 : Clock divider ratio is 2                                        *
                                                          **  2 : Clock divider ratio is 4                                        *
                                                          **  3 : Clock divider ratio is 8                                        *
                                                          **  4 : Clock divider ratio is 16                                       *
                                                          **  5 : Clock divider ratio is 32                                       *
                                                          **  6 : Clock divider ratio is 64                                       *
                                                          **  7 : Clock divider ratio is 128                                      *
                                                          **  Irrelevant in the disabled case                                     */
            uint32_t lane_mask             : 8;          /**< Lane to be configured                                               */
            #else  /* CAPI_IS_LITTLE_ENDIAN */
            uint32_t lane_mask             : 8;          /**< Lane to be configured                                               */
            uint32_t div_ratio             : 4;          /**< Division Ratio used for control recovered clock ratio : range[0,7] 
                                                          **  0 : Clock divider ratio is 1                                        *
                                                          **  1 : Clock divider ratio is 2                                        *
                                                          **  2 : Clock divider ratio is 4                                        *
                                                          **  3 : Clock divider ratio is 8                                        *
                                                          **  4 : Clock divider ratio is 16                                       *
                                                          **  5 : Clock divider ratio is 32                                       *
                                                          **  6 : Clock divider ratio is 64                                       *
                                                          **  7 : Clock divider ratio is 128                                      *
                                                          **  Irrelevant in the disabled case                                     */
            uint32_t rclk_type             : 1;          /**< 0: base frequence is VCO divided by 32 1: base frequence is VCO divided by 80 */
            uint32_t enable                : 1;          /**< Recovered Clock Enable:  0: Disable     1: Enable                   */
            uint32_t reserved              :18;          /**< Reserved                                                            */
            #endif /* CAPI_IS_BIG_ENDIAN */
        } config_analog_pin[1];                                 /**< Configuration of Pin[0-2]                                           */
        struct {
            #ifdef CAPI_IS_BIG_ENDIAN
            uint32_t reserved              :31;          /**< Reserved                                                            */
            uint32_t enable                : 1;          /**< Recovered Clock Enable:  0: Disable     1: Enable                   */
            #else  /* CAPI_IS_LITTLE_ENDIAN */
            uint32_t enable                : 1;          /**< Recovered Clock Enable:  0: Disable     1: Enable                   */
            uint32_t reserved              :31;          /**< Reserved                                                            */
            #endif /* CAPI_IS_BIG_ENDIAN */
        } config_vco_pin;                                 /**< Configuration of Pin[0-2]                                           */
    } value;
} capi_recovered_clock_info_t;                           /**< Recovered Clock Information type                                    */



/**
 * CAPI EPDM Config Information enumeration
 */
typedef struct capi_epdm_prbs_cfg_s {
    uint8_t poly;                                                         /**< Poly Type                                 */
    uint8_t invert;                                                       /**< Invert                                    */
} capi_epdm_prbs_cfg_t;                                                  /**< EPDM PRBS Configuration type              */

/**
 * CAPI EPDM Mode Config Set Information enumeration
 */
typedef struct capi_epdm_mode_config_set_info_s {
    uint32_t lane_mask;                                                   /**< Lane Mask                                 */
    uint16_t lane_data_rate;                                              /**< Lane Data Rate                            */
    uint8_t  media_type;                                                  /**< Media type                                */
    uint8_t  lane_fec_type;                                               /**< Lane FEC Type                             */
    uint8_t  lane_modulation;                                             /**< Lane Modulation                           */
    bool     hitless_mux_mode;                                            /**< Hitless Mux Mode                          */
    bool     hybrid_port_mode;                                            /**< Hybrid port mode                          */
} capi_epdm_mode_cfg_set_info_t;                                          /**< EPDM Mode Config Set Information          */

/**
 * CAPI EPDM archive information structure
 */
typedef struct capi_epdm_archive_info_s {

    union {
        uint32_t                           content;                       /**< Flag Content                             */
        struct {
            #ifdef CAPI_IS_BIG_ENDIAN
            uint32_t reserved              :22;                           /**< Reserved                                 */
            uint32_t sys_prbs_cfg_tx       : 1;                           /**< PRBS Configuration tx on sys             */
            uint32_t sys_prbs_cfg_rx       : 1;                           /**< PRBS Configuration rx on line            */
            uint32_t line_prbs_cfg_tx      : 1;                           /**< PRBS Configuration tx on line            */
            uint32_t line_prbs_cfg_rx      : 1;                           /**< PRBS Configuration rx on line            */
            uint32_t sys_intf_type         : 1;                           /**< system side interface type               */
            uint32_t line_intf_type        : 1;                           /**< line side interface type                 */
            uint32_t mode_config_info      : 1;                           /**< Mode Configuration Information           */
            uint32_t sys_pcs_scrm_idle_gen : 1;                           /**< PCS scrmable idle gen on sys             */
            uint32_t line_pcs_scrm_idle_gen: 1;                           /**< PCS scrmable idle gen on line            */
            uint32_t buffer                : 1;                           /**< Buffer for future use                    */
            #else  /* CAPI_IS_LITTLE_ENDIAN */
            uint32_t buffer                : 1;                           /**< Buffer for future use                    */
            uint32_t line_pcs_scrm_idle_gen: 1;                           /**< PCS scrmable idle gen on line            */
            uint32_t sys_pcs_scrm_idle_gen : 1;                           /**< PCS scrmable idle gen on sys             */
            uint32_t mode_config_info      : 1;                           /**< Mode Configuration Information           */
            uint32_t line_intf_type        : 1;                           /**< line side interface type                 */
            uint32_t sys_intf_type         : 1;                           /**< system side interface type               */
            uint32_t line_prbs_cfg_rx      : 1;                           /**< PRBS Configuration rx on line            */
            uint32_t line_prbs_cfg_tx      : 1;                           /**< PRBS Configuration tx on line            */
            uint32_t sys_prbs_cfg_rx       : 1;                           /**< PRBS Configuration rx on sys             */
            uint32_t sys_prbs_cfg_tx       : 1;                           /**< PRBS Configuration tx on sys             */
            uint32_t reserved              :22;                           /**< Reserved                                 */
            #endif /* CAPI_IS_BIG_ENDIAN */
        } is;
    } archive;

    /**< Archived Data */
    union {
        capi_epdm_prbs_cfg_t           sys_prbs_cfg_tx[MAX_BH_LANES];     /**< per sys lane prbs config for TX          */
        capi_epdm_prbs_cfg_t           sys_prbs_cfg_rx[MAX_BH_LANES];     /**< per sys lane prbs config for RX          */
        capi_epdm_prbs_cfg_t           line_prbs_cfg_tx[MAX_BH_LANES];    /**< per line lane prbs config for TX         */
        capi_epdm_prbs_cfg_t           line_prbs_cfg_rx[MAX_BH_LANES];    /**< per line lane prbs config for RX         */
        uint8_t                        sys_interface_type[MAX_BH_LANES];  /**< per lane interface type                  */
        uint8_t                        line_interface_type[MAX_BH_LANES]; /**< per lane interface type                  */
        capi_epdm_mode_cfg_set_info_t  mode_config_info;                  /**< per lane required for system side info   */
        uint8_t                        sys_pcs_scrm_idle_gen[MAX_BH_LANES];
        uint8_t                        line_pcs_scrm_idle_gen[MAX_BH_LANES];
        uint8_t                        buffer[64];                        /**< buffer for tuture use                    */
    } value;                                                              /**< Content/Value of the archived parameter  */

} capi_epdm_archive_info_t;                                               /**< EPDM Archive Information type            */

/**
 * EPDM archive structure - FW only
 * Firmware uses this structure to store the EPDM data in the SRAM
 */
typedef struct fw_epdm_archive_storage_s {

    union {
        uint32_t                           content;                       /**< Flag Content                             */
        struct {
            #ifdef CAPI_IS_BIG_ENDIAN
            uint32_t reserved              :22;                           /**< Reserved                                 */
            uint32_t buffer                : 1;                           /**< Buffer for future use                    */
            uint32_t line_pcs_scrm_idle_gen: 1;                           /**< PCS scrmable idle gen on line            */
            uint32_t sys_pcs_scrm_idle_gen : 1;                           /**< PCS scrmable idle gen on sys             */
            uint32_t mode_config_info      : 1;                           /**< Mode Configuration Information           */
            uint32_t line_intf_type        : 1;                           /**< line side interface type                 */
            uint32_t sys_intf_type         : 1;                           /**< system side interface type               */
            uint32_t line_prbs_cfg_rx      : 1;                           /**<  PRBS Configuration rx on line           */
            uint32_t line_prbs_cfg_tx      : 1;                           /**<  PRBS Configuration tx on line           */
            uint32_t sys_prbs_cfg_rx       : 1;                           /**<  PRBS Configuration rx on line           */
            uint32_t sys_prbs_cfg_tx       : 1;                           /**<  PRBS Configuration tx on sys            */
            #else /* CAPI_IS_LITTLE_ENDIAN */
            uint32_t sys_prbs_cfg_tx       : 1;                           /**<  PRBS Configuration tx on sys            */
            uint32_t sys_prbs_cfg_rx       : 1;                           /**<  PRBS Configuration rx on line           */
            uint32_t line_prbs_cfg_tx      : 1;                           /**<  PRBS Configuration tx on line           */
            uint32_t line_prbs_cfg_rx      : 1;                           /**<  PRBS Configuration rx on line           */
            uint32_t sys_intf_type         : 1;                           /**< system side interface type               */
            uint32_t line_intf_type        : 1;                           /**< line side interface type                 */
            uint32_t mode_config_info      : 1;                           /**< Mode Configuration Information           */
            uint32_t sys_pcs_scrm_idle_gen : 1;                           /**< PCS scrmable idle gen on sys             */
            uint32_t line_pcs_scrm_idle_gen: 1;                           /**< PCS scrmable idle gen on line            */
            uint32_t buffer                : 1;                           /**< Buffer for future use                    */
            uint32_t reserved              :22;                           /**< Reserved                                 */
            #endif /*CAPI_IS_BIG_ENDIAN*/
        } is;
    } archive;

    /**< Archived Data */
    capi_epdm_prbs_cfg_t           sys_prbs_cfg_tx[MAX_BH_LANES];     /**< per sys lane prbs config for TX          */
    capi_epdm_prbs_cfg_t           sys_prbs_cfg_rx[MAX_BH_LANES];     /**< per sys lane prbs config for RX          */
    capi_epdm_prbs_cfg_t           line_prbs_cfg_tx[MAX_BH_LANES];    /**< per line lane prbs config for TX         */
    capi_epdm_prbs_cfg_t           line_prbs_cfg_rx[MAX_BH_LANES];    /**< per line lane prbs config for RX         */
    uint8_t                        sys_interface_type[MAX_BH_LANES];  /**< per lane interface type                  */
    uint8_t                        line_interface_type[MAX_BH_LANES]; /**< per lane interface type                  */
    capi_epdm_mode_cfg_set_info_t  mode_config_info;                    /**< per lane required for system side info   */
    uint8_t                        sys_pcs_scrm_idle_gen[MAX_BH_LANES];   /**< PCS scramble idle gen flag on sys        */
    uint8_t                        line_pcs_scrm_idle_gen[MAX_BH_LANES];  /**< PCS scramble idle gen flag on line       */
    uint8_t                        buffer[64];                          /**< buffer for tuture use                    */

} fw_epdm_archive_storage_t;                                                 /**< EPDM Archive Information type            */

/**
 * CAPI Archive Information type enumeration
 */
typedef enum capi_archive_type_e {
    CAPI_ARCHIVE_INFO_EPDM_TYPE = 1                                      /**< Archive Information Type EPDM            */
} capi_archive_type_t;                                                   /**< Archive Information type                 */

/**
 * CAPI archive information structure
 */
typedef struct capi_archive_info_s {

    uint32_t                            type;                            /**< Archive Information Type, refer capi_archive_type_e */

    union {
        capi_epdm_archive_info_t        epdm_archive_info;                /**< FW init params required during by FW     */
    } element;                                                            /**< Content/Value of the archived parameter  */

} capi_archive_info_t;                                                    /**< Archive Information type                 */
/**
 * The structure contains lw CMIS internal information
 */
typedef struct capi_cmis_info_s {
    int16_t  slicer_threshold[3];             /**< FFE slicer threshold */
    int16_t  slicer_target[4];                /**< FFE slicer symbol value */
    float    slicer_mean_value[4];            /**< slicer mean value */
    float    slicer_sigma_sqr_value[4];       /**< slicer sigma square value */
    uint32_t slicer_p_value[4];               /**< slicer P value */
    int16_t  slicer_p_location[4];            /**< slicer P location */
    uint32_t slicer_v_value[3];               /**< slicer V value */
    int16_t  slicer_v_location[3];            /**< slicer V location */
} capi_cmis_info_t; /**< CMIS Info type */

#define HW_MUTEX_INVALID 0

typedef enum poll_feature_s {
    SNR_POLL = 1
} poll_feature_t;

/**
 * The structure for memory information
 */
typedef struct memory_info_s {
    uint32_t    ref_address;           /**< [In/Output] SRAM reference address               */
    uint32_t    ref_data_len;          /**< [In/Output] length of the data                   */
    uint32_t    hw_mutex;              /**< [In/Output] HW mutex for the memory region,      */
} memory_info_t;                    /**< Memory Information type                          */

/**
 * The structure for CMIS SNR LTP values
 */
typedef struct capi_cmis_snr_ltp_s {
    float    cmis_snr_value;                    /**< CMIS SNR value */
    float    cmis_ltp_value;                    /**< CMIS LTP value */
    float    brcm_snr_value;                    /**< Broadcom SNR value */
} capi_cmis_snr_ltp_t;                     /**< CAPI CMIS SNR LTP Payload type                                       */

/**
 * The structure for fast data access feature, contains input poll configuration and the output payload
 */
typedef struct memory_data_info_s {
    memory_info_t    memory_info;                /**< [In/Output] Memory details, returned by FW                            */
    
    union {
        struct {
            uint32_t start_stop;                 /**< [Input]  1=start : 0=stop. Refer to capi_turn_onoff_t enum            */
            uint32_t poll_interval;              /**< [Input]  polling timer in milliseconds. please refer                  */
                                                 /**<          to the user programming guidelines for the max/min pooling   */
                                                 /**<          intervals for each particular feature                        */
        } command;

        capi_cmis_snr_ltp_t lane_data[MAX_LW_LANES];/**< [Output] Lane based retrieved data payload                            */

                                                 /**< [Output] Other feature payload shall be added here                    */
    } param;
} memory_data_info_t;                            /**< Fast access memory Information                                        */



/**
 * The struct definition for MPI configuration
 */
typedef struct dsp_mpi_cfg_info_s {
    uint8_t         mpi_enable;           /**<MPI check enable/disable [0, 1] 0-disable MPI, 1; 1-enable MPI */
    uint8_t         mpi_interval;         /**<MPI update interval (unit: seconds ) [0, 15] 0-FW default, 1s; 1~15 configurable value*/
    float           ratio_threshold;      /**<MPI indicator ratio threshold [1.00, 2.00] 0-FW default 1.30, */
} dsp_mpi_cfg_info_t;      /**< DSP MPI configuration information define                      */

/**
 * The struct definition for dynamic MPI configuration
 */
typedef struct dsp_mission_mpi_cfg_info_s {
    uint8_t        mpi_enable;          /**<dynamic MPI check enable/disable [0, 1] 0-disable MPI, 1; 1-enable MPI, */
    uint8_t        mpi_chk_th;          /**<dynamic MPI check threshold, default is 11, with 9,10,11,12,13 and 14 as settable options */
    uint16_t       mpi_chk_cnt;         /**<dynamic MPI check counte default is 512, settable option are 128, 256, 512, 1024 */
} dsp_mission_mpi_cfg_info_t;      /**< DSP dynamic MPI configuration information state define                      */

/**
 * The struct definition for MPI state
 */
typedef struct dsp_mpi_st_info_s {
    uint8_t        read_clear;              /**<[input] 0-read only ; 1-read & clear */
    uint8_t        dust_indicator;          /**<MPI dust indicator state in last cycle */
    uint8_t        dust_indicator_sticky;   /**<MPI dust indicator sticky state. when read_clear is 1, after reporting sticky state, it will be cleared; */
    float          mpi_metric;              /**<MPI metric as: new_slope + 0.25*new_c0. */
    float          mpi_normalized_s[4];     /**<MPI Sorted Normalized standard deviation of level-0 */
    float          mpi_slope;               /**<MPI Slope of the linear fit of the four normalized variances */
    float          mpi_intercept;           /**<MPI Intercept of the linear fit of the four normalized variances */
} dsp_mpi_st_info_t;      /**< DSP MPI state information define                      */

/*
 * The struct definition for MPI state
 */
typedef struct dsp_mission_mpi_st_info_ss {
    uint8_t        read_clear;              /**<[input] 0-read only ; 1-read & clear */
    uint8_t        dust_indicator;          /**<MPI dust indicator state in last cycle */
    uint8_t        dust_indicator_sticky;   /**<MPI dust indicator sticky state. when read_clear is 1, after reporting sticky state, it will be cleared; */
    uint8_t        average_record;          /**<dynamic MPI recorded average value */
} dsp_mission_mpi_st_info_t;      /**< DSP dynamic MPI state information define                      */


/**
 * The struct definition for incoming traffic mode swith detect configuration
 */
typedef struct dsp_traffic_mode_switch_detect_cfg_info_s {
    uint8_t        tmsd_enable;          /**<incoming traffic mode switch from 10XG to 5Xg detect check enable/disable [0, 1] 0-disable, 1; 1-enable, default is disabled */
} dsp_tmsd_cfg_info_t;                 /**< DSP incoming traffic mode switch from 10XG to 5Xg detect check configuration define                      */

/**
 * The struct definition for incoming traffic mode swith detect state
 */
typedef struct dsp_traffic_mode_switch_detect_st_s {
    uint8_t        read_clear;                   /**<[input] 0-read only ; 1-read & clear */
    uint8_t        tmsd_detected;            /**<incoming traffic mode switch from 10XG to 5Xg detect state: 0-isn't detected; 1-detected */
    uint8_t         tmsd_detected_sticky;/**<incoming traffic mode switch from 10XG to 5Xg detect sticky state: 0-isn't detected; 1-detected */
} dsp_tmsd_st_t;                                  /**< DSP incoming traffic mode switch from 10XG to 5Xg detect state define                      */

/**
 * The struct definition for MPI canceller configuration
 */
typedef struct dsp_mpi_canceller_cfg_info_s {
    uint8_t        mpi_canceller_enable;          /**<MPI canceller enable/disable [0, 1] 0-disable, 1; 1-enable; default is enabled */
} dsp_mpi_canceller_cfg_info_t;                 /**< DSP MPI canceller configuration define                      */
/**
 * The structure of dsp_debug_dump information 
 */
typedef struct capi_dsp_debug_dump_info_s {

    float           dbg_s; 
    uint16_t        dbg_gap;
    int16_t         dbg_pm;
    float           dbg_ft[8];
} capi_dsp_debug_dump_info_t;           /**< DSP debug dump infomation type              */

typedef enum capi_dsp_debug_dump_stage_e {
    CAPI_DSP_DUMP_STAGE_0           = 0,          /*!< CAPI DSP_DUMP_STAGE 0 */
    CAPI_DSP_DUMP_STAGE_1           = 1,          /*!< CAPI DSP_DUMP_STAGE 1 */
    CAPI_DSP_DUMP_STAGE_2           = 2,          /*!< CAPI DSP_DUMP_STAGE 2*/
    CAPI_DSP_DUMP_STAGE_3           = 3,          /*!< CAPI DSP_DUMP_STAGE 3*/
    CAPI_DSP_DUMP_STAGE_MAX
} capi_dsp_debug_dump_stage_t;

typedef struct capi_dsp_debug_dump_s {
    uint8_t                       lane_id;                    /*!< input: lane ID (0/1/2/3)*/
    uint8_t                       dbg_stage_1_flag;            /*!< output: debug stage 1 flag */
    uint8_t                       dbg_stage_2_flag;            /*!< output: debug stage 2 flag */
    uint8_t                       dbg_stage_3_flag;            /*!< output: debug stage 3 flag */
    capi_dsp_debug_dump_info_t    dbg_dump[CAPI_DSP_DUMP_STAGE_MAX];    /*!< output: dsp_debug_dump */
} capi_dsp_debug_dump_t;                   /**< DSP DEBUG DUMP Information type              */


/**
 * The struct definition for hardware GAIN2 adaptation configuration
 */
typedef struct dsp_hw_gain2_adapt_cfg_info_s {
    uint8_t        hw_gain2_adapt_enable;          /**<hardware gain2 adaptation enable/disable [0, 1] 0-disable, 1; 1-enable; default is disabled */
} dsp_hw_gain2_adapt_cfg_info_t;                 /**< DSP hardware gainw2 adaptation configuration define                      */


/**
 * The struct definition for GAIN3 MANUAL A parameter configuration
 */
typedef struct dsp_gain3_a_param_cfg_info_s {
    uint8_t         gain3_a_override;       /** GAIN3 MANUAL A parameter enable/disable [0, 1] 0-disable, 1; 1-enable; default is disable */
    int8_t          gain3_a_oval;           /** GAIN3 MANUAL A parameter [-64, 63]. value could be 0 when gain3_a_override is set to 0 */
} dsp_gain3_a_param_cfg_info_t;             /**< DSP media GAIN3 MANUAL A parameter configuration define                      */

/**
 * The struct definition to get DSP Rx status: vga, afe_bw, peaking, adc_clipping, tap_saturation, phase_bias, snr, ppm, ffe taps, ...
 */
typedef struct dsp_rx_diag_status_s {
    // get DSP Rx status: vga, clipping, tap-saturation register reads, static phase bias value, current phase bias, lvl_snr, ffe taps
    uint16_t vga;                    /**<  vga */ 
    int8_t  c_ph_bias;               /**<  current phase bias  */
    int8_t  s_ph_bias;               /**<  static phase bias */
    int8_t  afe_bw;                  /**<  AFE BW  */
    uint8_t gain_boost;              /**< gain boost */
    uint8_t peaking;                 /**< peaking filter */
    uint8_t processor_status;        /**< processor status, 4 bits, 1 - processor alive, 0 - processor hang */
    uint8_t cdr_lock_counter;       /**<  CDR lock counter, 4 bits */ 
    uint8_t reserved[3];
    float   tx_ppm;                  /**<   tx ppm */
    float   rx_ppm;                  /**<   rx ppm */
    uint16_t port_status[8];         /**<   port fsm state */
    uint32_t adc_clip_cnt;           /**< ADC clipping count */ 
    uint32_t tap_under_saturation;   /**<  tap under saturation */
    uint32_t tap_over_saturation;    /**<  tap over saturation */
    float snr;                       /**<  total snr done */
    float level_snr[7];              /**<  level snr */
    float ffe_taps[9];               /**<   ffe taps */
    float gain3_a;                   /**<   gain3 a value */
} dsp_rx_diag_status_t;                 /**< DSP Rx status define                      */

/**
 * enumeration to represent the diagnostics dump type
 */
typedef enum diag_dump_type_e {
    DIAG_DUMP_TYPE_NONE = 0,                                             /**< Diag Dump type : None */
    DIAG_DUMP_TYPE_DSP  = 1,                                             /**< Diag Dump type : DSP  */
} diag_dump_type_t;                                                    /**< Diag Dump type        */

/**
 * enumeration to represent the DSP diagnostics dump type
 */  
typedef enum dsp_diag_dump_level_e {
    DSP_DIAG_LANE = 1,
    DSP_DIAG_CORE = 1<<1,
    DSP_DIAG_LANE_CONFIG = 1<<2,
    DSP_DIAG_LEVEL_1 = (DSP_DIAG_LANE),
    DSP_DIAG_LEVEL_2 = (DSP_DIAG_LEVEL_1 | DSP_DIAG_LANE_CONFIG),
    DSP_DIAG_LEVEL_3 = (DSP_DIAG_LEVEL_2 | DSP_DIAG_CORE),
} dsp_diag_dump_level_t;

/**
 * The structure of FW diagnostics dump 
 */
typedef struct capi_diag_dump_s {
    uint16_t    diag_dump_type;                            /**< [In] Diagnostic dump type : refer to the diag_dump_type_t                          */
    uint16_t    diag_dump_level;                           /**< [In] Diagnostic dump level                                                    */
                                                           /**< for the diag dump type DIAG_DUMP_TYPE_DSP refer to dsp_diag_dump_level_t */
    memory_info_t memory_info;                             /**< [In/Output] Memory details, returned by FW                         */
    uint8_t*      dump_buf_ptr;                           /**< [Output] Dump data buffer pointer                               */
    uint32_t      dump_buf_size;                          /**< [Output] Dump data buffer size                                  */
} capi_diag_dump_t;                                       /**< CAPI Diag Dump type                                            */

/**
 * The structure for Diag Command Request Information
 */
typedef struct capi_diag_command_request_info_s {
    uint16_t command_id;                  /**< Command identifier; refer to phy_command_id_t                  */

    union {
        memory_data_info_t                           cmis_snr_ltp;                /**< CMIS SNR LTP                                                   */
        dsp_mpi_cfg_info_t                             mpi_cfg;                       /**< MPI config information                                         */
        dsp_mpi_st_info_t                               mpi_st;                          /**< MPI state information                                          */

        dsp_mission_mpi_cfg_info_t             mission_mpi_cfg;       /**< dynamic MPI config information                                         */
        dsp_mission_mpi_st_info_t               mission_mpi_st;          /**< dynamic MPI state information                                          */

        dsp_tmsd_cfg_info_t                           tmsd_cfg;                     /**< DSP incoming traffic mode switch from 10XG to 5Xg detect  config*/
        dsp_tmsd_st_t                                      tmsd_st;                       /**< DSP incoming traffic mode switch from 10XG to 5Xg detect  state*/

        dsp_mpi_canceller_cfg_info_t            mpi_canceller_cfg;    /**< MPI canceller config*/
        dsp_hw_gain2_adapt_cfg_info_t       hw_gain2_a_cfg;       /**< hardware gain2 adaptation config*/
        capi_dsp_debug_dump_t                   ddump;                    /**< dsp_debug_dump                                          */
        capi_diag_dump_t                    dsp_diag_dump;            /**< DSP diagnostics dump  */         
        dsp_gain3_a_param_cfg_info_t      gain3_a_cfg;                   /*gain3 manaul a paramter config*/
        dsp_rx_diag_status_t              rx_diag_status;                /**< Rx diag status*/
    } type;                                                                                               /**< Payload/data type of the command Id                            */

} capi_diag_command_request_info_t;       /**< CAPI Diag Command Request Information                          */




/**
 * The struct of feature information
 */
typedef struct capi_feature_info_s {
    union {
        uint16_t content;

        struct {
            #ifdef CAPI_IS_BIG_ENDIAN
            uint16_t reserved                      :15;  /**< Reserved                                 */
            uint16_t disable_sanity_checker        : 1;  /**< Disable Sanity Checker                   */
            #else  /* CAPI_IS_LITTLE_ENDIAN */
            uint16_t disable_sanity_checker        : 1;  /**< Disable Sanity Checker                   */
            uint16_t reserved                      :15;  /**< Reserved                                 */
            #endif /* CAPI_IS_BIG_ENDIAN */
        } is;
    } param;

    union {
        uint16_t content;

        struct {
            #ifdef CAPI_IS_BIG_ENDIAN
            uint16_t reserved                      :15;  /**< Reserved                                 */
            uint16_t disable_sanity_checker        : 1;  /**< Disable Sanity Checker                   */
            #else  /* CAPI_IS_LITTLE_ENDIAN */
            uint16_t disable_sanity_checker        : 1;  /**< Disable Sanity Checker                   */
            uint16_t reserved                      :15;  /**< Reserved                                 */
            #endif /* CAPI_IS_BIG_ENDIAN */
        } is;
    } value;
} capi_feature_info_t;                                   /**< Feature Information type                 */


/**
 * The structure of cAPI PLL Info
 */
typedef struct capi_pll_info_s {
    /*config information*/
    uint8_t    enable_pll_dmode;                      /**< [0:1] 0- default setting; DSP config PLL1 or PLL2 mode */
    uint8_t    reg_divider_ratio;                     /**<[0:3]  REG divider ratio, only for HOST side; for media side, please keep this value to 0 */
    uint8_t    enable_frc_frac_mode;                  /**< [0:1] 0- default setting; 1-enable force fraction mode */
    uint16_t   frc_frac_value;                        /**< [0:0xFFFF] if 0, FW takes 1114 */
    uint8_t    enable_auto_pll_mode;                  /**< [0, 1]  0-default setting, disable auto pll mode; 1-enable auto pll mode*/
    uint8_t    pll_bandwidth;                         /**< [0:2] 0- default setting; if it is PLL2 mode, then PLL bw could support other two options */
    uint8_t    pad[1];                  
} capi_pll_info_t;                  /**< PLL information type  */



/**
 * The structure of command information 
 */
typedef struct capi_command_info_s {
    uint32_t command_id;                                   /**< Command Identifier, refer phy_command_id_t */

    union {
        tx_driver_t                 tx_driver;                  /**< Tx Driver                             */
        txpi_override_t             txpi_ovrd;                  /**< TxPI Override                         */
        capi_polarity_info_t        polarity_info;              /**< Polarity Info                         */
        capi_lane_ctrl_info_t       lane_ctrl_info;             /**< Lane Control Info                     */
        capi_config_info_t          config_info;                /**< Config Info                           */
        capi_port_info_t            port_info;                  /**< Port information                      */
        capi_loopback_info_t        lpbk_ctrl_info;             /**< Loopback Control Info                 */
        capi_prbs_info_t            prbs_info;                  /**< PRBS Info for set/get PRBS info       */
        capi_prbs_status_t          prbs_status_info;           /**< PRBS Status Info                      */
        capi_prbs_err_inject_t      prbs_err_inj_info;          /**< PRBS Error Injection Info             */
        capi_diag_lane_status_t     diag_lane_status;           /**< Diag Lane Status                      */
        capi_rptr_fec_st_t          rptr_fec_st;                /**< repeater mode FEC statistics          */
        capi_lane_info_t            lane_info;                  /**< Lane info                             */
        capi_lane_config_info_t     lane_config_info;           /**< Lane Config Info                      */
        capi_archive_info_t         archive_info;               /**< Archive info                          */
        capi_recovered_clock_info_t recovered_clock_info;       /**< Recovered Clock Information           */
        capi_cmis_info_t            cmis_diag_info;             /**< Usr Diag info                         */
        uint32_t                    linkcat_profile_addr;       /**< Linkcat profile address               */
        /* TODO: endianess */
        uint8_t                     dsp_opt_los_host_tx_unsquelch;  /**< option/feature to unsquelch client TX upon deassertion of OPRXLOS.
                                                                        0 : feature disabled    1 : feature enabled */
        capi_diag_command_request_info_t  diag_cmd_req_info;        /**< diag command request info */

        capi_pll_info_t             pll_info;                   /**< PLL Information                                     */
        capi_status_info_tt         status_info;                /**< status info                           */
        uint8_t                     dsp_lane_sr2;   /**< option/feature to set lane to SR2
                                                                        0 : disable SR2   1 : enable SR2 */
        uint8_t                     payload[150];               /**< Payload                               */
    } type;                                                     /**< Payload/data type of the command Id   */

} capi_command_info_t;                                          /**< Command information type              */

/**
 * The enumeration of optical_or_electric_mode
 */
typedef enum capi_dsp_mode_type_e {
    CAPI_DSP_OPTICAL_MODE,    /**< Optical bench mode */
    CAPI_DSP_ELECTRIC_MODE    /**< Electric bench mode */
} capi_dsp_mode_type_t;            /**< bench mode type */

/**
 * The enumeration of GPIO pullup
 */
typedef enum capi_gpio_pull_updown_type_e {
    CAPI_GPIO_PULL_UP_ACTIVE,    /**< GPIO pullup */
    CAPI_GPIO_PULL_DOWN_ACTIVE    /**< GPIO pulldown */
} capi_gpio_pull_updown_type_t;            /**< GPIO pullup/pulldown */

/**
 * The enumeration of GPIO direction
 */
typedef enum capi_gpio_dir_type_e {
    CAPI_GPIO_INPUT,    /**< CAPI_GPIO_INPUT */
    CAPI_GPIO_OUTPUT    /**< CAPI_GPIO_OUTPUT */
} capi_gpio_dir_type_t;            /**< GPIO input */

/**
 * The structure of capi gpio information 
 */
typedef struct capi_gpio_info_s {
    uint8_t gpio_id;                 /**< GPIO ID */
    uint8_t  gpio_set_value;              /**< GPIO set value */
    uint8_t  gpio_get_value;              /**< GPIO get value */
    capi_gpio_pull_updown_type_t  pull_updown_type;      /**< GPIO TYPE: PULL UP or DOWN */
    capi_gpio_dir_type_t   dir_type;       /**< GPIO dir type*/
   
} capi_gpio_info_t;/**< capi gpio Information */

typedef enum capi_spi_ctrl_enable_e {
    CAPI_SPI_CTRIL_DISABLE = 0,  /*capi spi ctrl enable*/
    CAPI_SPI_CTRIL_EMABLE = 1,  /*capi spi ctrl disable.*/
} capi_spi_ctrl_enable_t;

/**
 * The structure of capi spi information 
 */
typedef struct capi_spi_info_s {
    capi_spi_ctrl_enable_t  spi_ctrl_enable;
} capi_spi_info_t;                                 /**< capi spi Information              */


/**
 * The enum defintion of PLL dmode
 */

typedef enum capi_pll_dmode_e {
    CAPI_PLL1   = 0,
    CAPI_PLL2   = 1,
    CAPI_PLL_DMODE_MAX
} capi_pll_dmode_t;



typedef enum capi_pll_bw_e {
    CAPI_PLL_BW_0   = 0,
    CAPI_PLL_BW_1   = 1,
    CAPI_PLL_BW_2   = 2,
    CAPI_PLL_BW_MAX
} capi_pll_bw_t;

/**
 * The enum defintion of REG divider ratio
 */

typedef enum capi_reg_divider_ratio_e {
    CAPI_REG_DIVIDER_RATIO_DEFAULT   = 0,
    CAPI_REG_DIVIDER_RATIO_64T       = 1,
    CAPI_REG_DIVIDER_RATIO_80T       = 2,
    CAPI_REG_DIVIDER_RATIO_96T       = 3,
    CAPI_REG_DIVIDER_RATIO_MAX
} capi_reg_divider_ratio_t;



/**
 * The enumeration for regulator component type
 */
typedef enum regulator_component_e {
    REGULATOR_COMPONENT_MASTER_VDDM = 0x01,     /**< Regulator Component type "Master Only VDDM" */
    REGULATOR_COMPONENT_MASTER_AVDD = 0x02,     /**< Regulator Component type "Master Only AVDD" */
    REGULATOR_COMPONENT_MASTER_AVS  = 0x04,     /**< Regulator Component type "Master Only AVS"  */
    REGULATOR_COMPONENT_MASTER_ALL  = 0x07      /**< Regulator Component type "Master All"       */
} regulator_component_t;     

/**
 * The enumeration for regulator component type
 */
typedef enum regulator_command_e {
   REGULATOR_CMD_DISABLE,                         /**< Regulator command : Disable   */
   REGULATOR_CMD_ENABLE,                          /**< TBD, not suppored yet    */
} regulator_command_t;     


/**
 * The enumeration for regulator info 
 */
typedef struct capi_regulator_info_s{
    regulator_component_t  component;            /**< Regulator component   */
    regulator_command_t    command;              /**< Regulator command    */

} capi_regulator_info_t;


/**
 * The structure of chip command information 
 */
typedef struct capi_chip_command_info_s {

    phy_command_id_t            command_id;                 /**< Command Identifier                    */

    union {
        capi_gpio_info_t         gpio_info;                  /**< SPI Mode Information                  */
        capi_lpm_info_t          lpm_info;                   /**< Low Power Mode Information            */
        capi_spi_info_t           spi_info;                   /**< SPI Mode Information                  */
        capi_feature_info_t     capi_feature_info;          /**< CAPI feature Information              */
        capi_pll_info_t             pll_info;                   /**< PLL Information                                     */
    } type;                                                 /**< Payload/data type of the command Id   */
} capi_chip_command_info_t;                                 /**< Command Information type              */


/**
 * The enumeration for yes and no
 */
typedef enum capi_yes_no_e {
    CAPI_NO,                             /*!< operation disable      */
    CAPI_YES                             /*!< operation enable       */
} capi_yes_no_t;                         /*!< Yes No type            */


/**
 * The enumeration for the internal regulator component type
 */
typedef enum capi_internal_reg_component_e {
    CAPI_INTERNAL_REGULATOR_COMPONENT_VDDM,     /**< Internal Regulator Component Type VDDM */
    CAPI_INTERNAL_REGULATOR_COMPONENT_AVDD      /**< Internal Regulator Component Type AVDD */
} capi_internal_reg_component_t;                /**< Internal Regulator Component type      */


/**
 * The structure is used for the internal regulator voltage setting
 */
typedef struct capi_internal_regulator_voltage_s {
    uint16_t                        voltage;                   /**< Voltage setting                                        */
    uint16_t                        returned_voltage;          /**< The CAPI returns the final read voltage for validation */
    capi_yes_no_t                   while_fw_running;          /**< 0:Firmware isn't running   1:Firmware is running       */
    capi_internal_reg_component_t   int_reg_component_type;    /**< Internal Regulator Component type                      */
} capi_internal_regulator_voltage_t;                           /**< Fixed vddm _voltage config info type                   */



/**
 * The structure of fast BRCM SNR information 
 */
typedef union dsp_fast_brcm_snr_cfg_s {
    uint32_t                           words;                       /**< BRCM SNR related configuration Content  */
    struct {
        #ifdef CAPI_IS_BIG_ENDIAN
        uint32_t reserved              : 18;                           /**< Reserved                                           */
        uint32_t lineid                : 3;                           /**< line ID 0~7                                        */
        uint32_t physide               : 1;                           /**< phy side 0:HOST 1:LINE                       */
        uint32_t snr_mval              : 4;                           /**< snr calculation mvalue                         */
        uint32_t snr_level             : 3;                           /**< snr_level: 2, 4                                   */
        uint32_t slice_mode            : 3;                           /**< slice_mode: NRZ, EPAM2, PAM4, EPAM4  */
        #else /* CAPI_IS_LITTLE_ENDIAN */
        uint32_t slice_mode            : 3;                           /**< slice_mode: NRZ, EPAM2, PAM4, EPAM4  */
        uint32_t snr_level             : 3;                           /**< snr_level: 2, 4                                    */
        uint32_t snr_mval              : 4;                           /**< snr calculation mvalue                          */
        uint32_t physide               : 1;                           /**< phy side 0:HOST 1:LINE                       */
        uint32_t lineid                : 3;                           /**< line ID 0~7                                        */
        uint32_t reserved              : 18;                           /**< Reserved                                           */
        #endif /*CAPI_IS_BIG_ENDIAN*/
    } fields;
}dsp_fast_brcm_snr_cfg_t;

typedef struct capi_fast_brcm_snr_info_s {

    dsp_fast_brcm_snr_cfg_t snr_cfg;                                                            /**< SNR configuation items                         */
    float average_snr;                                                    /**< SNR average value                         */
    float lvl_snr[4];                                                     /**< SNR leveled  value                         */
} capi_fast_brcm_snr_info_t;                                        /**< BRCM SNR information               */

#ifdef __cplusplus
}
#endif

#endif //CAPI_DEF_H


/**
 *
 * @file     lw_tx_def.h
 * @author   various
 * @date     9/7/2018
 * @version 1.0
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
 * @brief   This file includes LINE WRAPPER (LW) TX IP block defines.
 *
 * @section
 * 
 */

#ifndef LW_TX_DEF_H
#define LW_TX_DEF_H

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *
 * Definition -- enum, structure
 *
******************************************************************************/

typedef enum {
    PTGEN_BYPASS = 0,
    PTGEN_PAM4_PRBS = 1,
    PTGEN_CL94_PRBS = 2,
    PTGEN_PCID = 3,
    PTGEN_SSPRQ = 4,
    PTGEN_PATTERN_GEN = 5
} lw_ptgen_mode_t;

typedef enum {
  STATIC_TAP     = 0,
  LUT4TAP        = 1,
  LUT7TAP        = 2,
  LIN10TAP       = 3,    /* Linear 10 Tap */
  NONLIN10TAP    = 4,    /* Non Linear 10 Tap */
  NONLINWFTAP    = 5     /* Non Linear with 3 Float Tap */
}lw_200_txfir_type_e;

/**
 * The structure of TxFIR info
 */
typedef struct lw_txfir_info_s {
    uint8_t      numb_of_taps;                         /**< Enabled TxFIR taps : refer to txfir_taps_t                */
    int16_t      tap[13];                               /**< TxFIR tap[pre2, pre1, main, post1, post2, post3, post4]   */
    int8_t       level_shift[4];                       /**< TxFIR level shift                                         */
    uint8_t      txfir_ftap_sel;                       /**< {i} LW TXFIR ftap_sel*/

} lw_txfir_info_t;                                     /**< TxFIR Info                                                */


typedef uint8_t tx_fir_type_t;  // force the enum to 1 bytes

typedef struct lw_tx_cfg_s {

  uint8_t       nrz_mode;            /**< (i) LW TX channel NRZ mode */
  uint8_t       os_mode;             /**< (i) LW TX channel Oversample Mode 0: OSx1, 1:OSx2 */
  uint8_t       precode_en;          /**< (i) LW TX channel precode enable */
  uint8_t       graycode_en;         /**< (i) LW TX channel gray code enable */
  uint8_t       polarity_inv;        /**< (i) LW TX channel polarity invert enable */
  uint8_t       symbol_swap;         /**< (i) LW TX channel symbol sway enable  */
  uint8_t       tx_datapath_sel;     /**< (i) LW TX channel Select datapath from patgen, patmem, pcs, linktraing */
  uint8_t       prbs_sel;            /**< (i) LW TX channel Select PRBS type */
  uint8_t       tx_squelch  ;        /**< (i) LW TX channel set squelch */
  uint8_t       tx_disable;          /**< (i) LW TX channel  TX disable that send all 0's or all 1's */
  uint8_t       tx_user_override;    /**< (i) LW TX channel user overrides FIR coef */
  uint8_t       tx_prbs_or_squelch ; /**< (i) L W TX channel choose PRBS to squelch */
  uint8_t       tx_remote_lnktrn ;   /**< (i) LW TX channel  */
  uint8_t       tx_ignore_fault;     /**< (i) LW TX channel tell FW to not reset TX when fault is detected */
  tx_fir_type_t tx_fir_type;         /**< (i) LW TX channel choose FIR Type, Static for LUT */
  lw_txfir_info_t lw_txfir_info;     /**< (i) LW TX FIR 7 tap value */
  lw_txfir_info_t lw_txfir_info_bk;  /**< (i) LW TX FIR 7 tap value, backed for LT special case */
  
  uint8_t       txpi_ppm_en : 1;     /**< (i) LW TX channel Enable TX PI */
  uint8_t       analog_tx_high_swing:1;/**< {i} LW TX analog high swing*/
  uint8_t       tx_reserved :6;      /**< {i} LW TX analog high swing*/
  
  int16_t       tx_pre1_fir;         /**< (i) LW TX pre1 TX FIR value*/
  int16_t       tx_main_fir;         /**< (i) LW TX main TX FIR value*/
  int16_t       tx_post1_fir;        /**< (i) LW TX post1 TX FIR value*/
  int16_t       tx_post2_fir;        /**< (i) LW TX post2 TX FIR value*/
  int8_t        fir_static_lvl0;      /**< {i} LW TX static FIR for Level 0*/
  int8_t        fir_static_lvl1;      /**< {i} LW TX static FIR for Level 1*/
  int8_t        fir_static_lvl2;      /**< {i} LW TX static FIR for Level 2*/
  int8_t        fir_static_lvl3;      /**< {i} LW TX static FIR for Level 3*/
  uint8_t       analog_skewp;         /**< {i} LW TX analog skewp*/


} lw_tx_cfg_t;

typedef struct {
  uint8_t  tx_ctrl_11_8_ovrd;          /**< override enable tx_ctrl[11:8]*/
  uint8_t  tx_ctrl_11_8_ovrd_val;      /**< override value  tx_ctrl[11:8]*/
  uint8_t  tx_ctrl_111_109_ovrd;       /**< override enable tx_ctrl[111:109]*/
  uint8_t  tx_ctrl_111_109_ovrd_val;   /**< override value  tx_ctrl[111:109]*/
  uint8_t  tx_ctrl_120_118_ovrd;       /**< override enable tx_ctrl[120:118]*/
  uint8_t  tx_ctrl_120_118_ovrd_val;   /**< override value  tx_ctrl[120:118]*/
} tx_rescal_ovrd;


typedef struct {

    uint8_t dac_adptvbias_cml_0p75;
    uint8_t dac_adptvbias_cmh_0p75;
    uint8_t dac_adptvbias_swing_0p75;
    uint8_t dac_adptvbias_bw_op75;
    uint8_t dac_en_fixedbias;
    uint8_t dac_adptvbias_pdcascap;
    uint8_t hs_dac_current;
    uint8_t dac_ctrl_ocm_0p75;
} hs_tx_special_set_t;


/**
 * The structure of tx bias information
 */
typedef struct {
    uint8_t tx_bias_reg1;                                     /**<tx_bias_reg1: TX reg1 [0:15]        */
    uint8_t tx_bias_reg2;                                     /**<tx_bias_reg2: TX  reg2 [0:15]        */
    uint8_t tx_bias_reg3;                                     /**< tx_bias_reg3: TX reg3 [0:15]        */
    uint8_t tx_bias_reg4;                                    /**< tx_bias_reg4:TX reg4 [0:15]        */
} lw_tx_bias_info_t;                                   /**< TX bias information type                                                */

#ifdef __cplusplus
}
#endif

#endif

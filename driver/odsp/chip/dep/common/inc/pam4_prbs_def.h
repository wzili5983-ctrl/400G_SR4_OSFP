/**
 *
 * @file    pam4_prbs_def.h
 * @author  pjackson
 * @date    9/11/2018
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
 * @brief   This file includes PAM4 PRBS Generator/Monitor IP block defines.
 *
 */

#ifndef PAM4_PRBS_DEF_H
#define PAM4_PRBS_DEF_H

#ifdef __cplusplus
extern "C" {
#endif

//==========================================================================
//                               Definitions
//==========================================================================

/**
 * Structure of PAM4 PRBS Generator Configuration
 */
typedef struct {
    // Note: FEC PGENs only support PRBS58 & PRBS31
    uint8_t pam4_pgen_poly_sel;     /**<  58=PRBS58, 31=PRBS31, 7=PRBS7, 9=PRBS9, 10=PRBS10, 11=PRBS11, 13=PRBS13, 15=PRBS15, 20=PRBS20, 23=PRBS23, 49=PRBS49,rest=RSVD */
    uint8_t pam4_pgen_dout_inv;     /**<  1=> Invert PGEN Dout, 0=>Don't */
    uint8_t pam4_pgen_seed_id;      /**<  LFSR Seed = {'d0, 1'b1, pgen_seed_id, 1'b1} */
    uint8_t pam4_pgen_pbi_ena;      /**<  PGEN PBI Enable */
    uint8_t pam4_pgen_pbi_2x1xn;    /**<  PGEN PBI 2X/1X-Not */
} pam4_pgen_cfg_t; /**< PAM4 PRBS Generator configuration structure type */

typedef struct pam4_pgen_misc_cfg_s {
    uint8_t polysel;
    uint8_t pgen_inv;
    uint8_t pgen_init;
    onoff_t fec_mode_ena;
    onoff_t prbs_on_fault_ena;
    onoff_t pam4_mode;
    uint8_t nrz_ctrl;
} pam4_pgen_misc_cfg_t;


typedef struct prbs_on_fault_cfg_s {
    onoff_t fault_mode_ena;
    onoff_t data_mux_sel_ovrd;
    onoff_t data_mux_sel_oval;
} prbs_on_fault_cfg_t;


/*
 * Enumeration for inject PRBS mode type
 */
typedef enum prbs_err_inj_mode_s{
    PRBS_ERR_INJ_MODE_SINGLE        = 0x0,                          /**< Inject PRBS error once */
    PRBS_ERR_INJ_MODE_PER_1_CYCLE   = 0x1,                          /**< Inject PRBS error per 1 cycle */
    PRBS_ERR_INJ_MODE_PER_32_CYCLE  = 0x2,                          /**< Inject PRBS error per 32 cycles */
    PRBS_ERR_INJ_MODE_PER_256_CYCLE = 0x3,                          /**< Inject PRBS error per 256 cycles */

} prbs_err_inj_mode_t;

typedef struct prbs_gen_err_cfg_s {
   onoff_t odd_err_ena;
   onoff_t even_err_ena;
   prbs_err_inj_mode_t err_rate;
} prbs_gen_err_cfg_t;


typedef struct pam4_pmon_misc_cfg_s {
    uint8_t polysel;
    uint8_t pmon_inv;
    uint8_t pmon_init;
    onoff_t fec_mode_ena;
    onoff_t pam4_mode;
    uint8_t nrz_ctrl;
} pam4_pmon_misc_cfg_t;



typedef struct pam4_pmon_config_acc_cfg_s {
    uint8_t acc_mode;
    uint8_t burst_wdw;
    uint8_t burst_thr;
    uint8_t freeze_acc_val;
} pam4_pmon_config_acc_cfg_t;


typedef struct pam4_pmon_config_lock_cfg_s {
    uint8_t relock_ena;
    uint8_t unlock_thr;
    uint8_t unlock_tol;
    uint8_t lock_thr;
} pam4_pmon_config_lock_cfg_t;


/**
 * Structure of PAM4 PRBS Monitor Configuration
 */
typedef struct {
    // Note: FEC PMONs only support PRBS58 & PRBS31
    uint8_t pam4_pmon_poly_sel;   /**<  58=PRBS58, 31=PRBS31, 7=PRBS7, 9=PRBS9, 10=PRBS10, 11=PRBS11, 13=PRBS13, 15=PRBS15, 20=PRBS20, 23=PRBS23, 49=PRBS49,rest=RSVD */
    uint8_t pam4_pmon_din_inv;    /**<  1=> Invert PMON Din, 0=>Don't */
    uint8_t pam4_pmon_din_swap;    /**<   */
    uint8_t pam4_pmon_din_gray;    /**<  */
    uint8_t pam4_pmon_lock_thr;   /**<  PMON lock threshold   (0-15)  */
    uint8_t pam4_pmon_unlock_thr; /**<  PMON unlock threshold (0-15)  */
    uint8_t pam4_pmon_unlock_tol; /**<  PMON unlock tolerance (0-127) */
    uint8_t pam4_pmon_relock_ena; /**<  PMON relock enable (1=>Relock is enabled) */
    uint8_t pam4_pmon_os_mode;    /**< 0=>T8, 1=>T12 (Line Wrapper RX PMON Only)  */
    uint8_t pam4_pmon_even_oddn;  /**< 0=>T8, 1=>T12 (Line Wrapper RX PMON Only)  */
    uint8_t pam4_pmon_pam4_mode;  /**< 0=>NRZ, 1=>PAM4 */
} pam4_pmon_cfg_t; /**< PAM4 PRBS Monitor configuration structure type */

/**
 * Structure of PAM4 PRBS Monitor Status
 */
typedef struct {
    uint32_t pam4_pmon_acc_odd;    /**< Odd/Burst  BER Accumulator */
    uint32_t pam4_pmon_acc_even;   /**< Even/Total BER Accumulator */
    uint32_t pam4_oflow_odd;       /**< Odd overflow indication */
    uint32_t pam4_oflow_even;      /**< Even overflow indication */
    uint8_t  pam4_pmon_lock_flag;  /**< Lock Flag */
    uint8_t  pam4_pmon_lol_flag;   /**< LOL Flag  */
} pam4_pmon_stat_t; /**< PAM4 PRBS Monitor status structure type */


#ifdef __cplusplus
}
#endif

#endif

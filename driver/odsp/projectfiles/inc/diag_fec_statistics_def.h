/**
 *
 * @file diag_fec_statistics.h 
 * @author  
 * @date     9/01/2018
 * @version 1.0
 *
  * @property   $Copyright: Copyright 2018 Broadcom INC. 
 *This program is the proprietary software of Broadcom INC
 *and/or its licensors, and may only be used, duplicated, modified
 *or distributed pursuant to the terms and conditions of a separate,
 *written license agreement executed between you and Broadcom
 *(an "Authorized License").  Except as set forth in an Authorized
 *License, Broadcom grants no license (express or implied), right
 *to use, or waiver of any kind with respect to the Software, and
 *Broadcom expressly reserves all rights in and to the Software
 *and all intellectual property rights therein.  IF YOU HAVE
 *NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE
 *IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE
 *ALL USE OF THE SOFTWARE.
 *
 *Except as expressly set forth in the Authorized License,
 *
 *1.     This program, including its structure, sequence and organization,
 *constitutes the valuable trade secrets of Broadcom, and you shall use
 *all reasonable efforts to protect the confidentiality thereof,
 *and to use this information only in connection with your use of
 *Broadcom integrated circuit products.
 *
 *2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS
 *PROVIDED "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
 *REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
 *OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
 *DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
 *NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
 *ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 *CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
 *OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 *3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL
 *BROADCOM OR ITS LICENSORS BE LIABLE FOR  CONSEQUENTIAL,
 *INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER
 *ARISING OUT OF OR IN ANY WAY RELATING TO YOUR USE OF OR INABILITY
 *TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF THE
 *POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF
 *THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR USD 1.00,
 *WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING
 *ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.$
 * @brief   this file includes cHAL function implementation.
 *
 * @section
 * 
 */
#ifndef DIAG_FEC_STATISTICS_DEF_H
#define DIAG_FEC_STATISTICS_DEF_H

#ifdef __cplusplus
extern "C" {
#endif

#define KP4_FEC_TOT_BITS_CORR_NUM 2      /**< KP4 FEC TOTAL BITS CORRECT NUMBER */
#define KP4_FEC_TOT_FRAMES_ERR_NUM 16    /**< KP4 FEC TOTAL FRAMES ERROR NUMBER */
#define MAX_FEC_LANE_NUM 16              /**< KP4 FEC LANE NUMBER */

typedef struct {
    uint16_t err_cntr_lo; /**< low 16-bit of 48-bit KP4FEC error counter */
    uint16_t err_cntr_md; /**< mid 16-bit of 48-bit KP4FEC error counter */
    uint16_t err_cntr_hi; /**< hi  16-bit of 48-bit KP4FEC error counter */
} kpr4fec_err_cntr_s;

typedef union {
    uint64_t err_cnt;        /**< 64 bits error counter */
    kpr4fec_err_cntr_s err;  /**< separated 16 bits error counter */
} kpr4fec_err_cnt_u;

typedef struct
{
    kpr4fec_err_cnt_u kpr4fec_tot_frame_cnt; /**<  KP4FEC total recieved frame counter */
    kpr4fec_err_cnt_u kpr4fec_tot_frame_uncorr_cnt;    /**<  KP4FEC uncorrectable frame count*/
    kpr4fec_err_cnt_u kpr4fec_tot_bits_corr_cnt[KP4_FEC_TOT_BITS_CORR_NUM]; /**<  KP4FEC corrected bit count*/
    kpr4fec_err_cnt_u kpr4fec_tot_frames_err_cnt[KP4_FEC_TOT_FRAMES_ERR_NUM]; /**<  KP4FEC total error frame count*/
} kpr4fec_cnt_s;

typedef struct
{
    uint32_t kpr4fec_ieee_corr_cnt;      
    uint32_t kpr4fec_ieee_uncorr_cnt;
    uint32_t kpr4fec_ieee_symbols_corr_cnt[MAX_FEC_LANE_NUM];
} kpr4fec_ieee_cnt_s;

typedef struct diag_port_cfg_list_s {
    uint8_t val_idx;                 /**< value index   */
    uint8_t gap_num;                 /**< gap number   */
} diag_port_cfg_list_t;               /**< _fec_prbs_cfg_list_t type */

/**
 * The enumeration of AM lock
 */
typedef enum {
    KP4FEC_AM_UNLOCK,
    KP4FEC_AM_LOCKED,
} kpr4fec_lock_e;

typedef struct {
    uint16_t max_frame; /*!< Number of frames used for error injection (0 ~ 2^16-1) */
    uint8_t mask_a_id; /*!< 1st 80-bit column used for error injection (0 ~ 67) */
    uint8_t mask_b_id; /*!< 2nd 80-bit column used for error injection (0 ~ 67), this value should be larger than 'mask_a_id' */
    uint16_t mask_a_0; /*!< [15: 0] 16-bit error mask for 80-bit 'mask_a_id' */
    uint16_t mask_a_1; /*!< [31:16] 16-bit error mask for 80-bit 'mask_a_id' */
    uint16_t mask_a_2; /*!< [47:32] 16-bit error mask for 80-bit 'mask_a_id' */
    uint16_t mask_a_3; /*!< [63:48] 16-bit error mask for 80-bit 'mask_a_id' */
    uint16_t mask_a_4; /*!< [79:64] 16-bit error mask for 80-bit 'mask_a_id' */
    uint16_t mask_b_0; /*!< [15: 0] 16-bit error mask for 80-bit 'mask_b_id' */
    uint16_t mask_b_1; /*!< [31:16] 16-bit error mask for 80-bit 'mask_b_id' */
    uint16_t mask_b_2; /*!< [47:32] 16-bit error mask for 80-bit 'mask_b_id' */
    uint16_t mask_b_3; /*!< [63:48] 16-bit error mask for 80-bit 'mask_b_id' */
    uint16_t mask_b_4; /*!< [79:64] 16-bit error mask for 80-bit 'mask_b_id' */
} kpr4fec_err_inj_cfg_s;


#ifdef ENABLE_FEC_IEEE_INFO
typedef struct {
    uint8_t mode_200g_8ln_0;            /*!< 200G mode LN 0~7 */
    uint8_t mode_200g_4ln_0;            /*!< 200G mode LN 0~3 */
    uint8_t mode_100g_4ln_0;            /*!< 100G mode LN 0~3 */
    uint8_t mode_100g_2ln_0;            /*!< 100G mode LN 0~1 */
    uint8_t mode_100g_2ln_2;            /*!< 100G mode LN 2~3 */
    uint8_t mode_100g_4ln_4_m3d;        /*!< 100G mode LN 4~7 */
    uint8_t mode_50g_2ln_0;             /*!< 50G mode LN 0~1 */
    uint8_t mode_50g_2ln_2;             /*!< 50G mode LN 2~3 */
    uint8_t mode_50g_2ln_2_m4d;         /*!< 50G mode LN 2~3 */
    uint8_t mode_50g_2ln_4_m4d;         /*!< 50G mode LN 4~5 */
    uint8_t mode_50g_2ln_6_m4d;         /*!< 50G mode LN 6~7 */
    uint8_t mode_50g_1ln_0;             /*!< 50G mode LN 0 */
    uint8_t mode_50g_1ln_1;             /*!< 50G mode LN 1 */
    uint8_t mode_50g_1ln_2;             /*!< 50G mode LN 2 */
    uint8_t mode_50g_1ln_3;             /*!< 50G mode LN 3 */
    uint8_t mode_25g_1ln_0;             /*!< 25G mode LN 0 */
    uint8_t mode_25g_1ln_1;             /*!< 25G mode LN 1 */
    uint8_t mode_25g_1ln_2;             /*!< 25G mode LN 2 */
    uint8_t mode_25g_1ln_3;             /*!< 25G mode LN 3 */

    uint8_t mode_ln0_rs544_fec_en;      /*!< LN0 using RS544 */
    uint8_t mode_ln1_rs544_fec_en;      /*!< LN1 using RS544 */
    uint8_t mode_ln2_rs544_fec_en;      /*!< LN2 using RS544 */
    uint8_t mode_ln3_rs544_fec_en;      /*!< LN3 using RS544 */
    uint8_t mode_ln0_rs528_fec_en;      /*!< LN0 using RS528 */
    uint8_t mode_ln1_rs528_fec_en;      /*!< LN1 using RS528 */
    uint8_t mode_ln2_rs528_fec_en;      /*!< LN2 using RS528 */
    uint8_t mode_ln3_rs528_fec_en;      /*!< LN3 using RS528 */
    uint8_t mode_ln0_dec_fwd_en;        /*!< LN0 using FEC_DEC_FWD */
    uint8_t mode_ln1_dec_fwd_en;        /*!< LN1 using FEC_DEC_FWD */
    uint8_t mode_ln2_dec_fwd_en;        /*!< LN2 using FEC_DEC_FWD */
    uint8_t mode_ln3_dec_fwd_en;        /*!< LN3 using FEC_DEC_FWD */
    uint8_t mode_ln0_berlinetta_mode;   /*!< LN0 using BERLINETTA mode */
    uint8_t mode_ln1_berlinetta_mode;   /*!< LN1 using BERLINETTA mode */
    uint8_t mode_ln2_berlinetta_mode;   /*!< LN2 using BERLINETTA mode */
    uint8_t mode_ln3_berlinetta_mode;   /*!< LN3 using BERLINETTA mode */
} kpr4fec_modes_sts_s;
#endif



#ifdef __cplusplus
}
#endif

#endif

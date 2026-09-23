/**
 *
 * @file cw_def.h 
 * @author  
 * @date     9/1/2018
 * @version 1.0
 *
 * * @property   $Copyright: Copyright 2018 Broadcom INC. 
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
 *
 * @brief   this file includes cHAL function implementation.
 *
 * @section
 * 
 */
#ifndef CW_DEF_H
#define CW_DEF_H


#ifdef __cplusplus
extern "C" {
#endif

 
#define INNER_FEC_TOT_BITS_CORR_NUM 2

/**
 * The enumeration of port enable
 */
typedef enum {
    PORT_OFF,   /*!< port off*/
    PORT_ON     /*!< port on*/
} cfg_port_en_t; /*!< cw port enable enum */

/**
 * The enumeration of EGR/IGR
 */
typedef enum {
    EGR,   /*!< EGR */
    IGR    /*!< IGR */
} cfg_egr_or_igr_t; /*!< cw egr or igr enum */

/**
 * The enumeration of PAM or NRZ port for CW
 */
typedef enum {
    CW_NRZ,   	/*!< NRZ */
    CW_PAM,   	/*!< PAM 50G */
    CW_PAM100   /*!< PAM 100G */
} cfg_pam_or_nrz_t; /*!< cw port PAM 50/100 or NRZ */   

/**
 * The enumeration of cw port host or line
 */
typedef enum {
    HOST, /*!< HOST */
    LINE  /*!< NRZ */
} cfg_host_or_line_t; /*!< cw port HOST or LINE */

/**
 * The enumeration of cw port tx or rx
 */
typedef enum {
    IS_TX, /*!< Transmit */
    IS_RX  /*!< Receive */
} cfg_direction_t; /*!< cw port direction*/

/**
 * The enumeration of cw repeater or retimer path
 */
typedef enum {
    REPEATER_PATH, /*!< repeater */
    RETIMER_PATH,   /*!< retimer */
    CW_PATH_MAX
} cfg_dp_type_t; /*!< cw repeater or retimer*/

/**
 * The enumeration of repeater mode
 */
typedef enum {
    DIRECT, 	/*!< one to one mapping */
    GEARBOX,   	/*!< GEAR MODE */
    R_GEARBOX	/*!< REVERSED GEARBOX MODE */ 
} cfg_rpt_type_t; /*!< cw repeater type */

/**
 * The enumeration of cw logic bundle speed
 */
typedef enum {
    SPEED_400G = 1,  /*!< 400g */
    SPEED_200G = 2, /*!< 200g */
    SPEED_100G = 3, /*!< 100g */
    SPEED_50G  = 4, /*!< 100g */
    SPEED_25G  = 5, /*!< 100g */
} cfg_speed_t; /*! cw logic speed */

/**
 * The enumeration of cw  fec termination mode
 */
typedef enum {
    BIT_SYMBOL_MUX,         /*!< bitmux to symbol mux, can be derived from fec_type=no_fec, and line symbol_mux=1*/
    FEC_DEC_XDEC_XENC_ENC,  /*!< only for 100g NRZ mode, terminate fec with decoder and then goes back to xdecoder-xencoder then fec encode*/
    FEC_DEC_ENC_KRKP,       /*!< fec_dec_enc mode, but host is kr, line is kp. Go through addition krkp gbox */ 
    FEC_DEC_ENC_KPKR,       /*!< fec_dec_enc mode, but host is kp, line is kr. Go through addition kpkr gbox */ 
    FEC_DEC_ENC,            /*!< fec decoder and eocoder back*/
    FEC_DEC_FWD,            /*!< fec decoder and pass payload and parity to transimit without encoding again to save latency*/
    PCS_XENC,               /*!< host is pcs, line is fec  */
    XENC_PCS                /*!< host is fec, line is pcs  */
} cfg_fec_dec_enc_mode_t;   /*!< fec termination type */


/**
 * The enumeration of cw line fec symbol mux on or off 
 */
typedef enum {
    SYMBOL_MUX_OFF,  /*!< line symbol mux off*/
    SYMBOL_MUX_ON    /*!< line symbol mux on*/
} cfg_symbol_mux_t;  /*!< line symbol mux */

/**
 * The enumeration of cw rtm slices
 */
typedef enum {
    SLICE0,  /*!< active data in slice0, 50g per slice*/
    SLICE1,  /*!< active data in slice1*/
    SLICE2,  /*!< active data in slice2*/
    SLICE3,  /*!< active data in slice3*/
    SLICE4,  /*!< active data in slice4*/
    SLICE5,  /*!< active data in slice5*/
    SLICE6,  /*!< active data in slice6*/
    SLICE7   /*!< active data in slice7*/
} cfg_rtm_slice_t; /*!< cw retimer active 50g data slices */                                                                                                                                                      
/**
 * The enumeration of cw fec prbs
 */
typedef enum {
    PRBS_OFF,  /*!< fec prbs off*/
    PRBS_ON    /*!< fec prbs on*/
} cfg_fec_prbs_t;  /*!< fec prbs */

/**
 * The enumeration of cw pcs test-pattern
 */
typedef enum {
    PCS_TPAT_OFF,  /*!< pcs test pattern off*/
    PCS_TPAT_ON    /*!< pcs test pattern on*/
} cfg_pcs_tpat_t;  /*!< pcs test pattern */

/**
 * The enumeration of cw fec enc input datamux 
 */
typedef enum {
    PRBS_DATA,       /*!< fec encoder take PRBS gen data output 0x0 */
    FEC_DEC_DATA,    /*!< fec encoder take fec decoder data output 0x1 */
    XENC_DATA        /*!< fec encoder take xencoder data, only valid for 100g modes 0x2*/
} cfg_fec_enc_datamux_t;  /*!< fec encoder data mux selection */

/**
 * The enumeration of cw fec receiver output datamux 
 */
typedef enum {
    FEC_SYNC_DATA,       /*!< fec receiver output is  fec sync data output 0x0 */
    KPKR_DATA,           /*!< fec receiver output is  kpkr gbox data output 0x1 , only valid in 100g modes*/
    RSFEC_DEC_DATA,      /*!< fec receiver output is  fec decoder data, 0x2*/
} cfg_fec_rcv_datamux_t;  /*!< fec receiver data output mux selection */

/**
 * The enumeration of cw fec sym/bit datamux  input
 */
typedef enum {
    FEC_ENC_DATA,             /*!< Bypass symbol distribution, used in bitmux_sym_mux mode 0x0 */
    SYMDIST_DATA               /*!< Use symbol distribution output. All other modes*/
} cfg_fec_symbdist_datamux_t;  /*!< fec sym/bit data input mux selection */



/**
 * The enumeration of cw simulation modes
 */
typedef enum {
////////////////////////////////////////////////////////////////////////////////////////////////////
// Direct mode 
    MODE_DIR_NRZ,			/*!< repeater direct mode, NRZ <> NRZ */ 
    MODE_DIR_PAM50,			/*!< repeater direct mode, PAM 50 <> PAM 50 */ 
    MODE_DIR_PAM100,			/*!< repeater direct mode, PAM 100 <> PAM 100 */ 
   
// GEARBOX mode 
    MODE_PAM50_PAM100,			/*!< repeater gearbox mode, PAM50 <> PAM 100 */
    MODE_NRZ_PAM50,			/*!< repeater gearbox mode, NRZ <> PAM 50 */
    MODE_NRZ_PAM100_P1,			/*!< repeater gearbox mode, NRZ <> PAM 100, use port 1 */
    MODE_NRZ_PAM100_P2,			/*!< repeater gearbox mode, NRZ <> PAM 100, use port 2 */


////////////////////////////////////////////////////////////////////////////////////////////////////
// Retimer Direct mode 25G NRZ <> NRZ
    MODE_25G_NRZ_DEC_FWD_RS528_RS528,           /*!< retimer mode 8, 1x25G NRZ <> 1x25G NRZ */
    MODE_25G_NRZ_DEC_ENC_RS528_RS528,           /*!< retimer mode 8, 1x25G NRZ <> 1x25G NRZ */
    MODE_25G_NRZ_XDEC_XENC_RS528_RS528,         /*!< retimer mode 8, 1x25G NRZ <> 1x25G NRZ */

// Retimer Direct mode 50G NRZ <> NRZ
    MODE_50G_NRZ_DEC_FWD_RS528_RS528,           /*!< retimer mode 12,  2x25G NRZ <> 2x25G NRZ */
    MODE_50G_NRZ_DEC_ENC_RS528_RS528,           /*!< retimer mode 12,  2x25G NRZ <> 2x25G NRZ */
    MODE_50G_NRZ_XDEC_XENC_RS528_RS528,         /*!< retimer mode 12,  2x25G NRZ <> 2x25G NRZ */
    MODE_50G_NRZ_PCS_XENC_RS528,                /*!< retimer mode 11,  2x25G NRZ <> 2x25G NRZ */
    MODE_50G_NRZ_XENC_PCS_RS528,                /*!< retimer mode 11R, 2x25G NRZ <> 2x25G NRZ */

// Retimer Direct mode 50G PAM <> PAM
    MODE_50G_PAM_DEC_FWD_RS544_RS544,           /*!< retimer mode 18, 1x53G PAM <> 1x53G PAM */
    MODE_50G_PAM_DEC_ENC_RS544_RS544,           /*!< retimer mode 18, 1x53G PAM <> 1x53G PAM */
    MODE_50G_PAM_XDEC_XENC_RS544_RS544,         /*!< retimer mode 18, 1x53G PAM <> 1x53G PAM */

// Retimer Fwd Gbox mode 50G NRZ <> PAM
    MODE_50G_NRZ2PAM_PCS_XENC_RS544_M0,         /*!< retimer mode 20,  2x25G NRZ <> 1x53G PAM */
    MODE_50G_NRZ2PAM_KRKP_RS528_RS544_M0,       /*!< retimer mode 21,  2x25G NRZ <> 1x53G PAM */
    MODE_50G_NRZ2PAM_XDEC_XENC_RS528_RS544_M0,  /*!< retimer mode 21,  2x25G NRZ <> 1x53G PAM */

    MODE_50G_NRZ2PAM_PCS_XENC_RS544_M1,         /*!< Portofino retimer mode 11b,  2x25G NRZ <> 1x53G PAM */
    MODE_50G_NRZ2PAM_KRKP_RS528_RS544_M1,       /*!< Portofino retimer mode 11b,  2x25G NRZ <> 1x53G PAM */
    MODE_50G_NRZ2PAM_XDEC_XENC_RS528_RS544_M1,  /*!< Portofino retimer mode 11b,  2x25G NRZ <> 1x53G PAM */

    MODE_50G_NRZ2PAM_PCS_XENC_RS544_M2,         /*!< retimer mode 20,  2x25G NRZ <> 1x53G PAM */
    MODE_50G_NRZ2PAM_KRKP_RS528_RS544_M2,       /*!< retimer mode 21,  2x25G NRZ <> 1x53G PAM */
    MODE_50G_NRZ2PAM_XDEC_XENC_RS528_RS544_M2,  /*!< retimer mode 21,  2x25G NRZ <> 1x53G PAM */

// Retimer Rev Gbox mode 50G PAM <> NRZ
    MODE_50G_PAM2NRZ_XENC_PCS_RS544_M0,         /*!< retimer mode 20R, 1x53G PAM <> 2x25G NRZ */
    MODE_50G_PAM2NRZ_KPKR_RS544_RS528_M0,       /*!< retimer mode 21R, 1x53G PAM <> 2x25G NRZ */  
    MODE_50G_PAM2NRZ_XDEC_XENC_RS544_RS528_M0,  /*!< retimer mode 21R, 1x53G PAM <> 2x25G NRZ */

    MODE_50G_PAM2NRZ_XENC_PCS_RS544_M1,         /*!< retimer mode 20R, 1x53G PAM <> 2x25G NRZ */
    MODE_50G_PAM2NRZ_KPKR_RS544_RS528_M1,       /*!< retimer mode 21R, 1x53G PAM <> 2x25G NRZ */
    MODE_50G_PAM2NRZ_XDEC_XENC_RS544_RS528_M1,  /*!< retimer mode 21R, 1x53G PAM <> 2x25G NRZ */

    MODE_50G_PAM2NRZ_XENC_PCS_RS544_M2,         /*!< retimer mode 20R, 1x53G PAM <> 2x25G NRZ */
    MODE_50G_PAM2NRZ_KPKR_RS544_RS528_M2,       /*!< retimer mode 21R, 1x53G PAM <> 2x25G NRZ */
    MODE_50G_PAM2NRZ_XDEC_XENC_RS544_RS528_M2,  /*!< retimer mode 21R, 1x53G PAM <> 2x25G NRZ */

// Retimer Direct mode 100G NRZ <> NRZ
    MODE_100G_NRZ_DEC_FWD_RS528_RS528,          /*!< retimer mode 24,  4x25G NRZ <> 4x25G NRZ */
    MODE_100G_NRZ_DEC_ENC_RS528_RS528,          /*!< retimer mode 24,  4x25G NRZ <> 4x25G NRZ */
    MODE_100G_NRZ_XDEC_XENC_RS528_RS528,        /*!< retimer mode 24,  4x25G NRZ <> 4x25G NRZ */
    MODE_100G_NRZ_PCS_XENC_RS528,               /*!< retimer mode 25,  4x25G NRZ <> 4x25G NRZ */
    MODE_100G_NRZ_XENC_PCS_RS528,               /*!< retimer mode 25R, 4x25G NRZ <> 4x25G NRZ */

// Retimer Direct mode 100G PAM <> PAM
    MODE_100G_PAM_DEC_FWD_RS544_RS544,          /*!< retimer mode 33, 2x53G PAM <> 2x53G PAM */
    MODE_100G_PAM_DEC_ENC_RS544_RS544,          /*!< retimer mode 33, 2x53G PAM <> 2x53G PAM */
    MODE_100G_PAM_XDEC_XENC_RS544_RS544,        /*!< retimer mode 33, 2x53G PAM <> 2x53G PAM */

// Retimer Direct mode 100G PAM100 <> PAM100
    MODE_100G_PAM100_DEC_FWD_RS544_RS544,          /*!< Portofino retimer mode 1a,  PAM100 <>  PAM100 */
    MODE_100G_PAM100_DEC_ENC_RS544_RS544,          /*!< Portofino retimer mode 1a,  PAM100 <>  PAM100 */
    MODE_100G_PAM100_XDEC_XENC_RS544_RS544,        /*!< Portofino retimer mode 1a,  PAM100 <>  PAM100 */

// Retimer Direct mode 100G 2xPAM50 <> PAM100
    MODE_100G_PAM50PAM100_DEC_FWD_RS544_RS544_2C,          /*!< Portofino retimer mode 2c, 2x53G PAM100 <> PAM100 */
    MODE_100G_PAM50PAM100_DEC_ENC_RS544_RS544_2C,          /*!< Portofino retimer mode 2c, 2x53G PAM100 <> PAM100 */
    MODE_100G_PAM50PAM100_XDEC_XENC_RS544_RS544_2C,        /*!< Portofino retimer mode 2c, 2x53G PAM100 <> PAM100 */

// Retimer Direct mode 100G 2xPAM50 <> PAM100
    MODE_100G_PAM50PAM100_DEC_FWD_RS544_RS544_2D,          /*!< Portofino retimer mode 2d, 2x53G PAM100 <> PAM100 */
    MODE_100G_PAM50PAM100_DEC_ENC_RS544_RS544_2D,          /*!< Portofino retimer mode 2d, 2x53G PAM100 <> PAM100 */
    MODE_100G_PAM50PAM100_XDEC_XENC_RS544_RS544_2D,        /*!< Portofino retimer mode 2d, 2x53G PAM100 <> PAM100 */

// Retimer Fwd Gbox mode 100G NRZ <> PAM
    MODE_100G_NRZ2PAM_PCS_XENC_RS544_M0,        /*!< retimer mode 29, 4x25G NRZ <> 2x53G PAM, 01-0, 23-2,45-4, 67-6 */
    MODE_100G_NRZ2PAM_KRKP_RS528_RS544_M0,      /*!< retimer mode 30, 4x25G NRZ <> 2x53G PAM */
    MODE_100G_NRZ2PAM_XDEC_XENC_RS528_RS544_M0, /*!< retimer mode 30, 4x25G NRZ <> 2x53G PAM */

    MODE_100G_NRZ2PAM100_PCS_XENC_RS544_M0,        /*!< Portofino retimer mode 8c, 4x25G NRZ <> 1x106G PAM100 port0,2*/
    MODE_100G_NRZ2PAM100_KRKP_RS528_RS544_M0,      /*!< Portofino retimer mode 8c, 4x25G NRZ <> 1x106G PAM100 port0,2*/
    MODE_100G_NRZ2PAM100_XDEC_XENC_RS528_RS544_M0, /*!< Portofino retimer mode 8c, 4x25G NRZ <> 1x106G PAM100 port0,2*/

    MODE_100G_NRZ2PAM_PCS_XENC_RS544_M1,        /*!< Portofino retimer mode 11b, 4x25G NRZ <> 2x53G PAM , 01-0, 23-1, 45-2, 67-3*/
    MODE_100G_NRZ2PAM_KRKP_RS528_RS544_M1,      /*!< Portofino retimer mode 11b, 4x25G NRZ <> 2x53G PAM */
    MODE_100G_NRZ2PAM_XDEC_XENC_RS528_RS544_M1, /*!< Portofino retimer mode 11b, 4x25G NRZ <> 2x53G PAM */

    MODE_100G_NRZ2PAM100_PCS_XENC_RS544_M1,        /*!< Portofino retimer mode 8c, 4x25G NRZ <> 1x106G PAM100 port 0,1*/
    MODE_100G_NRZ2PAM100_KRKP_RS528_RS544_M1,      /*!< Portofino retimer mode 8c, 4x25G NRZ <> 1x106G PAM100 port 0,1*/
    MODE_100G_NRZ2PAM100_XDEC_XENC_RS528_RS544_M1, /*!< Portofino retimer mode 8c, 4x25G NRZ <> 1x106G PAM100 port 0,1*/

    MODE_100G_NRZ2PAM_DEC_FWD_RS544_RS544_M0,   /*!< retimer mode ??, 4x26G NRZ <> 2x53G PAM */
    MODE_100G_NRZ2PAM_DEC_ENC_RS544_RS544_M0,   /*!< retimer mode ??, 4x26G NRZ <> 2x53G PAM */
    MODE_100G_NRZ2PAM_XDEC_XENC_RS544_RS544_M0, /*!< retimer mode ??, 4x26G NRZ <> 2x53G PAM */

    MODE_100G_NRZ2PAM_DEC_FWD_RS544_RS544_M1,   /*!< retimer mode ??, 4x26G NRZ <> 2x53G PAM */
    MODE_100G_NRZ2PAM_DEC_ENC_RS544_RS544_M1,   /*!< retimer mode ??, 4x26G NRZ <> 2x53G PAM */
    MODE_100G_NRZ2PAM_XDEC_XENC_RS544_RS544_M1, /*!< retimer mode ??, 4x26G NRZ <> 2x53G PAM */

// Retimer Rev Gbox mode 100G PAM <> NRZ
    MODE_100G_PAM2NRZ_XENC_PCS_RS544_M0,        /*!< retimer mode 29R, 2x53G PAM <> 4x25G NRZ */
    MODE_100G_PAM2NRZ_KPKR_RS544_RS528_M0,      /*!< retimer mode 30R, 2x53G PAM <> 4x25G NRZ */
    MODE_100G_PAM2NRZ_XDEC_XENC_RS544_RS528_M0, /*!< retimer mode 30R, 2x53G PAM <> 4x25G NRZ */
                                                                                              
    MODE_100G_PAM2NRZ_XENC_PCS_RS544_M1,        /*!< retimer mode 29R, 2x53G PAM <> 4x25G NRZ */
    MODE_100G_PAM2NRZ_KPKR_RS544_RS528_M1,      /*!< retimer mode 30R, 2x53G PAM <> 4x25G NRZ */
    MODE_100G_PAM2NRZ_XDEC_XENC_RS544_RS528_M1, /*!< retimer mode 30R, 2x53G PAM <> 4x25G NRZ */

    MODE_100G_PAM2NRZ_DEC_FWD_RS544_RS544_M0,   /*!< retimer mode ??R, 2x53G PAM <> 4x26G NRZ */
    MODE_100G_PAM2NRZ_DEC_ENC_RS544_RS544_M0,   /*!< retimer mode ??R, 2x53G PAM <> 4x26G NRZ */
    MODE_100G_PAM2NRZ_XDEC_XENC_RS544_RS544_M0, /*!< retimer mode ??R, 2x53G PAM <> 4x26G NRZ */

    MODE_100G_PAM2NRZ_DEC_FWD_RS544_RS544_M1,   /*!< retimer mode ??R, 2x53G PAM <> 4x26G NRZ */
    MODE_100G_PAM2NRZ_DEC_ENC_RS544_RS544_M1,   /*!< retimer mode ??R, 2x53G PAM <> 4x26G NRZ */
    MODE_100G_PAM2NRZ_XDEC_XENC_RS544_RS544_M1, /*!< retimer mode ??R, 2x53G PAM <> 4x26G NRZ */

// Retimer Direct mode 200G NRZ <> NRZ
    MODE_200G_NRZ_DEC_FWD_RS544_RS544,          /*!< retimer mode 37, 8x26G NRZ <> 8x26G NRZ */
    MODE_200G_NRZ_DEC_ENC_RS544_RS544,          /*!< retimer mode 37, 8x26G NRZ <> 8x26G NRZ */
    MODE_200G_NRZ_XDEC_XENC_RS544_RS544,        /*!< retimer mode 37, 8x26G NRZ <> 8x26G NRZ */

// Retimer Direct mode 200G PAM <> PAM
    MODE_200G_PAM_DEC_FWD_RS544_RS544,          /*!< retimer mode 35, 4x53G PAM <> 4x53G PAM */
    MODE_200G_PAM_DEC_ENC_RS544_RS544,          /*!< retimer mode 35, 4x53G PAM <> 4x53G PAM */
    MODE_200G_PAM_XDEC_XENC_RS544_RS544,        /*!< retimer mode 35, 4x53G PAM <> 4x53G PAM */

// Retimer Direct mode 200G PAM100 <> PAM100
    MODE_200G_PAM100_DEC_FWD_RS544_RS544,          /*!< Portofino retimer mode 1a, 4x53G PAM100 <> 4x53G PAM100 */
    MODE_200G_PAM100_DEC_ENC_RS544_RS544,          /*!< Portofino retimer mode 1a, 4x53G PAM100 <> 4x53G PAM100 */
    MODE_200G_PAM100_XDEC_XENC_RS544_RS544,        /*!< Portofino retimer mode 1a, 4x53G PAM100 <> 4x53G PAM100 */

// Retimer fgbox mode 200G 2xPAM50 <> PAM100
    MODE_200G_PAM50PAM100_DEC_FWD_RS544_RS544_2C,          /*!< Portofino retimer mode 2c, 4x53G PAM100 <> 2x100G PAM100 */
    MODE_200G_PAM50PAM100_DEC_ENC_RS544_RS544_2C,         /*!< Portofino retimer mode 2c, 4x53G PAM100 <> 2x100G PAM100 */
    MODE_200G_PAM50PAM100_XDEC_XENC_RS544_RS544_2C,        /*!< Portofino retimer mode 2c, 4x53G PAM100 <> 2x100G PAM100 */

// Retimer fgbox mode 200G 2xPAM50 <> PAM100
    MODE_200G_PAM50PAM100_DEC_FWD_RS544_RS544_2D,          /*!< Portofino retimer mode 2d, 4x53G PAM100 <> 2x100G PAM100 */
    MODE_200G_PAM50PAM100_DEC_ENC_RS544_RS544_2D,         /*!< Portofino retimer mode 2d, 4x53G PAM100 <> 2x100G PAM100 */
    MODE_200G_PAM50PAM100_XDEC_XENC_RS544_RS544_2D,        /*!< Portofino retimer mode 2d, 4x53G PAM100 <> 2x100G PAM100 */

// Retimer Fwd Gbox mode 200G NRZ <> PAM
    MODE_200G_NRZ2PAM_DEC_FWD_RS544_RS544,      /*!< retimer mode ??, 8x26G NRZ <> 4x53G PAM */
    MODE_200G_NRZ2PAM_DEC_ENC_RS544_RS544,      /*!< retimer mode ??, 8x26G NRZ <> 4x53G PAM */
    MODE_200G_NRZ2PAM_XDEC_XENC_RS544_RS544,    /*!< retimer mode ??, 8x26G NRZ <> 4x53G PAM */

// Retimer Rev Gbox mode 200G PAM <> NRZ
    MODE_200G_PAM2NRZ_DEC_FWD_RS544_RS544,      /*!< retimer mode ??R, 4x53G PAM <> 8x26G NRZ */
    MODE_200G_PAM2NRZ_DEC_ENC_RS544_RS544,      /*!< retimer mode ??R, 4x53G PAM <> 8x26G NRZ */
    MODE_200G_PAM2NRZ_XDEC_XENC_RS544_RS544,    /*!< retimer mode ??R, 4x53G PAM <> 8x26G NRZ */

// Retimer Direct mode 400G PAM <> PAM
    MODE_400G_PAM_DEC_FWD_RS544_RS544,          /*!< retimer mode 39, 8x53G PAM <> 8x53G PAM */
    MODE_400G_PAM_DEC_ENC_RS544_RS544,          /*!< retimer mode 39, 8x53G PAM <> 8x53G PAM */
    MODE_400G_PAM_XDEC_XENC_RS544_RS544,        /*!< retimer mode 39, 8x53G PAM <> 8x53G PAM */

// Retimer Direct mode 400G PAM100 <> PAM100
    MODE_400G_PAM100_DEC_FWD_RS544_RS544,          /*!< Portofino retimer mode 1a, 8x53G PAM100 <> 8x53G PAM100 */
    MODE_400G_PAM100_DEC_ENC_RS544_RS544,          /*!< Portofino retimer mode 1a, 8x53G PAM100 <> 8x53G PAM100 */
    MODE_400G_PAM100_XDEC_XENC_RS544_RS544,        /*!< Portofino retimer mode 1a, 8x53G PAM100 <> 8x53G PAM100 */

// Retimer fgbox mode 400G 2xPAM50 <> PAM100
    MODE_400G_PAM50PAM100_DEC_FWD_RS544_RS544_2C,          /*!< Portofino retimer mode 2c, 8x53G PAM100 <> 4x100G PAM100 */
    MODE_400G_PAM50PAM100_DEC_ENC_RS544_RS544_2C,          /*!< Portofino retimer mode 2c, 8x53G PAM100 <> 4x100G PAM100 */
    MODE_400G_PAM50PAM100_XDEC_XENC_RS544_RS544_2C,        /*!< Portofino retimer mode 2c, 8x53G PAM100 <> 4x100G PAM100 */

// Retimer fgbox mode 400G 2xPAM50 <> PAM100
    MODE_400G_PAM50PAM100_DEC_FWD_RS544_RS544_2D,          /*!< Portofino retimer mode 2d, 8x53G PAM100 <> 4x100G PAM100 */
    MODE_400G_PAM50PAM100_DEC_ENC_RS544_RS544_2D,          /*!< Portofino retimer mode 2d, 8x53G PAM100 <> 4x100G PAM100 */
    MODE_400G_PAM50PAM100_XDEC_XENC_RS544_RS544_2D,        /*!< Portofino retimer mode 2d, 8x53G PAM100 <> 4x100G PAM100 */

    MODE_RTMR_MAX,

} cw_mode_t; /*!< quad_core modes */

/**
 * The enumeration of bh lane selection
 */
typedef enum {
    SEL0, 	/*!< Select 0 */
    SEL1,   	/*!< Select 1 */
    SEL2	/*!< Select 2 */ 
} bh_mux_sel_t; /*!< BH mux selection type */

/**
 * The struction of cw mode parameters
 */
typedef struct cw_mode_parameter_s {
    cfg_dp_type_t           dp_type;               /**< datapath type, repeater or retimer*/   
    cfg_rpt_type_t          rpt_type;              /**< repeater transfer type */   
    cfg_speed_t             speed;                 /**< logic bundle speed */
    cfg_pam_or_nrz_t        host_pam_or_nrz_type;  /**< host serdes pam or nrz */
    cfg_pam_or_nrz_t        line_pam_or_nrz_type;  /**< line serdes pam or nrz */
    cw_host_fec_type_t      host_fec_type;         /**< host fec type */
    cw_line_fec_type_t      line_fec_type;         /**< line fec type */
    cfg_fec_dec_enc_mode_t  fec_dec_enc_mode;      /**< fec termination type*/
    uint16_t                fec_slice;             /**< fec slice number*/
    uint16_t                pcs_slice;             /**< pcs slice number*/
    uint16_t                host_plane_mask;       /**< host physical lane mask 0x1: 0001: host side ln0 on */
    uint16_t                line_plane_mask;       /**< line physical lane mask 0x1: 0001: line side ln0 on */
    uint16_t                host_llane_mask;       /**< host logical lane mask 0x1: 0001: host side ln0 on */
    uint16_t                line_llane_mask;       /**< line logical lane mask 0x1: 0001: line side ln0 on */
    cfg_fec_prbs_t          host_fec_prbs;         /**< host prbs gen */
    cfg_fec_prbs_t          line_fec_prbs;         /**< line prbs gen */
    cfg_fec_prbs_t          host_fec_prbs_mon;     /**< host prbs mon */
    cfg_fec_prbs_t          line_fec_prbs_mon;     /**< line prbs mon */
    cfg_pcs_tpat_t          host_pcs_tpat;         /**< host pcs test pattern gen */
    cfg_pcs_tpat_t          line_pcs_tpat;         /**< line pcs test pattern gen */
    cfg_pcs_tpat_t          host_pcs_tpat_mon;     /**< host pcs test pattern mon */
    cfg_pcs_tpat_t          line_pcs_tpat_mon;     /**< line pcs test pattern mon */

    cfg_egr_or_igr_t        direction;	           /**< 0:EGR; 1:IGR */	
    enabled_t 		    dis_lat_corr;          /**< repeater latency correction disable */
    enabled_t 		    nrz_port1_en;          /**< repeater enable NRZ > PAM100 to port 1 */
    enabled_t 		    L03to47_en;            /**< Direct line 0~3 to line 4~7 */

    /* TODO: HW not supported, will remove it. for BH mux select*/
    bh_mux_sel_t        egrmx_sel0;            /**< LN lane 0 port selection */
    bh_mux_sel_t        egrmx_sel1;            /**< LN lane 1 port selection */
    bh_mux_sel_t        egrmx_sel2;            /**< LN lane 2 port selection */
    bh_mux_sel_t        egrmx_sel3;            /**< LN lane 3 port selection */
    bh_mux_sel_t        egrmx_sel4;            /**< LN lane 4 port selection */
    bh_mux_sel_t        egrmx_sel5;            /**< LN lane 5 port selection */
    bh_mux_sel_t        egrmx_sel6;            /**< LN lane 6 port selection */
    bh_mux_sel_t        egrmx_sel7;            /**< LN lane 7 port selection */
    bh_mux_sel_t        igrmx_sel0;            /**< BH lane 0 port selection */
    bh_mux_sel_t        igrmx_sel1;            /**< BH lane 1 port selection */
    bh_mux_sel_t        igrmx_sel2;            /**< BH lane 2 port selection */
    bh_mux_sel_t        igrmx_sel3;            /**< BH lane 3 port selection */
    bh_mux_sel_t        igrmx_sel4;            /**< BH lane 4 port selection */
    bh_mux_sel_t        igrmx_sel5;            /**< BH lane 5 port selection */
    bh_mux_sel_t        igrmx_sel6;            /**< BH lane 6 port selection */
    bh_mux_sel_t        igrmx_sel7;            /**< BH lane 7 port selection */
    
    uint8_t             mixed_mode;            /**< mixed mode, refer to chip_mixed_mode_t */
} cw_mode_parameter_t; /**< cw mode parameter*/



/**
 * The struction of cw retimer port enable parameters
 */
typedef struct cw_port_config_s {
// for repeater mode only
    cfg_port_en_t   port_1g_en[8];   /**< 1g port*/
    cfg_port_en_t   port_20g_en[4];  /**< 20g port*/
    cfg_port_en_t   port_40g_en[2];  /**< 40g port*/
// for both retimer and repeater
    cfg_port_en_t   port_25g_en[8];  /**< 25g port0*/
    cfg_port_en_t   port_50g_en[8];  /**< 50g port0*/
    cfg_port_en_t   port_100g_en[4]; /**< 100g port0*/
    cfg_port_en_t   port_200g_en[2]; /**< 200g port0*/
    cfg_port_en_t   port_400g_en;    /**< 400g port*/
    uint8_t         port_id;
} cw_port_config_t; /**< repeater/retimer port enable */



// typedef struct cw_port_4p_config_s {
// // for repeater mode only
//     cfg_port_en_t   port_1g_en[8];   /**< 1g port*/
//     cfg_port_en_t   port_20g_en[4];  /**< 20g port*/
//     cfg_port_en_t   port_40g_en[2];  /**< 40g port*/
// // for both retimer and repeater
//     cfg_port_en_t   port_25g_en[8];  /**< 25g port0*/
//     cfg_port_en_t   port_50g_en[8];  /**< 50g port0*/
//     cfg_port_en_t   port_100g_en[4]; /**< 100g port0*/
//     cfg_port_en_t   port_200g_en[2]; /**< 200g port0*/
//     cfg_port_en_t   port_400g_en;    /**< 400g port*/
// } cw_port_config_t; /**< repeater/retimer port enable */


typedef struct cw_port_info_s {
    uint8_t       port_type;               /**< port mode type*/   
    uint8_t       port_idx;                /**< port index*/   
}cw_port_info_t;

typedef struct cw_fec_rx_status_s {    

    uint16_t host_fifo_flt;
    uint16_t host_pcs_gbox_flt;
    uint16_t fec_sync_algn_unlock;
    uint16_t pcs_sync_flt;
    uint16_t xdec_gbox_clsn_flt;
    uint16_t xdec_err_flt;
    uint16_t xenc_gbox_clsn_flt;
    uint16_t xenc_err_flt;
    uint16_t krkp_gbox_clsn;
    uint16_t line_pfifo_flt;
} cw_fec_rx_status_t;


typedef struct cw_pcs_err_s {
   uint8_t pcs_hi_ber; /**< pcs hi_ber flag */
   uint32_t pcs_sync_header_err_cnt; /**< pcs 22-bit sync header error counter */
} cw_pcs_err_t; /**< retimer pcs sync header error counter */

typedef struct tpat_gen_cfg_s {
    uint8_t tpat_gen_en;
    uint8_t amgap_gen_en;
    uint8_t amins_gen_en;
    uint8_t scr_en;
    uint8_t pcs_tpat_gen_en;  /**< indicates PCS TPAT GEN block is enabled in hardware */
} tpat_gen_cfg_t;

/**
 * The struction of FEC PRBS config list
 */
typedef struct _fec_prbs_cfg_list_s {
    uint8_t val_idx;                     /**< value index   */
    uint8_t lst_beg;                     /**< list beginning point   */
    uint8_t lst_end;                     /**< list end point   */
} _fec_prbs_cfg_list_t;                  /**< _fec_prbs_cfg_list_t type */

#ifdef __cplusplus
}
#endif


#endif  /*CW_DEF_H*/

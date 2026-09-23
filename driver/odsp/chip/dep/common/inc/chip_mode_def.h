/**
 *
 * @file chip_mode_def.h
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
#ifndef CHIP_MODE_DEF_H
#define CHIP_MODE_DEF_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAX_PORT
#define MAX_PORT                    4
#endif
#define MAX_CLIENT_LANES    4                          /**< Max number of lanes on the Client side                         */


typedef enum ref_clk_freq_e {
    CHIP_REF_CLK_156_25_MHZ,                           /*!< Line and System side multiplier is 165:2578125G and 170:26.5625G*/
    CHIP_REF_CLK_166_015625_MHZ,                       /*!< Line multiplier:160 and System side multiplier:168 for 2578125G */ 
    CHIP_REF_CLK_625_MHZ,                              /*!< Line multiplier:41.25 and System side multiplier:42.5 for 4X    */
    CHIP_REF_CLK_MAX                                   /*!< Enum size holder                                               */
} ref_clk_freq_t;                                      /*!< Reference Clock Frequency Types                                 */


/**
 * The enumeration of enabled txfir taps setting
 */
typedef enum txfir_taps_e {
    TXFIR_TAPS_NRZ_LP_3TAP,                            /**< TxFIR taps: NRZ 3 tap                                        */
    TXFIR_TAPS_NRZ_6TAP,                               /**< TxFIR taps: NRZ 6 tap                                        */
    TXFIR_TAPS_PAM4_LP_3TAP,                           /**< TxFIR taps: PAM4 3 tap                                       */
    TXFIR_TAPS_PAM4_6TAP,                              /**< TxFIR taps: PAM4 6 tap                                       */
    TXFIR_TAPS_4TAP,                                   /**< TxFIR taps: 4 tap  (DSP only)                                */
    TXFIR_TAPS_7TAP,                                    /**< TxFIR taps: 7 tap  (DSP only)                                */
    TXFIR_TAPS_LIN10TAP,                                /**< TxFIR taps: linear 10 tap  (DSP only)                    */
    TXFIR_TAPS_NONLIN10TAP,                             /**< TxFIR taps: nonlinear 10 tap  (DSP only)                    */
    TXFIR_TAPS_NONLINWFTAP,                             /**< TxFIR taps: nonlinear WF tap  (DSP only)                    */
    TXFIR_TAPS_MAX
} txfir_taps_t;                                        /**< TxFIR taps type                                              */

 
 #define MIN_TXFIR_NRZ_RANGE             0             /**< Min TxFIR NRZ Range                                          */
 #define MAX_TXFIR_NRZ_RANGE           127             /**< Max TxFIR NRZ Range                                          */
 #define MIN_TXFIR_PAM4_RANGE            0             /**< Min TxFIR PAM4 Range                                         */
 #define MAX_TXFIR_PAM4_RANGE          168             /**< Max TxFIR PAM4 Range                                         */
 

/**
 * The structure of TxFIR info
 */
typedef struct txfir_info_s {
    uint8_t      numb_of_taps;                         /**< Enabled TxFIR taps : refer to txfir_taps_t                   */
    int16_t      tap[13];                               /**< TxFIR tap[pre2, pre1, main, post1, post2, post3, post4]      */
    uint8_t      ftap_loc;                             /**< TxFIR float top location [0,11]                               */
} txfir_info_t;                                        /**< TxFIR Info                                                   */


/*Chip mode enum */
enum cw_chip_mode_e {
    CHIP_MODES_NONE                          = 0,
    CHIP_MODE_1X106G_KP4PAM_TO_1X106G_KP4PAM = 1, //41,42
    CHIP_MODE_1X53G_KP4PAM_TO_1X53G_KP4PAM   = 2,
    CHIP_MODE_1X51G_KR4PAM_TO_1X51G_KR4PAM   = 3,
    CHIP_MODE_1x25G_KR4NRZ_TO_1x25G_KR4NRZ   = 4,//45,46, ?47
    CHIP_MODE_1X26G_KP4NRZ_TO_1X26G_KP4NRZ   = 5,
//    CHIP_MODE_8x25G_KR4NRZ_TO_2x103G_KR4PAM  = 6,
//    CHIP_MODE_8X106G_KP4PAM_TO_8X106G_KP4PAM = 7,
    CHIP_MODE_4X106G_KP4PAM_TO_4X106G_KP4PAM = 6, //21,22
    CHIP_MODE_2X106G_KP4PAM_TO_2X106G_KP4PAM = 7, //31,32
//    CHIP_MODE_8X53G_KP4PAM_TO_8X53G_KP4PAM   = 10,
    CHIP_MODE_4X53G_KP4PAM_TO_4X53G_KP4PAM   = 8, //15,16
    CHIP_MODE_2X53G_KP4PAM_TO_2X53G_KP4PAM   = 9, //25,26
    CHIP_MODE_2X53G_KP4PAM_TO_1X106G_KP4PAM  = 10,//27,28
    CHIP_MODE_4X53G_KP4PAM_TO_2X106G_KP4PAM  = 11, //17,18
    CHIP_MODE_2X25G_KR4NRZ_TO_1X51G_KR4PAM   = 12,
    CHIP_MODE_4X25G_KR4NRZ_TO_1x103G_KR4PAM  = 13,
//    CHIP_MODE_8x25G_KR4NRZ_TO_8x25G_KR4NRZ   = 17,
//    CHIP_MODE_8X26G_KP4NRZ_TO_8X26G_KP4NRZ   = 18,
//    CHIP_MODE_8X53G_KP4PAM_TO_4X106G_KP4PAM  = 19,
//    CHIP_MODE_8X26G_KP4NRZ_TO_4X53G_KP4PAM   = 20,
    CHIP_MODE_4x25G_NRZ_TO_2X53G_KP4PAM   = 14, //9,10
    CHIP_MODE_4X25G_NRZ_TO_1x106G_KP4PAM  = 15, //5,6
//    CHIP_MODE_8x25G_NRZ_TO_4X53G_KP4PAM   = 23,
    CHIP_MODE_2X51G_KR4PAM_TO_2X51G_KR4PAM   = 16,
    CHIP_MODE_4X51G_KR4PAM_TO_4X51G_KR4PAM   = 17,
//    CHIP_MODE_8X51G_KR4PAM_TO_8X51G_KR4PAM   = 26,
    CHIP_MODE_2x25G_NRZ_TO_1X53G_KP4PAM   = 18,
//    CHIP_MODE_8X51G_KR4PAM_TO_4X103G_KR4PAM = 28,
    CHIP_MODE_1X53G_NRZ_TO_1X53G_NRZ        = 19, //48,?49
//    CHIP_MODE_8X26G_KP4NRZ_TO_2X106G_KP4PAM = 30,
    CHIP_MODE_2X26G_KP4NRZ_TO_2X26G_KP4NRZ   = 20,
    CHIP_MODE_4X26G_KP4NRZ_TO_4X26G_KP4NRZ   = 21,
    CHIP_MODE_4X26G_KP4NRZ_TO_2X53G_KP4PAM   = 22,//11, ?12
    CHIP_MODE_2X26G_KP4NRZ_TO_1X53G_KP4PAM   = 23,//39, ?38, ?40
    CHIP_MODE_2X51G_KR4PAM_TO_1X103G_KR4PAM = 24,
    CHIP_MODE_4X51G_KR4PAM_TO_2X103G_KR4PAM = 25,
    CHIP_MODE_2X25G_KR4NRZ_TO_2X25G_KR4NRZ   = 26,//35,36, ?37
    CHIP_MODE_4x25G_KR4NRZ_TO_4x25G_KR4NRZ   = 27,//2,3, ?4
    CHIP_MODE_4X26G_KP4NRZ_TO_1X106G_KP4PAM  = 28, //7, ?8
//    CHIP_MODE_4X106G_KP4PAM_TO_8X53G_KP4PAM  = 40,
    CHIP_MODE_1X106G_KP4PAM_TO_2X53G_KP4PAM  = 29,
//    CHIP_MODE_8X53G_KP4NRZ_TO_8X53G_KP4NRZ   = 42,
//    CHIP_MODE_2X53G_KP4PAM_TO_1X106G_KP4PAM_M1 = 30,
//    CHIP_MODE_4X53G_KP4PAM_TO_2X106G_KP4PAM_M1 = 31,

    CHIP_MODE_MAX_MODE,
};

typedef uint8_t cw_chip_mode_t;  // force the enum to 1 bytes

/*chip port mode related defnition*/
typedef enum chip_bh_baud_rate_e {
    CHIP_BH_BR_53_125 = 0,
    CHIP_BH_BR_51_563,
    CHIP_BH_BR_25_78125,
    CHIP_BH_BR_26_5625,
    CHIP_BH_BR_MAX
} chip_bh_baud_rate_t;

typedef enum chip_modulation_e {
    CHIP_MOD_NRZ,
    CHIP_MOD_PAM4
} chip_modulation_t;

typedef enum chip_bh_osr_e {
    CHIP_BH_OSR_1 = 0,
    CHIP_BH_OSR_MAX = 1
} chip_bh_osr_t;


typedef enum chip_bh_pll_multiplier_e {
    CHIP_BH_PLL_MULTIPLIER_170=0,
    CHIP_BH_PLL_MULTIPLIER_165=1,
    CHIP_BH_PLL_MULTIPLIER_160=2,
    CHIP_BH_PLL_MULTIPLIER_MAX
} chip_bh_pll_multiplier_t;


typedef enum chip_bh_vco_rate_e {
    CHIP_BH_VCO_26P5625=0,
    CHIP_BH_VCO_25P78125,
    CHIP_BH_VCO_MAX
} chip_bh_vco_rate_t ;

typedef struct chip_bh_config_s {
    uint16_t baud_rate  : 4;
    uint16_t modulation : 1;
    uint16_t osr        : 3;
    uint16_t multiplier : 4;
    uint16_t vco_rate   : 4;
    uint16_t lane_mask;
} chip_bh_config;

typedef enum cw_lw_baud_rate_e {
    CW_LW_BR_106_25 = 0 ,
    CW_LW_BR_103_13,
    CW_LW_BR_25_78125,
    CW_LW_BR_111_57,
    CW_LW_BR_53_125,
    CW_LW_BR_51_565,
    CW_LW_BR_26_5625,
    CW_LW_BR_MAX
} cw_lw_baud_rate_t;

typedef enum cw_lw_osr_e {
    CW_LW_OSR_1 = 1,
    CW_LW_OSR_2 = 2,
    CW_LW_OSR_MAX
} cw_lw_osr_t;

typedef enum cw_lw_pll_multiplier_e {
    CW_LW_PLL_MULTIPLIER_170 = 0,
    CW_LW_PLL_MULTIPLIER_165 ,
    CW_LW_PLL_MULTIPLIER_168 ,
    CW_LW_PLL_MULTIPLIER_MAX
} cw_lw_pll_multiplier_t;

typedef enum cw_lw_vco_rate_e {
    CW_LW_VCO_53P125 = 0,
    CW_LW_VCO_51P563,
    CW_LW_VCO_55P78125, 
    CW_LW_VCO_MAX
} cw_lw_vco_rate_t ;

typedef struct cw_lw_config_s {
    uint16_t br:4;
    uint16_t mod:1;
    uint16_t osr:3;
    uint16_t multiplier:4;
    uint16_t vco_rate:4;
    uint16_t lane_mask;
} cw_lw_config;

/**
 * The enumeration of chip function mode
 */
enum cw_function_mode_e {
    CHIP_MODE_NONE = 0,
    CHIP_MODE_400G = 1, /**< 400G mode */
    CHIP_MODE_200G = 2, /**< 200G mode */
    CHIP_MODE_100G = 3, /**< 100G mode */
    CHIP_MODE_50G = 4, /**< 50G mode */
    CHIP_MODE_25G = 5, /**< 25G mode */
    CHIP_MODE_800G = 6, /**< 800G mode */
}; /**< Chip function mode type */
typedef uint8_t cw_function_mode_t;  // force the enum to 1 bytes


enum cw_host_fec_type_e { 
    CHIP_HOST_FEC_TYPE_NA = 0, /**< No FEC Mode */
    CHIP_HOST_FEC_TYPE_RS528 = 1, /**< RS528 KR4*/
    CHIP_HOST_FEC_TYPE_RS544 = 2, /**< RS544 KP4 */
    CHIP_HOST_FEC_TYPE_PCS = 3, /**< PCS */
    CHIP_HOST_FEC_TYPE_CL161 = 4, /**< CL161 */
    CHIP_HOST_FEC_TYPE_MAX

}; /**< FEC Type definition */

typedef uint8_t cw_host_fec_type_t;  // force the enum to 1 bytes

enum cw_line_fec_type_e { 
    CHIP_LINE_FEC_TYPE_NA = 0, /**< No FEC Mode */
    CHIP_LINE_FEC_TYPE_RS528 = 1, /**< RS528 KR4*/
    CHIP_LINE_FEC_TYPE_RS544 = 2, /**< RS544 KP4 */
    CHIP_LINE_FEC_TYPE_PCS = 3, /**< PCS */
    CHIP_LINE_FEC_TYPE_CL161 = 4, /**< CL161 */
    CHIP_LINE_FEC_TYPE_INNER = 5, /**< TODO: delete it since Inner code is not supported */
    CHIP_LINE_FEC_TYPE_MAX

}; /**< FEC Type definition */
typedef uint8_t cw_line_fec_type_t;  // force the enum to 1 bytes

/**
 * The enumeration of lane mux Type
 */
enum cw_port_mux_e {
    CHIP_PORT_MUX_BIT_MUX = 0, /**< bit mux type */
    CHIP_PORT_MUX_SYMBOL_MUX = 1, /**< symbol mux type  */

    CHIP_PORT_MUX_MAX

}; /**< Chip lane mux type*/
typedef uint8_t cw_port_mux_t;  // force the enum to 1 bytes

/**
 * The enumeration of lane FEC term Type
 */
enum cw_port_fec_term_type_e {
    CHIP_PORT_FEC_TERM_BYPASS = 0, /**< NO FEC */
    CHIP_PORT_FEC_DEC_FWD = 1, /**< FEC_DEC_FWD */
    CHIP_PORT_FEC_DEC_ENC = 2, /**< FEC_DEC_ENC */
    CHIP_LANE_PCS_XENC = 3, /**< PCS_XENC: host_fec_type =  CHIP_HOST_FEC_TYPE_PCS*/
    CHIP_LANE_FEC_DEC_XDEC_XENC_ENC = 4, /**< FEC_DEC_XDEC_XENC_ENC */
    CHIP_LANE_XENC_PCS = 5,     /**< CHIP_LANE_XENC_PCS, not used in PF */
    CHIP_LANE_FEC_TERM_MAX
}; /**< Chip lane line inner code type*/

typedef uint8_t cw_port_fec_term_type_t;  // force the enum to 1 bytes

/**
 * The enumeration of mixed mode mode Type
 */
enum chip_mixed_mode_e {
    CHIP_MIXED_MODE_NONE             = 0,         /**< none mixed mode: data rate is same for all lanes                                       */       
    CHIP_MIXED_MODE_PORT_DEPENDENT   = 1,         /**< port dependent mixed mode: config one port won't disprupt traffic on the other port    */
    CHIP_MIXED_MODE_PORT_INDEPENDENT = 2,         /**< port in-dependent mixed mode: config one port won't disprupt traffic on the other port */
};            /**< Mixed Mode type */

typedef uint8_t chip_mixed_mode_t;  // force the enum to 1 bytes

/*chip port mode entry definition*/
typedef struct cw_port_entry_s {
    cw_chip_mode_t          port_type;
    cw_function_mode_t      func_mode;
    cw_port_fec_term_type_t egr_fec_term; 
    cw_port_fec_term_type_t igr_fec_term; 
    cw_host_fec_type_t      host_fec; 
    cw_line_fec_type_t      line_fec; 
    cw_lw_config            bh_cfg; //Abdi: Shall be changed in the final version
    cw_lw_config            lw_cfg;
    cw_port_mux_t           port_mux;
    chip_mixed_mode_t       mixed_mode;    
} cw_port_entry_t;


typedef struct cw_port_entry_ex_s {
    cw_chip_mode_t          port_type;
    cw_function_mode_t      func_mode;
    cw_port_fec_term_type_t egr_fec_term; 
    cw_port_fec_term_type_t igr_fec_term; 
    cw_host_fec_type_t      host_fec; 
    cw_line_fec_type_t      line_fec; 
    cw_lw_config            bh_cfg;
    cw_lw_config            lw_cfg;
    cw_port_mux_t           port_mux;
    chip_mixed_mode_t       mixed_mode;
   
    uint16_t                bh_lane_mask;
    uint16_t                lw_lane_mask;
} cw_port_entry_ex_t;




/**
 * The enumeration of FEC Type
 */
typedef enum capi_fec_type_e {
    CHIP_FEC_TYPE_NONE  = 0,                /**< No FEC Mode                                                     */
    CHIP_FEC_TYPE_RS528 = 1,                /**< RS528 KR4                                                       */
    CHIP_FEC_TYPE_RS544 = 2,                /**< RS544 KP4                                                       */
    CHIP_FEC_TYPE_PCS   = 3,                /**< PCS                                                             */
    CHIP_FEC_TYPE_CL161 = 4,                /**< CL161 FEC                                                       */
    CHIP_FEC_TYPE_INNER = 5,                /**< Inner                                                           */
    CHIP_FEC_TYPE_MAX
} chip_fec_type_t;                          /**< FEC type                                                        */


/**
 * The enumeration of chip FEC termination type
 */
typedef enum chip_term_type_e {
    CHIP_TERM_BYPASS            = 0,        /**< FEC Termination type bypass/no FEC                                     */
    CHIP_TERM_DEC_FWD           = 1,        /**< FEC Termination type FEC_DEC_FWD                                       */
    CHIP_TERM_DEC_ENC           = 2,        /**< FEC Termination type FEC_DEC_ENC                                       */
    CHIP_TERM_PCS_XENC          = 3,        /**< FEC Termination type PCS_XENC: host_fec_type =  CHIP_HOST_FEC_TYPE_PCS */
    CHIP_TERM_DEC_XDEC_XENC_ENC = 4,        /**< FEC Termination type FEC_DEC_XDEC_XENC_ENC                             */
    CHIP_TERM_XENC_PCS          = 5,        /**< FEC Termination type CHIP_LANE_XENC_PCS, not used in PF                */
    CHIP_TERM_MAX 
} chip_term_type_t;                         /**< Chip Termination type                                                  */


/**
 * The structure of chip port fec mode
 */
typedef struct cw_port_fec_mode_s {
    chip_fec_type_t  host_fec;          /**< host FEC type                                                          */
    chip_term_type_t core_term;         /**< FEC termination mode                                                   */
    chip_fec_type_t  media_fec;         /**< media FEC type                                                         */
} cw_port_fec_mode_t;


typedef cw_port_entry_t capi_port_entry_t;

#ifdef __cplusplus
}
#endif
#endif  /*CHIP_MODE_DEF_H*/

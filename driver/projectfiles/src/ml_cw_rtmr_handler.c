/**
 *
 * @file ml_cw_rtmr_handler.c
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
#include <stdio.h>
#include <stdlib.h>
#include "regs_common.h"
#include "common_util.h"
#include "chip_mode_def.h"
#include "cw_def.h"
#include "chip_common_config_ind.h"
#include "cw_comm_def.h"
#include "capi_def.h"
#include "ml_cw_rtmr_handler.h"
#include "ml_cw_rtmr_modes.h"
#include "ml_cw_xbar.h"
#include "chal_cw_rtmr_clkrst_control.h"
#include "chal_cw_rtmr_clockrst_mux.h"
#include "chal_cw_rtmr_datapath_cfg.h"
#include "chal_cw_rtmr_status_check.h"
#include "chal_cw_top.h"
#include "chal_cw_rtmr_xdec.h"
#include "chal_cw_rtmr_xenc.h"
#include "dsp_config.h"
#include "fw_event_log.h"


/**
 * @brief          ml_cw_rtmr_cfg_datapath_init_handler_core(cw_port_profile_t* port_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,
 *                                                      cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr,
 *                                                      cw_chip_mode_t chip_mode)
 * @details    cfg Core Wrapper retimer: Datapath Mux, mode and data gating configuration
 * @public      any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]    port_ptr  port pointer
 * @return void
 */
void ml_cw_rtmr_cfg_datapath_init_handler_core(phy_info_t* phy_info_ptr,
                                            cw_mode_parameter_t* cur_mode_parameter_ptr,
                                            cw_port_config_t* cur_port_config_ptr,
                                            cfg_egr_or_igr_t egr_or_igr,
                                            cw_chip_mode_t chip_mode)
{
    (void)chip_mode;  
    phy_info_t  phy_info={0};

    util_memcpy((void *)&phy_info, phy_info_ptr, sizeof(phy_info_t));
    phy_info.base_addr = phy_info_ptr->base_addr + ml_cw_rtmr_get_base_addr(egr_or_igr, cur_mode_parameter_ptr);
    //1.1.    Static configuration: No need to repeat when restart with same mode, called in Middle-layer INIT_DPATH function:  
    //1.1.1.    Wrap to a middle layer function: (so it can be used in different section of FW): Datapath Mux, mode and data gating configuration:
    //1.1.1.1.    Select retimer datapath  , enable data 
    cha_cw_rtmr_din_off(&phy_info, cur_mode_parameter_ptr, egr_or_igr, IS_ENABLED);
 
    //1.1.1.2.    Set PAM/NRZ mode for host side FIFO Top Repeater
    chal_cw_rtmr_rcv_pfifo_gbox_mode_cfg(&phy_info, cur_mode_parameter_ptr, egr_or_igr);
    chal_cw_rtmr_rcv_pfifo_gbox_doutmux_cfg(&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, egr_or_igr);
    chal_cw_rtmr_rcv_pfifo_gbox_corr_cfg(&phy_info, cur_mode_parameter_ptr,IS_ENABLED);
    chal_cw_rtmr_rcv_pfifo_gbox_ob_cfg(&phy_info, cur_mode_parameter_ptr, egr_or_igr);
    chal_cw_rtmr_rcv_pfifo_gbox_ib_cfg(&phy_info, cur_mode_parameter_ptr, egr_or_igr);
    chal_cw_rtmr_rcv_pfifo_gbox_bundle_cfg(&phy_info, cur_mode_parameter_ptr, egr_or_igr);
    chal_cw_rtmr_rcv_pfifo_gbox_wrmux_cfg(&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, egr_or_igr);

    chal_cw_rtmr_tmt_pfifo_gbox_mode_cfg(&phy_info, cur_mode_parameter_ptr, egr_or_igr);
    chal_cw_rtmr_tmt_pfifo_gbox_dinmux_cfg(&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, egr_or_igr);
    chal_cw_rtmr_tmt_pfifo_gbox_corr_cfg(&phy_info, cur_mode_parameter_ptr,IS_ENABLED);

    chal_cw_rtmr_tmt_pfifo_gbox_ob_cfg(&phy_info, cur_mode_parameter_ptr, egr_or_igr);
    chal_cw_rtmr_tmt_pfifo_gbox_ib_cfg(&phy_info, cur_mode_parameter_ptr, egr_or_igr);
    chal_cw_rtmr_tmt_pfifo_gbox_bundle_cfg(&phy_info, cur_mode_parameter_ptr, egr_or_igr);
    chal_cw_rtmr_tmt_pfifo_gbox_rdmux_cfg(&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, egr_or_igr);

    if (((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) && (egr_or_igr == IGR)) || 
       ((cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) && (egr_or_igr == EGR))) {
        chal_cw_rtmr_tmt_pfifo_gbox_doutmux_cfg(&phy_info, cur_mode_parameter_ptr, 0x1, egr_or_igr);
    } else {
        chal_cw_rtmr_tmt_pfifo_gbox_doutmux_cfg(&phy_info, cur_mode_parameter_ptr, 0x0, egr_or_igr);
    }


    //1.1.1.3.    Select FEC receiver datamux based on datapath mode
   phy_info.base_addr = phy_info_ptr->base_addr + ml_cw_rtmr_get_base_addr(egr_or_igr, cur_mode_parameter_ptr);
   if (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC || cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_FWD) {
        chal_cw_rtmr_fec_rcv_datamux_cfg(&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, RSFEC_DEC_DATA);
   } else if (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KRKP || cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KPKR) {
        chal_cw_rtmr_fec_rcv_datamux_cfg(&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, KPKR_DATA);
   }

    //1.1.1.4.    Select FEC Encoder datamux based on datapath mode
    phy_info.base_addr = phy_info_ptr->base_addr + ml_cw_rtmr_get_base_addr(egr_or_igr, cur_mode_parameter_ptr);
    if ((cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC) ||
       ((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) && (egr_or_igr == EGR)) || 
       ((cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) && (egr_or_igr == IGR))) {
        chal_cw_rtmr_fec_enc_datamux_cfg(&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, XENC_DATA);
    } else {
        chal_cw_rtmr_fec_enc_datamux_cfg(&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, FEC_DEC_DATA);
    }

    //1.1.1.5.    Select FEC xencoder datamux based on datapath mode
    phy_info.base_addr = phy_info_ptr->base_addr + ml_cw_rtmr_get_base_addr(egr_or_igr, cur_mode_parameter_ptr);
    if (((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) && (egr_or_igr == EGR)) || 
       ((cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) && (egr_or_igr == IGR))) {
       chal_cw_rtmr_xenc_datamux_cfg(&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, 0x1);
    } else if (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC ||
        ((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) && (egr_or_igr == IGR)) || 
       ((cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) && (egr_or_igr == EGR))) {
       chal_cw_rtmr_xenc_datamux_cfg(&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, 0x0);
    }

    //1.1.1.6     Configure kr mode
    phy_info.base_addr = phy_info_ptr->base_addr + ml_cw_rtmr_get_base_addr(egr_or_igr, cur_mode_parameter_ptr);
    chal_cw_rtmr_fec_mode_kr4_cfg(&phy_info, cur_mode_parameter_ptr, egr_or_igr);

    //1.1.1.7.    Configure datapath mode
    phy_info.base_addr = phy_info_ptr->base_addr + ml_cw_rtmr_get_base_addr(egr_or_igr, cur_mode_parameter_ptr);
    chal_cw_rtmr_dp_mode_mux_cfg(&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr);

    if (!((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) && (egr_or_igr == EGR)) &&
        !((cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) && (egr_or_igr == IGR))) {
        chal_cw_rtmr_fec_deskew_reorder_cfg(&phy_info, cur_mode_parameter_ptr, egr_or_igr);
    }

    //1.1.1.8.    Configure predec bypass 
    phy_info.base_addr = phy_info_ptr->base_addr + ml_cw_rtmr_get_base_addr(egr_or_igr, cur_mode_parameter_ptr);
    if ((cur_mode_parameter_ptr->speed == SPEED_25G) &&
        !((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) && (egr_or_igr == EGR)) &&
        !((cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) && (egr_or_igr == IGR))) {
        chal_cw_rtmr_predec_bypass_deint_cfg(&phy_info, cur_mode_parameter_ptr, 0x1);
    } else {
        chal_cw_rtmr_predec_bypass_deint_cfg(&phy_info, cur_mode_parameter_ptr, 0x0);
    }

    //1.1.1.9.    Configure kxky gbox speed and fec mode
    phy_info.base_addr = phy_info_ptr->base_addr + ml_cw_rtmr_get_base_addr(egr_or_igr, cur_mode_parameter_ptr);
    if (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KRKP ||
        cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KPKR) {
        chal_cw_rtmr_kxky_gbox_speed_cfg(&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr);
    }

    phy_info.base_addr = phy_info_ptr->base_addr + ml_cw_rtmr_get_base_addr(egr_or_igr, cur_mode_parameter_ptr);
    if (((cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KRKP) && (egr_or_igr == IGR)) ||
        ((cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KPKR) && (egr_or_igr == EGR))) {
        chal_cw_rtmr_kxky_gbox_fec_cfg(&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, 0x0);
    } else if (((cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KRKP) && (egr_or_igr == EGR)) ||
               ((cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KPKR) && (egr_or_igr == IGR))) {
        chal_cw_rtmr_kxky_gbox_fec_cfg(&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, 0x1);
    }

/*TODO: Needs to be fixed for Barchetta2*/
#if 1
    /**/
    //1.1.1.10.    Configure xdec gbox  and descambler
    //GD: Gearbox static configurations for pcs-xenc mode and fec_dec_xdec_xenc_enc
    //GD: Scrambler/Descrambler static configurations
    phy_info.base_addr = phy_info_ptr->base_addr + ml_cw_rtmr_get_base_addr(egr_or_igr, cur_mode_parameter_ptr);
    if ((cur_mode_parameter_ptr->fec_dec_enc_mode ==  FEC_DEC_XDEC_XENC_ENC) ||
        ((cur_mode_parameter_ptr->fec_dec_enc_mode ==  PCS_XENC) && (egr_or_igr == IGR)) ||
        ((cur_mode_parameter_ptr->fec_dec_enc_mode ==  XENC_PCS) && (egr_or_igr == EGR))) {
        chal_cw_rtmr_xdec_width_ctrl (&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr);
        chal_cw_rtmr_xdec_gbox_gap (&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, 0x5);
        chal_cw_rtmr_xdec_dscr_ctrl (&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr);
    }

    //1.1.1.11.    Configure xenc gbox  and scambler
    phy_info.base_addr = phy_info_ptr->base_addr + ml_cw_rtmr_get_base_addr(egr_or_igr, cur_mode_parameter_ptr);
    if ((cur_mode_parameter_ptr->fec_dec_enc_mode ==  FEC_DEC_XDEC_XENC_ENC) ||
        ((cur_mode_parameter_ptr->fec_dec_enc_mode ==  PCS_XENC) && (egr_or_igr == EGR)) ||
        ((cur_mode_parameter_ptr->fec_dec_enc_mode ==  XENC_PCS) && (egr_or_igr == IGR))) {
        chal_cw_rtmr_xenc_width_ctrl (&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr);
        chal_cw_rtmr_xenc_gbox_gap (&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, 0x6);
        chal_cw_rtmr_xenc_scr_ctrl (&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr);
    }
#endif

    //1.1.1.12.    Configure FEC encoder bypass 
    phy_info.base_addr = phy_info_ptr->base_addr + ml_cw_rtmr_get_base_addr(egr_or_igr, cur_mode_parameter_ptr);
    //phy_info.base_addr += ((egr_or_igr==IGR)? RTMR_RSFEC_ENC_TOP_IGR_0:RTMR_RSFEC_ENC_TOP_EGR_0);
    if (cur_mode_parameter_ptr->fec_dec_enc_mode ==  FEC_DEC_FWD) {
        chal_cw_rtmr_fec_enc_mux_cfg(&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, 0x1);
    } else {
        chal_cw_rtmr_fec_enc_mux_cfg(&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, 0x0);
    }

    //JW: Set md_enc_pass_thru_uncorr=0 for FEC_DEC_XDEC_XENC_ENC mode
    phy_info.base_addr = phy_info_ptr->base_addr + ml_cw_rtmr_get_base_addr(egr_or_igr, cur_mode_parameter_ptr);
    //phy_info.base_addr += ((egr_or_igr==IGR)? RTMR_RSFEC_ENC_TOP_IGR_0:RTMR_RSFEC_ENC_TOP_EGR_0);
    if (cur_mode_parameter_ptr->fec_dec_enc_mode ==  FEC_DEC_XDEC_XENC_ENC) {
        chal_cw_rtmr_fec_enc_pass_thru_uncorr_cfg(&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, 0x0);
    } else {
        chal_cw_rtmr_fec_enc_pass_thru_uncorr_cfg(&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, 0x1);
    }

    //1.1.1.13.    Configure FEC decoder/encodering data gating mux 
    phy_info.base_addr = phy_info_ptr->base_addr + ml_cw_rtmr_get_base_addr(egr_or_igr, cur_mode_parameter_ptr);
    //phy_info.base_addr += ((egr_or_igr==IGR)? RTMR_RSFEC_DEC_TOP_IGR_0:RTMR_RSFEC_DEC_TOP_EGR_0);
    if (((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) && (egr_or_igr == EGR)) ||
        ((cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) && (egr_or_igr == IGR))) {
        chal_cw_rtmr_fecdec_data_gating_cfg(&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, 0x1);
    } else {                                                                                                          
        chal_cw_rtmr_fecdec_data_gating_cfg(&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, 0x0);
    }

    phy_info.base_addr = phy_info_ptr->base_addr + ml_cw_rtmr_get_base_addr(egr_or_igr, cur_mode_parameter_ptr);
    //phy_info.base_addr += ((egr_or_igr==IGR)? RTMR_RSFEC_ENC_TOP_IGR_0:RTMR_RSFEC_ENC_TOP_EGR_0);
    if (((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) && (egr_or_igr == IGR)) ||
        ((cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) && (egr_or_igr == EGR)) ||
        (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_FWD)) {
        chal_cw_rtmr_fecenc_data_gating_cfg(&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, 0x1);
    } else {                                                                                                          
        chal_cw_rtmr_fecenc_data_gating_cfg(&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, 0x0);
    }

    if ((cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) ||
        (cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC)) {
        egr_or_igr = (cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) ? IGR : EGR;
        phy_info.base_addr = phy_info_ptr->base_addr + ml_cw_rtmr_get_base_addr(egr_or_igr, cur_mode_parameter_ptr);
        /*
        if(cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS)
            phy_info.base_addr = phy_info_ptr->base_addr + RETIMER_IGR;
        else
            phy_info.base_addr = phy_info_ptr->base_addr + RETIMER_EGR;
        */
        
        chal_cw_rtmr_ovr_am_cfg(&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr);
    }
}
      
/**
 * @brief          ml_cw_rtmr_cfg_datapath_init_handler(cw_port_profile_t* port_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,
 *                                                      cw_port_config_t* cur_port_config_ptr, cfg_egr_or_igr_t egr_or_igr,
 *                                                      cw_chip_mode_t chip_mode)
 * @details    cfg Core Wrapper retimer: Datapath Mux, mode and data gating configuration
 * @public      any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]    port_ptr  port pointer
 * @return void
 */
void ml_cw_rtmr_cfg_datapath_init_handler(phy_info_t* phy_info_ptr,
                                            cw_mode_parameter_t* cur_mode_parameter_ptr,
                                            cw_port_config_t* cur_port_config_ptr,
                                            cfg_egr_or_igr_t egr_or_igr,
                                            cw_chip_mode_t chip_mode)
{  
    phy_info_t  phy_info={0};
    //1.1.    Static configuration: No need to repeat when restart with same mode, called in Middle-layer INIT_DPATH function:  
    //1.1.1.    Wrap to a middle layer function: (so it can be used in different section of FW): Datapath Mux, mode and data gating configuration:
    //1.1.1.1.    Select retimer datapath  TOP repeater
    
    util_memcpy((void *)&phy_info, phy_info_ptr, sizeof(phy_info_t));
    phy_info.base_addr = phy_info_ptr->base_addr + ml_cw_rtmr_get_base_addr(egr_or_igr, cur_mode_parameter_ptr);
    chal_cw_tx_data_select(&phy_info, cur_mode_parameter_ptr, RETIMER_PATH, egr_or_igr, 0);
    
    ml_cw_rtmr_cfg_datapath_init_handler_core(phy_info_ptr, cur_mode_parameter_ptr, cur_port_config_ptr, egr_or_igr, chip_mode);
 }

 
/**
 * @brief          ml_cw_rtmr_cfg_clock_reset_mux_handler(cw_port_profile_t* port_ptr,
 *                                          cw_mode_parameter_t* cur_mode_parameter_ptr,
 *                                          cw_port_config_t* cur_port_config_ptr,
 *                                          cfg_egr_or_igr_t egr_or_igr)
 * @details    Clock and reset mux configuration
 * @public      any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]    port_ptr  port pointer
 * @return void
 */
void ml_cw_rtmr_cfg_clock_reset_mux_handler(phy_info_t* phy_info_ptr,
                                            cw_mode_parameter_t* cur_mode_parameter_ptr,
                                            cw_port_config_t* cur_port_config_ptr,
                                            cfg_egr_or_igr_t egr_or_igr)
{
    phy_info_t phy_info = {0};

    util_memcpy((void *)&phy_info, phy_info_ptr, sizeof(phy_info_t));
    //1.1.2. Wrap to a middle layer function: Clock and reset mux configuration:
    phy_info.base_addr = phy_info_ptr->base_addr + ml_cw_rtmr_get_base_addr(egr_or_igr, cur_mode_parameter_ptr);
    chal_cw_rtmr_tmt_pfifo_clk_mux_cfg (&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, egr_or_igr);
    chal_cw_rtmr_rcv_pfifo_clk_mux_cfg (&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, egr_or_igr);

    //1.1.2.1.   Configure rx clock and reset mux
    phy_info.base_addr = phy_info_ptr->base_addr + ml_cw_rtmr_get_base_addr(egr_or_igr, cur_mode_parameter_ptr);
    chal_cw_rtmr_fec_rcv_clk_rst_mux_cfg (&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, egr_or_igr);
    chal_cw_rtmr_pcs_rcv_clk_rst_mux_cfg (&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr);

    //1.1.2.2.    Configure tx  clock  and reset mux
    phy_info.base_addr = phy_info_ptr->base_addr + ml_cw_rtmr_get_base_addr(egr_or_igr, cur_mode_parameter_ptr);
    chal_cw_rtmr_fec_tmt_clk_rst_mux_cfg(&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, egr_or_igr);
    chal_cw_rtmr_pcs_tmt_cdr_clk_rst_mux_cfg(&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, egr_or_igr);
    chal_cw_rtmr_pcs_tmt_cmu_clk_rst_mux_cfg(&phy_info, cur_mode_parameter_ptr, cur_port_config_ptr);

    //1.1.2.3.    Configure FEC transmit clock mux
    phy_info.base_addr = phy_info_ptr->base_addr + ml_cw_rtmr_get_base_addr(egr_or_igr, cur_mode_parameter_ptr);
    if ((cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC) || 
        (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KRKP) ||
        (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KPKR) ||
        ((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) && (egr_or_igr == EGR)) ||
        ((cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) && (egr_or_igr == IGR))) {
        chal_cw_rtmr_fec_rcv_tmt_clk_mux_cfg(&phy_info, cur_mode_parameter_ptr, 0x1, egr_or_igr);
    } else {
        chal_cw_rtmr_fec_rcv_tmt_clk_mux_cfg(&phy_info, cur_mode_parameter_ptr, 0x0, egr_or_igr);
        chal_cw_rtmr_pcs_rcv_tmt_clk_mux_cfg(&phy_info, cur_mode_parameter_ptr, 0x0, egr_or_igr);
    }
    chal_cw_rtmr_cmu2cdr_lane_map_cfg(&phy_info, cur_mode_parameter_ptr, egr_or_igr);
}


/**
 * @brief        ml_cw_rtmr_cfg_lane_clk_handler(cw_port_profile_t* port_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,
 *                                                cw_port_config_t* cur_port_config_ptr, enabled_t enbl, cfg_egr_or_igr_t egr_or_igr)
 * @details     Control per lane clock
 * @public      any further information for the public domain
 * @private     any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]    port_ptr  port pointer
 * @param[in]    enbl  clock enable: IS_DISABLED or IS_ENABLED
 * @return void
 */
void ml_cw_rtmr_cfg_lane_clk_handler(phy_info_t *phy_info_ptr, 
                                     cw_mode_parameter_t * cur_mode_parameter_ptr,
                                     cw_port_config_t* cur_port_config_ptr,
                                     enabled_t enbl,
                                     cfg_egr_or_igr_t egr_or_igr)
{
    phy_info_t phy_info = {0};
    phy_info_t *cur_phy_info = &phy_info;

    util_memcpy((void *)cur_phy_info, phy_info_ptr, sizeof(phy_info_t));
    phy_info.base_addr = phy_info_ptr->base_addr + ml_cw_rtmr_get_base_addr(egr_or_igr, cur_mode_parameter_ptr);

    //Note: per lane clock disable sequence could be same as enable sequence;
    //1.2.    Waiting for Host side CDR lock and TX PI converge, middle layer FW check and re-check CDR lock/TX PI converge to make sure lock/converge is real.  After that, start clock/reset control for EGR init. Called  Middle layer wait_for_CDR_lock function: 

    //1.2.1.     Wrap to a middle layer function:  Control per lane clock
    if((egr_or_igr==EGR && enbl==IS_ENABLED) || (egr_or_igr==IGR && enbl==IS_DISABLED)) {
        //1.2.1.1.    Enable Host RX PCS per lane clocks when HOST is PCS. Enable FEC lane clock
        if ((egr_or_igr==EGR &&((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) || 
                                (cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) ||
                                (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC))) ||
            (egr_or_igr==IGR && cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC)) { 
            //phy_info.base_addr = phy_info_ptr->base_addr;
            chal_cw_rtmr_pcs_clk_gate_control(cur_phy_info, cur_mode_parameter_ptr, HOST, egr_or_igr, enbl);
        }
        //if (cur_mode_parameter_ptr->fec_dec_enc_mode != PCS_XENC) 
        {
            //phy_info.base_addr = phy_info_ptr->base_addr;
            chal_cw_rtmr_fec_clk_gate_control(cur_phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, HOST, egr_or_igr, enbl);  
        }
        //phy_info.base_addr = phy_info_ptr->base_addr + ml_cw_rtmr_get_base_addr(egr_or_igr, cur_mode_parameter_ptr);
        chal_cw_rtmr_rcv_gapclk_control(cur_phy_info, cur_mode_parameter_ptr, enbl, egr_or_igr);

        //1.2.1.2.    Enable Line TX per lane clocks. Line side is only FEC. 
        if ((egr_or_igr==EGR && (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC ||
                                 cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC)) ||
            (egr_or_igr==IGR && (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC ||
                                 cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC || 
                                 cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS ))) { 
            //phy_info.base_addr = phy_info_ptr->base_addr;
            chal_cw_rtmr_pcs_clk_gate_control(cur_phy_info, cur_mode_parameter_ptr, LINE, egr_or_igr, enbl);
        }
        //if (cur_mode_parameter_ptr->fec_dec_enc_mode != XENC_PCS) 
        { 
            //phy_info.base_addr = phy_info_ptr->base_addr;
            chal_cw_rtmr_fec_clk_gate_control(cur_phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, LINE, egr_or_igr, enbl);
        }
        //phy_info.base_addr = phy_info_ptr->base_addr + ml_cw_rtmr_get_base_addr(egr_or_igr, cur_mode_parameter_ptr);
        chal_cw_rtmr_tmt_gapclk_control(cur_phy_info, cur_mode_parameter_ptr, enbl, egr_or_igr);
    }
    else {
        //1.2.1.2.    Enable Line TX per lane clocks. Line side is only FEC. 
        if ((egr_or_igr==EGR && (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC ||
                                 cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC )) ||
            (egr_or_igr==IGR && (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC ||
                                 cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC || 
                                 cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS ))) { 
            //phy_info.base_addr = phy_info_ptr->base_addr;
            chal_cw_rtmr_pcs_clk_gate_control(cur_phy_info, cur_mode_parameter_ptr, LINE, egr_or_igr, enbl);
        }
        //if (cur_mode_parameter_ptr->fec_dec_enc_mode != XENC_PCS) 
        { 
            //phy_info.base_addr = phy_info_ptr->base_addr;
            chal_cw_rtmr_fec_clk_gate_control(cur_phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, LINE, egr_or_igr, enbl);
        }
        //phy_info.base_addr = phy_info_ptr->base_addr + ml_cw_rtmr_get_base_addr(egr_or_igr, cur_mode_parameter_ptr);
        chal_cw_rtmr_tmt_gapclk_control(cur_phy_info, cur_mode_parameter_ptr, enbl, egr_or_igr);

        //1.2.1.1.    Enable Host RX PCS per lane clocks when HOST is PCS. Enable FEC lane clock
        if ((egr_or_igr==EGR &&((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) || 
                                (cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) ||
                                (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC))) ||
            (egr_or_igr==IGR && cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC)) { 
            //phy_info.base_addr = phy_info_ptr->base_addr;
            chal_cw_rtmr_pcs_clk_gate_control(cur_phy_info, cur_mode_parameter_ptr, HOST, egr_or_igr, enbl);
        }
        //if (cur_mode_parameter_ptr->fec_dec_enc_mode != PCS_XENC) 
        {
            //phy_info.base_addr = phy_info_ptr->base_addr; 
            chal_cw_rtmr_fec_clk_gate_control(cur_phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, HOST, egr_or_igr, enbl);  
        }
        //phy_info.base_addr = phy_info_ptr->base_addr + ml_cw_rtmr_get_base_addr(egr_or_igr, cur_mode_parameter_ptr);
        chal_cw_rtmr_rcv_gapclk_control(cur_phy_info, cur_mode_parameter_ptr, enbl, egr_or_igr);
    }
}


/**
 * @brief      ml_cw_rtmr_cfg_lane_reset_handler(cw_port_profile_t* port_ptr,
 *                                                 cw_mode_parameter_t* cur_mode_parameter_ptr,
 *                                                 rst_assert_t rst, 
 *                                                 cfg_egr_or_igr_t egr_or_igr)
 * @details    config  Core Wrapper Retimer egress per lane reset
 * @public    any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]  port_ptr  port pointer
 * @param[in]  rst   reset asserted: RST_DEASSERTED or RST_ASSERTED
 * @return void
 */ 
void ml_cw_rtmr_cfg_lane_reset_handler(phy_info_t* phy_info_ptr,
                                        cw_mode_parameter_t* cur_mode_parameter_ptr,
                                        cw_port_config_t* cur_port_config_ptr,
                                        rst_assert_t rst, 
                                        cfg_egr_or_igr_t egr_or_igr)
{
    phy_info_t phy_info = {0};
    phy_info_t *cur_phy_info = &phy_info;

    util_memcpy((void *)cur_phy_info, phy_info_ptr, sizeof(phy_info_t));
    phy_info.base_addr = phy_info_ptr->base_addr + ml_cw_rtmr_get_base_addr(egr_or_igr, cur_mode_parameter_ptr);

    //1.2.2.    Wrap to a middle layer function: Control per lane reset
    if((egr_or_igr==EGR && rst==RST_DEASSERTED) || (egr_or_igr==IGR && rst==RST_ASSERTED)) {
        //1.2.2.1.    Release Host  RX PCS/FEC per lane resets. If host is PCS, only need to release host pcs lane reset. If host is FEC, and in fec_dec_xdec_xenc_enc mode, need to release both pcs and fec reset. Otherwise, only need to release fec reset. 
        if ((egr_or_igr==EGR &&((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) || 
                                (cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) ||
                                (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC))) ||
            (egr_or_igr==IGR && cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC)) {
            //phy_info.base_addr = phy_info_ptr->base_addr;
            chal_cw_rtmr_pcs_reset_control(cur_phy_info, cur_mode_parameter_ptr, HOST, egr_or_igr, rst);
        }
        //if (cur_mode_parameter_ptr->fec_dec_enc_mode != PCS_XENC) 
        { 
            //phy_info.base_addr = phy_info_ptr->base_addr;
            chal_cw_rtmr_fec_reset_control(cur_phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, HOST, egr_or_igr, rst);
        } 

        //1.2.2.2.    Release Line TX per lane resets
        if ((egr_or_igr==EGR && cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC) ||
            (egr_or_igr==IGR && (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC ||
                                 cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC || 
                                 cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS ))) { 
            //phy_info.base_addr = phy_info_ptr->base_addr;
            chal_cw_rtmr_pcs_reset_control(cur_phy_info, cur_mode_parameter_ptr, LINE, egr_or_igr,rst );
        }
        //if (cur_mode_parameter_ptr->fec_dec_enc_mode != XENC_PCS) 
        { 
            //phy_info.base_addr = phy_info_ptr->base_addr;
            chal_cw_rtmr_fec_reset_control(cur_phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, LINE, egr_or_igr, rst);
        }

        //1.2.2.3.    Release Line TX per lane resets
        // Release top level resets for fec_pcs_wrapper
        //phy_info.base_addr = phy_info_ptr->base_addr + ml_cw_rtmr_get_base_addr(egr_or_igr, cur_mode_parameter_ptr);
        chal_cw_rtmr_top_reset_cfg(cur_phy_info, cur_mode_parameter_ptr,rst, egr_or_igr);
    }
    else {
        //1.2.2.3.    Release Line TX per lane resets
        // Release top level resets for fec_pcs_wrapper
        //phy_info.base_addr = phy_info_ptr->base_addr + ml_cw_rtmr_get_base_addr(egr_or_igr, cur_mode_parameter_ptr);
        chal_cw_rtmr_top_reset_cfg(cur_phy_info, cur_mode_parameter_ptr, rst,egr_or_igr);

        //1.2.2.2.    Release Line TX per lane resets
        if ((egr_or_igr==EGR && cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC) ||
            (egr_or_igr==IGR && (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC ||
                                 cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC || 
                                 cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS ))) { 
            //phy_info.base_addr = phy_info_ptr->base_addr;
            chal_cw_rtmr_pcs_reset_control(cur_phy_info, cur_mode_parameter_ptr, LINE, egr_or_igr,rst);
        }
        //if (cur_mode_parameter_ptr->fec_dec_enc_mode != XENC_PCS) 
        { 
            //phy_info.base_addr = phy_info_ptr->base_addr;
            chal_cw_rtmr_fec_reset_control(cur_phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, LINE, egr_or_igr, rst);
        }
        //1.2.2.1.    Release Host  RX PCS/FEC per lane resets. If host is PCS, only need to release host pcs lane reset. If host is FEC, and in fec_dec_xdec_xenc_enc mode, need to release both pcs and fec reset. Otherwise, only need to release fec reset. 
        if ((egr_or_igr==EGR &&((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) || 
                                (cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) ||
                                (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC))) ||
            (egr_or_igr==IGR && cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC)) {
            //phy_info.base_addr = phy_info_ptr->base_addr;
            chal_cw_rtmr_pcs_reset_control(cur_phy_info, cur_mode_parameter_ptr, HOST, egr_or_igr, rst);
        }
        //if (cur_mode_parameter_ptr->fec_dec_enc_mode != PCS_XENC) 
        { 
            //phy_info.base_addr = phy_info_ptr->base_addr;
            chal_cw_rtmr_fec_reset_control(cur_phy_info, cur_mode_parameter_ptr, cur_port_config_ptr, HOST, egr_or_igr, rst);
        } 
    }
}


void ml_cw_rtmr_cfg_trnsmt_func_blk_clk_and_rst_handler(phy_info_t *cur_phy_info,
                                                             cw_mode_parameter_t* cur_mode_parameter_ptr,
                                                             cw_port_config_t* port_config_ptr,
                                                             rst_assert_t rst,
                                                             cfg_egr_or_igr_t egr_or_igr)
{
    if(rst == RST_DEASSERTED) {
#if 0
        //1.2.3.7.   pull fec sync or pcs sync status 
        if ((cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC) ||
             (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KRKP) ||
             (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KPKR) ||
             (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC) ||
             (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_FWD) ||
             ((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) && (egr_or_igr == IGR)) ||
             ((cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) && (egr_or_igr == EGR))) {
            // place holder
            // chal_rtm_fec_sync_status(cur_phy_info, cur_mode_parameter_ptr, cur_port_config_ptr); 
        }
        if (((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) && (egr_or_igr == EGR)) ||
             ((cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) && (egr_or_igr == IGR))) {
             // place holder
             //chal_cw_rtmr_pcs_sync_status(cur_phy_info, cur_mode_parameter_ptr,cur_port_config_ptr );
        }
#endif
        //1.2.3.8.    Enable FEC  kr2kp gbox clock  and release reset depends on fec mode.  If fec_dec_enc_krkp mode , turn on clock
        if (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KRKP || 
              cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KPKR) {
              //lilyTBD Note: need give correct base address based on egr_or_igr
              chal_cw_rtmr_krkp_gbox_clk_gate_control(cur_phy_info, cur_mode_parameter_ptr, IS_ENABLED);
              chal_cw_rtmr_krkp_gbox_reset_control(cur_phy_info, cur_mode_parameter_ptr, RST_DEASSERTED, egr_or_igr);
        }

        //1.2.3.9.    Deassert FEC  xdecoder reset depends on host fec mode.  If fec_dec_xdec_xenc_enc mode , deassert  xdec reset
        if ((cur_mode_parameter_ptr->fec_dec_enc_mode ==  FEC_DEC_XDEC_XENC_ENC) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode ==  PCS_XENC) && (egr_or_igr == IGR)) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode ==  XENC_PCS) && (egr_or_igr == EGR))) {
            //lilyTBD Note base_address
            chal_cw_rtmr_xdec_clk_gate_control(cur_phy_info, cur_mode_parameter_ptr, port_config_ptr, IS_ENABLED);
            chal_cw_rtmr_xdec_reset_control(cur_phy_info, cur_mode_parameter_ptr, RST_DEASSERTED, egr_or_igr);
        }

        //1.2.3.10.    Turn on xenc clock and release reset if host is PCS, or in fec_dec_xdec_xenc_enc mode
        if ((cur_mode_parameter_ptr->fec_dec_enc_mode ==  FEC_DEC_XDEC_XENC_ENC) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode ==  PCS_XENC) && (egr_or_igr == EGR)) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode ==  XENC_PCS) && (egr_or_igr == IGR))) {
            //lilyTBD Note base_address
            chal_cw_rtmr_xenc_clk_gate_control(cur_phy_info, cur_mode_parameter_ptr, port_config_ptr, IS_ENABLED);        
            chal_cw_rtmr_xenc_reset_control(cur_phy_info, cur_mode_parameter_ptr, RST_DEASSERTED, egr_or_igr); 
            //lilyTBD Note base_address       
            chal_cw_rtmr_xenc_am_ins_clk_gate_control(cur_phy_info, cur_mode_parameter_ptr, port_config_ptr, IS_ENABLED);        
        }

        //1.2.3.11.    Turn on fec encoder if it is not repeater bitmux_symbol_mux mode and not FEC_DEC_FWD mode
        if ((cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KRKP) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KPKR) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) && (egr_or_igr == EGR)) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) && (egr_or_igr == IGR))) {
            chal_cw_rtmr_fec_enc_clk_gate_control(cur_phy_info, cur_mode_parameter_ptr, IS_ENABLED, egr_or_igr);
        }

        //1.2.3.12.    Enable fec symbol_distribution clock  or pcs transmit clock depends on mode
        if ((cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KRKP) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KPKR) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_FWD) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) && (egr_or_igr == EGR)) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) && (egr_or_igr == IGR))) {
            chal_cw_rtmr_fec_symb_dist_clk_gate_control(cur_phy_info, cur_mode_parameter_ptr, IS_ENABLED, egr_or_igr);
        }

        if (((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) && (egr_or_igr == IGR)) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) && (egr_or_igr == EGR))) {
            chal_cw_rtmr_pcs_tmt_clk_gate_control(cur_phy_info, cur_mode_parameter_ptr, IS_ENABLED, egr_or_igr);
        }

        //1.2.3.13     Deassert tmt pfifo reset followed by enable tmt fifo clock
        if ((cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KRKP) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KPKR) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_FWD) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) && (egr_or_igr == EGR)) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) && (egr_or_igr == IGR))) {
            chal_cw_rtmr_tmt_pfifo_reset_control(cur_phy_info, cur_mode_parameter_ptr, RST_DEASSERTED, egr_or_igr);
            chal_cw_rtmr_tmt_pfifo_clk_gate_control(cur_phy_info, cur_mode_parameter_ptr, IS_ENABLED, egr_or_igr);
        }

        //1.2.3.14.    Deassert fec encoder and symbol distribution reset 
        if ((cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KRKP) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KPKR) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) && (egr_or_igr == EGR)) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) && (egr_or_igr == IGR))) {
            chal_cw_rtmr_fec_enc_reset_control(cur_phy_info, cur_mode_parameter_ptr, RST_DEASSERTED, egr_or_igr);
        }
        if ((cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KRKP) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KPKR) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_FWD) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) && (egr_or_igr == EGR)) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) && (egr_or_igr == IGR))) {
            chal_cw_rtmr_fec_symb_dist_reset_control(cur_phy_info, cur_mode_parameter_ptr, RST_DEASSERTED, egr_or_igr);
        }

        if (((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) && (egr_or_igr == IGR)) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) && (egr_or_igr == EGR))) {
            chal_cw_rtmr_pcs_tmt_reset_control(cur_phy_info, cur_mode_parameter_ptr, RST_DEASSERTED, egr_or_igr);
        }

        // 1.2.3.15.    Dassert tmt gbox reset
        if (((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) && (egr_or_igr == IGR)) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) && (egr_or_igr == EGR))) {
            chal_cw_rtmr_tmt_gbox_reset_control(cur_phy_info, cur_mode_parameter_ptr, RST_DEASSERTED, egr_or_igr);
        }
#if 0
        //Workaround    Deassert FEC  xdecoder reset depends on host fec mode.  If fec_dec_xdec_xenc_enc mode , deassert  xdec reset
        if (((cur_mode_parameter_ptr->fec_dec_enc_mode ==  PCS_XENC) && (egr_or_igr == IGR)) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode ==  XENC_PCS) && (egr_or_igr == EGR))) {
            //hsip TBD Note base_address
            chal_cw_rtmr_xdec_reset_control(cur_phy_info, cur_mode_parameter_ptr, RST_ASSERTED, egr_or_igr);
            HATS_DelayUsec(10);
            chal_cw_rtmr_xdec_reset_control(cur_phy_info, cur_mode_parameter_ptr, RST_DEASSERTED, egr_or_igr);
        }
#endif
    } else {
        // 1.2.3.15.    Dassert tmt gbox reset
        if (((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) && (egr_or_igr == IGR)) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) && (egr_or_igr == EGR))) {
            chal_cw_rtmr_tmt_gbox_reset_control(cur_phy_info, cur_mode_parameter_ptr, RST_ASSERTED, egr_or_igr);
        }
        //1.2.3.14.    Deassert fec encoder and symbol distribution reset 
        if ((cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KRKP) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KPKR) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) && (egr_or_igr == EGR)) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) && (egr_or_igr == IGR))) {
            chal_cw_rtmr_fec_enc_reset_control(cur_phy_info, cur_mode_parameter_ptr, RST_ASSERTED, egr_or_igr);
        }
        if ((cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KRKP) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KPKR) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_FWD) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) && (egr_or_igr == EGR)) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) && (egr_or_igr == IGR))) {
            chal_cw_rtmr_fec_symb_dist_reset_control(cur_phy_info, cur_mode_parameter_ptr, RST_ASSERTED, egr_or_igr);
        }

        if (((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) && (egr_or_igr == IGR)) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) && (egr_or_igr == EGR))) {
            chal_cw_rtmr_pcs_tmt_reset_control(cur_phy_info, cur_mode_parameter_ptr, RST_ASSERTED, egr_or_igr);
        }

        //1.2.3.13     Deassert tmt pfifo reset followed by enable tmt fifo clock
        if ((cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KRKP) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KPKR) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_FWD) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) && (egr_or_igr == EGR)) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) && (egr_or_igr == IGR))) {
            chal_cw_rtmr_tmt_pfifo_reset_control(cur_phy_info, cur_mode_parameter_ptr, RST_ASSERTED, egr_or_igr);
            chal_cw_rtmr_tmt_pfifo_clk_gate_control(cur_phy_info, cur_mode_parameter_ptr, IS_DISABLED, egr_or_igr);
        }
        //1.2.3.12.    Enable fec symbol_distribution clock  or pcs transmit clock depends on mode
        if ((cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KRKP) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KPKR) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_FWD) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) && (egr_or_igr == EGR)) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) && (egr_or_igr == IGR))) {
            chal_cw_rtmr_fec_symb_dist_clk_gate_control(cur_phy_info, cur_mode_parameter_ptr, IS_DISABLED, egr_or_igr);
        }

        if (((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) && (egr_or_igr == IGR)) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) && (egr_or_igr == EGR))) {
            chal_cw_rtmr_pcs_tmt_clk_gate_control(cur_phy_info, cur_mode_parameter_ptr, IS_DISABLED, egr_or_igr);
        }

        //1.2.3.11.    Turn on fec encoder if it is not repeater bitmux_symbol_mux mode and not FEC_DEC_FWD mode
        if ((cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KRKP) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KPKR) ||
            (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) && (egr_or_igr == EGR)) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) && (egr_or_igr == IGR))) {
            chal_cw_rtmr_fec_enc_clk_gate_control(cur_phy_info, cur_mode_parameter_ptr, IS_DISABLED, egr_or_igr);
        }
        //1.2.3.10.    Turn on xenc clock and release reset if host is PCS, or in fec_dec_xdec_xenc_enc mode
        if ((cur_mode_parameter_ptr->fec_dec_enc_mode ==  FEC_DEC_XDEC_XENC_ENC) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode ==  PCS_XENC) && (egr_or_igr == EGR)) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode ==  XENC_PCS) && (egr_or_igr == IGR))) {
            //hsip TBD Note base_address       
            chal_cw_rtmr_xenc_reset_control(cur_phy_info, cur_mode_parameter_ptr, RST_ASSERTED, egr_or_igr);   
            chal_cw_rtmr_xenc_clk_gate_control(cur_phy_info, cur_mode_parameter_ptr, port_config_ptr, IS_DISABLED); 
            //hsip TBD Note base_address     
            chal_cw_rtmr_xenc_am_ins_clk_gate_control(cur_phy_info, cur_mode_parameter_ptr, port_config_ptr, IS_DISABLED);        
        }
        //1.2.3.9.    Deassert FEC  xdecoder reset depends on host fec mode.  If fec_dec_xdec_xenc_enc mode , deassert  xdec reset
        if ((cur_mode_parameter_ptr->fec_dec_enc_mode ==  FEC_DEC_XDEC_XENC_ENC) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode ==  PCS_XENC) && (egr_or_igr == IGR)) ||
            ((cur_mode_parameter_ptr->fec_dec_enc_mode ==  XENC_PCS) && (egr_or_igr == EGR))) {
            //hsip TBD Note base_address
            chal_cw_rtmr_xdec_reset_control(cur_phy_info, cur_mode_parameter_ptr, RST_ASSERTED, egr_or_igr);
            chal_cw_rtmr_xdec_clk_gate_control(cur_phy_info, cur_mode_parameter_ptr, port_config_ptr, IS_DISABLED);
        }

        //1.2.3.8.    Enable FEC  kr2kp gbox clock  and release reset depends on fec mode.  If fec_dec_enc_krkp mode , turn on clock
        if (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KRKP || 
              cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KPKR) {
            //hsip TBD Note base_address
            chal_cw_rtmr_krkp_gbox_clk_gate_control(cur_phy_info, cur_mode_parameter_ptr, IS_DISABLED);
            chal_cw_rtmr_krkp_gbox_reset_control(cur_phy_info, cur_mode_parameter_ptr, RST_ASSERTED, egr_or_igr);
        }
        //1.2.3.7.   pull fec sync or pcs sync status 
        if ((cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_XDEC_XENC_ENC) ||
             (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KRKP) ||
             (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC_KPKR) ||
             (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_ENC) ||
             (cur_mode_parameter_ptr->fec_dec_enc_mode == FEC_DEC_FWD) ||
             ((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) && (egr_or_igr == IGR)) ||
             ((cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) && (egr_or_igr == EGR))) {
            // place holder
            // chal_rtm_fec_sync_status(cur_phy_info, cur_mode_parameter_ptr, cur_port_config_ptr); 
        }
        if (((cur_mode_parameter_ptr->fec_dec_enc_mode == PCS_XENC) && (egr_or_igr == EGR)) ||
             ((cur_mode_parameter_ptr->fec_dec_enc_mode == XENC_PCS) && (egr_or_igr == IGR))) {
             // place holder
             //chal_cw_rtmr_pcs_sync_status(cur_phy_info, cur_mode_parameter_ptr,cur_port_config_ptr );
        }
    }
}


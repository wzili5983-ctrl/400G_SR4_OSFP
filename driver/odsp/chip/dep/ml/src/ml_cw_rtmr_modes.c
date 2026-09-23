/**
 *
 * @file ml_cw_rtmr_modes.c
 * @author  
 * @date     11/18/2020
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
#include "regs_common.h"
#include "common_util.h"
#include "chip_mode_def.h"
#include "capi_def.h"
#include "cw_def.h"
#include "ml_cw_xbar.h"
#include "ml_cw_rtmr_modes.h"
#include "hw_chip_common_def_dep.h"

typedef struct cw_phy_log_lane_map_s {
uint8_t host;
uint8_t line;
} cw_phy_log_lane_map_t;

#define MAX_RPT_TYPE 3
#define MAX_MOD_MAP  3
#define CHIP_MAX_LANES    8
#define FGB_P50_P100  0
#define FGB_NRZ_P50   1
#define FGB_NRZ_P100  2

static cw_phy_log_lane_map_t rtmr_map[MAX_RPT_TYPE][MAX_MOD_MAP][CHIP_MAX_LANES] =
{// plane 0           1         2          3             4            5               6            7
    { // DIRECT
    {{0x1, 0x1}, {0x2, 0x2}, {0x4, 0x4}, {0x8, 0x8}, {0x10, 0x10}, {0x20, 0x20}, {0x40, 0x40}, {0x80, 0x80}}, // NRZ
    {{0x1, 0x1}, {0x2, 0x2}, {0x4, 0x4}, {0x8, 0x8}, {0x10, 0x10}, {0x20, 0x20}, {0x40, 0x40}, {0x80, 0x80}}, // PAM50
    {{0x3, 0x3}, {0xC, 0xC}, {0x30, 0x30}, {0xC0, 0xC0}, {0x3, 0x3}, {0xC, 0xC}, {0x30, 0x30}, {0xC0, 0xC0}}, // PAM100
    },
    { // FGB
    {{0x1, 0x3}, {0x2, 0xC}, {0x4, 0x30}, {0x8, 0xC0}, {0x10, 0x3}, {0x20, 0xC}, {0x40, 0x30}, {0x80, 0xC0}}, // P50>100
    {{0x1, 0x3}, {0x2, 0xC}, {0x4, 0x30}, {0x8, 0xC0}, {0x10, 0x3}, {0x20, 0xC}, {0x40, 0x30}, {0x80, 0xC0}}, // NRZ>P50
    {{0x1, 0x3}, {0x2, 0xC}, {0x4, 0x30}, {0x8, 0xC0}, {0x10, 0x3}, {0x20, 0xC}, {0x40, 0x30}, {0x80, 0xC0}}, // NRZ>P100
    },
    { // RGB
    {{0x3, 0x1}, {0xC, 0x2}, {0x30, 0x4}, {0xC0, 0x8}, {0x3, 0x10}, {0xC, 0x20}, {0x30, 0x40}, {0xC0, 0x80}}, // P50>100
    {{0x3, 0x1}, {0xC, 0x2}, {0x30, 0x4}, {0xC0, 0x8}, {0x3, 0x10}, {0xC, 0x20}, {0x30, 0x40}, {0xC0, 0x80}}, // NRZ>P50
    {{0x3, 0x1}, {0xC, 0x2}, {0x30, 0x4}, {0xC0, 0x8}, {0x3, 0x10}, {0xC, 0x20}, {0x30, 0x40}, {0xC0, 0x80}}, // NRZ>P100
    }
};

/* Function to get no of set bits in binary
   representation of passed binary no. using Brian Kernighan
   algorithm */
unsigned int _intf_count_set_bits(uint32_t n)
{
    unsigned int count = 0;
    while (n)
    {
      n &= (n-1) ;
      count++;
    }
    return count;
}

/**
 * @brief          ml_cw_rtmr_is_A1_4p_port(cw_mode_parameter_t* cw_mode_ptr)
 * @details    Is the port is lane 4-7 configured as port independent mode for A1 chip
 *      FWPLPHSIP-3998: move the 4x50G port from 8P (quad 0) to 4P (quad 1) to supported 4x50G/4x100G mixed mode 
        for repeater mode FEC monitor for A1 chip only
        refer to Portofino CW mode & register doc mixed_mode_2 sheet https://broadcom.ent.box.com/file/747177498971 
        50G port should use P0~P3 in 8P (same as before), P0,P2,P4,P6 in 4P (only for mixed_mode=2)
 *
 * @public        any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]     cw_mode_ptr
 * @return TRUE: port is lane 4-7 configured as port independent mode for A1 chip, FALSE: otherwise 
 */

bool ml_cw_rtmr_is_A1_4p_port(cw_mode_parameter_t* cw_mode_ptr)
{
    /* If this port is in the lanes 4 - 7 and uses DP wrapper 4P for A1 chip in mixed mode */
    if ((fw_read_rev_id() == HERCULES_REV_ID_A1) &&
        (cw_mode_ptr->host_plane_mask & 0xF0) &&
        (cw_mode_ptr->mixed_mode == CAPI_MIXED_MODE_PORT_INDEPENDENT)){
       return TRUE; 
    } else {
       return FALSE;
    }
}


/**
 * @brief          ml_cw_rtmr_get_base_addr(cfg_egr_or_igr_t egr_or_igr, cw_mode_parameter_t* cw_mode_ptr)
 * @details    Get CW retimer base addr
 * @public        any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]     egr_or_igr  
 * @param[in]     cw_mode_ptr
 * @return CW retimer base addr
 */

uint32_t ml_cw_rtmr_get_base_addr(cfg_egr_or_igr_t egr_or_igr, cw_mode_parameter_t* cw_mode_ptr)
{
    if((cw_mode_ptr->host_plane_mask & 0xF0) && (cw_mode_ptr->host_pam_or_nrz_type == CW_PAM100) && (cw_mode_ptr->line_pam_or_nrz_type == CW_PAM100)){
        /* If this port is in the lanes 4 - 7 for PAM100 and uses DP wrapper 4P */
       return ((egr_or_igr == EGR) ? RETIMER_EGR1 : RETIMER_IGR1);   
    } else if (ml_cw_rtmr_is_A1_4p_port(cw_mode_ptr)) {
       /* If this port is in mixed mode lanes 4 - 7 for A1 chip, uses DP wrapper 4P to support repeatedr mode fec mon */
       return ((egr_or_igr == EGR) ? RETIMER_EGR1 : RETIMER_IGR1);   
    } else {
       return ((egr_or_igr == EGR) ? RETIMER_EGR0 : RETIMER_IGR0);
    }
}

    
/**
 * @brief          ml_cw_rtmr_gen_cw_mode_param(cconst cw_port_entry_ex_t *port_entry_ptr, uint8_t port_index, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t * cur_port_config_ptr, cw_mode_parameter_t *cw_mode_ptr)
 * @details    Generate the CW mode param information
 * @public        any further information for the public domain
 * @private    any confidential information (i.e. limitation, precondition, etc. ) for internal domain (Optional)
 *
 *
 * @param[in]     port_entry_ptr
 * @param[in]     port_index
 * @param[in]     egr_or_igr
 * @param[out]     cur_port_config_ptr
 * @param[out]     cw_mode_ptr
 * @return void
 */

void ml_cw_rtmr_gen_cw_mode_param(const cw_port_entry_ex_t *port_entry_ptr, uint8_t port_index, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t * cur_port_config_ptr, cw_mode_parameter_t *cw_mode_ptr)
{
     int i; 
     uint8_t fgb_type = 0;
     cw_port_fec_term_type_t fec_term = (egr_or_igr == EGR) ? port_entry_ptr->egr_fec_term : port_entry_ptr->igr_fec_term;
        
     util_memset((void *)cw_mode_ptr, 0, sizeof(cw_mode_parameter_t));
     /* At this point the logical lane is same as physical lane */
     cw_mode_ptr->line_plane_mask =  ml_cw_get_xbar_logical_lane_mask(port_entry_ptr->lw_cfg.lane_mask, PHY_MEDIA_SIDE);
     cw_mode_ptr->line_llane_mask =  0;
     cw_mode_ptr->host_plane_mask =  ml_cw_get_xbar_logical_lane_mask(port_entry_ptr->bh_cfg.lane_mask, PHY_HOST_SIDE);
     cw_mode_ptr->host_llane_mask =  0;
     // TBD For now same as plane to apply convert_logic_ln later 
     
     if(port_entry_ptr->lw_cfg.mod == CHIP_MOD_NRZ){
        /* NRZ and PAM 50G uses 80 bits FIFO , NRZ 25G uses 40 bits FIFO */
        if(port_entry_ptr->lw_cfg.br == CW_LW_BR_53_125 || port_entry_ptr->lw_cfg.br == CW_LW_BR_51_565){
          cw_mode_ptr->line_pam_or_nrz_type = CW_PAM;
        }else {
          cw_mode_ptr->line_pam_or_nrz_type = CW_NRZ;
        }
     }else if(port_entry_ptr->lw_cfg.mod == CHIP_MOD_PAM4){
        if(port_entry_ptr->lw_cfg.br == CW_LW_BR_106_25 || port_entry_ptr->lw_cfg.br == CW_LW_BR_103_13){
          cw_mode_ptr->line_pam_or_nrz_type = CW_PAM100;
        }else {
          cw_mode_ptr->line_pam_or_nrz_type = CW_PAM;
        }
     }
     if(port_entry_ptr->bh_cfg.mod == CHIP_MOD_NRZ){
        /* NRZ and PAM 50G uses 80 bits FIFO , NRZ 25G uses 40 bits FIFO */
        if(port_entry_ptr->bh_cfg.br == CW_LW_BR_53_125 || port_entry_ptr->bh_cfg.br == CW_LW_BR_51_565){
          cw_mode_ptr->host_pam_or_nrz_type = CW_PAM;
        }else {
          cw_mode_ptr->host_pam_or_nrz_type = CW_NRZ;
        }
     }else if(port_entry_ptr->bh_cfg.mod == CHIP_MOD_PAM4){
        if(port_entry_ptr->bh_cfg.br == CW_LW_BR_106_25 || port_entry_ptr->bh_cfg.br == CW_LW_BR_103_13){
          cw_mode_ptr->host_pam_or_nrz_type= CW_PAM100;
        }else {
          cw_mode_ptr->host_pam_or_nrz_type = CW_PAM;
        }
     }

     cw_mode_ptr->mixed_mode =  port_entry_ptr->mixed_mode;
     cw_mode_ptr->speed = (cfg_speed_t)port_entry_ptr->func_mode;
     cw_mode_ptr->dp_type = RETIMER_PATH;
     cw_mode_ptr->host_fec_type =  port_entry_ptr->host_fec;
     cw_mode_ptr->line_fec_type =  port_entry_ptr->line_fec;

    /*
     fec traffic:  if both sides are PAM100
                         fec 100g:  port_index%4
                         fec 200g:  port_index%4 /2
                         fec 400g: port_index %4 /4
                       else 
                         fec 50g : port_index
                         fec 100g: port_index/2
                         fec 200g: port_index/4
                         fec 400g: port_index (only one port)
     port index = host for fgb modes and  = line for rgb modes
        */
        
     util_memset(cur_port_config_ptr, 0, sizeof(cw_port_config_t));
     cur_port_config_ptr->port_id = port_index;

     if(cw_mode_ptr->speed == SPEED_25G)
        cur_port_config_ptr->port_25g_en[port_index] = PORT_ON;
     else if(cw_mode_ptr->speed == SPEED_50G){
         /* for 50G mixed mode (1*50G lane 4-7), lane 4-7 is swapped to lane 0-3 to use port 0-3 of the 1st quad */
         if (ml_cw_get_quad_swap_enabled())
             port_index = (port_index % 4);
         else if (ml_cw_rtmr_is_A1_4p_port(cw_mode_ptr)) {
             /* 50G ports mixed mode at 4p, port_index: 4,5,6,7, port_50g_en: 0,2,4,6 */
             port_index = (port_index % 4) * 2;
         }         
         cur_port_config_ptr->port_50g_en[port_index] = PORT_ON;
     }else if(cw_mode_ptr->speed == SPEED_100G) {
         if (cw_mode_ptr->line_pam_or_nrz_type == CW_PAM100 &&
            cw_mode_ptr->host_pam_or_nrz_type == CW_PAM100)
            cur_port_config_ptr->port_100g_en[(port_index % 4)] = PORT_ON;
         else if (ml_cw_rtmr_is_A1_4p_port(cw_mode_ptr)) {
             /* 100G ports mixed mode at 4p, port_index: 4,6, port_100g_en: 0,2 */
             cur_port_config_ptr->port_100g_en[(port_index % 4)] = PORT_ON;
         } else
            cur_port_config_ptr->port_100g_en[(port_index / 2)] = PORT_ON;
     }
     else if(cw_mode_ptr->speed == SPEED_200G) {
         if (cw_mode_ptr->line_pam_or_nrz_type == CW_PAM100 &&
            cw_mode_ptr->host_pam_or_nrz_type == CW_PAM100)
            cur_port_config_ptr->port_200g_en[(port_index % 4)/2] = PORT_ON;
         else {
             /* for 200G mixed mode (4*50G lane 4-7), lane 4-7 is swapped to lane 0-3 to use port 0 of the 1st quad */
            if (ml_cw_get_quad_swap_enabled())
                cur_port_config_ptr->port_200g_en[0] = PORT_ON;
            else if (ml_cw_rtmr_is_A1_4p_port(cw_mode_ptr))
                cur_port_config_ptr->port_200g_en[0] = PORT_ON;
            else
                cur_port_config_ptr->port_200g_en[port_index/4] = PORT_ON;
         }
     }
     else if(cw_mode_ptr->speed == SPEED_400G) {
         cur_port_config_ptr->port_400g_en = PORT_ON;
     }
     
     //Derive the repeater type here
     if(cw_mode_ptr->line_plane_mask==cw_mode_ptr->host_plane_mask){
        cw_mode_ptr->rpt_type = DIRECT;
     } else if(_intf_count_set_bits(cw_mode_ptr->line_plane_mask)<_intf_count_set_bits(cw_mode_ptr->host_plane_mask)){
        cw_mode_ptr->rpt_type = GEARBOX;
        if(cw_mode_ptr->host_pam_or_nrz_type == CW_PAM && cw_mode_ptr->line_pam_or_nrz_type == CW_PAM100){
          fgb_type = FGB_P50_P100;
        } else if(cw_mode_ptr->host_pam_or_nrz_type == CW_NRZ && cw_mode_ptr->line_pam_or_nrz_type == CW_PAM){
          fgb_type = FGB_NRZ_P50;
        } else if(cw_mode_ptr->host_pam_or_nrz_type == CW_NRZ && cw_mode_ptr->line_pam_or_nrz_type == CW_PAM100){ 
          fgb_type = FGB_NRZ_P100;
        }

     } else if(_intf_count_set_bits(cw_mode_ptr->line_plane_mask)>_intf_count_set_bits(cw_mode_ptr->host_plane_mask)){
        cw_mode_ptr->rpt_type = R_GEARBOX;
        if(cw_mode_ptr->host_pam_or_nrz_type == CW_PAM100 && cw_mode_ptr->line_pam_or_nrz_type == CW_PAM){
          fgb_type = FGB_P50_P100;
        } else if(cw_mode_ptr->host_pam_or_nrz_type == CW_PAM && cw_mode_ptr->line_pam_or_nrz_type == CW_NRZ){
          fgb_type = FGB_NRZ_P50;
        } else if(cw_mode_ptr->host_pam_or_nrz_type == CW_PAM100 && cw_mode_ptr->line_pam_or_nrz_type == CW_NRZ){ 
          fgb_type = FGB_NRZ_P100;
        }
        
     }

     /* Get the logical lane mask for the physical lane mask */
     for(i=0; i < CHIP_MAX_LANES;i++){
        if(cw_mode_ptr->rpt_type == DIRECT) {
          if(cw_mode_ptr->line_plane_mask & (1<<i)){

            /* FWPLPHSIP-3998: move the 4x50G port from 8P (quad 0) to 4P (quad 1) to supported 4x50G/4x100G mixed mode 
               for repeater mode FEC monitor for A1 chip only
               refer to Portofino CW mode & register doc mixed_mode_2 sheet https://broadcom.ent.box.com/file/747177498971 
               50G port should use P0~P3 in 8P (same as before), P0,P2,P4,P6 in 4P (only for mixed_mode=2) */
               
            if (ml_cw_rtmr_is_A1_4p_port(cw_mode_ptr)) {
              cw_mode_ptr->line_llane_mask |= rtmr_map[cw_mode_ptr->rpt_type][CW_PAM100][i].line;
            } else
              cw_mode_ptr->line_llane_mask |= rtmr_map[cw_mode_ptr->rpt_type][cw_mode_ptr->line_pam_or_nrz_type][i].line;
          }
          if(cw_mode_ptr->host_plane_mask & (1<<i)){
            if (ml_cw_rtmr_is_A1_4p_port(cw_mode_ptr)) {
              cw_mode_ptr->host_llane_mask |= rtmr_map[cw_mode_ptr->rpt_type][CW_PAM100][i].host;
            } else
              cw_mode_ptr->host_llane_mask |= rtmr_map[cw_mode_ptr->rpt_type][cw_mode_ptr->host_pam_or_nrz_type][i].host;
          }
       } else if(cw_mode_ptr->rpt_type == GEARBOX || cw_mode_ptr->rpt_type == R_GEARBOX){
          if(cw_mode_ptr->line_plane_mask & (1<<i)){
            cw_mode_ptr->line_llane_mask |= rtmr_map[cw_mode_ptr->rpt_type][fgb_type][i].line;
          }
          if(cw_mode_ptr->host_plane_mask & (1<<i)){
            cw_mode_ptr->host_llane_mask |= rtmr_map[cw_mode_ptr->rpt_type][fgb_type][i].host;
          }
       }
    }
         
     //convert the capi fec_term to the internal format
     switch (fec_term) {
        case CHIP_LANE_FEC_DEC_XDEC_XENC_ENC:
            cw_mode_ptr->fec_dec_enc_mode = FEC_DEC_XDEC_XENC_ENC;
            break;

         case CHIP_PORT_FEC_DEC_ENC:
            if (port_entry_ptr->host_fec == CHIP_HOST_FEC_TYPE_RS528 && port_entry_ptr->line_fec==CHIP_LINE_FEC_TYPE_RS544)        
                cw_mode_ptr->fec_dec_enc_mode = FEC_DEC_ENC_KRKP;
            else if(port_entry_ptr->host_fec == CHIP_LINE_FEC_TYPE_RS544 && port_entry_ptr->line_fec==CHIP_HOST_FEC_TYPE_RS528)
                cw_mode_ptr->fec_dec_enc_mode = FEC_DEC_ENC_KPKR;
            else 
                cw_mode_ptr->fec_dec_enc_mode = FEC_DEC_ENC;            
            break;

         case CHIP_PORT_FEC_DEC_FWD:
             cw_mode_ptr->fec_dec_enc_mode = FEC_DEC_FWD;
             break;

         case CHIP_LANE_PCS_XENC:
             cw_mode_ptr->fec_dec_enc_mode = PCS_XENC;
             break;

         case CHIP_LANE_XENC_PCS:
             cw_mode_ptr->fec_dec_enc_mode = XENC_PCS;
             break;

         default:
             cw_mode_ptr->fec_dec_enc_mode = FEC_DEC_ENC;
             break;
     }


    /*
      pcs_slice =  PCS side logical lane mask
    */
    if ((cw_mode_ptr->host_pam_or_nrz_type == CW_NRZ) && (cw_mode_ptr->fec_dec_enc_mode == PCS_XENC)) 
    {
        cw_mode_ptr->pcs_slice = cw_mode_ptr->host_llane_mask;
    } 
    else if ((cw_mode_ptr->line_pam_or_nrz_type == CW_NRZ) && (cw_mode_ptr->fec_dec_enc_mode == XENC_PCS))
    {
        cw_mode_ptr->pcs_slice = cw_mode_ptr->line_llane_mask;
    }     

    /*
      Fec_slice =  Traffic speed / 50G
      E.g.    400G -> 0xFF
              200G -> 0x0F or 0xF0
              100G-> 0x3,0xC, 0x30, 0xC0
              50G-> 0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80
      except FEC 25G= port mask
              25G-> 0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80
    */
    switch (cw_mode_ptr->speed) {
        case SPEED_25G:
            for(i = 0; i < 8; i++) {
                if (cur_port_config_ptr->port_25g_en[i] == PORT_ON) {
                    cw_mode_ptr->fec_slice  = (0x1 << i);
                } 
            }
            break;
            
        case SPEED_50G:
            for(i = 0; i < 8; i++) {
                if (cur_port_config_ptr->port_50g_en[i] == PORT_ON) {
                    cw_mode_ptr->fec_slice  = (0x1 << i);
                } 
            }
            break;
            
        case SPEED_100G:
            if (cur_port_config_ptr->port_100g_en[0] == PORT_ON) {
                cw_mode_ptr->fec_slice  = 0x3;
            } 
            else if (cur_port_config_ptr->port_100g_en[1] == PORT_ON) {
                cw_mode_ptr->fec_slice  = 0xC;
            } 
            else if (cur_port_config_ptr->port_100g_en[2] == PORT_ON) {
                cw_mode_ptr->fec_slice  = 0x30;
            } 
            else if (cur_port_config_ptr->port_100g_en[3] == PORT_ON) {
                cw_mode_ptr->fec_slice  = 0xC0;
            } 
            break;

        case SPEED_200G:
            if (cur_port_config_ptr->port_200g_en[0] == PORT_ON) {
                cw_mode_ptr->fec_slice  = 0xF;
            } 
            else if (cur_port_config_ptr->port_200g_en[1] == PORT_ON) {
                cw_mode_ptr->fec_slice  = 0xF0;
            } 
            break;
                
        case SPEED_400G:
            cw_mode_ptr->fec_slice = 0xFF;
            break;
    }
}


/**
 *        cHAL C Hardware Abstract Layer for Line Wrapper IP
 *
 * @file chal_lw_diag_info_p.c
 * @author 
 * @date 9-5-2018
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
 * @brief   this file includes cHAL function implementation.
 *
 * @section
 *
 */
#include "math.h"
#include "chip_config_def.h"
#include "regs_common.h"
#include "chip_mode_def.h"
#include "capi_def.h"
#include "chal_lw_diag_info.h"

return_result_t chal_lw_diag_rx_clipdet_enable (phy_info_t* phy_info_ptr)
{
    
    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00005214)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00005214)), (phy_info_ptr)->phy_id) & (~(0x00000020))) | (((0x0) << (5)) & (0x00000020))), (phy_info_ptr)->phy_id)));

    
    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00005268)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00005268)), (phy_info_ptr)->phy_id) & (~(0x00000020))) | (((0x1) << (5)) & (0x00000020))), (phy_info_ptr)->phy_id)));

    
    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00005214)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00005214)), (phy_info_ptr)->phy_id) & (~(0x00000020))) | (((0x1) << (5)) & (0x00000020))), (phy_info_ptr)->phy_id)));

    
    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00004400)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00004400)), (phy_info_ptr)->phy_id) & (~(0x00000010))) | (((0x1) << (4)) & (0x00000010))), (phy_info_ptr)->phy_id)));

    
    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00004400)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00004400)), (phy_info_ptr)->phy_id) & (~(0x00000001))) | (((0x1) << (0)) & (0x00000001))), (phy_info_ptr)->phy_id)));
    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00004400)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00004400)), (phy_info_ptr)->phy_id) & (~(0x00000001))) | (((0x0) << (0)) & (0x00000001))), (phy_info_ptr)->phy_id)));

    return RR_SUCCESS;
}

return_result_t chal_lw_diag_rx_clipdet_get_enable (phy_info_t* phy_info_ptr, uint8_t *enabled)
{
    *enabled = ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00005214)), (phy_info_ptr)->phy_id) & (0x00000020)) >> (5));
    return RR_SUCCESS;
}

return_result_t chal_lw_diag_rx_clipdet_disable (phy_info_t* phy_info_ptr)
{
    
    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00005214)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00005214)), (phy_info_ptr)->phy_id) & (~(0x00000020))) | (((0x0) << (5)) & (0x00000020))), (phy_info_ptr)->phy_id)));

    
    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00005268)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00005268)), (phy_info_ptr)->phy_id) & (~(0x00000020))) | (((0x0) << (5)) & (0x00000020))), (phy_info_ptr)->phy_id)));

    return RR_SUCCESS;
}

return_result_t chal_lw_diag_rx_clipdet_config (phy_info_t* phy_info_ptr, clipdet_config_t* cfg_ptr)
{
    uint16_t reg_val = 0;

    reg_val =  (uint16_t)(rd_reg_ex(((phy_info_ptr)->base_addr) + (0x00004400), (phy_info_ptr)->phy_id));
    ((reg_val = (((reg_val) & ~(0x0000f800)) | (((cfg_ptr->adc_clp_exp_timer) << (11)) & (0x0000f800)))));
    ((reg_val = (((reg_val) & ~(0x00000780)) | (((cfg_ptr->adc_clp_sample_sel) << (7)) & (0x00000780)))));
    ((reg_val = (((reg_val) & ~(0x00000040)) | (((cfg_ptr->adc_clp_sample_rrbin) << (6)) & (0x00000040)))));
    ((reg_val = (((reg_val) & ~(0x00000008)) | (((cfg_ptr->adc_clp_neg_en) << (3)) & (0x00000008)))));
    ((reg_val = (((reg_val) & ~(0x00000004)) | (((cfg_ptr->adc_clp_pos_en) << (2)) & (0x00000004)))));
    ((reg_val = (((reg_val) & ~(0x00000020)) | (((0) << (5)) & (0x00000020)))));
    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00004400)), reg_val, (phy_info_ptr)->phy_id)));

    
    
    reg_val =  (uint16_t)(rd_reg_ex(((phy_info_ptr)->base_addr) + (0x00004404), (phy_info_ptr)->phy_id));
    ((reg_val = (((reg_val) & ~(0x00007f00)) | (((cfg_ptr->adc_clp_neg_thr) << (8)) & (0x00007f00)))));
    ((reg_val = (((reg_val) & ~(0x0000007f)) | (((cfg_ptr->adc_clp_pos_thr) << (0)) & (0x0000007f)))));
    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00004404)), reg_val, (phy_info_ptr)->phy_id)));

    return RR_SUCCESS;
}

return_result_t chal_lw_diag_rx_chk_clipdet (phy_info_t* phy_info_ptr, uint8_t* adc_clp_timer_maxed_ptr, uint32_t* adc_clp_cnt_ptr)
{
    
    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00004400)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00004400)), (phy_info_ptr)->phy_id) & (~(0x00000002))) | (((0x1) << (1)) & (0x00000002))), (phy_info_ptr)->phy_id)));
    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00004400)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00004400)), (phy_info_ptr)->phy_id) & (~(0x00000002))) | (((0x0) << (1)) & (0x00000002))), (phy_info_ptr)->phy_id)));

    *adc_clp_timer_maxed_ptr = (uint8_t) ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00004408)), (phy_info_ptr)->phy_id) & (0x00000001)) >> (0));
    *adc_clp_cnt_ptr         = (rd_reg_ex(((phy_info_ptr)->base_addr) + (0x0000440c), (phy_info_ptr)->phy_id)) +
                                   ((rd_reg_ex(((phy_info_ptr)->base_addr) + (0x00004410), (phy_info_ptr)->phy_id)) << 16);
    return RR_SUCCESS;
}

return_result_t chal_lw_diag_rx_clipdet_clear (phy_info_t* phy_info_ptr, uint8_t clear)
{
    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00004400)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00004400)), (phy_info_ptr)->phy_id) & (~(0x00000020))) | (((clear) << (5)) & (0x00000020))), (phy_info_ptr)->phy_id)));
    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00004400)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00004400)), (phy_info_ptr)->phy_id) & (~(0x00000001))) | (((0x1) << (0)) & (0x00000001))), (phy_info_ptr)->phy_id)));
    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00004400)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00004400)), (phy_info_ptr)->phy_id) & (~(0x00000001))) | (((0x0) << (0)) & (0x00000001))), (phy_info_ptr)->phy_id)));    
    return RR_SUCCESS;
}

return_result_t chal_lw_diag_rx_hist_en(phy_info_t* phy_info_ptr, uint8_t enable)
{
  if (enable) {
     
     
     ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00005504)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00005504)), (phy_info_ptr)->phy_id) & (~(0x00000010))) | (((1) << (4)) & (0x00000010))), (phy_info_ptr)->phy_id)));

     
     ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00005214)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00005214)), (phy_info_ptr)->phy_id) & (~(0x00000200))) | (((1) << (9)) & (0x00000200))), (phy_info_ptr)->phy_id)));

     
     ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x0000527c)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x0000527c)), (phy_info_ptr)->phy_id) & (~(0x0000003f))) | (((0) << (0)) & (0x0000003f))), (phy_info_ptr)->phy_id)));

     
     ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x0000527c)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x0000527c)), (phy_info_ptr)->phy_id) & (~(0x00000100))) | (((1) << (8)) & (0x00000100))), (phy_info_ptr)->phy_id)));

  } else {
     
     
     ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00005504)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00005504)), (phy_info_ptr)->phy_id) & (~(0x00000010))) | (((0) << (4)) & (0x00000010))), (phy_info_ptr)->phy_id)));

     
     ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00005214)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00005214)), (phy_info_ptr)->phy_id) & (~(0x00000200))) | (((0) << (9)) & (0x00000200))), (phy_info_ptr)->phy_id)));

     
     ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x0000527c)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x0000527c)), (phy_info_ptr)->phy_id) & (~(0x0000003f))) | (((0) << (0)) & (0x0000003f))), (phy_info_ptr)->phy_id)));

     
     ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x0000527c)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x0000527c)), (phy_info_ptr)->phy_id) & (~(0x00000100))) | (((0) << (8)) & (0x00000100))), (phy_info_ptr)->phy_id)));

  } 
  return (RR_SUCCESS);
}

return_result_t chal_lw_diag_rx_hist_config(phy_info_t* phy_info_ptr, hist_cfg_t* hist_cfg_ptr)
{
  ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00005b04)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00005b04)), (phy_info_ptr)->phy_id) & (~(0x07ffffff))) | (((hist_cfg_ptr->bin_cap) << (0)) & (0x07ffffff))), (phy_info_ptr)->phy_id)));
  (wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00005b08)), hist_cfg_ptr->timer_cap, (phy_info_ptr)->phy_id));
  ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00005b00)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00005b00)), (phy_info_ptr)->phy_id) & (~(0x0000000c))) | (((hist_cfg_ptr->hist_mode) << (2)) & (0x0000000c))), (phy_info_ptr)->phy_id)));
  ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00005b00)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00005b00)), (phy_info_ptr)->phy_id) & (~(0x00000010))) | (((hist_cfg_ptr->hist_sel) << (4)) & (0x00000010))), (phy_info_ptr)->phy_id)));

  return (RR_SUCCESS);
}

return_result_t chal_lw_diag_rx_hist_clear(phy_info_t* phy_info_ptr, uint8_t clear)
{
  if (clear) {
    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00005b00)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00005b00)), (phy_info_ptr)->phy_id) & (~(0x00000002))) | (((1) << (1)) & (0x00000002))), (phy_info_ptr)->phy_id)));
  } else {
    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00005b00)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00005b00)), (phy_info_ptr)->phy_id) & (~(0x00000002))) | (((0) << (1)) & (0x00000002))), (phy_info_ptr)->phy_id)));
  }

  return (RR_SUCCESS);
}

return_result_t chal_lw_diag_rx_hist_start(phy_info_t* phy_info_ptr, uint8_t start)
{
  if (start) {
    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00005b00)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00005b00)), (phy_info_ptr)->phy_id) & (~(0x00000001))) | (((1) << (0)) & (0x00000001))), (phy_info_ptr)->phy_id)));
  } else {
    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00005b00)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00005b00)), (phy_info_ptr)->phy_id) & (~(0x00000001))) | (((0) << (0)) & (0x00000001))), (phy_info_ptr)->phy_id)));
  }

  return (RR_SUCCESS);
}

return_result_t chal_lw_diag_rx_rd_hist(phy_info_t* phy_info_ptr, uint8_t mode, uint32_t* data_ptr)
{
  uint16_t i;
  if (mode == 1) {
      
      data_ptr+=64;
      for (i = 192; i < 256; i++)
          *data_ptr++ = (rd_reg_ex(((phy_info_ptr)->base_addr) + (0x00005700 + i*4), (phy_info_ptr)->phy_id));
      for (i = 0; i < 64; i++)
          *data_ptr++ = (rd_reg_ex(((phy_info_ptr)->base_addr) + (0x00005700 + i*4), (phy_info_ptr)->phy_id));
  } else {
      
      for (i = 128; i < 256; i++)
          *data_ptr++ = (rd_reg_ex(((phy_info_ptr)->base_addr) + (0x00005700 + i*4), (phy_info_ptr)->phy_id));
      for (i = 0; i < 128; i++)
          *data_ptr++ = (rd_reg_ex(((phy_info_ptr)->base_addr) + (0x00005700 + i*4), (phy_info_ptr)->phy_id));
  }
  return (RR_SUCCESS);
}

return_result_t chal_lw_diag_rx_hist_check_finish(phy_info_t* phy_info_ptr, uint8_t* data_ptr)
{
  *data_ptr = (uint8_t)((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00005b10)), (phy_info_ptr)->phy_id) & (0x00000001)) >> (0));
  return (RR_SUCCESS);
}

return_result_t chal_lw_diag_rx_hist_rd_timer(phy_info_t* phy_info_ptr, uint32_t* data_ptr)
{
  *data_ptr = (rd_reg_ex(((phy_info_ptr)->base_addr) + (0x00005b0c), (phy_info_ptr)->phy_id));
  return (RR_SUCCESS);
}

return_result_t chal_lw_diag_rx_dfe_mse_read (phy_info_t* phy_info_ptr, dsp_fast_brcm_snr_cfg_t *snr_cfg_ptr,  uint32_t *mse_val, uint32_t* mseval_lvl)
{
    uint32_t reg_rdata_lo, reg_rdata_hi;

    uint32_t  idx;
    uint8_t   lvl_multiplier = 1;

    *mseval_lvl = snr_cfg_ptr->fields.snr_level; 
    if(snr_cfg_ptr->fields.slice_mode<=EPAM2_MODE)
        lvl_multiplier = 3;

    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00005380)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00005380)), (phy_info_ptr)->phy_id) & (~(0x00000078))) | (((0x8) << (3)) & (0x00000078))), (phy_info_ptr)->phy_id))); 

    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00005380)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00005380)), (phy_info_ptr)->phy_id) & (~(0x00000002))) | (((0x1) << (1)) & (0x00000002))), (phy_info_ptr)->phy_id))); 
    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00005380)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00005380)), (phy_info_ptr)->phy_id) & (~(0x00000002))) | (((0x0) << (1)) & (0x00000002))), (phy_info_ptr)->phy_id))); 

    reg_rdata_lo = ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00005398)), (phy_info_ptr)->phy_id) & (0x0000ffff)) >> (0)); 
    reg_rdata_hi = ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x0000539c)), (phy_info_ptr)->phy_id) & (0x00000fff)) >> (0)); 
    *mse_val = (reg_rdata_hi << 16) | reg_rdata_lo; 

    for (idx = 0; idx < snr_cfg_ptr->fields.snr_level; idx++) {
        ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00005380)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00005380)), (phy_info_ptr)->phy_id) & (~(0x00000078))) | (((idx*lvl_multiplier) << (3)) & (0x00000078))), (phy_info_ptr)->phy_id)));
        ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00005380)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00005380)), (phy_info_ptr)->phy_id) & (~(0x00000002))) | (((0x1) << (1)) & (0x00000002))), (phy_info_ptr)->phy_id))); 
        ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00005380)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00005380)), (phy_info_ptr)->phy_id) & (~(0x00000002))) | (((0x0) << (1)) & (0x00000002))), (phy_info_ptr)->phy_id))); 

        reg_rdata_lo = ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00005398)), (phy_info_ptr)->phy_id) & (0x0000ffff)) >> (0)); 
        reg_rdata_hi = ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x0000539c)), (phy_info_ptr)->phy_id) & (0x00000fff)) >> (0)); 
        *(mseval_lvl + idx + 1) = (reg_rdata_hi << 16) | reg_rdata_lo;
    }
    return RR_SUCCESS;
}

return_result_t chal_lw_diag_ffe_mse_get (phy_info_t* phy_info_ptr, dsp_fast_brcm_snr_cfg_t *snr_cfg_ptr, uint32_t *mse_val, uint32_t* mseval_lvl)
{
    uint32_t reg_rdata_lo, reg_rdata_hi;
    uint32_t  idx;
    uint8_t   lvl_multiplier = 1;

    *mseval_lvl = snr_cfg_ptr->fields.snr_level; 
    if(snr_cfg_ptr->fields.slice_mode<=EPAM2_MODE)
        lvl_multiplier = 3;

    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x000055d8)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x000055d8)), (phy_info_ptr)->phy_id) & (~(0x00000078))) | ((((0x8 & 0x0f)) << (3)) & (0x00000078))), (phy_info_ptr)->phy_id))); 

    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x000055d8)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x000055d8)), (phy_info_ptr)->phy_id) & (~(0x00000002))) | (((LD_HIGH) << (1)) & (0x00000002))), (phy_info_ptr)->phy_id))); 
    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x000055d8)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x000055d8)), (phy_info_ptr)->phy_id) & (~(0x00000002))) | (((LD_LOW) << (1)) & (0x00000002))), (phy_info_ptr)->phy_id))); 

    reg_rdata_lo = (rd_reg_ex(((phy_info_ptr)->base_addr) + (0x000055f0), (phy_info_ptr)->phy_id)); 
    reg_rdata_hi = (rd_reg_ex(((phy_info_ptr)->base_addr) + (0x000055f4), (phy_info_ptr)->phy_id)); 
    *mse_val  = (reg_rdata_hi << 16) | reg_rdata_lo; 
    

    for (idx = 0; idx < snr_cfg_ptr->fields.snr_level; idx++) {
        ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x000055d8)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x000055d8)), (phy_info_ptr)->phy_id) & (~(0x00000078))) | (((idx*lvl_multiplier) << (3)) & (0x00000078))), (phy_info_ptr)->phy_id)));
        ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x000055d8)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x000055d8)), (phy_info_ptr)->phy_id) & (~(0x00000002))) | (((LD_HIGH) << (1)) & (0x00000002))), (phy_info_ptr)->phy_id))); 
        ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x000055d8)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x000055d8)), (phy_info_ptr)->phy_id) & (~(0x00000002))) | (((LD_LOW) << (1)) & (0x00000002))), (phy_info_ptr)->phy_id))); 

        reg_rdata_lo = (rd_reg_ex(((phy_info_ptr)->base_addr) + (0x000055f0), (phy_info_ptr)->phy_id)); 
        reg_rdata_hi = (rd_reg_ex(((phy_info_ptr)->base_addr) + (0x000055f4), (phy_info_ptr)->phy_id)); 
        *(mseval_lvl + idx + 1) = (reg_rdata_hi << 16) | reg_rdata_lo;
    }
    return RR_SUCCESS; 
}

double chal_lw_diag_calculate_snr(ffe_slc_mode_t slc_mode, uint32_t mse_val)
{
    double mval, mse_int;

    if ((slc_mode== NRZ_MODE)||(slc_mode== EPAM2_MODE))
        mval = 9;
    else   
        mval = 5;

    mse_int  = ((double)mse_val) / (1 << 28);
    if (mse_int == 0)
        mse_int = 0.0000001;

    return (10 * log10(mval) - 10 * log10(mse_int));
}

return_result_t chal_lw_diag_ffe_write_clear_saturation_st (phy_info_t* phy_info_ptr)
{
    (wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00005640)), 0xFFFF, (phy_info_ptr)->phy_id));
    (wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00005644)), 0xFFFF, (phy_info_ptr)->phy_id));
    (wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00005648)), 0xFFFF, (phy_info_ptr)->phy_id));
    (wr_reg_ex((((phy_info_ptr)->base_addr) + (0x0000564c)), 0xFFFF, (phy_info_ptr)->phy_id));

    return RR_SUCCESS;
}

return_result_t chal_lw_diag_ffe_get_saturation_st (phy_info_t* phy_info_ptr, ffe_saturation_t *ffe_stat_ptr)
{
    ffe_stat_ptr->over_stat_l  = (uint16_t)(rd_reg_ex(((phy_info_ptr)->base_addr) + (0x00005640), (phy_info_ptr)->phy_id));
    ffe_stat_ptr->under_stat_l = (uint16_t)(rd_reg_ex(((phy_info_ptr)->base_addr) + (0x00005644), (phy_info_ptr)->phy_id));
    ffe_stat_ptr->over_stat_h  = (uint16_t)(rd_reg_ex(((phy_info_ptr)->base_addr) + (0x00005648), (phy_info_ptr)->phy_id));
    ffe_stat_ptr->under_stat_h = (uint16_t)(rd_reg_ex(((phy_info_ptr)->base_addr) + (0x0000564c), (phy_info_ptr)->phy_id));

    return RR_SUCCESS;
}

return_result_t chal_lw_diag_ffe_capture_9_ffe_taps(phy_info_t* phy_info_ptr, uint8_t acc_sel, uint8_t round_sel, int32_t* coef_ptr, float* float_coef_ptr)
{

    uint16_t reg_val = 0;
    int16_t coef_16_lo;
    int16_t coef_16_hi;
    uint8_t i;
    float fmt;
    float main_tap_coef = 0;
    uint8_t maintap =  ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00004d1c)), (phy_info_ptr)->phy_id) & (0x0000000f)) >> (0));
    uint8_t ffe_tap_min = (maintap < 4) ? 0 : (maintap - 4) ;

    reg_val = (uint16_t)(rd_reg_ex(((phy_info_ptr)->base_addr) + (0x00005500), (phy_info_ptr)->phy_id));
    ((reg_val = (((reg_val) & ~(0x00000080)) | (((round_sel) << (7)) & (0x00000080)))));
    ((reg_val = (((reg_val) & ~(0x00004000)) | (((acc_sel) << (14)) & (0x00004000)))));
    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00005500)), reg_val, (phy_info_ptr)->phy_id)));  
  
    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x0000550c)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x0000550c)), (phy_info_ptr)->phy_id) & (~(0x00000100))) | (((1) << (8)) & (0x00000100))), (phy_info_ptr)->phy_id)));
    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x0000550c)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x0000550c)), (phy_info_ptr)->phy_id) & (~(0x00000100))) | (((0) << (8)) & (0x00000100))), (phy_info_ptr)->phy_id)));

    if (acc_sel) {
        if (round_sel == 0)
            fmt = 1 << 4;
        else
            fmt = 1 << 26;
    }
    else
        fmt = 1 << 10;

    for (i = 0; i < 9; i++) 
    {
        uint8_t tap = i + ffe_tap_min; 
        ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00005500)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00005500)), (phy_info_ptr)->phy_id) & (~(0x0000001f))) | (((tap) << (0)) & (0x0000001f))), (phy_info_ptr)->phy_id)));
        reg_val = (uint16_t)(rd_reg_ex(((phy_info_ptr)->base_addr) + (0x00005530), (phy_info_ptr)->phy_id));
        coef_16_lo = reg_val;
        coef_16_hi = (uint16_t)(rd_reg_ex(((phy_info_ptr)->base_addr) + (0x00005534), (phy_info_ptr)->phy_id));
        *(coef_ptr + i) = coef_16_lo |(coef_16_hi <<16);
        *(float_coef_ptr + i) = *(coef_ptr + i)/fmt;

        if (tap == maintap)
            main_tap_coef = *(float_coef_ptr + i);
    }
    for (i = 0; i < 9; i++) 
    {
        *(float_coef_ptr + i) /= main_tap_coef;
    }
    return RR_SUCCESS;
}

int32_t chal_lw_diag_rx_cdr_get_integ_ppm(phy_info_t* phy_info_ptr )
{
    int32_t rd_val; 

    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00004d04)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00004d04)), (phy_info_ptr)->phy_id) & (~(0x00000004))) | (((0x1) << (2)) & (0x00000004))), (phy_info_ptr)->phy_id)));
    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00004d04)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00004d04)), (phy_info_ptr)->phy_id) & (~(0x00000004))) | (((0x0) << (2)) & (0x00000004))), (phy_info_ptr)->phy_id)));

    rd_val = (int32_t)((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00004d88)), (phy_info_ptr)->phy_id) & (0x0000ffff)) >> (0));
    rd_val = rd_val << 16;
    rd_val |= (((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00004d8c)), (phy_info_ptr)->phy_id) & (0x0000ffff)) >> (0)) & 0xFFFF);

    return rd_val;
} 

return_result_t chal_lw_diag_hsip_jf_rd_integ(phy_info_t* phy_info_ptr, int16_t* integ_12_0)
{
    uint16_t reg_val;
    uint8_t msb;
    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00000110)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00000110)), (phy_info_ptr)->phy_id) & (~(0x00008000))) | (((0x0) << (15)) & (0x00008000))), (phy_info_ptr)->phy_id)));
    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00000110)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00000110)), (phy_info_ptr)->phy_id) & (~(0x00008000))) | (((0x1) << (15)) & (0x00008000))), (phy_info_ptr)->phy_id)));
    do { int __hsip_err; *(&__hsip_err) = 0; (reg_val = ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x0000011c)), (phy_info_ptr)->phy_id) & (0x00003fff)) >> (0))); if (*(&__hsip_err) != 0) { return RR_ERROR_MDIO; } (void)__hsip_err; } while(0);
    ((wr_reg_ex((((phy_info_ptr)->base_addr) + (0x00000110)), ((rd_reg_ex((((phy_info_ptr)->base_addr) + (0x00000110)), (phy_info_ptr)->phy_id) & (~(0x00008000))) | (((0x0) << (15)) & (0x00008000))), (phy_info_ptr)->phy_id)));

    msb = (reg_val >> 12);
    if (msb) {
        *integ_12_0 = (0xE000 | reg_val);
    } else {
        *integ_12_0 = reg_val;
    }
    return RR_SUCCESS;
}

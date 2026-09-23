/**
 *
 * @file     chal_cw_rtmr_status_check.c
 * @author   
 * @date     10/2/2020
 * @version  1.0
 *
 * @property 
 * $Copyright: (c) 2020 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * @brief    brief description of the block source file, sequences/flow/etc.
 *
 * @section  description
 *
 */
#include "regs_common.h"
#include "chip_mode_def.h"
#include "cw_def.h"
#include "chal_cw_rtmr_status_check.h"

extern uint8_t util_get_lowest_index_from_mask (uint16_t llane_mask);

uint16_t chal_cw_rtmr_fec_sync_status(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr)
{
     // read fec alignment status. Live status,not sticky
     uint16_t reg_val = 0;
     int __hsip_err = 0;
     reg_val = (uint16_t)hsip_rd_reg_(phy_info_ptr, AM_LANE_ALIGNMENT_STATUS_REG);
     switch (cur_mode_parameter_ptr->speed) {
         case (SPEED_400G):
             reg_val &= 0x1;  
         break;
         case (SPEED_200G):
             if (cur_port_config_ptr->port_200g_en[0] == PORT_ON ) {
                reg_val &= 0x1;
             } else if (cur_port_config_ptr->port_200g_en[1] == PORT_ON) {
                reg_val &= 0x10;
             } else {
            reg_val = 0x00;
             }
         break;
         case (SPEED_100G):
            if (cur_port_config_ptr->port_100g_en[0] == PORT_ON) {
                reg_val &= 0x01;
             } else if(cur_port_config_ptr->port_100g_en[1] == PORT_ON) {
                reg_val &= 0x04;
             } else if(cur_port_config_ptr->port_100g_en[2] == PORT_ON) {
                reg_val &= 0x10;
             } else if(cur_port_config_ptr->port_100g_en[3] == PORT_ON) {
                reg_val &= 0x40;
             } else {
            reg_val = 0x0;
             }
         break;
         case (SPEED_50G):
            if (cur_port_config_ptr->port_50g_en[0] == PORT_ON) {
                reg_val &= 0x01;
             } else if(cur_port_config_ptr->port_50g_en[1] == PORT_ON) {
                reg_val &= 0x02;
             } else if(cur_port_config_ptr->port_50g_en[2] == PORT_ON) {
                reg_val &= 0x04;
             } else if(cur_port_config_ptr->port_50g_en[3] == PORT_ON) {
                reg_val &= 0x08;
             } else if(cur_port_config_ptr->port_50g_en[4] == PORT_ON) {
                reg_val &= 0x10;
             } else if(cur_port_config_ptr->port_50g_en[5] == PORT_ON) {
                reg_val &= 0x20;
             } else if(cur_port_config_ptr->port_50g_en[6] == PORT_ON) {
                reg_val &= 0x40;
             } else if(cur_port_config_ptr->port_50g_en[7] == PORT_ON) {
                reg_val &= 0x80;
             } else {
                reg_val = 0x00;
             }
         break;
         case (SPEED_25G):
            if (cur_port_config_ptr->port_25g_en[0] == PORT_ON) {
                reg_val &= 0x01;
             } else if(cur_port_config_ptr->port_25g_en[1] == PORT_ON) {
                reg_val &= 0x02;
             } else if(cur_port_config_ptr->port_25g_en[2] == PORT_ON) {
                reg_val &= 0x04;
             } else if(cur_port_config_ptr->port_25g_en[3] == PORT_ON) {
                reg_val &= 0x08;
             } else if(cur_port_config_ptr->port_25g_en[4] == PORT_ON) {
                reg_val &= 0x10;
             } else if(cur_port_config_ptr->port_25g_en[5] == PORT_ON) {
                reg_val &= 0x20;
             } else if(cur_port_config_ptr->port_25g_en[6] == PORT_ON) {
                reg_val &= 0x40;
             } else if(cur_port_config_ptr->port_25g_en[7] == PORT_ON) {
                reg_val &= 0x80;
             } else {
                reg_val = 0x00;
             }
         break;
         default:
         break;
     }     
    (void) __hsip_err;
    return reg_val;
}

// pcs sync Checked RETIMER_IGR or RETIMER_EGR
uint16_t chal_cw_rtmr_pcs_sync_get_status(phy_info_t* phy_info_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, cw_port_config_t* cur_port_config_ptr)
{
   // get pcs  sync live status. 1 means pcs sync done
  uint16_t reg_val = 0;
  int __hsip_err = 0;
  switch (cur_mode_parameter_ptr->speed) {
  case (SPEED_50G):
      if (cur_port_config_ptr->port_50g_en[0] == PORT_ON) {
          reg_val = hsip_rd_field_(phy_info_ptr, PCS_STATUS1_REGISTER_50G_100G_0, PCS_BLOCK_LOCK);
      } else if (cur_port_config_ptr->port_50g_en[2] == PORT_ON) {
          reg_val = hsip_rd_field_(phy_info_ptr, PCS_STATUS1_REGISTER_50G_100G_2, PCS_BLOCK_LOCK);
      } 
//#ifdef SUPPORT_4_7_LANE
      else if (cur_port_config_ptr->port_50g_en[4] == PORT_ON) {
          reg_val = hsip_rd_field_(phy_info_ptr, PCS_STATUS1_REGISTER_50G_100G_4, PCS_BLOCK_LOCK);
      } else if (cur_port_config_ptr->port_50g_en[6]) {
          reg_val = hsip_rd_field_(phy_info_ptr, PCS_STATUS1_REGISTER_50G_100G_6, PCS_BLOCK_LOCK);
      }
//#endif
      break;
  case (SPEED_100G):
      if (cur_port_config_ptr->port_100g_en[0] == PORT_ON) {
          reg_val = hsip_rd_field_(phy_info_ptr, PCS_STATUS1_REGISTER_50G_100G_0, PCS_BLOCK_LOCK);
      } 
//#ifdef SUPPORT_4_7_LANE
      else if (cur_port_config_ptr->port_100g_en[2] == PORT_ON) {
          reg_val = hsip_rd_field_(phy_info_ptr, PCS_STATUS1_REGISTER_50G_100G_4, PCS_BLOCK_LOCK);
      }
//#endif
      break;
  default:
      break;
   }
  (void) __hsip_err;
  return reg_val;
}


uint16_t chal_cw_rtmr_tmt_pfifo_get_lane_mask(cw_mode_parameter_t* cur_mode_parameter_ptr, cfg_egr_or_igr_t egr_or_igr)
{
    uint16_t wmask;

    wmask = (egr_or_igr == IGR) ? cur_mode_parameter_ptr->host_llane_mask : cur_mode_parameter_ptr->line_llane_mask;

    // Special case handling for 50G (2x25G NRZ to 1x50G PAM mode) rgbox/fgbox mapping, PFIFO selection: NRZ side uses lane mask, PAM side uses FEC slice
    // Chip modes: CHIP_MODE_2X25G_KR4NRZ_TO_1X51G_KR4PAM, CHIP_MODE_2x25G_NRZ_TO_1X53G_KP4PAM, CHIP_MODE_2X26G_KP4NRZ_TO_1X53G_KP4PAM, CHIP_MODE_2X25G_KR4NRZ_TO_1X51G_KR4PAM
    if (cur_mode_parameter_ptr->speed == SPEED_50G) {
        if (egr_or_igr == EGR && cur_mode_parameter_ptr->rpt_type == GEARBOX) {
            // line side tx pfifo: 0x1, 0x4, 0x10, 0x40
            wmask = cur_mode_parameter_ptr->fec_slice;
        }
        if (egr_or_igr == IGR && cur_mode_parameter_ptr->rpt_type == R_GEARBOX) {
            // client side tx pfifo: 0x1, 0x4, 0x10, 0x40
            wmask = cur_mode_parameter_ptr->fec_slice;
        }
    }

    // Special case handling for 100G (4x25G NRZ to 2x50G PAM mode) rgbox/fgbox mapping, PFIFO selection: NRZ side uses lane mask, PAM side uses FEC slice
    // chip modes: CHIP_MODE_4x25G_NRZ_TO_2X53G_KP4PAM, CHIP_MODE_4X26G_KP4NRZ_TO_2X53G_KP4PAM
    if (cur_mode_parameter_ptr->speed == SPEED_100G) {
       if (egr_or_igr == EGR && cur_mode_parameter_ptr->rpt_type == GEARBOX && cur_mode_parameter_ptr->line_pam_or_nrz_type == CW_PAM) {
           // line side tx pfifo: 0x3, 0x30
           wmask = cur_mode_parameter_ptr->fec_slice;
       }
       if (egr_or_igr == IGR && cur_mode_parameter_ptr->rpt_type == R_GEARBOX && cur_mode_parameter_ptr->host_pam_or_nrz_type == CW_PAM) {
           // client side tx pfifo: 0x3, 0x30
           wmask = cur_mode_parameter_ptr->fec_slice;
       }
    }

	return wmask;
}



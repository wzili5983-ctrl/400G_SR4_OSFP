/**
 *
 * @file     chal_cw_rtmr_kp4prbs.c 
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
#include <stdio.h>
#include "access.h"
#include "regs_common.h"
#include "common_def.h"
#include "chip_mode_def.h"
#include "cw_def.h"
#include "common_util.h"
#include "pam4_prbs_def.h"
#include "chal_cw_prbs.h"
#include "chal_cw_rtmr_kp4prbs.h"
#include "chal_cw_rtmr_clkrst_control.h"
#include "chal_cw_rtmr_clockrst_mux.h"
#include "hr_time.h"

const uint32_t fec_prbs[2][8]={{RTMR_QC_PGEN_EGR_0, RTMR_QC_PGEN_EGR_1, RTMR_QC_PGEN_EGR_2, RTMR_QC_PGEN_EGR_3,
                                RTMR_QC_PGEN_EGR_4, RTMR_QC_PGEN_EGR_5, RTMR_QC_PGEN_EGR_6, RTMR_QC_PGEN_EGR_7
                                },
                               {RTMR_QC_PGEN_IGR_0, RTMR_QC_PGEN_IGR_1, RTMR_QC_PGEN_IGR_2, RTMR_QC_PGEN_IGR_3,
                               RTMR_QC_PGEN_IGR_4, RTMR_QC_PGEN_IGR_5, RTMR_QC_PGEN_IGR_6, RTMR_QC_PGEN_IGR_7
                                }};

//==========================================================================
//~~                            PGEN Functions
//==========================================================================


static void _get_cfg_list_array(cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr, _fec_prbs_cfg_list_t *cfg_list_ptr)
{
    uint8_t idx;
    switch (cur_mode_parameter_ptr->speed) {
        case (SPEED_100G):
            /*port 0: Lane 0,1 ; port 2:Lane 2,3 ; port 2: Lane 4,5 ; port 3:Lane 6,7 ; */
            for(idx=0; idx<4; idx++){
                if(cur_port_config_ptr->port_100g_en[idx]==0)
                    continue;
                cfg_list_ptr->val_idx = idx<<1;
                cfg_list_ptr->lst_beg = idx<<1;
                cfg_list_ptr->lst_end = (idx<<1)+1;
            }
            break;

        case (SPEED_200G):
            /*port 0: Lane 0,1,2,3 ; port 4:Lane 4,5,6,7 */
            for(idx=0; idx<2; idx++){
                if(cur_port_config_ptr->port_200g_en[idx]==0)
                    continue;
                cfg_list_ptr->val_idx = idx<<2;
                cfg_list_ptr->lst_beg = idx<<2;
                cfg_list_ptr->lst_end = (idx<<2)+3;
            }
            break;
        case (SPEED_400G):
            /*port 0: Lane 0,1,2,3,4,5,6,7 */
            cfg_list_ptr->val_idx = 0;
            cfg_list_ptr->lst_beg = 0;
            cfg_list_ptr->lst_end = 7;        
            break;
        case (SPEED_50G):
            /*port x: Lane x */
            for(idx=0; idx<8; idx++){
                if(cur_port_config_ptr->port_50g_en[idx]==0)
                    continue;
                cfg_list_ptr->val_idx = idx;
                cfg_list_ptr->lst_beg = idx;
                cfg_list_ptr->lst_end = idx;
            }
            break;
        case (SPEED_25G):
            /*port x: Lane x */
            for(idx=0; idx<8; idx++){
                if(cur_port_config_ptr->port_25g_en[idx]==0)
                    continue;
                cfg_list_ptr->val_idx = idx;
                cfg_list_ptr->lst_beg = idx;
                cfg_list_ptr->lst_end = idx;
            }
            break;
    }
}

/**
 * @brief  chal_pam4_pgen_fec_prbs_clk_cfg( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_mode_parameter_t* cur_mode_parameter_ptr)
 * @detail Function that makes the PGENs get bundled clock 
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in]  cur_mode_parameter_ptr : pointer to mode parameter structure 

 * @return    void:
*/
void chal_pam4_pgen_fec_prbs_clk_cfg (phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
{
    uint16_t clkval_list[MAX_PORT] = {0x100, 0x200, 0x400, 0x800};
    uint8_t wr_idx;
    _fec_prbs_cfg_list_t cfg_list;
    _get_cfg_list_array(cur_port_config_ptr, cur_mode_parameter_ptr, &cfg_list);
    for(wr_idx=cfg_list.lst_beg; wr_idx<=cfg_list.lst_end; wr_idx++){
        hsip_wr_fields(phy_info_ptr,  LANE_PRBS_CLK_RST_MUX_REG, clkval_list[wr_idx], 0x0000);       
    }
}


/**
 * @brief  chal_pam4_pmon_fec_prbs_fec_ip_data_mux_cfg (phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
 * @detail Function that configures the FEC slice inputs to be PGEN data. 
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in]  cur_mode_parameter_ptr : pointer to mode parameter structure 

 * @return    void:
*/
void chal_pam4_pmon_fec_prbs_fec_ip_data_mux_cfg (phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
{
    uint32_t reg_list[MAX_PORT] = {FEC_DATA_MUX_SEL0_REG, FEC_DATA_MUX_SEL1_REG, FEC_DATA_MUX_SEL2_REG, FEC_DATA_MUX_SEL3_REG};
    uint32_t reg_mask_list[MAX_PORT]={FEC_DATA_MUX_SEL0_REG_MD_FEC_ENC_DATA_MUX0_MASK, FEC_DATA_MUX_SEL1_REG_MD_FEC_ENC_DATA_MUX1_MASK, 
                               FEC_DATA_MUX_SEL2_REG_MD_FEC_ENC_DATA_MUX2_MASK, FEC_DATA_MUX_SEL3_REG_MD_FEC_ENC_DATA_MUX3_MASK};
    uint32_t reg_shift_list[MAX_PORT]={FEC_DATA_MUX_SEL0_REG_MD_FEC_ENC_DATA_MUX0_SHIFT, FEC_DATA_MUX_SEL1_REG_MD_FEC_ENC_DATA_MUX1_SHIFT, 
                               FEC_DATA_MUX_SEL2_REG_MD_FEC_ENC_DATA_MUX2_SHIFT, FEC_DATA_MUX_SEL3_REG_MD_FEC_ENC_DATA_MUX3_SHIFT};
    uint8_t wr_idx;
    _fec_prbs_cfg_list_t cfg_list;

    _get_cfg_list_array(cur_port_config_ptr, cur_mode_parameter_ptr, &cfg_list);
    for(wr_idx=cfg_list.lst_beg; wr_idx<=cfg_list.lst_end; wr_idx++){
        writereg32field(phy_info_ptr->base_addr+reg_list[wr_idx], reg_shift_list[wr_idx], reg_mask_list[wr_idx], 0x0, phy_info_ptr->phy_id);
    }
}

static uint32_t chal_cw_rtmr_fec_prbs_get_base_addr( phy_info_t* phy_info_ptr,
                                              cw_mode_parameter_t* cur_mode_parameter_ptr)
{
    uint32_t base_addr = phy_info_ptr->base_addr;

    // if port 4-7 and PAM100, use upper DP wrapper
    if((cur_mode_parameter_ptr->host_plane_mask & 0xF0) && 
        (cur_mode_parameter_ptr->host_pam_or_nrz_type == CW_PAM100) &&
        (cur_mode_parameter_ptr->line_pam_or_nrz_type == CW_PAM100))
        base_addr += (RETIMER_EGR1 - RETIMER_EGR0);
    return base_addr;
}

/**
 * @brief  chal_cw_rtmr_fec_prbs_pgen_config_wrapper( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, pam4_pgen_misc_cfg_t* pam4_pgen_misc_cfg_ptr,
 *                                                    cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
 * @detail Wrapper than decides the speed to decide which PGEN lanes should be active 
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in]  cur_mode_parameter_ptr : pointer to mode parameter structure 
 * @return    void:
*/
void chal_cw_rtmr_fec_prbs_pgen_config_wrapper( phy_info_t* phy_info_ptr, 
                                                cfg_egr_or_igr_t egr_or_igr, 
                                                pam4_pgen_misc_cfg_t* pam4_pgen_misc_cfg_ptr,
                                                cw_port_config_t* cur_port_config_ptr, 
                                                cw_mode_parameter_t* cur_mode_parameter_ptr)
{
    phy_info_t phy_info_ptr_1;
    uint8_t wr_idx;
    _fec_prbs_cfg_list_t cfg_list;

    _get_cfg_list_array(cur_port_config_ptr, cur_mode_parameter_ptr, &cfg_list);
    //Need to take copy of phy_info_ptr as we need to make multiple function calls. 
    util_memcpy((void *)&phy_info_ptr_1, phy_info_ptr, sizeof(phy_info_t)); 
    
    for(wr_idx=cfg_list.lst_beg; wr_idx<=cfg_list.lst_end; wr_idx++){
        phy_info_ptr_1.base_addr = chal_cw_rtmr_fec_prbs_get_base_addr(phy_info_ptr, cur_mode_parameter_ptr) 
            + fec_prbs[egr_or_igr][wr_idx]; 
        delay_ms(1);
        chal_cw_pgen_config (&phy_info_ptr_1,pam4_pgen_misc_cfg_ptr);
    }
}

/**
 * @brief  chal_cw_rtmr_fec_prbs_pgen_enable_disable_wrapper( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,onoff_t enable)
 * @detail Fuction that enables or disables the wrapper 
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in]  cur_mode_parameter_ptr : pointer to mode parameter structure 
 * @param[in]  enable : To enable or disable 
 * @return    void:
*/
uint8_t chal_cw_rtmr_fec_prbs_pgen_enable_disable_wrapper( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,onoff_t enable)
{

    phy_info_t phy_info_ptr_1;
    uint8_t wr_idx, lock_flag=0;
    _fec_prbs_cfg_list_t cfg_list;

    util_memcpy((void *)&phy_info_ptr_1, phy_info_ptr, sizeof(phy_info_t));
    _get_cfg_list_array(cur_port_config_ptr, cur_mode_parameter_ptr, &cfg_list);

    for(wr_idx=cfg_list.lst_beg; wr_idx<=cfg_list.lst_end; wr_idx++){
        phy_info_ptr_1.base_addr = chal_cw_rtmr_fec_prbs_get_base_addr(phy_info_ptr, cur_mode_parameter_ptr) +
            fec_prbs[egr_or_igr][wr_idx]; 
        lock_flag |= chal_cw_pgen_enable(&phy_info_ptr_1,enable);
    }
    return lock_flag;
}

/**
 * @brief  chal_cw_rtmr_fec_prbs_pmon_config_wrapper( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, 
            pam4_pmon_misc_cfg_t *pmon_misc_struct_inst_ptr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
 * @detail Wrapper than decides the speed to decide which PMON lanes should be active 
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in]  pmon_misc_struct_inst_ptr : pmon config 
 * @param[in]  cur_port_config_ptr : port config
 * @param[in]  cur_mode_parameter_ptr : pointer to mode parameter structure 
 * @return    void:
*/

void chal_cw_rtmr_fec_prbs_pmon_config_wrapper( phy_info_t* phy_info_ptr, 
                                            cfg_egr_or_igr_t egr_or_igr,
                                            pam4_pmon_misc_cfg_t *pmon_misc_struct_inst_ptr,
                                            cw_port_config_t* cur_port_config_ptr,
                                            cw_mode_parameter_t* cur_mode_parameter_ptr)
{
    phy_info_t phy_info_ptr_1;
    uint8_t wr_idx;
    _fec_prbs_cfg_list_t cfg_list;

    util_memcpy((void *)&phy_info_ptr_1, phy_info_ptr, sizeof(phy_info_t)); 

    _get_cfg_list_array(cur_port_config_ptr, cur_mode_parameter_ptr, &cfg_list);

    for(wr_idx=cfg_list.lst_beg; wr_idx<=cfg_list.lst_end; wr_idx++){
        phy_info_ptr_1.base_addr = chal_cw_rtmr_fec_prbs_get_base_addr(phy_info_ptr, cur_mode_parameter_ptr) +
            fec_prbs[egr_or_igr][wr_idx]; 
        chal_cw_pmon_config(&phy_info_ptr_1, pmon_misc_struct_inst_ptr);
    }
}

/**
 * @brief  chal_cw_rtmr_fec_prbs_pmon_enable_disable_wrapper( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,onoff_t enable)
 * @detail Fuction that enables or disables the PMON instances corresponding to the speed  
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in]  cur_mode_parameter_ptr : pointer to mode parameter structure 
 * @param[in]  enable : To enable or disable 
 * @return    void:
*/
void chal_cw_rtmr_fec_prbs_pmon_enable_disable_wrapper( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr,onoff_t enable)
{
    uint8_t wr_idx;
    _fec_prbs_cfg_list_t cfg_list;
    phy_info_t phy_info_ptr_1;

    util_memcpy((void *)&phy_info_ptr_1, phy_info_ptr, sizeof(phy_info_t)); 
    _get_cfg_list_array(cur_port_config_ptr, cur_mode_parameter_ptr, &cfg_list);

    for(wr_idx=cfg_list.lst_beg; wr_idx<=cfg_list.lst_end; wr_idx++){
        phy_info_ptr_1.base_addr = chal_cw_rtmr_fec_prbs_get_base_addr(phy_info_ptr, cur_mode_parameter_ptr) +
            fec_prbs[egr_or_igr][wr_idx]; 
        chal_cw_pmon_enable(&phy_info_ptr_1,enable);
    }
}


/**
 * @brief  chal_cw_rtmr_pmon_get_status( phy_info_t* phy_info_ptr, 
                                    cfg_egr_or_igr_t egr_or_igr, 
                                    cw_port_config_t* cur_port_config_ptr, 
                                    cw_mode_parameter_t* cur_mode_parameter_ptr, 
                                    pam4_pmon_stat_t* pam4_pmon_stat_ptr)
 * @detail Function that gets the PMON  status corresponding to the lane  
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in]  cur_port_config_ptr: port config pointer
 * @param[in]  cur_mode_parameter_ptr : pointer to mode parameter structure 
 * @param[out]  pam4_pmon_stat_ptr: pmon stat pointer. 
 * @return    void:
*/

void chal_cw_rtmr_pmon_get_status( phy_info_t* phy_info_ptr, 
                                    cfg_egr_or_igr_t egr_or_igr, 
                                    cw_port_config_t* cur_port_config_ptr, 
                                    cw_mode_parameter_t* cur_mode_parameter_ptr, 
                                    pam4_pmon_stat_t* pam4_pmon_stat_ptr)
{
    phy_info_t phy_info_ptr_1;
    uint8_t wr_idx;
    _fec_prbs_cfg_list_t cfg_list;  

    util_memcpy((void *)&phy_info_ptr_1, phy_info_ptr, sizeof(phy_info_t));
    _get_cfg_list_array(cur_port_config_ptr, cur_mode_parameter_ptr, &cfg_list);    

    pam4_pmon_stat_ptr->pam4_pmon_acc_odd = 0;
    pam4_pmon_stat_ptr->pam4_pmon_acc_even = 0;
    pam4_pmon_stat_ptr->pam4_oflow_odd = 0;
    pam4_pmon_stat_ptr->pam4_oflow_even = 0;
    pam4_pmon_stat_ptr->pam4_pmon_lock_flag = 1;
    pam4_pmon_stat_ptr->pam4_pmon_lol_flag = 0;

    for(wr_idx=cfg_list.lst_beg; wr_idx<=cfg_list.lst_end; wr_idx++){
        pam4_pmon_stat_t pam4_pmon_stat = {0};
        
        phy_info_ptr_1.base_addr = chal_cw_rtmr_fec_prbs_get_base_addr(phy_info_ptr, cur_mode_parameter_ptr) +
            fec_prbs[egr_or_igr][wr_idx]; 
        chal_cw_pmon_get_status (&phy_info_ptr_1, &pam4_pmon_stat);

        pam4_pmon_stat_ptr->pam4_pmon_acc_odd += pam4_pmon_stat.pam4_pmon_acc_odd;
        pam4_pmon_stat_ptr->pam4_pmon_acc_even += pam4_pmon_stat.pam4_pmon_acc_even;
        pam4_pmon_stat_ptr->pam4_oflow_odd |= pam4_pmon_stat.pam4_oflow_odd;
        pam4_pmon_stat_ptr->pam4_oflow_even |= pam4_pmon_stat.pam4_oflow_even;
        pam4_pmon_stat_ptr->pam4_pmon_lock_flag &= pam4_pmon_stat.pam4_pmon_lock_flag;
        pam4_pmon_stat_ptr->pam4_pmon_lol_flag |= pam4_pmon_stat.pam4_pmon_lol_flag;
    }
}

/**
 * @brief  chal_cw_rtmr_fec_pmon_clr_all( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
 * @detail Function that clears the PMON Accumulators, overflow flags, LOL flag corresponding to the speed  
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in]  cur_port_config_ptr: port config pointer
 * @param[in]  cur_mode_parameter_ptr : pointer to mode parameter structure 
 * @return    void:
*/
void chal_cw_rtmr_fec_pmon_clr_all( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
{
    uint8_t wr_idx;
    _fec_prbs_cfg_list_t cfg_list;  
    phy_info_t phy_info_ptr_1;

    util_memcpy((void *)&phy_info_ptr_1, phy_info_ptr, sizeof(phy_info_t)); 

    _get_cfg_list_array(cur_port_config_ptr, cur_mode_parameter_ptr, &cfg_list);

    for(wr_idx=cfg_list.lst_beg; wr_idx<=cfg_list.lst_end; wr_idx++){
        phy_info_ptr_1.base_addr = chal_cw_rtmr_fec_prbs_get_base_addr(phy_info_ptr, cur_mode_parameter_ptr) +
            fec_prbs[egr_or_igr][wr_idx]; 
        chal_cw_pmon_clr_all(&phy_info_ptr_1);
    }
}

/**
 * @brief  chal_cw_rtmr_fec_pgen_get_active_status( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
 * @detail Function that gets the PGEN active status corresponding to the speed  
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in]  cur_port_config_ptr: port config pointer
 * @param[in]  cur_mode_parameter_ptr : pointer to mode parameter structure 
 * @return    uint8_t:
*/
uint8_t chal_cw_rtmr_fec_pgen_get_active_status( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
{
    uint8_t wr_idx;
    _fec_prbs_cfg_list_t cfg_list;  
    phy_info_t phy_info_ptr_1;
    uint8_t active_stat=1;    

    util_memcpy((void *)&phy_info_ptr_1, phy_info_ptr, sizeof(phy_info_t)); 
    _get_cfg_list_array(cur_port_config_ptr, cur_mode_parameter_ptr, &cfg_list);

    for(wr_idx=cfg_list.lst_beg; wr_idx<=cfg_list.lst_end; wr_idx++){
        phy_info_ptr_1.base_addr = chal_cw_rtmr_fec_prbs_get_base_addr(phy_info_ptr, cur_mode_parameter_ptr) +
            fec_prbs[egr_or_igr][wr_idx]; 
        ERR_HSIP(active_stat &= hsip_rd_field_(&phy_info_ptr_1, QC_PGEN_CTRL0_REGISTER, PGEN_MD_ACTIVE_FLAG));
    }

  return active_stat;

}

/**
 * @brief  chal_cw_rtmr_fec_pmon_get_active_status( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
 * @detail Function that gets the PGEN active status corresponding to the speed  
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in]  cur_mode_parameter_ptr : pointer to mode parameter structure 
 * @param[in]  cur_port_config_ptr: port config pointer
 * @return    uint8_t:
*/
uint8_t chal_cw_rtmr_fec_pmon_get_active_status( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
{

    uint8_t wr_idx;
    _fec_prbs_cfg_list_t cfg_list;  
    phy_info_t phy_info_ptr_1;
    uint8_t active_stat=1;    

    util_memcpy((void *)&phy_info_ptr_1, phy_info_ptr, sizeof(phy_info_t)); 
    _get_cfg_list_array(cur_port_config_ptr, cur_mode_parameter_ptr, &cfg_list);

    for(wr_idx=cfg_list.lst_beg; wr_idx<=cfg_list.lst_end; wr_idx++){
        phy_info_ptr_1.base_addr = chal_cw_rtmr_fec_prbs_get_base_addr(phy_info_ptr, cur_mode_parameter_ptr) +
            fec_prbs[egr_or_igr][wr_idx]; 
        ERR_HSIP(active_stat &= (uint8_t)hsip_rd_field_(&phy_info_ptr_1, QC_PMON_CTRL0_REG, PMON_MD_ACTIVE_FLAG));
    }

    return active_stat;
}

/**
 * @brief  chal_cw_rtmr_pgen_get_poly( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr,  cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
 * @detail Function that gets the PGEN poly sel 
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in]  lane_num : lane number of PGEN instance for which we want to get the poly 
 * @return    uint8_t:
*/
uint8_t chal_cw_rtmr_pgen_get_poly( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
{
    _fec_prbs_cfg_list_t cfg_list;  
    phy_info_t phy_info_ptr_1;
     uint8_t poly; 

    util_memcpy((void *)&phy_info_ptr_1, phy_info_ptr, sizeof(phy_info_t)); 
    _get_cfg_list_array(cur_port_config_ptr, cur_mode_parameter_ptr, &cfg_list);

    phy_info_ptr_1.base_addr = chal_cw_rtmr_fec_prbs_get_base_addr(phy_info_ptr, cur_mode_parameter_ptr) +
        fec_prbs[egr_or_igr][cfg_list.val_idx]; 
    ERR_HSIP(poly = (uint8_t)hsip_rd_field_(&phy_info_ptr_1, QC_PGEN_CTRL0_REGISTER, MD_PGEN_POLY_SELECT));
    return poly;
}

/**
 * @brief  chal_cw_rtmr_pmon_get_poly( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_ig, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
 * @detail Function that gets the pmon poly sel for the specified lane
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in]  lane_num : lane number of pmon instance for which we want to get the poly 
 * @return    uint8_t:
*/
uint8_t  chal_cw_rtmr_pmon_get_poly( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
{
     uint8_t poly; 
     phy_info_t phy_info_ptr_1;
     _fec_prbs_cfg_list_t cfg_list;  

     util_memcpy((void *)&phy_info_ptr_1, phy_info_ptr, sizeof(phy_info_t)); 
    _get_cfg_list_array(cur_port_config_ptr, cur_mode_parameter_ptr, &cfg_list);

    phy_info_ptr_1.base_addr = chal_cw_rtmr_fec_prbs_get_base_addr(phy_info_ptr, cur_mode_parameter_ptr) +
        fec_prbs[egr_or_igr][cfg_list.val_idx]; 
     ERR_HSIP(poly   = (uint8_t)hsip_rd_field_(&phy_info_ptr_1,  QC_PMON_CTRL0_REG, MD_PMON_POLY_SELECT));
           
     return poly;
}

/**
 * @brief  chal_cw_rtmr_pgen_get_inv( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr,  cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
 * @detail Function that gets the PGEN DOUT INV 
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in]  lane_num : lane number of PGEN instance for which we want to get the poly 
 * @return    uint8_t:
*/

uint8_t chal_cw_rtmr_pgen_get_inv( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
{
    _fec_prbs_cfg_list_t cfg_list;  
    phy_info_t phy_info_ptr_1;
     uint8_t tx_inv; 

    util_memcpy((void *)&phy_info_ptr_1, phy_info_ptr, sizeof(phy_info_t)); 
    _get_cfg_list_array(cur_port_config_ptr, cur_mode_parameter_ptr, &cfg_list);

    phy_info_ptr_1.base_addr = chal_cw_rtmr_fec_prbs_get_base_addr(phy_info_ptr, cur_mode_parameter_ptr) +
        fec_prbs[egr_or_igr][cfg_list.val_idx]; 
    ERR_HSIP(tx_inv = hsip_rd_field_(&phy_info_ptr_1, QC_PGEN_CFG0_REGISTER, MD_PGEN_DOUT_INV_ENA));
    return tx_inv;
}

/**
 * @brief  chal_cw_rtmr_pmon_get_inv( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr,  cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
 * @detail Function that gets the PMON DIN INV 
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  cfg_egr_or_igr_t :egr or igr 
 * @param[in]  lane_num : lane number of PGEN instance for which we want to get the poly 
 * @return    uint8_t:
*/
uint8_t chal_cw_rtmr_pmon_get_inv( phy_info_t* phy_info_ptr, cfg_egr_or_igr_t egr_or_igr, cw_port_config_t* cur_port_config_ptr, cw_mode_parameter_t* cur_mode_parameter_ptr)
{

    _fec_prbs_cfg_list_t cfg_list;  
    phy_info_t phy_info_ptr_1;
     uint8_t rx_inv; 

    util_memcpy((void *)&phy_info_ptr_1, phy_info_ptr, sizeof(phy_info_t)); 
    _get_cfg_list_array(cur_port_config_ptr, cur_mode_parameter_ptr, &cfg_list);

    phy_info_ptr_1.base_addr = chal_cw_rtmr_fec_prbs_get_base_addr(phy_info_ptr, cur_mode_parameter_ptr) +
        fec_prbs[egr_or_igr][cfg_list.val_idx]; 
    ERR_HSIP(rx_inv = hsip_rd_field_(&phy_info_ptr_1, QC_PMON_CFG0_REG, MD_PMON_DIN_INV_ENA));
    return rx_inv;
}


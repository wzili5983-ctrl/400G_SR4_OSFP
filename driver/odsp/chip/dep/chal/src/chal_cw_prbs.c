/**
 *
 * @file     chal_cw_prbs.c
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
#include "regs_common.h"
#include "chip_mode_def.h"
#include "cw_def.h"
#include "common_util.h"
#include "pam4_prbs_def.h"
#include "chal_cw_prbs.h"

//==========================================================================
//~~                            PGEN Functions
//==========================================================================


/**
 * @brief      chal_cw_pgen_read_lock (phy_info_t* phy_info_ptr, uint8_t* lock_flag)
 * @detail     Read PGEN lock flag
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr : Device physical information pointer
 * @return     lock_flag     : 0 => Not Locked, 1=> Locked
 */
return_result_t chal_cw_pgen_read_lock (phy_info_t* phy_info_ptr, uint8_t* lock_flag)
{
    ERR_HSIP(*lock_flag = (uint8_t)hsip_rd_field_(phy_info_ptr, QC_PGEN_CTRL0_REGISTER, PGEN_MD_ACTIVE_FLAG));
    return RR_SUCCESS;
}

/**
 * @brief      chal_cw_pgen_enable (phy_info_t* phy_info_ptr, onoff_t onoff)
 * @detail     Enable/Disable the PGEN block.
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  onoff        :  ON => Turn on PGEN and enable it, OFF => Turn off PGEN
 * @return     lock_flag    :  0 => PGEN not locked, 1 => PGEN Locked
 */
uint8_t chal_cw_pgen_enable (phy_info_t* phy_info_ptr, onoff_t onoff)
{
    uint8_t lock_flag;
    if (onoff == OFF) {
        hsip_wr_field_(phy_info_ptr, QC_PGEN_CTRL0_REGISTER, MD_PGEN_ENABLE,  0x0); // Disable PGEN
    } else {
        hsip_wr_field_(phy_info_ptr, QC_PGEN_CTRL0_REGISTER, MD_PGEN_ENABLE,  0x1); // Disable PGEN. Same bit controls enable, clk_ena and sw_rstn
    }
    chal_cw_pgen_read_lock(phy_info_ptr, &lock_flag); // Check to see that PGEN is locked (should happen right away)

    return lock_flag;
}

/**
 * @brief      chal_cw_pgen_init (phy_info_t* phy_info_ptr)
 * @detail     Initialize the PGEN block (places the PGEN registers into their power-up reset states). Basic config for raw prbs mode
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @return     void
 */
void chal_cw_pgen_init (phy_info_t* phy_info_ptr)
{
    hsip_wr_reg_(phy_info_ptr, QC_PGEN_CTRL0_REGISTER, 0x01f0); // enable=0, Poly=PRBS31
    hsip_wr_reg_(phy_info_ptr,  QC_PGEN_CFG0_REGISTER, 0x2000); // pam mode, non fec mode, no fault on prbs 
    hsip_wr_reg_(phy_info_ptr,  QC_PGEN_DBG0_REGISTER, 0x0000); // Don't Generate Any Err-ors

}


/**
 * @brief  chal_cw_pgen_config (phy_info_t* phy_info_ptr, pam4_pgen_misc_cfg_t* pam4_pgen_misc_cfg_ptr)
 * @detail Configure PRBS Generator, FEC mode, data converter, prbs on fault etc. .
 *    Note: you may want to call chal_cw_pgen_config_pbi() after calling this.
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in] pam4_pgen_misc_cfg_ptr: structure containing config values. 

 */
void chal_cw_pgen_config (phy_info_t* phy_info_ptr, pam4_pgen_misc_cfg_t* pam4_pgen_misc_cfg_ptr)
{
    uint8_t seed_id = (phy_info_ptr->base_addr >> 8);

    //Make sure to call chal_cw_pgen_lane_prbs_clk_cfg incase of Lane PRBS. 
    //Make sure to call chal_cw_pgen_fec_prbs_clk_cfg incase of FEC PRBS 
    // Optionally Initialize PGEN First. Note that this sets POLY31, PAM Mode, Non-FEC Mode , no fault on PRBS
    if (pam4_pgen_misc_cfg_ptr->pgen_init) chal_cw_pgen_init(phy_info_ptr);

    // Configure PGEN
    hsip_wr_field_(phy_info_ptr, QC_PGEN_CTRL0_REGISTER, MD_PGEN_POLY_SELECT, (0x3f & pam4_pgen_misc_cfg_ptr->polysel) ); // Set poly select
    hsip_wr_field_(phy_info_ptr, QC_PGEN_CFG0_REGISTER, MD_PGEN_DOUT_INV_ENA, (0x1  & pam4_pgen_misc_cfg_ptr->pgen_inv)); // Set/Clear Dout Invert
    //hsip_wr_field_(phy_info_ptr, QC_PGEN_CFG0_REGISTER, MD_PGEN_SEED_ID,   pam4_pgen_cfg_ptr->pam4_pgen_seed_id);
    hsip_wr_field_(phy_info_ptr, QC_PGEN_CFG0_REGISTER, MD_PGEN_SEED_ID,   seed_id);

    if (pam4_pgen_misc_cfg_ptr->fec_mode_ena == OFF) {
        hsip_wr_field_(phy_info_ptr, QC_PGEN_CFG0_REGISTER, MD_PGEN_FEC_MODE_ENA, 0x0); // Disable FEC PRBS 
        //Below removed as it is not perlane. Will be called in some other place. call chal_cw_pgen_lane_prbs_clk_cfg
        //hsip_wr_field_(phy_info_ptr,    LANE_PRBS_CLK_RST_MUX_REG,MD_TMT_LANE_PRBS_MODE_LN, 0x1); //Select lanewise clock . 8 bits for 8 lanes. in fec_pcs_wrap_clkrst.rdb 
        //datamux sel config is automatically derived from hw incase of lane prbs mode 
    } else {
        hsip_wr_field_(phy_info_ptr, QC_PGEN_CFG0_REGISTER, MD_PGEN_FEC_MODE_ENA, 0x1); // Enable FEC PRBS
        //Below removed as it is not perlane. Will be called in some other place.call chal_cw_pgen_fec_prbs_clk_cfg 
        //hsip_wr_field_(phy_info_ptr,    LANE_PRBS_CLK_RST_MUX_REG,MD_TMT_LANE_PRBS_MODE_LN, 0x0); //Select bundled clock for fec prbs. 8 bits for 8 lanes. in fec_pcs_wrap_clkrst.rdb 
        //Below line may not be needed. should be part of mode config
         //hsip_wr_field_(phy_info_ptr,QUAD_CORE_EGRMGT_RDB_LINE_IFC_CTRL_3_RDB,MD_LINE_TMT_RTM_RPTB , 0x1); // In quad_core_egrmgt_reg.rdb- > datamux sel to send retimer data
    }

    if (pam4_pgen_misc_cfg_ptr->prbs_on_fault_ena == OFF) {
        hsip_wr_field_(phy_info_ptr, QC_PGEN_CFG0_REGISTER, MD_PGEN_FAULT_MODE_ENA, 0x0); // Disable PRBS on fault
    } else {
        if (pam4_pgen_misc_cfg_ptr->fec_mode_ena == OFF) { //Dont support prbs on fault in FEC modes. 
        hsip_wr_field_(phy_info_ptr, QC_PGEN_CFG0_REGISTER, MD_PGEN_FAULT_MODE_ENA, 0x1); // Enable PRBS on fault
     }
    }

    if (pam4_pgen_misc_cfg_ptr->pam4_mode == OFF) {
        hsip_wr_field_(phy_info_ptr, QC_PGEN_CFG0_REGISTER, MD_PGEN_1PAM4_0NRZ, 0x0); // NRZ Mode 
        hsip_wr_field_(phy_info_ptr, QC_PGEN_CFG0_REGISTER, MD_PGEN_0LOW_1HIGH_2EVENODD, pam4_pgen_misc_cfg_ptr->nrz_ctrl);     //Specify which 40 bits in 80 bit bus should contatin NRZ data 
    } else {
        hsip_wr_field_(phy_info_ptr, QC_PGEN_CFG0_REGISTER, MD_PGEN_1PAM4_0NRZ, 0x1); // PAM Mode
    }
}


/**
 * @brief  return_result_t chal_cw_pmon_enable (phy_info_t* phy_info_ptr, onoff_t onoff)
 * @detail Enable/Disable the PMON block.
 * @publics
 * @private
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in]  onoff        :  ON => Turn on PMON and enable it, OFF => Turn off PMON
 * @return     void
 */

return_result_t chal_cw_pmon_enable (phy_info_t* phy_info_ptr, onoff_t onoff)
{
    uint16_t reg_val;
    
    ERR_HSIP(reg_val =  (uint16_t)hsip_rd_reg_(phy_info_ptr,    QC_PMON_CTRL0_REG));
    hsip_set_field_(phy_info_ptr,    QC_PMON_CTRL0_REG, MD_PMON_ENABLE, reg_val, (onoff == OFF)?0:1);
    hsip_set_field_(phy_info_ptr,    QC_PMON_CTRL0_REG, PMON_LOL_IRQ_STAT, reg_val, 0);
    hsip_update_reg_(phy_info_ptr, QC_PMON_CTRL0_REG, reg_val);
    return RR_SUCCESS;
}


/**
 * @brief  void chal_cw_pmon_init (phy_info_t* phy_info_ptr)
 * @detail Initialize the PMON block (places the PMON registers into their power-up reset states)
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @return    void:
 */
void chal_cw_pmon_init (phy_info_t* phy_info_ptr)
{
    // Initialize PMON Top Level Regs
    hsip_wr_reg_(phy_info_ptr, QC_PMON_CTRL0_REG, 0x01f0); // enable=0,Poly=PRBS31, Do not freeze BERR ACC values, 
    hsip_wr_reg_(phy_info_ptr, QC_PMON_CFG0_REG, 0x2000); // Disable automatic format detect mode, do not invert/gray/swap din, raw pmon mode, PAM input
    //hsip_wr_reg_(phy_info_ptr, QC_PMON_FMT0, 0x0000); //  RO - PMON Auto Format Detect, detected poly, detected dinv_stat, format autodetect stat
    hsip_wr_reg_(phy_info_ptr, QC_PMON_LOCK0_REG, 0x0002); // md_pmon_lock_thr = 2, PMON will not relock when an LOL occurs
    hsip_wr_reg_(phy_info_ptr, QC_PMON_UNLOCK0_REG, 0x8010); // lol_irq_mask=1, Unlock_Thr=0 (disabled), Unlock_Tol=16
    hsip_wr_reg_(phy_info_ptr, QC_PMON_PBI0_REG,0x0530 );   // PBI/DC balance: lock_wdw=5, lock_thr=3,  ena=0
    hsip_wr_reg_(phy_info_ptr, QC_PMON_DBG0_REG,0x0000); // No Errors     
    hsip_wr_reg_(phy_info_ptr, QC_PMON_CHK_CSR0_REG, 0x0000); // Hardware controls bit error accumulators, md_pmon_berr_clr = 0, md_pmon_berr_oflow_clr = 0
    hsip_wr_reg_(phy_info_ptr, QC_PMON_CHK_CSR1_REG, 0x0000); //32-Bit Even Bit Errors,  Odd = 32-Bit Odd Bit Errors , md_pmon_burst_err_wdw =0, md_pmon_burst_err_thr =0
    
    //hsip_wr_reg_(phy_info_ptr, QC_PMON_CHK_BERO_LO_REG, 0x0000); // RO - Odd BER ACC Value Low
    //hsip_wr_reg_(phy_info_ptr, QC_PMON_CHK_BERO_HI_REG, 0x0000); // RO - Odd BER ACC Value High
    //hsip_wr_reg_(phy_info_ptr, QC_PMON_CHK_BERE_LO_REG, 0x0000); // RO - Even BER ACC Value Low
    //hsip_wr_reg_(phy_info_ptr, QC_PMON_CHK_NERE_HI_REG, 0x0000); // RO - Even BER ACC Value High
    
    //timer removed. No timer controls 
}


/**
 * @brief  chal_cw_pmon_config (phy_info_t* phy_info_ptr, pam4_pmon_misc_cfg_t* pam4_pmon_misc_cfg_ptr)
 * @detail Configure PMON, data converter, FEC mode etc, 
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @param[in] pam4_pmon_misc_cfg_ptr: pointer to pmon misc config structure 
 * @return void
 */
return_result_t chal_cw_pmon_config (phy_info_t* phy_info_ptr, pam4_pmon_misc_cfg_t* pam4_pmon_misc_cfg_ptr)
{
    //Make sure to call chal_cw_pmon_lane_prbs_clk_cfg in case of lane prbs 
    //Make sure to call chal_cw_pmon_fec_prbs_clk_cfg in case of FEC PRBS 
    uint16_t reg_val;

    // Optionally Initialize PMON RDB registers
    if (pam4_pmon_misc_cfg_ptr->pmon_init) {
      chal_cw_pmon_init(phy_info_ptr);
    }

    // Set Basic PMON Configuration
    //hsip_wr_field_(phy_info_ptr, QC_PMON_CTRL0_REG, MD_PMON_POLY_SELECT,   0x3f & pam4_pmon_misc_cfg_ptr->polysel  );
    ERR_HSIP(reg_val =  (uint16_t)hsip_rd_reg_(phy_info_ptr,    QC_PMON_CTRL0_REG));
    hsip_set_field_(phy_info_ptr,    QC_PMON_CTRL0_REG, MD_PMON_POLY_SELECT, reg_val, (0x3f & pam4_pmon_misc_cfg_ptr->polysel));
    hsip_set_field_(phy_info_ptr,    QC_PMON_CTRL0_REG, PMON_LOL_IRQ_STAT, reg_val, 0);
    hsip_update_reg_(phy_info_ptr, QC_PMON_CTRL0_REG, reg_val);
    hsip_wr_field_(phy_info_ptr, QC_PMON_CFG0_REG, MD_PMON_DIN_INV_ENA,    0x01 & pam4_pmon_misc_cfg_ptr->pmon_inv );

    if (pam4_pmon_misc_cfg_ptr->fec_mode_ena == OFF) {
        hsip_wr_field_(phy_info_ptr,QC_PMON_CFG0_REG, MD_PMON_FEC_MODE_ENA, 0x0);//Non FEC PRBS mode i.e. raw pmon 
         //below moved as it is not lane wise register. call chal_cw_pmon_lane_prbs_clk_cfg elsewhere
         //hsip_wr_field_(phy_info_ptr,    LANE_PRBS_CLK_RST_MUX_REG,MD_RCV_LANE_PRBS_MODE_LN, 0x1); //Select lanewise clock . 8 bits for 8 lanes. in fec_pcs_wrap_clkrst.rdb 
     } else {
        hsip_wr_field_(phy_info_ptr,QC_PMON_CFG0_REG, MD_PMON_FEC_MODE_ENA, 0x1);// FEC PRBS mode 
        //below moved as it is not lane wise register. call chal_cw_pmon_fec_prbs_clk_cfg elsewhere
        //hsip_wr_field_(phy_info_ptr,    LANE_PRBS_CLK_RST_MUX_REG,MD_RCV_LANE_PRBS_MODE_LN, 0x0); //Select bundled clock . 8 bits for 8 lanes. in fec_pcs_wrap_clkrst.rdb 
    }

    if (pam4_pmon_misc_cfg_ptr->pam4_mode == OFF) {
        hsip_wr_field_(phy_info_ptr, QC_PMON_CFG0_REG, MD_PMON_1PAM4_0NRZ, 0x0); // NRZ Mode 
        hsip_wr_field_(phy_info_ptr, QC_PMON_CFG0_REG, MD_PMON_0LOW_1HIGH_2EVEN_3ODD, pam4_pmon_misc_cfg_ptr->nrz_ctrl);     //Specify which 40 bits in 80 bit bus should contatin NRZ data 
    } else {
        hsip_wr_field_(phy_info_ptr, QC_PMON_CFG0_REG, MD_PMON_1PAM4_0NRZ, 0x1); // PAM Mode
    }
    hsip_wr_field_(phy_info_ptr, QC_PMON_UNLOCK0_REG, MD_PMON_UNLOCK_THR, 1); // Set Unlock Threshold
    hsip_wr_field_(phy_info_ptr, QC_PMON_LOCK0_REG, MD_PMON_RELOCK_ENA, 1); // Set Relock Enable
    return RR_SUCCESS;
}


/**
 * @brief  return_result_t chal_cw_pmon_get_status (phy_info_t* phy_info_ptr, pam4_pmon_stat_t* pam4_pmon_stat_ptr)
 * @detail Get PAM4 PMON Status
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr      : Device physical information pointer
 * @param[in] pam4_pmon_stat_ptr : Pointer to PAM4 PMON Config Type
 * @return return_result
*/
return_result_t chal_cw_pmon_get_status (phy_info_t* phy_info_ptr, pam4_pmon_stat_t* pam4_pmon_stat_ptr)
{
    uint32_t acc_odd, acc_even;
    uint16_t rdata_lo, rdata_hi;

    // read lock and LOL first
    ERR_HSIP(rdata_lo = hsip_rd_field_(phy_info_ptr, QC_PMON_CTRL0_REG, PMON_MD_LOCK_FLAG));    // Lock Flag
    ERR_HSIP(rdata_hi = hsip_rd_field_(phy_info_ptr, QC_PMON_CTRL0_REG, PMON_LOL_IRQ_STAT)); // LOL Flag
    pam4_pmon_stat_ptr->pam4_pmon_lock_flag = rdata_lo & 0x0001;
    pam4_pmon_stat_ptr->pam4_pmon_lol_flag  = rdata_hi & 0x0001;

    //chal_cw_pmon_stat_updt(phy_info_ptr); // Update status
    hsip_wr_field_(phy_info_ptr, QC_PMON_CTRL0_REG, MD_PMON_BERR_STAT_FREEZE, 1);

    ERR_HSIP(rdata_lo = hsip_rd_field_(phy_info_ptr, QC_PMON_CHK_BERO_LO_REG, PMON_MD_BERR_VALUE_ODD_LO)); // Odd/Burst Err-ors
    ERR_HSIP(rdata_hi = hsip_rd_field_(phy_info_ptr, QC_PMON_CHK_BERO_HI_REG, PMON_MD_BERR_VALUE_ODD_HI));
    acc_odd = (rdata_hi << 16) | rdata_lo;
    pam4_pmon_stat_ptr->pam4_pmon_acc_odd = acc_odd;

    ERR_HSIP(rdata_lo = hsip_rd_field_(phy_info_ptr, QC_PMON_CHK_BERE_LO_REG, PMON_MD_BERR_VALUE_EVEN_LO)); // Even/Total Err-ors
    ERR_HSIP(rdata_hi = hsip_rd_field_(phy_info_ptr, QC_PMON_CHK_NERE_HI_REG, PMON_MD_BERR_VALUE_EVEN_HI));
    acc_even = (rdata_hi << 16) | rdata_lo;
    pam4_pmon_stat_ptr->pam4_pmon_acc_even = acc_even;

    ERR_HSIP(pam4_pmon_stat_ptr->pam4_oflow_even = hsip_rd_field_(phy_info_ptr, QC_PMON_CHK_CSR0_REG, PMON_MD_BERR_OFLOW_EVEN)); // LSB BER Acc Overflow
    ERR_HSIP(pam4_pmon_stat_ptr->pam4_oflow_odd = hsip_rd_field_(phy_info_ptr, QC_PMON_CHK_CSR0_REG, PMON_MD_BERR_OFLOW_ODD)); // MSB BER Acc Overflow

    hsip_wr_field_(phy_info_ptr, QC_PMON_CTRL0_REG, MD_PMON_BERR_STAT_FREEZE, 0);
    return RR_SUCCESS; 
}


/**
 * @brief  void chal_cw_pmon_clr_accs (phy_info_t* phy_info_ptr)
 * @detail Clear sticky PMON bit err-or accumulators (and their overflow flags)
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @return    void:
*/
void chal_cw_pmon_clr_accs (phy_info_t* phy_info_ptr)
{
    hsip_wr_field_(phy_info_ptr, QC_PMON_CHK_CSR0_REG, MD_PMON_BERR_OFLOW_CLR, 0x1);
    hsip_wr_field_(phy_info_ptr, QC_PMON_CHK_CSR0_REG, MD_PMON_BERR_CLR, 0x1);
    hsip_wr_field_(phy_info_ptr, QC_PMON_CHK_CSR0_REG, MD_PMON_BERR_OFLOW_CLR, 0x0);
    hsip_wr_field_(phy_info_ptr, QC_PMON_CHK_CSR0_REG, MD_PMON_BERR_CLR, 0x0);
}

/**
 * @brief  void chal_cw_pmon_clr_lol (phy_info_t* phy_info_ptr)
 * @detail Clear sticky PMON loss-of-lock flag/IRQ
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @return    void:
*/
void chal_cw_pmon_clr_lol (phy_info_t* phy_info_ptr)
{
    hsip_wr_field_(phy_info_ptr, QC_PMON_CTRL0_REG, PMON_LOL_IRQ_STAT, 0x1); // W1C
    hsip_wr_field_(phy_info_ptr, QC_PMON_CTRL0_REG, PMON_LOL_IRQ_STAT, 0x0);
}

/**
 * @brief  void chal_cw_pmon_clr_all (phy_info_t* phy_info_ptr)
 * @detail Clear all PMON err-ors and sticky flags
 * @public 
 * @private 
 * 
 * @param[in]  phy_info_ptr :  Device physical information pointer
 * @return    void:
*/
void chal_cw_pmon_clr_all (phy_info_t* phy_info_ptr)
{
    chal_cw_pmon_clr_accs(phy_info_ptr); // Also clears sticky overflow flags
    chal_cw_pmon_clr_lol(phy_info_ptr);
}

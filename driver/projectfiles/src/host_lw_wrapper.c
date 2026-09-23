/**
 *
 * @file    host_lw_wrapper.c
 * @author  Firmware Team
 * @date    4/30/2019
 * @version 0.1
 *
 * @property 	
 * $Copyright: (c) 2019 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * @brief    brief description of the block source file, sequences/flow/etc.
 *
 * @section  description
 *
 */

/** @file host_lw_wrapper.c
 *  cAPI chal warpper implementation
 */
#include "access.h"
#include "regs_common.h"
#include "common_def.h"
#include "chip_mode_def.h"
#include "capi_def.h"
#include "capi_test_def.h"
#include "capi_custom_def.h"
#include "host_lw_wrapper.h"
#include "dsp_utils.h"
#include "common_util.h"
#include "chip_common_config_ind.h"
#include "lw_common_config_ind.h"
#include "hr_time.h"
#include "capi.h"

/**
 * @brief    host_lw_get_lane_modulation(capi_phy_info_t*   phy_info_ptr,
 *                                       uint8_t            lane_index,
 *                                       capi_modulation_t* modulation_ptr)
 * @details  This API is used to get FFE slicer histogram
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  lane_index: Lane ID
 * @param[out] modulation_ptr: lane modulation
  * @return     returns the result of the called method/function, RR_SUCCESS
 */
return_result_t host_lw_get_lane_modulation(capi_phy_info_t*   phy_info_ptr,
                                            uint8_t            lane_index,
                                            capi_modulation_t* modulation_ptr)
{
    capi_phy_info_t phy_info_cfg;

    lw_util_init_lane_config_base_addr(phy_info_ptr, &phy_info_cfg, lane_index);
    ERR_HSIP(*modulation_ptr = (capi_modulation_t)hsip_rd_field_(&phy_info_cfg, COMMON_LW_LANE_CONFIG17_REG, MODULATION));
    (void)lane_index;
    return RR_SUCCESS;
}


/**
 * @brief host_lw_tx_fir_4tap_prog_256_lut (phy_info_t* phy_info_ptr, int8_t *lut_val)
 * @details     Program TXFIR 256 LUT with the given 256 entries
 * @public
 * @private
 *
 *
 * @param[in]  phy_info_ptr         device info pointer
 * @param[in]  *lut_val             256 entries of TXFIR LUT value, the value should be 7 bit signed [-64, 63]
 * @return enum return_result_t
 */
return_result_t host_lw_tx_fir_4tap_prog_256_lut (phy_info_t* phy_info_ptr, int8_t *lut_val)
{
    return_result_t return_result = RR_SUCCESS;
    int i;


    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_4T_CONTROL_0_REG, LUT_4T_AUTO_ADR_EN, 0x0);

    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_4T_CONTROL_0_REG, LUT_4T_ADR,  0x0);

    // Toggle to reset addres back to 0  
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_4T_CONTROL_0_REG, LUT_4T_AUTO_ADR_EN,  0x0 );
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_4T_CONTROL_0_REG, LUT_4T_AUTO_ADR_EN,  0x1 );

    for ( i = 0; i < 256; i++ ) {
      int16_t val = lut_val[i];
#if 0
      /* DAC value range is [0, 127] */
      if ( val > 63 ) {
          val = 63;
          return_result = RR_WARNING_BOUNDS;
      }
      else if ( val < -64 ) {
          val = -64;
          return_result = RR_WARNING_BOUNDS;            
      }    
#endif
      hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_4T_WDAT_REG, LUT_4T_WDAT, val);
    }
    return return_result;
}

/**
 * @brief host_lw_tx_fir_4tap_read_256_lut (phy_info_t* phy_info_ptr,  int8_t *lut_val)
 * @details     Read TXFIR 256 LUT values from chip
 * @public
 * @private
 *
 *
 * @param[in]  phy_info_ptr         device info pointer
 * @param[out]  *lut_val            256 entries of TXFIR LUT value read from chip
 * @return enum return_result_t
 */
return_result_t host_lw_tx_fir_4tap_read_256_lut (phy_info_t* phy_info_ptr,  int8_t *lut_val)
{
    int i;
  
    // Toggle to reset addres back to 0
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_4T_CONTROL_0_REG, LUT_4T_AUTO_ADR_EN,  0x0);
  
    for ( i = 0; i < 256; i++ ) {
      hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_4T_CONTROL_0_REG, LUT_4T_ADR,  i);
      lut_val[i] = (int8_t) hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_4T_RDAT_REG, 8, 0);
    }
    return RR_SUCCESS;
}
/**
 * @brief host_lw_tx_fir_set_floating_13tap (phy_info_t* phy_info_ptr,  capi_txfir_nonlinear_13tap_table_info_t *txfir_tbl_ptr)
 * @details     Read TXFIR 256 LUT values from chip
 * @public
 * @private
 *
 *
 * @param[in]  phy_info_ptr         device info pointer
 * @param[out]  *lut_val            256 entries of TXFIR LUT value read from chip
 * @return enum return_result_t
 */
return_result_t host_lw_tx_fir_set_floating_13tap (phy_info_t* phy_info_ptr,  capi_txfir_nonlinear_13tap_table_info_t *txfir_tbl_ptr)
{
   return_result_t  return_result = RR_SUCCESS;
    /*    tap[0~9]  => 10_tap interface: 3pre+3post+4main + 3floating
                 tap[3~6]  => 4_tap interface
                 tap[10~12]  => float_tap interface
    */
    hsip_wr_field_(phy_info_ptr, LWTX_CLK_RST_LWTX_CLOCK_CONTROL_0_REG, MD_TXG_FIR_7TAP_CKE , 0x1 );
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_RESET_0_REG, TAP10_RSTB,  0x1 );

    //program tap0~2
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_0_REG, LUT_10TAP_0_LVL3,  txfir_tbl_ptr->tap0[3]);
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_1_REG, LUT_10TAP_0_LVL2,  txfir_tbl_ptr->tap0[2]);
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_2_REG, LUT_10TAP_0_LVL1,  txfir_tbl_ptr->tap0[1]);
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_3_REG, LUT_10TAP_0_LVL0,  txfir_tbl_ptr->tap0[0]);

    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_4_REG, LUT_10TAP_1_LVL3,  txfir_tbl_ptr->tap1[3]);
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_5_REG, LUT_10TAP_1_LVL2,  txfir_tbl_ptr->tap1[2]);
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_6_REG, LUT_10TAP_1_LVL1,  txfir_tbl_ptr->tap1[1]);
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_7_REG, LUT_10TAP_1_LVL0,  txfir_tbl_ptr->tap1[0]);

    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_8_REG, LUT_10TAP_2_LVL3,  txfir_tbl_ptr->tap2[3]);
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_9_REG, LUT_10TAP_2_LVL2,  txfir_tbl_ptr->tap2[2]);
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_10_REG, LUT_10TAP_2_LVL1, txfir_tbl_ptr->tap2[1]);
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_11_REG, LUT_10TAP_2_LVL0, txfir_tbl_ptr->tap2[0]);

    //clear tap 3~6
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_12_REG, LUT_10TAP_3_LVL3, 0 );
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_13_REG, LUT_10TAP_3_LVL2, 0 );
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_14_REG, LUT_10TAP_3_LVL1, 0 );
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_15_REG, LUT_10TAP_3_LVL0, 0 );

    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_16_REG, LUT_10TAP_4_LVL3, 0 );
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_17_REG, LUT_10TAP_4_LVL2, 0 );
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_18_REG, LUT_10TAP_4_LVL1, 0 );
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_19_REG, LUT_10TAP_4_LVL0, 0 );

    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_20_REG, LUT_10TAP_5_LVL3, 0 );
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_21_REG, LUT_10TAP_5_LVL2, 0 );
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_22_REG, LUT_10TAP_5_LVL1, 0 );
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_23_REG, LUT_10TAP_5_LVL0, 0 );

    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_24_REG, LUT_10TAP_6_LVL3, 0 );
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_25_REG, LUT_10TAP_6_LVL2, 0 );
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_26_REG, LUT_10TAP_6_LVL1, 0 );
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_27_REG, LUT_10TAP_6_LVL0, 0 );

    //program tap 7~9
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_28_REG, LUT_10TAP_7_LVL3, txfir_tbl_ptr->tap7[3]);
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_29_REG, LUT_10TAP_7_LVL2, txfir_tbl_ptr->tap7[2]);
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_30_REG, LUT_10TAP_7_LVL1, txfir_tbl_ptr->tap7[1]);
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_31_REG, LUT_10TAP_7_LVL0, txfir_tbl_ptr->tap7[0]);

    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_32_REG, LUT_10TAP_8_LVL3, txfir_tbl_ptr->tap8[3]);
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_33_REG, LUT_10TAP_8_LVL2, txfir_tbl_ptr->tap8[2]);
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_34_REG, LUT_10TAP_8_LVL1, txfir_tbl_ptr->tap8[1]);
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_35_REG, LUT_10TAP_8_LVL0, txfir_tbl_ptr->tap8[0]);

    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_36_REG, LUT_10TAP_9_LVL3, txfir_tbl_ptr->tap9[3]);
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_37_REG, LUT_10TAP_9_LVL2, txfir_tbl_ptr->tap9[2]);
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_38_REG, LUT_10TAP_9_LVL1, txfir_tbl_ptr->tap9[1]);
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_39_REG, LUT_10TAP_9_LVL0, txfir_tbl_ptr->tap9[0]);

    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_CONTROL_0_REG, LOAD_10TAP_LUT, 1);
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_CONTROL_0_REG, LOAD_10TAP_LUT, 0);
    
    //program nonlinear 256tap
    {
        uint16_t i;
        hsip_wr_field_(phy_info_ptr, LWTX_CLK_RST_LWTX_CLOCK_CONTROL_0_REG, MD_TXG_FIR_CKE , 0x1 );

        // set address to initial value in case (chal_lw_tx_fir_lut_rd_256 change LUT_4T_ADR), this is the initial value for auto address
        hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_4T_CONTROL_0_REG, LUT_4T_EN,  0x1);

        // Toggle to reset addres back to 0  
        hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_4T_CONTROL_0_REG, LUT_4T_AUTO_ADR_EN,  0x0 );
        hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_4T_CONTROL_0_REG, LUT_4T_ADR,  0x0);
        hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_4T_CONTROL_0_REG, LUT_4T_AUTO_ADR_EN,  0x0 );
        hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_4T_CONTROL_0_REG, LUT_4T_AUTO_ADR_EN,  0x1 );

        for ( i = 0; i < 256; i++ ) {
            int16_t val = txfir_tbl_ptr->tap3456[i];

            /* DAC value range is [0, 127] */
            if ( val > 255 ) {
                val = 255;
                return_result = RR_WARNING_BOUNDS;
            }
            else if ( val < -256 ) {
                val = -256;
                return_result = RR_WARNING_BOUNDS;            
            }    
            hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_4T_WDAT_REG, LUT_4T_WDAT, val);
        }
    }
    //program floating tap location
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_FTAP_CONTROL_0_REG, LUT_FTAP_EN, 0x1);
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_FTAP_CONTROL_0_REG, LUT_FTAP_SEL, txfir_tbl_ptr->float_loc);
    //program floating tap
    
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_12_REG, LUT_10TAP_3_LVL3, txfir_tbl_ptr->float_tap1[3]);
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_13_REG, LUT_10TAP_3_LVL2, txfir_tbl_ptr->float_tap1[2]);
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_14_REG, LUT_10TAP_3_LVL1, txfir_tbl_ptr->float_tap1[1]);
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_15_REG, LUT_10TAP_3_LVL0, txfir_tbl_ptr->float_tap1[0]);
                                                                                                   
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_16_REG, LUT_10TAP_4_LVL3, txfir_tbl_ptr->float_tap2[3]);
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_17_REG, LUT_10TAP_4_LVL2, txfir_tbl_ptr->float_tap2[2]);
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_18_REG, LUT_10TAP_4_LVL1, txfir_tbl_ptr->float_tap2[1]);
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_19_REG, LUT_10TAP_4_LVL0, txfir_tbl_ptr->float_tap2[0]);

    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_20_REG, LUT_10TAP_5_LVL3, txfir_tbl_ptr->float_tap3[3]);
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_21_REG, LUT_10TAP_5_LVL2, txfir_tbl_ptr->float_tap3[2]);
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_22_REG, LUT_10TAP_5_LVL1, txfir_tbl_ptr->float_tap3[1]);
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_23_REG, LUT_10TAP_5_LVL0, txfir_tbl_ptr->float_tap3[0]);


    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_CONTROL_0_REG, LOAD_10TAP_LUT, 1);
    hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_CONTROL_0_REG, LOAD_10TAP_LUT, 0);
    hsip_wr_field_(phy_info_ptr,  LWTX_FIR_HLS_LWTX_FIR_CONTROL_0_REG, FIR_MODE_SEL, 1);

    return return_result;
}


/**
 * @brief host_lw_tx_fir_get_floating_13tap (phy_info_t* phy_info_ptr,  capi_txfir_nonlinear_13tap_table_info_t *txfir_tbl_ptr)
 * @details     Read TXFIR 256 LUT values from chip
 * @public
 * @private
 *
 *
 * @param[in]  phy_info_ptr         device info pointer
 * @param[out]  *lut_val            256 entries of TXFIR LUT value read from chip
 * @return enum return_result_t
 */
return_result_t host_lw_tx_fir_get_floating_13tap (phy_info_t* phy_info_ptr,  capi_txfir_nonlinear_13tap_table_info_t *txfir_tbl_ptr)
{
    return_result_t return_result = RR_SUCCESS;
    
    /*    tap[0~9]  => 10_tap interface: 3pre+3post+4main + 3floating
                 tap[3~6]  => 4_tap interface
                 tap[10~12]  => float_tap interface
    */
    //Get tap0~2 info
    txfir_tbl_ptr->tap0[3] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_0_REG,  14, 0);
    txfir_tbl_ptr->tap0[2] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_1_REG,  14, 0);
    txfir_tbl_ptr->tap0[1] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_2_REG,  14, 0);
    txfir_tbl_ptr->tap0[0] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_3_REG,  14, 0);
                                                                                                                      
    txfir_tbl_ptr->tap1[3] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_4_REG,  14, 0);
    txfir_tbl_ptr->tap1[2] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_5_REG,  14, 0);
    txfir_tbl_ptr->tap1[1] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_6_REG,  14, 0);
    txfir_tbl_ptr->tap1[0] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_7_REG,  14, 0);

    txfir_tbl_ptr->tap2[3] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_8_REG,  14, 0);
    txfir_tbl_ptr->tap2[2] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_9_REG,  14, 0);
    txfir_tbl_ptr->tap2[1] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_10_REG, 14, 0);
    txfir_tbl_ptr->tap2[0] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_11_REG, 14, 0);


    //Get tap 7~9 info
    txfir_tbl_ptr->tap7[3] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_28_REG, 14, 0);
    txfir_tbl_ptr->tap7[2] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_29_REG, 14, 0);
    txfir_tbl_ptr->tap7[1] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_30_REG, 14, 0);
    txfir_tbl_ptr->tap7[0] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_31_REG, 14, 0);

    txfir_tbl_ptr->tap8[3] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_32_REG, 14, 0);
    txfir_tbl_ptr->tap8[2] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_33_REG, 14, 0);
    txfir_tbl_ptr->tap8[1] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_34_REG, 14, 0);
    txfir_tbl_ptr->tap8[0] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_35_REG, 14, 0);

    txfir_tbl_ptr->tap9[3] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_36_REG, 14, 0);
    txfir_tbl_ptr->tap9[2] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_37_REG, 14, 0);
    txfir_tbl_ptr->tap9[1] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_38_REG, 14, 0);
    txfir_tbl_ptr->tap9[0] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_39_REG, 14, 0);

    //get tap3/4/5/6 256 entries
    {
        int i;
  
        // Toggle to reset addres back to 0
        hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_4T_CONTROL_0_REG, LUT_4T_AUTO_ADR_EN,  0x0);
  
        for ( i = 0; i < TAP_NONLINEAR_TAP_NUM; i++ ) {
            hsip_wr_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_4T_CONTROL_0_REG, LUT_4T_ADR,  i);
            txfir_tbl_ptr->tap3456[i] = (int16_t) hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_4T_RDAT_REG, 8, 0 );
        }
    }
    //Get floationg tap info
    txfir_tbl_ptr->float_loc = (uint8_t)hsip_rd_field_(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_FTAP_CONTROL_0_REG, LUT_FTAP_SEL);
    txfir_tbl_ptr->float_tap1[3] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_12_REG, 14, 0);
    txfir_tbl_ptr->float_tap1[2] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_13_REG, 14, 0);
    txfir_tbl_ptr->float_tap1[1] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_14_REG, 14, 0);
    txfir_tbl_ptr->float_tap1[0] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_15_REG, 14, 0);

    txfir_tbl_ptr->float_tap2[3] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_16_REG, 14, 0);
    txfir_tbl_ptr->float_tap2[2] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_17_REG, 14, 0);
    txfir_tbl_ptr->float_tap2[1] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_18_REG, 14, 0);
    txfir_tbl_ptr->float_tap2[0] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_19_REG, 14, 0);

    txfir_tbl_ptr->float_tap3[3] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_20_REG, 14, 0);
    txfir_tbl_ptr->float_tap3[2] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_21_REG, 14, 0);
    txfir_tbl_ptr->float_tap3[1] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_22_REG, 14, 0);
    txfir_tbl_ptr->float_tap3[0] = (int16_t)hsip_rd_field_signed(phy_info_ptr, LWTX_FIR_HLS_LWTX_FIR_LUT_10TAP_CONTROL_23_REG, 14, 0);
    return return_result;

}

/**
 * @brief    host_lw_get_pll_configuration(capi_phy_info_t* phy_info_ptr, capi_pll_info_t *pll_info_ptr)
 * @details  This API is used to get PLL config information
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  pll_info_ptr: a pointer with pll configuration information
 *
 * @return     returns the result of the called methode/function, RR_SUCCESS
 */
return_result_t host_lw_get_pll_configuration(capi_phy_info_t* phy_info_ptr, capi_pll_info_t *pll_info_ptr)
{
    capi_phy_info_t         lw_phy, top_phy={0};
    uint32_t                       COMMON_LW_CONFIG1_REG = (phy_info_ptr->core_ip == CORE_IP_MEDIA_DSP)?MEDIA_DSP_CONFIG1_REG:HOST_DSP_CONFIG1_REG;
    uint16_t                       div_ratio;
    
    util_memcpy((void *)&top_phy, phy_info_ptr, sizeof(capi_phy_info_t));
    top_phy.base_addr = OCTAL_TOP_REGS;
    lw_util_init_lane_hw_base_addr(phy_info_ptr, &lw_phy, 0);
    ERR_HSIP(pll_info_ptr->enable_pll_dmode = (uint16_t)hsip_rd_field_(&lw_phy, ANA_PLL_ANA_PLL_CTRL_19_REGISTER, MD_ANA_PLL_EN_DOUBLER));
    ERR_HSIP(pll_info_ptr->enable_frc_frac_mode = (uint16_t)hsip_rd_field_(&top_phy, FW_INTERNAL_CONFIG_REG_1, PLL_FAKE_FRACTION));
    ERR_HSIP(pll_info_ptr->enable_auto_pll_mode = (uint16_t)!hsip_rd_field_(&top_phy, FW_INTERNAL_CONFIG_REG_1, DISABLE_PLL_AUTO_FRACN_MODE));
    ERR_HSIP(pll_info_ptr->frc_frac_value = (uint16_t)hsip_rd_reg_(&top_phy, COMMON_FW_DSP_PLL_FRC_FRAC_VAL));
    ERR_HSIP(pll_info_ptr->pll_bandwidth = (uint16_t)hsip_rd_field_(&top_phy, COMMON_LW_CONFIG1_REG,  CFG_INTEG_PLLX2_BW));

    ERR_HSIP(div_ratio = (uint16_t)hsip_rd_field_(&top_phy, COMMON_LW_CONFIG1_REG, CFG_REG_DIVIDER_RATIO));
    if(div_ratio==0)
        pll_info_ptr->reg_divider_ratio = CAPI_REG_DIVIDER_RATIO_DEFAULT;
    else
        ERR_HSIP(pll_info_ptr->reg_divider_ratio  = (uint16_t)hsip_rd_field_(&lw_phy, ANA_PLL_ANA_PLL_CTRL_23_REGISTER, MD_ANA_PLL_DIVRATIO_REG_CLK)+1);
    return RR_SUCCESS;

}

/**
 * @brief    host_lw_set_snr_threshold(capi_phy_info_t* phy_info_ptr, capi_lw_snr_info_t *snr_info_ptr)
 * @details  This API is used to config snr information
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  snr_info_ptr: a pointer which carries LW snr config information
 *
 * @return     returns the result of the called methode/function, RR_SUCCESS
 */
return_result_t host_lw_set_snr_threshold(capi_phy_info_t* phy_info_ptr, capi_lw_snr_info_t *snr_info_ptr)
{
    uint8_t lane_idx, lane_mask_bak = phy_info_ptr->lane_mask;
    phy_info_t     lw_cfg_phy;
    uint16_t cnt;

    if(snr_info_ptr->cdr_snr_threshold>7)
        return RR_ERROR_WRONG_INPUT_VALUE;
    
    for (lane_idx = 0; lane_idx < MAX_LW_LANES; lane_idx++) {
        if ((lane_mask_bak & (0x01<<lane_idx))==0)
            continue;
        lw_util_init_lane_config_base_addr(phy_info_ptr, &lw_cfg_phy, lane_idx);
        hsip_wr_field_(&lw_cfg_phy, COMMON_LW_LANE_CONFIG2_REG, LW_SNR_LVL,   snr_info_ptr->cdr_snr_threshold);
        hsip_wr_field_(&lw_cfg_phy, COMMON_LW_LANE_CONFIG19_REG, RX_RESTART_PROFILE, 1);
        cnt = 0;
        do{
            cnt++;
        }while(hsip_rd_field_(&lw_cfg_phy, COMMON_LW_LANE_CONFIG19_REG, RX_RESTART_PROFILE) && cnt<5000);
        if(cnt>=5000)
            return RR_ERROR;
    }
    return (RR_SUCCESS);
}

/**
 * @brief    host_lw_get_snr_threshold(capi_phy_info_t* phy_info_ptr, capi_lw_snr_info_t *snr_info_ptr)
 * @details  This API is used to retrieve snr configuration information
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  snr_info_ptr: a pointer which carries LW snr config information
 *
 * @return     returns the result of the called methode/function, RR_SUCCESS
 */
return_result_t host_lw_get_snr_threshold(capi_phy_info_t* phy_info_ptr, capi_lw_snr_info_t *snr_info_ptr)
{
    uint8_t lane_idx, lane_mask_bak = phy_info_ptr->lane_mask;
    phy_info_t     lw_cfg_phy;

    for (lane_idx = 0; lane_idx < MAX_LW_LANES; lane_idx++) {
        if ((lane_mask_bak & (0x01<<lane_idx))==0)
            continue;
        lw_util_init_lane_config_base_addr(phy_info_ptr, &lw_cfg_phy, lane_idx);
        ERR_HSIP(snr_info_ptr->cdr_snr_threshold = hsip_rd_field_(&lw_cfg_phy, COMMON_LW_LANE_CONFIG2_REG, LW_SNR_LVL));
        if(snr_info_ptr->cdr_snr_threshold>7)
            return RR_ERROR_WRONG_OUTPUT_VALUE;
        return RR_SUCCESS;
    }
    return (RR_SUCCESS);
}

/**
 * @brief    host_dsp_set_pll_fracn_dynamic_adjust(capi_phy_info_t* phy_info_ptr, capi_lw_dynamic_pll_fracn_cfg_info_t *dpll_info_ptr)
 * @details  This API is used to config dynamic PLL fracN information
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  dpll_info_ptr: a pointer which carries dynamic PLL config information
 *
 * @return     returns the result of the called methode/function, RR_SUCCESS
 */
return_result_t host_dsp_set_pll_fracn_dynamic_adjust(capi_phy_info_t* phy_info_ptr, capi_lw_dynamic_pll_fracn_cfg_info_t *dpll_info_ptr)
{
    phy_info_t    top_phy = {0}, dsp_phy[PHY_BOTH_SIDES], dsp_cfg_phy;
    uint8_t       pidx, dsp_pll2[PHY_BOTH_SIDES], pll_multi, lpll_pwd, hpll_pwd, dpll_enable;
    uint32_t      pll_base_addr[PHY_BOTH_SIDES]={HOST_BASE, MEDIA_BASE};
    uint32_t      COMMON_LW_CONFIG1_REG, pcnt=0;

    for(pidx=0; pidx<PHY_BOTH_SIDES; pidx++){
         util_memcpy((void *)&dsp_phy[pidx], phy_info_ptr, sizeof(phy_info_t));
         dsp_phy[pidx].base_addr = pll_base_addr[pidx];
    }
    
    util_memcpy((void *)&top_phy, phy_info_ptr, sizeof(capi_phy_info_t));
    top_phy.base_addr = OCTAL_TOP_REGS;
    ERR_HSIP(dpll_enable = (uint8_t)hsip_rd_field_(&top_phy, FW_INTERNAL_CONFIG_REG_2, ENABLE_DSP_PLL_FRAC_MODE_DYNM_ADJUST));
    if(dpll_info_ptr->enable_dynamic_adjust==0 && dpll_enable){
        hsip_wr_field_(&top_phy, FW_INTERNAL_CONFIG_REG_2, ENABLE_DSP_PLL_FRAC_MODE_DYNM_ADJUST, 0);
        hsip_wr_field_(&top_phy, COMMON_FW_DSP_PLL_MEDIA_FRACN_REQ, INTEG_PPM, 0);
        hsip_wr_field_(&dsp_phy[PHY_HOST_SIDE], LWPLL_CTRL_PLLCAL_CTRL_0, PLLCAL_PLL_PWRDN, 1);
        hsip_wr_field_(&dsp_phy[PHY_MEDIA_SIDE], LWPLL_CTRL_PLLCAL_CTRL_0, PLLCAL_PLL_PWRDN, 1);
        delay_ms(10);
         hsip_wr_field_(&dsp_phy[PHY_HOST_SIDE], LWPLL_CTRL_LOCKDET_CTRL_0, LKDT_BYPASS, dpll_info_ptr->enable_dynamic_adjust);
         hsip_wr_field_(&dsp_phy[PHY_MEDIA_SIDE], LWPLL_CTRL_LOCKDET_CTRL_0, LKDT_BYPASS, dpll_info_ptr->enable_dynamic_adjust);
    }
    
    hsip_wr_field_(&top_phy, COMMON_FW_DSP_PLL_MEDIA_FRACN_REQ, INTEG_PPM, 0);
    hsip_wr_field_(&top_phy, COMMON_FW_DSP_PLL_MEDIA_FRACN_CFG, DYNAMIC_PLL_RATIO_SHIFT_STEP_0P1,
                            dpll_info_ptr->adjust_step_0p1_ppm);
    hsip_wr_field_(&top_phy, COMMON_FW_DSP_PLL_MEDIA_FRACN_CFG, DYNAMIC_PLL_RATIO_SHIFT_DELAY_10US,
                            dpll_info_ptr->adjust_gap_timer_10us);

    
    /*check current pll ratio*/
    if(dpll_info_ptr->enable_dynamic_adjust){
        for(pidx=0; pidx<PHY_BOTH_SIDES; pidx++){
                lw_util_init_lane_config_base_addr(&dsp_phy[pidx], &dsp_cfg_phy, 0);
                ERR_HSIP(pll_multi = (uint8_t)hsip_rd_field_(&dsp_cfg_phy,  COMMON_LW_LANE_CONFIG17_REG, PLL_MULTIPLIER));
                if(pll_multi != 0){
                    return RR_ERROR_WRONG_PORT_CONFIGURATION;  /*this feature only is supported in 170 PLL ratio mode*/
                }
         }
        for(pidx=0; pidx<PHY_BOTH_SIDES; pidx++){
                ERR_HSIP(dsp_pll2[pidx] = (uint8_t)hsip_rd_field_(&dsp_phy[pidx],  ANA_PLL_ANA_PLL_CTRL_19_REGISTER, MD_ANA_PLL_EN_DOUBLER));
         }
        /*auto matically enable PLL2 if it has not been enabled*/
        for(pidx=0; pidx<PHY_BOTH_SIDES; pidx++){
            if(dsp_pll2[pidx]==0){
                 COMMON_LW_CONFIG1_REG = pidx?MEDIA_DSP_CONFIG1_REG:HOST_DSP_CONFIG1_REG;
                 hsip_wr_field_(&top_phy, COMMON_LW_CONFIG1_REG, LW_PLL_REFCLK_X2_ENABLE, 1);
                 hsip_wr_field_(&dsp_phy[pidx], LWPLL_CTRL_PLLCAL_CTRL_0, PLLCAL_PLL_PWRDN, 1);
            }
        }
        hsip_wr_field_(&top_phy, FW_INTERNAL_CONFIG_REG_2, ENABLE_DSP_PLL_FRAC_MODE_DYNM_ADJUST, 1);
    }
    do{
        ERR_HSIP(hpll_pwd = (uint8_t)hsip_rd_field_(&dsp_phy[PHY_HOST_SIDE], LWPLL_CTRL_PLLCAL_CTRL_0, PLLCAL_PLL_PWRDN));
        ERR_HSIP(lpll_pwd = (uint8_t)hsip_rd_field_(&dsp_phy[PHY_MEDIA_SIDE], LWPLL_CTRL_PLLCAL_CTRL_0, PLLCAL_PLL_PWRDN));
        pcnt++;
    }while((hpll_pwd || lpll_pwd)&&pcnt<1000);
    if(pcnt>=1000)
        return RR_ERROR;
    
    hsip_wr_field_(&dsp_phy[PHY_HOST_SIDE], LWPLL_CTRL_LOCKDET_CTRL_0, LKDT_BYPASS, dpll_info_ptr->enable_dynamic_adjust);
    hsip_wr_field_(&dsp_phy[PHY_MEDIA_SIDE], LWPLL_CTRL_LOCKDET_CTRL_0, LKDT_BYPASS, dpll_info_ptr->enable_dynamic_adjust);
    return (RR_SUCCESS);
}

/**
 * @brief    host_dsp_get_pll_fracn_dynamic_adjust(capi_phy_info_t* phy_info_ptr, capi_lw_dynamic_pll_fracn_cfg_info_t *dpll_info_ptr)
 * @details  This API is used to get dynamic PLL fracN information
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  dpll_info_ptr: a pointer which carries dynamic PLL config information
 *
 * @return     returns the result of the called methode/function, RR_SUCCESS
 */
return_result_t host_dsp_get_pll_fracn_dynamic_adjust(capi_phy_info_t* phy_info_ptr, capi_lw_dynamic_pll_fracn_cfg_info_t *dpll_info_ptr)
{
    phy_info_t    top_phy = {0};
    
    util_memcpy((void *)&top_phy, phy_info_ptr, sizeof(capi_phy_info_t));
    top_phy.base_addr = OCTAL_TOP_REGS;
    ERR_HSIP(dpll_info_ptr->enable_dynamic_adjust = (uint8_t)hsip_rd_field_(&top_phy, FW_INTERNAL_CONFIG_REG_2, 
                                    ENABLE_DSP_PLL_FRAC_MODE_DYNM_ADJUST));
    
    ERR_HSIP( dpll_info_ptr->adjust_step_0p1_ppm = (uint8_t)hsip_rd_field_(&top_phy, 
                            COMMON_FW_DSP_PLL_MEDIA_FRACN_CFG, DYNAMIC_PLL_RATIO_SHIFT_STEP_0P1));
    ERR_HSIP( dpll_info_ptr->adjust_gap_timer_10us = (uint8_t)hsip_rd_field_(&top_phy, 
                            COMMON_FW_DSP_PLL_MEDIA_FRACN_CFG, DYNAMIC_PLL_RATIO_SHIFT_DELAY_10US));
    return (RR_SUCCESS);
}


/**
 * @brief    host_lw_get_gain3_a_val(capi_phy_info_t* phy_info_ptr, int channel_id, float *gain3_a_val_ptr)
 * @details  This API is used to get gain3 A value
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  channel_id: ADC channel ID
 * @param[in]  gain3_a_val_ptr: point to gain3 a value
 * @return     returns the result of the called method/function, RR_SUCCESS
 */
return_result_t host_lw_get_gain3_a_val(capi_phy_info_t* phy_info_ptr, int channel_id, float *gain3_a_val_ptr)
{
    capi_phy_info_t  lw_phy_hw;
    uint16_t va_en;
    int16_t  va_a_val;
    capi_lane_info_t lane_info;
    return_result_t return_rslt;


    lw_util_init_lane_hw_base_addr(phy_info_ptr, &lw_phy_hw, channel_id);
    ERR_HSIP(va_en = hsip_rd_field_(&lw_phy_hw, LWRX_TOP_LWRX_DATA_CTRL_REG, MD_LWRX_DOUT_SELECT));

    *gain3_a_val_ptr = 0;

    util_memset((void *)&lane_info, 0, sizeof(capi_lane_info_t));
    lane_info.param.is.lane_status = 1;
    return_rslt = capi_get_lane_info(phy_info_ptr, &lane_info);
     //if (capi_get_pmd_info(phy_info_ptr, &pmd_info) != RR_SUCCESS) {
     if (return_rslt != RR_SUCCESS) {
         return return_rslt;
     }
     /*check CDR locked with gain2 enabled*/
     if((lane_info.value.lane_status.cdr_lock&(1<<channel_id))==0 || va_en==0){
         return RR_ERROR_FIRMWARE_NOT_READY;
     }
     
     hsip_wr_field_(&lw_phy_hw, VA_VA_PARA_A_MONITOR_REG, CAP_PARA_A, 1);
     va_a_val = (int16_t)hsip_rd_field_signed(&lw_phy_hw, VA_VA_PARA_A_MONITOR_REG, 6, 0);
     hsip_wr_field_(&lw_phy_hw, VA_VA_PARA_A_MONITOR_REG, CAP_PARA_A, 0);
     *gain3_a_val_ptr = (float)va_a_val/64;
     return RR_SUCCESS;
}


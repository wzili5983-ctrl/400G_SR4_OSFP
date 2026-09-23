/**
 *
 * @file    host_power_util.h
 * @author
 * @date    12/11/2017
 * @version 1.0
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
#ifndef HOST_POWER_UTIL_H
#define HOST_POWER_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#define ANA_REG_WRITE 0x1
#define ANA_REG_READ  0x2
#define VOLT_ADJ_DAC_REG_ADDR 0xC
#define ANA_REG_0             0x0

#define ANA_AVS_MASTER_PHY_ID 0x0
#define ANA_AVS_SLAVE1_PHY_ID 0x1
#define ANA_AVS_SLAVE2_PHY_ID 0x5
#define ANA_AVS_SLAVE3_PHY_ID 0x6
#define ANA_AVS_SLAVE4_PHY_ID 0x8

#define ANA_DVDDM_MASTER_PHY_ID 0x2
#define ANA_DVDDM_SLAVE_PHY_ID  0x3

#define ANA_AVDD_MASTER_PHY_ID 0x4
#define ANA_AVDD_SLAVE_PHY_ID  0x7

return_result_t chal_ana_reg_disable_avs_1_slaves(phy_info_t* phy_info_ptr);
return_result_t chal_ana_reg_disable_avs_2_slaves(phy_info_t* phy_info_ptr);
return_result_t chal_ana_reg_disable_avs_3_slaves(phy_info_t* phy_info_ptr);
return_result_t chal_ana_reg_disable_avs_slaves(phy_info_t* phy_info_ptr);
return_result_t chal_ana_reg_enable_avs_slave1 (phy_info_t* phy_info_ptr);
return_result_t chal_ana_reg_enable_avs_slave2 (phy_info_t* phy_info_ptr);
return_result_t chal_ana_reg_enable_avs_slave3 (phy_info_t* phy_info_ptr);
return_result_t chal_ana_reg_enable_avs_slave4 (phy_info_t* phy_info_ptr);
return_result_t top_supspend_resume(capi_phy_info_t*  phy_info_ptr, uint32_t suspend_resume_command);

return_result_t chal_ana_avs_set_volt(phy_info_t* phy_info_ptr, uint32_t avs_mv);
return_result_t host_set_internal_regulator_voltage(capi_phy_info_t*                   phy_info_ptr,
                                                    capi_internal_regulator_voltage_t* int_reg_voltage_ptr);

return_result_t chal_ana_vddm_set_volt(phy_info_t* phy_info_ptr, uint32_t avs_mv);
return_result_t chal_ana_vddm_get_volt (phy_info_t* phy_info_ptr, uint32_t* avs_reading);
return_result_t chal_ana_avdd_set_volt(phy_info_t* phy_info_ptr, uint32_t avs_mv);
return_result_t chal_ana_avdd_get_volt (phy_info_t* phy_info_ptr, uint32_t* avs_reading);

return_result_t chal_ana_reg_enable_avs_slaves(phy_info_t* phy_info_ptr);

return_result_t chal_ana_reg_disable_vddm_slave(phy_info_t* phy_info_ptr);
return_result_t chal_ana_reg_enable_vddm_slave(phy_info_t* phy_info_ptr);

return_result_t chal_ana_reg_disable_avdd_slave(phy_info_t* phy_info_ptr);
return_result_t chal_ana_reg_enable_avdd_slave(phy_info_t* phy_info_ptr);
return_result_t host_util_ana_disable_all_regs (phy_info_t* phy_info_ptr, capi_regulator_info_t* regulator_info_ptr) ;


#ifdef __cplusplus
}
#endif

#endif  /* HOST_POWER_UTIL_H */

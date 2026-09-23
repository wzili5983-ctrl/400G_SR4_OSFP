/**
 *
 * @file    host_power_util.c
 * @author  Firmware Team
 * @date    12/27/2018
 * @version 0.2
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
#include "hr_time.h"
#include "access.h"
#include "common_def.h"
#include "regs_common.h"
#include "chip_mode_def.h"
#include "capi_def.h"
#include "host_power_util.h"
#include "common_util.h"
#include "capi.h"
#include "host_avs.h"

/**
 * @brief return_result_t chal_ana_reg_access(phy_info_t* phy_info_ptr, uint8_t acc_type, uint8_t ana_phy_ad, uint8_t reg_ad, uint16_t wr_data, uint16_t* rd_data)
 * @details  MDIO indirect access to ANA regulators <BR>
 * @public   
 * @private 
 * @param  bbaddr device base address 
 * @param  acc_type  READ or WRITE
 * @param  ana_phy_ad  REGULATOR ID (0-5)
 * @param  reg_ad  REGULATOR internal register number (0x00-0xFF)
 * @param  wr_data  Write data value -- 16-b
 * @param  *rd_data  Read data value -- 16-b
 * @return enum return_result_t
 */
static return_result_t chal_ana_reg_access(phy_info_t* phy_info_ptr, uint8_t acc_type, uint8_t ana_phy_ad, uint8_t reg_ad, uint16_t wr_data, uint16_t* rd_data)
{
    const uint8_t st = (0x1 & 0x3); //- 2 bit variable
    const  uint8_t ta = (0x2 & 0x3); //- 2 bit variable
    uint32_t read_value;

    uint32_t event_count_timeout = 0;
    uint32_t cmd = 0;
    uint8_t phy_ad;
    uint8_t reg_addr;
    uint8_t access;
    uint16_t wr_val = 0;

    phy_ad   = ana_phy_ad & 0x1F;  // 5-bits
    reg_addr = reg_ad & 0x1F;      // 5-bits
    access   = acc_type & 0x3;     //2-bits

    if (access == ANA_REG_WRITE) { 
        wr_val   = wr_data;
    } else { 
        wr_val   = 0x0;
    }

    cmd = (st << 30 | ta << 16 | phy_ad << 23 | reg_addr << 18 | wr_val | access << 28);

    hsip_wr_reg_(phy_info_ptr, MDIOM_MDIOM_CTRL, 0x286); // start the clock to the MDIO slave
    delay_us(100);
    hsip_wr_reg_(phy_info_ptr, MDIOM_CMD, cmd);

    ERR_HSIP(read_value = hsip_rd_reg_(phy_info_ptr, MDIOM_MDIOM_CTRL));
    do {
         ERR_HSIP(read_value = hsip_rd_reg_(phy_info_ptr, MDIOM_MDIOM_CTRL));
    } while((read_value & 0x100) && (event_count_timeout++ <= 70000));

    if (event_count_timeout > 70000) { 
      return RR_ERROR;
    } 

    if(access == ANA_REG_READ) {
        ERR_HSIP(read_value = hsip_rd_reg_(phy_info_ptr, MDIOM_CMD));
        *rd_data = read_value & 0xFFFF;
        if((*rd_data!=0x6520)&&(*rd_data!=0x65A0)) {
            //WriteReg32(0x5ff20, (0x4444<< 16 | *rd_data));
        }
    }
    return RR_SUCCESS;
}

/**
 * @brief return_result_t chal_ana_avs_set_volt(phy_info_t* phy_info_ptr, uint32_t avs_mv)
 * @details  Adjust AVS voltage <BR>
 * @public   
 * @private 
 * @param  phy_info_ptr whose struct holds the device base address 
 * @param  avs_mv  in units of 1000mV type. Absolute value to be set in mV (should always be > 500mV, such that it is 500 + x*3.125, where x is the step)
 * @return enum return_result_t
 */
return_result_t chal_ana_avs_set_volt(phy_info_t* phy_info_ptr, uint32_t avs_mv) 
{
    uint32_t step;
    uint32_t dac_val;
    uint16_t reg_val;
    uint16_t rd_data;
    uint16_t wr_data;

    step = avs_mv - 500000;
    dac_val = step/3125;
    reg_val = (dac_val & 0xFF)<< 8; 

    // Do a read modify write to regC
    if(chal_ana_reg_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, VOLT_ADJ_DAC_REG_ADDR, 0x0, &rd_data)!=RR_SUCCESS){
       return RR_ERROR;
    }

    wr_data =  reg_val;
    chal_ana_reg_access (phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, VOLT_ADJ_DAC_REG_ADDR, wr_data, &rd_data);

    // Toggle by writing 1 to reg0[1] and then 0 to complete the transaction
    chal_ana_reg_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data | 0x0002;
    chal_ana_reg_access (phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    chal_ana_reg_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data & 0xFFFD;
    chal_ana_reg_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);
    return RR_SUCCESS;
}


static return_result_t chal_ana_reg_rdwr_access(phy_info_t* phy_info_ptr, uint8_t acc_type, uint8_t ana_phy_ad, uint8_t reg_ad, uint16_t wr_data, uint16_t* rd_data)
{
    const uint8_t st = (0x1 & 0x3); //- 2 bit variable
    const  uint8_t ta = (0x2 & 0x3); //- 2 bit variable
    uint32_t read_value;
    uint32_t event_count_timeout = 0;
    uint32_t cmd = 0;
    uint8_t phy_ad;
    uint8_t reg_addr;
    uint8_t access;
    uint16_t wr_val = 0;

    phy_ad   = ana_phy_ad & 0x1F;  // 5-bits
    reg_addr = reg_ad & 0x1F;      // 5-bits
    access   = acc_type & 0x3;     //2-bits

    if (access == ANA_REG_WRITE) { 
        wr_val   = wr_data;
    } else { 
        wr_val   = 0x0;
    }

    cmd = (st << 30 | ta << 16 | phy_ad << 23 | reg_addr << 18 | wr_val | access << 28);

    hsip_wr_reg_(phy_info_ptr, MDIOM_MDIOM_CTRL, 0x286); // start the clock to the MDIO slave
    hsip_wr_reg_(phy_info_ptr, MDIOM_CMD, cmd);

    ERR_HSIP(read_value = hsip_rd_reg_(phy_info_ptr, MDIOM_MDIOM_CTRL));
    do {
        ERR_HSIP(read_value = hsip_rd_reg_(phy_info_ptr, MDIOM_MDIOM_CTRL));
    } while((read_value & 0x100) && (event_count_timeout++ <= 70000));

    if (event_count_timeout > 70000) { 
      return RR_ERROR;
    }

    if(access == ANA_REG_READ) {
        ERR_HSIP(read_value = hsip_rd_reg_(phy_info_ptr, MDIOM_CMD));
        *rd_data = read_value & 0xFFFF;
    }

    return RR_SUCCESS;
}

return_result_t chal_ana_vddm_set_volt(phy_info_t* phy_info_ptr, uint32_t avs_mv) {
    uint32_t step;
    uint32_t dac_val;
    uint16_t reg_val;
    uint16_t rd_data;
    uint16_t wr_data;

    step = avs_mv - 500000;
    dac_val = step/3125;
    reg_val = (dac_val & 0xFF)<< 8; 

    /* Do a read modify write to regC */
    if(chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_DVDDM_MASTER_PHY_ID, VOLT_ADJ_DAC_REG_ADDR, 0x0, &rd_data)!=RR_SUCCESS){
        return RR_ERROR;
    }

    wr_data =  reg_val;
    chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_WRITE, ANA_DVDDM_MASTER_PHY_ID, VOLT_ADJ_DAC_REG_ADDR, wr_data, &rd_data);

    /* Toggle by writing 1 to reg0[1] and then 0 to complete the transaction */
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_DVDDM_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data | 0x0002;
    chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_WRITE, ANA_DVDDM_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_DVDDM_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data & 0xFFFD;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_DVDDM_MASTER_PHY_ID, ANA_REG_0,wr_data, &rd_data);
    return RR_SUCCESS;
}

return_result_t chal_ana_vddm_get_volt(phy_info_t* phy_info_ptr, uint32_t* avs_reading) 
{
    uint32_t dac_val;
    uint16_t reg_val;
    uint16_t rd_data;

    if(chal_ana_reg_access(phy_info_ptr, ANA_REG_READ, ANA_DVDDM_MASTER_PHY_ID, VOLT_ADJ_DAC_REG_ADDR, 0x0, &rd_data)!=RR_SUCCESS){
        return RR_ERROR;
    }
    reg_val = (rd_data & 0xFF00) >> 8; 
    dac_val = reg_val * 3125;
    *avs_reading = dac_val + 500000;
    return RR_SUCCESS;
}

return_result_t chal_ana_avdd_set_volt(phy_info_t* phy_info_ptr, uint32_t avs_mv) 
{
    uint32_t step;
    uint32_t dac_val;
    uint16_t reg_val;
    uint16_t rd_data;
    uint16_t wr_data;

    step = avs_mv - 500000;
    dac_val = step/3125;
    reg_val = (dac_val & 0xFF)<< 8; 

    /*Do a read modify write to regC*/
    if(chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVDD_MASTER_PHY_ID, VOLT_ADJ_DAC_REG_ADDR, 0x0, &rd_data)!=RR_SUCCESS){
        return RR_ERROR;
    }

    wr_data =  reg_val;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVDD_MASTER_PHY_ID, VOLT_ADJ_DAC_REG_ADDR, wr_data, &rd_data);

    /*Toggle by writing 1 to reg0[1] and then 0 to complete the transaction*/
    chal_ana_reg_rdwr_access(phy_info_ptr,ANA_REG_READ, ANA_AVDD_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data | 0x0002;
    chal_ana_reg_rdwr_access (phy_info_ptr,ANA_REG_WRITE, ANA_AVDD_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVDD_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data & 0xFFFD;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVDD_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);
    return RR_SUCCESS;
}

return_result_t chal_ana_avdd_get_volt(phy_info_t* phy_info_ptr, uint32_t* avs_reading) 
{
    uint32_t dac_val;
    uint16_t reg_val;
    uint16_t rd_data = 0;

    if(chal_ana_reg_access(phy_info_ptr, ANA_REG_READ, ANA_AVDD_MASTER_PHY_ID, VOLT_ADJ_DAC_REG_ADDR, 0x0, &rd_data)!=RR_SUCCESS){
        return RR_ERROR;
    }
    reg_val = (rd_data & 0xFF00) >> 8; 
    dac_val = reg_val * 3125;
    *avs_reading = dac_val + 500000;
    return RR_SUCCESS;
}

return_result_t chal_ana_reg_disable_avs_1_slaves(phy_info_t* phy_info_ptr) {
    uint16_t rd_data;
    uint16_t wr_data;

    /* Do a read modify write to reg0 */
    if(chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_SLAVE4_PHY_ID, ANA_REG_0,  0x0, &rd_data)!=RR_SUCCESS){
        return RR_ERROR;
    }

    wr_data =  rd_data | 0x0001; // set soft-reset
    chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_WRITE, ANA_AVS_SLAVE4_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    /* Toggle by writing 1 to reg0[1] and then 0 to complete the transaction */
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data | 0x0002;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data & 0xFFFD;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0,wr_data, &rd_data);

    return RR_SUCCESS;
}

return_result_t chal_ana_reg_disable_avs_2_slaves(phy_info_t* phy_info_ptr) {
    uint16_t rd_data;
    uint16_t wr_data;

    /* Do a read modify write to reg0 */

    if(chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_SLAVE3_PHY_ID, ANA_REG_0,  0x0, &rd_data)!=RR_SUCCESS){
        return RR_ERROR;
    }

    wr_data =  rd_data | 0x0001; // set soft-reset
    chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_WRITE, ANA_AVS_SLAVE3_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    /* Toggle by writing 1 to reg0[1] and then 0 to complete the transaction */
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data | 0x0002;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
     wr_data = rd_data & 0xFFFD;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0,wr_data, &rd_data);

    /* Do a read modify write to reg0 */
    if(chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_SLAVE4_PHY_ID, ANA_REG_0,  0x0, &rd_data)!=RR_SUCCESS){
        return RR_ERROR;
    }

    wr_data =  rd_data | 0x0001; // set soft-reset
    chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_WRITE, ANA_AVS_SLAVE4_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    /* Toggle by writing 1 to reg0[1] and then 0 to complete the transaction */
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data | 0x0002;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
     wr_data = rd_data & 0xFFFD;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0,wr_data, &rd_data);

    return RR_SUCCESS;
}

return_result_t chal_ana_reg_disable_avs_3_slaves(phy_info_t* phy_info_ptr) {
    uint16_t rd_data;
    uint16_t wr_data;

    /* Do a read modify write to reg0 */

    if(chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_SLAVE2_PHY_ID, ANA_REG_0,  0x0, &rd_data)!=RR_SUCCESS){
       return RR_ERROR;
    }

    wr_data =  rd_data | 0x0001; // set soft-reset
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_SLAVE2_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    /* Toggle by writing 1 to reg0[1] and then 0 to complete the transaction */
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data | 0x0002;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data & 0xFFFD;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0,wr_data, &rd_data);

    /* Do a read modify write to reg0 */
    if(chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_SLAVE3_PHY_ID, ANA_REG_0,  0x0, &rd_data)!=RR_SUCCESS){
        return RR_ERROR;
    }

    wr_data =  rd_data | 0x0001; // set soft-reset
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_SLAVE3_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    /* Toggle by writing 1 to reg0[1] and then 0 to complete the transaction */
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data | 0x0002;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data & 0xFFFD;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0,wr_data, &rd_data);

    /* Do a read modify write to reg0 */
    if(chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_SLAVE4_PHY_ID, ANA_REG_0,  0x0, &rd_data)!=RR_SUCCESS){
        return RR_ERROR;
    }

    wr_data =  rd_data | 0x0001; // set soft-reset
    chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_WRITE, ANA_AVS_SLAVE4_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    /* Toggle by writing 1 to reg0[1] and then 0 to complete the transaction */
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data | 0x0002;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data & 0xFFFD;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0,wr_data, &rd_data);

    return RR_SUCCESS;
}

return_result_t chal_ana_reg_disable_avs_slaves(phy_info_t* phy_info_ptr) {
    uint16_t rd_data;
    uint16_t wr_data;

    /* Do a read modify write to reg0 */

    if(chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_SLAVE1_PHY_ID, ANA_REG_0,  0x0, &rd_data)!=RR_SUCCESS){
        return RR_ERROR;
    }

    wr_data =  rd_data | 0x0001; // set soft-reset
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_SLAVE1_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    /* Toggle by writing 1 to reg0[1] and then 0 to complete the transaction */
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data | 0x0002;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data & 0xFFFD;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0,wr_data, &rd_data);

    /* Do a read modify write to reg0 */
    if(chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_SLAVE2_PHY_ID, ANA_REG_0,  0x0, &rd_data)!=RR_SUCCESS){
        return RR_ERROR;
    }

    wr_data =  rd_data | 0x0001; // set soft-reset
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_SLAVE2_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    /* Toggle by writing 1 to reg0[1] and then 0 to complete the transaction */
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data | 0x0002;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data & 0xFFFD;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0,wr_data, &rd_data);

    /* Do a read modify write to reg0 */

    if(chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_SLAVE3_PHY_ID, ANA_REG_0,  0x0, &rd_data)!=RR_SUCCESS){
        return RR_ERROR;
    }

    wr_data =  rd_data | 0x0001; // set soft-reset
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_SLAVE3_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    /* Toggle by writing 1 to reg0[1] and then 0 to complete the transaction */
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data | 0x0002;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data & 0xFFFD;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0,wr_data, &rd_data);

    /* Do a read modify write to reg0 */
    if(chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_SLAVE4_PHY_ID, ANA_REG_0,  0x0, &rd_data)!=RR_SUCCESS){
        return RR_ERROR;
    }

    wr_data =  rd_data | 0x0001; // set soft-reset
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_SLAVE4_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    /* Toggle by writing 1 to reg0[1] and then 0 to complete the transaction */
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data | 0x0002;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data & 0xFFFD;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0,wr_data, &rd_data);

    return RR_SUCCESS;
}

return_result_t top_supspend_resume(capi_phy_info_t*  phy_info_ptr, uint32_t suspend_resume_command) {
    chip_top_command_avsmgt_reg_t   chip_top_command_avsmgt_reg;
    chip_top_status_avsmgt_reg_t    chip_top_status_avsmgt_reg;
    phy_info_t fw_phy_core_top_reg;
    uint32_t try_counter =0;

    fw_phy_core_top_reg.phy_id = phy_info_ptr->phy_id;
    fw_phy_core_top_reg.base_addr = OCTAL_TOP_REGS;
    ERR_HSIP(chip_top_command_avsmgt_reg.words = (uint16_t) hsip_rd_reg_(&fw_phy_core_top_reg, CHIP_TOP_CHIP_COMMAND_AVSMGT_REG));
    ERR_HSIP(chip_top_status_avsmgt_reg.words = (uint16_t) hsip_rd_reg_(&fw_phy_core_top_reg, CHIP_TOP_CHIP_STATUS_AVSMGT_REG));
    if((chip_top_command_avsmgt_reg.fields.command_request !=0 ) && (chip_top_status_avsmgt_reg.fields.status_ack !=0 )){
        return RR_ERROR;
    }
    chip_top_command_avsmgt_reg.words = 0;
    chip_top_command_avsmgt_reg.fields.command_request=1;
    chip_top_command_avsmgt_reg.fields.suspend_resume = suspend_resume_command; //suspend

    hsip_wr_reg_(&fw_phy_core_top_reg, CHIP_TOP_CHIP_COMMAND_AVSMGT_REG, chip_top_command_avsmgt_reg.words);
    do {
        ERR_HSIP(chip_top_status_avsmgt_reg.words = (uint16_t) hsip_rd_reg_(&fw_phy_core_top_reg, CHIP_TOP_CHIP_STATUS_AVSMGT_REG));
        delay_ms(1);
    } while((chip_top_status_avsmgt_reg.fields.status_ack!=0x1) && (try_counter++ < 100));
    if (chip_top_status_avsmgt_reg.fields.status_ack!=0x1) return RR_ERROR;
    chip_top_command_avsmgt_reg.fields.command_request=0;
    chip_top_command_avsmgt_reg.fields.suspend_resume = 0; //suspend

    hsip_wr_reg_(&fw_phy_core_top_reg, CHIP_TOP_CHIP_COMMAND_AVSMGT_REG, chip_top_command_avsmgt_reg.words);
    return RR_SUCCESS;
}


return_result_t chal_ana_reg_disable_vddm_slave(phy_info_t* phy_info_ptr) {
    uint16_t rd_data;
    uint16_t wr_data;

    /* Do a read modify write to reg0 */
    if(chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_DVDDM_SLAVE_PHY_ID, ANA_REG_0,  0x0, &rd_data)!=RR_SUCCESS){
        return RR_ERROR;
    }

    wr_data =  rd_data | 0x0001; // set soft-reset
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_DVDDM_SLAVE_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    /* Toggle by writing 1 to reg0[1] and then 0 to complete the transaction */
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_DVDDM_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data | 0x0002;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_DVDDM_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_DVDDM_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data & 0xFFFD;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_DVDDM_MASTER_PHY_ID, ANA_REG_0,wr_data, &rd_data);

    return RR_SUCCESS;
}

return_result_t chal_ana_reg_enable_avs_slaves(phy_info_t* phy_info_ptr) {
    uint16_t rd_data;
    uint16_t wr_data;

    /* Do a read modify write to reg0 */
    if(chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_SLAVE1_PHY_ID, ANA_REG_0,  0x0, &rd_data)!=RR_SUCCESS){
        return RR_ERROR;
    }

    wr_data =  rd_data & 0xFFFE; // remove soft-reset
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_SLAVE1_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    /* Toggle by writing 1 to reg0[1] and then 0 to complete the transaction */
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data | 0x0002;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data & 0xFFFD;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0,wr_data, &rd_data);

    /* Do a read modify write to reg0 */
    if(chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_SLAVE2_PHY_ID, ANA_REG_0,  0x0, &rd_data)!=RR_SUCCESS){
        return RR_ERROR;
    }

    wr_data = rd_data & 0xFFFE; // remove soft-reset
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_SLAVE2_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    /* Toggle by writing 1 to reg0[1] and then 0 to complete the transaction */
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data | 0x0002;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data & 0xFFFD;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0,wr_data, &rd_data);

    /* Do a read modify write to reg0 */
    if(chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_SLAVE3_PHY_ID, ANA_REG_0,  0x0, &rd_data)!=RR_SUCCESS){
        return RR_ERROR;
    }

    wr_data =  rd_data & 0xFFFE; // remove soft-reset
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_SLAVE3_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    /* Toggle by writing 1 to reg0[1] and then 0 to complete the transaction */
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data | 0x0002;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data & 0xFFFD;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0,wr_data, &rd_data);

    /* Do a read modify write to reg0 */
    if(chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_SLAVE4_PHY_ID, ANA_REG_0,  0x0, &rd_data)!=RR_SUCCESS){
        return RR_ERROR;
    }

    wr_data =  rd_data & 0xFFFE; // remove soft-reset
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_SLAVE4_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    /* Toggle by writing 1 to reg0[1] and then 0 to complete the transaction */
    chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data | 0x0002;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data & 0xFFFD;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0,wr_data, &rd_data);

    return RR_SUCCESS;
}


return_result_t  chal_ana_reg_enable_avs_slave1 (phy_info_t* phy_info_ptr) {
   uint16_t rd_data = 0;
   uint16_t wr_data = 0;

   /* Do a read modify write to reg0 */

   if(chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_READ, ANA_AVS_SLAVE1_PHY_ID, ANA_REG_0,  0x0, &rd_data)!=RR_SUCCESS){
      return RR_ERROR;
   }

   wr_data =  rd_data & 0xFFFE; // remove soft-reset
   chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_WRITE, ANA_AVS_SLAVE1_PHY_ID, ANA_REG_0, wr_data, &rd_data);

   /* Toggle by writing 1 to reg0[1] and then 0 to complete the transaction */
   chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
   wr_data = rd_data | 0x0002;
   chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);

   chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data & 0xFFFD;
   chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0,wr_data, &rd_data);


   return RR_SUCCESS;
}

return_result_t  chal_ana_reg_enable_avs_slave2 (phy_info_t* phy_info_ptr) {
   uint16_t rd_data = 0;
   uint16_t wr_data = 0;

   /* Do a read modify write to reg0 */

   if(chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_READ, ANA_AVS_SLAVE2_PHY_ID, ANA_REG_0,  0x0, &rd_data)!=RR_SUCCESS){
      return RR_ERROR;
   }

   wr_data =  rd_data & 0xFFFE; // remove soft-reset
   chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_WRITE, ANA_AVS_SLAVE2_PHY_ID, ANA_REG_0, wr_data, &rd_data);

   /* Toggle by writing 1 to reg0[1] and then 0 to complete the transaction */
   chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
   wr_data = rd_data | 0x0002;
   chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);

   chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data & 0xFFFD;
   chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0,wr_data, &rd_data);


   return RR_SUCCESS;
}

return_result_t  chal_ana_reg_enable_avs_slave3 (phy_info_t* phy_info_ptr) {
   uint16_t rd_data = 0;
   uint16_t wr_data = 0;

   /* Do a read modify write to reg0 */

   if(chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_READ, ANA_AVS_SLAVE3_PHY_ID, ANA_REG_0,  0x0, &rd_data)!=RR_SUCCESS){
      return RR_ERROR;
   }

   wr_data =  rd_data & 0xFFFE; // remove soft-reset
   chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_WRITE, ANA_AVS_SLAVE3_PHY_ID, ANA_REG_0, wr_data, &rd_data);

   /* Toggle by writing 1 to reg0[1] and then 0 to complete the transaction */
   chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
   wr_data = rd_data | 0x0002;
   chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);

   chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data & 0xFFFD;
   chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0,wr_data, &rd_data);


   return RR_SUCCESS;
}

return_result_t  chal_ana_reg_enable_avs_slave4 (phy_info_t* phy_info_ptr) {
   uint16_t rd_data = 0;
   uint16_t wr_data = 0;

   /* Do a read modify write to reg0 */

   if(chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_READ, ANA_AVS_SLAVE4_PHY_ID, ANA_REG_0,  0x0, &rd_data)!=RR_SUCCESS){
      return RR_ERROR;
   }

   wr_data =  rd_data & 0xFFFE; // remove soft-reset
   chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_WRITE, ANA_AVS_SLAVE4_PHY_ID, ANA_REG_0, wr_data, &rd_data);

   /* Toggle by writing 1 to reg0[1] and then 0 to complete the transaction */
   chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
   wr_data = rd_data | 0x0002;
   chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);

   chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data & 0xFFFD;
   chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0,wr_data, &rd_data);

   return RR_SUCCESS;
}

return_result_t disable_avs(capi_phy_info_t*                   phy_info_ptr){
    capi_avs_mode_config_info_t capi_avs_mode_config;
    capi_avs_status_t capi_avs_status;
    return_result_t ret=RR_SUCCESS;

    host_get_avs_status(phy_info_ptr, &capi_avs_status);
    if(capi_avs_status.avs_enable==1){

        util_memset(&capi_avs_mode_config, 0x0, sizeof(capi_avs_mode_config_info_t));
        capi_avs_mode_config.avs = CAPI_DISABLE;
        capi_avs_mode_config.disable_type = CAPI_AVS_DISABLE_FIRMWARE_CONTROL;
        capi_avs_mode_config.type_of_regulator = CAPI_REGULATOR_INTERNAL;

        capi_set_avs_config(phy_info_ptr, &capi_avs_mode_config);
 
        do {
            ret = host_get_avs_status(phy_info_ptr, &capi_avs_status);
            delay_ms(1000);
        } while((ret!=RR_SUCCESS) || (capi_avs_status.avs_result!= CAPI_INIT_DONE_SUCCESS));

        if(capi_avs_status.avs_result!= CAPI_INIT_DONE_SUCCESS) {
                return(RR_ERROR);
        }
    }
    return RR_SUCCESS;
}

return_result_t host_set_internal_regulator_voltage(capi_phy_info_t*                   phy_info_ptr,
                                                    capi_internal_regulator_voltage_t* int_reg_voltage_ptr)
{
    uint16_t        voltage       = 0;
    uint32_t        read_value    = 0;
    uint32_t        firmware_flag = 0;
    uint32_t        try_counter   = 0;
    return_result_t return_result = RR_SUCCESS;
    capi_phy_info_t fw_phy;
    capi_phy_info_t fw_phy_core_top_reg;

    util_memset(&fw_phy, 0, sizeof(phy_info_t));
    util_memcpy(&fw_phy, phy_info_ptr, sizeof(phy_info_t));
    util_memcpy(&fw_phy_core_top_reg, phy_info_ptr, sizeof(phy_info_t));

    fw_phy_core_top_reg.base_addr = OCTAL_TOP_REGS;

    if (int_reg_voltage_ptr->while_fw_running) {
        hsip_wr_reg_(&fw_phy_core_top_reg, CHIP_TOP_CHIP_FIRMWARE_STATUS_P1_REG, 0); 

        do {
            ERR_HSIP(read_value = hsip_rd_reg_(&fw_phy_core_top_reg, CHIP_TOP_CHIP_FIRMWARE_STATUS_P1_REG));
            delay_ms(1);
        } while((read_value==0x0) && (try_counter++ < 10));

        if (read_value == 0x0) {
            firmware_flag = 0;
        } else {
            firmware_flag = 1;
        }

        if (firmware_flag == 1) {
            return_result = disable_avs(phy_info_ptr);
            if (return_result != RR_SUCCESS) return return_result;
        }
    }

    if (!int_reg_voltage_ptr->voltage) {
        if (int_reg_voltage_ptr->int_reg_component_type == CAPI_INTERNAL_REGULATOR_COMPONENT_VDDM) {
            voltage = 750;
        } else if (int_reg_voltage_ptr->int_reg_component_type == CAPI_INTERNAL_REGULATOR_COMPONENT_AVDD) {
            voltage = 900;
        } else {
            return(RR_ERROR_WRONG_INPUT_VALUE);
        }
    } else {
        if (int_reg_voltage_ptr->int_reg_component_type == CAPI_INTERNAL_REGULATOR_COMPONENT_VDDM) {
            if (int_reg_voltage_ptr->voltage > 800 || int_reg_voltage_ptr->voltage < 700) {
                return(RR_ERROR_WRONG_INPUT_VALUE);
            }
        } else if (int_reg_voltage_ptr->int_reg_component_type == CAPI_INTERNAL_REGULATOR_COMPONENT_AVDD) {
            if (int_reg_voltage_ptr->voltage > 950 || int_reg_voltage_ptr->voltage < 850) {
                return(RR_ERROR_WRONG_INPUT_VALUE);
            }
        }
        voltage = int_reg_voltage_ptr->voltage;
    }

    fw_phy.base_addr = MGT;

    if (int_reg_voltage_ptr->int_reg_component_type == CAPI_INTERNAL_REGULATOR_COMPONENT_VDDM) {
        chal_ana_vddm_set_volt(&fw_phy, (voltage * 1000));
        chal_ana_vddm_get_volt(&fw_phy, &read_value);
    } else if (int_reg_voltage_ptr->int_reg_component_type == CAPI_INTERNAL_REGULATOR_COMPONENT_AVDD) {
        chal_ana_avdd_set_volt(&fw_phy, (voltage * 1000));
        chal_ana_avdd_get_volt(&fw_phy, &read_value);
    } else {
        return(RR_ERROR_WRONG_INPUT_VALUE);
    }

    int_reg_voltage_ptr->returned_voltage = read_value/1000;
   
    return(return_result);
}



return_result_t chal_ana_reg_enable_vddm_slave(phy_info_t* phy_info_ptr) 
{
    uint16_t rd_data;
    uint16_t wr_data;

    /* Do a read modify write to reg0 */
    if(chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_DVDDM_SLAVE_PHY_ID, ANA_REG_0,  0x0, &rd_data)!=RR_SUCCESS){
        return RR_ERROR;
    }

    wr_data =  rd_data & 0xFFFE; // set soft-reset
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_DVDDM_SLAVE_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    /* Toggle by writing 1 to reg0[1] and then 0 to complete the transaction */
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_DVDDM_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data | 0x0002;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_DVDDM_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_DVDDM_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data & 0xFFFD;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_DVDDM_MASTER_PHY_ID, ANA_REG_0,wr_data, &rd_data);

    return RR_SUCCESS;
}

return_result_t chal_ana_reg_disable_avdd_slave(phy_info_t* phy_info_ptr) 
{
    uint16_t rd_data;
    uint16_t wr_data;

    /* Do a read modify write to reg0 */
    if(chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVDD_SLAVE_PHY_ID, ANA_REG_0,  0x0, &rd_data)!=RR_SUCCESS){
        return RR_ERROR;
    }

    wr_data =  rd_data | 0x0001; // set soft-reset
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVDD_SLAVE_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    /* Toggle by writing 1 to reg0[1] and then 0 to complete the transaction */
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVDD_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data | 0x0002;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVDD_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVDD_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data & 0xFFFD;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVDD_MASTER_PHY_ID, ANA_REG_0,wr_data, &rd_data);

   return RR_SUCCESS;
}

return_result_t chal_ana_reg_enable_avdd_slave(phy_info_t* phy_info_ptr) {
    uint16_t rd_data;
    uint16_t wr_data;

    /* Do a read modify write to reg0 */
    if(chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVDD_SLAVE_PHY_ID, ANA_REG_0,  0x0, &rd_data)!=RR_SUCCESS){
        return RR_ERROR;
    }

    wr_data =  rd_data & 0xFFFE; // set soft-reset
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVDD_SLAVE_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    /* Toggle by writing 1 to reg0[1] and then 0 to complete the transaction */
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVDD_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data | 0x0002;
    chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_WRITE, ANA_AVDD_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);

    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVDD_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
    wr_data = rd_data & 0xFFFD;
    chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_WRITE, ANA_AVDD_MASTER_PHY_ID, ANA_REG_0,wr_data, &rd_data);

    return RR_SUCCESS;
}

/**
 * @brief host_util_ana_disable_all_regs (phy_info_t* phy_info_ptr, capi_regulator_info_t* regulator_info_ptr)
 * @details  This utility function is used to enable/disable regulator component
 *
 * @param  phy_info_ptr : a reference to the phy information object
 * @param  regulator_info_ptr : a reference to the regulator information object
 * @return return_result : returns the outcome of the function call
 */
return_result_t   host_util_ana_disable_all_regs (phy_info_t* phy_info_ptr,capi_regulator_info_t* regulator_info_ptr) {
   uint16_t rd_data = 0;
   uint16_t wr_data = 0;
   regulator_component_t regulator_component = regulator_info_ptr->component;

   

     /* Do a read modify write to reg0 */
   if((regulator_component==REGULATOR_COMPONENT_MASTER_ALL) || (
       regulator_component==REGULATOR_COMPONENT_MASTER_AVDD)) {
       if(chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVDD_MASTER_PHY_ID, ANA_REG_0,  0x0, &rd_data)!=RR_SUCCESS){
          return RR_ERROR;
       }
   

       wr_data =  rd_data | 0x0001; // set soft-reset
       chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_WRITE, ANA_AVDD_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);

       /* Toggle by writing 1 to reg0[1] and then 0 to complete the transaction */
       chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_READ, ANA_AVDD_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
       wr_data = rd_data | 0x0002;
       chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_WRITE, ANA_AVDD_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);

       chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVDD_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
       wr_data = rd_data & 0xFFFD;
       chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_WRITE, ANA_AVDD_MASTER_PHY_ID, ANA_REG_0,wr_data, &rd_data);
   }
   /* Do a read modify write to reg0 */
    if((regulator_component==REGULATOR_COMPONENT_MASTER_ALL) || (
       regulator_component==REGULATOR_COMPONENT_MASTER_VDDM)) {
       if(chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_READ, ANA_DVDDM_MASTER_PHY_ID, ANA_REG_0,  0x0, &rd_data)!=RR_SUCCESS){
          return RR_ERROR;
       }

       wr_data =  rd_data | 0x0001; // set soft-reset
       chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_WRITE, ANA_DVDDM_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);

       /* Toggle by writing 1 to reg0[1] and then 0 to complete the transaction */
       chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_READ, ANA_DVDDM_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
       wr_data = rd_data | 0x0002;
       chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_WRITE, ANA_DVDDM_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);

       chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_DVDDM_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
        wr_data = rd_data & 0xFFFD;
       chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_WRITE, ANA_DVDDM_MASTER_PHY_ID, ANA_REG_0,wr_data, &rd_data);
    }
   /* Do a read modify write to reg0 */
    if((regulator_component==REGULATOR_COMPONENT_MASTER_ALL) || (
       regulator_component==REGULATOR_COMPONENT_MASTER_AVS)) {
       if(chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0,  0x0, &rd_data)!=RR_SUCCESS){
          return RR_ERROR;
       }

       wr_data =  rd_data | 0x0001; // set soft-reset
       chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);
       chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_WRITE, ANA_AVS_SLAVE1_PHY_ID, ANA_REG_0, wr_data, &rd_data);
       chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_WRITE, ANA_AVS_SLAVE2_PHY_ID, ANA_REG_0, wr_data, &rd_data);
       chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_WRITE, ANA_AVS_SLAVE3_PHY_ID, ANA_REG_0, wr_data, &rd_data);
       chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_WRITE, ANA_AVS_SLAVE4_PHY_ID, ANA_REG_0, wr_data, &rd_data);

       /* Toggle by writing 1 to reg0[1] and then 0 to complete the transaction */
       chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
       wr_data = rd_data | 0x0002;
       chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, wr_data, &rd_data);

       chal_ana_reg_rdwr_access(phy_info_ptr, ANA_REG_READ, ANA_AVS_MASTER_PHY_ID, ANA_REG_0, 0x0, &rd_data);
        wr_data = rd_data & 0xFFFD;
       chal_ana_reg_rdwr_access (phy_info_ptr, ANA_REG_WRITE, ANA_AVS_MASTER_PHY_ID, ANA_REG_0,wr_data, &rd_data);
    }
    if(!(regulator_component & REGULATOR_COMPONENT_MASTER_ALL)) {
        return(RR_ERROR_WRONG_INPUT_VALUE);
     }
   return RR_SUCCESS;
}
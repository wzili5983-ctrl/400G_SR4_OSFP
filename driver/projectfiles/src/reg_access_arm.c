/** @file reg_access_arm.c @brief Native ARM CAPI access without USB/COM/FPGA. */
#include "reg_access.h"
#include "bcm_arm_platform.h"
#include "bcm_gd32_i2c.h"
static uint32_t i2c_target = BCM_DSP_I2C_ADDRESS_7BIT;
static uint16_t mdio_address[32];
long rd_reg_ex(long address, long port)
{
    uint32_t value = UINT32_MAX;
    BcmArmRead(BCM_ARM_MEMORY, (uint32_t)port, (uint32_t)address, &value, 4);
    return (long)value;
}
void wr_reg_ex(long address, long data, long port)
{
    uint32_t value = (uint32_t)data;
    BcmArmWrite(BCM_ARM_MEMORY, (uint32_t)port, (uint32_t)address, &value, 4);
}
int rd16_reg_ex(long address, long port)
{
    uint16_t value = UINT16_MAX;
    if (BcmArmRead(BCM_ARM_MDIO, (uint32_t)port, (uint32_t)address, &value, 2)) { return -1; }
    return value;
}
void wr16_reg_ex(long address, long data, long port)
{
    uint16_t value = (uint16_t)data;
    BcmArmWrite(BCM_ARM_MDIO, (uint32_t)port, (uint32_t)address, &value, 2);
}
uint32_t rd8_reg_ex(uint32_t address, uint32_t port)
{
    uint8_t value = UINT8_MAX;
    BcmArmRead(BCM_ARM_MEMORY, port, address, &value, 1);
    return value;
}
void wr8_reg_ex(uint32_t address, uint32_t data, uint32_t port)
{
    uint8_t value = (uint8_t)data;
    BcmArmWrite(BCM_ARM_MEMORY, port, address, &value, 1);
}
unsigned char rd_qsfp_reg_ex(long address, long port)
{
    uint8_t value = UINT8_MAX;
    BcmArmRead(BCM_ARM_I2C, (uint32_t)port, (uint32_t)address, &value, 1);
    return value;
}
void wr_qsfp_reg_ex(long address, long data, long port)
{
    uint8_t value = (uint8_t)data;
    BcmArmWrite(BCM_ARM_I2C, (uint32_t)port, (uint32_t)address, &value, 1);
}
void set_i2c_driver_target_address(uint8_t target)
{
    uint8_t address7 = target > 0x7FU ? (uint8_t)(target >> 1) : target;
    if (target > 0x7FU && (target & 1U) != 0) { BcmArmSetError(BCM_ARM_INVALID); return; }
    if (address7 < 8 || address7 > 0x77) { BcmArmSetError(BCM_ARM_INVALID); return; }
    BcmGd32SetTarget(address7);
    i2c_target = address7;
}
void display_settings_view(unsigned int view_on)
{
    (void)view_on;
    BcmArmSetError(BCM_ARM_UNSUPPORTED);
}
void set_usb_com(int port) { (void)port; BcmArmSetError(BCM_ARM_UNSUPPORTED); }
void addrmdio45(uint32_t phy_id, uint32_t address)
{
    if (phy_id >= 32 || address > UINT16_MAX) { BcmArmSetError(BCM_ARM_INVALID); return; }
    mdio_address[phy_id] = (uint16_t)address;
    BcmArmSetError(0);
}
void writemdio45(uint32_t phy_id, uint32_t value)
{
    if (phy_id >= 32) { BcmArmSetError(BCM_ARM_INVALID); return; }
    wr16_reg_ex((long)(0x1F0000U | mdio_address[phy_id]), (long)value, (long)phy_id);
}
int readmdio45(int phyad, int devad)
{
    if ((unsigned int)phyad >= 32 || (unsigned int)devad >= 32) { BcmArmSetError(BCM_ARM_INVALID); return -1; }
    return rd16_reg_ex((long)(((uint32_t)devad << 16) | mdio_address[phyad]), phyad);
}
return_result_t burst_read_chip_sram(uint32_t address, uint32_t size, uint32_t *data, long port)
{
    return BcmArmRead(BCM_ARM_MEMORY, (uint32_t)port, address, data, size) == 0 ? RR_SUCCESS : RR_ERROR_MDIO;
}
return_result_t burst_write_chip_sram(uint32_t address, uint32_t size, uint32_t *data, long port)
{
    return BcmArmWrite(BCM_ARM_MEMORY, (uint32_t)port, address, data, size) == 0 ? RR_SUCCESS : RR_ERROR_MDIO;
}
/* Vendor callers interpret zero as success, despite the bool return type. */
bool burst_write_mdio(uint32_t address, uint32_t size, uint32_t *data, int port)
{
    return burst_write_chip_sram(address, size, data, port) != RR_SUCCESS;
}
bool burst_write_i2c(uint32_t address, uint32_t size, uint32_t *data)
{
    return BcmArmWrite(BCM_ARM_MEMORY, i2c_target, address, data, size) != 0;
}

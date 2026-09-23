/** @file bcm_arm_platform.c @brief Checked ARM transport dispatch. */
#include "bcm_arm_platform.h"
#include <string.h>
static BcmArmBus transport;
static int last_error = BCM_ARM_NO_BUS;
void BcmArmSetBus(const BcmArmBus *bus)
{
    if (bus != NULL) { transport = *bus; }
    else { memset(&transport, 0, sizeof(transport)); }
    last_error = (transport.read != NULL && transport.write != NULL) ? 0 : BCM_ARM_NO_BUS;
}
int BcmArmLastError(void) { return last_error; }
void BcmArmSetError(int error) { last_error = error; }
static int valid(uint32_t address, const void *data, uint32_t size)
{
    return data != NULL && size != 0 && size - 1 <= UINT32_MAX - address;
}
int BcmArmRead(BcmArmSpace space, uint32_t target, uint32_t address, void *data, uint32_t size)
{
    if (!valid(address, data, size)) { return last_error = BCM_ARM_INVALID; }
    if (transport.read == NULL) { last_error = BCM_ARM_NO_BUS; }
    else { last_error = transport.read(space, target, address, data, size); }
    if (last_error != 0) { memset(data, 0xFF, size); }
    return last_error;
}
int BcmArmWrite(BcmArmSpace space, uint32_t target, uint32_t address, const void *data, uint32_t size)
{
    if (!valid(address, data, size)) { return last_error = BCM_ARM_INVALID; }
    return last_error = transport.write == NULL ? BCM_ARM_NO_BUS : transport.write(space, target, address, data, size);
}

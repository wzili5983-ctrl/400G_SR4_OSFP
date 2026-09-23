/** @file bcm_arm_platform.h
 * @brief Bare-metal transport contract. Register callbacks before calling CAPI.
 * Callbacks return 0 on success, a negative error on failure, and must have a
 * bounded timeout. Serialize CAPI access externally (including interrupt use).
 * DSP memory buffers use little-endian byte order. I2C target uses the vendor
 * address convention; the board backend is responsible for address conversion.
 * Legacy scalar CAPI reads return all-ones on failure; inspect BcmArmLastError
 * immediately after access. The vendor void write API cannot return status.
 */
#ifndef BCM_ARM_PLATFORM_H
#define BCM_ARM_PLATFORM_H
#include <stdint.h>
#include <stddef.h>
enum { BCM_ARM_OK = 0, BCM_ARM_NO_BUS = -1, BCM_ARM_INVALID = -2,
       BCM_ARM_UNSUPPORTED = -3 };
typedef enum { BCM_ARM_MEMORY, BCM_ARM_MDIO, BCM_ARM_I2C } BcmArmSpace;
typedef struct {
    int (*read)(BcmArmSpace space, uint32_t target, uint32_t address, void *data, uint32_t size);
    int (*write)(BcmArmSpace space, uint32_t target, uint32_t address, const void *data, uint32_t size);
} BcmArmBus;
void BcmArmSetBus(const BcmArmBus *bus);
int BcmArmRead(BcmArmSpace space, uint32_t target, uint32_t address, void *data, uint32_t size);
int BcmArmWrite(BcmArmSpace space, uint32_t target, uint32_t address, const void *data, uint32_t size);
int BcmArmLastError(void);
void BcmArmSetError(int error);
/* Debugger-visible circular log, no semihosting or filesystem required. */
extern char bcm_arm_log[1024];
extern volatile uint32_t bcm_arm_log_written;
#endif

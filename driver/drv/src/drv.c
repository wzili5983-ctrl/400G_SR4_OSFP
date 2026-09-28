/** @file drv.c
 * @brief GN1848 (DRV) laser driver implementation.
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 */
#include "drv.h"
#include "chipresource.h"
#include "busresource.h"

/* 8-bit register space; higher addresses are not selectable. */
#define DRV_ADDR_MAX 0xFFU

DWORD InitDrv(BYTE rsttype)
{
    CONST ChipInfo_t *info = GetChipInfoById(CHIP_ID_DRV);
    DWORD id0;
    (void)rsttype;
    if(info == NULL || InitBus(info->chipform.busid) != OK) {
        return ERR;
    }
    /* Liveness check: ID0 must read back the device ID (0x30). */
    if(DrvRead(CHIP_ID_DRV, DRV_REG_ID0, &id0) != OK || id0 != DRV_ID0_VALUE) {
        return ERR;
    }
    return OK;
}

DWORD DrvWrite(BYTE id, DWORD addr, DWORD data)
{
    CONST ChipInfo_t *info = GetChipInfoById(id);
    BYTE tx[2];
    if(info == NULL || addr > DRV_ADDR_MAX) {
        return ERR;
    }
    tx[0] = (BYTE)addr;
    tx[1] = (BYTE)data;
    /* The chip id selects the ChipInfo_t entry; its busid selects the bus. */
    return WriteBus(info->chipform.busid, tx, (BYTE)sizeof(tx));
}

DWORD DrvRead(BYTE id, DWORD addr, DWORD *data)
{
    CONST ChipInfo_t *info = GetChipInfoById(id);
    BYTE reg, value = 0U;
    if(info == NULL || addr > DRV_ADDR_MAX || data == NULL) {
        return ERR;
    }
    /* Random read per DS 3.5: the dummy write loads the address pointer and
     * stays valid until the next access or RSTB, so a current-address read
     * returns the target register. */
    reg = (BYTE)addr;
    if(WriteBus(info->chipform.busid, &reg, 1U) != OK) {
        return ERR;
    }
    if(ReadBus(info->chipform.busid, &value, 1U) != OK) {
        return ERR;
    }
    *data = value;
    return OK;
}

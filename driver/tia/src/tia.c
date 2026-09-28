/** @file tia.c
 * @brief GN1816 (TIA) register implementation.
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 */
#include "tia.h"
#include "chipresource.h"
#include "busresource.h"

/* 8-bit register space; higher addresses are not selectable. */
#define TIA_ADDR_MAX 0xFFU

DWORD InitTia(BYTE rsttype)
{
    CONST ChipInfo_t *info = GetChipInfoById(CHIP_ID_TIA);
    DWORD id1;
    (void)rsttype;
    if(info == NULL || InitBus(info->chipform.busid) != OK) {
        return ERR;
    }
    /* Liveness check: DEVICE_ID1 must read back the device ID (0x19). */
    if(TiaRead(CHIP_ID_TIA, TIA_REG_DEVICE_ID1, &id1) != OK || id1 != TIA_ID1_VALUE) {
        return ERR;
    }
    return OK;
}

DWORD TiaWrite(BYTE id, DWORD addr, DWORD data)
{
    CONST ChipInfo_t *info = GetChipInfoById(id);
    BYTE tx[2];
    if(info == NULL || addr > TIA_ADDR_MAX) {
        return ERR;
    }
    tx[0] = (BYTE)addr;
    tx[1] = (BYTE)data;
    /* The chip id selects the ChipInfo_t entry; its busid selects the bus. */
    return WriteBus(info->chipform.busid, tx, (BYTE)sizeof(tx));
}

DWORD TiaRead(BYTE id, DWORD addr, DWORD *data)
{
    CONST ChipInfo_t *info = GetChipInfoById(id);
    BYTE reg, value = 0U;
    if(info == NULL || addr > TIA_ADDR_MAX || data == NULL) {
        return ERR;
    }
    /* Random read per DS 3.12: the dummy write loads the address pointer and
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

/** @file busresource.c
 * @brief Board resource implementation.
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 */
#include "busresource.h"

DWORD InitBus(BYTE id)
{
    CONST BusInfo_t *info = GetBusInfo(id);
    return info != NULL && info->busops.BusInit != NULL ? info->busops.BusInit(id) : ERR;
}

DWORD WriteBus(BYTE id, CONST BYTE *tx, BYTE txlen)
{
    CONST BusInfo_t *info = GetBusInfo(id);
    /* The transfer address comes from the bus config (i2c.i2caddr); the
     * implementation validates transfer arguments, including direct calls. */
    if(info == NULL || info->busops.BusWrite == NULL) {
        return ERR;
    }
    return info->busops.BusWrite(&info->bustotalform, tx, txlen);
}

DWORD ReadBus(BYTE id, BYTE *rx, BYTE rxlen)
{
    CONST BusInfo_t *info = GetBusInfo(id);
    if(info == NULL || info->busops.BusRead == NULL) {
        return ERR;
    }
    return info->busops.BusRead(&info->bustotalform, rx, rxlen);
}

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

DWORD TransferBus(BYTE id, BYTE address7, CONST BYTE *tx, BYTE txlen, BYTE *rx, BYTE rxlen)
{
    CONST BusInfo_t *info = GetBusInfo(id);
    /* The implementation validates transfer arguments, including direct calls. */
    if(info == NULL || info->busops.BusTransfer == NULL) {
        return ERR;
    }
    return info->busops.BusTransfer(id, address7, tx, txlen, rx, rxlen);
}

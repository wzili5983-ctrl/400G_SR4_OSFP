/** @file busresource.h
 * @brief Board resource implementation.
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 */
#ifndef BUSRESOURCE_H
#define BUSRESOURCE_H

#include "busresource_impl.h"

DWORD InitBus(BYTE id);
DWORD WriteBus(BYTE id, CONST BYTE *tx, BYTE txlen);
DWORD ReadBus(BYTE id, BYTE *rx, BYTE rxlen);

#endif

/** @file boardresource.h
 * @brief Board resource implementation.
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 */
#ifndef BOARDRESOURCE_H
#define BOARDRESOURCE_H

#include "basedef.h"

/* Apply documented GPIO defaults and initialize ADCs and master buses.
 * Does not sequence power rails or configure an unspecified host slave protocol.
 * Call once before using resources; ERR indicates incomplete initialization. */
DWORD InitBoardResources(VOID);

#endif

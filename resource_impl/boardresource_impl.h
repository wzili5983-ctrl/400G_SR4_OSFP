/** @file boardresource_impl.h
 * @brief Board resource initialization implementation interface.
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 */
#ifndef BOARDRESOURCE_IMPL_H
#define BOARDRESOURCE_IMPL_H

#include "basedef.h"

typedef struct {
    DWORD (*BoardInit)(VOID);
}BoardOps_t;

CONST BoardOps_t *GetBoardOps(VOID);

#endif

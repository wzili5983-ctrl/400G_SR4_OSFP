/** @file boardresource.c
 * @brief Board resource implementation.
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 */
#include "boardresource.h"
#include "boardresource_impl.h"

DWORD InitBoardResources(VOID)
{
    CONST BoardOps_t *ops = GetBoardOps();
    if(ops == NULL || ops->BoardInit == NULL) {
        return ERR;
    }
    return ops->BoardInit();
}

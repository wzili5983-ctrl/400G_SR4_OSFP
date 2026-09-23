/** @file boardresource_impl.c
 * @brief Board resource implementation.
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 */
#include "boardresource_impl.h"
#include "gpioresource.h"
#include "adcresource.h"
#include "busresource.h"

STATIC DWORD BoardInit(VOID);

STATIC CONST BoardOps_t g_BoardOps = {BoardInit};

CONST BoardOps_t *GetBoardOps(VOID)
{
    return &g_BoardOps;
}

STATIC DWORD BoardInit(VOID)
{
    BYTE id;
    for(id = 0; id < GPIO_ID_NUM; ++id) {
        if(InitGpio(id, 0) != OK) {
            return ERR;
        }
    }
    for(id = 0; id < ADC_ID_NUM; ++id) {
        if(InitAdc(id, 0) != OK) {
            return ERR;
        }
    }
    if(InitBus(BUS_ID_PHY) != OK || InitBus(BUS_ID_INNER) != OK) {
        return ERR;
    }
    return OK;
}

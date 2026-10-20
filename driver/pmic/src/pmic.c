/** @file pmic.c
 * @brief SY70300B (PMIC) register implementation.
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 */
#include "pmic.h"
#include "chipresource.h"
#include "busresource.h"

/* 8-bit register space per DS "Data Transactions". */
#define PMIC_ADDR_MAX 0xFFU

DWORD InitPmic(BYTE rsttype)
{
    CONST ChipInfo_t *info = GetChipInfoById(CHIP_ID_PMIC);
    DWORD chipname;
    (void)rsttype;
    if(info == NULL || InitBus(info->chipform.busid) != OK) {
        return ERR;
    }
    /* Liveness check: IO_CHIPNAME must read back (ACK). The value itself is
     * OTP-programmable (DS default 0xFF), so no fixed magic can be asserted;
     * a successful read means the slave responded at 0x1E on BUS_ID_PMIC. */
    if(PmicRead(CHIP_ID_PMIC, PMIC_REG_IO_CHIPNAME, &chipname) != OK) {
        return ERR;
    }
    return OK;
}

DWORD PmicWrite(BYTE id, DWORD addr, DWORD data)
{
    CONST ChipInfo_t *info = GetChipInfoById(id);
    BYTE tx[2];
    if(info == NULL || addr > PMIC_ADDR_MAX) {
        return ERR;
    }
    tx[0] = (BYTE)addr;
    tx[1] = (BYTE)data;
    /* The chip id selects the ChipInfo_t entry; its busid selects the bus. */
    return WriteBus(info->chipform.busid, tx, (BYTE)sizeof(tx));
}

DWORD PmicRead(BYTE id, DWORD addr, DWORD *data)
{
    CONST ChipInfo_t *info = GetChipInfoById(id);
    BYTE reg, value = 0U;
    if(info == NULL || addr > PMIC_ADDR_MAX || data == NULL) {
        return ERR;
    }
    /* Random read per DS "Data Transactions": the dummy write loads the
     * register address byte (slave ACKs it) and the pointer is expected to
     * hold, so a current-address read returns the target register. The DS
     * figure draws a repeated-START variant; the bus layer has no repeated-
     * start primitive (WriteBus/ReadBus are separate transactions) — if
     * reads misbehave on the bench, add a WriteReadBus with repeated start
     * (★ verify on hardware). */
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

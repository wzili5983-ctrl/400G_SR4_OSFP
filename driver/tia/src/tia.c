/** @file tia.c
 * @brief GN1816 (TIA) register implementation.
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 */
#include "tia.h"
#include "chipresource.h"
#include "busresource.h"
#include "adpat.h"              /* ReadSramProtocolMsaSpace：B6H 影子 */
#include "ral.h"                /* PAGE_ID_B6H / PAGE_BXH_* 布局宏 */

/* 8-bit register space; higher addresses are not selectable. */
#define TIA_ADDR_MAX 0xFFU

/* B6H/B7H 寄存器初始化页消费（0x00=REGNUM u8 + N×{ADDR u16BE@0x04+4n,
 * VALUE u16BE@0x06+4n}，布局见 ral.h PAGE_BXH_*）。GN1816 为 8bit 寄存器
 * 空间，ADDR 高字节（宽度）必须 0x00。B6H=主表、B7H=扩展页，InitTia
 * 顺序消费两页。 */
STATIC DWORD TiaApplyRegInitPage(TabIndex_t pageid)
{
    BYTE page[128U];            /* 页大小 128B */
    BYTE regnum;
    BYTE n;

    /* 整页快照可能被从机 ISR 的并发 host 写撕裂（memory_map.md R5「整页写
     * 窗口」）。包影子锁使 ISR 不得在拷贝期内写影子，快照原子一致
     * （方案 A：AdpatShadowLock/Unlock，取代原双读比对）。 */
    AdpatShadowLock();
    if(ReadSramProtocolMsaSpace(pageid, 0U, page, (BYTE)sizeof(page)) != OK) {
        AdpatShadowUnlock();
        return ERR;
    }
    AdpatShadowUnlock();

    regnum = page[PAGE_BXH_ADDR_REGNUM];             /* REGNUM u8 */
    if(regnum == 0U) { return OK; }                 /* 空表 */
    if(regnum > PAGE_BXH_REGINIT_NUM_MAX) { return ERR; }  /* 防呆 */

    for(n = 0U; n < regnum; ++n) {
        DWORD addr = ((DWORD)page[PAGE_BXH_ADDR_REGADDR(n)] << 8) |
                     (DWORD)page[PAGE_BXH_ADDR_REGADDR(n) + 1U];
        DWORD value = ((DWORD)page[PAGE_BXH_ADDR_REGVALUE(n)] << 8) |
                      (DWORD)page[PAGE_BXH_ADDR_REGVALUE(n) + 1U];
        if((addr >> 8) != 0U) {
            return ERR;         /* GN1816 为 8bit 空间，ADDR 高字节（宽度）必须 0x00 */
        }
        if(TiaWrite(CHIP_ID_TIA, (BYTE)addr, value) != OK) {
            return ERR;         /* 配置写失败 = 芯片不应答，交 InitChip → fault 38 */
        }
    }
    return OK;
}

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
    /* 寄存器初始化页消费（表驱动）：B6H=主表、B7H=扩展页，顺序消费。
     * ID 已确认总线/器件存活，配置失败即真错误（写不进 = 不应答）。 */
    if(TiaApplyRegInitPage(PAGE_ID_B6H) != OK) {
        return ERR;
    }
    if(TiaApplyRegInitPage(PAGE_ID_B7H) != OK) {
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

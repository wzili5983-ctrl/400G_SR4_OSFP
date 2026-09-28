/** @file data_transfer.h
 * @brief MCI 数据搬运上下文：当前模块操作的 page / bank（Lower Memory
 *        byte 126 BankSelect & byte 127 PageSelect）。
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * host 通过写 Lower Memory byte 126/127 切换当前操作的页与存储体。
 * 按 CMIS 5.3 要求，Byte 126 与 Byte 127 必须在同一次 WRITE 中写入，
 * 故二者合入一个结构体、由 DtTabCtrlSet() 一次更新，避免出现"页已换、
 * bank 未换"的中间态。
 *
 * page = PageSelect（byte 127）的当前值，即生效页号 00h-FFh；
 * bank = BankSelect（byte 126）的当前值，仅低 2 位有效（bank 0-3），
 * 仅对 banked 页（如 10h/11h）有意义，非 banked 页可忽略。
 *
 * MCI 在 ISR 中收到写 126/127 的帧后调用 DtTabCtrlSet() 更新上下文；
 * 读相位快照与 RAL 引擎均按此上下文确定"当前页/ bank"。
 */
#ifndef DATA_TRANSFER_H
#define DATA_TRANSFER_H

#include "basedef.h"
#include "ral.h"        /* OsfpRegAttr_t / OsfpTabForm_t / g_OsfpTabForm / TabIndex_t */

/* bank 编号：CMIS banked 页（如 A1h）固定 4 个存储体 */
typedef enum {
    BANK_ID_0 = 0,      /* bank 0 */
    BANK_ID_1,          /* bank 1 */
    BANK_ID_2,          /* bank 2 */
    BANK_ID_3,          /* bank 3 */
    BANK_ID_NUM         /* bank 个数 */
} DtBankId_t;

/* 当前操作的页/存储体上下文（Lower Memory byte 127/126 的镜像）。
 * 上电默认 page=00h、bank=0。 */
typedef struct {
    BYTE page;          /* PageSelect（byte 127）：当前生效页 00h-FFh */
    DtBankId_t bank;    /* BankSelect（byte 126）：bank 0-3（仅低 2 位有效） */
} DtTabCtrl_t;

/* 恢复默认上下文：page 00h / bank 0。上电或模块复位时调用。 */
VOID DtTabCtrlInit(VOID);
/* host 同写 byte 126/127 时更新上下文（page=byte127，bank=byte126）。
 * 必须一次调用完成两个值的更新（CMIS 5.3 同写约束）。 */
VOID DtTabCtrlSet(BYTE page, BYTE bank);
/* 当前生效页号 00h-FFh（PageSelect 值）。 */
BYTE DtTabCtrlGetPage(VOID);
/* 当前 bank 0-3（BankSelect 低 2 位）。 */
BYTE DtTabCtrlGetBank(VOID);

/* 指令解析：按当前窗口地址 memaddr(00h-FFh) 定位寄存器属性。
 * 1) bit7=0（00h-7Fh，Lower 半区）→ Page Low：g_OsfpTabForm[0] 固定直取
 *    （不参与二分，bank 不影响索引；per-bank 的 Flags Summary 属表内数据）；
 *    bit7=1（80h-FFh，Upper 半区）→ page=g_DtTabCtrl.page，从 [1] 起在
 *    g_OsfpTabForm[] 中左偏二分（[0]=Page Low 须跳过——其 tab 与 Page 00h
 *    同为 0x00，半区才是二者的区分键），得 tab 最左匹配项；该项 banked=1
 *    时索引顺延 bank 得匹配项（越界返回 NULL），banked=0 时 CMIS 规定
 *    BankSelect 被忽略，不加偏移。
 * 2) 在匹配项的 OsfpRegAttr_t 表中按 offset 二分（offset = memaddr &
 *    0x7F 页内偏移；Upper 表 offset 成员也存 00h-7Fh，bit7 已被半区
 *    选择剥离）。
 * 返回匹配的属性项指针；未命中（页未登记/Reserved/Custom 区）返回
 * NULL，由调用方按默认处理。表元素数约定 == TabIndex_t 的 PAGE_TAB_NUM。 */
CONST OsfpRegAttr_t *DtParse(BYTE memaddr);

#endif

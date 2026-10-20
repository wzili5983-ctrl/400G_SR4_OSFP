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

/* 当前 Upper 半区页表索引：g_DtTabCtrl（page+bank）的解析结果，读相位快照
 * 按它取页。ISR（DtTabCtrlSet）写、主循环可读，故 volatile。
 * 值为 (TabIndex_t)PAGE_TAB_NUM 表示选中页未登记（Upper 读回 dummy）。
 * 初始未刷新前即为哨兵；DtTabCtrlInit 后随默认上下文解析（= Page 00h）。 */
extern volatile TabIndex_t g_CurPageId;

/* 指令解析：按当前窗口地址 memaddr(00h-FFh) 定位寄存器属性。
 * 1) bit7=0（00h-7Fh，Lower 半区）→ Page Low：g_OsfpTabForm[0] 固定直取
 *    （不参与二分，bank 不影响索引；per-bank 的 Flags Summary 属表内数据）；
 *    bit7=1（80h-FFh，Upper 半区）→ page=g_DtTabCtrl.page，从 [1] 起在
 *    g_OsfpTabForm[] 中左偏二分（[0]=Page Low 须跳过——其 tab 与 Page 00h
 *    同为 0x00，半区才是二者的区分键），得 tab 最左匹配项；该项 banked=1
 *    时索引顺延 bank 得匹配项（越界返回 NULL），banked=0 时 CMIS 规定
 *    BankSelect 被忽略，不加偏移。
 * 2) 在匹配项的 OsfpRegAttr_t 表中按区间包含二分 offset，得「含该 offset
 *    的表项」（offset = memaddr & 0x7F 页内偏移；Upper 表 offset 成员也存
 *    00h-7Fh，bit7 已被半区选择剥离）。inf 表须 offset 升序、区间互不重叠。
 * 返回含该 offset 的属性项指针；未命中（页未登记/空隙/Reserved/Custom 区）
 * 返回 NULL，由调用方按默认处理。表元素数约定 == TabIndex_t 的 PAGE_TAB_NUM。 */
CONST OsfpRegAttr_t *DtParse(BYTE memaddr);

/* 指令解析帧回调（I2C1 EV STPDET 中断内执行，I2cSlaveInit 已默认接线）：
 * 按帧首地址 g_RxBuf[I2C_RX_ADDR] 经 DtParseForm 定位寄存器（内部用
 * g_DtTabCtrl 选页/ bank）后路由——
 *   RO / RO_COR 写   → 丢弃（CMIS：RO 写无效果）；
 *   oprfunc != NULL  → 触发寄存器，须整寄存器写（off==attr->offset 且
 *                      len==attr->size），按 opftype 立即执行或压 deferred
 *                      FIFO（126/127 页/ bank 切换、密码、复位触发）；
 *   纯存储 MEMT_SRAM → WriteProtocolMsaSpace 写影子。CMIS 字节寻址：帧
 *                      起点可在表项任意字节，[off, off+len) 整段须同
 *                      memtype 非 RO（跨 128B 半页 / 异质区 / RO 丢弃）；
 *   MEMT_FLASH/NONE  → 不落存储（Flash 持久化归线程 dirty 队列）。
 * 无写数据 / 页未登记 / 空隙 / Reserved 区直接返回。 */
VOID DtFrameCb(DWORD frame_bytes);

/* 读相位页快照数据源：注册到 I2cSlaveRegisterReadSource（I2cSlaveInit 已
 * 默认接线），读相位 ADDSEND+TR 时 I2C1 EV ISR 内调用。按 memaddr bit7
 * 选影子槽（0→Page Low，1→g_CurPageId，哨兵→0xFF dummy），从 memaddr 起
 * 半区内回卷轮转填充 128B。统一走 SRAM 影子（MEMT_FLASH 页依赖启动镜像）。
 * 纯 SRAM 拷贝，ISR 安全。 */
VOID DtReadSource(BYTE memaddr, BYTE *buf, DWORD len);

/* COR（Clear-On-Read）寄存器读后清除：STPDET 时经 I2cSlaveRegisterReadDone
 * 调用（I2cSlaveInit 已默认接线），I2C1 EV ISR 上下文。清除环形读区间
 * [memaddr, memaddr+rdlen)（mod 128 半页）覆盖到的 ACC_RO_COR 影子字节——
 * 只清 host 实际读过的字节（精确 COR；rdlen 由调用侧饱和到 128）。
 * 半区/页定位规则与 DtReadSource 相同。并发契约：影子字节单写原子；
 * app 层对 COR 字节的置位 RMW 须 BASEPRI 锁内（docs/memory_map.md 三写者
 * 模型），否则与清零交错丢更新。末字节 TI 到 STPDET 的 µs 窗口内新锁存
 * 事件会被一并清掉（概率性单事件丢失，bring-up 可接受）。 */
VOID DtCorClear(BYTE memaddr, BYTE rdlen);

/* deferred 操作 FIFO（深度 16）：ISR 压入，主循环轮询执行。
 * DtDeferredPush 由 DtFrameCb（ISR）压入 oprfunc + 入参副本，满则丢弃返回 ERR；
 * DtDeferredPoll 由主循环取一个执行（1=执行了，0=空）；DtDeferredCount 待处理数。 */
DWORD DtDeferredPush(OprateFunc func, CONST BYTE *cmd, BYTE len);
DWORD DtDeferredPoll(VOID);
DWORD DtDeferredCount(VOID);

/* 后台指令执行服务：主循环每轮调用。把 deferred FIFO 里待处理的 oprfunc
 * 逐项执行至清空。仅在主循环线程上下文运行——deferred oprfunc 允许阻塞
 * （Flash/NV 写），这是 OPF_DEFERRED 与 OPF_IMMEDIATE 的分界。 */
VOID CmdExeBackground(VOID);

#endif

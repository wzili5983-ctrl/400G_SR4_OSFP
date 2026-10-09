/** @file ral_operate.h
 * @brief RAL 寄存器操作回调（oprfunc）声明。
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * oprfunc 是 OsfpRegAttr_t 中每个寄存器可选的读写副作用回调，签名 OprateFunc：
 *   typedef DWORD (*OprateFunc)(BYTE* cmd, BYTE len);
 * 本文件集中声明各寄存器的 oprfunc 实现，供 ral.c 寄存器属性表的 oprfunc
 * 字段引用、RAL 引擎按事务方向调用。
 *
 * 方向约定（引擎按当前事务是读还是写来调；单个 oprfunc 只负责一个方向，
 * 方向由所在寄存器的 acctype 决定）：
 *   - WO / WO-SC（触发位）：仅写方向触发（SoftwareReset、PasswordEntry 等）；
 *   - RWW（校验写）：仅写方向校验，非法则修正 cmd 或返回 ERR 拒绝；
 *   - RO（派生值）：仅读方向回填（如按需计算的 PageChecksum）。
 * cmd 含义：写方向 = host 写入值（oprfunc 可就地修正）；读方向 = 待回填缓冲；
 * len = 字节数；返回 OK/ERR。
 *
 * opftype 关联：每个实现的 OPF_* 见其注释。OPF_IMMEDIATE 由引擎在 ISR/写
 * 路径同步调用；OPF_DEFERRED 由引擎置标志下沉主循环执行。本文件只写「动作
 * 本身」，不实现标志/调度机制——那是引擎的职责。
 *
 * 并发契约（oprfunc 在 ISR（OPF_IMMEDIATE）与主循环（OPF_DEFERRED）双
 * 上下文运行，二者无互斥；违反即引入竞态，引擎不做运行期保护）：
 *   1. 影子字节写者唯一：oprfunc 只允许写归属本回调的影子字节——默认
 *      即所在表项的 offset+size；表项之外的字节（如密码回调写 0x2A
 *      PasswordCmdResult）须为显式设计且全表唯一。两个上下文的回调
 *      RMW 同一字节会静默丢更新（M33 仅保证对齐单字节访存原子）。读侧
 *      并发（DtReadSource 快照）与单写者组合安全：逐字节非旧即新。
 *   2. 表项区间互不重叠：同一页表内各 oprfunc 表项（含外溢字节声明）
 *      两两不相交，IMMEDIATE 与 DEFERRED 表项亦不得共享区间——新增
 *      寄存器先核表再实现。
 *   3. 上下文禁忌：DEFERRED oprfunc 禁止调用 DtTabCtrlSet / 改写
 *      g_CurPageId（多字节非原子，与读相位快照撕裂会让 host 整页读错）；
 *      需要切页/ bank 上下文的寄存器一律配 OPF_IMMEDIATE。ISR 内
 *      oprfunc 禁止阻塞与 Flash 写（XIP 停等）——需要二者的指令一律
 *      配 OPF_DEFERRED。
 */
#ifndef RAL_OPERATE_H
#define RAL_OPERATE_H

#include "basedef.h"

/* Page Low 0x1A Module Control（混合字节：bit3 SoftwareReset 为 WO/SC，其余
 * 位普通 RW）：bit3=1 → NVIC_SystemReset() 直接内核系统复位（不返回）；
 * bit3=0 → 整字节写影子（读回 bit3 恒 0）。
 * 写方向；OPF_IMMEDIATE。 */
DWORD OsfpOperateModuleControl(BYTE *cmd, BYTE len);

/* Page Low 0x76 PasswordChangeEntryArea（WO/SC，U32 大端，CMIS 5.3 §8.2.14）：
 * 已解锁且新值 MSB=0（Host 范围）时更新 g_PwdHost 并置 g_PwdPersistPending；
 * 结果写 0x2A PasswordCmdResult（0010b/0011b）。
 * 写方向；OPF_DEFERRED（主循环 CmdExeBackground 执行）。 */
DWORD OsfpOperatePasswordChange(BYTE *cmd, BYTE len);

/* Page Low 0x7A PasswordEntryArea（WO/SC，U32 大端，CMIS 5.3 §8.2.14）：
 * 与 Module/Host 当前密码比对，命中解锁（0001b/0010b）、未命中回锁（0011b）；
 * 结果写 0x2A PasswordCmdResult。
 * 写方向；OPF_DEFERRED。 */
DWORD OsfpOperatePasswordEntry(BYTE *cmd, BYTE len);

/* 密码子系统状态（ral_operate.c 定义）：参数管理/应用层接口。
 *   g_PwdHost            运行时 Host 密码（启动时以 NV 值覆写，改密后须持久化）
 *   g_PwdUnlocked        非 0 = 密码保护功能已解锁（无效 entry 或复位后回锁）
 *   g_PwdPersistPending  非 0 = Host 密码已变更待持久化（参数管理写 NV 后清零） */
extern DWORD g_PwdHost;
extern volatile BYTE g_PwdUnlocked;
extern volatile BYTE g_PwdPersistPending;

/* Page Low 0x7E-0x7F BankSelect+PageSelect 合并域（RWW，2B，CMIS 5.3 二者
 * 须同一次 WRITE）：校验并切换当前页/ bank（DtTabCtrlSet），同时维护 126/127
 * 影子供 host 读回；写不支持的页则自清零回 Page 00h，bank 越界保持原值。
 * cmd[0]=BankSelect(126)，cmd[1]=PageSelect(127)，len 必须==2（单写 127
 * 已被 RAL 表结构丢弃，到不了这里）。
 * 写方向；OPF_IMMEDIATE（纯内存查表/写影子，无阻塞）。 */
DWORD OsfpOperateBankPageSelect(BYTE *cmd, BYTE len);

#endif /* RAL_OPERATE_H */

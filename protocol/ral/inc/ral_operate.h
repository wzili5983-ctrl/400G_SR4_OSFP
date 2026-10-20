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

/* Page 10h SCS0::ApplyDPInitLane1-8（0x0F，WO/SC，CMIS 5.3 Table 8-70）：
 * Provision 触发——把 SCS0（Page 10h 当前 bank）的 DPConfig/Tx/Rx 控制
 * 拷贝到 ACS（Page 11h 同 bank），完成后调 DpsmApplyDpInit(cmd[0]) 置
 * ConfigStatus/DPInitPending 并允许 DPInit。cmd[0]=host 写入的 lane 位图，
 * len 必须==1（Apply 触发须单字节 WRITE，多字节写拒收）。
 * 回调 OPF_IMMEDIATE：纯 SRAM 影子拷贝 + 内存表操作，无阻塞；且须在 host
 * 切页/bank 的同一上下文内同步执行（DEFERRED 会因 bank 已切换而拷错 bank）。
 * 写方向；OPF_IMMEDIATE。 */
DWORD OsfpOperateApplyDpInit(BYTE *cmd, BYTE len);

/* Page D9h 通用芯片寄存器读写邮箱（布局见 ral.h「D9H 布局」块）：
 * 0x01-0x04 REGADDR u32BE——host 写触发芯片读事务 ReadChip(CHIPID, REGADDR)，
 * 结果大端回填 0x09-0x0C REGRBVALUE；读事务失败 REGRBVALUE 保持旧值。
 * 0x05-0x08 REGVALUE u32BE——host 写触发芯片写事务 WriteChip(CHIPID, REGADDR,
 * REGVALUE)。CHIPID 取 0x00 影子（host 早一步纯存储写入）；len 必须==4
 * （帧首=本寄存器且单写 4B 才触发，顺序写整记录不触发）。
 * 两回调均 OPF_DEFERRED：芯片 I2C 事务（软 I2C bit-bang ~µs/bit）禁止在
 * STPDET ISR 内执行；影子持久化/REGRBVALUE 回填在主循环 DtDeferredPoll 生效，
 * host 不可即写即读。写方向；触发寄存器值由回调自行持久化到影子
 * （oprfunc 接管语义，引擎不写影子——host 读回一致）。
 * 并发契约：影子写者唯一——本回调写 REGADDR/REGVALUE（持久化）与 REGRBVALUE
 * （回填）均 D9H 表项内；CHIPID/REGADDR 从影子读，4B 非原子读窗口由 host
 * 协议规避（顺序写 CHIPID→REGADDR→REGVALUE，引擎不互斥）。 */
DWORD OsfpOperateD9hRegAddr(BYTE *cmd, BYTE len);
DWORD OsfpOperateD9hRegValue(BYTE *cmd, BYTE len);

/* Page D9h GPIO 记录（0x0D-0x0F，布局见 ral.h「D9H 布局」块）：
 * 0x0D GPIOID u8——host 写触发读事务 ReadGpio(GPIOID)，结果回填 0x0F GPIOSTATUS；
 * 0x0E GPIOVALUE u8——host 写触发写事务 WriteGpio(GPIOID, 非零→IO_HIGH/零→IO_LOW)。
 * GPIOID 取 host 本次写值（读事务）或 0x0D 影子（写事务，host 早一步写入）；
 * len 必须==1（帧首=本寄存器且单写 1B 才触发，顺序写整记录不触发）。
 * 两回调均 OPF_IMMEDIATE：GPIO 直读直写（寄存器级访问、无 I2C/无阻塞），
 * ISR 安全；影子持久化/GPIOSTATUS 回填即时生效，host 可同步读回。
 * 并发契约：GPIOSTATUS 唯一写者是 OsfpOperateD9hGpioId（表项之外字节，显式
 * 设计）；GPIOVALUE 回调只读 GPIOID 影子、不写 GPIOSTATUS。 */
DWORD OsfpOperateD9hGpioId(BYTE *cmd, BYTE len);
DWORD OsfpOperateD9hGpioValue(BYTE *cmd, BYTE len);

/* Page D9h DAC 记录（0x10-0x18，布局见 ral.h「D9H 布局」块）：
 * 0x10 DACID u8——host 写触发读事务 ReadDac(DACID)，结果回填 0x15-0x18 DACRDVALUE；
 * 0x11-0x14 DACVALUE u32BE——host 写触发写事务 WriteDac(DACID, Be32(DACVALUE))。
 * DACID 取 host 本次写值（读事务）或 0x10 影子（写事务，host 早一步写入）；
 * 触发须单写（DACID len==1、DACVALUE len==4；顺序写整记录不触发）。
 * 两回调均 OPF_DEFERRED：DAC 可能挂在外部芯片上走 I2C（禁止在 STPDET ISR 内
 * 执行）；影子持久化/DACRDVALUE 回填在主循环下一次 DtDeferredPoll 生效，host
 * 不可即写即读须轮询。
 * 并发契约：DACRDVALUE 唯一写者是 OsfpOperateD9hDacId（表项之外字节，显式
 * 设计）；DACVALUE 回调只读 DACID 影子、不写 DACRDVALUE。 */
DWORD OsfpOperateD9hDacId(BYTE *cmd, BYTE len);
DWORD OsfpOperateD9hDacValue(BYTE *cmd, BYTE len);

/* Page D9h ADC 记录（0x19-0x1D，布局见 ral.h「D9H 布局」块）：
 * 0x19 ADCID u8——host 写触发读事务 ReadAdc(ADCID)，结果回填 0x1A-0x1D ADCVALUE。
 * ADCID 取 host 本次写值（读事务，ADC 只读、无写事务通道）；len 必须==1
 * （帧首=本寄存器且单写 1B 才触发，顺序写整记录不触发）。
 * 回调 OPF_DEFERRED：ADC 可能走外部芯片 I2C（禁止在 STPDET ISR 内执行）；
 * 影子持久化/ADCVALUE 回填在主循环下一次 DtDeferredPoll 生效，host 不可
 * 即写即读须轮询。
 * 并发契约：ADCVALUE 唯一写者是 OsfpOperateD9hAdcId（表项之外字节，显式
 * 设计）。 */
DWORD OsfpOperateD9hAdcId(BYTE *cmd, BYTE len);

/* Page 9Fh CMDID（0x00-0x01，u16BE，CMIS 5.3 §8.23 CDB 命令触发）：
 * host 最后一次单独写 2B CMDID 触发命令——转发到 CdbTrigger（cdb.c）置
 * busy+In Progress+pending+前台 NACK hold-off，真正执行下沉主循环 CdbExeBackground。
 * cmd=2 字节 CMDID（大端），len 必须==2（触发须单写，多字节写拒收）。
 * 写方向；OPF_IMMEDIATE（纯影子写+置标志+寄存器级 hold-off，ISR 安全）。
 * 并发契约：CMDID 影子唯一写者=本回调（持久化供 CdbChkVerify 读）；
 * CdbStatus1(0x25)/完成标志(0x08.6) 由 cdb.c 单写者维护，见 cdb.h。 */
DWORD OsfpOperate9fCmdId(BYTE *cmd, BYTE len);

/* Page 2Fh 0x10 byte 144 VDM FreezeRequest（RW，CMIS 5.3 §8.22）：
 * bit7=1 → VdmFreezeApply（statistic 类实例定格）；bit7=0 → VdmUnfreezeApply
 * （CMIS：host 清 FreezeRequest 即恢复；本页 bit6=UnfreezeReq W1S 亦归此路径）。
 * len 必须==1（帧首=本寄存器且单写 1B 才触发）。oprfunc 接管语义、引擎不落
 * 存储——本回调持久化 host 写入值到 byte 144 影子供读回。
 * 写方向；OPF_IMMEDIATE（纯影子写 + 置门禁/状态位，无阻塞）。
 * 并发契约：byte 144 影子唯一写者=本回调；byte 145 状态字节唯一写者=
 * VdmFreezeApply/VdmUnfreezeApply（vdm.c，同 ISR 上下文串行）。 */
DWORD OsfpOperate2FhFreezeReq(BYTE *cmd, BYTE len);

/* Page 2Fh 0x11 byte 145 VDM UnfreezeRequest（bit6，本项目 2Fh 约定）：
 * 写 bit6=1 → VdmUnfreezeApply。状态位（bit7 FreezeDone / bit6 UnfreezeDone）
 * 由 VdmFreezeApply/VdmUnfreezeApply 唯一回写，本回调不持久化 host 写入值
 * （写即触发，读回为状态）。len 必须==1。
 * 写方向；OPF_IMMEDIATE。 */
DWORD OsfpOperate2FhUnfreezeReq(BYTE *cmd, BYTE len);

#endif /* RAL_OPERATE_H */

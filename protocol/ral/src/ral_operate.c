/***************************************************************************//**
 *   @file   ral_operate.c
 *   @brief  RAL 寄存器操作回调（oprfunc）实现。
 *   @author Firmware team
 *******************************************************************************
 * Copyright 2026 Terabitcom, Inc.
 *
 * 集中实现 ral.c 寄存器属性表里 oprfunc 字段引用的回调。回调只写「动作本身」；
 * 调用时机（ISR 同步 vs 主循环下沉）由 RAL 引擎按 OsfpRegAttr_t.opftype 决定，
 * 本文件不实现调度。方向约定见 ral_operate.h。
 *
 * 回调清单（均已实现）：ModuleControl（WO/SC 归一 + 触发）、密码 Entry/Change、
 * BankPageSelect、ApplyDPInit（→dpsm.c）、D9H 邮箱 CHIP/GPIO/DAC/ADC 事务。
 * 调度时机（ISR 同步 vs 主循环下沉）仍由 RAL 引擎按 opftype 决定。
 ******************************************************************************/
#include "ral.h"          /* OsfpRegAttr_t / OprateFunc / ACC_* / MEMT_* / LOW_ADDR_* / PAGE_TAB_NUM */
#include "ral_operate.h"
#include "data_transfer.h"/* DtTabCtrlSet / g_CurPageId：页/ bank 上下文 */
#include "adpat.h"        /* WriteSramProtocolMsaSpace：影子维护 */
#include "gd32e511_512.h" /* NVIC_SystemReset：SoftwareReset 直接请求内核系统复位 */
#include "chipresource.h" /* ReadChip / WriteChip：D9H 邮箱芯片事务派发 */
#include "gpioresource.h" /* ReadGpio / WriteGpio / IO_LOW / IO_HIGH：D9H GPIO 记录事务 */
#include "dacresource.h"  /* ReadDac / WriteDac：D9H DAC 记录事务 */
#include "adcresource.h"  /* ReadAdc：D9H ADC 记录事务 */
#include "dpsm.h"         /* DpsmApplyDpInit：SCS→ACS 拷贝完成后置 ConfigStatus/DPInitPending */
#include "cdb.h"          /* CdbTrigger：9Fh CMDID 写触发命令入口 */
#include "vdm.h"          /* VdmFreezeApply / VdmUnfreezeApply：2Fh Freeze 门禁 */


/*================ 密码子系统状态（CMIS 5.3 §8.2.14，寄存器 00h:118-125 / 00h:42.3-0）================
 * 密码值与锁状态由模块内部维护（规范：registers are self-clearing, the module
 * maintains the state internally）——密码区 MEMT_NONE 不落影子，host 读回值无定义。 */
#define OSFP_PWD_RES_MOD_ACC    0x01U                   /* 0001b module 密码 entry 已接受 */
#define OSFP_PWD_RES_HOST_ACC   0x02U                   /* 0010b host 密码 entry/change 已接受 */
#define OSFP_PWD_RES_REJECT     0x03U                   /* 0011b 未接受 */

#define OSFP_PWD_HOST_DEFAULT   0x00001011UL            /* 出厂 Host Password（CMIS 定值） */
#define OSFP_PWD_MODULE_DEFAULT 0x80005A5AUL            /* TODO：厂商定义 Module Password，后续移交参数管理 NV */

DWORD g_PwdHost = OSFP_PWD_HOST_DEFAULT;                /* 运行时 Host 密码；参数管理启动时以 NV 值覆写 */
STATIC DWORD g_PwdModule = OSFP_PWD_MODULE_DEFAULT;     /* Module 密码：host 不可改 */
volatile BYTE g_PwdUnlocked = 0U;                       /* MgmtInit 出态 = locked */
volatile BYTE g_PwdPersistPending = 0U;                 /* Host 密码已变更、待参数管理持久化（其清零） */

/* U32 大端拼装（密码区为 4B 大端写入） */
STATIC DWORD OsfpPwdBe32(CONST BYTE *cmd)
{
    return ((DWORD)cmd[0] << 24) | ((DWORD)cmd[1] << 16) |
           ((DWORD)cmd[2] << 8)  |  (DWORD)cmd[3];
}

/* 写 PasswordCmdResult（0x2A 低 4 位，保留位恒 0）。0x2A 属引擎专属 RO 寄存器、
 * 本密码子系统是唯一写者，单字节写原子、无并发 RMW，无需 BASEPRI 锁。 */
STATIC VOID OsfpPwdResultSet(BYTE code)
{
    BYTE b = (BYTE)(code & 0x0FU);

    (void)WriteSramProtocolMsaSpace(PAGE_ID_LOW, LOW_ADDR_MISC_STATUS, &b, 1U);
}

/* 0x1A Module Control（混合字节：bit7-4/2-0 普通 RW，bit3 SoftwareReset 为 WO/SC）。
 * 写方向；OPF_IMMEDIATE：
 *   - bit3 = 1 → 直接 NVIC_SystemReset() 请求内核系统复位（不返回）——复位
 *     后管理态自然回到 MgmtInit，无需软件安排复位时机；
 *   - bit3 = 0 → 整字节写影子（本路径写入值 bit3 已为 0，WO/SC 读回恒 0）。
 * 接线后纯存储路径不再覆盖 0x1A，影子维护是 oprfunc 职责。跨界 / 跨寄存器
 * 顺序写（len!=1）由引擎整寄存器写门禁丢弃——跨寄存器顺序写应落在纯存储区。 */
DWORD OsfpOperateModuleControl(BYTE *cmd, BYTE len)
{
    if((cmd == NULL) || (len != 1U)) {
        return ERR;
    }

    if((cmd[0] & 0x08U) != 0U) {
        NVIC_SystemReset();     /* SoftwareReset：内核系统复位，函数不返回 */
    }

    (void)WriteSramProtocolMsaSpace(PAGE_ID_LOW, LOW_ADDR_MODULE_CONTROL, cmd, 1U);
    return OK;
}

/* 0x76 PasswordChangeEntryArea（WO/SC，U32 大端，CMIS 5.3 §8.2.14 Table 8-24）。
 * 写方向；OPF_DEFERRED（主循环经 CmdExeBackground 执行）：
 *   - len 必须==4（size-matched 4 字节写）；
 *   - 仅已解锁（此前成功 entry）且新值 MSB=0（Host 范围）时接受：更新 g_PwdHost、
 *     置 g_PwdPersistPending（Flash 持久化归参数管理，本回调不直接写 NV）；
 *     写 Module 范围值效果未定义，按拒收处理、原密码保持；
 *   - 结果写 0x2A PasswordCmdResult（0010b 接受 / 0011b 拒收）。
 * 注意：改密成功不改变解锁状态（规范仅规定无效 entry 回锁）。 */
DWORD OsfpOperatePasswordChange(BYTE *cmd, BYTE len)
{
    DWORD pw;
    BYTE  res;

    if((cmd == NULL) || (len != 4U)) {
        return ERR;
    }

    pw  = OsfpPwdBe32(cmd);
    res = OSFP_PWD_RES_REJECT;
    if((g_PwdUnlocked != 0U) && ((pw & 0x80000000UL) == 0UL)) {
        g_PwdHost           = pw;
        g_PwdPersistPending = 1U;
        res                 = OSFP_PWD_RES_HOST_ACC;
    }
    OsfpPwdResultSet(res);
    return OK;
}

/* 0x7A PasswordEntryArea（WO/SC，U32 大端，CMIS 5.3 §8.2.14 Table 8-24）。
 * 写方向；OPF_DEFERRED：
 *   - len 必须==4；
 *   - 与 Module/Host 当前密码比对：命中即解锁（Module 命中 0001b，Host 命中
 *     0010b）；未命中回锁——规范：解锁态保持到「无效 entry 或模块重初始化」；
 *   - 结果写 0x2A PasswordCmdResult。
 * 校验纯 SRAM 比较，可为 IMMEDIATE；保持 DEFERRED 与声明一致，并给未来
 * NVM 慢速 Module 密码校验留窗口（1000b validation-in-progress 语义）。 */
DWORD OsfpOperatePasswordEntry(BYTE *cmd, BYTE len)
{
    DWORD pw;
    BYTE  res;

    if((cmd == NULL) || (len != 4U)) {
        return ERR;
    }

    pw  = OsfpPwdBe32(cmd);
    res = OSFP_PWD_RES_REJECT;
    if(pw == g_PwdModule) {
        g_PwdUnlocked = 1U;
        res           = OSFP_PWD_RES_MOD_ACC;
    } else if(pw == g_PwdHost) {
        g_PwdUnlocked = 1U;
        res           = OSFP_PWD_RES_HOST_ACC;
    } else {
        g_PwdUnlocked = 0U;
    }
    OsfpPwdResultSet(res);
    return OK;
}

/* 0x7E-0x7F BankSelect+PageSelect 合并域（RWW，2B）：cmd[0]=Bank(126)
 * cmd[1]=Page(127)，len 必须==2（单写 126 或 127 由引擎整寄存器写门禁
 * 丢弃，本自检是第二道防线）。
 * 语义（CMIS 5.3）：
 *   - BankSelect 仅低 2 位有效（DtTabCtrlSet 内掩码）；
 *   - 写不支持的页 → PageSelect 自清零回 Page 00h，bank 保持生效
 *     （用 g_CurPageId 哨兵判定：DtTabCtrlSet 内已按新上下文刷新解析）；
 *   - 影子维护：cmd 就地修正为生效值后写 126/127 影子，host 读回得见
 *     RWW 当前值（纯 SRAM 拷贝，ISR 安全）。
 * 返回 OK/ERR；OPF_IMMEDIATE，仅写方向触发。 */
DWORD OsfpOperateBankPageSelect(BYTE *cmd, BYTE len)
{
    BYTE bank;
    BYTE page;

    if((cmd == NULL) || (len != 2U)) {
        return ERR;     /* 合并域须一次写满 2B */
    }

    bank = (BYTE)(cmd[0] & 0x03U);
    page = cmd[1];

    DtTabCtrlSet(page, bank);
    if(g_CurPageId == (TabIndex_t)PAGE_TAB_NUM) {
        page = 0x00U;                   /* 页未登记（不支持）：自清零回 Page 00h */
        DtTabCtrlSet(page, bank);
    }

    /* 影子写生效值：bank 已掩码、page 可能已回零，host 读回与上下文一致 */
    cmd[0] = bank;
    cmd[1] = page;
    (void)WriteSramProtocolMsaSpace(PAGE_ID_LOW, LOW_ADDR_BANK_PAGE_SELECT, cmd, 2U);
    return OK;
}

/*================ Page 10h SCS0::ApplyDPInit —— Provision（SCS0 → ACS 拷贝） ================
 * host 协议：先在 SCS0（Page 10h，当前 bank）写好 DPConfig/Tx/Rx 控制，再写
 * ApplyDPInitLane1-8（10h:143，单字节 WRITE）触发。本回调执行「暂存 → 生效」：
 * 把 SCS0 的控制集整块拷贝到 ACS（Page 11h，同 bank），再交 DpsmApplyDpInit
 * 走 ConfigStatus/DPInitPending 流程（DPSM 之后在 DEACTIVATED 态按 ACS 配
 * 硬件）。回调 OPF_IMMEDIATE（DtFrameCb ISR 同步执行）：host 写 126/127 切
 * 页/bank 后紧接本触发，bank 即 host 意图——DEFERRED 下沉会在执行时读错 bank。
 * 纯 SRAM 影子拷贝 + 内存表操作，无 I2C/无 Flash/无阻塞，ISR 安全。
 * 并发契约：本回调只写 ACS（Page 11h 0x4E/0x56/0x5E）影子字节——全页 RO、
 * DPSM/监视任务是这些字节的唯一写者，本回调作为 Apply 触发点与 DPSM 同源
 * 写、不越界写 SCS0 之外的任何影子字节（SCS0 是 host 纯存储写者）。 */

/* SCS0（Page 10h，bank 内页偏移）→ ACS（Page 11h，bank 内页偏移）Provision
 * 拷贝映射（CMIS 5.3 Table 8-72~8-74 vs 8-93~8-95）：
 *   DPConfig 8B + Tx 控制 8B + Rx 控制 13B；SCS0 Rx 末 2B（宿主 174-175
 *   Reserved）不拷贝——ACS 侧本就没有对应字节。 */
#define APPLY_DPCFG_SIZE        8U
#define APPLY_TX_SIZE           8U
#define APPLY_RX_SIZE           13U     /* CDREnableRx 1B + Pre/PostCursor/Amplitude 各 4B */
#define APPLY_COPY_MAX          APPLY_RX_SIZE

DWORD OsfpOperateApplyDpInit(BYTE *cmd, BYTE len)
{
    BYTE      buf[APPLY_COPY_MAX];
    BYTE      bank;
    TabIndex_t tab10;
    TabIndex_t tab11;

    if((cmd == NULL) || (len != 1U)) {
        return ERR;     /* Apply 触发须单字节 WRITE（CMIS Table 8-70），多字节写拒收 */
    }

    /* 当前 bank 上下文（OPF_IMMEDIATE 同步执行，bank 即 host 本次切页意图） */
    bank  = DtTabCtrlGetBank();
    tab10 = (TabIndex_t)(PAGE_ID_10H_0 + (DWORD)bank);
    tab11 = (TabIndex_t)(PAGE_ID_11H_0 + (DWORD)bank);

    /* ① DPConfigLane1-8：SCS0 → ACS */
    if(ReadSramProtocolMsaSpace(tab10, PAGE_10H_ADDR_SCS_DPCFG, buf, APPLY_DPCFG_SIZE) != OK) {
        return ERR;
    }
    if(WriteSramProtocolMsaSpace(tab11, PAGE_11H_ADDR_ACS_DPCFG, buf, APPLY_DPCFG_SIZE) != OK) {
        return ERR;
    }

    /* ② Tx 控制：SCS0 → ACS */
    if(ReadSramProtocolMsaSpace(tab10, PAGE_10H_ADDR_SCS_TX, buf, APPLY_TX_SIZE) != OK) {
        return ERR;
    }
    if(WriteSramProtocolMsaSpace(tab11, PAGE_11H_ADDR_ACS_TX, buf, APPLY_TX_SIZE) != OK) {
        return ERR;
    }

    /* ③ Rx 控制（有效段，末 2B 保留不拷贝）：SCS0 → ACS */
    if(ReadSramProtocolMsaSpace(tab10, PAGE_10H_ADDR_SCS_RX, buf, APPLY_RX_SIZE) != OK) {
        return ERR;
    }
    if(WriteSramProtocolMsaSpace(tab11, PAGE_11H_ADDR_ACS_RX, buf, APPLY_RX_SIZE) != OK) {
        return ERR;
    }

    /* ④ 拷贝完成，交 DPSM 走 ConfigStatus/DPInitPending（lane5-8 悬空剥离） */
    return DpsmApplyDpInit(cmd[0]);
}

/*================ Page D9h 通用芯片寄存器读写邮箱（厂商页，布局见 ral.h） ================
 * host 协议：写 CHIPID（纯存储）→ 写 REGADDR（触发芯片读，REGRBVALUE 回填）或
 * 写 REGVALUE（触发芯片写）。两个触发回调均 OPF_DEFERRED——芯片 I2C 事务
 * （TIA/DRV 软 I2C ~µs/bit、PMIC 同、DSP 硬件 I2C 均需 ms 级窗口）禁止在
 * STPDET ISR 执行。引擎「整寄存器写」分发：触发须 off==0x01/0x05 且 len==4
 * 单写；0x00 起 9B 顺序写落 CHIPID 纯存储路径不触发（host 工具按单写拼帧）。 */

/* 邮箱公共：影子读 CHIPID(1B)+REGADDR(4B 大端→u32)。CHIPID 单字节原子；
 * REGADDR 4B 非原子——host 顺序写协议规避（见文件头注），不做 BASEPRI 锁。
 * CHIPID 越界（无对应 ChipInfo_t）不在这里拒，交 ReadChip/WriteChip 经
 * GetChipInfoById 判空返 ERR——单一否决点。 */
STATIC DWORD OsfpD9hMailBoxRead(BYTE *chipid, DWORD *regaddr)
{
    BYTE buf[PAGE_D9H_REGADDR_SIZE];

    if((chipid == NULL) || (regaddr == NULL)) {
        return ERR;
    }
    if(ReadSramProtocolMsaSpace(PAGE_ID_D9H, PAGE_D9H_ADDR_CHIPID, chipid, 1U) != OK) {
        return ERR;
    }
    if(ReadSramProtocolMsaSpace(PAGE_ID_D9H, PAGE_D9H_ADDR_REGADDR, buf, PAGE_D9H_REGADDR_SIZE) != OK) {
        return ERR;
    }
    *regaddr = OsfpPwdBe32(buf);
    return OK;
}

/* REGRBVALUE（0x09-0x0C）u32 大端回填——本回调是 D9H 页该字节的唯一写者
 * （并发契约 1：影子字节写者唯一；host 侧 ACC_RO，引擎丢弃 host 写）。 */
STATIC VOID OsfpD9hRegrbSet(DWORD value)
{
    BYTE b[PAGE_D9H_REGRBVALUE_SIZE];

    b[0] = (BYTE)(value >> 24);
    b[1] = (BYTE)(value >> 16);
    b[2] = (BYTE)(value >> 8);
    b[3] = (BYTE)(value);
    AdpatShadowLock();    /* 4B 多字节写（固件回填）：锁内防读相位快照撕裂 */
    (void)WriteSramProtocolMsaSpace(PAGE_ID_D9H, PAGE_D9H_ADDR_REGRBVALUE, b, PAGE_D9H_REGRBVALUE_SIZE);
    AdpatShadowUnlock();
}

/* 0x01-0x04 REGADDR（u32 大端）：写触发芯片读事务。
 * 写方向；OPF_DEFERRED：
 *   - len 必须==4；先持久化 REGADDR 到影子（oprfunc 接管语义，引擎不写影子——
 *     不持久化 host 读回将停留在旧值）；
 *   - 影子读 CHIPID+REGADDR → ReadChip(chipid, regaddr, &value)；
 *   - 成功大端回填 REGRBVALUE；失败（chip 未上电/NACK/ID 越界）保持旧值——
 *     REGRBVALUE 无 valid 位，host 无法区分旧值与新读到相同值★（归保留区
 *     STA 字节方案，未建）。返回 OK/ERR（ERR 仅为入参/影子异常，芯片读
 *     失败不回填但返回 OK——host 侧无可观察错误通道）。 */
DWORD OsfpOperateD9hRegAddr(BYTE *cmd, BYTE len)
{
    BYTE  chipid;
    DWORD regaddr;
    DWORD value;

    if((cmd == NULL) || (len != PAGE_D9H_REGADDR_SIZE)) {
        return ERR;
    }
    AdpatShadowLock();    /* 4B 多字节写（持久化 host 命令）：锁内防读相位快照撕裂 */
    if(WriteSramProtocolMsaSpace(PAGE_ID_D9H, PAGE_D9H_ADDR_REGADDR, cmd, PAGE_D9H_REGADDR_SIZE) != OK) {
        AdpatShadowUnlock();
        return ERR;
    }
    AdpatShadowUnlock();
    if(OsfpD9hMailBoxRead(&chipid, &regaddr) != OK) {
        return ERR;
    }
    if(ReadChip(chipid, regaddr, &value) == OK) {
        OsfpD9hRegrbSet(value);
    }
    return OK;
}

/* 0x05-0x08 REGVALUE（u32 大端）：写触发芯片写事务。
 * 写方向；OPF_DEFERRED：len==4 → 持久化影子 → 影子读 CHIPID+REGADDR →
 * WriteChip(chipid, regaddr, Be32(cmd))。返回 WriteChip 结果
 * （OK/ERR 仅记录——引擎对 oprfunc 返回值不做 host 可见反馈★同上）。 */
DWORD OsfpOperateD9hRegValue(BYTE *cmd, BYTE len)
{
    BYTE  chipid;
    DWORD regaddr;

    if((cmd == NULL) || (len != PAGE_D9H_REGVALUE_SIZE)) {
        return ERR;
    }
    AdpatShadowLock();    /* 4B 多字节写（持久化 host 命令）：锁内防读相位快照撕裂 */
    if(WriteSramProtocolMsaSpace(PAGE_ID_D9H, PAGE_D9H_ADDR_REGVALUE, cmd, PAGE_D9H_REGVALUE_SIZE) != OK) {
        AdpatShadowUnlock();
        return ERR;
    }
    AdpatShadowUnlock();
    if(OsfpD9hMailBoxRead(&chipid, &regaddr) != OK) {
        return ERR;
    }
    return WriteChip(chipid, regaddr, OsfpPwdBe32(cmd));
}

/*================ Page D9h GPIO 记录（0x0D-0x0F，布局见 ral.h） ================
 * host 协议：写 GPIOID（触发 ReadGpio→GPIOSTATUS 回填）或写 GPIOVALUE（触发
 * WriteGpio）。两回调均 OPF_IMMEDIATE——GPIO 直读直写（寄存器级访问，无 I2C、
 * 无阻塞、无 Flash），ISR 安全。引擎「整寄存器写」分发：触发须 off==0x0D/0x0E
 * 且 len==1 单写。 */

/* 0x0D GPIOID（u8）：写触发 GPIO 读事务。
 * 写方向；OPF_IMMEDIATE：
 *   - len 必须==1；先持久化 GPIOID 到影子（oprfunc 接管语义，引擎不写影子）；
 *   - cmd[0] 即 host 本次写入的 GPIOID，直作 ReadGpio 入参；
 *   - 成功把回读电平（低字节 0/1）回填 0x0F GPIOSTATUS；失败（ID 越界/未初始化）
 *     保持旧值——GPIOSTATUS 无 valid 位★（同 CHIP 记录 REGRBVALUE）。 */
DWORD OsfpOperateD9hGpioId(BYTE *cmd, BYTE len)
{
    DWORD status;
    BYTE  b;

    if((cmd == NULL) || (len != 1U)) {
        return ERR;
    }
    if(WriteSramProtocolMsaSpace(PAGE_ID_D9H, PAGE_D9H_ADDR_GPIOID, cmd, 1U) != OK) {
        return ERR;
    }
    if(ReadGpio(cmd[0], &status) == OK) {
        b = (BYTE)(status & 0xFFU);
        (void)WriteSramProtocolMsaSpace(PAGE_ID_D9H, PAGE_D9H_ADDR_GPIOSTATUS, &b, 1U);
    }
    return OK;
}

/* 0x0E GPIOVALUE（u8）：写触发 GPIO 写事务。
 * 写方向；OPF_IMMEDIATE：len==1 → 持久化影子 → 影子读 GPIOID（host 早一步写入）
 * → WriteGpio(GPIOID, 非零→IO_HIGH/零→IO_LOW)。返回 WriteGpio 结果
 * （OK/ERR 仅记录——引擎对 oprfunc 返回值不做 host 可见反馈★同上）。 */
DWORD OsfpOperateD9hGpioValue(BYTE *cmd, BYTE len)
{
    BYTE id;

    if((cmd == NULL) || (len != 1U)) {
        return ERR;
    }
    if(WriteSramProtocolMsaSpace(PAGE_ID_D9H, PAGE_D9H_ADDR_GPIOVALUE, cmd, 1U) != OK) {
        return ERR;
    }
    if(ReadSramProtocolMsaSpace(PAGE_ID_D9H, PAGE_D9H_ADDR_GPIOID, &id, 1U) != OK) {
        return ERR;
    }
    return WriteGpio(id, (cmd[0] != 0U) ? IO_HIGH : IO_LOW);
}

/*================ Page D9h DAC 记录（0x10-0x18，布局见 ral.h） ================
 * host 协议：写 DACID（触发 ReadDac→DACRDVALUE 回填）或写 DACVALUE（触发
 * WriteDac）。两回调均 OPF_DEFERRED——DAC 可能挂在外部芯片上走 I2C（禁止在
 * STPDET ISR 内执行）。引擎「整寄存器写」分发：触发须 off==0x10（len==1）或
 * 0x11（len==4）单写。 */

/* DACRDVALUE（0x15-0x18）u32 大端回填——本回调是 D9H 页该字节的唯一写者
 * （并发契约 1：影子字节写者唯一；host 侧 ACC_RO，引擎丢弃 host 写）。 */
STATIC VOID OsfpD9hDacRdbSet(DWORD value)
{
    BYTE b[PAGE_D9H_DACRDVALUE_SIZE];

    b[0] = (BYTE)(value >> 24);
    b[1] = (BYTE)(value >> 16);
    b[2] = (BYTE)(value >> 8);
    b[3] = (BYTE)(value);
    AdpatShadowLock();    /* 4B 多字节写（固件回填）：锁内防读相位快照撕裂 */
    (void)WriteSramProtocolMsaSpace(PAGE_ID_D9H, PAGE_D9H_ADDR_DACRDVALUE, b, PAGE_D9H_DACRDVALUE_SIZE);
    AdpatShadowUnlock();
}

/* 0x10 DACID（u8）：写触发 DAC 读事务。
 * 写方向；OPF_DEFERRED：
 *   - len 必须==1；先持久化 DACID 到影子（oprfunc 接管语义，引擎不写影子）；
 *   - cmd[0] 即 host 本次写入的 DACID，直作 ReadDac 入参；
 *   - 成功把回读值大端回填 0x15-0x18 DACRDVALUE；失败（ID 越界/未初始化）保持
 *     旧值——DACRDVALUE 无 valid 位★（同 CHIP 记录 REGRBVALUE）。 */
DWORD OsfpOperateD9hDacId(BYTE *cmd, BYTE len)
{
    DWORD value;

    if((cmd == NULL) || (len != 1U)) {
        return ERR;
    }
    if(WriteSramProtocolMsaSpace(PAGE_ID_D9H, PAGE_D9H_ADDR_DACID, cmd, 1U) != OK) {
        return ERR;
    }
    if(ReadDac(cmd[0], &value) == OK) {
        OsfpD9hDacRdbSet(value);
    }
    return OK;
}

/* 0x11-0x14 DACVALUE（u32 大端）：写触发 DAC 写事务。
 * 写方向；OPF_DEFERRED：len==4 → 持久化影子 → 影子读 DACID（host 早一步写入）
 * → WriteDac(DACID, Be32(cmd))。返回 WriteDac 结果
 * （OK/ERR 仅记录——引擎对 oprfunc 返回值不做 host 可见反馈★同上）。 */
DWORD OsfpOperateD9hDacValue(BYTE *cmd, BYTE len)
{
    BYTE id;

    if((cmd == NULL) || (len != PAGE_D9H_DACVALUE_SIZE)) {
        return ERR;
    }
    AdpatShadowLock();    /* 4B 多字节写（持久化 host 命令）：锁内防读相位快照撕裂 */
    if(WriteSramProtocolMsaSpace(PAGE_ID_D9H, PAGE_D9H_ADDR_DACVALUE, cmd, PAGE_D9H_DACVALUE_SIZE) != OK) {
        AdpatShadowUnlock();
        return ERR;
    }
    AdpatShadowUnlock();
    if(ReadSramProtocolMsaSpace(PAGE_ID_D9H, PAGE_D9H_ADDR_DACID, &id, 1U) != OK) {
        return ERR;
    }
    return WriteDac(id, OsfpPwdBe32(cmd));
}

/*================ Page D9h ADC 记录（0x19-0x1D，布局见 ral.h） ================
 * host 协议：写 ADCID（触发 ReadAdc→ADCVALUE 回填）。ADC 只读（无写事务通道），
 * 单一回调 OPF_DEFERRED——ADC 可能走外部芯片 I2C（禁止在 STPDET ISR 内执行）。
 * 引擎「整寄存器写」分发：触发须 off==0x19 且 len==1 单写。 */

/* ADCVALUE（0x1A-0x1D）u32 大端回填——本回调是 D9H 页该字节的唯一写者
 * （并发契约 1：影子字节写者唯一；host 侧 ACC_RO，引擎丢弃 host 写）。 */
STATIC VOID OsfpD9hAdcValueSet(DWORD value)
{
    BYTE b[PAGE_D9H_ADCVALUE_SIZE];

    b[0] = (BYTE)(value >> 24);
    b[1] = (BYTE)(value >> 16);
    b[2] = (BYTE)(value >> 8);
    b[3] = (BYTE)(value);
    AdpatShadowLock();    /* 4B 多字节写（固件回填）：锁内防读相位快照撕裂 */
    (void)WriteSramProtocolMsaSpace(PAGE_ID_D9H, PAGE_D9H_ADDR_ADCVALUE, b, PAGE_D9H_ADCVALUE_SIZE);
    AdpatShadowUnlock();
}

/* 0x19 ADCID（u8）：写触发 ADC 读事务。
 * 写方向；OPF_DEFERRED：
 *   - len 必须==1；先持久化 ADCID 到影子（oprfunc 接管语义，引擎不写影子）；
 *   - cmd[0] 即 host 本次写入的 ADCID，直作 ReadAdc 入参；
 *   - 成功把回读值大端回填 0x1A-0x1D ADCVALUE；失败（ID 越界/未初始化）保持
 *     旧值——ADCVALUE 无 valid 位★（同 CHIP 记录 REGRBVALUE）。 */
DWORD OsfpOperateD9hAdcId(BYTE *cmd, BYTE len)
{
    DWORD value;

    if((cmd == NULL) || (len != 1U)) {
        return ERR;
    }
    if(WriteSramProtocolMsaSpace(PAGE_ID_D9H, PAGE_D9H_ADDR_ADCID, cmd, 1U) != OK) {
        return ERR;
    }
    if(ReadAdc(cmd[0], &value) == OK) {
        OsfpD9hAdcValueSet(value);
    }
    return OK;
}

/*================ Page 9Fh CMDID（0x00-0x01，布局见 ral.h） ================
 * host 协议：最后一次单独写 2B CMDID（STOP 落 129）触发命令——CMIS 触发模型
 * 0「ending at 129」（01h:165.7=0）。回调 OPF_IMMEDIATE——纯影子写 + 置标志 +
 * 寄存器级 hold-off，无阻塞/无 Flash/无 I2C，ISR 安全。
 * 引擎「整寄存器写」分发：触发须 off==0x00 且 len==2 单写；含 header+LPL 的
 * 单次长 WRITE（len>2）由引擎门禁丢弃，符合「单独写 CMDID」触发模型。
 * ★遗留（CMIS 5.3 §8.23）：mode 0 亦允许「单字节写 129」触发（CMDID 高字节
 * 靠前次写入），本实现只收 2B 形态——支持它需「纯存储 + 触发侧效应」的第三
 * 种派发模式（引擎先落影子再按帧覆盖 byte 129 派发，回调改从影子取 CMDID），
 * 不是放宽长度门禁能解决的。 */

/* 0x00-0x01 CMDID（u16 大端）：写触发命令。
 * 写方向；OPF_IMMEDIATE：转发 CdbTrigger（cdb.c）——busy 拒收、捕获 9Fh bank
 * 影子槽 + 持久化 CMDID（供 CdbChkVerify 读）、置 busy+In Progress+pending、
 * 开前台 NACK hold-off；真正执行下沉主循环 CdbExeBackground。返回 CdbTrigger 结果
 * （OK/ERR 仅记录——引擎对 oprfunc 返回值不做 host 可见反馈★同上）。 */
DWORD OsfpOperate9fCmdId(BYTE *cmd, BYTE len)
{
    return CdbTrigger(cmd, len);
}

/*================ Page 2Fh VDM Freeze/Unfreeze（布局见 ral.c） ================
 * 影子偏移 = CMIS 字节号 - 128（影子槽只存 Upper 窗口 128B）。两回调均
 * OPF_IMMEDIATE——纯影子写 + 置门禁/状态位，无阻塞/无 Flash/无 I2C，ISR 安全。
 * 引擎「整寄存器写」分发：触发须帧首对齐本寄存器且 len==1 单写（跨界 /
 * 跨寄存器顺序写由引擎整寄存器写门禁丢弃）。 */

/* 0x10 byte 144 FreezeRequest（RW）：bit7=1 → 统计量定格；bit7=0 → 恢复
 * （CMIS：host 清 FreezeRequest 即解冻；本页 bit6=UnfreezeReq W1S 亦归此
 * 路径）。oprfunc 接管语义、引擎不落存储——本回调持久化 host 写入值到
 * byte 144 影子供读回。
 * 写方向；OPF_IMMEDIATE。 */
DWORD OsfpOperate2FhFreezeReq(BYTE *cmd, BYTE len)
{
    if((cmd == NULL) || (len != 1U)) {
        return ERR;
    }

    (void)WriteSramProtocolMsaSpace(PAGE_ID_2FH_0, PAGE_2FH_ADDR_FREEZE_REQ, cmd, 1U);

    if((cmd[0] & PAGE_2FH_BIT_FREEZE_REQ) != 0U) {
        VdmFreezeApply();
    } else {
        VdmUnfreezeApply();
    }
    return OK;
}

/* 0x11 byte 145 UnfreezeRequest 触发（bit6，本项目 2Fh 约定）：写 1 → 恢复
 * 统计量更新。状态位（bit7 FreezeDone / bit6 UnfreezeDone）唯一写者 =
 * VdmFreezeApply/VdmUnfreezeApply（vdm.c），本回调不持久化 host 写入值
 * （写即触发，读回为状态）。
 * 写方向；OPF_IMMEDIATE。 */
DWORD OsfpOperate2FhUnfreezeReq(BYTE *cmd, BYTE len)
{
    if((cmd == NULL) || (len != 1U)) {
        return ERR;
    }

    if((cmd[0] & PAGE_2FH_BIT_UNFREEZE_REQ) != 0U) {
        VdmUnfreezeApply();
    }
    return OK;
}

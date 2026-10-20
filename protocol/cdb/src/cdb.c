/***************************************************************************//**
 *   @file   cdb.c
 *   @brief  CMIS CDB (Command Data Block) 引擎：9Fh 消息页命令-应答交换，
 *           聚焦固件下载（0101h/0103h/0107h）与四条通用命令。
 *   @author Firmware team
 *******************************************************************************
 * Copyright 2026 Terabitcom, Inc.
 *
 * CDB 在 RAL 之上提供带应答、带状态、能搬大块数据的命令协议。host 把
 * Command Header + Local Payload 写进 Page 9Fh:128-255，最后一次单独写
 * CMDID（128-129，两字节，STOP 落 129）触发；模块在同一页回填 Reply
 * Header（RPLLength/RPLChkCode）+ Reply Payload，并把执行状态发布到
 * Lower Memory CdbStatus1（00h:37）与 CdbCmdCompleteFlag1（00h:8.6）。
 *
 * 本实现只支持前台（foreground）单实例模式：
 *   - 单 CDB 实例绑定当前 9Fh bank（host 用 bank0），一次一条命令；
 *   - CMDID 触发在 STPDET ISR 内立即置 busy + In Progress（oprfunc
 *     OPF_IMMEDIATE），否则 host 的 Acknowledge Polling 会读到旧状态误判
 *     完成；
 *   - 真正的命令执行（Flash 擦写、CRC 深校验）下沉主循环 CdbExeBackground()；
 *   - 前台 hold-off：busy 期间模块对一切 host 事务 NACK（I2cSlaveHoldOff），
 *     host 反复重试，直到 busy 清除读回最终 CdbStatus。
 *
 * 命令集（暂定稿）：
 *   0000h Query / 0004h Abort / 0040h Module Features / 0041h Firmware
 *   Management Features（通用）；0101h Start / 0103h Write Block / 0107h
 *   Complete（固件下载核心）；0109h Run（确认落盘 + 延时复位，拒 Hitless）
 *   / 010Ah Commit（本方案已收敛入 Run，直接 SUCCESS）。
 *
 * 固件下载传输约定（与 tools/pack_framed_bin.py 产物对齐）：
 *   - 镜像 = 负载（fwsize 字节，%4==0）；512B 帧头区是元数据，经 0101h
 *     VendorData[112]（= FrameHead_t）携带、0107h 由模块重建写入；
 *   - ImageSize = fwsize（同时容忍 fwsize+0x200 的整帧口径），
 *     StartCmdPayloadSize=0，BlockAddress = 负载字节偏移（0 基）；
 *   - 0107h 先补写 512B 帧头区（帧头最后写，掉电安全序）再 VerifyFramedImage
 *     深校验；
 *   - 尾块按 ⌈len/8⌉·8 垫 0xFF 双字写（擦除区写 0xFF 是 no-op）。
 ******************************************************************************/
#include "cdb.h"
#include "ral.h"              /* PAGE_ID_9FH_0 / PAGE_ID_LOW / PAGE_9FH_ADDR_* / LOW_ADDR_CDB_STATUS1 / LOW_ADDR_MODULE_FLAGS */
#include "data_transfer.h"    /* DtTabCtrlGetBank：CMDID 触发时捕获 9Fh bank */
#include "adpat.h"            /* Read/WriteSramProtocolMsaSpace / AdpatShadowLock/Unlock */
#include "flashresource.h"    /* frame routing / physical FlashId_t operations */
#ifndef BL_BUILD
#include "extflash.h"         /* InitExtFlash：DSP_FW Start 前初始化 U15 */
#include "gpioresource.h"     /* WriteGpio(GPIO_ID_RESET_PHY)：升级期间保持 DSP 复位 */
#endif
#include "i2c_slave.h"        /* I2cSlaveHoldOff：前台 hold-off 门 */
#include "ral_operate.h"      /* g_PwdUnlocked：Query 命令 Status 字节 */
#include "timer2_scheduler.h" /* Timer2SchedulerAdd/Remove / g_TaskIds / TASK_ID_FW_RESET：Run 延时复位 */
#include "gd32e511_512.h"     /* NVIC_SystemReset：Run 延时复位任务 */

/* 9Fh 页内偏移统一见 ral.h PAGE_9FH_ADDR_*（CMDID/EPLLEN/LPLLEN/CHKCODE/RPLLEN/RPLCHK/LPL） */

/*================ CdbStatus 字节码（00h:37 CdbStatus1） ================
 * 布局：bit7 CdbIsBusy | bit6 CdbHasFailed | bit5-0 CdbCommandResult。
 * 字面字节值与 CMIS 5.3 Table 8-x 一致（如 45h = 01 000101b CdbChkCode 错）。 */
#define CDB_STS_IDLE         0x00U   /* 复位/无命令 */
#define CDB_STS_SUCCESS      0x01U   /* 成功 */
#define CDB_STS_ABORTED      0x03U   /* 成功（被中止） */
#define CDB_STS_IN_PROGRESS  0x81U   /* 执行中（busy） */
#define CDB_STS_UNKNOWN_CMD  0x41U   /* CMDID 未知 */
#define CDB_STS_PARAM_ERR    0x42U   /* 参数错误 */
#define CDB_STS_TIMEOUT      0x44U   /* 超时 */
#define CDB_STS_CHK_ERR      0x45U   /* CdbChkCode 错 */
#define CDB_STS_PWD_ERR      0x46U   /* 密码错 */
#define CDB_STS_INCOMPAT     0x47U   /* 状态不兼容 */

/*================ 命令集（暂定稿） ================
 * CMDID 2 字节大端，命令码单调递增（表驱动二分查找前提，禁止乱序）。 */
typedef enum {
    CDB_CMD_QUERY        = 0x0000U,  /* 通用：Query */
    CDB_CMD_ABORT        = 0x0004U,  /* 通用：Abort */
    CDB_CMD_MODULE_FEAT  = 0x0040U,  /* 通用：Module Features */
    CDB_CMD_FW_MGMT_FEAT = 0x0041U,  /* 通用：Firmware Management Features */
    CDB_CMD_FW_START     = 0x0101U,  /* 固件下载：Start */
    CDB_CMD_FW_WRITE     = 0x0103U,  /* 固件下载：Write Block */
    CDB_CMD_FW_COMPLETE  = 0x0107U,  /* 固件下载：Complete */
    CDB_CMD_FW_RUN       = 0x0109U,  /* 固件下载：Run（确认落盘 + 延时复位） */
    CDB_CMD_FW_COMMIT    = 0x010AU   /* 固件下载：Commit（已收敛入 Run） */
} CdbCmd_t;

/* 命令处理函数签名：统一 VOID（通用命令与固件命令均经 g_Cdb9fSlot 落位） */
typedef BYTE (*CdbCmdFn)(VOID);

/* 命令表条目：cmd 单调递增（二分查找前提），fn 为对应处理函数（返回
 * CdbStatus 字节码）。 */
typedef struct {
    CdbCmd_t cmd;
    CdbCmdFn fn;
} CdbCmdInfo_t;

/* 00h:8 ModuleFlags 的 CdbCmdCompleteFlag1 = bit6 */
#define CDB_COMPLETE_FLAG    0x40U

/* 固件下载状态 */
#define CDB_FW_IDLE          0U
#define CDB_FW_RECEIVING     1U
#define CDB_FW_READY         1U     /* g_CdbFwReady：当前目标已装包+深校验通过，可 Run */

/* 帧头长度 = CDB 0101h VendorData 长度 = FrameHead_t（112B） */
#define CDB_FW_HDR_SIZE      112U

/* 能力/时长宣告（ms；MaxDurationMultiplier=0 → 单位 1ms） */
#define CDB_MAX_COMPLETION_TIME   3000U   /* 0040h：覆盖 U15 片擦除驱动 3s 预算 */
#define CDB_FW_MAXDUR_START       3000U
#define CDB_FW_MAXDUR_ABORT       10U
#define CDB_FW_MAXDUR_WRITE       100U
#define CDB_FW_MAXDUR_COMPLETE    2000U
#define CDB_FW_MAXDUR_COPY        0U

/* Logical frame (type, storeaddr) -> physical FlashId_t. APP always names
 * MEM_APP_A_BASE but is received into physical APP_B. DSP_FW is APP-only. */
#define CDB_CTX_APP    0x01U
#define CDB_CTX_BL     0x02U
#ifdef BL_BUILD
#define CDB_RUNTIME_CONTEXT  CDB_CTX_BL
#else
#define CDB_RUNTIME_CONTEXT  CDB_CTX_APP
#endif

typedef struct {
    BYTE  type;
    DWORD storeaddr;
    BYTE  physical_id;
    BYTE  context_mask;
} CdbFwRoute_t;

STATIC CONST CdbFwRoute_t g_CdbFwRoutes[] = {
    {FLASH_TYPE_APP,      MEM_APP_A_BASE,          FLASH_ID_APP_B,  CDB_CTX_APP | CDB_CTX_BL},
    {FLASH_TYPE_PARAM,    MEM_PARAM_MAIN_BASE,     FLASH_ID_PARA_A, CDB_CTX_APP | CDB_CTX_BL},
    {FLASH_TYPE_PARAM,    MEM_PARAM_BACKUP_BASE,   FLASH_ID_PARA_B, CDB_CTX_APP | CDB_CTX_BL},
    {FLASH_TYPE_EXTFLASH, MEM_DSP_FW_BASE,         FLASH_ID_DSP_FW, CDB_CTX_APP},
};
#define CDB_FW_ROUTE_NUM  (sizeof(g_CdbFwRoutes) / sizeof(g_CdbFwRoutes[0]))

/*================ 引擎状态 ================ */
STATIC volatile BYTE  g_CdbBusy;             /* 前台命令执行中（hold-off 生效） */
STATIC volatile BYTE  g_CdbPending;          /* CMDID 已触发、待 CdbExeBackground 执行 */
STATIC volatile CdbCmd_t g_CdbPendingCmdId;  /* 待执行命令 CMDID */
STATIC volatile TabIndex_t g_Cdb9fSlot;      /* 触发时捕获的 9Fh 影子槽 */
STATIC BYTE  g_CdbFwState;                   /* CDB_FW_IDLE / CDB_FW_RECEIVING */
STATIC BYTE  g_CdbFwTargetId;                /* Start 锁存的物理 FlashId_t */
STATIC BYTE  g_CdbFwHdr[CDB_FW_HDR_SIZE];    /* 0101h VendorData（FrameHead_t）副本 */
STATIC DWORD g_CdbFwSize;                    /* ImageSize == FrameHead_t.fwsize */
STATIC DWORD g_CdbFwRxDone;                  /* 已接收负载字节数（= 下一 BlockAddress） */
STATIC BYTE  g_CdbFwReady;                   /* 1 = 当前目标已装包+深校验通过，可 Run */
STATIC BYTE  g_CdbDspUpgradeActive;          /* DSP_FW Start 后持有 RESET_PHY/SPI2，直到复位 */

/* 发布 CdbStatus1（00h:37）。单字节写，M33 对齐单字节访问原子：ISR 写
 * In Progress 与主循环写最终态在一条命令周期内单写者唯一，无需锁。 */
STATIC VOID CdbStatusSet(BYTE code)
{
    (void)WriteSramProtocolMsaSpace(PAGE_ID_LOW, LOW_ADDR_CDB_STATUS1, &code, 1U);
}

/* 完成标志置/清（00h:8.6）。线程侧 RMW，锁内防 DtCorClear（ISR 读后清 COR）
 * 交错丢更新（docs/memory_map.md 三写者模型规则 3）。 */
STATIC VOID CdbCompleteFlagSet(BYTE set)
{
    BYTE flags;

    AdpatShadowLock();
    if(ReadSramProtocolMsaSpace(PAGE_ID_LOW, LOW_ADDR_MODULE_FLAGS, &flags, 1U) == OK) {
        flags = (set != 0U) ? (BYTE)(flags | CDB_COMPLETE_FLAG)
                            : (BYTE)(flags & (BYTE)~CDB_COMPLETE_FLAG);
        (void)WriteSramProtocolMsaSpace(PAGE_ID_LOW, LOW_ADDR_MODULE_FLAGS, &flags, 1U);
    }
    AdpatShadowUnlock();
}

/* u32 大端拼装辅助（LPL 多字节字段均为大端） */
STATIC DWORD CdbBe32(CONST BYTE *p)
{
    return ((DWORD)p[0] << 24) | ((DWORD)p[1] << 16) |
           ((DWORD)p[2] << 8)  |  (DWORD)p[3];
}

/* CdbChkCode 校验：sum = CMDID[2] + EPLLength[2] + LPLLength[1] +
 * LPL[lpllen]，(sum + chk) & 0xFF == 0xFF 通过。字段均从 9Fh 影子槽读
 * （CMDID 由 CdbTrigger 持久化；EPLLength/LPLLength/LPL 由 host 纯存储写落）。 */
STATIC DWORD CdbChkVerify(TabIndex_t slot)
{
    BYTE  hdr[5];
    BYTE  chk;
    BYTE  buf[PAGE_9FH_LPL_SIZE];
    DWORD sum, lpllen, i;

    if(ReadSramProtocolMsaSpace(slot, PAGE_9FH_ADDR_CMDID, hdr, 5U) != OK) {
        return ERR;
    }
    if(ReadSramProtocolMsaSpace(slot, PAGE_9FH_ADDR_CHKCODE, &chk, 1U) != OK) {
        return ERR;
    }
    lpllen = (DWORD)hdr[4];
    if(lpllen > PAGE_9FH_LPL_SIZE) {
        return ERR;
    }

    sum = 0U;
    for(i = 0U; i < 5U; i++) {
        sum += hdr[i];
    }
    if(lpllen > 0U) {
        if(ReadSramProtocolMsaSpace(slot, PAGE_9FH_ADDR_LPL, buf, (BYTE)lpllen) != OK) {
            return ERR;
        }
        for(i = 0U; i < lpllen; i++) {
            sum += buf[i];
        }
    }

    return (((sum + (DWORD)chk) & 0xFFU) == 0xFFU) ? OK : ERR;
}

/* 应答回填：RPLLength + 应答体（LPL 区）+ RPLChkCode。RPLChkCode =
 * ~sum(body)（无 body 则 0）。body==NULL 且 len>0 视为调用错误。 */
STATIC DWORD CdbRplWrite(TabIndex_t slot, CONST BYTE *body, BYTE len)
{
    BYTE  chk;
    DWORD sum, i;

    if(len > PAGE_9FH_LPL_SIZE) {
        return ERR;
    }
    if(WriteSramProtocolMsaSpace(slot, PAGE_9FH_ADDR_RPLLEN, &len, 1U) != OK) {
        return ERR;
    }
    sum = 0U;
    if(len > 0U) {
        if(body == NULL) {
            return ERR;
        }
        if(WriteSramProtocolMsaSpace(slot, PAGE_9FH_ADDR_LPL, body, len) != OK) {
            return ERR;
        }
        for(i = 0U; i < (DWORD)len; i++) {
            sum += body[i];
        }
    }
    chk = (len == 0U) ? 0U : (BYTE)(~sum);
    return WriteSramProtocolMsaSpace(slot, PAGE_9FH_ADDR_RPLCHK, &chk, 1U);
}

/*================ 通用命令 ================ */

/* 0000h Query：应答 [9Fh:136 保留, 9Fh:137 Status]。Status = 01h（Host
 * Password 已接受）或 00h（Boot Up）。 */
STATIC BYTE CdbCmdQuery(VOID)
{
    BYTE body[2];

    body[0] = 0x00U;
    body[1] = (g_PwdUnlocked != 0U) ? 0x01U : 0x00U;
    return (CdbRplWrite(g_Cdb9fSlot, body, 2U) == OK) ? CDB_STS_SUCCESS : CDB_STS_PARAM_ERR;
}

/* 0004h Abort：前台单实例无后台操作可中止，直接成功；同时复位进行中的
 * 固件下载状态。无应答体。 */
STATIC BYTE CdbCmdAbort(VOID)
{
    g_CdbFwState = CDB_FW_IDLE;
    g_CdbFwTargetId = FLASH_ID_NUM;
    g_CdbFwReady = 0U;
    /* DSP_FW 一旦 Start 擦除，Abort 不恢复旧镜像，也不释放 DSP 复位；
     * g_CdbDspUpgradeActive 保持到 MCU 复位，host 可重新 Start 重刷。 */
    return (CdbRplWrite(g_Cdb9fSlot, NULL, 0U) == OK) ? CDB_STS_SUCCESS : CDB_STS_PARAM_ERR;
}

/* 0040h Module Features：32 字节位图（每 bit 一个 0000h-00FFh 命令）+ U16
 * MaxCompletionTime。位图 byte i/8 的 bit i%8 对应 CMDID 低字节 i。 */
STATIC BYTE CdbCmdModuleFeatures(VOID)
{
    BYTE  body[36];
    DWORD i;

    for(i = 0U; i < 36U; i++) {
        body[i] = 0U;
    }
    /* 位图 @ body[2..33]（9Fh:138-169）；body[0..1]（136-137）保留 */
    body[2U + (0x00U >> 3)] |= (BYTE)(1U << (0x00U & 7U));   /* 0000h Query */
    body[2U + (0x04U >> 3)] |= (BYTE)(1U << (0x04U & 7U));   /* 0004h Abort */
    body[2U + (0x40U >> 3)] |= (BYTE)(1U << (0x40U & 7U));   /* 0040h Module Features */
    body[2U + (0x41U >> 3)] |= (BYTE)(1U << (0x41U & 7U));   /* 0041h FW Mgmt Features */
    /* MaxCompletionTime U16 BE @ body[34..35]（9Fh:170-171） */
    body[34] = (BYTE)(CDB_MAX_COMPLETION_TIME >> 8);
    body[35] = (BYTE)(CDB_MAX_COMPLETION_TIME);

    return (CdbRplWrite(g_Cdb9fSlot, body, 36U) == OK) ? CDB_STS_SUCCESS : CDB_STS_PARAM_ERR;
}

/* 0041h Firmware Management Features：报告固件下载能力。布局（LPL 起点
 * 9Fh:136 计 body[0]）：
 *   [1]    SkippingErasedBlocks(bit0)/MaxDurationMultiplier(bit3)=0
 *   [2..3] StartCmdPayloadSize = 0
 *   [4]    ReadWriteLengthExt = 0（无 EPL）
 *   [5]    WriteMechanism = 0（仅 LPL）
 *   [6]    ErasedByte = 0xFF
 *   [7]    HitlessRestart/ImageReadback = 0
 *   [8..17] MaxDuration Start/Abort/Write/Complete/Copy（U16 各） */
STATIC BYTE CdbCmdFwMgmtFeatures(VOID)
{
    BYTE  body[18];
    DWORD i;

    for(i = 0U; i < 18U; i++) {
        body[i] = 0U;
    }
    body[6] = 0xFFU;                                    /* ErasedByte = 0xFF */
    body[8]  = (BYTE)(CDB_FW_MAXDUR_START >> 8);
    body[9]  = (BYTE)(CDB_FW_MAXDUR_START);
    body[10] = (BYTE)(CDB_FW_MAXDUR_ABORT >> 8);
    body[11] = (BYTE)(CDB_FW_MAXDUR_ABORT);
    body[12] = (BYTE)(CDB_FW_MAXDUR_WRITE >> 8);
    body[13] = (BYTE)(CDB_FW_MAXDUR_WRITE);
    body[14] = (BYTE)(CDB_FW_MAXDUR_COMPLETE >> 8);
    body[15] = (BYTE)(CDB_FW_MAXDUR_COMPLETE);
    body[16] = (BYTE)(CDB_FW_MAXDUR_COPY >> 8);
    body[17] = (BYTE)(CDB_FW_MAXDUR_COPY);

    return (CdbRplWrite(g_Cdb9fSlot, body, 18U) == OK) ? CDB_STS_SUCCESS : CDB_STS_PARAM_ERR;
}

/*================ 固件下载命令（0101h/0103h/0107h） ================ */

STATIC CONST CdbFwRoute_t *CdbFwRouteFind(BYTE type, DWORD storeaddr)
{
    DWORD i;

    for(i = 0U; i < CDB_FW_ROUTE_NUM; i++) {
        if(g_CdbFwRoutes[i].type == type && g_CdbFwRoutes[i].storeaddr == storeaddr) {
            return &g_CdbFwRoutes[i];
        }
    }
    return NULL;
}

/* 0101h Start：LPL = ImageSize(U32BE) + VendorData[112]（= FrameHead_t）。
 * VendorData 是模块原生小端帧头；type+storeaddr 精确路由并锁存物理目标，
 * 所有校验通过后才擦除。DSP_FW 仅 APP：保持 DSP 复位后初始化并擦除 U15。 */
STATIC BYTE CdbFwStart(VOID)
{
    BYTE  lpl[116];
    BYTE  lpllen;
    DWORD imageSize, fwsize, i;
    FrameHead_t hdr;
    CONST CdbFwRoute_t *route;

    if(ReadSramProtocolMsaSpace(g_Cdb9fSlot, PAGE_9FH_ADDR_LPLLEN, &lpllen, 1U) != OK) {
        return CDB_STS_PARAM_ERR;
    }
    if(lpllen < 116U) {
        return CDB_STS_PARAM_ERR;   /* Start 至少 ImageSize(4)+VendorData(112) */
    }
    if(ReadSramProtocolMsaSpace(g_Cdb9fSlot, PAGE_9FH_ADDR_LPL, lpl, 116U) != OK) {
        return CDB_STS_PARAM_ERR;
    }
    for(i = 0U; i < CDB_FW_HDR_SIZE; i++) {
        ((BYTE *)&hdr)[i] = lpl[4U + i];
    }

    imageSize = CdbBe32(&lpl[0]);           /* 标准 ImageSize 为大端；VendorData 为原生帧头 */
    fwsize    = hdr.fwsize;
    route     = CdbFwRouteFind(hdr.type, hdr.storeaddr);
    if(route == NULL) {
        return CDB_STS_PARAM_ERR;           /* type 与 storeaddr 不是合法组合 */
    }
    if((route->context_mask & CDB_RUNTIME_CONTEXT) == 0U) {
        return CDB_STS_INCOMPAT;            /* BL 不支持 DSP_FW */
    }
    if(VerifyFrameHdrBuf(route->physical_id, (CONST BYTE *)&hdr, CDB_FW_HDR_SIZE) != OK) {
        return CDB_STS_PARAM_ERR;
    }

    /* 交叉核对 ImageSize == fwsize（容忍整帧口径 fwsize+512） */
    if((imageSize != fwsize) && (imageSize != (fwsize + FRAME_HDR_AREA_SIZE))) {
        return CDB_STS_PARAM_ERR;
    }

    /* 新下载通过全部纯校验后，先作废旧会话再改目标上下文。此后任一失败
     * 出口都保持 IDLE + rxDone=0，禁止 0103h 沿用旧偏移写入新目标。 */
    g_CdbFwState  = CDB_FW_IDLE;
    g_CdbFwRxDone = 0U;
    g_CdbFwReady  = 0U;

    /* 保存帧头副本（0107h 重建 512B 帧头区用）并锁存物理目标。 */
    for(i = 0U; i < CDB_FW_HDR_SIZE; i++) {
        g_CdbFwHdr[i] = ((CONST BYTE *)&hdr)[i];
    }
    g_CdbFwTargetId = route->physical_id;
    g_CdbFwSize = fwsize;

#ifndef BL_BUILD
    if(g_CdbFwTargetId == FLASH_ID_DSP_FW) {
        /* U15 与 DSP 共享 SPI：整个升级会话保持 DSP 复位，并暂停 ODSP 周期访问。 */
        if(WriteGpio(GPIO_ID_RESET_PHY, IO_HIGH) != OK) {
            return CDB_STS_PARAM_ERR;
        }
        g_CdbDspUpgradeActive = 1U;
        if(InitExtFlash(0U) != OK) {
            return CDB_STS_PARAM_ERR;
        }
    }
#endif

    if(EraseFlashWithFeed(g_CdbFwTargetId) != OK) {
        return CDB_STS_PARAM_ERR;
    }

    g_CdbFwState  = CDB_FW_RECEIVING;
    g_CdbFwRxDone = 0U;
    return CDB_STS_SUCCESS;
}

/* 0103h Write Block：LPL = BlockAddress(U32BE) + FirmwareBlock[≤112]。
 * 顺序续传（BlockAddress == 已收字节数）→ 写 Flash（非末块须 8 对齐，
 * 末块按 ⌈len/8⌉·8 垫 0xFF）→ 推进 rx_done。 */
STATIC BYTE CdbFwWrite(VOID)
{
    BYTE  lpl[116];
    BYTE  lpllen, block_len;
    BYTE  block[112];
    DWORD blockAddr, i;
    DWORD payloadOff = FlashFramePayloadOffset(g_CdbFwTargetId);

    if(g_CdbFwState != CDB_FW_RECEIVING) {
        return CDB_STS_INCOMPAT;        /* 未 Start 就写块 */
    }
    if(ReadSramProtocolMsaSpace(g_Cdb9fSlot, PAGE_9FH_ADDR_LPLLEN, &lpllen, 1U) != OK) {
        return CDB_STS_PARAM_ERR;
    }
    if(lpllen < 4U) {
        return CDB_STS_PARAM_ERR;
    }
    block_len = (BYTE)(lpllen - 4U);
    if((block_len == 0U) || (block_len > 112U)) {
        return CDB_STS_PARAM_ERR;
    }
    if(ReadSramProtocolMsaSpace(g_Cdb9fSlot, PAGE_9FH_ADDR_LPL, lpl, lpllen) != OK) {
        return CDB_STS_PARAM_ERR;
    }
    blockAddr = CdbBe32(&lpl[0]);
    for(i = 0U; i < (DWORD)block_len; i++) {
        block[i] = lpl[4U + i];
    }

    if(blockAddr != g_CdbFwRxDone) {
        return CDB_STS_PARAM_ERR;       /* 顺序续传：BlockAddress 须等于已收字节数 */
    }
    if((g_CdbFwRxDone + (DWORD)block_len) > g_CdbFwSize) {
        return CDB_STS_PARAM_ERR;       /* 越界 */
    }

    if((block_len % 8U) == 0U) {
        if(WriteFlash(g_CdbFwTargetId, payloadOff + g_CdbFwRxDone,
                      block, block_len) != OK) {
            return CDB_STS_PARAM_ERR;
        }
    } else {
        BYTE  pad[112U + 7U];
        DWORD padded, j;

        if((g_CdbFwRxDone + (DWORD)block_len) != g_CdbFwSize) {
            return CDB_STS_PARAM_ERR;   /* 非末块不许非 8 对齐（后续块偏移依赖） */
        }
        padded = ((DWORD)block_len + 7U) & ~7UL;
        for(j = 0U; j < (DWORD)block_len; j++) {
            pad[j] = block[j];
        }
        for(; j < padded; j++) {
            pad[j] = 0xFFU;             /* 尾块垫 0xFF（擦除区写 0xFF 是 no-op） */
        }
        if(WriteFlash(g_CdbFwTargetId, payloadOff + g_CdbFwRxDone,
                      pad, padded) != OK) {
            return CDB_STS_PARAM_ERR;
        }
    }

    g_CdbFwRxDone += (DWORD)block_len;
    return CDB_STS_SUCCESS;
}

/* 0107h Complete：负载已收满 → 重建 512B 帧头区（112B FrameHead_t + 0xFF）
 * 最后写（掉电安全序）→ VerifyFramedImage 深校验（读回 CRC32 vs 帧头
 * crc32data）→ 回 IDLE。 */
STATIC BYTE CdbFwComplete(VOID)
{
    BYTE  hdrArea[FRAME_HDR_AREA_SIZE];
    DWORD i;
    DWORD hdrOff = FlashFrameHeaderOffset(g_CdbFwTargetId);

    if(g_CdbFwState != CDB_FW_RECEIVING) {
        return CDB_STS_INCOMPAT;
    }
    if(g_CdbFwRxDone != g_CdbFwSize) {
        return CDB_STS_PARAM_ERR;       /* 未收满 */
    }

    for(i = 0U; i < FRAME_HDR_AREA_SIZE; i++) {
        hdrArea[i] = 0xFFU;
    }
    for(i = 0U; i < CDB_FW_HDR_SIZE; i++) {
        hdrArea[i] = g_CdbFwHdr[i];
    }
    if(WriteFlash(g_CdbFwTargetId, hdrOff, hdrArea, FRAME_HDR_AREA_SIZE) != OK) {
        return CDB_STS_PARAM_ERR;
    }

    if(VerifyFramedImage(g_CdbFwTargetId) != OK) {
        return CDB_STS_PARAM_ERR;
    }

    g_CdbFwState = CDB_FW_IDLE;
    g_CdbFwReady = CDB_FW_READY;   /* 当前物理目标已装包+深校验通过，可 Run */
    return CDB_STS_SUCCESS;
}

/*================ 命令表与分发 ================ */

/* 延时复位任务（Timer2 一次性）：Run 确认落盘后经 DelayToReset ms 触发。
 * 在 Timer2SchedulerRun（主循环）上下文执行 NVIC_SystemReset；复位后 BL
 * 读 OTA metadata J-A 的 CONFIRMED 记录，提升 B→A。 */
STATIC VOID CdbFwResetTask(VOID *arg)
{
    (void)arg;
    NVIC_SystemReset();
}

/* 0109h Run：LPL = [9Fh:136 保留, 9Fh:137 ImageToRun, 9Fh:138-139 DelayToReset
 * (U16 BE, ms)]。前置：Complete 已深校验通过（g_CdbFwReady）。忽略 bit0
 * （image select，固定 A 无选择），拒 bit1（Hitless 无缝切换不支持，回
 * CDB_STS_INCOMPAT）。APP_B 先注册复位再落盘 OtaConfirm；PARA/DSP_FW
 * 不写 APP 升级记录，直接按 DelayToReset 复位。复位任务只在主循环顶部
 * Timer2SchedulerRun 执行，必然晚于本函数发布 SUCCESS/完成标志/关 hold-off。 */
STATIC BYTE CdbCmdFwRun(VOID)
{
    BYTE  lpl[4];
    BYTE  lpllen, imageToRun;
    DWORD delayMs;

    if(g_CdbFwReady == 0U) {
        return CDB_STS_INCOMPAT;    /* 未 Start/Write/Complete 深校验通过 */
    }
    if(ReadSramProtocolMsaSpace(g_Cdb9fSlot, PAGE_9FH_ADDR_LPLLEN, &lpllen, 1U) != OK) {
        return CDB_STS_PARAM_ERR;
    }
    if(lpllen < 4U) {
        return CDB_STS_PARAM_ERR;
    }
    if(ReadSramProtocolMsaSpace(g_Cdb9fSlot, PAGE_9FH_ADDR_LPL, lpl, 4U) != OK) {
        return CDB_STS_PARAM_ERR;
    }
    imageToRun = lpl[1];                        /* 9Fh:137 ImageToRun */
    delayMs    = ((DWORD)lpl[2] << 8) | (DWORD)lpl[3];   /* 9Fh:138-139 DelayToReset */

    if((imageToRun & 0x02U) != 0U) {
        return CDB_STS_INCOMPAT;    /* bit1 Hitless 无缝切换不支持，拒收 */
    }
    if(g_CdbFwTargetId != FLASH_ID_APP_B
       && g_CdbFwTargetId != FLASH_ID_PARA_A
       && g_CdbFwTargetId != FLASH_ID_PARA_B
       && g_CdbFwTargetId != FLASH_ID_DSP_FW) {
        return CDB_STS_INCOMPAT;
    }

    /* 先注册复位再按需落盘：复位在下一轮主循环顶部才可能触发，时序安全。 */
    g_TaskIds[TASK_ID_FW_RESET] = Timer2SchedulerAdd(CdbFwResetTask, NULL, delayMs, 0U, 1U);
    if(g_TaskIds[TASK_ID_FW_RESET] < 0) {
        return CDB_STS_PARAM_ERR;
    }
    if(g_CdbFwTargetId == FLASH_ID_APP_B && OtaConfirm() != OK) {
        (void)Timer2SchedulerRemove(g_TaskIds[TASK_ID_FW_RESET]);   /* 落盘失败：撤销复位 */
        return CDB_STS_PARAM_ERR;
    }

    return CDB_STS_SUCCESS;
}

/* 010Ah Commit：无 LPL。本实现把「确认 + 复位」全部收敛到 Run（固定 A/B
 * 方案：Run 落盘 CONFIRMED 后即复位、BL 提升 B→A），无独立 Commit 阶段；
 * 直接回 SUCCESS（无应答体）。 */
STATIC BYTE CdbCmdFwCommit(VOID)
{
    return CDB_STS_SUCCESS;
}

/* 命令表：cmd 单调递增存放（二分查找前提，禁止乱序/零填充）。 */
STATIC CONST CdbCmdInfo_t g_CdbCmdInfo[] = {
    { CDB_CMD_QUERY,        CdbCmdQuery },
    { CDB_CMD_ABORT,        CdbCmdAbort },
    { CDB_CMD_MODULE_FEAT,  CdbCmdModuleFeatures },
    { CDB_CMD_FW_MGMT_FEAT, CdbCmdFwMgmtFeatures },
    { CDB_CMD_FW_START,     CdbFwStart },
    { CDB_CMD_FW_WRITE,     CdbFwWrite },
    { CDB_CMD_FW_COMPLETE,  CdbFwComplete },
    { CDB_CMD_FW_RUN,       CdbCmdFwRun },
    { CDB_CMD_FW_COMMIT,    CdbCmdFwCommit },
};
#define CDB_CMD_INFO_NUM  (sizeof(g_CdbCmdInfo) / sizeof(g_CdbCmdInfo[0]))

/* 二分左值查找 cmd 对应的处理函数并执行；未命中回 CDB_STS_UNKNOWN_CMD。 */
STATIC BYTE CdbCommandDispatch(CdbCmd_t cmdid)
{
    DWORD lo = 0U;
    DWORD hi = (DWORD)CDB_CMD_INFO_NUM;

    /* lo = 首个 cmd >= cmdid 的下标（与 DtTabLeftFind 同构）；等于 cmdid 即命中 */
    while(lo < hi) {
        DWORD mid = lo + ((hi - lo) >> 1);
        if(g_CdbCmdInfo[mid].cmd < cmdid) {
            lo = mid + 1U;
        } else {
            hi = mid;
        }
    }
    if((lo < (DWORD)CDB_CMD_INFO_NUM) && (g_CdbCmdInfo[lo].cmd == cmdid)) {
        return g_CdbCmdInfo[lo].fn();
    }
    return CDB_STS_UNKNOWN_CMD;
}

/*================ 公共入口 ================ */

VOID CdbInit(VOID)
{
    g_CdbBusy         = 0U;
    g_CdbPending      = 0U;
    g_CdbPendingCmdId = (CdbCmd_t)0U;
    g_Cdb9fSlot       = PAGE_ID_9FH_0;
    g_CdbFwState      = CDB_FW_IDLE;
    g_CdbFwTargetId   = FLASH_ID_NUM;
    g_CdbFwSize       = 0U;
    g_CdbFwRxDone     = 0U;
    g_CdbFwReady      = 0U;
    g_CdbDspUpgradeActive = 0U;
    CdbStatusSet(CDB_STS_IDLE);
    I2cSlaveHoldOff(0U);
}

/* 9Fh CMDID oprfunc（OPF_IMMEDIATE，STPDET ISR）：len==2、busy 拒收，
 * 捕获 9Fh bank 影子槽 + 持久化 CMDID（ChkCode 校验需读）→ 置 pending/busy
 * + In Progress + 前台 hold-off。真正执行下沉 CdbExeBackground。 */
DWORD CdbTrigger(CONST BYTE *cmd, BYTE len)
{
    CdbCmd_t   cmdid;
    TabIndex_t slot;

    if((cmd == NULL) || (len != 2U)) {
        return ERR;
    }
    if(g_CdbBusy != 0U) {
        return ERR;     /* 前台单实例：执行中再写 CMDID 行为未定义，拒收 */
    }

    cmdid = (CdbCmd_t)(((WORD)cmd[0] << 8) | (WORD)cmd[1]);
    slot  = (TabIndex_t)(PAGE_ID_9FH_0 + (DWORD)DtTabCtrlGetBank());

    /* 持久化 CMDID（oprfunc 接管语义，引擎不写影子） */
    if(WriteSramProtocolMsaSpace(slot, PAGE_9FH_ADDR_CMDID, cmd, 2U) != OK) {
        return ERR;
    }

    g_CdbPendingCmdId = cmdid;
    g_Cdb9fSlot       = slot;
    g_CdbPending      = 1U;
    g_CdbBusy         = 1U;

    CdbStatusSet(CDB_STS_IN_PROGRESS);   /* 立即 In Progress：host Acknowledge Polling 可见 busy */
    I2cSlaveHoldOff(1U);                 /* 前台 hold-off：NACK 一切 host 事务 */

    return OK;
}

/* 主循环每轮调用（封装为 CdbExeBackground，与 data_transfer 的 CmdExeBackground
 * 对应）：执行 pending 命令（ChkCode 校验 + 命令分派），完成后发布最终
 * CdbStatus + 完成标志，清 busy/关 hold-off。执行期间 hold-off 保持 NACK，
 * host 读被拒直到这里收尾。 */
VOID CdbExeBackground(VOID)
{
    BYTE code;

    if(g_CdbPending == 0U) {
        return;
    }

    CdbCompleteFlagSet(0U);                 /* 本命令执行周期：清完成标志 */
    (void)CdbRplWrite(g_Cdb9fSlot, NULL, 0U);   /* 清应答头（RPLLength=0/RPLChkCode=0） */

    if(CdbChkVerify(g_Cdb9fSlot) != OK) {
        code = CDB_STS_CHK_ERR;
    } else {
        code = CdbCommandDispatch(g_CdbPendingCmdId);
    }

    CdbStatusSet(code);                     /* 发布最终状态 */
    CdbCompleteFlagSet(1U);                 /* 置完成标志 */
    g_CdbBusy    = 0U;
    g_CdbPending = 0U;
    I2cSlaveHoldOff(0U);                    /* 关 hold-off：host 读回最终状态 */
}

BYTE CdbBusy(VOID)
{
    return g_CdbBusy;
}

BYTE CdbDspUpgradeActive(VOID)
{
    return g_CdbDspUpgradeActive;
}

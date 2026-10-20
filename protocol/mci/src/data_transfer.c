/***************************************************************************//**
 *   @file   data_transfer.c
 *   @brief  MCI 数据搬运上下文、指令解析与 oprfunc 分发：当前模块操作的
 *            page / bank（Lower Memory byte 126 BankSelect & byte 127
 *            PageSelect）、窗口地址 → OsfpRegAttr_t 的二分查表定位
 *            （DtParse），以及 STPDET 帧回调（DtFrameCb）——按目标寄存器
 *            的 opftype 将 oprfunc 立即执行或压 deferred FIFO 由主循环执行。
 *   @author Firmware team
 *******************************************************************************
 * Copyright 2026 Terabitcom, Inc.
 *
 * 上下文由 MCI 在收到 host 写 Lower Memory byte 126/127 的帧时更新，
 * 读相位快照与 RAL 引擎按此上下文确定"当前页/ bank"。指令解析流程
 * （DtParse）：
 *   1. g_MgmtMemAddr bit7 选半区：0~127 → Page Low，128~255 → 当前 page；
 *   2. 左偏二分 g_OsfpTabForm[] 得 tab 最左下标，+ bank 得匹配项下标；
 *   3. 在匹配项的 OsfpRegAttr_t 表中按区间包含二分 offset，得「含该 offset
 *      的表项」（inf 表 offset 升序、区间互不重叠，DtTabCtrlInit 校验）。
 * 帧回调（DtFrameCb，I2C1 EV STPDET ISR 上下文）：
 *   1. 取帧首字节（= 本帧写窗口地址）经 DtParse 定位寄存器属性；
 *   2. 触发寄存器（oprfunc != NULL）须整寄存器写（off==attr->offset 且
 *      len==attr->size），再按 opftype 分发：OPF_IMMEDIATE 立即调
 *      oprfunc(cmd, len)；OPF_DEFERRED → oprfunc + 入参副本压 FIFO（深度 16），
 *      主循环轮询 DtDeferredPoll 执行（重操作/Flash 写禁止在 ISR）；
 *   3. 纯存储寄存器按 CMIS 字节寻址落影子：帧起点可在表项任意字节，
 *      [off, off+len) 整段须同 memtype 非 RO（DtRangeWritable 逐表项遍历）。
 * 双上下文分发无互斥：IMMEDIATE 回调跑在 STPDET ISR，DEFERRED 回调跑在
 * 主循环，二者并发访问影子/页上下文。正确性依赖 oprfunc 实现遵守
 * ral_operate.h 的「并发契约」（影子字节写者唯一、表项区间不重叠、
 * DEFERRED 禁碰 DtTabCtrl/g_CurPageId）；本引擎不做运行期互斥。
 * 详见 data_transfer.h。
 ******************************************************************************/
#include "data_transfer.h"
#include "i2c_slave.h"      /* I2cSlaveGetRxBuf / I2C_RX_* */
#include "adpat.h"          /* WriteProtocolMsaSpace / ADPAT_MGMT_PAGE_SIZE */

#define DT_TAB_NONE     0xFFFFFFFFUL    /* 二分未命中标记 */

/* DtParseForm 定义在本文件后部；DtTabCtrlInit/Set 的页索引刷新需要它。 */
STATIC DWORD DtParseForm(BYTE memaddr, CONST OsfpRegAttr_t **attr);

STATIC DtTabCtrl_t g_DtTabCtrl = {0x00U, BANK_ID_0};    /* 上电默认 page 00h / bank 0 */

/* inf 表不变式违例计数（DtTabCtrlInit 累加）：0 = 全表通过。
 * volatile 防编译器死存储消除，供调试器监视；不提供运行期访问器。 */
STATIC volatile DWORD g_DtTabBad = 0U;

/* 当前 Upper 半区页表索引 = g_DtTabCtrl（page+bank）的解析结果，读相位快照
 * 按它取页数据；126/127 改写经 DtTabCtrlSet 时同步刷新（同 ISR 上下文，
 * 与 g_DtTabCtrl 原子一致）。(TabIndex_t)PAGE_TAB_NUM = 选中页未登记——
 * Upper 读应回 dummy。volatile：ISR 写、主循环可能读。 */
volatile TabIndex_t g_CurPageId = (TabIndex_t)PAGE_TAB_NUM;

/* 按当前 g_DtTabCtrl 刷新 g_CurPageId：解析任一 Upper 半区地址（bit7=1
 * 即走 page+bank 路径，与具体地址无关）取表下标；页未登记置哨兵。 */
STATIC VOID DtCurPageRefresh(VOID)
{
    CONST OsfpRegAttr_t *attr = NULL;
    DWORD idx = DtParseForm(0x80U, &attr);

    (void)attr;                                 /* 只取表下标，属性丢弃 */
    g_CurPageId = (idx == DT_TAB_NONE) ? (TabIndex_t)PAGE_TAB_NUM : (TabIndex_t)idx;
}

VOID DtTabCtrlInit(VOID)
{
    DWORD i;

    g_DtTabCtrl.page = 0x00U;
    g_DtTabCtrl.bank = BANK_ID_0;

    /* inf 表不变式一次性校验（区间包含查找依赖）：offset 升序、区间互不
     * 重叠、末项不越页尾。违例只累加 g_DtTabBad 不阻断启动——占位行
     * num=0/inf=NULL 自然跳过，实表违例应在改表时被发现而非上电拒启。 */
    for(i = 0U; i < (DWORD)PAGE_TAB_NUM; i++) {
        CONST OsfpRegAttr_t *t = g_OsfpTabForm[i].inf;
        DWORD n = (DWORD)g_OsfpTabForm[i].num;
        DWORD j;

        if((t == NULL) || (n == 0U)) {
            continue;
        }
        for(j = 1U; j < n; j++) {
            if(((DWORD)t[j - 1U].offset + (DWORD)t[j - 1U].size) > (DWORD)t[j].offset) {
                g_DtTabBad++;                       /* 与前项重叠 / 非升序 */
            }
        }
        if(((DWORD)t[n - 1U].offset + (DWORD)t[n - 1U].size) > (DWORD)ADPAT_MGMT_PAGE_SIZE) {
            g_DtTabBad++;                           /* 末项越页尾 */
        }
    }

    DtCurPageRefresh();
}

VOID DtTabCtrlSet(BYTE page, BYTE bank)
{
    g_DtTabCtrl.page = page;
    g_DtTabCtrl.bank = (DtBankId_t)(bank & 0x03U);      /* BankSelect 仅低 2 位有效 */
    DtCurPageRefresh();
}

BYTE DtTabCtrlGetPage(VOID)
{
    return g_DtTabCtrl.page;
}

BYTE DtTabCtrlGetBank(VOID)
{
    return (BYTE)g_DtTabCtrl.bank;
}

/* 页表索引左偏二分：在 [lo, PAGE_TAB_NUM) 内返回 tab 最左匹配下标，
 * 未命中返回 DT_TAB_NONE。
 * 依赖 g_OsfpTabForm[] 存放约束（见 ral.c）：[0]=Page Low 固定，[1..)
 * 按 tab 升序、banked 页 4 项连续连排；元素数 == TabIndex_t 的
 * PAGE_TAB_NUM。 */
STATIC DWORD DtTabLeftFind(BYTE tab, DWORD lo)
{
    DWORD hi = (DWORD)PAGE_TAB_NUM;

    while(lo < hi) {
        DWORD mid = lo + ((hi - lo) >> 1);
        if(g_OsfpTabForm[mid].tab < tab) {
            lo = mid + 1U;
        } else {
            hi = mid;
        }
    }
    /* lo = [lo, PAGE_TAB_NUM) 内首个 tab >= 目标的下标；等于目标即最左匹配 */
    if((lo < (DWORD)PAGE_TAB_NUM) && (g_OsfpTabForm[lo].tab == tab)) {
        return lo;
    }
    return DT_TAB_NONE;
}

/* 寄存器属性二分：在 form->inf[0..num) 中按区间包含定位 offset 所在表项
 *（即 offset ∈ [inf[i].offset, inf[i].offset + inf[i].size) 的那一项）。
 * inf 表须按 offset 升序、区间互不重叠存放（DtTabCtrlInit 一次性校验）。
 * 未命中返回 NULL（空隙 / Reserved / Custom 区，由调用方按默认处理）。
 * 区间包含取代精确匹配（2026-10-19）：CMIS RAL 以字节为寻址单位，多字节
 * 寄存器的任意字节都可作单字节 WRITE 目标；精确匹配会让合并块内非起始
 * 字节无写入路径（BxH u16 高字节、9Fh LPL 0x09-0x7F、Page Low 0x1F Mask
 * 0x20-0x24 此前全部落空，且大 size 项整块写因尾字节查不到而整帧丢弃）。 */
STATIC CONST OsfpRegAttr_t *DtRegFind(CONST OsfpTabForm_t *form, BYTE offset)
{
    BYTE lo = 0U;
    BYTE hi = form->num;
    CONST OsfpRegAttr_t *best = NULL;

    while(lo < hi) {
        BYTE mid = (BYTE)(lo + ((hi - lo) >> 1));
        if(form->inf[mid].offset <= offset) {
            best = &form->inf[mid];         /* 候选 = 起点 ≤ offset 的最右表项 */
            lo   = (BYTE)(mid + 1U);
        } else {
            hi = mid;
        }
    }
    /* (DWORD) 求和：9Fh {0x08,120} 之和恰为 128，紧贴页尾边界 */
    if((best != NULL) &&
       ((DWORD)offset < ((DWORD)best->offset + (DWORD)best->size))) {
        return best;
    }
    return NULL;
}

/* 指令解析核心：定位 memaddr 所在页的 g_OsfpTabForm 下标，并经区间包含二分
 * 得出寄存器属性。form_idx 恒出参——页登记即给下标，与页内 offset 是否命中
 * 无关（DtFrameCb 写存储路径需要它把数据落到对应 SRAM 影子槽位）：
 *   返回 DT_TAB_NONE → 页未登记 / bank 越界，*attr 置 NULL；
 *   否则返回表下标，*attr 为 DtRegFind 结果 = 含 memaddr 页内偏移的表项
 *   （空隙 / Reserved / Custom 区可为 NULL）。 */
STATIC DWORD DtParseForm(BYTE memaddr, CONST OsfpRegAttr_t **attr)
{
    BYTE page;
    DWORD idx;

    *attr = NULL;

    if((memaddr & 0x80U) == 0U) {           /* 00h-7Fh：Lower 半区 → Page Low */
        /* Page Low 固定 g_OsfpTabForm[0]，直取不二分：其 tab 值与 Page 00h
         * 同为 0x00 会撞 LeftFind；且 bank 不影响索引（per-bank 的 Flags
         * Summary 属表内数据，非索引职责） */
        *attr = DtRegFind(&g_OsfpTabForm[0], (BYTE)(memaddr & 0x7FU));
        return 0U;
    }

    /* 80h-FFh：Upper 半区 → 当前 page，从 [1] 起搜（跳过 [0] 的 Page Low） */
    page = g_DtTabCtrl.page;

    idx = DtTabLeftFind(page, 1U);
    if(idx == DT_TAB_NONE) {
        return DT_TAB_NONE;                 /* 页未登记 */
    }
    if(g_OsfpTabForm[idx].banked != 0U) {   /* banked 页：索引顺延 bank 个 */
        idx += (DWORD)g_DtTabCtrl.bank;
        if((idx >= (DWORD)PAGE_TAB_NUM) || (g_OsfpTabForm[idx].tab != page)) {
            return DT_TAB_NONE;             /* bank 越界（该页不足 4 个 bank） */
        }
    }
    /* unbanked 页：CMIS 规定 BankSelect 被忽略，不加 bank 偏移 */

    *attr = DtRegFind(&g_OsfpTabForm[idx], (BYTE)(memaddr & 0x7FU));
    return idx;
}

CONST OsfpRegAttr_t *DtParse(BYTE memaddr)
{
    CONST OsfpRegAttr_t *attr = NULL;

    (void)DtParseForm(memaddr, &attr);
    return attr;
}

/*================ 指令解析帧回调与 deferred 操作 FIFO ================
 * DtFrameCb 注册到 I2cSlaveRegisterFrameCb 后由 I2C1 EV ISR 在 STPDET
 * 时调用（I2cSlaveInit 内已默认接线）。deferred FIFO 为单生产者（ISR，
 * DtFrameCb → DtDeferredPush）/ 单消费者（主循环，DtDeferredPoll）无锁
 * 环形：head 仅 ISR 写、tail 仅主循环写，数据一致性由 head/tail 的
 * volatile 发布顺序保证（经典 SPSC 形态）。oprfunc 入参必须复制——
 * cmd 指向的 g_RxBuf 是帧暂存，回调返回即被下一帧覆盖，不能跨帧持有
 * 指针。 */

/* deferred FIFO 深度：2026-10-16 由 8 翻倍为 16（用户定案）——host 背靠背
 * 写触发寄存器（如 D9H 邮箱）时降低队列满丢命令概率。必须为 2 的幂
 * （head/tail 用 & (DEPTH-1) 取索引）。满仍静默丢弃返回 ERR（ISR 无
 * 阻塞/反馈通道，见 DtDeferredPush 注释）。 */
#define DT_DEFER_FIFO_DEPTH     16U                 /* deferred FIFO 深度 */
#define DT_DEFER_CMD_MAX        8U                  /* 单条入参副本上限（与 host ≤8B/帧约定一致） */

/* deferred 操作项：oprfunc + 入参副本。 */
typedef struct {
    OprateFunc func;                                /* 待执行的 oprfunc */
    BYTE       len;                                 /* cmd 有效字节数 */
    BYTE       cmd[DT_DEFER_CMD_MAX];               /* 入参副本 */
} DtDeferItem_t;

STATIC DtDeferItem_t g_DtDeferFifo[DT_DEFER_FIFO_DEPTH];
STATIC volatile DWORD g_DtDeferHead = 0U;           /* 生产者索引（ISR 压入） */
STATIC volatile DWORD g_DtDeferTail = 0U;           /* 消费者索引（主循环取出） */

/* 压入一个 deferred 操作（ISR 上下文调用）。FIFO 满或入参超上限时丢弃并
 * 返回 ERR——ISR 无法阻塞等待消费者，超长截断会曲解 host 写入意图。 */
DWORD DtDeferredPush(OprateFunc func, CONST BYTE *cmd, BYTE len)
{
    DWORD idx;
    DtDeferItem_t *item;
    BYTE i;

    if((func == NULL) || (cmd == NULL) ||
       (len > DT_DEFER_CMD_MAX) ||
       ((g_DtDeferHead - g_DtDeferTail) >= DT_DEFER_FIFO_DEPTH)) {
        return ERR;
    }
    idx  = g_DtDeferHead & (DT_DEFER_FIFO_DEPTH - 1U);
    item = &g_DtDeferFifo[idx];
    item->func = func;
    item->len  = len;
    for(i = 0U; i < len; i++) {
        item->cmd[i] = cmd[i];
    }
    g_DtDeferHead++;
    return OK;
}

/* 取一个 deferred 操作执行（主循环上下文轮询）。每次调用取一个，返回 1
 * 表示执行了操作、0 表示 FIFO 空。oprfunc 在此允许阻塞/Flash 写。 */
DWORD DtDeferredPoll(VOID)
{
    DWORD idx;
    DtDeferItem_t *item;

    if(g_DtDeferTail == g_DtDeferHead) {
        return 0U;                                  /* 空 */
    }
    idx  = g_DtDeferTail & (DT_DEFER_FIFO_DEPTH - 1U);
    item = &g_DtDeferFifo[idx];
    item->func(item->cmd, item->len);
    g_DtDeferTail++;
    return 1U;
}

/* 当前待处理的 deferred 操作数。 */
DWORD DtDeferredCount(VOID)
{
    return (g_DtDeferHead - g_DtDeferTail);
}

/* 后台指令执行服务（主循环每轮调用）：把 deferred FIFO 里待处理的 oprfunc
 * 逐项执行至清空。deferred 项由 host 写帧触发、深度 16，正常节奏下每轮
 * 至多消化 1-2 项；连续 drain 不饿死主循环——FIFO 空即返回。 */
VOID CmdExeBackground(VOID)
{
    while(DtDeferredPoll() != 0U) {
        /* 逐项执行直至 FIFO 空 */
    }
}

/* 全程可写校验：[off, off+len) 覆盖的每个字节都须落在同 memtype、非 RO 的
 * 表项内。每轮取当前字节所在表项、跳到该表项末尾；len ≤ DT_DEFER_CMD_MAX
 * 且 off+len ≤ 128 已由调用方保证，故至多 len 轮、无回绕。表项区间互不重叠
 * 由 DtTabCtrlInit 校验，此处不做重复判定。 */
STATIC BOOL DtRangeWritable(TabIndex_t form_idx, BYTE off, BYTE len, BYTE memtype)
{
    DWORD p = (DWORD)off;
    DWORD e = (DWORD)off + (DWORD)len;

    while(p < e) {
        CONST OsfpRegAttr_t *a = DtRegFind(&g_OsfpTabForm[form_idx], (BYTE)p);

        if((a == NULL) || (a->memtype != memtype) ||
           (a->acctype == ACC_RO) || (a->acctype == ACC_RO_COR)) {
            return FALSE;
        }
        p = (DWORD)a->offset + (DWORD)a->size;       /* 跳到下一表项起点 */
    }
    return TRUE;
}

/* STPDET 帧回调：解析当前写帧，按访问类型 / opftype 路由——
 *   RO / RO_COR 写     → 丢弃（CMIS：RO 写无效果）；
 *   oprfunc != NULL    → 触发寄存器，须整寄存器写（off==attr->offset 且
 *                        len==attr->size），oprfunc 独占语义（126/127 页/
 *                        bank 切换、密码、复位触发等），不再落存储；
 *   纯存储 MEMT_SRAM   → WriteProtocolMsaSpace 写 SRAM 影子。CMIS 字节
 *                        寻址：帧起点可在表项任意字节（区间包含定位），
 *                        [off, off+len) 整段须同 memtype 非 RO——
 *                        DtRangeWritable 逐表项遍历，跨 128B 半页 / 异质区 /
 *                        中间跨进 RO 一律丢弃；
 *   MEMT_FLASH         → 当前表无 RW Flash 寄存器；未来 NV 写走 dirty 队列
 *                        在线程上下文持久化（ISR 禁 Flash XIP 停等）；
 *   MEMT_NONE          → 无存储，丢弃。
 * 帧首字节 = 本帧写窗口地址：纯写帧时等于 g_MgmtMemAddr；合并写读事务中
 * g_MgmtMemAddr 已被读相位 TI 自增，必须以帧内记录为准。 */
VOID DtFrameCb(DWORD frame_bytes)
{
    BYTE *rxbuf;
    BYTE memaddr;
    BYTE off;
    BYTE len;
    DWORD form_idx;
    CONST OsfpRegAttr_t *attr;

    (void)frame_bytes;                              /* 数据长度以 g_RxBuf[I2C_RX_LEN] 为准 */

    rxbuf = I2cSlaveGetRxBuf();
    len   = rxbuf[I2C_RX_LEN];
    if(len == 0U) {
        return;                                     /* 纯读/纯设地址帧：无写数据，无指令可分发 */
    }

    memaddr  = rxbuf[I2C_RX_ADDR];
    off      = (BYTE)(memaddr & 0x7FU);
    form_idx = DtParseForm(memaddr, &attr);
    if((form_idx == DT_TAB_NONE) || (attr == NULL)) {
        return;                                     /* 页未登记 / 空隙 / Reserved 区：丢弃 */
    }

    /* 访问类型门禁：RO 寄存器写入丢弃 */
    if((attr->acctype == ACC_RO) || (attr->acctype == ACC_RO_COR)) {
        return;
    }

    if(attr->oprfunc != NULL) {                     /* 触发寄存器：整寄存器写。
                                                     * 区间包含只保证帧首落在本表项，
                                                     * 不保证帧尾——跨界帧会把别处字节
                                                     * 喂进回调（PasswordEntry 从 0x7C
                                                     * 起 len=4 会拿 0x7E-0x7F 的
                                                     * Bank/Page 当密码，D9H REGADDR
                                                     * 从 0x02 起 len=4 会错位写影子）。
                                                     * OprateFunc(cmd,len) 无偏移参数，
                                                     * 短帧会让回调越读 g_RxBuf，故长度
                                                     * 在此判而非回调。双上下文分发
                                                     * （ISR vs 主循环）无互斥，回调须
                                                     * 遵守 ral_operate.h 并发契约 */
        if((off != attr->offset) || (len != attr->size)) {
            return;
        }
        if(attr->opftype == OPF_IMMEDIATE) {        /* 短操作：ISR 内立即执行 */
            attr->oprfunc(&rxbuf[I2C_RX_DATA], len);
        } else if(attr->opftype == OPF_DEFERRED) {  /* 重操作：入参副本压 FIFO，主循环执行 */
            (void)DtDeferredPush(attr->oprfunc, &rxbuf[I2C_RX_DATA], len);
        } else {
            /* OPF_NONE 兜底：表项矛盾（oprfunc != NULL 但 opftype == NONE），忽略 */
        }
        return;
    }

    /* 纯存储寄存器：仅 SRAM 影子可在 ISR 写；Flash 持久化归线程 dirty 队列 */
    if(attr->memtype != MEMT_SRAM) {
        return;
    }

    /* 顺序写边界：页内 [off, off+len) 不得越过 128B 半页（off+len==128 合法：
     * 恰好写满半页末字节；>128 说明 host 帧越界，丢弃） */
    if(((DWORD)off + (DWORD)len) > (DWORD)ADPAT_MGMT_PAGE_SIZE) {
        return;
    }

    /* 全程可写校验：[off, off+len) 每个字节都须落在同 memtype、非 RO 的表项内
     * （只查头尾会放行「头尾同属性、中间跨进异质区/RO」的写穿） */
    if(DtRangeWritable((TabIndex_t)form_idx, off, len, attr->memtype) == FALSE) {
        return;
    }

    /* 入参校验（页越界/范围越界/NULL/len==0）在调用方已保证，返回值忽略 */
    (void)WriteProtocolMsaSpace(attr->memtype, (TabIndex_t)form_idx, off,
                                &rxbuf[I2C_RX_DATA], len);
}

/*================ 读相位页快照（g_ReadSrcCb 数据源）================*/

/* 读相位数据源回调（I2cSlaveInit 注册到 I2cSlaveRegisterReadSource）：
 * I2C1 EV ISR 在读相位 ADDSEND+TR 时调用，把「从 memaddr 起、半区内回卷」
 * 的 128 字节填入 buf——TI 随后从 g_TxIdx=0 顺序流出，首字节即 memaddr 处
 * 值，128 次后按 MEM_ADD_INC 同半区回卷，与轮转填充一一对应。
 * 半区选择：bit7=0 → Page Low 影子槽（g_OsfpTabForm[0]）；
 *          bit7=1 → g_CurPageId 影子槽（bank 已含在解析结果内）；
 *          g_CurPageId == 哨兵（页未登记）→ 全 0xFF dummy（CMIS 未实现页）。
 * 统一走 SRAM 影子：MEMT_FLASH 页（00h/01h 静态宣告）依赖启动时 Flash
 * 参数区整体镜像 [7] 的既定加载流程。填充后按本页属性表把 WO/WO_SC 字节
 * 清零（host 读回 0），再交 TI 流出。纯 SRAM 拷贝，ISR 安全。
 * len 非 128 / buf 空：契约外调用，丢弃。 */
VOID DtReadSource(BYTE memaddr, BYTE *buf, DWORD len)
{
    CONST OsfpTabForm_t *form;
    CONST BYTE *slot;
    BYTE off;
    DWORD i;

    if((buf == NULL) || (len != (DWORD)ADPAT_MGMT_PAGE_SIZE)) {
        return;
    }

    if((memaddr & 0x80U) == 0U) {           /* Lower 半区 → Page Low 槽 */
        form = &g_OsfpTabForm[0];
        slot = ADPAT_MGMT_MAP_PAGE(PAGE_ID_LOW);
    } else if(g_CurPageId < (TabIndex_t)PAGE_TAB_NUM) {   /* Upper → 当前页槽 */
        form = &g_OsfpTabForm[(DWORD)g_CurPageId];
        slot = ADPAT_MGMT_MAP_PAGE(g_CurPageId);
    } else {
        for(i = 0U; i < len; i++) {
            buf[i] = 0xFFU;                 /* 未登记页：全 dummy */
        }
        return;
    }

    off = (BYTE)(memaddr & 0x7FU);          /* 从 memaddr 起半区内回卷轮转填充 */
    for(i = 0U; i < len; i++) {
        buf[i] = slot[((DWORD)off + i) & 0x7FU];
    }

    /* WO/WO_SC 读回 0：MEMT_NONE 无存储，影子字节是参数区镜像残留、值无定义
     * ——CMIS Table 8-3 规定 WO/SC host 读恒 0（自清触发位），WO 读值不指定
     * （统一回 0）。逐表项把本页 WO/WO_SC 寄存器覆盖字节在输出快照清零：读相位
     * 恒 128B 全半区，每影子字节恰出现一次，(p-off)&0x7F 即其输出位置。
     * 说明：0x1A ModuleControl 的 WO/SC bit3 不走这里——该寄存器整字节 ACC_RW，
     * 其 oprfunc 写影子时已把 bit3 归一为 0（见 ral_operate.c）。 */
    for(i = 0U; i < (DWORD)form->num; i++) {
        CONST OsfpRegAttr_t *e = &form->inf[i];
        BYTE eend, p;

        if((e->acctype != ACC_WO_SC) && (e->acctype != ACC_WO)) {
            continue;
        }
        eend = (BYTE)(e->offset + e->size); /* ≤128，BYTE 不溢出 */
        for(p = e->offset; p < eend; p++) {
            buf[(BYTE)(p - off) & 0x7FU] = 0U;
        }
    }
}

/* COR（Clear-On-Read）寄存器读后清除：STPDET 时经读完成回调
 * （I2cSlaveRegisterReadDone，I2cSlaveInit 已默认接线 DtCorClear）调用，
 * I2C1 EV ISR 上下文。清除环形读区间 [memaddr, memaddr+rdlen)（mod 128
 * 半页）覆盖到的 ACC_RO_COR 影子字节——只清 host 实际读过的字节（精确
 * COR；快照期整半区清除会在部分读时丢未读事件）。rdlen 由调用侧饱和到
 * 128。半区/页定位规则与 DtReadSource 相同（写相位改页不影响——同一
 * 事务内读相位在前）。并发契约：影子字节单写原子（M33 保证），app 层
 * 对 COR 字节的置位 RMW 须 BASEPRI 锁内（docs/memory_map.md 三写者模型），
 * 否则与清零交错丢更新；末字节 TI 到 STPDET 的 µs 窗口内新锁存事件被一并
 * 清掉（概率性单事件丢失，bring-up 可接受）。 */
VOID DtCorClear(BYTE memaddr, BYTE rdlen)
{
    CONST OsfpTabForm_t *form;
    BYTE *slot;
    BYTE off, i, p;

    if(rdlen == 0U) {
        return;
    }

    if((memaddr & 0x80U) == 0U) {           /* Lower 半区 → Page Low */
        form = &g_OsfpTabForm[0];
        slot = ADPAT_MGMT_MAP_PAGE(PAGE_ID_LOW);
    } else {                                /* Upper 半区 → 当前 page */
        if(g_CurPageId >= (TabIndex_t)PAGE_TAB_NUM) {
            return;                         /* 哨兵：dummy 读，无 COR 可清 */
        }
        form = &g_OsfpTabForm[(DWORD)g_CurPageId];
        slot = ADPAT_MGMT_MAP_PAGE(g_CurPageId);
    }

    off = (BYTE)(memaddr & 0x7FU);
    for(i = 0U; i < form->num; i++) {
        CONST OsfpRegAttr_t *e = &form->inf[i];
        BYTE eend;

        if((e->acctype != ACC_RO_COR) || (e->memtype != MEMT_SRAM)) {
            continue;                       /* 只清 COR 且 SRAM 影子字节 */
        }
        eend = (BYTE)(e->offset + e->size); /* ≤128，BYTE 不溢出 */
        for(p = e->offset; p < eend; p++) {
            if(((BYTE)(p - off) & 0x7FU) < rdlen) {
                slot[p] = 0U;               /* 环形距离 < rdlen → 在读区间内 */
            }
        }
    }
}

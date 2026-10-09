/** @file adpat.h
 * @brief ADPAT 适配层：management map 的物理存储适配（RAL 表 ↔ SRAM/Flash）。
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * CMIS RAL 层（protocol/ral）用 memtype 标记每个寄存器值的存储地
 * （MEMT_FLASH / MEMT_SRAM / MEMT_NONE）。ADPAT 负责把 management map
 * 的页落到真实硬件资源，本文件定义其 SRAM 页缓存：
 *
 *   g_AdpatMgmtMapBuf：一维数组，长度 = PAGE_TAB_NUM × 128B（CMIS
 *   management 单页固定 128 字节），首地址绝对定位于 SRAM [7] 用户参数
 *   缓冲区基址 + 512B 处（0x20005700）。如此 Flash 主参数区
 *   （MEM_PARAM_MAIN_BASE 0x0806C000，32KB）可原封不动整体拷贝到 [7] 区，
 *   management map 影子自然落位，拷贝后无需搬移；管理页在 Flash 参数区
 *   内的偏移同为 +512 起（前 512B 预留作参数头/其它用途，参数区布局
 *   文档需登记此约定）。
 *
 *   页槽位约定：g_OsfpTabForm[i]（页表索引 i ∈ [0, PAGE_TAB_NUM)）的
 *   SRAM 槽位 = 数组 [i*128, (i+1)*128)。Page Low、Page 00h、banked 页
 *   各占独立槽位，经 ADPAT_MGMT_MAP_PAGE(i) 寻址；RAL 引擎对 MEMT_SRAM
 *   寄存器的读写落在对应槽位的 offset 处。
 *
 * 注意：[7] 区是用户参数的 SRAM 镜像，management map 影子只占
 * ADPAT_MGMT_MAP_BUF_OFFSET 起的 PAGE_TAB_NUM×128 字节——参数管理须为
 * 其预留这段，不与其它参数用途重叠；影子的持久化（写回 Flash 主参数区）
 * 由参数区流程负责。
 */
#ifndef ADPAT_H
#define ADPAT_H

#include "basedef.h"
#include "memory_map.h"     /* MEM_PARAM_BUF_BASE / MEM_PARAM_BUF_SIZE */
#include "ral.h"            /* TabIndex_t / PAGE_TAB_NUM / g_OsfpTabForm */

#define ADPAT_MGMT_PAGE_SIZE        128U        /* CMIS management map 单页字节数 */

/* 影子在 SRAM [7] 区内的偏移：前 512B 预留（参数头/其它用途），
 * Flash 主参数区整体原样拷贝到 [7] 后管理页即落位此偏移处。 */
#define ADPAT_MGMT_MAP_BUF_OFFSET   512U

/* 数组绝对定位地址 = SRAM [7] 区基址 + 512B。独立定义为无后缀字面量，
 * 供 section 名字符串化（.ARM.__at_0x20005700，armlink 绝对定位 ZI 段
 * 约定写法，AC5/AC6 均支持）。 */
#define ADPAT_MGMT_MAP_ADDR         0x20005700
#if ((ADPAT_MGMT_MAP_ADDR != (MEM_PARAM_BUF_BASE + ADPAT_MGMT_MAP_BUF_OFFSET)) || \
     (ADPAT_MGMT_MAP_BUF_OFFSET >= MEM_PARAM_BUF_SIZE))
#error "ADPAT_MGMT_MAP_ADDR must equal MEM_PARAM_BUF_BASE + 512 (SRAM [7] + 512B)"
#endif

/* management map SRAM 页缓存：一维数组，长度 = PAGE_TAB_NUM × 128B，
 * 首地址固定于 SRAM [7] 用户参数缓冲区。ZI 放置（无初始化值），
 * 不占 RW_IRAM1。容量校验（≤32KB）在 adpat.c 以编译期断言完成。 */
extern BYTE g_AdpatMgmtMapBuf[PAGE_TAB_NUM * ADPAT_MGMT_PAGE_SIZE];

/* Protocol MSA space（= management map SRAM 影子）的总字节数 */
#define ADPAT_PROTOCOL_MSA_SIZE     ((DWORD)PAGE_TAB_NUM * ADPAT_MGMT_PAGE_SIZE)

/*================ 协议空间（影子）访问 API：management application layer / RAL 引擎 ================
 * 面向程序员的拷贝式接口：按页表索引 + 页内偏移安全读写 SRAM 影子。
 * page_id：页表索引（TabIndex_t，g_OsfpTabForm 下标，如 PAGE_ID_LOW / PAGE_ID_00H；
 *          banked 页的 bank 由具体枚举项体现，如 PAGE_ID_A1H_2，本层不做 bank 运算）；
 * offset：页内偏移（00h-7Fh）；buf/len：数据缓冲及长度，允许部分页访问
 *         （CMIS 字节寻址模型），要求 offset + len ≤ 128。
 * 统一契约：成功 OK；页索引越界 / 页内范围越界 / buf 为空 / len=0（无意义访问）均 ERR。
 * 落地：经 SRAM 公共 API ReadSram/WriteSram(SRAM_ID_PARAM) 纯内存拷贝，无阻塞、
 * 无 NV 写时序副作用——本层只维护 SRAM 影子并即刻生效；MEMT_FLASH 寄存器的
 * 持久化（回写 Flash 主参数区、tWRITENV 完成语义）由参数管理流程负责，不在此层。
 * 注意：WriteSram 拒绝与目标区重叠的源缓冲，勿从影子内部指针写影子；
 * 热路径免检出入口用 ADPAT_MGMT_MAP_PAGE(i) 直达指针。 */
DWORD ReadSramProtocolMsaSpace(TabIndex_t page_id, BYTE offset, BYTE *buf, BYTE len);
DWORD WriteSramProtocolMsaSpace(TabIndex_t page_id, BYTE offset, CONST BYTE *buf, BYTE len);
DWORD ReadSramProtocolMsaPage(TabIndex_t page_id, BYTE *buf);          /* 整页 128B 读 */
DWORD WriteSramProtocolMsaPage(TabIndex_t page_id, CONST BYTE *buf);   /* 整页 128B 写 */

/* 页缓存槽位寻址宏：直达 g_OsfpTabForm[i] 的 SRAM 影子槽位首址。
 * 零拷贝、无检查——供 RAL 引擎热路径在已验证 page_id 后使用；
 * 要参数检查的程序员侧访问请用上面的拷贝 API。 */
#define ADPAT_MGMT_MAP_PAGE(i)      (&g_AdpatMgmtMapBuf[(DWORD)(i) * ADPAT_MGMT_PAGE_SIZE])

/*================ Flash 协议空间（用户参数·主区影子）访问 API ================
 * SRAM 影子（FLASH_ID_PARA_A，MEM_PARAM_MAIN_BASE 0x0806C000，32KB）的
 * 持久层：槽位布局与 SRAM [7] 影子同构（同用 ADPAT_MSA_SLOT_OFF 寻址），
 * 供参数管理流程做 MEMT_FLASH 寄存器的持久化与整区镜像维护。
 *
 * 与 SRAM 侧契约一致：成功 OK；页索引越界 / 页内范围越界 / buf 为空 /
 * len=0 均 ERR。读为 XIP 内存映射直读，字节级任意范围。
 *
 * 写路径特殊语义（源自律动资源层 WriteFlash 的 8B doubleword 编程模型）：
 *  - 目标 doubleword 必须处于已擦除态（0xFF）——闪存位只能 1→0，
 *    内部读-改-写合并只做对齐，不提供原位改写；未擦除时校验失败返回 ERR；
 *  - 寄存器级偏移/长度任意（内部按 8B 边界合并）；整页 128B 天然对齐直写。
 *
 * 上下文限制：擦除/编程为 ms 级且 XIP 下 CPU 停等——仅线程上下文调用；
 * host 访问路径（I2C ISR）只许走 SRAM 影子 API。擦除粒度为整个主区
 * （4×8KB 扇区，连带 512B 参数头），调用方须负责整体重建。 */
DWORD ReadFlashProtocolMsaSpace(TabIndex_t page_id, BYTE offset, BYTE *buf, BYTE len);
DWORD WriteFlashProtocolMsaSpace(TabIndex_t page_id, BYTE offset, CONST BYTE *buf, BYTE len);
DWORD ReadFlashProtocolMsaPage(TabIndex_t page_id, BYTE *buf);         /* 整页 128B 读 */
DWORD WriteFlashProtocolMsaPage(TabIndex_t page_id, CONST BYTE *buf);  /* 整页 128B 写 */
DWORD EraseFlashParamMainArea(VOID);                                   /* 整区擦除 → 0xFF */

/*================ memtype 分派统一封装 ================
 * 引擎按寄存器 OsfpRegAttr_t.memtype 标记直接调用，无需自行判断 SRAM/Flash：
 *   MEMT_FLASH → Flash 持久层；MEMT_SRAM → SRAM 影子；MEMT_NONE → ERR
 *   （无存储：WO 触发位/派生值，由 oprfunc 或上层特判）。
 * 参数校验契约与底层各函数一致（页越界/范围越界/buf 空/len==0 → ERR）。
 * 注意：WriteFlash 路径 ms 级且 XIP 停等，仅线程上下文；SRAM 路径无阻塞。 */
DWORD ReadProtocolMsaSpace(BYTE memtype, TabIndex_t page_id, BYTE offset, BYTE *buf, BYTE len);
DWORD WriteProtocolMsaSpace(BYTE memtype, TabIndex_t page_id, BYTE offset, CONST BYTE *buf, BYTE len);

#endif

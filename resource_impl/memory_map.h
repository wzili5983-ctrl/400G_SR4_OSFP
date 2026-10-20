/** @file memory_map.h
 * @brief GD32E512 flash memory map and SRAM layout.
 * @author Firmware team
 * Copyright 2026 Terabitcom, Inc.
 *
 * GD32E512 主存储区共 512KB，单页 8KB，共 64 页。
 * 分配原则：一切边界 8KB 对齐，无半页共用。WRP = 写保护区域。
 *
 * 地址图：
 *
 *     0x0800_0000  +------------------------------+ P0
 *                  |  Bootloader         32KB WRP | P3
 *     0x0800_8000  +------------------------------+ P4
 *                  |  密钥/设备身份        8KB WRP |
 *     0x0800_A000  +------------------------------+ P5
 *                  |  OTA 元数据 J-A       8KB    |  <- APP 升级信息
 *     0x0800_C000  +------------------------------+ P6
 *                  |  OTA 元数据 J-B       8KB    |  <- 保留
 *     0x0800_E000  +------------------------------+ P7
 *                  |  黑匣子/诊断          8KB    |
 *     0x0801_0000  +==============================+ P8
 *                  |  APP Slot A  固定运行区 184KB |
 *     0x0803_E000  +==============================+ P31
 *                  |  APP Slot B  升级唯一入口 184KB |
 *     0x0806_C000  +------------------------------+ P54
 *                  |  用户参数·主         32KB    |
 *     0x0807_4000  +------------------------------+ P58
 *                  |  用户参数·备份       32KB    |
 *     0x0807_C000  +------------------------------+ P62
 *                  |  其他参数 J-A         8KB    |
 *     0x0807_E000  +------------------------------+ P63
 *                  |  其他参数 J-B         8KB    |
 *     0x0808_0000  +------------------------------+
 */
#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H

#include "basedef.h"

/* Flash geometry: 512KB, 8KB per page, 64 pages. */
#define MEM_FLASH_BASE              0x08000000U
#define MEM_FLASH_SIZE              0x00080000U             /* 512KB */
#define MEM_FLASH_PAGE_SIZE         0x00002000U             /* 8KB */
#define MEM_FLASH_PAGE_NUM          64U

/* P0-P3: Bootloader, 32KB, write protected. */
#define MEM_BOOT_BASE               0x08000000U
#define MEM_BOOT_SIZE               0x00008000U             /* 32KB, P0-P3, WRP */

/* P4: key / device identity, 8KB, write protected. */
#define MEM_KEY_BASE                0x08008000U
#define MEM_KEY_SIZE                MEM_FLASH_PAGE_SIZE     /* 8KB, P4, WRP */

/* P5/P6: OTA metadata J-A / J-B, 8KB each. J-A holds the APP upgrade info
 * record (OtaInfo_t) at its page base; J-B is reserved. */
#define MEM_OTA_META_A_BASE         0x0800A000U             /* P5 */
#define MEM_OTA_META_B_BASE         0x0800C000U             /* P6 */
#define MEM_OTA_META_SIZE           MEM_FLASH_PAGE_SIZE     /* 8KB each */

/* P7: black box / diagnostics, 8KB. */
#define MEM_BLACKBOX_BASE           0x0800E000U             /* P7 */
#define MEM_BLACKBOX_SIZE           MEM_FLASH_PAGE_SIZE     /* 8KB */

/* P8-P30: APP slot A, 184KB, the fixed running region. */
#define MEM_APP_A_BASE              0x08010000U             /* P8 */
#define MEM_APP_SLOT_SIZE           0x0002E000U             /* 184KB, 23 pages */

/* P31-P53: APP slot B, 184KB, the A-mirror region and the SOLE APP upgrade
 * receive region: every APP upgrade (including the factory / blank-device
 * case) is received into B first. Its logical header always declares
 * FLASH_TYPE_APP + MEM_APP_A_BASE, so B can be promoted to A verbatim. */
#define MEM_APP_B_BASE              0x0803E000U             /* P31 */

/* P54-P57: user parameters main, 32KB. */
#define MEM_PARAM_MAIN_BASE         0x0806C000U             /* P54 */
#define MEM_PARAM_AREA_SIZE         0x00008000U             /* 32KB, 4 pages */

/* P58-P61: user parameters backup, 32KB. */
#define MEM_PARAM_BACKUP_BASE       0x08074000U             /* P58 */

/* P62/P63: misc parameters J-A / J-B, 8KB each. */
#define MEM_MISC_A_BASE             0x0807C000U             /* P62 */
#define MEM_MISC_B_BASE             0x0807E000U             /* P63 */
#define MEM_MISC_SIZE               MEM_FLASH_PAGE_SIZE     /* 8KB each */

/* U15 (SST25PF040C) DSP firmware image, in the external NOR's own address
 * space. The DSP raw boot payload starts at offset 0; the last 4KB sector is
 * reserved for the CDB management frame, so the payload never overlaps it. */
#define MEM_DSP_FW_BASE             0x00000000U
#define MEM_DSP_FW_SIZE             0x00080000U             /* 512KB */
#define MEM_DSP_FW_META_BASE        0x0007F000U             /* last 4KB sector */
#define MEM_DSP_FW_PAYLOAD_CAP      MEM_DSP_FW_META_BASE

/* ==================== SRAM 布局（0x2000_0000 - 0x2002_0000, 128KB） ====================
 *
 * 分区方案（10 区，容量核对：53.25 + 66.75 + 8 = 128.00 KB）：
 *
 * 固定区（[0]-[7]，53.25KB；[0] 保留备用，[1]-[3] 为 BL/APP 共享，
 * 两个工程的此处定义必须一致）：
 *
 *     0x2000_0000  +------------------------------+
 *                  |  [0] 保留备用(原共享区) 512 B |
 *     0x2000_0200  +------------------------------+
 *                  |  [1] 崩溃快照区         256 B |
 *     0x2000_0300  +------------------------------+
 *                  |  [2] Flash 页缓冲区      8 KB |
 *     0x2000_2300  +------------------------------+
 *                  |  [3] 加密/哈希工作区    512 B |
 *     0x2000_2500  +------------------------------+
 *                  |  [4] OTA 接收缓冲        4 KB |
 *     0x2000_3500  +------------------------------+
 *                  |  [5] DMA 缓冲区(32B对齐) 4 KB |
 *     0x2000_4500  +------------------------------+
 *                  |  [6] 通信缓冲区          4 KB |
 *     0x2000_5500  +==============================+
 *                  |  [7] 用户参数缓冲区     32 KB |
 *                  |      与 Flash 0x0806_C000 一一对应
 *     0x2000_D500  +==============================+  ← 固定区结束
 *
 * 灵活区（链接脚本放置；[8] 起点固定、终点由链接器决定；[9] 位置由链接器决定）：
 *
 *     0x2000_D500  +------------------------------+
 *                  |  [8] APP .data + .bss  ~66.75 KB
 *     0x2001_E000  +------------------------------+
 *                  |  [9] MSP 主栈 (↓ 增长)   8 KB |  计划位于 SRAM 顶部，
 *                  |                               |  实际位置以链接结果为准
 *     0x2002_0000  +------------------------------+  ← SRAM 顶部
 *
 * 配套要求（否则此图不生效）：
 *  - 链接脚本 .sct 的 RW_IRAM1 必须从 0x2000_D500 开始，容量 ≤ 0x12B00，
 *    否则 .data/.bss/Stack_Mem 会侵占 [0]-[7] 固定区
 *  - 启动文件 Stack_Size = 0x2000 (8KB)、Heap_Size = 0（无堆分配）
 */
#define MEM_SRAM_BASE               0x20000000U
#define MEM_SRAM_SIZE               0x00020000U             /* 128KB */
#define MEM_SRAM_LIMIT              (MEM_SRAM_BASE + MEM_SRAM_SIZE)

/* [0] 保留备用（原 BL/APP 共享工作区协议已删除，512B 区保留）。 */
#define MEM_SHARE_BASE              0x20000000U
#define MEM_SHARE_SIZE              0x00000200U             /* 512B */

/* [1] 崩溃快照区，256B，BL+APP 共用。 */
#define MEM_CRASH_SNAP_BASE         0x20000200U
#define MEM_CRASH_SNAP_SIZE         0x00000100U             /* 256B */

/* [2] Flash 页缓冲区，8KB，BL+APP 共用（页编程前的数据暂存）。 */
#define MEM_FLASH_PAGE_BUF_BASE     0x20000300U
#define MEM_FLASH_PAGE_BUF_SIZE     MEM_FLASH_PAGE_SIZE     /* 8KB = 一页 */

/* [3] 加密/哈希工作区，512B，BL+APP 共用。 */
#define MEM_CRYPTO_BASE             0x20002300U
#define MEM_CRYPTO_SIZE             0x00000200U             /* 512B */

/* [4] OTA 接收缓冲，4KB。 */
#define MEM_OTA_RX_BUF_BASE         0x20002500U
#define MEM_OTA_RX_BUF_SIZE         0x00001000U             /* 4KB */

/* [5] DMA 缓冲区，4KB，32B 对齐（0x3500 满足对齐）。 */
#define MEM_DMA_BUF_BASE            0x20003500U
#define MEM_DMA_BUF_SIZE            0x00001000U             /* 4KB */

/* [6] 通信缓冲区，4KB。 */
#define MEM_COMM_BUF_BASE           0x20004500U
#define MEM_COMM_BUF_SIZE           0x00001000U             /* 4KB */

/* [7] 用户参数缓冲区，32KB，SRAM 镜像，与 Flash 主参数区一一对应。 */
#define MEM_PARAM_BUF_BASE          0x20005500U
#define MEM_PARAM_BUF_SIZE          MEM_PARAM_AREA_SIZE     /* 32KB, ↔ MEM_PARAM_MAIN_BASE */

/* [8] APP .data + .bss 起始（固定），终点由链接脚本决定，计划 ≤ 0x2001E000。 */
#define MEM_APP_RAM_BASE            0x2000D500U

/* [9] MSP 主栈，8KB，向下增长（main + ISR + boot_api）。
 * 计划位于 SRAM 顶部 0x2001E000-0x2001FFFF；实际位置由链接器在 RW 区内安排。 */
#define MEM_STACK_SIZE              0x00002000U             /* 8KB，与启动文件 Stack_Size 一致 */

/* 布局一致性检查：固定区 [0]-[7] 首尾相接，[8] 紧接固定区末端。 */
#if (MEM_SHARE_BASE != MEM_SRAM_BASE)
#error "SRAM [0] must start at MEM_SRAM_BASE"
#endif
#if (MEM_CRASH_SNAP_BASE != (MEM_SHARE_BASE + MEM_SHARE_SIZE))
#error "SRAM [1] not contiguous after [0]"
#endif
#if (MEM_FLASH_PAGE_BUF_BASE != (MEM_CRASH_SNAP_BASE + MEM_CRASH_SNAP_SIZE))
#error "SRAM [2] not contiguous after [1]"
#endif
#if (MEM_CRYPTO_BASE != (MEM_FLASH_PAGE_BUF_BASE + MEM_FLASH_PAGE_BUF_SIZE))
#error "SRAM [3] not contiguous after [2]"
#endif
#if (MEM_OTA_RX_BUF_BASE != (MEM_CRYPTO_BASE + MEM_CRYPTO_SIZE))
#error "SRAM [4] not contiguous after [3]"
#endif
#if (MEM_DMA_BUF_BASE != (MEM_OTA_RX_BUF_BASE + MEM_OTA_RX_BUF_SIZE))
#error "SRAM [5] not contiguous after [4]"
#endif
#if (MEM_COMM_BUF_BASE != (MEM_DMA_BUF_BASE + MEM_DMA_BUF_SIZE))
#error "SRAM [6] not contiguous after [5]"
#endif
#if (MEM_PARAM_BUF_BASE != (MEM_COMM_BUF_BASE + MEM_COMM_BUF_SIZE))
#error "SRAM [7] not contiguous after [6]"
#endif
#if (MEM_APP_RAM_BASE != (MEM_PARAM_BUF_BASE + MEM_PARAM_BUF_SIZE))
#error "SRAM [8] must start right after fixed region end (0x2000D500)"
#endif

#endif

# 400G_SR4_OSFP 内存布局（Flash & SRAM）

> 项目：400G_SR4_OSFP 光模块固件（GD32E511，Cortex-M33，512KB Flash + 128KB SRAM，Keil µVision）
> 生成日期：2026-09-29 ｜ 性质：长期参考文档，与 `resource_impl/memory_map.h` 具有同等权威，改布局须同步更新
>
> 本文档覆盖：Flash 整体布局与各分区、SRAM 整体布局与十区、management map 的 SRAM 影子（Protocol MSA space）、镜像与工具链运行期约定、中断向量表重定位。

## 目录

- [Flash 整体布局与分区](#flash-整体布局与分区)
- [SRAM 整体布局与分区](#sram-整体布局与分区)
- [Protocol MSA space（management map SRAM 影子）](#protocol-msa-spacemanagement-map-sram-影子)
- [镜像与运行期内存约定（工具链视角）](#镜像与运行期内存约定工具链视角)
- [中断向量表与 VTOR 重定位](#中断向量表与-vtor-重定位)
- [Bootloader（救援模式，2026-10-14 落地）](#bootloader救援模式2026-10-14-落地)
- [维护说明](#维护说明)

## Flash 整体布局与分区

### 几何参数与分配原则

GD32E511 主存储区共 **512KB**，起始地址 `0x08000000`，结束于 `0x08080000`；单页 **8KB（0x2000）**，共 **64 页**（P0–P63）。分配原则：一切边界 8KB 对齐，无半页共用；WRP = 写保护区域，在资源层 API 直接拒绝写/擦（`resource_impl/memory_map.h:6-7`；`resource_impl/flashresource_impl.c:185-187, 234-236`）。

```c
/* resource_impl/memory_map.h:41-44 */
#define MEM_FLASH_BASE      0x08000000U
#define MEM_FLASH_SIZE      0x00080000U   /* 512KB */
#define MEM_FLASH_PAGE_SIZE 0x00002000U   /* 8KB/页 */
#define MEM_FLASH_PAGE_NUM  64U
```

### 地址图

```
0x0800_0000  +------------------------------+ P0
             |  Bootloader         32KB WRP | P3
0x0800_8000  +------------------------------+ P4
             |  密钥/设备身份       8KB WRP |
0x0800_A000  +------------------------------+ P5
             |  OTA 元数据 J-A      8KB     |  <- APP 升级信息
0x0800_C000  +------------------------------+ P6
             |  OTA 元数据 J-B      8KB     |  <- 保留
0x0800_E000  +------------------------------+ P7
             |  黑匣子/诊断         8KB     |
0x0801_0000  +==============================+ P8
             |  APP Slot A  固定运行区 184KB |
0x0803_E000  +==============================+ P31
             |  APP Slot B  升级唯一入口 184KB |
0x0806_C000  +------------------------------+ P54
             |  用户参数·主        32KB     |
0x0807_4000  +------------------------------+ P58
             |  用户参数·备份      32KB     |
0x0807_C000  +------------------------------+ P62
             |  其他参数 J-A        8KB     |
0x0807_E000  +------------------------------+ P63
             |  其他参数 J-B        8KB     |
0x0808_0000  +------------------------------+
```

### 分区总表

`g_FlashInfoImpl` 现包含 **12 个物理资源**：11 个片上 Flash 分区，以及独立 NOR 偏移地址空间中的 U15 DSP 固件区。`flashid` 与 `FlashId_t` 枚举值一一对应且即数组下标；`FrameHead_t.type/storeaddr` 描述逻辑镜像，不再用帧头字段直接表示物理 `FlashId_t`。

| # | 分区 | FLASH_ID | 地址范围 | 大小 | 页号 | WRP | 用途 | 所有者 |
|---|------|----------|----------|------|------|-----|------|--------|
| 0 | Bootloader | `FLASH_ID_BOOTLOADER` | 0x08000000–0x08007FFF | 32KB（0x8000） | P0–P3 | 是 | 引导程序、升级 B→A 搬运 | BL |
| 1 | 密钥/设备身份 | `FLASH_ID_KEY` | 0x08008000–0x08009FFF | 8KB（0x2000） | P4 | 是 | 密钥、设备身份信息 | BL |
| 2 | OTA 元数据 J-A | `FLASH_ID_METADATA_A` | 0x0800A000–0x0800BFFF | 8KB（0x2000） | P5 | 否 | `OtaInfo_t` APP 升级记录（页基址处，64B） | APP（OTA） |
| 3 | OTA 元数据 J-B | `FLASH_ID_METADATA_B` | 0x0800C000–0x0800DFFF | 8KB（0x2000） | P6 | 否 | 保留 | —（保留） |
| 4 | 黑匣子/诊断 | `FLASH_ID_BLACKBOX` | 0x0800E000–0x0800FFFF | 8KB（0x2000） | P7 | 否 | 诊断/黑匣子日志 | APP |
| 5 | APP Slot A | `FLASH_ID_APP_A` | 0x08010000–0x0803DFFF | 184KB（0x2E000，23 页） | P8–P30 | 否 | 固定运行区（始终从此运行） | APP（BL 负责升级时 B→A 拷贝） |
| 6 | APP Slot B | `FLASH_ID_APP_B` | 0x0803E000–0x0806BFFF | 184KB（0x2E000，23 页） | P31–P53 | 否 | A 镜像区、升级唯一入口 | APP |
| 7 | 用户参数·主 | `FLASH_ID_PARA_A` | 0x0806C000–0x08073FFF | 32KB（0x8000，4 页） | P54–P57 | 否 | 用户参数 | APP |
| 8 | 用户参数·备份 | `FLASH_ID_PARA_B` | 0x08074000–0x0807BFFF | 32KB（0x8000，4 页） | P58–P61 | 否 | 用户参数备份 | APP |
| 9 | 其他参数 J-A | `FLASH_ID_MISC_A` | 0x0807C000–0x0807DFFF | 8KB（0x2000） | P62 | 否 | 杂项参数日志页 A | APP |
| 10 | 其他参数 J-B | `FLASH_ID_MISC_B` | 0x0807E000–0x0807FFFF | 8KB（0x2000） | P63 | 否 | 杂项参数日志页 B | APP |
| 11 | U15 DSP 固件 | `FLASH_ID_DSP_FW` | NOR offset 0x00000000–0x0007FFFF | 512KB（0x80000） | 外部 NOR | 否 | DSP raw 启动负载 + CDB 管理帧 | APP（BL 无读写擦操作） |

注：所有者列按源码注释与升级流程归属推断（如 OTA 元数据 J-A 注释为 "APP 升级信息"，`resource_impl/memory_map.h:16`；B→A 提升由 Bootloader 执行，`resource/flashresource.h:79-81`），源码未对每区逐一显式声明所有者。

### A/B 双槽升级约定（fixed-A / mirror-B）

本项目采用固定 A / 镜像 B 方案（fixed-A / mirror-B）：

1. **Slot A 是固定运行区，Slot B 是唯一 APP 升级接收区**：任何 APP 升级——包括出厂/空片首次升级——都必须先接收到 B；CDB 不支持直接写 APP_A。
2. **APP 包只有一种逻辑定向**：帧头必须为 `FrameHead_t.type == FLASH_TYPE_APP` 且 `storeaddr == MEM_APP_A_BASE`。该逻辑头同时被 APP_A/APP_B 的 `FrameTargetsSlot` 接受，因此 BL 可把 B 原样复制到 A；不存在 `storeaddr == MEM_APP_B_BASE` 的 APP 包。
3. **接收门 + 校验先行**：`VerifyFrameHdrBuf` 先验 112B 帧头（company/type+storeaddr/几何/头 CRC32），通过后才允许擦写目标；`InstallSlotImage` 还会在擦除前完成 payload CRC32（`crc32data`）校验。
4. **掉电安全**：装包时先写 payload、最后写 512B 帧头区作为提交标记，中途掉电只会留下旧镜像或无有效帧头的目标，不会被深校验当作完整新镜像。
5. **确认后 BL 提升 B→A**：仅 APP_B 的 CDB Run 写 `OtaInfo_t` 确认记录；Bootloader 随后将 B 原样拷贝到 A，稳态不变量为 **Slot A == Slot B**。
6. **无 ping-pong、无回滚状态机**：OTA 元数据 J-A 仅保存单条 64B `OtaInfo_t` 确认记录（另带持久化 `update_cnt`），J-B 保留。

### CDB 逻辑定向与物理目标

CDB 0101h Start 从 `VendorData[112]` 取得模块原生小端 `FrameHead_t`，按 `(type, storeaddr)` 精确查表并锁存物理目标；后续 0103h/0107h/0109h 不再重新路由。标准 CDB `ImageSize` 字段仍为大端，且必须等于 `fwsize`（兼容 `fwsize + 0x200` 的整帧口径）。

| 逻辑 `type` | 逻辑 `storeaddr` | 物理写入目标 | APP 运行态 | BL 救援态 |
|---|---|---|---|---|
| `FLASH_TYPE_APP` | `MEM_APP_A_BASE` (0x08010000) | `FLASH_ID_APP_B` | 支持 | 支持 |
| `FLASH_TYPE_PARAM` | `MEM_PARAM_MAIN_BASE` (0x0806C000) | `FLASH_ID_PARA_A` | 支持 | 支持 |
| `FLASH_TYPE_PARAM` | `MEM_PARAM_BACKUP_BASE` (0x08074000) | `FLASH_ID_PARA_B` | 支持 | 支持 |
| `FLASH_TYPE_EXTFLASH` | `MEM_DSP_FW_BASE` (0) | `FLASH_ID_DSP_FW` / U15 | 支持 | **拒绝** |

其他组合在擦除前拒绝。有效的新 Start 在修改目标上下文前先作废旧会话；后续 GPIO/U15 初始化/擦除失败均保持 `IDLE + rxDone=0 + Ready=0`，0103h 不会沿用旧偏移写新目标。PARA_A/PARA_B 与 DSP_FW 的 Run 只注册 MCU 复位，不写 APP 的 `OtaInfo_t`；PARA_B 不会因此自动提升为 PARA_A。BL 虽保留相同的 `FLASH_ID_DSP_FW` 枚举与资源表项，但其 EXTFLASH Read/Write/Erase ops 为 `NULL`，且 CDB 上下文门先行返回不支持。

### U15 DSP 固件布局

U15 为 SST25PF040C 512KB NOR，使用独立于 MCU 地址空间的偏移编址：

```
NOR offset 0x00000  +------------------------------+
                    | DSP raw boot payload         |  从 0 开始，最大 0x7F000 B
NOR offset 0x7F000  +------------------------------+
                    | 512B CDB 管理帧头区           |  最后一个 4KB 扇区起点
                    | 其余保留                     |
NOR offset 0x80000  +------------------------------+
```

打包器生成的 framed 文件仍是“512B 帧头区 + raw payload”的 **CDB 传输包**，不能把整个 framed 文件原样烧到 U15 offset 0。固件在 0103h 将 payload 写到 NOR offset 0 起，在 0107h 将 512B 帧头区写到 `0x7F000`，再经 `VerifyFramedImage` 分块回读校验。DSP_FW Start 会先保持 DSP 复位并暂停 ODSP 周期访问；MSM 在 LOW_PWR 禁止重新进入 PWR_UP，已在 PWR_UP 的序列则冻结且不计超时，避免 DSP_RELEASE/CHIP_INIT 与 U15 擦写争用。Abort 不释放该升级状态，只有 MCU 复位后重新走启动流程。

### 8KB 页擦除粒度对参数区设计的意义

- **擦除以分区为单位整区逐页进行**：`EraseFlash` 对区域内每一页执行 `fmc_page_erase`，无子页擦除 API；因此任何一条记录的改写，最小代价就是其所在整页 8KB 的擦除（`resource_impl/flashresource_impl.c:225-249`）。
- **小记录也按页管理**：OTA 元数据 J-A 的 `OtaInfo_t` 仅 64B，但每次重写前都要整页擦除、页内其余保持 0xFF——这是围绕 8KB 粒度设计的直接例证（`resource_impl/flashresource_impl.h:94-102`）。
- **参数区采用主/备整区成对 + SRAM 镜像**：用户参数做成 32KB 主 + 32KB 备份（各 4 页），并配 32KB SRAM 参数缓冲 `MEM_PARAM_BUF` 与 Flash 主参数区一一对应，APP 在 RAM 中积累修改后整块写回 Flash，把页擦除次数降到最低（`resource_impl/memory_map.h:74-79, 158-160`）。
- **杂项参数用 J-A/J-B 双 8KB 页日志对**：按页粒度翻转写，避免在同一页内反复擦写。
- **编程粒度约束**：FMC 按 64 位双字编程，`offset/len` 必须 8 字节对齐（`FLASH_PROGRAM_UNIT = 8`，`resource_impl/flashresource_impl.c:42-43, 189-191`）；擦除后空区在偏移 0 读出 `crc32data = 0xFF…`，任何帧头校验必失败，天然充当"空槽"判定（`resource_impl/flashresource_impl.h:48-49`）。
- **内部 framed 区共用帧格式**：APP/PARA/MISC 的 A/B 六区在各自基地址携带 512B 帧头区（112B `FrameHead_t` + 0xFF 填充），payload 位于 +0x200；`fwsize` 上限 = 分区大小 − 512B。U15 同样使用 112B/512B 帧格式，但负载与管理帧分离，布局见上文。所有镜像长度按 CRC 外设要求 **4 字节对齐**；内部 Flash 最终双字写由尾块 0xFF 补齐到 8 字节。所有 CRC32 使用片上 CRC 约定（poly 0x04C11DB7、init 0xFFFFFFFF、无反射），打包工具须同约定。

### 升级校验链（接收 → 装包 → 确认）

| 步骤 | 函数 | 作用 |
|------|------|------|
| 1. 接收门 | `VerifyFrameHdrBuf` | 在首次擦写前校验 RAM 中的 112B 帧头：company、逻辑 type+storeaddr 与物理目标匹配、fwsize/容量、头 CRC32 |
| 2. 接收与装包 | CDB 0103h / 0107h，或 APP 辅助接口 `InstallSlotImage` | 先写 payload、最后写 512B 帧头区；CDB 按 Start 锁存的物理目标及 `FlashFramePayloadOffset/FlashFrameHeaderOffset` 写内部 Flash 或 U15 |
| 3. 回读深校验 | `VerifyFramedImage` | 从物理目标回读帧头与 payload，分块计算 CRC32 并与 `FrameHead_t.crc32data` 比对；`VerifySlotImage` 是内部 APP slot 的包装 |
| 4. 确认/复位 | CDB 0109h Run | APP_B 调 `OtaConfirm` 写 64B 记录后延时复位；PARA_A/PARA_B/DSP_FW 只延时复位 |
| 5. APP 提升 | Bootloader `BlBootDecide` | 仅对已确认的 APP_B 原样提升 B→A，稳态不变量 Slot A == Slot B |

### OTA 元数据 J-A 页内布局（OtaInfo_t，64B）

P5 页基址（`MEM_OTA_META_A_BASE`）处保存一条 64B 升级确认记录；每次重写前整页擦除，页内其余保持 0xFF（`resource_impl/flashresource_impl.h:94-151`）：

| 偏移 | 字段 | 说明 |
|------|------|------|
| +0x00 | magic[16] | `OTA_INFO_MAGIC` "TBCOTAUP" + 0xFF 填充 |
| +0x10 | ver | `OTA_INFO_VER` 0x00010000（记录布局版本） |
| +0x14 | state | `OtaState_t`：仅 IDLE / CONFIRMED |
| +0x15 | src | `OtaSrc_t`：仅 NONE / HOST |
| +0x16 | rsvd[2] | -> 0xFF |
| +0x18 | swver[16] | 目标固件版本（升级包 swver 拷贝） |
| +0x28 | fwsize | payload 大小 |
| +0x2C | crc32data | payload CRC32（`FrameHead_t` 拷贝） |
| +0x30 | crc32hdr | 帧头 CRC32 |
| +0x34 | update_cnt | OTA 升级计数（持久化，每次确认 +1） |
| +0x38 | crc32 | 覆盖 0x00..0x37（本字段按 0 计），片上 CRC 约定 |
| +0x3C | rsvd[4] | -> 0xFF |

state 状态机（`OtaState_t`）：**confirm-only**——无断点续传、无重试计数（`update_cnt` 是升级次数计数、非重试计数），flash 里只会出现 IDLE（擦除/无活动）与 CONFIRMED（APP 已确认，BL 提升 B→A）两种态。接收/装包/深校验的中间态全部由 APP 在 RAM 中自持（CDB 引擎 `g_CdbFwState`），不落 flash。


## SRAM 整体布局与分区

GD32E511 片上 SRAM 共 128KB（0x20000 B），地址空间 0x2000_0000 - 0x2002_0000（`MEM_SRAM_BASE` / `MEM_SRAM_SIZE`，见 resource_impl/memory_map.h:126-128）。本工程将其划分为 10 个区：固定区 [0]-[7] 共 53.25KB 由 `memory_map.h` 宏逐区钉死，灵活区 [8]（APP .data + .bss，约 66.75KB）与 [9]（MSP 主栈 8KB）由链接脚本/启动文件配套落实。容量核对 53.25 + 66.75 + 8 = 128.00 KB，与物理容量精确相等（resource_impl/memory_map.h:88）。

### 地址图

照 resource_impl/memory_map.h:93-119 的画法：

```
    0x2000_0000  +------------------------------+
                 |  [0] 保留备用(原共享区) 512 B |
    0x2000_0200  +------------------------------+
                 |  [1] 崩溃快照区         256 B |
    0x2000_0300  +------------------------------+
                 |  [2] Flash 页缓冲区      8 KB |
    0x2000_2300  +------------------------------+
                 |  [3] 加密/哈希工作区    512 B |
    0x2000_2500  +------------------------------+
                 |  [4] OTA 接收缓冲        4 KB |
    0x2000_3500  +------------------------------+
                 |  [5] DMA 缓冲区(32B对齐) 4 KB |
    0x2000_4500  +------------------------------+
                 |  [6] 通信缓冲区          4 KB |
    0x2000_5500  +==============================+
                 |  [7] 用户参数缓冲区     32 KB |
                 |      与 Flash 0x0806_C000 一一对应
    0x2000_D500  +==============================+  ← 固定区结束
    0x2000_D500  +------------------------------+
                 |  [8] APP .data + .bss  ~66.75 KB
    0x2001_E000  +------------------------------+
                 |  [9] MSP 主栈 (↓ 增长)   8 KB |  计划位于 SRAM 顶部，
                 |                               |  实际位置以链接结果为准
    0x2002_0000  +------------------------------+  ← SRAM 顶部
```

### 固定区 [0]-[7]（53.25KB）

地址与大小全部来自 memory_map.h:130-160 的宏定义，资源层实例表直接引用这些宏，改布局只动 `memory_map.h`（resource_impl/sramresource_impl.c:104-105）。

| 区号 | 名称 | 地址 | 大小 | 所有者（type） | 资源层 id | 读写属性 |
|------|------|------|------|----------------|-----------|----------|
| [0] | 保留备用（原 BL/APP 共享工作区） | 0x2000_0000 | 0x200（512 B） | SHARED | SRAM_ID_SHARE | 读/写/擦 |
| [1] | 崩溃快照区 | 0x2000_0200 | 0x100（256 B） | SHARED | SRAM_ID_CRASH | 读/写/擦 |
| [2] | Flash 页缓冲区 | 0x2000_0300 | 0x2000（8 KB） | SHARED | SRAM_ID_FLASHBUF | 读/写/擦 |
| [3] | 加密/哈希工作区 | 0x2000_2300 | 0x200（512 B） | SHARED | SRAM_ID_CRYPTO | 读/写/擦 |
| [4] | OTA 接收缓冲 | 0x2000_2500 | 0x1000（4 KB） | APP | SRAM_ID_OTA_RX | 读/写/擦 |
| [5] | DMA 缓冲区（32B 对齐） | 0x2000_3500 | 0x1000（4 KB） | APP | SRAM_ID_DMA | 读/写/擦 |
| [6] | 通信缓冲区 | 0x2000_4500 | 0x1000（4 KB） | APP | SRAM_ID_COMM | 读/写/擦 |
| [7] | 用户参数缓冲区 | 0x2000_5500 | 0x8000（32 KB） | APP | SRAM_ID_PARAM | 读/写/擦 |

要点：

- **共享/私有划分**：[0]-[3] 为 `SRAM_TYPE_SHARED`，BL 与 APP 两个工程的 `memory_map.h` 定义必须一致，否则跨工程数据交换会互相踩踏；[4]-[9] 为 `SRAM_TYPE_APP`，仅本工程使用（resource_impl/sramresource_impl.h:76-80）。
- **[0] 保留备用**：原 BL/APP 共享工作区协议（BootShare_t 状态区 + 服务表 + 有效性检查）已整体删除，512B 区保留，读写擦经通用资源层开放，作为未来 BL/APP 协议的存放地（resource_impl/sramresource_impl.h:120-125）。
- **[2] Flash 页缓冲区**：8KB 恰为一页，BL+APP 共用，用于页编程前的数据暂存（resource_impl/memory_map.h:138-140）。
- **[5] DMA 缓冲区**：基址 0x2000_3500 满足 32B 对齐（resource_impl/memory_map.h:150-152）。
- **[7] 用户参数缓冲区**：SRAM 镜像，与 Flash 主参数区 `MEM_PARAM_MAIN_BASE`（0x0806_C000，32KB）一一对应（resource_impl/memory_map.h:158-160）。
- **擦除语义**：`EraseSram` 将全区填 0xFF，模拟 Flash 擦除态（如复用崩溃快照前先失效旧数据）（resource/sramresource.h:38-45、resource_impl/sramresource_impl.c:87-102）。

### 灵活区 [8]/[9] 与链接脚本、启动文件的配套要求

memory_map.h 的地址图要成立，以下条件必须同时满足（resource_impl/memory_map.h:121-124），当前工程实际配置与之吻合：

- 链接脚本 `.sct` 的 `RW_IRAM1` 从 0x2000D500 开始、容量 0x12B00（74.75KB），否则 `.data/.bss/Stack_Mem` 会侵占 [0]-[7] 固定区：

  ```
  RW_IRAM1 0x2000D500 0x00012B00  {  ; RW data
    .ANY (+RW +ZI)
  }
  ```

  （projects/application/Objects/400G_SR4_OSFP.sct:12-14）
- 启动文件 `Stack_Size = 0x2000`（8KB）、`Heap_Size = 0`（无堆分配）（driver/mcu/startup/startup_gd32e511_512.s:33,44）。
- [8] `.data+.bss` 起点固定 0x2000D500，终点由链接器决定；g_SramInfo 表中其计划大小为 `MEM_SRAM_LIMIT - MEM_APP_RAM_BASE - MEM_STACK_SIZE` = 0x10B00（66.75KB），即计划终点 0x2001_E000（resource_impl/sramresource_impl.c:135-139）。
- [9] MSP 主栈 8KB 向下增长（main + ISR + boot_api），计划栈顶 0x2001_E000，实际位置以链接结果为准（resource_impl/memory_map.h:165-167、resource_impl/sramresource_impl.c:140-144）。

### 资源层映射：区索引即 SRAM_ID

资源层把 10 个区组织成实例表 `g_SramInfo[SRAM_ID_NUM]`，**枚举值与表索引一致，索引即区号**（resource_impl/sramresource_impl.h:61-74、resource_impl/sramresource_impl.c:110-145）。公共 API 为 `ReadSram / WriteSram / EraseSram(BYTE id, ...)`（resource/sramresource.h:47-49），`id` 越界返回 NULL/ERR。

| 区号 | SRAM_ID_xxx | 类型 | 操作挂载（read / write / erase） |
|------|-------------|------|----------------------------------|
| [0] | SRAM_ID_SHARE | SHARED | SramReadOp / SramWriteOp / SramEraseOp |
| [1] | SRAM_ID_CRASH | SHARED | SramReadOp / SramWriteOp / SramEraseOp |
| [2] | SRAM_ID_FLASHBUF | SHARED | SramReadOp / SramWriteOp / SramEraseOp |
| [3] | SRAM_ID_CRYPTO | SHARED | SramReadOp / SramWriteOp / SramEraseOp |
| [4] | SRAM_ID_OTA_RX | APP | SramReadOp / SramWriteOp / SramEraseOp |
| [5] | SRAM_ID_DMA | APP | SramReadOp / SramWriteOp / SramEraseOp |
| [6] | SRAM_ID_COMM | APP | SramReadOp / SramWriteOp / SramEraseOp |
| [7] | SRAM_ID_PARAM | APP | SramReadOp / SramWriteOp / SramEraseOp |
| [8] | SRAM_ID_APP_RAM | APP | SramReadOp / NULL / NULL（只读） |
| [9] | SRAM_ID_STACK | APP | SramReadOp / NULL / NULL（只读） |

[8]/[9] 覆盖运行期活数据（含当前栈），其写/擦除函数指针挂 NULL，公共层按 ERR 拒绝——这是设计上把这两区锁定为只读（resource_impl/sramresource_impl.h:96-101、resource_impl/sramresource_impl.c:135-144）。所有读写操作带溢出安全检查：`offset + len` 必须落在区内，src 与目标区不得重叠（resource_impl/sramresource_impl.c:36-45）。

### 布局一致性编译期检查

`memory_map.h` 末尾的 `#if` 链强制固定区 [0]-[7] 首尾相接、[8] 紧接固定区末端 0x2000D500，任何地址或大小改动导致错位都会在编译期触发 `#error`（resource_impl/memory_map.h:169-196）：

```c
#if (MEM_SHARE_BASE != MEM_SRAM_BASE)                       ... #error "[0] must start at MEM_SRAM_BASE"
#if (MEM_CRASH_SNAP_BASE != MEM_SHARE_BASE + MEM_SHARE_SIZE) ... #error "[1] not contiguous after [0]"
/* [2]-[7] 同构检查，逐区首尾相接 */
#if (MEM_APP_RAM_BASE != MEM_PARAM_BUF_BASE + MEM_PARAM_BUF_SIZE) ... #error "[8] must start right after fixed region end (0x2000D500)"
```


## Protocol MSA space（management map SRAM 影子）

Protocol MSA space 是 CMIS management map 在 SRAM 中的影子镜像，物理载体为 ADPAT 层的页缓存数组 `g_AdpatMgmtMapBuf`：一维数组，长度 = `PAGE_TAB_NUM × 128B`（CMIS management 单页固定 128 字节），经 armlink 绝对定位段 `.ARM.__at_0x20005700` 放置为 ZI 段，即 SRAM [7] 用户参数缓冲区基址 `0x20005500` + 512B（来源：`protocol/adpat/inc/adpat.h:35-53`、`protocol/adpat/src/adpat.c:17,29`）。

### 设计意图：与 Flash 主参数区同构拷贝

`ADPAT_MGMT_MAP_BUF_OFFSET = 512` 的约定使 Flash 主参数区（`MEM_PARAM_MAIN_BASE = 0x0806C000`，32KB）可以原封不动整体拷贝到 SRAM [7] 区，拷贝后 management map 影子自然落位，无需二次搬移；管理页在 Flash 参数区内的偏移同为 +512 起（来源：`adpat.h:37-39`、`resource_impl/memory_map.h:75-76,159-160`）。前 512B 预留作参数头/其它用途。影子的持久化（写回 Flash 主参数区）由参数区流程负责，ADPAT 不直接写 Flash（来源：`adpat.h:23-26`）。

### 影子区在 [7] 区内的偏移图

```
SRAM [7] 用户参数缓冲区  0x20005500 - 0x2000D4FF  (32KB = 0x8000，↔ Flash 0x0806C000)
  0x20005500 +0x0000  +------------------------------+
                     |  参数头预留（512B）            |
  0x20005700 +0x0200 +==============================+  ← g_AdpatMgmtMapBuf 起点
                     |  管理页槽位 [0] Page Low      |  当前在用 128B
  0x20005780 +0x0280 +------------------------------+
                     |  管理页槽位 [1] Page 00h      |  当前在用 128B
  0x20005800 +0x0300 +------------------------------+
                     |  槽位 [2..PAGE_TAB_NUM-1]     |  当前未用（数组已占位）
                     |  数组总长 149×128B            |
  0x2000A180 +0x4C80 +------------------------------+  ← 数组尾（ZI 段终点）
                     |  剩余参数区 0x3380 (12.875KB) |  参数用途，不与影子重叠
  0x2000D500 +0x8000 +==============================+  ← [7] 区终点
```

地址核算：`g_AdpatMgmtMapBuf` 首址 `0x20005700`，总长 `PAGE_TAB_NUM × 128B = 149 × 128 = 19072B = 0x4A80（18.625KB）`，末字节 `0x2000A17F`；[7] 区剩余 `0x2000D500 − 0x2000A180 = 0x3380`（12.875KB）供其它参数用途。当前仅槽位 [0]、[1] 共 256B 承载有效页数据（见下表），其余槽位为数组占位（ZI 无初始化值，上电内容由参数区加载流程建立，链接器不清零——来源：`adpat.c:26-30`）。

### 页槽位与 g_OsfpTabForm 索引对应表

槽位寻址宏 `ADPAT_MGMT_MAP_PAGE(i) = &g_AdpatMgmtMapBuf[i * 128]`（来源：`adpat.h:68`）。页表索引 `TabIndex_t` 当前共 149 项（`PAGE_TAB_NUM = 149`，来源：`protocol/ral/inc/ral.h:4-156`）；banked 页同 tab 连续 4 项、最左项 = bank0，匹配索引 = 最左项 + bank（来源：`ral.h:188-190`、`protocol/mci/inc/data_transfer.h:52-58`、`protocol/ral/src/ral.c:118-128`）。

| 槽位 i | 绝对地址 | 页表枚举项（示例） | 页号 tab | banked | 当前登记 |
|---|---|---|---|---|---|
| 0 | 0x20005700-0x2000577F | `PAGE_ID_LOW` | 0x00（Lower Memory 半区区分） | 否 | g_OsfpTabForm[0]（`g_OsfpInfPageLow`） |
| 1 | 0x20005780-0x200057FF | `PAGE_ID_00H` | 0x00 | 否 | g_OsfpTabForm[1]（`g_OsfpInfPage00H`） |
| 2 | 0x20005800-0x2000587F | `PAGE_ID_01H` | 0x01 | 否 | 未登记（初始化器零填充） |
| … | … | … | … | … | … |
| banked 页槽位 | … | `PAGE_ID_A1H_0..3`（bank0-3 各占独立槽位） | 0xA1 | 是（连续 4 项） | 未登记 |
| … | … | `PAGE_ID_10H_0..3` 等 | … | 是 | 未登记 |
| 148 | 0x2000A100-0x2000A17F | `PAGE_ID_A1H_3` | 0xA1 | 是 | 未登记 |

注意：`g_OsfpTabForm[PAGE_TAB_NUM]` 当前初始化器仅列出 [0]、[1] 两项，其余按 C 语义零填充（`tab=0, num=0, banked=0, inf=NULL`），即页表登记与 SRAM 槽位占位是解耦的——槽位按枚举全线占位，有效页数由页表登记数决定（来源：`ral.c:129-132`）。`DtParse` 二分以 `PAGE_TAB_NUM` 为上界但命中未登记项（`inf=NULL`）会返回 NULL 交调用方默认处理，不会误触影子区（来源：`data_transfer.h:51-64`）。

### 读写 API

拷贝式接口（程序员侧，带参数检查；2026-09-29 由单一读 API 扩为读写对称 + 整页封装）：

| API | 签名/定义 | 语义与边界 |
|---|---|---|
| `ReadSramProtocolMsaSpace` | `DWORD (TabIndex_t page_id, BYTE offset, BYTE *buf, BYTE len)`（`adpat.h:70`，实现 `adpat.c:54-61`） | 槽位 `offset` 处拷贝 `len` 字节 → `buf` |
| `WriteSramProtocolMsaSpace` | `DWORD (TabIndex_t page_id, BYTE offset, CONST BYTE *buf, BYTE len)`（`adpat.h:71`，实现 `adpat.c:64-71`） | `buf` → 槽位 `offset` 处，即刻生效；只维护 SRAM 影子，`MEMT_FLASH` 持久化归参数管理流程 |
| `ReadSramProtocolMsaPage` | `DWORD (TabIndex_t page_id, BYTE *buf)`（`adpat.h:72`，实现 `adpat.c:73-76`） | 整页 128B 读（读相位快照场景） |
| `WriteSramProtocolMsaPage` | `DWORD (TabIndex_t page_id, CONST BYTE *buf)`（`adpat.h:73`，实现 `adpat.c:78-81`） | 整页 128B 写（页初始化/整页刷新场景） |

统一契约（`AdpatMsaChk` 校验，`adpat.c:43-51`）：成功 `OK`；页索引越界 / `offset+len > 128` / `buf==NULL` / `len==0`（无意义访问）均 `ERR`。bank 由 page_id 枚举项体现（如 `PAGE_ID_A1H_2`），函数内不做 bank 运算。部分页访问合法（CMIS 字节寻址模型）。

Flash 持久层（用户参数·主区影子，`FLASH_ID_PARA_A`；2026-09-29 新增，槽位寻址与 SRAM 侧同构）：

| API | 签名/定义 | 语义与边界 |
|---|---|---|
| `ReadFlashProtocolMsaSpace` | `DWORD (TabIndex_t page_id, BYTE offset, BYTE *buf, BYTE len)`（`adpat.h:96`，实现 `adpat.c:117-124`） | XIP 直读，字节级任意范围，契约同上 |
| `WriteFlashProtocolMsaSpace` | `DWORD (TabIndex_t page_id, BYTE offset, CONST BYTE *buf, BYTE len)`（`adpat.h:97`，实现 `adpat.c:126-133`） | 经 `AdpatFlashMsaWrite` 8B doubleword 读-改-写合并（`adpat.c:92-114`）；**目标 doubleword 须已擦除**（位只能 1→0，合并不豁免擦除前提，未擦除时资源层校验失败返回 ERR） |
| `ReadFlashProtocolMsaPage` | `DWORD (TabIndex_t page_id, BYTE *buf)`（`adpat.h:98`，实现 `adpat.c:135-144`） | 整页 128B 读 |
| `WriteFlashProtocolMsaPage` | `DWORD (TabIndex_t page_id, CONST BYTE *buf)`（`adpat.h:99`，实现 `adpat.c:146-157`） | 整页 128B 直写（`512+page*128` 与 128B 天然 8B 对齐，免合并） |
| `EraseFlashParamMainArea` | `DWORD (VOID)`（`adpat.h:100`，实现 `adpat.c:159-162`） | 整区擦除：4×8KB 扇区 → 0xFF，**连带 512B 参数头**，调用方须整体重建 |

Flash 侧上下文限制：擦除/编程为 ms 级且 XIP 下 CPU 停等——**仅线程上下文**；host 访问路径（I2C ISR）只许走 SRAM 影子。备份区（`FLASH_ID_PARA_B`）的同步策略归参数管理流程，本层不触碰。

### 影子并发与一致性（三写者模型）

影子是三写者共享存储，撕裂风险全部长在「线程写 vs 中断读」的缝上：

| 写者 | 上下文 | 触碰方式 |
|---|---|---|
| I2C EV ISR（prio 2） | 中断 | **读+写**：读相位快照 128B（ADDSEND+TR → `g_ReadSrcCb`，TI 流出）；host 写帧经 `DtFrameCb` 落 MEMT_SRAM 影子、读帧经 `DtCorClear` 清 COR 位（均 ISR 内、单字节原子） |
| RAL 引擎 / 应用线程 | 线程 | host 写帧出环应用；模块侧更新监控/Flags/镜像 |
| Flash 持久化流程 | 线程（XIP 停等波及全部中断） | 擦/写 Flash 主区 |

风险登记：**R1** 快照撕裂（多字节寄存器写一半被快照，违反 CMIS §5.2.5.1 size-matched 原子读承诺）；**R2** RMW 丢失更新（engine 应用 host 写覆盖应用线程的同字节改动）；**R3** COR Flag 竞争（engine 清字节覆盖模块新置位 → 中断事件丢失）；**R4** PageMapping 撕裂（`g_DtTabCtrl`/126-127 镜像非原子更新 → 快照错槽）；**R5** 整页写窗口（128B memcpy 期间整页撕裂）；**R6** Flash 持久化窗口（XIP 停等期间 EV ISR 无法执行 → RBNE 溢出/TI 断流，最重）；**R7** 持锁超 I2C 字节预算的反向下溢。

规避规则（调用方纪律，写 API 自身不加锁——调用方持锁状态不可知，嵌套会死锁）：

1. **单写者所有权**：每影子字节唯一写者——RW/WO 仅 engine 线程；RO/RWW/Flags 仅应用线程；126/127+镜像仅 Dt 层。写前查 `acctype` 表确认 owner；Flags(RO) 与 Mask(RW) 永不共享字节（自定义区沿用）。
2. **寄存器粒度原子写**：多字节寄存器/RMW 全程在影子锁内完成；锁内严禁 Flash API 与长逻辑；单字节直写免锁。
3. **影子锁**：`AdpatShadowLock/Unlock`（adpat.h/adpat.c，**2026-10-13 已实现**）——线程专用、不可嵌套，`NVIC_DisableIRQ/EnableIRQ(I2C1_EV/ER_IRQn)` 直禁 I2C EV/ER 两中断源（效果等价原 BASEPRI 屏蔽 prio 2/3，保留 SysTick(0)/Timer2(1)；改用 NVIC API 与 i2c_slave.c 一致、免 `__NVIC_PRIO_BITS` 位移依赖）。ISR 内影子访问原子且禁止上锁。
4. **COR 协议**：engine 清 Flags 用 `DtCorClear`（ISR 内，单字节原子、不上锁）按 `flags &= ~快照值` 只清已被 host 读走的位；模块侧置位 RMW（`MsmShadowLatchBit`/`DpsmFlagStateChanged`/`MonFlagUpdate`）在影子锁内——线程侧置位有锁、ISR 侧清零原子，二者不再交错丢更新。
5. **页映射原子更新**：`DtTabCtrl` 与槽位 [0] 镜像锁内一次完成；快照侧按对齐 16 位单次读。
6. **批量写**：初始化在 host 总线使能前完成；运行期整页刷新按 `OsfpRegAttr_t` 表逐寄存器锁内写；整块拷贝避开 host 活动窗口。
7. **Flash 调度**：dirty-page 延迟队列攒批，总线静默期一次擦写；NV 写触发的持久化落在 CMIS `tWRITENV`(80ms) 合法 hold-off 窗口内；调试期入口断言非中断上下文（查 IPSR）。
8. **预算**：EV 响应预算 ≈ 2 字节时间（400kHz≈50µs / 1MHz≈16µs / 3.4MHz≈6µs）；3.4MHz 下只锁寄存器粒度。
9. **观测**：`g_ErrCnt.over/err` 上涨即 R6/R7 触发信号。

零拷贝热路径与落地：

| API | 签名/定义 | 语义与边界 |
|---|---|---|
| `ADPAT_MGMT_MAP_PAGE(i)` | 宏，返回槽位 i 首址（`adpat.h:78`） | RAL 引擎热路径直达 `g_OsfpTabForm[i]` 的 SRAM 影子槽位，无检查，调用方自证 page_id 合法 |
| `ReadSram` / `WriteSram`（公共层） | `DWORD (BYTE id, DWORD offset, BYTE *data, DWORD len)`（`resource/sramresource.h:47-48`） | 落地路径：`Read/WriteSram(SRAM_ID_PARAM, 512 + page_id*128 + offset, buf, len)`（`ADPAT_MSA_SLOT_OFF` 合成，`adpat.c:38-39`）；`WriteSram` 拒绝与目标区重叠的源缓冲 |

时序语义：本层为纯 SRAM 内存拷贝，无阻塞、无 NV 写时序副作用（`tWRITENV` 完成语义不在此层）。

### 最大容量与编译期断言

- 硬上限：`MEM_PARAM_BUF_SIZE − ADPAT_MGMT_MAP_BUF_OFFSET = 32768 − 512 = 32256B = 252` 页（按 128B/页计，与源码注释一致，来源：`adpat.c:20-24`）。
- 编译期断言（枚举常量 `#if` 不可见，用负长数组硬错误实现；0 长数组是 GNU 扩展会被静默接受，故失败分支取 -1）：

```c
/* adpat.c:23 — PAGE_TAB_NUM*128B ≤ 32KB-512B，当前 149 页 = 0x4A80，满足 */
typedef BYTE AdpatSizeCheck_t[
    (PAGE_TAB_NUM * ADPAT_MGMT_PAGE_SIZE <= (MEM_PARAM_BUF_SIZE - ADPAT_MGMT_MAP_BUF_OFFSET)) ? 1 : -1];
```

另有地址一致性 `#error` 校验 `ADPAT_MGMT_MAP_ADDR == MEM_PARAM_BUF_BASE + 512`（`adpat.h:45-48`）。

> 历史勘误（已修复）：`adpat.c` 文件头注释曾遗留早期版本表述「SRAM [2] Flash 页缓冲区首地址（0x20000300）」及容量笔误「255 页」，2026-09-29 已修正为「[7] 区基址 + 512B（0x20005700）」与「252 页」。


## 镜像与运行期内存约定（工具链视角）

本节从打包脚本、Keil 链接脚本、启动文件三个工具链产物出发，说明 APP 镜像的落盘格式、链接基址与 SRAM 运行期布局，并与 `resource_impl/memory_map.h` 的声明值逐项核对。

### 1. Framed 镜像格式（512B 帧头区 + 负载）

打包脚本 `tools/pack_framed_bin.py` 将原始 bin 封装为“512B 帧头区 + 负载”的传输/分区镜像。APP 的输入为 Keil 生成的 `bin/400G_SR4_OSFP.bin`；PARAM/MISC/EXTFLASH 使用各自 raw 数据。布局为：

```
+0x000  FrameHead_t  112B   crc32data | type | rsvd0[3] | storeaddr | fwsize |
                              company[16] / hdver[16] / swver[16] / modulename[16] |
                              rsvd1[16] / rsvd2[12] / crc32hdr
+0x070  rsvd         400B   固定 0xFF
+0x200  payload      fwsize B，4 字节对齐；文件本身不在尾部额外垫 0xFF
```

关键参数：

| 参数 | 值 |
|---|---|
| 帧头区总大小 `FRAME_HDR_AREA_SIZE` | 0x200（512B） |
| `FrameHead_t` 结构体大小 `FRAME_HDR_SIZE` | 112B（0x70） |
| 传输包负载起始偏移 `FRAME_PAYLOAD_OFF` | 0x200 |
| APP 负载上限 `APP_BIN_LIMIT` | 0x2DE00（187904B ≈ 183.5KB） |
| DSP_FW 负载上限 `DSP_FW_PAYLOAD_CAP` | 0x7F000（520192B = 508KB） |
| 帧头内 `crc32hdr` 偏移 | +0x6C（最后一个 DWORD） |

因此内部 APP 镜像在 Flash 中的实际占用为“分区基址 + 0x200 帧头区 + 负载”，负载上限恰为槽容量 0x2E000 减去 0x200。U15 是例外：传输包仍在 +0x200 携带 payload，但固件落盘时把 raw payload 写到 NOR offset 0、把帧头区写到 0x7F000。

CRC 约定与 GD32 片内 CRC 外设一致（对应固件侧 `BoardFlashCrc32`）：多项式 0x04C11DB7、初值 0xFFFFFFFF、无输入/输出反射、无最终异或，数据按小端 32 位字、每字 MSB 在前喂入，长度必须为 4 的倍数；`crc32hdr` 计算时该字段自身按 0 代入。

### 2. 逻辑定向、链接基址与物理槽位

打包与校验按 `flash_type + store_addr` 强制以下几何：

| 逻辑类型 | `store_addr` | 逻辑区域 / CDB 物理目标 | 负载上限 |
|---|---|---|---|
| `FLASH_TYPE_APP` | 0x08010000 | APP_A 逻辑镜像 / 物理 APP_B | 0x2DE00 |
| `FLASH_TYPE_PARAM` | 0x0806C000 | PARA_A / 物理 PARA_A | 0x7E00 |
| `FLASH_TYPE_PARAM` | 0x08074000 | PARA_B / 物理 PARA_B | 0x7E00 |
| `FLASH_TYPE_MISC` | 0x0807C000 / 0x0807E000 | MISC_A / MISC_B（仅通用打包，非当前 CDB 路由） | 0x1E00 |
| `FLASH_TYPE_EXTFLASH` | 0x00000000 | DSP_FW / 物理 U15（仅 APP） | 0x7F000 |

- **APP 负载必须链接在 store_addr + 0x200**，即 0x08010200。打包时仅对 APP 校验负载前 8 字节：初始 MSP 必须落在 SRAM `[0x20000000, 0x20020000)` 内，复位向量（清 Thumb 位后）必须落在 `[link_base, link_base + fwsize)` 内。PARAM/MISC/DSP 是数据镜像，不做 MSP/复位向量检查。
- **固定 A 运行 / 镜像 B 接收**：APP 包永远声明 `FLASH_TYPE_APP + 0x08010000`，CDB 一律写物理 Slot B；直接以 APP_B 地址或其他 type/storeaddr 组合打包会被拒绝。BL 提升时无需改写帧头。

### 3. 链接脚本（.sct）内存区参数

`outputs/` 下的 .sct 已从工作区删除，当前有效的散列加载文件为 `projects/application/Objects/400G_SR4_OSFP.sct:5-14`：

```
LR_IROM1 0x08010200 0x0002DE00  {          ; 加载区 = 执行区
  ER_IROM1 0x08010200 0x0002DE00  {        ; 只读：向量表 + 根段 + RO/XO
   *.o (RESET, +First)
   *(InRoot$$Sections)
   .ANY (+RO)
   .ANY (+XO)
  }
  RW_IRAM1 0x2000D500 0x00012B00  {        ; RW/ZI 数据（含 Stack_Mem）
   .ANY (+RW +ZI)
  }
}
```

| 区 | 基址 | 容量 | 十六进制换算 |
|---|---|---|---|
| ER_IROM1（只读执行区） | 0x08010200 | 0x2DE00 | 187904B ≈ 183.5KB |
| RW_IRAM1（RW/ZI） | 0x2000D500 | 0x12B00 | 76544B = 74.75KB |

即：Flash 侧镜像链接在 Slot A 基址 + 0x200 处（与第 2 节一致）；SRAM 侧链接器从 0x2000D500（固定区 [0]-[7] 之后）开始安放 `.data/.bss` 及启动文件中的 `Stack_Mem`，该区上限即 SRAM 顶部 0x20020000。

### 4. 栈 / 堆尺寸

`driver/mcu/startup/startup_gd32e511_512.s:33,44`：

```
Stack_Size  EQU  0x00002000    ; 8KB
Heap_Size   EQU  0x00000000    ; 无堆
```

栈顶符号 `__initial_sp` 取自 STACK 区（NOINIT READWRITE），因此栈空间落在 RW_IRAM1 内、由链接器在 `.data/.bss` 之上安排，并不独占 `memory_map.h` 注释中计划的 0x2001E000-0x2001FFFF 顶端 8KB（该位置仅是计划值，源文件 memory_map.h:165-167 亦注明"实际位置由链接器在 RW 区内安排"）。Heap_Size 为 0，固件不使用堆分配。

### 5. 启动代码中的 SRAM 固定区初始化

`main/main.c:40-49` 的 `main()` 仅调用 `SystemCoreClockUpdate()` 与 `InitBoardResources()`，未在 main.c 中找到对 SRAM 固定区（[0]-[7]）的显式初始化调用；固定区缓冲区（Flash 页缓冲、OTA 接收缓冲、参数缓冲等）的使用与清零散布在各资源实现（`InitBoardResources` 及其下游）中，main.c 本身无独立初始化语句。未在 main.c 中找到额外的 SRAM 布局相关代码。

### 6. 与 memory_map.h 声明值的一致性核对

| 核对项 | memory_map.h 声明 | 工具链实际值 | 结论 |
|---|---|---|---|
| SRAM 基址/容量 | MEM_SRAM_BASE=0x20000000，MEM_SRAM_SIZE=0x20000（memory_map.h:126-127） | 打包脚本 SRAM_BASE=0x20000000 / SRAM_SIZE=0x20000（pack_framed_bin.py:56-57） | 一致 |
| APP Slot A 基址/容量 | MEM_APP_A_BASE=0x08010000，MEM_APP_SLOT_SIZE=0x2E000 | 脚本 `ROUTES[FLASH_TYPE_APP]` 同值；.sct ER_IROM1 基址 0x08010200 = 槽基址 + 0x200 | 一致 |
| APP 链接基址 | 帧头约定 store_addr + 0x200 | .sct LR_IROM1/ER_IROM1 = 0x08010200 | 一致 |
| RW 区起点 | MEM_APP_RAM_BASE=0x2000D500（memory_map.h:163），且要求 .sct RW_IRAM1 必须从 0x2000D500 起（memory_map.h:122-123） | .sct RW_IRAM1 = 0x2000D500 | 一致 |
| RW 区容量上限 | ≤ 0x12B00（memory_map.h:122-123） | .sct RW_IRAM1 = 0x12B00，恰好等于上限（含 .data/.bss + 8KB 栈） | 一致（取满） |
| 栈尺寸 | MEM_STACK_SIZE=0x2000（memory_map.h:167） | 启动文件 Stack_Size=0x2000 | 一致 |
| 堆尺寸 | 要求 Heap_Size=0（memory_map.h:124） | 启动文件 Heap_Size=0 | 一致 |

结论：打包脚本、.sct 链接脚本、启动文件三方的地址与容量参数和 `memory_map.h` 的声明值全部一致；其中 RW 区按声明上限 0x12B00 取满，意味着 `.data + .bss + 栈` 合计不得超过 74.75KB，超出将侵占固定区 [0]-[7] 或导致链接失败。


## 中断向量表与 VTOR 重定位

APP 链接基址为 **0x08010200**（Slot A 基址 `0x08010000` + 512B 帧头区，即 payload 起点，见 `flashresource_impl.h` 的 `FRAME_PAYLOAD_OFF`）。`.sct` 中 `*.o (RESET, +First)` 将中断向量表放在 ER_IROM1 首址 0x08010200，而非 Cortex-M33 默认的 0x08000000，因此运行时必须重定位向量表：

- `driver/mcu/system/system_gd32e511_512.c:41` 定义 `VECT_TAB_OFFSET = 0x00010200`（注释：APP_A payload base - flash base）
- 依启动路径调用 `nvic_vector_table_set(NVIC_VECTTAB_FLASH, VECT_TAB_OFFSET)`（同文件 :211；RAM 调试路径 :209 用 `NVIC_VECTTAB_RAM`）
- 正常升级启动由 **Bootloader 在跳转前**把 `SCB->VTOR` 置为 0x08010200（Slot A payload 基址，见下文 BL 专节）；APP 自身 `VECT_TAB_OFFSET=0x00010200` 的 FLASH 路径只覆盖单机/调试场景，跳转前 BL 的 VTOR 写入与 APP SystemInit 的重置同值，双保险（`system_gd32e511_512.c:35-41`）
- **BL_BUILD 守卫**（2026-10-14）：`VECT_TAB_OFFSET` 宏定义包 `#ifndef`，BL 工程经 Define 传 `VECT_TAB_OFFSET=0`（BL 向量表在 0x08000000，无需偏移），APP 工程不定义时保持原值——守卫对 APP 编译零影响（`system_gd32e511_512.c:40-44`）

## Bootloader（救援模式，2026-10-14 落地）

Bootloader 从纸面分区（P0-P3，32KB）落地为独立 Keil 工程 `projects/bootloader/400G_SR4_OSFP_BL.uvprojx`（由 `tools/make_bl_uvprojx.py` 从 APP 工程克隆生成），镜像产物 `bin/400G_SR4_OSFP_BL.bin`（扁平 bin）。BL 职责只有两件：**启动决策（提升 B→A + 跳转 APP）**与**救援模式（I2C1 从机 0xA0 + CDB 9Fh 固件下载）**。救援态 CDB 支持 APP_B（A 逻辑包）、PARA_A 与 PARA_B，拒绝 DSP_FW；不支持 BL 自身升级。

### BL 工程链接布局

与 APP 同内存图、不同链接基址：

| 区 | 基址 | 容量 | 说明 |
|---|---|---|---|
| IROM1（OCR_RVCT4） | 0x08000000 | 0x8000（32KB） | BL 全量镜像，向量表在 0x08000000（VECT_TAB_OFFSET=0） |
| IRAM1（OCR_RVCT9） | 0x2000D500 | 0x12B00 | 与 APP 相同——BL 与 APP 不同时运行，共享无冲突 |
| 影子区 `.ARM.__at_0x20005700` | 0x20005700 | PAGE_TAB_NUM×128 = 5×128 = 640B | BL_BUILD 裁剪后仅 5 页槽位 |

构建产物（`projects/bootloader/build_type_route.log`）：Code=19096 RO-data=1520 RW-data=648 ZI-data=11136，0 Error / 0 Warning，总计远小于 32KB。BL 与 APP 的 axf 均含双加载域（ER_IROM1 + 影子区 `.ARM.__at_0x20005700` ZI），`fromelf --bin` 会按域拆成目录——两工程 AfterMake 统一用 cmd 链（fromelf 出目录 → `move` 提取 ER_IROM1 → `rmdir`，`&&` 串联、尾部 `|| echo AFTERMAKE-FAILED` 失败标记）得到扁平 bin（2026-10-15 修复：此前 APP 因同名 bin 文件已存在 Q0147E 静默失败、产物停留在 2026-09-28 旧镜像）。★APP 链尾再 `&&` 接打包器（`C:\Users\86177\.tools\py313\python.exe <repo>\tools\pack_framed_bin.py`，绝对 python + 绝对脚本路径、cwd 无关），每次链接自动刷新 `bin/400G_SR4_OSFP_framed.bin` + `tools/400G_SR4_OSFP_FRAMEHEAD.json`；BL 不打包（扁平 bin 无帧头）。★失败语义（2026-10-15 故障注入实测）：链失败即停（`&&`），`bin/app_tmp` 残留 + 构建日志 AFTERMAKE-FAILED 标记 + bin mtime 不刷新三重信号；★nStopA1X 退出码硬失败不可用——UV4 5.39 实测非零值无条件报 1 Error + Target not created 并删 axf（N=1/exit0、N=1/exit1、N=2/exit1 三探针全报错），故 nStopA1X 保持 0。★取证结论：ER_IROM1 文件 = **完整 LR_IROM1 加载域**（exec 区 + RW 压缩 .data 镜像），大小与 map 的 `LR_IROM1 COMPRESSED[n]` 注解精确相等（APP：53744 = 0xD1F0 = 0xCF78 + 0x278），bin 不缺 .data 初值；第二域影子文件（UNINIT）被 rmdir 丢弃，不属 flash 镜像。

### 启动流程（main.c + bl_promote.c）

```
复位（向量表 0x08000000，SystemInit 120MHz）
  → BlBootDecide()：
       读 METADATA_A OtaInfo_t（magic "TBCOTAUP" + ver + FlashCrc32 整 0x00..0x37 校验）
       ├─ state==CONFIRMED：VerifySlotImage(B) → 提升 B→A → 校验 A → 重写记录为 IDLE
       │    （保留 update_cnt）→ VerifySlotImage(A) 通过 → BL_BOOT_JUMP_APP
       └─ 否则（IDLE/记录损坏/槽 B 无帧头）：直接校验槽 A
  ├─ 槽 A 合法 → BlJumpToApp()：__disable_irq → SysTick 停 → SCB->VTOR=0x08010200
  │    → 切栈+清 PRIMASK+跳复位向量经【全寄存器内联汇编】一次完成（切栈后
  │    C 栈失效，跳转目标必须已装载在寄存器——曾实测编译器在切栈后从
  │    [sp,#0] 重载局部变量导致跳 0 地址 BL 自我重启；PRIMASK 亦必须清——
  │    函数指针跳转非硬件复位不会自动清零，APP 启动序列无重开点）（不返回）
  └─ 槽 A 无效 → 救援环：
       McuClockInit(80MHz，Timer2 分频硬编码 80M) → SystemCoreClockUpdate
       → BlShadowClear（影子 640B 清零，必须在 I2C 使能前）
       → BlIdentityLoad（00h:0-2 标识字节：Flash 主参数区 +512 → 影子，host 枚举
          读到真实身份而非恒 0；须在 I2C 使能前）
       → Timer2SchedulerInit(80000000U,1000U) → DtTabCtrlInit → CdbInit
       → I2cSlaveInit(0xA0)
       for(;;) { Timer2SchedulerRun(); CdbExeBackground(); }
```

掉电安全设计（崩溃安全提升，全部窗口收敛到「可启动 A」或「救援环」或「幂等重试」）：

1. **负载先行、帧头最后**：提升先拷 payload（按 fwsize 分 512B 片、8 字节对齐）、最后拷 512B 帧头区——帧头是提交标记，中途掉电槽 A 无帧头 → 校验必拒。
2. **记录后清、幂等重试**：CONFIRMED 记录在提升成功后、跳转前才重写为 IDLE；任何时刻掉电，下次上电记录仍 CONFIRMED → 重跑提升（B 完好则幂等无害）。清除时整页擦除 + 64B 重写之间掉电 → 记录损坏 → 走「校验槽 A」路径，A 合法则照常启动。
3. **记录↔镜像绑定**：提升前比对记录的 `fwsize/crc32data/crc32hdr` 与 B 现帧头，任一项不符（host 换入新镜像未 Run / 镜像损坏）视为陈旧记录 → 清回 IDLE（保留 update_cnt）、跳过提升、回落槽 A 校验——未经 Run 确认的暂存镜像不会被残留记录静默激活（CMIS Run 激活门保持）。
4. **无回滚状态机**：B 损坏但 A 完好 → 跳过提升、直接启动旧 A；A 损坏但 B 完好且无 CONFIRMED 记录 → 不擅自提升，进救援环等 host 重灌。
5. **跳转不复位**：BL 不调用 NVIC_SystemReset 跳转，直接重跑 APP Reset_Handler（SystemInit/__main 全套），RAM 复用无残留问题；`__disable_irq` 只保护 VTOR/MSP 切换窗口，切栈+清 PRIMASK+跳转经全寄存器内联汇编一次完成（对抗性审查曾抓出两代 critical：PRIMASK 不清零致 APP 全中断屏蔽；切栈后经 [sp,#0] 重载跳转目标致跳 0 地址自我重启）。
6. **BL 不启动 FWDGT**（启动后不可关会带进 APP 窗口期）；擦除期 `EraseFlashWithFeed` 逐页调 `WatchdogKick` 在未启动看门狗上是安全 no-op（`driver/mcu/adpat/src/watchdog.c`）。

### BL 裁剪页表（bl_ral_tab.c，BL_BUILD 枚举）

`protocol/ral/inc/ral.h` 的 `TabIndex_t` 在 `BL_BUILD` 下裁剪为 5 槽：`PAGE_ID_LOW=0`、`PAGE_ID_9FH_0..3=1..4`、`PAGE_TAB_NUM=5`（APP 分支 214 槽原样保留）。`projects/bootloader/src/bl_ral_tab.c`：

- `g_OsfpInfPageLow` 9 行：0x00/0x01/0x02 MEMT_FLASH、0x03、0x08 COR、0x1A ModuleControl、0x25、0x27（BL 中改 MEMT_SRAM）、0x7E BankPageSelect——其余逐字复制 `protocol/ral/src/ral.c`；
- `g_OsfpInfPage9FH` 7 行逐字复制（CDB 消息页）；
- `g_OsfpTabForm` 5 行：[0]=Low 非 banked，[1..4]=9Fh tab 升序 banked=1；编译期负长数组断言行数 == PAGE_TAB_NUM。

引擎四文件（`data_transfer.c`/`adpat.c`/`i2c_slave.c`/`cdb.c`）+ flash/sram 资源层**零修改编译**。`projects/bootloader/src/bl_ral_operate.c` 仅含 3 个 oprfunc（ModuleControl/BankPageSelect/9fCmdId，函数体逐字来自 `ral_operate.c`）与 `g_Pwd*` 三全局（cdb.c Query 读 `g_PwdUnlocked`——BL 恒 locked，无密码 Entry/Change 路径）。CDB 下载流程（Start→Write→Complete→Run→复位）在裁剪表下运行；仅 APP_B 在复位后由 BL 执行 B→A 提升，PARA_A/PARA_B 只复位，DSP_FW 在 BL 入口即拒绝。Run 的延时复位任务挂在 Timer2（BL 救援环 1ms tick 驱动）。

### 工程约定

- 组裁剪：common/main/bootloader(3)/protocols(4 引擎)/resource(2)/resourceimpl(3)/startup/mcu(24)/system/adapt(mcu_clock+timer2_scheduler+watchdog)；删除 odsp/projectfiles/drv/tia/pmic/mal/msl 及 mcu_i2c/mcu_spi。
- Define：`USE_STDPERIPH_DRIVER,GD32E511_512,VECT_TAB_OFFSET=0,BL_BUILD`；工程迁入 `projects/` 后，仓库根相对路径使用 `..\..\` 前缀，BL 自引用保留 `..\bootloader\inc`。
- 再生成：改 `tools/make_bl_uvprojx.py` 后重跑即可（幂等）；BL 源文件在 `projects/bootloader/src`、`projects/bootloader/inc`。

## 维护说明

- **权威来源**：本文档全部事实取自源码，主要来源：`resource_impl/memory_map.h`（布局单一事实源）、`resource/flashresource.h` + `resource_impl/flashresource_impl.*`（Flash 资源与 framed 镜像）、`protocol/cdb/*`（逻辑路由与下载状态机）、`driver/extflash/*`（U15 读写擦）、`resource/sramresource.h` + `resource_impl/sramresource_impl.*`（SRAM 十区）、`protocol/adpat/*`（management map 影子）、`projects/application/Objects/400G_SR4_OSFP.sct`（分散加载）、`driver/mcu/system/system_gd32e511_512.c`（VTOR）、`projects/bootloader/`（BL 工程与救援模式，见 BL 专节）、`tools/pack_framed_bin.py` + `tools/make_full_hex.py`（打包与全片镜像）。
- **改动联动**：修改 `memory_map.h` 布局后，必须同步更新本文档与 [2] 区使用方（BL）；新增 banked 页时同步"Protocol MSA space"节的槽位表；改动 `ral.h` 页表枚举时同步 `projects/bootloader/src/bl_ral_tab.c` 的 BL 裁剪表与本文档 BL 专节；BL 工程重生成只改 `tools/make_bl_uvprojx.py`。
- **布局一致性**：`memory_map.h` 末尾的 `#if` 检查链会在编译期强制固定区 [0]-[7] 首尾相接，本文档地址图与之一致。

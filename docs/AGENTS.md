# 工程编码规范与资源架构

本文以 `resource_impl/chipresource_impl.c` 和 `resource_impl/chipresource_impl.h` 为编码格式基准，结合各资源模块的实际接口约定说明分层架构。新增或修改资源模块时遵循以下约定；第三方库保留自身风格。基准文件中个别缩进、指针空格和行尾空白不一致之处，按下文统一，不作为必须复制的格式。

## 目录结构与职责

- `common/`：公共类型、状态码和位操作宏（当前为 `basedef.h`）。
- `resource/`：资源对外接口，负责资源查询、参数校验和操作转发，不保存硬件配置表。
- `resource_impl/`：板级资源实现，保存静态硬件配置表和运行状态，实现资源查询及硬件回调。
- `driver/`：MCU 外设库及其他底层器件驱动，供板级实现调用。
- 每类硬件资源使用同名文件对：`<name>resource.h/.c` 与
  `<name>resource_impl.h/.c`。

## 分层调用关系

业务代码 → `WriteGpio` / `ReadGpio` / `InitGpio` → `GetGpioInfo` → `GpioOps_t` 对应回调 → `BoardGpio*` → MCU 外设库。

Chip 读写路径：业务代码 → `WriteChip` / `ReadChip` → `GetChipInfoById` → `ChipOps_t` 对应回调 → 器件驱动；总线初始化通过 `InitBus` 转发到总线资源层。

- `<name>resource.h` 声明业务接口及调用契约，并包含对应实现头文件。
- `<name>resource.c` 保持轻量，通过操作表调用硬件实现，不直接操作寄存器或板级引脚。
- `<name>resource_impl.h` 定义资源 ID、Form、Ops、Info 类型及查询函数，使用工程公共类型描述接口。
- `<name>resource_impl.c` 按需包含总线资源、器件驱动或 MCU 头文件，集中保存板级差异、静态配置及硬件实现。
- 业务代码使用资源接口，不直接访问实现层静态表或调用私有回调。

## 编码约定

- 使用 C 语言；公共类型优先使用 `basedef.h` 中的 `BYTE`、`WORD`、`DWORD`、`BOOL`、`VOID`。
- 使用 `STATIC`、`CONST`、`TRUE`、`FALSE` 等工程定义；私有函数和对象使用 `STATIC`，只读配置使用 `STATIC CONST`。
- 文件头采用 Doxygen 风格注释，包含 `@file`、`@brief`、真实作者信息及版权声明；保留已有许可，不虚构作者或修改版权归属。
- 头文件使用全大写、基于文件名的 include guard（例如 `CHIPRESOURCE_IMPL_H`），依次写 `#ifndef`、`#define`、正文和 `#endif`。
- 结构体、联合体类型使用 PascalCase 并以 `_t` 结尾，例如 `BusForm_t`、`ChipForm_t`、`ChipOps_t`、`ChipInfo_t`。资源 ID 枚举沿用所属模块命名，例如 `CHIP_ID_ENUM`；枚举常量和宏使用全大写下划线命名，不为统一格式重命名已有接口。
- 函数使用 PascalCase；资源操作通过函数指针表（`*Ops_t`）注入，失败返回 `ERR`，成功返回 `OK`。
- 文件内静态对象使用 `g_` 前缀，例如 `g_ChipInfoImpl`；参数、成员和局部变量使用简洁小写名称，例如 `chipid`、`busid`、`addr`、`data`、`result`，既有模块的下划线字段命名保持一致。
- `.c` 首先包含自身头文件，再包含所需资源层和驱动头文件；例如依次包含 `chipresource_impl.h`、`busresource.h`、`bcm_arm_platform.h`。随后组织私有函数声明、静态配置表、查询函数、初始化辅助函数和读写回调；有可变状态时在静态数据区单独声明。
- `.h` 在 include guard 内先包含 `basedef.h`，再按依赖顺序定义 Addr（如需）、Form、Ops、Info 类型，随后放置资源 ID 枚举、相关宏和查询函数声明。不同逻辑段之间留一个空行。
- 缩进使用 4 个空格；函数体左花括号另起一行，控制语句按实现文件风格写作 `if(condition) {`、`switch(value) {`。
- 保护条件采用多行形式，`return ERR;` 独占一行；简单查询或状态转换可用三目运算符。一般每行一条语句，赋值和返回分开写。
- 指针统一写作 `CONST ChipInfo_t *info`、`DWORD *data`，返回指针的函数写作 `CONST ChipInfo_t *GetChipInfo(VOID)`。二元运算符两侧及逗号后留空格，函数名与 `(` 之间不留空格；无参数函数使用 `VOID`。
- 结构体、联合体和枚举的左花括号跟随声明，例如 `typedef struct {`；结束形式沿用 `}ChipForm_t;`。成员使用 4 个空格缩进，可在同一声明块内按列对齐，不混入 Tab，不保留行尾空白。
- 配置表按资源逐行列出，按类型声明顺序使用嵌套花括号初始化 Form 和 Ops；联合体成员使用指定初始化，例如 `{.i2caddr = 0xA0}`（若该资源定义了联合体）。避免为对齐而重排无关代码。
- 注释采用简洁的 `/* ... */`，重点解释硬件约束、操作顺序和调用契约。保留但未使用的参数用 `(void)rsttype;` 显式标记。

## 延时函数

- 工程内所有延时统一使用 `driver/mcu/adpat/src/mcu_clock.c` 中的延时函数：`McuClockDelayUs`、`McuClockDelayMs`、`McuClockDelayS`（声明见 `driver/mcu/adpat/inc/mcu_clock.h`）。
- 不新建其他延时实现，不调用 `hr_time_arm.c`、SDK 示例或内联循环等替代延时；第三方库保留自身实现，但新增和业务代码一律使用上述接口。

## 任务调度

- 任务 ID 统一在 `driver/mcu/adpat/inc/timer2_scheduler.h` 中定义（`TASK_ID_*`），各模块 include 该头文件使用，不在模块自己的头文件中重复定义任务 ID。
- 任务通过 `Timer2SchedulerAdd(function, arg, delay_ticks, interval_ticks, run_count)` 注册：delay_ticks 为首次执行延时，interval_ticks 为执行间隔，run_count 为执行次数（`TIMER2_TASK_FOREVER` 表示无限次）；保存返回的 `Timer2TaskId_t` 句柄。
- 已创建的任务用 `Timer2SchedulerModify` 修改函数、参数、延时、间隔和次数；周期性任务在主循环中调用 `Timer2SchedulerRun()` 执行。

## 资源数据模型

- 资源 ID 枚举：命名沿用模块接口；`<NAME>_ID_NUM` 是 ID 上界，不一定等于配置表长度。Chip 使用 `CHIP_ID_ENUM`，表内有效 ID 从 `CHIP_ID_START` 开始。
- `<Name>Addr_t`：按需使用联合体描述不同寻址方式（若该资源需要多种寻址方式）。
- `<Name>Form_t`：资源 ID 和板级配置参数，例如 `ChipForm_t` 的 `chipid`、`addr` 和 `busid`。
- `<Name>Ops_t`：操作函数指针表，回调签名与声明一致。
- `<Name>Info_t`：组合配置和操作表，例如 `ChipForm_t chipform` 与 `ChipOps_t chipops`。
- 查询接口沿用模块签名：Chip 的 `GetChipInfo(VOID)` 返回表首地址，`GetChipInfoById(BYTE chipid)` 按 ID 查询，非法 ID 返回 `NULL`；不要将两者统一为同一个带参接口。

硬件实例集中在实现 `.c` 的 `STATIC CONST` 配置表中，每项包含配置和操作表。Chip 的格式示例：

```c
STATIC CONST ChipInfo_t g_ChipInfoImpl[CHIP_ID_NUM - CHIP_ID_START] = {
    {{CHIP_ID_DSP, BUS_ID_DSP}, {DspWrite, DspRead, InitDsp}},
    {{CHIP_ID_DRV, BUS_ID_DRV}, {DrvWrite, DrvRead, InitDrv}},
    {{CHIP_ID_TIA, BUS_ID_TIA}, {TiaWrite, TiaRead, InitTia}},
    {{CHIP_ID_PMIC, BUS_ID_PMIC}, {PmicWrite, PmicRead, InitPmic}},
    {{CHIP_ID_EXTFLASH, BUS_ID_EXTFLASH}, {ExtFlashWrite, ExtFlashRead, InitExtFlash}}
};

CONST ChipInfo_t *GetChipInfoById(BYTE id)
{
    return id >= CHIP_ID_START && id < CHIP_ID_NUM ? &g_ChipInfoImpl[id - CHIP_ID_START] : NULL;
}
```

Chip 查询必须先保证 `CHIP_ID_START <= id < CHIP_ID_NUM`，再使用 `id - CHIP_ID_START` 索引；`CHIP_ID_MCU` 和 `CHIP_ID_DUMMY` 不属于该表。GPIO 等从零开始的表按各自实现直接索引。必须保证有效 ID 顺序、表项顺序和配置中的 ID 一致；增删资源时同步维护表长度及相关状态数组。

不支持的操作显式初始化为 `NULL`，例如输入 GPIO 的写回调。静态配置无需运行时注册或动态分配；可变状态单独保存，例如 `STATIC BOOL g_Ready[GPIO_ID_NUM]`。不要增加与现有表重复的默认表或分发层。

## 校验与返回值

- 对外接口先查询资源，再检查信息指针、操作指针及参数，验证通过后直接返回回调结果。
- 查询函数负责 ID 范围检查；硬件回调按实际签名检查 ID、指针和硬件操作限制。有就绪状态的模块还须检查初始化状态，不假设所有回调都接收信息指针。
- 短路条件先检查 `info == NULL`，再访问成员或用 ID 索引状态数组，防止越界和空指针解引用。
- 状态型操作返回 `DWORD`，成功为 `OK`，失败为 `ERR`；读取结果通过输出指针返回，仅在返回 `OK` 时有效。查询函数按指针接口返回配置或 `NULL`。
- 输出指针必须判空，失败条件尽早返回；硬件初始化成功后再设置就绪状态。
- 读写前必须初始化。GPIO 写入仅接受 `IO_LOW` / `IO_HIGH`，输入资源不可写；对外接口和硬件回调分别维护必要校验。
- `rsttype` 当前为保留参数，不赋予尚未实现的复位语义。

## GPIO 硬件语义

- 每个资源对应一个端口和一个 `GPIO_PIN_x` 位。
- `EnablePort` 集中使能端口时钟，配置引脚前先开启对应时钟。
- 输出初始化先预装 `default_level`，再配置开漏或推挽及输出模式，避免输出切换毛刺。
- 当前实现使用 `GPIO_OSPEED_2MHZ` 和 `GPIO_PUPD_NONE`；输入默认电平描述外部电路，不据此推导内部上下拉。
- 读写接口传递原始电平，不根据 `active_level` 自动反相。读取使用 `gpio_input_bit_get`，包括输出引脚的实际输入电平。
- 修改上下拉、有效电平或初始化顺序时，应依据实际硬件设计。

## Chip 硬件语义

- `ChipWrite` / `ChipRead` 接收芯片 ID、寄存器地址及数据或输出指针；`ChipInit` 仅接收 `rsttype`，回调声明、定义和操作表签名必须一致。
- DSP 读写通过 `BcmArmWrite` / `BcmArmRead` 完成，将驱动返回的 `0` 转为 `OK`，其他值转为 `ERR`；读取成功后再把临时结果写入输出指针。
- DRV、TIA 当前仅配置总线初始化，读写回调保持 `NULL`。已知 I2C 地址不足以推断寄存器协议，不默认使用 8 位寄存器；实现读写前须确认器件协议，并按需使用 `WriteBus` / `ReadBus`。
- U15（SST25PF040C，512KB 外部 SPI NOR Flash，DSP 启动 Flash）挂在 BUS_ID_EXTFLASH（硬件 SPI2：SCK=PG9/MOSI=PG11/MISO=PG10，CS=PG12=GPIO_ID_EXTFLASH_CS，低有效）。**该总线 SCK/MOSI/MISO 与 CS 网络均与 DSP（U11）SPI 主控共享（DSP 从 U15 自举微码）**——MCU 侧任何引脚都不得常占：每次访问由 driver/extflash 现取现放（Acquire：CS 网络→输出高、SPI2→AF 主控；Release：全部回 Hi-Z 输入；CS 在 GPIO 资源表保持输入默认）。调用前提：DSP 复位保持中，或启动完成后其 SPI 主控空闲（实机验证）；DSP 微码升级流程 = 复位 DSP → 编程 U15 → 释放复位让 DSP 自举新镜像。器件特性（Microchip DS20005397F）：**WP#/HOLD# 外接 3.3V（2026-10-16 用户定案）——状态寄存器常可写（BPL 锁不生效）、HOLD# 常失效**；BP0-2/TB/BPL 为非易失位，上电保护等级=上次编程值（不定），init 时 RDSR 读回 BP/TB 非零才 WREN+WRSR 00h 清一次；WRSR 需 WREN 前置且一次总线周期只写 1 字节状态（跨周期不识别）；命令集 WREN 06h/WRDI 04h/RDSR 05h/WRSR 01h/READ 03h（上限 25 MHz）/PP 02h/SE 20h(4KB，D7h 为别名)/BE D8h(64KB，唯一块擦除——52h 本料号不存在)/CE 60h(C7h 别名)/RDID 9Fh（**62 06 13 00 循环输出**，非 BF 25 8D；90h Read-ID 本料号也不存在）；PP 上限 256B 且页内回卷、编程不得跨 256B 页；擦除时序 4KB typ 40/max 150ms、64KB typ 80/max 250ms、整片 typ 250ms/max 2s——**无擦除挂起**，整片擦除最长占线 2s，升级流程的喂狗预算须覆盖。上电 TPU≥100µs 后才允许首条指令（本链路秒级后运行，天然满足）。NOR 命令+地址+数据必须同一次 CS 会话，`WriteBus`/`ReadBus` 的逐次 CS 包裹不满足时序，故 extflash 驱动绕过总线层、经 `mcu_spi` 直发并自驱 CS 网络（不经 GPIO 资源层写，因为该脚在资源表是输入）。

## 新增资源

1. 在 `resource/` 与 `resource_impl/` 创建或维护对应的四个文件。
2. 在实现头文件定义 ID、Form、Ops、Info 和查询接口，在公共头文件声明业务接口及调用契约。
3. 在实现源文件配置静态资源表、私有状态和硬件回调，在资源源文件实现校验及转发。
4. 核对 ID 与表项对应关系、操作指针和回调签名，并按现有构建方式加入源文件及包含路径。
5. 验证非法 ID、空指针、不支持的操作、初始化前访问、非法参数和正常读写路径；GPIO 还需检查默认电平和初始化顺序。
6. 执行可用的编译或针对性检查，记录实际结果；未进行板级验证时明确说明。

只修改任务涉及的模块。格式以 `chipresource_impl.c`、`chipresource_impl.h` 及本文明确的统一规则为准；接口职责、索引方式和硬件语义以所属模块实际实现为准，不因格式调整改变行为。

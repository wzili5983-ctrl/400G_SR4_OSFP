#!/usr/bin/env python3
"""VDM 页定稿：PAGE01H bit6 宣告 + PAGE20H 描述符 + PAGE28H 阈值（v5 方案）。

静态内容全部走 Flash 参数区（xlsx 默认值 → paratab 上位机 → bin →
Flash @0x0806C000 → MgiParamLoad memcpy → SRAM 影子），固件运行时不再
初始化这些页（VdmInitDescriptors/VdmInitThresholds/s_VdmThreshold 已删）。

数据来源：protocol/mal/src/vdm.c 的 g_VdmInsInfo（实例序/分类）与本表
THRESHOLDS（推荐阈值，原 s_VdmThreshold）。
"""
import openpyxl
from openpyxl.styles import Font, PatternFill, Alignment

XLSX = r"C:\Users\86177\Desktop\project\400G_SR4_OSFP\docs\400G_SR4_OSFP软硬件接口文档.xlsx"

# ---- 32 实例（名称, obs=VDM_OBS_*, res=VDM_RES_*, thrSet=LocalThresholdSetID） ----
INSTANCES = [
    ("SNR Media Lane1",  5,  0, 0),
    ("SNR Media Lane2",  5,  1, 0),
    ("SNR Media Lane3",  5,  2, 0),
    ("SNR Media Lane4",  5,  3, 0),
    ("SNR Host Lane1",   6,  0, 0),
    ("SNR Host Lane2",   6,  1, 0),
    ("SNR Host Lane3",   6,  2, 0),
    ("SNR Host Lane4",   6,  3, 0),
    ("LTP Media Lane1",  7,  0, 1),
    ("LTP Media Lane2",  7,  1, 1),
    ("LTP Media Lane3",  7,  2, 1),
    ("LTP Media Lane4",  7,  3, 1),
    ("LTP Host Lane1",   8,  0, 1),
    ("LTP Host Lane2",   8,  1, 1),
    ("LTP Host Lane3",   8,  2, 1),
    ("LTP Host Lane4",   8,  3, 1),
    ("BER Cur Media",   15, 15, 2),
    ("BER Avg Media",   13, 15, 2),
    ("BER Min Media",   11, 15, 2),
    ("BER Max Media",   12, 15, 2),
    ("BER Cur Host",    16, 15, 2),
    ("BER Avg Host",    14, 15, 2),
    ("BER Min Host",     9, 15, 2),
    ("BER Max Host",    10, 15, 2),
    ("FERC Cur Media",  23, 15, 3),
    ("FERC Avg Media",  19, 15, 3),
    ("FERC Min Media",  17, 15, 3),
    ("FERC Max Media",  18, 15, 3),
    ("FERC Cur Host",   24, 15, 3),
    ("FERC Avg Host",   22, 15, 3),
    ("FERC TOT Media",  25, 15, 3),
    ("FERC TOT Host",   26, 15, 3),
]

# ---- 4 组阈值推荐值（原 s_VdmThreshold[set][HA,LA,HW,LW]） ----
THRESHOLDS = [
    # (set 名, 量纲说明, HA, LA, HW, LW)
    ("Set0 SNR",  "U16 1/256dB", 0xFFFF, 0x0C00, 0xFFFF, 0x0E00),
    ("Set1 LTP",  "U16 1/256dB", 0xFFFF, 0x0A00, 0xFFFF, 0x0C00),
    ("Set2 BER",  "F16",         0x3800, 0x0000, 0x3400, 0x0000),
    ("Set3 FERC", "U16",         0x0064, 0x0000, 0x000A, 0x0000),
]

RES_NAMES = {0: "LANE1", 1: "LANE2", 2: "LANE3", 3: "LANE4", 15: "MODULE"}
OBS_NAMES = {
    5: "SNR_MEDIA", 6: "SNR_HOST", 7: "LTP_MEDIA", 8: "LTP_HOST",
    9: "BER_MIN_HOST", 10: "BER_MAX_HOST", 11: "BER_MIN_MEDIA", 12: "BER_MAX_MEDIA",
    13: "BER_AVG_MEDIA", 14: "BER_AVG_HOST", 15: "BER_CUR_MEDIA", 16: "BER_CUR_HOST",
    17: "FERC_MIN_MEDIA", 18: "FERC_MAX_MEDIA", 19: "FERC_AVG_MEDIA",
    20: "FERC_MIN_HOST", 21: "FERC_MAX_HOST", 22: "FERC_AVG_HOST",
    23: "FERC_CUR_MEDIA", 24: "FERC_CUR_HOST",
    25: "FERC_TOT_MEDIA", 26: "FERC_TOT_HOST",
}

# 实例分类（与 vdm.c g_VdmInsInfo 对照；Freeze 门禁/telemetry 侧选择）
STATISTIC = {17, 18, 19, 21, 22, 23, 25, 26, 27, 29, 30, 31}
HOST = {4, 5, 6, 7, 12, 13, 14, 15, 20, 21, 22, 23, 28, 29, 31}

# 样式
FONT_TITLE = Font(bold=True, size=12)
FONT_SECTION = Font(bold=True, size=12)
FONT_HDR = Font(bold=True, size=11)
FILL_HDR = PatternFill(start_color="FFDDEBF7", end_color="FFDDEBF7", fill_type="solid")
FONT_DATA = Font(size=11)
ALIGN_WRAP = Alignment(wrap_text=True, vertical="top")

COLS = ["序号", "宿主地址(HEX)", "页内偏移(HEX)", "字节数", "字段名称",
        "访问属性", "存储类型", "项目设定值", "说明（本项目取值）"]


def style_row(ws, row, values, font=None, fill=None):
    for i, v in enumerate(values, 1):
        c = ws.cell(row=row, column=i, value=v)
        if font:
            c.font = font
        if fill:
            c.fill = fill
        c.alignment = ALIGN_WRAP


def set_widths(ws):
    for col, w in {"A": 6, "B": 15, "C": 13, "D": 8, "E": 30,
                   "F": 10, "G": 13, "H": 42, "I": 60}.items():
        ws.column_dimensions[col].width = w


def meta_block(ws, r, rows):
    for label, val in rows:
        style_row(ws, r, [label, val], FONT_DATA)
        r += 1
    return r


def patch_page01h(wb):
    """Page 01h byte 0x0E bit6 = VDM supported（Flash 参数区声明，固件不再置位）。"""
    ws = wb["PAGE01H"]
    # 行 16 = 序号 4（byte 0x0E Supported Pages）
    assert ws.cell(row=16, column=3).value == "0x0E", ws.cell(row=16, column=3).value
    ws.cell(row=16, column=8, value="0x40").font = FONT_DATA
    ws.cell(row=16, column=8).alignment = ALIGN_WRAP
    ws.cell(row=16, column=9, value=(
        "BanksSupported=00(1 Bank=8 lane,SR4 4-lane 模块只用 lanes1-4);"
        "VDM=1 支持(0x0E bit6=1,Pages 20h-2Fh Group1,描述符/阈值见 PAGE20H/PAGE28H);"
        "Diag/Coherent/FF/03h 均不支持。"
        "★本字节由 Flash 参数区声明(固件 VdmInit 不再运行时置位);"
        "142 在 Byte 255 PageChecksum=sum(130..254) 区间内,改值须同步重算校验和"
    )).font = FONT_DATA
    ws.cell(row=16, column=9).alignment = ALIGN_WRAP

    # 行 38 = 备注④
    assert str(ws.cell(row=38, column=1).value).startswith("④"), ws.cell(row=38, column=1).value
    ws.cell(row=38, column=1, value=(
        "④ CDB/Diag/Coherent/NAD/HostLaneSwitching/LinkTraining/ScratchPad 宣告为不支持;"
        "VDM 宣告支持(0x0E bit6=1,Group1 32 实例);"
        "PasswordEntry/Result 与 FullPageRead 宣告支持。"
    )).font = FONT_DATA
    ws.cell(row=38, column=1).alignment = ALIGN_WRAP


def build_page20h(wb):
    if "PAGE20H" in wb.sheetnames:
        del wb["PAGE20H"]
    idx = wb.sheetnames.index("PAGE11H_0") + 1
    ws = wb.create_sheet("PAGE20H", idx)
    set_widths(ws)

    r = 1
    style_row(ws, r, ["PAGE20H —— VDM Instance Descriptor（CMIS 5.3 §8.22）"], FONT_TITLE)
    r += 1
    style_row(ws, r, ["（Page 20h 描述符 32 实例;静态内容走 Flash 参数区,固件不初始化本页）"])
    r += 1

    r = meta_block(ws, r, [
        ("页大小 / 宿主窗口",
         "128B(Upper Memory 窗口 0x80-0xFF;Page 20h 描述符占 0x00-0x3F = 32 实例 × 2B,0x40-0x7F Reserved)"),
        ("枚举项 / 槽位号", "PAGE_ID_20H_0 = 73(ral.h;banked 页 Bank0 槽)"),
        ("SRAM 影子地址", "0x20007B80 - 0x20007BFF(槽 73 = 0x20005700 + 73×128)"),
        ("Flash 主参数区", "偏移 0x02680(绝对 0x0806E680)= 0x200 + 73×128;MgiParamLoad 整区 memcpy 落影子"),
        ("属性总述",
         "全部字段静态只读(ACC_RO / MEMT_FLASH,无 RW/WO/COR);内容来自参数区镜像,固件运行时不写本页。"
         "ral.c 页表 [73] 已按「空表占位行」登记(num=0 → host 写丢弃、读影子镜像)"),
        ("布局",
         "32 实例 × 2B:byte 2n = [7:4]LocalThresholdSetID | [3:0]MonitoredResource, byte 2n+1 = Observable Type ID(CMIS 5.3 Table 8-170)"),
    ])
    r += 1

    style_row(ws, r, ["Page 20h —— VDM Instance Descriptor（32 实例）"], FONT_SECTION)
    r += 1
    style_row(ws, r, COLS, FONT_HDR, FILL_HDR)
    r += 1

    for i, (name, obs, res, thr_set) in enumerate(INSTANCES):
        off = i * 2
        b0 = (thr_set << 4) | (res & 0x0F)
        b1 = obs
        host = 0x80 + off
        kind = "Statistic(Freeze 保持)" if i in STATISTIC else "Basic(Freeze 不影响)"
        side = "Host" if i in HOST else "Media"
        style_row(ws, r, [
            i,
            f"0x{host:02X}-0x{host+1:02X}",
            f"0x{off:02X}",
            2,
            f"Inst{i} {name} Desc",
            "RO",
            "FLASH",
            f"{b0:02X} {b1:02X}",
            f"ThrSet={thr_set} Res={RES_NAMES.get(res,'?')} Obs={obs}({OBS_NAMES.get(obs,'?')});"
            f"{kind} / telSide={side}",
        ], FONT_DATA)
        r += 1

    return ws


def build_page28h(wb):
    if "PAGE28H" in wb.sheetnames:
        del wb["PAGE28H"]
    idx = wb.sheetnames.index("PAGE20H") + 1
    ws = wb.create_sheet("PAGE28H", idx)
    set_widths(ws)

    r = 1
    style_row(ws, r, ["PAGE28H —— VDM Threshold（CMIS 5.3 §8.22）"], FONT_TITLE)
    r += 1
    style_row(ws, r, ["（Page 28h 阈值 4 组;静态内容走 Flash 参数区,固件不初始化本页）"])
    r += 1

    r = meta_block(ws, r, [
        ("页大小 / 宿主窗口",
         "128B(Upper Memory 窗口 0x80-0xFF;Page 28h 阈值占 0x00-0x1F = 4 组 × 8B,0x20-0x7F Reserved)"),
        ("枚举项 / 槽位号", "PAGE_ID_28H_0 = 105(ral.h;banked 页 Bank0 槽)"),
        ("SRAM 影子地址", "0x20008B80 - 0x20008BFF(槽 105 = 0x20005700 + 105×128)"),
        ("Flash 主参数区", "偏移 0x03680(绝对 0x0806F680)= 0x200 + 105×128;MgiParamLoad 整区 memcpy 落影子"),
        ("属性总述",
         "全部字段静态只读(ACC_RO);阈值来源 = 参数区镜像,vdm.c VdmThresholdCheck 每周期读影子比较。"
         "ral.c 页表 [105] 已按「空表占位行」登记(num=0 → host 写丢弃、读影子镜像)——"
         "CMIS 定义本页 RW,本项目 host 运行时改阈值暂未支持;如需启用须恢复 28h 寄存器表(MEMT_SRAM 纯存储项)"),
        ("布局",
         "4 组 × 8B:组 N 基址 N×8,HA@+0 LA@+2 HW@+4 LW@+6(u16 大端);"
         "门禁约定:high 侧 0xFFFF = 禁用、low 侧 0x0000 = 禁用"),
    ])
    r += 1

    style_row(ws, r, ["Page 28h —— VDM Threshold（4 组,推荐值）"], FONT_SECTION)
    r += 1
    style_row(ws, r, COLS, FONT_HDR, FILL_HDR)
    r += 1

    for i, (name, unit, ha, la, hw, lw) in enumerate(THRESHOLDS):
        off = i * 8
        host = 0x80 + off
        style_row(ws, r, [
            i,
            f"0x{host:02X}-0x{host+7:02X}",
            f"0x{off:02X}",
            8,
            f"{name} Threshold",
            "RO",
            "FLASH",
            f"{ha:04X} {la:04X} {hw:04X} {lw:04X}",
            f"HA=0x{ha:04X} LA=0x{la:04X} HW=0x{hw:04X} LW=0x{lw:04X}"
            f"（{unit};u16 BE;HA@+0 LA@+2 HW@+4 LW@+6）",
        ], FONT_DATA)
        r += 1

    r += 1

    style_row(ws, r, ["阈值细节（每组 4 × u16 BE）"], FONT_SECTION)
    r += 1
    style_row(ws, r, COLS, FONT_HDR, FILL_HDR)
    r += 1

    THR_LABELS = ["HA (High Alarm)", "LA (Low Alarm)", "HW (High Warning)", "LW (Low Warning)"]
    THR_OFFSETS = [0, 2, 4, 6]
    n = 0
    for si, (name, unit, ha, la, hw, lw) in enumerate(THRESHOLDS):
        vals = [ha, la, hw, lw]
        for ti in range(4):
            off = si * 8 + THR_OFFSETS[ti]
            host = 0x80 + off
            style_row(ws, r, [
                n,
                f"0x{host:02X}-0x{host+1:02X}",
                f"0x{off:02X}",
                2,
                f"{name} {THR_LABELS[ti]}",
                "RO",
                "FLASH",
                f"{vals[ti]:04X}",
                f"0x{vals[ti]:04X}（{unit}）"
                + (";0xFFFF = 该侧禁用" if ti in (0, 2) else ";0x0000 = 该侧禁用"),
            ], FONT_DATA)
            r += 1
            n += 1

    return ws


def main():
    wb = openpyxl.load_workbook(XLSX)
    patch_page01h(wb)
    build_page20h(wb)
    build_page28h(wb)

    # 公式自检：openpyxl 把 = 开头字符串存成公式,Excel 打开会静默删格
    for name in ("PAGE01H", "PAGE20H", "PAGE28H"):
        for row in wb[name].iter_rows():
            for c in row:
                if c.data_type == "f":
                    raise SystemExit(f"公式陷阱: {name}!{c.coordinate} 被写成公式!")

    wb.save(XLSX)
    print(f"PAGE01H bit6=VDM 已置 0x40（含备注④）")
    print(f"PAGE20H 描述符 32 条已写入（MEMT_FLASH）")
    print(f"PAGE28H 阈值 4 组 + 细节 16 条已写入（MEMT_FLASH）")


if __name__ == "__main__":
    main()

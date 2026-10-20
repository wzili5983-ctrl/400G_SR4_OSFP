#!/usr/bin/env python
# -*- coding: utf-8 -*-
r"""从 application/400G_SR4_OSFP.uvprojx 克隆生成 bootloader/400G_SR4_OSFP_BL.uvprojx。

BL 工程差异（与 APP 基线对齐，见 docs/memory_map.md BL 段）：
  - IROM1（OCR_RVCT4）：0x08000000 / 0x8000（P0-P3，32KB）；IRAM1 不变（共享 0x2000D500 起）；
  - Define 追加 VECT_TAB_OFFSET=0,BL_BUILD（system_gd32e511_512.c / ral.h 的编译期分支）；
  - IncludePath 与 APP 同层深度（bootloader\ 与 application\ 同级），仅追加 ..\bootloader\inc；
  - 组裁剪：去掉 odsp/projectfiles(仅留 system)/drv/tia/pmic/mal/msl 等，
    引擎文件（data_transfer/adpat/i2c_slave/cdb）+ flash/sram 资源不修改编译；
  - AfterMake fromelf 输出 ..\bin\400G_SR4_OSFP_BL.bin。
"""
import os
import xml.etree.ElementTree as ET

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SRC_PROJ = os.path.join(REPO, "application", "400G_SR4_OSFP.uvprojx")
DST_PROJ = os.path.join(REPO, "bootloader", "400G_SR4_OSFP_BL.uvprojx")

tree = ET.parse(SRC_PROJ)
root = tree.getroot()
proj = root.find("Targets/Target")
tco = proj.find("TargetOption/TargetCommonOption")

# ---- 1. 输出名 + AfterMake fromelf ----
# axf 含多加载域（ER_IROM1 + .ARM.__at_0x20005700 影子区）时 fromelf --bin
# 会按加载域拆分成目录；提取 ER_IROM1 得到扁平 BL 镜像（与 APP 侧同坑，
# APP 的 bin 因同名文件已存在而 Q0147E 报错、产物停留在旧镜像——见 build 陷阱）。
tco.find("OutputName").text = "400G_SR4_OSFP_BL"
after = tco.find("AfterMake/UserProg1Name")
after.text = (r"cmd /c C:\Keil_v5\ARM\ARMCC\bin\fromelf.exe --bin --output "
              r"..\bin\bl_tmp .\Objects\400G_SR4_OSFP_BL.axf & "
              r"move /Y ..\bin\bl_tmp\ER_IROM1 ..\bin\400G_SR4_OSFP_BL.bin & "
              r"rmdir /S /Q ..\bin\bl_tmp")

# ---- 2. Define ----
varc = proj.find("TargetOption/TargetArmAds/Cads/VariousControls")
varc.find("Define").text = "USE_STDPERIPH_DRIVER,GD32E511_512,VECT_TAB_OFFSET=0,BL_BUILD"

# ---- 3. IncludePath：与 APP 同层深度（bootloader\ 与 application\ 同级），
#        路径前缀保持不变，仅追加 bootloader\inc ----
inc = varc.find("IncludePath")
entries = [e for e in inc.text.split(";") if e]
entries.append(r"..\bootloader\inc")
inc.text = ";".join(entries)

# ---- 4. IROM1：0x08000000 / 0x8000 ----
ocm = proj.find("TargetOption/TargetArmAds/ArmAdsMisc/OnChipMemories")
irom1 = ocm.find("OCR_RVCT4")
irom1.find("StartAddress").text = "0x8000000"
irom1.find("Size").text = "0x8000"

# ---- 5. 组重写 ----
def mkfile(name, ftype, path):
    f = ET.Element("File")
    ET.SubElement(f, "FileName").text = name
    ET.SubElement(f, "FileType").text = str(ftype)
    ET.SubElement(f, "FilePath").text = path
    return f

def mkfiles(files, base):
    """base 为普通字符串目录前缀（如 "..\\..\\bootloader\\src\\"），files 为无扩展名列表。"""
    return [mkfile(n + ".c", 1, base + n + ".c") for n in files]

groups = ET.Element("Groups")
def add_group(gname, file_els):
    g = ET.SubElement(groups, "Group")
    ET.SubElement(g, "GroupName").text = gname
    files = ET.SubElement(g, "Files")
    for f in file_els:
        files.append(f)

add_group("common", [mkfile("basedef.h", 5, r"..\common\basedef.h")])
add_group("main", mkfiles(["main"], "..\\bootloader\\src\\"))
add_group("bootloader", mkfiles(["bl_promote", "bl_ral_tab", "bl_ral_operate"],
                                "..\\bootloader\\src\\"))
add_group("protocols", [mkfile("data_transfer.c", 1, r"..\protocol\mci\src\data_transfer.c"),
                        mkfile("adpat.c", 1, r"..\protocol\adpat\src\adpat.c"),
                        mkfile("i2c_slave.c", 1, r"..\protocol\mci\src\i2c_slave.c"),
                        mkfile("cdb.c", 1, r"..\protocol\cdb\src\cdb.c")])
add_group("resource", [mkfile("flashresource.c", 1, r"..\resource\flashresource.c"),
                       mkfile("sramresource.c", 1, r"..\resource\sramresource.c")])
add_group("resourceimpl", [mkfile("flashresource_impl.c", 1, r"..\resource_impl\flashresource_impl.c"),
                           mkfile("sramresource_impl.c", 1, r"..\resource_impl\sramresource_impl.c"),
                           mkfile("memory_map.h", 5, r"..\resource_impl\memory_map.h")])
add_group("startup", [mkfile("startup_gd32e511_512.s", 2,
                             r"..\driver\mcu\startup\startup_gd32e511_512.s")])
add_group("mcu", mkfiles(["gd32e511_512_adc", "gd32e511_512_cla", "gd32e511_512_cmp",
                          "gd32e511_512_crc", "gd32e511_512_dac", "gd32e511_512_dbg",
                          "gd32e511_512_dma", "gd32e511_512_exti", "gd32e511_512_fmc",
                          "gd32e511_512_fwdgt", "gd32e511_512_gpio", "gd32e511_512_i2c",
                          "gd32e511_512_i3c", "gd32e511_512_mdio", "gd32e511_512_misc",
                          "gd32e511_512_opa", "gd32e511_512_pmu", "gd32e511_512_rcu",
                          "gd32e511_512_rtc", "gd32e511_512_spi", "gd32e511_512_syscfg",
                          "gd32e511_512_timer", "gd32e511_512_usart", "gd32e511_512_wwdgt"],
                         "..\\driver\\mcu\\peripheral\\src\\"))
add_group("system", [mkfile("system_gd32e511_512.c", 1,
                            r"..\driver\mcu\system\system_gd32e511_512.c")])
add_group("adapt", [mkfile("mcu_clock.c", 1, r"..\driver\mcu\adpat\src\mcu_clock.c"),
                    mkfile("timer2_scheduler.c", 1, r"..\driver\mcu\adpat\src\timer2_scheduler.c"),
                    mkfile("watchdog.c", 1, r"..\driver\mcu\adpat\src\watchdog.c")])

old_groups = proj.find("Groups")
proj.remove(old_groups)
proj.append(groups)

# Layer 名
layer = root.find("LayerInfo/Layers/Layer/LayName")
if layer is not None:
    layer.text = "400G_SR4_OSFP_BL"

ET.ElementTree(root).write(DST_PROJ, encoding="utf-8", xml_declaration=True)
print("written:", DST_PROJ)

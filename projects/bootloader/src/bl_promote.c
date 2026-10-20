/***************************************************************************/ /**
 *   @file   bl_promote.c
 *   @brief  Bootloader 启动决策实现：提升 B→A + 确认清除 + 跳转 APP。
 *   @author Firmware team
 ********************************************************************************
 * Copyright 2026 Terabitcom, Inc.
 *
 * 流程见 bl_promote.h。安全要点：
 *   - 提升前先 VerifySlotImage(B)（深校验），槽 A 的擦除永远发生在 B 校验
 *     通过之后——B 损坏时旧 A 原样保留，回退启动旧固件；
 *   - 拷贝序：负载先行、512B 帧头区最后——掉电中断的槽 A 无帧头，
 *     VerifySlotImage(A) 必拒，下次启动按残留 CONFIRMED 记录重试（幂等）；
 *   - METADATA_A 重写：整页擦除 + 单条 64B 记录（state=IDLE、update_cnt
 *     保留、crc32 重算）；若本步掉电，记录损坏 → 走槽 A 校验路径
 *     （A 此刻已完整）——两条掉电分支都收敛到安全态；
 *   - 记录↔镜像绑定：CONFIRMED 与 B 的 crc32data/fwsize 比对，不符视为
 *     陈旧记录清回 IDLE——残留记录不激活未经 Run 确认的新镜像；
 *   - 跳转：__disable_irq 保护 VTOR/MSP 切换；切栈+清 PRIMASK+跳转经全
 *     寄存器内联汇编一次完成（切栈后 C 栈失效，不得经栈取跳转目标）。
 ******************************************************************************/
#include "bl_promote.h"
#include "flashresource.h"       /* ReadFlash/WriteFlash/EraseFlash/VerifySlotImage/GetFrameHead/FlashCrc32 */
#include "flashresource_impl.h"  /* OtaInfo_t / OTA_* / FRAME_* */
#include "memory_map.h"          /* MEM_APP_A_BASE / MEM_APP_SLOT_SIZE */
#include "adpat.h"               /* g_AdpatMgmtMapBuf / ADPAT_PROTOCOL_MSA_SIZE */
#include "gd32e511_512.h"        /* SCB / SysTick / __disable_irq / __set_MSP（CMSIS core） */

/* 拷贝 staging 缓冲：512B 帧头区可一次搬完；负载按 512B 分片（8 对齐）。 */
STATIC BYTE s_CopyBuf[FRAME_HDR_AREA_SIZE];

/* OtaInfo_t 合法性：magic + 版本 + crc32（撕裂写守卫，覆盖 0x00..0x37）。 */
STATIC DWORD BlOtaInfoRead(OtaInfo_t *info)
{
    CONST BYTE magic[8] = { 'T', 'B', 'C', 'O', 'T', 'A', 'U', 'P' };
    DWORD crc = 0U;
    DWORD i;

    if(ReadFlash(FLASH_ID_METADATA_A, 0U, (BYTE *)info, OTA_INFO_REC_SIZE) != OK) {
        return ERR;
    }
    for(i = 0U; i < 8U; i++) {
        if(info->magic[i] != magic[i]) {
            return ERR;     /* 擦除区读 0xFF，magic 必不符 */
        }
    }
    if(info->ver != OTA_INFO_VER) {
        return ERR;
    }
    if(FlashCrc32(info, OTA_INFO_CRC_OFF, &crc) != OK) {
        return ERR;
    }
    if(crc != info->crc32) {
        return ERR;         /* 撕裂写：按无记录处理 */
    }
    return OK;
}

/* 提升 B→A：先校验 B，再擦 A（逐页喂狗），负载先行、帧头最后，A 深校验。
 * 掉电中断后重入安全（见文件头）；返回 OK/ERR。 */
STATIC DWORD BlPromoteBtoA(VOID)
{
    CONST FrameHead_t *hdr;
    DWORD fwsize;
    DWORD end;      /* 负载拷贝终点（8 对齐） */
    DWORD off;

    hdr = GetFrameHead(FLASH_ID_APP_B);
    if(hdr == NULL) {
        return ERR;
    }
    fwsize = hdr->fwsize;
    if((fwsize == 0U) || (fwsize > (MEM_APP_SLOT_SIZE - FRAME_HDR_AREA_SIZE))) {
        return ERR;         /* 几何检查：负载须落在槽负载区 */
    }
    if(VerifySlotImage(FLASH_ID_APP_B) != OK) {
        return ERR;         /* B 镜像深校验（负载 CRC 回读比对） */
    }

    if(EraseFlashWithFeed(FLASH_ID_APP_A) != OK) {
        return ERR;
    }

    /* 负载先行（8 对齐分片；尾片跨出 fwsize 读擦除区 0xFF、写 0xFF 为
     * no-op，与 CDB 尾块垫 0xFF 同约定） */
    end = FRAME_PAYLOAD_OFF + ((fwsize + 7U) & ~7U);
    for(off = FRAME_PAYLOAD_OFF; off < end; off += FRAME_HDR_AREA_SIZE) {
        DWORD chunk = FRAME_HDR_AREA_SIZE;
        if(chunk > (end - off)) {
            chunk = end - off;
        }
        if(ReadFlash(FLASH_ID_APP_B, off, s_CopyBuf, chunk) != OK) {
            return ERR;
        }
        if(WriteFlash(FLASH_ID_APP_A, off, s_CopyBuf, chunk) != OK) {
            return ERR;
        }
    }
    /* 512B 帧头区最后写（头在槽才被校验/启动识别） */
    if(ReadFlash(FLASH_ID_APP_B, 0U, s_CopyBuf, FRAME_HDR_AREA_SIZE) != OK) {
        return ERR;
    }
    if(WriteFlash(FLASH_ID_APP_A, 0U, s_CopyBuf, FRAME_HDR_AREA_SIZE) != OK) {
        return ERR;
    }

    return VerifySlotImage(FLASH_ID_APP_A);
}

/* 确认清除：state→IDLE（保留 update_cnt 等其余字段，重算 crc32）后整页
 * 擦除 + 重写。失败无碍——残留 CONFIRMED 记录下次启动重试提升（幂等）。 */
STATIC DWORD BlConfirmClear(OtaInfo_t *info)
{
    DWORD crc = 0U;

    info->state = OTA_ST_IDLE;
    if(FlashCrc32(info, OTA_INFO_CRC_OFF, &crc) != OK) {
        return ERR;
    }
    info->crc32 = crc;

    if(EraseFlash(FLASH_ID_METADATA_A) != OK) {
        return ERR;
    }
    return WriteFlash(FLASH_ID_METADATA_A, 0U, (CONST BYTE *)info, OTA_INFO_REC_SIZE);
}

DWORD BlBootDecide(VOID)
{
    OtaInfo_t info;
    BYTE have_meta = (BlOtaInfoRead(&info) == OK) ? 1U : 0U;

    if((have_meta != 0U) && (info.state == OTA_ST_CONFIRMED)) {
        CONST FrameHead_t *b_hdr = GetFrameHead(FLASH_ID_APP_B);

        /* 记录↔镜像身份绑定：CONFIRMED 只提升其当初确认的那个镜像，三项
         * fwsize/crc32data/crc32hdr 必须全部一致。B 现内容与记录不符（host
         * 已换入新镜像但未 Run / 镜像损坏）→ 陈旧记录清回 IDLE（保留
         * update_cnt）、跳过提升，回落槽 A 校验——未经 Run 确认的暂存镜像
         * 不会被残留记录静默激活（CMIS Run 激活门保持）。 */
        if((b_hdr == NULL) || (b_hdr->fwsize != info.fwsize)
                           || (b_hdr->crc32data != info.crc32data)
                           || (b_hdr->crc32hdr != info.crc32hdr)) {
            (void)BlConfirmClear(&info);
        } else if(BlPromoteBtoA() == OK) {
            (void)BlConfirmClear(&info);
        }
    }

    if(VerifySlotImage(FLASH_ID_APP_A) == OK) {
        return BL_BOOT_JUMP_APP;
    }
    return BL_BOOT_RESCUE;
}

VOID BlJumpToApp(VOID)
{
    DWORD app_payload = MEM_APP_A_BASE + FRAME_PAYLOAD_OFF;   /* 0x08010200 */

    __disable_irq();                    /* 关中断保护 VTOR/MSP 切换窗口 */
    SysTick->CTRL = 0U;                 /* BL 未启用 SysTick，双保险关停 */
    SCB->VTOR = app_payload;            /* APP 中断向量表（其 SystemInit 会再置一次） */
    /* 切换+跳转全部经寄存器一次完成：__set_MSP 切栈后 C 栈即失效，跳转目标
     * 必须已在寄存器里——曾实测编译器在切栈后从 [sp,#0] 重载局部变量
     * （新栈处被启动清零）→ 跳 0 地址，BL 自我重启。复位向量含 Thumb 位，
     * bx 直接取用；cpsie 清 PRIMASK（函数指针跳转非硬件复位，PRIMASK 不会
     * 自动清零，APP 启动序列无重开点），其使能效果在下一指令边界生效。 */
    __ASM volatile (
        "ldr r2, [%0, #0]   \n"    /* r2 = APP 初始 MSP */
        "ldr r1, [%0, #4]   \n"    /* r1 = APP 复位向量 */
        "msr msp, r2        \n"    /* 切栈（此后不再碰 C 栈） */
        "cpsie i            \n"    /* 清 PRIMASK：不补则 APP 全局中断屏蔽 */
        "bx   r1            \n"    /* 跳转 APP Reset_Handler，不返回 */
        :
        : "r"(app_payload)
        : "r1", "r2", "memory");
    for(;;) {
    }                                   /* 不返回 */
}

VOID BlShadowClear(VOID)
{
    DWORD i;

    for(i = 0U; i < ADPAT_PROTOCOL_MSA_SIZE; i++) {
        g_AdpatMgmtMapBuf[i] = 0x00U;
    }
}

/* 救援模式标识装载：00h:0-2（SFF-8024 Identifier / CmisRevision / MgmtChar）
 * 取自 Flash 主参数区 Page Low 槽（基址 +512）——与 APP MgmtInit 的
 * MgiParamLoad 同源，host 枚举救援模块读到真实身份而非恒 0。其余字节保持
 * BlShadowClear 的 0 值（0x25 CdbStatus 恒 0=无固件等约定不变）。 */
VOID BlIdentityLoad(VOID)
{
    BYTE buf[8];        /* 8B 对齐读，取前 3B */

    if(ReadFlash(FLASH_ID_PARA_A, ADPAT_MGMT_MAP_BUF_OFFSET, buf, 8U) != OK) {
        return;         /* 参数区异常（擦除态等）：标识保持 0，救援环照常工作 */
    }
    AdpatShadowLock();
    (void)WriteSramProtocolMsaSpace(PAGE_ID_LOW, 0x00U, buf, 3U);
    AdpatShadowUnlock();
}

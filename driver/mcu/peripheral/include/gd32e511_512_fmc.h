/*!
    \file    gd32e511_512_fmc.h
    \brief   definitions for the FMC

    \version 2025-12-25, V0.2.0, firmware for GD32E511_512
*/

/*
    Copyright (c) 2025, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#ifndef GD32E511_512_FMC_H
#define GD32E511_512_FMC_H

#include "gd32e511_512.h"

/* FMC and option byte definition */
#define FMC                         FMC_BASE                          /*!< FMC register base address */
#define OB                          OB_BASE                           /*!< option byte base address */

/* registers definitions */
#define FMC_WS                      REG32((FMC) + 0x00000000U)        /*!< FMC wait state register */
#define FMC_KEY0                    REG32((FMC) + 0x00000004U)        /*!< FMC unlock key 0 register */
#define FMC_OBKEY                   REG32((FMC) + 0x00000008U)        /*!< FMC option bytes unlock key register */
#define FMC_STAT0                   REG32((FMC) + 0x0000000CU)        /*!< FMC status 0 register */
#define FMC_CTL0                    REG32((FMC) + 0x00000010U)        /*!< FMC control 0 register */
#define FMC_ADDR0                   REG32((FMC) + 0x00000014U)        /*!< FMC address 0 register */
#define FMC_OBSTAT                  REG32((FMC) + 0x0000001CU)        /*!< FMC option bytes status register */
#define FMC_WP                      REG32((FMC) + 0x00000020U)        /*!< FMC write protection register */
#define FMC_KEY1                    REG32((FMC) + 0x00000044U)        /*!< FMC unlock key 1 register */
#define FMC_STAT1                   REG32((FMC) + 0x0000004CU)        /*!< FMC status 1 register */
#define FMC_CTL1                    REG32((FMC) + 0x00000050U)        /*!< FMC control 1 register */
#define FMC_ADDR1                   REG32((FMC) + 0x00000054U)        /*!< FMC address 1 register */
#define FMC_EFSWD                   REG32((FMC) + 0x00000058U)        /*!< FMC efuse SWD protection register */
#define FMC_EFBOOT                  REG32((FMC) + 0x0000005CU)        /*!< FMC efuse BOOT protection register */
#define FMC_PID                     REG32((FMC) + 0x00000100U)        /*!< FMC product ID register */
#define FMC_ECCERR                  REG32((FMC) + 0x00000120U)        /*!< FMC ECC error register */
 

#define OB_SPC                      REG16((OB) + 0x00000000U)         /*!< option byte security protection value, user value and data*/
#define OB_USER0                    REG16((OB) + 0x00000002U)         /*!< option byte user0 value*/
#define OB_DATA                     REG32((OB) + 0x00000004U)         /*!< option byte data value*/
#define OB_WP                       REG64((OB) + 0x00000008U)         /*!< option byte write protection */
#define OB_USER1                    REG16((OB) + 0x00000010U)         /*!< option byte user1 value*/

/* bits definitions */
/* FMC_WS */
#define FMC_WS_WSCNT                BITS(0,2)                         /*!< wait state counter */
#define FMC_WS_PFEN                 BIT(4)                            /*!< pre-fetch enable */
#define FMC_WS_IDCEN                BIT(9)                            /*!< cache enable */
#define FMC_WS_IDCRST               BIT(11)                           /*!< cache reset */

/* FMC_KEY0 */
#define FMC_KEY0_KEY                BITS(0,31)                        /*!< FMC main flash unlock key bits */

/* FMC_OBKEY */
#define FMC_OBKEY_OBKEY             BITS(0,31)                        /*!< option bytes unlock key bits */

/* FMC_STAT0 */
#define FMC_STAT0_BUSY              BIT(0)                            /*!< flash bank0 busy flag bit */
#define FMC_STAT0_PGERR             BIT(2)                            /*!< flash bank0 program error flag bit */
#define FMC_STAT0_PGAERR            BIT(3)                            /*!< bank0 program alignment error flag bit */
#define FMC_STAT0_WPERR             BIT(4)                            /*!< flash bank0 write protection error flag bit */
#define FMC_STAT0_ENDF              BIT(5)                            /*!< bank0 end of operation flag bit */

/* FMC_CTL0 */
#define FMC_CTL0_PG                 BIT(0)                            /*!< main flash bank0 program command bit */
#define FMC_CTL0_PER                BIT(1)                            /*!< main flash bank0 page erase bit */
#define FMC_CTL0_MER                BIT(2)                            /*!< main flash bank0 mass erase bit */
#define FMC_CTL0_OBPG               BIT(4)                            /*!< option bytes program command bit */
#define FMC_CTL0_OBER               BIT(5)                            /*!< option bytes erase command bit */
#define FMC_CTL0_START              BIT(6)                            /*!< send erase command to FMC bit */
#define FMC_CTL0_LK                 BIT(7)                            /*!< flash bank0 lock bit */
#define FMC_CTL0_OBWEN              BIT(9)                            /*!< option bytes erase/program enable bit */
#define FMC_CTL0_ERRIE              BIT(10)                           /*!< bank0 error interrupt enable bit */
#define FMC_CTL0_ENDIE              BIT(12)                           /*!< bank0 end of operation interrupt enable bit */
#define FMC_CTL0_OBRLD              BIT(13)                           /*!< option bytes reload bit */

/* FMC_ADDR0 */
#define FMC_ADDR0_ADDR              BITS(0,31)                        /*!< flash bank0 command address bits */

/* FMC_OBSTAT */
#define FMC_OBSTAT_OBERR            BIT(0)                            /*!< option bytes read error bit */
#define FMC_OBSTAT_PLEVEL_BIT0      BIT(1)                            /*!< protection level bit 0 */
#define FMC_OBSTAT_PLEVEL_BIT1      BIT(2)                            /*!< protection level bit 1 */
#define FMC_OBSTAT_USER             BITS(8,15)                        /*!< option bytes user bits */
#define FMC_OBSTAT_DATA             BITS(16,31)                       /*!< option byte data bits */

/* FMC_WP */
#define FMC_WP_WP                   BITS(0,31)                        /*!< store WP[15:0] of option byte block after system reset */

/* FMC_KEY1 */
#define FMC_KEY1_KEY                BITS(0,31)                        /*!< FMC main flash unlock key bits */

/* FMC_STAT1 */
#define FMC_STAT1_BUSY              BIT(0)                            /*!< flash bank1 busy flag bit */
#define FMC_STAT1_PGERR             BIT(2)                            /*!< flash bank1 program error flag bit */
#define FMC_STAT1_PGAERR            BIT(3)                            /*!< bank1 program alignment error flag bit */
#define FMC_STAT1_WPERR             BIT(4)                            /*!< flash bank1 write protection error flag bit */
#define FMC_STAT1_ENDF              BIT(5)                            /*!< bank1 end of operation flag bit */

/* FMC_CTL1 */
#define FMC_CTL1_PG                 BIT(0)                            /*!< main flash bank1 program command bit */
#define FMC_CTL1_PER                BIT(1)                            /*!< main flash bank1 page erase bit */
#define FMC_CTL1_MER                BIT(2)                            /*!< main flash bank1 mass erase bit */
#define FMC_CTL1_START              BIT(6)                            /*!< send erase command to FMC bit */
#define FMC_CTL1_LK                 BIT(7)                            /*!< flash bank1 lock bit */
#define FMC_CTL1_ERRIE              BIT(10)                           /*!< bank1 error interrupt enable bit */
#define FMC_CTL1_ENDIE              BIT(12)                           /*!< bank1 end of operation interrupt enable bit */

/* FMC_ADDR1 */
#define FMC_ADDR1_ADDR              BITS(0,31)                        /*!< flash bank1 command address bits */

/* FMC_EFSWD */
#define FMC_EFSWD_LK                BIT(0)                            /*!< efuse SWD lock flag bit */
#define FMC_EFSWD_START             BIT(1)                            /*!< efuse SWD start bit */
#define FMC_EFSWD_BIT               BIT(2)                            /*!< efuse SWD on-off flag bit */
#define FMC_EFSWD_VALUE             BITS(0,15)                        /*!< efuse SWD value bit */

/* FMC_EFBOOT */
#define FMC_EFBOOT_LK               BIT(0)                            /*!< efuse BOOT lock flag bit */
#define FMC_EFBOOT_START            BIT(1)                            /*!< efuse BOOT start bit */
#define FMC_EFBOOT_BIT              BIT(2)                            /*!< efuse BOOT on-off flag bit */
#define FMC_EFBOOT_VALUE            BITS(0,15)                        /*!< efuse BOOT value bit */

/* FMC_PID */
#define FMC_PID_PID                 BITS(0,31)                        /*!< product ID bits */

/* FMC_ECCERR */
#define FMC_ECCERR_SECCERR          BIT(0)                            /*!< Single-bit error bit */
#define FMC_ECCERR_DECCERR          BIT(1)                            /*!< Double-bit error bit */
#define FMC_ECCERR_SYS_ECCERR       BIT(2)                            /*!< System memory ECC error bit */
#define FMC_ECCERR_BANK0_ECCERR     BIT(3)                            /*!< Bank0 ECC error bit */
#define FMC_ECCERR_OTP_ECCERR       BIT(4)                            /*!< OTP ECC error bit */
#define FMC_ECCERR_OB_ECCERR        BIT(5)                            /*!< Option bytes ECC error bit */
#define FMC_ECCERR_BANK1_ECCERR     BIT(6)                            /*!< Bank1 ECC error bit */
#define FMC_ECCERR_BANK_ECCERR      BIT(7)                            /*!< Bank ECC error bit */
#define FMC_ECCERR_DECCERRIE        BIT(14)                           /*!< Double bit errors detect interrupt enable bit */
#define FMC_ECCERR_SECCERRIE        BIT(15)                           /*!< Single bit error correct interrupt enable bit */
#define FMC_ECCERR_ECCADDR          BITS(16,31)                       /*!< The ECC error offset address bits */

/* constants definitions */
/* option byte */
typedef struct
{
    uint16_t ob_spc;                                              /*!< SPC of option byte */
    uint16_t ob_user0;                                            /*!< USER0 of option byte */
    uint32_t ob_data;                                             /*!< DATA of option byte */
    uint64_t ob_wp;                                               /*!< WP of option byte */
    uint16_t ob_user1;                                            /*!< USER1 of option byte */
}option_byte_struct;

/* fmc state */
typedef enum
{
    FMC_READY,                                                    /*!< the operation has been completed */
    FMC_BUSY,                                                     /*!< the operation is in progress */
    FMC_PGERR,                                                    /*!< program error */
    FMC_PGAERR,                                                   /*!< program alignment error */
    FMC_WPERR,                                                    /*!< erase/program protection error */
    FMC_TOERR,                                                    /*!< timeout error */
    FMC_ILLEGAL_OPERATION                                         /*!< user illegal operation */
}fmc_state_enum;

/* two 64 bits of option byte */
typedef enum
{
    DOUBLEWORD_SPC_USER0_DATA = 0,                                /*!< SPC, USER0, DATA of option byte */
    DOUBLEWORD_WP = 1,                                            /*!< WP of option byte */
    DOUBLEWORD_USER1 = 2                                          /*!< USER1 of option byte */
}option_byte_enum;

/* unlock key */
#define UNLOCK_KEY0                ((uint32_t)0x45670123U)        /*!< unlock key 0 */
#define UNLOCK_KEY1                ((uint32_t)0xCDEF89ABU)        /*!< unlock key 1 */

/* wait state counter value */
#define WS_WSCNT_0                 ((uint8_t)0x00U)               /*!< 0 wait state added */
#define WS_WSCNT_1                 ((uint8_t)0x01U)               /*!< 1 wait state added */
#define WS_WSCNT_2                 ((uint8_t)0x02U)               /*!< 2 wait state added */
#define WS_WSCNT_3                 ((uint8_t)0x03U)               /*!< 3 wait state added */
#define WS_WSCNT_4                 ((uint8_t)0x04U)               /*!< 4 wait state added */

/* read protect configure */
#define FMC_NSPC                   ((uint16_t)0x5AA5U)            /*!< no security protection */
#define FMC_LSPC                   ((uint16_t)0x44BBU)            /*!< low security protection, any value except 0xA5 or 0xCC */
#define FMC_HSPC                   ((uint16_t)0x33CCU)            /*!< high security protection */

/* option byte software/hardware free watchdog timer */  
#define OB_FWDGT_HW                ((uint8_t)(~BIT(0)))           /*!< hardware free watchdog timer */
#define OB_FWDGT_SW                ((uint8_t)BIT(0))              /*!< software free watchdog timer */

/* option byte reset or not entering deep sleep mode */
#define OB_DEEPSLEEP_RST           ((uint8_t)(~BIT(1)))           /*!< generate a reset instead of entering deepsleep mode */
#define OB_DEEPSLEEP_NRST          ((uint8_t)BIT(1))              /*!< no reset when entering deepsleep mode */

/* option byte reset or not entering standby mode */
#define OB_STDBY_RST               ((uint8_t)(~BIT(2)))           /*!< generate a reset instead of entering standby mode */
#define OB_STDBY_NRST              ((uint8_t)BIT(2))              /*!< no reset when entering standby mode */

/* option byte OB_BOOT1_n set */
#define OB_BOOT1_SET_1             ((uint8_t)(~BIT(4)))           /*!< BOOT1 bit is 1 */
#define OB_BOOT1_SET_0             ((uint8_t)BIT(4))              /*!< BOOT1 bit is 0 */

/* option byte VDDA monitor enable/disable */
#define OB_VDDA_DISABLE            ((uint8_t)(~BIT(5)))           /*!< disable VDDA monitor */
#define OB_VDDA_ENABLE             ((uint8_t)BIT(5))              /*!< enable VDDA monitor */

/* option byte SRAM parity enable/disable */
#define OB_SRAM_PARITY_ENABLE      ((uint8_t)(~BIT(6)))           /*!< enable SRAM parity check */
#define OB_SRAM_PARITY_DISABLE     ((uint8_t)BIT(6))              /*!< disable SRAM parity check */

/* option byte flash bank exchange switch on/off */
#define OB_BANK_MAPPING_SWAP       ((uint8_t)(~BIT(7)))           /*!< turn on flash bank exchange switch */
#define OB_BANK_MAPPING_NOT_SWAP   ((uint8_t)BIT(7))              /*!< turn off flash bank exchange switch */

/* option byte security protection level in FMC_OBSTAT register */
#define OB_OBSTAT_PLEVEL_NO        ((uint8_t)0x00U)               /*!< no security protection */
#define OB_OBSTAT_PLEVEL_LOW       ((uint8_t)0x01U)               /*!< low security protection */
#define OB_OBSTAT_PLEVEL_HIGH      ((uint8_t)0x03U)               /*!< high security protection */

/* option byte data address */
#define OB_DATA_ADDR0              ((uint32_t)0x1FFFF804U)        /*!< option byte data address 0 */
#define OB_DATA_ADDR1              ((uint32_t)0x1FFFF806U)        /*!< option byte data address 1 */

/* option byte SRAM ECC enable/disable */
#define OB_SRAM_ECC_DISABLE        ((uint8_t)(~BIT(0)))           /*!< disable SRAM ECC check */
#define OB_SRAM_ECC_ENABLE         ((uint8_t)BIT(0))              /*!< enable SRAM ECC check */

/* option byte Flash ECC enable/disable */
#define OB_FLASH_ECC_DISABLE       ((uint8_t)(~BIT(1)))           /*!< disable Flash ECC check */
#define OB_FLASH_ECC_ENABLE        ((uint8_t)BIT(1))              /*!< enable Flash ECC check */

/* FMC bank0 flags */
#define FMC_BANK0_FLAG_BUSY        FMC_STAT0_BUSY                 /*!< FMC bank0 busy flag */
#define FMC_BANK0_FLAG_PGERR       FMC_STAT0_PGERR                /*!< FMC bank0 programming error flag */
#define FMC_BANK0_FLAG_PGAERR      FMC_STAT0_PGAERR               /*!< FMC bank0 program alignment error flag */
#define FMC_BANK0_FLAG_WPERR       FMC_STAT0_WPERR                /*!< FMC bank0 write protection error flag */
#define FMC_BANK0_FLAG_END         FMC_STAT0_ENDF                 /*!< FMC bank0 end of programming flag */

/* FMC bank1 flags */
#define FMC_BANK1_FLAG_BUSY        FMC_STAT1_BUSY                 /*!< FMC bank0 busy flag */
#define FMC_BANK1_FLAG_PGERR       FMC_STAT1_PGERR                /*!< FMC bank0 programming error flag */
#define FMC_BANK1_FLAG_PGAERR      FMC_STAT1_PGAERR               /*!< FMC bank0 program alignment error flag */
#define FMC_BANK1_FLAG_WPERR       FMC_STAT1_WPERR                /*!< FMC bank0 write protection error flag */
#define FMC_BANK1_FLAG_END         FMC_STAT1_ENDF                 /*!< FMC bank0 end of programming flag */

/* FMC bank0 interrupt enable */
#define FMC_BANK0_INT_END          FMC_CTL0_ENDIE                 /*!< enable FMC bank0 end of operation interrupt */
#define FMC_BANK0_INT_ERR          FMC_CTL0_ERRIE                 /*!< enable FMC bank0 error interrupt */

/* FMC bank1 interrupt enable */
#define FMC_BANK1_INT_END          FMC_CTL1_ENDIE                 /*!< enable bank1 FMC end of operation interrupt */
#define FMC_BANK1_INT_ERR          FMC_CTL1_ERRIE                 /*!< enable bank1 FMC error interrupt */

/* FMC bank0 interrupt flags */
#define FMC_BANK0_INT_FLAG_PGERR   FMC_STAT0_PGERR                /*!< FMC bank0 programming error flag */
#define FMC_BANK0_INT_FLAG_PGAERR  FMC_STAT0_PGAERR               /*!< FMC bank0 program alignment error flag */
#define FMC_BANK0_INT_FLAG_WPERR   FMC_STAT0_WPERR                /*!< FMC bank0 write protection error flag */
#define FMC_BANK0_INT_FLAG_END     FMC_STAT0_ENDF                 /*!< FMC bank0 end of programming flag */

/* FMC bank1 interrupt flags */
#define FMC_BANK1_INT_FLAG_PGERR   FMC_STAT1_PGERR                /*!< FMC bank1 programming error flag */
#define FMC_BANK1_INT_FLAG_PGAERR  FMC_STAT1_PGAERR               /*!< FMC bank1 program alignment error flag */
#define FMC_BANK1_INT_FLAG_WPERR   FMC_STAT1_WPERR                /*!< FMC bank1 write protection error flag */
#define FMC_BANK1_INT_FLAG_END     FMC_STAT1_ENDF                 /*!< FMC bank1 end of programming flag */

/* FMC ECC flag */
#define FMC_FLAG_SINGLE_ECC_ERROR  FMC_ECCERR_SECCERR             /*!< Single bit error detected and correct flag */
#define FMC_FLAG_DOUBLE_ECC_ERROR  FMC_ECCERR_DECCERR             /*!< Double bit errors detect flag */
#define FMC_FLAG_SYS_ECC_ERROR     FMC_ECCERR_SYS_ECCERR          /*!< System memory ECC bit error flag */
#define FMC_FLAG_BANK0_ECC_ERROR   FMC_ECCERR_BANK0_ECCERR        /*!< Bank0 Flash ECC bit error flag */
#define FMC_FLAG_OTP_ECC_ERROR     FMC_ECCERR_OTP_ECCERR          /*!< OTP ECC bit error flag */
#define FMC_FLAG_OB_ECC_ERROR      FMC_ECCERR_OB_ECCERR           /*!< Option bytes ECC bit error flag */
#define FMC_FLAG_BANK1_ECC_ERROR   FMC_ECCERR_BANK1_ECCERR        /*!< Bank1 Flash ECC bit error flag */
#define FMC_FLAG_BANK_ECC_ERROR    FMC_ECCERR_BANK_ECCERR         /*!< Bank Flash ECC bit error flag */

/* FMC ECC interrupt enable */
#define FMC_SINGLE_ECC_ERROR       FMC_ECCERR_SECCERRIE           /*!< enable single-bit errors detect interrupt */
#define FMC_DOUBLE_ECC_ERROR       FMC_ECCERR_DECCERRIE           /*!< enable double-bit errors detect interrupt */

/* FMC ECC interrupt flags */
#define FMC_INT_FLAG_SINGLE_ECC_ERROR   FMC_ECCERR_SECCERR        /*!< Single bit error detected and correct interrupt flag */
#define FMC_INT_FLAG_DOUBLE_ECC_ERROR   FMC_ECCERR_DECCERR        /*!< Double bit errors detect interrupt flag */

/* constants definitions */
#define FMC_TIMEOUT_COUNT               ((uint32_t)0x00F0000U)                    /*!< count to judge of FMC timeout */
#define FMC_SIZE                        REG16(0x1FFFF7E0)                         /*!< flash size */
#define FLASH_BASE_ADDRESS              ((uint32_t)0x08000000U)                   /*!< flash base address */
#define BANK_ADDRESS_BOUNDARY           (0x08000000U + ((FMC_SIZE) * 0x200U))     /*!< bank0 boundary address */
#define FLASH_END_ADDRESS               (0x08000000U + ((FMC_SIZE) * 0x400U))     /*!< flash end address */
#define EFUSE_VALUE                      0x33CCU                                  /*!< efuse SWD/BOOT key value */
#define OB_USER0_VALUE                  ((uint8_t)(REG16(0x4002201CU)>> 8U))      /*!< option byte USER0 value */
#define OB_USER1_VALUE                  ((uint8_t)(REG16(0x4002201CU)>> 8U))      /*!< option byte USER1 value */
#define OTP_BASE_ADDRESS                ((uint32_t)0x1FFF7000U)                   /*!< OTP base address */
#define OTP_END_ADDRESS                 ((uint32_t)0x1FFF8000U)                   /*!< OTP end address */

/* parameter check definitions */
#ifdef FW_DEBUG_ERR_REPORT

/* check FMC page valid address */
#define NOT_FMC_PAGE_VALID_ADDRESS(page_address)      (((page_address) >= (FLASH_BASE_ADDRESS + (FMC_SIZE * 0x400U))) || ((page_address) < FLASH_BASE_ADDRESS))
/* check FMC flash valid address */
#define NOT_FMC_FLASH_VALID_ADDRESS(address)          (((address) >= FLASH_END_ADDRESS) || ((address) < FLASH_BASE_ADDRESS) || (0U != ((address) & 0x00000007U)))
/* check FMC OTP valid address */
#define NOT_FMC_OTP_VALID_ADDRESS(address)            (((address) < OTP_BASE_ADDRESS) || ((address) >= OTP_END_ADDRESS) || (0U != ((address) & 0x00000007U)))
/* check FMC OB_WP value */
#define NOT_FMC_OB_WP(ob_wp)                          (0U != (((ob_wp) & 0x7FFFFFFFU) >> (FMC_SIZE /0x10U)))

#endif /* FW_DEBUG_ERR_REPORT */

/* function declarations */
/* FMC main memory programming functions */
/* unlock the main flash operation */
void fmc_unlock(void);
/* unlock the main flash bank0 operation */
void fmc_bank0_unlock(void);
/* unlock the main flash bank1 operation */
void fmc_bank1_unlock(void);
/* lock the main flash operation */
void fmc_lock(void);
/* lock the main flash bank0 operation */
void fmc_bank0_lock(void);
/* lock the main flash bank1 operation */
void fmc_bank1_lock(void);

/* set the wait state counter value */
void fmc_wscnt_set(uint8_t wscnt);
/* enable pre-fetch */
void fmc_prefetch_enable(void);
/* disable pre-fetch */
void fmc_prefetch_disable(void);
/* enable cache */
void fmc_cache_enable(void);
/* disable cache */
void fmc_cache_disable(void);
/* enable cache reset */
void fmc_cache_reset_enable(void);
/* disable cache reset */
void fmc_cache_reset_disable(void);

/* erase page */
fmc_state_enum fmc_page_erase(uint32_t page_address);
/* erase whole chip */
fmc_state_enum fmc_mass_erase(void);
/* erase flash bank0 */
fmc_state_enum fmc_bank0_mass_erase(void);
/* erase flash bank1 */
fmc_state_enum fmc_bank1_mass_erase(void);
/* FMC program a double word at the corresponding address */
fmc_state_enum fmc_doubleword_program(uint32_t address, uint64_t data);
/* OTP program a double word at the corresponding address */
fmc_state_enum otp_doubleword_program(uint32_t address, uint64_t data);

/* FMC option bytes programming functions */
/* unlock the option byte operation */
void ob_unlock(void);
/* lock the option byte operation */
void ob_lock(void);
/* reload the option byte and generate a system reset */
void ob_reset(void);
/* erase option byte */
fmc_state_enum ob_erase(void);
/* enable option byte write protection (OB_WP) */
fmc_state_enum ob_write_protection_enable(uint32_t ob_wp);
/* configure read out protect */
fmc_state_enum ob_security_protection_config(uint16_t ob_spc);
/* write the FMC option byte user0 */
fmc_state_enum ob_user0_write(uint8_t ob_user0);
/* write the FMC option byte user1 */
fmc_state_enum ob_user1_write(uint8_t ob_user1);
/* write the FMC option byte data */
fmc_state_enum ob_data_program(uint16_t data);
/* get the value of FMC option byte OB_USER0 in FMC_OBSTAT register */
uint8_t ob_user0_get(void);
/* get the value of FMC option byte OB_DATA in FMC_OBSTAT register */
uint16_t ob_data_get(void);
/* get the value of FMC option byte WP in FMC_WP register */
uint32_t ob_write_protection_get(void);
/* get the value of FMC option byte security protection level (PLEVEL) in FMC_OBSTAT register */
uint8_t ob_plevel_get(void);
/* get the value of OB_USER1 in FMC option byte */
uint8_t ob_user1_get(void);

/* FMC interrupts and flags management functions */
/* get bank0 flag set or reset */
FlagStatus fmc_bank0_flag_get(uint32_t flag);
/* get bank1 flag set or reset */
FlagStatus fmc_bank1_flag_get(uint32_t flag);
/* clear the FMC bank0 pending flag */
void fmc_bank0_flag_clear(uint32_t flag);
/* clear the FMC bank1 pending flag */
void fmc_bank1_flag_clear(uint32_t flag);
/* enable FMC bank0 interrupt */
void fmc_bank0_interrupt_enable(uint32_t interrupt);
/* enable FMC bank1 interrupt */
void fmc_bank1_interrupt_enable(uint32_t interrupt);
/* disable FMC bank0 interrupt */
void fmc_bank0_interrupt_disable(uint32_t interrupt);
/* disable FMC bank1 interrupt */
void fmc_bank1_interrupt_disable(uint32_t interrupt);
/* get interrupt bank0 flag set or reset */
FlagStatus fmc_bank0_interrupt_flag_get(uint32_t int_flag);
/* get interrupt bank1 flag set or reset */
FlagStatus fmc_bank1_interrupt_flag_get(uint32_t int_flag);
/* clear the FMC bank0 interrupt pending flag by writing 1 */
void fmc_bank0_interrupt_flag_clear(uint32_t int_flag);
/* clear the FMC bank1 interrupt pending flag by writing 1 */
void fmc_bank1_interrupt_flag_clear(uint32_t int_flag);
/* return the FMC bank0 state */
fmc_state_enum fmc_bank0_state_get(void);
/* return the FMC bank1 state */
fmc_state_enum fmc_bank1_state_get(void);
/* check FMC bank0 ready or not */
fmc_state_enum fmc_bank0_ready_wait(uint32_t timeout);
/* check FMC bank1 ready or not */
fmc_state_enum fmc_bank1_ready_wait(uint32_t timeout);

/* MCU BOOT and SWD management functions */
/* forbid ram/system boot, merely suppport flash boot */
fmc_state_enum boot_forbid(void);
/* disable SWD interface */
fmc_state_enum swd_disable(void);

/* FMC ECC interrupts and flags management functions */
/* get the offset address of FMC ECC error */
uint16_t fmc_ecc_addr_get(void);
/* get the FMC ECC flag status */
FlagStatus fmc_ecc_flag_get(uint32_t ecc_flag);
/* clear the FMC ECC flag status */
void fmc_ecc_flag_clear(uint32_t ecc_flag);
/* enable FMC ECC interrupt */
void fmc_ecc_interrupt_enable(uint32_t ecc_interrupt);
/* disable FMC ECC interrupt */
void fmc_ecc_interrupt_disable(uint32_t ecc_interrupt);
/* get the FMC ECC interrupt flag status */
FlagStatus fmc_ecc_interrupt_flag_get(uint32_t ecc_int_flag);
/* clear the FMC ECC interrupt flag status */
void fmc_ecc_interrupt_flag_clear(uint32_t ecc_int_flag);

#endif /* GD32E511_512_FMC_H */

/*!
    \file    gd32e511_512_adc.h
    \brief   definitions for the ADC
    
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

#ifndef GD32E511_512_ADC_H
#define GD32E511_512_ADC_H

#include "gd32e511_512.h"

/* ADC definitions */
#define ADC0                                              ADC_BASE
#define ADC1                                              (ADC_BASE + 0x400U)

/* registers definitions */                               
#define ADC_STAT(adcx)                                    REG32((adcx) + 0x00U)                         /*!< ADC status register */
#define ADC_CTL0(adcx)                                    REG32((adcx) + 0x04U)                         /*!< ADC control register 0 */
#define ADC_CTL1(adcx)                                    REG32((adcx) + 0x08U)                         /*!< ADC control register 1 */
#define ADC_SAMPT(adcx,x)                                 REG32((adcx) + (0x0CU + (x)*4U))              /*!< ADC sampling time register (x = 0~1) */
#define ADC_SAMPT0(adcx)                                  REG32((adcx) + 0x0CU)                         /*!< ADC sampling time register 0 */
#define ADC_SAMPT1(adcx)                                  REG32((adcx) + 0x10U)                         /*!< ADC sampling time register 1 */
#define ADC_IOFF0(adcx)                                   REG32((adcx) + 0x14U)                         /*!< ADC inserted channel data offset register 0 */
#define ADC_IOFF1(adcx)                                   REG32((adcx) + 0x18U)                         /*!< ADC inserted channel data offset register 1 */
#define ADC_IOFF2(adcx)                                   REG32((adcx) + 0x1CU)                         /*!< ADC inserted channel data offset register 2 */
#define ADC_IOFF3(adcx)                                   REG32((adcx) + 0x20U)                         /*!< ADC inserted channel data offset register 3 */
#define ADC_WD0HT(adcx)                                   REG32((adcx) + 0x24U)                         /*!< ADC watchdog 0 high threshold register */
#define ADC_WD0LT(adcx)                                   REG32((adcx) + 0x28U)                         /*!< ADC watchdog 0 low threshold register */
#define ADC_RSQ(adcx,x)                                   REG32((adcx) + (0x2CU + (x)*4U))              /*!< ADC routine sequence register(x = 0~2)  */
#define ADC_RSQ0(adcx)                                    REG32((adcx) + 0x2CU)                         /*!< ADC routine sequence register 0 */
#define ADC_RSQ1(adcx)                                    REG32((adcx) + 0x30U)                         /*!< ADC routine sequence register 1 */
#define ADC_RSQ2(adcx)                                    REG32((adcx) + 0x34U)                         /*!< ADC routine sequence register 2 */
#define ADC_ISQ(adcx)                                     REG32((adcx) + 0x38U)                         /*!< ADC inserted sequence register */
#define ADC_LDATA0(adcx)                                  REG32((adcx) + 0x3CU)                         /*!< ADC latch data register 0 */
#define ADC_LDATA1(adcx)                                  REG32((adcx) + 0x40U)                         /*!< ADC latch data register 1 */
#define ADC_LDATA2(adcx)                                  REG32((adcx) + 0x44U)                         /*!< ADC latch data register 2 */
#define ADC_LDATA3(adcx)                                  REG32((adcx) + 0x48U)                         /*!< ADC latch data register 3 */
#define ADC_RDATA(adcx)                                   REG32((adcx) + 0x4CU)                         /*!< ADC routine data register */
#define ADC_OVSAMPCTL(adcx)                               REG32((adcx) + 0x80U)                         /*!< ADC oversampling control register */
#define ADC_CTL2(adcx)                                    REG32((adcx) + 0x88U)                         /*!< ADC control register 2 */
#define ADC_ESAMPT(adcx)                                  REG32((adcx) + 0x8CU)                         /*!< ADC sample time select register, 3239H exclusive function */
#define ADC_RSQ3_4(adcx,x)                                REG32((adcx) + (0x90U + ((x)-3U)*4U))         /*!< ADC routine sequence register(x = 3~4)  */
#define ADC_RSQ3(adcx)                                    REG32((adcx) + 0x90U)                         /*!< ADC routine sequence register 3 */
#define ADC_RSQ4(adcx)                                    REG32((adcx) + 0x94U)                         /*!< ADC routine sequence register 4 */   
#define ADC_SAMPT2(adcx)                                  REG32((adcx) + 0x98U)                         /*!< ADC sample time register 2 */
#define ADC_SYNCCTL(adcx)                                 REG32((adcx) + 0x9CU)                         /*!< ADC sync control register */
#define ADC_SYNCDATA0(adcx)                               REG32((adcx) + 0x100U)                        /*!< ADC sync routine data register 0 */
#define ADC_IDATA(adcx)                                   REG32((adcx) + 0x104U)                        /*!< ADC inserted data register */
#define ADC_LDCTL(adcx)                                   REG32((adcx) + 0x108U)                        /*!< ADC latch data control register */

/* bits definitions */                                    
/* ADC_STAT */                                            
#define ADC_STAT_WD0E                                     BIT(0)                                        /*!< analog watchdog 0 event flag */
#define ADC_STAT_EORC                                     BIT(1)                                        /*!< end of conversion flag */
#define ADC_STAT_EOIC                                     BIT(2)                                        /*!< inserted channel end of conversion flag */
#define ADC_STAT_STIC                                     BIT(3)                                        /*!< inserted channel start flag */
#define ADC_STAT_STRC                                     BIT(4)                                        /*!< routine channel start flag */
#define ADC_STAT_ROVF                                     BIT(5)                                        /*!< routine data register overflow flag */
#define ADC_STAT_IOVF                                     BIT(6)                                        /*!< inserted data register overflow flag */

/* ADC_CTL0 */                                                                                          
#define ADC_CTL0_WD0CHSEL                                 BITS(0, 4)                                    /*!< analog watchdog 0 channel select bits */
#define ADC_CTL0_EORCIE                                   BIT(5)                                        /*!< interrupt enable for EORC */
#define ADC_CTL0_WD0EIE                                   BIT(6)                                        /*!< analog watchdog 0 interrupt enable */
#define ADC_CTL0_EOICIE                                   BIT(7)                                        /*!< interrupt enable for inserted channels */
#define ADC_CTL0_SM                                       BIT(8)                                        /*!< scan mode */
#define ADC_CTL0_WD0SC                                    BIT(9)                                        /*!< when in scan mode, analog watchdog 0 is effective on a single channel */
#define ADC_CTL0_ICA                                      BIT(10)                                       /*!< automatic inserted sequence conversion */
#define ADC_CTL0_DISRC                                    BIT(11)                                       /*!< discontinuous mode on routine channels */
#define ADC_CTL0_DISIC                                    BIT(12)                                       /*!< discontinuous mode on inserted channels */
#define ADC_CTL0_DISNUM                                   BITS(13, 15)                                  /*!< discontinuous mode channel count */
#define ADC_CTL0_IWD0EN                                   BIT(22)                                       /*!< analog watchdog 0 enable on inserted channels */
#define ADC_CTL0_RWD0EN                                   BIT(23)                                       /*!< analog watchdog 0 enable on routine channels */
#define ADC_CTL0_DRES                                     BITS(24, 25)                                  /*!< ADC data resolution */
#define ADC_CTL0_ROVFIE                                   BIT(26)                                       /*!< interrupt enable for routine data register overflow */
#define ADC_CTL0_IOVFIE                                   BIT(27)                                       /*!< interrupt enable for inserted data register overflow */

/* ADC_CTL1 */                                                                                          
#define ADC_CTL1_ADCON                                    BIT(0)                                        /*!< ADC converter on */
#define ADC_CTL1_CTN                                      BIT(1)                                        /*!< continuous conversion */
#define ADC_CTL1_CLB                                      BIT(2)                                        /*!< ADC calibration */
#define ADC_CTL1_RSTCLB                                   BIT(3)                                        /*!< reset calibration */
#define ADC_CTL1_RDMA                                     BIT(8)                                        /*!< DMA request enable for routine channel */
#define ADC_CTL1_EORCM                                    BIT(10)                                       /*!< end of conversion mode for routine channel */
#define ADC_CTL1_DAL                                      BIT(11)                                       /*!< data alignment */
#define ADC_CTL1_ETSIC                                    (BIT(25) | BITS(12, 14))                      /*!< external trigger select for inserted channel */
#define ADC_CTL1_ETMIC                                    BIT(15)                                       /*!< external trigger enable for inserted channel */
#define ADC_CTL1_ETSRC                                    (BIT(26) | BITS(17, 19))                      /*!< external trigger select for routine channel */
#define ADC_CTL1_ETMRC                                    BIT(20)                                       /*!< external trigger enable for routine channel */
#define ADC_CTL1_SWICST                                   BIT(21)                                       /*!< start on inserted channel */
#define ADC_CTL1_SWRCST                                   BIT(22)                                       /*!< start on routine channel */
#define ADC_CTL1_TSVREN                                   BIT(23)                                       /*!< enable channel 16 and 17 */
#define ADC_CTL1_ETSIC4                                   BIT(25)                                       /*!< bit 4 of ETSIC */
#define ADC_CTL1_ETSRC4                                   BIT(26)                                       /*!< bit 4 of ETSRC */
#define ADC_CTL1_TSVEN                                    BIT(24)                                       /*!< enable channel 16, 3239H exclusive function, for ADC0 only */
#define ADC_CTL1_INREFEN                                  BIT(27)                                       /*!< enable channel 17, 3239H exclusive function, for ADC0 only */
#define ADC_CTL1_VDDAEN                                   BIT(28)                                       /*!< enable channel 18, 3239H exclusive function, for ADC0 only */

/* ADC_SAMPTx x=0..2 */                                                                                 
#define ADC_SAMPTX_SPTN                                   BITS(0, 2)                                    /*!< channel n(n=0..24) sample time selection */

/* ADC_IOFFx x=0..3 */                                                                                  
#define ADC_IOFFX_IOFF                                    BITS(0, 11)                                   /*!< data offset for inserted channel x */

/* ADC_WD0HT */                                                                                         
#define ADC_WD0HT_WD0HT                                   BITS(0, 19)                                   /*!< analog watchdog 0 high threshold */

/* ADC_WD0LT */                                                                                         
#define ADC_WD0LT_WD0LT                                   BITS(0, 19)                                   /*!< analog watchdog 0 low threshold */

/* ADC_RSQx x=0..4 */                                                                                   
#define ADC_RSQX_RSQN                                     BITS(0, 4)                                    /*!< n conversion in routine sequence */
#define ADC_RSQ0_RL                                       BITS(20, 24)                                  /*!< routine sequence length, BITS(20, 23) for 3239 */

/* ADC_ISQ */                                                                                           
#define ADC_ISQ_ISQN                                      BITS(0, 4)                                    /*!< n conversion in routine sequence */
#define ADC_ISQ_IL                                        BITS(20, 21)                                  /*!< inserted sequence length */

/* ADC_LDATAx x=0..3*/                                                                                  
#define ADC_LDATAx_LDATAN                                 BITS(0, 15)                                   /*!< latch channel conversion data  */

/* ADC_RDATA */                                                                                         
#define ADC_RDATA_RDATA                                   BITS(0, 15)                                   /*!< routine channel data */

/* ADC_OVSAMPCTL */                                                                                     
#define ADC_OVSAMPCTL_OVSEN                                BIT(0)                                       /*!< oversampling enable */
#define ADC_OVSAMPCTL_OVSR                                 BITS(2, 4)                                   /*!< oversampling ratio */
#define ADC_OVSAMPCTL_OVSS                                 BITS(5, 8)                                   /*!< oversampling shift */
#define ADC_OVSAMPCTL_TOVS                                 BIT(9)                                       /*!< triggered oversampling */

/* ADC_CTL2 */                                                                                          
#define ADC_CTL2_IDMA                                      BIT(1)                                       /*!< DMA request enable for inserted channel */
#define ADC_CTL2_IREP                                      BITS(2, 7)                                   /*!< inserted sequence repeat times */

/* ADC_ESAMPT */                                                                                          
#define ADC_ESAMPT_SPTN                                    BIT(0)                                       /*!< channel n(n=0..24) expand sample time selection */

/* ADC_SYNCCTL */                                                                                       
#define ADC_SYNCCTL_SYNCM                                  BITS(0,3)                                    /*!< ADC synchronization mode */
#define ADC_SYNCCTL_SYNCDLY                                BITS(8,11)                                   /*!< ADC synchronization delay */
#define ADC_SYNCCTL_SYNCDMA                                BITS(14,15)                                  /*!< ADC synchronization DMA mode selection */

/* ADC_SYNCDATA0 */                                        
#define ADC_SYNCDATA0_SYNCDATA0                             BITS(0,15)                                  /*!< ADC0 routine data in ADC synchronization mode */
#define ADC_SYNCDATA0_SYNCDATA1                             BITS(16,31)                                 /*!< ADC1 routine data in ADC synchronization mode */

/* ADC_IDATA */                                           
#define ADC_IDATA_IDATA                                    BITS(0, 15)                                  /*!< inserted channel conversion data  */

/* ADC_LDCTL */                                           
#define ADC_LDCTL_COVSELx                                  BITS(0,4)                                    /*!< ADC_LDATAx register conversion source selection */
#define ADC_LDCTL_SEQSELx                                  BIT(7)                                       /*!< ADC_LDATAx register sequence source selection */

/* constants definitions */                   
/* ADC_CTL0 register value */                 
#define CTL0_DISNUM(regval)                                (BITS(13, 15) & ((uint32_t)(regval) << 13))  /*!< number of conversions in discontinuous mode */

/* ADC resolution definitions */              
#define CTL0_DRES(regval)                                  (BITS(24, 25) & ((uint32_t)(regval) << 24))  /*!< ADC resolution */
#define ADC_RESOLUTION_12B                                 CTL0_DRES(0)                                 /*!< 12-bit ADC resolution */
#define ADC_RESOLUTION_10B                                 CTL0_DRES(1)                                 /*!< 10-bit ADC resolution */
#define ADC_RESOLUTION_8B                                  CTL0_DRES(2)                                 /*!< 8-bit ADC resolution */
#define ADC_RESOLUTION_6B                                  CTL0_DRES(3)                                 /*!< 6-bit ADC resolution */

/* ADC special function */                    
#define ADC_SCAN_MODE                                      ADC_CTL0_SM                                  /*!< scan mode */
#define ADC_INSERTED_CHANNEL_AUTO                          ADC_CTL0_ICA                                 /*!< inserted sequence convert automatically */
#define ADC_CONTINUOUS_MODE                                ADC_CTL1_CTN                                 /*!< continuous mode */

/* ADC data alignment */                                                                    
#define ADC_DATAALIGN_RIGHT                                ((uint32_t)0x00000000U)                      /*!< right alignment */
#define ADC_DATAALIGN_LEFT                                 ADC_CTL1_DAL                                 /*!< left alignment */

/* end of conversion mode */                                                                
#define ADC_EORC_SET_SEQUENCE                              ((uint32_t)0x00000000U)                      /*!< only at the end of a sequence of routine conversions, the EORC bit is set */
#define ADC_EORC_SET_CONVERSION                            ADC_CTL1_EORCM                               /*!< at the end of each routine conversion, the EOC bit is set */

/* ADC internal channel definitions */        
#define ADC_CHANNEL_INTERNAL_VREFINT                       ADC_CTL1_INREFEN                             /*!< vrefint channel */
#define ADC_CHANNEL_INTERNAL_VDDA                          ADC_CTL1_VDDAEN                              /*!< vdda channel */
#define ADC_CHANNEL_INTERNAL_HP_TEMPSENSOR                 ADC_CTL1_TSVEN                               /*!< high-precision temperature sensor channel */

/* external trigger select for routine channel */
#define CTL1_ETSRC(regval)                                 (BITS(17, 19) & ((uint32_t)(regval) << 17))
#define ADC_EXTTRIG_ROUTINE_T0_CH0                         CTL1_ETSRC(0)                                /*!< TIMER0 CH0 event select */
#define ADC_EXTTRIG_ROUTINE_T0_CH1                         CTL1_ETSRC(1)                                /*!< TIMER0 CH1 event select */
#define ADC_EXTTRIG_ROUTINE_T0_CH2                         CTL1_ETSRC(2)                                /*!< TIMER0 CH2 event select */
#define ADC_EXTTRIG_ROUTINE_T1_CH1                         CTL1_ETSRC(3)                                /*!< TIMER1 CH1 event select */
#define ADC_EXTTRIG_ROUTINE_T2_TRGO                        CTL1_ETSRC(4)                                /*!< TIMER2 TRGO event select */
#define ADC_EXTTRIG_ROUTINE_T14_CH0                        CTL1_ETSRC(5)                                /*!< TIMER14 CH0 event select */
#define ADC_EXTTRIG_ROUTINE_EXTI_11                        CTL1_ETSRC(6)                                /*!< external interrupt line 11 */
#define ADC_EXTTRIG_ROUTINE_NONE                           CTL1_ETSRC(7)                                /*!< software trigger */
#define ADC_EXTTRIG_ROUTINE_CLA0                           (ADC_CTL1_ETSRC4 | CTL1_ETSRC(0))            /*!< CLA0 output select */
#define ADC_EXTTRIG_ROUTINE_CLA2                           (ADC_CTL1_ETSRC4 | CTL1_ETSRC(1))            /*!< CLA2 output select */

/* external trigger select for inserted channel */
#define CTL1_ETSIC(regval)                                 (BITS(12, 14) & ((uint32_t)(regval) << 12))
#define ADC_EXTTRIG_INSERTED_T0_TRGO                       CTL1_ETSIC(0)                                /*!< TIMER0 TRGO event select */
#define ADC_EXTTRIG_INSERTED_T0_CH3                        CTL1_ETSIC(1)                                /*!< TIMER0 CH3 event select */
#define ADC_EXTTRIG_INSERTED_T1_TRGO                       CTL1_ETSIC(2)                                /*!< TIMER1 TRGO event select */
#define ADC_EXTTRIG_INSERTED_T1_CH0                        CTL1_ETSIC(3)                                /*!< TIMER1 CH0 event select */
#define ADC_EXTTRIG_INSERTED_T2_CH3                        CTL1_ETSIC(4)                                /*!< TIMER2 CH3 event select */
#define ADC_EXTTRIG_INSERTED_T14_TRGO                      CTL1_ETSIC(5)                                /*!< TIMER14 TRGO event select */
#define ADC_EXTTRIG_INSERTED_EXTI_15                       CTL1_ETSIC(6)                                /*!< external interrupt line 15 */
#define ADC_EXTTRIG_INSERTED_NONE                          CTL1_ETSIC(7)                                /*!< software trigger */
#define ADC_EXTTRIG_INSERTED_CLA1                          (ADC_CTL1_ETSIC4 | CTL1_ETSIC(0))            /*!< CLA1 output select */
#define ADC_EXTTRIG_INSERTED_CLA3                          (ADC_CTL1_ETSIC4 | CTL1_ETSIC(1))            /*!< CLA3 output select */

/* ADC_SAMPTX register value */                            
#define SAMPTX_SPT(regval)                                 (BITS(0, 2) & ((uint32_t)(regval) << 0))
#define ADC_SAMPLETIME_1POINT5                             SAMPTX_SPT(0)                                /*!< 1.5 sampling cycles */
#define ADC_SAMPLETIME_7POINT5                             SAMPTX_SPT(1)                                /*!< 7.5 sampling cycles */
#define ADC_SAMPLETIME_13POINT5                            SAMPTX_SPT(2)                                /*!< 13.5 sampling cycles */
#define ADC_SAMPLETIME_28POINT5                            SAMPTX_SPT(3)                                /*!< 28.5 sampling cycles */
#define ADC_SAMPLETIME_41POINT5                            SAMPTX_SPT(4)                                /*!< 41.5 sampling cycles */
#define ADC_SAMPLETIME_55POINT5                            SAMPTX_SPT(5)                                /*!< 55.5 sampling cycles */
#define ADC_SAMPLETIME_71POINT5                            SAMPTX_SPT(6)                                /*!< 71.5 sampling cycles */
#define ADC_SAMPLETIME_239POINT5                           SAMPTX_SPT(7)                                /*!< 239.5 sampling cycles */

#define SAMPT3_SPT                                         BIT(3)
#define ADC_SAMPLETIME_3POINT5                             (SAMPT3_SPT | SAMPTX_SPT(0))                 /*!< 3.5 sampling cycles, 3239H exclusive function */
#define ADC_SAMPLETIME_9POINT5                             (SAMPT3_SPT | SAMPTX_SPT(1))                 /*!< 9.5 sampling cycles, 3239H exclusive function */
#define ADC_SAMPLETIME_479POINT5                           (SAMPT3_SPT | SAMPTX_SPT(7))                 /*!< 479.5 sampling cycles, 3239H exclusive function */

/* ADC data offset for inserted channel x */               
#define IOFFX_IOFF(regval)                                 (BITS(0, 11) & ((uint32_t)(regval) << 0))

/* ADC analog watchdog 0 high threshold */                   
#define WD0HT_WD0HT(regval)                                (BITS(0, 19) & ((uint32_t)(regval) << 0))

/* ADC analog watchdog 0 low  threshold */                   
#define WD0LT_WD0LT(regval)                                (BITS(0, 19) & ((uint32_t)(regval) << 0))

/* ADC routine sequence length */                          
#define RSQ0_RL(regval)                                    (BITS(20, 24) & ((uint32_t)(regval) << 20))

/* ADC inserted sequence length */                         
#define ISQ_IL(regval)                                     (BITS(20, 21) & ((uint32_t)(regval) << 20))

/* ADC oversampling shift */                               
#define OVSAMPCTL_OVSS(regval)                             (BITS(5, 8) & ((uint32_t)(regval) << 5))
#define ADC_OVERSAMPLING_SHIFT_NONE                        OVSAMPCTL_OVSS(0)                            /*!< no oversampling shift */
#define ADC_OVERSAMPLING_SHIFT_1B                          OVSAMPCTL_OVSS(1)                            /*!< 1-bit oversampling shift */
#define ADC_OVERSAMPLING_SHIFT_2B                          OVSAMPCTL_OVSS(2)                            /*!< 2-bit oversampling shift */
#define ADC_OVERSAMPLING_SHIFT_3B                          OVSAMPCTL_OVSS(3)                            /*!< 3-bit oversampling shift */
#define ADC_OVERSAMPLING_SHIFT_4B                          OVSAMPCTL_OVSS(4)                            /*!< 4-bit oversampling shift */
#define ADC_OVERSAMPLING_SHIFT_5B                          OVSAMPCTL_OVSS(5)                            /*!< 5-bit oversampling shift */
#define ADC_OVERSAMPLING_SHIFT_6B                          OVSAMPCTL_OVSS(6)                            /*!< 6-bit oversampling shift */
#define ADC_OVERSAMPLING_SHIFT_7B                          OVSAMPCTL_OVSS(7)                            /*!< 7-bit oversampling shift */
#define ADC_OVERSAMPLING_SHIFT_8B                          OVSAMPCTL_OVSS(8)                            /*!< 8-bit oversampling shift */

/* ADC oversampling ratio */                               
#define OVSAMPCTL_OVSR(regval)                             (BITS(2, 4) & ((uint32_t)(regval) << 2))
#define ADC_OVERSAMPLING_RATIO_MUL2                        OVSAMPCTL_OVSR(0)                            /*!< oversampling ratio multiple 2 */
#define ADC_OVERSAMPLING_RATIO_MUL4                        OVSAMPCTL_OVSR(1)                            /*!< oversampling ratio multiple 4 */
#define ADC_OVERSAMPLING_RATIO_MUL8                        OVSAMPCTL_OVSR(2)                            /*!< oversampling ratio multiple 8 */
#define ADC_OVERSAMPLING_RATIO_MUL16                       OVSAMPCTL_OVSR(3)                            /*!< oversampling ratio multiple 16 */
#define ADC_OVERSAMPLING_RATIO_MUL32                       OVSAMPCTL_OVSR(4)                            /*!< oversampling ratio multiple 32 */
#define ADC_OVERSAMPLING_RATIO_MUL64                       OVSAMPCTL_OVSR(5)                            /*!< oversampling ratio multiple 64 */
#define ADC_OVERSAMPLING_RATIO_MUL128                      OVSAMPCTL_OVSR(6)                            /*!< oversampling ratio multiple 128 */
#define ADC_OVERSAMPLING_RATIO_MUL256                      OVSAMPCTL_OVSR(7)                            /*!< oversampling ratio multiple 256 */

/* ADC triggered oversampling */                           
#define ADC_OVERSAMPLING_ALL_CONVERT                       0U                                           /*!< all oversampled conversions for a channel are done consecutively after a trigger */
#define ADC_OVERSAMPLING_ONE_CONVERT                       1U                                           /*!< each oversampled conversion for a channel needs a trigger */

/* ADC sequence definitions */                             
#define ADC_ROUTINE_CHANNEL                                ((uint8_t)0x01U)                             /*!< ADC routine sequence */
#define ADC_INSERTED_CHANNEL                               ((uint8_t)0x02U)                             /*!< ADC inserted sequence */
#define ADC_ROUTINE_INSERTED_CHANNEL                       ((uint8_t)0x03U)                             /*!< both routine and inserted sequence */
#define ADC_CHANNEL_DISCON_DISABLE                         ((uint8_t)0x04U)                             /*!< disable discontinuous mode of routine & inserted channel */

/* ADC inserted channel definitions */                     
#define ADC_INSERTED_CHANNEL_0                             ((uint8_t)0x00U)                             /*!< ADC inserted channel 0 */
#define ADC_INSERTED_CHANNEL_1                             ((uint8_t)0x01U)                             /*!< ADC inserted channel 1 */
#define ADC_INSERTED_CHANNEL_2                             ((uint8_t)0x02U)                             /*!< ADC inserted channel 2 */
#define ADC_INSERTED_CHANNEL_3                             ((uint8_t)0x03U)                             /*!< ADC inserted channel 3 */

/* ADC channel definitions */                              
#define ADC_CHANNEL_0                                      ((uint8_t)0x00U)                             /*!< ADC channel 0 */
#define ADC_CHANNEL_1                                      ((uint8_t)0x01U)                             /*!< ADC channel 1 */
#define ADC_CHANNEL_2                                      ((uint8_t)0x02U)                             /*!< ADC channel 2 */
#define ADC_CHANNEL_3                                      ((uint8_t)0x03U)                             /*!< ADC channel 3 */
#define ADC_CHANNEL_4                                      ((uint8_t)0x04U)                             /*!< ADC channel 4 */
#define ADC_CHANNEL_5                                      ((uint8_t)0x05U)                             /*!< ADC channel 5 */
#define ADC_CHANNEL_6                                      ((uint8_t)0x06U)                             /*!< ADC channel 6 */
#define ADC_CHANNEL_7                                      ((uint8_t)0x07U)                             /*!< ADC channel 7 */
#define ADC_CHANNEL_8                                      ((uint8_t)0x08U)                             /*!< ADC channel 8 */
#define ADC_CHANNEL_9                                      ((uint8_t)0x09U)                             /*!< ADC channel 9 */
#define ADC_CHANNEL_10                                     ((uint8_t)0x0AU)                             /*!< ADC channel 10 */
#define ADC_CHANNEL_11                                     ((uint8_t)0x0BU)                             /*!< ADC channel 11 */
#define ADC_CHANNEL_12                                     ((uint8_t)0x0CU)                             /*!< ADC channel 12 */
#define ADC_CHANNEL_13                                     ((uint8_t)0x0DU)                             /*!< ADC channel 13 */
#define ADC_CHANNEL_14                                     ((uint8_t)0x0EU)                             /*!< ADC channel 14 */
#define ADC_CHANNEL_15                                     ((uint8_t)0x0FU)                             /*!< ADC channel 15 */
#define ADC_CHANNEL_16                                     ((uint8_t)0x10U)                             /*!< ADC channel 16 */
#define ADC_CHANNEL_17                                     ((uint8_t)0x11U)                             /*!< ADC channel 17 */
#define ADC_CHANNEL_18                                     ((uint8_t)0x12U)                             /*!< ADC channel 18 */
#define ADC_CHANNEL_19                                     ((uint8_t)0x13U)                             /*!< ADC channel 19 */
#define ADC_CHANNEL_20                                     ((uint8_t)0x14U)                             /*!< ADC channel 20 */
#define ADC_CHANNEL_21                                     ((uint8_t)0x15U)                             /*!< ADC channel 21 */
#define ADC_CHANNEL_22                                     ((uint8_t)0x16U)                             /*!< ADC channel 22 */
#define ADC_CHANNEL_23                                     ((uint8_t)0x17U)                             /*!< ADC channel 23 */
#define ADC_CHANNEL_24                                     ((uint8_t)0x18U)                             /*!< ADC channel 24 */

/* ADC latch data definitions */                           
#define ADC_LATCH_DATA_0                                   ((uint8_t)0x00U)                             /*!< ADC latch data 0 */
#define ADC_LATCH_DATA_1                                   ((uint8_t)0x01U)                             /*!< ADC latch data 1 */
#define ADC_LATCH_DATA_2                                   ((uint8_t)0x02U)                             /*!< ADC latch data 2 */
#define ADC_LATCH_DATA_3                                   ((uint8_t)0x03U)                             /*!< ADC latch data 3 */

/* ADC flag definitions */
#define ADC_FLAG_WD0E                                      ADC_STAT_WD0E                                /*!< analog watchdog 0 event flag */
#define ADC_FLAG_EORC                                      ADC_STAT_EORC                                /*!< end of routine sequence conversion flag */
#define ADC_FLAG_EOIC                                      ADC_STAT_EOIC                                /*!< end of inserted sequence conversion flag */
#define ADC_FLAG_STIC                                      ADC_STAT_STIC                                /*!< start flag of inserted sequence */
#define ADC_FLAG_STRC                                      ADC_STAT_STRC                                /*!< start flag of routine sequence */
#define ADC_FLAG_ROVF                                      ADC_STAT_ROVF                                /*!< flag of routine data register overflow */
#define ADC_FLAG_IOVF                                      ADC_STAT_IOVF                                /*!< flag of inserted data register overflow */

/* ADC interrupt definitions */                                                                         
#define ADC_INT_WD0E                                       ADC_CTL0_WD0EIE                              /*!< analog watchdog 0 event interrupt */ 
#define ADC_INT_EORC                                       ADC_CTL0_EORCIE                              /*!< end of routine sequence conversion interrupt */
#define ADC_INT_EOIC                                       ADC_CTL0_EOICIE                              /*!< end of inserted sequence conversion interrupt */
#define ADC_INT_ROVF                                       ADC_CTL0_ROVFIE                              /*!< routine data register overflow interrupt */
#define ADC_INT_IOVF                                       ADC_CTL0_IOVFIE                              /*!< inserted data register overflow interrupt */

/* ADC interrupt flag */                                                                                
#define ADC_INT_FLAG_WD0E                                  ADC_STAT_WD0E                                /*!< analog watchdog 0 event interrupt */
#define ADC_INT_FLAG_EORC                                  ADC_STAT_EORC                                /*!< end of sequence conversion interrupt */
#define ADC_INT_FLAG_EOIC                                  ADC_STAT_EOIC                                /*!< end of inserted sequence conversion interrupt */
#define ADC_INT_FLAG_ROVF                                  ADC_STAT_ROVF                                /*!< routine data register overflow interrupt flag */
#define ADC_INT_FLAG_IOVF                                  ADC_STAT_IOVF                                /*!< inserted data register overflow interrupt flag */

/* ADC synchronization delay */                            
#define SYNCCTL_SYNCDLY(regval)                            (BITS(8,11) & ((uint32_t)(regval) << 8U))    /*!< write value to ADC_SYNCCTL_SYNCDLY bit field */
#define ADC_SYNC_DELAY_5CYCLE                              SYNCCTL_SYNCDLY(0)                           /*!< the delay between 2 sampling phases in ADC synchronization modes to 5 ADC clock cycles. */
#define ADC_SYNC_DELAY_6CYCLE                              SYNCCTL_SYNCDLY(1)                           /*!< the delay between 2 sampling phases in ADC synchronization modes to 6 ADC clock cycles. */
#define ADC_SYNC_DELAY_7CYCLE                              SYNCCTL_SYNCDLY(2)                           /*!< the delay between 2 sampling phases in ADC synchronization modes to 7 ADC clock cycles. */
#define ADC_SYNC_DELAY_8CYCLE                              SYNCCTL_SYNCDLY(3)                           /*!< the delay between 2 sampling phases in ADC synchronization modes to 8 ADC clock cycles. */
#define ADC_SYNC_DELAY_9CYCLE                              SYNCCTL_SYNCDLY(4)                           /*!< the delay between 2 sampling phases in ADC synchronization modes to 9 ADC clock cycles. */
#define ADC_SYNC_DELAY_10CYCLE                             SYNCCTL_SYNCDLY(5)                           /*!< the delay between 2 sampling phases in ADC synchronization modes to 10 ADC clock cycles. */
#define ADC_SYNC_DELAY_11CYCLE                             SYNCCTL_SYNCDLY(6)                           /*!< the delay between 2 sampling phases in ADC synchronization modes to 11 ADC clock cycles. */
#define ADC_SYNC_DELAY_12CYCLE                             SYNCCTL_SYNCDLY(7)                           /*!< the delay between 2 sampling phases in ADC synchronization modes to 12 ADC clock cycles. */
#define ADC_SYNC_DELAY_13CYCLE                             SYNCCTL_SYNCDLY(8)                           /*!< the delay between 2 sampling phases in ADC synchronization modes to 13 ADC clock cycles. */
#define ADC_SYNC_DELAY_14CYCLE                             SYNCCTL_SYNCDLY(9)                           /*!< the delay between 2 sampling phases in ADC synchronization modes to 14 ADC clock cycles. */
#define ADC_SYNC_DELAY_15CYCLE                             SYNCCTL_SYNCDLY(10)                          /*!< the delay between 2 sampling phases in ADC synchronization modes to 15 ADC clock cycles. */
#define ADC_SYNC_DELAY_16CYCLE                             SYNCCTL_SYNCDLY(11)                          /*!< the delay between 2 sampling phases in ADC synchronization modes to 16 ADC clock cycles. */
#define ADC_SYNC_DELAY_17CYCLE                             SYNCCTL_SYNCDLY(12)                          /*!< the delay between 2 sampling phases in ADC synchronization modes to 17 ADC clock cycles. */
#define ADC_SYNC_DELAY_18CYCLE                             SYNCCTL_SYNCDLY(13)                          /*!< the delay between 2 sampling phases in ADC synchronization modes to 18 ADC clock cycles. */
#define ADC_SYNC_DELAY_19CYCLE                             SYNCCTL_SYNCDLY(14)                          /*!< the delay between 2 sampling phases in ADC synchronization modes to 19 ADC clock cycles. */
#define ADC_SYNC_DELAY_20CYCLE                             SYNCCTL_SYNCDLY(15)                          /*!< the delay between 2 sampling phases in ADC synchronization modes to 20 ADC clock cycles. */

/* ADC synchronization DMA mode selection */               
#define SYNCCTL_SYNCDMA(regval)                            (BITS(14,15) & ((uint32_t)(regval) << 14U))  /*!< write value to ADC_SYNCCTL_SYNCDMA bit field */
#define ADC_SYNC_DMA_DISABLE                               SYNCCTL_SYNCDMA(0)                           /*!< ADC synchronization DMA disabled */
#define ADC_SYNC_DMA_MODE0                                 SYNCCTL_SYNCDMA(1)                           /*!< ADC synchronization DMA mode 0 */
#define ADC_SYNC_DMA_MODE1                                 SYNCCTL_SYNCDMA(2)                           /*!< ADC synchronization DMA mode 1 */

/* ADC synchronization mode */                                                                          
#define SYNCCTL_SYNCM(regval)                              (BITS(0,3) & ((uint32_t)(regval) << 0U))     /*!< write value to ADC_SYNCCTL_SYNCM bit field */
#define ADC_SYNC_MODE_INDEPENDENT                          SYNCCTL_SYNCM(0)                             /*!< ADC synchronization mode disabled.All the ADCs work independently */
#define ADC_DAUL_ROUTINE_PARALLEL_INSERTED_PARALLEL        SYNCCTL_SYNCM(1)                             /*!< combined routine parallel & inserted parallel mode */
#define ADC_DAUL_ROUTINE_PARALLEL_INSERTED_ROTATION        SYNCCTL_SYNCM(2)                             /*!< combined routine parallel & trigger rotation mode */
#define ADC_DAUL_INSERTED_PARALLEL                         SYNCCTL_SYNCM(5)                             /*!< inserted parallel mode */
#define ADC_DAUL_ROUTINE_PARALLEL                          SYNCCTL_SYNCM(6)                             /*!< routine parallel mode */
#define ADC_DAUL_ROUTINE_FOLLOW_UP                         SYNCCTL_SYNCM(7)                             /*!< follow-up mode, also is time-interleaved mode */
#define ADC_DAUL_INSERTED_TRRIGGER_ROTATION                SYNCCTL_SYNCM(9)                             /*!< trigger rotation mode */
                                                                           

/* function declarations */
/* ADC deinitialization and initialization functions */
/* reset ADC */
void adc_deinit(uint32_t adc_periph);
/* enable ADC interface */
void adc_enable(uint32_t adc_periph);
/* disable ADC interface */
void adc_disable(uint32_t adc_periph);

/* ADC calibration and DMA functions */
/* ADC calibration and reset calibration */
void adc_calibration_enable(uint32_t adc_periph);
/* enable DMA request*/
void adc_dma_mode_enable(uint32_t adc_periph, uint8_t adc_sequence);
/* disable DMA request*/
void adc_dma_mode_disable(uint32_t adc_periph, uint8_t adc_sequence);
/* configure end of conversion mode */
void adc_end_of_conversion_config(uint32_t adc_periph, uint32_t end_selection);
/* enable or disable ADC internal channels */
void adc_internal_channel_config(uint32_t internal_channel, ControlStatus newvalue);

/* ADC special function functions */
/* configure ADC discontinuous mode */
void adc_discontinuous_mode_config(uint32_t adc_periph, uint8_t adc_sequence, uint8_t length);
/* configure ADC special function */
void adc_special_function_config(uint32_t adc_periph, uint32_t function, ControlStatus newvalue);

/* ADC channel configuration functions */
/* configure ADC data alignment */
void adc_data_alignment_config(uint32_t adc_periph , uint32_t data_alignment);
/* configure the length of routine sequence or inserted sequence */
void adc_channel_length_config(uint32_t adc_periph, uint8_t adc_sequence, uint32_t length);
/* configure ADC routine channel */
void adc_routine_channel_config(uint32_t adc_periph, uint8_t rank, uint8_t adc_channel, uint32_t sample_time);
/* configure ADC inserted channel */
void adc_inserted_channel_config(uint32_t adc_periph, uint8_t rank, uint8_t adc_channel, uint32_t sample_time);
/* configure ADC inserted channel offset */
void adc_inserted_channel_offset_config(uint32_t adc_periph, uint8_t inserted_channel, uint16_t offset);
/* configure ADC inserted sequence repeat times */
void adc_inserted_channel_repeat_time_config(uint32_t adc_periph, uint8_t time);

/* ADC external trigger functions */
/* configure ADC external trigger */
void adc_external_trigger_config(uint32_t adc_periph, uint8_t adc_sequence, ControlStatus newvalue);
/* configure ADC external trigger source */
void adc_external_trigger_source_config(uint32_t adc_periph, uint8_t adc_sequence, uint32_t external_trigger_source);
/* enable ADC software trigger */
void adc_software_trigger_enable(uint32_t adc_periph, uint8_t adc_sequence);


/* ADC data read functions */
/* read ADC routine sequence data register */
uint16_t adc_routine_data_read(uint32_t adc_periph);
/* read ADC inserted sequence data register */
uint16_t adc_inserted_data_read(uint32_t adc_periph);
/* read ADC latch data register */
uint16_t adc_latch_data_read(uint32_t adc_periph, uint8_t latch_data);
/* configure ADC latch data source */
void adc_latch_data_source_config(uint32_t adc_periph, uint8_t latch_data, uint8_t adc_sequence,uint8_t rank);

/* ADC analog watchdog functions */
/* configure ADC analog watchdog 0 single channel */
void adc_watchdog0_single_channel_enable(uint32_t adc_periph, uint8_t adc_channel);
/* configure ADC analog watchdog 0 sequence channel */
void adc_watchdog0_sequence_channel_enable(uint32_t adc_periph, uint8_t adc_sequence);
/* disable ADC analog watchdog 0 */
void adc_watchdog0_disable(uint32_t adc_periph);
/* configure ADC analog watchdog 0 threshold */
void adc_watchdog0_threshold_config(uint32_t adc_periph, uint32_t low_threshold, uint32_t high_threshold);

/* ADC resolution and oversample functions */
/* configure ADC resolution */
void adc_resolution_config(uint32_t adc_periph, uint32_t resolution);
/* configure ADC oversample mode */
void adc_oversample_mode_config(uint32_t adc_periph, uint8_t mode, uint16_t shift, uint8_t ratio);
/* enable ADC oversample mode */
void adc_oversample_mode_enable(uint32_t adc_periph);
/* disable ADC oversample mode */
void adc_oversample_mode_disable(uint32_t adc_periph);

/* ADC synchronization */
/* configure the ADC sync mode */
void adc_sync_mode_config(uint32_t sync_mode);
/* configure the delay between 2 sampling phases in ADC sync modes */
void adc_sync_delay_config(uint32_t sample_delay);
/* configure ADC sync DMA mode selection */
void adc_sync_dma_config(uint32_t dma_mode);
/* read ADC sync routine data register 0 */
uint32_t adc_sync_routine_data0_read(void);
/* read ADC sync master adc routine data register 0 */
uint32_t adc_sync_master_adc_routine_data0_read(void);
/* read ADC sync slave adc routine data register 0 */
uint32_t adc_sync_slave_adc_routine_data0_read(void);

/* flag and interrupt functions */
/* get the ADC flag */
FlagStatus adc_flag_get(uint32_t adc_periph, uint32_t flag);
/* clear the ADC flag */
void adc_flag_clear(uint32_t adc_periph, uint32_t flag);
/* enable ADC interrupt */
void adc_interrupt_enable(uint32_t adc_periph, uint32_t interrupt);
/* disable ADC interrupt */
void adc_interrupt_disable(uint32_t adc_periph, uint32_t interrupt);
/* get the ADC interrupt */
FlagStatus adc_interrupt_flag_get(uint32_t adc_periph, uint32_t flag);
/* clear the ADC flag */
void adc_interrupt_flag_clear(uint32_t adc_periph, uint32_t flag);
#endif /* GD32E511_512_ADC_H */

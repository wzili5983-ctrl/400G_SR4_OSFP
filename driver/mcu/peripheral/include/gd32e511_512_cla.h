/*!
    \file    gd32e511_512_cla.h
    \brief   definitions for the CLA

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

#ifndef GD32E511_512_CLA_H
#define GD32E511_512_CLA_H

#include "gd32e511_512.h"

/*CLA definitions */
#define CLA                                CLA_BASE

/* registers definitions */
#define CLA_GCTL                           REG32((CLA) + 0x00000000U)                              /*!< CLA global control register */
#define CLA_INTE                           REG32((CLA) + 0x00000004U)                              /*!< CLA interrupt flag enable register */
#define CLA_INTF                           REG32((CLA) + 0x00000008U)                              /*!< CLA interrupt flag register */
#define CLA_STAT                           REG32((CLA) + 0x0000000CU)                              /*!< CLA status register */
#define CLA_SIGS(CLAx)                     REG32((CLA) + 0x00000010U + ((CLAx) * 0x0000000CU))     /*!< CLA multiplexer selection register */
#define CLA_LCUCTL(CLAx)                   REG32((CLA) + 0x00000014U + ((CLAx) * 0x0000000CU))     /*!< CLA LCU control register */
#define CLA_CTL(CLAx)                      REG32((CLA) + 0x00000018U + ((CLAx) * 0x0000000CU))     /*!< CLA control register */

/* bits definitions */
/* CLA_GCTL */
#define CLA_GCTL_CLA0EN                    BIT(0)                           /*!< CLA0 unit enable */
#define CLA_GCTL_CLA1EN                    BIT(1)                           /*!< CLA1 unit enable */
#define CLA_GCTL_CLA2EN                    BIT(2)                           /*!< CLA2 unit enable */
#define CLA_GCTL_CLA3EN                    BIT(3)                           /*!< CLA3 unit enable */
#define CLA_GCTL_CLA4EN                    BIT(4)                           /*!< CLA4 unit enable */
#define CLA_GCTL_CLA5EN                    BIT(5)                           /*!< CLA5 unit enable */
#define CLA_GCTL_CLA6EN                    BIT(6)                           /*!< CLA6 unit enable */
#define CLA_GCTL_CLA7EN                    BIT(7)                           /*!< CLA7 unit enable */

/* CLA_INTE */
#define CLA_INTE_CLA0NIE                   BIT(0)                           /*!< CLA0 unit negedge interrupt enable */
#define CLA_INTE_CLA0PIE                   BIT(1)                           /*!< CLA0 unit posedge interrupt enable */
#define CLA_INTE_CLA1NIE                   BIT(2)                           /*!< CLA1 unit negedge interrupt enable */
#define CLA_INTE_CLA1PIE                   BIT(3)                           /*!< CLA1 unit posedge interrupt enable */
#define CLA_INTE_CLA2NIE                   BIT(4)                           /*!< CLA2 unit negedge interrupt enable */
#define CLA_INTE_CLA2PIE                   BIT(5)                           /*!< CLA2 unit posedge interrupt enable */
#define CLA_INTE_CLA3NIE                   BIT(6)                           /*!< CLA3 unit negedge interrupt enable */
#define CLA_INTE_CLA3PIE                   BIT(7)                           /*!< CLA3 unit posedge interrupt enable */
#define CLA_INTE_CLA4NIE                   BIT(8)                           /*!< CLA4 unit negedge interrupt enable */
#define CLA_INTE_CLA4PIE                   BIT(9)                           /*!< CLA4 unit posedge interrupt enable */
#define CLA_INTE_CLA5NIE                   BIT(10)                          /*!< CLA5 unit negedge interrupt enable */
#define CLA_INTE_CLA5PIE                   BIT(11)                          /*!< CLA5 unit posedge interrupt enable */
#define CLA_INTE_CLA6NIE                   BIT(12)                          /*!< CLA6 unit negedge interrupt enable */
#define CLA_INTE_CLA6PIE                   BIT(13)                          /*!< CLA6 unit posedge interrupt enable */
#define CLA_INTE_CLA7NIE                   BIT(14)                          /*!< CLA7 unit negedge interrupt enable */
#define CLA_INTE_CLA7PIE                   BIT(15)                          /*!< CLA7 unit posedge interrupt enable */

/* CLA_INTF */
#define CLA_INTF_CLA0NF                    BIT(0)                           /*!< CLA0 unit negedge flag */
#define CLA_INTF_CLA0PF                    BIT(1)                           /*!< CLA0 unit posedge flag */
#define CLA_INTF_CLA1NF                    BIT(2)                           /*!< CLA1 unit negedge flag */
#define CLA_INTF_CLA1PF                    BIT(3)                           /*!< CLA1 unit posedge flag */
#define CLA_INTF_CLA2NF                    BIT(4)                           /*!< CLA2 unit negedge flag */
#define CLA_INTF_CLA2PF                    BIT(5)                           /*!< CLA2 unit posedge flag */
#define CLA_INTF_CLA3NF                    BIT(6)                           /*!< CLA3 unit negedge flag */
#define CLA_INTF_CLA3PF                    BIT(7)                           /*!< CLA3 unit posedge flag */
#define CLA_INTF_CLA4NF                    BIT(8)                           /*!< CLA4 unit negedge flag */
#define CLA_INTF_CLA4PF                    BIT(9)                           /*!< CLA4 unit posedge flag */
#define CLA_INTF_CLA5NF                    BIT(10)                          /*!< CLA5 unit negedge flag */
#define CLA_INTF_CLA5PF                    BIT(11)                          /*!< CLA5 unit posedge flag */
#define CLA_INTF_CLA6NF                    BIT(12)                          /*!< CLA6 unit negedge flag */
#define CLA_INTF_CLA6PF                    BIT(13)                          /*!< CLA6 unit posedge flag */
#define CLA_INTF_CLA7NF                    BIT(14)                          /*!< CLA7 unit negedge flag */
#define CLA_INTF_CLA7PF                    BIT(15)                          /*!< CLA7 unit posedge flag */

/* CLA_STAT */
#define CLA_STAT_CLA0OUT                   BIT(0)                           /*!< CLA0 unit output state */
#define CLA_STAT_CLA1OUT                   BIT(1)                           /*!< CLA1 unit output state */
#define CLA_STAT_CLA2OUT                   BIT(2)                           /*!< CLA2 unit output state */
#define CLA_STAT_CLA3OUT                   BIT(3)                           /*!< CLA3 unit output state */
#define CLA_STAT_CLA4OUT                   BIT(4)                           /*!< CLA4 unit output state */
#define CLA_STAT_CLA5OUT                   BIT(5)                           /*!< CLA5 unit output state */
#define CLA_STAT_CLA6OUT                   BIT(6)                           /*!< CLA6 unit output state */
#define CLA_STAT_CLA7OUT                   BIT(7)                           /*!< CLA7 unit output state */

/* CLAx_SIGS */
#define CLA_SIGS_SIGS1                      BITS(0,3)                        /*!< multiplexer 1 input selection */
#define CLA_SIGS_SIGS0                      BITS(4,7)                        /*!< multiplexer 0 input selection */
#define CLA_SIGS_SIGS1_4                    BIT(14)                          /*!< the fifth bit of multiplexer 1 input selection */
#define CLA_SIGS_SIGS0_4                    BIT(15)                          /*!< the fifth bit of multiplexer 0 input selection */

/* CLAx_LCUCTL */
#define CLA_LCU                            BITS(0,7)                        /*!< LCU control */

/* CLAx_CTL */
#define CLA_CTL_CSEL                       BITS(0,1)                        /*!< clock selection */
#define CLA_CTL_CPOL                       BIT(2)                           /*!< clock polarity*/
#define CLA_CTL_FFRST                      BIT(3)                           /*!< Flip- flop output reset */
#define CLA_CTL_OEN                        BIT(6)                           /*!< output enable */
#define CLA_CTL_OSEL                       BIT(7)                           /*!< output selection */
#define CLA_CTL_SWSIGS1                    BIT(8)                           /*!< SIG1 Software input value */
#define CLA_CTL_SWSIGS0                    BIT(9)                           /*!< SIG0 Software input value */
#define CLA_CTL_SWSIGS1EN                  BIT(10)                          /*!< SIG1 Software input enable */
#define CLA_CTL_SWSIGS0EN                  BIT(11)                          /*!< SIG0 Software input enable */

/* CLA multiplexer */
typedef enum {
    SIGS0,            /*!< CLA multiplexer0 */
    SIGS1             /*!< CLA multiplexer1 */
} cla_sigs_enum;

/* output state of CLAx unit */
typedef enum {
    CLA_OUTPUT_LOW,  /*!< Software output low as CLA multiplexer input */
    CLA_OUTPUT_HIGH  /*!< Software output high as CLA multiplexer input */
} cla_outputstatus_enum;

/* CAL units */
#define CLA0                             ((uint32_t)0x00000000U)           /*!< CLA0 unit */
#define CLA1                             ((uint32_t)0x00000001U)           /*!< CLA1 unit */
#define CLA2                             ((uint32_t)0x00000002U)           /*!< CLA2 unit */
#define CLA3                             ((uint32_t)0x00000003U)           /*!< CLA3 unit */
#define CLA4                             ((uint32_t)0x00000004U)           /*!< CLA4 unit */
#define CLA5                             ((uint32_t)0x00000005U)           /*!< CLA5 unit */
#define CLA6                             ((uint32_t)0x00000006U)           /*!< CLA6 unit */
#define CLA7                             ((uint32_t)0x00000007U)           /*!< CLA7 unit */

/* CLA negedge and posedge interrupt enable */
#define CLA0_INT_NEGEDGE                  CLA_INTE_CLA0NIE                   /*!< enable CLA0 unit negedge interrupt */
#define CLA0_INT_POSEDGE                  CLA_INTE_CLA0PIE                   /*!< enable CLA0 unit posedge interrupt */
#define CLA1_INT_NEGEDGE                  CLA_INTE_CLA1NIE                   /*!< enable CLA1 unit negedge interrupt */
#define CLA1_INT_POSEDGE                  CLA_INTE_CLA1PIE                   /*!< enable CLA1 unit posedge interrupt */
#define CLA2_INT_NEGEDGE                  CLA_INTE_CLA2NIE                   /*!< enable CLA2 unit negedge interrupt */
#define CLA2_INT_POSEDGE                  CLA_INTE_CLA2PIE                   /*!< enable CLA2 unit posedge interrupt */
#define CLA3_INT_NEGEDGE                  CLA_INTE_CLA3NIE                   /*!< enable CLA3 unit negedge interrupt */
#define CLA3_INT_POSEDGE                  CLA_INTE_CLA3PIE                   /*!< enable CLA3 unit posedge interrupt */
#define CLA4_INT_NEGEDGE                  CLA_INTE_CLA4NIE                   /*!< enable CLA4 unit negedge interrupt */
#define CLA4_INT_POSEDGE                  CLA_INTE_CLA4PIE                   /*!< enable CLA4 unit posedge interrupt */
#define CLA5_INT_NEGEDGE                  CLA_INTE_CLA5NIE                   /*!< enable CLA5 unit negedge interrupt */
#define CLA5_INT_POSEDGE                  CLA_INTE_CLA5PIE                   /*!< enable CLA5 unit posedge interrupt */
#define CLA6_INT_NEGEDGE                  CLA_INTE_CLA6NIE                   /*!< enable CLA6 unit negedge interrupt */
#define CLA6_INT_POSEDGE                  CLA_INTE_CLA6PIE                   /*!< enable CLA6 unit posedge interrupt */
#define CLA7_INT_NEGEDGE                  CLA_INTE_CLA7NIE                   /*!< enable CLA7 unit negedge interrupt */
#define CLA7_INT_POSEDGE                  CLA_INTE_CLA7PIE                   /*!< enable CLA7 unit posedge interrupt */


/* CLA flags */
#define CLA0_FLAG_NEGEDGE                 CLA_INTF_CLA0NF                    /*!< CLA0 unit negedge flag */
#define CLA0_FLAG_POSEDGE                 CLA_INTF_CLA0PF                    /*!< CLA0 unit posedge flag */
#define CLA1_FLAG_NEGEDGE                 CLA_INTF_CLA1NF                    /*!< CLA1 unit negedge flag */
#define CLA1_FLAG_POSEDGE                 CLA_INTF_CLA1PF                    /*!< CLA1 unit posedge flag */
#define CLA2_FLAG_NEGEDGE                 CLA_INTF_CLA2NF                    /*!< CLA2 unit negedge flag */
#define CLA2_FLAG_POSEDGE                 CLA_INTF_CLA2PF                    /*!< CLA2 unit posedge flag */
#define CLA3_FLAG_NEGEDGE                 CLA_INTF_CLA3NF                    /*!< CLA3 unit negedge flag */
#define CLA3_FLAG_POSEDGE                 CLA_INTF_CLA3PF                    /*!< CLA3 unit posedge flag */
#define CLA4_FLAG_NEGEDGE                 CLA_INTF_CLA4NF                    /*!< CLA4 unit negedge flag */
#define CLA4_FLAG_POSEDGE                 CLA_INTF_CLA4PF                    /*!< CLA4 unit posedge flag */
#define CLA5_FLAG_NEGEDGE                 CLA_INTF_CLA5NF                    /*!< CLA5 unit negedge flag */
#define CLA5_FLAG_POSEDGE                 CLA_INTF_CLA5PF                    /*!< CLA5 unit posedge flag */
#define CLA6_FLAG_NEGEDGE                 CLA_INTF_CLA6NF                    /*!< CLA6 unit negedge flag */
#define CLA6_FLAG_POSEDGE                 CLA_INTF_CLA6PF                    /*!< CLA6 unit posedge flag */
#define CLA7_FLAG_NEGEDGE                 CLA_INTF_CLA7NF                    /*!< CLA7 unit negedge flag */
#define CLA7_FLAG_POSEDGE                 CLA_INTF_CLA7PF                    /*!< CLA7 unit posedge flag */

/* CLA interrupt flags */
#define CLA0_INT_FLAG_NEGEDGE             CLA_INTF_CLA0NF                    /*!< CLA0 unit negedge interrupt flag */
#define CLA0_INT_FLAG_POSEDGE             CLA_INTF_CLA0PF                    /*!< CLA0 unit posedge interrupt flag */
#define CLA1_INT_FLAG_NEGEDGE             CLA_INTF_CLA1NF                    /*!< CLA1 unit negedge interrupt flag */
#define CLA1_INT_FLAG_POSEDGE             CLA_INTF_CLA1PF                    /*!< CLA1 unit posedge interrupt flag */
#define CLA2_INT_FLAG_NEGEDGE             CLA_INTF_CLA2NF                    /*!< CLA2 unit negedge interrupt flag */
#define CLA2_INT_FLAG_POSEDGE             CLA_INTF_CLA2PF                    /*!< CLA2 unit posedge interrupt flag */
#define CLA3_INT_FLAG_NEGEDGE             CLA_INTF_CLA3NF                    /*!< CLA3 unit negedge interrupt flag */
#define CLA3_INT_FLAG_POSEDGE             CLA_INTF_CLA3PF                    /*!< CLA3 unit posedge interrupt flag */
#define CLA4_INT_FLAG_NEGEDGE             CLA_INTF_CLA4NF                    /*!< CLA4 unit negedge interrupt flag */
#define CLA4_INT_FLAG_POSEDGE             CLA_INTF_CLA4PF                    /*!< CLA4 unit posedge interrupt flag */
#define CLA5_INT_FLAG_NEGEDGE             CLA_INTF_CLA5NF                    /*!< CLA5 unit negedge interrupt flag */
#define CLA5_INT_FLAG_POSEDGE             CLA_INTF_CLA5PF                    /*!< CLA5 unit posedge interrupt flag */
#define CLA6_INT_FLAG_NEGEDGE             CLA_INTF_CLA6NF                    /*!< CLA6 unit negedge interrupt flag */
#define CLA6_INT_FLAG_POSEDGE             CLA_INTF_CLA6PF                    /*!< CLA6 unit posedge interrupt flag */
#define CLA7_INT_FLAG_NEGEDGE             CLA_INTF_CLA7NF                    /*!< CLA7 unit negedge interrupt flag */
#define CLA7_INT_FLAG_POSEDGE             CLA_INTF_CLA7PF                    /*!< CLA7 unit posedge interrupt flag */

/* multiplexer 0 input selection */
#define CLA_SIGS0(regval)                 (BITS(4,7) & ((uint32_t)(regval) << 4))
#define CLA0SIGS0_CLA0_ASYNC_OUT          CLA_SIGS0(0)                        /*!< the input of CLA0SIGS0 is CLA0_ASYNC_OUT */
#define CLA1SIGS0_CLA0_ASYNC_OUT          CLA_SIGS0(0)                        /*!< the input of CLA1SIGS0 is CLA0_ASYNC_OUT */
#define CLA2SIGS0_CLA0_ASYNC_OUT          CLA_SIGS0(0)                        /*!< the input of CLA2SIGS0 is CLA0_ASYNC_OUT */
#define CLA3SIGS0_CLA0_ASYNC_OUT          CLA_SIGS0(0)                        /*!< the input of CLA3SIGS0 is CLA0_ASYNC_OUT */
#define CLA4SIGS0_CLA0_ASYNC_OUT          CLA_SIGS0(0)                        /*!< the input of CLA4SIGS0 is CLA0_ASYNC_OUT */
#define CLA5SIGS0_CLA0_ASYNC_OUT          CLA_SIGS0(0)                        /*!< the input of CLA5SIGS0 is CLA0_ASYNC_OUT */
#define CLA6SIGS0_CLA0_ASYNC_OUT          CLA_SIGS0(0)                        /*!< the input of CLA6SIGS0 is CLA0_ASYNC_OUT */
#define CLA7SIGS0_CLA0_ASYNC_OUT          CLA_SIGS0(0)                        /*!< the input of CLA7SIGS0 is CLA0_ASYNC_OUT */

#define CLA0SIGS0_CLA1_ASYNC_OUT          CLA_SIGS0(1)                        /*!< the input of CLA0SIGS0 is CLA1_ASYNC_OUT */
#define CLA1SIGS0_CLA1_ASYNC_OUT          CLA_SIGS0(1)                        /*!< the input of CLA1SIGS0 is CLA1_ASYNC_OUT */
#define CLA2SIGS0_CLA1_ASYNC_OUT          CLA_SIGS0(1)                        /*!< the input of CLA2SIGS0 is CLA1_ASYNC_OUT */
#define CLA3SIGS0_CLA1_ASYNC_OUT          CLA_SIGS0(1)                        /*!< the input of CLA3SIGS0 is CLA1_ASYNC_OUT */
#define CLA4SIGS0_CLA1_ASYNC_OUT          CLA_SIGS0(1)                        /*!< the input of CLA4SIGS0 is CLA1_ASYNC_OUT */
#define CLA5SIGS0_CLA1_ASYNC_OUT          CLA_SIGS0(1)                        /*!< the input of CLA5SIGS0 is CLA1_ASYNC_OUT */
#define CLA6SIGS0_CLA1_ASYNC_OUT          CLA_SIGS0(1)                        /*!< the input of CLA6SIGS0 is CLA1_ASYNC_OUT */
#define CLA7SIGS0_CLA1_ASYNC_OUT          CLA_SIGS0(1)                        /*!< the input of CLA7SIGS0 is CLA1_ASYNC_OUT */

#define CLA0SIGS0_CLA2_ASYNC_OUT          CLA_SIGS0(2)                        /*!< the input of CLA0SIGS0 is CLA2_ASYNC_OUT */
#define CLA1SIGS0_CLA2_ASYNC_OUT          CLA_SIGS0(2)                        /*!< the input of CLA1SIGS0 is CLA2_ASYNC_OUT */
#define CLA2SIGS0_CLA2_ASYNC_OUT          CLA_SIGS0(2)                        /*!< the input of CLA2SIGS0 is CLA2_ASYNC_OUT */
#define CLA3SIGS0_CLA2_ASYNC_OUT          CLA_SIGS0(2)                        /*!< the input of CLA3SIGS0 is CLA2_ASYNC_OUT */
#define CLA4SIGS0_CLA2_ASYNC_OUT          CLA_SIGS0(2)                        /*!< the input of CLA4SIGS0 is CLA2_ASYNC_OUT */
#define CLA5SIGS0_CLA2_ASYNC_OUT          CLA_SIGS0(2)                        /*!< the input of CLA5SIGS0 is CLA2_ASYNC_OUT */
#define CLA6SIGS0_CLA2_ASYNC_OUT          CLA_SIGS0(2)                        /*!< the input of CLA6SIGS0 is CLA2_ASYNC_OUT */
#define CLA7SIGS0_CLA2_ASYNC_OUT          CLA_SIGS0(2)                        /*!< the input of CLA7SIGS0 is CLA2_ASYNC_OUT */

#define CLA0SIGS0_CLA3_ASYNC_OUT          CLA_SIGS0(3)                        /*!< the input of CLA0SIGS0 is CLA3_ASYNC_OUT */
#define CLA1SIGS0_CLA3_ASYNC_OUT          CLA_SIGS0(3)                        /*!< the input of CLA1SIGS0 is CLA3_ASYNC_OUT */
#define CLA2SIGS0_CLA3_ASYNC_OUT          CLA_SIGS0(3)                        /*!< the input of CLA2SIGS0 is CLA3_ASYNC_OUT */
#define CLA3SIGS0_CLA3_ASYNC_OUT          CLA_SIGS0(3)                        /*!< the input of CLA3SIGS0 is CLA3_ASYNC_OUT */
#define CLA4SIGS0_CLA3_ASYNC_OUT          CLA_SIGS0(3)                        /*!< the input of CLA4SIGS0 is CLA3_ASYNC_OUT */
#define CLA5SIGS0_CLA3_ASYNC_OUT          CLA_SIGS0(3)                        /*!< the input of CLA5SIGS0 is CLA3_ASYNC_OUT */
#define CLA6SIGS0_CLA3_ASYNC_OUT          CLA_SIGS0(3)                        /*!< the input of CLA6SIGS0 is CLA3_ASYNC_OUT */
#define CLA7SIGS0_CLA3_ASYNC_OUT          CLA_SIGS0(3)                        /*!< the input of CLA7SIGS0 is CLA3_ASYNC_OUT */

#define CLA0SIGS0_TIMER1_TRGO             CLA_SIGS0(4)                        /*!< the input of CLA0SIGS0 is TIMER1_TRGO */
#define CLA1SIGS0_TIMER2_TRGO             CLA_SIGS0(4)                        /*!< the input of CLA1SIGS0 is TIMER2_TRGO */
#define CLA2SIGS0_TIMER5_TRGO             CLA_SIGS0(4)                        /*!< the input of CLA2SIGS0 is TIMER5_TRGO */
#define CLA3SIGS0_TIMER6_TRGO             CLA_SIGS0(4)                        /*!< the input of CLA3SIGS0 is TIMER6_TRGO */
#define CLA4SIGS0_TIMER1_TRGO             CLA_SIGS0(4)                        /*!< the input of CLA4SIGS0 is TIMER1_TRGO */
#define CLA5SIGS0_TIMER2_TRGO             CLA_SIGS0(4)                        /*!< the input of CLA5SIGS0 is TIMER2_TRGO */
#define CLA6SIGS0_TIMER5_TRGO             CLA_SIGS0(4)                        /*!< the input of CLA6SIGS0 is TIMER5_TRGO */
#define CLA7SIGS0_TIMER6_TRGO             CLA_SIGS0(4)                        /*!< the input of CLA7SIGS0 is TIMER6_TRGO */

#define CLA0SIGS0_TIMER0_CH0              CLA_SIGS0(5)                        /*!< the input of CLA0SIGS0 is TIMER0_CH0 */
#define CLA1SIGS0_TIMER0_CH0              CLA_SIGS0(5)                        /*!< the input of CLA1SIGS0 is TIMER0_CH0 */
#define CLA2SIGS0_TIMER0_CH1              CLA_SIGS0(5)                        /*!< the input of CLA2SIGS0 is TIMER0_CH1 */
#define CLA3SIGS0_TIMER0_CH2              CLA_SIGS0(5)                        /*!< the input of CLA3SIGS0 is TIMER0_CH2 */
#define CLA4SIGS0_TIMER0_CH0              CLA_SIGS0(5)                        /*!< the input of CLA4SIGS0 is TIMER0_CH0 */
#define CLA5SIGS0_TIMER0_CH0              CLA_SIGS0(5)                        /*!< the input of CLA5SIGS0 is TIMER0_CH0 */
#define CLA6SIGS0_TIMER0_CH1              CLA_SIGS0(5)                        /*!< the input of CLA6SIGS0 is TIMER0_CH1 */
#define CLA7SIGS0_TIMER0_CH2              CLA_SIGS0(5)                        /*!< the input of CLA7SIGS0 is TIMER0_CH2 */

#define CLA0SIGS0_TIMER0_CH1              CLA_SIGS0(6)                        /*!< the input of CLA0SIGS0 is TIMER0_CH1 */
#define CLA1SIGS0_TIMER0_CH3              CLA_SIGS0(6)                        /*!< the input of CLA1SIGS0 is TIMER0_CH3 */
#define CLA2SIGS0_TIMER0_CH3              CLA_SIGS0(6)                        /*!< the input of CLA2SIGS0 is TIMER0_CH3 */
#define CLA3SIGS0_TIMER1_CH0              CLA_SIGS0(6)                        /*!< the input of CLA3SIGS0 is TIMER1_CH0 */
#define CLA4SIGS0_TIMER0_CH1              CLA_SIGS0(6)                        /*!< the input of CLA4SIGS0 is TIMER0_CH1 */
#define CLA5SIGS0_TIMER0_CH3              CLA_SIGS0(6)                        /*!< the input of CLA5SIGS0 is TIMER0_CH3 */
#define CLA6SIGS0_TIMER0_CH3              CLA_SIGS0(6)                        /*!< the input of CLA6SIGS0 is TIMER0_CH3 */
#define CLA7SIGS0_TIMER1_CH0              CLA_SIGS0(6)                        /*!< the input of CLA7SIGS0 is TIMER1_CH0 */

#define CLA0SIGS0_TIMER0_CH2              CLA_SIGS0(7)                        /*!< the input of CLA0SIGS0 is TIMER0_CH2 */
#define CLA1SIGS0_TIMER1_CH0              CLA_SIGS0(7)                        /*!< the input of CLA1SIGS0 is TIMER1_CH0 */
#define CLA2SIGS0_TIMER1_CH1              CLA_SIGS0(7)                        /*!< the input of CLA2SIGS0 is TIMER1_CH1 */
#define CLA3SIGS0_TIMER1_CH1              CLA_SIGS0(7)                        /*!< the input of CLA3SIGS0 is TIMER1_CH1 */
#define CLA4SIGS0_TIMER0_CH2              CLA_SIGS0(7)                        /*!< the input of CLA4SIGS0 is TIMER0_CH2 */
#define CLA5SIGS0_TIMER1_CH0              CLA_SIGS0(7)                        /*!< the input of CLA5SIGS0 is TIMER1_CH0 */
#define CLA6SIGS0_TIMER1_CH1              CLA_SIGS0(7)                        /*!< the input of CLA6SIGS0 is TIMER1_CH1 */
#define CLA7SIGS0_TIMER1_CH1              CLA_SIGS0(7)                        /*!< the input of CLA7SIGS0 is TIMER1_CH1 */

#define CLA0SIGS0_CLAIN0                  CLA_SIGS0(8)                        /*!< the input of CLA0SIGS0 is CLAIN0(PA15) */
#define CLA1SIGS0_CLAIN4                  CLA_SIGS0(8)                        /*!< the input of CLA1SIGS0 is CLAIN4(PB6) */
#define CLA2SIGS0_CLAIN0                  CLA_SIGS0(8)                        /*!< the input of CLA2SIGS0 is CLAIN0(PA15) */
#define CLA3SIGS0_CLAIN2                  CLA_SIGS0(8)                        /*!< the input of CLA3SIGS0 is CLAIN2(PB4) */
#define CLA4SIGS0_CLAIN0                  CLA_SIGS0(8)                        /*!< the input of CLA4SIGS0 is CLAIN0(PA15) */
#define CLA5SIGS0_CLAIN4                  CLA_SIGS0(8)                        /*!< the input of CLA5SIGS0 is CLAIN4(PB6) */
#define CLA6SIGS0_CLAIN0                  CLA_SIGS0(8)                        /*!< the input of CLA6SIGS0 is CLAIN0(PA15) */
#define CLA7SIGS0_CLAIN2                  CLA_SIGS0(8)                        /*!< the input of CLA7SIGS0 is CLAIN2(PB4) */

#define CLA0SIGS0_CLAIN2                  CLA_SIGS0(9)                        /*!< the input of CLA0SIGS0 is CLAIN2(PB4) */
#define CLA1SIGS0_CLAIN5                  CLA_SIGS0(9)                        /*!< the input of CLA1SIGS0 is CLAIN5(PB7) */
#define CLA2SIGS0_CLAIN1                  CLA_SIGS0(9)                        /*!< the input of CLA2SIGS0 is CLAIN1(PB3) */
#define CLA3SIGS0_CLAIN3                  CLA_SIGS0(9)                        /*!< the input of CLA3SIGS0 is CLAIN3(PB5) */
#define CLA4SIGS0_CLAIN2                  CLA_SIGS0(9)                        /*!< the input of CLA4SIGS0 is CLAIN2(PB4) */
#define CLA5SIGS0_CLAIN5                  CLA_SIGS0(9)                        /*!< the input of CLA5SIGS0 is CLAIN5(PB7) */
#define CLA6SIGS0_CLAIN1                  CLA_SIGS0(9)                        /*!< the input of CLA6SIGS0 is CLAIN1(PB3) */
#define CLA7SIGS0_CLAIN3                  CLA_SIGS0(9)                        /*!< the input of CLA7SIGS0 is CLAIN3(PB5) */

#define CLA0SIGS0_CLAIN4                  CLA_SIGS0(10)                       /*!< the input of CLA0SIGS0 is CLAIN4(PB6) */
#define CLA1SIGS0_CLAIN8                  CLA_SIGS0(10)                       /*!< the input of CLA1SIGS0 is CLAIN8(PB0) */
#define CLA2SIGS0_CLAIN8                  CLA_SIGS0(10)                       /*!< the input of CLA2SIGS0 is CLAIN8(PB0) */
#define CLA3SIGS0_CLAIN6                  CLA_SIGS0(10)                       /*!< the input of CLA3SIGS0 is CLAIN6(PB8) */
#define CLA4SIGS0_CLAIN4                  CLA_SIGS0(10)                       /*!< the input of CLA4SIGS0 is CLAIN4(PB6) */
#define CLA5SIGS0_CLAIN8                  CLA_SIGS0(10)                       /*!< the input of CLA5SIGS0 is CLAIN8(PB0) */
#define CLA6SIGS0_CLAIN8                  CLA_SIGS0(10)                       /*!< the input of CLA6SIGS0 is CLAIN8(PB0) */
#define CLA7SIGS0_CLAIN6                  CLA_SIGS0(10)                       /*!< the input of CLA7SIGS0 is CLAIN6(PB8) */

#define CLA0SIGS0_CLAIN6                  CLA_SIGS0(11)                       /*!< the input of CLA0SIGS0 is CLAIN6(PB8) */
#define CLA1SIGS0_CLAIN10                 CLA_SIGS0(11)                       /*!< the input of CLA1SIGS0 is CLAIN10(PB2) */
#define CLA2SIGS0_CLAIN9                  CLA_SIGS0(11)                       /*!< the input of CLA2SIGS0 is CLAIN9(PB1) */
#define CLA3SIGS0_CLAIN7                  CLA_SIGS0(11)                       /*!< the input of CLA3SIGS0 is CLAIN7(PB9) */
#define CLA4SIGS0_CLAIN6                  CLA_SIGS0(11)                       /*!< the input of CLA4SIGS0 is CLAIN6(PB8) */
#define CLA5SIGS0_CLAIN10                 CLA_SIGS0(11)                       /*!< the input of CLA5SIGS0 is CLAIN10(PB2) */
#define CLA6SIGS0_CLAIN9                  CLA_SIGS0(11)                       /*!< the input of CLA6SIGS0 is CLAIN9(PB1) */
#define CLA7SIGS0_CLAIN7                  CLA_SIGS0(11)                       /*!< the input of CLA7SIGS0 is CLAIN7(PB9) */

#define CLA0SIGS0_CLAIN8                  CLA_SIGS0(12)                       /*!< the input of CLA0SIGS0 is CLAIN8(PB0) */
#define CLA1SIGS0_CLAIN12                 CLA_SIGS0(12)                       /*!< the input of CLA1SIGS0 is CLAIN12(PA9) */
#define CLA2SIGS0_CLAIN14                 CLA_SIGS0(12)                       /*!< the input of CLA2SIGS0 is CLAIN14(PA11) */
#define CLA3SIGS0_CLAIN10                 CLA_SIGS0(12)                       /*!< the input of CLA3SIGS0 is CLAIN10(PB2) */
#define CLA4SIGS0_CLAIN8                  CLA_SIGS0(12)                       /*!< the input of CLA4SIGS0 is CLAIN8(PB0) */
#define CLA5SIGS0_CLAIN12                 CLA_SIGS0(12)                       /*!< the input of CLA5SIGS0 is CLAIN12(PA9) */
#define CLA6SIGS0_CLAIN14                 CLA_SIGS0(12)                       /*!< the input of CLA6SIGS0 is CLAIN14(PA11) */
#define CLA7SIGS0_CLAIN10                 CLA_SIGS0(12)                       /*!< the input of CLA7SIGS0 is CLAIN10(PB2) */

#define CLA0SIGS0_CLAIN10                 CLA_SIGS0(13)                       /*!< the input of CLA0SIGS0 is CLAIN10(PB2) */
#define CLA1SIGS0_CLAIN13                 CLA_SIGS0(13)                       /*!< the input of CLA1SIGS0 is CLAIN13(PA10) */
#define CLA2SIGS0_CLAIN15                 CLA_SIGS0(13)                       /*!< the input of CLA2SIGS0 is CLAIN15(PA12) */
#define CLA3SIGS0_CLAIN11                 CLA_SIGS0(13)                       /*!< the input of CLA3SIGS0 is CLAIN11(PA8) */
#define CLA4SIGS0_CLAIN10                 CLA_SIGS0(13)                       /*!< the input of CLA4SIGS0 is CLAIN10(PB2) */
#define CLA5SIGS0_CLAIN13                 CLA_SIGS0(13)                       /*!< the input of CLA5SIGS0 is CLAIN13(PA10) */
#define CLA6SIGS0_CLAIN15                 CLA_SIGS0(13)                       /*!< the input of CLA6SIGS0 is CLAIN15(PA12) */
#define CLA7SIGS0_CLAIN11                 CLA_SIGS0(13)                       /*!< the input of CLA7SIGS0 is CLAIN11(PA8) */

#define CLA0SIGS0_CLAIN12                 CLA_SIGS0(14)                       /*!< the input of CLA0SIGS0 is CLAIN12(PA9) */
#define CLA1SIGS0_CLAIN16                 CLA_SIGS0(14)                       /*!< the input of CLA1SIGS0 is CLAIN16(PF0) */
#define CLA2SIGS0_CLAIN16                 CLA_SIGS0(14)                       /*!< the input of CLA2SIGS0 is CLAIN16(PF0) */
#define CLA3SIGS0_CLAIN18                 CLA_SIGS0(14)                       /*!< the input of CLA3SIGS0 is CLAIN18(PA0) */
#define CLA4SIGS0_CLAIN12                 CLA_SIGS0(14)                       /*!< the input of CLA4SIGS0 is CLAIN12(PA9) */
#define CLA5SIGS0_CLAIN16                 CLA_SIGS0(14)                       /*!< the input of CLA5SIGS0 is CLAIN16(PF0) */
#define CLA6SIGS0_CLAIN16                 CLA_SIGS0(14)                       /*!< the input of CLA6SIGS0 is CLAIN16(PF0) */
#define CLA7SIGS0_CLAIN18                 CLA_SIGS0(14)                       /*!< the input of CLA7SIGS0 is CLAIN18(PA0) */

#define CLA0SIGS0_CLAIN14                 CLA_SIGS0(15)                       /*!< the input of CLA0SIGS0 is CLAIN14(PA11) */
#define CLA1SIGS0_CLAIN18                 CLA_SIGS0(15)                       /*!< the input of CLA1SIGS0 is CLAIN18(PA0) */
#define CLA2SIGS0_CLAIN17                 CLA_SIGS0(15)                       /*!< the input of CLA2SIGS0 is CLAIN17(PF1) */
#define CLA3SIGS0_CLAIN19                 CLA_SIGS0(15)                       /*!< the input of CLA3SIGS0 is CLAIN19(PA1) */
#define CLA4SIGS0_CLAIN14                 CLA_SIGS0(15)                       /*!< the input of CLA4SIGS0 is CLAIN14(PA11) */
#define CLA5SIGS0_CLAIN18                 CLA_SIGS0(15)                       /*!< the input of CLA5SIGS0 is CLAIN18(PA0) */
#define CLA6SIGS0_CLAIN17                 CLA_SIGS0(15)                       /*!< the input of CLA6SIGS0 is CLAIN17(PF1) */
#define CLA7SIGS0_CLAIN19                 CLA_SIGS0(15)                       /*!< the input of CLA7SIGS0 is CLAIN19(PA1) */

#define CLA0SIGS0_CLA4_ASYNC_OUT          CLA_SIGS_SIGS0_4|CLA_SIGS0(0)        /*!< the input of CLA0SIGS0 is CLA4_ASYNC_OUT */
#define CLA1SIGS0_CLA4_ASYNC_OUT          CLA_SIGS_SIGS0_4|CLA_SIGS0(0)        /*!< the input of CLA1SIGS0 is CLA4_ASYNC_OUT */
#define CLA2SIGS0_CLA4_ASYNC_OUT          CLA_SIGS_SIGS0_4|CLA_SIGS0(0)        /*!< the input of CLA2SIGS0 is CLA4_ASYNC_OUT */
#define CLA3SIGS0_CLA4_ASYNC_OUT          CLA_SIGS_SIGS0_4|CLA_SIGS0(0)        /*!< the input of CLA3SIGS0 is CLA4_ASYNC_OUT */
#define CLA4SIGS0_CLA4_ASYNC_OUT          CLA_SIGS_SIGS0_4|CLA_SIGS0(0)        /*!< the input of CLA4SIGS0 is CLA4_ASYNC_OUT */
#define CLA5SIGS0_CLA4_ASYNC_OUT          CLA_SIGS_SIGS0_4|CLA_SIGS0(0)        /*!< the input of CLA5SIGS0 is CLA4_ASYNC_OUT */
#define CLA6SIGS0_CLA4_ASYNC_OUT          CLA_SIGS_SIGS0_4|CLA_SIGS0(0)        /*!< the input of CLA6SIGS0 is CLA4_ASYNC_OUT */
#define CLA7SIGS0_CLA4_ASYNC_OUT          CLA_SIGS_SIGS0_4|CLA_SIGS0(0)        /*!< the input of CLA7SIGS0 is CLA4_ASYNC_OUT */

#define CLA0SIGS0_CLA5_ASYNC_OUT          CLA_SIGS_SIGS0_4|CLA_SIGS0(1)        /*!< the input of CLA0SIGS0 is CLA5_ASYNC_OUT */
#define CLA1SIGS0_CLA5_ASYNC_OUT          CLA_SIGS_SIGS0_4|CLA_SIGS0(1)        /*!< the input of CLA1SIGS0 is CLA5_ASYNC_OUT */
#define CLA2SIGS0_CLA5_ASYNC_OUT          CLA_SIGS_SIGS0_4|CLA_SIGS0(1)        /*!< the input of CLA2SIGS0 is CLA5_ASYNC_OUT */
#define CLA3SIGS0_CLA5_ASYNC_OUT          CLA_SIGS_SIGS0_4|CLA_SIGS0(1)        /*!< the input of CLA3SIGS0 is CLA5_ASYNC_OUT */
#define CLA4SIGS0_CLA5_ASYNC_OUT          CLA_SIGS_SIGS0_4|CLA_SIGS0(1)        /*!< the input of CLA4SIGS0 is CLA5_ASYNC_OUT */
#define CLA5SIGS0_CLA5_ASYNC_OUT          CLA_SIGS_SIGS0_4|CLA_SIGS0(1)        /*!< the input of CLA5SIGS0 is CLA5_ASYNC_OUT */
#define CLA6SIGS0_CLA5_ASYNC_OUT          CLA_SIGS_SIGS0_4|CLA_SIGS0(1)        /*!< the input of CLA6SIGS0 is CLA5_ASYNC_OUT */
#define CLA7SIGS0_CLA5_ASYNC_OUT          CLA_SIGS_SIGS0_4|CLA_SIGS0(1)        /*!< the input of CLA7SIGS0 is CLA5_ASYNC_OUT */

#define CLA1SIGS0_CLA6_ASYNC_OUT          CLA_SIGS_SIGS0_4|CLA_SIGS0(2)        /*!< the input of CLA1SIGS0 is CLA6_ASYNC_OUT */
#define CLA2SIGS0_CLA6_ASYNC_OUT          CLA_SIGS_SIGS0_4|CLA_SIGS0(2)        /*!< the input of CLA2SIGS0 is CLA6_ASYNC_OUT */
#define CLA3SIGS0_CLA6_ASYNC_OUT          CLA_SIGS_SIGS0_4|CLA_SIGS0(2)        /*!< the input of CLA3SIGS0 is CLA6_ASYNC_OUT */
#define CLA4SIGS0_CLA6_ASYNC_OUT          CLA_SIGS_SIGS0_4|CLA_SIGS0(2)        /*!< the input of CLA4SIGS0 is CLA6_ASYNC_OUT */
#define CLA5SIGS0_CLA6_ASYNC_OUT          CLA_SIGS_SIGS0_4|CLA_SIGS0(2)        /*!< the input of CLA5SIGS0 is CLA6_ASYNC_OUT */
#define CLA6SIGS0_CLA6_ASYNC_OUT          CLA_SIGS_SIGS0_4|CLA_SIGS0(2)        /*!< the input of CLA6SIGS0 is CLA6_ASYNC_OUT */
#define CLA7SIGS0_CLA6_ASYNC_OUT          CLA_SIGS_SIGS0_4|CLA_SIGS0(2)        /*!< the input of CLA7SIGS0 is CLA6_ASYNC_OUT */

#define CLA0SIGS0_CLA7_ASYNC_OUT          CLA_SIGS_SIGS0_4|CLA_SIGS0(3)        /*!< the input of CLA0SIGS0 is CLA7_ASYNC_OUT */
#define CLA1SIGS0_CLA7_ASYNC_OUT          CLA_SIGS_SIGS0_4|CLA_SIGS0(3)        /*!< the input of CLA1SIGS0 is CLA7_ASYNC_OUT */
#define CLA2SIGS0_CLA7_ASYNC_OUT          CLA_SIGS_SIGS0_4|CLA_SIGS0(3)        /*!< the input of CLA2SIGS0 is CLA7_ASYNC_OUT */
#define CLA3SIGS0_CLA7_ASYNC_OUT          CLA_SIGS_SIGS0_4|CLA_SIGS0(3)        /*!< the input of CLA3SIGS0 is CLA7_ASYNC_OUT */
#define CLA4SIGS0_CLA7_ASYNC_OUT          CLA_SIGS_SIGS0_4|CLA_SIGS0(3)        /*!< the input of CLA4SIGS0 is CLA7_ASYNC_OUT */
#define CLA5SIGS0_CLA7_ASYNC_OUT          CLA_SIGS_SIGS0_4|CLA_SIGS0(3)        /*!< the input of CLA5SIGS0 is CLA7_ASYNC_OUT */
#define CLA6SIGS0_CLA7_ASYNC_OUT          CLA_SIGS_SIGS0_4|CLA_SIGS0(3)        /*!< the input of CLA6SIGS0 is CLA7_ASYNC_OUT */
#define CLA7SIGS0_CLA7_ASYNC_OUT          CLA_SIGS_SIGS0_4|CLA_SIGS0(3)        /*!< the input of CLA7SIGS0 is CLA7_ASYNC_OUT */

#define CLA0SIGS0_SOFTWARE                CLA_CTL_SWSIGS0EN                    /*!< the input of CLA0SIGS0 is software */
#define CLA1SIGS0_SOFTWARE                CLA_CTL_SWSIGS0EN                    /*!< the input of CLA1SIGS0 is software */
#define CLA2SIGS0_SOFTWARE                CLA_CTL_SWSIGS0EN                    /*!< the input of CLA2SIGS0 is software */
#define CLA3SIGS0_SOFTWARE                CLA_CTL_SWSIGS0EN                    /*!< the input of CLA3SIGS0 is software */
#define CLA4SIGS0_SOFTWARE                CLA_CTL_SWSIGS0EN                    /*!< the input of CLA4SIGS0 is software */
#define CLA5SIGS0_SOFTWARE                CLA_CTL_SWSIGS0EN                    /*!< the input of CLA5SIGS0 is software */
#define CLA6SIGS0_SOFTWARE                CLA_CTL_SWSIGS0EN                    /*!< the input of CLA6SIGS0 is software */
#define CLA7SIGS0_SOFTWARE                CLA_CTL_SWSIGS0EN                    /*!< the input of CLA7SIGS0 is software */

/* multiplexer 1 input selection */
#define CLA_SIGS1(regval)                 (BITS(0,3) & ((uint32_t)(regval) << 0))
#define CLA0SIGS1_CLA0_ASYNC_OUT          CLA_SIGS1(0)                        /*!< the input of CLA0SIGS1 is CLA0_ASYNC_OUT */
#define CLA1SIGS1_CLA0_ASYNC_OUT          CLA_SIGS1(0)                        /*!< the input of CLA1SIGS1 is CLA0_ASYNC_OUT */
#define CLA2SIGS1_CLA0_ASYNC_OUT          CLA_SIGS1(0)                        /*!< the input of CLA2SIGS1 is CLA0_ASYNC_OUT */
#define CLA3SIGS1_CLA0_ASYNC_OUT          CLA_SIGS1(0)                        /*!< the input of CLA3SIGS1 is CLA0_ASYNC_OUT */
#define CLA4SIGS1_CLA0_ASYNC_OUT          CLA_SIGS1(0)                        /*!< the input of CLA4SIGS1 is CLA0_ASYNC_OUT */
#define CLA5SIGS1_CLA0_ASYNC_OUT          CLA_SIGS1(0)                        /*!< the input of CLA5SIGS1 is CLA0_ASYNC_OUT */
#define CLA6SIGS1_CLA0_ASYNC_OUT          CLA_SIGS1(0)                        /*!< the input of CLA6SIGS1 is CLA0_ASYNC_OUT */
#define CLA7SIGS1_CLA0_ASYNC_OUT          CLA_SIGS1(0)                        /*!< the input of CLA7SIGS1 is CLA0_ASYNC_OUT */

#define CLA0SIGS1_CLA1_ASYNC_OUT          CLA_SIGS1(1)                        /*!< the input of CLA0SIGS1 is CLA1_ASYNC_OUT */
#define CLA1SIGS1_CLA1_ASYNC_OUT          CLA_SIGS1(1)                        /*!< the input of CLA1SIGS1 is CLA1_ASYNC_OUT */
#define CLA2SIGS1_CLA1_ASYNC_OUT          CLA_SIGS1(1)                        /*!< the input of CLA2SIGS1 is CLA1_ASYNC_OUT */
#define CLA3SIGS1_CLA1_ASYNC_OUT          CLA_SIGS1(1)                        /*!< the input of CLA3SIGS1 is CLA1_ASYNC_OUT */
#define CLA4SIGS1_CLA1_ASYNC_OUT          CLA_SIGS1(1)                        /*!< the input of CLA4SIGS1 is CLA1_ASYNC_OUT */
#define CLA5SIGS1_CLA1_ASYNC_OUT          CLA_SIGS1(1)                        /*!< the input of CLA5SIGS1 is CLA1_ASYNC_OUT */
#define CLA6SIGS1_CLA1_ASYNC_OUT          CLA_SIGS1(1)                        /*!< the input of CLA6SIGS1 is CLA1_ASYNC_OUT */
#define CLA7SIGS1_CLA1_ASYNC_OUT          CLA_SIGS1(1)                        /*!< the input of CLA7SIGS1 is CLA1_ASYNC_OUT */

#define CLA0SIGS1_CLA2_ASYNC_OUT          CLA_SIGS1(2)                        /*!< the input of CLA0SIGS1 is CLA2_ASYNC_OUT */
#define CLA1SIGS1_CLA2_ASYNC_OUT          CLA_SIGS1(2)                        /*!< the input of CLA1SIGS1 is CLA2_ASYNC_OUT */
#define CLA2SIGS1_CLA2_ASYNC_OUT          CLA_SIGS1(2)                        /*!< the input of CLA2SIGS1 is CLA2_ASYNC_OUT */
#define CLA3SIGS1_CLA2_ASYNC_OUT          CLA_SIGS1(2)                        /*!< the input of CLA3SIGS1 is CLA2_ASYNC_OUT */
#define CLA4SIGS1_CLA2_ASYNC_OUT          CLA_SIGS1(2)                        /*!< the input of CLA4SIGS1 is CLA2_ASYNC_OUT */
#define CLA5SIGS1_CLA2_ASYNC_OUT          CLA_SIGS1(2)                        /*!< the input of CLA5SIGS1 is CLA2_ASYNC_OUT */
#define CLA6SIGS1_CLA2_ASYNC_OUT          CLA_SIGS1(2)                        /*!< the input of CLA6SIGS1 is CLA2_ASYNC_OUT */
#define CLA7SIGS1_CLA2_ASYNC_OUT          CLA_SIGS1(2)                        /*!< the input of CLA7SIGS1 is CLA2_ASYNC_OUT */

#define CLA0SIGS1_CLA3_ASYNC_OUT          CLA_SIGS1(3)                        /*!< the input of CLA0SIGS1 is CLA3_ASYNC_OUT */
#define CLA1SIGS1_CLA3_ASYNC_OUT          CLA_SIGS1(3)                        /*!< the input of CLA1SIGS1 is CLA3_ASYNC_OUT */
#define CLA2SIGS1_CLA3_ASYNC_OUT          CLA_SIGS1(3)                        /*!< the input of CLA2SIGS1 is CLA3_ASYNC_OUT */
#define CLA3SIGS1_CLA3_ASYNC_OUT          CLA_SIGS1(3)                        /*!< the input of CLA3SIGS1 is CLA3_ASYNC_OUT */
#define CLA4SIGS1_CLA3_ASYNC_OUT          CLA_SIGS1(3)                        /*!< the input of CLA4SIGS1 is CLA3_ASYNC_OUT */
#define CLA5SIGS1_CLA3_ASYNC_OUT          CLA_SIGS1(3)                        /*!< the input of CLA5SIGS1 is CLA3_ASYNC_OUT */
#define CLA6SIGS1_CLA3_ASYNC_OUT          CLA_SIGS1(3)                        /*!< the input of CLA6SIGS1 is CLA3_ASYNC_OUT */
#define CLA7SIGS1_CLA3_ASYNC_OUT          CLA_SIGS1(3)                        /*!< the input of CLA7SIGS1 is CLA3_ASYNC_OUT */

#define CLA0SIGS1_ADC0_CONV               CLA_SIGS1(4)                        /*!< the input of CLA0SIGS1 is ADC0_CONV */
#define CLA1SIGS1_ADC0_CONV               CLA_SIGS1(4)                        /*!< the input of CLA1SIGS1 is ADC0_CONV */
#define CLA2SIGS1_ADC0_CONV               CLA_SIGS1(4)                        /*!< the input of CLA2SIGS1 is ADC0_CONV */
#define CLA3SIGS1_ADC0_CONV               CLA_SIGS1(4)                        /*!< the input of CLA3SIGS1 is ADC0_CONV */
#define CLA4SIGS1_ADC0_CONV               CLA_SIGS1(4)                        /*!< the input of CLA4SIGS1 is ADC0_CONV */
#define CLA5SIGS1_ADC0_CONV               CLA_SIGS1(4)                        /*!< the input of CLA5SIGS1 is ADC0_CONV */
#define CLA6SIGS1_ADC0_CONV               CLA_SIGS1(4)                        /*!< the input of CLA6SIGS1 is ADC0_CONV */
#define CLA7SIGS1_ADC0_CONV               CLA_SIGS1(4)                        /*!< the input of CLA7SIGS1 is ADC0_CONV */

#define CLA0SIGS1_TIMER0_CH3              CLA_SIGS1(5)                        /*!< the input of CLA0SIGS1 is TIMER0_CH3 */
#define CLA1SIGS1_TIMER0_CH1              CLA_SIGS1(5)                        /*!< the input of CLA1SIGS1 is TIMER0_CH1 */
#define CLA2SIGS1_TIMER0_CH0              CLA_SIGS1(5)                        /*!< the input of CLA2SIGS1 is TIMER0_CH0 */
#define CLA3SIGS1_TIMER0_CH0              CLA_SIGS1(5)                        /*!< the input of CLA3SIGS1 is TIMER0_CH0 */
#define CLA4SIGS1_TIMER0_CH3              CLA_SIGS1(5)                        /*!< the input of CLA4SIGS1 is TIMER0_CH3 */
#define CLA5SIGS1_TIMER0_CH1              CLA_SIGS1(5)                        /*!< the input of CLA5SIGS1 is TIMER0_CH1 */
#define CLA6SIGS1_TIMER0_CH0              CLA_SIGS1(5)                        /*!< the input of CLA6SIGS1 is TIMER0_CH0 */
#define CLA7SIGS1_TIMER0_CH0              CLA_SIGS1(5)                        /*!< the input of CLA7SIGS1 is TIMER0_CH0 */

#define CLA0SIGS1_TIMER1_CH0              CLA_SIGS1(6)                        /*!< the input of CLA0SIGS1 is TIMER1_CH0 */
#define CLA1SIGS1_TIMER0_CH2              CLA_SIGS1(6)                        /*!< the input of CLA1SIGS1 is TIMER0_CH2 */
#define CLA2SIGS1_TIMER0_CH2              CLA_SIGS1(6)                        /*!< the input of CLA2SIGS1 is TIMER0_CH2 */
#define CLA3SIGS1_TIMER0_CH1              CLA_SIGS1(6)                        /*!< the input of CLA3SIGS1 is TIMER0_CH1 */
#define CLA4SIGS1_TIMER1_CH0              CLA_SIGS1(6)                        /*!< the input of CLA4SIGS1 is TIMER1_CH0 */
#define CLA5SIGS1_TIMER0_CH2              CLA_SIGS1(6)                        /*!< the input of CLA5SIGS1 is TIMER0_CH2 */
#define CLA6SIGS1_TIMER0_CH2              CLA_SIGS1(6)                        /*!< the input of CLA6SIGS1 is TIMER0_CH2 */
#define CLA7SIGS1_TIMER0_CH1              CLA_SIGS1(6)                        /*!< the input of CLA7SIGS1 is TIMER0_CH1 */

#define CLA0SIGS1_TIMER1_CH1              CLA_SIGS1(7)                        /*!< the input of CLA0SIGS1 is TIMER1_CH1 */
#define CLA1SIGS1_TIMER1_CH1              CLA_SIGS1(7)                        /*!< the input of CLA1SIGS1 is TIMER1_CH1 */
#define CLA2SIGS1_TIMER1_CH0              CLA_SIGS1(7)                        /*!< the input of CLA2SIGS1 is TIMER1_CH0 */
#define CLA3SIGS1_TIMER0_CH3              CLA_SIGS1(7)                        /*!< the input of CLA3SIGS1 is TIMER0_CH3 */
#define CLA4SIGS1_TIMER1_CH1              CLA_SIGS1(7)                        /*!< the input of CLA4SIGS1 is TIMER1_CH1 */
#define CLA5SIGS1_TIMER1_CH1              CLA_SIGS1(7)                        /*!< the input of CLA5SIGS1 is TIMER1_CH1 */
#define CLA6SIGS1_TIMER1_CH0              CLA_SIGS1(7)                        /*!< the input of CLA6SIGS1 is TIMER1_CH0 */
#define CLA7SIGS1_TIMER0_CH3              CLA_SIGS1(7)                        /*!< the input of CLA7SIGS1 is TIMER0_CH3 */

#define CLA0SIGS1_CLAIN1                  CLA_SIGS1(8)                        /*!< the input of CLA0SIGS1 is CLAIN1(PB3) */
#define CLA1SIGS1_CLAIN6                  CLA_SIGS1(8)                        /*!< the input of CLA1SIGS1 is CLAIN6(PB8) */
#define CLA2SIGS1_CLAIN2                  CLA_SIGS1(8)                        /*!< the input of CLA2SIGS1 is CLAIN2(PB4) */
#define CLA3SIGS1_CLAIN0                  CLA_SIGS1(8)                        /*!< the input of CLA3SIGS1 is CLAIN0(PA15) */
#define CLA4SIGS1_CLAIN1                  CLA_SIGS1(8)                        /*!< the input of CLA4SIGS1 is CLAIN1(PB3) */
#define CLA5SIGS1_CLAIN6                  CLA_SIGS1(8)                        /*!< the input of CLA5SIGS1 is CLAIN6(PB8) */
#define CLA6SIGS1_CLAIN2                  CLA_SIGS1(8)                        /*!< the input of CLA6SIGS1 is CLAIN2(PB4) */
#define CLA7SIGS1_CLAIN0                  CLA_SIGS1(8)                        /*!< the input of CLA7SIGS1 is CLAIN0(PA15) */

#define CLA0SIGS1_CLAIN3                  CLA_SIGS1(9)                        /*!< the input of CLA0SIGS1 is CLAIN3(PB5) */
#define CLA1SIGS1_CLAIN7                  CLA_SIGS1(9)                        /*!< the input of CLA1SIGS1 is CLAIN7(PB9) */
#define CLA2SIGS1_CLAIN3                  CLA_SIGS1(9)                        /*!< the input of CLA2SIGS1 is CLAIN3(PB5) */
#define CLA3SIGS1_CLAIN1                  CLA_SIGS1(9)                        /*!< the input of CLA3SIGS1 is CLAIN1(PB3) */
#define CLA4SIGS1_CLAIN3                  CLA_SIGS1(9)                        /*!< the input of CLA4SIGS1 is CLAIN3(PB5) */
#define CLA5SIGS1_CLAIN7                  CLA_SIGS1(9)                        /*!< the input of CLA5SIGS1 is CLAIN7(PB9) */
#define CLA6SIGS1_CLAIN3                  CLA_SIGS1(9)                        /*!< the input of CLA6SIGS1 is CLAIN3(PB5) */
#define CLA7SIGS1_CLAIN1                  CLA_SIGS1(9)                        /*!< the input of CLA7SIGS1 is CLAIN1(PB3) */

#define CLA0SIGS1_CLAIN5                  CLA_SIGS1(10)                       /*!< the input of CLA0SIGS1 is CLAIN5(PB7) */
#define CLA1SIGS1_CLAIN9                  CLA_SIGS1(10)                       /*!< the input of CLA1SIGS1 is CLAIN9(PB1) */
#define CLA2SIGS1_CLAIN10                 CLA_SIGS1(10)                       /*!< the input of CLA2SIGS1 is CLAIN10(PB2) */
#define CLA3SIGS1_CLAIN4                  CLA_SIGS1(10)                       /*!< the input of CLA3SIGS1 is CLAIN4(PB6) */
#define CLA4SIGS1_CLAIN5                  CLA_SIGS1(10)                       /*!< the input of CLA4SIGS1 is CLAIN5(PB7) */
#define CLA5SIGS1_CLAIN9                  CLA_SIGS1(10)                       /*!< the input of CLA5SIGS1 is CLAIN9(PB1) */
#define CLA6SIGS1_CLAIN10                 CLA_SIGS1(10)                       /*!< the input of CLA6SIGS1 is CLAIN10(PB2) */
#define CLA7SIGS1_CLAIN4                  CLA_SIGS1(10)                       /*!< the input of CLA7SIGS1 is CLAIN4(PB6) */

#define CLA0SIGS1_CLAIN6                  CLA_SIGS1(11)                       /*!< the input of CLA0SIGS1 is CLAIN6(PB9) */
#define CLA1SIGS1_CLAIN11                 CLA_SIGS1(11)                       /*!< the input of CLA1SIGS1 is CLAIN11(PA8) */
#define CLA2SIGS1_CLAIN11                 CLA_SIGS1(11)                       /*!< the input of CLA2SIGS1 is CLAIN11(PA8) */
#define CLA3SIGS1_CLAIN5                  CLA_SIGS1(11)                       /*!< the input of CLA3SIGS1 is CLAIN5(PB7) */
#define CLA4SIGS1_CLAIN6                  CLA_SIGS1(11)                       /*!< the input of CLA4SIGS1 is CLAIN6(PB9) */
#define CLA5SIGS1_CLAIN11                 CLA_SIGS1(11)                       /*!< the input of CLA5SIGS1 is CLAIN11(PA8) */
#define CLA6SIGS1_CLAIN11                 CLA_SIGS1(11)                       /*!< the input of CLA6SIGS1 is CLAIN11(PA8) */
#define CLA7SIGS1_CLAIN5                  CLA_SIGS1(11)                       /*!< the input of CLA7SIGS1 is CLAIN5(PB7) */

#define CLA0SIGS1_CLAIN9                  CLA_SIGS1(12)                       /*!< the input of CLA0SIGS1 is CLAIN9(PB1) */
#define CLA1SIGS1_CLAIN14                 CLA_SIGS1(12)                       /*!< the input of CLA1SIGS1 is CLAIN14(PA11) */
#define CLA2SIGS1_CLAIN12                 CLA_SIGS1(12)                       /*!< the input of CLA2SIGS1 is CLAIN12(PA9) */
#define CLA3SIGS1_CLAIN8                  CLA_SIGS1(12)                       /*!< the input of CLA3SIGS1 is CLAIN8(PB0) */
#define CLA4SIGS1_CLAIN9                  CLA_SIGS1(12)                       /*!< the input of CLA4SIGS1 is CLAIN9(PB1) */
#define CLA5SIGS1_CLAIN14                 CLA_SIGS1(12)                       /*!< the input of CLA5SIGS1 is CLAIN14(PA11) */
#define CLA6SIGS1_CLAIN12                 CLA_SIGS1(12)                       /*!< the input of CLA6SIGS1 is CLAIN12(PA9) */
#define CLA7SIGS1_CLAIN8                  CLA_SIGS1(12)                       /*!< the input of CLA7SIGS1 is CLAIN8(PB0) */

#define CLA0SIGS1_CLAIN11                 CLA_SIGS1(13)                       /*!< the input of CLA0SIGS1 is CLAIN11(PA8) */
#define CLA1SIGS1_CLAIN15                 CLA_SIGS1(13)                       /*!< the input of CLA1SIGS1 is CLAIN15(PA12) */
#define CLA2SIGS1_CLAIN13                 CLA_SIGS1(13)                       /*!< the input of CLA2SIGS1 is CLAIN13(PA10) */
#define CLA3SIGS1_CLAIN9                  CLA_SIGS1(13)                       /*!< the input of CLA3SIGS1 is CLAIN9(PB1) */
#define CLA4SIGS1_CLAIN11                 CLA_SIGS1(13)                       /*!< the input of CLA4SIGS1 is CLAIN11(PA8) */
#define CLA5SIGS1_CLAIN15                 CLA_SIGS1(13)                       /*!< the input of CLA5SIGS1 is CLAIN15(PA12) */
#define CLA6SIGS1_CLAIN13                 CLA_SIGS1(13)                       /*!< the input of CLA6SIGS1 is CLAIN13(PA10) */
#define CLA7SIGS1_CLAIN9                  CLA_SIGS1(13)                       /*!< the input of CLA7SIGS1 is CLAIN9(PB1) */

#define CLA0SIGS1_CLAIN13                 CLA_SIGS1(14)                       /*!< the input of CLA0SIGS1 is CLAIN13(PA10) */
#define CLA1SIGS1_CLAIN17                 CLA_SIGS1(14)                       /*!< the input of CLA1SIGS1 is CLAIN17(PF1) */
#define CLA2SIGS1_CLAIN18                 CLA_SIGS1(14)                       /*!< the input of CLA2SIGS1 is CLAIN18(PA0) */
#define CLA3SIGS1_CLAIN16                 CLA_SIGS1(14)                       /*!< the input of CLA3SIGS1 is CLAIN16(PF0) */
#define CLA4SIGS1_CLAIN13                 CLA_SIGS1(14)                       /*!< the input of CLA4SIGS1 is CLAIN13(PA10) */
#define CLA5SIGS1_CLAIN17                 CLA_SIGS1(14)                       /*!< the input of CLA5SIGS1 is CLAIN17(PF1) */
#define CLA6SIGS1_CLAIN18                 CLA_SIGS1(14)                       /*!< the input of CLA6SIGS1 is CLAIN18(PA0) */
#define CLA7SIGS1_CLAIN16                 CLA_SIGS1(14)                       /*!< the input of CLA7SIGS1 is CLAIN16(PF0) */

#define CLA0SIGS1_CLAIN15                 CLA_SIGS1(15)                       /*!< the input of CLA0SIGS1 is CLAIN15(PA12) */
#define CLA1SIGS1_CLAIN19                 CLA_SIGS1(15)                       /*!< the input of CLA1SIGS1 is CLAIN19(PA1) */
#define CLA2SIGS1_CLAIN19                 CLA_SIGS1(15)                       /*!< the input of CLA2SIGS1 is CLAIN19(PA1) */
#define CLA3SIGS1_CLAIN17                 CLA_SIGS1(15)                       /*!< the input of CLA3SIGS1 is CLAIN17(PF1) */
#define CLA4SIGS1_CLAIN15                 CLA_SIGS1(15)                       /*!< the input of CLA4SIGS1 is CLAIN15(PA12) */
#define CLA5SIGS1_CLAIN19                 CLA_SIGS1(15)                       /*!< the input of CLA5SIGS1 is CLAIN19(PA1) */
#define CLA6SIGS1_CLAIN19                 CLA_SIGS1(15)                       /*!< the input of CLA6SIGS1 is CLAIN19(PA1) */
#define CLA7SIGS1_CLAIN17                 CLA_SIGS1(15)                       /*!< the input of CLA7SIGS1 is CLAIN17(PF1) */

#define CLA0SIGS1_CLA4_ASYNC_OUT          CLA_SIGS_SIGS1_4|CLA_SIGS1(0)       /*!< the input of CLA0SIGS1 is CLA4_ASYNC_OUT */
#define CLA1SIGS1_CLA4_ASYNC_OUT          CLA_SIGS_SIGS1_4|CLA_SIGS1(0)       /*!< the input of CLA1SIGS1 is CLA4_ASYNC_OUT */
#define CLA2SIGS1_CLA4_ASYNC_OUT          CLA_SIGS_SIGS1_4|CLA_SIGS1(0)       /*!< the input of CLA2SIGS1 is CLA4_ASYNC_OUT */
#define CLA3SIGS1_CLA4_ASYNC_OUT          CLA_SIGS_SIGS1_4|CLA_SIGS1(0)       /*!< the input of CLA3SIGS1 is CLA4_ASYNC_OUT */
#define CLA4SIGS1_CLA4_ASYNC_OUT          CLA_SIGS_SIGS1_4|CLA_SIGS1(0)       /*!< the input of CLA4SIGS1 is CLA4_ASYNC_OUT */
#define CLA5SIGS1_CLA4_ASYNC_OUT          CLA_SIGS_SIGS1_4|CLA_SIGS1(0)       /*!< the input of CLA5SIGS1 is CLA4_ASYNC_OUT */
#define CLA6SIGS1_CLA4_ASYNC_OUT          CLA_SIGS_SIGS1_4|CLA_SIGS1(0)       /*!< the input of CLA6SIGS1 is CLA4_ASYNC_OUT */
#define CLA7SIGS1_CLA4_ASYNC_OUT          CLA_SIGS_SIGS1_4|CLA_SIGS1(0)       /*!< the input of CLA7SIGS1 is CLA4_ASYNC_OUT */

#define CLA0SIGS1_CLA5_ASYNC_OUT          CLA_SIGS_SIGS1_4|CLA_SIGS1(1)       /*!< the input of CLA0SIGS1 is CLA5_ASYNC_OUT */
#define CLA1SIGS1_CLA5_ASYNC_OUT          CLA_SIGS_SIGS1_4|CLA_SIGS1(1)       /*!< the input of CLA1SIGS1 is CLA5_ASYNC_OUT */
#define CLA2SIGS1_CLA5_ASYNC_OUT          CLA_SIGS_SIGS1_4|CLA_SIGS1(1)       /*!< the input of CLA2SIGS1 is CLA5_ASYNC_OUT */
#define CLA3SIGS1_CLA5_ASYNC_OUT          CLA_SIGS_SIGS1_4|CLA_SIGS1(1)       /*!< the input of CLA3SIGS1 is CLA5_ASYNC_OUT */
#define CLA4SIGS1_CLA5_ASYNC_OUT          CLA_SIGS_SIGS1_4|CLA_SIGS1(1)       /*!< the input of CLA4SIGS1 is CLA5_ASYNC_OUT */
#define CLA5SIGS1_CLA5_ASYNC_OUT          CLA_SIGS_SIGS1_4|CLA_SIGS1(1)       /*!< the input of CLA5SIGS1 is CLA5_ASYNC_OUT */
#define CLA6SIGS1_CLA5_ASYNC_OUT          CLA_SIGS_SIGS1_4|CLA_SIGS1(1)       /*!< the input of CLA6SIGS1 is CLA5_ASYNC_OUT */
#define CLA7SIGS1_CLA5_ASYNC_OUT          CLA_SIGS_SIGS1_4|CLA_SIGS1(1)       /*!< the input of CLA7SIGS1 is CLA5_ASYNC_OUT */

#define CLA1SIGS1_CLA6_ASYNC_OUT          CLA_SIGS_SIGS1_4|CLA_SIGS1(2)       /*!< the input of CLA1SIGS1 is CLA6_ASYNC_OUT */
#define CLA2SIGS1_CLA6_ASYNC_OUT          CLA_SIGS_SIGS1_4|CLA_SIGS1(2)       /*!< the input of CLA2SIGS1 is CLA6_ASYNC_OUT */
#define CLA3SIGS1_CLA6_ASYNC_OUT          CLA_SIGS_SIGS1_4|CLA_SIGS1(2)       /*!< the input of CLA3SIGS1 is CLA6_ASYNC_OUT */
#define CLA4SIGS1_CLA6_ASYNC_OUT          CLA_SIGS_SIGS1_4|CLA_SIGS1(2)       /*!< the input of CLA4SIGS1 is CLA6_ASYNC_OUT */
#define CLA5SIGS1_CLA6_ASYNC_OUT          CLA_SIGS_SIGS1_4|CLA_SIGS1(2)       /*!< the input of CLA5SIGS1 is CLA6_ASYNC_OUT */
#define CLA6SIGS1_CLA6_ASYNC_OUT          CLA_SIGS_SIGS1_4|CLA_SIGS1(2)       /*!< the input of CLA6SIGS1 is CLA6_ASYNC_OUT */
#define CLA7SIGS1_CLA6_ASYNC_OUT          CLA_SIGS_SIGS1_4|CLA_SIGS1(2)       /*!< the input of CLA7SIGS1 is CLA6_ASYNC_OUT */

#define CLA0SIGS1_CLA7_ASYNC_OUT          CLA_SIGS_SIGS1_4|CLA_SIGS1(3)       /*!< the input of CLA0SIGS1 is CLA7_ASYNC_OUT */
#define CLA1SIGS1_CLA7_ASYNC_OUT          CLA_SIGS_SIGS1_4|CLA_SIGS1(3)       /*!< the input of CLA1SIGS1 is CLA7_ASYNC_OUT */
#define CLA2SIGS1_CLA7_ASYNC_OUT          CLA_SIGS_SIGS1_4|CLA_SIGS1(3)       /*!< the input of CLA2SIGS1 is CLA7_ASYNC_OUT */
#define CLA3SIGS1_CLA7_ASYNC_OUT          CLA_SIGS_SIGS1_4|CLA_SIGS1(3)       /*!< the input of CLA3SIGS1 is CLA7_ASYNC_OUT */
#define CLA4SIGS1_CLA7_ASYNC_OUT          CLA_SIGS_SIGS1_4|CLA_SIGS1(3)       /*!< the input of CLA4SIGS1 is CLA7_ASYNC_OUT */
#define CLA5SIGS1_CLA7_ASYNC_OUT          CLA_SIGS_SIGS1_4|CLA_SIGS1(3)       /*!< the input of CLA5SIGS1 is CLA7_ASYNC_OUT */
#define CLA6SIGS1_CLA7_ASYNC_OUT          CLA_SIGS_SIGS1_4|CLA_SIGS1(3)       /*!< the input of CLA6SIGS1 is CLA7_ASYNC_OUT */
#define CLA7SIGS1_CLA7_ASYNC_OUT          CLA_SIGS_SIGS1_4|CLA_SIGS1(3)       /*!< the input of CLA7SIGS1 is CLA7_ASYNC_OUT */

#define CLA0SIGS1_ADC1_CONV               CLA_SIGS_SIGS1_4|CLA_SIGS1(4)       /*!< the input of CLA0SIGS1 is ADC1_CONV */
#define CLA1SIGS1_ADC1_CONV               CLA_SIGS_SIGS1_4|CLA_SIGS1(4)       /*!< the input of CLA1SIGS1 is ADC1_CONV */
#define CLA2SIGS1_ADC1_CONV               CLA_SIGS_SIGS1_4|CLA_SIGS1(4)       /*!< the input of CLA2SIGS1 is ADC1_CONV */
#define CLA3SIGS1_ADC1_CONV               CLA_SIGS_SIGS1_4|CLA_SIGS1(4)       /*!< the input of CLA3SIGS1 is ADC1_CONV */
#define CLA4SIGS1_ADC1_CONV               CLA_SIGS_SIGS1_4|CLA_SIGS1(4)       /*!< the input of CLA4SIGS1 is ADC1_CONV */
#define CLA5SIGS1_ADC1_CONV               CLA_SIGS_SIGS1_4|CLA_SIGS1(4)       /*!< the input of CLA5SIGS1 is ADC1_CONV */
#define CLA6SIGS1_ADC1_CONV               CLA_SIGS_SIGS1_4|CLA_SIGS1(4)       /*!< the input of CLA6SIGS1 is ADC1_CONV */
#define CLA7SIGS1_ADC1_CONV               CLA_SIGS_SIGS1_4|CLA_SIGS1(4)       /*!< the input of CLA7SIGS1 is ADC1_CONV */

#define CLA0SIGS1_SOFTWARE                CLA_CTL_SWSIGS1EN                   /*!< the input of CLA0SIGS1 is software */
#define CLA1SIGS1_SOFTWARE                CLA_CTL_SWSIGS1EN                   /*!< the input of CLA1SIGS1 is software */
#define CLA2SIGS1_SOFTWARE                CLA_CTL_SWSIGS1EN                   /*!< the input of CLA2SIGS1 is software */
#define CLA3SIGS1_SOFTWARE                CLA_CTL_SWSIGS1EN                   /*!< the input of CLA3SIGS1 is software */
#define CLA4SIGS1_SOFTWARE                CLA_CTL_SWSIGS1EN                   /*!< the input of CLA4SIGS1 is software */
#define CLA5SIGS1_SOFTWARE                CLA_CTL_SWSIGS1EN                   /*!< the input of CLA5SIGS1 is software */
#define CLA6SIGS1_SOFTWARE                CLA_CTL_SWSIGS1EN                   /*!< the input of CLA6SIGS1 is software */
#define CLA7SIGS1_SOFTWARE                CLA_CTL_SWSIGS1EN                   /*!< the input of CLA7SIGS1 is software */

/* LCU control */
#define LCUCTL(regval)                   (BITS(0,7) & ((uint32_t)(regval) << 0))

/* CLA output selection */
#define FLIP_FLOP_OUTPUT                 ((uint32_t )0x00000000U)           /*!< flip-flop output is selected as CLAx output */
#define LCU_RESULT                       CLA_CTL_OSEL                       /*!< LCU result is selected as CLAx output */

/* flip-flop clock polarity selection */
#define CLA_CLOCKPOLARITY_POSEDGE        ((uint32_t)0x00000000U)            /*!< the clock polarity of flip-flop is posedge */
#define CLA_CLOCKPOLARITY_NEGEDGE        CLA_CTL_CPOL                       /*!< the clock polarity of flip-flop is negedge */

/* flip-flop clock source selection */
#define PRE_CLA_LCU_RESULT               ((uint32_t)0x00000000U)            /*!< the LCU result of the previous CLA units */
#define SIGS0_OUTPUT                     ((uint32_t)0x00000001U)            /*!< the multiplexer output of SIGS0 */
#define HCLK                             ((uint32_t)0x00000002U)            /*!< HCLK */
#define TIMER_TRGO                       ((uint32_t)0x00000003U)            /*!< TIMER_TRGO */

/* CLA software output */
#define SOFTWARE_OUTPUT_HIGH             ((uint8_t)0x01U)                   /*!< tmultiplexer output high by software */
#define SOFTWARE_OUTPUT_LOW              ((uint8_t)0x00U)                   /*!< tmultiplexer output low by software */

/* function declarations */
/* CLA initialization and configuration functions */
/* reset CLA */
void cla_deinit(void);
/* enable CLA */
void cla_enable(uint32_t cla_periph);
/* disable CLA */
void cla_disable(uint32_t cla_periph);
/* get CLA output state */
cla_outputstatus_enum cla_output_state_get(uint32_t cla_periph);
/* configure signal selector input */
void cla_sigs_input_config(uint32_t cla_periph, cla_sigs_enum sigs, uint32_t input);
/* configure CLA LCU control register value */
void cla_lcu_control_config(uint32_t cla_periph, uint8_t lcuctl_value);
/* configure CLA output */
void cla_output_config(uint32_t cla_periph, uint32_t output);
/* enable CLA output */
void cla_output_enable(uint32_t cla_periph);
/* disable CLA output */
void cla_output_disable(uint32_t cla_periph);
/* configure multiplexer software output */
void cla_sigs_software_output_set(uint32_t cla_periph, cla_sigs_enum sigs, uint8_t output);

/* flip-flop configuration */
/* reset the flip-flop output asynchronously */
void cla_flip_flop_output_reset(uint32_t cla_periph);
/* configure clock polarity of flip-flop */
void cla_flip_flop_clockpolarity_config(uint32_t cla_periph, uint32_t polarity);
/* configure clock source of flip-flop */
void cla_flip_flop_clocksource_config(uint32_t cla_periph, uint32_t clock_source);

/* flag and interrupt functions */
/* check CLA flag is set or not */
FlagStatus cla_flag_get(uint32_t flag);
/* clear CLA flag */
void cla_flag_clear(uint32_t flag);
/* enable CLA interrupt */
void cla_interrupt_enable(uint32_t interrupt);
/* disable CLA interrupt */
void cla_interrupt_disable(uint32_t interrupt);
/* check CLA interrupt flag is set or not */
FlagStatus cla_interrupt_flag_get(uint32_t int_flag);
/* clear CLA interrupt flag */
void cla_interrupt_flag_clear(uint32_t int_flag);

#endif /* GD32E511_512_CLA_H */

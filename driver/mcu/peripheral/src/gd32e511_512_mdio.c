/*!
    \file    gd32e511_512_mdio.c
    \brief   MDIO driver

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

#include "gd32e511_512_mdio.h"

#define MDIO_FLAG_MASK                        ((uint32_t)0x00002FFFU)  /*!< MDIO flag get mask */
#define MDIO_INT_MASK                         ((uint32_t)0x00002FFFU)  /*!< MDIO interrupt enable mask */

/*!
    \brief      reset MDIO registers (API_ID(0x0001U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void mdio_deinit(void)
{
    /* reset MDIO */
    rcu_periph_reset_enable(RCU_MDIORST);
    rcu_periph_reset_disable(RCU_MDIORST);
}

/*!
    \brief      reset MDIO block (API_ID(0x0002U))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void mdio_software_reset(void)
{
    MDIO_CTL |= MDIO_CTL_SWRST;
}

/*!
    \brief      initialize MDIO for communication (API_ID(0x0003U))
    \param[in]  phy_size: PHY bit length
                only one parameter can be selected which is shown as below:
      \arg        MDIO_PHY_BITS_3: PHY use 3 bits
      \arg        MDIO_PHY_BITS_5: PHY use 5 bits
    \param[in]  phy_softaddr: software provided PHYADR
                only one parameter can be selected which is shown as below:
      \arg        0 - 31
    \param[in]  phy_sel: PHYADR select
                only one parameter can be selected which is shown as below:
      \arg        MDIO_PHYADR_HARDWARE: sets expected PHYADR = PHYPIN[4:0]
      \arg        MDIO_PHYADR_SOFTWARE: sets expected PHYADR = PHYSW[4:0]
      \arg        other user defined value: 1 - 30
    \param[in]  devadd: device type
                only one parameter can be selected which is shown as below:
      \arg        DEVADD_PMA_PMD: device type PMA/PMD
      \arg        DEVADD_WIS: device type WIS
      \arg        DEVADD_PCS: device type PCS
      \arg        DEVADD_PHY_XS: device type PHY XS
      \arg        DEVADD_DTE_XS: device type DTE XS
    \param[out] none
    \retval     the PHYADR that the device will respond to
*/
uint32_t mdio_init(uint32_t phy_size, uint32_t phy_softaddr, uint32_t phy_sel, uint16_t devadd)
{
    uint32_t phy_communicate = 0U, phy_hard = 0U;
    /* configure MDIO phy bit length */
    MDIO_CTL &= ~MDIO_CTL_PHYB;
    MDIO_CTL |= (phy_size & MDIO_CTL_PHYB);

    /* configure the PHYADR and DEVADD */
    MDIO_CFG &= ~(MDIO_CFG_PHYSW | MDIO_CFG_EPHYSEL | MDIO_CFG_EDEVADD);
    MDIO_CFG |= (CFG_PHYSW(phy_softaddr) & MDIO_CFG_PHYSW) | \
                (CFG_EPHYSEL(phy_sel) & MDIO_CFG_EPHYSEL) | (CFG_EDEVADD(devadd)& MDIO_CFG_EDEVADD);

    /* calculate the PHYADR that the device will respond to */
    phy_hard = mdio_phy_pin_read();
    phy_communicate = (phy_hard & (~phy_sel)) | (phy_softaddr & phy_sel);

    return phy_communicate;
}

/*!
    \brief      configure MDIO phy bit length (API_ID(0x0004U))
    \param[in]  phy_bit: PHY bit length
                only one parameter can be selected which is shown as below:
      \arg        MDIO_PHY_BITS_3: PHY use 3 bits
      \arg        MDIO_PHY_BITS_5: PHY use 5 bits
    \param[out] none
    \retval     none
*/
void mdio_phy_length_config(uint32_t phy_bit)
{
    MDIO_CTL &= ~MDIO_CTL_PHYB;
    MDIO_CTL |= (phy_bit & MDIO_CTL_PHYB);
}

/*!
    \brief      set the software PHYADR value (API_ID(0x0005U))
    \param[in]  phy_soft: software provided PHYADR
                only one parameter can be selected which is shown as below:
      \arg        0 - 31
    \param[out] none
    \retval     none
*/
void mdio_soft_phyadr_set(uint32_t phy_soft)
{
    MDIO_CFG &= ~MDIO_CFG_PHYSW;
    MDIO_CFG |= (CFG_PHYSW(phy_soft) & MDIO_CFG_PHYSW);
}

/*!
    \brief      select the expected frame field PHYADR (API_ID(0x0006U))
    \param[in]  phy_sel: PHYADR select
                only one parameter can be selected which is shown as below:
      \arg        MDIO_PHYADR_HARDWARE: sets expected PHYADR = PHYPIN[4:0]
      \arg        MDIO_PHYADR_SOFTWARE: sets expected PHYADR = PHYSW[4:0]
      \arg        other user defined value: 1 - 30
    \param[out] none
    \retval     none
*/
void mdio_framefield_phyadr_config(uint32_t phy_sel)
{
    MDIO_CFG &= ~MDIO_CFG_EPHYSEL;
    MDIO_CFG |= (CFG_EPHYSEL(phy_sel)& MDIO_CFG_EPHYSEL);
}

/*!
    \brief      configure the expected frame field DEVADD (API_ID(0x0007U))
    \param[in]  type: device type
                only one parameter can be selected which is shown as below:
      \arg        DEVADD_PMA_PMD: device type PMA/PMD
      \arg        DEVADD_WIS: device type WIS
      \arg        DEVADD_PCS: device type PCS
      \arg        DEVADD_PHY_XS: device type PHY XS
      \arg        DEVADD_DTE_XS: device type DTE XS
    \param[out] none
    \retval     none
*/
void mdio_framefield_devadd_config(uint16_t type)
{
    MDIO_CFG &= ~MDIO_CFG_EDEVADD;
    MDIO_CFG |= (CFG_EDEVADD(type) & MDIO_CFG_EDEVADD);
}

/*!
    \brief      read the hardware PRTADR[4:0] value (API_ID(0x0008U))
    \param[in]  none
    \param[out] none
    \retval     PRTADR[4:0] pin value
*/
uint32_t mdio_phy_pin_read(void)
{
    return GET_PIN_PHYPIN(MDIO_PIN);
}

/*!
    \brief      configure the expected frame bit timeout (API_ID(0x0009U))
    \param[in]  timeout: timeout counter among frame bits
                only one parameter can be selected which is shown as below:
                0 - 0xFFFF
    \param[out] none
    \retval     none
*/
void mdio_timeout_config(uint16_t timeout)
{
    MDIO_TO &= ~MDIO_TO_TOCNT;
    MDIO_TO |= (TO_TOCNT(timeout) & MDIO_TO_TOCNT);
}

/*!
    \brief      enable MDIO frame bit timeout (API_ID(0x000AU))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void mdio_timeout_enable(void)
{
    MDIO_TO |= MDIO_TO_TOEN;
}

/*!
    \brief      disable MDIO frame bit timeout (API_ID(0x000BU))
    \param[in]  none
    \param[out] none
    \retval     none
*/
void mdio_timeout_disable(void)
{
    MDIO_TO &= ~MDIO_TO_TOEN;
}

/*!
    \brief      read the received frame field OP (API_ID(0x000CU))
    \param[in]  none
    \param[out] none
    \retval     OP of received
*/
uint16_t mdio_op_receive(void)
{
    return (uint16_t)(GET_RFRM_ROP(MDIO_RFRM));
}

/*!
    \brief      read the received frame field PHYADR (API_ID(0x000DU))
    \param[in]  none
    \param[out] none
    \retval     PHYADR of received
*/
uint16_t mdio_phyadr_receive(void)
{
    return (uint16_t)(GET_RFRM_RPHY(MDIO_RFRM));
}

/*!
    \brief      read the received frame field DEVADD (API_ID(0x000EU))
    \param[in]  none
    \param[out] none
    \retval     DEVADD of received
*/
uint16_t mdio_devadd_receive(void)
{
    return (uint16_t)(GET_RFRM_RDEV(MDIO_RFRM));
}

/*!
    \brief      read the received frame field TA (API_ID(0x000FU))
    \param[in]  none
    \param[out] none
    \retval     TA of received
*/
uint16_t mdio_ta_receive(void)
{
    return (uint16_t)(GET_RFRM_RTA(MDIO_RFRM));
}

/*!
    \brief      read the received frame field DATA (API_ID(0x0010U))
    \param[in]  none
    \param[out] none
    \retval     DATA of received
*/
uint16_t mdio_data_receive(void)
{
    return (uint16_t)(GET_RDATA_RDATA(MDIO_RDATA));
}

/*!
    \brief      read the received frame field ADDRESS (API_ID(0x0011U))
    \param[in]  none
    \param[out] none
    \retval     ADDRESS of received
*/
uint16_t mdio_address_receive(void)
{
    return (uint16_t)(GET_RADDR_RADDR(MDIO_RADDR));
}

/*!
    \brief      transmit the frame field DATA (API_ID(0x0012U))
    \param[in]  data: data to put in a read or post read increment address frame for transmission
                only one parameter can be selected which is shown as below:
      \arg        0 - 0xFFFF
    \param[out] none
    \retval     none
*/
void mdio_data_transmit(uint16_t data)
{
    MDIO_TDATA = (uint32_t)data;
}

/*!
    \brief      get the flag status of the frame (API_ID(0x0013U))
    \param[in]  flag:
                only one parameter can be selected which is shown as below:
      \arg        MDIO_FLAG_WRFRM: a write data frame flag status
      \arg        MDIO_FLAG_ADDRFRM: an address frame flag status
      \arg        MDIO_FLAG_RDINCFRM: a post read increment address frame flag status
      \arg        MDIO_FLAG_RDFRM: a read data frame flag status
      \arg        MDIO_FLAG_DEVM: a DEVADD match frame flag status
      \arg        MDIO_FLAG_DEVNM: a DEVADD nonmatch frame flag status
      \arg        MDIO_FLAG_PHYM: a PHYADR match frame flag status
      \arg        MDIO_FLAG_PHYNM: a PHYADR nonmatch frame flag status
      \arg        MDIO_FLAG_TANM: a TA nonmatch frame flag status
      \arg        MDIO_FLAG_TIMEOUT: timeout flag
      \arg        MDIO_FLAG_TX_UNDERRUN: transmit underrun flag
      \arg        MDIO_FLAG_RX_OVERRUN: receive overrun flag
      \arg        MDIO_FLAG_RBNE: read data buffer not empty flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus mdio_flag_get(uint32_t flag)
{
    FlagStatus temp = RESET;
    __IO uint32_t reg = 0U;
    reg = MDIO_STAT;
    if(RESET != (reg & (flag & MDIO_FLAG_MASK))) {
        temp = SET;
    } else {
        temp = RESET;
    }
    return temp;
}

/*!
    \brief      clear the flag status (API_ID(0x0014U))
    \param[in]  flag:
                one or more parameters can be selected which are shown as below:
      \arg        MDIO_FLAG_FRAME: bit[9:0] of MDIO_STAT
      \arg        MDIO_FLAG_RX_BUFFER: bit 13&11 of MDIO_STAT
      \arg        MDIO_FLAG_TX_BUFFER: bit 10 of MDIO_STAT
    \param[out] none
    \retval     none
*/
void mdio_flag_clear(uint32_t flag)
{
    __IO uint32_t reg = 0U;
    reg = MDIO_TDATA;
    if(MDIO_FLAG_FRAME == (flag & MDIO_FLAG_FRAME)) {
        /* read MDIO_STAT to clear MDIO_FLAG_FRAME */
        (void)(MDIO_STAT);
    }
    if(MDIO_FLAG_RX_BUFFER == (flag & MDIO_FLAG_RX_BUFFER)) {
        /* read MDIO_RDATA to clear MDIO_FLAG_RX_BUFFER */
        (void)(MDIO_RDATA);
    }
    if(MDIO_FLAG_TX_BUFFER == (flag & MDIO_FLAG_TX_BUFFER)) {
        /* write data back to MDIO_TDATA to clear MDIO_FLAG_TX_BUFFER,
           notice to write real data again in time to avoid wrong data transmission */
        MDIO_TDATA = reg;
    }
}

/*!
    \brief      enable MDIO interrupt (API_ID(0x0015U))
    \param[in]  interrupt:
                one or more parameters can be selected which are shown as below:
      \arg        MDIO_INT_WRFRM: a write data frame interrupt
      \arg        MDIO_INT_ADDRFRM: an address frame interrupt
      \arg        MDIO_INT_RDINCFRM: a post read increment address frame interrupt
      \arg        MDIO_INT_RDFRM: a read data frame interrupt
      \arg        MDIO_INT_DEVM: a DEVADD match frame interrupt
      \arg        MDIO_INT_DEVNM: a DEVADD nonmatch frame interrupt
      \arg        MDIO_INT_PHYM: a PHYADR match frame interrupt
      \arg        MDIO_INT_PHYNM: a PHYADR nonmatch frame interrupt
      \arg        MDIO_INT_TANM: a TA nonmatch frame flag interrupt
      \arg        MDIO_INT_TIMEOUT: a timeout interrupt
      \arg        MDIO_INT_TX_UNDERRUN: a transmit underrun interrupt
      \arg        MDIO_INT_RX_OVERRUN: a receive overrun interrupt
      \arg        MDIO_INT_RBNE: a read data buffer not empty interrupt
    \param[out] none
    \retval     none
*/
void mdio_interrupt_enable(uint32_t interrupt)
{
    MDIO_INTEN |= interrupt & MDIO_INT_MASK;
}

/*!
    \brief      disable MDIO interrupt (API_ID(0x0016U))
    \param[in]  interrupt:
                one or more parameters can be selected which are shown as below:
      \arg        MDIO_INT_WRFRM: a write data frame interrupt
      \arg        MDIO_INT_ADDRFRM: an address frame interrupt
      \arg        MDIO_INT_RDINCFRM: a post read increment address frame interrupt
      \arg        MDIO_INT_RDFRM: a read data frame interrupt
      \arg        MDIO_INT_DEVM: a DEVADD match frame interrupt
      \arg        MDIO_INT_DEVNM: a DEVADD nonmatch frame interrupt
      \arg        MDIO_INT_PHYM: a PHYADR match frame interrupt
      \arg        MDIO_INT_PHYNM: a PHYADR nonmatch frame interrupt
      \arg        MDIO_INT_TANM: a TA nonmatch frame flag interrupt
      \arg        MDIO_INT_TIMEOUT: a timeout interrupt
      \arg        MDIO_INT_TX_UNDERRUN: a transmit underrun interrupt
      \arg        MDIO_INT_RX_OVERRUN: a receive overrun interrupt
      \arg        MDIO_INT_RBNE: a read data buffer not empty interrupt
    \param[out] none
    \retval     none
*/
void mdio_interrupt_disable(uint32_t interrupt)
{
    MDIO_INTEN &= ~(interrupt & MDIO_INT_MASK);
}

/*!
    \brief      get MDIO interrupt and flag status (API_ID: 0x0017U)
    \param[in]  int_flag: interrupt and flag type
                only one parameter can be selected which is shown as below:
      \arg        MDIO_INT_FLAG_WRFRM: a write data frame interrupt and flag
      \arg        MDIO_INT_FLAG_ADDRFRM: an address frame interrupt and flag
      \arg        MDIO_INT_FLAG_RDINCFRM: a post read increment address frame interrupt and flag
      \arg        MDIO_INT_FLAG_RDFRM: a read data frame interrupt and flag
      \arg        MDIO_INT_FLAG_DEVM: a DEVADD match frame interrupt and flag
      \arg        MDIO_INT_FLAG_DEVNM: a DEVADD nonmatch frame interrupt and flag
      \arg        MDIO_INT_FLAG_PHYM: a PHYADR match frame interrupt and flag
      \arg        MDIO_INT_FLAG_PHYNM: a PHYADR nonmatch frame interrupt and flag
      \arg        MDIO_INT_FLAG_TANM: a TA nonmatch frame interrupt and flag
      \arg        MDIO_INT_FLAG_TIMEOUT: a timeout interrupt and flag
      \arg        MDIO_INT_FLAG_TX_UNDERRUN: a transmit underrun interrupt and flag
      \arg        MDIO_INT_FLAG_RX_OVERRUN: a receive overrun interrupt and flag
      \arg        MDIO_INT_FLAG_RBNE: a read data buffer not empty interrupt and flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus mdio_interrupt_flag_get(uint32_t int_flag)
{
    uint32_t intenable = 0U, flagstatus = 0U;
    FlagStatus status = RESET;
    /* get the corresponding flag bit status and interrupt enable bit status */
    switch(int_flag) {
    case MDIO_INT_FLAG_WRFRM:
        flagstatus = (MDIO_STAT & MDIO_FLAG_WRFRM);
        intenable = (MDIO_INTEN & MDIO_INT_WRFRM);
        break;
    case MDIO_INT_FLAG_ADDRFRM:
        flagstatus = (MDIO_STAT & MDIO_FLAG_ADDRFRM);
        intenable = (MDIO_INTEN & MDIO_INT_ADDRFRM);
        break;
    case MDIO_INT_FLAG_RDINCFRM:
        flagstatus = (MDIO_STAT & MDIO_FLAG_RDINCFRM);
        intenable = (MDIO_INTEN & MDIO_INT_RDINCFRM);
        break;
    case MDIO_INT_FLAG_RDFRM:
        flagstatus = (MDIO_STAT & MDIO_FLAG_RDFRM);
        intenable = (MDIO_INTEN & MDIO_INT_RDFRM);
        break;
    case MDIO_INT_FLAG_DEVM:
        flagstatus = (MDIO_STAT & MDIO_FLAG_DEVM);
        intenable = (MDIO_INTEN & MDIO_INT_DEVM);
        break;
    case MDIO_INT_FLAG_DEVNM:
        flagstatus = (MDIO_STAT & MDIO_FLAG_DEVNM);
        intenable = (MDIO_INTEN & MDIO_INT_DEVNM);
        break;
    case MDIO_INT_FLAG_PHYM:
        flagstatus = (MDIO_STAT & MDIO_FLAG_PHYM);
        intenable = (MDIO_INTEN & MDIO_INT_PHYM);
        break;
    case MDIO_INT_FLAG_PHYNM:
        flagstatus = (MDIO_STAT & MDIO_FLAG_PHYNM);
        intenable = (MDIO_INTEN & MDIO_INT_PHYNM);
        break;
    case MDIO_INT_FLAG_TANM:
        flagstatus = (MDIO_STAT & MDIO_FLAG_TANM);
        intenable = (MDIO_INTEN & MDIO_INT_TANM);
        break;
    case MDIO_INT_FLAG_TIMEOUT:
        flagstatus = (MDIO_STAT & MDIO_FLAG_TIMEOUT);
        intenable = (MDIO_INTEN & MDIO_INT_TIMEOUT);
        break;
    case MDIO_INT_FLAG_TX_UNDERRUN:
        flagstatus = (MDIO_STAT & MDIO_FLAG_TX_UNDERRUN);
        intenable = (MDIO_INTEN & MDIO_INT_TX_UNDERRUN);
        break;
    case MDIO_INT_FLAG_RX_OVERRUN:
        flagstatus = (MDIO_STAT & MDIO_FLAG_RX_OVERRUN);
        intenable = (MDIO_INTEN & MDIO_INT_RX_OVERRUN);
        break;
    case MDIO_INT_FLAG_RBNE:
        flagstatus = (MDIO_STAT & MDIO_FLAG_RBNE);
        intenable = (MDIO_INTEN & MDIO_INT_RBNE);
        break;
    default:
        flagstatus = 0U;
        break;
    }

    if(flagstatus && intenable) {
        status = SET;
    } else {
        status = RESET;
    }
    return status;
}

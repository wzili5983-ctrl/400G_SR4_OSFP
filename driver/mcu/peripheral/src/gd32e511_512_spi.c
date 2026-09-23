/*!
    \file    gd32e511_512_spi.c
    \brief   SPI driver

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

#include "gd32e511_512_spi.h"

/* SPI parameter initialization mask */
#define SPI_INIT_MASK                        ((uint32_t)0x00003040U)  /*!< SPI0/SPI2 parameter initialization mask */
#define SPI_FIFO_INIT_MASK1                  ((uint32_t)0x00003840U)  /*!< SPI1/SPI3 parameter initialization mask1 */
#define SPI_FIFO_INIT_MASK2                  ((uint32_t)0x0000F0FFU)  /*!< SPI1/SPI3 parameter initialization mask2*/

#define SPI_BYTEN_MASK                       ((uint32_t)0x00001000U)  /*!< SPI1/SPI3 access to FIFO mask */
#define SPI_TXLVL_MASK                       ((uint32_t)0x00001800U)  /*!< SPI1/SPI3 TXFIFO mask */
#define SPI_RXLVL_MASK                       ((uint32_t)0x00000600U)  /*!< SPI1/SPI3 RXFIFO mask */

#define SPI_TRANS_MODE_MASK                  ((uint32_t)0x0000C400U)  /*!< SPI transfer mode mask */
#define SPI_INT_MASK                         ((uint32_t)0x000100E0U)  /*!< SPI interrupt enable mask */

/*!
    \brief      reset SPI (API_ID(0x0001U))
    \param[in]  spi_periph: SPIx(x=0,1,2,3)
    \param[out] none
    \retval     none
*/
void spi_deinit(uint32_t spi_periph)
{
    switch(spi_periph) {
    case SPI0:
        /* reset SPI0 */
        rcu_periph_reset_enable(RCU_SPI0RST);
        rcu_periph_reset_disable(RCU_SPI0RST);
        break;
    case SPI1:
        /* reset SPI1 */
        rcu_periph_reset_enable(RCU_SPI1RST);
        rcu_periph_reset_disable(RCU_SPI1RST);
        break;
    case SPI2:
        /* reset SPI2 */
        rcu_periph_reset_enable(RCU_SPI2RST);
        rcu_periph_reset_disable(RCU_SPI2RST);
        break;
    case SPI3:
        /* reset SPI3 */
        rcu_periph_reset_enable(RCU_SPI3RST);
        rcu_periph_reset_disable(RCU_SPI3RST);
        break;
    default:
        break;
    }
}

/*!
    \brief      initialize the parameters of SPI structure with the default values (API_ID(0x0002U))
    \param[in]  spi_struct: the initialized structure spi_parameter_struct pointer
    \param[out] none
    \retval     none
*/
void spi_struct_para_init(spi_parameter_struct *spi_struct)
{
#ifdef FW_DEBUG_ERR_REPORT
    /* check parameter */
    if(NOT_VALID_POINTER(spi_struct)) {
        fw_debug_report_err(SPI_MODULE_ID, API_ID(0x0002U), ERR_PARAM_POINTER);
    } else
#endif /* FW_DEBUG_ERR_REPORT */
    {
        /* configure the SPI structure with the default values */
        spi_struct->device_mode          = SPI_SLAVE;
        spi_struct->trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
        spi_struct->frame_size           = SPI_FRAMESIZE_8BIT;
        spi_struct->nss                  = SPI_NSS_HARD;
        spi_struct->endian               = SPI_ENDIAN_MSB;
        spi_struct->clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
        spi_struct->prescale             = SPI_PSC_2;
    }
}

/*!
    \brief      initialize SPI parameter (API_ID(0x0003U))
    \param[in]  spi_periph: SPIx(x=0,1,2,3)
    \param[in]  spi_struct: SPI parameter initialization struct members of the structure
                            and the member values are shown as below:
                  device_mode: SPI_MASTER, SPI_SLAVE
                  trans_mode: SPI_TRANSMODE_FULLDUPLEX, SPI_TRANSMODE_RECEIVEONLY,
                              SPI_TRANSMODE_BDRECEIVE, SPI_TRANSMODE_BDTRANSMIT
                  frame_size: SPI_FRAMESIZE_xBIT(x=4,5..16, for SPI1 and SPI3, x=8,16, for SPI0 and SPI2)
                  nss: SPI_NSS_SOFT, SPI_NSS_HARD
                  endian: SPI_ENDIAN_MSB, SPI_ENDIAN_LSB
                  clock_polarity_phase: SPI_CK_PL_LOW_PH_1EDGE, SPI_CK_PL_HIGH_PH_1EDGE
                                        SPI_CK_PL_LOW_PH_2EDGE, SPI_CK_PL_HIGH_PH_2EDGE
                  prescale: SPI_PSC_n (n=2,4,8,16,32,64,128,256)
    \param[out] none
    \retval     ErrStatus: ERROR or SUCCESS
*/
ErrStatus spi_init(uint32_t spi_periph, const spi_parameter_struct *spi_struct)
{
    ErrStatus reval = SUCCESS;

#ifdef FW_DEBUG_ERR_REPORT
    /* check parameter */
    if(NOT_VALID_POINTER(spi_struct)) {
        fw_debug_report_err(SPI_MODULE_ID, API_ID(0x0003U), ERR_PARAM_POINTER);
    } else
#endif /* FW_DEBUG_ERR_REPORT */
    {
        uint32_t reg1, reg2, reg3;

        reg1 = SPI_CTL0(spi_periph);
        reg1 &= SPI_INIT_MASK;

        reg2 = SPI_CTL0(spi_periph);
        reg2 &= SPI_FIFO_INIT_MASK1;

        reg3 = SPI_CTL1(spi_periph);
        reg3 &= SPI_FIFO_INIT_MASK2;

        if((SPI0 == spi_periph) || (SPI2 == spi_periph)) {
            /* select SPI as master or slave */
            reg1 |= spi_struct->device_mode & SPI_MASTER;
            /* select SPI transfer mode */
            reg1 |= spi_struct->trans_mode & SPI_TRANS_MODE_MASK;
            /* select SPI NSS use hardware or software */
            reg1 |= spi_struct->nss & SPI_NSS_SOFT;
            /* select SPI LSB or MSB */
            reg1 |= spi_struct->endian & SPI_ENDIAN_LSB;
            /* select SPI polarity and phase */
            reg1 |= spi_struct->clock_polarity_phase & SPI_CK_PL_HIGH_PH_2EDGE;
            /* select SPI prescaler to adjust transmit speed */
            reg1 |= spi_struct->prescale & SPI_PSC_256;
            /* select SPI frame size */
            /* check SPI0 frame size is 8bits/16bits or not*/
            if((SPI_FRAMESIZE_8BIT != spi_struct->frame_size) && (SPI_FRAMESIZE_16BIT != spi_struct->frame_size)) {
                reval = ERROR;
            } else {
                reg1 |= (uint32_t)((spi_struct->frame_size) & SPI_CTL0_FF16);
            }

            /* write to SPI_CTL0 register */
            SPI_CTL0(spi_periph) = (uint32_t)reg1;

        } else {
            /* select SPI as master or slave */
            reg2 |= spi_struct->device_mode & SPI_MASTER;
            /* select SPI transfer mode */
            reg2 |= spi_struct->trans_mode & SPI_TRANS_MODE_MASK;
            /* select SPI NSS use hardware or software */
            reg2 |= spi_struct->nss & SPI_NSS_SOFT;
            /* select SPI LSB or MSB */
            reg2 |= spi_struct->endian & SPI_ENDIAN_LSB;
            /* select SPI polarity and phase */
            reg2 |= spi_struct->clock_polarity_phase & SPI_CK_PL_HIGH_PH_2EDGE;
            /* select SPI prescaler to adjust transmit speed */
            reg2 |= spi_struct->prescale & SPI_PSC_256;
            /* write to SPI_CTL0 register */
            SPI_CTL0(spi_periph) = (uint32_t)reg2;

            /* select SPI data size */
            reg3 |= ((uint32_t)(spi_struct->frame_size) & SPI_CTL1_DZ);
            /* write to SPI_CTL0 register */
            SPI_CTL1(spi_periph) = (uint32_t)reg3;
        }
    }
    return reval;
}

/*!
    \brief      enable SPI (API_ID(0x0004U))
    \param[in]  spi_periph: SPIx(x=0,1,2,3)
    \param[out] none
    \retval     none
*/
void spi_enable(uint32_t spi_periph)
{
    SPI_CTL0(spi_periph) |= SPI_CTL0_SPIEN;
}

/*!
    \brief      disable SPI (API_ID(0x0005U))
    \param[in]  spi_periph: SPIx(x=0,1,2,3)
    \param[out] none
    \retval     none
*/
void spi_disable(uint32_t spi_periph)
{
    SPI_CTL0(spi_periph) &= ~SPI_CTL0_SPIEN;
}

/*!
    \brief      enable SPI NSS output (API_ID(0x0006U))
    \param[in]  spi_periph: SPIx(x=0,1,2,3)
    \param[out] none
    \retval     none
*/
void spi_nss_output_enable(uint32_t spi_periph)
{
    SPI_CTL1(spi_periph) |= SPI_CTL1_NSSDRV;
}

/*!
    \brief      disable SPI NSS output (API_ID(0x0007U))
    \param[in]  spi_periph: SPIx(x=0,1,2,3)
    \param[out] none
    \retval     none
*/
void spi_nss_output_disable(uint32_t spi_periph)
{
    SPI_CTL1(spi_periph) &= ~SPI_CTL1_NSSDRV;
}

/*!
    \brief      SPI NSS pin high level in software mode (API_ID(0x0008U))
    \param[in]  spi_periph: SPIx(x=0,1,2,3)
    \param[out] none
    \retval     none
*/
void spi_nss_internal_high(uint32_t spi_periph)
{
    SPI_CTL0(spi_periph) |= SPI_CTL0_SWNSS;
}

/*!
    \brief      SPI NSS pin low level in software mode (API_ID(0x0009U))
    \param[in]  spi_periph: SPIx(x=0,1,2,3)
    \param[out] none
    \retval     none
*/
void spi_nss_internal_low(uint32_t spi_periph)
{
    SPI_CTL0(spi_periph) &= ~SPI_CTL0_SWNSS;
}

/*!
    \brief      enable SPI DMA (API_ID(0x000AU))
    \param[in]  spi_periph: SPIx(x=0,1,2,3)
    \param[in]  dma: SPI DMA mode
                only one parameter can be selected which is shown as below:
      \arg        SPI_DMA_TRANSMIT: SPI transmit data using DMA
      \arg        SPI_DMA_RECEIVE: SPI receive data using DMA
    \param[out] none
    \retval     none
*/
void spi_dma_enable(uint32_t spi_periph, uint8_t dma)
{
    if(SPI_DMA_TRANSMIT == dma) {
        SPI_CTL1(spi_periph) |= SPI_CTL1_DMATEN;
    } else {
        SPI_CTL1(spi_periph) |= SPI_CTL1_DMAREN;
    }
}

/*!
    \brief      disable SPI DMA (API_ID(0x000BU))
    \param[in]  spi_periph: SPIx(x=0,1,2,3)
    \param[in]  dma: SPI DMA mode
                only one parameter can be selected which is shown as below:
      \arg        SPI_DMA_TRANSMIT: SPI transmit data using DMA
      \arg        SPI_DMA_RECEIVE: SPI receive data using DMA
    \param[out] none
    \retval     none
*/
void spi_dma_disable(uint32_t spi_periph, uint8_t dma)
{
    if(SPI_DMA_TRANSMIT == dma) {
        SPI_CTL1(spi_periph) &= ~SPI_CTL1_DMATEN;
    } else {
        SPI_CTL1(spi_periph) &= ~SPI_CTL1_DMAREN;
    }
}

/*!
    \brief      configure SPI total number of data to transmit by DMA is odd or not (API_ID(0x000CU))
    \param[in]  spi_periph: SPIx(x=1,3)
    \param[in]  odd: odd bytes in TX DMA channel
                only one parameter can be selected which is shown as below:
      \arg        SPI_TXDMA_EVEN: number of byte in TX DMA channel is even
      \arg        SPI_TXDMA_ODD: number of byte in TX DMA channel is odd
    \param[out] none
    \retval     none
*/
void spi_transmit_odd_config(uint32_t spi_periph, uint16_t odd)
{
    /* clear SPI_CTL1_TXDMA_ODD bit */
    SPI_CTL1(spi_periph) &= ~SPI_CTL1_TXDMA_ODD;
    /* configure SPI_CTL1_TXDMA_ODD bit */
    SPI_CTL1(spi_periph) |= ((uint32_t)odd) & SPI_CTL1_TXDMA_ODD;
}

/*!
    \brief      configure SPI total number of data to receive by DMA is odd or not (API_ID(0x000DU))
    \param[in]  spi_periph: SPIx(x=1,3)
    \param[in]  odd: odd bytes in RX DMA channel
                only one parameter can be selected which is shown as below:
      \arg        SPI_RXDMA_EVEN: number of bytes in RX DMA channel is even
      \arg        SPI_RXDMA_ODD: number of bytes in RX DMA channel is odd
    \param[out] none
    \retval     none
*/
void spi_receive_odd_config(uint32_t spi_periph, uint16_t odd)
{
    /* clear SPI_CTL1_RXDMA_ODD bit */
    SPI_CTL1(spi_periph) &= ~SPI_CTL1_RXDMA_ODD;
    /* configure SPI_CTL1_RXDMA_ODD bit */
    SPI_CTL1(spi_periph) |= ((uint32_t)odd) & SPI_CTL1_RXDMA_ODD;
}

/*!
    \brief      configure SPI data frame format (API_ID(0x000EU))
    \param[in]  spi_periph: SPIx(x=0,1,2,3)
    \param[in]  frame_format: SPI frame size
                only one parameter can be selected which is shown as below:
      \arg         SPI_FRAMESIZE_xBIT(x=4,5..16, for SPI1 and SPI3, x=8,16, for SPI0 and SPI2):SPI frame size is x bits
    \param[out] none
    \retval     ErrStatus: ERROR or SUCCESS
*/
ErrStatus spi_data_frame_format_config(uint32_t spi_periph, uint16_t frame_format)
{
    ErrStatus reval = SUCCESS;
    uint32_t reg;

    if((SPI0 == spi_periph) || (SPI2 == spi_periph)) {
        /* check SPI0 frame size is 8bits/16bits or not*/
        if((SPI_FRAMESIZE_8BIT != frame_format) && (SPI_FRAMESIZE_16BIT != frame_format)) {
            reval = ERROR;
        } else {
            /* clear SPI_CTL0_FF16 bit */
            SPI_CTL0(spi_periph) &= ~SPI_CTL0_FF16;
            /* configure SPI_CTL0_FF16 bit */
            SPI_CTL0(spi_periph) |= ((uint32_t)frame_format & SPI_CTL0_FF16);
        }
    } else {
        reg = SPI_CTL1(spi_periph);
        /* clear SPI_CTL1_DZ bits */
        reg &= ~SPI_CTL1_DZ;
        reg |= ((uint32_t)frame_format) & SPI_CTL1_DZ;
        /* configure SPI_CTL1_DZ bits */
        SPI_CTL1(spi_periph) = reg;
    }

    return reval;
}

/*!
    \brief      configure SPI access size to FIFO (8-bit or 16-bit) (API_ID(0x000FU))
    \param[in]  spi_periph: SPIx(x=1,3)
    \param[in]  fifo_access_size: byte access enable
                only one parameter can be selected which is shown as below:
      \arg        SPI_HALFWORD_ACCESS: half-word access to FIFO
      \arg        SPI_BYTE_ACCESS: byte access to FIFO
    \param[out] none
    \retval     none
*/
void spi_fifo_access_size_config(uint32_t spi_periph, uint16_t fifo_access_size)
{
    /* clear SPI_CTL1_BYTEN bit */
    SPI_CTL1(spi_periph) &= ~SPI_CTL1_BYTEN;
    /* configure SPI_CTL1_BYTEN bit */
    SPI_CTL1(spi_periph) |= ((uint32_t)fifo_access_size) & SPI_CTL1_BYTEN;
}

/*!
    \brief      configure SPI bidirectional transfer direction (API_ID(0x0010U))
    \param[in]  spi_periph: SPIx(x=0,1,2,3)
    \param[in]  transfer_direction: SPI transfer direction
                only one parameter can be selected which is shown as below:
      \arg        SPI_BIDIRECTIONAL_TRANSMIT: SPI work in transmit-only mode
      \arg        SPI_BIDIRECTIONAL_RECEIVE: SPI work in receive-only mode
    \param[out] none
    \retval     none
*/
void spi_bidirectional_transfer_config(uint32_t spi_periph, uint32_t transfer_direction)
{
    /* clear SPI_CTL0_BDOEN bit */
    SPI_CTL0(spi_periph) &= ~SPI_CTL0_BDOEN;
    /* configure SPI_CTL0_BDOEN bit */
    SPI_CTL0(spi_periph) |= transfer_direction & SPI_CTL0_BDOEN;
}

/*!
    \brief      SPI transmit data (API_ID(0x0011U))
    \param[in]  spi_periph: SPIx(x=0,1,2,3)
    \param[in]  data: data to be transmitted
    \param[out] none
    \retval     none
*/
void spi_data_transmit(uint32_t spi_periph, uint16_t data)
{
    uint32_t reg, byten;

    if((SPI0 == spi_periph) || (SPI2 == spi_periph)) {
        SPI_DATA(spi_periph) = (uint32_t)data;
    } else {
        /* get the access size to FIFO */
        byten = SPI_CTL1(spi_periph) & SPI_BYTEN_MASK;
        if(RESET != byten) {
            reg = spi_periph + 0x0CU;
            *(uint8_t *)(reg) = (uint8_t)data;
        } else {
            SPI_DATA(spi_periph) = (uint16_t)data;
        }
    }
}

/*!
    \brief      SPI receive data (API_ID(0x0012U))
    \param[in]  spi_periph: SPIx(x=0,1,2,3)
    \param[out] none
    \retval     16-bit data
*/
uint16_t spi_data_receive(uint32_t spi_periph)
{
    uint16_t reval = 0U;
    uint32_t reg, byten;

    if((SPI0 == spi_periph) || (SPI2 == spi_periph)) {
        reval = ((uint16_t)SPI_DATA(spi_periph));
    } else {
        /* get the access size to FIFO */
        byten = SPI_CTL1(spi_periph) & SPI_BYTEN_MASK;
        if(RESET != byten) {
            reg = spi_periph + 0x0CU;
            reval = (uint16_t)(*(uint8_t *)(reg));
        } else {
            reval = ((uint16_t)SPI_DATA(spi_periph));
        }
    }

    return reval;
}

/*!
    \brief      clear SPI format error flag status (API_ID(0x0013U))
    \param[in]  spi_periph: SPIx(x=0,1,2,3)
    \param[in]  flag: SPI frame format error flag
      \arg        SPI_FLAG_FERR: only for SPI work in TI mode
    \param[out] none
    \retval     none
*/
void spi_format_error_clear(uint32_t spi_periph, uint32_t flag)
{
    SPI_STAT(spi_periph) = ~(flag & SPI_STAT_FERR);
}

/*!
    \brief      set SPI CRC polynomial (API_ID(0x0014U))
    \param[in]  spi_periph: SPIx(x=0,1,2,3)
    \param[in]  crc_poly: CRC polynomial value
    \param[out] none
    \retval     none
*/
void spi_crc_polynomial_set(uint32_t spi_periph, uint16_t crc_poly)
{
    /* set SPI CRC polynomial */
    SPI_CRCPOLY(spi_periph) = ((uint32_t)crc_poly & SPI_CRCPOLY_CRCPOLY);
}

/*!
    \brief      get SPI CRC polynomial (API_ID(0x0015U))
    \param[in]  spi_periph: SPIx(x=0,1,2,3)
    \param[out] none
    \retval     16-bit CRC polynomial
*/
uint16_t spi_crc_polynomial_get(uint32_t spi_periph)
{
    return ((uint16_t)SPI_CRCPOLY(spi_periph));
}

/*!
    \brief      set CRC length (API_ID(0x0016U))
    \param[in]  spi_periph: SPIx(x=1,3)
    \param[in]  crc_length: CRC length
                only one parameter can be selected which is shown as below:
      \arg        SPI_CRC_8BIT: CRC length is 8 bits
      \arg        SPI_CRC_16BIT: CRC length is 16 bits
    \param[out] none
    \retval     none
*/
void spi_crc_length_set(uint32_t spi_periph, uint16_t crc_length)
{
    /* clear SPI_CTL0_CRCL bit */
    SPI_CTL0(spi_periph) &= ~SPI_CTL0_CRCL;
    /* configure SPI_CTL0_CRCL bit */
    SPI_CTL0(spi_periph) |= ((uint32_t)crc_length) & SPI_CTL0_CRCL;
}

/*!
    \brief      turn on SPI CRC function (API_ID(0x0017U))
    \param[in]  spi_periph: SPIx(x=0,1,2,3)
    \param[out] none
    \retval     none
*/
void spi_crc_on(uint32_t spi_periph)
{
    SPI_CTL0(spi_periph) |= SPI_CTL0_CRCEN;
}

/*!
    \brief      turn off SPI CRC function (API_ID(0x0018U))
    \param[in]  spi_periph: SPIx(x=0,1,2,3)
    \param[out] none
    \retval     none
*/
void spi_crc_off(uint32_t spi_periph)
{
    SPI_CTL0(spi_periph) &= ~SPI_CTL0_CRCEN;
}

/*!
    \brief      SPI next data is CRC value (API_ID(0x0019U))
    \param[in]  spi_periph: SPIx(x=0,1,2,3)
    \param[out] none
    \retval     none
*/
void spi_crc_next(uint32_t spi_periph)
{
    SPI_CTL0(spi_periph) |= SPI_CTL0_CRCNT;
}

/*!
    \brief      get SPI CRC send value or receive value (API_ID(0x001AU))
    \param[in]  spi_periph: SPIx(x=0,1,2,3)
    \param[in]  crc: SPI crc value
                only one parameter can be selected which is shown as below:
      \arg        SPI_CRC_TX: get transmit crc value
      \arg        SPI_CRC_RX: get receive crc value
    \param[out] none
    \retval     16-bit CRC value
*/
uint16_t spi_crc_get(uint32_t spi_periph, uint8_t crc)
{
    uint16_t reval = 0U;

    if(SPI_CRC_TX == crc) {
        reval = ((uint16_t)(SPI_TCRC(spi_periph)));
    } else {
        reval = ((uint16_t)(SPI_RCRC(spi_periph)));
    }
    return reval;
}

/*!
    \brief      clear SPI CRC error flag status (API_ID(0x001BU))
    \param[in]  spi_periph: SPIx(x=0,1,2,3)
    \param[out] none
    \retval     none
*/
void spi_crc_error_clear(uint32_t spi_periph)
{
    SPI_STAT(spi_periph) = ~SPI_FLAG_CRCERR;
}

/*!
    \brief      enable SPI TI mode (API_ID(0x001CU))
    \param[in]  spi_periph: SPIx(x=0,1,2,3)
    \param[out] none
    \retval     none
*/
void spi_ti_mode_enable(uint32_t spi_periph)
{
    SPI_CTL1(spi_periph) |= SPI_CTL1_TMOD;
}

/*!
    \brief      disable SPI TI mode (API_ID(0x001DU))
    \param[in]  spi_periph: SPIx(x=0,1,2,3)
    \param[out] none
    \retval     none
*/
void spi_ti_mode_disable(uint32_t spi_periph)
{
    SPI_CTL1(spi_periph) &= ~SPI_CTL1_TMOD;
}

/*!
    \brief      enable SPI NSS pulse mode (API_ID(0x001EU))
    \param[in]  spi_periph: SPIx(x=0,1,2,3)
    \param[out] none
    \retval     none
*/
void spi_nssp_mode_enable(uint32_t spi_periph)
{
    SPI_CTL1(spi_periph) |= SPI_CTL1_NSSP;
}

/*!
    \brief      disable SPI NSS pulse mode (API_ID(0x001FU))
    \param[in]  spi_periph: SPIx(x=0,1,2,3)
    \param[out] none
    \retval     none
*/
void spi_nssp_mode_disable(uint32_t spi_periph)
{
    SPI_CTL1(spi_periph) &= ~SPI_CTL1_NSSP;
}

/*!
    \brief      config SPI overlap lenth (API_ID(0x0020U))
    \param[in]  spi_periph: SPIx(x=1,3)
    \param[in]  cmd_length: lenth of command
    \param[in]  sck_length: lenth of sck
    \param[out] none
    \retval     none
*/
void spi_overlap_length_set(uint32_t spi_periph, uint32_t cmd_length, uint32_t sck_length)
{
    /* clear SPI_CTL0_CKLEN and SPI_CTL0_CMDLEN bits */
    SPI_CTL0(spi_periph) &= ~(SPI_CTL0_OVLP_CK_LEN | SPI_CTL0_OVLP_CMD_LEN);
    /* configure SPI_CTL0_CKLEN and SPI_CTL0_CMDLEN bits */
    SPI_CTL0(spi_periph) |= ((cmd_length << 22U) & SPI_CTL0_OVLP_CMD_LEN) | ((sck_length << 16U) & SPI_CTL0_OVLP_CK_LEN);
}

/*!
    \brief      enable overlap mode (API_ID(0x0021U))
    \param[in]  spi_periph: SPIx(x=1,3)
    \param[out] none
    \retval     none
*/
void spi_overlap_enable(uint32_t spi_periph)
{
    SPI_CTL1(spi_periph) |= SPI_CTL1_OVLPEN;
}

/*!
    \brief      disable overlap mode (API_ID(0x0022U))
    \param[in]  spi_periph: SPIx(x=1,3)
    \param[out] none
    \retval     none
*/
void spi_overlap_disable(uint32_t spi_periph)
{
    SPI_CTL1(spi_periph) &= ~SPI_CTL1_OVLPEN;
}

/*!
    \brief      get received data(sent from the slave to the master) of overlap mode (API_ID(0x0023U))
    \param[in]  spi_periph: SPIx(x=1,3)
    \param[out] none
    \retval     16-bit data of overlap mode
*/
uint16_t spi_overlap_data_get(uint32_t spi_periph)
{
    return ((uint16_t)SPI_OVLPDATA0(spi_periph));
}


/*!
    \brief      enable SPI quad wire mode (API_ID(0x0024U))
    \param[in]  spi_periph: SPIx(x=1,3)
    \param[out] none
    \retval     none
*/
void spi_quad_enable(uint32_t spi_periph)
{
    SPI_QCTL(spi_periph) |= SPI_QCTL_QMOD;
}

/*!
    \brief      disable SPI quad wire mode (API_ID(0x0025U))
    \param[in]  spi_periph: SPIx(x=1,3)
    \param[out] none
    \retval     none
*/
void spi_quad_disable(uint32_t spi_periph)
{
    SPI_QCTL(spi_periph) &= ~SPI_QCTL_QMOD;
}

/*!
    \brief      enable SPI quad wire mode write (API_ID(0x0026U))
    \param[in]  spi_periph: SPIx(x=1,3)
    \param[out] none
    \retval     none
*/
void spi_quad_write_enable(uint32_t spi_periph)
{
    SPI_QCTL(spi_periph) &= ~SPI_QCTL_QRD;
}

/*!
    \brief      enable SPI quad wire mode read (API_ID(0x0027U))
    \param[in]  spi_periph: SPIx(x=1,3)
    \param[out] none
    \retval     none
*/
void spi_quad_read_enable(uint32_t spi_periph)
{
    SPI_QCTL(spi_periph) |= SPI_QCTL_QRD;
}

/*!
    \brief      get SPI and flag status (API_ID(0x002AU))
    \param[in]  spi_periph: SPIx(x=0,1,2,3)
    \param[in]  flag: SPI flag status
                only one parameter can be selected which are shown as below:
      \arg        SPI_FLAG_TBE: transmit buffer empty flag
      \arg        SPI_FLAG_RBNE: receive buffer not empty flag
      \arg        SPI_FLAG_TRANS: transmit on-going flag
      \arg        SPI_FLAG_RXORERR: receive overrun error flag
      \arg        SPI_FLAG_CONFERR: mode config error flag
      \arg        SPI_FLAG_CRCERR: CRC error flag
      \arg        SPI_FLAG_FERR: SPI format error interrupt flag
                only for SPI1 and SPI3:
      \arg        SPI_FLAG_TXLVL_EMPTY: SPI TXFIFO is empty
      \arg        SPI_FLAG_TXLVL_QUARTER_FULL: SPI TXFIFO is a quarter of full
      \arg        SPI_FLAG_TXLVL_HALF_FULL: SPI TXFIFO is a half of full
      \arg        SPI_FLAG_TXLVL_FULL: SPI TXFIFO is full
      \arg        SPI_FLAG_RXLVL_EMPTY: SPI RXFIFO is empty
      \arg        SPI_FLAG_RXLVL_QUARTER_FULL: SPI RXFIFO is a quarter of full
      \arg        SPI_FLAG_RXLVL_HALF_FULL: SPI RXFIFO is a half of full
      \arg        SPI_FLAG_RXLVL_FULL: SPI RXFIFO is full
      \arg        SPI_FLAG_OVLP_RBNE: overlap data not empty flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus spi_flag_get(uint32_t spi_periph, uint32_t flag)
{
    FlagStatus reval = RESET;
    uint32_t reg = SPI_STAT(spi_periph);

    switch(flag) {
    case SPI_FLAG_TXLVL_EMPTY:
        if(0U == (reg & SPI_TXLVL_MASK)) {
            reval = SET;
        }
        break;
    case SPI_FLAG_TXLVL_QUARTER_FULL:
    case SPI_FLAG_TXLVL_HALF_FULL:
    case SPI_FLAG_TXLVL_FULL:
        if(flag == (reg & SPI_TXLVL_MASK)) {
            reval = SET;
        }
        break;
    case SPI_FLAG_RXLVL_EMPTY:
        if(0U == (reg & SPI_RXLVL_MASK)) {
            reval = SET;
        }
        break;
    case SPI_FLAG_RXLVL_QUARTER_FULL:
    case SPI_FLAG_RXLVL_HALF_FULL:
    case SPI_FLAG_RXLVL_FULL:
        if(flag == (reg & SPI_RXLVL_MASK)) {
            reval = SET;
        }
        break;
    default:
        if(0U != (reg & flag)) {
            reval = SET;
        }
        break;
    }

    return reval;
}

/*!
    \brief      enable SPI interrupt (API_ID(0x002BU))
    \param[in]  spi_periph: SPIx(x=0,1,2,3)
    \param[in]  interrupt: SPI interrupt
                only one parameter can be selected which is shown as below:
      \arg        SPI_INT_TBE: transmit buffer empty interrupt
      \arg        SPI_INT_RBNE: receive buffer not empty interrupt
      \arg        SPI_INT_ERR: CRC error, configuration error, reception overrun error,
                               transmission underrun error and format error interrupt
      \arg        SPI_INT_OVLP_RBNE: overlap data not empty interrupt
    \param[out] none
    \retval     none
*/
void spi_interrupt_enable(uint32_t spi_periph, uint32_t interrupt)
{
    SPI_CTL1(spi_periph) |= interrupt & SPI_INT_MASK;
}

/*!
    \brief      disable SPI interrupt (API_ID(0x002CU))
    \param[in]  spi_periph: SPIx(x=0,1,2,3)
    \param[in]  interrupt: SPI interrupt
                only one parameter can be selected which is shown as below:
      \arg        SPI_INT_TBE: transmit buffer empty interrupt
      \arg        SPI_INT_RBNE: receive buffer not empty interrupt
      \arg        SPI_INT_ERR: CRC error, configuration error, reception overrun error,
                               transmission underrun error and format error interrupt
      \arg        SPI_INT_OVLP_RBNE: overlap data not empty interrupt
    \param[out] none
    \retval     none
*/
void spi_interrupt_disable(uint32_t spi_periph, uint32_t interrupt)
{
    SPI_CTL1(spi_periph) &= ~(interrupt & SPI_INT_MASK);
}

/*!
    \brief      get SPI interrupt flag status (API_ID(0x002DU))
    \param[in]  spi_periph: SPIx(x=0,1,2,3)
    \param[in]  int_flag: SPI interrupt flag status
                only one parameter can be selected which is shown as below:
      \arg        SPI_INT_FLAG_TBE: transmit buffer empty interrupt flag
      \arg        SPI_INT_FLAG_RBNE: receive buffer not empty interrupt flag
      \arg        SPI_INT_FLAG_RXORERR: overrun interrupt flag
      \arg        SPI_INT_FLAG_CONFERR: config error interrupt flag
      \arg        SPI_INT_FLAG_CRCERR: CRC error interrupt flag
      \arg        SPI_INT_FLAG_FERR: format error interrupt flag
      \arg        SPI_INT_FLAG_OVLP_RBNE: overlap data not empty interrupt flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus spi_interrupt_flag_get(uint32_t spi_periph, spi_interrupt_flag_enum int_flag)
{
    FlagStatus reval = RESET;
    uint32_t ret1;
    uint32_t ret2;

    /* get the status of interrupt enable bit */
    ret1 = (SPI_CTL1(spi_periph) & BIT(SPI_BIT_POS(int_flag)));
    /* get the status of interrupt flag */
    ret2 = (SPI_STAT(spi_periph) & BIT(SPI_BIT_POS2(int_flag)));

    if((RESET != ret1) && (RESET != ret2)) {
        reval = SET;
    } else {
        reval = RESET;
    }

    return reval;
}

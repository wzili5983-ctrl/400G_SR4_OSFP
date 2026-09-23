/*!
    \file    gd32e511_512_err_report.h
    \brief   Reporting Error driver

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

#ifndef ERR_REPORT_H
#define ERR_REPORT_H

#include <stdint.h>

/* define the size of the error report buffer */
#define ERR_REPORT_BUFFER_SIZE          2U

/* define the unique identifier of peripherals */
#define SYSCFG_MODULE_ID                                  ((uint8_t)0x01U)               /*!< SYSTEM module ID */
#define FMC_MODULE_ID                                     ((uint8_t)0x02U)               /*!< FMC module ID */
#define PMU_MODULE_ID                                     ((uint8_t)0x03U)               /*!< PMU module ID */
#define RCU_MODULE_ID                                     ((uint8_t)0x04U)               /*!< RCU module ID */
#define EXTI_MODULE_ID                                    ((uint8_t)0x05U)               /*!< EXTI module ID */
#define GPIO_MODULE_ID                                    ((uint8_t)0x06U)               /*!< GPIO module ID */
#define CRC_MODULE_ID                                     ((uint8_t)0x07U)               /*!< CRC module ID */
#define DBG_MODULE_ID                                     ((uint8_t)0x08U)               /*!< DBG module ID */
#define DMA_MODULE_ID                                     ((uint8_t)0x09U)               /*!< DMA module ID */
#define ADC_MODULE_ID                                     ((uint8_t)0x0AU)               /*!< ADC module ID */
#define DAC_MODULE_ID                                     ((uint8_t)0x0BU)               /*!< DAC module ID */
#define FWDGT_MODULE_ID                                   ((uint8_t)0x0CU)               /*!< Free watchdog timer module ID */
#define WWDGT_MODULE_ID                                   ((uint8_t)0x0DU)               /*!< Window watchdog timer module ID */
#define RTC_MODULE_ID                                     ((uint8_t)0x0EU)               /*!< RTC module ID */
#define TIMER_MODULE_ID                                   ((uint8_t)0x0FU)               /*!< TIMER module ID */
#define IFRP_MODULE_ID                                    ((uint8_t)0x10U)               /*!< IFRP module ID */
#define USART_MODULE_ID                                   ((uint8_t)0x11U)               /*!< USART module ID */
#define I2C_MODULE_ID                                     ((uint8_t)0x12U)               /*!< I2C module ID */
#define I3C_MODULE_ID                                     ((uint8_t)0x13U)               /*!< I3C module ID */
#define SPI_MODULE_ID                                     ((uint8_t)0x14U)               /*!< SPI module ID */
#define CMP_MODULE_ID                                     ((uint8_t)0x15U)               /*!< CMP module ID */
#define VREF_MODULE_ID                                    ((uint8_t)0x16U)               /*!< VREF module ID */
#define CLA_MODULE_ID                                     ((uint8_t)0x17U)               /*!< CLA module ID */
#define MDIO_MODULE_ID                                    ((uint8_t)0x18U)               /*!< MDIO module ID */
#define OPA_MODULE_ID                                     ((uint8_t)0x19U)               /*!< OPA module ID */

/* define the unique identifier of error type */
#define ERR_PERIPH                                        ((uint8_t)0x01U)               /*!< peripheral error */
#define ERR_PARAM_POINTER                                 ((uint8_t)0x02U)               /*!< invalid pointer */
#define ERR_PARAM_OUT_OF_RANGE                            ((uint8_t)0x03U)               /*!< out of range */
#define ERR_PARAM_INVALID                                 ((uint8_t)0x04U)               /*!< invalid parameter */

/* define the unique identifier of API */
#define API_ID(x)                                         ((uint16_t)(x))               /*!< API ID */

/* definitions for parameter checking */
#define NOT_VALID_POINTER(x)                              ((void *) 0 == (x))           /*!< check the invalid pointer */

/* defining the structure to store the parameters of report error function */
typedef struct {
    /* module ID where the error occurred */
    uint16_t moduleid;
    /* API ID associated with the error */
    uint16_t apiid;
    /* error ID indicating the specific error type */
    uint8_t errid;
} err_report_struct;

/* declare external arrays and variables for error reporting */
extern err_report_struct err_report_buffer[ERR_REPORT_BUFFER_SIZE];
/* index to track the next available position in the error report buffer */
extern uint8_t err_report_buff_index;

/* reporting errors in debug mode */
void fw_debug_report_err(uint16_t moduleid, uint16_t apiid, uint8_t errid);

#endif /* ERR_REPORT_H */

/****************************************************************************
*
*     Copyright (c) 2016 Broadcom Limited
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom Limited 
*           1320 Ridder Park Dr.
*            San Jose, California 95131
*            United States
*
*     All information contained in this document is Broadcom Limited 
*     company private, proprietary, and trade secret.
*
****************************************************************************/

/**
 *        CHAL type define header fil e
 *
 * @file type_defns.h
 * @author $Id$
 * @date 12-8-2016
 * @brief This file includes type define header file
 */

#ifndef TYPEDEF_H
#define TYPEDEF_H

#ifdef __linux
#include <phymod/phymod.h>
#endif

#ifndef __linux
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef long long int64_t;
typedef unsigned int   uintptr_t;
#endif
typedef signed char int8_t;

#ifdef __linux
#ifndef boolean
#define boolean unsigned char
#define FALSE 0
#define TRUE 1
#endif
#endif

/**
 * For Big Endian processor Enable this
 */
#ifdef __linux

#ifdef BE_HOST
#define IS_BIG_ENDIAN    (1)
#define IS_LITTLE_ENDIAN (0)
#else
#define IS_BIG_ENDIAN    (0)
#define IS_LITTLE_ENDIAN (1)
#endif

#if  IS_BIG_ENDIAN
#ifndef CAPI_IS_BIG_ENDIAN
#define CAPI_IS_BIG_ENDIAN
#endif /*CAPI_IS_BIG_ENDIAN*/
#endif

#endif

typedef enum return_result_e {
    RR_SUCCESS = 0,                                              /*!< Success                                                       */
    RR_ERROR,                                                    /*!< Error Category                                                */
    RR_ERROR_UNKNOWN,                                            /*!< Unknown Error                                                 */
    RR_ERROR_NOT_INITIALIZED,                                    /*!< Not Initialized Error                                         */
    RR_ERROR_INITIALIZATION,                                     /*!< Initialization Error                                          */
    RR_ERROR_WRONG_INPUT_VALUE,                                  /*!< Wrong Input Value Error                                       */
    RR_ERROR_WRONG_OUTPUT_VALUE,                                 /*!< Wrong Output Value Error                                      */
    RR_ERROR_WRONG_CORE_IP_VALUE,                                /*!< Wrong Core IP Error                                           */
    RR_ERROR_WRONG_LANE_INDEX,                                   /*!< Wrong Lane Index Error                                        */
    RR_ERROR_SYSTEM_UNAVAILABLE,                                 /*!< System Unavailable Error                                      */
    RR_ERROR_FIRMWARE_NOT_READY,                                 /*!< Firmware isn't ready yet                                      */
    RR_ERROR_IPC_HANDSHAKE_FAILED,                               /*!< Client IPC failed                                             */
    RR_ERROR_IPC_TIME_OUT,                                       /*!< IPC timeout                                                   */
    RR_ERROR_POLARITY_CMD_FAILED,                                /*!< Polarity Command Failed                                       */
    RR_ERROR_WRONG_PORT_CONFIGURATION,                           /*!< Wrong Port Configuration                                      */
    RR_ERROR_PLL_NOT_LOCKED,                                     /*!< PLL Isn't locked                                              */
    RR_ERROR_DOWNLOAD_FAILED,                                    /*!< Download failed                                               */
    RR_ERROR_AVS_INITIALIZATION_FAILED,                          /*!< AVS initialization failed                                     */
    RR_ERROR_AVS_INITIALIZATION_TIME_OUT,                        /*!< AVS initialization timeout                                    */
    RR_ERROR_NOT_ALLOWED_WRONG_STATE,                            /*!< Not Allowed Wrong State                                       */
    RR_ERROR_MODE_CHANGE_PORTS_NOT_DROPPED,                      /*!< Ports are not dropped when configuring mode                   */
       
    RR_ERROR_BOUNDS,                                             /*!< Failed Bounds Check                                           */
    RR_ERROR_FEATURE_NOT_SUPPORTED,                              /*!< Not available or not supported>                               */
    RR_ERROR_GLOBAL_FAULT,                                       /*!< Global fault, such as pll_lol>                                */
    RR_ERROR_DIAGNOSTIC,                                         /*!< Diagnostic error>                                             */
    RR_ERROR_MDIO,                                               /*!< MDIO driver read/write error                                  */

    RR_ERROR_EEPROM_NOT_READY,                                   /*!< EEPROM Not ready, or not installed>                           */
    RR_ERROR_FW_VERSION_MISMATCH,                                /*!< Firmware Version Mismatch>                                    */    
    RR_ERROR_CRC32_MISMATCH,                                     /*!< CRC mismatch for SRAM image>                                  */
    RR_ERROR_CRC32_SPI_MISMATCH,                                 /*!< CRC mismatch for SPI image>                                   */

    RR_ERROR_CLIENT_CRC_MISMATCH,                                /*!< Client: CRC mismatch for BH image>                            */
    RR_ERROR_CLIENT_INVALID_RAM_ADDR,                            /*!< Client: Invalid RAM Address                                   */
    RR_ERROR_CLIENT_SERDES_DELAY,                                /*!< Client: SerDes Delay                                          */
    RR_ERROR_CLIENT_POLLING_TIMEOUT,                             /*!< Client: Polling Timeout                                       */
    RR_ERROR_CLIENT_CFG_PATT_INVALID_PATTERN,                    /*!< Client: CFG pattern Invalid Pattern                           */
    RR_ERROR_CLIENT_CFG_PATT_INVALID_PATT_LENGTH,                /*!< Client: CFG pattern Invalid Pattern Length                    */
    RR_ERROR_CLIENT_CFG_PATT_LEN_MISMATCH,                       /*!< Client: CFG pattern Lenght Mismatch                           */
    RR_ERROR_CLIENT_CFG_PATT_PATTERN_BIGGER_THAN_MAXLEN,         /*!< Client: CFG pattern Pattern Bigger Than Max Length            */
    RR_ERROR_CLIENT_CFG_PATT_INVALID_HEX,                        /*!< Client: CFG pattern Invalid Hex                               */
    RR_ERROR_CLIENT_CFG_PATT_INVALID_BIN2HEX,                    /*!< Client: CFG pattern Invalid Bin 2 Hex                         */
    RR_ERROR_CLIENT_CFG_PATT_INVALID_SEQ_WRITE,                  /*!< Client: CFG pattern Invalid Sequence Write                    */
    RR_ERROR_CLIENT_PATT_GEN_INVALID_MODE_SEL,                   /*!< Client: Pattern Gen Valid Mode Selection                      */
    RR_ERROR_CLIENT_INVALID_UCODE_LEN,                           /*!< Client: Invalid Ucode Lenth                                   */
    RR_ERROR_CLIENT_MICRO_INIT_NOT_DONE,                         /*!< Client: Micro Initialization Not Done                         */
    RR_ERROR_CLIENT_UCODE_LOAD_FAIL,                             /*!< Client: Ucode Load Failed                                     */
    RR_ERROR_CLIENT_UCODE_VERIFY_FAIL,                           /*!< Client: Ucode Verify Fail                                     */
    RR_ERROR_CLIENT_INVALID_TEMP_IDX,                            /*!< Client: Invalid Temp Index                                    */
    RR_ERROR_CLIENT_INVALID_PLL_CFG,                             /*!< Client: Invalid PLL CFG                                       */
    RR_ERROR_CLIENT_TX_HPF_INVALID,                              /*!< Client: Tx HPF Invalid                                        */
    RR_ERROR_CLIENT_VGA_INVALID,                                 /*!< Client: VGA_Invalid                                           */
    RR_ERROR_CLIENT_PF_INVALID,                                  /*!< Client: PF Invalid                                            */
    RR_ERROR_CLIENT_TX_AMP_CTRL_INVALID,                         /*!< Client: Tx Amplitude Controller Invalid                       */
    RR_ERROR_CLIENT_INVALID_EVENT_LOG_WRITE,                     /*!< Client: Invalid Event Log Write                               */
    RR_ERROR_CLIENT_INVALID_EVENT_LOG_READ,                      /*!< Client: Invalid Event Log Read                                */
    RR_ERROR_CLIENT_UC_CMD_RETURN_ERROR,                         /*!< Client: UC Command Retun Error                                */
    RR_ERROR_CLIENT_DATA_NOTAVAIL,                               /*!< Client: Data Not Available                                    */
    RR_ERROR_CLIENT_BAD_PTR_OR_INVALID_INPUT,                    /*!< Client: Bad Pointer of Invalid Input                          */
    RR_ERROR_CLIENT_UC_NOT_STOPPED,                              /*!< Client: UC Not Stopped                                        */
    RR_ERROR_CLIENT_UC_CRC_NOT_MATCH,                            /*!< Client: UC CRC Not Match                                      */
    RR_ERROR_CLIENT_CORE_DP_NOT_RESET,                           /*!< Client: Core Data Path Not Reset                              */
    RR_ERROR_CLIENT_LANE_DP_NOT_RESET,                           /*!< Client: Lane Data Patch not Reset                             */
    RR_ERROR_CLIENT_EXCEPTION,                                   /*!< Client: Exception                                             */
    RR_ERROR_CLIENT_INFO_TABLE_ERROR,                            /*!< Client: Information Table Error                               */
    RR_ERROR_CLIENT_REFCLK_FREQUENCY_INVALID,                    /*!< Client: Reference Clock Frequency Invalid                     */
    RR_ERROR_CLIENT_PLL_DIV_INVALID,                             /*!< Client: PLL Divisor Invalid                                   */
    RR_ERROR_CLIENT_VCO_FREQUENCY_INVALID,                       /*!< Client: VCO Frequency Invalid                                 */
    RR_ERROR_CLIENT_INSUFFICIENT_PARAMETERS,                     /*!< Client: Insufficient Parameters                               */
    RR_ERROR_CLIENT_CONFLICTING_PARAMETERS,                      /*!< Client: Conflicting Parameters                                */
    RR_ERROR_CLIENT_BAD_LANE_COUNT,                              /*!< Client: Bad Lane Count                                        */
    RR_ERROR_CLIENT_BAD_LANE,                                    /*!< Client: Bad Lane                                              */
    RR_ERROR_CLIENT_UC_NOT_RESET,                                /*!< Client: UC Not Reset                                          */
    RR_ERROR_CLIENT_FFE_TAP_INVALID,                             /*!< Client: FFE Tap INvalid                                       */
    RR_ERROR_CLIENT_FFE_NOT_AVAILABLE,                           /*!< Client: FFE Not Available                                     */
    RR_ERROR_CLIENT_INVALID_RX_PAM_MODE,                         /*!< Client: Invalid Rx PAM Mode                                   */
    RR_ERROR_CLIENT_INVALID_PRBS_ERR_ANALYZER_FEC_SIZE,          /*!< Client: Invalid PRBS Error Analyzer FEC Size                  */
    RR_ERROR_CLIENT_INVALID_PRBS_ERR_ANALYZER_ERR_THRESH,        /*!< Client: Invalid PRBS Error ANalyzer Error Threshold           */
    RR_ERROR_CLIENT_INVALID_PRBS_ERR_ANALYZER_NO_PLL_LOCK,       /*!< Client: Invalid PRBS Error Analyzer No PLL Lock               */
    RR_ERROR_CLIENT_CFG_PATT_INVALID_PAM4,                       /*!< Client: CFG Pattern Invalid PAM4                              */
    RR_ERROR_CLIENT_INVALID_TDT_PATTERN_FOR_HW_MODE,             /*!< Client: Invalid TDT Pattern for HW Mode                       */
    RR_ERROR_CLIENT_ODD_PRE_OR_POST_TAP_INPUT,                   /*!< Client: Odd Pre Or Post Tap Input                             */
    RR_ERROR_CLIENT_RX_PI_DISP_MSB_STATUS_IS_1,                  /*!< Client: Rx Pi Disp MSB Status Is 1 we                         */
    RR_ERROR_CLIENT_IMAGE_SIZE_NOT_SUPPORTED,                    /*!< Client: Image Size Not Supported                              */
    RR_ERROR_CLIENT_TDT_CLIPPED_WAVEFORM,                        /*!< Client: TD Clipped Waveform                                   */
    RR_ERROR_CLIENT_DBSTOP_NOT_WORKING,                          /*!< Client: DB Stop Not Working                                   */
    RR_ERROR_CLIENT_PRBS_CHK_HW_TIMERS_NOT_EXPIRED,              /*!< Client: PRBS Check HW Timers Not Expired                      */
    RR_ERROR_CLIENT_INVALID_VALUE,                               /*!< Client: Invalid Value                                         */
    RR_ERROR_CLIENT_UC_CMD_POLLING_TIMEOUT,                      /*!< Client: UC Command Polling Time Out                           */
    RR_ERROR_CLIENT_INVALID_INFO_TABLE_ADDR,                     /*!< Client: Invalid Information Table Address                     */
    RR_ERROR_CLIENT_TXFIR,                                       /*!< Client: TxFIR                                                 */
    RR_ERROR_CLIENT_DFE_TAP,                                     /*!< Client: DFE Tap                                               */
    RR_ERROR_CLIENT_DIAG,                                        /*!< Client: Diagnostic                                            */
    RR_ERROR_CLIENT_TDT_PATTERN_LENGTH_WR_FAILED,                /*!< Client: TDT Pattern Length Write Failed                       */
    RR_ERROR_CLIENT_API_INVOKE_ERROR,                            /*!< Client: Blackhawk API Failed                                  */
    RR_ERROR_CLIENT_PAM_MODE_FRC_FAILED,                         /*!< Client: PAM mode force Failed                                 */
    RR_ERROR_SPI_BURST_WRITE_ERROR,                              /*!< SPI: burst write failed                                       */
    RR_ERROR_SPI_PROGRAM_NOT_ACK,                                /*!< SPI: RR_ERROR_SPI_BURST_WRITE_ERROR                           */
    RR_ERROR_SPI_PROGRAM_VERIFY_FAILED,                          /*!< SPI: RR_ERROR_SPI_PROGRAM_NOT_ACK                             */
    RR_ERROR_SPI_SIZE_INCORRECT,                                 /*!< SPI: RR_ERROR_SPI_SIZE_INCORRECT                              */
    RR_ERROR_SRAM_BURST_WRITE_ERROR,                             /*!< SPI: burst write failed                                       */
    RR_ERROR_SRAM_PROGRAM_NOT_ACK,                               /*!< SPI: RR_ERROR_SPI_BURST_WRITE_ERROR                           */
    RR_ERROR_SRAM_PROGRAM_VERIFY_FAILED,                         /*!< SPI: RR_ERROR_SPI_PROGRAM_NOT_ACK                             */
    RR_ERROR_SRAM_SIZE_INCORRECT,                                /*!< SPI: RR_ERROR_SPI_SIZE_INCORRECT                              */
    RR_ERROR_SPI_FW_UPGRADE_ERROR,                               /*!< SPI: RR_ERROR_SPI _FW_UPGRADE_ERROR                           */
    RR_ERROR_SPI_FW_UPGRADE_ERASE_ERROR,                         /*!< SPI: RR_ERROR_SPI_FW_UPGRADE_ERASE_ERROR                      */
    RR_LAST_ERROR,                                               /*!< Last Error :RR_ERROR_SPI_PROGRAM_VERIFY_FAILED                */

    RR_WARNING,                                                  /*!< Warning Category                                              */
    RR_WARNING_BUSY_TRY_LATER,                                   /*!< Busy Try LaterWarning                                         */
    RR_WARNING_NOT_INITIALIZED,                                  /*!< Not Initialized Warning                                       */
    RR_WARNING_INITIALIZATION,                                   /*!< Initialization Warning                                        */
    RR_WARNING_WRONG_INPUT_VALUE,                                /*!< Wrong Input Value Warning                                     */
    RR_WARNING_SYSTEM_UNAVAILABLE,                               /*!< System Unavailable Warning                                    */
    RR_WARNING_BOUNDS,                                           /*!< Out of Bounds                                                 */
    RR_WARNING_NOT_COMPATIBLE_WITH_CURRENT_PLL_SETTING,          /*!< The new port setting isn't compatible with the PLL config     */
    RR_WARNING_MUTEX_UNAVAILABLE_RETRY_LATER,                    /*!< Given HW Mutex unavailable currently. retry later             */
    RR_WARNING_NO_MORE_HW_MUTEX_AVAILABLE,                       /*!< No more HW mutex available in the mutex pool                  */
    RR_WARNING_HW_MUTEX_NOT_ACQUIRED,                            /*!< No HW mutex was acquired successfully at this time            */
    RR_WARNING_WRONG_VOLTAGE_READING,                            /*!< Wrong Voltage reading                                         */
    RR_WARNING_IN_PROCESS_TRY_LATER,                             /*!< FW has not finished the last operation                        */
    RR_LAST_WARNING,                                             /*!< Last Warning                                                  */
} return_result_t;                                               /*!< Retur Result Type                                             */



#ifdef FPGA_PLATFORM /*if testing run on C++ Debugger*/

#ifndef boolean
typedef unsigned char boolean;
#define FALSE 0
#define TRUE 1
#endif
#else
#ifndef __linux
//typedef unsigned long uintptr_t;
enum {
    FALSE = 0,
    TRUE = 1
};
typedef uint8_t boolean;  // force the enum to 1 bytes

#endif
#endif

#ifndef GTEST
#ifndef bool
#define bool boolean
#endif
#endif

#ifndef false
#define false FALSE
#endif

#ifndef true
#define true TRUE
#endif

#if !defined STDIO && !defined NULL
#define NULL ((void*)0)
#endif
typedef unsigned int ubaddr_t;

enum {
    OFF = 0,
    ON
};
typedef uint8_t onoff_t;  // force the enum to 1 bytes

typedef enum {
    CH0 = 0,
    CH1 = 1,
    CH2 = 2,
    CH3
} chan_t;

typedef enum {
    TX = 0,
    RX
} txrx_t;

#if defined(_MSC_VER)
#define ALIGNED_(x) __declspec(align(x))
#define PACKED_
#else
#if defined(__GNUC__)
#define ALIGNED_(x) __attribute__ ((aligned(x)))
#define PACKED_     __attribute__ ((__packed__))
#else
#define ALIGNED_(x)
#define PACKED_
#endif
#endif

#define REGLEN 4

/** A compile time assertion check.
 *
 *  Validate at compile time that the exp is true without
 *  generating code. This can be used at any point in a source file
 *  where typedef is legal.
 *
 *  On success, compilation proceeds normally.
 *
 *  On failure, attempts to typedef an array type of negative size. 
 */
/* combine arguments (after expanding arguments)*/
#define COMPILER_VERIFY(exp)

#endif /*TYPEDEF_H*/

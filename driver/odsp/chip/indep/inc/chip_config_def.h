/**
 * @file     chip_config_def.h
 * @author  
 * @date     01-21-2021
 * @version 1.0
 *
 * @property    $ Copyright: (c) 2021 Broadcom Limited All Rights Reserved $
 *       No portions of this material may be reproduced in any form without the
 *       written permission of: 
 *               Broadcom Limited
 *               1320 Ridder Park Drive
 *               San Jose, California 95131
 *               United States
 * All information contained in this document/file is Broadcom Limit company
 * private proprietary, trade secret, and remains the property of Broadcom
 * Limited. The intellectual and technical concepts contained herein are
 * proprietary to Broadcom Limited and may be covered by U.S. and Foreign
 * Patents, patents in process, and are protected by trade secret or copyright
 * law. Dissemination of this information or reproduction of this material is
 * strictly forbidden unless prior written permission is obtained from Bloadcom
 * Limited.
 *
 * @brief   
 *
 * @section
 * 
 */
#ifndef CHIP_CONFIG_DEF_H
#define CHIP_CONFIG_DEF_H

#ifdef __cplusplus
extern "C" {
#endif

#define CAPI_CONFIG_ENABLE       1          /**< CAPI configuration Enable    */
#define CAPI_CONFIG_DISABLE      0          /**< CAPI configuration Disable   */

/***********************************************************/
/*********************Diagnostic Category*******************/
/***********************************************************/
#if CAPI_CONFIG_ENABLE
#define CAPI_CONFIG_CMIS_INTF_ENABLE                /**< Enable CMIS feature  */
#define CAPI_DSP_GET_CAPTURE_MEM                    /**< Enable capture memory feature  */
#define CAPI_DSP_GET_DIAG_INFO                      /**< Enable dsp rx diag info        */
#endif

//#define CAPI_CMIS_FEC_STATS_SMALL_FOOTPRINT         /**< Use small footprint version of CMIS FEC stats feature which reports the final result only */

/***********************************************************/
/*********************Test Category*************************/
/***********************************************************/
#if CAPI_CONFIG_ENABLE
#define CAPI_CONFIG_FEC_PRBS_ENABLE                 /**< Enable FEC PRBS      */
#define CAPI_CONFIG_RPC_CMD_FEC_STATISTICS_ENABLE   /**< Enable FEC Statistics Inforation RPC CMD handler  */
#define CAPI_CONFIG_FEC_ST_ENABLE                   /**< Enable FEC stats feature  */
#endif


#if CAPI_CONFIG_DISABLE
#define PORTOFINO_TBD
#define CAPI_CONFIG_CW_PRBS_ENABLE                  /**< Enable CW PRBS      */
#endif

/***********************************************************/
/*********************Log/Trace Category********************/
/***********************************************************/
#if CAPI_CONFIG_DISABLE
#define CAPI_CONFIG_RUNTIME_LOG                     /**< Enable/Disable CAPI logging at runtime */ 
#define ENABLE_CAPI_LOG                             /**< Enable CAPI logging  */
#endif

/***********************************************************/
/***********************CAPI Category***********************/
/***********************************************************/
#if CAPI_CONFIG_ENABLE
#define CAPI_CONFIG_CMD_SANITY_CHK_ENABLE              /**< Enable Sanity Checker for cAPI          */
#define CAPI_CONFIG_FAST_SPI_PROGRAM_WITH_BLOCK_WRITE  /**< Enable SPI programming with block write */
#endif

#if CAPI_CONFIG_DISABLE
#define CAPI_CONFIG_ARCHIVED                           /**< Features archived for now. To be implemented as required */
#endif

/***********************************************************/
/*********************Serdes Category***********************/
/***********************************************************/


/***********************************************************/
/***********************DSP Category************************/
/***********************************************************/
#if CAPI_CONFIG_DISABLE
#define CAPI_DSP_PLANNED                               /**< DSP features to be implemented as required */
#endif


/***********************************************************/
/***********************Chip Category***********************/
/***********************************************************/
#if CAPI_CONFIG_ENABLE
#define CAPI_CONFIG_SPI_BLOCK_WRITE_ENABLE             /**< Enable Block write for SPI module */
#endif



#ifdef __cplusplus
}
#endif

#endif /* CHIP_CONFIG_DEF_H */


/**
 *
 * @file       dsp_internal_config.c
 * @author     PLP Firmware Team
 * @date       02/15/2020
 * @version    1.0
 *
 * @property 
 * $Copyright: (c) 2020 Broadcom.
 *             Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * @brief      The CAPI files contain all the CAPIs used by the the module and line card products.
 *
 * @section  description
 *
 */
#include "type_defns.h"
#include "regs_common.h"
#include "chip_mode_def.h"
#include "capi_def.h"
#include "dsp_config.h"

#define DSP_MEDIA_SIDE_GPR_OFFSET                             0x00000300                         /**< DSP MEDIA Side GPR Offset                       */
#define DSP_LANE_0_CONFIG_GPR_BASE_ADDRESS                    (SRAM_GP_REGION_START +  0)        /**< DSP Lane 0 Configuration Base Address          */
#define DSP_LANE_1_CONFIG_GPR_BASE_ADDRESS                    (SRAM_GP_REGION_START +  4)        /**< DSP Lane 1 Configuration Base Address          */
#define DSP_LANE_2_CONFIG_GPR_BASE_ADDRESS                    (SRAM_GP_REGION_START +  8)        /**< DSP Lane 2 Configuration Base Address          */
#define DSP_LANE_3_CONFIG_GPR_BASE_ADDRESS                    (SRAM_GP_REGION_START + 12)        /**< DSP Lane 3 Configuration Base Address          */

/*
SRAM_GP_REGION_START 0x52000
host: 0x52000  ~ 0x522FF
line: 0x52300  ~ 0x525FF
*/
/**< dsp lane base address per line id */
const ubaddr_t dsp_lane_bbaddr[PHY_BOTH_SIDES][DSP_LANE_NUM_MAX] =
                   {{ DSP_HOST_SIDE_ADDRESS  + DSP_LANE_0_ADDRESS_OFFSET,    /**< Host DSP Lane 0 Base Address  */
                      DSP_HOST_SIDE_ADDRESS  + DSP_LANE_1_ADDRESS_OFFSET,    /**< Host DSP Lane 1 Base Address  */
                      DSP_HOST_SIDE_ADDRESS  + DSP_LANE_2_ADDRESS_OFFSET,    /**< Host DSP Lane 2 Base Address  */
                      DSP_HOST_SIDE_ADDRESS  + DSP_LANE_3_ADDRESS_OFFSET},    /**< Host DSP Lane 3 Base Address  */
                    { DSP_MEDIA_SIDE_ADDRESS + DSP_LANE_0_ADDRESS_OFFSET,    /**< Media DSP Lane 0 Base Address */
                      DSP_MEDIA_SIDE_ADDRESS + DSP_LANE_1_ADDRESS_OFFSET,    /**< Media DSP Lane 1 Base Address */
                      DSP_MEDIA_SIDE_ADDRESS + DSP_LANE_2_ADDRESS_OFFSET,    /**< Media DSP Lane 2 Base Address */
                      DSP_MEDIA_SIDE_ADDRESS + DSP_LANE_3_ADDRESS_OFFSET},    /**< Media DSP Lane 3 Base Address */
                    };

/**< DSP GPR lane configuration base address */
const ubaddr_t dsp_lane_config_gpr_bbaddr[PHY_BOTH_SIDES][DSP_LANE_NUM_MAX] =
                   {{ DSP_LANE_0_CONFIG_GPR_BASE_ADDRESS,    /**< DSP Host Side Lane 0 Configuration Base Address   */
                      DSP_LANE_1_CONFIG_GPR_BASE_ADDRESS,    /**< DSP Host Side Lane 1 Configuration Base Address   */
                      DSP_LANE_2_CONFIG_GPR_BASE_ADDRESS,    /**< DSP Host Side Lane 2 Configuration Base Address   */
                      DSP_LANE_3_CONFIG_GPR_BASE_ADDRESS},    /**< DSP Host Side Lane 3 Configuration Base Address   */
                    { DSP_LANE_0_CONFIG_GPR_BASE_ADDRESS + DSP_MEDIA_SIDE_GPR_OFFSET, /**< DSP Media Side Lane 0 Configuration Base Addresss */
                      DSP_LANE_1_CONFIG_GPR_BASE_ADDRESS + DSP_MEDIA_SIDE_GPR_OFFSET, /**< DSP Media Side Lane 1 Configuration Base Addresss */
                      DSP_LANE_2_CONFIG_GPR_BASE_ADDRESS + DSP_MEDIA_SIDE_GPR_OFFSET, /**< DSP Media Side Lane 2 Configuration Base Addresss */
                      DSP_LANE_3_CONFIG_GPR_BASE_ADDRESS + DSP_MEDIA_SIDE_GPR_OFFSET}, /**< DSP Media Side Lane 3 Configuration Base Addresss */
                    };

/**
 *
 * @file       dsp_internal_config.h
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
#ifndef DSP_CONFIG_H
#define DSP_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#define DSP_LANE_NUM_MAX                      4                         /**< DSP (Host/Media-Side) Max Number of Lanes = 8  */

#define DSP_HOST_SIDE_ADDRESS                 HOST_BASE                 /**< DSP Host Side Base Address                     */
#define DSP_MEDIA_SIDE_ADDRESS                MEDIA_BASE                /**< DSP Media Side Base Address                    */
#define DSP_LANE_0_ADDRESS_OFFSET             TOP_LW_BLOCK_CH0          /**< DSP Lane 0 Base Address Offset                 */
#define DSP_LANE_1_ADDRESS_OFFSET             TOP_LW_BLOCK_CH1          /**< DSP Lane 1 Base Address Offset                 */
#define DSP_LANE_2_ADDRESS_OFFSET             TOP_LW_BLOCK_CH2          /**< DSP Lane 2 Base Address Offset                 */
#define DSP_LANE_3_ADDRESS_OFFSET             TOP_LW_BLOCK_CH3          /**< DSP Lane 3 Base Address Offset                 */
#if 0
#define DSP_LANE_4_ADDRESS_OFFSET             TOP_LW_BLOCK_CH3          /**< DSP Lane 4 Base Address Offset                 */
#define DSP_LANE_5_ADDRESS_OFFSET             TOP_LW_BLOCK_CH3          /**< DSP Lane 5 Base Address Offset                 */
#define DSP_LANE_6_ADDRESS_OFFSET             TOP_LW_BLOCK_CH3          /**< DSP Lane 6 Base Address Offset                 */
#define DSP_LANE_7_ADDRESS_OFFSET             TOP_LW_BLOCK_CH3          /**< DSP Lane 7 Base Address Offset                 */
#endif

extern const ubaddr_t dsp_lane_bbaddr[2][DSP_LANE_NUM_MAX];             /**< DSP Lane base address                          */
extern const ubaddr_t dsp_lane_config_gpr_bbaddr[2][DSP_LANE_NUM_MAX];  /**< DSP Lane Configuration GPR Base Address        */

#define DSP_TX_JF_BW                             8

#define DSP_TX_JF2_ENALBE                        0
#define DSP_TX_JF2_BW                            6

#define VA_A_PARAM_DEFAULT						 0x8

#ifdef __cplusplus
}
#endif

#endif /* DSP_CONFIG_H */

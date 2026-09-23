/**
 * @file     capi_custome_def.h
 * @author  
 * @date     02-26-2021
 * @version  1.0
 *
 * @property
 * $Copyright: (c) 2021 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * @brief    brief description of the block source file, sequences/flow/etc.
 *
 * @section  description
 *
 */
#ifndef CAPI_CUSTOME_DEF_H
#define CAPI_CUSTOME_DEF_H

#ifdef __cplusplus
extern "C" {
#endif
    

#define TAP_VALUE_LEVEL_NUM         4
#define TAP_NONLINEAR_TAP_NUM       256
#define TXFIR_TAP_LVL0_SYM          (-3*32/2)
#define TXFIR_TAP_LVL1_SYM          (-1*32/2)
#define TXFIR_TAP_LVL2_SYM          (1*32/2)
#define TXFIR_TAP_LVL3_SYM          (3*32/2)

/**
 * The structure of TxFIR 4TAP Table Info
 */
typedef struct capi_txfir_4tap_table_info_s {
    int8_t      four_taps[256];                       /**< TxFIR 4 tap[0,....,255]                                         */
} capi_txfir_4tap_table_info_t;                                  /**< TxFIR Table Info type*/

/**
 * The structure of TxFIR nonlinear 13TAP Table Info
 */
typedef struct capi_txfir_nonlinear_13tap_table_info_s {
    int16_t    tap0[TAP_VALUE_LEVEL_NUM];     /**< TxFIR 0[level(0~3):4 levels as (-3, -1, 1, 3) ]                                         */
    int16_t    tap1[TAP_VALUE_LEVEL_NUM];     /**< TxFIR 1[level(0~3):4 levels as (-3, -1, 1, 3) ]                                         */
    int16_t    tap2[TAP_VALUE_LEVEL_NUM];     /**< TxFIR 2[level(0~3):4 levels as (-3, -1, 1, 3) ]                                         */
    int16_t    tap3456[TAP_NONLINEAR_TAP_NUM];/**< TxFIR 3/4/5/6 nonlinear 256 tap input,  the same scale as 4*DAC value.
                                                   Its address index is organized as [symbol3, symbol4, symbol5, symbol6].
                                                   Symbol3 is the earlier one                                                              */
    int16_t    tap7[TAP_VALUE_LEVEL_NUM];     /**< TxFIR 7[level(0~3):4 levels as (-3, -1, 1, 3) ]                                         */
    int16_t    tap8[TAP_VALUE_LEVEL_NUM];     /**< TxFIR 8[level(0~3):4 levels as (-3, -1, 1, 3) ]                                         */
    int16_t    tap9[TAP_VALUE_LEVEL_NUM];     /**< TxFIR 9[level(0~3):4 levels as (-3, -1, 1, 3) ]                                         */
    
    uint16_t    float_loc;                     /**<the floating tap location before tap0,  the range is [0, 11].
                                                  0 means the 3 floating taps are right before tap0. 
                                                  11 means the 3 floating taps are 11 symbols before tap0      */
    int16_t    float_tap1[TAP_VALUE_LEVEL_NUM];/**< TxFIR floating tap1[level(0~3):4 levels as (-3, -1, 1, 3) ]                            */
    int16_t    float_tap2[TAP_VALUE_LEVEL_NUM];/**< TxFIR floating tap2[level(0~3):4 levels as (-3, -1, 1, 3) ]                            */
    int16_t    float_tap3[TAP_VALUE_LEVEL_NUM];/**< TxFIR floating tap3[level(0~3):4 levels as (-3, -1, 1, 3) ]                            */
} capi_txfir_nonlinear_13tap_table_info_t;                                  /**< TxFIR Table Info type*/

/**
 * The structure of custome command information 
 */
typedef struct capi_custom_command_info_s {
    phy_command_id_t command_id;                                   /**< Command Identifier                    */

    union {
        capi_txfir_4tap_table_info_t     txfir_4tap_table_info;              /**< Costome: TxFIR 4TAP Information Table      */
        capi_txfir_nonlinear_13tap_table_info_t txfir_nonlinear_13tap_info;         /**< Costome: TxFIR non-linear 13TAP Information Table      */
    } type;                                                        /**< Payload/data type of the command Id   */

} capi_custom_command_info_t;                                     /**< Custome Command Information type      */

#ifdef __cplusplus
}
#endif

#endif /* CAPI_CUSTOME_DEF_H */


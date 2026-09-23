/**
 *
 * @file     chal_gpio.h
 * @author   
 * @date     9/13/2018
 * @version  1.0
 *
 * @property  $Copyright: Copyright 2018 Broadcom INC.
 * This program is the proprietary software of Broadcom INC
 * and/or its licensors, and may only be used, duplicated, modified
 * or distributed pursuant to the terms and conditions of a separate,
 * written license agreement executed between you and Broadcom
 * (an "Authorized License").  Except as set forth in an Authorized
 * License, Broadcom grants no license (express or implied), right
 * to use, or waiver of any kind with respect to the Software, and
 * Broadcom expressly reserves all rights in and to the Software
 * and all intellectual property rights therein.  IF YOU HAVE
 * NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE
 * IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE
 * ALL USE OF THE SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1.     This program, including its structure, sequence and organization,
 * constitutes the valuable trade secrets of Broadcom, and you shall use
 * all reasonable efforts to protect the confidentiality thereof,
 * and to use this information only in connection with your use of
 * Broadcom integrated circuit products.
 *
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS
 * PROVIDED "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
 * REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
 * OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
 * DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
 * NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
 * ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
 * OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL
 * BROADCOM OR ITS LICENSORS BE LIABLE FOR   CONSEQUENTIAL,
 * INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER
 * ARISING OUT OF OR IN ANY WAY RELATING TO YOUR USE OF OR INABILITY
 * TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF
 * THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR USD 1.00,
 * WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING
 * ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.$
 *
 * @brief   this file includes LINE WRAPPER AFE IP block cHAL function implementation.
 *
 * @section
 * 
 */

#ifndef CHAL_GPIO_H
#define CHAL_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif


return_result_t chal_gpio_set_dir(phy_info_t* phy_info_ptr, uint8_t gpio_num, uint8_t dir) ;
return_result_t chal_gpio_get_dir(phy_info_t* phy_info_ptr, uint8_t gpio_num, uint8_t* dir_ptr) ;

return_result_t chal_gpio_set_output(phy_info_t* phy_info_ptr, uint8_t gpio_num, uint8_t output_data);

return_result_t chal_gpio_get_input(phy_info_t* phy_info_ptr, uint8_t gpio_num, uint8_t* input_data_ptr) ;

return_result_t chal_gpio_set_pullup(phy_info_t* phy_info_ptr, uint8_t gpio_num, uint8_t active) ;
return_result_t chal_gpio_get_pullup(phy_info_t* phy_info_ptr, uint8_t gpio_num, uint8_t* active_pttr) ;
return_result_t chal_gpio_set_pulldown(phy_info_t* phy_info_ptr, uint8_t gpio_num, uint8_t active) ;
return_result_t chal_gpio_get_pulldown(phy_info_t* phy_info_ptr, uint8_t gpio_num, uint8_t* active_ptr) ;
#ifdef __cplusplus
}
#endif

#endif


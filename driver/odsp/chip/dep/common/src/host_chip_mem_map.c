/**
 *
 * @file    host_chip_mem_map.c
 * @author  HSIP FW Team
 * @date    10/18/2021
 * @version    0.1
 *
 * @property    $ Copyright: (c) 2021 Broadcom Limited All Rights Reserved $
 *        No portions of this material may be reproduced in any form without the
 *        written permission of: 
 *                    Broadcom Limited
 *                    1320 Ridder Park Drive
 *                    San Jose, California 95131
 *                    United States
 *        All information contained in this document/file is Broadcom Limit company
 *        private proprietary, trade secret, and remains the property of Broadcom
 *         Limited. The intellectual and technical concepts contained herein are
 *         proprietary to Broadcom Limited and may be covered by U.S. and Foreign Patents,
 *        patents in process, and are protected by trade secret or copyright law.
 *        Dissemination of this information or reproduction of this material is strictly
 *        forbidden unless prior written permission is obtained from Bloadcom Limited.
 *
 * @brief    brief description of the block source file, sequences/flow/etc.
 *
 * @section    host chip memoery map data
 * 
 */

/** @file host_chip_mem_map.c
 */

#include <stdlib.h>
#include "regs_common.h"
#include "access.h"
#include "common_util.h"
#include "host_chip_mem_map.h"

static host_chip_memmap_info_t *g_chip_mem_map_data_ptr = NULL;

/**
 * @brief       chip_memmap_get_data_ptr(phy_info_t *phy_info_ptr)
 * @details     This function returns host chip memory map region base address
 * 
 * @return      host chip memory map region base address 
 */
host_chip_memmap_info_t *host_chip_memmap_get_data_ptr(phy_info_t *phy_info_ptr)
{
    uint32_t chip_memmap_addr;

    if (g_chip_mem_map_data_ptr == NULL) {
        chip_memmap_addr = (rd_reg_(OCTAL_TOP_REGS, FW_MEM_MAP_ADDR_HI_REG, phy_info_ptr->phy_id) << 16) |
                            (rd_reg_(OCTAL_TOP_REGS, FW_MEM_MAP_ADDR_LO_REG, phy_info_ptr->phy_id) & 0xFFFF);
        g_chip_mem_map_data_ptr = (host_chip_memmap_info_t*)(uintptr_t)chip_memmap_addr;
    }
    return g_chip_mem_map_data_ptr;
}


#ifndef HW_CHIP_COMMON_DEF_H
#define HW_CHIP_COMMON_DEF_H

/* number of clock cycle in 1 millisecond */
#define CLK_CYCLE_1MS 156250
#define CLK_CYCLE_1US 156

/**
 * Hercules Chip Identification
 */
#define HERCULES_87420_CHIP_ID         0x87420                     /**< CHIP ID 0x87420       */
#define HERCULES_87423_CHIP_ID        0x87423                     /**< CHIP ID 0x87423       */

#define HERCULES_REV_ID_A0            0xA0                        /**< REV ID A0             */
#define HERCULES_REV_ID_A1            0xA1                        /**< REV ID A0             */

#define CHIP_HOST_LANE_MAX         4            /**< Host Maximum Number of Lanes  */
#define CHIP_MEDIA_LANE_MAX        4            /**< Media Maximum Number of Lanes */

#endif

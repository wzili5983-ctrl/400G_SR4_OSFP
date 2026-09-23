#ifndef IPC_REGS_H
#define IPC_REGS_H

#define INTF_CWTOLW_COMMANDSTATUS_LANE0_CMDSTAT_CHG_MASK 0x00008000 /*!< CMDSTAT Change*/
#define INTF_CWTOLW_COMMANDSTATUS_LANE0_CMDSTAT_CHG_SHIFT 15
#define INTF_CWTOLW_COMMANDSTATUS_LANE0_RSVD1_MASK 0x00007800 /*!< RSVD*/
#define INTF_CWTOLW_COMMANDSTATUS_LANE0_RSVD1_SHIFT 11
#define INTF_CWTOLW_COMMANDSTATUS_LANE0_ANLINKUP_MASK 0x00000400 /*!< AN Link up*/
#define INTF_CWTOLW_COMMANDSTATUS_LANE0_ANLINKUP_SHIFT 10
#define INTF_CWTOLW_COMMANDSTATUS_LANE0_EGR_MISSION_STATE_MASK 0x00000200 /*!< Egr Mission State*/
#define INTF_CWTOLW_COMMANDSTATUS_LANE0_EGR_MISSION_STATE_SHIFT 9
#define INTF_CWTOLW_COMMANDSTATUS_LANE0_OPPOSITESIDECDRLOCK_MASK 0x00000100 /*!< Opposite side CDR lock */
#define INTF_CWTOLW_COMMANDSTATUS_LANE0_OPPOSITESIDECDRLOCK_SHIFT 8 
#define INTF_CWTOLW_COMMANDSTATUS_LANE0_RSVD2_MASK 0x000000E0 /*!< RSVD */
#define INTF_CWTOLW_COMMANDSTATUS_LANE0_RSVD2_SHIFT 5
#define INTF_CWTOLW_COMMANDSTATUS_LANE0_TXPI_RESTART_MASK 0x00000010 /*!< OC2LW txpi restart */
#define INTF_CWTOLW_COMMANDSTATUS_LANE0_TXPI_RESTART_SHIFT 4
#define INTF_CWTOLW_COMMANDSTATUS_LANE0_TURNONEGRCLK_MASK 0x00000008 /*!< TURNON Egr clock */
#define INTF_CWTOLW_COMMANDSTATUS_LANE0_TURNONEGRCLK_SHIFT 3 
#define INTF_CWTOLW_COMMANDSTATUS_LANE0_TURNONIGRCLK_MASK 0x00000004 /*!< TURNON Igr clock */
#define INTF_CWTOLW_COMMANDSTATUS_LANE0_TURNONIGRCLK_SHIFT 2 
#define INTF_CWTOLW_COMMANDSTATUS_LANE0_SQUELCHTX_MASK 0x00000002 /*!< Squelch Tx */
#define INTF_CWTOLW_COMMANDSTATUS_LANE0_SQUELCHTX_SHIFT 1 
#define INTF_CWTOLW_COMMANDSTATUS_LANE0_FORCECDRRESTART_MASK 0x00000001 /*!< Force CDR Restart */
#define INTF_CWTOLW_COMMANDSTATUS_LANE0_FORCECDRRESTART_SHIFT 0          

#define INTF_LWTOCW_COMMANDSTATUS_LANE0_LANE_CFG_STATUS_MASK 0x0000C000 /*!< LANE_CFG_STATUS*/
#define INTF_LWTOCW_COMMANDSTATUS_LANE0_LANE_CFG_STATUS_SHIFT 14
#define INTF_LWTOCW_COMMANDSTATUS_LANE0_GLOBALLOOPBACKENABLE_MASK 0x00002000 /*!< GlobalLoopback Enable*/
#define INTF_LWTOCW_COMMANDSTATUS_LANE0_GLOBALLOOPBACKENABLE_SHIFT 13
#define INTF_LWTOCW_COMMANDSTATUS_LANE0_CDR_LOCKED_MASK 0x00001000 /*!< CDR_LOCKED*/
#define INTF_LWTOCW_COMMANDSTATUS_LANE0_CDR_LOCKED_SHIFT 12
#define INTF_LWTOCW_COMMANDSTATUS_LANE0_TXPI_DONE_MASK 0x00000800 /*!< TXPI_DONE */
#define INTF_LWTOCW_COMMANDSTATUS_LANE0_TXPI_DONE_SHIFT 11 
#define INTF_LWTOCW_COMMANDSTATUS_LANE0_TXPI_CONV_FAILED_MASK 0x00000400 /*!< TxPI Converged Failed */
#define INTF_LWTOCW_COMMANDSTATUS_LANE0_TXPI_CONV_FAILED_SHIFT 10 
#define INTF_LWTOCW_COMMANDSTATUS_LANE0_ANGOODCHECKORANGOOD_MASK 0x00000200 /*!< AN Goodcheck or AN Good*/
#define INTF_LWTOCW_COMMANDSTATUS_LANE0_ANGOODCHECKORANGOOD_SHIFT 9
#define INTF_LWTOCW_COMMANDSTATUS_LANE0_ANMASTERENABLE_MASK 0x00000100 /*!< Opposite side CDR lock */
#define INTF_LWTOCW_COMMANDSTATUS_LANE0_ANMASTERENABLE_SHIFT 8 
#define INTF_LWTOCW_COMMANDSTATUS_LANE0_CDRPPM_MASK 0x000000FF /*!< CDR PPM */
#define INTF_LWTOCW_COMMANDSTATUS_LANE0_CDRPPM_SHIFT 0   

/****************************************************************************
 * CWTOBH Lane0 Register: INTF_CWTOBH_COMMANDSTATUS_LANE0
 ***************************************************************************/
#define INTF_CWTOBH_COMMANDSTATUS_LANE0_CMDSTAT_CHG_MASK 0x00008000 /*!< CMDSTAT Change*/
#define INTF_CWTOBH_COMMANDSTATUS_LANE0_CMDSTAT_CHG_SHIFT 15
#define INTF_CWTOBH_COMMANDSTATUS_LANE0_RSVD1_MASK 0x00007800 /*!< RSVD*/
#define INTF_CWTOBH_COMMANDSTATUS_LANE0_RSVD1_SHIFT 11
#define INTF_CWTOBH_COMMANDSTATUS_LANE0_ANLINKUP_MASK 0x00000400 /*!< AN Link up*/
#define INTF_CWTOBH_COMMANDSTATUS_LANE0_ANLINKUP_SHIFT 10
#define INTF_CWTOBH_COMMANDSTATUS_LANE0_EGR_MISSION_STATE_MASK 0x00000200 /*!< Egr Mission State*/
#define INTF_CWTOBH_COMMANDSTATUS_LANE0_EGR_MISSION_STATE_SHIFT 9
#define INTF_CWTOBH_COMMANDSTATUS_LANE0_OPPOSITESIDECDRLOCK_MASK 0x00000100 /*!< Opposite side CDR lock */
#define INTF_CWTOBH_COMMANDSTATUS_LANE0_OPPOSITESIDECDRLOCK_SHIFT 8 
#define INTF_CWTOBH_COMMANDSTATUS_LANE0_RSVD2_MASK 0x000000E0 /*!< RSVD */
#define INTF_CWTOBH_COMMANDSTATUS_LANE0_RSVD2_SHIFT 5
#define INTF_CWTOBH_COMMANDSTATUS_LANE0_TXPI_RESTART_MASK 0x00000010 /*!< Txpi Done */
#define INTF_CWTOBH_COMMANDSTATUS_LANE0_TXPI_RESTART_SHIFT 4
#define INTF_CWTOBH_COMMANDSTATUS_LANE0_TURNONEGRCLK_MASK 0x00000008 /*!< TURNON Egr clock */
#define INTF_CWTOBH_COMMANDSTATUS_LANE0_TURNONEGRCLK_SHIFT 3 
#define INTF_CWTOBH_COMMANDSTATUS_LANE0_TURNONIGRCLK_MASK 0x00000004 /*!< TURNON Igr clock */
#define INTF_CWTOBH_COMMANDSTATUS_LANE0_TURNONIGRCLK_SHIFT 2 
#define INTF_CWTOBH_COMMANDSTATUS_LANE0_SQUELCHTX_MASK 0x00000002 /*!< Squelch Tx */
#define INTF_CWTOBH_COMMANDSTATUS_LANE0_SQUELCHTX_SHIFT 1 
#define INTF_CWTOBH_COMMANDSTATUS_LANE0_FORCECDRRESTART_MASK 0x00000001 /*!< Force CDR Restart */
#define INTF_CWTOBH_COMMANDSTATUS_LANE0_FORCECDRRESTART_SHIFT 0

/****************************************************************************
 * BHTOCW Lane0 Register: INTF_BHTOCW_COMMANDSTATUS_LANE0
 ***************************************************************************/
#define INTF_BHTOCW_COMMANDSTATUS_LANE0_LANE_CFG_STATUS_MASK 0x0000C000 /*!< LANE_CFG_STATUS*/
#define INTF_BHTOCW_COMMANDSTATUS_LANE0_LANE_CFG_STATUS_SHIFT 14
#define INTF_BHTOCW_COMMANDSTATUS_LANE0_GLOBALLOOPBACKENABLE_MASK 0x00002000 /*!< GlobalLoopback Enable*/
#define INTF_BHTOCW_COMMANDSTATUS_LANE0_GLOBALLOOPBACKENABLE_SHIFT 13
#define INTF_BHTOCW_COMMANDSTATUS_LANE0_CDR_LOCKED_MASK 0x00001000 /*!< CDR_LOCKED*/
#define INTF_BHTOCW_COMMANDSTATUS_LANE0_CDR_LOCKED_SHIFT 12
#define INTF_BHTOCW_COMMANDSTATUS_LANE0_TXPI_DONE_MASK 0x00000800 /*!< TXPI_DONE */
#define INTF_BHTOCW_COMMANDSTATUS_LANE0_TXPI_DONE_SHIFT 11 
#define INTF_BHTOCW_COMMANDSTATUS_LANE0_TXPI_CONV_FAILED_MASK 0x00000400 /*!< TXPI_CONV_FAILED */
#define INTF_BHTOCW_COMMANDSTATUS_LANE0_TXPI_CONV_FAILED_SHIFT 10 
#define INTF_BHTOCW_COMMANDSTATUS_LANE0_ANGOODCHECKORANGOOD_MASK 0x00000200 /*!< AN Goodcheck or AN Good*/
#define INTF_BHTOCW_COMMANDSTATUS_LANE0_ANGOODCHECKORANGOOD_SHIFT 9
#define INTF_BHTOCW_COMMANDSTATUS_LANE0_ANMASTERENABLE_MASK 0x00000100 /*!< Opposite side CDR lock */
#define INTF_BHTOCW_COMMANDSTATUS_LANE0_ANMASTERENABLE_SHIFT 8 
#define INTF_BHTOCW_COMMANDSTATUS_LANE0_CDRPPM_MASK 0x000000FF /*!< CDR PPM */
#define INTF_BHTOCW_COMMANDSTATUS_LANE0_CDRPPM_SHIFT 0 

/****************************************************************************
 * Octal Core to LW Communications per LW lane (octal_core.GP_REGS3_[8..15])
 ***************************************************************************/
typedef union intf_cwtolw_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t cmdstat_chg:1;
        uint16_t rsvd1:4;
        uint16_t an_linkup:1;
        uint16_t egress_mission:1;
        uint16_t oppsite_side_cdrlock:1;
        uint16_t rsvd2:3;
        uint16_t txpi_restart:1;
        uint16_t turnon_egr_clock:1;
        uint16_t turnon_igr_clock:1;
        uint16_t squelch_tx:1;
        uint16_t force_cdr_restart:1;
#else
        uint16_t force_cdr_restart:1;
        uint16_t squelch_tx:1;
        uint16_t turnon_igr_clock:1;
        uint16_t turnon_egr_clock:1;
        uint16_t txpi_restart:1;
        uint16_t rsvd2:3;
        uint16_t oppsite_side_cdrlock:1;
        uint16_t egress_mission:1;
        uint16_t an_linkup:1;
        uint16_t rsvd1:4;
        uint16_t cmdstat_chg:1;
#endif
    } fields;
} intf_cwtolw_t;

/****************************************************************************
 * LW to Octal Core Communications per LW lane (octal_core.GP_REGS3_[16..23])
 ***************************************************************************/
typedef union intf_lwtocw_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t lane_cfg_stat:2;
        uint16_t gloopback_en:1;
        uint16_t cdr_lock:1;
        uint16_t txpi_done:1;
        uint16_t txpi_conv_fail:1;
        uint16_t cdr_ppm:10;
#else
        uint16_t cdr_ppm:10;
        uint16_t txpi_conv_fail:1;
        uint16_t txpi_done:1;
        uint16_t cdr_lock:1;
        uint16_t gloopback_en:1;
        uint16_t lane_cfg_stat:2;
#endif
    } fields;
} intf_lwtocw_t;

/****************************************************************************
 * Octal Core to BH Communications per BH lane (octal_core.GP_REGS3_[24..39])
 ***************************************************************************/
typedef union intf_cwtobh_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t cmdstat_chg:1;
        uint16_t rsvd1:4;
        uint16_t an_linkup:1;
        uint16_t egress_mission:1;
        uint16_t oppsite_side_cdrlock:1;
        uint16_t rsvd2:3;
        uint16_t txpi_restart:1;
        uint16_t turnon_egr_clock:1;
        uint16_t turnon_igr_clock:1;
        uint16_t squelch_tx:1;
        uint16_t force_cdr_restart:1;
#else
        uint16_t force_cdr_restart:1;
        uint16_t squelch_tx:1;
        uint16_t turnon_igr_clock:1;
        uint16_t turnon_egr_clock:1;
        uint16_t txpi_restart:1;
        uint16_t rsvd2:3;
        uint16_t oppsite_side_cdrlock:1;
        uint16_t egress_mission:1;
        uint16_t an_linkup:1;
        uint16_t rsvd1:4;
        uint16_t cmdstat_chg:1;
#endif
    } fields;
} intf_cwtobh_t;

/****************************************************************************
 * BH to Octal Core Communications per BH lane (octal_core.GP_REGS3_[40..55])
 ***************************************************************************/
typedef union intf_bhtocw_u {
    uint16_t words;
    struct {
#ifdef CAPI_IS_BIG_ENDIAN
        uint16_t lane_cfg_stat:2;
        uint16_t gloopback_en:1;
        uint16_t cdr_lock:1;
        uint16_t txpi_done:1;
        uint16_t txpi_conv_fail:1;
        uint16_t cdr_ppm:10;
#else
        uint16_t cdr_ppm:10;
        uint16_t txpi_conv_fail:1;
        uint16_t txpi_done:1;
        uint16_t cdr_lock:1;
        uint16_t gloopback_en:1;
        uint16_t lane_cfg_stat:2;
#endif
    } fields;
} intf_bhtocw_t;

#endif

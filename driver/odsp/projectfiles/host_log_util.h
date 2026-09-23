/**
 * @file     host_log_util.h
 * @author  
 * @date     02-12-2018
 * @version 1.0
 *
 * @property 
 * $Copyright: (c) 2019 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * @brief    brief description of the block source file, sequences/flow/etc.
 *
 * @section  description
 *
 */
#ifndef HOST_LOG_UTIL_H
#define HOST_LOG_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#define FW_RESPONSE        2                 /**< Print the configuration values     */
#define PRINT_ALL          3                 /**< Print all the configuration values */

#ifdef __linux
#define CAPI_DEBUG_PRINTF capi_printf
#else
#define CAPI_DEBUG_PRINTF printf
#endif

#ifndef __linux
#define PHYMOD_DIAG_OUT(stuff_)     printf stuff_ ;
#endif

int capi_printf(const char * format, ...);

#if !defined(__linux) || defined(LINUX_CLI)
#define PHYMOD_DIAG_OUT(stuff_)  printf stuff_ ;
#endif

#define PRINT_ERR_RETURN(expr)  \
    do  { \
        int __hsip_err; \
        *(__HSIP_ERR) = 0; \
        PHYMOD_DIAG_OUT(expr); \
        if (*(__HSIP_ERR) != 0) \
            {   return  RR_ERROR_MDIO; }\
        (void)__hsip_err; \
    }   while(0)

#define PRINT_DIAG_OUT(expr)  \
    do  { \
        int __hsip_err; \
        *(__HSIP_ERR) = 0; \
        PHYMOD_DIAG_OUT(expr); \
        (void)__hsip_err; \
    }   while(0)

int  util_log(const char* fmt, ...);
void util_log_time(char* module, char* logLevel);

void util_log_func(char* func, capi_phy_info_t* phy_info_ptr);
void util_log_init(void);
void util_log_close(void);

#define CAPI_LOG_INFO_LEVEL     0x01
#define CAPI_LOG_WARN_LEVEL     0x02
#define CAPI_LOG_ERROR_LEVEL    0x04

#ifdef ENABLE_CAPI_LOG
#define CAPI_LOG_LEVEL    (CAPI_LOG_INFO_LEVEL | CAPI_LOG_WARN_LEVEL | CAPI_LOG_ERROR_LEVEL )
#else
/*#ifdef CAPI_LOG_DISABLED */
#define CAPI_LOG_LEVEL    0
/*
#else
#define CAPI_LOG_LEVEL    CAPI_LOG_ERROR_LEVEL
#endif
*/
#endif

#ifdef CAPI_CONFIG_RUNTIME_LOG
void capi_runtime_log_enable(int enable);
#endif

/* log macros for cAPI. Can use hsip_rd macros in log directly */
#define __CAPI_LOG__(format, loglevel, ...)  do {\
    int __hsip_err; \
    util_log_time("cAPI", loglevel); \
    util_log(format, ##__VA_ARGS__); \
    (void)__hsip_err; \
} while(0)

#define __CAPI_LOG_DATA__(format, ...)  do {\
    util_log("\t"); \
    util_log(format, ##__VA_ARGS__); \
} while(0)

#if (CAPI_LOG_LEVEL != 0)
#define CAPI_LOG_INIT()     util_log_init()
#define CAPI_LOG_CLOSE()    util_log_close()
#else
#define CAPI_LOG_INIT()
#define CAPI_LOG_CLOSE()
#endif

#if (CAPI_LOG_LEVEL & CAPI_LOG_INFO_LEVEL)
#define CAPI_LOG_INFO(format, ...)      __CAPI_LOG__(format, "I", ##__VA_ARGS__)
#define CAPI_LOG_FUNC(func, phy_info)   util_log_func(func, phy_info)
#define CAPI_LOG_DATA(format, ...)      __CAPI_LOG_DATA__(format, ##__VA_ARGS__)
#else
#define CAPI_LOG_INFO(format, ...)
#define CAPI_LOG_FUNC(func, phy_info)
#define CAPI_LOG_DATA(format, ...)
#endif

#if (CAPI_LOG_LEVEL & CAPI_LOG_WARN_LEVEL)
#define CAPI_LOG_WARN(format, ...)   __CAPI_LOG__(format, "W", ##__VA_ARGS__)
#else
#define CAPI_LOG_WARN(format, ...)
#endif

#if (CAPI_LOG_LEVEL & CAPI_LOG_ERROR_LEVEL)
#define CAPI_LOG_ERROR(format, ...)   __CAPI_LOG__(format, "E", ##__VA_ARGS__)
#else
#define CAPI_LOG_ERROR(format, ...)
#endif

#if (CAPI_LOG_LEVEL != 0)
/* utility functions to convert enum value to a string */
const char* util_core_ip_str(core_ip_t core_ip);
const char* util_capi_direction_str(capi_direction_t dir);
const char* util_capi_prbs_type_str(capi_pattern_gen_mon_type_t ptype);
const char* util_capi_bh_prbs_poly_str(capi_prbs_poly_t poly);
const char* util_capi_lw_prbs_mon_poly_str(capi_prbs_poly_t poly);
const char* util_capi_lw_prbs_gen_poly_str(capi_prbs_poly_t poly);
const char* util_capi_fec_prbs_gen_poly_str(capi_fec_prbs_gen_poly_t poly);
const char* util_capi_enable_str(capi_enable_t enable);
const char* util_capi_loopback_mode_str(capi_loopback_mode_t mode);
const char* util_capi_polarity_str(capi_polarity_action_t polarity);
const char* util_capi_reset_mode_str(capi_reset_mode_t mode);
const char* util_capi_download_mode_str(capi_download_mode_t mode);
const char* util_capi_media_type_str(capi_media_type_t type);
const char* util_capi_ref_clk_frq_mode_str(capi_ref_clk_frq_mode_t ref);
const char* util_capi_function_mode_str(capi_function_mode_t mode);
const char* util_capi_modulation_str(capi_modulation_t mod);
const char* util_capi_lane_data_rate_str(capi_lane_data_rate_t lrate);
const char* util_capi_fec_type_str(capi_fec_type_t fec);
const char* util_capi_switch_str(capi_switch_t sw);
//const char* util_capi_bh_txfir_tap_enable_str(capi_txfir_tap_t tap);
const char* util_capi_lane_ctrl_str(capi_lane_ctrl_info_t lane_ctrl);

/* log structure values */
void log_capi_config_info(capi_config_info_t* config_ptr, uint8_t cfg_cmd);
void log_capi_config_ex_info(capi_config_ex_info_t * config_ex_info_ptr, uint8_t cfg_cmd);
void log_capi_polarity_info(capi_polarity_info_t* polarity_info_ptr, uint8_t cfg_cmd);
void log_capi_tx_info(lane_tx_info_t* lane_tx_info, uint8_t cfg_cmd);
void log_capi_rx_info(lane_rx_info_t* lane_rx_info, uint8_t cfg_cmd);
void log_lane_ctrl_info(capi_lane_ctrl_info_t* lane_ctrl_info_ptr, uint8_t cfg_cmd);
void log_capi_command_info(capi_command_info_t* cmd_inf_ptr, uint8_t cfg_cmd);
void log_capi_prbs_status_info(capi_prbs_status_t* prbs_status, uint8_t cfg_cmd);
void log_capi_prbs_type (capi_pattern_gen_mon_type_t* prbs_type);
void log_capi_prbs_info(capi_prbs_info_t* prbs_info_ptr, uint8_t cfg_cmd);
void log_capi_lnktrn_info(capi_lnktrn_info_t* link_trn_info_ptr, uint8_t cfg_cmd);
void log_capi_lpbk_config(capi_loopback_info_t* loopback_ptr, uint8_t cfg_cmd);
void log_capi_lane_info(capi_lane_info_t* lane_info_ptr, uint8_t cfg_cmd);
void log_capi_prbs_err_inject(capi_prbs_err_inject_t* err_inj_ptr);
void log_capi_diag_lane_status(capi_diag_lane_status_t* diag_status_ptr, uint8_t cfg_cmd);
void log_capi_config_fec_term(uint16_t fec_term);
void log_capi_lane_swap_info(lane_swap_info_t* lane_swap_info_ptr);

#define CAPI_LOG_CONFIG_INFO(x)      log_capi_config_info(x, PRINT_ALL)
#define CAPI_LOG_CONFIG_EX_INFO(x)   log_capi_config_ex_info(x, PRINT_ALL)
#define CAPI_LOG_CONFIG_FEC_TERM(x)  log_capi_config_fec_term(x)
#define CAPI_LOG_POLARITY_INFO(x)    log_capi_polarity_info(x, PRINT_ALL)
#define CAPI_LOG_TX_INFO(x)          log_capi_tx_info(x, PRINT_ALL)
#define CAPI_LOG_RX_INFO(x)          log_capi_rx_info(x, PRINT_ALL)
#define CAPI_LOG_LANE_CTRL_INFO(x)   log_lane_ctrl_info(x, PRINT_ALL)
#define CAPI_LOG_PRBS_INFO(x)        log_capi_prbs_info(x, PRINT_ALL)
#define CAPI_LOG_PRBS_TYPE(x)        log_capi_prbs_type(x)
#define CAPI_LOG_PRBS_STATUS(x)      log_capi_prbs_status_info(x, PRINT_ALL)
#define CAPI_LOG_LNKTRN_INFO(x)      log_capi_lnktrn_info(x, PRINT_ALL)
#define CAPI_LOG_AN_CONFIG(x)        log_capi_an_config(x)
#define CAPI_LOG_LPBK_INFO(x)        log_capi_lpbk_config(x, PRINT_ALL)
#define CAPI_LOG_LANE_CONFIG_INFO(x) log_capi_lane_config_info(x)
#define CAPI_LOG_LANE_INFO(x)        log_capi_lane_info(x, PRINT_ALL)
#define CAPI_LOG_LANE_SWAP_INFO(x)   log_capi_lane_swap_info(x)
#define CAPI_LOG_COMMAND_INFO(x, cmd) log_capi_command_info(x, cmd)

#else
#define CAPI_LOG_CONFIG_INFO(x)     
#define CAPI_LOG_CONFIG_EX_INFO(x)     
#define CAPI_LOG_CONFIG_FEC_TERM(x)
#define CAPI_LOG_POLARITY_INFO(x)   
#define CAPI_LOG_TX_INFO(x)         
#define CAPI_LOG_RX_INFO(x)         
#define CAPI_LOG_LANE_CTRL_INFO(x)  
#define CAPI_LOG_PRBS_INFO(x)       
#define CAPI_LOG_PRBS_STATUS(x)     
#define CAPI_LOG_PRBS_TYPE(x)       
#define CAPI_LOG_LNKTRN_INFO(x)     
#define CAPI_LOG_AN_CONFIG(x)       
#define CAPI_LOG_LPBK_INFO(x)
#define CAPI_LOG_LANE_STATUS(x)
#define CAPI_LOG_LANE_CONFIG_INFO(x)
#define CAPI_LOG_LANE_INFO(x)
#define CAPI_LOG_LANE_SWAP_INFO(x)
#define CAPI_LOG_COMMAND_INFO(x, cmd)
#endif

/* Error-check a blackhawk function call, returning RR_ERROR if it fails. */
#define CAPI_CLIENT_EFUN(expr)                            \
    do {                                                  \
        err_code_t __err = (err_code_t)expr;              \
        if (__err != ERR_CODE_NONE) {                     \
            return RR_ERROR;                              \
        }                                                 \
    } while (0)

/* Error-checking on BH read macro/function
 * returning RR_ERROR_CLIENT_BH_API_READ_ERROR if it fails. */
#ifdef ENABLE_BH_API_ERR_CHK
#define CAPI_ESTM(expr) \
    do  { \
        err_code_t srds_err_code; \
        *(__ERR) = ERR_CODE_NONE; \
        (expr); \
        if (*(__ERR) != ERR_CODE_NONE) { \
            blackhawk7_centenario_error_report(sa__, *(__ERR)); \
            return RR_ERROR_CLIENT_API_INVOKE_ERROR; \
        } \
        (void)srds_err_code; \
    }   while(0)
#else
#define CAPI_ESTM(expr) \
    do  { \
        err_code_t srds_err_code; \
        *(__ERR) = ERR_CODE_NONE; \
        (expr); \
        (void)srds_err_code; \
    }   while(0)
#endif
    
/**
* Used for all child functions
* If child function return RR_ERROR_*; then return immediately from parent function as well
* otherwise it's either RR_WARNING_* or RR_SUCCESS
* if not RR_ERROR_*; continue the processing
*/
#define ERR_FUNC(expr)  \
    do  { \
        int __hsip_func_err; \
        *(__HSIP_FUNC_ERR) = 0; \
        *(__HSIP_FUNC_ERR) = (expr); \
        if ((*(__HSIP_FUNC_ERR) != RR_SUCCESS) && (*(__HSIP_FUNC_ERR) < RR_LAST_ERROR)) \
        {   return  (return_result_t)(*(__HSIP_FUNC_ERR)); }\
        (void)__hsip_func_err; \
    }   while(0)


#ifdef __cplusplus
}
#endif

#endif /* ifndef HOST_LOG_UTIL_H */

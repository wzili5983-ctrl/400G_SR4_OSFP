/**
 *
 * @file     host_log_util.c
 * @author  
 * @date     02-03-2018
 * @version 1.0
 *
 * @property    $ Copyright: (c) 2018 Broadcom Limited All Rights Reserved $
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
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include "common_def.h"
#include "chip_mode_def.h"
#include "cw_def.h"
#include "capi_def.h"
#include "common_util.h"
#include "host_log_util.h"

/* enum to string conversion function macros */
#define ENUM_TO_STRING_FUNC_START(funcName, pType) \
    char* funcName(pType pValue) \
{ \
    switch (pValue) \
    {

#define ENUM_TO_STRING_CASE_BLK(pValue, pName) \
        case (pValue): \
        { \
            static char ret_str[] = pName; \
            return ret_str; \
        } \
        break;

#define ENUM_TO_STR_CASE(pValue) ENUM_TO_STRING_CASE_BLK(pValue, #pValue)

#define ENUM_TO_STRING_FUNC_END(pDefault) \
        default: \
        { \
            static char ret_str[] = pDefault; \
            return ret_str; \
        } \
    } \
    \
    return ""; \
}

ENUM_TO_STRING_FUNC_START(get_phy_command_id_str, phy_command_id_t)
    ENUM_TO_STR_CASE(COMMAND_ID_ALL)
    ENUM_TO_STR_CASE(COMMAND_ID_SET_TXPI_OVERRIDE)
    ENUM_TO_STR_CASE(COMMAND_ID_SET_LOW_POWER_MODE)
    ENUM_TO_STR_CASE(COMMAND_ID_GET_LOW_POWER_MODE)
    ENUM_TO_STR_CASE(COMMAND_ID_SET_POLARITY)
    ENUM_TO_STR_CASE(COMMAND_ID_GET_POLARITY)
    ENUM_TO_STR_CASE(COMMAND_ID_SET_LANE_CTRL_INFO)
    ENUM_TO_STR_CASE(COMMAND_ID_GET_LANE_CTRL_INFO)
    ENUM_TO_STR_CASE(COMMAND_ID_SET_CONFIG_INFO)
    ENUM_TO_STR_CASE(COMMAND_ID_GET_CONFIG_INFO)
    ENUM_TO_STR_CASE(COMMAND_ID_SET_LOOPBACK_INFO)
    ENUM_TO_STR_CASE(COMMAND_ID_GET_LOOPBACK_INFO)
    ENUM_TO_STR_CASE(COMMAND_ID_SET_PRBS_INFO)
    ENUM_TO_STR_CASE(COMMAND_ID_GET_PRBS_INFO)
    ENUM_TO_STR_CASE(COMMAND_ID_CLEAR_PRBS_STATUS)
    ENUM_TO_STR_CASE(COMMAND_ID_GET_PRBS_STATUS)
    ENUM_TO_STR_CASE(COMMAND_ID_INJ_PRBS_ERROR)
    ENUM_TO_STR_CASE(COMMAND_ID_DIAG_LANE_STATUS)
    ENUM_TO_STR_CASE(COMMAND_ID_SET_LANE_CONFIG_INFO)
    ENUM_TO_STR_CASE(COMMAND_ID_GET_LANE_CONFIG_INFO)
    ENUM_TO_STR_CASE(COMMAND_ID_GET_LANE_INFO)
    ENUM_TO_STR_CASE(COMMAND_ID_SET_ARCHIVE_INFO)
    ENUM_TO_STR_CASE(COMMAND_ID_GET_ARCHIVE_INFO)
    ENUM_TO_STR_CASE(COMMAND_ID_GET_CMIS_INFO)
    ENUM_TO_STR_CASE(COMMAND_ID_CLIENT_SIDE)
    ENUM_TO_STR_CASE(COMMAND_ID_SET_CLIENT_TX_DRIVER_VOLTAGE)
    ENUM_TO_STR_CASE(COMMAND_ID_LW_SIDE)
    ENUM_TO_STR_CASE(COMMAND_ID_CW_SIDE)
    ENUM_TO_STR_CASE(COMMAND_ID_SET_CW_RPTR_FEC_MON_CONFIG)
    ENUM_TO_STR_CASE(COMMAND_ID_SET_RECOVERED_CLOCK_INFO)
    ENUM_TO_STR_CASE(COMMAND_ID_GET_RECOVERED_CLOCK_INFO)
    ENUM_TO_STR_CASE(COMMAND_ID_SET_GPIO_INFO)
    ENUM_TO_STR_CASE(COMMAND_ID_GET_GPIO_INFO)
ENUM_TO_STRING_FUNC_END("Not_Found")


/** 
 * NOTE: 
 * 1. Assumption made that all the enums starts with value 0
 * and can be indexed directly with its value.
 * 2. Update the respective enum string array, when a enum is changed.
 * 3. enum having different values cannot use array of strings and may
 *    use switch case of if blocks to give the string values.
 */


#if (CAPI_LOG_LEVEL != 0)
static char g_err_str[32] = "ERROR";
FILE *f_hdnl = NULL;

#define VALIDATE_INPUT(val, enum_max)    \
    if ((((int)val) < 0) || (((int)val) > enum_max)) {\
        sprintf(g_err_str, "ERROR (val 0x%x)", val); \
        return g_err_str; \
    }
#endif

#ifdef CAPI_CONFIG_RUNTIME_LOG
static int g_runtime_log_enable = 0;

void capi_runtime_log_enable(int enable)
{
    g_runtime_log_enable = enable;
}
#endif

/**
 * @brief    capi_print(const char * format, ...)
 * @details  To fake the printf
 *
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  format
 * @param[in]  ....
 *
 * @return     returns the performance result of the called methode/function
 */
int capi_printf(const char * format, ...)
{
#if CAPI_DSP_PLANNED
    int n = 0;
    {
        va_list args;
        util_memset(&args, 0x00, sizeof(va_list));

        va_start(args, format);
        n = vprintf(format, args);        
        va_end(args);        
    }
    return n;
#else
    return 0;
#endif  
}

#if (CAPI_LOG_LEVEL != 0)
/**
 * @brief      util_log_init()
 * @details     opens log file in the current directory
 *              File name format:
 *              capi_YYMMDD_HHMMSS.log
 *              If failed to open the log file, the logs will be printed
 *              on stdout.
 */
void util_log_init()
{
    char   file_name[64];
    time_t rawtime;
    struct tm* timeinfo;
    char   time_str[20];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    util_memset(&file_name[0], 0, sizeof(file_name));
    util_memset(&time_str[0], 0, sizeof(time_str));

    if (NULL != timeinfo)
        strftime(time_str, 20, "%Y%m%d_%H%M%S", timeinfo);

    sprintf(file_name, "capi_epdm_%s.log", time_str);

    f_hdnl = fopen(file_name, "w");
    if (NULL == f_hdnl)
        printf("Failed to open file %s\n", file_name);
}

/**
 * @brief      util_log_close()
 * @details    Close the log file
 *
 */
void util_log_close()
{
    if (f_hdnl)
        fclose(f_hdnl);
}

/**
 * @brief     util_log
 * @details   logs given format onto stdout
 *
 * @return    int
 */
int util_log(const char* fmt, ...)
{
#ifdef CAPI_CONFIG_RUNTIME_LOG
    if (g_runtime_log_enable)
    {
#endif
        va_list argptr;
        va_start(argptr, fmt);
        if (NULL == f_hdnl)
            vfprintf(stdout, fmt, argptr);
        else
            vfprintf(f_hdnl, fmt, argptr);
        va_end(argptr);
#ifdef CAPI_CONFIG_RUNTIME_LOG
    }
#endif

    return 0;
}

/**
 * @brief      util_log_time(char* module, char* logLevel)
 * @details    logs time and log level
 *
 * @param      logLevel
 */
void util_log_time(char* module, char* logLevel)
{
#ifdef CAPI_CONFIG_RUNTIME_LOG
    if (g_runtime_log_enable)
    {
#endif
        time_t rawtime;
        struct tm* timeinfo;
        char   time_str[20];

        time(&rawtime);
        timeinfo = localtime(&rawtime);

        /*strftime(time_str, 20, "%Y-%m-%d %H:%M:%S", timeinfo);*/
        util_memset(&time_str[0], 0, sizeof(time_str));

        if (NULL != timeinfo)
            strftime(time_str, 20, "%H:%M:%S", timeinfo);
        util_log("[%s][%s][%s] ", module, logLevel, time_str);
#ifdef CAPI_CONFIG_RUNTIME_LOG
    }
#endif
}

/**
 * @brief      util_log_func(char* func, capi_phy_info_t* phy_info_ptr)
 * @details    logs phy_info details along with function name
 *
 * @param      func
 * @param      phy_info_ptr
 */
void util_log_func(char* func, capi_phy_info_t* phy_info_ptr)
{
#ifdef CAPI_CONFIG_RUNTIME_LOG
    if (g_runtime_log_enable)
    {
#endif
        time_t rawtime;
        struct tm* timeinfo;
        char   time_str[20];

        time(&rawtime);
        timeinfo = localtime(&rawtime);

        /*strftime(time_str, 20, "%Y-%m-%d %H:%M:%S", timeinfo); */
        util_memset(&time_str[0], 0, sizeof(time_str));

        if (NULL != timeinfo)
            strftime(time_str, 20, "%H:%M:%S", timeinfo);

        util_log("[cAPI][I][%s] %s: phy_id %d core_ip %s lane_mask 0x%x\n",
                time_str, func, phy_info_ptr->phy_id,
                util_core_ip_str(phy_info_ptr->core_ip),
                phy_info_ptr->lane_mask);
#ifdef CAPI_CONFIG_RUNTIME_LOG
    }
#endif
}

static char cmd_type_text[3][50] = {
    "SET",
    "GET",
    "RESP"
};

const char* util_cmd_type_str(capi_command_type_t cmd_type)
{
    VALIDATE_INPUT(cmd_type, RESP);
    return cmd_type_text[cmd_type];
}

/**< core_ip_t num text for logging purpose */
static char core_ip_text[CORE_IP_KP4_KR4_FEC_DEC+1][32] = {
    "CORE_IP_ALL",                                        /*!< Core IP ALL                          */
    "CORE_IP_CW",                                         /*!< Core IP Core Wrapper                 */
    "CORE_IP_MEDIA_DSP",                                  /*!< Core IP Media Side Handled by DSP    */
    "CORE_IP_MEDIA_SERDES",                               /*!< Core IP Media Side Handled by SERDES */
    "CORE_IP_HOST_DSP",                                   /*!< Core IP Host Side Handled by DSP     */
    "CORE_IP_HOST_SERDES",                                /*!< Core IP Host Side Handled by SERDES  */
    "CORE_IP_KP4_KR4_FEC_DEC"                             /*!< Core IP KP4, KR4 Encoder Decoder     */
};

const char* util_core_ip_str(core_ip_t core_ip)
{
    VALIDATE_INPUT(core_ip, CORE_IP_KP4_KR4_FEC_DEC);
    return core_ip_text[core_ip];
}

/**< capi_direction_t enum to string */
static char capi_direction_text[DIR_BOTH+1][20] = {
    "DIR_INGRESS",
    "DIR_EGRESS",
    "DIR_BOTH"
};
const char* util_capi_direction_str(capi_direction_t dir)
{
    VALIDATE_INPUT(dir, DIR_BOTH);
    return capi_direction_text[dir];
}

/**< enum capi_pattern_gen_mon_type_t to string */
static char capi_pattern_gen_mon_type_text[CAPI_PCS_SCRM_IDLE_GENERATOR+1][50] = {
    "CAPI_PRBS_TYPE_NONE",
    "CAPI_PRBS_GENERATOR",
    "CAPI_PRBS_MONITOR",
    "CAPI_PRBS_GEN_MON",
    "CAPI_PRBS_SSPRQ_GENERATOR",
    "CAPI_PRBS_SSPRQ_MONITOR",
    "CAPI_PRBS_SSPRQ_GEN_MON",
    "CAPI_PRBS_Q_PRBS_13_GENERATOR",
    "CAPI_PRBS_SQUARE_WAVE_GENERATOR",
    "CAPI_PRBS_TX_LINEARITY_GENERATOR",
    "CAPI_PRBS_SHARED_TX_PATTERN_GENERATOR",
    "CAPI_PRBS_KP4_HOST_GENERATOR",
    "CAPI_PRBS_KP4_HOST_MONITOR",
    "CAPI_PRBS_KP4_HOST_GEN_MON",
    "CAPI_PRBS_KP4_MEDIA_GENERATOR",
    "CAPI_PRBS_KP4_MEDIA_MONITOR",
    "CAPI_PRBS_KP4_MEDIA_GEN_MON",
    "CAPI_PRBS_JP03B",
    "CAPI_PRBS_STAIRCASE_PATTERN",
    "CAPI_PRBS_CW_GENERATOR",
    "CAPI_PRBS_CW_MONITOR",
    "CAPI_PCS_SCRM_IDLE_GENERATOR"
};

const char* util_capi_prbs_type_str(capi_pattern_gen_mon_type_t ptype)
{
    VALIDATE_INPUT(ptype, CAPI_PCS_SCRM_IDLE_GENERATOR);
    return capi_pattern_gen_mon_type_text[ptype];
}


/**< enum capi_bh_prbs_poly_t to string */
static char capi_bh_prbs_poly_text[CAPI_BH_PRBS_PRBS_UNKNOWN][50] = {
    "CAPI_BH_PRBS_POLY_7",
    "CAPI_BH_PRBS_POLY_9",
    "CAPI_BH_PRBS_POLY_11",
    "CAPI_BH_PRBS_POLY_15",
    "CAPI_BH_PRBS_POLY_23",
    "CAPI_BH_PRBS_POLY_31",
    "CAPI_BH_PRBS_POLY_58",
    "CAPI_BH_PRBS_POLY_49",
    "CAPI_BH_PRBS_POLY_10",
    "CAPI_BH_PRBS_POLY_20",
    "CAPI_BH_PRBS_POLY_13",
    "CAPI_BH_PRBS_USER_40_BIT_REPEAT",
    "CAPI_BH_PRBS_PRBS_AUTO_DETECT"
};
const char* util_capi_bh_prbs_poly_str(capi_prbs_poly_t poly)
{
    //VALIDATE_INPUT(poly.poly_type, CAPI_PRBS_POLY_13);
    return capi_bh_prbs_poly_text[poly.poly_type];
}

const char* util_capi_lw_prbs_mon_poly_str(capi_prbs_poly_t poly)
{
    static char poly_str[50];
    VALIDATE_INPUT(poly.poly_type, CAPI_PRBS_POLY_58);
    //return capi_lw_prbs_mon_poly_text[poly];
    VALIDATE_INPUT(poly.poly_type, CAPI_PRBS_POLY_58);
    util_memset(poly_str, 0, sizeof(poly_str));
    sprintf(poly_str, "CAPI_PRBS_POLY_%d", poly.poly_type);
    return poly_str;
}

/**< enum capi_lw_prbs_gen_poly_t to string */
const char* util_capi_lw_prbs_gen_poly_str(capi_prbs_poly_t poly)
{
    static char poly_str[50];
    VALIDATE_INPUT(poly.poly_type, CAPI_PRBS_POLY_58);
    util_memset(poly_str, 0, sizeof(poly_str));
    sprintf(poly_str, "CAPI_LW_PRBS_GEN_POLY_%d", poly.poly_type);
    return poly_str;
    //return capi_lw_prbs_gen_poly_text[poly];
}

static char capi_fec_prbs_gen_poly_text[CAPI_FEC_PRBS_GEN_POLY_COUNT][50] = {
    "CAPI_FEC_PRBS_GEN_POLY_58",
    "CAPI_FEC_PRBS_GEN_POLY_31"
};
const char* util_capi_fec_prbs_gen_poly_str(capi_fec_prbs_gen_poly_t poly)
{
    VALIDATE_INPUT(poly, CAPI_FEC_PRBS_GEN_POLY_COUNT);
    return capi_fec_prbs_gen_poly_text[poly];
}

/**< enum capi_enable_t to string */
static char capi_enable_text[CAPI_ENABLE+1][20] = {
    "CAPI_DISABLE",
    "CAPI_ENABLE"
};

const char* util_capi_enable_str(capi_enable_t enable)
{
    VALIDATE_INPUT(enable, CAPI_ENABLE);
    return capi_enable_text[enable];
}

/**< enum capi_loopback_mode_t to string */
static char loopback_mode_text[CAPI_LAST_LOOPBACK_MODE][50] = {
    "CAPI_GLOBAL_LOOPBACK_MODE",
    "CAPI_REMOTE_LOOPBACK_MODE",
};
const char* util_capi_loopback_mode_str(capi_loopback_mode_t mode)
{
    VALIDATE_INPUT(mode, CAPI_LAST_LOOPBACK_MODE);
    return loopback_mode_text[mode];
}

/**< enum capi_polarity_t to string */
static char capi_polarity_text[POLARITY_SWAP_CURRENT_SETTING+1][50] = {
    "POLARITY_DEFAULT_SETTING",
    "POLARITY_INVERT_DEFAULT_SETTING",
    "POLARITY_SWAP_CURRENT_SETTING"
};

const char* util_capi_polarity_str(capi_polarity_action_t polarity)
{
    VALIDATE_INPUT(polarity, POLARITY_SWAP_CURRENT_SETTING);
    return capi_polarity_text[polarity];
}

/**< enum capi_reset_mode_t to string */
static char capi_reset_mode_text[CAPI_SOFT_RESET_MODE+1][32] = {
    "CAPI_HARD_RESET_MODE",
    "CAPI_SOFT_RESET_MODE"
};
const char* util_capi_reset_mode_str(capi_reset_mode_t mode)
{
    VALIDATE_INPUT(mode, CAPI_SOFT_RESET_MODE);
    return capi_reset_mode_text[mode];
}

/**< enum capi_download_mode_t to string */
static char capi_download_mode_text[CAPI_DOWNLOAD_MODE_I2C_EEPROM+1][50] =  {
    "CAPI_DOWNLOAD_MODE_NONE",
    "CAPI_DOWNLOAD_MODE_MDIO_SRAM",
    "CAPI_DOWNLOAD_MODE_I2C_SRAM",
    "CAPI_DOWNLOAD_MODE_MDIO_EEPROM",
    "CAPI_DOWNLOAD_MODE_I2C_EEPROM"
};
const char* util_capi_download_mode_str(capi_download_mode_t mode)
{
    VALIDATE_INPUT(mode, CAPI_DOWNLOAD_MODE_I2C_EEPROM);
    return capi_download_mode_text[mode];
}

/**< enum capi_media_type_t to string */
static char capi_media_type_text[CAPI_MEDIA_TYPE_COPPER_CABLE+1][50] = {
	"CAPI_MEDIA_TYPE_OPTICAL",
    "CAPI_MEDIA_TYPE_PCB_TRACE_BACK_PLANE",
    "CAPI_MEDIA_TYPE_COPPER_CABLE"  
};
const char* util_capi_media_type_str(capi_media_type_t type)
{
    VALIDATE_INPUT(type, CAPI_MEDIA_TYPE_COPPER_CABLE);
    return capi_media_type_text[type];
}

/**< enum capi_ref_clk_frq_mode_t to string */
static char capi_ref_clk_frq_mode_text[CAPI_REF_CLK_FRQ_MAX][50] = {
    "CAPI_REF_CLK_FRQ_156_25_MHZ_ETHERNET",
    "CAPI_REF_CLK_FRQ_625_MHZ_ETHERNET",
    "CAPI_REF_CLK_FRQ_166_15625_MHZ_ETHERNET",
};
const char* util_capi_ref_clk_frq_mode_str(capi_ref_clk_frq_mode_t ref)
{
    VALIDATE_INPUT(ref, CAPI_REF_CLK_FRQ_166_15625_MHZ_ETHERNET);
    return capi_ref_clk_frq_mode_text[ref];
}

static char capi_lane_fec_term_type_text[CAPI_LANE_FEC_TERM_MAX][50] = {
    "CAPI_LANE_FEC_TERM_BYPASS",
    "CAPI_LANE_FEC_DEC_FWD",
    "CAPI_LANE_FEC_DEC_ENC",
    "CAPI_LANE_PCS_XENC",
    "CAPI_LANE_FEC_DEC_XDEC_XENC_ENC",
};

const char* util_capi_lane_fec_term_type_str(capi_lane_fec_term_type_t fecterm)
{
    VALIDATE_INPUT(fecterm, CAPI_LANE_FEC_TERM_MAX);
    return capi_lane_fec_term_type_text[fecterm];
}

static char capi_lane_mux_text[CAPI_LANE_MUX_MAX][50] = {
    "CAPI_LANE_MUX_BIT_MUX",
    "CAPI_LANE_MUX_SYMBOL_MUX"
};

const char* util_capi_lane_mux_str(capi_lane_mux_t mux)
{
    VALIDATE_INPUT(mux, CAPI_LANE_MUX_MAX);
    return capi_lane_mux_text[mux];
}

static char capi_line_fec_type_text[CAPI_LINE_FEC_TYPE_MAX][50] = {
    "CAPI_LINE_FEC_TYPE_NA",
    "CAPI_LINE_FEC_TYPE_RS528",
    "CAPI_LINE_FEC_TYPE_RS544",
    "CAPI_LINE_FEC_TYPE_PCS"
};

const char* util_capi_line_fec_type_str(capi_line_fec_type_t val)
{
    VALIDATE_INPUT(val, CAPI_LINE_FEC_TYPE_MAX);
    return capi_line_fec_type_text[val];
}

static char capi_host_fec_type_text[CAPI_HOST_FEC_TYPE_MAX][50] = {
    "CAPI_HOST_FEC_TYPE_NA",
    "CAPI_HOST_FEC_TYPE_RS528",
    "CAPI_HOST_FEC_TYPE_RS544",
    "CAPI_HOST_FEC_TYPE_PCS"
};

const char* util_capi_host_fec_type_str(capi_host_fec_type_t val)
{
    VALIDATE_INPUT(val, CAPI_HOST_FEC_TYPE_MAX);
    return capi_host_fec_type_text[val];
}

static char capi_bh_baud_rate_text[CAPI_BH_BR_MAX][50] = {
    "CAPI_BH_BR_106_25",
    "CAPI_BH_BR_53_125",
    "CAPI_BH_BR_51_563",
    "CAPI_BH_BR_25_78125",
    "CAPI_BH_BR_26_5625",
};

static char capi_lw_baud_rate_text[CAPI_LW_BR_MAX][50] = {
    "CAPI_LW_BR_106_25",
    "CAPI_LW_BR_103_13",
    "CAPI_LW_BR_25_78125",
    "CAPI_LW_BR_111_57",
    "CAPI_LW_BR_53_125",
    "CAPI_LW_BR_51_565",
    "CAPI_LW_BR_26_5625"
};

const char* util_capi_bh_baud_rate_text(capi_bh_baud_rate_t val)
{
    VALIDATE_INPUT(val, CAPI_BH_BR_MAX);
    return capi_bh_baud_rate_text[val];
}

const char* util_capi_lw_baud_rate_text(capi_lw_baud_rate_t val)
{
    VALIDATE_INPUT(val, CAPI_LW_BR_MAX);
    return capi_lw_baud_rate_text[val];
}

/**< enum capi_function_mode_t to string */
static char capi_function_mode_text[CAPI_MODE_MAX][32] = {
    "CAPI_MODE_NONE",
    "CAPI_MODE_400G",
    "CAPI_MODE_200G",
    "CAPI_MODE_100G",
    "CAPI_MODE_50G",
    "CAPI_MODE_25G",
};
const char* util_capi_function_mode_str(capi_function_mode_t mode)
{
    //VALIDATE_INPUT(mode, CAPI_MODE_1G); //1G and 10G no longer in ENUM
    return capi_function_mode_text[mode];
}

/**< enum capi_modulation_t to string */
static char capi_modulation_text[CAPI_MODULATION_PAM4+1][50] = {
    "CAPI_MODULATION_NRZ",
    "CAPI_MODULATION_PAM4"
};
const char* util_capi_modulation_str(capi_modulation_t mod)
{
    VALIDATE_INPUT(mod, CAPI_MODULATION_PAM4);
    return capi_modulation_text[mod];
}

static char capi_port_config_status_text[CAPI_PORT_CONFIG_STATUS_FAILED+1][50] = {
    "CAPI_PORT_CONFIG_STATUS_NOT_CONFIGURED",
    "CAPI_PORT_CONFIG_STATUS_IN_PROGRESS",
    "CAPI_PORT_CONFIG_STATUS_SUCCESS",
    "CAPI_PORT_CONFIG_STATUS_FAILED"
};
const char* util_capi_port_config_status_str(capi_port_config_status_t val)
{
    VALIDATE_INPUT(val, CAPI_PORT_CONFIG_STATUS_FAILED+1);
    return capi_port_config_status_text[val];
}

static char capi_port_power_down_status_text[CAPI_PORT_POWER_DOWN_STATUS_POWER_DOWN+1][50] = {
    "CAPI_PORT_POWER_DOWN_STATUS_POWER_UP",
    "CAPI_PORT_POWER_DOWN_STATUS_POWER_DOWN"
};
const char* util_capi_port_power_down_status_str(capi_port_power_down_status_t val)
{
    VALIDATE_INPUT(val, CAPI_PORT_POWER_DOWN_STATUS_POWER_DOWN+1);
    return capi_port_power_down_status_text[val];
}

/**< enum capi_lane_data_rate_t to string */
static char capi_lane_data_rate_text[CAPI_LANE_DATA_RATE_MAX][50] = {
    "CAPI_LANE_DATA_RATE_NONE",
    "CAPI_LANE_DATA_RATE_1P25",
    "CAPI_LANE_DATA_RATE_10P3125",
    "CAPI_LANE_DATA_RATE_20P625",
    "CAPI_LANE_DATA_RATE_25P78125",
    "CAPI_LANE_DATA_RATE_26P5625",
    "CAPI_LANE_DATA_RATE_27P95",
    "CAPI_LANE_DATA_RATE_28",
    "CAPI_LANE_DATA_RATE_51P5625",
    "CAPI_LANE_DATA_RATE_53P125",
    "CAPI_LANE_DATA_RATE_54",
    "CAPI_LANE_DATA_RATE_56",
    "CAPI_LANE_DATA_RATE_57"
};
const char* util_capi_lane_data_rate_str(capi_lane_data_rate_t lrate)
{
    VALIDATE_INPUT(lrate, CAPI_LANE_DATA_RATE_MAX-1);
    return capi_lane_data_rate_text[lrate];
}

/**< enum capi_fec_type_t to string */
static char capi_fec_type_text[5][32] = {
    "CAPI_FEC_TYPE_PCS",
    "CAPI_FEC_TYPE_RS528",
    "CAPI_FEC_TYPE_RS544",
    "CAPI_FEC_TYPE_RS544_BRLNT",
    "CAPI_FEC_TYPE_NA"
};
const char* util_capi_fec_type_str(capi_fec_type_t fec)
{
    VALIDATE_INPUT(fec.host_fec, CAPI_HOST_FEC_TYPE_NA);
    return capi_fec_type_text[fec.host_fec];
}

const char* util_capi_command_id_str(phy_command_id_t cmd)
{
    return get_phy_command_id_str(cmd);
}

static char capi_switch_text[CAPI_SWITCH_TOGGLE+1][20] = {
    "CAPI_SWITCH_OFF",
    "CAPI_SWITCH_ON",
    "CAPI_SWITCH_TOGGLE"
};
const char* util_capi_switch_str(capi_switch_t sw)
{
    VALIDATE_INPUT(sw, CAPI_SWITCH_TOGGLE);
    return capi_switch_text[sw];
}

static char capi_txfir_tap_text[TXFIR_TAPS_7TAP + 1][32] = {
    "TXFIR_TAPS_NRZ_LP_3TAP",
    "TXFIR_TAPS_NRZ_6TAP",
    "TXFIR_TAPS_PAM4_LP_3TAP",
    "TXFIR_TAPS_PAM4_6TAP",
    "TXFIR_TAPS_4TAP",
    "TXFIR_TAPS_7TAP",
    "TXFIR_TAPS_LIN10TAP",     
    "TXFIR_TAPS_NONLIN10TAP",  
    "TXFIR_TAPS_NONLINWFTAP"  
};
const char* util_capi_txfir_tap_str(txfir_taps_t tap)
{
    VALIDATE_INPUT(tap, TXFIR_TAPS_NONLINWFTAP);
    return capi_txfir_tap_text[tap];
}

static char capi_lw_tx_precode_indep_text[CAPI_LW_INDEP_TX_PRECODE_ON+1][50] = {
    "CAPI_LW_INDEP_TX_PRECODE_DEFAULT",
    "CAPI_LW_INDEP_TX_PRECODE_OFF",
    "CAPI_LW_INDEP_TX_PRECODE_ON"
};
const char* util_capi_lw_tx_precode_indep_str(capi_lw_tx_precode_indep_t pre)
{
    VALIDATE_INPUT(pre, CAPI_LW_INDEP_TX_PRECODE_ON);
    return capi_lw_tx_precode_indep_text[pre];
}



static char capi_auto_net_mode_text[CAPI_AUTO_NEG_MODE_C50G_50G_X8+1][80] = {
    "CAPI_AUTO_NEG_MODE_INCOMPATIBLE",
    "CAPI_AUTO_NEG_MODE_IEEE_10G_KR",
    "CAPI_AUTO_NEG_MODE_IEEE_40G_KR4",
    "CAPI_AUTO_NEG_MODE_IEEE_40G_CR4",
    "CAPI_AUTO_NEG_MODE_IEEE_100G_KR4",
    "CAPI_AUTO_NEG_MODE_IEEE_100G_CR4",
    "CAPI_AUTO_NEG_MODE_IEEE_25G_CRSKRS",
    "CAPI_AUTO_NEG_MODE_IEEE_25G_CR1KR1",
    "CAPI_AUTO_NEG_MODE_IEEE_50GKRCR",
    "CAPI_AUTO_NEG_MODE_IEEE_100GKR2CR2",
    "CAPI_AUTO_NEG_MODE_IEEE_200GKR4CR4",
    "CAPI_AUTO_NEG_MODE_BAM_50G_BRCM_FEC_544_CR2_KR2",
    "CAPI_AUTO_NEG_MODE_BAM_200G_BRCM_KR4_CR4",
    "CAPI_AUTO_NEG_MODE_BAM_100G_BRCM_KR4_CR4",
    "CAPI_AUTO_NEG_MODE_BAM_100G_BRCM_FEC_528_KR2_CR2",
    "CAPI_AUTO_NEG_MODE_BAM_100G_BRCM_NO_FEC_KR2_CR2",
    "CAPI_AUTO_NEG_MODE_BAM_100G_BRCM_NO_FEC_X4",
    "CAPI_AUTO_NEG_MODE_C50G_25G_CR1KR1",
    "CAPI_AUTO_NEG_MODE_C50G_50G_CR2KR2",
    "CAPI_AUTO_NEG_MODE_C50G_50G_X8"
};

const char* util_capi_auto_neg_mode_str(capi_auto_neg_mode_t auto_neg_mode)
{
    VALIDATE_INPUT(auto_neg_mode, CAPI_AUTO_NEG_MODE_C50G_50G_X8);
    return capi_auto_net_mode_text[auto_neg_mode];
}

static char capi_bh_prbs_checker_mode_text[CAPI_BH_PRBS_INITIAL_SEED_NO_HYSTERESIS+1][64] = {
    "CAPI_BH_PRBS_SELF_SYNC_HYSTERESIS",
    "CAPI_BH_PRBS_INITIAL_SEED_HYSTERESIS",
    "CAPI_BH_PRBS_INITIAL_SEED_NO_HYSTERESIS"
};

const char* util_capi_bh_prbs_checker_mode_str(capi_bh_prbs_checker_mode_t mode)
{
    VALIDATE_INPUT(mode, CAPI_BH_PRBS_INITIAL_SEED_NO_HYSTERESIS);
    return capi_bh_prbs_checker_mode_text[mode];
}

static char capi_lnktrn_type_text[LNKTRN_TYPE_MAX][16] = {
    "LNKTRN_CL72",
    "LNKTRN_CL93",
    "LNKTRN_802_3CD"
};

const char* util_capi_lnktrn_type_str(capi_lnktrn_type_t ltype)
{
    VALIDATE_INPUT(ltype, LNKTRN_802_3CD);
    return capi_lnktrn_type_text[ltype];
}


static char capi_lnktrn_frm_size_text[LNKTRN_FRM_SIZE_MAX][32] = {
    "LNKTRN_FRM_SIZE_4K",
    "LNKTRN_FRM_SIZE_16K"
};

const char* util_capi_lnktrn_frm_size_str(capi_lnktrn_frm_size_t fsize)
{
    VALIDATE_INPUT(fsize, LNKTRN_FRM_SIZE_16K);
    return capi_lnktrn_frm_size_text[fsize];
}

static char capi_lnktrn_init_cond_text[LNKTRN_INIT_COND_MAX][32] = {
    "LNKTRN_INIT_COND_NO",
    "LNKTRN_INIT_COND_NO_EQ"
};

const char* util_capi_lnktrn_init_cond_str(capi_lnktrn_init_cond_t cond)
{
    VALIDATE_INPUT(cond, LNKTRN_INIT_COND_NO_EQ);
    return capi_lnktrn_init_cond_text[cond];
}

static char capi_lnktrn_cl72_cl93_init_text[LNKTRN_CL_INIT_MAX][24] = {
    "LNKTRN_CL_INIT_NORMAL",
    "LNKTRN_CL_INIT_EN"
};

const char* util_capi_lnktrn_cl72_cl93_init_str(capi_lnktrn_cl72_cl93_init_t cl_init)
{
    VALIDATE_INPUT(cl_init, LNKTRN_CL_INIT_EN);
    return capi_lnktrn_cl72_cl93_init_text[cl_init];
}
    
static char capi_lnktrn_cl72_cl93_preset_text[LNKTRN_CL_PRESET_MAX][50] = {
    "LNKTRN_CL_PRESET_NORMAL",
    "LNKTRN_CL_PRESET_EN"
};

const char* util_capi_lnktrn_cl72_cl93_preset_str(capi_lnktrn_cl72_cl93_preset_t pre)
{
    VALIDATE_INPUT(pre, LNKTRN_CL_PRESET_EN);
    return capi_lnktrn_cl72_cl93_preset_text[pre];
}

static char capi_polarity_action_preset_text[POLARITY_SWAP_CURRENT_SETTING+1][50] = {
    "POLARITY_DEFAULT_SETTING",
    "POLARITY_INVERT_DEFAULT_SETTING",
    "POLARITY_SWAP_CURRENT_SETTING"
};

const char* util_capi_polarity_action_str(capi_polarity_action_t polarity_action)
{
	VALIDATE_INPUT(polarity_action, POLARITY_SWAP_CURRENT_SETTING);
	return capi_polarity_action_preset_text[polarity_action];
}


/**
 * @brief      log_capi_config_info(capi_config_info_t* config_ptr)
 * @details    Logs capi_config_info_t values
 *
 * @param      config_ptr
 */
void log_capi_config_info(capi_config_info_t* config_ptr, uint8_t cfg_cmd)
{
    CAPI_LOG_DATA("capi_config_info {\n");
    if (cfg_cmd == GET_CONFIG) {
        CAPI_LOG_DATA("line_lane.lane_mask  : 0x%x\n", config_ptr->line_lane.lane_mask);
        CAPI_LOG_DATA("host_lane.lane_mask  : 0x%x\n", config_ptr->host_lane.lane_mask);
    } else {
        CAPI_LOG_DATA("ref_clk              : %s\n", util_capi_ref_clk_frq_mode_str(config_ptr->ref_clk));
        CAPI_LOG_DATA("func_mode            : %s\n", util_capi_function_mode_str(config_ptr->func_mode));
        CAPI_LOG_DATA("fec_term             : %s\n", util_capi_lane_fec_term_type_str(config_ptr->fec_term));
        CAPI_LOG_DATA("mux_type             : %s\n", util_capi_lane_mux_str(config_ptr->mux_type));
        CAPI_LOG_DATA("line_fec_type        : %s\n", util_capi_line_fec_type_str(config_ptr->line_fec_type));
        CAPI_LOG_DATA("host_fec_type        : %s\n", util_capi_host_fec_type_str(config_ptr->host_fec_type));
        CAPI_LOG_DATA("bh_br                : %s\n", util_capi_lw_baud_rate_text(config_ptr->bh_br));
        CAPI_LOG_DATA("lw_br                : %s\n", util_capi_lw_baud_rate_text(config_ptr->lw_br));
        CAPI_LOG_DATA("line_lane.modulation : %s\n", util_capi_modulation_str(config_ptr->line_lane.modulation));
        CAPI_LOG_DATA("line_lane.lane_mask  : 0x%x\n", config_ptr->line_lane.lane_mask);
        CAPI_LOG_DATA("host_lane.modulation : %s\n", util_capi_modulation_str(config_ptr->host_lane.modulation));
        CAPI_LOG_DATA("host_lane.lane_mask  : 0x%x\n", config_ptr->host_lane.lane_mask);
        CAPI_LOG_DATA("status               : %s\n", util_capi_port_config_status_str(config_ptr->status));
        CAPI_LOG_DATA("pwd_status           : %s\n", util_capi_port_power_down_status_str(config_ptr->pwd_status));
    }
    CAPI_LOG_DATA("}\n");
}

/**
 * @brief      log_capi_config_ex_info(capi_config_ex_info_t * config_ex_info_ptr, uint8_t cfg_cmd)
 * @details    Logs capi_config_ex_info_t values
 *
 * @param      config_ex_info_ptr : reference pointer to the capi config ex info object 
 */
void log_capi_config_ex_info(capi_config_ex_info_t * config_ex_info_ptr, uint8_t cfg_cmd)
{
    CAPI_LOG_DATA("capi_config_ex_info {\n");
    if (cfg_cmd == GET_CONFIG) {
        CAPI_LOG_DATA("line_lane.lane_mask  : 0x%x\n", config_ex_info_ptr->line_lane.lane_mask);
        CAPI_LOG_DATA("host_lane.lane_mask  : 0x%x\n", config_ex_info_ptr->host_lane.lane_mask);
    } else {
        CAPI_LOG_DATA("ref_clk              : %s\n", util_capi_ref_clk_frq_mode_str(config_ex_info_ptr->ref_clk));
        CAPI_LOG_DATA("func_mode            : %s\n", util_capi_function_mode_str(config_ex_info_ptr->func_mode));
        CAPI_LOG_DATA("egr fec_term         : %s\n", util_capi_lane_fec_term_type_str(config_ex_info_ptr->egr_fec_term));
        CAPI_LOG_DATA("igr fec_term         : %s\n", util_capi_lane_fec_term_type_str(config_ex_info_ptr->igr_fec_term));        
        CAPI_LOG_DATA("mux_type             : %s\n", util_capi_lane_mux_str(config_ex_info_ptr->mux_type));
        CAPI_LOG_DATA("line_fec_type        : %s\n", util_capi_line_fec_type_str(config_ex_info_ptr->line_fec_type));
        CAPI_LOG_DATA("host_fec_type        : %s\n", util_capi_host_fec_type_str(config_ex_info_ptr->host_fec_type));
        CAPI_LOG_DATA("bh_br                : %s\n", util_capi_lw_baud_rate_text(config_ex_info_ptr->bh_br));
        CAPI_LOG_DATA("lw_br                : %s\n", util_capi_lw_baud_rate_text(config_ex_info_ptr->lw_br));
        CAPI_LOG_DATA("line_lane.modulation : %s\n", util_capi_modulation_str(config_ex_info_ptr->line_lane.modulation));
        CAPI_LOG_DATA("line_lane.lane_mask  : 0x%x\n", config_ex_info_ptr->line_lane.lane_mask);
        CAPI_LOG_DATA("host_lane.modulation : %s\n", util_capi_modulation_str(config_ex_info_ptr->host_lane.modulation));
        CAPI_LOG_DATA("host_lane.lane_mask  : 0x%x\n", config_ex_info_ptr->host_lane.lane_mask);
        CAPI_LOG_DATA("status               : %s\n", util_capi_port_config_status_str(config_ex_info_ptr->status));
        CAPI_LOG_DATA("pwd_status           : %s\n", util_capi_port_power_down_status_str(config_ex_info_ptr->pwd_status));
    }
    CAPI_LOG_DATA("}\n");
}

/**
 * @brief      log_capi_config_fec_term(uint16_t fec_term)
 * @details    log fec term value
 *
 * @param      fec_term
 */
void log_capi_config_fec_term(uint16_t fec_term)
{
    CAPI_LOG_DATA("fec_term             : %s\n", util_capi_lane_fec_term_type_str(fec_term));
}

/**
 * @brief      log_capi_tx_info(capi_tx_info_t* tx_info_ptr, uint8_t cfg_cmd)
 * @details    log capi_tx_info_t values
 *
 * @param      tx_info_ptr
 */

void log_capi_tx_info(lane_tx_info_t* lane_tx_info, uint8_t cfg_cmd)
{
    CAPI_LOG_DATA("lane_tx_info_t {\n");
    if (cfg_cmd == GET_CONFIG) {
        CAPI_LOG_DATA("  param.content : 0x%x\n", lane_tx_info->param.content);
    } else {
        CAPI_LOG_DATA("  TXFIR enable: %d\n", lane_tx_info->param.is.txfir);
        CAPI_LOG_DATA("  numb_of_taps: %d\n", lane_tx_info->value.txfir.numb_of_taps);
        CAPI_LOG_DATA("  pre1        : %d\n", lane_tx_info->value.txfir.tap[0]);
        CAPI_LOG_DATA("  pre2        : %d\n", lane_tx_info->value.txfir.tap[1]);
        CAPI_LOG_DATA("  main        : %d\n", lane_tx_info->value.txfir.tap[2]);
        CAPI_LOG_DATA("  post1       : %d\n", lane_tx_info->value.txfir.tap[3]);
        CAPI_LOG_DATA("  post2       : %d\n", lane_tx_info->value.txfir.tap[4]);
        CAPI_LOG_DATA("  post3       : %d\n", lane_tx_info->value.txfir.tap[5]);
        CAPI_LOG_DATA("  post4       : %d\n", lane_tx_info->value.txfir.tap[6]);
        CAPI_LOG_DATA("  taps        : %s\n", util_capi_txfir_tap_str((txfir_taps_t)lane_tx_info->value.txfir.numb_of_taps));
        CAPI_LOG_DATA("  symbol_swap enable: %d\n", lane_tx_info->param.is.symbol_swap);
        CAPI_LOG_DATA("  symbol_swap : %d\n", lane_tx_info->value.symbol_swap);
    }
    CAPI_LOG_DATA("}\n");
}


#define CAPI_LOG_OVERRIDE(p) \
    CAPI_LOG_DATA("%s.enable : %d\n", #p, lane_rx_info->param.is.p); \
    CAPI_LOG_DATA("%s.value : %d\n", #p, lane_rx_info->value.p);
#define CAPI_LOG_OVERRIDE_GRP(p, i) \
    CAPI_LOG_DATA("%s.enable : %d\n", #p, lane_rx_info->param.is.p); \
    CAPI_LOG_DATA("%s.%s.value : %d\n", #p, #i, lane_rx_info->value.p.i);
#define CAPI_LOG_OVERRIDE_CORE_GRP(p, i) \
    CAPI_LOG_DATA("%s.enable : %d\n", #p, lane_rx_info->param.is. p##_##i); \
    CAPI_LOG_DATA("%s.%s.value : %d\n", #p, #i, lane_rx_info->value.core_ip.p.i);
#define CAPI_LOG_OVERRIDE_CORE_BIG_GRP(p, i, j) \
    CAPI_LOG_DATA("%s.enable : %d\n", #p, lane_rx_info->param.is. p##_##i); \
	CAPI_LOG_DATA("%s.%s.%s.value : %d\n", #p, #i, #j, lane_rx_info->value.core_ip.p.i.j);
/**
 * @brief      log_capi_rx_info(capi_rx_info_t* rx_info_ptr, uint8_t cfg_cmd)
 * @details    Log capi_rx_info_t values
 *
 * @param      rx_info_ptr
 */
void log_capi_rx_info(lane_rx_info_t* lane_rx_info, uint8_t cfg_cmd)
{
    int i;

    CAPI_LOG_DATA("lane_rx_info_t {\n");
    if (cfg_cmd == GET_CONFIG) {
        CAPI_LOG_DATA("lane_rx_info->param.content : 0x%x\n", lane_rx_info->param.content);
    } else {
        CAPI_LOG_OVERRIDE(vga)
            CAPI_LOG_DATA("lane_rx_info->value.core_ip.dsp.dfe_info.num_of_dfe_taps : %d\n", lane_rx_info->value.eq2_info.num_of_eq2_taps);
        for (i = 0; i < CAPI_NUM_DFE_TAPS-1; i++) {
            CAPI_LOG_DATA("lane_rx_info->value.dsp_dfe_info.dfe[%d] : %d\n", i, lane_rx_info->value.eq2_info.eq2[i]);
        }
        CAPI_LOG_OVERRIDE_GRP(peaking_filter_info, oven)
        CAPI_LOG_OVERRIDE_GRP(peaking_filter_info, oval)
        CAPI_LOG_OVERRIDE_GRP(peaking_filter_info, low_freq)
        CAPI_LOG_OVERRIDE_GRP(peaking_filter_info, high_freq)
        CAPI_LOG_DATA("line side:\n");
        CAPI_LOG_OVERRIDE_CORE_GRP(dsp,dc_wander_mu)
        CAPI_LOG_OVERRIDE_CORE_GRP(dsp,gain_boost)
        CAPI_LOG_OVERRIDE_CORE_BIG_GRP(dsp, phase_bias_auto_tuning_info, phase_auto_tune_en)
        CAPI_LOG_OVERRIDE_CORE_BIG_GRP(dsp, phase_bias_auto_tuning_info, dynamic_phase_auto_tune_en)
        CAPI_LOG_OVERRIDE_CORE_BIG_GRP(dsp, phase_bias_auto_tuning_info, max_phase_bias_th)
        CAPI_LOG_OVERRIDE_CORE_BIG_GRP(dsp, phase_bias_auto_tuning_info, min_phase_bias_th)
        CAPI_LOG_OVERRIDE_CORE_BIG_GRP(dsp, phase_bias_auto_tuning_info, phase_tune_step_size)
        CAPI_LOG_OVERRIDE_CORE_BIG_GRP(dsp, phase_bias_auto_tuning_info, phase_tune_dwell_time)
        CAPI_LOG_OVERRIDE_CORE_BIG_GRP(dsp, phase_bias_auto_tuning_info, phase_dtune_bias_range)
        CAPI_LOG_OVERRIDE_CORE_BIG_GRP(dsp, phase_bias_auto_tuning_info, phase_dtune_snr_change_th)
        CAPI_LOG_OVERRIDE_CORE_BIG_GRP(dsp, phase_bias_auto_tuning_info, phase_tune_link_down_snr)
        CAPI_LOG_OVERRIDE_CORE_BIG_GRP(dsp, nldet_info, nldet_en)
        CAPI_LOG_OVERRIDE_CORE_BIG_GRP(dsp, los_info, los_th_idx)
        CAPI_LOG_DATA("auto phase_bias tuning config: \n");
        CAPI_LOG_OVERRIDE_CORE_BIG_GRP(dsp, phase_bias_auto_tuning_info, dynamic_phase_auto_tune_en)
        CAPI_LOG_OVERRIDE_CORE_BIG_GRP(dsp, phase_bias_auto_tuning_info, max_phase_bias_th)
        CAPI_LOG_OVERRIDE_CORE_BIG_GRP(dsp, phase_bias_auto_tuning_info, min_phase_bias_th)
        CAPI_LOG_OVERRIDE_CORE_GRP(dsp, precode)
    }
    CAPI_LOG_DATA("}\n");
}



#define CAPI_LOG_LANE_CTRL_INFO_OVERRIDE(p) \
    CAPI_LOG_DATA("%s.enable : %d\n", #p, lane_ctrl_info_ptr->param.is.p); \
    CAPI_LOG_DATA("%s.value : %d\n", #p, lane_ctrl_info_ptr->cmd_value.is.p);

/**
 * @brief      log_lane_ctrl_info(capi_lane_ctrl_info_t* lane_ctrl_info_ptr, uint8_t cfg_cmd)
 * @details    log capi_lane_ctrl_info_t values
 *
 * @param      lane_ctrl_info_ptr
 */
void log_lane_ctrl_info(capi_lane_ctrl_info_t* lane_ctrl_info_ptr, uint8_t cfg_cmd)
{
    CAPI_LOG_DATA("capi_lane_ctrl_info {\n");
    if (cfg_cmd == GET_CONFIG) {
        CAPI_LOG_DATA(" param.content : 0x%x\n", lane_ctrl_info_ptr->param.content);
    } else {
        CAPI_LOG_LANE_CTRL_INFO_OVERRIDE(tx_afe_power)
        CAPI_LOG_LANE_CTRL_INFO_OVERRIDE(rx_afe_power)
        CAPI_LOG_LANE_CTRL_INFO_OVERRIDE(tx_suspend_resume)
        CAPI_LOG_LANE_CTRL_INFO_OVERRIDE(rx_suspend_resume)
        CAPI_LOG_LANE_CTRL_INFO_OVERRIDE(ignore_fault)
        CAPI_LOG_LANE_CTRL_INFO_OVERRIDE(tx_electric_idle)
        CAPI_LOG_LANE_CTRL_INFO_OVERRIDE(tx_squelch)
        CAPI_LOG_LANE_CTRL_INFO_OVERRIDE(rx_squelch)
        CAPI_LOG_LANE_CTRL_INFO_OVERRIDE(tx_lane_cfg_reset)
        CAPI_LOG_LANE_CTRL_INFO_OVERRIDE(rx_lane_cfg_reset)
        CAPI_LOG_LANE_CTRL_INFO_OVERRIDE(tx_datapath_power)
        CAPI_LOG_LANE_CTRL_INFO_OVERRIDE(rx_datapath_power)
    }
    CAPI_LOG_DATA("}\n");
}


#if CAPI_DSP_PLANNED
/**
 * @brief      log_fw_lane_config(capi_fw_lane_config_t* lane_config_ptr)
 * @details    Log capi_fw_lane_config_t values
 *
 * @param      lane_config_ptr
 */
void log_fw_lane_config(capi_fw_lane_config_t* lane_config_ptr)
{
    CAPI_LOG_DATA("capi_fw_lane_config_t-->\n");
    CAPI_LOG_DATA("Lane_config_from_pcs       : %d\n", lane_config_ptr->Lane_config_from_pcs);
    CAPI_LOG_DATA("eq2_en                     : %d\n", lane_config_ptr->eq2_on);
    CAPI_LOG_DATA("force_baud_rate_eq2        : %d\n", lane_config_ptr->force_baud_rate_eq2);
    CAPI_LOG_DATA("low_power_eq2_on           : %d\n", lane_config_ptr->low_power_eq2_on);
    CAPI_LOG_DATA("media_type                 : %s\n", util_capi_media_type_str(lane_config_ptr->media_type));
    CAPI_LOG_DATA("unreliable_los             : %d\n", lane_config_ptr->unreliable_los);
    CAPI_LOG_DATA("scrambling_off             : %d\n", lane_config_ptr->scrambling_off);
    CAPI_LOG_DATA("cl72_auto_pol_en           : %d\n", lane_config_ptr->cl72_auto_pol_en);
    CAPI_LOG_DATA("cl72_rest_to               : %d\n", lane_config_ptr->cl72_rest_to);
    CAPI_LOG_DATA("bh_db_loss                 : %d\n", lane_config_ptr->bh_db_loss);
    CAPI_LOG_DATA("lp_has_prec_en             : %d\n", lane_config_ptr->lp_has_prec_en);
    CAPI_LOG_DATA("pam_er_nr                  : %d\n", lane_config_ptr->pam_er_nr);
    CAPI_LOG_DATA("bh_link_training           : %d\n", lane_config_ptr->bh_link_training);
    CAPI_LOG_DATA("linktrn_restart_timeout_en : %d\n", lane_config_ptr->linktrn_restart_timeout_en);
    CAPI_LOG_DATA("autopeaking_en             : %d\n", lane_config_ptr->autopeaking_en);
    CAPI_LOG_DATA("opposite_cdr_first         : %d\n", lane_config_ptr->opposite_cdr_first);
}
#endif

/**
 * @brief      log_capi_lane_swap_info(lane_swap_info_t* lane_swap_info_ptr)
 * @details    log lane_swap_info_t values
 *
 * @param      lane_swap_info_ptr
 */
void log_capi_lane_swap_info(lane_swap_info_t* lane_swap_info_ptr)
{
    uint8_t i;
    CAPI_LOG_INFO("lane_swap_info -->\n");
    CAPI_LOG_DATA("num_of_lanes       : %d\n", lane_swap_info_ptr->num_of_lanes);
    for (i=0; i<8; i++)
        CAPI_LOG_DATA("rx_lane_list[%d]=       : %d\n", i, lane_swap_info_ptr->rx_lane_list[i]);
    for (i=0; i<8; i++)
        CAPI_LOG_DATA("tx_lane_list[%d]=       : %d\n", i, lane_swap_info_ptr->tx_lane_list[i]);
}

/**
 * @brief      log_capi_command_info(capi_command_info_t* cmd_inf_ptr)
 * @details    Log capi_command_info_t values
 *
 * @param      cmd_inf_ptr
 */
void log_capi_command_info(capi_command_info_t* cmd_inf_ptr, uint8_t cfg_cmd)
{
    CAPI_LOG_DATA("command (%d)  -> %s (%s)\n", cmd_inf_ptr->command_id,
                        util_capi_command_id_str(cmd_inf_ptr->command_id),
                        util_cmd_type_str((capi_command_type_t)cfg_cmd));

    switch(cmd_inf_ptr->command_id)
    {
        case COMMAND_ID_SET_TXPI_OVERRIDE:
            CAPI_LOG_DATA("enable : %d ppm : %d\n", cmd_inf_ptr->type.txpi_ovrd.enable,
                                                    cmd_inf_ptr->type.txpi_ovrd.ppm);
            break;

        case COMMAND_ID_SET_POLARITY:
        case COMMAND_ID_GET_POLARITY:
            log_capi_polarity_info(&cmd_inf_ptr->type.polarity_info, cfg_cmd);
            break;

        case COMMAND_ID_GET_LANE_CTRL_INFO:
        case COMMAND_ID_SET_LANE_CTRL_INFO:
            log_lane_ctrl_info(&cmd_inf_ptr->type.lane_ctrl_info, cfg_cmd);
            break;

        case COMMAND_ID_GET_CONFIG_INFO:
        case COMMAND_ID_SET_CONFIG_INFO:
            log_capi_config_info(&cmd_inf_ptr->type.config_info , cfg_cmd);
            break;

        case COMMAND_ID_SET_LOOPBACK_INFO:
        case COMMAND_ID_GET_LOOPBACK_INFO:
            log_capi_lpbk_config(&cmd_inf_ptr->type.lpbk_ctrl_info, cfg_cmd);
            break;

        case COMMAND_ID_SET_PRBS_INFO:
        case COMMAND_ID_GET_PRBS_INFO:
            log_capi_prbs_info(&cmd_inf_ptr->type.prbs_info, cfg_cmd);
            break;

        case COMMAND_ID_CLEAR_PRBS_STATUS:
            CAPI_LOG_DATA("  prbs_type  : %s\n",
                    util_capi_prbs_type_str((capi_pattern_gen_mon_type_t)cmd_inf_ptr->type.prbs_status_info.prbs_type));
            break;

        case COMMAND_ID_GET_PRBS_STATUS:
            log_capi_prbs_status_info(&cmd_inf_ptr->type.prbs_status_info, cfg_cmd);
            break;

        case COMMAND_ID_INJ_PRBS_ERROR:
            log_capi_prbs_err_inject(&cmd_inf_ptr->type.prbs_err_inj_info);
            break;

        case COMMAND_ID_DIAG_LANE_STATUS:
            log_capi_diag_lane_status(&cmd_inf_ptr->type.diag_lane_status, cfg_cmd);
            break;

        case COMMAND_ID_SET_LANE_CONFIG_INFO:
        case COMMAND_ID_GET_LANE_CONFIG_INFO:
            if (cmd_inf_ptr->type.lane_config_info.lane_config_type == LANE_CONFIG_TYPE_LANE_RX_INFO) {
                log_capi_rx_info(&cmd_inf_ptr->type.lane_config_info.type.lane_rx_info, cfg_cmd);
            } else if (cmd_inf_ptr->type.lane_config_info.lane_config_type == LANE_CONFIG_TYPE_LANE_TX_INFO) {
                log_capi_tx_info(&cmd_inf_ptr->type.lane_config_info.type.lane_tx_info, cfg_cmd);
            } else if (cmd_inf_ptr->type.lane_config_info.lane_config_type == LANE_CONFIG_TYPE_LANE_LINK_TRAINING_INFO) {
                log_capi_lnktrn_info(&cmd_inf_ptr->type.lane_config_info.type.lane_lnktrn_info, cfg_cmd);
            } else if (cmd_inf_ptr->type.lane_config_info.lane_config_type == LANE_CONFIG_TYPE_LANE_SWAP_INFO) {
                log_capi_lane_swap_info(&cmd_inf_ptr->type.lane_config_info.type.lane_swap_info);
            } else {
                CAPI_LOG_ERROR("Unrecognized lane_config_type %d\n", cmd_inf_ptr->type.lane_config_info.lane_config_type);
            }
            break;

        case COMMAND_ID_GET_LANE_INFO:
            log_capi_lane_info(&cmd_inf_ptr->type.lane_info, cfg_cmd);
            break;

        default:
            break;
    }
}

/**
 * @brief      log_capi_prbs_info(capi_prbs_info_t* prbs_info_ptr)
 * @details    Log capi_prbs_info_t values
 *
 * @param      prbs_info_ptr
 */
void log_capi_prbs_info(capi_prbs_info_t* prbs_info_ptr, uint8_t cfg_cmd)
{
    CAPI_LOG_DATA("capi_prbs_info {\n");

    if (cfg_cmd == GET_CONFIG) {
        CAPI_LOG_DATA("  ptype                    : %s\n", util_capi_prbs_type_str((capi_pattern_gen_mon_type_t)prbs_info_ptr->ptype));
    } else { 
        CAPI_LOG_DATA("  ptype                    : %s\n", util_capi_prbs_type_str((capi_pattern_gen_mon_type_t)prbs_info_ptr->ptype));
        CAPI_LOG_DATA("  gen_switch               : %s\n", util_capi_switch_str((capi_switch_t)prbs_info_ptr->gen_switch));

        if (prbs_info_ptr->ptype == CAPI_PCS_SCRM_IDLE_GENERATOR) {
            CAPI_LOG_DATA("  pcs_scramble.txpi_on     : %d\n", (prbs_info_ptr->op.pcs_scramble_idle.txpi_on));
        }
        else {
            CAPI_LOG_DATA("  pcfg.poly.prbs_type      : %d\n", prbs_info_ptr->op.pcfg.poly.poly_type);
            CAPI_LOG_DATA("  pcfg.poly.fec_pgen_poly  : %d\n", prbs_info_ptr->op.pcfg.poly.fec_pgen_poly);
        }
    }
    CAPI_LOG_DATA("}\n");
}

/**
 * @brief      log_capi_prbs_status_info(capi_prbs_info_t* prbs_info_ptr, uint8_t cfg_cmd)
 * @details    Log capi_prbs_info_t values
 *
 * @param      prbs_info_ptr
 */
void log_capi_prbs_status_info(capi_prbs_status_t* prbs_status, uint8_t cfg_cmd)
{
    CAPI_LOG_DATA("capi_prbs_status_t {\n");
    if (GET_CONFIG == cfg_cmd) {
        CAPI_LOG_DATA("  prbs_type                : %s\n", util_capi_prbs_type_str((capi_pattern_gen_mon_type_t)prbs_status->prbs_type));
    } else {
        CAPI_LOG_DATA("  prbs_type                : %s\n", util_capi_prbs_type_str((capi_pattern_gen_mon_type_t)prbs_status->prbs_type));
        CAPI_LOG_DATA("  err.ml_err.lsb_err       : %d\n", prbs_status->err.ml_err.lsb_err);
        CAPI_LOG_DATA("  err.ml_err.msb_err       : %d\n", prbs_status->err.ml_err.msb_err);
        CAPI_LOG_DATA("  err.s_err                : %ld\n", prbs_status->err.s_err);
        CAPI_LOG_DATA("  lock                     : %d\n", prbs_status->lock);
        CAPI_LOG_DATA("  lock_loss                : %d\n", prbs_status->lock_loss);
        CAPI_LOG_DATA("  lock_loss                : %s\n", util_capi_prbs_type_str(prbs_status->prbs_type));
    }
    CAPI_LOG_DATA("}\n");
}

/**
 * @brief      log_capi_prbs_err_inject(capi_prbs_err_inject_t* err_inj_ptr)
 * @details    Log capi prbs error injection structure values
 *
 * @param      err_inj_ptr
 */
void log_capi_prbs_err_inject(capi_prbs_err_inject_t* err_inj_ptr)
{
    CAPI_LOG_DATA("capi_prbs_err_inject_t {\n");
    CAPI_LOG_DATA("  ptype                : %s\n", util_capi_prbs_type_str((capi_pattern_gen_mon_type_t)err_inj_ptr->ptype));
    CAPI_LOG_DATA("  enable               : %d\n", err_inj_ptr->enable);
    CAPI_LOG_DATA("  inject_err_num       : %d\n", err_inj_ptr->inject_err_num);
    CAPI_LOG_DATA("  err_type             : %d\n", err_inj_ptr->err_type);
    CAPI_LOG_DATA("}\n");
}

/**
 * @brief      log_capi_diag_lane_status(capi_diag_lane_status_t* diag_status_ptr, uint8_t cfg_cmd)
 * @details    Log diag lane status info
 *
 * @param      diag_status_ptr
 * @param      cfg_cmd
 */
void log_capi_diag_lane_status(capi_diag_lane_status_t* diag_status_ptr, uint8_t cfg_cmd)
{
    CAPI_LOG_DATA("capi_diag_lane_status_t {\n");
    if (cfg_cmd == GET_CONFIG) {
        CAPI_LOG_DATA(" param.content : 0x%x\n", diag_status_ptr->param.content);
    } else {
        if (diag_status_ptr->param.is.eye_margin_estimate) {
            CAPI_LOG_DATA(" eye_margin_est.left_eye_mui  : %d\n", diag_status_ptr->value.eye_margin_est.left_eye_mui);
            CAPI_LOG_DATA(" eye_margin_est.right_eye_mui : %d\n", diag_status_ptr->value.eye_margin_est.right_eye_mui);
            CAPI_LOG_DATA(" eye_margin_est.upper_eye_mv  : %d\n", diag_status_ptr->value.eye_margin_est.upper_eye_mv);
            CAPI_LOG_DATA(" eye_margin_est.lower_eye_mv  : %d\n", diag_status_ptr->value.eye_margin_est.lower_eye_mv);
        }
        else if (diag_status_ptr->param.is.dc_offset) {
            CAPI_LOG_DATA(" dc_offset : %d\n", diag_status_ptr->value.dc_offset);
        }
    }
    CAPI_LOG_DATA("}\n");
}

void log_capi_prbs_type (capi_pattern_gen_mon_type_t* prbs_type){
    CAPI_LOG_DATA("capi_pattern_gen_mon_type_t-->\n");
    CAPI_LOG_DATA("prbs_type       : %d\n", (int) *prbs_type);
}

/**
 * @brief      log_capi_lnktrn_info(capi_lnktrn_info_t* link_trn_info_ptr)
 * @details    Log capi_lnktrn_info_t values
 *
 * @param      link_trn_info_ptr
 */

void log_capi_lnktrn_info(lane_lnktrn_info_t* link_trn_info_ptr, uint8_t cfg_cmd)
{
    CAPI_LOG_DATA("lane_lnktrn_info_t {\n");
    if (cfg_cmd == GET_CONFIG) {
        CAPI_LOG_DATA("  param.content              : 0x%x\n", link_trn_info_ptr->param.content);
        CAPI_LOG_DATA("  opposite_cdr_first         : %d\n", link_trn_info_ptr->opposite_cdr_first);
    } else {
        CAPI_LOG_DATA("  lnktrn_en                  : %s\n", util_capi_enable_str((capi_enable_t)link_trn_info_ptr->lnktrn_en));
        CAPI_LOG_DATA("  lnktrn_type                : %s\n", util_capi_lnktrn_type_str((capi_lnktrn_type_t)link_trn_info_ptr->value.serdes.lnktrn_type));
        CAPI_LOG_DATA("  restart                    : %d\n", link_trn_info_ptr->value.dsp.restart);
        CAPI_LOG_DATA("  linktrn_restart_timeout_en : %d\n", link_trn_info_ptr->value.serdes.lnktrn_restart_timeout);
        CAPI_LOG_DATA("  pam_802_3.lnktrn_frm_size  : %s\n", util_capi_lnktrn_frm_size_str((capi_lnktrn_frm_size_t)link_trn_info_ptr->value.dsp.lnktrn_cfg.pam_802_3.lnktrn_frm_size));
        CAPI_LOG_DATA("  pam_802_3.lnktrn_init_cond : %s\n", util_capi_lnktrn_init_cond_str((capi_lnktrn_init_cond_t)link_trn_info_ptr->value.dsp.lnktrn_cfg.pam_802_3.lnktrn_init_cond));
        CAPI_LOG_DATA("  nrz_cl.lnktrn_cl_init      : %s\n", util_capi_lnktrn_cl72_cl93_init_str((capi_lnktrn_cl72_cl93_init_t)link_trn_info_ptr->value.dsp.lnktrn_cfg.nrz_cl.lnktrn_cl_init));
        CAPI_LOG_DATA("  nrz_cl.lnktrn_cl_preset    : %s\n", util_capi_lnktrn_cl72_cl93_preset_str((capi_lnktrn_cl72_cl93_preset_t)link_trn_info_ptr->value.dsp.lnktrn_cfg.nrz_cl.lnktrn_cl_preset));
    }
    CAPI_LOG_DATA("}\n");
}

/**
 * @brief      log_capi_polarity_info(capi_polarity_info_t* polarity_info_ptr)
 * @details    Log capi_polarity_info values
 *
 * @param      polarity_info_ptr
 */
void log_capi_polarity_info(capi_polarity_info_t* polarity_info_ptr, uint8_t cfg_cmd)
{
    if (cfg_cmd == GET_CONFIG) {
        CAPI_LOG_DATA("capi_polarity_info {\n");
        CAPI_LOG_DATA("  direction : %s\n", util_capi_direction_str((capi_direction_t)polarity_info_ptr->direction));
        CAPI_LOG_DATA("}\n");
    } else {
        CAPI_LOG_DATA("capi_polarity_info {\n");
        CAPI_LOG_DATA("  action                     : %s\n", util_capi_polarity_action_str((capi_polarity_action_t)polarity_info_ptr->action));
        CAPI_LOG_DATA("  direction                  : %s\n", util_capi_direction_str((capi_direction_t)polarity_info_ptr->direction));
        CAPI_LOG_DATA("}\n");
    }
}

/**
 * @brief      log_capi_lpbk_config(capi_loopback_info_t* loopback_ptr, uint8_t cfg_cmd)
 * @details    Log capi_loopback_info_t values
 *
 * @param      loopback_ptr
 */
void log_capi_lpbk_config(capi_loopback_info_t* loopback_ptr, uint8_t cfg_cmd)
{
    CAPI_LOG_DATA("capi_loopback_info {\n");
    if (cfg_cmd == GET_CONFIG) {
        CAPI_LOG_DATA("  capi_loopback_mode_t       : %s\n", util_capi_loopback_mode_str(loopback_ptr->mode));
    } else {
        CAPI_LOG_DATA("  capi_loopback_mode_t       : %s\n", util_capi_loopback_mode_str(loopback_ptr->mode));
        CAPI_LOG_DATA("  enable                     : %d\n", loopback_ptr->enable);
    }
    CAPI_LOG_DATA("}\n");
}

/**
 * @brief      log_capi_pmd_info(capi_pmd_info_t* pmd_info)
 * @details    Log capi_pmd_info_t values
 *
 * @param      pmd_info
 */
void log_capi_pmd_info(capi_pmd_info_t* pmd_info)
{
    CAPI_LOG_DATA("capi_pmd_info_t-->\n");
    CAPI_LOG_DATA("cdr_lock        : %d\n", pmd_info->cdr_lock);
    CAPI_LOG_DATA("los             : %d\n", pmd_info->los);
    CAPI_LOG_DATA("pll_lock        : %d\n", pmd_info->pll_lock);
    CAPI_LOG_DATA("tx_squelch      : %d\n", pmd_info->tx_squelch);
}

/**
 * @brief      log_capi_lane_info(capi_lane_info_t* lane_info, uint8_t cfg_cmd)
 * @details    Log capi_lane_info_t values
 *
 * @param     lane info
 */
void log_capi_lane_info(capi_lane_info_t* lane_info, uint8_t cfg_cmd)
{
    CAPI_LOG_DATA("capi_lane_info_t {\n");
    if (cfg_cmd == GET_CONFIG) {
        CAPI_LOG_DATA("  param.content : 0x%x\n", lane_info->param.content);
    } else {
        if (lane_info->param.is.lane_status) {
            CAPI_LOG_DATA("  lane_status.pll_lock        : 0x%x\n", lane_info->value.lane_status.pll_lock);
            CAPI_LOG_DATA("  lane_status.cdr_lock        : 0x%x\n", lane_info->value.lane_status.cdr_lock);
            CAPI_LOG_DATA("  lane_status.los             : 0x%x\n", lane_info->value.lane_status.los);
            CAPI_LOG_DATA("  lane_status.tx_squelch      : 0x%x\n", lane_info->value.lane_status.tx_squelch);
        }

        if (lane_info->param.is.lnktrn_status) {
            CAPI_LOG_DATA("  lnktrn_status.enabled          : %d\n", lane_info->value.lnktrn_status.enabled);
            CAPI_LOG_DATA("  lnktrn_status.signal_detect    : %d\n", lane_info->value.lnktrn_status.signal_detect);
            CAPI_LOG_DATA("  lnktrn_status.failure_detected : %d\n", lane_info->value.lnktrn_status.failure_detected);
            CAPI_LOG_DATA("  lnktrn_status.training_status  : %d\n", lane_info->value.lnktrn_status.training_status);
            CAPI_LOG_DATA("  lnktrn_status.receiver_status  : %d\n", lane_info->value.lnktrn_status.receiver_status);
            CAPI_LOG_DATA("  lnktrn_status.receiver_lock    : %d\n", lane_info->value.lnktrn_status.receiver_lock);
            CAPI_LOG_DATA("  lnktrn_status.frame_lock       : %d\n", lane_info->value.lnktrn_status.frame_lock);
        }

        if (lane_info->param.is.sticky_loss_electrical) {
            CAPI_LOG_DATA("  Electrical Loss Sticky : 0x%x\n", lane_info->value.sticky_loss.electrical);
        }

        if (lane_info->param.is.sticky_loss_optical) {
            CAPI_LOG_DATA("  Optical Loss Sticky : 0x%x\n", lane_info->value.sticky_loss.optical);
        }

        if (lane_info->param.is.sticky_loss_link) {
            CAPI_LOG_DATA("  Link Loss Sticky : 0x%x\n", lane_info->value.sticky_loss.link);
        }
    }
    CAPI_LOG_DATA("}\n");
}
#endif


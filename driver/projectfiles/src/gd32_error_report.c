/** @file gd32_error_report.c @brief Retain peripheral diagnostics for the debugger. */
#include "gd32e511_512.h"
#include "gd32e511_512_err_report.h"
err_report_struct err_report_buffer[ERR_REPORT_BUFFER_SIZE];
uint8_t err_report_buff_index;
void fw_debug_report_err(uint16_t moduleid, uint16_t apiid, uint8_t errid)
{
    uint32_t mask = __get_PRIMASK();
    __disable_irq();
    err_report_buffer[err_report_buff_index].moduleid = moduleid;
    err_report_buffer[err_report_buff_index].apiid = apiid;
    err_report_buffer[err_report_buff_index].errid = errid;
    err_report_buff_index = (uint8_t)((err_report_buff_index + 1U) % ERR_REPORT_BUFFER_SIZE);
    __set_PRIMASK(mask);
}

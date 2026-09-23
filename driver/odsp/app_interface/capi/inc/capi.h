/**
 *
 * @file     capi.h
 * @author   
 * @date     9/15/2018
 * @version  1.0
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

#ifndef CAPI_H
#define CAPI_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief      capi_read_register(capi_phy_info_t* phy_info_ptr, capi_reg_info_t* reg_info_ptr)
 * @details    This API is used to read register/s
 *
 * @param[in]     phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in/out] reg_info_ptr: when calling, give the 32 bit reg_address element defined in  the capi_reg_info_t
                                and return content element defined in  the capi_reg_info_t
 * 
 * @return     returns the result of the called method/function
 */
return_result_t capi_read_register(capi_phy_info_t* phy_info_ptr, capi_reg_info_t* reg_info_ptr);

/**
 * @brief      capi_write_register(capi_phy_info_t* phy_info_ptr, capi_reg_info_t* reg_info_ptr)
 * @details    This API is used to write register/s
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  reg_info_ptr: this pointer contains the information to write to the register
 *                              give the 32 bit reg_address element defined in  the capi_reg_info_t
 *                              and register value, content element defined in  the capi_reg_info_t
 * 
 * @return     returns the result of the called method/function
 */
return_result_t capi_write_register(capi_phy_info_t* phy_info_ptr, capi_reg_info_t* reg_info_ptr);


/**
 * @brief    capi_get_chip_info(capi_phy_info_t* phy_info_ptr, capi_chip_info_t* chip_info_ptr)
 * @details  This API is used to get the Chip information
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[out] chip_info_ptr: this pointer contains chip info defined by capi_chip_info_t, which has 
 *                            chip_id and chip_revision
 * 
 * @return     returns the result of the called methode/function, RR_SUCCESS
 */
return_result_t capi_get_chip_info(capi_phy_info_t* phy_info_ptr, capi_chip_info_t* chip_info_ptr);

/**
* @brief      capi_get_chip_status(capi_phy_info_t* phy_info_ptr_in, capi_chip_status_info_t* chip_status_info_ptr)
* @details    This API is used to get the chip status
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  chip_status_info_ptr: pointer to capi_chip_status_info_t
* @return     returns the performance result of the called method/function
*/
return_result_t capi_get_chip_status(capi_phy_info_t* phy_info_ptr, capi_chip_status_info_t* chip_status_info_ptr);


/**
 * @brief       capi_set_polarity(capi_phy_info_t* phy_info_ptr, capi_polarity_info_t* polarity_info_ptr)
 * @details       This API is used to set the Client side or Line side RX and TX polarity swap <BR>
 *             Note: This function support Line Side, Client side or both Line and Client side <BR>
 *                   This function also support multi-lane operation <BR>
 *             direction: INGRES or EGRESS <BR>
 *             polarity:  not invert, invert or swap <BR>
 *             en_disable:enable or disable   <BR>
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  polarity_info_ptr: this pointer carries the necessary information to set polarity
 * 
 * @return       returns the performance result of the called method/function
 */
return_result_t capi_set_polarity(capi_phy_info_t* phy_info_ptr, capi_polarity_info_t* polarity_info_ptr);

/**
 * @brief       capi_get_polarity(capi_phy_info_t* phy_info_ptr, capi_polarity_info_t* polarity_info_ptr)
 * @details       This API is used to get the Client side or Line side RX or TX polarity swap <BR>
 *             Note: This function support either Line Side or Client side <BR>
 *                   This function only support single-lane operation. <BR>
 *                   If multi-lane mask is passed in, then first valid lane status will be returned. <BR>
 *             direction: INGRES or EGRESS <BR>
 *             polarity:  not invert, invert or swap <BR>
 *             en_disable:enable or disable   <BR>
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[out] polarity_info_ptr: this pointer return polarity information
 * 
 * @return       returns the performance result of the called method/function
 */
return_result_t capi_get_polarity(capi_phy_info_t* phy_info_ptr, capi_polarity_info_t* polarity_info_ptr);

/**
 * @brief      capi_set_lane_config_info(capi_phy_info_t* phy_info_ptr, capi_lane_config_info_t* lane_config_info_ptr)
 * @details    This API is used to set lane config info parameters
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  lane_config_info_ptr: a pointer which holds lane config parameters to be set
 *
 * @return     returns the performance result of the called method/function
 */
return_result_t capi_set_lane_config_info(capi_phy_info_t* phy_info_ptr, capi_lane_config_info_t* lane_config_info_ptr);

/**
 * @brief      capi_get_lane_config_info(capi_phy_info_t* phy_info_ptr, capi_lane_config_info_t* lane_config_info_ptr)
 * @details    This API is used to get lane config parameter value from Client side or Line side
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[out] lane_config_info_ptr: a pointer which holds lane config parameters read back from Client or Line
 *
 * @return     returns the performance result of the called method/function
 */
return_result_t capi_get_lane_config_info(capi_phy_info_t* phy_info_ptr, capi_lane_config_info_t* lane_config_info_ptr);

/**
 * @brief      capi_set_lane_ctrl_info(capi_phy_info_t* phy_info_ptr, capi_lane_ctrl_info_t* lane_ctrl_info_ptr)
 * @details    This API is used to set lane control of Client or Line side<BR>
 *             direction: INGRESS, EGRESS or both direction <BR>
 *             lane_ctrl: define the request type <BR>
 *             when core_ip is CORE_IP_LW, the LW support below request <BR>
 *                  in EGRESS direction:
 *                       LANE_RESET: reset toggle line TX data path <BR>
 *                       LANE_SQUELCH_ON: squelch line TX data path <BR>
 *                       LANE_SQUELCH_OFF: unsquelch line TX data path <BR>
 *                       LANE_TX_TRAFFIC_DISABLE:  disable line TX traffic <BR>
 *                       LANE_TX_TRAFFIC_ENABLE: re-enable line TX traffic from disable <BR>
 *                       LANE_TX_ELECTRICAL_IDLE_ENABLE: output electric idle signal from line TX <BR>
 *                       LANE_TX_ELECTRICAL_IDLE_DISABLE: line TX normal traffic <BR>
 *                  in INGRESS direction:
 *                       LANE_RESET: reset toggle line RX data path <BR>
 *                       LANE_SQUELCH_ON: disable line RX data path <BR>
 *                       LANE_SQUELCH_OFF: enable line RX data path <BR>
 *                  in both EGRESS and INGRESS direction:
 *                       LANE_RESET: reset toggle line both RX and TX data path <BR>
 *                       LANE_SQUELCH_ON: disable line both RX and TX data path  <BR>
 *                       LANE_SQUELCH_OFF: enable line both RX and TX data path  <BR>
 *             yes_or_nob: No use in set function <BR>
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  lane_ctrl_info_ptr: a pointer of lane control command
 * 
 * @return     returns the performance result of the called method/function
 */
return_result_t capi_set_lane_ctrl_info(capi_phy_info_t* phy_info_ptr, capi_lane_ctrl_info_t* lane_ctrl_info_ptr);

/**
 * @brief      capi_get_lane_ctrl_info(capi_phy_info_t* phy_info_ptr, capi_lane_ctrl_info_t* lane_ctrl_info_ptr)
 * @details    This API is used to get lane control status of Client or Line side <BR>
 *             when core_ip is CORE_IP_LW, the LW support below request <BR>
 *                  in EGRESS direction:
 *                       LANE_RESET: return if line TX data path is in reset; 1:reset; 0:un-reset <BR>
 *                       LANE_SQUELCH_ON: return if line TX data path is in squelch; 1: squelch; 0: unsquelch <BR>
 *                       LANE_SQUELCH_OFF: return if line TX data path is in unsquelch; 1: unsquelch; 0: squelch <BR>
 *                       LANE_TX_TRAFFIC_DISABLE:  return if line TX data path is disable; 1: disable; 0: enable <BR>
 *                       LANE_TX_TRAFFIC_ENABLE: return if line TX data path is disable; 1: enable; 0: disable <BR>
 *                  in INGRESS direction:
 *                       LANE_RESET: return if line RX data path is in reset; 1:reset; 0:un-reset <BR>
 *                       LANE_SQUELCH_ON: return if line RX data path is in squelch; 1: squelch; 0: unsquelch <BR>
 *                       LANE_SQUELCH_OFF: return if line RX data path is in unsquelch; 1: unsquelch; 0: squelch <BR>
 *                  in both EGRESS and INGRESS direction:
 *                       LANE_RESET: return if line side is in reset; 1:reset; 0:un-reset <BR>
 *                       LANE_SQUELCH_ON: return if both line RX and TX data path is disable; 1: both disable; 0: either one or both are enable  <BR>
 *                       LANE_SQUELCH_OFF: return if both line RX and TX data path is enable; 1: both enable; 0: either one or both are disable  <BR>
 *             yes_or_nob: refer to above details <BR>
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[out] lane_ctrl_info_ptr: a pointer of lane control status
 * 
 * @return     returns the performance result of the called method/function
 */
return_result_t capi_get_lane_ctrl_info(capi_phy_info_t* phy_info_ptr, capi_lane_ctrl_info_t* lane_ctrl_info_ptr);



/************************************************************************************************************************/
/****************************************************GET LANE INFO*******************************************************/
/************************************************************************************************************************/
/**
* @brief      capi_get_lane_info(capi_phy_info_t* phy_info_ptr, capi_lane_info_t* lane_info_ptr)
* @details    This API is used to get the lane info
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[out] lane_info_ptr: this pointer return lane status, refer to capi_lane_info_t
*
* @return     returns the performance result of the called method/function
*/
return_result_t capi_get_lane_info(capi_phy_info_t* phy_info_ptr, capi_lane_info_t* lane_info_ptr);



/**
 * @brief      capi_set_config(capi_phy_info_t* phy_info_ptr, capi_config_info_t* config_info_ptr)
 * @details    This API is used to set the chip mode configuration information including: <BR>
 *             Note: This function is used for whole lane/whole chip per chip feature <BR>
 *             ref_clk:   Reference clock <BR>
 *             mode_rate: Data rate (please use the client side mode rate if client and line side rate mode is different)  <BR>
 *             fec_mode:  FEC mode   <BR>
 *             func_mode: Function Mode  <BR>
 *             Please call this function before all other configuration function;  <BR>
 *             Refer to design document for each parameter detailed value assignment <BR>
 *             and make sure the parameter combination are valid                      <BR>
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  config_info_ptr: this pointer carries the necessary information for configuration procedure
 * 
 * @return     returns the performance result of the called method/function
 */
return_result_t capi_set_config(capi_phy_info_t* phy_info_ptr, capi_config_info_t* config_info_ptr);

/**
* @brief      capi_get_config(capi_phy_info_t* phy_info_ptr, capi_config_info_t* config_info_ptr)
* @details    This API is used to get the configuration information
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[out] config_info_ptr: this pointer return configuration information
* 
* @return     returns the performance result of the called method/function
*/
return_result_t capi_get_config(capi_phy_info_t* phy_info_ptr, capi_config_info_t* config_info_ptr);


/**
* @brief        capi_set_port(capi_phy_info_t* phy_info_ptr, capi_port_info_t* port_info_ptr)
* @details      This API drops the port/s
*
* @param[in]    phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]    port_info_ptr: reference to the port information object
*   
* @return       returns the performance result of the called methode/function
*/
return_result_t capi_set_port(capi_phy_info_t* phy_info_ptr, capi_port_info_t* port_info_ptr);

/**
* @brief        capi_get_port_info(capi_phy_info_t* phy_info_ptr, capi_port_info_t* port_info_ptr)
* @details      This API get the port config status
*
* @param[in]    phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in/out]    port_info_ptr: reference to the port information object
*   
* @return       returns the performance result of the called methode/function
*/
return_result_t capi_get_port_info(capi_phy_info_t* phy_info_ptr, capi_port_info_t* port_info_ptr);

/**
 * @brief      capi_command_request(capi_phy_info_t*     phy_info_ptr,
 *                                  capi_command_info_t* cmd_inf_ptr,
 *                                  uint16_t             payload_size,
 *                                  uint8_t              config_cmd)
 * @details    This API is used to send command to interface.
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  cmd_inf_ptr: a pointer which carries command 
 * @param[in]  payload_size: size of the payload type
 * @param[in]  config_cmd: the command configuration flag for set or get configuration
 * 
 * @return     returns the performance result of the called method/function
 */
return_result_t capi_command_request(capi_phy_info_t*     phy_info_ptr,
                                     capi_command_info_t* cmd_inf_ptr,
                                     uint16_t             payload_size,
                                     uint8_t              config_cmd);

/**
 * @brief    capi_reset(capi_phy_info_t* phy_info_ptr, capi_reset_mode_t* reset_mode_ptr)
 * @details  This API is used to reset the chip 
 *           for example:
 *                capi_phy_info_t capi_phy;
 *                capi_reset_mode_t reset_mode;
 *                reset_mode = CAPI_HARD_RESET_MODE;
 *                ret = capi_reset(&capi_phy, &reset_mode);
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  reset_mode_ptr: this pointer carries the necessary information for reseting procedure
 *                             defined by capi_reset_mode_t
 * 
 * @return     returns the  result of the called method/function
 */
return_result_t capi_reset(capi_phy_info_t* phy_info_ptr, capi_reset_mode_t* reset_mode_ptr);

/**
 * @brief    capi_download(capi_phy_info_t* phy_info_ptr,
 *                         capi_download_info_t* download_info_ptr)
 * @details  This API is used to download firmware , require to include  "whole_image_sram.h" for SRAM images.
 *           SPI EEPROM download will support later, in order to compile this CAPI, need to include both 
 *            "whole_image_sram.h" and "whole_image_spi.h" files
 *           for example:   
 *                          #include "whole_image_sram.h"
 *                            #include "whole_image_spi.h"
 *                          capi_phy_info_t capi_phy;
 *                          capi_download_info_t download_info;
 *                          download_info.mode = CAPI_DOWNLOAD_MODE_I2C_SRAM;
 *                          capi_download(&capi_phy, &download_info);
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  download_info_ptr: This pointer did not return download status because this API only download firmware
 *              please use capi_get_download_status to get download status.
 * 
 * @return     returns the result of the called methode/function, either RR_ERROR or RR_SUCCESS
 */
return_result_t capi_download(capi_phy_info_t*      phy_info_ptr,
                              capi_download_info_t* download_info_ptr);

/**
 * @brief    capi_get_download_status(capi_phy_info_t*      phy_info_ptr,
 *                                    capi_download_info_t* download_info_ptr)
 * @details  This API is used to get firmware download status
 *
 *           for example: 
 *                         capi_phy_info_t capi_phy;
 *                         capi_download_info_t download_info;
 *                         download_info.mode = CAPI_DOWNLOAD_MODE_I2C_SRAM;
 *                         capi_get_download_status(&capi_phy, &download_info);
 *                          dprintf("crc check:%x, status result:%x, version:%x \n", 
 *                                     download_info.status.crc_check, 
 *                                     download_info.status.result, 
 *                                     download_info.status.version);
 * @ property  None
 * @ public    None
 * @ private   None
 * @ example   None
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  download_info_ptr: this pointer carries the necessary information for downloading status
 *                                defined by capi_download_info_t
 * 
 * @return     returns the  result of the called method/function, either RR_ERROR or RR_SUCCESS
 */
return_result_t capi_get_download_status(capi_phy_info_t*      phy_info_ptr,
                                         capi_download_info_t* download_info_ptr);

/**
 * @brief    capi_get_firmware_status(capi_phy_info_t* phy_info_ptr, capi_status_info_t* status_info_ptr)
 * @details  This API is used to get firmware status
 *
 * @param[in]    phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[out]   status_info_ptr: this pointer retuns the firmware status info defined by capi_status_info_t
 * 
 * @return    returns the result of the called methode/function, either RR_ERROR or RR_SUCCESS
 */
return_result_t capi_get_firmware_status(capi_phy_info_t* phy_info_ptr, capi_status_info_t* status_info_ptr);

/**
 * @brief      capi_get_temperture_status(capi_phy_info_t*         phy_info_ptr,
 *                                        capi_temp_status_info_t* temp_status_info_ptr)
 * @details    This API is used obtain the temperature in degree C
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  capi_temp_status_info_ptr: this parameter pointed to the temperature info defined by
 *                                        capi_temp_status_info_t
 * 
 * @return     returns the result of the called method/function
 */
return_result_t capi_get_temperture_status(capi_phy_info_t*         phy_info_ptr,
                                           capi_temp_status_info_t* capi_temp_status_info_ptr);
/**
 * @brief    capi_set_avs_config(capi_phy_info_t*             phy_info_ptr,
 *                               capi_avs_mode_config_info_t* avs_config_ptr)
 * @details  This API is used to set the AVS configuration
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  avs_config_ptr: this pointer contains AVS mode configuration defined by capi_avs_mode_config_info_t
 * 
 * @return     returns the result of the called method/function
 */
return_result_t capi_set_avs_config(capi_phy_info_t*             phy_info_ptr,
                                    capi_avs_mode_config_info_t* avs_config_ptr);

/**
 * @brief    capi_get_avs_config(capi_phy_info_t*             phy_info_ptr,
 *                               capi_avs_mode_config_info_t* avs_config_ptr)
 * @details  This API is used to set the AVS configuration
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  avs_config_ptr: this pointer contains AVS mode configuration defined by 
 *                                   capi_avs_mode_config_info_t
 * 
 * @return     returns the result of the called method/function
 */
return_result_t capi_get_avs_config(capi_phy_info_t*             phy_info_ptr,
                                    capi_avs_mode_config_info_t* avs_config_ptr);

/**
* @brief    capi_cleanup_avs_config_status(capi_phy_info_t*             phy_info_ptr)
* @details  This API is used to cleanup the AVS configuration and status information when
*           calling capi_set_avs_config() fails, for example, due to wrong i2c address
*
* @ property  None
* @ public    None
* @ private   None
* @ example   None
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* 
* @return     returns the result of the called method/function
*/
return_result_t capi_cleanup_avs_config_status(capi_phy_info_t*             phy_info_ptr);

/**
 * @brief    capi_set_voltage_config(capi_phy_info_t*        phy_info_ptr,
 *                                   capi_fixed_voltage_config_info_t* fixed_config_ptr)
 * @details  This API is used to set the fixed voltage
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  fixed_config_ptr: this pointer contains AVS mode configuration defined
 *             by capi_fixed_voltage_config_info_t
 * 
 * @return     returns the result of the called method/function
 */
return_result_t capi_set_voltage_config(capi_phy_info_t*                  phy_info_ptr,
                                        capi_fixed_voltage_config_info_t* fixed_config_ptr);

/**
 * @brief      capi_get_voltage_status(capi_phy_info_t*            phy_info_ptr,
 *                                     capi_voltage_status_info_t* capi_voltage_status_info_ptr)
 * @details    This API is used obtain the voltage in MV
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  capi_voltage_status_info_ptr: this parameter pointed to the voltage info defined by
 *                                             capi_voltage_status_info_t
 * 
 * @return     returns the result of the called method/function
 */
return_result_t capi_get_voltage_status(capi_phy_info_t*            phy_info_ptr,
                                        capi_voltage_status_info_t* capi_voltage_status_info_ptr);

/**
 * @brief    capi_disable_avs_slaves(capi_phy_info_t*                  phy_info_ptr,
                                              capi_disable_avs_slave_info__t* capi_disable_avs_slave_info_ptr )
 * @details  This API is used to disable avs slave
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  capi_disable_avs_slave_info__ptr: this pointer contains avs slave defined
 *             by capi_disable_avs_slave_info__t
 * 
 * @return     returns the result of the called method/function
 */
return_result_t capi_disable_avs_slaves(capi_phy_info_t*                  phy_info_ptr,
                                              capi_disable_avs_slave_info__t* capi_disable_avs_slave_info_ptr );

/**
 * @brief    capi_enable_avs_slaves(capi_phy_info_t*                  phy_info_ptr,
                                              capi_enable_avs_slave_info__t* capi_disable_avs_slave_info_ptr )
 * @details  This API is used to enable avs slaves
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  capi_enable_avs_slave_info__ptr: this pointer contains avs slave defined
 *             by capi_enable_avs_slave_info__t
 * 
 * @return     returns the result of the called method/function
 */
return_result_t capi_enable_avs_slaves(capi_phy_info_t*                  phy_info_ptr,
                                              capi_enable_avs_slave_info__t* capi_enable_avs_slave_info_ptr );

/**
 * @brief      capi_set_internal_regulator_voltage(capi_phy_info_t*                   phy_info_ptr,
 *                                                 capi_internal_regulator_voltage_t* int_reg_voltage_ptr)
 * @details    This API is used to set either the internal regulator VDDM voltage (default 0.75V)
 *             or the AVDD voltage (default 0x9V)
 *
 * @param[in]  phy_info_ptr: a reference to the phy information object
 * @param[in]  int_reg_voltage_ptr: a referenec to the internal regulator voltage object
 * 
 * @return     returns the result of the called method/function
 */
return_result_t capi_set_internal_regulator_voltage(capi_phy_info_t*                   phy_info_ptr,
                                                    capi_internal_regulator_voltage_t* int_reg_voltage_ptr);

/**
 * @brief    capi_disable_vddm_slave(capi_phy_info_t*                  phy_info_ptr,
                                              capi_disable_vddm_slave_info__t* capi_disable_vddm_slave_info_ptr )
 * @details  This API is used to disable vddm slaves
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  capi_disable_vddm_slave_info__ptr: this pointer contains vddm slave defined
 *             by capi_disable_vddm_slave_info__t
 * 
 * @return     returns the result of the called method/function
 */
return_result_t capi_disable_vddm_slave(capi_phy_info_t*                  phy_info_ptr,
                                        capi_disable_vddm_slave_info__t* capi_disable_vddm_slave_info_ptr );

/**
 * @brief    capi_disable_avdd_slave(capi_phy_info_t*                  phy_info_ptr,
                                              capi_disable_avdd_slave_info__t* capi_disable_avdd_slave_info_ptr )
 * @details  This API is used to disable avdd slaves
 *
 * @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
 * @param[in]  capi_disable_avdd_slave_info__ptr: this pointer contains avdd slave defined
 *             by capi_disable_avdd_slave_info__t
 * 
 * @return     returns the result of the called method/function
 */
return_result_t capi_disable_avdd_slave(capi_phy_info_t*                  phy_info_ptr,
                                        capi_disable_avdd_slave_info__t* capi_disable_avdd_slave_info_ptr );

/**
* @brief    capi_get_download_crc_status(capi_phy_info_t*      phy_info_ptr)
* @details  This API is used to get firmware download crc status
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* 
* @return     returns the  result of the called methode/function, either RR_ERROR or RR_SUCCESS
*/
return_result_t capi_get_download_crc_status(capi_phy_info_t*  phy_info_ptr);

/**
 * @brief      capi_set_chip_command(capi_phy_info_t*          phy_info_ptr,
 *                                   capi_chip_command_info_t* chip_cmd_inf_ptr)
 * @details    This API is used to invoke chip set command
 *
 * @param[in]  phy_info_ptr: a reference to the phy information object
 * @param[in]  chip_cmd_info_ptr: a reference to the chip command object 
 * @return     returns the performance result of the called method/function
 */
return_result_t capi_set_chip_command(capi_phy_info_t*          phy_info_ptr,
                                      capi_chip_command_info_t* chip_cmd_info_ptr);

/**
 * @brief      capi_get_chip_command(capi_phy_info_t*          phy_info_ptr,
 *                                   capi_chip_command_info_t* chip_cmd_info_ptr)
 * @details    This API is used to invoke chip get command
 *
 * @param[in]  phy_info_ptr: a reference to the phy information object
 * @param[in]  chip_cmd_info_ptr: a reference to the chip command object 
 * @return     returns the performance result of the called method/function
 */
return_result_t capi_get_chip_command(capi_phy_info_t*          phy_info_ptr,
                                      capi_chip_command_info_t* chip_cmd_info_ptr);

/**
 * @brief   capi_set_regulator_info(capi_phy_info_t*  phy_info_ptr, capi_regulator_info_t* regulator_info_ptr)
 * @details  This API is used to disable all regulator
 *
 * @param[in]  phy_info_ptr: a reference to the phy information object
 * @param[in]  regulator_info_ptr: a reference to the regulator information object
 * 
 * @return     returns the result of the called method/function
 */
return_result_t capi_set_regulator_info(capi_phy_info_t*  phy_info_ptr, capi_regulator_info_t* regulator_info_ptr);

/**
 * @brief    capi_disable_all_regs(capi_phy_info_t* phy_info_ptr)
 * @details  This API is used to disable all regulator
 *
 * @param[in]  phy_info_ptr: a reference to the phy information object
 * 
 * @return     returns the result of the called method/function
 */
return_result_t capi_disable_all_regs(capi_phy_info_t*  phy_info_ptr );

/**
 * @brief      capi_get_status(capi_phy_info_t* phy_info_ptr, capi_gpr_lane_status_t* status_ptr)
 * @details    Get lane status from GP registers
 *
 * @param      phy_info_ptr : phy information
 * @param      status_ptr : output gpr lane status
 *
 * @return     return_result_t:return status code as success/warning/error
 */
return_result_t capi_get_status(capi_phy_info_t* phy_info_ptr, capi_gpr_lane_status_t* status_ptr);

/**
* @brief    capi_set_fw_download_info(capi_phy_info_t* phy_info_ptr,
*                           capi_fw_download_info_t* fw_download_info_ptr)
* @details  This API is used to download/upgrade firmware in the flash
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  fw_download_info_ptr: This pointer which carries fw download/upgrade info
*
* @return     returns the result of the called method/function, either RR_ERROR or RR_SUCCESS
*/
return_result_t capi_set_fw_download_info(capi_phy_info_t* phy_info_ptr, capi_fw_download_info_t* fw_download_info_ptr);

/**
 * @brief        capi_get_status_info(capi_phy_info_t* phy_info_ptr, capi_status_info_tt* status_info_ptr)
 * @details      This API retrieves the status information
 *
 * @param[in]    phy_info_ptr: a reference to the phy information object
 * @param[out]   status_info_ptr: a reference to the module status information object
 *
 * @return       returns the result of the called method/function, RR_SUCCESS
 */
return_result_t capi_get_status_info(capi_phy_info_t* phy_info_ptr, capi_status_info_tt* status_info_ptr);

/**
* @brief    capi_override_upgrade_status(capi_phy_info_t* phy_info_ptr,
*                                        firmware_image_status_t* upgrade_status_ptr)
* @details  This API is used to override the firmware upgrade status in the flash
*
* @param[in]  phy_info_ptr: a pointer which carries detail information necessary to identify the Phy
* @param[in]  upgrade_status_ptr: a pointer which carries the override status
*
* @return     returns the result of the called method/function, either RR_ERROR or RR_SUCCESS
*/
return_result_t capi_override_upgrade_status(capi_phy_info_t* phy_info_ptr, firmware_image_status_t* upgrade_status_ptr);

#ifdef __cplusplus
}
#endif

#endif /**< CAPI_H */


/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_scd4x.h
 * @brief     driver scd4x header file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2023-09-25
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2023/09/25  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#ifndef DRIVER_SCD4X_H
#define DRIVER_SCD4X_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup scd4x_driver scd4x driver function
 * @brief    scd4x driver modules
 * @{
 */

/**
 * @addtogroup scd4x_basic_driver
 * @{
 */

/**
 * @brief scd4x type enumeration definition
 */
typedef enum
{
    SCD40 = 0x00,        /**< scd40 */
    SCD41 = 0x01,        /**< scd41 */
} scd4x_t;

/**
 * @brief scd4x bool enumeration definition
 */
typedef enum
{
    SCD4X_BOOL_FALSE = 0x00,        /**< false */
    SCD4X_BOOL_TRUE  = 0x01,        /**< true */
} scd4x_bool_t;

/**
 * @brief scd4x handle structure definition
 */
typedef struct scd4x_handle_s
{
    uint8_t (*iic_init)(void);                                                 /**< point to an iic_init function address */
    uint8_t (*iic_deinit)(void);                                               /**< point to an iic_deinit function address */
    uint8_t (*iic_write_cmd)(uint8_t addr, uint8_t *buf, uint16_t len);        /**< point to an iic_write_cmd function address */
    uint8_t (*iic_read_cmd)(uint8_t addr, uint8_t *buf, uint16_t len);         /**< point to an iic_read_cmd function address */
    void (*delay_ms)(uint32_t ms);                                             /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                           /**< point to a debug_print function address */
    uint8_t inited;                                                            /**< inited flag */
    uint8_t type;                                                              /**< chip type */
} scd4x_handle_t;

/**
 * @brief scd4x information structure definition
 */
typedef struct scd4x_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} scd4x_info_t;

/**
 * @}
 */

/**
 * @defgroup scd4x_link_driver scd4x link driver function
 * @brief    scd4x link driver modules
 * @ingroup  scd4x_driver
 * @{
 */

/**
 * @brief     initialize scd4x_handle_t structure
 * @param[in] HANDLE points to an scd4x handle structure
 * @param[in] STRUCTURE is scd4x_handle_t
 * @note      none
 */
#define DRIVER_SCD4X_LINK_INIT(HANDLE, STRUCTURE)            memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link iic_init function
 * @param[in] HANDLE points to an scd4x handle structure
 * @param[in] FUC points to an iic_init function address
 * @note      none
 */
#define DRIVER_SCD4X_LINK_IIC_INIT(HANDLE, FUC)              (HANDLE)->iic_init = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE points to an scd4x handle structure
 * @param[in] FUC points to an iic_deinit function address
 * @note      none
 */
#define DRIVER_SCD4X_LINK_IIC_DEINIT(HANDLE, FUC)            (HANDLE)->iic_deinit = FUC

/**
 * @brief     link iic_read_cmd function
 * @param[in] HANDLE points to an scd4x handle structure
 * @param[in] FUC points to an iic_read_cmd function address
 * @note      none
 */
#define DRIVER_SCD4X_LINK_IIC_READ_COMMAND(HANDLE, FUC)      (HANDLE)->iic_read_cmd = FUC

/**
 * @brief     link iic_write_cmd function
 * @param[in] HANDLE points to an scd4x handle structure
 * @param[in] FUC points to an iic_write_cmd function address
 * @note      none
 */
#define DRIVER_SCD4X_LINK_IIC_WRITE_COMMAND(HANDLE, FUC)     (HANDLE)->iic_write_cmd = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to an scd4x handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_SCD4X_LINK_DELAY_MS(HANDLE, FUC)              (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to an scd4x handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_SCD4X_LINK_DEBUG_PRINT(HANDLE, FUC)           (HANDLE)->debug_print = FUC

/**
 * @}
 */

/**
 * @defgroup scd4x_basic_driver scd4x basic driver function
 * @brief    scd4x basic driver modules
 * @ingroup  scd4x_driver
 * @{
 */

/**
 * @brief      get chip information
 * @param[out] *info points to an scd4x info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t scd4x_info(scd4x_info_t *info);

/**
 * @brief     set type
 * @param[in] *handle points to an scd4x handle structure
 * @param[in] type is the chip type
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t scd4x_set_type(scd4x_handle_t *handle, scd4x_t type);

/**
 * @brief      get type
 * @param[in]  *handle points to an scd4x handle structure
 * @param[out] *type points to a chip type buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t scd4x_get_type(scd4x_handle_t *handle, scd4x_t *type);

/**
 * @brief     initialize the chip
 * @param[in] *handle points to an scd4x handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 * @note      none
 */
uint8_t scd4x_init(scd4x_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle points to an scd4x handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 stop periodic measurement failed
 * @note      none
 */
uint8_t scd4x_deinit(scd4x_handle_t *handle);

/**
 * @brief     start periodic measurement
 * @param[in] *handle points to an scd4x handle structure
 * @return    status code
 *            - 0 success
 *            - 1 start periodic measurement failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t scd4x_start_periodic_measurement(scd4x_handle_t *handle);

/**
 * @brief      read data
 * @param[in]  *handle points to an scd4x handle structure
 * @param[out] *co2_raw points to a co2 raw buffer
 * @param[out] *co2_ppm points to a co2 ppm buffer
 * @param[out] *temperature_raw points to a temperature raw buffer
 * @param[out] *temperature_s points to a temperature buffer
 * @param[out] *humidity_raw points to a humidity raw buffer
 * @param[out] *humidity_s points to a humidity buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 crc is error
 *             - 5 data is not ready
 * @note       none
 */
uint8_t scd4x_read(scd4x_handle_t *handle, uint16_t *co2_raw, uint16_t *co2_ppm,
                   uint16_t *temperature_raw, float *temperature_s,
                   uint16_t *humidity_raw, float *humidity_s);

/**
 * @brief     stop periodic measurement
 * @param[in] *handle points to an scd4x handle structure
 * @return    status code
 *            - 0 success
 *            - 1 stop periodic measurement failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t scd4x_stop_periodic_measurement(scd4x_handle_t *handle);

/**
 * @brief     set temperature offset
 * @param[in] *handle points to an scd4x handle structure
 * @param[in] offset is the temperature offset
 * @return    status code
 *            - 0 success
 *            - 1 set temperature offset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t scd4x_set_temperature_offset(scd4x_handle_t *handle, uint16_t offset);

/**
 * @brief      get temperature offset
 * @param[in]  *handle points to an scd4x handle structure
 * @param[out] *offset points to a temperature offset buffer
 * @return     status code
 *             - 0 success
 *             - 1 get temperature offset failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 crc is error
 * @note       none
 */
uint8_t scd4x_get_temperature_offset(scd4x_handle_t *handle, uint16_t *offset);

/**
 * @brief      convert the temperature offset to the register raw data
 * @param[in]  *handle points to an scd4x handle structure
 * @param[in]  degrees is the degrees
 * @param[out] *reg points to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t scd4x_temperature_offset_convert_to_register(scd4x_handle_t *handle, float degrees, uint16_t *reg);

/**
 * @brief      convert the register raw data to the temperature offset
 * @param[in]  *handle points to an scd4x handle structure
 * @param[in]  reg is the register raw data
 * @param[out] *degrees points to a temperature offset buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t scd4x_temperature_offset_convert_to_data(scd4x_handle_t *handle, uint16_t reg, float *degrees);

/**
 * @brief     set sensor altitude
 * @param[in] *handle points to an scd4x handle structure
 * @param[in] altitude is the set altitude
 * @return    status code
 *            - 0 success
 *            - 1 set sensor altitude failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t scd4x_set_sensor_altitude(scd4x_handle_t *handle, uint16_t altitude);

/**
 * @brief      get sensor altitude
 * @param[in]  *handle points to an scd4x handle structure
 * @param[out] *altitude points to an altitude buffer
 * @return     status code
 *             - 0 success
 *             - 1 get sensor altitude failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 crc is error
 * @note       none
 */
uint8_t scd4x_get_sensor_altitude(scd4x_handle_t *handle, uint16_t *altitude);

/**
 * @brief      convert the sensor altitude to the register raw data
 * @param[in]  *handle points to an scd4x handle structure
 * @param[in]  m is the set m
 * @param[out] *reg points to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t scd4x_sensor_altitude_convert_to_register(scd4x_handle_t *handle, float m, uint16_t *reg);

/**
 * @brief      convert the register raw data to the sensor altitude
 * @param[in]  *handle points to an scd4x handle structure
 * @param[in]  reg is the register raw data
 * @param[out] *m points to an m buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t scd4x_sensor_altitude_convert_to_data(scd4x_handle_t *handle, uint16_t reg, float *m);

/**
 * @brief     set ambient pressure
 * @param[in] *handle points to an scd4x handle structure
 * @param[in] pressure is the set pressure
 * @return    status code
 *            - 0 success
 *            - 1 set ambient pressure failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t scd4x_set_ambient_pressure(scd4x_handle_t *handle, uint16_t pressure);

/**
 * @brief      get ambient pressure
 * @param[in]  *handle points to an scd4x handle structure
 * @param[out] *pressure points to a pressure buffer
 * @return     status code
 *             - 0 success
 *             - 1 get ambient pressure failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 crc is error
 * @note       none
 */
uint8_t scd4x_get_ambient_pressure(scd4x_handle_t *handle, uint16_t *pressure);

/**
 * @brief      convert the ambient pressure to the register raw data
 * @param[in]  *handle points to an scd4x handle structure
 * @param[in]  pa is the set pa
 * @param[out] *reg points to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t scd4x_ambient_pressure_convert_to_register(scd4x_handle_t *handle, float pa, uint16_t *reg);

/**
 * @brief      convert the register raw data to the ambient pressure
 * @param[in]  *handle points to an scd4x handle structure
 * @param[in]  reg is the register raw data
 * @param[out] *pa points to a pa buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t scd4x_ambient_pressure_convert_to_data(scd4x_handle_t *handle, uint16_t reg, float *pa);

/**
 * @brief      perform forced recalibration
 * @param[in]  *handle points to an scd4x handle structure
 * @param[in]  co2_raw is the co2 raw data
 * @param[out] *frc points to a frc buffer
 * @return     status code
 *             - 0 success
 *             - 1 perform forced recalibration failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 crc is error
 * @note       none
 */
uint8_t scd4x_perform_forced_recalibration(scd4x_handle_t *handle, uint16_t co2_raw, uint16_t *frc);

/**
 * @brief      convert the co2 to the register raw data
 * @param[in]  *handle points to an scd4x handle structure
 * @param[in]  ppm is the set ppm
 * @param[out] *reg points to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t scd4x_co2_convert_to_register(scd4x_handle_t *handle, float ppm, uint16_t *reg);

/**
 * @brief      convert the register raw data to the co2
 * @param[in]  *handle points to an scd4x handle structure
 * @param[in]  reg is the register raw data
 * @param[out] *ppm points to a ppm buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t scd4x_co2_convert_to_data(scd4x_handle_t *handle, uint16_t reg, float *ppm);

/**
 * @brief     enable or disable automatic self calibration 
 * @param[in] *handle points to an scd4x handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set automatic self calibration failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t scd4x_set_automatic_self_calibration(scd4x_handle_t *handle, scd4x_bool_t enable);

/**
 * @brief      get automatic self calibration status
 * @param[in]  *handle points to an scd4x handle structure
 * @param[out] *enable points to a bool buffer
 * @return     status code
 *             - 0 success
 *             - 1 get automatic self calibration failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 crc is error
 * @note       none
 */
uint8_t scd4x_get_automatic_self_calibration(scd4x_handle_t *handle, scd4x_bool_t *enable);

/**
 * @brief     start low power periodic measurement
 * @param[in] *handle points to an scd4x handle structure
 * @return    status code
 *            - 0 success
 *            - 1 start low power periodic measurement failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t scd4x_start_low_power_periodic_measurement(scd4x_handle_t *handle);

/**
 * @brief      get data ready status
 * @param[in]  *handle points to an scd4x handle structure
 * @param[out] *enable points to a bool buffer
 * @return     status code
 *             - 0 success
 *             - 1 get data ready status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 crc is error
 * @note       none
 */
uint8_t scd4x_get_data_ready_status(scd4x_handle_t *handle, scd4x_bool_t *enable);

/**
 * @brief     persist settings
 * @param[in] *handle points to an scd4x handle structure
 * @return    status code
 *            - 0 success
 *            - 1 persist settings failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t scd4x_persist_settings(scd4x_handle_t *handle);

/**
 * @brief      get serial number
 * @param[in]  *handle points to an scd4x handle structure
 * @param[out] *number points to a number buffer
 * @return     status code
 *             - 0 success
 *             - 1 get serial number failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 crc is error
 * @note       none
 */
uint8_t scd4x_get_serial_number(scd4x_handle_t *handle, uint16_t number[3]);

/**
 * @brief      perform self test
 * @param[in]  *handle points to an scd4x handle structure
 * @param[out] *malfunction_detected points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 perform self test failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 crc is error
 * @note       none
 */
uint8_t scd4x_perform_self_test(scd4x_handle_t *handle, scd4x_bool_t *malfunction_detected);

/**
 * @brief     perform factory reset
 * @param[in] *handle points to an scd4x handle structure
 * @return    status code
 *            - 0 success
 *            - 1 perform factory reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t scd4x_perform_factory_reset(scd4x_handle_t *handle);

/**
 * @brief     reinit
 * @param[in] *handle points to an scd4x handle structure
 * @return    status code
 *            - 0 success
 *            - 1 reinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t scd4x_reinit(scd4x_handle_t *handle);

/**
 * @}
 */

/**
 * @defgroup scd4x_advance_driver scd4x advance driver function
 * @brief    scd4x advance driver modules
 * @ingroup  scd4x_driver
 * @{
 */

/**
 * @brief     measure single shot
 * @param[in] *handle points to an scd4x handle structure
 * @return    status code
 *            - 0 success
 *            - 1 measure single shot failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 only scd41 has this function
 * @note      none
 */
uint8_t scd4x_measure_single_shot(scd4x_handle_t *handle);

/**
 * @brief     measure single shot rht only
 * @param[in] *handle points to an scd4x handle structure
 * @return    status code
 *            - 0 success
 *            - 1 measure single shot rht only failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 only scd41 has this function
 * @note      none
 */
uint8_t scd4x_measure_single_shot_rht_only(scd4x_handle_t *handle);

/**
 * @brief     power down
 * @param[in] *handle points to an scd4x handle structure
 * @return    status code
 *            - 0 success
 *            - 1 power down failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 only scd41 has this function
 * @note      none
 */
uint8_t scd4x_power_down(scd4x_handle_t *handle);

/**
 * @brief     wake up
 * @param[in] *handle points to an scd4x handle structure
 * @return    status code
 *            - 0 success
 *            - 1 wake up failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 only scd41 has this function
 * @note      none
 */
uint8_t scd4x_wake_up(scd4x_handle_t *handle);

/**
 * @brief     set automatic self calibration initial period
 * @param[in] *handle points to an scd4x handle structure
 * @param[in] hour is the set hour
 * @return    status code
 *            - 0 success
 *            - 1 set automatic self calibration initial period failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 only scd41 has this function
 *            - 5 hour is not integer multiples of 4
 * @note      none
 */
uint8_t scd4x_set_automatic_self_calibration_initial_period(scd4x_handle_t *handle, uint16_t hour);

/**
 * @brief      get automatic self calibration initial period
 * @param[in]  *handle points to an scd4x handle structure
 * @param[out] *hour points to an hour buffer
 * @return     status code
 *             - 0 success
 *             - 1 get automatic self calibration initial period failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 only scd41 has this function
 *             - 5 crc is error
 * @note       none
 */
uint8_t scd4x_get_automatic_self_calibration_initial_period(scd4x_handle_t *handle, uint16_t *hour);

/**
 * @brief     set automatic self calibration standard period
 * @param[in] *handle points to an scd4x handle structure
 * @param[in] hour is the set hour
 * @return    status code
 *            - 0 success
 *            - 1 set automatic self calibration standard period failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 only scd41 has this function
 *            - 5 hour is not integer multiples of 4
 * @note      none
 */
uint8_t scd4x_set_automatic_self_calibration_standard_period(scd4x_handle_t *handle, uint16_t hour);

/**
 * @brief      get automatic self calibration standard period
 * @param[in]  *handle points to an scd4x handle structure
 * @param[out] *hour points to an hour buffer
 * @return     status code
 *             - 0 success
 *             - 1 get automatic self calibration standard period failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 only scd41 has this function
 *             - 5 crc is error
 * @note       none
 */
uint8_t scd4x_get_automatic_self_calibration_standard_period(scd4x_handle_t *handle, uint16_t *hour);

/**
 * @}
 */

/**
 * @defgroup scd4x_extern_driver scd4x extern driver function
 * @brief    scd4x extern driver modules
 * @ingroup  scd4x_driver
 * @{
 */

/**
 * @brief     set the chip register
 * @param[in] *handle points to an scd4x handle structure
 * @param[in] reg is the iic register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t scd4x_set_reg(scd4x_handle_t *handle, uint16_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief      get the chip register
 * @param[in]  *handle points to an scd4x handle structure
 * @param[in]  reg is the iic register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data buffer length
 * @param[in]  delay_ms is the delay time in ms
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t scd4x_get_reg(scd4x_handle_t *handle, uint16_t reg, uint8_t *buf, uint16_t len, uint16_t delay_ms);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif

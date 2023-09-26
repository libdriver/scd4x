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
 * @file      driver_scd4x_register_test.c
 * @brief     driver scd4x register test source file
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

#include "driver_scd4x_register_test.h"
#include <stdlib.h>

static scd4x_handle_t gs_handle;        /**< scd4x handle */

/**
 * @brief     register test
 * @param[in] type is the chip type
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t scd4x_register_test(scd4x_t type)
{
    uint8_t res;
    uint16_t reg;
    uint16_t reg_check;
    uint16_t number[3];
    float f;
    float f_check;
    scd4x_bool_t enable;
    scd4x_t type_check;
    scd4x_info_t info;
    
    /* link functions */
    DRIVER_SCD4X_LINK_INIT(&gs_handle, scd4x_handle_t);
    DRIVER_SCD4X_LINK_IIC_INIT(&gs_handle, scd4x_interface_iic_init);
    DRIVER_SCD4X_LINK_IIC_DEINIT(&gs_handle, scd4x_interface_iic_deinit);
    DRIVER_SCD4X_LINK_IIC_WRITE_COMMAND(&gs_handle, scd4x_interface_iic_write_cmd);
    DRIVER_SCD4X_LINK_IIC_READ_COMMAND(&gs_handle, scd4x_interface_iic_read_cmd);
    DRIVER_SCD4X_LINK_DELAY_MS(&gs_handle, scd4x_interface_delay_ms);
    DRIVER_SCD4X_LINK_DEBUG_PRINT(&gs_handle, scd4x_interface_debug_print);
    
    /* scd4x info */
    res = scd4x_info(&info);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip information */
        scd4x_interface_debug_print("scd4x: chip is %s.\n", info.chip_name);
        scd4x_interface_debug_print("scd4x: manufacturer is %s.\n", info.manufacturer_name);
        scd4x_interface_debug_print("scd4x: interface is %s.\n", info.interface);
        scd4x_interface_debug_print("scd4x: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        scd4x_interface_debug_print("scd4x: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        scd4x_interface_debug_print("scd4x: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        scd4x_interface_debug_print("scd4x: max current is %0.2fmA.\n", info.max_current_ma);
        scd4x_interface_debug_print("scd4x: max temperature is %0.1fC.\n", info.temperature_max);
        scd4x_interface_debug_print("scd4x: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start register test */
    scd4x_interface_debug_print("scd4x: start register test.\n");
    
    /* scd4x_set_type/scd4x_get_type test */
    scd4x_interface_debug_print("scd4x: scd4x_set_type/scd4x_get_type test.\n");
    
    /* scd40 */
    res = scd4x_set_type(&gs_handle, SCD40);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: set type failed.\n");
    
        return 1;
    }
    scd4x_interface_debug_print("scd4x: set type scd40.\n");
    res = scd4x_get_type(&gs_handle, &type_check);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: get type failed.\n");
    
        return 1;
    }
    scd4x_interface_debug_print("scd4x: check type %s.\n", SCD40 == type_check ? "ok" : "error");
    
    /* scd41 */
    res = scd4x_set_type(&gs_handle, SCD41);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: set type failed.\n");
    
        return 1;
    }
    scd4x_interface_debug_print("scd4x: set type scd41.\n");
    res = scd4x_get_type(&gs_handle, &type_check);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: get type failed.\n");
    
        return 1;
    }
    scd4x_interface_debug_print("scd4x: check type %s.\n", SCD41 == type_check ? "ok" : "error");
    
    /* set chip type */
    res = scd4x_set_type(&gs_handle, type);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: set type failed.\n");
    
        return 1;
    }
    
    /* scd4x init */
    res = scd4x_init(&gs_handle);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: init failed.\n");
    
        return 1;
    }
    
    /* scd4x_set_temperature_offset/scd4x_get_temperature_offset test */
    scd4x_interface_debug_print("scd4x: scd4x_set_temperature_offset/scd4x_get_temperature_offset test.\n");
    
    reg = rand() % 0xFFFFU;
    res = scd4x_set_temperature_offset(&gs_handle, reg);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: set temperature offset failed.\n");
        (void)scd4x_deinit(&gs_handle);
        
        return 1;
    }
    scd4x_interface_debug_print("scd4x: set temperature offset %d.\n", reg);
    res = scd4x_get_temperature_offset(&gs_handle, &reg_check);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: get temperature offset failed.\n");
        (void)scd4x_deinit(&gs_handle);
        
        return 1;
    }
    scd4x_interface_debug_print("scd4x: check temperature offset %s.\n", reg == reg_check ? "ok" : "error");
    
    /* scd4x_temperature_offset_convert_to_register/scd4x_temperature_offset_convert_to_data test */
    scd4x_interface_debug_print("scd4x: scd4x_temperature_offset_convert_to_register/scd4x_temperature_offset_convert_to_data test.\n");
    
    f = (float)(rand() % 10) + 25.0f;
    res = scd4x_temperature_offset_convert_to_register(&gs_handle, f, &reg);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: temperature offset convert to register failed.\n");
        (void)scd4x_deinit(&gs_handle);
        
        return 1;
    }
    scd4x_interface_debug_print("scd4x: set temperature %0.2f.\n", f);
    res = scd4x_temperature_offset_convert_to_data(&gs_handle, reg, &f_check);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: temperature offset convert to data failed.\n");
        (void)scd4x_deinit(&gs_handle);
        
        return 1;
    }
    scd4x_interface_debug_print("scd4x: check temperature %0.2f.\n", f_check);
    
    /* scd4x_set_sensor_altitude/scd4x_get_sensor_altitude test */
    scd4x_interface_debug_print("scd4x: scd4x_set_sensor_altitude/scd4x_get_sensor_altitude test.\n");
    
    reg = rand() % 0xFFFFU;
    res = scd4x_set_sensor_altitude(&gs_handle, reg);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: set sensor altitude failed.\n");
        (void)scd4x_deinit(&gs_handle);
        
        return 1;
    }
    scd4x_interface_debug_print("scd4x: set sensor altitude %d.\n", reg);
    res = scd4x_get_sensor_altitude(&gs_handle, &reg_check);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: get sensor altitude failed.\n");
        (void)scd4x_deinit(&gs_handle);
        
        return 1;
    }
    scd4x_interface_debug_print("scd4x: check sensor altitude %s.\n", reg == reg_check ? "ok" : "error");
    
    /* scd4x_sensor_altitude_convert_to_register/scd4x_sensor_altitude_convert_to_data test */
    scd4x_interface_debug_print("scd4x: scd4x_sensor_altitude_convert_to_register/scd4x_sensor_altitude_convert_to_data test.\n");
    
    f = (float)(rand() % 10) + 100.0f;
    res = scd4x_sensor_altitude_convert_to_register(&gs_handle, f, &reg);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: sensor altitude convert to register failed.\n");
        (void)scd4x_deinit(&gs_handle);
        
        return 1;
    }
    scd4x_interface_debug_print("scd4x: set altitude %0.2f.\n", f);
    res = scd4x_sensor_altitude_convert_to_data(&gs_handle, reg, &f_check);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: sensor altitude convert to data failed.\n");
        (void)scd4x_deinit(&gs_handle);
        
        return 1;
    }
    scd4x_interface_debug_print("scd4x: check altitude %0.2f.\n", f_check);
    
    /* scd4x_set_ambient_pressure/scd4x_get_ambient_pressure test */
    scd4x_interface_debug_print("scd4x: scd4x_set_ambient_pressure/scd4x_get_ambient_pressure test.\n");
    
    reg = rand() % 0xFFFFU;
    res = scd4x_set_ambient_pressure(&gs_handle, reg);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: set ambient pressure failed.\n");
        (void)scd4x_deinit(&gs_handle);
        
        return 1;
    }
    scd4x_interface_debug_print("scd4x: set ambient pressure %d.\n", reg);
    res = scd4x_get_ambient_pressure(&gs_handle, &reg_check);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: get ambient pressure failed.\n");
        (void)scd4x_deinit(&gs_handle);
        
        return 1;
    }
    scd4x_interface_debug_print("scd4x: check ambient pressure %s.\n", reg == reg_check ? "ok" : "error");
    
    /* scd4x_ambient_pressure_convert_to_register/scd4x_ambient_pressure_convert_to_data test */
    scd4x_interface_debug_print("scd4x: scd4x_ambient_pressure_convert_to_register/scd4x_ambient_pressure_convert_to_data test.\n");
    
    f = (float)(rand() % 1000) + 100000.0f;
    res = scd4x_ambient_pressure_convert_to_register(&gs_handle, f, &reg);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: ambient pressure convert to register failed.\n");
        (void)scd4x_deinit(&gs_handle);
        
        return 1;
    }
    scd4x_interface_debug_print("scd4x: set ambient pressure %0.2f.\n", f);
    res = scd4x_ambient_pressure_convert_to_data(&gs_handle, reg, &f_check);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: ambient pressure convert to data failed.\n");
        (void)scd4x_deinit(&gs_handle);
        
        return 1;
    }
    scd4x_interface_debug_print("scd4x: check ambient pressure %0.2f.\n", f_check);
    
    /* scd4x_co2_convert_to_register/scd4x_co2_convert_to_data test */
    scd4x_interface_debug_print("scd4x: scd4x_co2_convert_to_register/scd4x_co2_convert_to_data test.\n");
    
    f = (float)(rand() % 100);
    res = scd4x_co2_convert_to_register(&gs_handle, f, &reg);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: co2 convert to register failed.\n");
        (void)scd4x_deinit(&gs_handle);
        
        return 1;
    }
    scd4x_interface_debug_print("scd4x: set co2 %0.2f.\n", f);
    res = scd4x_co2_convert_to_data(&gs_handle, reg, &f_check);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: co2 convert to data failed.\n");
        (void)scd4x_deinit(&gs_handle);
        
        return 1;
    }
    scd4x_interface_debug_print("scd4x: check co2 %0.2f.\n", f_check);
    
    /* scd4x_set_automatic_self_calibration/scd4x_get_automatic_self_calibration test */
    scd4x_interface_debug_print("scd4x: scd4x_set_automatic_self_calibration/scd4x_get_automatic_self_calibration test.\n");
    
    /* disable */
    res = scd4x_set_automatic_self_calibration(&gs_handle, SCD4X_BOOL_FALSE);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: set automatic self calibration failed.\n");
        (void)scd4x_deinit(&gs_handle);
        
        return 1;
    }
    scd4x_interface_debug_print("scd4x: disable automatic self calibration.\n");
    res = scd4x_get_automatic_self_calibration(&gs_handle, &enable);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: get automatic self calibration failed.\n");
        (void)scd4x_deinit(&gs_handle);
        
        return 1;
    }
    scd4x_interface_debug_print("scd4x: check automatic self calibration %s.\n", enable == SCD4X_BOOL_FALSE ? "ok" : "error");
    
    /* enable */
    res = scd4x_set_automatic_self_calibration(&gs_handle, SCD4X_BOOL_TRUE);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: set automatic self calibration failed.\n");
        (void)scd4x_deinit(&gs_handle);
        
        return 1;
    }
    scd4x_interface_debug_print("scd4x: enable automatic self calibration.\n");
    res = scd4x_get_automatic_self_calibration(&gs_handle, &enable);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: get automatic self calibration failed.\n");
        (void)scd4x_deinit(&gs_handle);
        
        return 1;
    }
    scd4x_interface_debug_print("scd4x: check automatic self calibration %s.\n", enable == SCD4X_BOOL_TRUE ? "ok" : "error");
    
    /* scd4x_get_data_ready_status test */
    scd4x_interface_debug_print("scd4x: scd4x_get_data_ready_status test.\n");
    
    /* get data ready status */
    res = scd4x_get_data_ready_status(&gs_handle, &enable);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: get data ready status failed.\n");
        (void)scd4x_deinit(&gs_handle);
        
        return 1;
    }
    scd4x_interface_debug_print("scd4x: check data ready status %s.\n", enable == SCD4X_BOOL_TRUE ? "ready" : "not ready");
    
    /* scd4x_get_serial_number test */
    scd4x_interface_debug_print("scd4x: scd4x_get_serial_number test.\n");
    
    /* get serial number */
    res = scd4x_get_serial_number(&gs_handle, number);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: get serial number failed.\n");
        (void)scd4x_deinit(&gs_handle);
        
        return 1;
    }
    scd4x_interface_debug_print("scd4x: serial number is 0x%04X%04X%04X.\n", number[0], number[1], number[2]);
    
    /* scd4x_perform_self_test test */
    scd4x_interface_debug_print("scd4x: scd4x_perform_self_test test.\n");
    
    /* perform self test */
    res = scd4x_perform_self_test(&gs_handle, &enable);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: perform self test failed.\n");
        (void)scd4x_deinit(&gs_handle);
        
        return 1;
    }
    scd4x_interface_debug_print("scd4x: check perform self test %s.\n", enable == SCD4X_BOOL_FALSE ? "ok" : "error");
    
    /* scd4x_perform_factory_reset test */
    scd4x_interface_debug_print("scd4x: scd4x_perform_factory_reset test.\n");
    
    /* perform factory reset */
    res = scd4x_perform_factory_reset(&gs_handle);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: perform factory reset failed.\n");
        (void)scd4x_deinit(&gs_handle);
        
        return 1;
    }
    scd4x_interface_debug_print("scd4x: check perform factory reset %s.\n", res == 0 ? "ok" : "error");
    
    /* scd4x_reinit test */
    scd4x_interface_debug_print("scd4x: scd4x_reinit test.\n");
    
    /* reinit */
    res = scd4x_reinit(&gs_handle);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: reinit failed.\n");
        (void)scd4x_deinit(&gs_handle);
        
        return 1;
    }
    scd4x_interface_debug_print("scd4x: check reinit %s.\n", res == 0 ? "ok" : "error");
    
    /* scd4x_persist_settings test */
    scd4x_interface_debug_print("scd4x: scd4x_persist_settings test.\n");
    
    /* persist settings */
    res = scd4x_persist_settings(&gs_handle);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: persist settings failed.\n");
        (void)scd4x_deinit(&gs_handle);
        
        return 1;
    }
    scd4x_interface_debug_print("scd4x: check persist settings %s.\n", res == 0 ? "ok" : "error");
    
    /* scd4x_perform_forced_recalibration test */
    scd4x_interface_debug_print("scd4x: scd4x_perform_forced_recalibration test.\n");
    
    res = scd4x_stop_periodic_measurement(&gs_handle);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: stop periodic measurement failed.\n");
        (void)scd4x_deinit(&gs_handle);
        
        return 1;
    }
    reg = rand() % 5;
    res = scd4x_perform_forced_recalibration(&gs_handle, reg, &reg_check);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: perform forced recalibration failed.\n");
        (void)scd4x_deinit(&gs_handle);
        
        return 1;
    }
    scd4x_interface_debug_print("scd4x: check perform forced recalibration 0x%04X.\n", reg_check);
    
    /* scd41 test */
    if (type == SCD41)
    {
        /* scd4x_power_down/scd4x_wake_up test */
        scd4x_interface_debug_print("scd4x: scd4x_power_down/scd4x_wake_up test.\n");
        
        /* power down */
        res = scd4x_power_down(&gs_handle);
        if (res != 0)
        {
            scd4x_interface_debug_print("scd4x: power down failed.\n");
            (void)scd4x_deinit(&gs_handle);
            
            return 1;
        }
        scd4x_interface_debug_print("scd4x: power down.\n");
        
        /* wake up */
        res = scd4x_wake_up(&gs_handle);
        if (res != 0)
        {
            scd4x_interface_debug_print("scd4x: wake up failed.\n");
            (void)scd4x_deinit(&gs_handle);
            
            return 1;
        }
        scd4x_interface_debug_print("scd4x: wake up.\n");
        
        /* scd4x_set_automatic_self_calibration_initial_period/scd4x_get_automatic_self_calibration_initial_period test */
        scd4x_interface_debug_print("scd4x: scd4x_set_automatic_self_calibration_initial_period/scd4x_get_automatic_self_calibration_initial_period test.\n");
        
        reg = (rand() % 0xFF) * 4;
        res = scd4x_set_automatic_self_calibration_initial_period(&gs_handle, reg);
        if (res != 0)
        {
            scd4x_interface_debug_print("scd4x: set automatic self calibration initial period failed.\n");
            (void)scd4x_deinit(&gs_handle);
            
            return 1;
        }
        scd4x_interface_debug_print("scd4x: set automatic self calibration initial period %d.\n", reg);
        res = scd4x_get_automatic_self_calibration_initial_period(&gs_handle, &reg_check);
        if (res != 0)
        {
            scd4x_interface_debug_print("scd4x: get automatic self calibration initial period failed.\n");
            (void)scd4x_deinit(&gs_handle);
            
            return 1;
        }
        scd4x_interface_debug_print("scd4x: check automatic self calibration initial period %s.\n", reg == reg_check ? "ok" : "error");
        
        /* scd4x_set_automatic_self_calibration_standard_period/scd4x_get_automatic_self_calibration_standard_period test */
        scd4x_interface_debug_print("scd4x: scd4x_set_automatic_self_calibration_standard_period/scd4x_get_automatic_self_calibration_standard_period test.\n");
        
        reg = (rand() % 0xFF) * 4;
        res = scd4x_set_automatic_self_calibration_standard_period(&gs_handle, reg);
        if (res != 0)
        {
            scd4x_interface_debug_print("scd4x: set automatic self calibration standard period failed.\n");
            (void)scd4x_deinit(&gs_handle);
            
            return 1;
        }
        scd4x_interface_debug_print("scd4x: set automatic self calibration standard period %d.\n", reg);
        res = scd4x_get_automatic_self_calibration_standard_period(&gs_handle, &reg_check);
        if (res != 0)
        {
            scd4x_interface_debug_print("scd4x: get automatic self calibration standard period failed.\n");
            (void)scd4x_deinit(&gs_handle);
            
            return 1;
        }
        scd4x_interface_debug_print("scd4x: check automatic self calibration standard period %s.\n", reg == reg_check ? "ok" : "error");
    }
    
    /* finish register test */
    scd4x_interface_debug_print("scd4x: finish register test.\n");
    (void)scd4x_deinit(&gs_handle);
    
    return 0;
}

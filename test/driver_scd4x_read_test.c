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
 * @file      driver_scd4x_read_test.c
 * @brief     driver scd4x read test source file
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

#include "driver_scd4x_read_test.h"

static scd4x_handle_t gs_handle;        /**< scd4x handle */

/**
 * @brief     read test
 * @param[in] type is the chip type
 * @param[in] times is test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t scd4x_read_test(scd4x_t type, uint32_t times)
{
    uint8_t res;
    uint32_t i;
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
    
    /* start read test */
    scd4x_interface_debug_print("scd4x: start read test.\n");
    
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
    
    /* continuous read test */
    scd4x_interface_debug_print("scd4x: continuous read test.\n");
    
    /* start */
    res = scd4x_start_periodic_measurement(&gs_handle);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: start periodic measurement failed.\n");
        (void)scd4x_deinit(&gs_handle);
        
        return 1;
    }
    
    /* loop */
    for (i = 0; i < times; i++)
    {
        uint16_t co2_raw;
        uint16_t co2_ppm;
        uint16_t temperature_raw;
        float temperature_s;
        uint16_t humidity_raw;
        float humidity_s;
        
        /* delay 5000ms */
        scd4x_interface_delay_ms(5000);
        
        /* read data */
        res = scd4x_read(&gs_handle, &co2_raw, &co2_ppm,
                         &temperature_raw, &temperature_s,
                         &humidity_raw, &humidity_s);
        if (res != 0)
        {
            scd4x_interface_debug_print("scd4x: read failed.\n");
            (void)scd4x_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        scd4x_interface_debug_print("scd4x: co2 is %02dppm.\n", co2_ppm);
        scd4x_interface_debug_print("scd4x: temperature is %0.2fC.\n", temperature_s);
        scd4x_interface_debug_print("scd4x: humidity is %0.2f%%.\n", humidity_s);
    }
    
    /* stop */
    res = scd4x_stop_periodic_measurement(&gs_handle);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: stop periodic measurement failed.\n");
        (void)scd4x_deinit(&gs_handle);
        
        return 1;
    }
    
    /* low power continuous read test */
    scd4x_interface_debug_print("scd4x: low power continuous read test.\n");
    
    /* start */
    res = scd4x_start_low_power_periodic_measurement(&gs_handle);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: start low power periodic measurement failed.\n");
        (void)scd4x_deinit(&gs_handle);
        
        return 1;
    }
    
    /* loop */
    for (i = 0; i < times; i++)
    {
        uint16_t co2_raw;
        uint16_t co2_ppm;
        uint16_t temperature_raw;
        float temperature_s;
        uint16_t humidity_raw;
        float humidity_s;
        
        /* delay 30000ms */
        scd4x_interface_delay_ms(30000);
        
        /* read data */
        res = scd4x_read(&gs_handle, &co2_raw, &co2_ppm,
                         &temperature_raw, &temperature_s,
                         &humidity_raw, &humidity_s);
        if (res != 0)
        {
            scd4x_interface_debug_print("scd4x: read failed.\n");
            (void)scd4x_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        scd4x_interface_debug_print("scd4x: co2 is %02dppm.\n", co2_ppm);
        scd4x_interface_debug_print("scd4x: temperature is %0.2fC.\n", temperature_s);
        scd4x_interface_debug_print("scd4x: humidity is %0.2f%%.\n", humidity_s);
    }
    
    /* stop */
    res = scd4x_stop_periodic_measurement(&gs_handle);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: stop periodic measurement failed.\n");
        (void)scd4x_deinit(&gs_handle);
        
        return 1;
    }
    
    /* only scd41 */
    if (type == SCD41)
    {
        uint32_t timeout;
        scd4x_bool_t enable;
        
        /* measure single shot test */
        scd4x_interface_debug_print("scd4x: measure single shot test.\n");
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            /* measure single shot */
            res = scd4x_measure_single_shot(&gs_handle);
            if (res != 0)
            {
                scd4x_interface_debug_print("scd4x: measure single shot failed.\n");
                (void)scd4x_deinit(&gs_handle);
                
                return 1;
            }
            
            /* set 30s */
            timeout = 30;
            
            /* check timeout */
            while (timeout != 0)
            {
                /* delay 1000ms */
                scd4x_interface_delay_ms(1000);
                
                /* timeout-- */
                timeout--;
                
                /* read data */
                res = scd4x_get_data_ready_status(&gs_handle, &enable);
                if (res != 0)
                {
                    scd4x_interface_debug_print("scd4x: measure single shot failed.\n");
                    (void)scd4x_deinit(&gs_handle);
                    
                    return 1;
                }
                if (enable == SCD4X_BOOL_TRUE)
                {
                    uint16_t co2_raw;
                    uint16_t co2_ppm;
                    uint16_t temperature_raw;
                    float temperature_s;
                    uint16_t humidity_raw;
                    float humidity_s;
                    
                    /* read data */
                    res = scd4x_read(&gs_handle, &co2_raw, &co2_ppm,
                                     &temperature_raw, &temperature_s,
                                     &humidity_raw, &humidity_s);
                    if (res != 0)
                    {
                        scd4x_interface_debug_print("scd4x: read failed.\n");
                        (void)scd4x_deinit(&gs_handle);
                        
                        return 1;
                    }
                    
                    /* output */
                    scd4x_interface_debug_print("scd4x: co2 is %02dppm.\n", co2_ppm);
                    scd4x_interface_debug_print("scd4x: temperature is %0.2fC.\n", temperature_s);
                    scd4x_interface_debug_print("scd4x: humidity is %0.2f%%.\n", humidity_s);
                    
                    break;
                }
            }
            if (timeout == 0)
            {
                scd4x_interface_debug_print("scd4x: timeout.\n");
                (void)scd4x_deinit(&gs_handle);
                
                return 1;
            }
        }
        
        /* measure single shot rht only test */
        scd4x_interface_debug_print("scd4x: measure single shot rht only test.\n");
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            /* measure single shot rht only */
            res = scd4x_measure_single_shot_rht_only(&gs_handle);
            if (res != 0)
            {
                scd4x_interface_debug_print("scd4x: measure single shot rht only failed.\n");
                (void)scd4x_deinit(&gs_handle);
                
                return 1;
            }
            
            /* set 30s */
            timeout = 30;
            
            /* check timeout */
            while (timeout != 0)
            {
                /* delay 1000ms */
                scd4x_interface_delay_ms(1000);
                
                /* timeout-- */
                timeout--;
                
                /* read data */
                res = scd4x_get_data_ready_status(&gs_handle, &enable);
                if (res != 0)
                {
                    scd4x_interface_debug_print("scd4x: measure single shot failed.\n");
                    (void)scd4x_deinit(&gs_handle);
                    
                    return 1;
                }
                if (enable == SCD4X_BOOL_TRUE)
                {
                    uint16_t co2_raw;
                    uint16_t co2_ppm;
                    uint16_t temperature_raw;
                    float temperature_s;
                    uint16_t humidity_raw;
                    float humidity_s;
                    
                    /* read data */
                    res = scd4x_read(&gs_handle, &co2_raw, &co2_ppm,
                                     &temperature_raw, &temperature_s,
                                     &humidity_raw, &humidity_s);
                    if (res != 0)
                    {
                        scd4x_interface_debug_print("scd4x: read failed.\n");
                        (void)scd4x_deinit(&gs_handle);
                        
                        return 1;
                    }
                    
                    /* output */
                    scd4x_interface_debug_print("scd4x: humidity is %0.2f%%.\n", humidity_s);
                    
                    break;
                }
            }
            if (timeout == 0)
            {
                scd4x_interface_debug_print("scd4x: timeout.\n");
                (void)scd4x_deinit(&gs_handle);
                
                return 1;
            }
        }
    }
    
    /* finish read test */
    scd4x_interface_debug_print("scd4x: finish read test.\n");
    (void)scd4x_deinit(&gs_handle);

    return 0;
}

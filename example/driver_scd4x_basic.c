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
 * @file      driver_scd4x_basic.c
 * @brief     driver scd4x basic source file
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

#include "driver_scd4x_basic.h"

static scd4x_handle_t gs_handle;        /**< scd4x handle */

/**
 * @brief     basic example init
 * @param[in] type is the chip type
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t scd4x_basic_init(scd4x_t type)
{
    uint8_t res;
    
    /* link functions */
    DRIVER_SCD4X_LINK_INIT(&gs_handle, scd4x_handle_t);
    DRIVER_SCD4X_LINK_IIC_INIT(&gs_handle, scd4x_interface_iic_init);
    DRIVER_SCD4X_LINK_IIC_DEINIT(&gs_handle, scd4x_interface_iic_deinit);
    DRIVER_SCD4X_LINK_IIC_WRITE_COMMAND(&gs_handle, scd4x_interface_iic_write_cmd);
    DRIVER_SCD4X_LINK_IIC_READ_COMMAND(&gs_handle, scd4x_interface_iic_read_cmd);
    DRIVER_SCD4X_LINK_DELAY_MS(&gs_handle, scd4x_interface_delay_ms);
    DRIVER_SCD4X_LINK_DEBUG_PRINT(&gs_handle, scd4x_interface_debug_print);

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
    
    /* start */
    res = scd4x_start_periodic_measurement(&gs_handle);
    if (res != 0)
    {
        scd4x_interface_debug_print("scd4x: start periodic measurement failed.\n");
        (void)scd4x_deinit(&gs_handle);
        
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example read
 * @param[out] *co2_ppm points to a converted co2 buffer
 * @param[out] *temperature points to a converted temperature buffer
 * @param[out] *humidity points to a converted humidity buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       delay over 5s
 */
uint8_t scd4x_basic_read(uint16_t *co2_ppm, float *temperature, float *humidity)
{
    uint8_t res;
    uint16_t co2_raw;
    uint16_t temperature_raw;
    uint16_t humidity_raw;
    
    /* read data */
    res = scd4x_read(&gs_handle, &co2_raw, co2_ppm,
                     &temperature_raw, temperature,
                     &humidity_raw, humidity);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t scd4x_basic_deinit(void)
{
    /* stop */
    if (scd4x_stop_periodic_measurement(&gs_handle) != 0)
    {
        return 1;
    }
    
    /* close scd4x */
    if (scd4x_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example get serial number
 * @param[out] *num points to a number buffer
 * @return     status code
 *             - 0 success
 *             - 1 get serial number failed
 * @note       none
 */
uint8_t scd4x_basic_get_serial_number(uint16_t num[3])
{
    /* get serial number */
    if (scd4x_get_serial_number(&gs_handle, num) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic wake up
 * @return status code
 *         - 0 success
 *         - 1 wake up failed
 * @note   none
 */
uint8_t scd4x_basic_wake_up(void)
{
    /* wake up */
    if (scd4x_wake_up(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic power down
 * @return status code
 *         - 0 success
 *         - 1 power down failed
 * @note   none
 */
uint8_t scd4x_basic_power_down(void)
{
    /* power down */
    if (scd4x_power_down(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

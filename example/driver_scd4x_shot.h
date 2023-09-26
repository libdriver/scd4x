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
 * @file      driver_scd4x_shot.h
 * @brief     driver scd4x shot header file
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

#ifndef DRIVER_SCD4X_SHOT_H
#define DRIVER_SCD4X_SHOT_H

#include "driver_scd4x_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup scd4x_example_driver
 * @{
 */

/**
 * @brief     shot example init
 * @param[in] type is the chip type
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t scd4x_shot_init(scd4x_t type);

/**
 * @brief  shot example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t scd4x_shot_deinit(void);

/**
 * @brief      shot example read
 * @param[out] *co2_ppm points to a converted co2 buffer
 * @param[out] *temperature points to a converted temperature buffer
 * @param[out] *humidity points to a converted humidity buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 timeout
 * @note       none
 */
uint8_t scd4x_shot_read(uint16_t *co2_ppm, float *temperature, float *humidity);

/**
 * @brief      shot example get serial number
 * @param[out] *num points to a number buffer
 * @return     status code
 *             - 0 success
 *             - 1 get serial number failed
 * @note       none
 */
uint8_t scd4x_shot_get_serial_number(uint16_t num[3]);

/**
 * @brief  shot wake up
 * @return status code
 *         - 0 success
 *         - 1 wake up failed
 * @note   none
 */
uint8_t scd4x_shot_wake_up(void);

/**
 * @brief  shot power down
 * @return status code
 *         - 0 success
 *         - 1 power down failed
 * @note   none
 */
uint8_t scd4x_shot_power_down(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif

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
 * @file      driver_scd4x.c
 * @brief     driver scd4x source file
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

#include "driver_scd4x.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Sensirion SCD4X"        /**< chip name */
#define MANUFACTURER_NAME         "Sensirion"              /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        2.40f                    /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        5.50f                    /**< chip max supply voltage */
#define MAX_CURRENT               205.0f                   /**< chip max current */
#define TEMPERATURE_MIN           -10.0f                   /**< chip min operating temperature */
#define TEMPERATURE_MAX           60.0f                    /**< chip max operating temperature */
#define DRIVER_VERSION            1000                     /**< driver version */

/**
 * @brief chip address definition
 */
#define SCD4X_ADDRESS             (0x62 << 1)              /**< chip iic address */

/**
 * @brief chip command definition
 */
#define SCD4X_COMMAND_START_PERIODIC                                0x21B1U        /**< start periodic measurement command */
#define SCD4X_COMMAND_READ                                          0xEC05U        /**< read measurement command */
#define SCD4X_COMMAND_STOP_PERIODIC                                 0x3F86U        /**< stop periodic measurement command */
#define SCD4X_COMMAND_SET_TEMPERATURE_OFFSET                        0x241DU        /**< set temperature offset command */
#define SCD4X_COMMAND_GET_TEMPERATURE_OFFSET                        0x2318U        /**< get temperature offset command */
#define SCD4X_COMMAND_SET_SENSOR_ALTITUDE                           0x2427U        /**< set sensor altitude command */
#define SCD4X_COMMAND_GET_SENSOR_ALTITUDE                           0x2322U        /**< get sensor altitude command */
#define SCD4X_COMMAND_SET_AMBIENT_PRESSURE                          0xE000U        /**< set ambient pressure command */
#define SCD4X_COMMAND_GET_AMBIENT_PRESSURE                          0xE000U        /**< get ambient pressure command */
#define SCD4X_COMMAND_PERFORM_FORCED_RECALIBRATION                  0x362FU        /**< perform forced recalibration command */
#define SCD4X_COMMAND_SET_AUTO_SELF_CALIBRATION                     0x2416U        /**< set automatic self calibration enabled command */
#define SCD4X_COMMAND_GET_AUTO_SELF_CALIBRATION                     0x2313U        /**< get automatic self calibration enabled command */
#define SCD4X_COMMAND_START_LOW_POWER_PERIODIC                      0x21ACU        /**< start low power periodic measurement command */
#define SCD4X_COMMAND_GET_DATA_READY_STATUS                         0xE4B8U        /**< get data ready status command */
#define SCD4X_COMMAND_PERSIST_SETTINGS                              0x3615U        /**< persist settings command */
#define SCD4X_COMMAND_GET_SERIAL_NUMBER                             0x3682U        /**< get serial number command */
#define SCD4X_COMMAND_PERFORM_SELF_TEST                             0x3639U        /**< perform self test command */
#define SCD4X_COMMAND_PERFORM_FACTORY_RESET                         0x3632U        /**< perform factory reset command */
#define SCD4X_COMMAND_REINIT                                        0x3646U        /**< reinit command */
#define SCD4X_COMMAND_MEASURE_SINGLE_SHOT                           0x219DU        /**< measure single shot command */
#define SCD4X_COMMAND_MEASURE_SINGLE_SHOT_RHT_ONLY                  0x2196U        /**< measure single shot rht only command */
#define SCD4X_COMMAND_POWER_DOWN                                    0x36E0U        /**< power down command */
#define SCD4X_COMMAND_WAKE_UP                                       0x36F6U        /**< wake up command */
#define SCD4X_COMMAND_SET_AUTO_SELF_CALIBRATION_INIT_PERIOD         0x2445U        /**< set automatic self calibration initial period command */
#define SCD4X_COMMAND_GET_AUTO_SELF_CALIBRATION_INIT_PERIOD         0x2340U        /**< get automatic self calibration initial period command */
#define SCD4X_COMMAND_SET_AUTO_SELF_CALIBRATION_STANDARD_PERIOD     0x244EU        /**< set automatic self calibration standard period command */
#define SCD4X_COMMAND_GET_AUTO_SELF_CALIBRATION_STANDARD_PERIOD     0x234BU        /**< get automatic self calibration standard period command */

/**
 * @brief crc8 definition
 */
#define SCD4X_CRC8_POLYNOMIAL        0x31
#define SCD4X_CRC8_INIT              0xFF

/**
 * @brief      read bytes with param
 * @param[in]  *handle points to an scd4x handle structure
 * @param[in]  reg is the iic register address
 * @param[in]  *data points to a data buffer
 * @param[in]  len is the data length
 * @param[in]  delay_ms is the delay time in ms
 * @param[out] *output points to an output buffer
 * @param[in]  output_len is the output length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_scd4x_iic_read_with_param(scd4x_handle_t *handle, uint16_t reg, uint8_t *data, uint16_t len,
                                           uint16_t delay_ms, uint8_t *output, uint16_t output_len)
{
    uint8_t buf[16];
    uint16_t i;
        
    if ((len + 2) > 16)                                                            /* check length */
    {
        return 1;                                                                  /* return error */
    }
    memset(buf, 0, sizeof(uint8_t) * 16);                                          /* clear the buffer */
    buf[0] = (uint8_t)((reg >> 8) & 0xFF);                                         /* set MSB of reg */
    buf[1] = (uint8_t)(reg & 0xFF);                                                /* set LSB of reg */
    for (i = 0; i < len; i++)
    {
        buf[2 + i] = data[i];                                                      /* copy write data */
    }
    
    if (handle->iic_write_cmd(SCD4X_ADDRESS, (uint8_t *)buf, len + 2) != 0)        /* write iic command */
    {
        return 1;                                                                  /* write command */
    }
    handle->delay_ms(delay_ms);                                                    /* delay ms */
    if (handle->iic_read_cmd(SCD4X_ADDRESS, output, output_len) != 0)              /* read data */
    {
        return 1;                                                                  /* write command */
    }
    else
    {
        return 0;                                                                  /* success return 0 */
    }
}

/**
 * @brief      read bytes
 * @param[in]  *handle points to an scd4x handle structure
 * @param[in]  reg is the iic register address
 * @param[out] *data points to a data buffer
 * @param[in]  len is the data length
 * @param[in]  delay_ms is the delay time in ms
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_scd4x_iic_read(scd4x_handle_t *handle, uint16_t reg, uint8_t *data, uint16_t len, uint16_t delay_ms)
{
    uint8_t buf[2];
    
    memset(buf, 0, sizeof(uint8_t) * 2);                                     /* clear the buffer */
    buf[0] = (uint8_t)((reg >> 8) & 0xFF);                                   /* set reg MSB */
    buf[1] = (uint8_t)(reg & 0xFF);                                          /* set reg LSB */
    if (handle->iic_write_cmd(SCD4X_ADDRESS, (uint8_t *)buf, 2) != 0)        /* write command */
    {   
        return 1;                                                            /* return error */
    }
    handle->delay_ms(delay_ms);                                              /* delay ms */
    if (handle->iic_read_cmd(SCD4X_ADDRESS, data, len) != 0)                 /* read data */
    {
        return 1;                                                            /* write command */
    }
    else
    {
        return 0;                                                            /* success return 0 */
    }
}

/**
 * @brief     write bytes
 * @param[in] *handle points to an scd4x handle structure
 * @param[in] reg is the iic register address
 * @param[in] *data points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_scd4x_iic_write(scd4x_handle_t *handle, uint16_t reg, uint8_t *data, uint16_t len)
{
    uint8_t buf[16];
    uint16_t i;
        
    if ((len + 2) > 16)                                                            /* check length */
    {
        return 1;                                                                  /* return error */
    }
    memset(buf, 0, sizeof(uint8_t) * 16);                                          /* clear the buffer */
    buf[0] = (uint8_t)((reg >> 8) & 0xFF);                                         /* set MSB of reg */
    buf[1] = (uint8_t)(reg & 0xFF);                                                /* set LSB of reg */
    for (i = 0; i < len; i++)
    {
        buf[2 + i] = data[i];                                                      /* copy write data */
    }
    
    if (handle->iic_write_cmd(SCD4X_ADDRESS, (uint8_t *)buf, len + 2) != 0)        /* write iic command */
    {
        return 1;                                                                  /* write command */
    }
    else
    {
        return 0;                                                                  /* success return 0 */
    }
}

/**
 * @brief     generate the crc
 * @param[in] *data points to a data buffer
 * @param[in] count is the data length
 * @return    crc
 * @note      none
 */
static uint8_t a_scd4x_generate_crc(uint8_t* data, uint8_t count)
{
    uint8_t current_byte;
    uint8_t crc = SCD4X_CRC8_INIT;
    uint8_t crc_bit;

    for (current_byte = 0; current_byte < count; ++current_byte)        /* calculate crc */
    {
        crc ^= (data[current_byte]);                                    /* xor data */
        for (crc_bit = 8; crc_bit > 0; --crc_bit)                       /* 8 bit */
        {
            if ((crc & 0x80) != 0)                                      /* if 7th bit is 1 */
            {
                crc = (crc << 1) ^ SCD4X_CRC8_POLYNOMIAL;               /* xor */
            }
            else
            {
                crc = crc << 1;                                         /* left shift 1 */
            }
        }
    }
    
    return crc;                                                         /* return crc */
}

/**
 * @brief     set type
 * @param[in] *handle points to an scd4x handle structure
 * @param[in] type is the chip type
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t scd4x_set_type(scd4x_handle_t *handle, scd4x_t type)
{
    if (handle == NULL)         /* check handle */
    {
        return 2;               /* return error */
    }
    
    handle->type = type;        /* set type */
    
    return 0;                   /* success return 0 */
}

/**
 * @brief      get type
 * @param[in]  *handle points to an scd4x handle structure
 * @param[out] *type points to a chip type buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t scd4x_get_type(scd4x_handle_t *handle, scd4x_t *type)
{
    if (handle == NULL)                     /* check handle */
    {
        return 2;                           /* return error */
    }
    
    *type = (scd4x_t)(handle->type);        /* get type */
    
    return 0;                               /* success return 0 */
}

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
uint8_t scd4x_start_periodic_measurement(scd4x_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }

    res = a_scd4x_iic_write(handle, SCD4X_COMMAND_START_PERIODIC, NULL, 0);        /* write config */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("scd4x: start periodic measurement failed.\n");        /* start periodic measurement failed */
       
        return 1;                                                                  /* return error */
    }
    
    return 0;                                                                      /* success return 0 */
}

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
                   uint16_t *humidity_raw, float *humidity_s)
{
    uint8_t res;
    uint8_t buf[9];
    uint16_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_scd4x_iic_read(handle, SCD4X_COMMAND_GET_DATA_READY_STATUS, buf, 3, 1);       /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("scd4x: get data ready status failed.\n");                    /* get data ready status failed */
       
        return 1;                                                                         /* return error */
    }
    if (buf[2] != a_scd4x_generate_crc(&buf[0], 2))                                       /* check crc */
    {
        handle->debug_print("scd4x: crc is error.\n");                                    /* crc is error */
       
        return 4;                                                                         /* return error */
    }
    prev = (uint16_t)(((uint16_t)buf[0]) << 8) | buf[1];                                  /* set prev */
    if ((prev & 0x0FFF) == 0)                                                             /* check data */
    {
        handle->debug_print("scd4x: data is not ready.\n");                               /* data is not ready */
       
        return 5;                                                                         /* return error */
    }
    
    res = a_scd4x_iic_read(handle, SCD4X_COMMAND_READ, buf, 9, 1);                        /* read data */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("scd4x: read failed.\n");                                     /* read failed */
       
        return 1;                                                                         /* return error */
    }
    
    if (buf[2] != a_scd4x_generate_crc(&buf[0], 2))                                       /* check crc */
    {
        handle->debug_print("scd4x: crc is error.\n");                                    /* crc is error */
       
        return 4;                                                                         /* return error */
    }
    if (buf[5] != a_scd4x_generate_crc(&buf[3], 2))                                       /* check crc */
    {
        handle->debug_print("scd4x: crc is error.\n");                                    /* crc is error */
       
        return 4;                                                                         /* return error */
    }
    if (buf[8] != a_scd4x_generate_crc(&buf[6], 2))                                       /* check crc */
    {
        handle->debug_print("scd4x: crc is error.\n");                                    /* crc is error */
       
        return 4;                                                                         /* return error */
    }
    
    *co2_raw = (uint16_t)(((uint16_t)buf[0]) << 8) | buf[1];                              /* set co2 raw */
    *temperature_raw = (uint16_t)(((uint16_t)buf[3]) << 8) | buf[4];                      /* set temperature raw */
    *humidity_raw = (uint16_t)(((uint16_t)buf[6]) << 8) | buf[7];                         /* set humidity raw */
    *co2_ppm = *co2_raw;                                                                  /* set co2 ppm */
    *temperature_s = -45.0f + 175.0f * (float)(*temperature_raw) / 65535.0f;              /* set temperature */
    *humidity_s = 100.0f * (float)(*humidity_raw) / 65535.0f;                             /* set humidity */
    
    return 0;                                                                             /* success return 0 */
}

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
uint8_t scd4x_stop_periodic_measurement(scd4x_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }

    res = a_scd4x_iic_write(handle, SCD4X_COMMAND_STOP_PERIODIC, NULL, 0);        /* write config */
    if (res != 0)                                                                 /* check result */
    {
        handle->debug_print("scd4x: stop periodic measurement failed.\n");        /* stop periodic measurement failed */
       
        return 1;                                                                 /* return error */
    }
    handle->delay_ms(500);                                                        /* delay 500ms */
    
    return 0;                                                                     /* success return 0 */
}

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
uint8_t scd4x_set_temperature_offset(scd4x_handle_t *handle, uint16_t offset)
{
    uint8_t res;
    uint8_t buf[3];
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }

    buf[0] = (offset >> 8) & 0xFF;                                                        /* msb */
    buf[1] = (offset >> 0) & 0xFF;                                                        /* lsb */
    buf[2] = a_scd4x_generate_crc(&buf[0], 2);                                            /* set crc */
    res = a_scd4x_iic_write(handle, SCD4X_COMMAND_SET_TEMPERATURE_OFFSET, buf, 3);        /* write config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("scd4x: set temperature offset failed.\n");                   /* set temperature offset failed */
       
        return 1;                                                                         /* return error */
    }
    handle->delay_ms(1);                                                                  /* delay 1ms */
    
    return 0;                                                                             /* success return 0 */
}

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
uint8_t scd4x_get_temperature_offset(scd4x_handle_t *handle, uint16_t *offset)
{
    uint8_t res;
    uint8_t buf[3];
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }

    res = a_scd4x_iic_read(handle, SCD4X_COMMAND_GET_TEMPERATURE_OFFSET, buf, 3, 1);        /* read config */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("scd4x: get temperature offset failed.\n");                     /* get temperature offset failed */
       
        return 1;                                                                           /* return error */
    }
    if (buf[2] != a_scd4x_generate_crc(&buf[0], 2))                                         /* check crc */
    {
        handle->debug_print("scd4x: crc is error.\n");                                      /* crc is error */
       
        return 4;                                                                           /* return error */
    }
    *offset = (uint16_t)(((uint16_t)buf[0]) << 8) | buf[1];                                 /* set offset */
    
    return 0;                                                                               /* success return 0 */
}

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
uint8_t scd4x_temperature_offset_convert_to_register(scd4x_handle_t *handle, float degrees, uint16_t *reg)
{
    if (handle == NULL)                                      /* check handle */
    {
        return 2;                                            /* return error */
    }
    if (handle->inited != 1)                                 /* check handle initialization */
    {
        return 3;                                            /* return error */
    }
    
    *reg = (uint16_t)(degrees * (65535.0f / 175.0f));        /* convert real data to register data */
    
    return 0;                                                /* success return 0 */
}

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
uint8_t scd4x_temperature_offset_convert_to_data(scd4x_handle_t *handle, uint16_t reg, float *degrees)
{
    if (handle == NULL)                                 /* check handle */
    {
        return 2;                                       /* return error */
    }
    if (handle->inited != 1)                            /* check handle initialization */
    {
        return 3;                                       /* return error */
    }
    
    *degrees = (float)(reg) * 175.0f / 65535.0f;        /* convert raw data to real data */
    
    return 0;                                           /* success return 0 */
}

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
uint8_t scd4x_set_sensor_altitude(scd4x_handle_t *handle, uint16_t altitude)
{
    uint8_t res;
    uint8_t buf[3];
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }

    buf[0] = (altitude >> 8) & 0xFF;                                                    /* msb */
    buf[1] = (altitude >> 0) & 0xFF;                                                    /* lsb */
    buf[2] = a_scd4x_generate_crc(&buf[0], 2);                                          /* set crc */
    res = a_scd4x_iic_write(handle, SCD4X_COMMAND_SET_SENSOR_ALTITUDE , buf, 3);        /* write config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("scd4x: set sensor altitude failed.\n");                    /* set sensor altitude failed */
       
        return 1;                                                                       /* return error */
    }
    handle->delay_ms(1);                                                                /* delay 1ms */
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t scd4x_get_sensor_altitude(scd4x_handle_t *handle, uint16_t *altitude)
{
    uint8_t res;
    uint8_t buf[3];
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }

    res = a_scd4x_iic_read(handle, SCD4X_COMMAND_GET_SENSOR_ALTITUDE, buf, 3, 1);        /* read config */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("scd4x: get sensor altitude failed.\n");                     /* get sensor altitude failed */
       
        return 1;                                                                        /* return error */
    }
    if (buf[2] != a_scd4x_generate_crc(&buf[0], 2))                                      /* check crc */
    {
        handle->debug_print("scd4x: crc is error.\n");                                   /* crc is error */
       
        return 4;                                                                        /* return error */
    }
    *altitude = (uint16_t)(((uint16_t)buf[0]) << 8) | buf[1];                            /* set offset */
    
    return 0;                                                                            /* success return 0 */
}

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
uint8_t scd4x_sensor_altitude_convert_to_register(scd4x_handle_t *handle, float m, uint16_t *reg)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    *reg = (uint16_t)(m);           /* convert real data to register data */
    
    return 0;                       /* success return 0 */
}

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
uint8_t scd4x_sensor_altitude_convert_to_data(scd4x_handle_t *handle, uint16_t reg, float *m)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    *m = (float)(reg);              /* convert raw data to real data */
    
    return 0;                       /* success return 0 */
}

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
uint8_t scd4x_set_ambient_pressure(scd4x_handle_t *handle, uint16_t pressure)
{
    uint8_t res;
    uint8_t buf[3];
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }

    buf[0] = (pressure >> 8) & 0xFF;                                                    /* msb */
    buf[1] = (pressure >> 0) & 0xFF;                                                    /* lsb */
    buf[2] = a_scd4x_generate_crc(&buf[0], 2);                                          /* set crc */
    res = a_scd4x_iic_write(handle, SCD4X_COMMAND_SET_AMBIENT_PRESSURE , buf, 3);       /* write config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("scd4x: set ambient pressure failed.\n");                   /* set ambient pressure failed */
       
        return 1;                                                                       /* return error */
    }
    handle->delay_ms(1);                                                                /* delay 1ms */
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t scd4x_get_ambient_pressure(scd4x_handle_t *handle, uint16_t *pressure)
{
    uint8_t res;
    uint8_t buf[3];
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }

    res = a_scd4x_iic_read(handle, SCD4X_COMMAND_GET_AMBIENT_PRESSURE, buf, 3, 1);       /* read config */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("scd4x: get ambient pressure failed.\n");                    /* get ambient pressure failed */
       
        return 1;                                                                        /* return error */
    }
    if (buf[2] != a_scd4x_generate_crc(&buf[0], 2))                                      /* check crc */
    {
        handle->debug_print("scd4x: crc is error.\n");                                   /* crc is error */
       
        return 4;                                                                        /* return error */
    }
    *pressure = (uint16_t)(((uint16_t)buf[0]) << 8) | buf[1];                            /* set offset */
    
    return 0;                                                                            /* success return 0 */
}

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
uint8_t scd4x_ambient_pressure_convert_to_register(scd4x_handle_t *handle, float pa, uint16_t *reg)
{
    if (handle == NULL)                       /* check handle */
    {
        return 2;                             /* return error */
    }
    if (handle->inited != 1)                  /* check handle initialization */
    {
        return 3;                             /* return error */
    }
    
    *reg = (uint16_t)(pa / 100.0f);           /* convert real data to register data */
    
    return 0;                                 /* success return 0 */
}

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
uint8_t scd4x_ambient_pressure_convert_to_data(scd4x_handle_t *handle, uint16_t reg, float *pa)
{
    if (handle == NULL)                 /* check handle */
    {
        return 2;                       /* return error */
    }
    if (handle->inited != 1)            /* check handle initialization */
    {
        return 3;                       /* return error */
    }
    
    *pa = (float)(reg * 100.0f);        /* convert raw data to real data */
    
    return 0;                           /* success return 0 */
}

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
uint8_t scd4x_perform_forced_recalibration(scd4x_handle_t *handle, uint16_t co2_raw, uint16_t *frc)
{
    uint8_t res;
    uint8_t in_buf[3];
    uint8_t out_buf[3];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    in_buf[0] = (co2_raw >> 8) & 0xFF;                                                           /* set msb */
    in_buf[1] = (co2_raw >> 0) & 0xFF;                                                           /* set lsb */
    in_buf[2] = a_scd4x_generate_crc(&in_buf[0], 2);                                             /* set crc */
    res = a_scd4x_iic_read_with_param(handle, SCD4X_COMMAND_PERFORM_FORCED_RECALIBRATION, 
                                      in_buf, 3, 400, out_buf, 3);                               /* read config */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("scd4x: perform forced recalibration failed.\n");                    /* perform forced recalibration failed */
       
        return 1;                                                                                /* return error */
    }
    if (out_buf[2] != a_scd4x_generate_crc(&out_buf[0], 2))                                      /* check crc */
    {
        handle->debug_print("scd4x: crc is error.\n");                                           /* crc is error */
       
        return 4;                                                                                /* return error */
    }
    *frc = (uint16_t)(((uint16_t)out_buf[0]) << 8) | out_buf[1];                                 /* set frc */
    
    return 0;                                                                                    /* success return 0 */
}

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
uint8_t scd4x_co2_convert_to_register(scd4x_handle_t *handle, float ppm, uint16_t *reg)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    *reg = (uint16_t)(ppm);         /* convert real data to register data */
    
    return 0;                       /* success return 0 */
}

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
uint8_t scd4x_co2_convert_to_data(scd4x_handle_t *handle, uint16_t reg, float *ppm)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    *ppm = (float)(reg);            /* convert raw data to real data */
    
    return 0;                       /* success return 0 */
}

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
uint8_t scd4x_set_automatic_self_calibration(scd4x_handle_t *handle, scd4x_bool_t enable)
{
    uint8_t res;
    uint8_t buf[3];
    uint16_t prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }

    prev = enable;                                                                           /* set bool */
    buf[0] = (prev >> 8) & 0xFF;                                                             /* msb */
    buf[1] = (prev >> 0) & 0xFF;                                                             /* lsb */
    buf[2] = a_scd4x_generate_crc(&buf[0], 2);                                               /* set crc */
    res = a_scd4x_iic_write(handle, SCD4X_COMMAND_SET_AUTO_SELF_CALIBRATION , buf, 3);       /* write config */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("scd4x: set automatic self calibration failed.\n");              /* set automatic self calibration failed */
       
        return 1;                                                                            /* return error */
    }
    handle->delay_ms(1);                                                                     /* delay 1ms */
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t scd4x_get_automatic_self_calibration(scd4x_handle_t *handle, scd4x_bool_t *enable)
{
    uint8_t res;
    uint8_t buf[3];
    uint16_t prev;
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }

    res = a_scd4x_iic_read(handle, SCD4X_COMMAND_GET_AUTO_SELF_CALIBRATION, buf, 3, 1);       /* read config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("scd4x: get automatic self calibration failed.\n");               /* get automatic self calibration failed */
       
        return 1;                                                                             /* return error */
    }
    if (buf[2] != a_scd4x_generate_crc(&buf[0], 2))                                           /* check crc */
    {
        handle->debug_print("scd4x: crc is error.\n");                                        /* crc is error */
       
        return 4;                                                                             /* return error */
    }
    prev = (uint16_t)(((uint16_t)buf[0]) << 8) | buf[1];                                      /* set prev */
    *enable = (scd4x_bool_t)((prev >> 0) & 0x01);                                             /* get bool */
    
    return 0;                                                                                 /* success return 0 */
}

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
uint8_t scd4x_start_low_power_periodic_measurement(scd4x_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }

    res = a_scd4x_iic_write(handle, SCD4X_COMMAND_START_LOW_POWER_PERIODIC, NULL, 0);        /* write config */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("scd4x: start low power periodic measurement failed.\n");        /* start low power periodic measurement failed */
       
        return 1;                                                                            /* return error */
    }
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t scd4x_get_data_ready_status(scd4x_handle_t *handle, scd4x_bool_t *enable)
{
    uint8_t res;
    uint8_t buf[3];
    uint16_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }

    res = a_scd4x_iic_read(handle, SCD4X_COMMAND_GET_DATA_READY_STATUS, buf, 3, 1);       /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("scd4x: get data ready status failed.\n");                    /* get data ready status failed */
       
        return 1;                                                                         /* return error */
    }
    if (buf[2] != a_scd4x_generate_crc(&buf[0], 2))                                       /* check crc */
    {
        handle->debug_print("scd4x: crc is error.\n");                                    /* crc is error */
       
        return 4;                                                                         /* return error */
    }
    prev = (uint16_t)(((uint16_t)buf[0]) << 8) | buf[1];                                  /* set prev */
    if ((prev & 0x0FFF) != 0)                                                             /* check data */
    {
        *enable = SCD4X_BOOL_TRUE;                                                        /* ready */
    }
    else
    {
        *enable = SCD4X_BOOL_FALSE;                                                       /* not ready */
    }
    
    return 0;                                                                             /* success return 0 */
}

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
uint8_t scd4x_persist_settings(scd4x_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }

    res = a_scd4x_iic_write(handle, SCD4X_COMMAND_PERSIST_SETTINGS, NULL, 0);        /* write config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("scd4x: persist settings failed.\n");                    /* persist settings failed */
       
        return 1;                                                                    /* return error */
    }
    handle->delay_ms(800);                                                           /* delay 800ms */
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t scd4x_get_serial_number(scd4x_handle_t *handle, uint16_t number[3])
{
    uint8_t res;
    uint8_t buf[9];
    
    if (handle == NULL)                                                               /* check handle */
    {
        return 2;                                                                     /* return error */
    }
    if (handle->inited != 1)                                                          /* check handle initialization */
    {
        return 3;                                                                     /* return error */
    }

    res = a_scd4x_iic_read(handle, SCD4X_COMMAND_GET_SERIAL_NUMBER, buf, 9, 1);       /* read config */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("scd4x: get serial number failed.\n");                    /* get serial number failed */
       
        return 1;                                                                     /* return error */
    }
    if (buf[2] != a_scd4x_generate_crc(&buf[0], 2))                                   /* check crc */
    {
        handle->debug_print("scd4x: crc is error.\n");                                /* crc is error */
       
        return 4;                                                                     /* return error */
    }
    if (buf[5] != a_scd4x_generate_crc(&buf[3], 2))                                   /* check crc */
    {
        handle->debug_print("scd4x: crc is error.\n");                                /* crc is error */
       
        return 4;                                                                     /* return error */
    }
    if (buf[8] != a_scd4x_generate_crc(&buf[6], 2))                                   /* check crc */
    {
        handle->debug_print("scd4x: crc is error.\n");                                /* crc is error */
       
        return 4;                                                                     /* return error */
    }
    number[0] = (uint16_t)(((uint16_t)buf[0]) << 8) | buf[1];                         /* set number0 */
    number[1] = (uint16_t)(((uint16_t)buf[3]) << 8) | buf[4];                         /* set number1 */
    number[2] = (uint16_t)(((uint16_t)buf[6]) << 8) | buf[7];                         /* set number2 */
    
    return 0;                                                                         /* success return 0 */
}

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
uint8_t scd4x_perform_self_test(scd4x_handle_t *handle, scd4x_bool_t *malfunction_detected)
{
    uint8_t res;
    uint8_t buf[3];
    uint16_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }

    res = a_scd4x_iic_read(handle, SCD4X_COMMAND_PERFORM_SELF_TEST, buf, 3, 10000);       /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("scd4x: perform self test failed.\n");                        /* perform self test failed */
       
        return 1;                                                                         /* return error */
    }
    if (buf[2] != a_scd4x_generate_crc(&buf[0], 2))                                       /* check crc */
    {
        handle->debug_print("scd4x: crc is error.\n");                                    /* crc is error */
       
        return 4;                                                                         /* return error */
    }
    prev = (uint16_t)(((uint16_t)buf[0]) << 8) | buf[1];                                  /* set prev */
    if (prev != 0)                                                                        /* check data */
    {
        *malfunction_detected = SCD4X_BOOL_TRUE;                                          /* true */
    }
    else
    {
        *malfunction_detected = SCD4X_BOOL_FALSE;                                         /* false */
    }
    
    return 0;                                                                             /* success return 0 */
}

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
uint8_t scd4x_perform_factory_reset(scd4x_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }

    res = a_scd4x_iic_write(handle, SCD4X_COMMAND_PERFORM_FACTORY_RESET, NULL, 0);        /* write config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("scd4x: perform factory reset failed.\n");                    /* perform factory reset failed */
       
        return 1;                                                                         /* return error */
    }
    handle->delay_ms(1200);                                                               /* delay 1200ms */
    
    return 0;                                                                             /* success return 0 */
}

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
uint8_t scd4x_reinit(scd4x_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }

    res = a_scd4x_iic_write(handle, SCD4X_COMMAND_REINIT, NULL, 0);        /* write config */
    if (res != 0)                                                          /* check result */
    {
        handle->debug_print("scd4x: reinit failed.\n");                    /* reinit failed */
       
        return 1;                                                          /* return error */
    }
    handle->delay_ms(30);                                                  /* delay 30ms */
    
    return 0;                                                              /* success return 0 */
}

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
uint8_t scd4x_measure_single_shot(scd4x_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    if (handle->type != SCD41)                                                          /* check type */
    {
        handle->debug_print("scd4x: only scd41 has this function.\n");                  /* only scd41 has this function */
       
        return 4;                                                                       /* return error */
    }

    res = a_scd4x_iic_write(handle, SCD4X_COMMAND_MEASURE_SINGLE_SHOT, NULL, 0);        /* write config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("scd4x: measure single shot failed.\n");                    /* measure single shot failed */
       
        return 1;                                                                       /* return error */
    }
    handle->delay_ms(5000);                                                             /* delay 5000ms */
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t scd4x_measure_single_shot_rht_only(scd4x_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    if (handle->type != SCD41)                                                                   /* check type */
    {
        handle->debug_print("scd4x: only scd41 has this function.\n");                           /* only scd41 has this function */
       
        return 4;                                                                                /* return error */
    }

    res = a_scd4x_iic_write(handle, SCD4X_COMMAND_MEASURE_SINGLE_SHOT_RHT_ONLY, NULL, 0);        /* write config */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("scd4x: measure single shot rht only failed.\n");                    /* measure single shot rht only failed */
       
        return 1;                                                                                /* return error */
    }
    handle->delay_ms(50);                                                                        /* delay 50ms */
    
    return 0;                                                                                    /* success return 0 */
}

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
uint8_t scd4x_power_down(scd4x_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    if (handle->type != SCD41)                                                 /* check type */
    {
        handle->debug_print("scd4x: only scd41 has this function.\n");         /* only scd41 has this function */
       
        return 4;                                                              /* return error */
    }

    res = a_scd4x_iic_write(handle, SCD4X_COMMAND_POWER_DOWN, NULL, 0);        /* write config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("scd4x: power down failed.\n");                    /* power down failed */
       
        return 1;                                                              /* return error */
    }
    handle->delay_ms(1);                                                       /* delay 1ms */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t scd4x_wake_up(scd4x_handle_t *handle)
{
    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                  /* check handle initialization */
    {
        return 3;                                                             /* return error */
    }
    if (handle->type != SCD41)                                                /* check type */
    {
        handle->debug_print("scd4x: only scd41 has this function.\n");        /* only scd41 has this function */
       
        return 4;                                                             /* return error */
    }

    (void)a_scd4x_iic_write(handle, SCD4X_COMMAND_WAKE_UP, NULL, 0);          /* write config */
    handle->delay_ms(30);                                                     /* delay 30ms */
    
    return 0;                                                                 /* success return 0 */
}

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
uint8_t scd4x_set_automatic_self_calibration_initial_period(scd4x_handle_t *handle, uint16_t hour)
{
    uint8_t res;
    uint8_t buf[3];
    
    if (handle == NULL)                                                                                  /* check handle */
    {
        return 2;                                                                                        /* return error */
    }
    if (handle->inited != 1)                                                                             /* check handle initialization */
    {
        return 3;                                                                                        /* return error */
    }
    if (handle->type != SCD41)                                                                           /* check type */
    {
        handle->debug_print("scd4x: only scd41 has this function.\n");                                   /* only scd41 has this function */
       
        return 4;                                                                                        /* return error */
    }
    if ((hour % 4) != 0)                                                                                 /* check hour */
    {
        handle->debug_print("scd4x: hour is not integer multiples of 4.\n");                             /* hour is not integer multiples of 4 */
       
        return 5;                                                                                        /* return error */
    }
    
    buf[0] = (hour >> 8) & 0xFF;                                                                         /* msb */
    buf[1] = (hour >> 0) & 0xFF;                                                                         /* lsb */
    buf[2] = a_scd4x_generate_crc(&buf[0], 2);                                                           /* crc */
    res = a_scd4x_iic_write(handle, SCD4X_COMMAND_SET_AUTO_SELF_CALIBRATION_INIT_PERIOD, buf, 3);        /* write config */
    if (res != 0)                                                                                        /* check result */
    {
        handle->debug_print("scd4x: set automatic self calibration initial period failed.\n");           /* set automatic self calibration initial period failed */
       
        return 1;                                                                                        /* return error */
    }
    handle->delay_ms(1);                                                                                 /* delay 1ms */
    
    return 0;                                                                                            /* success return 0 */
}

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
uint8_t scd4x_get_automatic_self_calibration_initial_period(scd4x_handle_t *handle, uint16_t *hour)
{
    uint8_t res;
    uint8_t buf[3];
    
    if (handle == NULL)                                                                                    /* check handle */
    {
        return 2;                                                                                          /* return error */
    }
    if (handle->inited != 1)                                                                               /* check handle initialization */
    {
        return 3;                                                                                          /* return error */
    }
    if (handle->type != SCD41)                                                                             /* check type */
    {
        handle->debug_print("scd4x: only scd41 has this function.\n");                                     /* only scd41 has this function */
       
        return 4;                                                                                          /* return error */
    }
    
    res = a_scd4x_iic_read(handle, SCD4X_COMMAND_GET_AUTO_SELF_CALIBRATION_INIT_PERIOD, buf, 3, 1);        /* read config */
    if (res != 0)                                                                                          /* check result */
    {
        handle->debug_print("scd4x: get automatic self calibration initial period failed.\n");             /* get automatic self calibration initial period failed */
       
        return 1;                                                                                          /* return error */
    }
    if (buf[2] != a_scd4x_generate_crc(&buf[0], 2))                                                        /* check crc */
    {
        handle->debug_print("scd4x: crc is error.\n");                                                     /* crc is error */
       
        return 5;                                                                                          /* return error */
    }
    *hour = (uint16_t)(((uint16_t)buf[0]) << 8) | buf[1];                                                  /* get hour */
    
    return 0;                                                                                              /* success return 0 */
}

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
uint8_t scd4x_set_automatic_self_calibration_standard_period(scd4x_handle_t *handle, uint16_t hour)
{
    uint8_t res;
    uint8_t buf[3];
    
    if (handle == NULL)                                                                                  /* check handle */
    {
        return 2;                                                                                        /* return error */
    }
    if (handle->inited != 1)                                                                             /* check handle initialization */
    {
        return 3;                                                                                        /* return error */
    }
    if (handle->type != SCD41)                                                                           /* check type */
    {
        handle->debug_print("scd4x: only scd41 has this function.\n");                                   /* only scd41 has this function */
       
        return 4;                                                                                        /* return error */
    }
    if ((hour % 4) != 0)                                                                                 /* check hour */
    {
        handle->debug_print("scd4x: hour is not integer multiples of 4.\n");                             /* hour is not integer multiples of 4 */
       
        return 5;                                                                                        /* return error */
    }
    
    buf[0] = (hour >> 8) & 0xFF;                                                                         /* msb */
    buf[1] = (hour >> 0) & 0xFF;                                                                         /* lsb */
    buf[2] = a_scd4x_generate_crc(&buf[0], 2);                                                           /* crc */
    res = a_scd4x_iic_write(handle, SCD4X_COMMAND_SET_AUTO_SELF_CALIBRATION_STANDARD_PERIOD, buf, 3);    /* write config */
    if (res != 0)                                                                                        /* check result */
    {
        handle->debug_print("scd4x: set automatic self calibration standard period failed.\n");          /* set automatic self calibration standard period failed */
       
        return 1;                                                                                        /* return error */
    }
    handle->delay_ms(1);                                                                                 /* delay 1ms */
    
    return 0;                                                                                            /* success return 0 */
}

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
uint8_t scd4x_get_automatic_self_calibration_standard_period(scd4x_handle_t *handle, uint16_t *hour)
{
    uint8_t res;
    uint8_t buf[3];
    
    if (handle == NULL)                                                                                    /* check handle */
    {
        return 2;                                                                                          /* return error */
    }
    if (handle->inited != 1)                                                                               /* check handle initialization */
    {
        return 3;                                                                                          /* return error */
    }
    if (handle->type != SCD41)                                                                             /* check type */
    {
        handle->debug_print("scd4x: only scd41 has this function.\n");                                     /* only scd41 has this function */
       
        return 4;                                                                                          /* return error */
    }
    
    res = a_scd4x_iic_read(handle, SCD4X_COMMAND_GET_AUTO_SELF_CALIBRATION_STANDARD_PERIOD, buf, 3, 1);    /* read config */
    if (res != 0)                                                                                          /* check result */
    {
        handle->debug_print("scd4x: get automatic self calibration standard period failed.\n");            /* get automatic self calibration standard period failed */
       
        return 1;                                                                                          /* return error */
    }
    if (buf[2] != a_scd4x_generate_crc(&buf[0], 2))                                                        /* check crc */
    {
        handle->debug_print("scd4x: crc is error.\n");                                                     /* crc is error */
       
        return 5;                                                                                          /* return error */
    }
    *hour = (uint16_t)(((uint16_t)buf[0]) << 8) | buf[1];                                                  /* get hour */
    
    return 0;                                                                                              /* success return 0 */
}

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
uint8_t scd4x_init(scd4x_handle_t *handle)
{ 
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->debug_print == NULL)                                         /* check debug_print */
    {
        return 3;                                                            /* return error */
    }
    if (handle->iic_init == NULL)                                            /* check iic_init */
    {
        handle->debug_print("scd4x: iic_init is null.\n");                   /* iic_init is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->iic_deinit == NULL)                                          /* check iic_deinit */
    {
        handle->debug_print("scd4x: iic_deinit is null.\n");                 /* iic_deinit is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->iic_write_cmd == NULL)                                       /* check iic_write_cmd */
    {
        handle->debug_print("scd4x: iic_write_cmd is null.\n");              /* iic_write_cmd is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->iic_read_cmd == NULL)                                        /* check iic_read_cmd */
    {
        handle->debug_print("scd4x: iic_read_cmd is null.\n");               /* iic_read_cmd is null */
    
        return 3;                                                            /* return error */
    }
    if (handle->delay_ms == NULL)                                            /* check delay_ms */
    {
        handle->debug_print("scd4x: delay_ms is null.\n");                   /* delay_ms is null */
    
        return 3;                                                            /* return error */
    }
    
    if (handle->iic_init() != 0)                                             /* iic init */
    {
        handle->debug_print("scd4x: iic init failed.\n");                    /* iic init failed */
    
        return 3;                                                            /* return error */
    }
    handle->inited = 1;                                                      /* flag finish initialization */
  
    return 0;                                                                /* success return 0 */
}

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
uint8_t scd4x_deinit(scd4x_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }    
    
    res = a_scd4x_iic_write(handle, SCD4X_COMMAND_STOP_PERIODIC, NULL, 0);        /* write config */
    if (res != 0)                                                                 /* check result */
    {
        handle->debug_print("scd4x: stop periodic measurement failed.\n");        /* stop periodic measurement failed */
       
        return 4;                                                                 /* return error */
    }
    if (handle->iic_deinit() != 0)                                                /* iic deinit */
    {
        handle->debug_print("scd4x: iic close failed.\n");                        /* iic close failed */
    
        return 3;                                                                 /* return error */
    }
    handle->inited = 0;                                                           /* flag close initialization */
  
    return 0;                                                                     /* success return 0 */
}

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
uint8_t scd4x_set_reg(scd4x_handle_t *handle, uint16_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                       /* check handle */
    {
        return 2;                                             /* return error */
    }
    if (handle->inited != 1)                                  /* check handle initialization */
    {
        return 3;                                             /* return error */
    }
  
    return a_scd4x_iic_write(handle, reg, buf, len);          /* write data */
}

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
uint8_t scd4x_get_reg(scd4x_handle_t *handle, uint16_t reg, uint8_t *buf, uint16_t len, uint16_t delay_ms)
{
    if (handle == NULL)                                            /* check handle */
    {
        return 2;                                                  /* return error */
    }
    if (handle->inited != 1)                                       /* check handle initialization */
    {
        return 3;                                                  /* return error */
    }
  
    return a_scd4x_iic_read(handle, reg, buf, len, delay_ms);      /* read data */
}

/**
 * @brief      get chip information
 * @param[out] *info points to an scd4x info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t scd4x_info(scd4x_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(scd4x_info_t));                          /* initialize scd4x info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "IIC", 8);                             /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}

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
 * @file      main.c
 * @brief     main source file
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
#include "driver_scd4x_register_test.h"
#include "driver_scd4x_basic.h"
#include "driver_scd4x_shot.h"
#include <getopt.h>
#include <stdlib.h>

/**
 * @brief     scd4x full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t scd4x(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"times", required_argument, NULL, 1},
        {"type", required_argument, NULL, 2},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    uint32_t times = 3;
    scd4x_t chip_type = SCD41;
    
    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }
    
    /* init 0 */
    optind = 0;
    
    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);
        
        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");
                
                break;
            }
            
            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");
                
                break;
            }
            
            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");
                
                break;
            }
            
            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);
                
                break;
            }
            
            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);
                
                break;
            }
            
            /* running times */
            case 1 :
            {
                /* set the times */
                times = atol(optarg);
                
                break;
            } 
            
            /* chip type */
            case 2 :
            {
                if (strcmp(optarg, "SCD40") == 0)
                {
                    chip_type = SCD40;
                }
                else if (strcmp(optarg, "SCD41") == 0)
                {
                    chip_type = SCD41;
                }
                else
                {
                    return 5;
                }
                
                break;
            } 
            
            /* the end */
            case -1 :
            {
                break;
            }
            
            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run functions */
    if (strcmp("t_reg", type) == 0)
    {
        /* run reg test */
        if (scd4x_register_test(chip_type) != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("t_read", type) == 0)
    {
        /* run read test */
        if (scd4x_read_test(chip_type, times) != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("e_read", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        uint16_t co2_ppm;
        float temperature;
        float humidity;
        
        /* basic init */
        res = scd4x_basic_init(chip_type);
        if (res != 0)
        {
            return 1;
        }
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            /* delay 5000ms */
            scd4x_interface_delay_ms(5000);
            
            /* read data */
            res = scd4x_basic_read((uint16_t *)&co2_ppm, (float *)&temperature, (float *)&humidity);
            if (res != 0)
            {
                (void)scd4x_basic_deinit();
                
                return 1;
            }
            
            /* output */
            scd4x_interface_debug_print("scd4x: %d/%d.\n", (uint32_t)(i + 1), (uint32_t)times);
            scd4x_interface_debug_print("scd4x: co2 is %02dppm.\n", co2_ppm);
            scd4x_interface_debug_print("scd4x: temperature is %0.2fC.\n", temperature);
            scd4x_interface_debug_print("scd4x: humidity is %0.2f%%.\n", humidity);
        }
        
        /* basic deinit */
        (void)scd4x_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_shot", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        uint16_t co2_ppm;
        float temperature;
        float humidity;
        
        /* shot init */
        res = scd4x_shot_init(chip_type);
        if (res != 0)
        {
            return 1;
        }
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            /* delay 5000ms */
            scd4x_interface_delay_ms(5000);
            
            /* read data */
            res = scd4x_shot_read((uint16_t *)&co2_ppm, (float *)&temperature, (float *)&humidity);
            if (res != 0)
            {
                (void)scd4x_shot_deinit();
                
                return 1;
            }
            
            /* output */
            scd4x_interface_debug_print("scd4x: %d/%d.\n", (uint32_t)(i + 1), (uint32_t)times);
            scd4x_interface_debug_print("scd4x: co2 is %02dppm.\n", co2_ppm);
            scd4x_interface_debug_print("scd4x: temperature is %0.2fC.\n", temperature);
            scd4x_interface_debug_print("scd4x: humidity is %0.2f%%.\n", humidity);
        }
        
        /* shot deinit */
        (void)scd4x_shot_deinit();
        
        return 0;
    }
    else if (strcmp("e_number", type) == 0)
    {
        uint8_t res;
        uint16_t num[3];
        
        /* shot init */
        res = scd4x_shot_init(chip_type);
        if (res != 0)
        {
            return 1;
        }
        
        /* get serial number */
        res = scd4x_shot_get_serial_number(num);
        if (res != 0)
        {
            (void)scd4x_shot_deinit();
            
            return 1;
        }
        
        /* output */
        scd4x_interface_debug_print("scd4x: serial number is 0x%04X%04X%04X.\n", num[0], num[1], num[2]);
        
        /* shot deinit */
        (void)scd4x_shot_deinit();
        
        return 0;
    }
    else if (strcmp("e_wake-up", type) == 0)
    {
        uint8_t res;
        
        /* shot init */
        res = scd4x_shot_init(chip_type);
        if (res != 0)
        {
            return 1;
        }
        
        /* wake up */
        res = scd4x_shot_wake_up();
        if (res != 0)
        {
            (void)scd4x_shot_deinit();
            
            return 1;
        }
        
        /* output */
        scd4x_interface_debug_print("scd4x: wake up.\n");
        
        /* shot deinit */
        (void)scd4x_shot_deinit();
        
        return 0;
    }
    else if (strcmp("e_power-down", type) == 0)
    {
        uint8_t res;
        
        /* shot init */
        res = scd4x_shot_init(chip_type);
        if (res != 0)
        {
            return 1;
        }
        
        /* power down */
        res = scd4x_shot_power_down();
        if (res != 0)
        {
            (void)scd4x_shot_deinit();
            
            return 1;
        }
        
        /* wake up */
        res = scd4x_shot_wake_up();
        if (res != 0)
        {
            (void)scd4x_shot_deinit();
            
            return 1;
        }
        
        /* output */
        scd4x_interface_debug_print("scd4x: power down.\n");
        
        /* shot deinit */
        (void)scd4x_shot_deinit();
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        scd4x_interface_debug_print("Usage:\n");
        scd4x_interface_debug_print("  scd4x (-i | --information)\n");
        scd4x_interface_debug_print("  scd4x (-h | --help)\n");
        scd4x_interface_debug_print("  scd4x (-p | --port)\n");
        scd4x_interface_debug_print("  scd4x (-t reg | --test=reg) [--type=<SCD40 | SCD41>]\n");
        scd4x_interface_debug_print("  scd4x (-t read | --test=read) [--type=<SCD40 | SCD41>] [--times=<num>]\n");
        scd4x_interface_debug_print("  scd4x (-e read | --example=read) [--type=<SCD40 | SCD41>] [--times=<num>]\n");
        scd4x_interface_debug_print("  scd4x (-e shot | --example=shot) [--type=<SCD40 | SCD41>] [--times=<num>]\n");
        scd4x_interface_debug_print("  scd4x (-e wake-up | --example=wake-up) [--type=<SCD40 | SCD41>]\n");
        scd4x_interface_debug_print("  scd4x (-e power-down | --example=power-down) [--type=<SCD40 | SCD41>]\n");
        scd4x_interface_debug_print("  scd4x (-e number | --example=number) [--type=<SCD40 | SCD41>]\n");
        scd4x_interface_debug_print("\n");
        scd4x_interface_debug_print("Options:\n");
        scd4x_interface_debug_print("  -e <read | shot | wake-up | power-down | number>, --example=<read | shot | wake-up | power-down | number>\n");
        scd4x_interface_debug_print("                        Run the driver example.\n");
        scd4x_interface_debug_print("  -h, --help            Show the help.\n");
        scd4x_interface_debug_print("  -i, --information     Show the chip information.\n");
        scd4x_interface_debug_print("  -p, --port            Display the pin connections of the current board.\n");
        scd4x_interface_debug_print("  -t <reg | read>, --test=<reg | read>\n");
        scd4x_interface_debug_print("                        Run the driver test.\n");
        scd4x_interface_debug_print("      --times=<num>     Set the running times.([default: 3])\n");
        scd4x_interface_debug_print("      --type=<SCD40 | SCD41>\n");
        scd4x_interface_debug_print("                        Set the chip type.([default: SCD41])\n");
        
        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        scd4x_info_t info;
        
        /* print scd4x info */
        scd4x_info(&info);
        scd4x_interface_debug_print("scd4x: chip is %s.\n", info.chip_name);
        scd4x_interface_debug_print("scd4x: manufacturer is %s.\n", info.manufacturer_name);
        scd4x_interface_debug_print("scd4x: interface is %s.\n", info.interface);
        scd4x_interface_debug_print("scd4x: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        scd4x_interface_debug_print("scd4x: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        scd4x_interface_debug_print("scd4x: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        scd4x_interface_debug_print("scd4x: max current is %0.2fmA.\n", info.max_current_ma);
        scd4x_interface_debug_print("scd4x: max temperature is %0.1fC.\n", info.temperature_max);
        scd4x_interface_debug_print("scd4x: min temperature is %0.1fC.\n", info.temperature_min);
        
        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        scd4x_interface_debug_print("scd4x: SCL connected to GPIO3(BCM).\n");
        scd4x_interface_debug_print("scd4x: SDA connected to GPIO2(BCM).\n");
        
        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief     main function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 * @note      none
 */
int main(uint8_t argc, char **argv)
{
    uint8_t res;

    res = scd4x(argc, argv);
    if (res == 0)
    {
        /* run success */
    }
    else if (res == 1)
    {
        scd4x_interface_debug_print("scd4x: run failed.\n");
    }
    else if (res == 5)
    {
        scd4x_interface_debug_print("scd4x: param is invalid.\n");
    }
    else
    {
        scd4x_interface_debug_print("scd4x: unknown status code.\n");
    }

    return 0;
}

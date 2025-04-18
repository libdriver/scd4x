[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver SCD4X

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/scd4x/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

The SCD4x is Sensirion’s next generation miniature CO2 sensor. This sensor builds on the photoacoustic NDIR sensing principle and Sensirion’s patented PASens® and CMOSens® technology to offer high accuracy at an unmatched price and smallest form factor. SMD assembly allows cost- and space-effective integration of the sensor combined with maximal freedom of design. On-chip signal compensation is realized with the built-in SHT4x humidity and temperature sensor.CO2 is a key indicator for indoor air quality (IAQ) as high levels compromise humans’ cognitive performance and well-being. The SCD4x enables smart ventilation systems to regulate ventilation in the most energy-efficient and human-friendly way. Moreover, indoor air quality monitors and other connected devices based on the SCD4x can help maintain low CO2 concentration for a healthy, productive environment.

LibDriver SCD4X is a full-featured driver of SCD4X launched by LibDriver.It provides CO2 reading, temperature reading, humidity reading and other functions. LibDriver is MISRA compliant.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver SCD4X source files.

/interface includes LibDriver SCD4X IIC platform independent template.

/test includes LibDriver SCD4X driver test code and this code can test the chip necessary function simply.

/example includes LibDriver SCD4X sample code.

/doc includes LibDriver SCD4X offline document.

/datasheet includes SCD4X datasheet.

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

/misra includes the LibDriver MISRA code scanning results.

### Install

Reference /interface IIC platform independent template and finish your platform IIC driver.

Add the /src directory, the interface driver for your platform, and your own drivers to your project, if you want to use the default example drivers, add the /example directory to your project.

### Usage

You can refer to the examples in the /example directory to complete your own driver. If you want to use the default programming examples, here's how to use them.

#### example basic

```C
#include "driver_scd4x_basic.h"

uint8_t res;
uint32_t i;
uint32_t times = 3;
uint16_t co2_ppm;
float temperature;
float humidity;

/* basic init */
res = scd4x_basic_init(SCD41);
if (res != 0)
{
    return 1;
}

...
    
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
    
    ...
}

...
    
/* basic deinit */
(void)scd4x_basic_deinit();

return 0;
```
#### example shot

```C
#include "driver_scd4x_shot.h"

uint8_t res;
uint32_t i;
uint32_t times = 3;
uint16_t co2_ppm;
float temperature;
float humidity;

/* shot init */
res = scd4x_shot_init(SCD41);
if (res != 0)
{
    return 1;
}

...
    
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
    
    ...
}

...
    
/* shot deinit */
(void)scd4x_shot_deinit();

return 0;
```
### Document

Online documents: [https://www.libdriver.com/docs/scd4x/index.html](https://www.libdriver.com/docs/scd4x/index.html).

Offline documents: /doc/html/index.html.

### Contributing

Please refer to CONTRIBUTING.md.

### License

Copyright (c) 2015 - present LibDriver All rights reserved



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please send an e-mail to lishifenging@outlook.com.
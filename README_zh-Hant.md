[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver SCD4X

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/scd4x/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

SCD4x是Sensionon的下一代微型二氧化碳感測器。 該感測器基於光聲NDIR傳感原理和Sensionon的專利PASens ® 和CMOSens ® 科技，提供無與倫比的價格和最小封裝的高精度解決方案。 SMD組件允許感測器的成本和空間有效集成，並具有最大的設計自由度。 晶片上的訊號補償是通過內寘SHT4x濕度和溫度感測器實現的。 二氧化碳是室內空氣品質（IAQ）的一個關鍵名額，因為高水准的二氧化碳會損害人類的認知能力和幸福感。 SCD4x使智慧通風系統能够以最節能、最人性化的管道調節通風。 此外，基於SCD4x的室內空氣品質監測器和其他連接設備有助於保持低二氧化碳濃度，營造健康、高效的環境。

LibDriver SCD4X是LibDriver推出的SCD4X全功能驅動，該驅動提供CO2讀取、溫度讀取、濕度讀取等功能並且它符合MISRA標準。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver SCD4X的源文件。

/interface目錄包含了LibDriver SCD4X與平台無關的IIC總線模板。

/test目錄包含了LibDriver SCD4X驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver SCD4X編程範例。

/doc目錄包含了LibDriver SCD4X離線文檔。

/datasheet目錄包含了SCD4X數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

### 安裝

參考/interface目錄下與平台無關的IIC總線模板，完成指定平台的IIC總線驅動。

將/src目錄，您使用平臺的介面驅動和您開發的驅動加入工程，如果您想要使用默認的範例驅動，可以將/example目錄加入您的工程。

### 使用

您可以參考/example目錄下的程式設計範例完成適合您的驅動，如果您想要使用默認的程式設計範例，以下是它們的使用方法。

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

### 文檔

在線文檔: [https://www.libdriver.com/docs/scd4x/index.html](https://www.libdriver.com/docs/scd4x/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。
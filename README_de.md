[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver SCD4X

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/scd4x/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Der SCD4x ist der Miniatur-CO2-Sensor der nächsten Generation von Sensirion. Dieser Sensor basiert auf dem photoakustischen NDIR-Sensorprinzip und der patentierten PASens®- und CMOSens®-Technologie von Sensirion und bietet hohe Genauigkeit zu einem unübertroffenen Preis und kleinsten Formfaktor. Die SMD-Montage ermöglicht eine kostengünstige und platzsparende Integration des Sensors bei maximaler Designfreiheit. Die Signalkompensation auf dem Chip wird mit dem integrierten Feuchtigkeits- und Temperatursensor SHT4x realisiert. CO2 ist ein Schlüsselindikator für die Luftqualität in Innenräumen (IAQ), da hohe Werte die kognitive Leistungsfähigkeit und das Wohlbefinden des Menschen beeinträchtigen. Mit dem SCD4x können intelligente Lüftungssysteme die Belüftung auf die energieeffizienteste und menschenfreundlichste Weise regeln. Darüber hinaus können Raumluftqualitätsmonitore und andere angeschlossene Geräte auf Basis des SCD4x dazu beitragen, die CO2-Konzentration niedrig zu halten und so eine gesunde, produktive Umgebung zu schaffen.

LibDriver SCD4X ist ein voll funktionsfähiger Treiber von SCD4X, der von LibDriver gestartet wurde. Er bietet CO2-Messung, Temperaturmessung, Luftfeuchtigkeitsmessung und andere Funktionen. LibDriver ist MISRA-kompatibel.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver SCD4X-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver SCD4X IIC.

/test enthält den Testcode des LibDriver SCD4X-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver SCD4X-Beispielcode.

/doc enthält das LibDriver SCD4X-Offlinedokument.

/Datenblatt enthält SCD4X-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige IIC-Schnittstellenvorlage und stellen Sie Ihren Plattform-IIC-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

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
### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/scd4x/index.html](https://www.libdriver.com/docs/scd4x/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.
### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

IIC Pin: SCL/SDA PB8/PB9.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. SCD4X

#### 3.1 Command Instruction

1. Show scd4x chip and driver information.

   ```shell
   scd4x (-i | --information)
   ```

2. Show scd4x help.

   ```shell
   scd4x (-h | --help)
   ```

3. Show scd4x pin connections of the current board.

   ```shell
   scd4x (-p | --port)
   ```

4. Run scd4x register test.

   ```shell
   scd4x (-t reg | --test=reg) [--type=<SCD40 | SCD41>]
   ```

5. Run scd4x read test, num is test times.

   ```shell
   scd4x (-t read | --test=read) [--type=<SCD40 | SCD41>] [--times=<num>]
   ```

6. Run scd4x read function, num is read times.

   ```shell
   scd4x (-e read | --example=read) [--type=<SCD40 | SCD41>] [--times=<num>]
   ```

7. Run scd4x shot function, num is read times.

   ```shell
   scd4x (-e shot | --example=shot) [--type=<SCD40 | SCD41>] [--times=<num>]
   ```

8. Run scd4x wake up function.

   ```shell
   scd4x (-e wake-up | --example=wake-up) [--type=<SCD40 | SCD41>]
   ```

9. Run scd4x power down function.

   ```shell
   scd4x (-e power-down | --example=power-down) [--type=<SCD40 | SCD41>]
   ```

10. Run scd4x number function.

   ```shell
   scd4x (-e number | --example=number) [--type=<SCD40 | SCD41>]
   ```

#### 3.2 Command Example

```shell
scd4x -i

scd4x: chip is Sensirion SCD4X.
scd4x: manufacturer is Sensirion.
scd4x: interface is IIC.
scd4x: driver version is 1.0.
scd4x: min supply voltage is 2.4V.
scd4x: max supply voltage is 5.5V.
scd4x: max current is 205.00mA.
scd4x: max temperature is 60.0C.
scd4x: min temperature is -10.0C.
```

```shell
scd4x -p

scd4x: SCL connected to GPIOB PIN8.
scd4x: SDA connected to GPIOB PIN9.
```

```shell
scd4x -t reg --type=SCD41

scd4x: chip is Sensirion SCD4X.
scd4x: manufacturer is Sensirion.
scd4x: interface is IIC.
scd4x: driver version is 1.0.
scd4x: min supply voltage is 2.4V.
scd4x: max supply voltage is 5.5V.
scd4x: max current is 205.00mA.
scd4x: max temperature is 60.0C.
scd4x: min temperature is -10.0C.
scd4x: start register test.
scd4x: scd4x_set_type/scd4x_get_type test.
scd4x: set type scd40.
scd4x: check type ok.
scd4x: set type scd41.
scd4x: check type ok.
scd4x: scd4x_set_temperature_offset/scd4x_get_temperature_offset test.
scd4x: set temperature offset 63223.
scd4x: check temperature offset ok.
scd4x: scd4x_temperature_offset_convert_to_register/scd4x_temperature_offset_convert_to_data test.
scd4x: set temperature 32.00.
scd4x: check temperature 32.00.
scd4x: scd4x_set_sensor_altitude/scd4x_get_sensor_altitude test.
scd4x: set sensor altitude 56029.
scd4x: check sensor altitude ok.
scd4x: scd4x_sensor_altitude_convert_to_register/scd4x_sensor_altitude_convert_to_data test.
scd4x: set altitude 100.00.
scd4x: check altitude 100.00.
scd4x: scd4x_set_ambient_pressure/scd4x_get_ambient_pressure test.
scd4x: set ambient pressure 1518.
scd4x: check ambient pressure ok.
scd4x: scd4x_ambient_pressure_convert_to_register/scd4x_ambient_pressure_convert_to_data test.
scd4x: set ambient pressure 100741.00.
scd4x: check ambient pressure 100700.00.
scd4x: scd4x_co2_convert_to_register/scd4x_co2_convert_to_data test.
scd4x: set co2 60.00.
scd4x: check co2 60.00.
scd4x: scd4x_set_automatic_self_calibration/scd4x_get_automatic_self_calibration test.
scd4x: disable automatic self calibration.
scd4x: check automatic self calibration ok.
scd4x: enable automatic self calibration.
scd4x: check automatic self calibration ok.
scd4x: scd4x_get_data_ready_status test.
scd4x: check data ready status not ready.
scd4x: scd4x_get_serial_number test.
scd4x: serial number is 0xF2F1CF073B26.
scd4x: scd4x_perform_self_test test.
scd4x: check perform self test ok.
scd4x: scd4x_perform_factory_reset test.
scd4x: check perform factory reset ok.
scd4x: scd4x_reinit test.
scd4x: check reinit ok.
scd4x: scd4x_persist_settings test.
scd4x: check persist settings ok.
scd4x: scd4x_perform_forced_recalibration test.
scd4x: check perform forced recalibration 0xFFFF.
scd4x: scd4x_power_down/scd4x_wake_up test.
scd4x: power down.
scd4x: wake up.
scd4x: scd4x_set_automatic_self_calibration_initial_period/scd4x_get_automatic_self_calibration_initial_period test.
scd4x: set automatic self calibration initial period 528.
scd4x: check automatic self calibration initial period ok.
scd4x: scd4x_set_automatic_self_calibration_standard_period/scd4x_get_automatic_self_calibration_standard_period test.
scd4x: set automatic self calibration standard period 736.
scd4x: check automatic self calibration standard period ok.
scd4x: finish register test.
```

```shell
scd4x -t read --type=SCD41 --times=3

scd4x: chip is Sensirion SCD4X.
scd4x: manufacturer is Sensirion.
scd4x: interface is IIC.
scd4x: driver version is 1.0.
scd4x: min supply voltage is 2.4V.
scd4x: max supply voltage is 5.5V.
scd4x: max current is 205.00mA.
scd4x: max temperature is 60.0C.
scd4x: min temperature is -10.0C.
scd4x: start read test.
scd4x: continuous read test.
scd4x: co2 is 929ppm.
scd4x: temperature is 29.91C.
scd4x: humidity is 42.80%.
scd4x: co2 is 940ppm.
scd4x: temperature is 29.89C.
scd4x: humidity is 42.80%.
scd4x: co2 is 1058ppm.
scd4x: temperature is 29.79C.
scd4x: humidity is 42.74%.
scd4x: low power continuous read test.
scd4x: co2 is 1147ppm.
scd4x: temperature is 30.52C.
scd4x: humidity is 43.15%.
scd4x: co2 is 1198ppm.
scd4x: temperature is 28.63C.
scd4x: humidity is 42.08%.
scd4x: co2 is 1071ppm.
scd4x: temperature is 27.51C.
scd4x: humidity is 41.43%.
scd4x: measure single shot test.
scd4x: co2 is 921ppm.
scd4x: temperature is 25.91C.
scd4x: humidity is 40.52%.
scd4x: co2 is 921ppm.
scd4x: temperature is 26.12C.
scd4x: humidity is 40.64%.
scd4x: co2 is 862ppm.
scd4x: temperature is 26.30C.
scd4x: humidity is 40.74%.
scd4x: measure single shot rht only test.
scd4x: humidity is 40.82%.
scd4x: humidity is 40.82%.
scd4x: humidity is 40.79%.
scd4x: finish read test.
```

```shell
scd4x -e read --type=SCD41 --times=3

scd4x: 1/3.
scd4x: co2 is 1260ppm.
scd4x: temperature is 29.67C.
scd4x: humidity is 42.67%.
scd4x: 2/3.
scd4x: co2 is 1291ppm.
scd4x: temperature is 29.73C.
scd4x: humidity is 42.70%.
scd4x: 3/3.
scd4x: co2 is 1272ppm.
scd4x: temperature is 29.62C.
scd4x: humidity is 42.64%.
```

```shell
scd4x -e shot --type=SCD41 --times=3

scd4x: 1/3.
scd4x: co2 is 1475ppm.
scd4x: temperature is 25.86C.
scd4x: humidity is 40.49%.
scd4x: 2/3.
scd4x: co2 is 1302ppm.
scd4x: temperature is 26.07C.
scd4x: humidity is 40.61%.
scd4x: 3/3.
scd4x: co2 is 1323ppm.
scd4x: temperature is 26.17C.
scd4x: humidity is 40.67%.
```

```shell
scd4x -e wake-up --type=SCD41

scd4x: wake up.
```

```shell
scd4x -e power-down --type=SCD41

scd4x: power down.
```

```shell
scd4x -e number --type=SCD41

scd4x: serial number is 0xF2F1CF073B26.
```

```shell
scd4x -h

Usage:
  scd4x (-i | --information)
  scd4x (-h | --help)
  scd4x (-p | --port)
  scd4x (-t reg | --test=reg) [--type=<SCD40 | SCD41>]
  scd4x (-t read | --test=read) [--type=<SCD40 | SCD41>] [--times=<num>]
  scd4x (-e read | --example=read) [--type=<SCD40 | SCD41>] [--times=<num>]
  scd4x (-e shot | --example=shot) [--type=<SCD40 | SCD41>] [--times=<num>]
  scd4x (-e wake-up | --example=wake-up) [--type=<SCD40 | SCD41>]
  scd4x (-e power-down | --example=power-down) [--type=<SCD40 | SCD41>]
  scd4x (-e number | --example=number) [--type=<SCD40 | SCD41>]

Options:
  -e <read | shot | wake-up | power-down | number>, --example=<read | shot | wake-up | power-down | number>
                        Run the driver example.
  -h, --help            Show the help.
  -i, --information     Show the chip information.
  -p, --port            Display the pin connections of the current board.
  -t <reg | read>, --test=<reg | read>
                        Run the driver test.
      --times=<num>     Set the running times.([default: 3])
      --type=<SCD40 | SCD41>
                        Set the chip type.([default: SCD41])
```


### 1. Board

#### 1.1 Board Info

Board Name: Raspberry Pi 4B.

IIC Pin: SCL/SDA GPIO3/GPIO2.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```

#### 2.2 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.3 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
make test
```

Find the compiled library in CMake. 

```cmake
find_package(scd4x REQUIRED)
```
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
./scd4x -i

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
./scd4x -p

scd4x: SCL connected to GPIO3(BCM).
scd4x: SDA connected to GPIO2(BCM).
```

```shell
./scd4x -t reg --type=SCD41

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
scd4x: set temperature offset 45298.
scd4x: check temperature offset ok.
scd4x: scd4x_temperature_offset_convert_to_register/scd4x_temperature_offset_convert_to_data test.
scd4x: set temperature 31.00.
scd4x: check temperature 31.00.
scd4x: scd4x_set_sensor_altitude/scd4x_get_sensor_altitude test.
scd4x: set sensor altitude 64677.
scd4x: check sensor altitude ok.
scd4x: scd4x_sensor_altitude_convert_to_register/scd4x_sensor_altitude_convert_to_data test.
scd4x: set altitude 105.00.
scd4x: check altitude 105.00.
scd4x: scd4x_set_ambient_pressure/scd4x_get_ambient_pressure test.
scd4x: set ambient pressure 20738.
scd4x: check ambient pressure ok.
scd4x: scd4x_ambient_pressure_convert_to_register/scd4x_ambient_pressure_convert_to_data test.
scd4x: set ambient pressure 100335.00.
scd4x: check ambient pressure 100300.00.
scd4x: scd4x_co2_convert_to_register/scd4x_co2_convert_to_data test.
scd4x: set co2 86.00.
scd4x: check co2 86.00.
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
scd4x: set automatic self calibration initial period 996.
scd4x: check automatic self calibration initial period ok.
scd4x: scd4x_set_automatic_self_calibration_standard_period/scd4x_get_automatic_self_calibration_standard_period test.
scd4x: set automatic self calibration standard period 484.
scd4x: check automatic self calibration standard period ok.
scd4x: finish register test.
```

```shell
./scd4x -t read --type=SCD41 --times=3

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
scd4x: co2 is 1332ppm.
scd4x: temperature is 29.26C.
scd4x: humidity is 42.43%.
scd4x: co2 is 1334ppm.
scd4x: temperature is 29.24C.
scd4x: humidity is 42.42%.
scd4x: co2 is 1351ppm.
scd4x: temperature is 29.17C.
scd4x: humidity is 42.38%.
scd4x: low power continuous read test.
scd4x: co2 is 1426ppm.
scd4x: temperature is 29.88C.
scd4x: humidity is 42.79%.
scd4x: co2 is 1610ppm.
scd4x: temperature is 28.12C.
scd4x: humidity is 41.79%.
scd4x: co2 is 1615ppm.
scd4x: temperature is 27.00C.
scd4x: humidity is 41.14%.
scd4x: measure single shot test.
scd4x: co2 is 1500ppm.
scd4x: temperature is 25.48C.
scd4x: humidity is 40.28%.
scd4x: co2 is 1496ppm.
scd4x: temperature is 25.70C.
scd4x: humidity is 40.40%.
scd4x: co2 is 1433ppm.
scd4x: temperature is 25.87C.
scd4x: humidity is 40.50%.
scd4x: measure single shot rht only test.
scd4x: humidity is 40.60%.
scd4x: humidity is 40.60%.
scd4x: humidity is 40.59%.
scd4x: finish read test.
```

```shell
./scd4x -e read --type=SCD41 --times=3

scd4x: 1/3.
scd4x: co2 is 1074ppm.
scd4x: temperature is 28.97C.
scd4x: humidity is 42.27%.
scd4x: 2/3.
scd4x: co2 is 1187ppm.
scd4x: temperature is 28.95C.
scd4x: humidity is 42.26%.
scd4x: 3/3.
scd4x: co2 is 1291ppm.
scd4x: temperature is 28.88C.
scd4x: humidity is 42.22%.
```

```shell
./scd4x -e shot --type=SCD41 --times=3

scd4x: 1/3.
scd4x: co2 is 1398ppm.
scd4x: temperature is 25.19C.
scd4x: humidity is 40.11%.
scd4x: 2/3.
scd4x: co2 is 1448ppm.
scd4x: temperature is 25.33C.
scd4x: humidity is 40.19%.
scd4x: 3/3.
scd4x: co2 is 1393ppm.
scd4x: temperature is 25.52C.
scd4x: humidity is 40.30%.
```

```shell
./scd4x -e wake-up --type=SCD41

scd4x: wake up.
```

```shell
./scd4x -e power-down --type=SCD41

scd4x: power down.
```

```shell
./scd4x -e number --type=SCD41

scd4x: serial number is 0xF2F1CF073B26.
```

```shell
./scd4x -h

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

PZEM-004T v3.0
================
[![GitHub issues](https://img.shields.io/github/issues/mandulaj/PZEM-004T-v30)](https://github.com/avaldebe/mandulaj/PZEM-004T-v30)
[![GitHub license](https://img.shields.io/github/license/mandulaj/PZEM-004T-v30)](https://github.com/mandulaj/PZEM-004T-v30/blob/master/LICENSE)
[![PlatformIO Build](https://github.com/mandulaj/PZEM-004T-v30/actions/workflows/platformio_build.yml/badge.svg)](https://github.com/mandulaj/PZEM-004T-v30/actions/workflows/platformio_build.yml)
[![Buy me a Beer](https://img.shields.io/badge/Buy%20me%20a%20Beer-%245-orange)](https://www.buymeacoffee.com/mandula)


__[LINKS & REFs](LINKS.md)__


Arduino library for Peacefair **PZEM-004T-10A** and [**PZEM-004T-100A v3.0**](https://innovatorsguru.com/pzem-004t-v3/) Energy monitor using the ModBUS interface.


The Version 3.0 PZEM is an upgraded version of the older PZEM-004T for which you can find the library [Here](https://github.com/olehs/PZEM004T) 

### Main features
 * Measures Voltage, Current, Power, Energy, **Power Factor** and **Frequency** (New in Version 3.0)
 * 247 unique programmable slave addresses
    * Enables multiple slaves to use the same Serial interface [PZEM MultiDevice Demo](examples/PZEMMultiDevice/PZEMMultiDevice.ino)
      (May need to use external transistors to drive multiple devices on one bus due to GPIO current limitations)
 * Internal Energy counter up to 9999.99kWh



## Common issues: READ FIRST BEFORE OPENING NEW ISSUE!
* Make sure the PZEM device is **connected to the 230V AC power**! The 5V pin only powers the optocouplers, not the actual chip. 
* Make sure the **5V and GND are BOTH connected**! They are essential for the optocouplers!
* If you are getting `NaN`s and only the TX LED is blinking, try **swapping the RX/TX wires**.
* Make sure you are using the correct custom address (change with [PZEMChangeAddress](https://github.com/mandulaj/PZEM-004T-v30/tree/master/examples/PZEMChangeAddress)) or you are using the default address `PZEM_DEFAULT_ADDR` = `0xF8` (only works for 1 device on ModBus)
* If you want to use multiple devices on the same ModBus, please set a custom address for each and use [PZEMMultiDevice](https://github.com/mandulaj/PZEM-004T-v30/blob/master/examples/PZEMMultiDevice/PZEMMultiDevice.ino)
* If the current is much higher than you would expect (eg. 0.5A for a 60W device), don't panic, welcome to the world of AC! You are probably dealing with a device with bad powerfactor <1. In such cases the classic `P=V*I` does not apply. You can read more about this here: [Power Factor](https://en.wikipedia.org/wiki/Power_factor)
* Please be safe, AC is dangerous! If you don't know what you are doing, you can **die**! You are responsible for your own safety.


### Other features
  * Over power alarm
  * Energy counter reset
  * CRC16 checksum
  * Better, but not perfect mains isolation


# The module
<img alt="PZEM-004T v3.0 Image" src="https://innovatorsguru.com/wp-content/uploads/2019/06/PZEM-004-module.jpg" width="200" align="right">

This module is an upgraded version of the PZEM-004T with frequency and power factor measurement features, available at the usual places. It communicates using a TTL interface over a Modbus-RTU like communication protocol but is incompatible with the older [@olehs](https://github.com/olehs) library found here: [https://github.com/olehs/PZEM004T](https://github.com/olehs/PZEM004T). I would like to thank [@olehs](https://github.com/olehs) for the great library which inspired me to write this one.


### Manufacturer (optimistic) specifications

| Function      | Measuring range      | Resolution      | Accuracy | TODO: Realistic specifications |
|---------------|----------------------|-----------------|----------|--------------------------------|
| Voltage       | 80~260V              | 0.1V            | 0.5%     |                                |
| Current       | 0\~10A or 0\~100A*   | 0.01A or 0.02A* | 0.5%     |                                |
| Active power  | 0\~2.3kW or 0\~23kW* | 0.1W            | 0.5%     |                                |
| Active energy | 0~9999.99kWh         | 1Wh             | 0.5%     |                                |
| Frequency     | 45~65Hz              | 0.1Hz           | 0.5%     |                                |
| Power factor  | 0.00~1.00            | 0.01            | 1%       |                                |

\* Using the external current transformer instead of the built in shunt

## Compatibility

| MCU                       | Hardware Serial    | Software Serial  | Not Tested | Examples                              | Notes                                                                          |
|---------------------------|:------------------:|:----------------:|:----------:|---------------------------------------|--------------------------------------------------------------------------------|
|ATmega168                  |                    |                  | X          | [HardwareSerial][] [SoftwareSerial][] |                                                                                | 
|ATmega328 (`Arduino Uno`)  |(:white_check_mark:)|:heavy_check_mark:|            | [HardwareSerial][] [SoftwareSerial][] | HW Serial conflicts with Debug output. It can be used however without having any Serial Console output | 
|ATmega2560 (`Arduino Mega`)|:heavy_check_mark:  |:heavy_check_mark:|            | [HardwareSerial][] [SoftwareSerial][] |                                                                                | 
|ESP8266                    |(:white_check_mark:)|:heavy_check_mark:|            | [SoftwareSerial][]                    | HW Serial conflicts with Debug output Serial                                         | 
|ESP32                      |:heavy_check_mark:  | :x:              |            | [HardwareSerial][]                    | SW Serial not really needed as ESP32 has 3 HW serials with configurable pins   | 
|STM32 BluePill             |                    |                  | X          |                                       |                                                                                | 

[HardwareSerial]: examples/PZEMHardSerial/PZEMHardSerial.ino
[SoftwareSerial]: examples/PZEMSoftwareSerial/PZEMSoftwareSerial.ino

# Examples
## Hardware Serial
This example uses Hardware `Serial2` in order to interface with the PZEM module. Note that not all MCUs feature multiple Serial ports. It won't for example work on the `Arduino Uno`.  

```c++

#include <PZEM004Tv30.h>

/* Hardware Serial2 is only available on certain boards.
 * For example the Arduino MEGA 2560
*/
#if defined(ESP32)
PZEM004Tv30 pzem(Serial2, 16, 17);
#else
PZEM004Tv30 pzem(Serial2);
#endif

void setup() {
    Serial.begin(115200);

    // Uncomment in order to reset the internal energy counter
    // pzem.resetEnergy();
}

void loop() {
        
    Serial.print("Custom Address:");
    Serial.println(pzem.readAddress(), HEX);

    // Read the data from the sensor
    float voltage = pzem.voltage();
    float current = pzem.current();
    float power = pzem.power();
    float energy = pzem.energy();
    float frequency = pzem.frequency();
    float pf = pzem.pf();

    // Check if the data is valid
    if(isnan(voltage)){
        Serial.println("Error reading voltage");
    } else if (isnan(current)) {
        Serial.println("Error reading current");
    } else if (isnan(power)) {
        Serial.println("Error reading power");
    } else if (isnan(energy)) {
        Serial.println("Error reading energy");
    } else if (isnan(frequency)) {
        Serial.println("Error reading frequency");
    } else if (isnan(pf)) {
        Serial.println("Error reading power factor");
    } else {

        // Print the values to the Serial console
        Serial.print("Voltage: ");      Serial.print(voltage);      Serial.println("V");
        Serial.print("Current: ");      Serial.print(current);      Serial.println("A");
        Serial.print("Power: ");        Serial.print(power);        Serial.println("W");
        Serial.print("Energy: ");       Serial.print(energy,3);     Serial.println("kWh");
        Serial.print("Frequency: ");    Serial.print(frequency, 1); Serial.println("Hz");
        Serial.print("PF: ");           Serial.println(pf);

    }

    Serial.println();
    delay(2000);
}
```
### Output:
```
Custom Address:10
Voltage: 229.60V
Current: 0.10A
Power: 4.50W
Energy: 7.368kWh
Frequency: 50.0Hz
PF: 0.19
```


## Software Serial
Using the [`<SoftwareSerial.h>`](https://www.arduino.cc/en/Reference/softwareSerial) library...
```c++
#include <PZEM004Tv30.h>
#include <SoftwareSerial.h>

/* Use software serial for the PZEM
 * Pin 11 Rx (Connects to the Tx pin on the PZEM)
 * Pin 12 Tx (Connects to the Rx pin on the PZEM)
*/

SoftwareSerial pzemSWSerial(11, 12);
PZEM004Tv30 pzem;

void setup() {
  Serial.begin(115200);
  
  pzem = PZEM004Tv30(pzemSWSerial);
}

void loop() {
         
    Serial.print("Custom Address:");
    Serial.println(pzem.readAddress(), HEX);

    // Read the data from the sensor
    float voltage = pzem.voltage();
    float current = pzem.current();
    float power = pzem.power();
    float energy = pzem.energy();
    float frequency = pzem.frequency();
    float pf = pzem.pf();

    // Check if the data is valid
    if(isnan(voltage)){
        Serial.println("Error reading voltage");
    } else if (isnan(current)) {
        Serial.println("Error reading current");
    } else if (isnan(power)) {
        Serial.println("Error reading power");
    } else if (isnan(energy)) {
        Serial.println("Error reading energy");
    } else if (isnan(frequency)) {
        Serial.println("Error reading frequency");
    } else if (isnan(pf)) {
        Serial.println("Error reading power factor");
    } else {

        // Print the values to the Serial console
        Serial.print("Voltage: ");      Serial.print(voltage);      Serial.println("V");
        Serial.print("Current: ");      Serial.print(current);      Serial.println("A");
        Serial.print("Power: ");        Serial.print(power);        Serial.println("W");
        Serial.print("Energy: ");       Serial.print(energy,3);     Serial.println("kWh");
        Serial.print("Frequency: ");    Serial.print(frequency, 1); Serial.println("Hz");
        Serial.print("PF: ");           Serial.println(pf);

    }

    Serial.println();
    delay(2000);
}
```
### Output:
```
Custom Address:11
Voltage: 229.60V
Current: 0.10A
Power: 4.50W
Energy: 7.368kWh
Frequency: 50.0Hz
PF: 0.19
```

# Links
Check [LINKS & REFs](LINKS.md) page for additional DOC's and related links

# Contributing
Many thanks to all the other contributors that add new features, find bugs and generally keep this project afloa.
 * [@mandulaj](https://github.com/mandulaj)
 * [@FXeio](https://github.com/FXeio)
 * [@ste94pz](https://github.com/ste94pz)
 * [@vortigont](https://github.com/vortigont)
 * [@kozfelipe](https://github.com/kozfelipe)



***
Thank you to [@olehs](https://github.com/olehs) for inspiring this library.

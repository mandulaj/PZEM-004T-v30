/*
Copyright (c) 2021 Jakub Mandula

Example of using one PZEM module with Hardware Serial interface.
================================================================

If desired, a HardwareSerial handle can be passed to the constructor
which will then be used for the communication with the module.

Note that ESP32 HardwareSerial must also be provided with the RX and TX
pins.

*/

#include <PZEM004Tv30.h>

/* Hardware Serial2 is only available on certain boards.
 * For example the Arduino MEGA 2560
*/
#if defined(ESP32)
PZEM004Tv30 pzem(&Serial2, 16, 17);
#else
PZEM004Tv30 pzem(&Serial2);
#endif

void setup() {
  Serial.begin(115200);
}

void loop() {
        
    Serial.print("Custom Address:");
    Serial.println(pzem.readAddress(false), HEX);

    float voltage = pzem.voltage();
    if(!isnan(voltage)){
        Serial.print("Voltage: "); Serial.print(voltage); Serial.println("V");
    } else {
        Serial.println("Error reading voltage");
    }

    float current = pzem.current();
    if(!isnan(current)){
        Serial.print("Current: "); Serial.print(current); Serial.println("A");
    } else {
        Serial.println("Error reading current");
    }

    float power = pzem.power();
    if(!isnan(power)){
        Serial.print("Power: "); Serial.print(power); Serial.println("W");
    } else {
        Serial.println("Error reading power");
    }

    float energy = pzem.energy();
    if(!isnan(energy)){
        Serial.print("Energy: "); Serial.print(energy,3); Serial.println("kWh");
    } else {
        Serial.println("Error reading energy");
    }

    float frequency = pzem.frequency();
    if(!isnan(frequency)){
        Serial.print("Frequency: "); Serial.print(frequency, 1); Serial.println("Hz");
    } else {
        Serial.println("Error reading frequency");
    }

    float pf = pzem.pf();
    if(!isnan(pf)){
        Serial.print("PF: "); Serial.println(pf);
    } else {
        Serial.println("Error reading power factor");
    }

    Serial.println();
    delay(2000);
}

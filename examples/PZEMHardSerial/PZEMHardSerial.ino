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


#if !defined(PZEM_RX_PIN) && !defined(PZEM_TX_PIN)
#define PZEM_RX_PIN 16
#define PZEM_TX_PIN 17
#endif

#if !defined(PZEM_SERIAL)
#define PZEM_SERIAL Serial2
#endif


#if defined(ESP32)
/*************************
 *  ESP32 initialization
 * ---------------------
 * 
 * The ESP32 HW Serial interface can be routed to any GPIO pin 
 * Here we initialize the PZEM on Serial2 with RX/TX pins 16 and 17
 */
PZEM004Tv30 pzem(PZEM_SERIAL, PZEM_RX_PIN, PZEM_TX_PIN);
#elif defined(ESP8266)
/*************************
 *  ESP8266 initialization
 * ---------------------
 * 
 * Not all Arduino boards come with multiple HW Serial ports.
 * Serial2 is for example available on the Arduino MEGA 2560 but not Arduino Uno!
 * The ESP32 HW Serial interface can be routed to any GPIO pin 
 * Here we initialize the PZEM on Serial2 with default pins
 */
//PZEM004Tv30 pzem(Serial1);
#else
/*************************
 *  Arduino initialization
 * ---------------------
 * 
 * Not all Arduino boards come with multiple HW Serial ports.
 * Serial2 is for example available on the Arduino MEGA 2560 but not Arduino Uno!
 * The ESP32 HW Serial interface can be routed to any GPIO pin 
 * Here we initialize the PZEM on Serial2 with default pins
 */
PZEM004Tv30 pzem(PZEM_SERIAL);
#endif

void setup() {
    // Debugging Serial port
    Serial.begin(115200);

    // Uncomment in order to reset the internal energy counter
    // pzem.resetEnergy()
}

void loop() {
    // Print the custom address of the PZEM
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

/*
Copyright (c) 2021 Jakub Mandula

Example of using multiple PZEM modules together on one ModBUS.
================================================================

First of all, use the PZEMChangeAddress example in order to assign 
each individual PZEM module a unique custom address. This example 
requires 2 PZEM modules with addresses 0x10 and 0x11.


Then for each PZEM module create a PZEM004Tv30 instance passing a custom address
to the address field.

The instances can either be stored as individual objects:

```c
PZEM004Tv30 pzem0(&Serial2, 0x10);
PZEM004Tv30 pzem1(&Serial2, 0x11);
PZEM004Tv30 pzem2(&Serial2, 0x12);

pzem0.voltage();
pzem1.pf();
```

Or in an array and addressed using the array index:

```c
PZEM004Tv30 pzems[] = {
    PZEM004Tv30(&Serial2, 0x10),
    PZEM004Tv30(&Serial2, 0x11),
    PZEM004Tv30(&Serial2, 0x12)};

pzems[0].voltage();
pzems[1].pf();
```

*/

#include <PZEM004Tv30.h>


#if !defined(PZEM_RX_PIN) && !defined(PZEM_TX_PIN)
#define PZEM_RX_PIN 16
#define PZEM_TX_PIN 17
#endif

#if !defined(PZEM_SERIAL)
#define PZEM_SERIAL Serial2
#endif

#define NUM_PZEMS 2

PZEM004Tv30 pzems[NUM_PZEMS];

/* ***************************************************************
 * Uncomment USE_SOFTWARE_SERIAL in order to enable Softare serial
 *
 * Does not work for ESP32
 *****************************************************************/
//#define USE_SOFTWARE_SERIAL



#if defined(USE_SOFTWARE_SERIAL) && defined(ESP32)
    #error "Can not use SoftwareSerial with ESP32"
#elif defined(USE_SOFTWARE_SERIAL)

#include <SoftwareSerial.h>

SoftwareSerial pzemSWSerial(PZEM_RX_PIN, PZEM_TX_PIN);
#endif

void setup() {
    Serial.begin(115200);

    // For each PZEM, initialize it
    for(int i = 0; i < NUM_PZEMS; i++)
    {

#if defined(USE_SOFTWARE_SERIAL)
        // Initialize the PZEMs with Software Serial
        pzems[i] = PZEM004Tv30(pzemSWSerial, 0x10 + i);
#elif defined(ESP32)
        // Initialize the PZEMs with Hardware Serial2 on RX/TX pins 16 and 17
        pzems[i] = PZEM004Tv30(PZEM_SERIAL, PZEM_RX_PIN, PZEM_TX_PIN, 0x10 + i);
#else
        // Initialize the PZEMs with Hardware Serial2 on the default pins

        /* Hardware Serial2 is only available on certain boards.
        *  For example the Arduino MEGA 2560
        */
        pzems[i] = PZEM004Tv30(PZEM_SERIAL, 0x10 + i);
#endif
    }
}



void loop() {
    // Print out the measured values from each PZEM module
    for(int i = 0; i < NUM_PZEMS; i++){
        // Print the Address of the PZEM
        Serial.print("PZEM ");
        Serial.print(i);
        Serial.print(" - Address:");
        Serial.println(pzems[i].getAddress(), HEX);
        Serial.println("===================");



        // Read the data from the sensor
        float voltage = pzems[i].voltage();
        float current = pzems[i].current();
        float power = pzems[i].power();
        float energy = pzems[i].energy();
        float frequency = pzems[i].frequency();
        float pf = pzems[i].pf();


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

        Serial.println("-------------------");
        Serial.println();
    }

    Serial.println();
    delay(2000);
}

/*
Copyright (c) 2021 Jakub Mandula

Example of setting a custom ModBUS address for the PZEM modules.
================================================================

Each PZEM modules has two addresses. A default general address (0xF8),
which every module will listen on by default. And a custom address.

The custom address can be used if multiple PZEM modules are to be used
on a single ModBUS.

This script will use the general address in order to set the custom 
address of the connected PZEM module.

Therefore make sure only one PZEM module is connected at a time! 
Otherwise all connected modules will receive the same custom address.

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

/*******************************************
 * Set your address over here..
 * The address can be between 0x01 and 0xF7
 *******************************************/
#define SET_ADDRESS 0x55

// Set to true in order to increment the address every iteration
#define INCREMENT false


void setup() {
  Serial.begin(115200);
}

void loop() {
    static uint8_t addr = SET_ADDRESS;

    // Print out current custom address
    Serial.print("Previous address:   0x");
    Serial.println(pzem.readAddress(false), HEX);

    // Set the custom address
    Serial.print("Setting address to: 0x");
    Serial.println(addr, HEX);
    if(!pzem.setAddress(addr))
    {
      // Setting custom address failed. Probably no PZEM connected
      Serial.println("Error setting address.");
    } else {
      // Print out the new custom address
      Serial.print("Current address:    0x");
      Serial.println(pzem.readAddress(), HEX);
      Serial.println();
    }

    // Increment the address every loop if desired
    if(INCREMENT){
      addr++;
      if(addr >= PZEM_DEFAULT_ADDR)
        addr = 0x01;
    }

    delay(5000);
}

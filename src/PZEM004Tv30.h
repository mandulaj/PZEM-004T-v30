/*
Copyright (c) 2019 Jakub Mandula

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the “Software”), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


/*
 * PZEM-004Tv30.h
 *
 * Interface library for the upgraded version of PZEM-004T v3.0
 * Based on the PZEM004T library by @olehs https://github.com/olehs/PZEM004T
 *
 * Author: Jakub Mandula https://github.com/mandulaj
 *
 *
*/


#ifndef PZEM004TV30_H
#define PZEM004TV30_H



#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/* Disable Software Serial completely by defining: */
// #define PZEM004_NO_SWSERIAL

#ifndef PZEM004_NO_SWSERIAL
 /* Software serial is only available for AVRs and ESP8266 */
 #if defined(__AVR__) || defined(ESP8266)
 #define PZEM004_SOFTSERIAL
 #endif
#endif

#if defined(PZEM004_SOFTSERIAL)
#include <SoftwareSerial.h>
#endif


#define PZEM_DEFAULT_ADDR   0xF8
#define PZEM_BAUD_RATE      9600


class PZEM004Tv30
{
public:
#if defined(PZEM004_SOFTSERIAL)
    /* This will be deprecated */
    [[deprecated("Replaced by PZEM004Tv30(SoftwareSerial& port, uint8_t addr). Please pass the SoftwareSerial object instead: PZEM004Tv30(RX, TX)=>PZEM004Tv30(SWSerial)")]]
    PZEM004Tv30(uint8_t receivePin, uint8_t transmitPin, uint8_t addr=PZEM_DEFAULT_ADDR);
    
    /* SoftwareSerial version calls begin method */
    /* Stream version doesn't */
    PZEM004Tv30(SoftwareSerial& port, uint8_t addr=PZEM_DEFAULT_ADDR);
    PZEM004Tv30(Stream& port, uint8_t addr=PZEM_DEFAULT_ADDR);

#endif

#if defined(ESP32)
    /* ESP32 Hardware serial interface requires the receive and transmit pin specified */
    PZEM004Tv30(HardwareSerial& port, uint8_t receivePin, uint8_t transmitPin, uint8_t addr=PZEM_DEFAULT_ADDR);
    
    // Deprecate passing pointer
    [[deprecated("Replaced by PZEM004Tv30(HardwareSerial& port, uint8_t receivePin, uint8_t transmitPin, uint8_t addr). Please pass a reference instead of a pointer: PZEM004Tv30(&SerialX)=>PZEM004Tv30(SerialX)")]]
    PZEM004Tv30(HardwareSerial* port, uint8_t receivePin, uint8_t transmitPin, uint8_t addr=PZEM_DEFAULT_ADDR) : PZEM004Tv30(*port, receivePin, transmitPin, addr) {};
#else
    PZEM004Tv30(HardwareSerial& port, uint8_t addr=PZEM_DEFAULT_ADDR);
    
    // Deprecate passing pointer
    [[deprecated("Replaced by PZEM004Tv30(HardwareSerial& port, uint8_t addr). Please pass a reference instead of a pointer: PZEM004Tv30(&SerialX)=>PZEM004Tv30(SerialX)")]]
    PZEM004Tv30(HardwareSerial* port, uint8_t addr=PZEM_DEFAULT_ADDR) : PZEM004Tv30(*port, addr) {};
#endif
    // Empty constructor for creating arrays
    PZEM004Tv30(){};

    ~PZEM004Tv30();


    float voltage();
    float current();
    float power();
    float energy();
    float frequency();
    float pf();


    bool setAddress(uint8_t addr);
    uint8_t getAddress();
    uint8_t readAddress(bool update = false);


    bool setPowerAlarm(uint16_t watts);
    bool getPowerAlarm();

    bool resetEnergy();

    void search();

private:

    Stream* _serial; // Serial interface
    bool _isSoft;    // Is serial interface software

    // TODO: if we remove the Local SW serial handling, we can get rid of this trash
    #if defined(PZEM004_SOFTSERIAL)
    SoftwareSerial* localSWserial = nullptr; // Pointer to the Local SW serial object
    #endif

    uint8_t _addr;   // Device address

    bool _isConnected; // Flag set on successful communication

    struct {
        float voltage;
        float current;
        float power;
        float energy;
        float frequency;
        float pf;
        uint16_t alarms;
    }  _currentValues; // Measured values

    uint64_t _lastRead; // Last time values were updated



    void init(Stream* port, bool isSoft, uint8_t addr); // Init common to all constructors

    bool updateValues();    // Get most up to date values from device registers and cache them
    uint16_t receive(uint8_t *resp, uint16_t len); // Receive len bytes into a buffer

    bool sendCmd8(uint8_t cmd, uint16_t rAddr, uint16_t val, bool check=false, uint16_t slave_addr=0xFFFF); // Send 8 byte command

    void setCRC(uint8_t *buf, uint16_t len);           // Set the CRC for a buffer
    bool checkCRC(const uint8_t *buf, uint16_t len);   // Check CRC of buffer

    uint16_t CRC16(const uint8_t *data, uint16_t len); // Calculate CRC of buffer
};



// Define for setting debug level verbosity
//#define PZEM004TV30_DEBUG 0

#ifdef PZEM004TV30_DEBUG

// For configuring custom serial debug interface, default: Serial
#ifndef PZEM004TV30_DEBUG_SERIAL
#define PZEM004TV30_DEBUG_SERIAL Serial
#endif


// For debugging
extern HardwareSerial PZEM004TV30_DEBUG_SERIAL;


// Debugging function;
void printBuf(uint8_t* buffer, uint16_t len) {
    for(uint16_t i = 0; i < len; i++){
        char temp[6];
        sprintf(temp, "%.2x ", buffer[i]);
        PZEM004TV30_DEBUG_SERIAL.print(temp);

    }
    PZEM004TV30_DEBUG_SERIAL.println();
}
    #define DEBUG(...) PZEM004TV30_DEBUG_SERIAL.print(__VA_ARGS__)
    #define DEBUGLN(...) PZEM004TV30_DEBUG_SERIAL.println(__VA_ARGS__)
    #define DEBUGBUF(buf, len) printBuf(buf, len)


#else
    // Debugging mode off, disable the macros
    #define DEBUG(...)
    #define DEBUGLN(...)
    #define DEBUGBUF(buf, len)
#endif



#endif // PZEM004T_H

#include <Arduino.h>
#include <PZEM004Tv30.h>

/* Hardware Serial1 is usually occupied by USB2Serial converter
 * so we will use Serial2
*/
HardwareSerial PzemSerial(2);     // HW Serial UART2 default mapping is at pins IO-16 (RX2) and IO-17 (TX2)

// But just for example we will remap it to pins 21 (rx) and 22 (tx) 
PZEM004Tv30 pzem(&PzemSerial, 21, 22);

void setup() {
  Serial.begin(115200);         // Serial1 is used for console output
}

void loop() {
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
    Serial.printf("End: %u\n", millis());

    delay(5000);
}

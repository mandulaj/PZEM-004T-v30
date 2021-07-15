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

uint8_t addr = 0x01;

void loop() {
    pzem.setAddress(addr);
    Serial.print("Current address:");
    Serial.println(pzem.getAddress());
    Serial.println();

    if(++addr == 0xF8)
        addr = 0x01;

    delay(1000);
}

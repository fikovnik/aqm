#include <ESP8266WiFi.h>
#include <U8x8lib.h>
#include <SenseAirS8.h>


// void s8_init() {
//   //byte ch, cmd[] = {0xFE, 0x6, 0x0, 0x1F, 0x0, 0xB4, 0xAC, 0x74}; // Set
//   ABC Period to be 180 hours
//   //byte ch, cmd[] = {0xFE, 0x6, 0x0, 0x1F, 0x0, 0x30, 0xAC, 0x17}; // Set
//   ABC Period to be 48 hours byte ch, cmd[] = {0xFE, 0x6, 0x0, 0x1F, 0x0,
//   0x18, 0xAC, 0x09}; // Set ABC Period to be 24 hours
//   //s8_comm.enableTx(true);
//   s8_comm.write(cmd, 8);
//   //s8_comm.enableTx(false);
//   delay(250);

//   if (s8_comm.available()) {
//     Serial.print("SenseAir Response:");
//     while (s8_comm.available()) {
//       ch = (byte) s8_comm.read();
//       Serial.print(ch < 0x01 ? " 0" : " ");
//       Serial.print(ch, HEX);
//     }
//     Serial.println();
//   }
// }

SenseAirS8 co2sensor(D7, D8);
U8X8_SSD1306_128X64_NONAME_SW_I2C display(D5, D2, U8X8_PIN_NONE);

void setup() {
  Serial.begin(9600);

  co2sensor.begin();
  display.begin();
}

#define SIZE 8
unsigned int m = 0;

void loop() {
  display.setFont(u8x8_font_chroma48medium8_r);

  int co2 = co2sensor.read_co2();

  if (m == 0) {
    display.clear();
  }

  display.printf("%d: %d\n", m, co2);

  if (m + 1 == SIZE) {
    m = 0;
  } else {
    m++;
  }

  delay(15000);
}

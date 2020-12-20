#include <ESP8266WiFi.h>
#include <U8x8lib.h>
#include <SenseAirS8.h>
#include <debug.h>

SenseAirS8 co2sensor(D7, D8);
U8X8_SSD1306_128X64_NONAME_SW_I2C display(D5, D2, U8X8_PIN_NONE);

void setup() {
  Serial.begin(9600);

  co2sensor.begin();
  display.begin();
}

#define SIZE 8
unsigned int m = 0;
// bool calibrated = false;

void loop() {
  // if (!calibrated) {
  //   co2sensor.calibrate();
  //   calibrated = true;
  //   delay(15000);
  // }

  display.setFont(u8x8_font_chroma48medium8_r);

  int co2 = co2sensor.read_co2();
  DEBUG("co2: %d\n", co2);

  int abs = co2sensor.read_ABS();
  DEBUG("abs: %d\n", abs);

  int status = co2sensor.read_status();
  DEBUG("status: %d\n", status);

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

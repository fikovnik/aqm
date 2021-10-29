#include "Adafruit_SHT4x.h"

#define SDA_PIN D2
#define SCL_PIN D1

Adafruit_SHT4x sht4 = Adafruit_SHT4x();

void setup() {
  Serial.begin(74880);

  // wait until Serial is available
  while (!Serial) delay(10); 

  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(100000);

  if (!sht4.begin()) {
    Serial.println("Couldn't find SHT4x");
    while (1) delay(1);
  }

  Serial.println("Found SHT4x sensor");
  Serial.print("Serial number 0x");
  Serial.println(sht4.readSerial(), HEX);

  switch (sht4.getPrecision()) {
     case SHT4X_HIGH_PRECISION: 
       Serial.println("High precision");
       break;
     case SHT4X_MED_PRECISION: 
       Serial.println("Med precision");
       break;
     case SHT4X_LOW_PRECISION: 
       Serial.println("Low precision");
       break;
  }

  switch (sht4.getHeater()) {
     case SHT4X_NO_HEATER: 
       Serial.println("No heater");
       break;
     case SHT4X_HIGH_HEATER_1S: 
       Serial.println("High heat for 1 second");
       break;
     case SHT4X_HIGH_HEATER_100MS: 
       Serial.println("High heat for 0.1 second");
       break;
     case SHT4X_MED_HEATER_1S: 
       Serial.println("Medium heat for 1 second");
       break;
     case SHT4X_MED_HEATER_100MS: 
       Serial.println("Medium heat for 0.1 second");
       break;
     case SHT4X_LOW_HEATER_1S: 
       Serial.println("Low heat for 1 second");
       break;
     case SHT4X_LOW_HEATER_100MS: 
       Serial.println("Low heat for 0.1 second");
       break;
  }

  Serial.println("Done setup");
  delay(1000);
}

void loop() {
  sensors_event_t humidity, temp;
  uint32_t timestamp = millis();
  
  sht4.getEvent(&humidity, &temp);

  timestamp = millis() - timestamp;

  Serial.printf("Temperature: %.2f C\n", temp.temperature);
  Serial.printf("Humidity: %.2f%% rH\n", humidity.relative_humidity);
  Serial.printf("Duration: %d ms\n", timestamp);

  delay(5000);
}


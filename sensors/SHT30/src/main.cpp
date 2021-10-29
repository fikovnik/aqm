#include <WEMOS_SHT3X.h>

SHT3X sht30(0x45);

#define SDA_PIN D2
#define SCL_PIN D1

void setup() {
  Serial.begin(74880);

  // wait until Serial is available
  while (!Serial) delay(10); 

  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(100000);

  Serial.println("Done setup");
  delay(1000);
}

void loop() {
  uint32_t timestamp = millis();
  byte status = sht30.get();
  timestamp = millis() - timestamp;

  if (status == 0) {
    Serial.printf("Temperature: %.2f C\n", sht30.cTemp);
    Serial.printf("Humidity: %.2f%% rH\n", sht30.humidity);
    Serial.printf("Duration: %d ms\n", timestamp);
  } else {
    Serial.printf("Error %d\n", status);
  }

  delay(5000);
}


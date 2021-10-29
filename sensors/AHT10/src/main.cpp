#include <Adafruit_AHTX0.h>

#define SDA_PIN D2
#define SCL_PIN D1

Adafruit_AHTX0 aht;

void setup() {
  Serial.begin(74880);

  // wait until Serial is available
  while (!Serial) delay(10); 

  Wire.begin(SDA_PIN, SCL_PIN);
  // TODO: why does not 1e5 clock speed work?
  Wire.setClock(10000);

  if (!aht.begin()) {
    Serial.println("Couldn't find AHT");
    while (1) delay(1);
  }

  Serial.println("Found AHT sensor");
  Serial.println("Done setup");
  delay(1000);
}

void loop() {
  sensors_event_t humidity, temp;
  uint32_t timestamp = millis();
  
  aht.getEvent(&humidity, &temp);

  timestamp = millis() - timestamp;

  Serial.printf("Temperature: %.2f C\n", temp.temperature);
  Serial.printf("Humidity: %.2f%% rH\n", humidity.relative_humidity);
  Serial.printf("Duration: %d ms\n", timestamp);

  delay(5000);
}

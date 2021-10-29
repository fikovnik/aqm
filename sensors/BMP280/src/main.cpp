#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp;

#define SDA_PIN D2
#define SCL_PIN D1

void setup() {
  Serial.begin(74880);

  // wait until Serial is available
  while (!Serial) delay(10); 

  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(100000);

  if (!bmp.begin(BMP280_ADDRESS)) {
    Serial.println("Could not find a valid BMP280 sensor");
    while(1) delay(100);
  } else {
    Serial.printf("Found BMP280 with ID: 0x%X\n", bmp.sensorID());

    bmp.setSampling(
        Adafruit_BMP280::MODE_NORMAL,   /* Operating Mode. */
        Adafruit_BMP280::SAMPLING_X2,   /* Temp. oversampling */
        Adafruit_BMP280::SAMPLING_X16,  /* Pressure oversampling */
        Adafruit_BMP280::FILTER_X4,     /* Filtering. */
        Adafruit_BMP280::STANDBY_MS_500 /* Standby time. */
    ); 
  }

  Serial.println("Done setup");
  delay(1000);
}

void loop() {
  uint32_t timestamp = millis();
  float temp = bmp.readTemperature();
  float pressure = bmp.readPressure();
  float altitute = bmp.readAltitude(1013.25); // https://en.wikipedia.org/wiki/Atmospheric_pressure
  timestamp = millis() - timestamp;

  Serial.printf("Temperature: %.2f C\n", temp);
  Serial.printf("Pressure: %.2f Pa\n", pressure);
  Serial.printf("Altitute: %.2f m\n", altitute);
  Serial.printf("Duration: %d ms\n", timestamp);

  delay(5000);
}

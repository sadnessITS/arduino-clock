#include "interface.h"
#include "storage.h"

void setup() {
  using namespace objects;
  Wire.begin();
  Serial.begin(9600);

#ifndef ESP8266
  while (!Serial)
    ; // wait for serial port to connect. Needed for native USB
#endif

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1)
      delay(10);
  }

  if (!bmp.begin()) {
    Serial.println("Couldn't find BMP280");
    while (1)
      delay(10);
  }

  if (!aht20.begin()) {
    Serial.println("Couldn't find AHT20");
    while (1)
      delay(10);
  }

  matrix_cascade.setIntensity(5);
  matrix_cascade.setRotation(2);
}

void loop() {
  using namespace constants;
  using namespace timings;
  using objects::matrix_cascade;
  using objects::aht20;
  using objects::bmp;
  using enums::mode;

  DateTime now = getTime();

  matrix_cascade.setIntensity(getCascadeBrightness());

  switch (mode) {
  case Mode::ShowTime:
    if (millis() - timingMode > 1000) {
      showTime12H(now);
      blinkColon(now, 1, 1);
      timingMode = millis();
    }
    break;
  case Mode::EditTime:
    editTime(now);
    break;
  case Mode::ShowTemperature:
    if (millis() - timingMode > 100) {
      showSensorValue((int)aht20.getTemperature(), SYMBOL_CELSIUS);
      timingMode = millis();
    }
    break;
  case Mode::ShowHumidity:
    if (millis() - timingMode > 100) {
      showSensorValue((int)aht20.getHumidity(), SYMBOL_PERCENTAGE);
      timingMode = millis();
    }
    break;
  case Mode::ShowPressure:
    if (millis() - timingMode > 100) {
      showSensorValue((int)(bmp.readPressure() * 0.0075006375541921),
                      SYMBOL_HG);
      timingMode = millis();
    }
    break;
  }
}
#include "interface.h"
#include "storage.h"

void setup() {
  Serial.begin(9600);
  configureSensors();
  configureMatrix();
}

void loop() {
  using namespace constants;
  using namespace timings;
  using namespace flags;
  using enums::mode;
  using objects::aht20;
  using objects::bmp;
  using objects::matrix_cascade;

  DateTime now = getTime();

  matrix_cascade.setIntensity(getCascadeBrightness());

  if (digitalRead(CHANGE_MODE_BTN) == HIGH && !changeModeFlag &&
      millis() - timingChangeModeButton > 100) {
    changeModeFlag = true;
    timingChangeModeButton = millis();
    changeMode();
  }
  if (digitalRead(CHANGE_MODE_BTN) == LOW && changeModeFlag &&
      millis() - timingChangeModeButton > 100) {
    changeModeFlag = false;
    timingChangeModeButton = millis();
  }

  switch (mode) {
  case Mode::ShowTime12H:
    if (millis() - timingMode > 100) {
      showTime12H(now);
      timingMode = millis();
    }
    break;
  case Mode::ShowTime24H:
    if (millis() - timingMode > 100) {
      showTime24H(now);
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
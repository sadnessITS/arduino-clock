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

  Serial.println(now.year() % 100);

  matrix_cascade.setIntensity(getCascadeBrightness());

  controlModeBtn();
  controlFunctionalBtn();

  if (millis() - timingMode > 100) {
    switch (mode) {
    case Mode::ShowTime:
      if (is24HFormat)
        showTime24H(now, true);
      else
        showTime12H(now, true);
      break;
    case Mode::ShowDate:
      showDate(now);
      break;
    case Mode::EditTime:
      editTime(now);
      break;
    case Mode::ShowTemperature:
      if (isFarengeightFormat)
        showSensorValue((int)aht20.getTemperature() * 1.8 + 32, SYMBOL_FARENGEIGHT);
      else
        showSensorValue((int)aht20.getTemperature(), SYMBOL_CELSIUS);
      break;
    case Mode::ShowHumidity:
      showSensorValue((int)aht20.getHumidity(), SYMBOL_PERCENTAGE);
      break;
    case Mode::ShowPressure:
      showSensorValue((int)(bmp.readPressure() * 0.0075006375541921),
                      SYMBOL_HG);
      break;
    }
    timingMode = millis();
  }
}
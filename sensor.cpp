#include "interface.h"
#include "storage.h"
#include <Arduino.h>
#include <Encoder.h>

void configureSensors() {
  using namespace objects;
  // using constants::BUZZER_PIN;
  using constants::CHANGE_MODE_BTN;
  using constants::FUNC_BTN;

  // pinMode(BUZZER_PIN, OUTPUT);
  pinMode(FUNC_BTN, INPUT_PULLUP);
  pinMode(CHANGE_MODE_BTN, INPUT_PULLUP);

  Wire.begin();

  if (!rtc.begin()) {
    while (1)
      delay(10);
  }

  if (!bmp.begin()) {
    while (1)
      delay(10);
  }

  if (!aht20.begin()) {
    while (1)
      delay(10);
  }
}

DateTime getTime() {
  using objects::rtc;
  return rtc.now();
}

int getCascadeBrightness() {
  using brightness::brightness;
  using brightness::previous_encoder_brightness;
  using objects::encoder;

  int current_encoder_brightness = encoder.read();
  if (current_encoder_brightness > previous_encoder_brightness + 4) {
    brightness += 1;
    previous_encoder_brightness = current_encoder_brightness;
  } else if (current_encoder_brightness < previous_encoder_brightness - 4) {
    brightness -= 1;
    previous_encoder_brightness = current_encoder_brightness;
  }
  brightness = constrain(brightness, 1, 15);
  return brightness;
}

void controlModeBtn() {
  using constants::CHANGE_MODE_BTN;
  using flags::changeModeBtnFlag;
  using timings::timingChangeModeButton;
  using enums::mode;

  if (digitalRead(CHANGE_MODE_BTN) == HIGH && !changeModeBtnFlag &&
      millis() - timingChangeModeButton > 100) {
    changeModeBtnFlag = true;
    timingChangeModeButton = millis();

    switch (mode) {
    case Mode::ShowTime:
      mode = Mode::ShowDate;
      break;
    case Mode::ShowDate:
      mode = Mode::ShowTemperature;
      break;
    case Mode::ShowTemperature:
      mode = Mode::ShowHumidity;
      break;
    case Mode::ShowHumidity:
      mode = Mode::ShowPressure;
      break;
    case Mode::ShowPressure:
      mode = Mode::ShowTime;
      break;
    }
  }
  if (digitalRead(CHANGE_MODE_BTN) == LOW && changeModeBtnFlag &&
      millis() - timingChangeModeButton > 100) {
    changeModeBtnFlag = false;
    timingChangeModeButton = millis();
  }
}

void controlFunctionalBtn() {
  using constants::FUNC_BTN;
  using enums::mode;
  using flags::funcBtnFlag;
  using flags::is24HFormat;
  using flags::isFarengeightFormat;
  using timings::timingFuncButton;

  if (digitalRead(FUNC_BTN) == HIGH && !funcBtnFlag &&
      millis() - timingFuncButton > 100) {
    Serial.println("FUNC");
    funcBtnFlag = true;
    timingFuncButton = millis();

    switch (mode) {
    case Mode::ShowTime:
      is24HFormat = !is24HFormat;
      break;
    case Mode::ShowDate:
      break;
    case Mode::ShowTemperature:
      isFarengeightFormat = !isFarengeightFormat;
      break;
    case Mode::ShowHumidity:
      break;
    case Mode::ShowPressure:
      break;
    }
  }
  if (digitalRead(FUNC_BTN) == LOW && funcBtnFlag &&
      millis() - timingFuncButton > 100) {
    funcBtnFlag = false;
    timingFuncButton = millis();
  }
}
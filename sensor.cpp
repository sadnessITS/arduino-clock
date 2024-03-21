#include "storage.h"
#include <Arduino.h>
#include <Encoder.h>

void configureSensors() {
  using namespace objects;
  using constants::CHANGE_MODE_BTN;

  // pinMode(8, INPUT_PULLUP);
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
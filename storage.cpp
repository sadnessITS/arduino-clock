#include "storage.h"
#include <Arduino.h>

namespace timings {
  unsigned long timingMode = 0;
  unsigned long timingBrightness = 0;
  unsigned long timingChangeModeButton = 0;
}

namespace brightness {
  int previous_encoder_brightness = 0;
  int brightness = 2;
}

namespace enums {
  Mode mode = Mode::ShowTime12H;
  EditedPart edited_part = EditedPart::Hours;
}

namespace objects {
  MatrixCascade<constants::CASCADE_SIZE> matrix_cascade(12, 10, 11);
  RTC_DS3231 rtc;
  Adafruit_BMP280 bmp;
  AHT20 aht20;
  Encoder encoder(4, 5);
}

namespace flags {
    int changeModeFlag = 0;
}
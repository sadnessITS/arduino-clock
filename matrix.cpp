#include "storage.h"
#include "interface.h"
#include "MatrixCascade.h"
#include "RTClib.h"
#include <Arduino.h>

void configureMatrix() {
  using objects::matrix_cascade;
  matrix_cascade.setRotation(2);
}

void showTime12H(DateTime time) {
  using namespace constants;
  using objects::matrix_cascade;
  uint8_t *symbol_f;
  int countDigitHours = 1;
  int countDigitMinutes = 1;
  if (time.twelveHour() >= 10)
    countDigitHours = 2;
  if (time.minute() >= 10)
    countDigitMinutes = 2;
  if (time.isPM())
    symbol_f = shiftToRightArray(symbols[SYMBOL_P], 5);
  else
    symbol_f = shiftToRightArray(symbols[SYMBOL_A], 5);

  if (countDigitHours == 1) {
    uint8_t *hour_d1 = shiftToRightArray(symbols[time.twelveHour()], 4);
    matrix_cascade[0].set(hour_d1);
    delete hour_d1;
  } else if (countDigitHours == 2) {
    uint8_t *hour_d1 = shiftToRightArray(symbols[SYMBOL_1], 1);
    uint8_t *hour_d2 = shiftToRightArray(symbols[time.twelveHour() % 10], 4);
    uint8_t *hour = mergeArray(hour_d1, hour_d2);
    delete hour_d1;
    delete hour_d2;
    matrix_cascade[0].set(hour);
    delete hour;
  }
  if (countDigitMinutes == 1) {
    uint8_t *min_d1 = shiftToRightArray(symbols[SYMBOL_0], 3);
    uint8_t *min_d1_colon = setColon(time, min_d1, 6);
    matrix_cascade[1].set(min_d1_colon);
    delete min_d1;
    delete min_d1_colon;
    uint8_t *min_d2 = shiftToRightArray(symbols[time.minute()], 0);
    uint8_t *matrix_2 = mergeArray(min_d2, symbol_f);
    delete min_d2;
    delete symbol_f;
    matrix_cascade[2].set(matrix_2);
    delete matrix_2;
  } else if (countDigitMinutes == 2) {
    int min_d1_index = time.minute() / 10;
    int shift_count = 3;
    if (min_d1_index == 1)
      shift_count = 5;
    uint8_t *min_d1 = shiftToRightArray(symbols[min_d1_index], shift_count);
    uint8_t *min_d1_colon = setColon(time, min_d1, 6);
    matrix_cascade[1].set(min_d1_colon);
    delete min_d1;
    delete min_d1_colon;
    uint8_t *min_d2 = shiftToRightArray(symbols[time.minute() % 10], 0);
    uint8_t *matrix_2 = mergeArray(min_d2, symbol_f);
    delete min_d2;
    delete symbol_f;
    matrix_cascade[2].set(matrix_2);
    delete matrix_2;
  }
  uint8_t *symbol_m = shiftToRightArray(symbols[SYMBOL_M], 1);
  matrix_cascade[3].set(symbol_m);
  delete symbol_m;
}

void showTime24H(DateTime time) {
  using namespace constants;
  using objects::matrix_cascade;
  int countDigitHours = 1;
  int countDigitMinutes = 1;
  if (time.hour() >= 10)
    countDigitHours = 2;
  if (time.minute() >= 10)
    countDigitMinutes = 2;

  if (countDigitHours == 1) {
    uint8_t *hour_d1 = shiftToRightArray(symbols[SYMBOL_0], 3);
    uint8_t *hour_d2 = shiftToRightArray(symbols[time.hour()], 0);
    uint8_t *hour_d2_colon = setColon(time, hour_d2, 0);
    matrix_cascade[0].set(hour_d1);
    matrix_cascade[1].set(hour_d2_colon);
    delete hour_d1;
    delete hour_d2;
    delete hour_d2_colon;
  } else if (countDigitHours == 2) {
    int hour_d1_index = time.hour() / 10;
    int shift_count = 3;
    if (hour_d1_index == 1)
      shift_count = 5;
    uint8_t *hour_d1 = shiftToRightArray(symbols[hour_d1_index], shift_count);
    uint8_t *hour_d2 = shiftToRightArray(symbols[time.hour() % 10], 0);
    uint8_t *hour_d2_colon = setColon(time, hour_d2, 0);
    matrix_cascade[0].set(hour_d1);
    matrix_cascade[1].set(hour_d2_colon);
    delete hour_d1;
    delete hour_d2;
    delete hour_d2_colon;
  }
  if (countDigitMinutes == 1) {
    uint8_t *min_d1 = shiftToRightArray(symbols[SYMBOL_0], 3);
    uint8_t *min_d2 = shiftToRightArray(symbols[time.minute()], 0);
    matrix_cascade[2].set(min_d1);
    matrix_cascade[3].set(min_d2);
    delete min_d1;
    delete min_d2;
  } else if (countDigitMinutes == 2) {
    int min_d1_index = time.minute() / 10;
    int shift_count = 3;
    if (min_d1_index == 1)
      shift_count = 5;
    uint8_t *min_d1 = shiftToRightArray(symbols[min_d1_index], shift_count);
    uint8_t *min_d2 = shiftToRightArray(symbols[time.minute() % 10], 0);
    matrix_cascade[2].set(min_d1);
    matrix_cascade[3].set(min_d2);
    delete min_d1;
    delete min_d2;
  }
}

uint8_t* setColon(DateTime time, uint8_t matrix[8], int column_number) {
  using objects::matrix_cascade;
  uint8_t *result = new uint8_t[8];
  for (int i = 0; i < 8; i++)
    result[i] = matrix[i];

  uint8_t mask = 1 << column_number;
  
  if (time.second() % 2 == 1) {
    result[2] = result[2] | mask;
    result[4] = result[4] | mask;
  }
  else {
    result[2] = result[2] & ~mask;
    result[4] = result[4] & ~mask;
  }
  
  return result;
}

void showSensorValue(int value, int unit) {
  using namespace constants;
  using objects::matrix_cascade;
  int shift = 0;
  int countDigit = 1;
  if (value >= 10 && value < 100)
    countDigit = 2;
  else if (value >= 100)
    countDigit = 3;

  switch (countDigit) {
  case 1: {
    uint8_t *digit_d1 = shiftToRightArray(symbols[SYMBOL_0], 2);
    matrix_cascade[1].set(digit_d1);
    delete digit_d1;
    uint8_t *digit_d2 = shiftToRightArray(symbols[value % 10], 0);
    matrix_cascade[2].set(digit_d2);
    delete digit_d2;
    
    uint8_t *clear_matrix = shiftToRightArray(symbols[SYMBOL_CLEAR], 0);
    matrix_cascade[0].set(clear_matrix);
    delete clear_matrix;
  } break;
  case 2: {
    int digit_1 = value / 10;
    if (digit_1 == 1)
      shift = 4;
    else
      shift = 2;
    uint8_t *digit_d1 = shiftToRightArray(symbols[digit_1], shift);
    matrix_cascade[1].set(digit_d1);
    delete digit_d1;

    uint8_t *digit_d2 = shiftToRightArray(symbols[value % 10], 0);
    matrix_cascade[2].set(digit_d2);
    delete digit_d2;

    uint8_t *clear_matrix = shiftToRightArray(symbols[SYMBOL_CLEAR], 0);
    matrix_cascade[0].set(clear_matrix);
    delete clear_matrix;
  } break;
  case 3: {
    int digit_1 = value / 100;
    if (digit_1 == 1)
      shift = 6;
    else
      shift = 4;
    uint8_t *digit_d1 = shiftToRightArray(symbols[digit_1], shift);
    matrix_cascade[0].set(digit_d1);
    delete digit_d1;

    int digit_2 = (value % 100) / 10;
    if (digit_2 == 1)
      shift = 4;
    else
      shift = 2;
    uint8_t *digit_d2 = shiftToRightArray(symbols[digit_2], shift);
    matrix_cascade[1].set(digit_d2);
    delete digit_d2;

    uint8_t *digit_d3 = shiftToRightArray(symbols[value % 10], 0);
    matrix_cascade[2].set(digit_d3);
    delete digit_d3;
  } break;
  }

  uint8_t *unit_symbol = shiftToRightArray(symbols[unit], 0);
  matrix_cascade[3].set(unit_symbol);
  delete unit_symbol;
}

void changeMode() {
  using enums::mode;

  switch (mode) {
  case Mode::ShowTime12H:
    mode = Mode::ShowTime24H;
    break;
  case Mode::ShowTime24H:
    mode = Mode::ShowTemperature;
    break;
  case Mode::ShowTemperature:
    mode = Mode::ShowHumidity;
    break;
  case Mode::ShowHumidity:
    mode = Mode::ShowPressure;
    break;
  case Mode::ShowPressure:
    mode = Mode::ShowTime12H;
    break;
  }
}

void editTime(DateTime time) {
  showTime24H(time);
  while (true) {
    
  }
}

void blinkDigit(int matrix_number, int shift_number, int digit) {
  // in progress
}
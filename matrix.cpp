#include "storage.h"
#include "interface.h"
#include "MatrixCascade.h"
#include "RTClib.h"
#include <Arduino.h>

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
    matrix_cascade[1].set(min_d1);
    delete min_d1;
    uint8_t *min_d2 = shiftToRightArray(symbols[time.minute()], 0);
    uint8_t *matrix_2 = mergeArray(min_d2, symbol_f);
    delete min_d2;
    delete symbol_f;
    matrix_cascade[2].set(matrix_2);
    delete matrix_2;
  } else if (countDigitMinutes == 2) {
    int min_d1_index = time.minute() / 10;
    int shift_count = 0;
    if (min_d1_index == 1)
      shift_count = 5;
    else
      shift_count = 3;
    uint8_t *min_d1 = shiftToRightArray(symbols[min_d1_index], shift_count);
    matrix_cascade[1].set(min_d1);
    delete min_d1;
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

void blinkColon(DateTime time, int matrix_number, int column_number) {
  using objects::matrix_cascade;
  uint8_t colInd = 0;
  for (auto &col : matrix_cascade[matrix_number].cols()) {
    if (colInd == column_number)
      if (time.second() % 2 == 1)
        matrix_cascade[matrix_number].set(col, {0b00101000});
      else
        matrix_cascade[matrix_number].set(col, {0b00000000});
    colInd++;
  }
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

void editTime(DateTime time) {
  // in progress
}

void blinkDigit(int matrix_number, int shift_number, int digit) {
  // in progress
}
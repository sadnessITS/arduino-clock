#include "interface.h"
#include "storage.h"
#include <Arduino.h>

uint8_t *copyArray(uint8_t arr[8]) {
  uint8_t *result = new uint8_t[8];
  for (int i = 0; i < 8; i++)
    result[i] = arr[i];
  return result;
}

void shiftToRightArray(uint8_t arr[8], int n) {
  for (int i = 0; i < 8; i++)
    arr[i] = arr[i] >> n;
}

uint8_t *mergeArray(uint8_t a1[8], uint8_t a2[8]) {
  uint8_t *result = new uint8_t[8];
  for (int i = 0; i < 8; i++)
    result[i] = a1[i] | a2[i];
  return result;
}

void setColon(DateTime time, uint8_t arr[8], int column_number, bool isBlink) {
  uint8_t mask = 1 << column_number;

  if (isBlink) {
    if (time.second() % 2 == 1) {
      arr[2] = arr[2] | mask;
      arr[4] = arr[4] | mask;
    } else {
      arr[2] = arr[2] & ~mask;
      arr[4] = arr[4] & ~mask;
    }
  } else {
    arr[2] = arr[2] | mask;
    arr[4] = arr[4] | mask;
  }
  return arr;
}
#include "storage.h"
#include <Arduino.h>

uint8_t *shiftToRightArray(uint8_t arr[8], int n) {
  uint8_t *result = new uint8_t[8];
  for (int i = 0; i < 8; i++)
    result[i] = arr[i] >> n;
  return result;
}

uint8_t *mergeArray(uint8_t a1[8], uint8_t a2[8]) {
  uint8_t *result = new uint8_t[8];
  for (int i = 0; i < 8; i++)
    result[i] = a1[i] | a2[i];
  return result;
}
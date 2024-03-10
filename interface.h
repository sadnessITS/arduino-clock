#pragma once

#ifndef INTERFACE_H
#define INTERFACE_H

#include <Arduino.h>
#include "RTClib.h"

//BLOCK: prototypes
int getCascadeBrightness();
DateTime getTime();
void showTime12H(DateTime time);
void blinkColon(DateTime time, int matrix_number, int column_number);
void showSensorValue(int value, int unit);
void editTime(DateTime time);
void blinkDigit(int matrix_number, int shift_number, int digit);
uint8_t* shiftToRightArray(uint8_t arr[8], int n);
uint8_t* mergeArray(uint8_t a1[8], uint8_t a2[8]);

#endif
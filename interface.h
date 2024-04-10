#pragma once

#ifndef INTERFACE_H
#define INTERFACE_H

#include <Arduino.h>
#include "RTClib.h"

//BLOCK: prototypes
void configureSensors();
DateTime getTime();
int getCascadeBrightness();
void controlModeBtn();
void controlFunctionalBtn();
void configureMatrix();
void showTime12H(DateTime time, bool isBlink);
void showTime24H(DateTime time, bool isBlink);
void showSensorValue(int value, int unit);
void showDate(DateTime time);
void editTime(DateTime time);
void blinkDigit(int matrix_number, int shift_number, int digit);
uint8_t* copyArray(uint8_t arr[8]);
void shiftToRightArray(uint8_t arr[8], int n);
uint8_t* mergeArray(uint8_t a1[8], uint8_t a2[8]);
void setColon(DateTime time, uint8_t arr[8], int column_number, bool isBlink);

#endif
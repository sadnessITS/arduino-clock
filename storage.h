#ifndef STORAGE_H
#define STORAGE_H

//BLOCK: imports
#include "MatrixCascade.h"
#include "RTClib.h"
#include <Encoder.h>

// BLOCK: symbols
#define SYMBOL_0 0
#define SYMBOL_1 1
#define SYMBOL_2 2
#define SYMBOL_3 3
#define SYMBOL_4 4
#define SYMBOL_5 5
#define SYMBOL_6 6
#define SYMBOL_7 7
#define SYMBOL_8 8
#define SYMBOL_9 9
#define SYMBOL_A 10
#define SYMBOL_P 11
#define SYMBOL_M 12


//BLOCK: enum
enum class Mode {ShowTime, EditTime, ShowTemperature, ShowHumidity, ShowPressure};

// BLOCK: const
#define CASCADE_SIZE 4

// BLOCK: variable
unsigned long timing;
int previous_encoder_value = 0;
int brightness = 8;
Mode mode = Mode::ShowTime;

// BLOCK: objects
MatrixCascade<CASCADE_SIZE> matrix_cascade(4, 8, 6);
RTC_DS3231 rtc;
Encoder brightness_encoder(10, 11);
extern const uint8_t symbols[13][8] = 
{
    {
        0b11110000,
        0b10010000,
        0b10010000,
        0b10010000,
        0b10010000,
        0b10010000,
        0b11110000,
        0b00000000
    },
    {
        0b01000000,
        0b11000000,
        0b01000000,
        0b01000000,
        0b01000000,
        0b01000000,
        0b01000000,
        0b00000000
    },
    {
        0b11110000,
        0b10010000,
        0b00010000,
        0b00100000,
        0b01000000,
        0b10000000,
        0b11110000,
        0b00000000
    },
    {
        0b11110000,
        0b10010000,
        0b00010000,
        0b01110000,
        0b00010000,
        0b10010000,
        0b11110000,
        0b00000000
    },
    {
        0b10010000,
        0b10010000,
        0b10010000,
        0b11110000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00000000
    },
    {
        0b11110000,
        0b10000000,
        0b10000000,
        0b11110000,
        0b00010000,
        0b10010000,
        0b11110000,
        0b00000000
    },
    {
        0b11110000,
        0b10010000,
        0b10000000,
        0b11110000,
        0b10010000,
        0b10010000,
        0b11110000,
        0b00000000
    },
    {
        0b11110000,
        0b10010000,
        0b00010000,
        0b00100000,
        0b01000000,
        0b10000000,
        0b10000000,
        0b00000000
    },
    {
        0b11110000,
        0b10010000,
        0b10010000,
        0b11110000,
        0b10010000,
        0b10010000,
        0b11110000,
        0b00000000
    },
    {
        0b11110000,
        0b10010000,
        0b10010000,
        0b11110000,
        0b00010000,
        0b10010000,
        0b11110000,
        0b00000000
    },
    {
        0b00000000,
        0b00000000,
        0b00000000,
        0b01000000,
        0b10100000,
        0b11100000,
        0b10100000,
        0b00000000
    },
    {
        0b00000000,
        0b00000000,
        0b00000000,
        0b11000000,
        0b10100000,
        0b11000000,
        0b10000000,
        0b00000000
    },
    {
        0b00000000,
        0b00000000,
        0b00000000,
        0b10001000,
        0b11011000,
        0b10101000,
        0b10001000,
        0b00000000
    }
};

#endif
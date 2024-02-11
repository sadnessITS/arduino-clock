#ifndef STORAGE_H
#define STORAGE_H

//BLOCK: imports
#include "MatrixCascade.h"
#include "RTClib.h"

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

// BLOCK: const
#define CASCADE_SIZE 4

// BLOCK: variable
unsigned long timing;

//BLOCK: enum
enum class Mode {ShowTime, EditTime};

// BLOCK: objects
MatrixCascade<CASCADE_SIZE> MATRIX_CASCADE(4, 8, 6);
RTC_DS3231 rtc;
extern const uint8_t SYMBOLS[13][8] = 
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
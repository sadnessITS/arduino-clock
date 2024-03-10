#pragma once

#ifndef STORAGE_H
#define STORAGE_H

//BLOCK: imports
#include "MatrixCascade.h"
#include <Encoder.h>
#include <Wire.h>
#include <AHT20.h>
#include <Adafruit_BMP280.h>
#include "RTClib.h"
#include <Arduino.h>

namespace constants {
    const int SYMBOL_0 = 0;
    const int SYMBOL_1 = 1;
    const int SYMBOL_2 = 2;
    const int SYMBOL_3 = 3;
    const int SYMBOL_4 = 4;
    const int SYMBOL_5 = 5;
    const int SYMBOL_6 = 6;
    const int SYMBOL_7 = 7;
    const int SYMBOL_8 = 8;
    const int SYMBOL_9 = 9;
    const int SYMBOL_A = 10;
    const int SYMBOL_P = 11;
    const int SYMBOL_M = 12;
    const int SYMBOL_CELSIUS = 13;
    const int SYMBOL_PERCENTAGE = 14;
    const int SYMBOL_HG = 15;

    const int CASCADE_SIZE = 4;

    const uint8_t symbols[16][8] = 
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
        },
        {
            0b01000000,
            0b10100100,
            0b01001010,
            0b00001000,
            0b00001000,
            0b00001010,
            0b00000100,
            0b00000000
        },
        {
            0b01000000,
            0b10100100,
            0b01001000,
            0b00010000,
            0b00100100,
            0b01001010,
            0b00000100,
            0b00000000
        },
        {
            0b00000000,
            0b00000000,
            0b10100000,
            0b10100100,
            0b11101010,
            0b10100110,
            0b10100010,
            0b00000100
        }
    };
}

enum Mode { ShowTime, EditTime, ShowTemperature, ShowHumidity, ShowPressure };
enum EditedPart { Hours, Minutes, HalfOfDay };

namespace timings {
    extern unsigned long timingMode;
    extern unsigned long timingBrightness;
}

namespace brightness {
    extern int previous_encoder_brightness;
    extern int brightness;
}

namespace enums {
    extern Mode mode;
    extern EditedPart edited_part;
}

namespace objects {
    extern MatrixCascade<constants::CASCADE_SIZE> matrix_cascade;
    extern RTC_DS3231 rtc;
    extern Adafruit_BMP280 bmp;
    extern AHT20 aht20;
    extern Encoder encoder;
}

#endif
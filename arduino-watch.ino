#include "storage.h"

void setup() 
{
    Serial.begin(57600);

    #ifndef ESP8266
        while (!Serial); // wait for serial port to connect. Needed for native USB
    #endif

    if (! rtc.begin()) {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (1) delay(10);
    }

    MATRIX_CASCADE.setIntensity(5);
    MATRIX_CASCADE.setRotation(2);
}

void loop() 
{
    DateTime now = getTime();
    
    if (millis() - timing > 1000)
    {
        timing = millis();
        setTime(now);
    }
}

uint8_t* shfitToRight(uint8_t arr[8], int n)
{
    uint8_t* result = new uint8_t[8];
    for (int i = 0; i < 8; i++) {
        result[i] = arr[i] >> n;
    }
    return result;
}

uint8_t* mergeArray(uint8_t a1[8], uint8_t a2[8])
{
    uint8_t* result = new uint8_t[8];

    for (int i = 0; i < 8; i++) {
        result[i] = a1[i] | a2[i];
    }
    return result;
}

DateTime getTime()
{
    return rtc.now();
}

void setTime(DateTime time)
{
    setHours(time);
    setMinutesAndM(time);
    setColon(time);
}

void setHours(DateTime time) 
{
    int count = 1;
    if (time.twelveHour() >= 10) count = 2;

    if (count == 1)
    {
        uint8_t* hour_d1 = shfitToRight(SYMBOLS[time.twelveHour()], 4);
        MATRIX_CASCADE[0].set(hour_d1);
        delete hour_d1;
    }
    else if (count == 2) 
    {
        uint8_t* hour_d1 = shfitToRight(SYMBOLS[SYMBOL_1], 1);
        uint8_t* hour_d2 = shfitToRight(SYMBOLS[time.twelveHour() % 10], 4);
        uint8_t* hour = mergeArray(hour_d1, hour_d2);
        delete hour_d1;
        delete hour_d2;
        MATRIX_CASCADE[0].set(hour);
        delete hour;
    }
}

void setMinutesAndM(DateTime time)
{
    uint8_t* symbol_f;
    if (time.isPM())
        symbol_f = shfitToRight(SYMBOLS[SYMBOL_P], 5);
    else 
        symbol_f = shfitToRight(SYMBOLS[SYMBOL_A], 5);

    int count = 1;
    if (time.minute() >= 10) count = 2;

    if (count == 1)
    {
        uint8_t* min_d1 = shfitToRight(SYMBOLS[SYMBOL_0], 3);
        MATRIX_CASCADE[1].set(min_d1);
        delete min_d1;
        uint8_t* min_d2 = shfitToRight(SYMBOLS[time.minute()], 0);
        uint8_t* matrix_2 = mergeArray(min_d2, symbol_f);
        delete min_d2;
        delete symbol_f;
        MATRIX_CASCADE[2].set(matrix_2);
        delete matrix_2;
    }
    else if (count == 2)
    {
        int min_d1_index = time.minute() / 10;
        int shift_count = 0;
        if (min_d1_index == 1)
            shift_count = 5;
        else
            shift_count = 3;
        uint8_t* min_d1 = shfitToRight(SYMBOLS[min_d1_index], shift_count);
        MATRIX_CASCADE[1].set(min_d1);
        delete min_d1;
        uint8_t* min_d2 = shfitToRight(SYMBOLS[time.minute() % 10], 0);
        uint8_t* matrix_2 = mergeArray(min_d2, symbol_f);
        delete min_d2;
        delete symbol_f;
        MATRIX_CASCADE[2].set(matrix_2);
        delete matrix_2;
    }
    uint8_t* symbol_m = shfitToRight(SYMBOLS[SYMBOL_M], 1);
    MATRIX_CASCADE[3].set(symbol_m);
    delete symbol_m;
}

void setColon(DateTime time)
{
    uint8_t colInd = 0;
    for (auto &col: MATRIX_CASCADE[1].cols())
    {
        if (colInd == 1)
            if (time.second() % 2 == 1)
                MATRIX_CASCADE[1].set(col, {0b00101000});
            else
                MATRIX_CASCADE[1].set(col, {0b00000000});
        colInd++;
    }
}
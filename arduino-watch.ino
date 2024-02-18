#include "storage.h"

void setup() 
{
    Wire.begin();
    Serial.begin(9600);

    #ifndef ESP8266
        while (!Serial); // wait for serial port to connect. Needed for native USB
    #endif

    if (! rtc.begin()) 
    {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (1) delay(10);
    }

    if (!bmp.begin()) {
        Serial.println("Couldn't find BMP280");
        while (1) delay(10);
    }

    if (!aht20.begin()) {
        Serial.println("Couldn't find AHT20");
        while (1);
    }

    matrix_cascade.setIntensity(5);
    matrix_cascade.setRotation(2);
}

void loop() 
{
    DateTime now = getTime();
    mode = Mode::ShowHumidity;
    
    matrix_cascade.setIntensity(getBrightness());
    Serial.println(getBrightness());
    
    switch(mode)
    {
        case Mode::ShowTime: 
            if (millis() - timingMode > 1000)
            {
                showTime(now);
                setColon(now);
                timingMode = millis();
            } break;
        case Mode::EditTime: showTime(now); break;
        case Mode::ShowTemperature: 
            if (millis() - timingMode > 100)
            {
                showSensorValue((int)aht20.getTemperature(), SYMBOL_CELSIUS);
                timingMode = millis();
            } break;
        case Mode::ShowHumidity:
            if (millis() - timingMode > 100)
            {
                showSensorValue((int)aht20.getHumidity(), SYMBOL_PERCENTAGE);
                timingMode = millis();
            } break;
        case Mode::ShowPressure: 
            if (millis() - timingMode > 100)
            {
                showSensorValue((int)(bmp.readPressure() * 0.0075006375541921), SYMBOL_HG);
                timingMode = millis();
            } break;
    }
}

uint8_t* shiftToRight(uint8_t arr[8], int n)
{
    uint8_t* result = new uint8_t[8];
    for (int i = 0; i < 8; i++)
        result[i] = arr[i] >> n;
    return result;
}

uint8_t* mergeArray(uint8_t a1[8], uint8_t a2[8])
{
    uint8_t* result = new uint8_t[8];
    for (int i = 0; i < 8; i++)
        result[i] = a1[i] | a2[i];
    return result;
}

DateTime getTime()
{
    return rtc.now();
}

void showTime(DateTime time) 
{
    uint8_t* symbol_f;
    int countDigitHours = 1;
    int countDigitMinutes = 1;
    if (time.twelveHour() >= 10) countDigitHours = 2;
    if (time.minute() >= 10) countDigitMinutes = 2;
    if (time.isPM())
        symbol_f = shiftToRight(symbols[SYMBOL_P], 5);
    else 
        symbol_f = shiftToRight(symbols[SYMBOL_A], 5);

    if (countDigitHours == 1)
    {
        uint8_t* hour_d1 = shiftToRight(symbols[time.twelveHour()], 4);
        matrix_cascade[0].set(hour_d1);
        delete hour_d1;
    }
    else if (countDigitHours == 2) 
    {
        uint8_t* hour_d1 = shiftToRight(symbols[SYMBOL_1], 1);
        uint8_t* hour_d2 = shiftToRight(symbols[time.twelveHour() % 10], 4);
        uint8_t* hour = mergeArray(hour_d1, hour_d2);
        delete hour_d1;
        delete hour_d2;
        matrix_cascade[0].set(hour);
        delete hour;
    }
    if (countDigitMinutes == 1)
    {
        uint8_t* min_d1 = shiftToRight(symbols[SYMBOL_0], 3);
        matrix_cascade[1].set(min_d1);
        delete min_d1;
        uint8_t* min_d2 = shiftToRight(symbols[time.minute()], 0);
        uint8_t* matrix_2 = mergeArray(min_d2, symbol_f);
        delete min_d2;
        delete symbol_f;
        matrix_cascade[2].set(matrix_2);
        delete matrix_2;
    }
    else if (countDigitMinutes == 2)
    {
        int min_d1_index = time.minute() / 10;
        int shift_count = 0;
        if (min_d1_index == 1)
            shift_count = 5;
        else
            shift_count = 3;
        uint8_t* min_d1 = shiftToRight(symbols[min_d1_index], shift_count);
        matrix_cascade[1].set(min_d1);
        delete min_d1;
        uint8_t* min_d2 = shiftToRight(symbols[time.minute() % 10], 0);
        uint8_t* matrix_2 = mergeArray(min_d2, symbol_f);
        delete min_d2;
        delete symbol_f;
        matrix_cascade[2].set(matrix_2);
        delete matrix_2;
    }
    uint8_t* symbol_m = shiftToRight(symbols[SYMBOL_M], 1);
    matrix_cascade[3].set(symbol_m);
    delete symbol_m;
}

void setColon(DateTime time)
{
    uint8_t colInd = 0;
    for (auto &col: matrix_cascade[1].cols())
    {
        if (colInd == 1)
            if (time.second() % 2 == 1)
                matrix_cascade[1].set(col, {0b00101000});
            else
                matrix_cascade[1].set(col, {0b00000000});
        colInd++;
    }
}

int getBrightness()
{
    int current_encoder_value = brightness_encoder.read();
    if (current_encoder_value > previous_encoder_value + 4)
    {
        brightness += 1;
        previous_encoder_value = current_encoder_value;
    }
    else if (current_encoder_value < previous_encoder_value - 4)
    {
        brightness -= 1;
        previous_encoder_value = current_encoder_value;
    }
    brightness = constrain(brightness, 1, 15);
    return brightness;
}

void showSensorValue(int value, int unit)
{
    int shift = 0;
    int countDigit = 1;
    if (value >= 10 && value < 100) countDigit = 2;
    else if (value >= 100) countDigit = 3;

    switch (countDigit)
    {
        case 1:
        {
            uint8_t* digit_d1 = shiftToRight(symbols[SYMBOL_0], 2);
            matrix_cascade[1].set(digit_d1);
            delete digit_d1;
            uint8_t* digit_d2 = shiftToRight(symbols[value % 10], 0);
            matrix_cascade[2].set(digit_d2);
            delete digit_d2;
        } break;
        case 2:
        { 
            int digit_1 = value / 10;
            if (digit_1 == 1)
                shift = 4;
            else
                shift = 2;
            uint8_t* digit_d1 = shiftToRight(symbols[digit_1], shift);
            matrix_cascade[1].set(digit_d1);
            delete digit_d1;
            
            uint8_t* digit_d2 = shiftToRight(symbols[value % 10], 0);
            matrix_cascade[2].set(digit_d2);
            delete digit_d2;
        } break;
        case 3:
        {
            int digit_1 = value / 100;
            if (digit_1 == 1)
                shift = 6;
            else
                shift = 4;
            uint8_t* digit_d1 = shiftToRight(symbols[digit_1], shift);
            matrix_cascade[0].set(digit_d1);
            delete digit_d1;

            int digit_2 = (value % 100) / 10;
            if (digit_2 == 1)
                shift = 4;
            else
                shift = 2;
            uint8_t* digit_d2 = shiftToRight(symbols[digit_2], shift);
            matrix_cascade[1].set(digit_d2);
            delete digit_d2;

            uint8_t* digit_d3 = shiftToRight(symbols[value % 10], 0);
            matrix_cascade[2].set(digit_d3);
            delete digit_d3;
        } break;
    }

    uint8_t* unit_symbol = shiftToRight(symbols[unit], 0);
    matrix_cascade[3].set(unit_symbol);
    delete unit_symbol;
}

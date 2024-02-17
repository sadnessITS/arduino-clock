#include "storage.h"

void setup() 
{
    Serial.begin(57600);
    sensor.begin();

    #ifndef ESP8266
        while (!Serial); // wait for serial port to connect. Needed for native USB
    #endif

    if (! rtc.begin()) 
    {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (1) delay(10);
    }

    matrix_cascade.setIntensity(5);
    matrix_cascade.setRotation(2);
}

void loop() 
{
    DateTime now = getTime();
    sensors_event_t humiditySensor, temperatureSensor;
    sensor.getEvent(&humiditySensor, &temperatureSensor);
    
    matrix_cascade.setIntensity(getBrightness());

    mode = Mode::ShowHumidity;
    
    switch(mode)
    {
        case Mode::ShowTime: 
            if (millis() - timing > 1000)
            {
                showTime(now);
                setColon(now);
                timing = millis();
            } break;
        case Mode::EditTime: showTime(now); break;
        case Mode::ShowTemperature: showTemperature(temperatureSensor); break;
        case Mode::ShowHumidity: showHumidity(humiditySensor); break;
        case Mode::ShowPressure: showTime(now); break;
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
    int current_encoder_value = brightness_encoder.read() / 4;
    if (current_encoder_value > previous_encoder_value)
        brightness += 1;
    else if (current_encoder_value < previous_encoder_value)
        brightness -= 1;
    brightness = constrain(brightness, 1, 15);
    previous_encoder_value = current_encoder_value;
    return brightness;
}

void showTemperature(sensors_event_t temperatureSensor)
{
    int countDigitTemperature = 1;
    if ((int)temperatureSensor.temperature >= 10) countDigitTemperature = 2;

    if (countDigitTemperature == 1)
    {
        uint8_t* temperature_d1 = shiftToRight(symbols[SYMBOL_0], 4);
        matrix_cascade[0].set(temperature_d1);
        delete temperature_d1;
        uint8_t* temperature_d2 = shiftToRight(symbols[(int)temperatureSensor.temperature], 1);
        matrix_cascade[1].set(temperature_d2);
        delete temperature_d2;
    }
    else
    {
        uint8_t* temperature_d1 = shiftToRight(symbols[(int)temperatureSensor.temperature / 10], 4);
        matrix_cascade[0].set(temperature_d1);
        delete temperature_d1;
        uint8_t* temperature_d2 = shiftToRight(symbols[(int)temperatureSensor.temperature % 10], 1);
        matrix_cascade[1].set(temperature_d2);
        delete temperature_d2;
    }
    uint8_t* celsius = shiftToRight(symbols[SYMBOL_CELSIUS], 0);
    matrix_cascade[2].set(celsius);
    delete celsius;
}

void showHumidity(sensors_event_t humiditySensor)
{
    int countDigitHumidity = 1;
    if ((int)humiditySensor.relative_humidity >= 10) countDigitHumidity = 2;
    if ((int)humiditySensor.relative_humidity == 100) countDigitHumidity = 3;

    if (countDigitHumidity == 1)
    {
        uint8_t* digit_d1 = shiftToRight(symbols[(int)humiditySensor.relative_humidity], 1);
        matrix_cascade[1].set(digit_d1);
        delete digit_d1;
    }
    else if (countDigitHumidity == 2)
    {
        uint8_t* digit_d1 = shiftToRight(symbols[(int)humiditySensor.relative_humidity / 10], 4);
        matrix_cascade[0].set(digit_d1);
        delete digit_d1;
        uint8_t* digit_d2 = shiftToRight(symbols[(int)humiditySensor.relative_humidity % 10], 1);
        matrix_cascade[1].set(digit_d2);
        delete digit_d2;
    }
    else if (countDigitHumidity == 3)
    {
        uint8_t* digit_d1 = shiftToRight(symbols[SYMBOL_1], 1);
        uint8_t* digit_d2 = shiftToRight(symbols[(int)humiditySensor.relative_humidity / 10], 4);
        uint8_t* merged_d1_d2 = mergeArray(digit_d1, digit_d2);
        delete digit_d1;
        delete digit_d2;
        matrix_cascade[0].set(merged_d1_d2);
        uint8_t* digit_d3 = shiftToRight(symbols[(int)humiditySensor.relative_humidity % 10], 1);
        matrix_cascade[1].set(digit_d3);
        delete digit_d3;
    }
    uint8_t* percentage = shiftToRight(symbols[SYMBOL_PERCENTAGE], 0);
    matrix_cascade[2].set(percentage);
    delete percentage;
}
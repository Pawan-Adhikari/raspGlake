#pragma once
#include <Arduino.h>
const float RAW_MAX_DRY = 1005.00; 
const float RAW_MIN_WET = 10.00;

#pragma pack(push, 1)
struct HumidityData{
    bool isDry = true;
    float moisture = -1.0; 
    void display() const;
};
#pragma pack(pop)

HumidityData getHumidity(int aPin = 26, int dPin = 22, bool scale = false);
float getOutTemp(int aPin = 27);
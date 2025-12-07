#pragma once
#include <Arduino.h>
const float RAW_MAX_DRY = 1005.00; 
const float RAW_MIN_WET = 10.00;

struct HumidityData{
    bool isDry = true;
    float moisture = -1.0; 
    void display() const;
};

HumidityData getHumidity(int aPin = 26, int dPin = 22, bool scale = false);
#pragma once
#include <Arduino.h>
const float RAW_MAX_DRY = 6436.9305; 
const float RAW_MIN_WET = 4929.9705;

struct HumidityData{
    bool isDry = true;
    float moisture = -1.0; 
    void display() const;
};

HumidityData getHumidity(int aPin = 26, int dPin = 22, bool scale = false);
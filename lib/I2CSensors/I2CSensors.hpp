#pragma once
#include "Globals.hpp"

struct BMPdata {
    float temperature = -1;
    float pressure = -1;
    float altitude = -1; 
    void display() const;
};

struct IMUData {
    float accelX = -1.0;
    float accelY = -1.0;
    float accelZ = -1.0;
    
    float gyroX = 0.0;
    float gyroY = 0.0;
    float gyroZ = 0.0;

    void display() const ;
};

bool beginBMP(uint8_t address = 0x76);
bool beginMPU(TwoWire *myWire = &Wire, uint8_t address = 0x68);
BMPdata measBMP();
IMUData readMPU();
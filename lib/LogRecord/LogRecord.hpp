#pragma once
#include "I2CSensors.hpp"
#include "HumidityS.hpp"

struct LogRecord {
    unsigned long timestamp_ms; 
    IMUData imu; 
    BMPdata bmp; 
    HumidityData hum;
public:
    LogRecord(){}
    LogRecord(unsigned long t, IMUData i, BMPdata b, HumidityData h);
    int toCSV(char* buffer, const size_t bufferSize) const;
    void display() const;
};

#pragma pack(push, 1)  // No padding
struct PackedRecord {
    uint32_t timestamp_ms;

    // IMUData
    float accelX;
    float accelY;
    float accelZ;
    float gyroX;
    float gyroY;
    float gyroZ;

    // BMPdata
    float temperature;
    float pressure;
    float altitude;

    // HumidityData
    uint8_t isDry;      // bool stored as 1 byte
    float moisture;
};
#pragma pack(pop)

PackedRecord packRecord(const LogRecord &r);
void sendRecord(const PackedRecord &r);


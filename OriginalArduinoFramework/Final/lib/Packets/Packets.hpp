#pragma once
#include "I2CSensors.hpp"
#include "HumidityS.hpp"

/*
struct LogRecord {
    unsigned long timestamp_ms; 
    IMUData imu; 
    BMPdata bmp; 
    HumidityData hum;
public:
    LogRecord(){}
    LogRecord(unsigned long t, IMUData i, BMPdata b, HumidityData h);
    //void display() const;
};
*/

//Defining packets to store and transmit data

//Includes other sensor's data to be sent at lower frequency
#pragma pack(push, 1) 
struct OtherSensorsPacket {
    //Header
    uint8_t sync = 0x55;

    //Payload
    uint32_t timestamp_ms;
    // BMPdata
    BMPdata bmp;
    // HumidityData
    HumidityData hum;

    //Checksum
    uint8_t checksum;

    void display(){
        bmp.display();
        hum.display();
    }
};
#pragma pack(pop)

//Separate imu packet for higher sampling rate
#pragma pack(push, 1) 
struct IMUPacket {
    //Header
    uint8_t sync = 0xAA;

    //Payload
    uint32_t timestamp_ms;
    // IMUData, using aggregate type, Fine right now since we dont have padding in IMUData, only floats.
    IMUData imu;

    //Checksum
    uint8_t checksum;



};
#pragma pack(pop)


//Functions to handle checksum
unsigned char getChecksum(const uint8_t *data, int sz);

template <class T>
void setChecksum(T *pkt){
    pkt->checksum = getChecksum((uint8_t *)pkt, sizeof(T)-1);
}



//packet packRecord(const LogRecord &r);



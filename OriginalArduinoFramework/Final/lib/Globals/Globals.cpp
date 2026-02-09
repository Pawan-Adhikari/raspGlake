#define __FREERTOS 1
#include "Globals.hpp"

int thermoCLK = 10;
int thermoDO = 12;
int thermoCS = 13;
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

Adafruit_MPU6050 mpu;
Adafruit_BMP280 bmp;

QueueHandle_t IMUQueue;
QueueHandle_t OtherSensorQueue;
StreamBufferHandle_t xIMUStream;
SemaphoreHandle_t xSerialMutex;
SemaphoreHandle_t xI2CMutex;

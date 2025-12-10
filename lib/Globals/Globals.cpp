#include "Globals.hpp"

Adafruit_MPU6050 mpu;
Adafruit_BMP280 bmp;

QueueHandle_t IMUQueue;
QueueHandle_t OtherSensorQueue;


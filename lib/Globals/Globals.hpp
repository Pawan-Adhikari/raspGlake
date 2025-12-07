#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <stdint.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_BMP280.h>
#include <FreeRTOS.h>

extern Adafruit_MPU6050 mpu;
extern Adafruit_BMP280 bmp;

extern QueueHandle_t sensorQueue;

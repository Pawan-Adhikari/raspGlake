#define __FREERTOS 1
#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <stdint.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_BMP280.h>
#include <max6675.h>
#include <FreeRTOS.h>
#include <stream_buffer.h>

#define DATA_READY_PIN 20 

extern Adafruit_MPU6050 mpu;
extern Adafruit_BMP280 bmp;
extern MAX6675 thermocouple;

extern QueueHandle_t IMUQueue;
extern QueueHandle_t OtherSensorQueue;
extern StreamBufferHandle_t xIMUStream;
extern SemaphoreHandle_t xSerialMutex;
extern SemaphoreHandle_t xI2CMutex;


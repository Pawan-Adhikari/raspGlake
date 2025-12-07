#pragma once
#include "Globals.hpp"
#include "I2CSensors.hpp"
#include "HumidityS.hpp"
#include "LogRecord.hpp"
#define RX2_PIN 15
#define TX2_PIN 14

void vSensorThread(void* parameter);
void vSerialOutThread(void* parameter);
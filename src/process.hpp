#pragma once
#include "Globals.hpp"
#include "I2CSensors.hpp"
#include "HumidityS.hpp"
#include "Packets.hpp"

void vOtherSensorsThread(void *parameter);
void vIMUThread(void *parameter);
void vIMUSerialOutThread(void *parameter);
void vOtherSensorsSerialOutThread(void *parameter);
void vBlinkTask(void *params);
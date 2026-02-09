#define __FREERTOS 1
#include "process.hpp"

void vOtherSensorsThread(void *parameter){
    OtherSensorsPacket opkt;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = 10000; 
    while (1){
        vTaskDelayUntil(&xLastWakeTime, xFrequency);

        xSemaphoreTake(xI2CMutex, portMAX_DELAY);
        opkt.timestamp_ms = millis();
        opkt.bmp = measBMP(0x76);
        opkt.hum = getHumidity(26, 22, false);
        xSemaphoreGive(xI2CMutex);

        setChecksum<OtherSensorsPacket>(&opkt);

        //xSemaphoreTake(xSerialMutex, portMAX_DELAY);
        //opkt.bmp.display();
        //xSemaphoreGive(xSerialMutex);

        xQueueSend(OtherSensorQueue, &opkt, 1);
    } 
}

void vIMUThread(void *parameter){
    IMUPacket ipkt;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = 100; 
    while (1){
        vTaskDelayUntil(&xLastWakeTime, xFrequency);

        xSemaphoreTake(xI2CMutex, portMAX_DELAY);
        ipkt.timestamp_ms = millis();
        ipkt.imu = readMPU(&Wire, 0x68);
        xSemaphoreGive(xI2CMutex);

        setChecksum<IMUPacket>(&ipkt);

        //xSemaphoreTake(xSerialMutex, portMAX_DELAY);
        //ipkt.imu.display();
        //xSemaphoreGive(xSerialMutex);

        xStreamBufferSend(xIMUStream, &ipkt, sizeof(IMUPacket), 1);
    }
}

void vIMUSerialOutThread(void *parameter) {
  char buffer[4000];
  size_t bytes_read;
  for(;;) { 
    bytes_read = xStreamBufferReceive(xIMUStream, buffer, sizeof(buffer), portMAX_DELAY);
    if (bytes_read > 0) {
      //xSemaphoreTake(xSerialMutex, portMAX_DELAY);
      Serial.write((uint8_t*)buffer, sizeof(buffer));
      //xSemaphoreGive(xSerialMutex);
    }
  }
}

void vOtherSensorsSerialOutThread(void *parameter) {
  OtherSensorsPacket opkt;
  for(;;) {
    if (xQueueReceive(OtherSensorQueue, &opkt, portMAX_DELAY) == pdTRUE) {
      //xSemaphoreTake(xSerialMutex, portMAX_DELAY);
      Serial.write((uint8_t*)&opkt, sizeof(OtherSensorsPacket));
      //xSemaphoreGive(xSerialMutex);
    }
  }
}

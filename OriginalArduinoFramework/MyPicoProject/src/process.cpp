#define __FREERTOS 1
#include "process.hpp"

void vOtherSensorsThread(void *parameter){
    OtherSensorsPacket opkt;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = 10000; 
    while (1){
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        opkt.timestamp_ms = millis();
        opkt.bmp = measBMP();
        opkt.hum = getHumidity(26, 22, true);
        setChecksum<OtherSensorsPacket>(&opkt);
        xQueueSend(OtherSensorQueue, &opkt, 1);
    }
}

void vIMUThread(void *parameter){
    IMUPacket ipkt;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = 20; 
    while (1){
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        ipkt.timestamp_ms = millis();
        ipkt.imu = readMPU();
        setChecksum<IMUPacket>(&ipkt);
        //ipkt.imu.display();
        xStreamBufferSend(xIMUStream, &ipkt, sizeof(IMUPacket), 1);
    }
}

void vIMUSerialOutThread(void *parameter) {
  char buffer[20000];
  size_t bytes_read;
  for(;;) { 
    bytes_read = xStreamBufferReceive(xIMUStream, buffer, sizeof(buffer), portMAX_DELAY);
    if (bytes_read > 0) {
        Serial.write((uint8_t*)buffer, sizeof(buffer));
    }
  }
}

void vOtherSensorsSerialOutThread(void *parameter) {
  OtherSensorsPacket opkt;
  for(;;) {
    if (xQueueReceive(OtherSensorQueue, &opkt, portMAX_DELAY) == pdTRUE) {
        Serial.write((uint8_t*)&opkt, sizeof(OtherSensorsPacket));
    }
  }
}

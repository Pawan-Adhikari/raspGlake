#include "process.hpp"

/*
void vSensorThread(void *parameter){
    uint16_t bmp_count = 0;
    uint16_t hum_count = 0;
    LogRecord currentRecord;

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = 20; //2ms = 500hz

    while (1){
        vTaskDelayUntil(&xLastWakeTime, xFrequency);

        currentRecord.timestamp_ms = millis();
        currentRecord.imu = readMPU();
        bmp_count++;
        hum_count++;

        if (bmp_count >= 50){
            currentRecord.bmp = measBMP();
            bmp_count = 0;
        }
        if (hum_count >= 500){
            currentRecord.hum = getHumidity(26, 22, true);
            hum_count = 0;
        }     
        
        xQueueSend(sensorQueue, &currentRecord, portMAX_DELAY);
    }
}
*/

void vOtherSensorsThread(void *parameter){

    OtherSensorsPacket opkt;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = 60000; //60000ms = 60s

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
    const TickType_t xFrequency = 10; //2ms = 500hz

    while (1){
        vTaskDelayUntil(&xLastWakeTime, xFrequency);

        ipkt.timestamp_ms = millis();
        ipkt.imu = readMPU();
        setChecksum<IMUPacket>(&ipkt);

        xQueueSend(IMUQueue, &ipkt, 1);
    }
}

/*
void vSerialOutThread(void *parameter) {
  Serial.println("vSerialOutThread: STARTED");
  LogRecord record;
  uint16_t BufCount = 0;
  static packet packetBuf[400];

  for(;;) {
    if (xQueueReceive(sensorQueue, &record, portMAX_DELAY) == pdTRUE) {
        packet pkt = packRecord(record);
        packetBuf[BufCount++] = pkt;
    }

    // check buffer full
    if (BufCount >= 400) {
        digitalWrite(DATA_READY_PIN, HIGH);
        Serial.println("Sending packets");
        //vTaskDelay(1000);
        SPI.transfer((uint8_t*)packetBuf, NULL, BufCount * sizeof(packet));
        digitalWrite(DATA_READY_PIN, LOW);
        BufCount = 0;
        Serial.println("Sent packets.");
    }
  }
}
*/

void vIMUSerialOutThread(void *parameter) {
  //Serial.println("vIMUSerialOutThread: STARTED");
  IMUPacket ipkt;

  for(;;) {
    if (xQueueReceive(IMUQueue, &ipkt, portMAX_DELAY) == pdTRUE) {
        //Serial.println("Received an IMUSensorPacket!");
        Serial.write((uint8_t*)&ipkt, sizeof(IMUPacket));
    }
  }
}

void vOtherSensorsSerialOutThread(void *parameter) {
  //Serial.println("vOtherSensorsSerialOutThread: STARTED");
  OtherSensorPacket opkt;

  for(;;) {
    if (xQueueReceive(OtherSensorQueue, &opkt, portMAX_DELAY) == pdTRUE) {
        //Serial.println("Received an OtherSensorPacket!");
        Serial.write((uint8_t*)&opkt, sizeof(OtherSensorPacket));
    }

  }
}

void vBlinkTask(void *params){
    for(;;){
        digitalWrite(LED_BUILTIN, HIGH);
        vTaskDelay(2000/portTICK_PERIOD_MS);
        digitalWrite(LED_BUILTIN, LOW);      
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}




//void vlistenCommandsFromESP(){
    //this thread will listen to commands in Serial2 sent by ESP in format "$COMMAND\0\n". High priority.
//}
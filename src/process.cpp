#include "process.hpp"

void vSensorThread(void *parameter){
    uint16_t bmp_count = 0;
    uint16_t hum_count = 0;
    LogRecord currentRecord;

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = 2; //2ms = 500hz

    while (1){
        vTaskDelayUntil(&xLastWakeTime, xFrequency);

        currentRecord.timestamp_ms = millis();
        currentRecord.imu = readMPU();
        bmp_count++;
        hum_count++;

        if (bmp_count >= 500){
            currentRecord.bmp = measBMP();
            bmp_count = 0;
        }
        if (hum_count >= 2500){
            currentRecord.hum = getHumidity(26, 22, true);
            hum_count = 0;
        }     
        
        xQueueSend(sensorQueue, &currentRecord, portMAX_DELAY);
    }
}

void vSerialOutThread(void *parameter) {
  Serial.println("vSerialOutThread: STARTED");
  LogRecord record;

  for(;;) {
    if (xQueueReceive(sensorQueue, &record, portMAX_DELAY)==pdTRUE) {
        //PackedRecord pkt = packRecord(record);
        //sendRecord(pkt);
        Serial.print(record.timestamp_ms);
        Serial.println("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"); // ~33 chars + timestamp = ~40 bytes
        //record.display();
    }
  }
}

//void vlistenCommandsFromESP(){
    //this thread will listen to commands in Serial2 sent by ESP in format "$COMMAND\0\n". High priority.
//}
#include "Globals.hpp"
#include "LogRecord.hpp"
#include "process.hpp"
TaskHandle_t sensorTaskHandle, serialOutTaskHandle;

void setup(){
  //Serial Stuff
  Serial.begin(115200);
  //while(!Serial);
  Serial.println("Started Serial!");

  //Sensors initialisation
  Wire.begin();
  Wire.setClock(400000); //400khz I2C bus speed for High frequency Logging
  beginBMP();
  beginMPU();

  //Queue Setup
  sensorQueue = xQueueCreate(256, sizeof(LogRecord));

  //Task Setup
  xTaskCreate(
    vSensorThread,
    "SensorTask",
    7000,
    NULL,
    2,
    &sensorTaskHandle
  );
  xTaskCreate(
    vSerialOutThread,
    "SerialOutTask",
    7000,
    NULL,
    2,
    &serialOutTaskHandle
  );

  //Task Core setup, 1 = Core0, 2 = Core1 and 3 = Both
  vTaskCoreAffinitySet(sensorTaskHandle, 1); 
  vTaskCoreAffinitySet(sensorTaskHandle, 2); 
}

void loop(){
  vTaskDelete(NULL);
}
#define __FREERTOS 1
#include "Globals.hpp"
#include "Packets.hpp"
#include "process.hpp"

TaskHandle_t OtherSensorsTaskHandle, OtherSensorSerialOutTaskHandle, IMUTaskHandle, IMUSerialOutTaskHandle;

void setup(){
  //Board status
  pinMode(LED_BUILTIN, OUTPUT);
  //Serial Stuff
  Serial.begin(115200);
  delay(1000);

  //Sensors initialisation
  Wire.begin();
  Wire.setClock(400000); //400khz I2C bus speed for High frequency Logging
  beginBMP();
  beginMPU();

  //Queue Setup
  //IMUQueue = xQueueCreate(1024, sizeof(IMUPacket));
  OtherSensorQueue = xQueueCreate(32, sizeof(OtherSensorsPacket));
  xIMUStream = xStreamBufferCreate(24000, 20000);


  //Task Setup

  xTaskCreate(
    vIMUThread,
    "IMUTask",
    2048,
    NULL,
    2,
    &IMUTaskHandle
  );

  xTaskCreate(
    vOtherSensorsThread,
    "OtherSensorTask",
    1024,
    NULL,
    2,
    &OtherSensorsTaskHandle
  );

  xTaskCreate(
    vIMUSerialOutThread,
    "vIMUSerialOutTask",
    5256,
    NULL,
    2,
    &IMUSerialOutTaskHandle
  );

  xTaskCreate(
    vOtherSensorsSerialOutThread,
    "OtherSensorSerialOutTask",
    1024,
    NULL,
    2,
    &OtherSensorSerialOutTaskHandle
  );

  //Task Core setup, 1 = Core0, 2 = Core1 and 3 = Both 
  vTaskCoreAffinitySet(IMUTaskHandle, 2); 
  vTaskCoreAffinitySet(OtherSensorsTaskHandle, 3); 
  vTaskCoreAffinitySet(IMUSerialOutTaskHandle, 2); 
  vTaskCoreAffinitySet(OtherSensorSerialOutTaskHandle, 3); 

}

void loop(){
  vTaskDelete(NULL);
}
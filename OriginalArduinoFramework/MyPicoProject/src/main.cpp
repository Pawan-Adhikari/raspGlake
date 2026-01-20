#define __FREERTOS 1
#include "Globals.hpp"
#include "Packets.hpp"
#include "process.hpp"

TaskHandle_t OtherSensorsTaskHandle, OtherSensorSerialOutTaskHandle, IMUTaskHandle, IMUSerialOutTaskHandle;

void setup(){
  //Serial Stuff
  Serial.begin(115200);
  delay(1000);

  //Sensors initialisation
  Wire.setSDA(20); // Using GP16
  Wire.setSCL(21);
  Wire.begin(); 
  beginBMP();
  beginMPU();

  //Queue Setup
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
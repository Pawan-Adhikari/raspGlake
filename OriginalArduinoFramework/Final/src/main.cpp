#define __FREERTOS 1
#include "Globals.hpp"
#include "Packets.hpp"
#include "process.hpp"

TaskHandle_t OtherSensorsTaskHandle, OtherSensorSerialOutTaskHandle, IMUTaskHandle, IMUSerialOutTaskHandle;
//Adafruit_BMP280 bmp;
//Adafruit_MPU6050 mpu;

void setup(){
  //Serial Stuff
  Serial.begin(115200);
  delay(3000);

  //Sensors initialisation
  Wire.setSDA(16); // Using GP16
  Wire.setSCL(17);
  analogReadResolution(12);

  Wire.begin(); 
  beginBMP(0x76);
  beginMPU(&Wire, 0x68);

  //Queue Setup
  OtherSensorQueue = xQueueCreate(32, sizeof(OtherSensorsPacket));
  xIMUStream = xStreamBufferCreate(4000, 3000);
  xSerialMutex = xSemaphoreCreateMutex();
  xI2CMutex = xSemaphoreCreateMutex();


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
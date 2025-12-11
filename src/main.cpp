#include "Globals.hpp"
#include "Packets.hpp"
#include "process.hpp"

TaskHandle_t OtherSensorsTaskHandle, OtherSensorSerialOutTaskHandle, IMUTaskHandle, IMUSerialOutTaskHandle;

void setup(){
  //Board status
  pinMode(LED_BUILTIN, OUTPUT);
  //Serial Stuff
  Serial.begin(115200);
  //Serial1.begin(115200);
  delay(1000);
  //Serial.println("Started Serials!");
  /*
  SPI.setRX(16);
  SPI.setCS(17);
  SPI.setSCK(18);
  SPI.setTX(19);
  SPI.begin(true); // true = Slave mode
  */
  //pinMode(DATA_READY_PIN, OUTPUT);
  //digitalWrite(DATA_READY_PIN, LOW);

  //Sensors initialisation
  Wire.begin();
  Wire.setClock(400000); //400khz I2C bus speed for High frequency Logging
  beginBMP();
  beginMPU();

  //Queue Setup
  IMUQueue = xQueueCreate(512, sizeof(IMUPacket));
  OtherSensorQueue = xQueueCreate(128, sizeof(OtherSensorsPacket));


  //Task Setup

  /*
  xTaskCreate(
    vSensorThread,
    "SensorTask",
    2048,
    NULL,
    2,
    &sensorTaskHandle
  );
  */

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
    2048,
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

  xTaskCreate(
    vBlinkTask,
    "Blinker",
    256,
    NULL,
    1,
    NULL
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
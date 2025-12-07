#include "Globals.hpp"
#include "I2CSensors.hpp"
#include "HumidityS.hpp"
#include "LogRecord.hpp"

// Define Humidity Sensor Pins (Adjust if you wired differently)
#define HUMIDITY_ANALOG_PIN 26
#define HUMIDITY_DIGITAL_PIN 22

void setup() {
  Serial.begin(115200);
  delay(3000); // Wait for Serial to be ready
  Serial.println("\n--- RaspIGLake Full System Test ---");

  // 1. Initialize I2C (GP4 SDA, GP5 SCL)
  Wire.begin();

  // 2. Initialize BMP280
  Serial.print("Initializing BMP280... ");
  if (beginBMP()) {
    Serial.println("OK!");
  } else {
    Serial.println("FAILED! Check wiring.");
  }

  // 3. Initialize MPU6050
  Serial.print("Initializing MPU6050... ");
  if (beginMPU()) { 
    Serial.println("OK!");
  } else {
    Serial.println("FAILED! Check wiring.");
  }

  // 4. Initialize Humidity Sensor Pins
  pinMode(HUMIDITY_DIGITAL_PIN, INPUT);
  // Analog pin (GP26) doesn't need pinMode on Pico for analogRead, but good practice
  pinMode(HUMIDITY_ANALOG_PIN, INPUT);
  Serial.println("Humidity Sensor Pins Configured.");
}

void loop() {
  Serial.println("\n------------------------------------------------");
  unsigned long currentTimestamp = millis();

  // --- 1. Gather Data ---
  BMPdata bmpData = measBMP();
  IMUData imuData = readMPU();
  
  // Read Humidity (pass pins explicitly to be safe)
  HumidityData humData = getHumidity(HUMIDITY_ANALOG_PIN, HUMIDITY_DIGITAL_PIN, true);

  // --- 2. Create Log Record ---
  LogRecord record(currentTimestamp, imuData, bmpData, humData);

  // --- 3. Display Data (using LogRecord's display method) ---
  Serial.println(">>> LogRecord Content:");
  record.display();

  // --- 4. Test CSV Formatting ---
  char csvBuffer[256];
  record.toCSV(csvBuffer, sizeof(csvBuffer));
  Serial.print(">>> CSV Output: ");
  Serial.println(csvBuffer);

  // --- 5. Test Binary Packing (for radio transmission) ---
  PackedRecord packed = packRecord(record);
  Serial.print(">>> Packed Size: ");
  Serial.print(sizeof(packed));
  Serial.println(" bytes");

  delay(2000);
}
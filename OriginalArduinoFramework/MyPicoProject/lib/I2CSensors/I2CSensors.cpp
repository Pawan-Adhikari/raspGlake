#include "I2CSensors.hpp"

bool beginBMP(uint8_t address){
    if (bmp.sensorID() == BMP280_CHIPID) return true;
    if (!bmp.begin(address, BMP280_CHIPID)) {
        return false;
    }
    bmp.setSampling(Adafruit_BMP280::MODE_FORCED,      // Forced mode
                    Adafruit_BMP280::SAMPLING_X1,      // Temp sampling
                    Adafruit_BMP280::SAMPLING_X1,      // Pressure sampling
                    Adafruit_BMP280::FILTER_OFF, 
                    Adafruit_BMP280::STANDBY_MS_500);
    return true;
}

bool beginMPU(TwoWire *myWire, uint8_t address){
    if(!mpu.begin(address, myWire)){
        return false;
    }
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G); 
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);       
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);     
    return true;
}

BMPdata measBMP(){
    BMPdata bmpReadings;
    if (bmp.sensorID() != BMP280_CHIPID) beginBMP();
    if (bmp.takeForcedMeasurement()) {
        bmpReadings.temperature = bmp.readTemperature();
        bmpReadings.pressure = bmp.readPressure();
        bmpReadings.altitude = bmp.readAltitude(1013.25);
    }
    return bmpReadings;
}

IMUData readMPU(){
    IMUData imuReadings;
    sensors_event_t a, g, temp;
    
    if(!mpu.getEvent(&a, &g, &temp)){
        if(!beginMPU()){
            //Serial.println("MPU6050 couldn't initialise.");
            return imuReadings;
        }
        else mpu.getEvent(&a, &g, &temp);
    }
    
    imuReadings.accelX = a.acceleration.x;
    imuReadings.accelY = a.acceleration.y;
    imuReadings.accelZ = a.acceleration.z;
    
    imuReadings.gyroX = g.gyro.x;
    imuReadings.gyroY = g.gyro.y;
    imuReadings.gyroZ = g.gyro.z;
    
    
    return imuReadings; 
}

void BMPdata::display() const {
        Serial.println("Temperature, Pressure, and altitude: ");
        Serial.println(temperature);
        Serial.println("Celsius");
        Serial.println(pressure);
        Serial.println("Pa");
        Serial.println(altitude);
        Serial.println("m");
    }


void IMUData::display() const {
        Serial.println("Acceleration, XYZ: ");
        Serial.println(accelX);
        Serial.println(accelY);
        Serial.println(accelZ);
        Serial.println("Gyroscope, XYZ: ");
        Serial.println(gyroX);
        Serial.println(gyroY);
        Serial.println(gyroZ);
    }

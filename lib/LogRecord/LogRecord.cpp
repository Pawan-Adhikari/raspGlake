#include"LogRecord.hpp"

LogRecord::LogRecord(unsigned long t, IMUData i, BMPdata b, HumidityData h){
    timestamp_ms = t;
    imu = i;
    bmp = b;
    hum = h;
}

int LogRecord::toCSV(char* buffer, const size_t bufferSize) const{
// Format: Timestamp, T, P, A, aX, aY, aZ, gX, gY, gZ, RH, DryStatus
    int len = snprintf(
        buffer, 
        bufferSize, 
        "%lu,%.2f,%.2f,%.2f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.2f,%d\n",
        
        timestamp_ms,
        
        // BMP Data
        bmp.temperature,
        bmp.pressure,
        bmp.altitude,
        
        // MPU Data
        imu.accelX,
        imu.accelY,
        imu.accelZ,
        
        imu.gyroX,
        imu.gyroY,
        imu.gyroZ,
        
        // Humidity Data
        hum.moisture,
        (int)hum.isDry 
    );
    //Serial.println("Currently converting the following reading to csv:");
    //Serial.println(buffer);
return len;
}

void LogRecord::display() const {
    Serial.println("TimeStamp: ");
    Serial.println(timestamp_ms);
    imu.display();
    bmp.display();
    hum.display();

}

PackedRecord packRecord(const LogRecord &r) {
    PackedRecord p;
    p.timestamp_ms = r.timestamp_ms;

    p.accelX = r.imu.accelX;
    p.accelY = r.imu.accelY;
    p.accelZ = r.imu.accelZ;

    p.gyroX = r.imu.gyroX;
    p.gyroY = r.imu.gyroY;
    p.gyroZ = r.imu.gyroZ;

    p.temperature = r.bmp.temperature;
    p.pressure = r.bmp.pressure;
    p.altitude = r.bmp.altitude;

    p.isDry = r.hum.isDry ? 1 : 0;
    p.moisture = r.hum.moisture;

    return p;
}

void sendRecord(const PackedRecord &r) {
    Serial2.write(0xAA);                  // Start byte
    Serial2.write((const uint8_t*)&r, sizeof(PackedRecord));  // Raw bytes
}

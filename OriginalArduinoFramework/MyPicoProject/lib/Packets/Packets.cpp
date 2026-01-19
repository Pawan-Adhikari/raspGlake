#include"Packets.hpp"

/*
LogRecord::LogRecord(unsigned long t, IMUData i, BMPdata b, HumidityData h){
    timestamp_ms = t;
    imu = i;
    bmp = b;
    hum = h;
}
*/
/*
void LogRecord::display() const {
    Serial.println("TimeStamp: ");
    Serial.println(timestamp_ms);
    imu.display();
    bmp.display();
    hum.display();

}
*/

unsigned char getChecksum(const uint8_t *data, int sz){
    uint8_t checksum=0;
    for (int i=0; i<sz; i++){
        checksum += data[i];
    }
    return checksum;
}

/*
packet packRecord(const LogRecord &r) {
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

    packet pac;
    pac.sync = 0xAA;
    pac.rec = p;
    setChecksum(&pac);

    return pac;
}
*/


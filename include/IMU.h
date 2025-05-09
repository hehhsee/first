#ifndef IMU_H
#define IMU_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

class IMU {
public:
    struct IMUData {
        float accelX, accelY, accelZ;
        float gyroX, gyroY, gyroZ;
        float pitch, roll;
    };

    bool begin(uint8_t sda_pin = 21, uint8_t scl_pin = 22);
    bool update();
    const IMUData& getData() const;

private:
    Adafruit_MPU6050 mpu;
    IMUData currentData;
    unsigned long lastUpdate = 0;
    
    void calculateAngles();
};

#endif
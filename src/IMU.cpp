#include "IMU.h"
#include <Wire.h>  // 必须包含Wire库

bool IMU::begin(uint8_t sda_pin, uint8_t scl_pin) {
    Wire.begin(sda_pin, scl_pin);
    if (!mpu.begin(0x70, &Wire)) { // 默认I2C地址0x68  //购买国产 id 0x70
        return false;
    }
    
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    
    return true;
}

bool IMU::update() {
    if (millis() - lastUpdate < 10) return false; // 10ms采样间隔
    
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    
    currentData.accelX = a.acceleration.x;
    currentData.accelY = a.acceleration.y;
    currentData.accelZ = a.acceleration.z;
    currentData.gyroX = g.gyro.x;
    currentData.gyroY = g.gyro.y;
    currentData.gyroZ = g.gyro.z;
    
    calculateAngles();
    lastUpdate = millis();
    return true;
}

void IMU::calculateAngles() {
    // 简化的姿态计算（可根据需求改进）
    currentData.roll = atan2(currentData.accelY, currentData.accelZ) * 180/M_PI;
    currentData.pitch = atan2(-currentData.accelX, 
                             sqrt(currentData.accelY*currentData.accelY + 
                                  currentData.accelZ*currentData.accelZ)) * 180/M_PI;
}
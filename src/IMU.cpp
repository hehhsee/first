#include "IMU.h"
#include <Wire.h>  // 必须包含Wire库


bool IMU::init(uint8_t sda_pin, uint8_t scl_pin) {
    Wire.begin(sda_pin, scl_pin);
    Wire.setClock(400000); // 400kHz I2C
    
    if(!_mpu.begin(MPU6050_I2CADDR_DEFAULT, &Wire)){
        return false;
    }

    _mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    _mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    _mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    
    return true;
}

bool IMU::update() {
    if(millis() - _last_update < 10) return false;
    
    sensors_event_t accel, gyro, temp;
    _mpu.getEvent(&accel, &gyro, &temp);

    _data.accel[0] = accel.acceleration.x;
    _data.accel[1] = accel.acceleration.y;
    _data.accel[2] = accel.acceleration.z;
    
    _data.gyro[0] = gyro.gyro.x * RAD_TO_DEG;
    _data.gyro[1] = gyro.gyro.y * RAD_TO_DEG;
    _data.gyro[2] = gyro.gyro.z * RAD_TO_DEG;

    _calculate_angles();
    _last_update = millis();
    return true;
}

void IMU::_calculate_angles() {
    // 使用加速度计计算姿态角
    _data.roll  = atan2(_data.accel[1], _data.accel[2]) * RAD_TO_DEG;
    _data.pitch = atan2(-_data.accel[0], sqrt(_data.accel[1]*_data.accel[1] + 
                                         _data.accel[2]*_data.accel[2])) * RAD_TO_DEG;
}
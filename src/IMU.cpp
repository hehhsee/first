#include "IMU.h"
#include <Wire.h>  // 必须包含Wire库
#include <Arduino.h>
void IMU::begin(uint8_t sda, uint8_t scl) {
    // 实现代码，例如初始化 MPU6050
    Wire.begin(sda, scl); // 假设使用 I2C 初始化
    // 检查设备是否连接
}
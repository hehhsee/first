#ifndef IMU_H
#define IMU_H

#include <Arduino.h>

class IMU {
public:
   void begin(uint8_t sda, uint8_t scl); // 确保声明正确
};

#endif
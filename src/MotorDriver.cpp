#include "MotorDriver.h"
#include <Arduino.h>

uint8_t MotorDriver::channel_counter = 0;

void MotorDriver::begin(const uint8_t* pins, uint32_t freq, uint8_t resolution) {
    // 初始化代码...
    for(int i=0; i<4; i++){
        _pins[i] = pins[i];
        _channels[i] = channel_counter++;
        ledcSetup(_channels[i], freq, resolution);
        ledcAttachPin(_pins[i], _channels[i]);
    }
    _resolution = resolution;
    emergencyStop();
}

void MotorDriver::setSpeed(uint8_t motor_index, uint8_t speed) {
    // 单电机控制代码...
}

// 确保实现 setAllSpeed
void MotorDriver::setAllSpeed(uint8_t speed) {
    speed = constrain(speed, 0, (1 << _resolution) - 1);
    for (int i = 0; i < 4; i++) {
        ledcWrite(_channels[i], speed);
    }
}

void MotorDriver::emergencyStop() {
    // 急停代码...
}
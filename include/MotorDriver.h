#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <Arduino.h>

class MotorDriver {
public:
    void begin(const uint8_t* pins, uint32_t freq = 20000, uint8_t resolution = 8);
    void setSpeed(uint8_t motor_index, uint8_t speed);
    void setAllSpeed(uint8_t speed); // 确保声明存在
    void emergencyStop();

private:
    static uint8_t channel_counter;
    uint8_t _pins[4];
    uint8_t _channels[4];
    uint8_t _resolution;
};

#endif
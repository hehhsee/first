#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <Arduino.h>

class MotorDriver {
public:
    void begin(const uint8_t pins[4], uint32_t freq = 20000, uint8_t resolution = 8);
    void set_speed(uint8_t motor_index, uint8_t speed);
    void set_all_speed(uint8_t speed);
    void emergency_stop();

private:
    static uint8_t _channel_counter;
    uint8_t _pins[4];
    uint8_t _channels[4];
    uint8_t _resolution;
};

#endif
#include "MotorDriver.h"
#include <Arduino.h>


void MotorDriver::begin(const uint8_t pins[4], const uint8_t Channels[4], uint32_t freq , uint8_t resolution ) {
    for(uint8_t i=0; i<4; i++){
        _pins[i] = pins[i];
        _channels[i] = Channels[i];
        ledcSetup(_channels[i], freq, resolution);
        ledcAttachPin(_pins[i], _channels[i]);
    }
    _resolution = resolution;
 //   emergency_stop();
}

void MotorDriver::set_speed(uint8_t motor_index, uint8_t speed) {
    if(motor_index >= 4) return;
    speed = constrain(speed, 0, (1 << _resolution) - 1);
    ledcWrite(_channels[motor_index], speed);
}

void MotorDriver::set_all_speed(uint8_t speed) {
    speed = constrain(speed, 0, (1 << _resolution) - 1);
    for(uint8_t i=0; i<4; i++){
        ledcWrite(_channels[i], speed);
    }
}

void MotorDriver::emergency_stop() {
   // memset(_channels, 0, sizeof(_channels)); // 快速清零
    set_all_speed(0);
}
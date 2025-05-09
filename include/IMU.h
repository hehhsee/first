#ifndef IMU_H
#define IMU_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

class IMU {
public:
    struct Data {
        float accel[3];  // x,y,z (m/s²)
        float gyro[3];    // x,y,z (rad/s)
        float pitch;      // 俯仰角
        float roll;       // 横滚角
    };

    bool init(uint8_t sda_pin = 21, uint8_t scl_pin = 22);
    bool update();
    const Data& data() const { return _data; }

private:
    Adafruit_MPU6050 _mpu;
    Data _data;
    uint32_t _last_update = 0;

    void _calculate_angles();
};
#endif
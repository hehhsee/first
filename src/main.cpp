#include <Arduino.h>
#include "MotorDriver.h"
#include "IMU.h"
#include <BluetoothSerial.h>

// 硬件配置
const uint8_t MOTOR_PINS[] = {12, 13, 14, 15};
MotorDriver motors;
IMU imu;
BluetoothSerial SerialBT;

// 电机速度变量
uint8_t motor_speed = 0;

void setup() {
    Serial.begin(115200);
    delay(1000);

    motors.begin(MOTOR_PINS);
    if(!imu.init()) {
        Serial.println("IMU初始化失败!");
        while(1);
    }

    Serial.println("串口控制已启动，请输入速度值(0~255)：");
}

void loop() {
    // 通过串口控制电机速度
    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        int speed = cmd.toInt();
        if (speed >= 0 && speed <= 255) {
            motor_speed = speed;
            motors.set_all_speed(motor_speed);
            Serial.print("已设置电机速度为: ");
            Serial.println(motor_speed);
        } else {
            Serial.println("请输入0~255之间的速度值");
        }
    }

    // 读取IMU数据
    //if(imu.update()) {
    //    const auto& data = imu.data();
    //    Serial.print("Pitch: ");
     //   Serial.print(data.pitch);
     //   Serial.print("  Roll: ");
     //   Serial.print(data.roll);
    //    Serial.println();
   // }
}
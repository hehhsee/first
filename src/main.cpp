
#include <Arduino.h>
#include "MotorDriver.h"
#include "IMU.h"

// 电机控制引脚定义
const uint8_t motorPins[] = {12, 13, 14, 15};

// 创建电机驱动实例
MotorDriver motorController;
IMU imu;
void setup() {
    //初始化串口
    Serial.begin(115200);
    delay(1000);





    // 初始化电机驱动
    motorController.begin(motorPins); // 使用默认20kHz频率
    Serial.println("电机驱动初始化完成");

    // 初始化IMU
    if (!imu.begin()) {
        Serial.println("MPU6050初始化失败!");
        while(1);
    }
    Serial.println("MPU6050初始化成功");



    Serial.println("系统就绪");

    
}

void loop() {
    // 示例控制代码
    static uint8_t speed = 0;
    static bool increasing = true;
    
    // 呼吸灯效果
    motorController.setAllSpeed(speed);
    
    if(increasing) {
        speed++;
        if(speed >= 255) increasing = false;
    } else {
        speed--;
        if(speed <= 0) increasing = true;
    }
    
    delay(10);
    
    // 紧急停止测试（按需启用）
    // if(digitalRead(BUTTON_PIN)) motorController.emergencyStop();
}
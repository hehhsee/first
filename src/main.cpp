
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
    motorController.begin(motorPins); // 使用默认20kHz频率

    imu.begin(21,22);

    Serial.println("系统就绪");
    // 初始化电机驱动
    
    // 其他初始化...
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
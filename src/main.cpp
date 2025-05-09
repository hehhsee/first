
#include <Arduino.h>
#include "MotorDriver.h"
#include "IMU.h"

// 电机控制引脚定义
const uint8_t motorPins[] = {12, 13, 14, 15};
Adafruit_MPU6050 mpu;
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
    if (!imu.init()) {
        Serial.println("MPU6050初始化失败!");
        while(1);
    }
    Serial.println("MPU6050初始化成功");
    Serial.println("系统就绪");

    
}

void loop() {

  imu.update(); // 更新IMU数据

 Serial.printf("俯仰角: %.2f°\t横滚角: %.2f°\n", 
                     imu.data().pitch, imu.data().roll);

  delay(1000); // 延时0.5秒
  motorController.setAllSpeed(100); // 设置所有电机速度为255
  
}
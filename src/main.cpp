#include <Arduino.h>
#include "MotorDriver.h"
#include <WiFi.h>
#include <WebServer.h>

// PWM配置
const uint8_t pwmPins[4] = {16, 19, 32, 33};
const uint8_t pwmChannels[4] = {0, 1, 2, 3};
const uint32_t pwmFreq = 20000;
const uint8_t pwmResolution = 8;

MotorDriver motors;
uint8_t motor_speed = 0;
uint8_t motor_speeds[4] = {0,0,0,0};

// WiFi配置
const char* ssid = "xiaoli";
const char* password = "11111111";

WebServer server(80);

void handleRoot() {
    String html = R"rawliteral(
    <html>
    <head>
        <meta charset="utf-8">
        <title>ESP32 电机速度控制</title>
        <style>
            body { font-size: 2em; text-align: center; }
            button { font-size: 1.2em; margin: 10px 10px; padding: 10px 30px; }
            #speedValues span { font-weight: bold; color: #0078D7; margin: 0 10px;}
        </style>
    </head>
    <body>
        <h1>ESP32 电机速度控制</h1>
        <button onclick="clearAll()">全部清零</button><br>
        <button onclick="incMotor(0)">电机1+</button>
        <button onclick="incMotor(1)">电机2+</button>
        <button onclick="incMotor(2)">电机3+</button>
        <button onclick="incMotor(3)">电机4+</button>
        <p id="speedValues">
            电机1: <span id="m0">0</span>
            电机2: <span id="m1">0</span>
            电机3: <span id="m2">0</span>
            电机4: <span id="m3">0</span>
        </p>
        <script>
            let speeds = [0,0,0,0];
            function updateSpeeds(arr) {
                for(let i=0;i<4;i++) {
                    document.getElementById("m"+i).textContent = arr[i];
                }
            }
            function clearAll() {
                fetch('/clear').then(r=>r.json()).then(data=>{
                    speeds = data.speeds;
                    updateSpeeds(speeds);
                });
            }
            function incMotor(idx) {
                fetch('/inc?motor='+idx).then(r=>r.json()).then(data=>{
                    speeds = data.speeds;
                    updateSpeeds(speeds);
                });
            }
            // 页面加载时获取当前速度
            fetch('/getspeeds').then(r=>r.json()).then(data=>{
                speeds = data.speeds;
                updateSpeeds(speeds);
            });
        </script>
    </body>
    </html>
    )rawliteral";
    server.send(200, "text/html", html);
}

void handleSet() {
    if (server.hasArg("speed")) {
        int speed = server.arg("speed").toInt();
        if (speed >= 0 && speed <= 255) {
            motor_speed = speed;
            motors.set_all_speed(motor_speed);
            server.send(200, "text/html", "<p>设置成功，当前速度: " + String(motor_speed) + "</p><a href='/'>返回</a>");
            Serial.print("WiFi设置电机速度为: ");
            Serial.println(motor_speed);
            return;
        }
    }
    server.send(400, "text/html", "<p>参数错误</p><a href='/'>返回</a>");
}

void handleGetSpeed() {
    server.send(200, "application/json", "{\"speed\":" + String(motor_speed) + "}");
}

void handleInc() {
    if (server.hasArg("motor")) {
        int idx = server.arg("motor").toInt();
        if (idx >= 0 && idx < 4) {
            if (motor_speeds[idx] < 255) motor_speeds[idx]++;
            motors.set_speed(idx, motor_speeds[idx]);
        }
    }
    server.send(200, "application/json", 
        "{\"speeds\":[" + String(motor_speeds[0]) + "," + String(motor_speeds[1]) + "," +
        String(motor_speeds[2]) + "," + String(motor_speeds[3]) + "]}");
}

void handleClear() {
    for(int i=0;i<4;i++) {
        motor_speeds[i] = 0;
        motors.set_speed(i, 0);
    }
    server.send(200, "application/json", 
        "{\"speeds\":[0,0,0,0]}");
}

void handleGetSpeeds() {
    server.send(200, "application/json", 
        "{\"speeds\":[" + String(motor_speeds[0]) + "," + String(motor_speeds[1]) + "," +
        String(motor_speeds[2]) + "," + String(motor_speeds[3]) + "]}");
}

void setup() {
    Serial.begin(115200);
    motors.begin(pwmPins, pwmChannels, pwmFreq, pwmResolution);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("正在连接WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi已连接，IP地址: ");
    Serial.println(WiFi.localIP());

    server.on("/", handleRoot);
    server.on("/set", handleSet);
    server.on("/getspeed", handleGetSpeed); // 新增
    server.on("/inc", handleInc); // 新增
    server.on("/clear", handleClear); // 新增
    server.on("/getspeeds", handleGetSpeeds); // 新增
    server.begin();
    Serial.println("Web服务器已启动");
}

void loop() {
    server.handleClient();

    // 检查WiFi连接状态，断开时关闭电机
    if (WiFi.status() != WL_CONNECTED) {
        motors.set_all_speed(0);
    }
}
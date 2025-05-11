#include "wifi_controller.h"
#include "SPIFFS.h"

void WiFiController::begin(const char* ssid, const char* password) {
    // 启动WiFi
    if(password) {
        WiFi.softAP(ssid, password);
    } else {
        WiFi.softAP(ssid);
    }

    // 初始化文件系统
    if(!SPIFFS.begin(true)){
        Serial.println("SPIFFS初始化失败");
        return;
    }

    // WebSocket回调
    ws.onEvent([this](AsyncWebSocket* server, AsyncWebSocketClient* client, 
                     AwsEventType type, void* arg, uint8_t* data, size_t len){
        if(type == WS_EVT_DATA)
            this->on_web_message(client, arg, data, len);
    });

    // 配置Web服务器
    server.addHandler(&ws);
    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
    
    server.onNotFound([](AsyncWebServerRequest* request){
        request->send(404, "text/plain", "Not found");
    });

    server.begin();
}

void WiFiController::set_control_callback(ControlCallback cb) {
    control_cb = cb;
}

void WiFiController::update_display(float pitch, float roll) {
    String json = String("{\"pitch\":") + pitch + ",\"roll\":" + roll + "}";
    ws.textAll(json);
}

void WiFiController::on_web_message(AsyncWebSocketClient* client, 
                                   void* arg, uint8_t* data, size_t len) {
    AwsFrameInfo* info = (AwsFrameInfo*)arg;
    if(info->final && info->index == 0 && info->len == len){
        data[len] = 0;
        String cmd = String((char*)data);
        
        if(cmd.startsWith("SPEED:")){
            uint8_t speed = cmd.substring(6).toInt();
            if(control_cb) control_cb(speed, 0.0f);
        }
    }
}

String WiFiController::processor(const String& var) {
    return var == "IP" ? WiFi.softAPIP().toString() : String();
}
#ifndef WIFI_CONTROLLER_H
#define WIFI_CONTROLLER_H

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "MotorDriver.h"

class WiFiController {
public:
    typedef std::function<void(uint8_t speed, float yaw)> ControlCallback;
    
    void begin(const char* ssid, const char* password = nullptr);
    void set_control_callback(ControlCallback cb);
    void update_display(float pitch, float roll);

private:
    AsyncWebServer server{80};
    AsyncWebSocket ws{"/ws"};
    ControlCallback control_cb;
    
    void init_web_server();
    void on_web_message(AsyncWebSocketClient* client, void* arg, 
                       uint8_t* data, size_t len);
    static String processor(const String& var);
};

#endif
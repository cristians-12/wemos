#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <ESP8266WiFi.h>

class DisplayManager; // Forward declaration

class WiFiManager {
private:
    const char* _ssid;
    const char* _password;
    int _timeoutMs;
    DisplayManager* _display;

public:
    WiFiManager(const char* ssid, const char* password, int timeout);
    
    void setDisplay(DisplayManager* display);
    bool connect();
    bool isConnected();
    String getIP();
};

#endif
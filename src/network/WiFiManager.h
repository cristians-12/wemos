#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <ESP8266WiFi.h>

class WiFiManager {
public:
    WiFiManager(const char* ssid, const char* password, int timeout);
    
    bool connect();
    bool isConnected() const;
    void disconnect();
    String getIPAddress() const;
    
private:
    const char* _ssid;
    const char* _password;
    int _timeoutMs;
    
    void logStatus(const String& message) const;
};

#endif
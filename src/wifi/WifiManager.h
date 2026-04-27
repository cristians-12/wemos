#pragma once
#include <ESP8266WiFi.h>
#include "config.h"

class WiFiManager {
public:
    void begin();
    bool isConnected();
    void reconnectIfNeeded();

private:
    void connect();
};
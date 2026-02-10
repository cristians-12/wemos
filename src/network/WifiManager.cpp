#include "WifiManager.h"
#include "../display/DisplayManager.h"
#include <Arduino.h>

WiFiManager::WiFiManager(const char* ssid, const char* password, int timeout)
    : _ssid(ssid), _password(password), _timeoutMs(timeout), _display(nullptr) {}

void WiFiManager::setDisplay(DisplayManager* display) {
    _display = display;
}

bool WiFiManager::connect() {
    if (_display) {
        _display->showMessage("WiFi...");
    }

    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _password);

    unsigned long startTime = millis();

    while (WiFi.status() != WL_CONNECTED) {
        if (millis() - startTime > _timeoutMs) {
            Serial.println(F("\nTimeout WiFi"));
            if (_display) {
                _display->showMessage("WiFi\nError");
            }
            return false;
        }
        delay(500);
        Serial.print(".");
    }

    Serial.println(F("\nWiFi conectado!"));
    Serial.print(F("IP: "));
    Serial.println(WiFi.localIP());

    return true;
}

bool WiFiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

String WiFiManager::getIP() {
    return WiFi.localIP().toString();
}
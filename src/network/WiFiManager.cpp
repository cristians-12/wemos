#include "WiFiManager.h"

WiFiManager::WiFiManager(const char* ssid, const char* password, int timeout)
    : _ssid(ssid), _password(password), _timeoutMs(timeout) {}

bool WiFiManager::connect() {
    if (isConnected()) {
        return true;
    }
    
    logStatus("Conectando a WiFi...");
    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _password);
    
    unsigned long startTime = millis();
    while (!isConnected() && (millis() - startTime) < _timeoutMs) {
        delay(100);
        yield();
    }
    
    if (isConnected()) {
        logStatus("WiFi conectado - IP: " + WiFi.localIP().toString());
        return true;
    }
    
    logStatus("Error: Timeout WiFi");
    return false;
}

bool WiFiManager::isConnected() const {
    return WiFi.status() == WL_CONNECTED;
}

void WiFiManager::disconnect() {
    WiFi.disconnect();
    logStatus("WiFi desconectado");
}

String WiFiManager::getIPAddress() const {
    return WiFi.localIP().toString();
}

void WiFiManager::logStatus(const String& message) const {
    Serial.println("[WiFiManager] " + message);
}
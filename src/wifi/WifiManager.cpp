#include "WiFiManager.h"

void WiFiManager::begin() {
    Serial.print("Conectando a WiFi");
    connect();
}

void WiFiManager::connect() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    uint8_t attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n WiFi conectado");
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\n Falló la conexión WiFi");
    }
}

bool WiFiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void WiFiManager::reconnectIfNeeded() {
    if (!isConnected()) {
        Serial.println(" WiFi desconectado, reconectando...");
        connect();
    }
}

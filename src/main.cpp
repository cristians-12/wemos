#include <Arduino.h>
#include "config.h"
#include "wifi/WiFiManager.h"
#include "http/HttpClient.h"

WiFiManager  wifiManager;
HttpClient   httpClient;

void setup() {
    Serial.begin(115200);
    delay(10);

    wifiManager.begin();
}

void loop() {
    wifiManager.reconnectIfNeeded();

    delay(50);
}
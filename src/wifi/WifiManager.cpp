#include "WiFiManager.h"

void WiFiManager::begin() {
    pinMode(LED_GREEN_PIN, OUTPUT);
    digitalWrite(LED_GREEN_PIN, LOW);

    Serial.print("Conectando a WiFi");
    connect();
}

void WiFiManager::connect() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    uint8_t attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        blinkFast(); // 💨 Parpadeo rápido mientras intenta
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n✅ WiFi conectado");
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());

        // 🐢 Parpadeo lento al conectar exitosamente
        for (uint8_t i = 0; i < 3; i++) {
            blinkSlow();
        }

        // ✅ LED fijo encendido = conectado
        digitalWrite(LED_GREEN_PIN, HIGH);

    } else {
        Serial.println("\n❌ Falló la conexión WiFi");
        digitalWrite(LED_GREEN_PIN, LOW);
    }
}

bool WiFiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void WiFiManager::reconnectIfNeeded() {
    if (!isConnected()) {
        Serial.println("⚠️ WiFi desconectado, reconectando...");
        digitalWrite(LED_GREEN_PIN, LOW);
        connect();
    }
}

// ─── Parpadeo rápido (100ms) ──────────────────────────
void WiFiManager::blinkFast() {
    digitalWrite(LED_GREEN_PIN, HIGH);
    delay(100);
    digitalWrite(LED_GREEN_PIN, LOW);
    delay(100);
}

// ─── Parpadeo lento (500ms) ───────────────────────────
void WiFiManager::blinkSlow() {
    digitalWrite(LED_GREEN_PIN, HIGH);
    delay(500);
    digitalWrite(LED_GREEN_PIN, LOW);
    delay(500);
}
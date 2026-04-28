#include "AccessController.h"

AccessController::AccessController(HttpClient& httpClient)
    : _httpClient(httpClient) {}

void AccessController::begin() {
    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(LED_RED_PIN,   OUTPUT);
    digitalWrite(LED_GREEN_PIN, LOW);
    digitalWrite(LED_RED_PIN,   LOW);
    Serial.println("✅ AccessController listo");
}

void AccessController::processCard(const String& uid) {
    Serial.println("🪪 UID detectado: " + uid);

    // ✅ JsonDocument en lugar de StaticJsonDocument
    JsonDocument doc;
    doc["uid"] = uid;
    String payload;
    serializeJson(doc, payload);

    String response;
    bool success = _httpClient.post(API_ACCESS, payload, response);

    if (!success) {
        Serial.println("❌ Error comunicando con el servidor");
        denyAccess();
        return;
    }

    if (parseResponse(response)) {
        grantAccess();
    } else {
        denyAccess();
    }
}

bool AccessController::parseResponse(const String& response) {
    // ✅ JsonDocument en lugar de StaticJsonDocument
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, response);

    if (error) {
        Serial.println("❌ Error parseando JSON");
        return false;
    }

    return doc["access"].as<bool>();
}

void AccessController::grantAccess() {
    Serial.println("✅ Acceso PERMITIDO");
    digitalWrite(LED_GREEN_PIN, HIGH);
    delay(LED_ON_TIME);
    digitalWrite(LED_GREEN_PIN, LOW);
}

void AccessController::denyAccess() {
    Serial.println("🚫 Acceso DENEGADO");
    digitalWrite(LED_RED_PIN, HIGH);
    delay(LED_ON_TIME);
    digitalWrite(LED_RED_PIN, LOW);
}
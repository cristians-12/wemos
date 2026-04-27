#include "HttpClient.h"

bool HttpClient::beginRequest(const String& url) {
    _http.setTimeout(API_TIMEOUT);
    return _http.begin(_wifiClient, url);
}

void HttpClient::setHeaders() {
    _http.addHeader("Content-Type", "application/json");
    _http.addHeader("Accept",       "application/json");
}

void HttpClient::endRequest() {
    _http.end();
}

// ─── GET ──────────────────────────────────────────────
bool HttpClient::get(const String& endpoint, String& response) {
    String url = String(API_BASE_URL) + endpoint;

    if (!beginRequest(url)) {
        Serial.println(" Error al iniciar GET");
        return false;
    }

    setHeaders();
    int httpCode = _http.GET();

    if (httpCode == HTTP_CODE_OK) {
        response = _http.getString();
        Serial.println(" GET exitoso");
        Serial.println(" Response: " + response);
        endRequest();
        return true;
    }

    Serial.printf("GET falló, código: %d\n", httpCode);
    endRequest();
    return false;
}

// ─── POST ─────────────────────────────────────────────
bool HttpClient::post(const String& endpoint, const String& payload, String& response) {
    String url = String(API_BASE_URL) + endpoint;

    if (!beginRequest(url)) {
        Serial.println(" Error al iniciar POST");
        return false;
    }

    setHeaders();
    int httpCode = _http.POST(payload);

    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_CREATED) {
        response = _http.getString();
        Serial.println("POST exitoso");
        Serial.println("📥 Response: " + response);
        endRequest();
        return true;
    }

    Serial.printf("POST falló, código: %d\n", httpCode);
    endRequest();
    return false;
}
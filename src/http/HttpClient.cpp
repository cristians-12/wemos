#include "HttpClient.h"

bool HttpClient::beginRequest(const String &url)
{
    _wifiClient.setInsecure();
    _http.setTimeout(API_TIMEOUT);
    return _http.begin(_wifiClient, url);
}

void HttpClient::setHeaders()
{
    _http.addHeader("Content-Type", "application/json");
    _http.addHeader("Accept", "application/json");
    _http.addHeader("Authorization", "Bearer " + String(API_KEY));
}

void HttpClient::endRequest()
{
    _http.end();
}

// ─── GET ──────────────────────────────────────────────
bool HttpClient::get(const String &endpoint, String &response)
{
    String url = String(API_BASE_URL) + endpoint;
    Serial.println("URL: " + url);

    if (!beginRequest(url))
    {
        Serial.println(" Error al iniciar GET");
        return false;
    }

    setHeaders();
    int httpCode = _http.GET();

    response = _http.getString();

    Serial.printf("HTTP Code: %d\n", httpCode);
    Serial.println("Response: " + response);

    if (httpCode == HTTP_CODE_OK)
    {
        Serial.println(" GET exitoso");
        endRequest();
        return true;
    }

    Serial.println(" GET falló");
    endRequest();
    return false;
}

// ─── POST ─────────────────────────────────────────────
bool HttpClient::post(const String &endpoint, const String &payload, String &response)
{
    String url = String(API_BASE_URL) + endpoint;

    if (!beginRequest(url))
    {
        Serial.println(" Error al iniciar POST");
        return false;
    }

    setHeaders();
    int httpCode = _http.POST(payload);

    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_CREATED)
    {
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
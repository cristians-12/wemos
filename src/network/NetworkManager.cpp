#include "NetworkManager.h"
#include "../display/DisplayManager.h"
#include <Arduino.h>

NetworkManager::NetworkManager(const char *ssid, const char *password,
                               const char *apiUrl, int wifiTimeout, int httpTimeout)
    : _ssid(ssid), _password(password),
      _wifiTimeout(wifiTimeout), _httpTimeout(httpTimeout),
      _apiUrl(apiUrl), _display(nullptr), _lastHttpCode(-1) {} // ← Orden corregido

void NetworkManager::setDisplay(DisplayManager *display)
{
    _display = display;
}

// ==================== WiFi ====================

bool NetworkManager::connectWiFi()
{
    if (_display)
    {
        _display->showMessage("WiFi...");
    }

    Serial.println(F("\n--- Conectando WiFi ---"));
    Serial.printf("SSID: %s\n", _ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _password);

    unsigned long startTime = millis();

    while (WiFi.status() != WL_CONNECTED)
    {
        if ((millis() - startTime) > (unsigned long)_wifiTimeout)
        { // ← Cast para evitar warning
            Serial.println(F("\n✗ Timeout WiFi"));
            _lastError = "WiFi Timeout";

            if (_display)
            {
                _display->showMessage("WiFi\nError");
            }
            return false;
        }
        delay(500);
        Serial.print(".");
    }

    Serial.println(F("\n✓ WiFi conectado!"));
    Serial.print(F("IP: "));
    Serial.println(WiFi.localIP());
    Serial.printf("RSSI: %d dBm\n", WiFi.RSSI());

    if (_display)
    {
        _display->showMessage("WiFi OK");
    }

    return true;
}

bool NetworkManager::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

String NetworkManager::getIP()
{
    return WiFi.localIP().toString();
}

void NetworkManager::checkConnection()
{
    if (!isConnected())
    {
        Serial.println(F("⚠ WiFi desconectado, reconectando..."));

        if (_display)
        {
            _display->showMessage("Recon...");
        }

        connectWiFi();
    }
    else
    {
        Serial.println(F("✓ WiFi: Conectado"));
    }
}

// ==================== HTTP ====================

bool NetworkManager::httpGet(String &response)
{
    return executeHttpRequest("GET", "", response);
}

bool NetworkManager::httpPost(const String &payload, String &response)
{
    return executeHttpRequest("POST", payload, response);
}

bool NetworkManager::executeHttpRequest(const char *method, const String &payload, String &response)
{
    if (!isConnected())
    {
        Serial.println(F("✗ Error: No hay conexión WiFi"));
        _lastError = "No WiFi";

        if (_display)
        {
            _display->showMessage("No WiFi");
        }
        return false;
    }

    if (_display)
    {
        _display->showMessage("HTTP...");
    }

    Serial.printf("\n--- HTTP %s ---\n", method);
    Serial.printf("URL: %s\n", _apiUrl);

    // ========== CONFIGURACIÓN HTTPS ==========
    _wifiClient.setInsecure(); // ← Ahora funciona con WiFiClientSecure

    _http.begin(_wifiClient, _apiUrl);
    _http.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
    _http.setTimeout(_httpTimeout);

    // ========== HEADERS (simular navegador) ==========
    _http.addHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36");
    _http.addHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
    _http.addHeader("Accept-Language", "es-ES,es;q=0.9,en;q=0.8");
    _http.addHeader("Accept-Encoding", "identity");
    _http.addHeader("Connection", "keep-alive");
    _http.addHeader("Cache-Control", "max-age=0");

    Serial.println(F("Headers: User-Agent: Mozilla/5.0..."));

    // ========== EJECUTAR PETICIÓN ==========
    if (strcmp(method, "GET") == 0)
    {
        _lastHttpCode = _http.GET();
    }
    else if (strcmp(method, "POST") == 0)
    {
        _http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        Serial.printf("Payload: %s\n", payload.c_str());
        _lastHttpCode = _http.POST(payload);
    }
    else
    {
        _lastHttpCode = -1;
    }

    bool success = false;

    if (_lastHttpCode > 0)
    {
        Serial.printf("HTTP Code: %d\n", _lastHttpCode);

        String serverResponse = _http.getString();

        if (_lastHttpCode == HTTP_CODE_OK ||
            _lastHttpCode == HTTP_CODE_CREATED ||
            _lastHttpCode == HTTP_CODE_MOVED_PERMANENTLY ||
            _lastHttpCode == HTTP_CODE_FOUND)
        {

            response = serverResponse;

            Serial.println(F("✓ Respuesta recibida:"));
            Serial.println(response.substring(0, 500));
            Serial.printf("Tamaño: %d bytes\n", response.length());

            success = true;
            _lastError = "";

            if (_display)
            {
                _display->showMessage("HTTP OK");
            }
        }
        else
        {
            _lastError = "HTTP " + String(_lastHttpCode);
            Serial.printf("✗ Error HTTP: %d\n", _lastHttpCode);
            Serial.println(F("Respuesta del servidor:"));
            Serial.println(serverResponse);

            if (_display)
            {
                _display->showMessage("HTTP Err");
            }
        }
    }
    else
    {
        _lastError = _http.errorToString(_lastHttpCode);
        Serial.printf("✗ Error conexión: %s\n", _lastError.c_str());

        if (_display)
        {
            _display->showMessage("Con Err");
        }
    }

    _http.end();
    return success;
}

int NetworkManager::getLastHttpCode()
{
    return _lastHttpCode;
}

String NetworkManager::getLastError()
{
    return _lastError;
}

bool NetworkManager::httpPostToUrl(const String &url, const String &payload, String &response)
{
    if (!isConnected())
    {
        Serial.println(F("✗ Error: No hay conexión WiFi"));
        _lastError = "No WiFi";
        return false;
    }

    if (_display)
    {
        _display->showMessage("POST...");
    }

    Serial.printf("\n--- HTTP POST ---\n");
    Serial.printf("URL: %s\n", url.c_str());

    _wifiClient.setInsecure();

    _http.begin(_wifiClient, url);
    _http.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
    _http.setTimeout(_httpTimeout);

    // Headers para formulario
    _http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    _http.addHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36");
    _http.addHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9");

    Serial.printf("Payload: %s\n", payload.c_str());

    _lastHttpCode = _http.POST(payload);

    bool success = false;

    if (_lastHttpCode > 0)
    {
        Serial.printf("HTTP Code: %d\n", _lastHttpCode);

        String serverResponse = _http.getString();

        if (_lastHttpCode == HTTP_CODE_OK ||
            _lastHttpCode == HTTP_CODE_CREATED ||
            _lastHttpCode == HTTP_CODE_FOUND ||
            _lastHttpCode == HTTP_CODE_SEE_OTHER)
        {

            response = serverResponse;
            Serial.println(F("✓ Respuesta recibida"));
            success = true;
            _lastError = "";
        }
        else
        {
            _lastError = "HTTP " + String(_lastHttpCode);
            Serial.printf("✗ Error HTTP: %d\n", _lastHttpCode);
            Serial.println(serverResponse);
        }
    }
    else
    {
        _lastError = _http.errorToString(_lastHttpCode);
        Serial.printf("✗ Error: %s\n", _lastError.c_str());
    }

    _http.end();
    return success;
}
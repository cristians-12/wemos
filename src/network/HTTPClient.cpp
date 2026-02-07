#include "HTTPClient.h"

SecureHTTPClient::SecureHTTPClient(int timeout)
    : _timeoutMs(timeout), _lastStatusCode(0), _lastError("") {
    _wifiClient.setInsecure(); // Para desarrollo
}

bool SecureHTTPClient::get(const String& url, String& response) {
    HTTPClient http;
    
    logRequest(url);
    
    http.begin(_wifiClient, url);
    http.setTimeout(_timeoutMs);
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    
    _lastStatusCode = http.GET();
    
    if (_lastStatusCode == HTTP_CODE_OK) {
        response = http.getString();
        logResponse(_lastStatusCode, response);
        http.end();
        return true;
    }
    
    _lastError = (_lastStatusCode > 0) 
        ? "HTTP Error: " + String(_lastStatusCode)
        : http.errorToString(_lastStatusCode);
    
    logResponse(_lastStatusCode, _lastError);
    http.end();
    return false;
}

void SecureHTTPClient::logRequest(const String& url) const {
    Serial.println("[HTTPClient] GET -> " + url);
}

void SecureHTTPClient::logResponse(int code, const String& response) const {
    Serial.printf("[HTTPClient] Response: %d | Length: %d\n", code, response.length());
}
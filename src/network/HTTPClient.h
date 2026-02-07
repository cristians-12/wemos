#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

class SecureHTTPClient {
public:
    SecureHTTPClient(int timeout);
    
    bool get(const String& url, String& response);
    int getLastStatusCode() const { return _lastStatusCode; }
    String getLastError() const { return _lastError; }
    
private:
    int _timeoutMs;
    int _lastStatusCode;
    String _lastError;
    WiFiClientSecure _wifiClient;
    
    void logRequest(const String& url) const;
    void logResponse(int code, const String& response) const;
};

#endif
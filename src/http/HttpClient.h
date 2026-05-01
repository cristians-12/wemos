#pragma once
#include <ESP8266HTTPClient.h>
// #include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include "config.h"

class HttpClient
{
public:
    bool get(const String &endpoint, String &response);
    bool post(const String &endpoint, const String &payload, String &response);

private:
    WiFiClientSecure _wifiClient;
    HTTPClient _http;

    bool beginRequest(const String &url);
    void endRequest();
    void setHeaders();
};

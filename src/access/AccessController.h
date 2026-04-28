#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include "config.h"
#include "http/HttpClient.h"

class AccessController {
public:
    AccessController(HttpClient& httpClient);
    void begin();
    void processCard(const String& uid);

private:
    HttpClient& _httpClient;

    void grantAccess();
    void denyAccess();
    bool parseResponse(const String& response);
};
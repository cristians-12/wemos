#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

class DisplayManager;

class NetworkManager
{
private:
    // WiFi
    const char *_ssid;
    const char *_password;
    int _wifiTimeout;

    // API
    const char *_apiUrl;
    int _httpTimeout;

    // Display
    DisplayManager *_display;

    // HTTP - CAMBIO IMPORTANTE
    WiFiClientSecure _wifiClient;
    HTTPClient _http;

    // Estado
    int _lastHttpCode;
    String _lastError;

public:
    NetworkManager(const char *ssid, const char *password,
                   const char *apiUrl, int wifiTimeout, int httpTimeout);

    void setDisplay(DisplayManager *display);

    // WiFi
    bool connectWiFi();
    bool isConnected();
    String getIP();
    void checkConnection();

    // HTTP
    bool httpGet(String &response);
    bool httpPost(const String &payload, String &response);
    bool httpPostToUrl(const String &url, const String &payload, String &response);

    // Estado
    int getLastHttpCode();
    String getLastError();

private:
    bool executeHttpRequest(const char *method, const String &payload, String &response);
};

#endif
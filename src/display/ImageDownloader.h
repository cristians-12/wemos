#ifndef IMAGE_DOWNLOADER_H
#define IMAGE_DOWNLOADER_H

#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

class ImageDownloader {
public:
    ImageDownloader(int timeout);
    
    bool download(const String& url, uint8_t** buffer, size_t& length);
    void freeBuffer(uint8_t* buffer);
    
    int getLastStatusCode() const { return _lastStatusCode; }
    String getLastError() const { return _lastError; }
    
private:
    int _timeoutMs;
    int _lastStatusCode;
    String _lastError;
    WiFiClientSecure _wifiClient;
    
    void logDownload(const String& url, size_t size) const;
};

#endif
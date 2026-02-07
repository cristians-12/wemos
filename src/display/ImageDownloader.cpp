#include "ImageDownloader.h"

ImageDownloader::ImageDownloader(int timeout)
    : _timeoutMs(timeout), _lastStatusCode(0), _lastError("") {
    _wifiClient.setInsecure();
}

bool ImageDownloader::download(const String& url, uint8_t** buffer, size_t& length) {
    HTTPClient http;
    
    Serial.println("[ImageDownloader] Descargando: " + url);
    
    http.begin(_wifiClient, url);
    http.setTimeout(_timeoutMs);
    
    _lastStatusCode = http.GET();
    
    if (_lastStatusCode == HTTP_CODE_OK) {
        length = http.getSize();
        
        if (length <= 0 || length > 100000) { // Max 100KB
            _lastError = "Tamaño inválido: " + String(length);
            http.end();
            return false;
        }
        
        *buffer = (uint8_t*)malloc(length);
        if (!*buffer) {
            _lastError = "Sin memoria para buffer";
            http.end();
            return false;
        }
        
        WiFiClient* stream = http.getStreamPtr();
        size_t bytesRead = stream->readBytes(*buffer, length);
        
        logDownload(url, bytesRead);
        http.end();
        return (bytesRead == length);
    }
    
    _lastError = "HTTP Error: " + String(_lastStatusCode);
    Serial.println("[ImageDownloader] " + _lastError);
    http.end();
    return false;
}

void ImageDownloader::freeBuffer(uint8_t* buffer) {
    if (buffer) {
        free(buffer);
    }
}

void ImageDownloader::logDownload(const String& url, size_t size) const {
    Serial.printf("[ImageDownloader] ✓ Descargado: %d bytes\n", size);
}
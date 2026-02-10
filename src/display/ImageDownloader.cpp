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
        
        Serial.printf("[ImageDownloader] Tamaño: %d bytes\n", length);
        
        // ✅ LIMITAR tamaño máximo
        if (length <= 0 || length > 30000) {  // ← Reducir de 100KB a 30KB
            _lastError = "Tamaño inválido o muy grande: " + String(length);
            Serial.println("[ImageDownloader] " + _lastError);
            http.end();
            return false;
        }
        
        // ✅ Verificar memoria disponible antes de malloc
        uint32_t freeHeap = ESP.getFreeHeap();
        Serial.printf("[ImageDownloader] RAM libre: %d bytes\n", freeHeap);
        
        if (freeHeap < length + 10000) {  // Necesitamos espacio extra
            _lastError = "RAM insuficiente";
            Serial.println("[ImageDownloader] " + _lastError);
            http.end();
            return false;
        }
        
        *buffer = (uint8_t*)malloc(length);
        if (!*buffer) {
            _lastError = "malloc() falló";
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
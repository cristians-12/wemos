#ifndef QRCODE_MANAGER_H
#define QRCODE_MANAGER_H

#include <Arduino.h>

class DisplayManager;

class QRCodeManager {
private:
    DisplayManager* _display;
    uint8_t* _qrBuffer;
    int _qrWidth;
    int _qrHeight;

public:
    QRCodeManager();
    ~QRCodeManager();
    
    void setDisplay(DisplayManager* display);
    
    String extractQRUrl(const String& html);
    bool downloadAndDisplayQR(const String& qrUrl);
    bool convertToBitmap(const uint8_t* imageData, size_t imageSize);

private:
    void freeBuffer();
};

#endif
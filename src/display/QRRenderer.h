#ifndef QR_RENDERER_H
#define QR_RENDERER_H

#include <qrcode.h>
#include "DisplayManager.h"

class QRRenderer {
public:
    QRRenderer(DisplayManager& display, int minScale, int maxScale);
    
    bool render(const String& data);
    
private:
    DisplayManager& _display;
    int _minScale;
    int _maxScale;
    
    uint8_t calculateVersion(int dataLength) const;
    int calculateScale(int qrSize) const;
    void drawQRCode(const QRCode& qrcode, int scale);
};

#endif
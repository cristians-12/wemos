#include "QRRenderer.h"

QRRenderer::QRRenderer(DisplayManager& display, int minScale, int maxScale)
    : _display(display), _minScale(minScale), _maxScale(maxScale) {}

bool QRRenderer::render(const String& data) {
    if (data.length() == 0) {
        Serial.println("[QRRenderer] Error: Datos vacíos");
        return false;
    }
    
    uint8_t version = calculateVersion(data.length());
    
    QRCode qrcode;
    uint8_t qrcodeData[qrcode_getBufferSize(version)];
    
    qrcode_initText(&qrcode, qrcodeData, version, ECC_LOW, data.c_str());
    
    Serial.printf("[QRRenderer] Versión: %d, Tamaño: %d\n", version, qrcode.size);
    
    int scale = calculateScale(qrcode.size);
    
    _display.clear(TFT_WHITE);
    drawQRCode(qrcode, scale);
    
    Serial.println("[QRRenderer] QR renderizado exitosamente");
    return true;
}

uint8_t QRRenderer::calculateVersion(int dataLength) const {
    if (dataLength <= 50) return 3;
    if (dataLength <= 80) return 4;
    return 5;
}

int QRRenderer::calculateScale(int qrSize) const {
    int maxScale = min(_display.getWidth() / qrSize, _display.getHeight() / qrSize);
    int scale = min(maxScale, _maxScale);
    return max(scale, _minScale);
}

void QRRenderer::drawQRCode(const QRCode& qrcode, int scale) {
    int qrPixelSize = qrcode.size * scale;
    int offsetX = (_display.getWidth() - qrPixelSize) / 2;
    int offsetY = (_display.getHeight() - qrPixelSize) / 2;
    
    TFT_eSPI& tft = _display.getTFT();
    
    for (uint8_t y = 0; y < qrcode.size; y++) {
        for (uint8_t x = 0; x < qrcode.size; x++) {
            uint16_t color = qrcode_getModule(&qrcode, x, y) ? TFT_BLACK : TFT_WHITE;
            tft.fillRect(offsetX + x * scale, offsetY + y * scale, scale, scale, color);
        }
        yield();
    }
}
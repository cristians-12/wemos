#include "ImageRenderer.h"

// Variable estática para acceder al display en callback
static TFT_eSPI* _staticTFT = nullptr;
static int _xOffset = 0;
static int _yOffset = 0;

ImageRenderer::ImageRenderer(DisplayManager& display)
    : _display(display) {
    _staticTFT = &_display.getTFT();
}

bool ImageRenderer::renderPNG(uint8_t* pngData, size_t length) {
    if (!pngData || length == 0) {
        Serial.println("[ImageRenderer] Error: Datos inválidos");
        return false;
    }
    
    _display.clear(TFT_WHITE);
    
    int result = _png.openRAM(pngData, length, pngDrawCallback);
    
    if (result == PNG_SUCCESS) {
        Serial.printf("[ImageRenderer] PNG: %dx%d, %d bpp\n", 
                     _png.getWidth(), _png.getHeight(), _png.getBpp());
        
        // Centrar imagen
        _xOffset = (_display.getWidth() - _png.getWidth()) / 2;
        _yOffset = (_display.getHeight() - _png.getHeight()) / 2;
        
        result = _png.decode(NULL, 0);
        _png.close();
        
        if (result == PNG_SUCCESS) {
            Serial.println("[ImageRenderer] ✓ PNG renderizado");
            return true;
        }
    }
    
    Serial.println("[ImageRenderer] ✗ Error decodificando PNG");
    return false;
}

// Callback estático para PNGdec
void ImageRenderer::pngDrawCallback(PNGDRAW* pDraw) {
    uint16_t lineBuffer[pDraw->iWidth];
    
    _png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
    
    _staticTFT->pushImage(_xOffset, _yOffset + pDraw->y, 
                          pDraw->iWidth, 1, lineBuffer);
}
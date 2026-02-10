#include "ImageRenderer.h"

// Definir variables estáticas
Adafruit_SSD1306* ImageRenderer::_staticDisplay = nullptr;
int ImageRenderer::_xOffset = 0;
int ImageRenderer::_yOffset = 0;
int ImageRenderer::_threshold = 128;  // Umbral de gris (0-255)

ImageRenderer::ImageRenderer(DisplayManager& display)
    : _display(display) {
    _staticDisplay = &_display.getDisplay();
}

bool ImageRenderer::renderPNG(uint8_t* pngData, size_t length) {
    if (!pngData || length == 0) {
        Serial.println("[ImageRenderer] Error: Datos inválidos");
        return false;
    }
    
    // Verificar firma PNG
    if (pngData[0] != 0x89 || pngData[1] != 0x50 || 
        pngData[2] != 0x4E || pngData[3] != 0x47) {
        Serial.println("[ImageRenderer] Error: No es PNG válido");
        Serial.printf("Firma: %02X %02X %02X %02X\n", 
                     pngData[0], pngData[1], pngData[2], pngData[3]);
        return false;
    }
    
    _display.clear();
    
    int result = _png.openRAM(pngData, length, pngDrawCallback);
    
    if (result == PNG_SUCCESS) {
        int imgWidth = _png.getWidth();
        int imgHeight = _png.getHeight();
        
        Serial.printf("[ImageRenderer] PNG: %dx%d\n", imgWidth, imgHeight);
        
        // Centrar imagen en OLED
        _xOffset = (_display.getWidth() - imgWidth) / 2;
        _yOffset = (_display.getHeight() - imgHeight) / 2;
        
        // Si es más grande que la pantalla, ajustar
        if (_xOffset < 0) _xOffset = 0;
        if (_yOffset < 0) _yOffset = 0;
        
        Serial.printf("[ImageRenderer] Offset: (%d, %d)\n", _xOffset, _yOffset);
        
        // Decodificar y mostrar
        result = _png.decode(NULL, 0);
        _png.close();
        
        if (result == PNG_SUCCESS) {
            _display.display();
            Serial.println("[ImageRenderer] ✓ PNG renderizado");
            return true;
        } else {
            Serial.printf("[ImageRenderer] Error decode: %d\n", result);
        }
    } else {
        Serial.printf("[ImageRenderer] Error openRAM: %d\n", result);
    }
    
    return false;
}

// Callback: convierte cada línea PNG a píxeles OLED
int ImageRenderer::pngDrawCallback(PNGDRAW* pDraw) {
    uint16_t lineBuffer[pDraw->iWidth];
    PNG png;
    
    // Obtener línea como RGB565
    png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
    
    if (!_staticDisplay) return 0;
    
    int displayWidth = _staticDisplay->width();
    int displayHeight = _staticDisplay->height();
    
    // Dibujar cada píxel de la línea
    for (int x = 0; x < pDraw->iWidth; x++) {
        int screenX = _xOffset + x;
        int screenY = _yOffset + pDraw->y;
        
        // Verificar límites
        if (screenX >= 0 && screenX < displayWidth && 
            screenY >= 0 && screenY < displayHeight) {
            
            // Convertir RGB565 a escala de grises
            uint16_t pixel = lineBuffer[x];
            uint8_t r = ((pixel >> 11) & 0x1F) * 8;  // 5 bits a 8 bits
            uint8_t g = ((pixel >> 5) & 0x3F) * 4;   // 6 bits a 8 bits
            uint8_t b = (pixel & 0x1F) * 8;          // 5 bits a 8 bits
            
            // Luminosidad (escala de grises)
            uint8_t gray = (r * 30 + g * 59 + b * 11) / 100;
            
            // Convertir a blanco/negro según umbral
            if (gray < _threshold) {
                // Píxel negro del QR → blanco en OLED
                _staticDisplay->drawPixel(screenX, screenY, SSD1306_WHITE);
            }
            // Píxel blanco del QR → negro en OLED (no dibujar nada)
        }
    }
    
    return 1; 
}
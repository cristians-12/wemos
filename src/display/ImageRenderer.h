#ifndef IMAGE_RENDERER_H
#define IMAGE_RENDERER_H

#include <Adafruit_SSD1306.h>
#include <PNGdec.h>
#include "DisplayManager.h"

class ImageRenderer {
public:
    ImageRenderer(DisplayManager& display);
    
    bool renderPNG(uint8_t* pngData, size_t length);
    
private:
    DisplayManager& _display;
    PNG _png;
    
    // Callback para decodificar línea por línea
    static int pngDrawCallback(PNGDRAW* pDraw);
    
    // Variables estáticas para el callback
    static Adafruit_SSD1306* _staticDisplay;
    static int _xOffset;
    static int _yOffset;
    static int _threshold;  // Umbral para blanco/negro
};

#endif
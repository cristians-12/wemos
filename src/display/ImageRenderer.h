#ifndef IMAGE_RENDERER_H
#define IMAGE_RENDERER_H

#include <TFT_eSPI.h>
#include <PNGdec.h>  // o <TJpg_Decoder.h>
#include "DisplayManager.h"

class ImageRenderer {
public:
    ImageRenderer(DisplayManager& display);
    
    bool renderPNG(uint8_t* pngData, size_t length);
    bool renderJPG(uint8_t* jpgData, size_t length);
    
private:
    DisplayManager& _display;
    PNG _png;
    
    static void pngDrawCallback(PNGDRAW* pDraw);
    void drawPNGLine(PNGDRAW* pDraw);
};

#endif
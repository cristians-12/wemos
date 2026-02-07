#include "DisplayManager.h"

DisplayManager::DisplayManager() {}

void DisplayManager::init(int rotation) {
    _tft.init();
    _tft.setRotation(rotation);
    clear();
    Serial.println("[Display] Inicializado");
}

void DisplayManager::showMessage(const String& message, DisplayState state) {
    clear(getColorForState(state));
    
    _tft.setTextColor(TFT_WHITE);
    _tft.setTextSize(2);
    
    centerText(message, _tft.height() / 2 - 10);
    
    Serial.println("[Display] Mensaje: " + message);
}

void DisplayManager::clear(uint16_t color) {
    _tft.fillScreen(color);
}

uint16_t DisplayManager::getColorForState(DisplayState state) const {
    switch (state) {
        case DisplayState::ERROR: return TFT_RED;
        case DisplayState::CONNECTING: return TFT_BLUE;
        case DisplayState::LOADING: return TFT_ORANGE;
        case DisplayState::DISPLAYING: return TFT_WHITE;
        default: return TFT_BLACK;
    }
}

void DisplayManager::centerText(const String& text, int y) {
    int16_t x1, y1;
    uint16_t w, h;
    _tft.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    int x = (_tft.width() - w) / 2;
    _tft.setCursor(x, y);
    _tft.print(text);
}
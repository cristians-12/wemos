#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <TFT_eSPI.h>

enum class DisplayState {
    IDLE,
    CONNECTING,
    LOADING,
    DISPLAYING,
    ERROR
};

class DisplayManager {
public:
    DisplayManager();
    
    void init(int rotation);
    void showMessage(const String& message, DisplayState state);
    void clear(uint16_t color = TFT_BLACK);
    
    TFT_eSPI& getTFT() { return _tft; }
    int getWidth() const { return _tft.width(); }
    int getHeight() const { return _tft.height(); }
    
private:
    TFT_eSPI _tft;
    
    uint16_t getColorForState(DisplayState state) const;
    void centerText(const String& text, int y);
};

#endif
#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

enum class DisplayState {
    IDLE,
    CONNECTING,
    LOADING,
    DISPLAYING,
    ERROR
};

class DisplayManager {
public:
    DisplayManager(int width, int height, int address, int resetPin);
    
    void init();
    void showMessage(const char* message, DisplayState state);  // ← const char* en lugar de String
    void clear();
    void display();
    
    Adafruit_SSD1306& getDisplay() { return _display; }
    int getWidth() { return _display.width(); }
    int getHeight() { return _display.height(); }
    
private:
    Adafruit_SSD1306 _display;
    
    void centerText(const char* text, int y);  // ← const char*
};

#endif
#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Adafruit_SSD1306.h>
#include <Wire.h>

class DisplayManager {
private:
    Adafruit_SSD1306 _display;

public:
    DisplayManager(int width, int height, int address);
    
    bool begin();
    void showMessage(const char* message);
    void showWiFiInfo(const char* ip);
    void showQRCode(const uint8_t* bitmap, int width, int height);  
    void clear();
    
    Adafruit_SSD1306& getDisplay();
};

#endif
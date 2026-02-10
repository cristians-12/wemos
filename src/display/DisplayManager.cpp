#include "DisplayManager.h"
#include "../config/Config.h"

DisplayManager::DisplayManager(int width, int height, int address, int resetPin)
    : _display(width, height, &Wire, resetPin) {}

void DisplayManager::init()
{
    Wire.begin();

    if (!_display.begin(SSD1306_SWITCHCAPVCC, Config::OLED_ADDRESS))
    {
        Serial.println(F("[Display] Error init"));
        for (;;);
    }

    Serial.println(F("[Display] OK")); 

    _display.clearDisplay();
    _display.setTextColor(SSD1306_WHITE);
    _display.setTextSize(1);
    _display.display();
}

void DisplayManager::showMessage(const char* message, DisplayState state)
{
    clear();
    _display.setTextSize(1);
    _display.setTextColor(SSD1306_WHITE);
    centerText(message, _display.height() / 2 - 4);
    display();
    
    Serial.print(F("[Display] "));
    Serial.println(message);
}

void DisplayManager::clear()
{
    _display.clearDisplay();
}

void DisplayManager::display()
{
    _display.display();
}

void DisplayManager::centerText(const char* text, int y)
{
    int16_t x1, y1;
    uint16_t w, h;

    _display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    int x = (_display.width() - w) / 2;

    if (x < 0) x = 0;

    _display.setCursor(x, y);
    _display.print(text);
}
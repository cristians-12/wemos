#include "DisplayManager.h"
#include "../config/Config.h"
#include <Arduino.h>

DisplayManager::DisplayManager(int width, int height, int address)
    : _display(width, height, &Wire, -1) {}

bool DisplayManager::begin()
{
    Wire.begin();

    if (!_display.begin(SSD1306_SWITCHCAPVCC, Config::OLED_ADDRESS))
    {
        Serial.println(F("Error: OLED no encontrado"));
        return false;
    }

    Serial.println(F("OLED inicializado"));
    _display.clearDisplay();
    _display.display();
    return true;
}

void DisplayManager::showMessage(const char *message)
{
    _display.clearDisplay();
    _display.setTextSize(2);
    _display.setTextColor(SSD1306_WHITE);
    _display.setCursor(10, 25);
    _display.print(message);
    _display.display();

    Serial.println(message);
}

void DisplayManager::showWiFiInfo(const char *ip)
{
    _display.clearDisplay();
    _display.setTextSize(1);
    _display.setCursor(0, 0);
    _display.println(F("WiFi: OK"));
    _display.println();
    _display.print(F("IP: "));
    _display.println(ip);
    _display.display();
}

void DisplayManager::clear()
{
    _display.clearDisplay();
    _display.display();
}
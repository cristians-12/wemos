#ifndef CONFIG_H
#define CONFIG_H

namespace Config {
    constexpr const char* WIFI_SSID = "FAMILIA SANCHEZ";
    constexpr const char* WIFI_PASSWORD = "12051004863507";
    constexpr int WIFI_TIMEOUT_MS = 15000;
    
    constexpr const char* API_URL = "https://facturacion.academiadesarrollodesoftware.com/capturar.php";
    constexpr int HTTP_TIMEOUT_MS = 15000;
    constexpr int UPDATE_INTERVAL_MS = 30000;
    
    constexpr int OLED_WIDTH = 128;
    constexpr int OLED_HEIGHT = 64;
    constexpr int OLED_ADDRESS = 0x3C;
    constexpr int OLED_RESET_PIN = -1;
    
    constexpr int QR_VERSION = 3;
    constexpr int QR_SCALE = 2; 
    

    constexpr bool DEBUG_ENABLED = true;
    constexpr int SERIAL_BAUD = 115200;
}

#endif
#ifndef CONFIG_H
#define CONFIG_H

namespace Config {
    constexpr const char* WIFI_SSID = "TU_WIFI";
    constexpr const char* WIFI_PASSWORD = "TU_PASSWORD";
    constexpr int WIFI_TIMEOUT_MS = 15000;
    
    constexpr const char* API_URL = "https://facturacion.academiadesarrollodesoftware.com/capturar.php";
    constexpr int HTTP_TIMEOUT_MS = 10000;
    constexpr int UPDATE_INTERVAL_MS = 30000;
    
    constexpr int TFT_ROTATION = 1;
    constexpr int QR_MIN_SCALE = 2;
    constexpr int QR_MAX_SCALE = 6;
    constexpr int QR_VERSION_DEFAULT = 3;
    constexpr int QR_MAX_DATA_LENGTH = 120;
    
    constexpr bool DEBUG_ENABLED = true;
    constexpr int SERIAL_BAUD = 115200;
}

#endif
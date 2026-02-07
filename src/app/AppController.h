// AppController.h
#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#include <Arduino.h>
#include "../network/WiFiManager.h"
#include "../network/ImageDownloader.h"
#include "../display/DisplayManager.h"
#include "../display/ImageRenderer.h" 

enum class AppState {
    INIT,
    CONNECTING_WIFI,
    DOWNLOADING_QR,
    DISPLAYING_QR,
    ERROR,
    IDLE
};

class AppController {
public:
    AppController();
    
    void init();
    void update();
    
private:
    WiFiManager _wifiManager;
    ImageDownloader _imageDownloader;
    DisplayManager _displayManager;
    ImageRenderer _imageRenderer;
    
    AppState _currentState;
    unsigned long _lastUpdateTime;
    
    uint8_t* _imageBuffer;
    size_t _imageLength;
    
    void handleStateInit();
    void handleStateConnectingWiFi();
    void handleStateDownloadingQR();
    void handleStateDisplayingQR();
    void handleStateError();
    void handleStateIdle();
    
    void transitionTo(AppState newState);
    void cleanupImageBuffer();
};

#endif
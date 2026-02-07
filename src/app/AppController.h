#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#include "../network/WiFiManager.h"
#include "../network/HTTPClient.h"
#include "../display/DisplayManager.h"
#include "../display/QRRenderer.h"


enum class AppState {
    INIT,
    CONNECTING_WIFI,
    FETCHING_DATA,
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
    SecureHTTPClient _httpClient;
    DisplayManager _displayManager;
    QRRenderer _qrRenderer;
    
    AppState _currentState;
    unsigned long _lastUpdateTime;
    String _lastQRData;
    
    void handleStateInit();
    void handleStateConnectingWiFi();
    void handleStateFetchingData();
    void handleStateDisplayingQR();
    void handleStateError();
    void handleStateIdle();
    
    void transitionTo(AppState newState);
};

#endif
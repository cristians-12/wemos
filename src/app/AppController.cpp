#include "AppController.h"
#include "../config/Config.h"

AppController::AppController()
    : _wifiManager(Config::WIFI_SSID, Config::WIFI_PASSWORD, Config::WIFI_TIMEOUT_MS),
      _httpClient(Config::HTTP_TIMEOUT_MS),
      _qrRenderer(_displayManager, Config::QR_MIN_SCALE, Config::QR_MAX_SCALE),
      _currentState(AppState::INIT),
      _lastUpdateTime(0) {}

void AppController::init() {
    Serial.begin(Config::SERIAL_BAUD);
    delay(100);
    Serial.println("\n[App] Iniciando...");
    
    _displayManager.init(Config::TFT_ROTATION);
    transitionTo(AppState::CONNECTING_WIFI);
}

void AppController::update() {
    switch (_currentState) {
        case AppState::INIT:
            handleStateInit();
            break;
        case AppState::CONNECTING_WIFI:
            handleStateConnectingWiFi();
            break;
        case AppState::FETCHING_DATA:
            handleStateFetchingData();
            break;
        case AppState::DISPLAYING_QR:
            handleStateDisplayingQR();
            break;
        case AppState::ERROR:
            handleStateError();
            break;
        case AppState::IDLE:
            handleStateIdle();
            break;
    }
}

void AppController::handleStateInit() {
    transitionTo(AppState::CONNECTING_WIFI);
}

void AppController::handleStateConnectingWiFi() {
    _displayManager.showMessage("Conectando WiFi...", DisplayState::CONNECTING);
    
    if (_wifiManager.connect()) {
        _displayManager.showMessage("WiFi OK!", DisplayState::IDLE);
        _displayManager.showMessage(_wifiManager.getIPAddress(), DisplayState::IDLE);
        delay(2000);
        transitionTo(AppState::FETCHING_DATA);
    } else {
        transitionTo(AppState::ERROR);
    }
}

void AppController::handleStateFetchingData() {
    _displayManager.showMessage("Obteniendo datos...", DisplayState::LOADING);
    
    String response;
    if (_httpClient.get(Config::API_URL, response)) {
        _lastQRData = response;
        transitionTo(AppState::DISPLAYING_QR);
    } else {
        transitionTo(AppState::ERROR);
    }
}

void AppController::handleStateDisplayingQR() {
    if (_qrRenderer.render(_lastQRData)) {
        _lastUpdateTime = millis();
        transitionTo(AppState::IDLE);
    } else {
        transitionTo(AppState::ERROR);
    }
}

void AppController::handleStateError() {
    _displayManager.showMessage("Error", DisplayState::ERROR);
    delay(3000);
    transitionTo(AppState::CONNECTING_WIFI);
}

void AppController::handleStateIdle() {
    if (millis() - _lastUpdateTime >= Config::UPDATE_INTERVAL_MS) {
        transitionTo(AppState::FETCHING_DATA);
    }
}

void AppController::transitionTo(AppState newState) {
    Serial.printf("[App] Estado: %d -> %d\n", (int)_currentState, (int)newState);
    _currentState = newState;
}
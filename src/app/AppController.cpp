// AppController.cpp
#include "AppController.h"
#include "../config/Config.h"

AppController::AppController()
    : _wifiManager(Config::WIFI_SSID, Config::WIFI_PASSWORD, Config::WIFI_TIMEOUT_MS),
      _imageDownloader(Config::HTTP_TIMEOUT_MS),
      _imageRenderer(_displayManager),
      _currentState(AppState::INIT),
      _lastUpdateTime(0),
      _imageBuffer(nullptr),
      _imageLength(0) {}

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
        case AppState::DOWNLOADING_QR:
            handleStateDownloadingQR();
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

void AppController::handleStateConnectingWiFi() {
    _displayManager.showMessage("Conectando WiFi...", DisplayState::CONNECTING);
    
    if (_wifiManager.connect()) {
        _displayManager.showMessage("WiFi OK!", DisplayState::IDLE);
        delay(1000);
        transitionTo(AppState::DOWNLOADING_QR);
    } else {
        transitionTo(AppState::ERROR);
    }
}

void AppController::handleStateDownloadingQR() {
    _displayManager.showMessage("Descargando QR...", DisplayState::LOADING);
    
    cleanupImageBuffer();
    
    if (_imageDownloader.download(Config::API_URL, &_imageBuffer, _imageLength)) {
        Serial.printf("[App] Descargado: %d bytes\n", _imageLength);
        transitionTo(AppState::DISPLAYING_QR);
    } else {
        Serial.println("[App] Error descargando");
        transitionTo(AppState::ERROR);
    }
}

void AppController::handleStateDisplayingQR() {
    _displayManager.showMessage("Mostrando QR...", DisplayState::LOADING);
    delay(500);
    
    if (_imageRenderer.renderPNG(_imageBuffer, _imageLength)) {
        _lastUpdateTime = millis();
        transitionTo(AppState::IDLE);
    } else {
        transitionTo(AppState::ERROR);
    }
}

void AppController::handleStateError() {
    _displayManager.showMessage("Error", DisplayState::ERROR);
    cleanupImageBuffer();
    delay(3000);
    transitionTo(AppState::CONNECTING_WIFI);
}

void AppController::handleStateIdle() {
    if (millis() - _lastUpdateTime >= Config::UPDATE_INTERVAL_MS) {
        cleanupImageBuffer();
        transitionTo(AppState::DOWNLOADING_QR);
    }
}

void AppController::handleStateInit() {
    transitionTo(AppState::CONNECTING_WIFI);
}

void AppController::transitionTo(AppState newState) {
    Serial.printf("[App] Estado: %d -> %d\n", (int)_currentState, (int)newState);
    _currentState = newState;
}

void AppController::cleanupImageBuffer() {
    if (_imageBuffer) {
        _imageDownloader.freeBuffer(_imageBuffer);
        _imageBuffer = nullptr;
        _imageLength = 0;
    }
}
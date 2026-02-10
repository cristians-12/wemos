#include "AppController.h"

AppController::AppController()
    : _wifiManager(Config::WIFI_SSID, Config::WIFI_PASSWORD, Config::WIFI_TIMEOUT_MS),
      _imageDownloader(Config::HTTP_TIMEOUT_MS),
      _displayManager(Config::OLED_WIDTH, Config::OLED_HEIGHT,
                      Config::OLED_ADDRESS, Config::OLED_RESET_PIN),
      _imageRenderer(_displayManager),
      _currentState(AppState::INIT),
      _lastUpdateTime(0),
      _imageBuffer(nullptr),
      _imageLength(0) {}

void AppController::init()
{
    Serial.begin(Config::SERIAL_BAUD);
    delay(100);
    Serial.println("\n[App] Iniciando...");

    _displayManager.init();
    // transitionTo(AppState::CONNECTING_WIFI);
}

void AppController::update()
{
    switch (_currentState)
    {
    case AppState::INIT:
        handleStateInit();
        break;
    case AppState::CONNECTING_WIFI:
        handleStateConnectingWiFi();
        break;
    case AppState::DOWNLOADING_QR:
        handleStateDownloadingQR();
        break;
        // case AppState::DISPLAYING_QR:
        //     handleStateDisplayingQR();
        //     break;
        // case AppState::ERROR:
        //     handleStateError();
        //     break;
        // case AppState::IDLE:
        //     handleStateIdle();
        //     break;
    }
}

void AppController::handleStateInit()
{
    transitionTo(AppState::CONNECTING_WIFI);
}

void AppController::handleStateConnectingWiFi()
{
    _displayManager.showMessage("WiFi...", DisplayState::CONNECTING);

    if (_wifiManager.connect())
    {
        _displayManager.showMessage("OK!", DisplayState::IDLE);
        delay(1000);
        transitionTo(AppState::DOWNLOADING_QR);
    }
    else
    {
        transitionTo(AppState::ERROR);
    }
}

void AppController::handleStateDownloadingQR()
{
    _displayManager.showMessage("Descarga...", DisplayState::LOADING);

    cleanupImageBuffer();

    if (_imageDownloader.download(Config::API_URL, &_imageBuffer, _imageLength))
    {
        Serial.printf("[App] Imagen: %d bytes\n", _imageLength);
        transitionTo(AppState::DISPLAYING_QR);
    }
    else
    {
        Serial.println(F("[App] Error descarga"));
        transitionTo(AppState::ERROR);
    }
}

void AppController::handleStateDisplayingQR()
{
    _displayManager.showMessage("Mostrando...", DisplayState::LOADING);
    delay(500);

    if (_imageRenderer.renderPNG(_imageBuffer, _imageLength))
    {
        _lastUpdateTime = millis();
        transitionTo(AppState::IDLE);
    }
    else
    {
        Serial.println(F("[App] Error render"));
        transitionTo(AppState::ERROR);
    }
}

void AppController::handleStateError()
{
    _displayManager.showMessage("Error", DisplayState::ERROR);
    cleanupImageBuffer();
    delay(3000);
    transitionTo(AppState::CONNECTING_WIFI);
}

void AppController::handleStateIdle()
{
    if (millis() - _lastUpdateTime >= Config::UPDATE_INTERVAL_MS)
    {
        cleanupImageBuffer();
        transitionTo(AppState::DOWNLOADING_QR);
    }
}

void AppController::transitionTo(AppState newState)
{
    Serial.printf("[App] Estado: %d -> %d\n", (int)_currentState, (int)newState);
    _currentState = newState;
}

void AppController::cleanupImageBuffer()
{
    if (_imageBuffer)
    {
        _imageDownloader.freeBuffer(_imageBuffer);
        _imageBuffer = nullptr;
        _imageLength = 0;
        Serial.println("[App] Buffer liberado");
    }
}
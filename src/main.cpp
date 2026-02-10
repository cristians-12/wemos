#include <Arduino.h>
#include "config/Config.h"
#include "display/DisplayManager.h"
#include "network/NetworkManager.h"
#include "network/QRCodeManager.h"

DisplayManager display(Config::OLED_WIDTH, Config::OLED_HEIGHT, Config::OLED_ADDRESS);
NetworkManager network(Config::WIFI_SSID, Config::WIFI_PASSWORD,
                       Config::API_URL, Config::WIFI_TIMEOUT_MS,
                       Config::HTTP_TIMEOUT_MS);
QRCodeManager qrManager;

bool firstApiCallDone = false;

void setup()
{

    Serial.begin(Config::SERIAL_BAUD);
    delay(2000);

    if (!display.begin())
    {
        Serial.println(F("Error al inicializar display"));
        for (;;)
            ;
    }

    network.setDisplay(&display);
    qrManager.setDisplay(&display);

    if (network.connectWiFi())
    {
        display.showMessage("Conectado");
        delay(2000);
        display.showWiFiInfo(network.getIP().c_str());
        Serial.println(F("Setup completo ✓\n"));
    }
    else
    {
        for (;;)
        {
            delay(1000);
        }
    }
}

void loop()
{
    static unsigned long lastWiFiCheck = 0;
    static unsigned long lastApiCall = 0;

    // Primera petición después de 3 segundos
    if (!firstApiCallDone && millis() > 3000)
    {
        Serial.println(F("\n>>> Obteniendo QR <<<"));

        String htmlResponse;
        if (network.httpGet(htmlResponse))
        {
            Serial.println(F("✓ HTML recibido"));
            Serial.println(F("========== RESPUESTA COMPLETA =========="));
            Serial.println(htmlResponse); // ← Ver respuesta completa
            Serial.println(F("========================================"));

            // Extraer URL del QR
            String qrUrl = qrManager.extractQRUrl(htmlResponse);

            if (qrUrl.length() > 0)
            {
                Serial.printf("URL extraída: %s\n", qrUrl.c_str());

                // Descargar y mostrar QR
                if (qrManager.downloadAndDisplayQR(qrUrl))
                {
                    Serial.println(F("✓ QR mostrado en pantalla"));
                }
                else
                {
                    Serial.println(F("✗ Error al procesar QR"));
                    display.showMessage("QR Err");
                }
            }
            else
            {
                Serial.println(F("✗ No se pudo extraer URL"));
                display.showMessage("URL Err");
            }
        }
        else
        {
            // ← AGREGAR ESTO
            Serial.println(F("✗ Error en petición HTTP"));
            Serial.printf("Código HTTP: %d\n", network.getLastHttpCode());
            Serial.printf("Error: %s\n", network.getLastError().c_str());
        }

        firstApiCallDone = true;
        lastApiCall = millis();
    }

    // Verificar WiFi cada 5 segundos
    if (millis() - lastWiFiCheck > 5000)
    {
        network.checkConnection();
        lastWiFiCheck = millis();
    }

    if (firstApiCallDone && (millis() - lastApiCall > 120000))
    {
        Serial.println(F("\n>>> Actualizando QR <<<"));

        String htmlResponse;
        if (network.httpGet(htmlResponse))
        {
            String qrUrl = qrManager.extractQRUrl(htmlResponse);
            if (qrUrl.length() > 0)
            {
                qrManager.downloadAndDisplayQR(qrUrl);
            }
        }

        lastApiCall = millis();
    }

    delay(100);
    yield();
}
#include <Arduino.h>
#include "config/Config.h"
#include "display/DisplayManager.h"
#include "network/NetworkManager.h"
#include "network/AttendanceManager.h"

DisplayManager display(Config::OLED_WIDTH, Config::OLED_HEIGHT, Config::OLED_ADDRESS);
NetworkManager network(Config::WIFI_SSID, Config::WIFI_PASSWORD, 
                       Config::API_URL, Config::WIFI_TIMEOUT_MS, 
                       Config::HTTP_TIMEOUT_MS);
AttendanceManager attendance(Config::CEDULA);

bool firstMarkDone = false;

void setup() {
    Serial.begin(Config::SERIAL_BAUD);
    delay(2000);
    
    Serial.println(F("\n╔════════════════════════════════════════╗"));
    Serial.println(F("║   Sistema de Asistencia Automático    ║"));
    Serial.println(F("╚════════════════════════════════════════╝\n"));
    
    Serial.printf("Cédula configurada: %s\n", Config::CEDULA);
    Serial.printf("RAM libre: %d bytes\n\n", ESP.getFreeHeap());

    if (!display.begin()) {
        Serial.println(F("✗ Error al inicializar display"));
        for(;;);
    }

    network.setDisplay(&display);
    attendance.setNetwork(&network);
    attendance.setDisplay(&display);
    
    if (network.connectWiFi()) {
        display.showMessage("WiFi OK");
        delay(2000);
        display.showWiFiInfo(network.getIP().c_str());
        Serial.println(F("✓ Setup completo\n"));
    } else {
        for(;;) {
            delay(1000);
        }
    }
}

void loop() {
    static unsigned long lastWiFiCheck = 0;
    static unsigned long lastMark = 0;

    // Primera marcación después de 3 segundos
    if (!firstMarkDone && millis() > 3000) {
        if (attendance.markAttendance()) {
            Serial.println(F("\n✓ Primera marcación exitosa"));
            delay(3000);
            display.showWiFiInfo(network.getIP().c_str());
        } else {
            Serial.println(F("\n✗ Error en primera marcación"));
            delay(3000);
        }
        
        firstMarkDone = true;
        lastMark = millis();
    }

    // Verificar WiFi cada 5 segundos
    if (millis() - lastWiFiCheck > 5000) {
        network.checkConnection();
        lastWiFiCheck = millis();
    }

    // Marcar asistencia cada 2 minutos (120000ms)
    if (firstMarkDone && (millis() - lastMark > 120000)) {
        Serial.println(F("\n>>> Marcación periódica <<<"));
        
        if (attendance.markAttendance()) {
            delay(3000);
            display.showWiFiInfo(network.getIP().c_str());
        }
        
        lastMark = millis();
    }

    delay(100);
    yield();
}
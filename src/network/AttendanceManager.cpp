#include "AttendanceManager.h"
#include "NetworkManager.h"
#include "../display/DisplayManager.h"

AttendanceManager::AttendanceManager(const char* cedula)
    : _network(nullptr), _display(nullptr), _cedula(cedula) {}

void AttendanceManager::setNetwork(NetworkManager* network) {
    _network = network;
}

void AttendanceManager::setDisplay(DisplayManager* display) {
    _display = display;
}

String AttendanceManager::extractValidationUrl(const String& html) {
    Serial.println(F("\n=== Extrayendo URL de validación ==="));
    
    // El QR contiene: https://facturacion.../validar.php?token=XXXXX
    // Buscar en: data=URL dentro del src del QR
    
    int dataStart = html.indexOf("data=");
    if (dataStart == -1) {
        Serial.println(F("✗ No se encontró 'data=' en HTML"));
        return "";
    }
    
    dataStart += 5; // Saltar "data="
    
    // Buscar el final (antes de &size=)
    int dataEnd = html.indexOf("&size=", dataStart);
    if (dataEnd == -1) {
        dataEnd = html.indexOf("'", dataStart);
        if (dataEnd == -1) {
            dataEnd = html.indexOf("\"", dataStart);
        }
    }
    
    if (dataEnd == -1) {
        Serial.println(F("✗ No se encontró fin de URL"));
        return "";
    }
    
    String validationUrl = html.substring(dataStart, dataEnd);
    
    // URL decode si es necesario (reemplazar %3A por :, etc)
    validationUrl.replace("%3A", ":");
    validationUrl.replace("%2F", "/");
    validationUrl.replace("%3F", "?");
    validationUrl.replace("%3D", "=");
    
    Serial.println(F("✓ URL de validación extraída:"));
    Serial.println(validationUrl);
    
    return validationUrl;
}

bool AttendanceManager::markAttendance() {
    if (!_network || !_display) {
        Serial.println(F("✗ Network o Display no configurado"));
        return false;
    }
    
    Serial.println(F("\n╔════════════════════════════════════════╗"));
    Serial.println(F("║   MARCANDO ASISTENCIA                  ║"));
    Serial.println(F("╚════════════════════════════════════════╝"));
    
    _display->showMessage("Obtenien\ntoken...");
    
    // Paso 1: Obtener HTML con el token
    String htmlResponse;
    if (!_network->httpGet(htmlResponse)) {
        Serial.println(F("✗ Error obteniendo token"));
        _display->showMessage("Token\nError");
        return false;
    }
    
    Serial.println(F("✓ HTML recibido"));
    
    // Paso 2: Extraer URL de validación
    String validationUrl = extractValidationUrl(htmlResponse);
    
    if (validationUrl.length() == 0) {
        Serial.println(F("✗ No se pudo extraer URL"));
        _display->showMessage("URL\nError");
        return false;
    }
    
    // Paso 3: Preparar datos POST
    _display->showMessage("Enviando\ncedula..");
    
    // El formulario envía: cedula=XXXXXXX
    String postData = "cedula=";
    postData += _cedula;
    
    Serial.println(F("\n--- Enviando POST ---"));
    Serial.printf("URL: %s\n", validationUrl.c_str());
    Serial.printf("Data: %s\n", postData.c_str());
    
    // Paso 4: Hacer POST a la URL de validación
    String response;
    
    // Necesitamos hacer POST a una URL específica, así que usamos httpPostToUrl
    // Vamos a agregar este método al NetworkManager
    
    // Por ahora, hacemos un wrapper temporal
    if (!_network->httpPostToUrl(validationUrl, postData, response)) {
        Serial.println(F("✗ Error en POST"));
        Serial.printf("Código: %d\n", _network->getLastHttpCode());
        Serial.printf("Error: %s\n", _network->getLastError().c_str());
        _display->showMessage("POST\nError");
        return false;
    }
    
    // Paso 5: Analizar respuesta
    Serial.println(F("\n✓ Respuesta recibida:"));
    Serial.println(response);
    
    // Verificar si fue exitoso
    if (response.indexOf("exitoso") != -1 || 
        response.indexOf("success") != -1 ||
        response.indexOf("registrado") != -1) {
        
        Serial.println(F("\n✓✓✓ ASISTENCIA MARCADA ✓✓✓"));
        _display->showMessage("Asisten\nOK!");
        return true;
        
    } else if (response.indexOf("error") != -1 || 
               response.indexOf("invalid") != -1) {
        
        Serial.println(F("\n✗ Error en validación"));
        _display->showMessage("Validac\nError");
        return false;
        
    } else {
        Serial.println(F("\n⚠ Respuesta desconocida"));
        _display->showMessage("Respues\n???");
        return false;
    }
}
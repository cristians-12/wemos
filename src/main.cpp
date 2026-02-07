#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <TFT_eSPI.h>
#include <qrcode.h>

TFT_eSPI tft = TFT_eSPI();

const char* ssid = "TU_WIFI";
const char* password = "TU_PASSWORD";
const char* serverUrl = "https://facturacion.academiadesarrollodesoftware.com/capturar.php";

void setup() {
    Serial.begin(115200);
    delay(100);
    Serial.println("\n\nIniciando...");
    
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 10);
    tft.println("Conectando WiFi...");
    
    // Conectar WiFi con timeout
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 30) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n✓ WiFi conectado");
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
        
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(10, 10);
        tft.println("WiFi OK!");
        tft.setCursor(10, 30);
        tft.setTextSize(1);
        tft.println(WiFi.localIP());
        delay(2000);
        
        // Obtener datos del QR
        String qrData = getQRData();
        
        if (qrData.length() > 0) {
            // Generar y mostrar QR
            displayQR(qrData);
        } else {
            showError("No hay datos");
        }
    } else {
        Serial.println("\n✗ Error WiFi");
        showError("Error WiFi");
    }
}

// Petición GET para obtener datos (HTTPS)
String getQRData() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi no conectado");
        return "";
    }
    
    WiFiClientSecure client;
    HTTPClient http;
    String payload = "";
    client.setInsecure();
    
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(1);
    tft.setCursor(5, 5);
    tft.println("Obteniendo datos...");
    
    Serial.println("Conectando a servidor...");
    
    http.begin(client, serverUrl);
    http.setTimeout(10000);
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    
    int httpCode = http.GET();
    
    Serial.print("Código HTTP: ");
    Serial.println(httpCode);
    
    if (httpCode == HTTP_CODE_OK || httpCode == 200) {
        payload = http.getString();
        Serial.println("✓ Datos recibidos: " + payload);
        Serial.print("Longitud: ");
        Serial.println(payload.length());
    } else if (httpCode > 0) {
        Serial.println("✗ Error HTTP: " + String(httpCode));
        showError("HTTP " + String(httpCode));
    } else {
        Serial.println("✗ Error de conexión: " + http.errorToString(httpCode));
        showError("Sin conexion");
    }
    
    http.end();
    return payload;
}

void displayQR(String data) {
    if (data.length() == 0) {
        showError("Datos vacios");
        return;
    }
    
    tft.fillScreen(TFT_WHITE);
    uint8_t version = 3;
    if (data.length() > 50) version = 4;
    if (data.length() > 80) version = 5;
    if (data.length() > 120) {
        Serial.println("⚠️ Datos muy largos, truncando...");
        data = data.substring(0, 120);
        version = 5;
    }
    
    QRCode qrcode;
    uint8_t qrcodeData[qrcode_getBufferSize(version)];
    
    qrcode_initText(&qrcode, qrcodeData, version, ECC_LOW, data.c_str());
    
    Serial.print("QR Size: ");
    Serial.print(qrcode.size);
    Serial.print(" Versión: ");
    Serial.println(version);
    int tftWidth = tft.width();
    int tftHeight = tft.height();
    
    int maxScale = min(tftWidth / qrcode.size, tftHeight / qrcode.size);
    int scale = min(maxScale, 6);
    scale = max(scale, 2);
    
    int qrPixelSize = qrcode.size * scale;
    int offsetX = (tftWidth - qrPixelSize) / 2;
    int offsetY = (tftHeight - qrPixelSize) / 2;
    
    Serial.print("Escala: ");
    Serial.print(scale);
    Serial.print(" Offset X: ");
    Serial.print(offsetX);
    Serial.print(" Y: ");
    Serial.println(offsetY);
    
    // Dibujar QR
    for (uint8_t y = 0; y < qrcode.size; y++) {
        for (uint8_t x = 0; x < qrcode.size; x++) {
            uint16_t color = qrcode_getModule(&qrcode, x, y) ? TFT_BLACK : TFT_WHITE;
            tft.fillRect(offsetX + x*scale, offsetY + y*scale, scale, scale, color);
        }
        yield();
    }
    
    Serial.println("✓ QR mostrado en pantalla");
}

void showError(String msg) {
    tft.fillScreen(TFT_RED);
    tft.setTextColor(TFT_WHITE, TFT_RED);
    tft.setTextSize(2);
    tft.setCursor(10, tft.height() / 2 - 10);
    tft.println("ERROR:");
    tft.setCursor(10, tft.height() / 2 + 10);
    tft.println(msg);
    Serial.println("ERROR: " + msg);
}

void loop() {
    delay(30000);
    
    Serial.println("\n--- Actualizando QR ---");
    String qrData = getQRData();
    
    if (qrData.length() > 0) {
        displayQR(qrData);
    } else {
        showError("Sin datos");
    }
}
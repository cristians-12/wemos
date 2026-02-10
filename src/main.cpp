#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "config/Config.h"

Adafruit_SSD1306 display(Config::OLED_WIDTH,
                         Config::OLED_HEIGHT,
                         &Wire,
                         -1);

void showMessage(const char *message)
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 25);
  display.print(message);
  display.display();

  Serial.println(message);
}

bool connect()
{
  showMessage("WiFi...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(Config::WIFI_SSID, Config::WIFI_PASSWORD);

  unsigned long startTime = millis();

  while (WiFi.status() != WL_CONNECTED)
  {
    if (millis() - startTime > Config::WIFI_TIMEOUT_MS)
    {
      Serial.println(F("\nTimeout WiFi"));
      showMessage("WiFi\nError");
      return false;
    }
    delay(500);
    Serial.print(".");
  }

  Serial.println(F("\nWiFi conectado!"));
  Serial.print(F("IP: "));
  Serial.println(WiFi.localIP());

  return true;
}

void setup()
{
  Serial.begin(Config::SERIAL_BAUD);
  delay(1000);

  Serial.println(F("\n=== Inicio ==="));
  Serial.printf("RAM libre: %d bytes\n", ESP.getFreeHeap());

  // Inicializar I2C y OLED
  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, Config::OLED_ADDRESS))
  {
    Serial.println(F("Error: OLED no encontrado"));
    for (;;)
      ;
  }

  Serial.println(F("OLED inicializado"));

  display.clearDisplay();
  display.display();

  // Conectar WiFi
  if (connect())
  {
    showMessage("Conectado");

    // Mostrar IP en la pantalla
    delay(2000);
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println(F("WiFi: OK"));
    display.println();
    display.print(F("IP:"));
    display.println(WiFi.localIP());
    display.display();

    Serial.println(F("=== Setup completo ==="));
  }
  else
  {
    Serial.println(F("Error: No se pudo conectar"));
    for (;;)
    {
      delay(1000);
    }
  }
}

void loop()
{
  // Verificar conexión cada 5 segundos
  static unsigned long lastCheck = 0;

  if (millis() - lastCheck > 5000)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println(F("WiFi: Conectado"));
    }
    else
    {
      Serial.println(F("WiFi: Desconectado, reconectando..."));
      showMessage("Recon...");
      connectWiFi();
    }
    lastCheck = millis();
  }

  delay(100);
  yield();
}
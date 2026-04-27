#pragma once

// ─── Pines RC522 ─────────────────────────────────────
#define RC522_SS_PIN    D8    // SDA/SS
#define RC522_RST_PIN   D3    // RST

// ─── Pines LEDs ──────────────────────────────────────
#define LED_GREEN_PIN   D1    // Acceso permitido
#define LED_RED_PIN     D2    // Acceso denegado
#define LED_ON_TIME     2000  // ms que permanece encendido

// ─── WiFi ─────────────────────────────────────────────
#define WIFI_SSID       "TuSSID"
#define WIFI_PASS       "TuPassword"

// ─── API ──────────────────────────────────────────────
#define API_BASE_URL    "http://api.ejemplo.com"
#define API_ACCESS      "/api/v1/access"
#define API_TIMEOUT     5000  // ms
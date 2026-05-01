#pragma once

// ─── Pines RC522 ─────────────────────────────────────
#define RC522_SS_PIN    D8    // SDA/SS
#define RC522_RST_PIN   D0    // RST (cambiado de D3 para evitar conflicto con GPIO0)

// ─── Pines LEDs ──────────────────────────────────────
#define LED_GREEN_PIN   D1    // Acceso permitido
#define LED_RED_PIN     D2    // Acceso denegado
#define LED_ON_TIME     2000  // ms que permanece encendido

// ─── WiFi ─────────────────────────────────────────────
#define WIFI_SSID       "FAMILIA SANCHEZ"
#define WIFI_PASS       "12051004863507"

// ─── API ──────────────────────────────────────────────
#define API_BASE_URL    "https://cgcnurqwbywszdwxdavm.supabase.co/functions/v1/"
#define API_ACCESS      "/hours"
#define API_TIMEOUT     5000  // ms
#pragma once

// ─── Pines RC522 ─────────────────────────────────────
#define RC522_SS_PIN    D8    // SDA/SS
#define RC522_RST_PIN   D0    // RST (cambiado de D3 para evitar conflicto con GPIO0)

// ─── Pines LEDs ──────────────────────────────────────
#define LED_GREEN_PIN   D1    // Acceso permitido
#define LED_RED_PIN     D4    // Acceso denegado
#define LED_ON_TIME     2000  // ms que permanece encendido

// ─── WiFi ─────────────────────────────────────────────
#define WIFI_SSID       "FAMILIA SANCHEZ"
#define WIFI_PASS       "12051004863507"

// ─── API ──────────────────────────────────────────────
#define API_BASE_URL    "https://cgcnurqwbywszdwxdavm.supabase.co/functions/v1"
#define API_ACCESS      "/hours"
#define API_TIMEOUT     5000  //
#define API_KEY "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImNnY251cnF3Ynl3c3pkd3hkYXZtIiwicm9sZSI6ImFub24iLCJpYXQiOjE3Nzc1OTkwNTgsImV4cCI6MjA5MzE3NTA1OH0.mnlREoUGBFaGxVBVw5XjowketQ6bmd57yv-kgoBuG8c"
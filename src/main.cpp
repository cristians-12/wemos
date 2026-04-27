#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>

IRrecv irrecv(RECV_PIN);
IRsend irsend(IR_LED_PIN);
decode_results results;

uint16_t currentFrequency = 38; // Frecuencia inicial en kHz

void setup() {
  Serial.begin(115200);
  delay(10);

  irrecv.enableIRIn();
  irsend.begin();
  Serial.println("IR Receiver y Emisor listos.");
  Serial.println("Comandos disponibles:");
  Serial.println("f36 - Cambiar frecuencia a 36 kHz");
  Serial.println("f38 - Cambiar frecuencia a 38 kHz");
  Serial.println("f40 - Cambiar frecuencia a 40 kHz");
  Serial.print("Frecuencia actual: ");
  Serial.print(currentFrequency);
  Serial.println(" kHz");
}

void loop() {
  // Verificar si hay entrada del usuario en el Monitor Serie
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim(); // Eliminar espacios o saltos de línea
    if (command == "f36") {
      currentFrequency = 36;
      Serial.print("Frecuencia cambiada a: ");
      Serial.print(currentFrequency);
      Serial.println(" kHz");
    } else if (command == "f38") {
      currentFrequency = 38;
      Serial.print("Frecuencia cambiada a: ");
      Serial.print(currentFrequency);
      Serial.println(" kHz");
    } else if (command == "f40") {
      currentFrequency = 40;
      Serial.print("Frecuencia cambiada a: ");
      Serial.print(currentFrequency);
      Serial.println(" kHz");
    } else {
      Serial.println("Comando no reconocido. Use f36, f38 o f40.");
    }
  }

  // Verificar si se recibió una señal IR
  if (irrecv.decode(&results)) {
    // Mostrar el protocolo detectado
    Serial.print("Protocolo: ");
    switch (results.decode_type) {
      case NEC: Serial.println("NEC"); break;
      case SONY: Serial.println("SONY"); break;
      case RC5: Serial.println("RC5"); break;
      case RC6: Serial.println("RC6"); break;
      case PANASONIC: Serial.println("PANASONIC"); break;
      case LG: Serial.println("LG"); break;
      case JVC: Serial.println("JVC"); break;
      case MITSUBISHI: Serial.println("MITSUBISHI"); break;
      case SAMSUNG: Serial.println("SAMSUNG"); break;
      default: Serial.println("Desconocido o no soportado");
    }

    // Mostrar el valor recibido (si es menor a 64 bits)
    Serial.print("Código recibido: 0x");
    Serial.println((unsigned long)results.value, HEX);

    // Mostrar el número de bits
    Serial.print("Número de bits: ");
    Serial.println(results.bits);

    // Mostrar datos en bruto si es necesario
    if (results.decode_type == UNKNOWN || results.bits > 32 || results.decode_type == PANASONIC) {
      Serial.println("Datos en bruto (Raw Data):");
      for (uint16_t i = 0; i < results.rawlen; i++) {
        Serial.print(results.rawbuf[i] * 50); // Usamos 50 microsegundos por tick
        if (i < results.rawlen - 1) Serial.print(", ");
        if (i % 8 == 7) Serial.println(); // Nueva línea cada 8 valores
      }
      Serial.println();
    }

    // Reemitir la señal automáticamente usando datos en bruto
    Serial.println("Reenviando señal IR...");
    uint16_t rawData[results.rawlen];
    for (uint16_t i = 0; i < results.rawlen; i++) {
      rawData[i] = results.rawbuf[i];
    }
    irsend.sendRaw(rawData, results.rawlen, currentFrequency);
    Serial.print("Señal reenviada usando datos en bruto a ");
    Serial.print(currentFrequency);
    Serial.println(" kHz.");
    
    // Esperar un poco para no saturar el receptor del aire acondicionado
    delay(500);

    irrecv.resume();
  }
  delay(100);
}
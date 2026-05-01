#include <Arduino.h>
#include "config.h"
#include "wifi/WiFiManager.h"
#include "http/HttpClient.h"
#include "rfid/RFIDReader.h"
#include "access/AccessController.h"

WiFiManager wifiManager;
HttpClient httpClient;
RFIDReader rfidReader;
AccessController accessController(httpClient);


void setup()
{
    Serial.begin(115200);
    delay(10);

    wifiManager.begin();
    rfidReader.begin();
}

void loop()
{
    wifiManager.reconnectIfNeeded();
    if (rfidReader.cardPresent())
    {
        String uid = rfidReader.getUID();
        Serial.print("Tarjeta detectada - UID: ");
        Serial.println(uid);
    }

    delay(50);
}
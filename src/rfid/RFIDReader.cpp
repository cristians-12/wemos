#include "RFIDReader.h"

void RFIDReader::begin() {
    SPI.begin();
    _mfrc522.PCD_Init();
}

bool RFIDReader::cardPresent() {
    return _mfrc522.PICC_IsNewCardPresent() &&
           _mfrc522.PICC_ReadCardSerial();
}

String RFIDReader::getUID() {
    String uid = "";
    for (byte i = 0; i < _mfrc522.uid.size; i++) {
        if (_mfrc522.uid.uidByte[i] < 0x10) uid += "0";
        uid += String(_mfrc522.uid.uidByte[i], HEX);
        if (i < _mfrc522.uid.size - 1) uid += ":";
    }
    uid.toUpperCase();

    _mfrc522.PICC_HaltA();
    _mfrc522.PCD_StopCrypto1();

    return uid;
}

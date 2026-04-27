#pragma once
#include <MFRC522.h>
#include "config.h"

class RFIDReader {
public:
    void begin();
    bool cardPresent();
    String getUID();

private:
    MFRC522 _mfrc522{RC522_SS_PIN, RC522_RST_PIN};
};
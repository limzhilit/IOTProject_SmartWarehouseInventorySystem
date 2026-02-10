#include "globals.h"

bool waitingForCard = false;
unsigned long cardStartTime;
const unsigned long cardTimeout = 3000;
uint8_t uid[7];
uint8_t uidLength;
bool cardDetected = false;
String lastCardUID = "";

void handleAddItem() {
  String itemName = rfid(0, "");
  server.send(200, "text/plain", itemName);
}

String rfid(int x, String itemName){
  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength)) {
    Serial.println("Found a card");
    Serial.print("UID Value: ");
    nfc.PrintHex(uid, uidLength);
    Serial.println("Trying to authenticate block 4 with default KEYA value");
    uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    if (nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya)) {
      Serial.println("Sector 1 authenticated");
      uint8_t data[16];
      if (x == 1) {
        uint8_t data[16] = {0};
        itemName.toCharArray((char*)data, 16);
        nfc.mifareclassic_WriteDataBlock (4, data);
      }
      if (nfc.mifareclassic_ReadDataBlock(4, data)) {
        Serial.println("Reading Block 4:");
        nfc.PrintHexChar(data, 16);
        Serial.println("");
        String text = String((char*)data);
        Serial.println(text);   
        return text;
      }
      else {
        Serial.println("Ooops ... unable to read the requested block.  Try another key?");
      }
    } else {
      Serial.println("Ooops ... authentication failed: Try another key?");
    }
  }
  return "-1";
}

void handleWaitForCard() {
  Serial.println("Waiting for card...");
  waitingForCard = true;
  cardStartTime = millis();
}

void checkCard() {
  if (!waitingForCard) return;

  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength)) {
    server.send(200, "text/plain", "detected");
    waitingForCard = false;
  } else if (millis() - cardStartTime >= cardTimeout) {
    server.send(408, "text/plain", "timeout");
    waitingForCard = false;
  }
}

void handleAssign() {
  if (server.hasArg("item")) {
    String itemName = server.arg("item");
    rfid(1, itemName);
    server.send(200, "text/plain", "Card assigned: " + itemName);
  } else {
    server.send(400, "text/plain", "Missing 'item' parameter");
  }
}


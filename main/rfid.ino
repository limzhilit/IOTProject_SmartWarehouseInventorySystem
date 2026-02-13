#include "globals.h"

bool waitingForCard = false;
unsigned long cardStartTime;
const unsigned long cardTimeout = 10000;
uint8_t uid[7];
uint8_t uidLength;
bool cardDetected = false;
String lastCardUID = "";

// --- Updated handleAddItem ---
void handleAddItem(AsyncWebServerRequest *request) {
  String itemName = rfid(0, "");
  request->send(200, "text/plain", itemName);
}

String rfid(int mode, String itemName) {
  if (!nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength)) {
    Serial.println("ERROR: cannot read card");
    return "-1";
  }

  uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
  if (!nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya)) {
    return "-1";
  }

  uint8_t data[16] = {0};
  if (mode == 1) {
    itemName.toCharArray((char*)data, 16);
    nfc.mifareclassic_WriteDataBlock(4, data);
  }

  if (!nfc.mifareclassic_ReadDataBlock(4, data)) {
    return "-1";
  }

  return String((char*)data);
}

// --- Updated handleWaitForCard ---
void handleWaitForCard(AsyncWebServerRequest *request) {
  Serial.println("Waiting for card...");
  waitingForCard = true;
  cardStartTime = millis();
  request->send(200, "text/plain", "Waiting started");
}

// --- Updated checkCard (IMPORTANT) ---
void checkCard() {
  if (!waitingForCard) return;

  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength)) {
    // Instead of server.send, we use WebSocket to push data to the UI
    webSocket.broadcastTXT("detected"); 
    waitingForCard = false;
  } 
  else if (millis() - cardStartTime >= cardTimeout) {
    webSocket.broadcastTXT("timeout");
    waitingForCard = false;
  }
}

// --- Updated handleAssign ---
void handleAssign(AsyncWebServerRequest *request) {
  if (request->hasArg("item")) {
    String itemName = request->arg("item");
    rfid(1, itemName);
    request->send(200, "text/plain", "Card assigned: " + itemName);
  } else {
    request->send(400, "text/plain", "Missing 'item' parameter");
  }
}
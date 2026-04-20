#include "globals.h"

unsigned long cardStartTime;
const unsigned long cardTimeout = 10000;
uint8_t uid[7];
uint8_t uidLength = 4;
bool cardDetected = false;
String lastCardUID = "";

String rfid(int mode, String itemName) {

if (uidLength == 0) {
    Serial.println("No valid UID!");
    return "false";
}
  uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
  if (!nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya)) {
    return "false";
  }

  uint8_t data[16] = { 0 };
  if (mode == 1) {
    itemName.toCharArray((char *)data, 16);
    nfc.mifareclassic_WriteDataBlock(4, data);
  }

  if (!nfc.mifareclassic_ReadDataBlock(4, data)) {
    return "false";
  }

  String result = String((char *)data);  // interpret bytes as characters
  Serial.println("Card: " + result);

  return result;
}

void waitForCard() {
  Serial.println("Waiting for card...");
  waitingForCard = true;
  cardStartTime = millis();
  nfc.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A);
}

void checkCard() {
  if (!waitingForCard) return;

  if (nfc.readDetectedPassiveTargetID(uid, &uidLength)) {
    if (uidLength == 0) return;  // safety check

    waitingForCard = false;
    Serial.println();
    Serial.print("Detected Card: ");
    Serial.print("uidLength: ");
    Serial.println(uidLength);
    Serial.print(getUIDString());
    Serial.println();
    if (waitingForAddItem) {
      handleAddItem();
      waitingForAddItem = false;
    } else if (waitingForAssignCard) {
      // assign item
      sendJson("assignCard", "detected");
      waitingForAssignCard = false;  // Stop waiting
      Serial.println("Card detected and sent to frontend");
    }
  } else if (millis() - cardStartTime >= cardTimeout) {
    Serial.println();
    Serial.println("No Card Detected");
    waitingForCard = false;
    sendJson("cardTimeout");
  }
}

void handleAddItem() {
  Serial.println("Adding item");
  String itemName = rfid(0, "");

  memset(uid, 0, sizeof(uid));
  uidLength = 0;
  // Creates a JSON container
  StaticJsonDocument<128> doc;
  doc["type"] = "addItem";
  doc["item"] = itemName;
  // If overflow happens: JSON may be incomplete
  if (doc.overflowed()) {
    return;
  }
  // Converts the JSON object into a text string
  // Stores it in output
  String output;
  if (serializeJson(doc, output) == 0) {
    return;
  }
  // Sends the JSON message to all connected WebSocket clients
  webSocket.textAll(output);
}
void setHandleAssignFlag(JsonDocument &doc) {
  Serial.println("Setting flag");
  handleAssignFlag = true;
  currentItemName = String(doc["item"].as<const char *>());
}

void handleAssign() {
  Serial.println("Assigning Card");
  if (rfid(1, currentItemName) != "false" ) {
    sendJson("assignSuccessful");
    Serial.println("Card assigned successfully");
  } else {
    sendJson("assignFailed");
    Serial.println("Card assigned failed");
  }
  handleAssignFlag = false;
  clearUID();
}

String getUIDString() {
    String result = "";
    
    if (uidLength == 0 || uidLength > sizeof(uid)) return "INVALID";

    for (uint8_t i = 0; i < uidLength; i++) {
        if (uid[i] < 0x10) result += "0"; // leading zero
        result += String(uid[i], HEX);
    }
    return result;
}

void clearUID() {
    memset(uid, 0, sizeof(uid));
    uidLength = 0;
}

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

#define PN532_IRQ   (21)
#define PN532_RESET (22) 

Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  nfc.begin();
  Serial.println("Waiting for a Card ...");
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    Serial.println("Found a card");
    Serial.print("UID Value: ");
    nfc.PrintHex(uid, uidLength);

    Serial.println("Trying to authenticate block 4 with default KEYA value");
    uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);
    if (success) {
      Serial.println("Sector 1 authenticated");
      uint8_t data[16];
      memcpy(data, (const uint8_t[]){ 'a', 'p', 'p', 'l', 'e', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, sizeof data);
      success = nfc.mifareclassic_WriteDataBlock (4, data);
      success = nfc.mifareclassic_ReadDataBlock(4, data);

      if (success) {
        Serial.println("Reading Block 4:");
        nfc.PrintHexChar(data, 16);
        Serial.println("");
        delay(1000);
      }
      else {
        Serial.println("Ooops ... unable to read the requested block.  Try another key?");
      }
    } else {
      Serial.println("Ooops ... authentication failed: Try another key?");
    }
  }
}

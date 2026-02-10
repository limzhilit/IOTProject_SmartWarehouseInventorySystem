#include <WiFi.h>
#include <NetworkClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "DHT.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include "LittleFS.h"

#define DHTPIN 2 
#define DHTTYPE DHT22 
#define PN532_IRQ   (21)
#define PN532_RESET (22)

#define M1 33
#define M2 26
#define E1 32
#define E2 25


DHT dht(DHTPIN, DHTTYPE);
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

const char *ssid = "Zelig";
const char *password = "12345678";

WebServer server(80);

const int led = 13;

bool cardDetected = false;
String lastCardUID = "";

String rfid(int x, String itemName){
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
      if (x == 1) {
        uint8_t data[16] = {0};
        itemName.toCharArray((char*)data, 16);
        success = nfc.mifareclassic_WriteDataBlock (4, data);
      }
      success = nfc.mifareclassic_ReadDataBlock(4, data);
      if (success) {
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

void handleAddItem() {
  String itemName = rfid(0, "");
  server.send(200, "text/plain", itemName);
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

void handleWaitForCard() {
  Serial.println("Waiting for card...");
  unsigned long start = millis();
  bool cardFound = false;
  uint8_t uid[7];
  uint8_t uidLength;
  // Wait for card (timeout after 15 seconds)
  while (millis() - start < 3000) {
    if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength)) {
      cardFound = true;
      break;
    }
    delay(100);
  }
  if (cardFound) {
    Serial.println("Card detected!");
    server.send(200, "text/plain", "detected");
  } else {
    server.send(408, "text/plain", "timeout");
    Serial.println("Card detected!");
  }
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void handleTempHum() {
  String json = "{";
  json += "\"temperature\":" + String(dht.readTemperature(), 2) + ",";
  json += "\"humidity\":" + String(dht.readHumidity(), 2);
  json += "}";
  server.send(200, "application/json", json);
}

void handleRover() {

  String direction = server.arg("direction");
  String speed = server.arg("speed");

  int temp = direction.toInt();
  int spd = speed.toInt();

  Serial.printf("dir: %d  spd: %d\n", temp , spd);

  int sector = (temp/45) + 1;
  float angle = temp % 45;

  Serial.printf("sector: %d  angle: %.2f\n", sector , angle);

  switch(sector) {
    case 1:
      digitalWrite(M1,HIGH);
      digitalWrite(M2,HIGH);
      analogWrite(E1, spd);
      analogWrite(E2, (int)(45 - angle)*spd/45);
      Serial.printf("case 1 | M1: HIGH, M2: HIGH | E1: %d, E2: %d | angle: %.2f\n", spd, (int)((45 - angle)*spd/45), angle);
      break;

    case 2:
      digitalWrite(M1,HIGH);
      digitalWrite(M2,LOW);
      analogWrite(E1, spd);
      analogWrite(E2, (int)(angle)*spd/45);
      Serial.printf("case 2 | M1: HIGH, M2: LOW | E1: %d, E2: %d | angle: %.2f\n", spd, (int)((45 - angle)*spd/45), angle);
      break;

    case 3:
      digitalWrite(M1,HIGH);
      digitalWrite(M2,LOW);
      analogWrite(E1, (int)(45 - angle)*spd/45);
      analogWrite(E2, spd);
      Serial.printf("case 3 | M1: HIGH, M2: LOW | E1: %d, E2: %d | angle: %.2f\n", (int)((45 - angle)*spd/45), spd, angle);
      break;

    case 4:
      digitalWrite(M1,HIGH);
      digitalWrite(M2,LOW);
      analogWrite(E1, (int)(angle)*spd/45);
      analogWrite(E2, spd);
      Serial.printf("case 4 | M1: HIGH, M2: LOW | E1: %d, E2: %d | angle: %.2f\n", (int)((45 - angle)*spd/45), spd, angle);
      break;

    case 5:
      digitalWrite(M1,LOW);
      digitalWrite(M2,LOW);
      analogWrite(E1, (int)(45 - angle)*spd/45);
      analogWrite(E2, spd);
      Serial.printf("case 5 | M1: LOW, M2: LOW | E1: %d, E2: %d | angle: %.2f\n", (int)((45 - angle)*spd/45), spd, angle);
      break;

    case 6:
      digitalWrite(M1,LOW);
      digitalWrite(M2,HIGH);
      analogWrite(E1, spd);
      analogWrite(E2, (int)(angle)*spd/45);
      Serial.printf("case 6 | M1: HIGH, M2: LOW | E1: %d, E2: %d | angle: %.2f\n", spd, (int)((45 - angle)*spd/45), angle);
      break;

    case 7:
      digitalWrite(M1,LOW);
      digitalWrite(M2,HIGH);
      analogWrite(E1, spd);
      analogWrite(E2, (int)(45 - angle)*spd/45);
      Serial.printf("case 7 | M1: HIGH, M2: LOW | E1: %d, E2: %d | angle: %.2f\n", spd, (int)((45 - angle)*spd/45), angle);
      break;

    case 8:
      digitalWrite(M1,HIGH);
      digitalWrite(M2,HIGH);
      analogWrite(E1, spd);
      analogWrite(E2, (int)(angle)*spd/45);
      Serial.printf("case 8 | M1: HIGH, M2: LOW | E1: %d, E2: %d | angle: %.2f\n", spd, (int)((45 - angle)*spd/45), angle);
      break;

    default:
      Serial.printf("Error for direction | sector: %d | angle: %.2f\n", sector, angle);
  }
  server.send(200);
}


void connectWifi() {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
}

void serveFile(String path, String type) {
  File file = LittleFS.open(path, "r");
  if (!file) {
    server.send(404, "text/plain", "File Not Found");
    return;
  }
  server.streamFile(file, type);
  file.close();
}

void setup(void) {

  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  
  connectWifi();

  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  server.on("/", []() { serveFile("/index.html", "text/html"); });
  server.on("/style.css", []() { serveFile("/style.css", "text/css"); });
  server.on("/script.js", []() { serveFile("/script.js", "application/javascript"); });
  server.on("/rover", []() { serveFile("/rover/rover.html", "text/html"); });
  server.on("/rover.css", []() { serveFile("/rover/rover.css", "text/css"); });
  server.on("/rover.js", []() { serveFile("/rover/rover.js", "application/javascript"); });
  server.on("/handleRover", handleRover); 
  server.on("/temphum", handleTempHum);
  server.on("/addItem", handleAddItem);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.on("/assign", handleAssign);
  server.onNotFound(handleNotFound);

  server.on("/waitForCard", handleWaitForCard);

  server.begin();
  Serial.println("HTTP server started");

  //temp////////////////////////////////////////////////
  dht.begin();

  //RFID////////////////////////////////////////////////
  nfc.begin();


}

void loop(void) {
  server.handleClient();
  delay(2);  //allow the cpu to switch to other tasks



}

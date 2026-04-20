#include "globals.h"

Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);
AsyncWebServer server(80);
AsyncWebSocket webSocket("/ws");
DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;
static unsigned long lastPrint = 0;
static unsigned long lastSend = 0;


void setup(void) {
  Serial.begin(115200);

  digitalWrite(M1, LOW);
  digitalWrite(M2, LOW);
  analogWrite(E1, 0);
  analogWrite(E2, 0);

  Wire.begin(21, 22);

  Serial.println("Scanning I2C...");

  for (byte i = 1; i < 127; i++) {
    Wire.beginTransmission(i);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found device at 0x");
      Serial.println(i, HEX);
    }
  }

  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);

  Serial.println("Connecting WiFi");
  connectWifi();

  if (!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }


  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");
  server.serveStatic("/rover", LittleFS, "/rover/");

  server.on("/inline", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.addHandler(&webSocket);
  webSocket.onEvent(onWebSocketEvent);

  server.begin();
  Serial.println("HTTP server started");

  dht.begin();
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("Didn't find PN532");
  }

  Serial.print("PN532 found, firmware: ");
  Serial.println((versiondata >> 16) & 0xFF, HEX);

  nfc.SAMConfig();
  ThingSpeak.begin(client);
}

void loop(void) {
  unsigned long now = millis();
  if (now - lastPrint > 1000) {
    Serial.print(".");
    lastPrint = millis();
  }

  if (handleAssignFlag) {
    handleAssign();
  }

  if (now - lastSend > 1000) {
    handleTempHum();
    lastSend = millis();
  }
  webSocket.cleanupClients();
  checkCard();

  static unsigned long lastThingSpeak = 0;

  if (millis() - lastThingSpeak > 20000) {
    thingSpeak();
    lastThingSpeak = millis();
  }

}
#include "globals.h"

Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);
AsyncWebServer server(80); // This is now an AsyncWebServer type
WebSocketsServer webSocket = WebSocketsServer(81);
DHT dht(DHTPIN, DHTTYPE);

void setup(void) {
  Serial.begin(115200);

  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);

  Serial.println("Connecting WiFi");
  connectWifi();

  if (!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  // 1. Static file serving (The line you wanted!)
  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");
  server.serveStatic("/rover", LittleFS, "/rover.html");

  // 2. Updated Handlers
  // Note: These functions (handleRover, etc.) must be updated in their 
  // respective .ino files to accept (AsyncWebServerRequest *request)
  // server.on("/handleRover", HTTP_GET, handleRover);
  server.on("/temphum", HTTP_GET, handleTempHum);
  server.on("/addItem", HTTP_GET, handleAddItem);
  server.on("/assign", HTTP_GET, handleAssign);
  server.on("/waitForCard", HTTP_GET, handleWaitForCard);

  // 3. Updated Inline Lambdas
  server.on("/diagnostics", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "application/json", getDiagnosticsJSON());
  });

  server.on("/inline", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  webSocket.begin(); // AsyncWebServer usually pairs better with webSocket.begin()
  webSocket.onEvent(webSocketEvent);
  Serial.println("HTTP server started");

  dht.begin();
  nfc.begin();
}

void loop(void) {
  checkCard();
  webSocket.loop();
  
  // 4. IMPORTANT: server.handleClient() is REMOVED.
  // The Async server handles requests automatically via interrupts.
  
  delay(2); 
}
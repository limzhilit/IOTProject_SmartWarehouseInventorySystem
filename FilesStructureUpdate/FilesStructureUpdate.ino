#include "globals.h"

Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);
WebServer server(80);
DHT dht(DHTPIN, DHTTYPE);

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
  checkCard();
  server.handleClient();
  delay(2);  //allow the cpu to switch to other tasks
}

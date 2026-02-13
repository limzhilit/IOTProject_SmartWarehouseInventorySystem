#include "globals.h"

void handleTempHum(AsyncWebServerRequest *request) {
  String json = "{";
  json += "\"temperature\":" + String(dht.readTemperature(), 2) + ",";
  json += "\"humidity\":" + String(dht.readHumidity(), 2);
  json += "}";
  request->send(200, "application/json", json);
}

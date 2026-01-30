#include "globals.h"

void handleTempHum() {
  String json = "{";
  json += "\"temperature\":" + String(dht.readTemperature(), 2) + ",";
  json += "\"humidity\":" + String(dht.readHumidity(), 2);
  json += "}";
  server.send(200, "application/json", json);
}

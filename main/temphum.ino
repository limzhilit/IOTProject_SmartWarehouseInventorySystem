#include "globals.h"

void handleTempHum() {

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    //Serial.println("DHT read failed, skipping");
    return;  // bail out early, don't broadcast garbage
  }

  StaticJsonDocument<64> doc;
  doc["type"] = "temphum";
  doc["temp"] = String(temp, 2);
  doc["hum"] = String(hum, 2);

  String output;
  serializeJson(doc, output);

  webSocket.textAll(output);
}

void thingSpeak(){
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, hum);

  String status;
  if (temp > 30) {
    status = "Temp too high";
  } else if (temp < 20) {
    status = "Temp too low";
  } else {
    status = "Normal";
  }

  ThingSpeak.setStatus(status);

  int x = ThingSpeak.writeFields(3350017, "SAYSYVIN5D8XVB1T");

  Serial.println(x == 200 ? "ThingSpeak OK" : "ThingSpeak Error");

}

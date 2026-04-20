#include "globals.h"

const char *ssid = "Zelig";
const char *password = "12345678";

void onWebSocketEvent(AsyncWebSocket *server,
                      AsyncWebSocketClient *client,
                      AwsEventType type,
                      void *arg,
                      uint8_t *data,
                      size_t len) {

  if (type == WS_EVT_CONNECT) {
    Serial.println("Client connected");
  }

  else if (type == WS_EVT_DISCONNECT) {
    Serial.println("Client disconnected");
  }

  else if (type == WS_EVT_DATA) {

    StaticJsonDocument<256> doc;
    DeserializationError err = deserializeJson(doc, data, len);

    if (err) {
      Serial.println("Invalid JSON");
      return;
    }

    routeMessage(doc);
  }
}

void routeMessage(JsonDocument &doc) {
  String type = doc["type"];
  Serial.println();
  Serial.println("Message type: " + type);

  if (type == "addItem") {
    waitingForAddItem = true;
    waitForCard();
  } else if (type == "assignCard") {
    waitingForAssignCard = true;
    waitForCard();
  } else if (type == "assign") {
    setHandleAssignFlag(doc);
  } else if (type == "cancelAddItem") {
    waitingForAddItem = false;
    waitingForCard = false;
    Serial.println("Cancel add item");
  } else if (type == "cancelAssignCard") {
    waitingForAssignCard = false;
    waitingForCard = false;
    Serial.println("Cancel assign card");
  } else if (type == "rover") {
    handleRover(doc);
  } else {
    Serial.print("Unknown message type: ");
    Serial.println(type);
  }
}

void handleNotFound(AsyncWebServerRequest *request) {
  digitalWrite(led, HIGH);

  StaticJsonDocument<512> doc;

  doc["error"] = "File Not Found";
  doc["url"] = request->url();
  doc["method"] = (request->method() == HTTP_GET) ? "GET" : "POST";
  doc["argCount"] = request->args();

  JsonArray args = doc.createNestedArray("arguments");

  for (uint8_t i = 0; i < request->args(); i++) {
    JsonObject arg = args.createNestedObject();
    arg["name"] = request->argName(i);
    arg["value"] = request->arg(i);
  }

  String output;
  serializeJson(doc, output);

  request->send(404, "application/json", output);

  digitalWrite(led, LOW);
}

void connectWifi() {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  // Serial.begin is usually already in main.ino, but keeping for consistency
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("*");
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

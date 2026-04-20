#pragma once

#include <WiFi.h>
#include <NetworkClient.h>
#include <ESPAsyncWebServer.h> 
#include <ESPmDNS.h>
#include "DHT.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include "LittleFS.h"
#include <ArduinoJson.h>
#include "ThingSpeak.h"

#define PN532_IRQ   (21)
#define PN532_RESET (22)
#define M1 33
#define M2 26
#define E1 32
#define E2 25
#define DHTPIN 2 
#define DHTTYPE DHT22 
#define led 13

extern AsyncWebServer server;
extern AsyncWebSocket webSocket;
extern Adafruit_PN532 nfc;
extern DHT dht;

bool waitingForAddItem = false;
bool waitingForAssignCard = false;
bool waitingForCard = false;
bool handleAssignFlag = false;

String currentItemName;


void controlRover(int temp, int spd);
void onWebSocketEvent(AsyncWebSocket *server,
                      AsyncWebSocketClient *client,
                      AwsEventType type,
                      void *arg,
                      uint8_t *data,
                      size_t len);

void sendJson(const char* type, const char* status = nullptr, const char* item = nullptr) {
  StaticJsonDocument<128> doc;

  doc["type"] = type;

  if (status) doc["status"] = status;
  if (item) doc["item"] = item;

  String output;
  serializeJson(doc, output);

  webSocket.textAll(output);
}





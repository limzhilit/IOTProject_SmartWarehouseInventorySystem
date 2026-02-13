#pragma once

#include <WiFi.h>
#include <NetworkClient.h>
#include <ESPAsyncWebServer.h> // Change from <WebServer.h>
#include <WebSocketsServer.h>
#include <ESPmDNS.h>
#include "DHT.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include "LittleFS.h"

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
extern Adafruit_PN532 nfc;
extern DHT dht;
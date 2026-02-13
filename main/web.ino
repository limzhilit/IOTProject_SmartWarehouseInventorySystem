#include "globals.h"

const char *ssid = "Zelig";
const char *password = "12345678";

void handleNotFound(AsyncWebServerRequest *request) {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URL: ";
  message += request->url(); // In Async, use .url()
  message += "\nMethod: ";
  message += (request->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += request->args();
  message += "\n";
  
  for (uint8_t i = 0; i < request->args(); i++) {
    message += " " + request->argName(i) + ": " + request->arg(i) + "\n";
  }
  
  request->send(404, "text/plain", message);
  digitalWrite(led, 0);
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

// --- Updated serveFile ---
// Note: With server.serveStatic() in main.ino, you might not even need this anymore!
void serveFile(AsyncWebServerRequest *request, String path, String type) {
  if (LittleFS.exists(path)) {
    request->send(LittleFS, path, type);
  } else {
    request->send(404, "text/plain", "File Not Found");
  }
}


// #include "globals.h"

// const char *ssid = "Zelig";
// const char *password = "12345678";

// void handleNotFound() {
//   digitalWrite(led, 1);
//   String message = "File Not Found\n\n";
//   message += "URI: ";
//   message += server.uri();
//   message += "\nMethod: ";
//   message += (server.method() == HTTP_GET) ? "GET" : "POST";
//   message += "\nArguments: ";
//   message += server.args();
//   message += "\n";
//   for (uint8_t i = 0; i < server.args(); i++) {
//     message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
//   }
//   server.send(404, "text/plain", message);
//   digitalWrite(led, 0);
// }

// void connectWifi() {
//   pinMode(led, OUTPUT);
//   digitalWrite(led, 0);
//   Serial.begin(115200);
//   WiFi.mode(WIFI_STA);
//   WiFi.begin(ssid, password);
//   Serial.println("");

//   // Wait for connection
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println("");
//   Serial.print("Connected to ");
//   Serial.println(ssid);
//   Serial.print("IP address: ");
//   Serial.println(WiFi.localIP());

//   if (MDNS.begin("esp32")) {
//     Serial.println("MDNS responder started");
//   }
// }

// void serveFile(String path, String type) {
//   File file = LittleFS.open(path, "r");
//   if (!file) {
//     server.send(404, "text/plain", "File Not Found");
//     return;
//   }
//   server.streamFile(file, type);
//   file.close();
// }
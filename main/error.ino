
bool checkDHT() {
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    if (isnan(temp) || isnan(hum)) {
        Serial.println("DHT sensor read failed!");
        return false;
    }
    Serial.println("DHT OK: Temp=" + String(temp) + " Hum=" + String(hum));
    return true;
}

bool checkPN532() {
    uint32_t versiondata = nfc.getFirmwareVersion();
    if (!versiondata) {
        Serial.println("PN532 not found. Check wiring!");
        return false;
    }
    Serial.print("PN532 OK: Firmware version: ");
    Serial.println((versiondata >> 16) & 0xFF, DEC);
    return true;
}

bool checkLED() {
    pinMode(led, OUTPUT);
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
    Serial.println("LED test completed.");
    return true;
}

bool checkMotors() {
    pinMode(M1, OUTPUT);
    pinMode(M2, OUTPUT);
    pinMode(E1, INPUT);
    pinMode(E2, INPUT);

    digitalWrite(M1, HIGH);
    digitalWrite(M2, HIGH);
    delay(50);
    digitalWrite(M1, LOW);
    digitalWrite(M2, LOW);

    Serial.println("Motors pins toggled. Check encoder inputs manually if needed.");
    return true;
}

#include <Wire.h>

bool checkI2C() {
    Wire.begin();
    Serial.println("Scanning I2C bus...");
    byte error, address;
    int nDevices = 0;

    for(address = 1; address < 127; address++ ) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0) {
            Serial.print("I2C device found at 0x");
            Serial.println(address, HEX);
            nDevices++;
        }
    }
    if(nDevices == 0) {
        Serial.println("No I2C devices found. Check wiring.");
        return false;
    }
    return true;
}

void runDiagnostics() {
    Serial.println("=== Running Hardware Diagnostics ===");
    bool dhtOK = checkDHT();
    bool nfcOK = checkPN532();
    bool ledOK = checkLED();
    bool motorsOK = checkMotors();
    bool i2cOK = checkI2C();

    if(dhtOK && nfcOK && ledOK && motorsOK && i2cOK)
        Serial.println("All hardware connections OK!");
    else
        Serial.println("Some hardware connections failed. Check above messages.");
}

String getDiagnosticsJSON() {
    String json = "{";
    json += "\"DHT\":" + String(checkDHT() ? "true" : "false") + ",";
    json += "\"PN532\":" + String(checkPN532() ? "true" : "false") + ",";
    json += "\"LED\":" + String(checkLED() ? "true" : "false") + ",";
    json += "\"Motors\":" + String(checkMotors() ? "true" : "false") + ",";
    json += "\"I2C\":" + String(checkI2C() ? "true" : "false");
    json += "}";
    return json;
}

// On your server endpoint
server.on("/diagnostics", []() {
    server.send(200, "application/json", getDiagnosticsJSON());
});


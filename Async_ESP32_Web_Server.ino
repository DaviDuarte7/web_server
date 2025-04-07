  #include <Arduino.h>
  #include <WiFi.h>
  #include <AsyncTCP.h>
  #include <ESPAsyncWebServer.h>
  #include "SPIFFS.h"
  #include "JsonController.h"
  #include "LedController.h"
  #include "HootController.h"

void setup() {
    Serial.begin(115200);
    setupHoot();
    setupleds();
    setupRoutes(server);
    server.begin();
}

void loop() {
    piscarLed();
}

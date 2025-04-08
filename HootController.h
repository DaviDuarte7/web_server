#ifndef HOOTCONTROLLER_H
#define HOOTCONTROLLER_H

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"

// Defining Wi-Fi credentials
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
AsyncWebServer server(80);

void setupHoot() {
    Serial.begin(115200);

    if (!SPIFFS.begin(true)) {
        Serial.println("Failed to mount SPIFFS!");
        return;
    }
    Serial.println("SPIFFS mounted successfully.");

    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi...");

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Route to serve HTML
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", String(), false);
    });

    // Route to serve CSS
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/style.css", "text/css");
    });

    // Route to serve additional HTML data page
    server.on("/dados.html", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/dados.html", String(), false);
    });

    server.begin();
    Serial.println("Server started.");
}

#endif

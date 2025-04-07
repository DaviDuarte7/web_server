#ifndef HOOTCONTROLLER_H
#define HOOTCONTROLLER_H

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"

// Definindo as credenciais da rede Wi-Fi
  const char* ssid = "Atlantic Safaris";
  const char* password = "atlanticmarinainterna";
  AsyncWebServer server(80);

void setupHoot() {
    Serial.begin(115200);

    if (!SPIFFS.begin(true)) {
        Serial.println("Erro ao montar SPIFFS!");
        return;
    }
    Serial.println("SPIFFS montado com sucesso.");

    WiFi.begin(ssid, password);
    Serial.print("Conectando ao WiFi...");

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    Serial.println("\nWiFi Conectado!");
    Serial.print("Endereço IP: ");
    Serial.println(WiFi.localIP());

// Rota para servir o HTML
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", String(), false);
    });

// Rota para servir o CSS
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/style.css", "text/css");
    });

     server.on("/dados.html", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/dados.html", String(), false);
});

    server.begin();
    Serial.println("Servidor iniciado.");
}

#endif

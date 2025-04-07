#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H
#include "JsonController.h"
#include <Arduino.h>
#include <ESPAsyncWebServer.h>

enum ledID {
    LED1 = 1,
    LED2 = 2,
    LED3 = 3,
    PISCAR_LEDS = 4
};

struct led {
    ledID id;
    int pin;
};

const led leds[] = {
    {LED1, 32},
    {LED2, 34},
    {LED3, 27}
};

  const int led1 = 32, led2 = 34, led3 = 27;
  bool piscarAtivo = false;
  unsigned long tempoAnterior = 0;
  unsigned long intervalo = 1000;
  int estado = 0;

void logActionJSON(const ledID id, String action);

void piscarLed(ledID id, bool state) {
    if (state) {
        piscarAtivo = true; // Activate blinking
    } else {
        piscarAtivo = false; // Stop blinking
    }
}

void setupleds() {
    for (const auto &led : leds) {
        pinMode(led.pin, OUTPUT);
        digitalWrite(led.pin, LOW);
    }
}

void controlarled(ledID id, bool estado) {
    for (const auto &led : leds) {
        if (led.id == id) {
            digitalWrite(led.pin, estado ? HIGH : LOW);
            Serial.printf("O LED %d está %s\n", id, estado ? "ligado" : "desligado");
        }
    }
} 

void setupRoutes(AsyncWebServer& server) {
    server.on("/led1/on", HTTP_GET, [](AsyncWebServerRequest *request) {
        controlarled(LED1, true);
        logActionJSON(LED1, "Ligado");
        request->send(200, "text/plain", "LED 1 Ligado");
    });

    server.on("/led1/off", HTTP_GET, [](AsyncWebServerRequest *request) {
        controlarled(LED1, false);
        logActionJSON(LED1, "Desligado");
        request->send(200, "text/plain", "LED 1 Desligado");
    });

    server.on("/led2/on", HTTP_GET, [](AsyncWebServerRequest *request) {
        controlarled(LED2, true);
        logActionJSON(LED2, "Ligado");
        request->send(200, "text/plain", "LED 2 Ligado");
    });

    server.on("/led2/off", HTTP_GET, [](AsyncWebServerRequest *request) {
        controlarled(LED2, false);
        logActionJSON(LED2, "Desligado");
        request->send(200, "text/plain", "LED 2 Desligado");
    });

    server.on("/led3/on", HTTP_GET, [](AsyncWebServerRequest *request) {
        controlarled(LED3, true);
        logActionJSON(LED3, "Ligado");
        request->send(200, "text/plain", "LED 3 Ligado");
    });

    server.on("/led3/off", HTTP_GET, [](AsyncWebServerRequest *request) {
        controlarled(LED3, false);
        logActionJSON(LED3, "Desligado");
        request->send(200, "text/plain", "LED 3 Desligado");
    });
    
    server.on("/piscar/on", HTTP_GET, [](AsyncWebServerRequest *request) {
        piscarLed(PISCAR_LEDS, true);
        logActionJSON(PISCAR_LEDS, "ligado");
        request->send(200, "text/plain", "Modo Piscar Ativado");
    });

    server.on("/piscar/off", HTTP_GET, [](AsyncWebServerRequest *request) {
        piscarLed(PISCAR_LEDS, false);
        for (const auto &led : leds) {
            digitalWrite(led.pin, LOW);
        }
        logActionJSON(PISCAR_LEDS, "Desligado");
        request->send(200, "text/plain", "Modo Piscar Desativado");
    });
    
       server.on("/state", HTTP_GET, [](AsyncWebServerRequest *request) {
    String jsonResponse = "{";
    jsonResponse += "\"LED1\": \"" + String(digitalRead(leds[0].pin) == HIGH ? "ON" : "OFF") + "\", ";
    jsonResponse += "\"LED2\": \"" + String(digitalRead(leds[1].pin) == HIGH ? "ON" : "OFF") + "\", ";
    jsonResponse += "\"LED3\": \"" + String(digitalRead(leds[2].pin) == HIGH ? "ON" : "OFF") + "\", ";
    jsonResponse += "\"piscarLed\": \"" + String(piscarAtivo ? "ON" : "OFF") + "\", ";
    jsonResponse += "\"intervalo\": " + String(intervalo); // Adiciona o intervalo no JSON
    jsonResponse += "}";
    request->send(200, "application/json", jsonResponse);
});

  server.on("/setIntervalo", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("tempo")) {
        intervalo = request->getParam("tempo")->value().toInt();
        Serial.printf("Intervalo atualizado para %d ms\n", intervalo);
        request->send(200, "text/plain", "Intervalo atualizado para " + String(intervalo) + "ms");
    } else {
        request->send(400, "text/plain", "Erro: falta o parâmetro tempo");
    }
});

}


void piscarLed() {
    static unsigned long tempoAnterior = 0;
    static int estado = 0;

    if (piscarAtivo) {
        unsigned long tempoAtual = millis();

        if (tempoAtual - tempoAnterior >= intervalo) {
            tempoAnterior = tempoAtual;

            // Desligar todos os LEDs antes de ligar o próximo
            for (const auto &led : leds) {
                digitalWrite(led.pin, LOW);
            }

            // Alternar entre os LEDs
            digitalWrite(leds[estado].pin, HIGH);
            Serial.printf("O LED %d está ligado\n", estado + 1);

            estado = (estado + 1) % 3; // Alternar entre 0, 1, 2
        }
    }
}

#endif

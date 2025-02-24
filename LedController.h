#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H
#include "jsonController.h"
#include <Arduino.h>
#include <ESPAsyncWebServer.h>

enum ledID {
    LED1 = 1,
    LED2 = 2,
    LED3 = 3
};

struct led {
    ledID id;
    int pin;
};

const led leds[] = {
    {LED1, 32},
    {LED2, 33},
    {LED3, 27}
};

const int led1 = 32, led2 = 33, led3 = 27;
bool piscarAtivo = false;
unsigned long tempoAnterior = 0;
unsigned long intervalo = 1000;
int estado = 0;

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
        logActionJSON("LED1", "Ligado");
        request->send(200, "text/plain", "LED 1 Ligado");
    });

    server.on("/led1/off", HTTP_GET, [](AsyncWebServerRequest *request) {
        controlarled(LED1, false);
        logActionJSON("LED1", "Desligado");
        request->send(200, "text/plain", "LED 1 Desligado");
    });

    server.on("/led2/on", HTTP_GET, [](AsyncWebServerRequest *request) {
        controlarled(LED2, true);
        logActionJSON("LED2", "Ligado");
        request->send(200, "text/plain", "LED 2 Ligado");
    });

    server.on("/led2/off", HTTP_GET, [](AsyncWebServerRequest *request) {
        controlarled(LED2, false);
        logActionJSON("LED2", "Desligado");
        request->send(200, "text/plain", "LED 2 Desligado");
    });

    server.on("/led3/on", HTTP_GET, [](AsyncWebServerRequest *request) {
        controlarled(LED3, true);
        logActionJSON("LED3", "Ligado");
        request->send(200, "text/plain", "LED 3 Ligado");
    });

    server.on("/led3/off", HTTP_GET, [](AsyncWebServerRequest *request) {
        controlarled(LED3, false);
        logActionJSON("LED3", "Desligado");
        request->send(200, "text/plain", "LED 3 Desligado");
    });
    
      server.on("/state", HTTP_GET, [](AsyncWebServerRequest *request) {
        String state = "{";
        for (const auto &led : leds) {
            state += "\"LED" + String(led.id) + "\": \"" + (digitalRead(led.pin) == HIGH ? "ON" : "OFF") + "\",";
        }
        state.remove(state.length() - 1); // Remove a última vírgula
        state += "}";
        request->send(200, "application/json", state);
    });

    server.on("/piscar/on", HTTP_GET, [](AsyncWebServerRequest *request) {
        piscarAtivo = true;
    logActionJSON("piscar", "ativo");
        request->send(200, "text/plain", "Modo Piscar Ativado");
    });

    server.on("/piscar/off", HTTP_GET, [](AsyncWebServerRequest *request) {
        piscarAtivo = false;
       for (const auto &led : leds){
        digitalWrite(led.pin, LOW);
        }
        logActionJSON("piscar","desativado");
        request->send(200, "text/plain", "Modo Piscar Desativado");
    });
    server.on("/piscar/state", HTTP_GET, [](AsyncWebServerRequest *request){
     request->send(200, "text/plain", piscarAtivo ? "ativado" : "desativado");
    });

       server.on("/setIntervalo", HTTP_GET, [](AsyncWebServerRequest *request){
        if (request->hasParam("tempo")){
            intervalo = request->getParam("tempo")->value().toInt();
            Serial.println("Intervalo atualizado: " + String(intervalo) + "ms");
            request->send(200, "text/plain", "Intervalo atualizado para " + String(intervalo) + "ms");
        } else {
            request->send(400, "text/plain", "Erro: falta o parametro tempo");
        }
    });

server.on("/dados.html", HTTP_GET, [](AsyncWebServerRequest *request) {
    String jsonResponse = "{\"LED1\": \"" + String(digitalRead(32) == HIGH ? "ON" : "OFF") + "\", ";
    jsonResponse += "\"LED2\": \"" + String(digitalRead(33) == HIGH ? "ON" : "OFF") + "\", ";
    jsonResponse += "\"LED3\": \"" + String(digitalRead(27) == HIGH ? "ON" : "OFF") + "\"}";

    Serial.println("Respondendo a requisição de dados JSON: " + jsonResponse); // Debug
    request->send(200, "application/json", jsonResponse);
});

}
 
void piscarLeds() {
    if (piscarAtivo) {
        unsigned long tempoAtual = millis();

        if (tempoAtual - tempoAnterior >= intervalo) {
            tempoAnterior = tempoAtual;

            digitalWrite(led1, LOW);
            digitalWrite(led2, LOW);
            digitalWrite(led3, LOW);

            if (estado == 0) {  
                digitalWrite(led1, HIGH);
                Serial.println("O LED 1 está ligado");
            } else if (estado == 1) {
                digitalWrite(led2, HIGH);
                Serial.println("O LED 2 está ligado");
            } else if (estado == 2) {
                digitalWrite(led3, HIGH);
                Serial.println("O LED 3 está ligado");
            }

            estado++;
            if (estado > 2) estado = 0;
        }
    }
}

#endif

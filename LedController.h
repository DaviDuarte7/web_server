#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include "JsonController.h"
#include <Arduino.h>
#include <ESPAsyncWebServer.h>

enum ledID {
    LED1 = 1,
    LED2 = 2,
    LED3 = 3,
    BLINK_LEDS = 4
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
bool blinkingActive = false;
unsigned long previousTime = 0;
unsigned long interval = 1000;
int state = 0;

void logActionJSON(const ledID id, String action);

void blinkLed(ledID id, bool state) {
    blinkingActive = state;
}

void setupLeds() {
    for (const auto &led : leds) {
        pinMode(led.pin, OUTPUT);
        digitalWrite(led.pin, LOW);
    }
}

void controlLed(ledID id, bool state) {
    for (const auto &led : leds) {
        if (led.id == id) {
            digitalWrite(led.pin, state ? HIGH : LOW);
            Serial.printf("LED %d is %s\n", id, state ? "ON" : "OFF");
        }
    }
} 

void setupRoutes(AsyncWebServer& server) {
    server.on("/led1/on", HTTP_GET, [](AsyncWebServerRequest *request) {
        controlLed(LED1, true);
        logActionJSON(LED1, "ON");
        request->send(200, "text/plain", "LED 1 ON");
    });

    server.on("/led1/off", HTTP_GET, [](AsyncWebServerRequest *request) {
        controlLed(LED1, false);
        logActionJSON(LED1, "OFF");
        request->send(200, "text/plain", "LED 1 OFF");
    });

    server.on("/led2/on", HTTP_GET, [](AsyncWebServerRequest *request) {
        controlLed(LED2, true);
        logActionJSON(LED2, "ON");
        request->send(200, "text/plain", "LED 2 ON");
    });

    server.on("/led2/off", HTTP_GET, [](AsyncWebServerRequest *request) {
        controlLed(LED2, false);
        logActionJSON(LED2, "OFF");
        request->send(200, "text/plain", "LED 2 OFF");
    });

    server.on("/led3/on", HTTP_GET, [](AsyncWebServerRequest *request) {
        controlLed(LED3, true);
        logActionJSON(LED3, "ON");
        request->send(200, "text/plain", "LED 3 ON");
    });

    server.on("/led3/off", HTTP_GET, [](AsyncWebServerRequest *request) {
        controlLed(LED3, false);
        logActionJSON(LED3, "OFF");
        request->send(200, "text/plain", "LED 3 OFF");
    });
    
    server.on("/blink/on", HTTP_GET, [](AsyncWebServerRequest *request) {
        blinkLed(BLINK_LEDS, true);
        logActionJSON(BLINK_LEDS, "ON");
        request->send(200, "text/plain", "Blink Mode Activated");
    });

    server.on("/blink/off", HTTP_GET, [](AsyncWebServerRequest *request) {
        blinkLed(BLINK_LEDS, false);
        for (const auto &led : leds) {
            digitalWrite(led.pin, LOW);
        }
        logActionJSON(BLINK_LEDS, "OFF");
        request->send(200, "text/plain", "Blink Mode Deactivated");
    });
    
    server.on("/state", HTTP_GET, [](AsyncWebServerRequest *request) {
        String jsonResponse = "{";
        jsonResponse += "\"LED1\": \"" + String(digitalRead(leds[0].pin) == HIGH ? "ON" : "OFF") + "\", ";
        jsonResponse += "\"LED2\": \"" + String(digitalRead(leds[1].pin) == HIGH ? "ON" : "OFF") + "\", ";
        jsonResponse += "\"LED3\": \"" + String(digitalRead(leds[2].pin) == HIGH ? "ON" : "OFF") + "\", ";
        jsonResponse += "\"blinkLed\": \"" + String(blinkingActive ? "ON" : "OFF") + "\", ";
        jsonResponse += "\"interval\": " + String(interval);
        jsonResponse += "}";
        request->send(200, "application/json", jsonResponse);
    });

    server.on("/setInterval", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request->hasParam("time")) {
            interval = request->getParam("time")->value().toInt();
            Serial.printf("Interval updated to %d ms\n", interval);
            request->send(200, "text/plain", "Interval updated to " + String(interval) + " ms");
        } else {
            request->send(400, "text/plain", "Error: missing 'time' parameter");
        }
    });
}

void blinkLed() {
    static unsigned long previousTime = 0;
    static int state = 0;

    if (blinkingActive) {
        unsigned long currentTime = millis();

        if (currentTime - previousTime >= interval) {
            previousTime = currentTime;

            // Turn off all LEDs before turning on the next one
            for (const auto &led : leds) {
                digitalWrite(led.pin, LOW);
            }

            // Toggle between LEDs
            digitalWrite(leds[state].pin, HIGH);
            Serial.printf("LED %d is ON\n", state + 1);

            state = (state + 1) % 3; // Cycle through 0, 1, 2
        }
    }
}

#endif

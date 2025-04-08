#ifndef JSONCONTROLLER_H
#define JSONCONTROLLER_H

#include <ArduinoJson.h>
#include "SPIFFS.h"
#include "LedController.h"

// Function to set up JSON and initialize SPIFFS
void setupJson() {
    if (!SPIFFS.begin()) {
        Serial.println("Error initializing SPIFFS.");
        return;
    }
    Serial.println("JSON Controller configured!");
}

// Function to log actions to JSON and print to Serial Monitor
void logActionJSON(const ledID id, String action) {
    StaticJsonDocument<2048> doc;
    File logFile;

    // Check if the log file already exists
    if (SPIFFS.exists("/log.json")) {
        logFile = SPIFFS.open("/log.json", "r");
        if (logFile) {
            DeserializationError error = deserializeJson(doc, logFile);
            logFile.close();
            if (error) {
                Serial.println("Error reading JSON, creating a new one...");
                doc.clear();
            }
        } else {
            Serial.println("Error opening log.json for reading.");
            return;
        }
    }

    // If file doesn't exist or deserialization failed, create a new JSON array
    JsonArray logArray = doc.isNull() ? doc.to<JsonArray>() : doc.as<JsonArray>();

    // Create a new entry for the LED action
    JsonObject newEntry = logArray.createNestedObject();
    newEntry[String("LED_") + String(id)] = action;
    newEntry["timestamp"] = millis();
    
    // Write updated JSON back to the file
    logFile = SPIFFS.open("/log.json", "w");
    if (logFile) {
        serializeJson(doc, logFile);
        logFile.close();
        Serial.println("JSON updated in file!");
    } else {
        Serial.println("Error opening log.json for writing!");
    }

    // Display the log on the Serial Monitor in a clean format
    Serial.println("==============================");
    Serial.println("       LED Status Log");
    Serial.println("==============================");

    // Extract and display each value
    JsonArray arr = doc.as<JsonArray>();
    for (JsonObject obj : arr) {
        for (JsonPair kv : obj) {
            String led = kv.key().c_str();
            String status = kv.value().as<String>();
            if (led.startsWith("LED")) {
                Serial.print(led);
                Serial.print(": ");
                Serial.println(status);
            }
        }
    }

    // Display timestamp
    long timestamp = millis();
    Serial.print("Timestamp: ");
    Serial.println(timestamp);

    Serial.println("==============================");
}

#endif

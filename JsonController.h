#ifndef JSONCONTROLLER_H
#define JSONCONTROLLER_H

#include <ArduinoJson.h>
#include "SPIFFS.h"

void setupJson() {
    Serial.println("JSON Controller configurado!");
}

// Função para registrar ações no JSON e imprimir no Serial
void logActionJSON(String ledID, String action) {
    StaticJsonDocument<2048> doc;
    File logFile = SPIFFS.open("/log.json", "r");

    // Verifica se o arquivo existe e lê seu conteúdo
    if (logFile) {
        DeserializationError error = deserializeJson(doc, logFile);
        logFile.close();
        if (error) {
            Serial.println("Erro ao ler o JSON, inicializando novo documento.");
            doc.clear(); // Limpa o documento se houver erro
        }
    } else {
        Serial.println("Arquivo log.json não encontrado, criando novo.");
    }

    JsonArray logArray = doc.to<JsonArray>(); // Sempre retorna um JsonArray

    JsonObject newEntry = logArray.createNestedObject();
    newEntry ["ledID"] = ledID; //adciona o ledID ao objeto json
    newEntry ["action"] = action;
    newEntry ["timestamp"] = millis();

    // Abre o arquivo para escrita
    File logFileWrite = SPIFFS.open("/log.json", "w");
    if (logFileWrite) {
        serializeJson(doc, logFileWrite); // Serializa o documento JSON atualizado
        logFileWrite.close();
        Serial.println("JSON atualizado no arquivo!");
    } else {
        Serial.println("Erro ao abrir log.json para escrita!");
    }

    Serial.println("===== LOG ATUALIZADO =====");
    serializeJsonPretty(doc, Serial);
    Serial.println("\n==========================");
}

#endif

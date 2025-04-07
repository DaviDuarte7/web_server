#ifndef JSONCONTROLLER_H
#define JSONCONTROLLER_H
#include <ArduinoJson.h>
#include "SPIFFS.h"
#include "LedController.h"
  
// Função para configurar o JSON e inicializar SPIFFS
void setupJson() {
      if (!SPIFFS.begin()) {
          Serial.println("Erro ao inicializar SPIFFS.");
          return;
      }
      Serial.println("JSON Controller configurado!");
  }
  
// Função para registrar ações no JSON e imprimir no Serial
void logActionJSON(const ledID id, String action) {
      StaticJsonDocument<2048> doc;
      File logFile;
  
      // Verifica se o arquivo de log já existe
      if (SPIFFS.exists("/log.json")) {
          logFile = SPIFFS.open("/log.json", "r");
          if (logFile) {
              DeserializationError error = deserializeJson(doc, logFile);
              logFile.close();
              if (error) {
                  Serial.println("Erro ao ler JSON, criando um novo...");
                  doc.clear();
              }
          } else {
              Serial.println("Erro ao abrir log.json para leitura.");
              return;
          }
      }
  
// Se o arquivo não existir ou se houve erro na leitura, criamos um novo array JSON
      JsonArray logArray = doc.isNull() ? doc.to<JsonArray>() : doc.as<JsonArray>();
  
// Criando um novo registro para o LED
      JsonObject newEntry = logArray.createNestedObject();
      newEntry[String("LED_") + String(id)] = action;
      newEntry["timestamp"] = millis();
      
// Escreve o JSON atualizado de volta no arquivo
      logFile = SPIFFS.open("/log.json", "w");
      if (logFile) {
          serializeJson(doc, logFile);
          logFile.close();
          Serial.println("JSON atualizado no arquivo!");
      } else {
          Serial.println("Erro ao abrir log.json para escrita!");
      }
  
      // Exibe o log no Monitor Serial de forma limpa e organizada
      Serial.println("==============================");
      Serial.println("     Estado dos LEDs");
      Serial.println("==============================");
      
      // Extrai os valores para exibir no formato desejado
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
  
      // Exibe o timestamp
      long timestamp = millis();
      Serial.print("Timestamp: ");
      Serial.println(timestamp);
  
      Serial.println("==============================");
  }
  
  #endif

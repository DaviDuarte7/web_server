# 🔌 Sistema de Controle de LEDs com ESP32

Controle LEDs via navegador, registre ações em JSON e visualize tudo em tempo real — tudo isso hospedado direto no seu ESP32.

---

## ✨ O que o projeto faz

Este sistema permite que você controle 3 LEDs físicos conectados ao ESP32 por meio de uma interface web. Você pode:

- Ligar/desligar os LEDs individualmente
- Ativar o modo "piscar" em loop entre os LEDs
- Verificar o estado atual de cada LED
- Alterar o intervalo de tempo do piscar
- Registrar automaticamente todas as ações (ligar/desligar/piscar) em um arquivo `log.json` armazenado no SPIFFS

---

## ⚙️ Pré-requisitos

- Placa ESP32
- Arduino IDE
- Protoboard para conexão de fios e LEDs
- Fios para conexão dos LEDs
- Bibliotecas:
  - `ESPAsyncWebServer`
  - `AsyncTCP`
  - `SPIFFS`
  - `ArduinoJson`

---

## 🚀 Instalação e Execução

### 1. Instalar o Arduino IDE

Baixe aqui:  
🔗 [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)

---

### 2. Instalar suporte ao ESP32

- Vá em: `Arquivo > Preferências`
- No campo **"URLs Adicionais para Gerenciador de Placas"**, adicione:

https://dl.espressif.com/dl/package_esp32_index.json

- Logo em seguida vá em: `Ferramentas > Placa > Gerenciador de Placas`
- Busque por “ESP32” e clique em instalar

---

### 3. Instalar bibliotecas necessárias

Abra o **Gerenciador de Bibliotecas** e instale:

- `ESPAsyncWebServer`
- `AsyncTCP`
- `SPIFFS`
- `ArduinoJson`

---

### 4. Configurar sua rede Wi-Fi

No arquivo `HootController.h`, atualize as credenciais:

    cpp
const char* ssid = "NOME_DA_SUA_REDE";
const char* password = "SUA_SENHA";

### 5. Fazer upload do código e verificar monitor serial

Após ajustar a rede e carregar o código no ESP32:

    plaintext
- Clique em **Upload** no Arduino IDE
- Abra o **Monitor Serial** (configuração de baud: 115200)
- Aguarde a conexão com o Wi-Fi
- O endereço IP será exibido no console — copie esse IP e cole no navegador para acessar a interface web

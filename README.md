# 🔌 LED Control System with ESP32

Control LEDs via a web browser, log actions in JSON, and visualize everything in real-time — all hosted directly on your ESP32.

---

## ✨ What the project does

This system allows you to control 3 physical LEDs connected to the ESP32 through a web interface. You can:

- Turn LEDs on/off individually
- Activate the "blink" mode in a loop between the LEDs
- Check the current state of each LED
- Change the blink interval
- Automatically log all actions (on/off/blink) into a `log.json` file stored in the SPIFFS

---

## ⚙️ Prerequisites

- ESP32 board
- Arduino IDE
- Breadboard for connecting wires and LEDs
- Wires to connect the LEDs
- Libraries:
  - `ESPAsyncWebServer`
  - `AsyncTCP`
  - `SPIFFS`
  - `ArduinoJson`

---

## 🚀 Installation and Execution

### 1. Install the Arduino IDE

Download here:  
🔗 [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)

---

### 2. Install ESP32 support

- Go to: `File > Preferences`
- In the **"Additional Boards Manager URLs"** field, add:

https://dl.espressif.com/dl/package_esp32_index.json

- Then, go to: `Tools > Board > Board Manager`
- Search for "ESP32" and click install

---

### 3. Install the necessary libraries

Open the **Library Manager** and install:

- `ESPAsyncWebServer`
- `AsyncTCP`
- `SPIFFS`
- `ArduinoJson`

---

### 4. Configure your Wi-Fi network

In the `HootController.h` file, update the credentials:

- `const char* ssid = "YOUR_NETWORK_NAME";`
- `const char* password = "YOUR_PASSWORD";`

### 5. Upload the code and check the serial monitor

- After updating the network credentials, click "Upload"
- Open the **Serial Monitor** (baud rate: 115200)
- Wait for the ESP32 to connect to Wi-Fi
- The IP address will be displayed in the Serial Monitor console. Access the web interface by opening the IP address directly in your browser.

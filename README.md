# ESP-NOW Ultrasonic Distance Monitoring with ESP32

This project demonstrates wireless communication between two ESP32 boards using **ESP-NOW**. An **ultrasonic sensor** (HC-SR04) on the **transmitter** side measures distance and sends the data **only when a change is detected**. The **receiver** ESP32 receives and displays the distance.

## 📦 Features

- 📶 Uses ESP-NOW for fast, connectionless communication
- 📏 Ultrasonic sensor (HC-SR04) measures distance
- 🧠 Sends data only when distance changes
- 📺 Displays data on Serial Monitor of receiver

## 🛠️ Hardware Required

- 2x ESP32 Dev Boards
- 1x HC-SR04 Ultrasonic Sensor
- Jumper wires
- Breadboard

## 🔌 Pin Connections

| HC-SR04 Pin | ESP32 Pin |
|-------------|-----------|
| VCC         | 5V        |
| GND         | GND       |
| Trig        | GPIO 5    |
| Echo        | GPIO 18   |

## 🚀 Getting Started

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/espnow-ultrasonic-distance.git
   cd espnow-ultrasonic-distance

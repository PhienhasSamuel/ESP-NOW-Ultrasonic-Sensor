#include <WiFi.h>
#include <esp_now.h>

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing ESP-NOW Receiver...");

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();  // Disconnect from any previously connected WiFi
  delay(100);

  // Print MAC Address
  Serial.print("ESP32 MAC Address: ");
  Serial.println(WiFi.macAddress());

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the receive callback
  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("ESP-NOW Receiver Initialized");
}

// Correct callback for ESP32 core 2.0.7+
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  if (len == sizeof(float)) {
    float receivedDistance;
    memcpy(&receivedDistance, incomingData, sizeof(receivedDistance));

    Serial.print("📏 Distance Received: ");
    Serial.print(receivedDistance);
    Serial.println(" cm");

    Serial.print("📡 From MAC: ");
    char macStr[18];
    snprintf(macStr, sizeof(macStr),
             "%02X:%02X:%02X:%02X:%02X:%02X",
             info->src_addr[0], info->src_addr[1], info->src_addr[2],
             info->src_addr[3], info->src_addr[4], info->src_addr[5]);
    Serial.println(macStr);
  } else {
    Serial.print("⚠️ Invalid data size received: ");
    Serial.println(len);
  }
}

void loop() {
  // Nothing to do in loop for receiver
}

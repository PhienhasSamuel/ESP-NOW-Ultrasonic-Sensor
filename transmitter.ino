#include <WiFi.h>
#include <esp_now.h>

// Define ultrasonic sensor pins
const int trigPin = 5;
const int echoPin = 18;

// MAC address of receiver ESP32 (replace with your receiver's MAC)
uint8_t receiverMAC[] = {0xF4, 0x65, 0x0B, 0x4A, 0x88, 0xC0};

// Previous distance to compare changes
float previousDistance = -1;

// Threshold for change detection (in cm)
const float threshold = 2.0;

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing ESP-NOW Transmitter...");

  // Setup ultrasonic pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Initialize Wi-Fi in station mode
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.print("ESP32 MAC Address: ");
  Serial.println(WiFi.macAddress());

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (!esp_now_is_peer_exist(receiverMAC)) {
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
      Serial.println("Failed to add peer");
      return;
    }
  }

  // Register send callback
  esp_now_register_send_cb(OnDataSent);

  Serial.println("ESP-NOW Transmitter Initialized");
}

// Callback for sending data
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success ✅" : "Fail ❌");
}

// Read distance from ultrasonic sensor
float readUltrasonicDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);  // timeout: 30ms
  float distance = duration * 0.034 / 2;          // convert to cm

  if (distance == 0 || distance >= 400) {
    return -1; // Invalid reading
  }

  return distance;
}

void loop() {
  float currentDistance = readUltrasonicDistance();

  if (currentDistance > 0 && abs(currentDistance - previousDistance) > threshold) {
    Serial.print("📤 Sending Distance: ");
    Serial.print(currentDistance);
    Serial.println(" cm");

    esp_err_t result = esp_now_send(receiverMAC, (uint8_t *)&currentDistance, sizeof(currentDistance));

    if (result == ESP_OK) {
      Serial.println("✅ Data Sent");
    } else {
      Serial.print("❌ Error Sending Data: ");
      Serial.println(result);
    }

    previousDistance = currentDistance;
  }

  delay(1000); // Check every second
}

#include <ESP8266WiFi.h>
#include <espnow.h>

const int touchPin = 5;

int prevState = 0;

//Reciever MAC Address: 84:0D:8E:8D:27:A9
// REPLACE WITH RECEIVER MAC Address
uint8_t broadcastAddress[] = { 0x84, 0x0D, 0x8E, 0x8D, 0x27, 0xA9 };

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  bool button_state;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0) {
    Serial.println("Delivery success");
  } else {
    Serial.println("Delivery fail");
  }
}

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  pinMode(touchPin, INPUT);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop() {

  int touchState = digitalRead(touchPin);

  // Serial.println(touchState);

  if (touchState != prevState) {

    prevState = touchState;

    if (touchState == HIGH) {
      myData.button_state = 0;
    } else {
      myData.button_state = 1;
    }

    // Send message via ESP-NOW
    esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));

  }
}
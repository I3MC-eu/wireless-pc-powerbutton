#include <ESP8266WiFi.h>
#include <espnow.h>

//Reciever MAC Address: 84:0D:8E:8D:27:A9

const int BUTTON_PIN = 5;  // Define the button pin
const int RELAY_PIN = 14;  // Define the relay pin

bool touchState = HIGH;

typedef struct struct_message {
  bool button_state;
} struct_message;


struct_message myData;


void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));

  Serial.println(myData.button_state);

  touchState = myData.button_state;

  // digitalWrite(RELAY_PIN, !myData.button_state);  // Turn the relay on
}

void setup() {
  Serial.begin(115200);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);

  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Set the button pin as input with pull-up resistor
  pinMode(RELAY_PIN, OUTPUT);         // Set the relay pin as output
  digitalWrite(RELAY_PIN, HIGH);      // digitalWrite(RELAY_PIN, HIGH);
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW || touchState == LOW) {  // If the button is pressed
    digitalWrite(RELAY_PIN, LOW);                             // Turn the relay on
  } else {
    digitalWrite(RELAY_PIN, HIGH);  // Turn the relay off
  }
}

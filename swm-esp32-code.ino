#include <WiFi.h>
#include <HTTPClient.h>
#include <HardwareSerial.h>

// Wi-Fi credentials
const char* ssid = "chandraprasad@vianet";
const char* password = "chandraprasad459950";

// Server URL
const String serverUrl = "http://192.168.1.14:5000";

// Variables to store previous value
int prevState = -1;

void setup() {
  Serial.begin(9600);  // Start Serial communication with Arduino
  WiFi.begin(ssid, password);  // Connect to Wi-Fi
  
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to Wi-Fi");
}

void loop() {
  if (Serial.available()) {
    String receivedData = Serial.readStringUntil('\n');  // Read the full line of data
    
    // Convert received data to integer (Assuming it's the state value)
    int state = receivedData.toInt();
    Serial.println("Received state value: " + String(state));

    // If the value has changed, send it to the server
    if (state != prevState) {
      sendToServer(state);
      prevState = state;  // Update previous value
    }
  }
}

void sendToServer(int state) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    String url = serverUrl + "?m=" + String(state);  // Send state value as query parameter
    http.begin(url);  // Begin HTTP request to the server

    int httpCode = http.GET();  // Send GET request
    
    if (httpCode > 0) {
      Serial.println("Data sent successfully: m = " + String(state));
      Serial.println("HTTP Response code: " + String(httpCode));
    } else {
      Serial.println("Error in sending data");
    }
    
    http.end();  // End HTTP request
  } else {
    Serial.println("WiFi not connected.");
  }
}

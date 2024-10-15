#include <WiFi.h>        // For ESP32. Use <ESP8266WiFi.h> for ESP8266
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include "ArduRoomba.h"  // Roomba control library

// Wi-Fi network credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Wi-Fi and UDP setup
WiFiUDP udp;
const unsigned int localPort = 8888;  // Port for receiving OSC messages

ArduRoomba roomba(2, 3, 4);  // rxPin, txPin, brcPin

void setup() {
  // Start serial communication for debugging
  Serial.begin(115200);

  // Start Roomba
  roomba.roombaSetup();
  Serial.println("Roomba Setup Complete");

  // Connect to Wi-Fi
  connectToWiFi();

  // Start listening for UDP messages on the defined port
  udp.begin(localPort);
  Serial.printf("Listening for OSC messages on UDP port %d\n", localPort);
}

void loop() {
  // Buffer for incoming OSC messages
  int packetSize = udp.parsePacket();
  
  if (packetSize) {
    // Create a buffer to hold the incoming packet
    char packetBuffer[packetSize];
    udp.read(packetBuffer, packetSize);  // Read incoming data

    // Create an OSCMessage and fill it with the incoming data
    OSCMessage msg;
    msg.fill(packetBuffer, packetSize);

    // Route the incoming OSC messages to specific functions
    msg.route("/roomba/forward", goForward);
    msg.route("/roomba/backward", goBackward);
    msg.route("/roomba/left", turnLeft);
    msg.route("/roomba/right", turnRight);
    msg.route("/roomba/stop", halt);
  }
}

// Connect to Wi-Fi function
void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// OSC handlers for Roomba commands
void goForward(OSCMessage &msg) {
  roomba.halt();
  delay(50);
  roomba.goForward();
  delay(50);
  Serial.println("OSC: Sent goForward to Roomba");
}

void goBackward(OSCMessage &msg) {
  roomba.halt();
  delay(50);
  roomba.goBackward();
  delay(50);
  Serial.println("OSC: Sent goBackward to Roomba");
}

void turnLeft(OSCMessage &msg) {
  roomba.halt();
  delay(50);
  roomba.turnLeft();
  delay(50);
  Serial.println("OSC: Sent turnLeft to Roomba");
}

void turnRight(OSCMessage &msg) {
  roomba.halt();
  delay(50);
  roomba.turnRight();
  delay(50);
  Serial.println("OSC: Sent turnRight to Roomba");
}

void halt(OSCMessage &msg) {
  roomba.halt();
  delay(50);
  Serial.println("OSC: Sent halt to Roomba");
}

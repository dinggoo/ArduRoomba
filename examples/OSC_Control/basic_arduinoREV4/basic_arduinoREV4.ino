#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include "ArduRoomba.h"  // Include the ArduRoomba library

// WiFi credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Define UDP
WiFiUDP Udp;
const unsigned int localPort = 8888;  // Local port to listen for OSC messages

// Buffer for incoming packets
char packetBuffer[255];

// Initialize ArduRoomba object
ArduRoomba roomba;

// Function declarations
void goForward(OSCMessage &msg, int addrOffset);
void goBackward(OSCMessage &msg, int addrOffset);
void turnLeft(OSCMessage &msg, int addrOffset);
void turnRight(OSCMessage &msg, int addrOffset);
void halt(OSCMessage &msg, int addrOffset);

void setup() {
  // Start serial communication
  Serial.begin(115200);

  // Initialize Roomba
  roomba.begin();

  // Connect to WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Start UDP
  Udp.begin(localPort);
  Serial.print("Listening for OSC messages on UDP port ");
  Serial.println(localPort);
}

void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    // Receive incoming UDP packet
    Udp.read(packetBuffer, 255);

    OSCMessage msg;
    msg.fill((uint8_t*)packetBuffer, packetSize);

    // Route the message based on the OSC address pattern
    msg.route("/roomba/forward", goForward);
    msg.route("/roomba/backward", goBackward);
    msg.route("/roomba/left", turnLeft);
    msg.route("/roomba/right", turnRight);
    msg.route("/roomba/stop", halt);
  }
}

// Function to handle forward movement
void goForward(OSCMessage &msg, int addrOffset) {
  Serial.println("Moving forward");
  roomba.drive(200, 32767);  // Move forward at speed 200
}

// Function to handle backward movement
void goBackward(OSCMessage &msg, int addrOffset) {
  Serial.println("Moving backward");
  roomba.drive(-200, 32767);  // Move backward at speed -200
}

// Function to handle turning left
void turnLeft(OSCMessage &msg, int addrOffset) {
  Serial.println("Turning left");
  roomba.drive(200, 1);  // Turn left
}

// Function to handle turning right
void turnRight(OSCMessage &msg, int addrOffset) {
  Serial.println("Turning right");
  roomba.drive(200, -1);  // Turn right
}

// Function to handle stopping the Roomba
void halt(OSCMessage &msg, int addrOffset) {
  Serial.println("Stopping");
  roomba.drive(0, 0);  // Stop the Roomba
}

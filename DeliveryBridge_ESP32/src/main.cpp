/**
 * ESP32 Socket.IO Client for ROS2 Delivery Bridge
 * 
 * A Socket.IO implementation for ESP32 that connects to the
 * delivery_bridge ROS2 package to send cmd_vel commands.
 * 
 * Features:
 * - Socket.IO communication with delivery_bridge
 * - cmd_vel event emission for robot control
 * - Automatic reconnection logic
 * - Event callbacks
 * 
 * @author Kaléin Tamaríz - TheBIGduke
 * @version 2.0.0
 *
 */

#include <Arduino.h>            // Arduino core functions for PlatformIO
#include <WiFi.h>               // ESP32 WiFi functionality
#include <SocketIOclient.h>     // Socket.IO client library

// ============= CONFIGURATION SECTION =============
// Update these values for your specific setup

// WiFi credentials
const char* ssid = "ESP32";
const char* password = "12345678";

// Socket.IO server (replace with your computer's IP where delivery_bridge is running)
const char* socketio_host = "192.168.0.102";
const int socketio_port = 9009;

SocketIOclient socketIO;



// Socket.IO event handler
void socketIOEvent(socketIOmessageType_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case sIOtype_DISCONNECT:
      Serial.printf("[IOc] Disconnected!\n");
      break;
    case sIOtype_CONNECT:
      Serial.printf("[IOc] Connected to url: %s\n", payload);
      // Join default namespace (no auto join in Socket.IO)
      socketIO.send(sIOtype_CONNECT, "/");
      break;
    case sIOtype_EVENT:
      Serial.printf("[IOc] get event: %s\n", payload);
      break;
    case sIOtype_ACK:
      Serial.printf("[IOc] get ack: %u\n", length);
      break;
    case sIOtype_ERROR:
      Serial.printf("[IOc] get error: %u\n", length);
      break;
    case sIOtype_BINARY_EVENT:
    case sIOtype_BINARY_ACK:
      // Not used in this example
      break;
  }
}

void setup() {
  Serial.begin(9600);
  Serial.setDebugOutput(true);
  
  // Connect to WiFi network
  WiFi.begin(ssid, password);

  // Verify connection and return state
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Setup Socket.IO client
  socketIO.begin(socketio_host, socketio_port, "/socket.io/?EIO=4");
  socketIO.onEvent(socketIOEvent);
}

void loop() {
  socketIO.loop();
  
  // Send cmd_vel command every 3 seconds for testing
  static unsigned long lastCmdVel = 0;
  static float direction = 1.0; // Alternate between forward/backward
  
  if (millis() - lastCmdVel > 3000) {
    lastCmdVel = millis();
    
    // Create cmd_vel JSON payload
    String payload = "[\"cmd_vel\",{\"linear_x\":" + String(direction * 0.2) + ",\"angular_z\":" + String(direction * 0.1) + "}]";
    
    // Send cmd_vel event to delivery_bridge
    socketIO.sendEVENT(payload);
    
    Serial.println("Sent cmd_vel: linear_x=" + String(direction * 0.2) + ", angular_z=" + String(direction * 0.1));
    
    // Alternate direction for next command
    direction *= -1.0;
  }
  
  delay(10); // Prevent watchdog triggers
}
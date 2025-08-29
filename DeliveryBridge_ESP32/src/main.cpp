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
 * - State machine for movement demo
 *
 * @author Kaléin Tamaríz - TheBIGduke
 * @version 2.1.0
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
const char* socketio_host = "192.168.0.103";
const int socketio_port = 9009;

// Movement parameters
const float LINEAR_SPEED = 0.3;   // Linear velocity (m/s)
const float ANGULAR_SPEED = 0.5;  // Angular velocity (rad/s)
const unsigned long STATE_DURATION = 3000; // Duration for each state (ms)

// Robot states
enum RobotState {
  FORWARD,
  ROTATE_RIGHT,
  ROTATE_LEFT,
  BACKWARD,
  STOP
};

SocketIOclient socketIO;

// State machine variables
RobotState currentState = FORWARD;
unsigned long stateStartTime = 0;
unsigned long lastCmdVel = 0;
const unsigned long CMD_VEL_INTERVAL = 100; // Interval to send cmd_vel commands

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

String getStateName(RobotState state) {
  switch(state) {
    case FORWARD: return "FORWARD";
    case ROTATE_RIGHT: return "ROTATE_RIGHT";
    case ROTATE_LEFT: return "ROTATE_LEFT";
    case BACKWARD: return "BACKWARD";
    case STOP: return "STOP";
    default: return "UNKNOWN";
  }
}

void sendCmdVel(float linear_x, float angular_z) {
  // Create cmd_vel JSON payload
  String payload = "[\"cmd_vel\",{\"linear_x\":" + String(linear_x) + ",\"angular_z\":" + String(angular_z) + "}]";
  
  // Send cmd_vel event to delivery_bridge
  socketIO.sendEVENT(payload);
  
  Serial.println("State: " + getStateName(currentState) + " | linear_x=" + String(linear_x) + ", angular_z=" + String(angular_z));
}

void updateStateMachine() {
  unsigned long currentTime = millis();
  
  // Check if it's time to transition to the next state
  if (currentTime - stateStartTime >= STATE_DURATION) {
    stateStartTime = currentTime;
    
    // Chain of events: FORWARD -> ROTATE_RIGHT -> ROTATE_LEFT -> BACKWARD -> STOP -> repeat
    switch(currentState) {
      case FORWARD:
        currentState = ROTATE_RIGHT;
        Serial.println("=== Transitioning to ROTATE_RIGHT ===");
        break;
      case ROTATE_RIGHT:
        currentState = ROTATE_LEFT;
        Serial.println("=== Transitioning to ROTATE_LEFT ===");
        break;
      case ROTATE_LEFT:
        currentState = BACKWARD;
        Serial.println("=== Transitioning to BACKWARD ===");
        break;
      case BACKWARD:
        currentState = STOP;
        Serial.println("=== Transitioning to STOP ===");
        break;
      case STOP:
        currentState = FORWARD;
        Serial.println("=== Restarting cycle - FORWARD ===");
        break;
    }
  }
}

void executeCurrentState() {
  float linear_x = 0.0;
  float angular_z = 0.0;
  
  switch(currentState) {
    case FORWARD:
      linear_x = LINEAR_SPEED;
      angular_z = 0.0;
      break;
      
    case ROTATE_RIGHT:
      linear_x = 0.0;
      angular_z = -ANGULAR_SPEED; // Negative for right rotation
      break;
      
    case ROTATE_LEFT:
      linear_x = 0.0;
      angular_z = ANGULAR_SPEED;  // Positive for left rotation
      break;
      
    case BACKWARD:
      linear_x = -LINEAR_SPEED;   // Negative for backward
      angular_z = 0.0;
      break;
      
    case STOP:
      linear_x = 0.0;
      angular_z = 0.0;
      break;
  }
  
  sendCmdVel(linear_x, angular_z);
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
  
  // Initialize state machine
  stateStartTime = millis();
  Serial.println("=== Robot Movement Demo Started ===");
  Serial.println("Chain of events: FORWARD -> ROTATE_RIGHT -> ROTATE_LEFT -> BACKWARD -> STOP -> Repeat");
  Serial.println("Each state duration: " + String(STATE_DURATION/1000) + " seconds");
  Serial.println("=== Starting with FORWARD ===");
}

void loop() {
  socketIO.loop();
  
  // Update state machine
  updateStateMachine();
  
  // Send cmd_vel commands at regular intervals
  if (millis() - lastCmdVel >= CMD_VEL_INTERVAL) {
    lastCmdVel = millis();
    executeCurrentState();
  }
 
  delay(10); // Prevent watchdog triggers
}
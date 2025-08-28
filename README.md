# ESP32 Socket.IO Client for ROS2 Bridge

[![PlatformIO](https://img.shields.io/badge/PlatformIO-Compatible-orange.svg)](https://platformio.org/)
[![ESP32](https://img.shields.io/badge/ESP32-Compatible-blue.svg)](https://www.espressif.com/en/products/socs/esp32)
[![ROS2](https://img.shields.io/badge/ROS2-Humble%2B-blue.svg)](https://docs.ros.org/en/humble/)
[![Socket.IO](https://img.shields.io/badge/Socket.IO-4.0%2B-green.svg)](https://socket.io/)

A Socket.IO client implementation for ESP32 using PlatformIO that enables real-time communication between your ESP32 device and a modified ROS2 delivery bridge system for robot control via cmd_vel commands. The delivery bridge has been optimized for ESP32 communication by removing web interface components while preserving all Socket.IO and API functionality.

## Features

- Real-time Socket.IO communication between ESP32 and ROS2 delivery bridge
- PlatformIO project structure for easy development
- Event-driven callbacks for connection status monitoring
- cmd_vel command transmission for robot control
- Automatic reconnection logic
- JSON-based message protocol
- Easy configuration and deployment
- **Modified delivery bridge** - Web interface removed, optimized for ESP32 access
- **No authentication required** - Direct API access for ESP32
- **Real-time robot data** - Battery status, position updates, system status

## Table of Contents

- [Requirements](#requirements)
- [Quick Start](#quick-start)
- [Installation](#installation)
- [Configuration](#configuration)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [API Reference](#api-reference)
- [Contributing](#contributing)

## Requirements

### Hardware
- **ESP32 Development Board** (ESP32 DevKitC, NodeMCU-32S, or compatible)
- **USB Cable** (micro-USB or USB-C depending on your board)
- **Computer** with available USB port
- **Robot** compatible with ROS2 cmd_vel commands (optional for testing)

### Software
- **Visual Studio Code** (recommended) or any text editor
- **PlatformIO IDE** extension for VS Code
- **ROS2** (Humble or newer) for the delivery bridge
- **Python 3.8+** for the delivery bridge server
- **Git** for cloning repositories

## Quick Start

1. **Clone this repository (includes modified delivery_bridge)**
   ```bash
   cd ~/colcon_ws/src
   git clone https://github.com/TheBIGduke/ESP32-SocketIO-ROS2-Bridge.git
   cd ESP32-SocketIO-ROS2-Bridge/delivery_bridge
   pip install -r requirements.txt
   ```

2. **Build and test the delivery bridge**
   ```bash
   cd ~/colcon_ws/
   colcon build --packages-select delivery_bridge --symlink-install --allow-overriding delivery_bridge
   source install/setup.bash
   ros2 run delivery_bridge server_node
   ```

3. **Configure WiFi and server settings** in `DeliveryBridge_ESP32/src/main.cpp`

4. **Upload to ESP32** using PlatformIO

5. **Test ESP32 connection** - Server should be ready for ESP32 Socket.IO communication

## Installation

### Step 1: Install Development Environment

#### Option A: Visual Studio Code + PlatformIO (Recommended)

1. **Download and install [Visual Studio Code](https://code.visualstudio.com/)**

2. **Install PlatformIO IDE extension:**
   - Open VS Code
   - Go to Extensions (`Ctrl+Shift+X` / `Cmd+Shift+X`)
   - Search for "PlatformIO IDE"
   - Click "Install"
   - Restart VS Code when prompted

#### Option B: PlatformIO Core CLI

```bash
# Install PlatformIO Core
pip install platformio

# Verify installation
pio --version
```

### Step 2: Setup Modified ROS2 Delivery Bridge

**Important:** This project includes a modified version of the delivery bridge ROS2 package optimized for ESP32 communication (web interface removed, direct API access enabled).

**Prerequisites:** ROS2 Humble or newer installed on your system. Follow [ROS2 installation guide](https://docs.ros.org/en/humble/Installation.html) if needed.

1. **Clone this repository (includes modified delivery_bridge):**
   ```bash
   cd ~/colcon_ws/src
   git clone https://github.com/TheBIGduke/ESP32-SocketIO-ROS2-Bridge.git
   cd ESP32-SocketIO-ROS2-Bridge
   ```

2. **Install Python dependencies:**
   ```bash
   cd delivery_bridge
   pip install -r requirements.txt
   ```

3. **Build the ROS2 package:**
   ```bash
   cd ~/colcon_ws/
   colcon build --packages-select delivery_bridge --symlink-install --allow-overriding delivery_bridge
   source install/setup.bash
   ```

4. **Test the modified delivery bridge:**
   ```bash
   ros2 run delivery_bridge server_node
   ```
   
   Expected output:
   ```
   [INFO] [server_node]: Starting node BaseNode ...
   [INFO] [server_node]: Database created successfully.
   [INFO] [server_node]: FunctionManager thread started
   INFO: Uvicorn running on http://0.0.0.0:9009
   INFO: connection open
   ```

5. **Verify the server (ESP32-optimized):**
   - Server listening on port 9009: ✅
   - API documentation: `http://127.0.0.1:9009/docs` ✅
   - Socket.IO ready for ESP32 connections ✅
   - No web interface (removed for optimization) ✅

**Key Modifications Made:**
- Removed web interface components (templates, static files, user authentication)
- Preserved all Socket.IO and API functionality for ESP32
- No authentication required for direct ESP32 access
- CORS enabled for cross-origin requests

### Step 3: Setup ESP32 Project

1. **Navigate to ESP32 project (already cloned in Step 2):**
   ```bash
   cd ~/colcon_ws/src/ESP32-SocketIO-ROS2-Bridge/DeliveryBridge_ESP32
   ```

2. **Open in VS Code:**
   - Launch VS Code
   - File → Open Folder
   - Select the `ESP32-SocketIO-ROS2-Bridge` directory

3. **Wait for PlatformIO to initialize:**
   - PlatformIO will automatically detect the project
   - Dependencies will be installed automatically
   - Check the PlatformIO terminal for completion

## Configuration

### WiFi Configuration

Edit `DeliveryBridge_ESP32/src/main.cpp` and update your network credentials:

```cpp
// WiFi credentials
const char* ssid = "ESP32";
const char* password = "12345678";
```

### Socket.IO Server Configuration

1. **Find your computer's IP address (where delivery_bridge is running):**

   **Windows:**
   ```cmd
   ipconfig
   ```
   
   **macOS/Linux:**
   ```bash
   ifconfig
   # or
   ip addr show
   ```

2. **Update the server address in `DeliveryBridge_ESP32/src/main.cpp`:**
   ```cpp
   // Socket.IO server (replace with your computer's IP where delivery_bridge is running)
   const char* socketio_host = "192.168.0.102";
   const int socketio_port = 9009;
   ```
   Replace `192.168.0.102` with your computer's IP address.

### Advanced Configuration

You can modify these settings in `DeliveryBridge_ESP32/src/main.cpp`:

```cpp
// Serial Configuration (currently set to 9600)
Serial.begin(9600);

// cmd_vel sending interval (currently 3 seconds for testing)
if (millis() - lastCmdVel > 3000) {
  // Send cmd_vel logic
}

// cmd_vel values for testing
String payload = "[\"cmd_vel\",{\"linear_x\":" + String(direction * 0.2) + ",\"angular_z\":" + String(direction * 0.1) + "}]";
```

## Usage

### 1. Start the Modified ROS2 Delivery Bridge

First, ensure your modified delivery bridge is running:

```bash
# Navigate to your ROS2 workspace
cd /path/to/your/colcon_ws

# Source ROS2 environment
source /opt/ros/humble/setup.bash
source install/setup.bash

# Start the delivery bridge server
ros2 run delivery_bridge server_node
```

Expected output:
```
[INFO] [server_node]: Starting node BaseNode ...
[INFO] [server_node]: Database created successfully.
[INFO] [server_node]: FunctionManager thread started
INFO: Uvicorn running on http://0.0.0.0:9009
INFO: connection open
```

**Key differences from original:**
- No web interface startup (removed)
- No user authentication required
- Direct Socket.IO and API access for ESP32
- Optimized for embedded device communication

### 2. Upload Code to ESP32

#### Using VS Code + PlatformIO:
1. Connect ESP32 via USB
2. Open the `DeliveryBridge_ESP32` folder in VS Code
3. Click PlatformIO icon in sidebar
4. Project Tasks → esp32doit-devkit-v1 → General → Upload
5. Monitor progress in terminal

#### Using PlatformIO CLI:
```bash
# Navigate to ESP32 project directory
cd DeliveryBridge_ESP32

# Build the project
pio run

# Upload to device
pio run -t upload

# Monitor serial output
pio device monitor -b 9600
```

### 3. Monitor Communication

#### Serial Monitor (ESP32):
```bash
# From DeliveryBridge_ESP32 directory
pio device monitor -b 9600
```

Expected ESP32 output:
```
Connecting to WiFi...
Connected to WiFi
IP address: 192.168.0.101
[IOc] Connected to url: /socket.io/?EIO=4
Sent cmd_vel: linear_x=0.20, angular_z=0.10
Sent cmd_vel: linear_x=-0.20, angular_z=-0.10
```

#### Delivery Bridge Console:
Check the delivery bridge server logs for incoming Socket.IO connections and cmd_vel commands.

## Project Structure

```
ESP32-SocketIO-ROS2-Bridge/
├── DeliveryBridge_ESP32/
│   ├── src/
│   │   └── main.cpp                 # ESP32 Socket.IO client code
│   ├── include/                     # (empty directory)
│   ├── lib/                         # (empty directory)
│   ├── platformio.ini               # PlatformIO configuration
│   └── .vscode/
│       └── extensions.json          # VS Code extensions
├── src/                             # (empty directory)
├── .gitignore                       # Git ignore file
└── README.md                        # This file
```

## API Reference

### ESP32 Client Functions

The main code uses the ArduinoWebsockets library (Socket.IO compatible) with these key components:

```cpp
// Socket.IO client instance
SocketIOclient socketIO;

// Connection
socketIO.begin(socketio_host, socketio_port, "/socket.io/?EIO=4");

// Event handler
socketIO.onEvent(socketIOEvent);

// Send cmd_vel event
String payload = "[\"cmd_vel\",{\"linear_x\":0.2,\"angular_z\":0.1}]";
socketIO.sendEVENT(payload);

// Poll for messages
socketIO.loop();
```

### Modified Delivery Bridge API

The delivery bridge now provides these ESP32-optimized endpoints:

#### HTTP REST API (No authentication required)
```cpp
// Robot control
POST /ros/functionality_mode/     // Set robot mode
{"mode": "delivery"}

// Navigation
GET  /waypoints/waypoint         // Get available waypoints
POST /navigation/set_goal/{id}   // Navigate to waypoint
POST /navigation/set_wp/pause    // Pause navigation
POST /navigation/set_wp/resume   // Resume navigation
POST /navigation/set_wp/stop     // Stop navigation
```

#### Socket.IO Events
```cpp
// Outgoing (ESP32 → Server)
["cmd_vel", {"linear_x": 0.2, "angular_z": 0.1}]  // Robot movement

// Incoming (Server → ESP32)
["battery", {"voltage": 12.5, "percentage": 85.2}]     // Battery status
["robot_pose", {"position_x": 1.2, "position_y": 3.4, "orientation": 0.5}]  // Position
["on_status_change", {"general": {"ready": true}}]     // System status
```

### Socket.IO Events

| Event | Description |
|-------|-------------|
| `sIOtype_CONNECT` | Successfully connected to server |
| `sIOtype_DISCONNECT` | Disconnected from server |
| `sIOtype_EVENT` | Received event from server |
| `sIOtype_ACK` | Received acknowledgment |
| `sIOtype_ERROR` | Connection or protocol error |

### cmd_vel Message Format

The ESP32 sends cmd_vel commands in this JSON format:

```json
[
  "cmd_vel",
  {
    "linear_x": 0.2,
    "angular_z": 0.1
  }
]
```

Where:
- `linear_x`: Forward/backward velocity (m/s)
- `angular_z`: Rotational velocity (rad/s)

## PlatformIO Commands

### Development Commands
```bash
# Build project
pio run

# Build and upload
pio run -t upload

# Clean build files
pio run -t clean

# Monitor serial output
pio device monitor

# List connected devices
pio device list

# Update platforms and libraries
pio update
```

### Library Management
```bash
# Install library
pio lib install "library_name"

# List installed libraries  
pio lib list

# Search for libraries
pio lib search "socket.io"
```

## Troubleshooting

### Common Issues

#### ESP32 Won't Connect to WiFi
**Symptoms:** ESP32 continuously attempts WiFi connection
- Verify WiFi credentials in `DeliveryBridge_ESP32/src/main.cpp`
- Ensure WiFi network is 2.4GHz (ESP32 doesn't support 5GHz)
- Check ESP32 is within range of router
- Try different WiFi network for testing

#### Socket.IO Connection Failed
**Symptoms:** WiFi connected but Socket.IO connection fails
- Verify delivery bridge server is running and accessible
- Check server IP address and port in ESP32 code
- Ensure both devices are on same network
- Confirm delivery bridge is listening on port 9009
- Test server accessibility: `telnet YOUR_IP 9009`

#### Upload/Build Errors
**Symptoms:** PlatformIO compilation or upload errors
```bash
# Clean and rebuild
pio run -t clean
pio run

# Update PlatformIO
pio upgrade --dev

# Check device connection
pio device list
```

#### Serial Monitor Shows Nothing
**Symptoms:** No output in serial monitor
- Verify baud rate (9600 - as set in the code)
- Check USB cable and connection
- Try different USB port
- Press ESP32 reset button
- Verify correct COM port selection

#### Modified Delivery Bridge Issues
**Symptoms:** ESP32 connects but robot doesn't move
- Verify modified delivery bridge ROS2 setup is complete
- Check ROS2 nodes are running properly: `ros2 topic list`
- Confirm cmd_vel topic is being published: `ros2 topic echo /cmd_vel`
- Test Socket.IO connection: Check server logs for "connection open"
- Verify API endpoints: Visit `http://YOUR_IP:9009/docs`
- Check delivery bridge logs for received Socket.IO events

#### Web Interface Removed
**Note:** The original web interface has been removed for ESP32 optimization
- No login page or user authentication
- No static files (CSS, JS, images)
- Direct API access without authentication
- Socket.IO server still fully functional
- API documentation available at `/docs` endpoint

### Performance Optimization

For better performance:
- Increase WiFi TX power: `WiFi.setTxPower(WIFI_POWER_19_5dBm)`
- Adjust cmd_vel sending frequency (currently 3 seconds for testing)
- Optimize message content size
- Use appropriate cmd_vel values for your robot

## Contributing

We welcome contributions!

### Development Setup
1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test with actual delivery bridge setup
5. Submit a pull request

### Acknowledgments

This project builds upon the excellent work of the [delivery_bridge](https://github.com/cmauricioae8/delivery_bridge) ROS2 package created by **C. Mauricio Arteaga-Escamilla** ([@cmauricioae8](https://github.com/cmauricioae8)). 

**Modifications for ESP32 Optimization:**
- Removed web interface components (templates, static files, user authentication)
- Preserved all Socket.IO and API functionality
- Enabled direct ESP32 access without authentication
- Optimized for embedded device communication
- Maintained full ROS2 integration and robot control capabilities

**Original Author Credits:**
- **Author:** C. Mauricio Arteaga-Escamilla
- **GitHub:** [@cmauricioae8](https://github.com/cmauricioae8)
- **Repository:** [delivery_bridge](https://github.com/cmauricioae8/delivery_bridge)
- **Location:** Mexico
- **Email:** cmauricioae8@gmail.com

**ESP32 Bridge Implementation:**
- **Author:** Kaléin Tamaríz - TheBIGduke
- **Modifications:** Web interface removal, ESP32 optimization
- **Version:** 2.0.0

---

**Made with care for the ESP32 and ROS2 community**

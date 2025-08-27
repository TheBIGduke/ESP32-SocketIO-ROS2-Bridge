# ESP32 Socket.IO Client for ROS2 Bridge

[![PlatformIO](https://img.shields.io/badge/PlatformIO-Compatible-orange.svg)](https://platformio.org/)
[![ESP32](https://img.shields.io/badge/ESP32-Compatible-blue.svg)](https://www.espressif.com/en/products/socs/esp32)
[![ROS2](https://img.shields.io/badge/ROS2-Humble%2B-blue.svg)](https://docs.ros.org/en/humble/)
[![Socket.IO](https://img.shields.io/badge/Socket.IO-4.0%2B-green.svg)](https://socket.io/)

A Socket.IO client implementation for ESP32 using PlatformIO that enables real-time communication between your ESP32 device and a ROS2 delivery bridge system for robot control via cmd_vel commands.

## Features

- Real-time Socket.IO communication between ESP32 and ROS2 delivery bridge
- PlatformIO project structure for easy development
- Event-driven callbacks for connection status monitoring
- cmd_vel command transmission for robot control
- Automatic reconnection logic
- JSON-based message protocol
- Easy configuration and deployment

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

1. **Clone and setup the delivery bridge (ROS2 backend)**
   ```bash
   # Clone the delivery bridge repository (original author's work)
   git clone https://github.com/cmauricioae8/delivery_bridge.git
   cd delivery_bridge
   
   # Follow the delivery_bridge README instructions for ROS2 setup
   # This will handle the ROS2 package installation and configuration
   ```

2. **Clone this ESP32 project**
   ```bash
   git clone https://github.com/your-username/ESP32-SocketIO-ROS2-Bridge.git
   cd ESP32-SocketIO-ROS2-Bridge
   ```

3. **Configure WiFi and server settings** in `DeliveryBridge_ESP32/src/main.cpp`

4. **Upload to ESP32** using PlatformIO

5. **Start the delivery bridge server** (follow delivery_bridge repository instructions)

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

### Step 2: Setup ROS2 Delivery Bridge

**Important:** This project depends on the delivery bridge ROS2 package created by the original author. You must set this up first.

1. **Clone the delivery bridge repository:**
   ```bash
   git clone https://github.com/cmauricioae8/delivery_bridge.git
   cd delivery_bridge
   ```

2. **Follow the delivery bridge setup instructions:**
   - Refer to the delivery_bridge repository README for complete ROS2 installation
   - Install all required ROS2 dependencies
   - Configure the Socket.IO server settings
   - Test the delivery bridge functionality

3. **Verify delivery bridge is working:**
   - Start the ROS2 delivery bridge server
   - Confirm it's listening on the expected port (default: 9009)
   - Note your computer's IP address for ESP32 configuration

### Step 3: Setup ESP32 Project

1. **Clone or download this project:**
   ```bash
   git clone https://github.com/your-username/ESP32-SocketIO-ROS2-Bridge.git
   cd ESP32-SocketIO-ROS2-Bridge
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

### 1. Start the ROS2 Delivery Bridge

First, ensure your delivery bridge is running:

```bash
# Navigate to delivery bridge directory
cd delivery_bridge

# Start the ROS2 delivery bridge server
# (Follow specific instructions from delivery_bridge README)
# This typically involves:
# - Starting ROS2 nodes
# - Running the Socket.IO server
# - Confirming it's listening on port 9009
```

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

#### ROS2 Delivery Bridge Issues
**Symptoms:** ESP32 connects but robot doesn't move
- Verify delivery bridge ROS2 setup is complete
- Check ROS2 nodes are running properly
- Confirm cmd_vel topic is being published
- Test delivery bridge independently first
- Check delivery bridge logs for received commands

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

This project builds upon the excellent work of the [delivery_bridge](https://github.com/cmauricioae8/delivery_bridge) ROS2 package created by **C. Mauricio Arteaga-Escamilla** ([@cmauricioae8](https://github.com/cmauricioae8)). Please refer to that repository for the complete ROS2 backend implementation and setup instructions.

**Original Author Credits:**
- **Author:** C. Mauricio Arteaga-Escamilla
- **GitHub:** [@cmauricioae8](https://github.com/cmauricioae8)
- **Repository:** [delivery_bridge](https://github.com/cmauricioae8/delivery_bridge)
- **Location:** Mexico

---

**Made with care for the ESP32 and ROS2 community**

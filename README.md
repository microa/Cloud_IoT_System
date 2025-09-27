# 🌐 Cloud IoT System

A multi-device IoT information interaction system based on cloud computing services, supporting cloud storage and real-time monitoring of IoT node sensor information.

## 📋 Project Overview

This project implements a complete IoT system including:
- **Cloud Services**: TCP-based cloud servers for data reception, storage, and forwarding
- **Device End**: Support for 8051 and STM32 microcontrollers
- **Sensors**: DS18B20 temperature sensor
- **Communication Module**: ESP8266 WiFi module
- **Display Interface**: TFT LCD and OLED displays

## 🏗️ System Architecture

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   8051 Device   │    │   STM32 Device  │    │  Cloud Server  │
│                 │    │                 │    │                 │
│ ┌─────────────┐ │    │ ┌─────────────┐ │    │ ┌─────────────┐ │
│ │   DS18B20   │ │    │ │   OLED      │ │    │ │ TCP Server  │ │
│ │ Temperature │ │    │ │ Display     │ │    │ │ Port 1011   │ │
│ │  Sensor     │ │    │ │             │ │    │ │             │ │
│ └─────────────┘ │    │ └─────────────┘ │    │ └─────────────┘ │
│                 │    │                 │    │                 │
│ ┌─────────────┐ │    │ ┌─────────────┐ │    │ ┌─────────────┐ │
│ │   ESP8266   │ │    │ │   ESP8266   │ │    │ │ Data Logger │ │
│ │   WiFi      │ │    │ │   WiFi      │ │    │ │ Port 1013   │ │
│ │   Module    │ │    │ │   Module    │ │    │ │             │ │
│ └─────────────┘ │    │ └─────────────┘ │    │ └─────────────┘ │
│                 │    │                 │    │                 │
│ ┌─────────────┐ │    │ ┌─────────────┐ │    │ ┌─────────────┐ │
│ │   TFT LCD   │ │    │ │   LED       │ │    │ │ Data Sender │ │
│ │   Display   │ │    │ │   Control   │ │    │ │ Port 1014   │ │
│ └─────────────┘ │    │ └─────────────┘ │    │ └─────────────┘ │
└─────────────────┘    └─────────────────┘    └─────────────────┘
         │                       │                       │
         └───────────────────────┼───────────────────────┘
                                 │
                    ┌─────────────────┐
                    │   Internet      │
                    │   Connection   │
                    └─────────────────┘
```

## 🔧 Technology Stack

### Cloud Services (Cloud_Host_Service)
- **Language**: C++
- **Protocol**: TCP/IP
- **Features**: 
  - Data reception servers (Port 1011, 1013, 1014)
  - Temperature data logging
  - Data forwarding and acknowledgment
  - File storage management

### Device End (Device_8051)
- **Microcontroller**: STC12C5A60S2
- **Sensor**: DS18B20 temperature sensor
- **Communication**: ESP8266 WiFi module
- **Display**: TFT LCD screen
- **Features**: Temperature acquisition, WiFi connection, data upload

### Device End (Device_STM32)
- **Microcontroller**: STM32F103
- **Communication**: ESP8266 WiFi module
- **Display**: OLED display
- **Control**: LED control, key input
- **Features**: Remote control, status display

## 📁 Project Structure

```
Cloud_IoT_System/
├── Cloud_Host_Service/          # Cloud Services
│   ├── Tcp.Client.cpp           # TCP Client
│   ├── Send.Log.cpp             # Data Sending Service (Port 1014)
│   ├── Rcv.Log.cpp              # Data Reception Service (Port 1013)
│   └── Celsius.Rcv.Log.cpp      # Temperature Data Reception (Port 1011)
├── Device_8051/                  # 8051 Device End
│   ├── main.c                   # Main Program
│   ├── DS18B20.C                # Temperature Sensor Driver
│   ├── LCD.C                    # LCD Display Driver
│   ├── UART.C                   # Serial Communication
│   └── sys.C                    # System Functions
├── Device_STM32/                 # STM32 Device End
│   ├── main.c                   # Main Program
│   ├── HARDWARE/                # Hardware Drivers
│   │   ├── OLED/                # OLED Display
│   │   ├── LED/                 # LED Control
│   │   ├── KEY/                 # Key Processing
│   │   └── TIMER/               # Timer
│   └── SYSTEM/                  # System Libraries
│       ├── delay/               # Delay Functions
│       ├── sys/                 # System Configuration
│       └── usart/               # Serial Communication
└── Diagram/                      # System Diagrams
    ├── IoT System Diagram.jpg   # System Architecture Diagram
    └── STC.Hardware.PDF         # Hardware Design Diagram
```

## 🚀 Features

### 🌡️ Temperature Monitoring
- **Real-time Acquisition**: DS18B20 digital temperature sensor
- **Accuracy**: ±0.5°C
- **Range**: -55°C to +125°C
- **Display**: TFT LCD real-time temperature display

### 📡 Wireless Communication
- **WiFi Connection**: ESP8266 module
- **Protocol**: TCP/IP
- **Ports**: 
  - 1011: Temperature data reception
  - 1013: General data reception
  - 1014: Data sending service

### 💾 Data Storage
- **Cloud Storage**: Temperature data automatically saved to `CelsiusLog.bin`
- **Log Recording**: General data saved to `TcpLog.bin`
- **Data Format**: Binary file storage

### 🎛️ Remote Control
- **LED Control**: Support for remote LED switching
- **Command Format**: 
  - `RED`: Turn on red LED
  - `REDOFF`: Turn off red LED
  - `GREEN`: Turn on green LED
  - `GREENOFF`: Turn off green LED

## 📖 Usage Instructions

### 1. Cloud Service Deployment

```bash
# Compile cloud services
g++ -o TcpServer Send.Log.cpp -lws2_32
g++ -o RcvServer Rcv.Log.cpp -lws2_32
g++ -o CelsiusServer Celsius.Rcv.Log.cpp -lws2_32

# Run services
./TcpServer    # Data sending service (Port 1014)
./RcvServer    # Data reception service (Port 1013)
./CelsiusServer # Temperature data service (Port 1011)
```

### 2. Device End Configuration

#### 8051 Device Configuration
1. Modify WiFi connection information:
```c
// In main.c
SendString("AT+CWJAP=\"Your_WiFi_Name\",\"Your_WiFi_Password\"\r\n");
```

2. Configure server address:
```c
// Modify server IP and port
SendString("AT+CIPSTART=\"TCP\",\"Your_Server_IP\",1011\r\n");
```

#### STM32 Device Configuration
1. Configure WiFi connection:
```c
// In main.c
printf("AT+CWJAP=\"Your_WiFi_Name\",\"Your_WiFi_Password\"\r\n");
```

2. Set server connection:
```c
printf("AT+CIPSTART=\"TCP\",\"Your_Server_IP\",1014\r\n");
```

### 3. System Operation Flow

1. **Start Cloud Services**
   - Run three TCP servers
   - Listen on corresponding ports

2. **Device Connection**
   - 8051 device connects to temperature data port (1011)
   - STM32 device connects to data sending port (1014)

3. **Data Acquisition**
   - 8051 collects temperature every 30 seconds
   - Send to cloud via WiFi

4. **Data Storage**
   - Cloud automatically saves temperature data
   - Support data query and forwarding

## 🔧 Hardware Requirements

### 8051 Device
- **MCU**: STC12C5A60S2
- **Sensor**: DS18B20 temperature sensor
- **Communication**: ESP8266 WiFi module
- **Display**: TFT LCD screen
- **Power**: 5V supply

### STM32 Device
- **MCU**: STM32F103 series
- **Communication**: ESP8266 WiFi module
- **Display**: OLED display (SSD1306)
- **Control**: LED indicators, keys
- **Power**: 3.3V supply

## 📊 Data Format

### Temperature Data Format
```
Format: TXXXX
Example: T2345 (represents 23.45°C)
```

### Control Command Format
```
LED Control:
- RED: Turn on red LED
- REDOFF: Turn off red LED
- GREEN: Turn on green LED
- GREENOFF: Turn off green LED
```

## 🛠️ Development Environment

### Cloud Services
- **Compiler**: GCC/G++
- **Platform**: Windows
- **Library**: WinSock2

### Device End
- **8051**: Keil C51
- **STM32**: Keil MDK-ARM
- **Debugging**: ST-Link debugger

## 📈 Performance Metrics

- **Temperature Accuracy**: ±0.5°C
- **Response Time**: <1 second
- **Communication Range**: WiFi coverage area
- **Data Update**: 30-second intervals
- **Storage Capacity**: Unlimited (depends on server storage)

## 🔒 Security Features

- **Data Encryption**: TCP protocol transmission
- **Access Control**: Port access restrictions
- **Data Backup**: Local file storage
- **Error Handling**: Automatic reconnection on connection exceptions

## 📝 Changelog

- **v1.0** (2015-03): Initial version
- **v1.4** (2015-05): Added dedicated temperature data service
- **v1.0** (2015-05): STM32 device end completed

## 🤝 Contributing

Welcome to submit Issues and Pull Requests to improve this project!

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 📞 Contact

- **Project Demo Video**: [YouTube Link](https://youtu.be/n4Au37AL_gw)
- **Author**: Binhua Huang
- **Email**: bhua.huang@gmail.com

---

> 💡 **Tip**: This is a complete IoT system implementation that demonstrates the full process from sensor data acquisition to cloud storage. Suitable for learning IoT development, embedded system design, and network programming.
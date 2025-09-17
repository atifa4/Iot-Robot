# iot-based-robot
Description:

IoT-based robot for real-time survivor detection and monitoring in disaster/war zones focuses on human detection, data transmission, navigation, and environmental sensing.  Achieving detection accuracy, low-network communication, battery optimization, and rough terrain navigation.  Aims to enhance rescue efficiency, reduce response times.

Overview:

This project presents an IoT-enabled inspection robot designed for search and rescue operations in disaster-prone areas such as earthquakes, floods, fires, and collapsed structures.
The robot is equipped with:
Gas Sensors – Detects toxic gases like carbon monoxide, methane, and hydrogen sulfide.
Temperature Sensor – Monitors environmental temperature to identify fire hazards or trapped survivors.
MEMS Accelerometer – Assesses structural stability by detecting vibrations, tilts, and collapses.
IoT Communication – Sends real-time data to rescue teams via Wi-Fi.
GSM Module – Provides SMS-based alerts in low or no Wi-Fi network areas.
Motorized Mobility – Navigates hazardous environments remotely.
By providing real-time environmental data, the robot enhances rescue operations, minimizes risks to human responders, and improves the efficiency of disaster response.

Features:

Multi-Sensor Hazard Detection:
Gas sensor (MQ-series) for hazardous gas detection.
LM35 temperature sensor for thermal monitoring.
MEMS accelerometer for structural stability analysis.
Dual Communication System:
Wi-Fi (ESP32) for high-speed IoT data streaming.
GSM Module for SMS alerts when Wi-Fi is unavailable.
Robust Navigation:
L293D motor driver for bidirectional motor control.
DC motors for mobility over rough terrain.
Failsafe Design:
Automatic GSM-based alerts if Wi-Fi connection drops.
Backup battery support for extended operations.

Hardware Requirements:

| Component          | Model              | Purpose                         |
| ------------------ | ------------------ | ------------------------------- |
| Microcontroller    | ESP32              | Central control + IoT           |
| Gas Sensor         | MQ-8               | Detect hazardous gases          |
| Temperature Sensor | LM35               | Detect hot/cold zones           |
| MEMS Sensor        | ADXL335            | Detect tilt, vibration          |
| Motor Driver       | L293D              | Control DC motors               |
| DC Motors          | 12V, 60 RPM        | Movement                        |
| GSM Module         | SIM900             | SMS alerts in low network zones |
| GPS Module         | NEO-6M             | Location tracking               |
| Power Supply       | 12V Battery        | Power robot                     |
| Display (Optional) | 16x2 LCD           | Local display output            |

Software Requirements:


Programming Language: Embedded C
IDE: Arduino IDE
Libraries Used:
WiFi.h – ESP32 Wi-Fi connectivity
HTTPClient.h – IoT communication
Wire.h – I2C communication
SoftwareSerial.h – GSM & GPS serial interface
TinyGPS++ – GPS data parsing
Sensor-specific libraries (MQ gas sensor, accelerometer, etc.)

Data Flow:

1. Sensors collect environmental readings.
2. ESP32 processes and formats the data.
3. Data is sent to the IoT dashboard via Wi-Fi.
4. If Wi-Fi fails, GSM module sends SMS alerts with GPS nodes.

Setup & Installation:

1. Hardware Connections
   
Connect Gas Sensor analog output to ESP32 ADC pin.
Connect LM35 output to another ADC pin.
Connect MEMS accelerometer via I2C/SPI pins.
Connect L293D motor driver to ESP32 digital pins for motor control.
Connect GSM module TX/RX to ESP32 using SoftwareSerial.
Power all components via a 12V battery with voltage regulation for ESP32.

2. Software Installation

Install Arduino IDE.
Add ESP32 board support via Board Manager.
Install necessary libraries:
Arduino IDE → Tools → Manage Libraries
Upload the provided .ino code to ESP32.

3. Running the System

Power on the robot.
Connect ESP32 to a Wi-Fi network.
Monitor sensor data on the dashboard or serial monitor.
If network drops, check GSM-based SMS alerts.


Sample output:
Gas Level: 200 ppm
Temperature: 35 °C
Vibration: Detected
Status: ALERT - Possible hazard detected
Wi-Fi: Disconnected, sending GSM alert...
GSM Alert Sent: "Gas:200ppm Temp:35C Vibration:Yes Location: Lat,Long"

Example SMS Alert:

ALERT: Gas=210ppm Temp=36°C
Location: 12.9716 N, 77.5946 E
Status: Human detected.

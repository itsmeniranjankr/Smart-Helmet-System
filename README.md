# Smart-Helmet-System
www.itsmeniranjankr.github.io/Smart-Helmet-System/
An IoT-based safety ecosystem featuring an ESP32-driven alcohol interlock, buckle detection, and real-time G-force telemetry. It automatically broadcasts GPS coordinates to emergency contacts upon detecting critical impacts to ensure rapid accident response.
Secure • Protect • Respond

An IoT-based safety ecosystem featuring an ESP32-driven alcohol interlock, buckle detection, and real-time G-force telemetry. It automatically broadcasts GPS coordinates to emergency contacts upon detecting critical impacts to ensure rapid accident response.

# 🛠 Features
Alcohol Interlock (Logic Alpha): Prevents engine ignition via a relay if the MQ-3 sensor detects breath alcohol levels above the safety threshold.

Buckle Detection: Ensures the helmet is securely fastened before the engine can be started.

Impact Telemetry (Logic Beta): Monitors real-time gravitational forces using an MPU6050 IMU; triggers SOS if force exceeds 8G.

SOS Broadcast: Wraps precise GPS coordinates into an SMS packet via SIM800L and sends it to +918848825350.

Asynchronous State Machine: Non-blocking firmware architecture ensures high-frequency sensor polling even during GSM/GPS data transmission.

# 📂 System Architecture
The system integrates the following hardware components:

Microcontroller: ESP32 (Master Hub)

Alcohol Sensor: MQ-3 (Tin Dioxide sensitive layer)

IMU: MPU6050 (MEMS Accelerometer/Gyroscope)

GPS: NEO-6M (NMEA spatial data)

GSM: SIM800L (Cellular Uplink)

Output: 5V Relay & Piezo Buzzer

# 🚀 Roadmap
Phase 1 (Jan W1-2): Concept & Logic Design

Phase 2 (Jan 15): Abstract Submission

Phase 3 (Jan 29): Technical Presentation

Phase 4 (Feb): Hardware Build & Field Testing

# 💻 Tech Stack
Firmware: C++ / Arduino Framework

Interface: HTML5, CSS3 (Bento Grid UI), JavaScript

Hardware Platform: Espressif Systems

Developed at CUSAT Electronics Engineering // 2026

Lead Architect: # Niranjan K R

Team: Neeraj Subash, Naslul H, Naveen S Babu

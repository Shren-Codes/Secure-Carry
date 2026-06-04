# 🎒 SecureCarry: IoT-Based Smart Bag Security System

> An intelligent embedded security system that combines RFID authentication, GPS tracking, water intrusion monitoring, servo-based locking, and real-time Telegram notifications to protect personal belongings.

![Platform](https://img.shields.io/badge/Platform-ESP32-blue)
![Language](https://img.shields.io/badge/Language-C++-orange)
![IoT](https://img.shields.io/badge/Domain-IoT-success)
![Status](https://img.shields.io/badge/Status-Completed-brightgreen)

---

## 📌 Overview

SecureCarry is an IoT-enabled smart bag security system developed using ESP32 and embedded technologies. The system enhances bag security through multi-layer authentication, environmental monitoring, location tracking, and real-time alert notifications.

The project integrates RFID-based access control, keypad authentication, GPS tracking, water leak detection, servo-controlled locking, and Telegram Bot communication to create a portable and intelligent security solution.

---

## 🚀 Key Features

🔐 RFID-Based Authentication

⌨️ Keypad-Based Secondary Access Control

📍 Real-Time GPS Location Tracking

💧 Water Leak Detection and Monitoring

📲 Telegram Bot Alert Notifications

🔔 Security Alerts for Unauthorized Access

🔒 Servo-Controlled Smart Locking Mechanism

⏱️ Automatic Re-Locking after Inactivity

🌐 Wi-Fi Enabled IoT Connectivity

---

## 🛠 Hardware Components

- ESP32 WROOM
- MFRC522 RFID Module
- 4x3 Keypad
- NEO-6M GPS Module
- SG90 Servo Motor
- Water Leak Sensor
- Buzzer
- Li-ion Battery
- TP4056 Charging Module

---

## 💻 Software Stack

### Development Environment
- Arduino IDE

### Libraries Used
- WiFi
- WiFiClientSecure
- UniversalTelegramBot
- MFRC522
- TinyGPS++
- ESP32Servo
- SPI
- Keypad

---

## 🔄 Working Principle

1. The system starts in a secure locked state.
2. Users authenticate using an RFID card.
3. Authorized RFID access unlocks the bag.
4. Invalid RFID attempts are monitored continuously.
5. After three failed RFID attempts, keypad authentication is activated.
6. Failed authentication triggers a Telegram security alert.
7. GPS coordinates are attached to alert notifications.
8. Water sensors continuously monitor the bag interior.
9. Water intrusion instantly generates an alert message.
10. The servo lock automatically re-locks after a predefined timeout period.

---

## 📊 Results

✅ Successful RFID Authentication

✅ GPS-Based Location Tracking

✅ Water Intrusion Detection

✅ Real-Time Telegram Alerts

✅ Automatic Locking Mechanism

✅ Reliable Embedded System Operation

✅ Portable Security Solution

---

## 🔮 Future Enhancements

- Fingerprint Authentication
- Mobile Application Integration
- Battery Health Monitoring
- LoRa-Based Long Range Tracking
- Cloud Dashboard Monitoring
- AI-Based Threat Detection

---

## 🎯 Engineering Skills Demonstrated

- Embedded Systems Design
- ESP32 Development
- Internet of Things (IoT)
- RFID Authentication Systems
- GPS Integration
- Sensor Interfacing
- UART Communication
- SPI Communication
- Real-Time Monitoring
- Event-Driven Programming
- Telegram Bot API Integration
- PCB Design
- Hardware Prototyping
- Arduino Framework


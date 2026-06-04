# 🎒 SecureCarry: Intelligent Smart Bag

> An IoT-enabled smart bag security system designed to protect personal belongings through RFID authentication, GPS tracking, water leak detection, and real-time Telegram alerts.

![Platform](https://img.shields.io/badge/Platform-ESP32%20%7C%20ESP8266-blue)
![Language](https://img.shields.io/badge/Language-C%2B%2B-orange)
![IDE](https://img.shields.io/badge/IDE-Arduino-green)
![Status](https://img.shields.io/badge/Status-Completed-success)

---

## 📖 Overview

SecureCarry is a smart embedded system that enhances bag security using IoT technologies. The system prevents unauthorized access, detects water intrusion, provides location tracking, and sends instant notifications to the owner's Telegram account.

The project demonstrates the integration of embedded systems, wireless communication, sensor interfacing, and real-time alert mechanisms into a portable and practical security solution.

---

## ✨ Key Features

🔐 RFID-Based Authentication

📍 GPS Location Tracking using NEO-6M

💧 Water Leak Detection with Instant Alerts

📲 Telegram Bot Notifications

🔔 Buzzer-Based Security Alerts

⚙️ Servo Motor Controlled Smart Lock

🌐 Wi-Fi Enabled IoT Monitoring

👜 Portable Smart Bag Security Solution

---

## 🛠 Hardware Components

| Component | Quantity |
|------------|------------|
| ESP32 / ESP8266 | 1 |
| RFID RC522 Module | 1 |
| GPS NEO-6M Module | 1 |
| SG90 Servo Motor | 1 |
| Water Leak Sensors | 2 |
| Buzzer | 1 |
| Jumper Wires | Multiple |
| Power Supply | 1 |

---

## 💻 Software & Libraries

### Development Environment
- Arduino IDE

### Libraries Used
- MFRC522
- TinyGPSPlus
- ESP8266WiFi
- WiFiClientSecure
- UniversalTelegramBot
- SPI
- Servo

---

## 🏗 System Architecture

```text
RFID Authentication
        │
        ▼
   ESP32 / ESP8266
        │
 ┌──────┼──────┐
 │      │      │
 ▼      ▼      ▼
GPS   Water   Servo
      Sensor   Lock
 │
 ▼
Telegram Alerts
```

---

## 🚨 Alert System

The system sends Telegram notifications when:

- Unauthorized RFID access is detected
- Water leakage is detected inside the bag
- Security events occur

Example Alert:

```text
⚠️ ALERT: Water Leakage Detected

📍 Location:
Latitude: XX.XXXXXX
Longitude: YY.YYYYYY
```

---

## 📍 Applications

- Smart Travel Luggage
- Student Security Bags
- Document Protection Systems
- Electronic Equipment Bags
- Anti-Theft Portable Storage

---

## 📊 Project Results

✅ Successful RFID Authentication

✅ Reliable GPS Tracking

✅ Real-Time Telegram Alerts

✅ Water Leak Detection

✅ Servo-Based Smart Locking

✅ Low-Cost Embedded Security Solution

---

## 🔮 Future Enhancements

- Fingerprint Authentication
- Mobile Application Support
- Battery Health Monitoring
- LoRa-Based Tracking
- Cloud Dashboard Integration

---

## 📂 Repository Structure

```text
SecureCarry-Intelligent-Smart-Bag
│
├── Code
│   └── SecureCarryCODE.ino
│
├── Documentation
│   └── SecureCarry_An_Intelligent_Bag.pdf
│
├── Images
│   ├── Hardware_Setup.jpg
│   ├── Circuit_Diagram.png
│   ├── PCB_Layout.png
│   └── PCB_3D_View.png
│
├── Demo
│   └── Working_DEMO.mp4
│
└── README.md
```

---

## 👩‍💻 Authors

**Shrenica Chawda A**  
B.Tech Electronics and Communication Engineering  
VIT Chennai

**Lakshita M S**

**Akshaya R G**

---

## 📜 License

This project is developed for academic and educational purposes.

⭐ If you found this project useful, consider giving the repository a star.

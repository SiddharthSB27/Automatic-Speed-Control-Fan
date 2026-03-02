# 🌡 Automatic Speed Control Fan using Arduino & DHT11

## 📌 Overview
This project implements an automatic temperature-based fan speed control system using Arduino and DHT11 sensor. The system monitors ambient temperature and adjusts the fan speed automatically using PWM.

---

## 🛠 Components Used
- Arduino UNO
- DHT11 Temperature Sensor
- DC Fan
- Transistor / MOSFET
- External Power Supply
- Resistors & Jumper Wires

---

## 🔌 Pin Configuration
| Component | Arduino Pin |
|-----------|-------------|
| DHT11 Data | D2 |
| Fan Control | D9 (PWM) |
| VCC | 5V |
| GND | GND |

---

## ⚙ Working
1. DHT11 reads temperature.
2. Arduino processes temperature data.
3. Fan speed increases as temperature increases.
4. Uses PWM for smooth speed control.

---

## 🚀 Future Improvements
- IoT monitoring using ESP32
- Data logging
- LCD display integration
- Mobile app control

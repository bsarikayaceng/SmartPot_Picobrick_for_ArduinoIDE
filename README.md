# 🌱 PicoBricks Smart Pot

The **PicoBricks Smart Pot** is an educational IoT project that automates plant watering using the ESP32-based PicoBricks platform. It is designed to monitor soil moisture and control watering via a relay module, with manual override and threshold configuration using an IR remote.

---

## 🧠 Project Description

This project helps you build a smart plant pot that waters itself based on soil moisture. Using the OLED screen, users can view real-time soil values. The IR remote is used to set a soil moisture threshold for automatic watering.

When soil moisture drops below the threshold, the relay activates a pump or valve. A buzzer also alerts the user, providing both visual and auditory feedback.

---

## 🔧 Features

- 📡 IR Remote to set moisture threshold
- 💧 Automatic watering using relay
- 📺 OLED display for real-time feedback
- 🔔 Buzzer for audio notification
- 🌱 Soil moisture monitoring with analog sensor

---

## 🧰 Hardware Requirements

- ✅ PicoBricks (ESP32)
- ✅ Soil Moisture Sensor (Analog)
- ✅ IR Receiver
- ✅ OLED Display (I2C)
- ✅ Relay Module
- ✅ Buzzer Module

---

## 📂 File Structure

```
PicoBricks-SmartPot/
│
├── arduino/
│   └── SmartPot_NEW.ino
│
├── README.md
└── LICENSE
```

---

## 📜 License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

## 📸 Preview

> You can include wiring diagrams or real project photos here.

---

## ✨ Author

Developed using [PicoBricks](https://www.picobricks.com) educational platform.

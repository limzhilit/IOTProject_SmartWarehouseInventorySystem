

 🏭 Intelligent Warehouse Automation System

 📌 Overview

This project presents an IoT-based warehouse automation system designed to improve inventory tracking, reduce manual labour, and enable real-time monitoring of warehouse operations.

By integrating RFID technology, embedded systems, and web-based control, the system provides a scalable solution for modern warehouse management.

---

 🚀 Features

* 📦 RFID Inventory Tracking
  Automatically identifies and logs incoming and outgoing goods.

* 🌡️ Temperature Monitoring
  Continuously monitors storage conditions to prevent spoilage.

* 🤖 Robotic Cart System
  Uses DC motors to transport goods within the warehouse.

* 🌐 Real-Time Web Dashboard
  Live updates using WebSocket communication.

* ⚡ Event-Driven Architecture
  Non-blocking system for efficient and responsive performance.

* 🛠️ Error Handling
  Designed for system stability and reliability.

---

 🏗️ System Architecture
```
[RFID]        [Temperature Sensor]
     \             /
      \           /
        ---> [ESP32] ---> WebSocket ---> [Web Interface]
                      |
                      ↓
                 [Motor Control]
                      ↓
                 [Robotic Cart]
```
---

 🧠 Technologies Used

 Hardware

* ESP32 Microcontroller
* RFID Module
* Temperature Sensor
* DC Motors & Motor Driver
* Servo Motor (optional)

 Software

* C/C++ (Arduino IDE)
* HTML, CSS, JavaScript
* WebSocket Protocol

 Future Expansion

* Python
* OpenCV
* ArUco Marker Detection

---

 💻 Web Interface

The system includes a responsive web dashboard that allows users to:

* View real-time inventory data
* Monitor temperature conditions
* Control robot movement
* Track system status

---

 🔧 Setup Instructions

 1. Clone the Repository

bash
git clone https://github.com/your-username/warehouse-iot-system.git
cd warehouse-iot-system


 2. ESP32 Setup

* Open the `.ino` file in Arduino IDE
* Install required libraries:

  * WiFi
  * WebSockets
  * RFID library
* Connect ESP32 and upload code

 3. Configure WiFi

Update your WiFi credentials in the code:

cpp
const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";


 4. Run Web Interface

* Open `index.html` in your browser
* Or host using Live Server (VS Code)

---

 ⚙️ How It Works

1. RFID scans items entering/leaving warehouse
2. ESP32 processes data and reads sensor values
3. Data is sent via WebSocket to the web dashboard
4. User can monitor and control system in real time
5. Motors drive robotic cart based on commands

---

 🌱 Sustainability Impact

This project contributes to:

* ♻️ Reduced waste through accurate inventory tracking
* ⚡ Energy-efficient operation using event-driven design
* 🏭 Improved industrial efficiency (UN SDG 9)
* 📉 Reduced overproduction and spoilage (UN SDG 12)

---

 🔍 Challenges Faced

* WebSocket synchronization issues
* RFID reading inconsistencies
* Motor control stability

 Solutions

* Implemented event-driven communication
* Added filtering for RFID signals
* Optimized PWM motor control

---

 🔮 Future Improvements

* 📷 Computer Vision with OpenCV
* 📍 Indoor positioning using ArUco markers
* 🤖 Fully autonomous navigation
* ☁️ Cloud integration for analytics

---

 📂 Project Structure

```
/src
  ├── esp32_code.ino
/web
  ├── index.html
  ├── style.css
  ├── script.js
/docs
  ├── report.pdf
README.md
```

---

 👨‍💻 Author

Zhi Lit Lim
B.Eng. (Hons) Software & Electronic Engineering
Atlantic Technological University

---

 📄 License

This project is for academic purposes. You may use or adapt it with proper credit.

---

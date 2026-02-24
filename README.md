# Caffeine Content – Control of Your Coffee’s Caffeine Level

<p align="center">
  <img src="https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=cplusplus&logoColor=white">
  <img src="https://img.shields.io/badge/Raspberry%20Pi%20Pico-C51A4A?style=for-the-badge&logo=raspberrypi&logoColor=white">
  <img src="https://img.shields.io/badge/Arduino_IDE-00979D?style=for-the-badge&logo=arduino&logoColor=white">
</p>

## 📌 Project Overview
This project consists of a system that models caffeine extraction curve, determines the correct start and stop times for the coffe machine, and actuates the machine to produce a coffee with the desired caffeine level. The following diagram shows the steps

![Overview](images/Overview.png)
---
## 💻 Software Requirements
- Arduino IDE 2.x + Raspberry Pi Pico Board 4.5.2 or later

## 🧩 List of Components
- Raspberry Pi Pico + Firmware (Arduino IDE C++)
- Solenoid/electromagnet push - Activate the coffe button in the coffe machine
- 1x LED + 1.5kΩ resistor - Indicate that selected caffeine is not valid (less than minimum possible or more than available caffeine)
- 2x PNP transistors with Ic >= 200 mA (e.g. BC327) + 2x 680Ω resistors to drive the two displays 
- 1x Logic level NMOS or NPN transistor capable of driving the solenoid (e.g. IRLZ44N)
- 2x 7-segments display + 7x 51Ω resistors - Display caffeine level
- 3x buttons - Select desired caffeine with (+), (-) and (OK/Reset) buttons
  
## 🔌 Circuit Assembly

![Schematic](images/schematic.png)

### Notes:
- The displays are multiplexed, with the segments connected in paralell and the respective anodes alternating activating each display
- The buttons are connected to the 3V3 pin, with pull-down resistors setup internally in the code (Buttons.cpp/h)

<p align="center">
  <img src="images/sensor_breadboard.jpg" >
  <br>
  <em>a) Sensor detail &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;  b) Breadboard assembly</em>
</p>
<p align="center">
  <img src="images/sensor_array.jpg" >
  <br>
  <em>Sensors connected in matrix form</em>
</p>

---

## 🚀 How to Use





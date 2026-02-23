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

## 🧩 Components and Function
- Raspberry Pi Pico + Firmware (Arduino IDE C++)
- Solenoid/electromagnet push - Activate the coffe machine button
- 1x LED - Indicate that current selected caffeine is valid (minimum possible < X < available caffeine)
- 3x PNP transistors supporting colector current at least 400 mA (e.g. BC327) - Drive the 2 displays and the solenoid
- 2x 270Ω + 2x 120Ω resistors - Limit transistor base current and LED current
- 2x 7-segments display - Display caffeine level
- 7x 51Ω resistors - Limit segment current
- 3x buttons - Select desired caffeine with (+), (-) and (OK/Reset) buttons
  
## 🔌 Circuit Assembly

![Schematic](images/schematic.png)

### Notes:
- The displays are multiplexed, with the segments connected in paralell and the respective anodes are alternating activad by each transistor
- The buttons are connected to the 3V3 pin, with pull-down resistors setup internally in the code
- The transistor driving the solenoid needs more base current and has a smaller value resistor (120Ω) than the display transistors (270Ω)


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





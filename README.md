# Caffeine Content – Control of Your Coffee’s Caffeine Level

<p align="center">
  <img src="https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=cplusplus&logoColor=white">
  <img src="https://img.shields.io/badge/Raspberry%20Pi%20Pico-C51A4A?style=for-the-badge&logo=raspberrypi&logoColor=white">
  <img src="https://img.shields.io/badge/Arduino_IDE-00979D?style=for-the-badge&logo=arduino&logoColor=white">
</p>

## 📌 Project Overview
This project implements a system that lets you control the caffeine level of your coffee. While caffeine can help maintain focus, the amount in a typical espresso is often excessive and may lead to a mental crash some time later, especially after lunch or eating. This system gives you control by allowing smaller, more frequent doses instead of large, infrequent ones.

The diagram below illustrates the process:
![Overview](images/Overview.png)

## ☕ How It Works
The system models the caffeine extraction curve, calculates the volume that the coffe must have to have your desired caffeine level, and actuates the machine to produce a coffe with that volume.
The diagram below shows an example of how the amount of extracted caffeine grows as more coffee comes out.

![Caffeine curve plot](images/caffeine_plot.png)

Imagine you want a coffee containing 20 mg of caffeine. The capsule has already been used previously, and 30 mg have been extracted so far — this is state A. To obtain your desired 20 mg, the system must extract a total of 50 mg (30 + 20) from the capsule — this is state B.
The difference on the y‑axis between **state A** and **state B** represents the additional caffeine that will be extracted: 20 mg.
The difference on the x‑axis represents the volume of coffee the machine must produce.
The system then translates the desired volume to the **flow time** - duration in which coffe is flowing out of the machine.

## 🧩 List of Components
- Raspberry Pi Pico running the firmware
- Solenoid/electromagnetic actuator — presses the coffee machine button
- 1× LED + 1.5 kΩ resistor — indicates invalid caffeine selection (below minimum or above available caffeine)
- 2× PNP transistors with Ic ≥ 200 mA (e.g., BC327) + 2× 680 Ω resistors — drive the two 7‑segment displays
- 1× logic‑level NMOS or NPN transistor capable of driving the solenoid (e.g., IRLZ44N)
- 2x 7-segments display + 7x 51Ω resistors - display caffeine level
- 3× buttons — increase (+), decrease (–), and confirm/reset (OK)
  
## 🔌 Circuit Assembly

![Schematic](images/schematic.png)

### Notes:
- The displays are multiplexed: all segment pins are connected in parallel, and the anodes are alternated activated by the driving transistors
- The buttons are connected to the 3.3 V rail, with internal pull‑down resistors configured in software (Buttons.cpp/h).


## 📦 Software Requirements
- Arduino IDE 2.x
- Raspberry Pi Pico Board (from earlephilhower) version 4.5.2 or later 

## ⚙️ Configurable Parameters

Before using the system, you need to configure a few parameters.  
The capsule is defined by **two parameters** that shape the exponential extraction model, and the coffee machine is defined by **three parameters** that determine the actuator timing.  
All parameters can be adjusted in `MakeCoffee.h`.

### Capsule Parameters

#### `CAFFEINE_30` [mg]  
Amount of caffeine extracted from a *new* capsule when producing **30 mL** of coffee (the standard espresso volume).  
In a 30 mL coffee, a mild capsule usually gives **40–60 mg**, while a strong or “intense” capsule typically gives **70–100 mg**. Values vary between brands — mild lungo-style capsules are often 100% Arabica, while intense or ristretto capsules usually include Robusta, which naturally contains more caffeine.

#### `LAMBDA` [mL]  
Controls how slow the extraction curve decays. It works like a *time constant*, but applied to **volume** instead of time.  
A value around **13.6 mL** means that a 15 mL extraction from a new capsule yields roughly **75% of `CAFFEINE_30`**, which is a good approximation for most cases. Larger or heavier capsules tend to have a higher λ.

### Machine Parameters

#### `DEAD_TIME` [s]  
Time between pressing the button and the moment coffee actually starts flowing.

#### `OVER_TIME` [s]  
Time the machine continues to run *after* the button is pressed again to stop.  
(If your machine works with press‑to‑start and release‑to‑stop, you may need to adjust the code.)  
To measure both values, record a coffee being made and check the timing in the video.

#### `FLOW_RATE` [mL/s]  
How many millilitres of coffee the machine produces per second.  
You can measure this using the same video: weigh the coffee produced (1 mL ≈ 1 g) and divide by the extraction time.



## ☕ How to Use

The system lets you choose how much caffeine you have in your coffe.  
You interact with it using the three buttons and the 7‑segment displays.

Below is a demonstration:  
![Schematic](images/system_demo.gif)

### Selecting the desired caffeine
Use the **-** and **+** buttons (left and middle buttons) to choose the target caffeine value.

- A **short press** increases or decreases by 1 mg  
- A **long press** scrolls quickly through the values

If the selected value is **invalid**, the LED turns on.  
A value can be invalid because:
- it is **too low** for the machine to switch on and off fast enough, or  
- it is **too high** for the current capsule (the capsule no longer contains that much caffeine)

### Confirming the selection
When the desired value is set, press **OK** (right button).

Pressing OK immediately **activates the machine**.

### During extraction
- While the machine is in **dead time** (between activation and coffee starts flowing), the displays show the animation **“COFFEE”** scrolling across the two 7‑segment displays.
- Once coffee starts flowing, the system begins a **count‑up** from 0 mg to the selected caffeine value, estimating in real time how much caffeine is already in the cup based on the extraction curve.

### Resetting when changing capsule
When you insert a **new capsule**, press and hold **OK** to reset the system. The displays will show "--" confirming the reset.



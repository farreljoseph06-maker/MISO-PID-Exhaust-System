# Multi-Input Single-Output (MISO) Dual-Loop PID Controller (my first project :D)

This repository contains the hardware codebase and software simulation for an Industrial-grade Exhaust System using Override Control Logic.

## Key Features
* **Dual-Loop PID Architecture:** Independent PID computation for air pollutant (MQ-135) and ambient temperature (DHT22).
* **Max Selector Override Control:** Implements high-selector logic `max(Output_Asap, Output_Suhu)` to prioritize critical environmental parameters, mimicking industrial safety failsafes.
* **Non-Blocking Timer Execution:** Utilizing `millis()` for CPU-efficient sensor reading and actuator mapping without interrupting the control loop.
* **Anti-Windup Protection:** Engineered with reverse-acting mode and hard-clamped saturation to prevent integral windup.

##  Hardware Stack
* Microcontroller: Arduino Uno
* Sensors: MQ-135 (Gas/Pollutant), DHT22 (Temperature & Humidity)
* Actuator: DC Motor (Exhaust Fan) driven by D4184 MOSFET
* Display: 16x2 I2C LCD & Visual LED Indicator

## Software & Simulation
* **C++ (Arduino IDE):** Discrete digital PID implementation.
* **MATLAB Simulink:** Mathematical validation and step-response testing for MISO architecture. *(Simulink block diagrams and scope graphs are included in this repository).*

## 📸 System Demonstration
*<img width="1599" height="899" alt="Alat" src="https://github.com/user-attachments/assets/1d6c5c90-d8d4-4bbb-90b9-3f6d459107fc" />
*<img width="1920" height="1080" alt="Scope Simulink" src="https://github.com/user-attachments/assets/614b53c4-48b0-4d4a-9a58-7926a4197ebd" />

# Smart Digital OLED Clock using ATtiny85

## Overview

This project is a Smart, Power-Efficient Digital OLED Clock built using the ATtiny85 microcontroller and a DS3231 RTC module. The clock is designed to maximize battery life while offering essential clock features like time display, manual time setting, and battery percentage monitoring.


## Key Features
- #### Power Efficiency:
	- Utilizes a latch circuit and clap switch mechanism.
	- On clap detection, the latch circuit powers ON the microcontroller and OLED for 5 seconds to display the time.
	- After 5 seconds, the microcontroller sends a LOW signal to turn OFF the latch circuit, entering ultra-low power mode.
	- Idle current consumption: ~0.22 mA, allowing long runtimes with a LiPo battery.
- #### Minimal Memory Usage:
	- Carefully optimized Arduino program with lightweight libraries.
	- Custom fonts used for compact memory footprint and better display aesthetics.
- #### Functionalities:
	- Time Display
	- Manual Time Setting
	- Battery Percentage Monitoring


## Components Used
- Microcontroller: ATtiny85
- RTC: DS3231 IC
- Display: OLED (SSD1306 128x64)
- Resistance: 220 ohm, 1K, 10K, 100K, 10M
- Condenser Mic
- Capacitors: 10uF, 0.1uF
- Transistors: BC847
- P-Mosfet: AO3401
- LiPo Battery 
- Lipo Battery Charging Module: TP4050 module
- Switches

### Schematic Diagram
![App Screenshot](https://github.com/Prasanjit137/Attiny85-Smart-OLED-Clock/blob/0da069395b2fd593bac9f89493a6adcefdb5dd78/Schematic_SMD-Clock-latest.png)


## Repository Contents
- Program Code (.ino file)
- Required Libraries (included)
- Circuit Diagram (schematic image / PDF)
- Gerber File (for PCB fabrication)


## How It Works
- #### Idle Mode:
	The system remains OFF, drawing negligible current.
- #### Clap Detection:
	A clap sound triggers the clap switch, activating the latch circuit.
- #### Time Display:
	The latch circuit powers ON the ATtiny85 and OLED display.
	The current time is shown for 5 seconds.
- #### Auto Power OFF:
	After 5 seconds, the ATtiny85 sends a LOW signal to the latch circuit, turning itself OFF to save battery.

### Final Build
![App Screenshot](https://github.com/Prasanjit137/Attiny85-Smart-OLED-Clock/blob/0da069395b2fd593bac9f89493a6adcefdb5dd78/Final.JPG)

## Power Efficiency
- Idle Current Draw: ~0.22 mA
- Battery Life:  With a small LiPo battery, the clock can operate for a very long time before requiring a recharge (Approx 25-30 days on a single charge using 700 mAh battery).


## Manual Time Setting

Built-in functionality allows users to manually set the time if needed, ensuring accurate clock operation without needing external programming.

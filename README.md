# DriveWire
## Wireless Mini EV Telemetry Platform

DriveWire V1 is a scaled down electric‑vehicle (EV) platform built to explore wireless telemetry, embedded firmware, and mechanical design. The goal is to create a self‑contained test bed that can collect vehicle data and transmit it wirelessly to a ground station for analysis in real time over WiFi. This repository contains both the hardware notes and the firmware for an ESP32‑S3 microcontroller that powers the vehicle’s on‑board electronics.

The project started from the need to rapidly prototype and test EV subsystems. A small chassis makes it easy to experiment with sensors, motor controllers and wireless communication without the cost and complexity of a full‑size vehicle. DriveWire V1 demonstrates how to integrate mechanical and electrical components, write embedded C++ firmware and configure a PlatformIO project.

## V1 Architecture
<p align="center">
  <img src="Images/HighLevelDriveWire.png" width="500">
</p>

High-level architecture of the DriveWire V1 platform.

## Features

- Wireless telemetry: The ESP32‑S3 microcontroller streams sensor data over Wi‑Fi or serial, allowing live monitoring of speed, battery voltage and other parameters.
- Mini EV chassis: Two DC motors mounted to the chassis provide propulsion. A rear hammer caster and code wheels for velocimetry complete the drivetrain assembly.
- Modular firmware: The code is structured as a PlatformIO project with separate src and include directories. It uses the Arduino framework for rapid development.
- Test scaffolding: A test directory is included for future unit tests using PlatformIO’s built‑in testing framework.
- Cross‑platform build: The platformio.ini file targets the esp32‑s3‑devkitc‑1 board with configurable upload and monitor ports.

## Hardware (V1)

| Component | Purpose |
|------------|------------|
| ESP32-S3 DevKitC-1 N8R8 | Main microcontroller |
| TB6612FNG Motor Driver | Controls left and right motors |
| 2x DC Gear Motors | Vehicle propulsion |
| 4x AA Battery Pack | Motor power source |
| USB Power Bank | ESP32 power source |
| Chassis Kit | Mechanical platform |
| Misc Sensors | Current, voltage, distance sensing |



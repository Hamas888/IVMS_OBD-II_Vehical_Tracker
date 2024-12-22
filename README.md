# IVMS OBD-II Vehicle Tracker

![Project Image](https://drive.google.com/uc?export=view&id=15xmFmcu_6ijC77EyhqKIXPR4fQtI_uQ0) 

## Table of Contents

1. **[Overview](#overview)**  
   - [Key Highlights](#key-highlights)  
2. **[Features](#features)**  
3. **[Technical Specifications](#technical-specifications)**  
   - [ESP32-S3 Microcontroller](#esp32-s3-microcontroller)  
   - [SIM7600-H Module](#sim7600-h-module)  
   - [STN OBD-II Interface](#stn-obd-ii-interface)  
   - [BLE Proximity Detection](#ble-proximity-detection)  
   - [ESP-NOW Communication](#esp-now-communication)  
4. **[Key Functions](#key-functions)**  
   - [Vehicle Tracking and Status Monitoring](#vehicle-tracking-and-status-monitoring)  
   - [Smart Security Features](#smart-security-features)  
   - [Cloud Integration and OTA Updates](#cloud-integration-and-ota-updates)  
5. **[Vehicle Parameters Monitored](#vehicle-parameters-monitored)**  
   - [Basic Vehicle Data](#basic-vehicle-data)  
   - [Engine Performance](#engine-performance)  
   - [Sensor & Diagnostic Data](#sensor--diagnostic-data)  
   - [Battery and System Health](#battery-and-system-health)  
6. **[Hardware Design](#hardware-design)**  
   - [Key Components](#key-components)  
7. **[Firmware](#firmware)**  
   - [Data Collection and Logging](#data-collection-and-logging)  
   - [Driving Behavior Analysis](#driving-behavior-analysis)  
   - [Security Management](#security-management)  
   - [OTA Firmware Updates](#ota-firmware-updates)  
8. **[Future Enhancements](#future-enhancements)**  
   - [Enhanced Data Analytics](#enhanced-data-analytics)  
   - [Enhanced Security Features](#enhanced-security-features)  
   - [Support for CAN Bus Integration](#support-for-can-bus-integration)  
   - [Geofencing Capabilities](#geofencing-capabilities)  
9. **[Licensing](#licensing)**  
   - [Non-Commercial Use](#non-commercial-use)  
   - [Commercial Use](#commercial-use)  

## Overview
The IVMS OBD-II is a powerful, plug-and-play GPS vehicle tracker, designed for comprehensive real-time vehicle monitoring, tracking, and management. It connects directly to the OBD-II port, enabling access to various vehicle parameters, driver behavior insights, and remote management capabilities. This tracker is ideal for personal vehicle security and fleet management applications, offering seamless integration with a mobile app and cloud portal.

### Key Highlights
- **Smart Access Control**: Requires pairing with the owner’s mobile app and proximity detection to enable vehicle functionality.
- **Real-Time Vehicle Insights**: Tracks location, car status, and driving behavior, reporting data through a secure cloud portal.
- **Remote Fleet Management**: Fleet managers can monitor vehicle location, driving patterns, and other parameters via the portal over LTE.
- **Automatic Firmware Updates**: The device receives OTA firmware updates via 4G LTE, ensuring the latest features and security patches.

## Features

- **Multi-Level Security**: The device uses Bluetooth Low Energy (BLE) proximity detection to lock/unlock vehicle functions, ensuring only authorized users can start the vehicle. An alternative BLE-based security tag can be used as an emergency access method.
- **App Integration**: Available for Android and iOS, the companion app provides real-time vehicle status, location tracking, and driving alerts.
- **Remote Immobilization**: Fleet managers can disable the vehicle remotely via the cloud portal, performing reset or special functions for enhanced security.
- **Customizable Alerts**: This feature sends alerts on driving behaviors, such as harsh acceleration or braking, along with real-time notifications to the driver and fleet manager.

## Technical Specifications

- **ESP32-S3 Microcontroller**: The main controller for processing and communication.
- **SIM7600-H Module**: Provides high-speed 4G LTE connectivity for remote data logging, location tracking, and OTA updates.
- **STN OBD-II Interface**: Reads comprehensive vehicle diagnostics and parameters directly from the OBD-II port.
- **BLE Proximity Detection**: The device calculates the distance from the paired mobile app to secure vehicle access.
- **ESP-NOW Communication**: Wireless protocol used for secure, low-latency communication with in-vehicle relays and sensors, such as a tamper sensor for license plate detection.

## Key Functions

1. **Vehicle Tracking and Status Monitoring**
   - Provides real-time GPS location updates.
   - Monitors car parameters like speed, RPM, fuel level, and battery voltage.
   - Tracks driver behavior, such as harsh acceleration, braking, and cornering.
   - Uploads data to the cloud portal for fleet management and review.

2. **Smart Security Features**
   - BLE-based proximity control restricts vehicle use when the owner's phone or security tag is not nearby.
   - Detects unauthorized actions, such as license plate tampering, via ESP-NOW communication with in-vehicle sensors.
   - Remote immobilization allows fleet managers to disable the vehicle via the portal, adding an extra layer of security.

3. **Cloud Integration and OTA Updates**
   - The device supports automatic over-the-air firmware updates through the cloud portal over 4G LTE.
   - Portal administrators can force firmware updates for specific devices or groups based on IMEI numbers.
   - Offers remote reset and configuration management to keep all devices up-to-date.

## Vehicle Parameters Monitored
The DB3 tracker provides detailed information on a range of vehicle parameters:

- **Basic Vehicle Data**: Odometer, Speed, RPM, Fuel Level
- **Engine Performance**: Engine Load, Throttle Position, Timing Advance
- **Sensor & Diagnostic Data**: Coolant and Intake Air Temperatures, MAF (Mass Air Flow) Sensor Data, Intake Manifold Pressure, Fuel Pressure, Oxygen Sensor Data
- **Battery and System Health**: Battery Voltage, System Errors, DTC (Diagnostic Trouble Codes)

## Hardware Design
The hardware is designed for durability and reliability, using high-quality components suitable for automotive and industrial environments. The device’s compact design fits seamlessly into standard OBD-II ports, supporting robust thermal management and EMC protections.

### Key Components
- **ESP32-S3**: Handles processing, BLE, and ESP-NOW communication.
- **SIM7600-H**: Manages 4G LTE connectivity for remote data logging and OTA updates.
- **STN OBD-II Chip**: Reads vehicle diagnostics and relays data to the main controller.
- **Wireless Relays & Sensors**: Communicate with the ESP32-S3 via ESP-NOW, allowing the device to detect and respond to tampering or security breaches.

## Firmware
The firmware is optimized to provide the following key functionalities:

- **Data Collection and Logging**: Collects vehicle data from OBD-II, logs events, and uploads them to the cloud.
- **Driving Behavior Analysis**: Monitors and reports on driving patterns, generating alerts for fleet managers and drivers.
- **Security Management**: Enforces BLE proximity-based access control and triggers alerts for unauthorized access or tampering.
- **OTA Firmware Updates**: Enables automatic firmware updates over LTE for device improvements and security patches.

## Future Enhancements
- **Enhanced Data Analytics**: Introducing advanced cloud-based monitoring and analytics for deeper insights into vehicle performance and driver behavior.
- **Enhanced Security Features**: Adding biometric or multi-factor authentication to further secure vehicle access.
- **Support for CAN Bus Integration**: Extending support to include CAN bus data, providing a broader set of diagnostic information from the vehicle.
- **Geofencing Capabilities**: Enabling configurable geofencing to alert fleet managers if vehicles leave or enter specified zones.


## Licensing
The header files provided in this repository are available for non-commercial use under the following conditions:

- **Non-Commercial Use**: You may use and distribute the header files for non-commercial purposes.
- **Restricted Modification**: You may not reverse-engineer, modify, or distribute any part of the implementation not explicitly provided in this repository.

### Commercial Use
To obtain full access to implementation files (.cpp) or to use this project in a commercial product, please contact [hamasaeed@gmail.com](mailto:hamasaeed@gmail.com) for a commercial license.

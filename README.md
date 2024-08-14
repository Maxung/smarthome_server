# Simple Smart Home Server

## Overview
This project is a C++23 implementation that manages Bluetooth Low Energy (BLE) devices, storing their data in an SQLite database and providing access to the data via a RESTful API. It utilizes modern C++ features such as unique and shared pointers, new range algorithms, and optionals.

## Features
- **Asynchronous Device Management:** The Bluetooth manager asynchronously handles connections to BLE devices, subscribing to characteristics and receiving data through notifications.
- **Data Storage:** Received data is converted and stored in an SQLite database, organized by device, service, characteristic, timestamp, and value.
- **REST API:** A lightweight HTTP server built with the CrowCpp framework provides a REST API, allowing retrieval of the latest or all measurements in JSON format.
- **Easy Configuration:** Device configurations are handled through a simple YAML file, enabling seamless connection to multiple BLE devices.
- **Companion App:** A companion app, implemented with Flutter, can visualize the data in charts and show real-time updates.

## Getting Started
To use this codebase, ensure you have a C++23 compatible compiler and the necessary dependencies installed. As everything is downloaded and configured on compile time, only SQLite3 is needed. The YAML configuration file allows you to specify the BLE devices to connect to. Once set up, the server will automatically manage these devices, store data, and make it accessible over the local network. 

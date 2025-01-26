# Cargo Damage Detection System

## Project Overview
This project is designed to detect and analyze potential causes of damage to packages during transportation by cargo firms. By monitoring environmental factors such as **G-force, pressure, moisture, and temperature**, the system determines whether the package has been mishandled or subjected to conditions likely to cause damage.

The system records data when a **threshold G-force** is exceeded, indicating a possible impact or mishandling event. The collected data is stored on an **SD card** for later analysis to identify the environmental conditions leading to the damage.

## Features
- **G-Force Detection**: Continuously monitors acceleration and triggers data logging when a predefined threshold is exceeded.
- **Environmental Monitoring**: Records pressure, moisture, and temperature readings to identify potential environmental causes of damage.
- **Data Logging**: Saves event data to an SD card for post-incident analysis.
- **Threshold-Based Trigger**: Optimized for efficient power consumption by only logging data during significant events.

## Components
### Hardware
- G-force sensor: **MPU6050**
- Environmental sensors: **BME280** for pressure, temperature, and humidity
- Real-Time Clock: **DS1302 RTC**
- Microcontroller: **ESP32S3**
- SD card module for data storage
- Power source (battery or USB)

### Software
- Code written in **C++** for microcontroller programming
- **Arduino IDE** or **PlatformIO** used for development and debugging
- Libraries for sensor interfacing, real-time clock, and SD card handling

## How It Works
1. The system initializes all sensors and prepares for data collection.
2. **G-force monitoring**: The **MPU6050** continuously checks for motion events exceeding the threshold.
3. When motion is detected, environmental data is collected from the **BME280** (temperature, pressure, and humidity) and the current timestamp from the **DS1302 RTC**.
4. Data is formatted into a message and stored on the **SD card** in CSV format for later analysis.
5. The system then enters a low-power sleep mode until the next event.

## Installation and Setup
### Hardware Assembly
1. Connect the **MPU6050** to the ESP32S3 via I2C (SDA and SCL).
2. Connect the **BME280** to the ESP32S3 via I2C.
3. Connect the **DS1302 RTC** to the ESP32S3 using the RTC data, clock, and reset pins.
4. Attach the SD card module to the ESP32S3.
5. Ensure proper power supply connections to all components as per the provided PCB schematic.

### Software Setup
1. Clone this repository to your local machine:
   ```bash
   git clone https://github.com/onurkarakoc79/Cargo-Damage-Detection-System
   ```
2. Open the project in the Arduino IDE or PlatformIO.
3. Install the required libraries:
   - `Wire.h` for I2C communication
   - `Adafruit_Sensor.h` for BME280
   - Custom libraries for **MPU6050**, **DS1302 RTC**, and **SDCard**
4. Upload the code to your ESP32S3 microcontroller.
5. Insert an SD card into the SD card module.

## Data Format
The system logs data in **CSV format** for easy analysis. Each log entry includes the following fields:
- **Timestamp**: Current time recorded by the RTC
- **Temperature (°C)**: Ambient temperature
- **Pressure (hPa)**: Barometric pressure
- **Altitude (m)**: Altitude derived from pressure
- **Humidity (%)**: Relative humidity
- **Acceleration (g)**: X, Y, and Z-axis acceleration values

### Example Data Format
| Timestamp         | Temperature (°C) | Pressure (hPa) | Altitude (m) | Humidity (%) | AccX (g) | AccY (g) | AccZ (g) |
|--------------------|-------------------|----------------|--------------|--------------|----------|----------|----------|
| 2025-01-25 12:00  | 22.5              | 1013.25        | 50.2         | 45           | 0.98     | -0.12    | -1.19    |
| 2025-01-25 12:01  | 23.0              | 1013.30        | 50.3         | 46           | 1.12     | 0.08     | -1.22    |

## Applications
- Package monitoring for logistics and cargo firms
- Root cause analysis for damaged goods
- Real-time quality control in shipping environments

## Contribution
Contributions are welcome! Feel free to open an issue or submit a pull request to improve this project.

## License
This project is licensed under the MIT License. See the `LICENSE` file for more details.

## Contact

If you have any questions, feedback, or inquiries related to the project, feel free to reach out to me:

My personal mail: onurkarakoc79@gmail.com

       
I appreciate your interest in my projects and look forward to collaborating with you in the fascinating field of scientific exploration and discovery.




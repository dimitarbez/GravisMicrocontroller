# Gravis Project Microcontroller Firmware

This repository contains the PlatformIO project for the microcontroller firmware of the Gravis project.

## Overview

Gravis is a pioneering modular robotic platform engineered with versatility at its core. Purpose-built for education, research, and development, Gravis bridges the gap between theoretical understanding and hands-on experimentation. The firmware, designed for Arduino, communicates with a Raspberry Pi via serial, ensuring intuitive and detailed control, and facilitating seamless interactions between an array of hardware components. From advanced sensor integrations to nuanced actuations, Gravis's firmware guarantees real-time responses, making it an indispensable tool for learners, researchers, and developers alike. Dive into the future of robotics with Gravis, where innovation meets hands-on application.

## Installation and Setup

1. Install [PlatformIO](https://platformio.org/).
2. Clone this repository: `git clone [URL of this repo]`.
3. Navigate to the project folder: `cd [project-folder-name]`.
4. Install the required libraries and dependencies using PlatformIO.
5. Build and upload the firmware to your target board.

## Configuration

The PlatformIO configuration for this project can be found in the `platformio.ini` file. This file specifies the target board, framework, monitor speed, and library dependencies.

Current configuration:

```ini
[env:ardino-mega]
platform = atmelavr
board = megaatmega2560
framework = arduino
monitor_speed = 115200
lib_deps = 
	adafruit/DHT sensor library@^1.4.4
	adafruit/Adafruit Unified Sensor@^1.1.7
	adafruit/Adafruit SSD1306@^2.5.7
	CustomServo
	L298N_ArduinoMega
	adafruit/Adafruit NeoPixel@^1.11.0
	arduino-libraries/Servo@^1.2.1
```

## Communication to the Arduino

Commands to control the various components of the Gravis project should be sent via the serial port to the Arduino Mega microcontroller. This communication is typically facilitated by a Raspberry Pi, which interfaces with the Arduino Mega over the serial connection.

When sending commands, ensure that the Raspberry Pi and the Arduino Mega have the correct baud rate settings to guarantee accurate data transmission. As per the current configuration, the communication baud rate is set to `115200`.

### Sending Commands

1. Establish a serial connection between the Raspberry Pi and the Arduino Mega.
2. Ensure that both devices are set to the same baud rate.
3. Send the desired command strings from the Raspberry Pi to the Arduino Mega over the serial connection.

Note: Be mindful of the serial buffer limits. If sending multiple commands in rapid succession, consider introducing a short delay between command transmissions.

## Commands

### Motor Commands

- **Front Motor**:
  - Command: `motor:front`
  - Description: Activates or controls the front motor.

- **Left Motor**:
  - Command: `motor:left`
  - Description: Activates or controls the left motor.

- **Right Motor**:
  - Command: `motor:right`
  - Description: Activates or controls the right motor.

- **Forward Motor**:
  - Command: `motor:forward`
  - Description: Commands the motor to move forward.

- **Set Motor Speed**:
  - Command: `motor:speed:[x]`
  - Description: Sets the speed of the motor. Replace `[x]` with the desired speed value.

### Telemetry Data

- **Baterry Voltage**
  - Command: `read:battery`
  - Description: prints the battery voltage

- **DHT 22 Data**
  - Command: `read:dht`
  - Description: prints the humidity and the temperature in celsius

### Servo Commands

- **Camera Pitch**:
  - Command: `servo:cam_pitch:[x]`
  - Description: Adjusts the pitch of the camera servo. Replace `[x]` with the desired angle.

- **Camera Yaw**:
  - Command: `servo:cam_yaw:[x]`
  - Description: Adjusts the yaw of the camera servo. Replace `[x]` with the desired angle.

### Light Commands

- **Front Strip RGB Color Setting**:
  - Command: `lights:front:[r]:[g]:[b]`
  - Description: Sets the RGB values for the front strip. Replace `[r]`, `[g]`, and `[b]` with the desired red, green, and blue values respectively.

- **Back Strip RGB Color Setting**:
  - Command: `lights:back:[r]:[g]:[b]`
  - Description: Sets the RGB values for the back strip. Replace `[r]`, `[g]`, and `[b]` with the desired red, green, and blue values respectively.

- **Turn Off Both Strips**:
  - Command: `lights:animation:off`
  - Description: Turns off both the front and back strips, setting their colors to black (0,0,0).

- **Blink Left Animation**:
  - Command: `lights:animation:blink_left`
  - Description: Initiates the blink left animation on both light strips.

- **Blink Right Animation**:
  - Command: `lights:animation:blink_right`
  - Description: Initiates the blink right animation on both light strips.

- **Blink All Animation**:
  - Command: `lights:animation:blink_all`
  - Description: Initiates the blink animation on both light strips.

- **Police Animation**:
  - Command: `lights:animation:police`
  - Description: Initiates the police lights animation on both strips.

## More Information

For detailed documentation, user manuals, and more information about the Gravis project, please visit the [Gravis Docs](https://dimitarbez.github.io/GravisDocs) page.

## License

This project is licensed under the terms of the GNU General Public License v3.0 (GPL v3).

### Main Points of the GPL v3 License:

- **Freedom to Use**: You can use the software for any purpose.
- **Freedom to Study**: You can study how the software works and modify it.
- **Freedom to Share**: You can copy and distribute the software.
- **Freedom to Improve**: You can improve upon the software and then distribute those improvements.

However, when distributing derived works, the source code of the work must be made available under the same, or a compatible license.

For the full license text, please refer to the [GPL v3 License](https://www.gnu.org/licenses/gpl-3.0.en.html).

## Contributors

[Dimitar Bezhanovski](https://www.linkedin.com/in/dimitar-bezhanovski/)

# Flooding Alert System using ESP32

## Project Overview
This project is a **Flooding Alert System** designed to detect water presence using an ESP32 microcontroller. Upon detecting a flood of water, the system activates an LED and a buzzer as local alerts, and sends an email notification to a specified recipient.

## Features
- **Water Detection**: Uses a water sensor to monitor for the presence of water.
- **Alerts**:
  - **LED**: Turns on when water is detected.
  - **Buzzer**: Sounds an alarm when water is detected.
  - **Email Notification**: Sends an email alert to a predefined recipient.
- **Wi-Fi Connectivity**: Connects to a Wi-Fi network for email notifications.
- **Task Scheduling**: Uses FreeRTOS tasks for efficient multitasking.

## Components Used
- **ESP32**
- **Water Sensor** (connected to GPIO 32)
- **LED** (connected to GPIO 22)
- **Buzzer** (connected to GPIO 23)
- **Wi-Fi Network**

## How It Works
1. The ESP32 continuously monitors the water sensor.
2. If water is detected:
   - The LED and buzzer are activated.
   - An email notification is sent via an SMTP server.
3. The system uses FreeRTOS to manage tasks for:
   - Reading the water sensor.
   - Handling Wi-Fi reconnection.

## Configuration
### Wi-Fi Settings
Update the following variables in the code with your Wi-Fi credentials:
```cpp
const char* ssid = "<Your_SSID>";
const char* password = "<Your_Password>";
```

### Email Settings
Configure the email settings for sending alerts:
```cpp
#define EMAIL_ADDRESS "<Your_Email_Address>"
#define EMAIL_PASSWORD "<Your_Email_Password>"
#define RECIPIENT_EMAIL "<Recipient_Email_Address>"
```
> **Note:** The email password must be an application-specific password if using Gmail.

## Dependencies
- [ESP Mail Client Library](https://github.com/mobizt/ESP-Mail-Client)

## Setup Instructions
1. Install the [Arduino IDE](https://www.arduino.cc/en/software/) and configure it for ESP32 development.
2. Install the **ESP Mail Client** library via the Arduino Library Manager.
3. Connect the components as per the GPIO pin configuration in the code.
4. Upload the code to the ESP32.

## Usage
1. Power on the ESP32.
2. Ensure the device connects to the specified Wi-Fi network.
3. Place the water sensor in the desired location.
4. Observe:
   - LED and buzzer activation when water is detected.
   - Email notifications for flooding alerts.



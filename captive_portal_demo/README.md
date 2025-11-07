ESP32 Captive Portal Demo (Sanitized)
====================================

Overview
--------
This repository folder contains a safe, educational demo for an ESP32 that:

- Runs as a Wi-Fi Access Point (AP)
- Presents a captive-portal style web UI
- Scans for nearby Wi-Fi networks and lists them
- Allows the user to select a network and attempt a normal Wi-Fi connection using WiFi.begin()

Importantly: any deauthentication/evil-twin/packet-injection functionality has been removed for legal and ethical reasons. This demo is intended for classroom learning only.

Equipment Used
---------------
- ESP32 development board (e.g., AI-Thinker ESP32-CAM, ESP32 DevKit)
- USB cable
- Computer with Arduino IDE

Tools Used
----------
- Arduino IDE (or PlatformIO)
- ESP32 board support package for Arduino
- Optional: Serial monitor to view device logs

Files
-----
- `ESP32_CaptivePortal_Demo.ino` — The sanitized demo sketch
- `README.md` — This file

Steps I Followed
----------------
1. Install Arduino IDE and add ESP32 board support (via Boards Manager).
2. Open `ESP32_CaptivePortal_Demo.ino` in the Arduino IDE.
3. Select your ESP32 board (e.g., "ESP32 Dev Module").
4. Select the correct COM port.
5. Upload the sketch to the ESP32.
6. After upload, open the serial monitor (115200 baud) to follow logs.
7. Connect a phone or laptop to the AP `ESP32-Demo-AP` (password: `12345678`).
8. Open a browser — the captive portal should present the scanned networks.
9. (Classroom use) Select a network and optionally enter a password to attempt a normal connection. The device will try WiFi.begin() for up to 10 seconds and then return to AP mode.

Problems / Solutions
--------------------
- Problem: Device not showing AP
  - Solution: Ensure WiFi.softAP() call succeeded and check serial monitor for errors. Verify power and USB cable.
- Problem: No networks shown in scan
  - Solution: Make sure Wi-Fi is enabled, wait a few seconds for the scan to complete, and be in an area with Wi-Fi networks.

Final Report
------------
This demo provides a safe environment to learn how an ESP32 can provide a captive-portal style web UI and perform read-only scans of nearby Wi-Fi networks. All potentially harmful actions (deauthentication, packet injection, spoofing) were intentionally removed to ensure the code is suitable for distribution and classroom use.

If you need the original (unedited) code for instructor-approved lab work, please obtain explicit written permission from your instructor and ensure you operate on test networks where you have authorization.

License / Ethical Use
---------------------
Use this code only for learning and on networks you own or are authorized to test. Unauthorized network interference is illegal and unethical.

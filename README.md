# Cybersecurity : CSN150
Project: ESP32 XXXXXXXX

## Purpose
Set up ESP32 and Arduino enviornment. Execute sketch " Wifiscanner". 

## Equipment
* [ESP32Cam](https://www.amazon.com/Aideepen-ESP32-CAM-Bluetooth-ESP32-CAM-MB-Arduino/dp/B08P2578LV/ref=sr_1_3?crid=4FY0ECFW0ZX7&keywords=ESP32+Cam&qid=1678902050&sprefix=esp32+cam%2Caps%2C240&sr=8-3)

* [USB Micro Data Cable](https://www.amazon.com/AmazonBasics-Male-Micro-Cable-Black/dp/B0711PVX6Z/ref=sr_1_1_sspa?keywords=micro+usb+data+cable&qid=1678902214&sprefix=Micro+USB+data+%2Caps%2C89&sr=8-1-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUFaU0NaUVZHU1RFUlAmZW5jcnlwdGVkSWQ9QTA3NTA4MDVFVERCS01HVlgxM1YmZW5jcnlwdGVkQWRJZD1BMDE4NTE1NTIwWUdONkdWSzU1M1Amd2lkZ2V0TmFtZT1zcF9hdGYmYWN0aW9uPWNsaWNrUmVkaXJlY3QmZG9Ob3RMb2dDbGljaz10cnVl)

## Links to documentation and tools

##### Video 1: 

##### Other Links: 

##### AI GPTs used

## Steps I followed
1. Install required software:
   - Install Arduino IDE
   - Add ESP32 board support
   - Install ESP32 libraries

2. Hardware setup:
   - Connect ESP32-CAM to FTDI adapter:
     * ESP32-CAM VCC → FTDI VCC (5V)
     * ESP32-CAM GND → FTDI GND
     * ESP32-CAM U0R → FTDI TX
     * ESP32-CAM U0T → FTDI RX
     * GPIO 0 → GND (for programming mode)

3. Arduino IDE setup:
   - Select Board: "AI Thinker ESP32-CAM"
   - Select Partition Scheme: "Huge APP (3MB No OTA/1MB SPIFFS)"
   - Select the correct COM port
   - Set upload speed to 115200

4. Upload procedure:
   - Connect GPIO 0 to GND
   - Press and hold RESET button
   - Start upload in Arduino IDE
   - Release RESET button after upload begins
   - Wait for upload to complete

5. After upload:
   - Disconnect GPIO 0 from GND
   - Press RESET button
   - Look for WiFi network "ESP32-CAM Access Point"
   - Connect using password "123456789"
   - Access camera at http://192.168.4.1

## Problems and Solutions
Note your problems or errors here.  Google any error you may come across, and not what you tried (even if it does not work), and what was the final answer. Document your errors and solutions that worked for you.  

**Problem:** E (485) camera: Camera probe failed with error 0x105(ESP_ERR_NOT_FOUND)
Camera init failed with error 0x105
**Solution:**

**Problem:** Failed to connect to ESP32: Invalid head of packet (0xFC): Possible serial noise or corruption
**Solution:** 
1. Make sure the ESP32-CAM is properly connected:
   - Connect GPIO0 to GND (this puts it in download mode)
   - Hold down the RST (reset) button
   - Release the RST button after upload begins
2. Wiring connections:
   - ESP32-CAM | FTDI Adapter
   - 5V/VCC    | VCC (5V)
   - GND       | GND
   - UOT/TX    | RX
   - UOR/RX    | TX
3. Try a different USB cable
4. Verify COM port in Arduino IDE
5. Use a lower baud rate (115200 instead of 460800)

### Example Problem
**Problem:** Arduino code will not load on ESP32 Cam.
**Solution:** Camera drivers were incorrect I needed to install the driver: [https://www.wch-ic.com/downloads/CH341SER_ZIP.html](https://github.com/martin-ger/esp32_nat_router).  I used file, "CH341SER.ZIP" and it worked.

## Final Report

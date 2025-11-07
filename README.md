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

## AI-Thinker Camera Code and Instructions

Below is the sketch used for the AI-Thinker ESP32-CAM configured as a WiFi Access Point and web camera server. Save this into a file named `ESP32_Camera_AP.ino` in the project root (it's already included in this repo).

```cpp
#include "esp_camera.h"
#include <WiFi.h>

//
// WARNING!!! PSRAM IC required for UXGA resolution and high JPEG quality
//            Ensure ESP32 Wrover Module or other board with PSRAM is selected
//            Partial images will be transmitted if image exceeds buffer size
//
//            You must select partition scheme from the board menu that has at least 3MB APP space.
//            Face Recognition is DISABLED for ESP32 and ESP32-S2, because it takes up from 15
//            seconds to process single frame. Face Detection is ENABLED if PSRAM is enabled as well

// ===================
// Select camera model
// ===================
//#define CAMERA_MODEL_WROVER_KIT // Has PSRAM
//#define CAMERA_MODEL_ESP_EYE // Has PSRAM
//#define CAMERA_MODEL_ESP32S3_EYE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_PSRAM // Has PSRAM
//#define CAMERA_MODEL_M5STACK_V2_PSRAM // M5Camera version B Has PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_ESP32CAM // No PSRAM
//#define CAMERA_MODEL_M5STACK_UNITCAM // No PSRAM
//#define CAMERA_MODEL_M5STACK_CAMS3_UNIT // Has PSRAM
#define CAMERA_MODEL_AI_THINKER // Has PSRAM
//#define CAMERA_MODEL_TTGO_T_JOURNAL // No PSRAM
//#define CAMERA_MODEL_XIAO_ESP32S3 // Has PSRAM
// ** Espressif Internal Boards **
//#define CAMERA_MODEL_ESP32_CAM_BOARD
//#define CAMERA_MODEL_ESP32S2_CAM_BOARD
//#define CAMERA_MODEL_ESP32S3_CAM_LCD
//#define CAMERA_MODEL_DFRobot_FireBeetle2_ESP32S3 // Has PSRAM
//#define CAMERA_MODEL_DFRobot_Romeo_ESP32S3 // Has PSRAM
#include "camera_pins.h"

// ===========================
// Enter your WiFi credentials
// ===========================
const char* ssid = "ESP32-CAM Access Point";
const char* password = "123456789";

void startCameraServer();
void setupLedFlash(int pin);

void setup() {
   Serial.begin(115200);
   Serial.setDebugOutput(true);
   Serial.println();

   camera_config_t config;
   config.ledc_channel = LEDC_CHANNEL_0;
   config.ledc_timer = LEDC_TIMER_0;
   config.pin_d0 = Y2_GPIO_NUM;
   config.pin_d1 = Y3_GPIO_NUM;
   config.pin_d2 = Y4_GPIO_NUM;
   config.pin_d3 = Y5_GPIO_NUM;
   config.pin_d4 = Y6_GPIO_NUM;
   config.pin_d5 = Y7_GPIO_NUM;
   config.pin_d6 = Y8_GPIO_NUM;
   config.pin_d7 = Y9_GPIO_NUM;
   config.pin_xclk = XCLK_GPIO_NUM;
   config.pin_pclk = PCLK_GPIO_NUM;
   config.pin_vsync = VSYNC_GPIO_NUM;
   config.pin_href = HREF_GPIO_NUM;
   config.pin_sccb_sda = SIOD_GPIO_NUM;
   config.pin_sccb_scl = SIOC_GPIO_NUM;
   config.pin_pwdn = PWDN_GPIO_NUM;
   config.pin_reset = RESET_GPIO_NUM;
   config.xclk_freq_hz = 20000000;
   config.frame_size = FRAMESIZE_UXGA;
   config.pixel_format = PIXFORMAT_JPEG; // for streaming
   config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
   config.fb_location = CAMERA_FB_IN_PSRAM;
   config.jpeg_quality = 12;
   config.fb_count = 1;
  
   // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
   //                      for larger pre-allocated frame buffer.
   if(config.pixel_format == PIXFORMAT_JPEG){
      if(psramFound()){
         config.jpeg_quality = 10;
         config.fb_count = 2;
         config.grab_mode = CAMERA_GRAB_LATEST;
      } else {
         // Limit the frame size when PSRAM is not available
         config.frame_size = FRAMESIZE_SVGA;
         config.fb_location = CAMERA_FB_IN_DRAM;
      }
   } else {
      // Best option for face detection/recognition
      config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
      config.fb_count = 2;
#endif
   }

   // camera init
   esp_err_t err = esp_camera_init(&config);
   if (err != ESP_OK) {
      Serial.printf("Camera init failed with error 0x%x", err);
      return;
   }

   sensor_t * s = esp_camera_sensor_get();
   // initial sensors are flipped vertically and colors are a bit saturated
   if (s->id.PID == OV3660_PID) {
      s->set_vflip(s, 1);        // flip it back
      s->set_brightness(s, 1);    // up the brightness just a bit
      s->set_saturation(s, -2);   // lower the saturation
   }
   // drop down frame size for higher initial frame rate
   if(config.pixel_format == PIXFORMAT_JPEG){
      s->set_framesize(s, FRAMESIZE_QVGA);
   }

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
   s->set_vflip(s, 1);
   s->set_hmirror(s, 1);
#endif

#if defined(CAMERA_MODEL_ESP32S3_EYE)
   s->set_vflip(s, 1);
#endif

// Setup LED FLash if LED pin is defined in camera_pins.h
#if defined(LED_GPIO_NUM)
   setupLedFlash(LED_GPIO_NUM);
#endif

   // Set ESP32 as Access Point
   WiFi.mode(WIFI_AP);
   WiFi.softAP(ssid, password);

   IPAddress IP = WiFi.softAPIP();
   Serial.print("AP IP address: ");
   Serial.println(IP);

   startCameraServer();

   Serial.print("Camera Ready! Use '");
   Serial.print(IP);
   Serial.println("' to connect");
}

void loop() {
   // Do nothing. Everything is done in another task by the web server
   delay(10000);
}
```

Quick steps (summary):

1. Hardware wiring for programming:
    - ESP32-CAM VCC → FTDI 5V
    - ESP32-CAM GND → FTDI GND
    - ESP32-CAM U0R (RX) → FTDI TX
    - ESP32-CAM U0T (TX) → FTDI RX
    - GPIO0 → GND (hold during upload)

2. Arduino IDE settings:
    - Board: "AI Thinker ESP32-CAM" (or "ESP32 Wrover Module")
    - Partition Scheme: "Huge APP (3MB No OTA/1MB SPIFFS)"
    - Upload Speed: 115200

3. Upload:
    - Connect GPIO0 to GND, hold RESET, start upload, release RESET after upload begins
    - After upload, disconnect GPIO0 from GND and reset the board
    - Connect to WiFi SSID: `ESP32-CAM Access Point` (password `123456789`)
    - Open the camera web interface at the AP IP (default `192.168.4.1`)

If you'd like, I can now push this updated README to a GitHub repository you control — tell me the GitHub repository URL (https://github.com/YOUR_USERNAME/REPO_NAME.git) and I will add it as a remote and push the `main` branch.

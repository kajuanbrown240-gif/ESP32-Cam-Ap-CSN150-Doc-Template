#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

// Safe demo: AP + captive portal + network scanner + benign connection attempt
// No deauthentication, no packet injection, no evil-twin functionality.

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;
WebServer webServer(80);

String scannedNetworksHTML();

void handleRoot();
void handleConnect();

void setup() {
  Serial.begin(115200);
  delay(100);

  // Start as Access Point
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("ESP32-Demo-AP", "12345678");

  // Start DNS server to redirect all requests to the AP IP (captive portal behavior)
  dnsServer.start(DNS_PORT, "*", apIP);

  // Web server endpoints
  webServer.on("/", handleRoot);
  webServer.on("/connect", HTTP_POST, handleConnect);
  webServer.onNotFound(handleRoot);
  webServer.begin();

  Serial.println("Captive portal demo started. Connect to SSID: ESP32-Demo-AP (pw:12345678)");
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}

String scannedNetworksHTML() {
  String html = "<h3>Available WiFi networks (scan)</h3>";
  int n = WiFi.scanNetworks();
  if (n == 0) {
    html += "<p>No networks found</p>";
  } else {
    html += "<form method='POST' action='/connect'><table border=1 cellpadding=6><tr><th>SSID</th><th>RSSI</th><th>Channel</th><th>Select</th></tr>";
    for (int i = 0; i < n && i < 32; ++i) {
      String ssid = WiFi.SSID(i);
      int32_t rssi = WiFi.RSSI(i);
      int ch = WiFi.channel(i);
      html += "<tr><td>" + ssid + "</td><td>" + String(rssi) + "</td><td>" + String(ch) + "</td>";
      html += "<td><input type='radio' name='ssid' value='" + ssid + "'></td></tr>";
    }
    html += "</table><p><label>Password (will be used only to attempt a normal connection):</label>";
    html += "<input type='password' name='password' minlength='8' placeholder='password'><p>";
    html += "<input type='submit' value='Try Connect'></form>";
  }
  return html;
}

void handleRoot() {
  String page = "<!DOCTYPE html><html><head><meta name=viewport content='width=device-width,initial-scale=1'>";
  page += "<title>ESP32 Captive Portal Demo</title></head><body><h1>ESP32 Captive Portal Demo</h1>";
  page += "<p>This is a safe demonstration for class: the device runs as an Access Point and shows available networks. "
          "It can attempt a normal WiFi connection to a selected network (no packet injection or deauth will be performed).</p>";
  page += scannedNetworksHTML();
  page += "<hr><small>Note: attack code has been removed for safety and legal/ethical reasons.</small>";
  page += "</body></html>";

  webServer.send(200, "text/html", page);
}

void handleConnect() {
  if (!webServer.hasArg("ssid")) {
    webServer.send(400, "text/plain", "Missing ssid");
    return;
  }
  String ssid = webServer.arg("ssid");
  String password = webServer.arg("password");

  String statusPage = "<!DOCTYPE html><html><head><meta name=viewport content='width=device-width,initial-scale=1'>";
  statusPage += "<title>Connection Attempt</title></head><body>";
  statusPage += "<h2>Attempting connection to: " + ssid + "</h2>";
  statusPage += "<p>Trying to connect using normal WiFi.begin() for up to 10 seconds...</p>";
  webServer.send(200, "text/html", statusPage + "<p>(Check serial monitor for details)</p></body></html>");

  // Attempt a normal connection (benign)
  WiFi.disconnect(true);
  delay(1000);
  WiFi.begin(ssid.c_str(), password.c_str());

  unsigned long start = millis();
  bool connected = false;
  while (millis() - start < 10000) { // 10s timeout
    if (WiFi.status() == WL_CONNECTED) {
      connected = true;
      break;
    }
    delay(500);
  }

  if (connected) {
    Serial.println("Connected to " + ssid + " IP: " + WiFi.localIP().toString());
  } else {
    Serial.println("Failed to connect to " + ssid);
  }

  // After attempt, go back to AP mode for demo
  WiFi.disconnect(true);
  delay(500);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255,255,255,0));
  WiFi.softAP("ESP32-Demo-AP", "12345678");
}

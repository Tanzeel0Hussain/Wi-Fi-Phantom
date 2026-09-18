#include <Arduino.h>
#include <WiFi.h>
#include "dashboard.h"
#include "scanner.h"

namespace {
constexpr char AP_SSID[] = "Wi-Fi-Phantom-Monitor";
constexpr char AP_PASSWORD[] = "phantomsecure";
constexpr unsigned long AUTO_SCAN_INTERVAL_MS = 30000;
unsigned long lastScanMs = 0;
}

void setup() {
  Serial.begin(115200);
  delay(300);

  WiFi.mode(WIFI_AP_STA);
  WiFi.setSleep(false);
  WiFi.softAP(AP_SSID, AP_PASSWORD);

  Serial.println();
  Serial.println("Wi-Fi Phantom defensive monitor");
  Serial.print("Monitor AP: ");
  Serial.println(AP_SSID);
  Serial.print("Dashboard IP: ");
  Serial.println(WiFi.softAPIP());

  performNetworkScan();
  lastScanMs = millis();
  beginDashboard();
}

void loop() {
  handleDashboardClient();

  if (millis() - lastScanMs >= AUTO_SCAN_INTERVAL_MS) {
    performNetworkScan();
    lastScanMs = millis();
  }

  delay(2);
}

#pragma once

#include <Arduino.h>
#include <WiFi.h>

constexpr size_t MAX_OBSERVED_NETWORKS = 32;

struct NetworkObservation {
  String ssid;
  String bssid;
  int32_t rssi = -127;
  uint8_t channel = 0;
  wifi_auth_mode_t auth = WIFI_AUTH_OPEN;
  bool openNetwork = false;
  bool duplicateSsid = false;
  bool channelChanged = false;
  bool securityChanged = false;
};

struct ScanSummary {
  size_t networks = 0;
  size_t openNetworks = 0;
  size_t duplicateSsids = 0;
  size_t changedNetworks = 0;
  unsigned long completedAtMs = 0;
};

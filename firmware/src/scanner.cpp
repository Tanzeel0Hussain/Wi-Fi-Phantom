#include "scanner.h"

namespace {
NetworkObservation current[MAX_OBSERVED_NETWORKS];
NetworkObservation previous[MAX_OBSERVED_NETWORKS];
size_t currentCount = 0;
size_t previousCount = 0;
ScanSummary summary;

int previousIndexForBssid(const String& bssid) {
  for (size_t i = 0; i < previousCount; ++i) {
    if (previous[i].bssid.equalsIgnoreCase(bssid)) {
      return static_cast<int>(i);
    }
  }
  return -1;
}
}

String authModeLabel(wifi_auth_mode_t auth) {
  switch (auth) {
    case WIFI_AUTH_OPEN: return "Open";
    case WIFI_AUTH_WEP: return "WEP";
    case WIFI_AUTH_WPA_PSK: return "WPA";
    case WIFI_AUTH_WPA2_PSK: return "WPA2";
    case WIFI_AUTH_WPA_WPA2_PSK: return "WPA/WPA2";
    case WIFI_AUTH_WPA2_ENTERPRISE: return "WPA2-Enterprise";
#ifdef WIFI_AUTH_WPA3_PSK
    case WIFI_AUTH_WPA3_PSK: return "WPA3";
#endif
#ifdef WIFI_AUTH_WPA2_WPA3_PSK
    case WIFI_AUTH_WPA2_WPA3_PSK: return "WPA2/WPA3";
#endif
    default: return "Other";
  }
}

void performNetworkScan() {
  previousCount = currentCount;
  for (size_t i = 0; i < previousCount; ++i) {
    previous[i] = current[i];
  }

  currentCount = 0;
  summary = ScanSummary{};

  const int found = WiFi.scanNetworks(false, true);
  if (found <= 0) {
    WiFi.scanDelete();
    summary.completedAtMs = millis();
    return;
  }

  const size_t limit = min(static_cast<size_t>(found), MAX_OBSERVED_NETWORKS);
  for (size_t i = 0; i < limit; ++i) {
    NetworkObservation item;
    item.ssid = WiFi.SSID(static_cast<int>(i));
    item.bssid = WiFi.BSSIDstr(static_cast<int>(i));
    item.rssi = WiFi.RSSI(static_cast<int>(i));
    item.channel = static_cast<uint8_t>(WiFi.channel(static_cast<int>(i)));
    item.auth = WiFi.encryptionType(static_cast<int>(i));
    item.openNetwork = item.auth == WIFI_AUTH_OPEN;

    const int oldIndex = previousIndexForBssid(item.bssid);
    if (oldIndex >= 0) {
      item.channelChanged = previous[oldIndex].channel != item.channel;
      item.securityChanged = previous[oldIndex].auth != item.auth;
    }

    current[currentCount++] = item;
  }

  for (size_t i = 0; i < currentCount; ++i) {
    if (current[i].ssid.length() == 0) continue;
    for (size_t j = 0; j < currentCount; ++j) {
      if (i == j) continue;
      if (current[i].ssid == current[j].ssid &&
          !current[i].bssid.equalsIgnoreCase(current[j].bssid)) {
        current[i].duplicateSsid = true;
        break;
      }
    }
  }

  summary.networks = currentCount;
  for (size_t i = 0; i < currentCount; ++i) {
    if (current[i].openNetwork) ++summary.openNetworks;
    if (current[i].duplicateSsid) ++summary.duplicateSsids;
    if (current[i].channelChanged || current[i].securityChanged) {
      ++summary.changedNetworks;
    }
  }
  summary.completedAtMs = millis();
  WiFi.scanDelete();
}

size_t getObservationCount() {
  return currentCount;
}

const NetworkObservation& getObservation(size_t index) {
  return current[index];
}

const ScanSummary& getScanSummary() {
  return summary;
}

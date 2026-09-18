#pragma once

#include "network_model.h"

void performNetworkScan();
size_t getObservationCount();
const NetworkObservation& getObservation(size_t index);
const ScanSummary& getScanSummary();
String authModeLabel(wifi_auth_mode_t auth);

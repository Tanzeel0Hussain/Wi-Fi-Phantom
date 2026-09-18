# Wi-Fi Phantom — ESP32-C3 Wireless Security Monitor

[![Firmware Build](https://github.com/Tanzeel0Hussain/Wi-Fi-Phantom/actions/workflows/firmware.yml/badge.svg)](https://github.com/Tanzeel0Hussain/Wi-Fi-Phantom/actions/workflows/firmware.yml)
[![Live Installer](https://img.shields.io/badge/Live-Web%20Installer-2563eb?logo=github)](https://tanzeel0hussain.github.io/Wi-Fi-Phantom/)
[![Firmware](https://img.shields.io/badge/Download-Firmware-16a34a?logo=espressif)](https://github.com/Tanzeel0Hussain/Wi-Fi-Phantom/releases/latest)
[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

**Wi-Fi Phantom** is a defensive ESP32-C3 wireless visibility project. The maintained firmware passively scans nearby 2.4 GHz Wi-Fi networks and presents observations through a responsive dashboard hosted by the board.

> **Defensive scope:** the maintained firmware does not disconnect clients, impersonate third-party access points, or collect Wi-Fi credentials.

## Quick Links

- **[Open Live Web Installer](https://tanzeel0hussain.github.io/Wi-Fi-Phantom/)**
- **[Download Latest Firmware](https://github.com/Tanzeel0Hussain/Wi-Fi-Phantom/releases/latest)**
- **[View Build Status](https://github.com/Tanzeel0Hussain/Wi-Fi-Phantom/actions)**
- **[Browse Source Code](firmware/src)**
- **[Responsible-Use Guidance](SECURITY.md)**

## Current Features

- Passive nearby Wi-Fi scanning
- SSID and BSSID inventory
- RSSI and 2.4 GHz channel visibility
- Advertised security-mode identification
- Open-network warnings
- Duplicate-SSID observations for manual review
- Channel/security changes between scans
- Responsive ESP32-C3 local dashboard
- Device information and free-heap visibility
- PlatformIO project structure
- Browser-based firmware installation
- Automated firmware build and downloadable release

> A duplicate SSID is **not automatically a rogue access point**. Mesh systems, extenders and enterprise deployments commonly advertise one SSID from multiple BSSIDs. Wi-Fi Phantom reports observations; the operator decides whether they are expected.

## Browser Installation

Use **[Install Wi-Fi Phantom](https://tanzeel0hussain.github.io/Wi-Fi-Phantom/)** for the easiest setup.

1. Connect the ESP32-C3 with a data-capable USB cable.
2. Open the installer in desktop Chrome or Microsoft Edge.
3. Click **Install Wi-Fi Phantom**.
4. Approve the serial device requested by the browser.
5. Wait for flashing to finish, then restart the board.

The browser always requires the user to approve serial-device access.

## After Installation

1. Restart the ESP32-C3.
2. Connect to **Wi-Fi-Phantom-Monitor**.
3. Enter password **phantomsecure**.
4. Open **[Device Dashboard](http://192.168.4.1)**.
5. Press **Scan now** whenever you want a fresh observation.

## What the Dashboard Shows

| Field | Purpose |
| --- | --- |
| SSID | Advertised network name |
| BSSID | Access-point MAC address |
| Channel | Observed 2.4 GHz channel |
| RSSI | Observed signal strength |
| Security | Advertised Wi-Fi security mode |
| Observation | Open, duplicate SSID, changed channel/security, or normal observation |

## Build from Source

Requirements: Python 3, PlatformIO, and a compatible ESP32-C3 development board.

```bash
git clone https://github.com/Tanzeel0Hussain/Wi-Fi-Phantom.git
cd Wi-Fi-Phantom
pio run -e esp32c3
```

Upload through PlatformIO:

```bash
pio run -e esp32c3 -t upload
```

## Project Structure

```text
Wi-Fi-Phantom/
├── .github/workflows/firmware.yml
├── docs/
│   ├── firmware/
│   ├── index.html
│   └── manifest.json
├── firmware/
│   ├── include/
│   │   ├── dashboard.h
│   │   ├── network_model.h
│   │   └── scanner.h
│   └── src/
│       ├── dashboard.cpp
│       ├── main.cpp
│       └── scanner.cpp
├── platformio.ini
├── SECURITY.md
├── LICENSE
└── README.md
```

## Firmware Design

```text
ESP32-C3
   │
   ├── Passive Wi-Fi Scanner
   │      └── SSID / BSSID / RSSI / Channel / Security
   │
   ├── Observation Engine
   │      └── Open / Duplicate SSID / Changed properties
   │
   └── Local Web Dashboard
          └── Browser at 192.168.4.1
```

## Safety and Scope

Use Wi-Fi Phantom only with hardware and wireless environments you own or are explicitly authorized to assess. The current project is designed for visibility, learning and defensive review rather than disruption or credential collection.

See **[SECURITY.md](SECURITY.md)** for details.

## USB Driver Note

USB-UART support depends on the bridge used by your board. Use the official driver supplied by the board/USB-bridge manufacturer rather than keeping third-party driver archives in this repository.

## Roadmap

- Known-network allowlist for more useful duplicate-SSID review
- Channel-distribution summary
- Optional local event history
- Additional tested ESP32-C3 boards
- Hardware screenshots and wiring notes

## License

Released under the **[MIT License](LICENSE)**.

## Maintainer

**Tanzeel Hussain** · **[GitHub Profile](https://github.com/Tanzeel0Hussain)**

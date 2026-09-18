# Wi-Fi Phantom — ESP32-C3 Wireless Security Monitor

[![Firmware Build](https://github.com/Tanzeel0Hussain/Wi-Fi-Phantom/actions/workflows/firmware.yml/badge.svg)](https://github.com/Tanzeel0Hussain/Wi-Fi-Phantom/actions/workflows/firmware.yml)
[![Live Installer](https://img.shields.io/badge/Live-Web%20Installer-2563eb?logo=github)](https://tanzeel0hussain.github.io/Wi-Fi-Phantom/)
[![Firmware](https://img.shields.io/badge/Download-Firmware-16a34a?logo=espressif)](https://github.com/Tanzeel0Hussain/Wi-Fi-Phantom/releases/latest)
[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

**Wi-Fi Phantom** is a defensive ESP32-C3 wireless visibility project. It passively scans nearby 2.4 GHz Wi-Fi networks and presents review-oriented observations through a responsive dashboard hosted by the board.

> **Defensive scope:** the maintained firmware does not disconnect clients, impersonate third-party access points, or collect Wi-Fi credentials.

## Quick Links

- **[Open Live Web Installer](https://tanzeel0hussain.github.io/Wi-Fi-Phantom/)**
- **[Download Latest Firmware](https://github.com/Tanzeel0Hussain/Wi-Fi-Phantom/releases/latest)**
- **[View Firmware Releases](https://github.com/Tanzeel0Hussain/Wi-Fi-Phantom/releases)**
- **[View Build Status](https://github.com/Tanzeel0Hussain/Wi-Fi-Phantom/actions)**
- **[Browse Source Code](firmware/src)**
- **[Responsible-Use Guidance](SECURITY.md)**

## Features

- Passive nearby Wi-Fi scanning
- SSID and BSSID inventory
- RSSI and 2.4 GHz channel visibility
- Advertised security-mode identification
- Open-network warnings
- Duplicate-SSID observations for manual review
- Channel/security changes observed between scans
- Responsive ESP32-C3 local dashboard
- Device/chip and free-heap information
- PlatformIO-based modular firmware
- Browser installation with ESP Web Tools
- Automated firmware build and downloadable release

> A duplicate SSID is **not automatically a rogue access point**. Mesh systems, extenders and enterprise deployments commonly advertise one SSID from multiple BSSIDs. Wi-Fi Phantom reports observations that require context.

## Browser Installation

Use **[Install Wi-Fi Phantom](https://tanzeel0hussain.github.io/Wi-Fi-Phantom/)**.

1. Connect a compatible ESP32-C3 using a data-capable USB cable.
2. Open the installer in desktop Chrome or Microsoft Edge.
3. Approve the serial device requested by the browser.
4. Install the current firmware and wait for flashing to complete.
5. Restart the board.

## After Installation

1. Connect to **Wi-Fi-Phantom-Monitor**.
2. Use password **phantomsecure**.
3. Open **[Device Dashboard](http://192.168.4.1)**.
4. Press **Scan now** for a fresh observation.

## Dashboard Data

| Field | Meaning |
| --- | --- |
| SSID | Advertised network name |
| BSSID | Access-point MAC address |
| Channel | Observed 2.4 GHz channel |
| RSSI | Observed signal strength |
| Security | Advertised Wi-Fi security mode |
| Observation | Open, duplicate SSID, changed properties, or normal observation |

## Build from Source

Requirements: Python 3, PlatformIO and a compatible ESP32-C3 board.

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

## Architecture

```text
ESP32-C3
   │
   ├── Passive Scanner
   │      └── SSID · BSSID · RSSI · Channel · Security
   │
   ├── Observation Engine
   │      └── Open · Duplicate SSID · Changed properties
   │
   └── Local Dashboard
          └── Browser at 192.168.4.1
```

## Automated Firmware Pipeline

Each relevant push to `main` compiles the ESP32-C3 project, creates a browser-installable merged firmware image, uploads a build artifact, updates the firmware used by the live installer, and publishes the current GitHub Release.

## Safety and Scope

Use Wi-Fi Phantom only with hardware and wireless environments you own or are explicitly authorized to assess. The maintained project focuses on visibility, learning and defensive review rather than disruption or credential collection.

See **[SECURITY.md](SECURITY.md)** for responsible-use guidance.

## USB Driver Note

USB-UART support depends on the bridge used by your board. Install the official driver supplied by the board or USB-bridge manufacturer instead of keeping third-party driver archives in this repository.

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

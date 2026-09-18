# Wi-Fi Phantom

[![Firmware Build](https://github.com/Tanzeel0Hussain/Wi-Fi-Phantom/actions/workflows/firmware.yml/badge.svg)](https://github.com/Tanzeel0Hussain/Wi-Fi-Phantom/actions/workflows/firmware.yml)
[![Live Installer](https://img.shields.io/badge/Live-Web%20Installer-2563eb?logo=github)](https://tanzeel0hussain.github.io/Wi-Fi-Phantom/)
[![Firmware](https://img.shields.io/badge/Download-ESP32--C3%20Firmware-16a34a?logo=espressif)](https://github.com/Tanzeel0Hussain/Wi-Fi-Phantom/releases)
[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

**Wi-Fi Phantom v3.0** is a defensive ESP32-C3 wireless visibility project. It passively inventories nearby Wi-Fi networks, highlights review-worthy duplicate SSIDs and scan-to-scan changes, and serves a responsive local dashboard from the device.

> **Current main branch:** no client deauthentication, third-party access-point impersonation, or Wi-Fi credential collection.

## Quick Links

- **[Open Live Web Installer](https://tanzeel0hussain.github.io/Wi-Fi-Phantom/)**
- **[Download Latest Firmware](https://github.com/Tanzeel0Hussain/Wi-Fi-Phantom/releases)**
- **[View Firmware Builds](https://github.com/Tanzeel0Hussain/Wi-Fi-Phantom/actions)**
- **[Read Security Guidance](SECURITY.md)**
- **[Browse Source Code](firmware/src)**

## What It Shows

| Observation | Purpose |
| --- | --- |
| SSID | Advertised Wi-Fi network name |
| BSSID | Access-point radio MAC address |
| Channel | Current 2.4 GHz channel |
| RSSI | Observed signal strength |
| Security mode | Advertised authentication mode |
| Open network | Highlights networks advertising no Wi-Fi authentication |
| Duplicate SSID | Marks the same SSID seen from multiple BSSIDs for review |
| Channel/security change | Compares the same BSSID with the previous scan |

A duplicate SSID is **not automatically a rogue access point**. Mesh Wi-Fi, extenders and enterprise deployments commonly advertise the same SSID from several BSSIDs.

## Features

- Passive nearby Wi-Fi scanning
- ESP32-C3 focused firmware
- SSID, BSSID, RSSI and channel inventory
- Advertised security-mode visibility
- Open-network warning
- Duplicate-SSID review flag
- Channel/security change observations between scans
- Responsive local device dashboard
- Automatic scan refresh every 30 seconds
- Manual **Scan now** control
- PlatformIO project structure
- GitHub Actions firmware compilation
- Browser firmware installation with ESP Web Tools
- Downloadable full-flash firmware image

## Browser Installation

For the simplest setup, use **[Open Live Web Installer](https://tanzeel0hussain.github.io/Wi-Fi-Phantom/)**.

1. Connect a supported ESP32-C3 development board using a **data-capable USB cable**.
2. Open the installer in desktop Chrome or Microsoft Edge.
3. Click the install button.
4. Approve the serial device selected in your browser.
5. Wait for flashing to complete and reboot the board.

The website cannot silently access your serial device; browser permission is always required.

## After Installation

1. Power or reboot the ESP32-C3.
2. Connect your phone/laptop to **`Wi-Fi-Phantom-Monitor`**.
3. Use password **`phantomsecure`**.
4. Open **[Local Device Dashboard](http://192.168.4.1)**.
5. Press **Scan now** whenever you want a fresh observation.

The dashboard also refreshes automatically.

## Manual Firmware

The CI pipeline creates a full ESP32-C3 flash image containing the bootloader, partition table, boot application metadata and application firmware.

Use **[Download Latest Firmware](https://github.com/Tanzeel0Hussain/Wi-Fi-Phantom/releases)** if you prefer manual flashing.

## Build from Source

Requirements:

- Python 3
- PlatformIO CLI or PlatformIO IDE
- ESP32-C3 development board compatible with the `esp32-c3-devkitm-1` PlatformIO target

Clone and build:

```bash
git clone https://github.com/Tanzeel0Hussain/Wi-Fi-Phantom.git
cd Wi-Fi-Phantom
pio run -e esp32c3
```

The application binary is produced under:

```text
.pio/build/esp32c3/firmware.bin
```

Upload directly through PlatformIO:

```bash
pio run -e esp32c3 -t upload
```

## Repository Structure

```text
Wi-Fi-Phantom/
├── .github/
│   └── workflows/
│       └── firmware.yml
├── docs/
│   ├── firmware/
│   │   └── wifi-phantom-esp32c3-full.bin
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

## Firmware Architecture

```text
ESP32-C3
   │
   ├── Passive Wi-Fi Scanner
   │      └── SSID / BSSID / RSSI / Channel / Security
   │
   ├── Observation Engine
   │      ├── Open network warning
   │      ├── Duplicate SSID review
   │      └── Channel/security change comparison
   │
   └── Local Dashboard
          └── Wi-Fi-Phantom-Monitor → 192.168.4.1
```

## Automated Firmware Pipeline

Every relevant push to `main` automatically:

1. Installs PlatformIO.
2. Compiles the ESP32-C3 firmware.
3. Creates a full browser-installable flash image.
4. Uploads the binary as a GitHub Actions artifact.
5. Updates the firmware used by the live Web Installer.
6. Publishes or updates the GitHub firmware Release.

## Safety and Interpretation

Wi-Fi Phantom is intended for your own hardware, classroom labs and explicitly authorized wireless observation.

Dashboard flags are **observations, not conclusions**. For example, a duplicate SSID may be expected in a mesh system. Always compare results with your known AP inventory and network design.

See **[Security and Responsible Use](SECURITY.md)** for details.

## USB Driver Note

USB-UART support depends on the board and USB interface. Use the official vendor driver for the specific CP210x, CH340/CH341 or other bridge used by your board rather than storing third-party driver packages inside this repository.

## Roadmap

- Known-network baseline profiles
- Channel distribution summary
- Observation history
- Optional OLED status display
- Additional tested ESP32-C3 board profiles
- Hardware screenshots and wiring notes

## License

Released under the **[MIT License](LICENSE)**.

## Maintainer

**[Tanzeel Hussain](https://github.com/Tanzeel0Hussain)**

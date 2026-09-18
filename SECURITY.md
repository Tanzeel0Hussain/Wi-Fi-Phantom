# Security and Responsible Use

Wi-Fi Phantom is maintained as a **defensive ESP32-C3 wireless visibility project**.

## Current scope

The current main-branch firmware is designed to:

- passively scan nearby Wi-Fi advertisements;
- show SSID, BSSID, channel, RSSI and advertised security mode;
- flag open-network configuration;
- highlight duplicate SSIDs for review;
- compare a BSSID's channel/security mode with the previous scan;
- host a local dashboard on the user's ESP32-C3.

It is not intended to disconnect clients, impersonate third-party access points, or collect Wi-Fi credentials.

## Interpreting observations

A duplicate SSID is not proof of a rogue access point. Mesh Wi-Fi, enterprise deployments and extenders commonly use the same SSID across several BSSIDs. Validate observations against your known inventory and network design.

## Reporting a security issue

Please use the repository's GitHub Issues for non-sensitive bugs. Avoid posting credentials, private network identifiers or other sensitive data in public issues.

## Authorized use

Use this project only with hardware and networks you own or are explicitly authorized to observe.

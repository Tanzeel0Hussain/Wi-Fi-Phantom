# Wi-Fi Phantom v2.0 🛡️💻

**Wi-Fi Phantom v2.0** is a professional-grade WiFi security testing tool developed for the **ESP32** (specifically optimized for **ESP32-C3**). It combines a powerful **Deauthenticator** with a sophisticated **Evil Twin (Captive Portal)** attack to demonstrate WiFi vulnerabilities for educational and security auditing purposes.

---

## ⚡ Features

### 🖥️ Admin Dashboard (Attacker Side)
- **Hacker Aesthetic**: A sleek, Cyberpunk-themed management interface.
- **Real-time Scanner**: Scans all nearby 2.4GHz networks.
- **Signal Indicators**: Visual RSSI signal bars (▂▄▆█) for accurate targeting.
- **Mobile Responsive**: Fully optimized for smartphones and tablets.

### 🌐 User Portal (Victim Side)
- **Professional Design**: A modern, clean "Wireless Gateway" login page that mimics real router admin panels.
- **Credential Harvesting**: Securely captures and stores WiFi passwords.
- **Automatic Redirect**: Uses a DNS Captive Portal to redirect users to the login page automatically.

### ⚔️ Deauther Logic
- **High-Frequency Bursts**: Sends Deauth and Disassociation frames every 100ms.
- **Burst Mode**: Fires multiple packets per cycle to maximize the success rate.
- **Multi-Interface Injection**: Uses the ESP32 Station interface for reliable frame injection.

---

## 🚀 Getting Started

### Prerequisites
- **Hardware**: ESP32-C3 (or any ESP32 series board).
- **Drivers**: If your PC doesn't detect the ESP32 port, install the provided drivers.
- **Software**: Arduino IDE (Download from [arduino.cc](https://www.arduino.cc/en/software)).

### 1. Windows Driver Installation
If you are using Windows and your board's COM port is not detected:
1. Locate `CP210x_Windows_Drivers.zip` in this folder.
2. Extract the ZIP file.
3. Run `CP210xVCPInstaller_x64.exe` (for 64-bit Windows) or `CP210xVCPInstaller_x86.exe` (for 32-bit).
4. Follow the installation wizard. Once finished, reconnect your ESP32.

### 2. Arduino IDE Configuration
1. Open **Arduino IDE**.
2. Go to **File > Preferences**.
3. In the "Additional Boards Manager URLs" field, paste this link:
   `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
4. Click **OK**.
5. Go to **Tools > Board > Boards Manager...**
6. Search for `esp32` and click **Install** for the "esp32 by Espressif Systems" package.
7. After installation, go to **Tools > Board > esp32** and select **ESP32C3 Dev Module** (or the specific board you are using).
8. Select the correct **Port** under **Tools > Port**.

### 3. Installation
1. Clone or download this repository.
2. Open `Wi-Fi-Phantom.ino` in Arduino IDE.
3. Click the **Upload** button (arrow icon) to flash the firmware to your ESP32.

---

## 🛠️ Usage

1. **Power on** your ESP32.
2. **Connect** to the management WiFi:
   - **SSID**: `TP-Link_3473`
   - **Password**: `Password`
3. **Open Browser**: Go to `192.168.4.1` or `http://start.com`.
4. **Select Target**: Scan for nearby networks and select your target AP.
5. **Execute**:
   - Start **Deauther** to disconnect users from the original network.
   - Start **Evil Twin** to launch the fake access point.
6. **Capture**: Wait for users to enter their credentials on the professional-looking portal.

---

## ⚠️ Important Note on PMF (802.11w)

If the target router uses **Protected Management Frames (PMF)** (common in WPA3 and modern WPA2 routers), the deauthentication attack may fail as management frames are encrypted. This tool is designed to demonstrate vulnerabilities on non-PMF or legacy networks.

---

## 📜 Disclaimer

**For Educational Purposes Only.**
The use of this tool for attacking networks without prior mutual consent is illegal. It is the end user's responsibility to obey all applicable local, state, and federal laws. Developers assume no liability and are not responsible for any misuse or damage caused by this program.

---

## 🤝 Contributing

Contributions, issues, and feature requests are welcome! Feel free to check the [issues page](https://github.com/Tanzeel0Hussain/Wi-Fi-Phantom/issues).

---

## ⭐️ Support

If you found this project helpful, please give it a **Star** on GitHub!

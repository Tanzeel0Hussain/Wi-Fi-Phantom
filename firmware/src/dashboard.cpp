#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>
#include "dashboard.h"
#include "scanner.h"

namespace {
WebServer server(80);

String escapeHtml(String value) {
  value.replace("&", "&amp;");
  value.replace("<", "&lt;");
  value.replace(">", "&gt;");
  value.replace(""", "&quot;");
  value.replace("'", "&#39;");
  return value;
}

String flagBadges(const NetworkObservation& item) {
  String badges;
  if (item.openNetwork) badges += "<span class='tag danger'>Open</span>";
  if (item.duplicateSsid) badges += "<span class='tag warn'>Duplicate SSID</span>";
  if (item.channelChanged) badges += "<span class='tag info'>Channel changed</span>";
  if (item.securityChanged) badges += "<span class='tag info'>Security changed</span>";
  if (badges.length() == 0) badges = "<span class='tag ok'>Observed</span>";
  return badges;
}

String renderPage() {
  const ScanSummary& summary = getScanSummary();
  String html;
  html.reserve(20000);

  html += R"HTML(<!doctype html><html><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<meta http-equiv="refresh" content="30">
<title>Wi-Fi Phantom Monitor</title>
<style>
:root{--bg:#071019;--panel:#0e1b28;--line:#203447;--text:#edf6ff;--muted:#8ea6ba;--cyan:#35d5e8;--green:#36d399;--amber:#f5b942;--red:#fb7185;--violet:#a78bfa}
*{box-sizing:border-box}body{margin:0;background:radial-gradient(circle at top,#10293c,#071019 48%);color:var(--text);font-family:Inter,system-ui,-apple-system,Segoe UI,sans-serif}
.wrap{max-width:1180px;margin:auto;padding:24px 16px 60px}.top{display:flex;justify-content:space-between;gap:16px;align-items:center;margin-bottom:18px}.brand b{font-size:1.15rem}.brand span{display:block;color:var(--muted);font-size:.78rem;margin-top:3px}
.btn{display:inline-block;background:var(--cyan);color:#041117;text-decoration:none;font-weight:800;border-radius:11px;padding:10px 14px}
.grid{display:grid;grid-template-columns:repeat(4,1fr);gap:12px}.card{background:rgba(14,27,40,.95);border:1px solid var(--line);border-radius:16px;padding:18px;box-shadow:0 16px 40px rgba(0,0,0,.17)}
.k{color:var(--muted);font-size:.72rem;text-transform:uppercase;letter-spacing:.08em}.v{font-size:1.8rem;font-weight:900;margin-top:6px}.section{margin-top:14px}.section h2{font-size:1rem;margin:0 0 12px}
.table-wrap{overflow:auto}.table{width:100%;border-collapse:collapse;min-width:780px}.table th,.table td{padding:11px 9px;border-bottom:1px solid var(--line);text-align:left;font-size:.78rem}.table th{color:var(--muted);font-size:.69rem;text-transform:uppercase;letter-spacing:.05em}
.rssi{font-variant-numeric:tabular-nums}.tag{display:inline-block;border-radius:999px;padding:4px 7px;margin:2px;font-size:.65rem;font-weight:800}.ok{background:#103b31;color:#72e5be}.warn{background:#493512;color:#ffd978}.danger{background:#481a25;color:#ff9aaa}.info{background:#302552;color:#c7b9ff}
.note{color:var(--muted);font-size:.76rem;line-height:1.6}.device{display:grid;grid-template-columns:repeat(4,1fr);gap:12px}.device div{background:#0a1621;border:1px solid var(--line);border-radius:12px;padding:13px}.device b{display:block;margin-top:4px}
@media(max-width:760px){.grid,.device{grid-template-columns:1fr 1fr}.top{align-items:flex-start;flex-direction:column}}@media(max-width:430px){.grid,.device{grid-template-columns:1fr}}
</style></head><body><main class="wrap">
<div class="top"><div class="brand"><b>Wi-Fi Phantom · ESP32-C3 Monitor</b><span>Passive wireless visibility and review-oriented observations</span></div><a class="btn" href="/scan">Scan now</a></div>)HTML";

  html += "<section class='grid'>";
  html += "<div class='card'><div class='k'>Networks</div><div class='v'>" + String(summary.networks) + "</div></div>";
  html += "<div class='card'><div class='k'>Open</div><div class='v'>" + String(summary.openNetworks) + "</div></div>";
  html += "<div class='card'><div class='k'>Duplicate SSIDs</div><div class='v'>" + String(summary.duplicateSsids) + "</div></div>";
  html += "<div class='card'><div class='k'>Changed since scan</div><div class='v'>" + String(summary.changedNetworks) + "</div></div></section>";

  html += "<section class='card section'><h2>Nearby networks</h2><div class='table-wrap'><table class='table'><thead><tr><th>SSID</th><th>BSSID</th><th>CH</th><th>RSSI</th><th>Security</th><th>Observation</th></tr></thead><tbody>";
  for (size_t i = 0; i < getObservationCount(); ++i) {
    const NetworkObservation& item = getObservation(i);
    const String ssid = item.ssid.length() ? escapeHtml(item.ssid) : String("&lt;hidden&gt;");
    html += "<tr><td><b>" + ssid + "</b></td><td>" + escapeHtml(item.bssid) + "</td><td>" + String(item.channel) + "</td><td class='rssi'>" + String(item.rssi) + " dBm</td><td>" + authModeLabel(item.auth) + "</td><td>" + flagBadges(item) + "</td></tr>";
  }
  if (getObservationCount() == 0) {
    html += "<tr><td colspan='6'>No networks observed yet. Press Scan now.</td></tr>";
  }
  html += "</tbody></table></div><p class='note'><b>Important:</b> a duplicate SSID is not automatically a rogue access point. Mesh systems, extenders and enterprise deployments commonly advertise the same SSID from multiple BSSIDs. Review the BSSID, channel, security mode and your known network inventory before drawing conclusions.</p></section>";

  html += "<section class='card section'><h2>Device</h2><div class='device'>";
  html += "<div><span class='k'>Chip</span><b>" + String(ESP.getChipModel()) + "</b></div>";
  html += "<div><span class='k'>Free heap</span><b>" + String(ESP.getFreeHeap() / 1024) + " KB</b></div>";
  html += "<div><span class='k'>Monitor AP</span><b>Wi-Fi-Phantom-Monitor</b></div>";
  html += "<div><span class='k'>Dashboard</span><b>192.168.4.1</b></div>";
  html += "</div></section>";

  html += "<p class='note section'>This firmware observes advertised Wi-Fi metadata only. It does not disconnect clients, impersonate third-party access points, or collect Wi-Fi passwords.</p></main></body></html>";
  return html;
}
}

void beginDashboard() {
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html; charset=utf-8", renderPage());
  });

  server.on("/scan", HTTP_GET, []() {
    performNetworkScan();
    server.sendHeader("Location", "/");
    server.send(303);
  });

  server.on("/health", HTTP_GET, []() {
    server.send(200, "application/json", "{\"status\":\"ok\",\"mode\":\"defensive-monitor\"}");
  });

  server.begin();
}

void handleDashboardClient() {
  server.handleClient();
}

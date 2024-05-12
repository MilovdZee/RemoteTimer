void setup_wifi() {
  Settings settings = get_settings();
  if (settings.ssid[0] != 0) {
    Serial.printf("Connecting to WiFi '%s': .", settings.ssid);
    WiFi.begin(settings.ssid, settings.wifi_password);
    int max_delay = 30;
    while (WiFi.status() != WL_CONNECTED && max_delay-- > 0) {
      // Update screen
      Serial.print('.');

      char buffer[BUFFER_SIZE];
      snprintf(buffer, sizeof(buffer), "C%3d", max_delay);
      show_string(buffer);

      delay(1000);
    }
    Serial.println();
  }

  String ipAddress;
  if (WiFi.status() != WL_CONNECTED) {
    Serial.printf("Failed to connect to WiFi '%s', starting AP mode.\n", settings.ssid);
    WiFi.mode(WIFI_AP);
    WiFi.softAP(OTA_HOSTNAME);
    ipAddress = WiFi.softAPIP().toString();
  } else {
    ipAddress = WiFi.localIP().toString();
    Serial.printf("Connected to WiFi '%s': %s\n", settings.ssid, ipAddress);
  }
  display.startStringScroll(ipAddress.c_str(), 300);
  while (display.Animate()) delay(10);
}

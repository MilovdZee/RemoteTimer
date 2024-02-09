void setup_wifi() {
  Settings settings = get_settings();
  if (settings.ssid[0] == 0) return;

  Serial.printf("Connecting to WiFi '%s': .", settings.ssid);
  WiFi.begin(settings.ssid, settings.wifi_password);
  int max_delay = 20;
  while (WiFi.status() != WL_CONNECTED && max_delay-- > 0) {
    // Update screen
    Serial.print('.');
    delay(1000);
  }
  Serial.println();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.printf("Failed to connect to WiFi '%s'\n", settings.ssid);
  } else {
    String ipAddress = WiFi.localIP().toString();
    Serial.printf("Connected to WiFi '%s': %s\n", settings.ssid, ipAddress);
  }
}

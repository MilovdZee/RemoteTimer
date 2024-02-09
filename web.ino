void handle_root() {
  String rootForm = String(CSS) + "<html>\
  <body>\
    <div class=\"container\">\
      <h1>Settings</h1>\
      <div><a href=\"/wifi\">wifi</a></div>\
    </div>\
  </body>\
</html>";

  server.send(200, "text/html", rootForm);
}

void handle_wifi() {
  Settings settings = get_settings();

  if (server.method() == HTTP_POST) {
    char ssid[60];
    char wifi_password[60];
    snprintf(ssid, sizeof(ssid), server.arg("ssid").c_str());
    snprintf(wifi_password, sizeof(wifi_password), server.arg("password").c_str());

    Serial.printf("Settings:\n");
    Serial.printf("  - ssid                 : '%s'\n", ssid);
    Serial.printf("  - wifi_password        : '%s'\n", wifi_password);

    Settings updated_settings = Settings(ssid, wifi_password, settings.button_code, settings.step_size, settings.max_duration);
    write_settings(updated_settings);

    delay(500);
    ESP.restart();
  }

  // Read back to check if the values are stored correctly
  String wifiForm = String(CSS) + "<html>\
  <body>\
    <div class=\"container\">\
    <h1>WiFi Settings</h1>\
      <form method=\"POST\" action=\"/wifi\">\
        SSID:</br>\
        <input type=\"text\" name=\"ssid\" value=\""
                    + String(settings.ssid) + "\"></br></br>\
        Password:</br>\
        <input type=\"text\" name=\"password\" value=\""
                    + String(settings.wifi_password) + "\">\
        <input type=\"submit\" value=\"Submit\">\
      </form>\
    </div>\
  </body>\
</html>";

  server.send(200, "text/html", wifiForm);
}

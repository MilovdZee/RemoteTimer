void handle_root() {
  bool do_reset = false;
  Settings settings = get_settings();

  if (server.method() == HTTP_POST) {
    unsigned int step_size = server.arg("step_size").toInt();
    unsigned int max_duration = server.arg("max_duration").toInt();
    unsigned short brightness = server.arg("brightness").toInt();

    Settings updated_settings = Settings(settings.ssid, settings.wifi_password, settings.button_code, step_size, max_duration, brightness);
    write_settings(updated_settings);
    settings = get_settings();

    do_reset = true;
  }

  String rootForm = String(CSS) + "<html>\
  <body>\
    <div class=\"container\">\
      <h1>Settings</h1>\
      <form method=\"POST\" action=\"/\">\
        Brightness:</br>\
        <input type=\"range\" name=\"brightness\" min=\"0\" max=\"7\" value=\""
                    + String(settings.brightness) + "\">\
        Max duration:<br/>\
        <input type=\"text\" name=\"max_duration\" value=\""
                    + String(settings.max_duration) + "\"><br/><br/>\
        Step size:<br/>\
        <input type=\"text\" name=\"step_size\" value=\""
                    + String(settings.step_size) + "\"><br/>\
        </br>\
        <input type=\"submit\" value=\"Submit\">\
      </form>\
      <div><a href=\"/wifi\">wifi</a></div>\
    </div>\
  </body>\
</html>";

  server.send(200, "text/html", rootForm);

  if (do_reset) {
    handling_delay(500);
    ESP.restart();
  }
}

void handle_wifi() {
  bool do_reset = false;
  Settings settings = get_settings();

  if (server.method() == HTTP_POST) {
    char ssid[60];
    char wifi_password[60];
    snprintf(ssid, sizeof(ssid), server.arg("ssid").c_str());
    snprintf(wifi_password, sizeof(wifi_password), server.arg("password").c_str());

    Settings updated_settings = Settings(ssid, wifi_password, settings.button_code, settings.step_size, settings.max_duration, settings.brightness);
    write_settings(updated_settings);
    settings = get_settings();

    do_reset = true;
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

  if (do_reset) {
    handling_delay(500);
    ESP.restart();
  }
}

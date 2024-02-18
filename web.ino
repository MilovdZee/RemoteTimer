void handle_root() {
  bool do_reset = false;
  Settings settings = get_settings();

  if (server.method() == HTTP_POST) {
    unsigned int step_size = server.arg("step_size").toInt();
    unsigned int max_duration = server.arg("max_duration").toInt();
    unsigned short brightness = server.arg("brightness").toInt();

    settings.step_size = step_size;
    settings.max_duration = max_duration;
    settings.brightness = brightness;
    snprintf(settings.on_time, sizeof(settings.on_time), server.arg("on_time").c_str());
    snprintf(settings.off_time, sizeof(settings.off_time), server.arg("off_time").c_str());
    write_settings(settings);

    do_reset = true;
  }

  String rootForm = String(css) + "<html>\
  <body>\
    <div class=\"container\">\
      <h1>Settings</h1>\
      <form method=\"POST\" action=\"/\">\
        <div>\
          <div>Brightness:</div>\
          <div><input type=\"range\" name=\"brightness\" min=\"0\" max=\"7\" value=\"" + String(settings.brightness) + "\"></div>\
        </div>\
        \
        <div>\
          <div>Max duration:</div>\
          <div><input type=\"text\" name=\"max_duration\" value=\"" + String(settings.max_duration) + "\"></div>\
        </div>\
        \
        <div>\
          <div>Step size:</div>\
          <div><input type=\"text\" name=\"step_size\" value=\"" + String(settings.step_size) + "\"></div>\
        </div>\
        \
        <div>\
          <div>Light On/Off times:</div>\
          <div>\
            <input type=\"text\" name=\"on_time\" maxlength=\"5\" value=\"" + String(settings.on_time) + "\">\
            &nbsp;-&nbsp;\
            <input type=\"text\" name=\"off_time\" maxlength=\"5\" value=\"" + String(settings.off_time) + "\">\
          </div>\
        </div>\
        \
        </br>\
        <input type=\"submit\" value=\"Submit\">\
      </form>\
      <div class=\"wifi_link\"><a href=\"/wifi\">wifi</a></div>\
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
    snprintf(settings.ssid, sizeof(settings.ssid), server.arg("ssid").c_str());
    snprintf(settings.wifi_password, sizeof(settings.wifi_password), server.arg("password").c_str());
    write_settings(settings);

    do_reset = true;
  }

  // Read back to check if the values are stored correctly
  String wifiForm = String(css) + "<html>\
  <body>\
    <div class=\"container\">\
    <h1>WiFi Settings</h1>\
      <form method=\"POST\" action=\"/wifi\">\
        SSID:</br>\
        <input type=\"text\" name=\"ssid\" value=\"" + String(settings.ssid) + "\"></br></br>\
        Password:</br>\
        <input type=\"text\" name=\"password\" value=\"" + String(settings.wifi_password) + "\">\
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

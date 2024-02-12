#define CHECK_ADDR 0                                                   // char[10]
#define SSID_ADDR (CHECK_ADDR + sizeof(eeprom_check))                  // char[60]
#define WIFI_PASSWORD_ADDR (SSID_ADDR + sizeof(ssid))                  // char[60]
#define BUTTON_CODE_ADDR (WIFI_PASSWORD_ADDR + sizeof(wifi_password))  // long
#define STEP_SIZE_ADDR (BUTTON_CODE_ADDR + sizeof(button_code))        // int
#define MAX_DURATION_ADDR (STEP_SIZE_ADDR + sizeof(step_size))         // int
#define BRIGHTNESS_ADDR (MAX_DURATION_ADDR + sizeof(max_duration))     // short

Preferences preferences;

Settings settings;

void read_settings() {
  // For the ESP the flash has to be read to a buffer
  preferences.begin("RemoteTimer", false);

  int settings_size = preferences.getBytesLength("settings");
  Serial.printf("'settings' data structure has size: %d\n", settings_size);

  if (!preferences.getBytes("settings", &settings, sizeof(settings))) {
    Serial.println("ERROR: Failed to read preferences from EEPROM!");
    return;
  }

  Serial.printf("settings.eeprom_check: '%s'\n", settings.eeprom_check);
  if (check_validity(settings)) {
    Serial.println("EEPROM data found");
  } else {
    Serial.println("EEPROM data NOT found");
    settings = Settings();
  }
  show_settings(settings);
}

void write_settings(Settings newSettings) {
  preferences.begin("RemoteTimer", false);
  if (!preferences.putBytes("settings", &newSettings, sizeof(newSettings))) {
    Serial.println("ERROR: Failed to write preferences to EEPROM!");
    return;
  }
  show_settings(newSettings);
  settings = newSettings;
}

boolean check_validity(Settings settings) {
  return strncmp(settings.eeprom_check, EEPROM_CHECK_STRING, sizeof(EEPROM_CHECK_STRING)) == 0;
}

Settings get_settings() {
  return settings;
}

void show_settings(Settings settings) {
  Serial.printf("\nSettings:\n");
  Serial.printf("  - ssid                 : '%s'\n", settings.ssid);
  Serial.printf("  - wifi_password        : '%s'\n", settings.wifi_password);
  Serial.printf("  - button_code          : '%ld'\n", settings.button_code);
  Serial.printf("  - step_size            : '%d'\n", settings.step_size);
  Serial.printf("  - max_duration         : '%d'\n", settings.max_duration);
  Serial.printf("  - brightness           : '%d'\n", settings.brightness);
}

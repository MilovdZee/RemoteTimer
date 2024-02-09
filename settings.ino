#define CHECK_ADDR 0                                                         // char[10]
#define SSID_ADDR (CHECK_ADDR + sizeof(eeprom_check))                        // char[60]
#define WIFI_PASSWORD_ADDR (SSID_ADDR + sizeof(ssid))                        // char[60]
#define BUTTON_CODE_ADDR (WIFI_PASSWORD_ADDR + sizeof(wifi_password))        // long
#define STEP_SIZE_ADDR (BUTTON_CODE_ADDR + sizeof(button_code))              // int
#define MAX_DURATION_ADDR (STEP_SIZE_ADDR + sizeof(step_size))               // int

// MACRO : Combines FROM_BYTES into TO_OBJECT of datatype "DATATYPE"
#define combine_bytes(FROM_BYTES, DATATYPE, TO_OBJECT) TO_OBJECT = *((DATATYPE *)FROM_BYTES);

// MACRO : Splits FROM_DATA of DATATYPE into equivalent byte array TO_BYTES
#define split_bytes(FROM_DATA, DATATYPE, TO_BYTES) *((DATATYPE *)TO_BYTES) = FROM_DATA;

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
}

void write_settings(Settings newSettings) {
  preferences.begin("RemoteTimer", false);
  if (!preferences.putBytes("settings", &newSettings, sizeof(newSettings))) {
    Serial.println("ERROR: Failed to write preferences to EEPROM!");
    return;
  }
  settings = newSettings;
}

boolean check_validity(Settings settings) {
  return strncmp(settings.eeprom_check, EEPROM_CHECK_STRING, sizeof(EEPROM_CHECK_STRING)) == 0;
}

Settings get_settings() {
  return settings;
}

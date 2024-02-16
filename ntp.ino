void timeUpdated() {
  time_t now = time(nullptr);
  Serial.printf("\nNTP Updated: %s\n", ctime(&now));
}

void setup_ntp() {
  // implement NTP update of timekeeping (with automatic hourly updates)
  configTime(0, 0, NTP_SERVER);
  setenv("TZ", MY_TZ, 1);
  tzset();

  settimeofday_cb(timeUpdated);

  // Wait for time sync to come in
  Serial.print("Waiting for NTP time sync: ");
  int max_wait = 60;  // 30 seconds
  time_t now = time(nullptr);
  while (max_wait-- > 0 && now < 8 * 3600 * 2) {
    Serial.print(".");
    delay(500);
    now = time(nullptr);
  }
}
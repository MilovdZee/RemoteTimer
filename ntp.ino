void setup_ntp() {
  // implement NTP update of timekeeping (with automatic hourly updates)
  configTime(0, 0, NTP_SERVER);
  setenv("TZ", MY_TZ, 1);
  tzset();

  // Wait for time sync to come in
  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");

  struct tm *timeinfo = localtime(&now);
  Serial.printf("Current time: %s\n", asctime(timeinfo));
}
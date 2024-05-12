void switch_light(bool state) {
  Serial.println("Switching light");
  digitalWrite(LED1, state ? LOW : HIGH);
  digitalWrite(LED2, state ? LOW : HIGH);
}

void check_night_light() {
  Settings settings = get_settings();
  int on_second = str_to_time(settings.on_time);
  int off_second = str_to_time(settings.off_time);
  int now = get_second_of_day();

  if ((on_second < off_second && now > on_second && now < off_second) || (on_second > off_second && (now > on_second || now < off_second))) {
    switch_light(true);
  } else {
    switch_light(false);
  }
}

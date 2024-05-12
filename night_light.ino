void switch_light_on() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
}

void switch_light_off() {
  pinMode(LED1, SPECIAL);
  pinMode(LED2, SPECIAL);
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
}

void check_night_light() {
  Settings settings = get_settings();
  int on_second = str_to_time(settings.on_time);
  int off_second = str_to_time(settings.off_time);
  int now = get_second_of_day();

  if ((on_second < off_second && now > on_second && now < off_second) || (on_second > off_second && (now > on_second || now < off_second))) {
    switch_light_on();
  } else {
    switch_light_off();
  }
}

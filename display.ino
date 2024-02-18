display_state state = SHOWING;

void setup_display() {
  Settings settings = get_settings();
  Serial.printf("Setting brightness to: %d\n", settings.brightness);
  display.setBrightness(settings.brightness);

  state = SHOWING;
}

void start_waiting_animation() {
  if (state != WAITING) {
    Serial.println("Starting waiting animation");

    display.clear();
    display.setBrightness(0);
    display.startAnimation(WAITING_ANIMATION, FRAMES(WAITING_ANIMATION), TIME_MS(1000));

    state = WAITING;
  }
}

void start_rotate_animation() {
  if (state != ANIMATING_ROTATE) {
    display.setBrightness(get_settings().brightness);
    display.startAnimation(ROTATE_ANIMATION, FRAMES(ROTATE_ANIMATION), TIME_MS(50));

    state = ANIMATING_ROTATE;
  }
}

void switch_light_on() {
  if (state != LIGHT_ON) {
    Serial.println("Switching light on");
    show_number(8888);

    state = LIGHT_ON;
  }
}

void show_number(int number) {
  display.stopAnimation();
  display.setBrightness(get_settings().brightness);
  display.showNumber(number);

  state = SHOWING;
}

void show_string(const char* string) {
  display.stopAnimation();
  display.setBrightness(get_settings().brightness);
  display.showString(string);

  state = SHOWING;
}

void check_waiting() {
  if (state == WAITING || state == LIGHT_ON) {
    Settings settings = get_settings();
    int on_second = str_to_time(settings.on_time);
    int off_second = str_to_time(settings.off_time);
    int now = get_second_of_day();

    if ((on_second < off_second && now > on_second && now < off_second) || (on_second > off_second && (now > on_second || now < off_second))) {
      switch_light_on();
    } else {
      start_waiting_animation();
    }
  }
}
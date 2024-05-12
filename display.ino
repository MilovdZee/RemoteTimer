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


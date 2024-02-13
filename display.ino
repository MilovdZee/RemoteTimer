void setup_display() {
    Settings settings = get_settings();
    Serial.printf("Setting brightness to: %d\n", settings.brightness);
    display.setBrightness(settings.brightness);
}

void start_waiting_animation() {
  display.clear();
  display.setBrightness(0);
  display.startAnimation(WAITING_ANIMATION, FRAMES(WAITING_ANIMATION), TIME_MS(1000));
}

void show_number(int number) {
  display.stopAnimation();
  display.setBrightness(get_settings().brightness);
  display.showNumber(number);
}

void show_string(const char* string) {
  display.stopAnimation();
  display.setBrightness(get_settings().brightness);
  display.showString(string);
}
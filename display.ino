void setup_display() {
    Settings settings = get_settings();
    Serial.printf("Setting brightness to: %d\n", settings.brightness);
    display.setBrightness(settings.brightness);
}
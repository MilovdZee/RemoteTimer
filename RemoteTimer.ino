#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <RCSwitch.h>
#include <Preferences.h>
#include <TM1637TinyDisplay.h>
#include <WiFiClientSecure.h>
#include <coredecls.h> // required for settimeofday_cb() (NTP sync callback)

#include "config.h"

ESP8266WebServer server(80);
RCSwitch remoteSwitch = RCSwitch();
TM1637TinyDisplay display(DISPLAY_CLK, DISPLAY_DIO);

void setup() {
  Serial.begin(115200);

  delay(2000);

  Serial.println();
  Serial.println("Starting setup...");

  pinMode(SWITCH, INPUT_PULLUP);
  digitalWrite(RELAIS, LOW);
  pinMode(RELAIS, OUTPUT);

  read_settings();
  setup_display();
  setup_wifi();
  setup_ota();
  setup_ntp();

  check_for_updates();

  // Setup the web server
  server.on("/", handle_root);
  server.on("/wifi", handle_wifi);
  server.begin();

  // Start listening to the 433MHz receiver
  int interrupt = digitalPinToInterrupt(RECEIVE_PIN);
  Serial.printf("Linking interrupt %d to pin %d\n", interrupt, RECEIVE_PIN);
  remoteSwitch.enableReceive(interrupt);

  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(LED_BUILTIN, OUTPUT);

  start_waiting_animation();

  Serial.println("Finished setup");
}

void handling_delay(int ms) {
  while (ms-- > 0) {
    delay(1);
    handler();
  }
}

bool store_next_received_switch_code = false;
unsigned long switch_start_millis = 0;
int enabled_minutes_to_go = 0;
unsigned long last_minute_tick_millis = 0;

void start_button_code_listening() {
  Serial.println("Start listining for button code...");
  store_next_received_switch_code = true;
  display.setBrightness(get_settings().brightness);
  display.startAnimation(ROTATE_ANIMATION, FRAMES(ROTATE_ANIMATION), TIME_MS(50));
}

void stop_button_code_listening() {
  switch_start_millis = 0;
  store_next_received_switch_code = false;
  start_waiting_animation();
}

void increase_minutes_to_go() {
  if (enabled_minutes_to_go == 0) last_minute_tick_millis = millis();
  enabled_minutes_to_go += get_settings().step_size;
  if (enabled_minutes_to_go > get_settings().max_duration) {
    enabled_minutes_to_go = 0;
    digitalWrite(RELAIS, LOW);
    start_waiting_animation();
  } else {
    digitalWrite(RELAIS, HIGH);
    show_number(enabled_minutes_to_go);
  }
}

void update_minutes_to_go() {
  if (enabled_minutes_to_go > 0) {
    if (millis() - last_minute_tick_millis >= 60 * 1000L || millis() < last_minute_tick_millis) {
      last_minute_tick_millis = millis();
      enabled_minutes_to_go--;
      if (enabled_minutes_to_go == 0) {
        digitalWrite(RELAIS, LOW);
        start_waiting_animation();
      } else {
        show_number(enabled_minutes_to_go);
      }
    }
  }
}

void handler() {
  ArduinoOTA.handle();
  server.handleClient();
  display.Animate(true);
  update_minutes_to_go();
}

void loop() {
  handler();

  static time_t last_check_time = -1;
  time_t now = time(nullptr);
  if (now != last_check_time && now % (3600 * 24) == 0) {
    // run once a day
    last_check_time = now;
    check_for_updates();
  }

  if (remoteSwitch.available()) {
    unsigned long button_code = remoteSwitch.getReceivedValue();
    Serial.printf("value: %ld, bit length: %d, delay: %d, protocol: %d\n",
                  button_code,
                  remoteSwitch.getReceivedBitlength(),
                  remoteSwitch.getReceivedDelay(),
                  remoteSwitch.getReceivedProtocol());

    if (store_next_received_switch_code) {
      stop_button_code_listening();

      Serial.printf("Storing button code: %ld\n", button_code);
      Settings settings = get_settings();
      settings.button_code = button_code;
      write_settings(settings);
    }

    if (button_code == get_settings().button_code) {
      Serial.println("Remote button code received");
      increase_minutes_to_go();
    }

    digitalWrite(LED_BUILTIN, LOW);
    handling_delay(200);
    digitalWrite(LED_BUILTIN, HIGH);
    remoteSwitch.resetAvailable();
  }

  if (digitalRead(SWITCH) == 0 && !store_next_received_switch_code) {
    // Start of pressing the button
    if (switch_start_millis == 0) switch_start_millis = millis();

    // Ok, button was pressed long enough
    if (millis() - switch_start_millis > LONG_PRESS_PERIOD) {
      start_button_code_listening();
    }

    // debounce
    handling_delay(10);
  }

  // Listening for button code timed out
  if (store_next_received_switch_code && millis() - switch_start_millis > BUTTON_CODE_LISTENING_TIMEOUT) {
    Serial.println("Waiting for button code timed out");
    stop_button_code_listening();
  }

  if (digitalRead(SWITCH) == 1 && switch_start_millis != 0 && !store_next_received_switch_code) {
    Serial.println("Short button press");
    switch_start_millis = 0;
    increase_minutes_to_go();
  }
}

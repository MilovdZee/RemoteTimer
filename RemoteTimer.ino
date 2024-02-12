#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <RCSwitch.h>
#include <Preferences.h>
#include <TM1637TinyDisplay.h>

#include "config.h"

ESP8266WebServer server(80);
RCSwitch remoteSwitch = RCSwitch();
TM1637TinyDisplay display(DISPLAY_CLK, DISPLAY_DIO);

void setup() {
  Serial.begin(115200);

  delay(2000);

  Serial.println();
  Serial.println("Starting setup...");

  read_settings();
  setup_display();
  setup_wifi();
  setup_ota();

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

  Serial.println("Finished setup");
}

void handling_delay(int ms) {
  while (ms-- > 0) {
    delay(1);
    ArduinoOTA.handle();
    server.handleClient();
    display.Animate();
  }
}

int count = 0;
void loop() {
  ArduinoOTA.handle();
  server.handleClient();
  display.Animate();

  if (remoteSwitch.available()) {
    Serial.printf("value: %ld, bit length: %d, delay: %d, protocol: %d\n",
                  remoteSwitch.getReceivedValue(),
                  remoteSwitch.getReceivedBitlength(),
                  remoteSwitch.getReceivedDelay(),
                  remoteSwitch.getReceivedProtocol());
    remoteSwitch.resetAvailable();

    digitalWrite(LED_BUILTIN, LOW);
    handling_delay(50);
    digitalWrite(LED_BUILTIN, HIGH);
  }

  display.showNumber(count++);
  handling_delay(200);
}

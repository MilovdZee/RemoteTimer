#ifndef CONFIG_H
#define CONFIG_H

#define EEPROM_CHECK_STRING "EEPROM001"
#define CURRENT_FIRMWARE_VERSION 1

#define OTA_PASSWORD "RemoteTimerPassword"
#define OTA_HOSTNAME "RemoteTimer"

#define RECEIVE_PIN D2
#define DISPLAY_CLK D5
#define DISPLAY_DIO D0

#define BUFFER_SIZE 150
#define ERROR_VALUE -1

class Settings {
public:
  char eeprom_check[10];
  char ssid[60];
  char wifi_password[60];
  unsigned long button_code;
  unsigned int step_size;
  unsigned int max_duration;
  unsigned short brightness;

  Settings() {
    eeprom_check[0] = 0;
    ssid[0] = 0;
    wifi_password[0] = 0;
    button_code = 0;
    step_size = 30;
    max_duration = 120;
    brightness = 5;
  };

  Settings(const char *ssid, const char *wifi_password, long button_code, int step_size, int max_duration, int brightness) {
    strncpy(this->eeprom_check, EEPROM_CHECK_STRING, sizeof(this->eeprom_check));
    strncpy(this->ssid, ssid, sizeof(this->ssid));
    strncpy(this->wifi_password, wifi_password, sizeof(this->wifi_password));
    this->button_code = button_code;
    this->step_size = step_size;
    this->max_duration = max_duration;
    this->brightness = brightness;
  };
};

#define CSS "<style>\
  body {\
    background: #fad7a0;\
    color: #154360;\
    padding: 20px;\
    font-size: 3em;\
    text-align: center;\
  }\
  div.container {\
    display: inline-block;\
    width: 90%;\
    height: 90%;\
    background: #f8c471;\
    box-shadow: 15px 20px 20px #88888888;\
    border-radius: 20px;\
    padding: 2%;\
    text-align: left;\
  }\
  h1 {\
    margin-top: 0;\
  }\
  input {\
    width: 100%;\
    border: 0;\
    border-bottom: 2px solid grey;\
    background: none;\
    color: #154360;\
    font-size: 1.2em;\
  }\
  input[type=\"range\"] {\
    width: 100%;\
  }\
  input[type=\"submit\"] {\
    background: #154360;\
    color: #fad7a0;\
    border: 0;\
    border-radius: 5px;\
    width: 40%;\
    height: 10%;\
    cursor: pointer;\
    font-size: 1em;\
    position: absolute;\
    left: 30%;\
    bottom: 10%;\
  }\
  div div {\
    position: absolute;\
    right: 2%;\
    bottom: 2%;\
    font-size: .6em;\
  }\
</style>"

#endif
#ifndef CONFIG_H
#define CONFIG_H

#define EEPROM_CHECK_STRING "EEPROM001"
#define CURRENT_FIRMWARE_VERSION 5

#define OTA_PASSWORD "RemoteTimerPassword"
#define OTA_HOSTNAME "RemoteTimer"

#define RECEIVE_PIN D2
#define DISPLAY_CLK D5
#define DISPLAY_DIO D0
#define SWITCH D6
#define RELAIS D1

#define BUFFER_SIZE 150
#define ERROR_VALUE -1

#define LONG_PRESS_PERIOD 5000
#define BUTTON_CODE_LISTENING_TIMEOUT 15000

// The timezone to sync the date/time to, using NTP
#define TZ_Europe_Amsterdam "CET-1CEST,M3.5.0,M10.5.0/3"
#define MY_TZ TZ_Europe_Amsterdam
#define NTP_SERVER "pool.ntp.org"

class Settings {
public:
  char eeprom_check[10];
  char ssid[60];
  char wifi_password[60];
  unsigned long button_code;
  unsigned int step_size;
  unsigned int max_duration;
  unsigned short brightness;
  char on_time[6];
  char off_time[6];

  Settings() {
    strncpy(this->eeprom_check, EEPROM_CHECK_STRING, sizeof(this->eeprom_check));
    ssid[0] = 0;
    wifi_password[0] = 0;
    button_code = 0;
    step_size = 30;
    max_duration = 120;
    brightness = 5;
    on_time[0] = 0;
    off_time[0] = 0;
  };
};

enum display_state {WAITING, LIGHT_ON, ANIMATING_ROTATE, SHOWING};

const char css[] PROGMEM = R"=====(
<style>
  body {
    background: #fad7a0;
    color: #154360;
    padding: 20px;
    font-size: 3em;
    text-align: center;
  }
  div.container {
    display: inline-block;
    width: 90%;
    height: 90%;
    background: #f8c471;
    box-shadow: 15px 20px 20px #88888888;
    border-radius: 20px;
    padding: 2%;
    text-align: left;
  }
  h1 {
    margin-top: 0;
  }
  form div {
    display: flex;
    align-items:flex-end;
    width: 100%;
    margin-bottom: 5px;
  }
  form div div {
    width: 9em;
  }
  form div div + div {
    flex-grow: 1;
  }
  input {
    width: 100%;
    border: 0;
    border-bottom: 2px solid grey;
    background: none;
    color: #154360;
    font-size: 1.2em;
  }
  input[type="range"] {
    width: 100%;
  }
  input[type="submit"] {
    background: #154360;
    color: #fad7a0;
    border: 0;
    border-radius: 5px;
    width: 40%;
    height: 10%;
    cursor: pointer;
    font-size: 1em;
    position: absolute;
    left: 30%;
    bottom: 10%;
  }
  .wifi_link {
    position: absolute;
    right: 2%;
    bottom: 2%;
    font-size: .6em;
  }
</style>
)=====";

const uint8_t ROTATE_ANIMATION[12][4] = {
  { 0x01, 0x00, 0x00, 0x00 },  // Frame 0
  { 0x00, 0x01, 0x00, 0x00 },  // Frame 1
  { 0x00, 0x00, 0x01, 0x00 },  // Frame 2
  { 0x00, 0x00, 0x00, 0x01 },  // Frame 3
  { 0x00, 0x00, 0x00, 0x02 },  // Frame 4
  { 0x00, 0x00, 0x00, 0x04 },  // Frame 5
  { 0x00, 0x00, 0x00, 0x08 },  // Frame 6
  { 0x00, 0x00, 0x08, 0x00 },  // Frame 7
  { 0x00, 0x08, 0x00, 0x00 },  // Frame 8
  { 0x08, 0x00, 0x00, 0x00 },  // Frame 9
  { 0x10, 0x00, 0x00, 0x00 },  // Frame 10
  { 0x20, 0x00, 0x00, 0x00 }   // Frame 11
};

const uint8_t WAITING_ANIMATION[12][4] = {
  { 0x40, 0x00, 0x00, 0x00 },  // Frame 0
  { 0x40, 0x40, 0x00, 0x00 },  // Frame 1
  { 0x40, 0x40, 0x40, 0x00 },  // Frame 2
  { 0x40, 0x40, 0x40, 0x40 },  // Frame 3
  { 0x00, 0x40, 0x40, 0x40 },  // Frame 4
  { 0x00, 0x00, 0x40, 0x40 },  // Frame 5
  { 0x00, 0x00, 0x00, 0x40 },  // Frame 6
  { 0x00, 0x00, 0x40, 0x40 },  // Frame 7
  { 0x00, 0x40, 0x40, 0x40 },  // Frame 8
  { 0x40, 0x40, 0x40, 0x40 },  // Frame 9
  { 0x40, 0x40, 0x40, 0x00 },  // Frame 10
  { 0x40, 0x40, 0x00, 0x00 }   // Frame 11
};

const char root_ca[] PROGMEM = R"=====(
-----BEGIN CERTIFICATE-----
MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh
MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3
d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD
QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT
MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j
b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG
9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB
CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97
nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt
43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P
T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4
gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO
BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR
TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw
DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr
hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg
06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF
PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls
YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk
CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=
-----END CERTIFICATE-----
)=====";
#endif
#ifndef CREDENTIALS_H
#define CREDENTIALS_H

// Substitute your values below and rename file as credentials.h

// Fill-in your WiFi details below
constexpr char* WIFI_SSID = "";
constexpr char* WIFI_PASSWORD = "";

// Fill-in details for RF transmitter below
namespace Transmitter {
  constexpr short PROTOCOL = 0;
  constexpr short PULSE_LENGTH = 0;
  constexpr char* SWITCH_ON = ""; // Binary code for turning the switch on
  constexpr char* SWITCH_OFF = "";  // Binary code for turning the switch off
}

#endif

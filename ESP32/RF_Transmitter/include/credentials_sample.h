#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include <string>

// Substitute your values below and rename file as credentials.h

// Fill-in your WiFi details below
const std::string WIFI_SSID = "";
const std::string WIFI_PASSWORD = "";

// Fill-in details for RF transmitter below
namespace Transmitter {
  constexpr short PROTOCOL = 0;
  constexpr short PULSE_LENGTH = 0;
  const std::string SWITCH_ON = ""; // Binary code for turning the switch on
  const std::string SWITCH_OFF = "";  // Binary code for turning the switch off
}

#endif

// Uncomment to enable debugging
#define DEBUG_ON

#include <string>
// Arduino libraries
#include <Arduino.h>
#include <WiFi.h>

// Third party libraries]
#include <fauxmoESP.h> // Set up a web server to interface with Amazon Alexa
#include <RCSwitch.h>   // Control RF devices

// WiFi and RF Credentials
#include "credentials.h"

constexpr int SERIAL_BAUDRATE = 115200;
constexpr short LED_PIN = 2;  // ESP32-WROOM-32 BUILT-IN LED ON GPIO 2
constexpr short SERVER_PORT = 80; // Port for the FauxMo server to listen on

// Transmitter variables
namespace Transmitter {
  constexpr short PIN = 13;  // RF transmitter data on GPIO 13
  const std::string SWITCH_NAME{"test"};  // Name for the device in Alexa
}

// Create objects
fauxmoESP fauxmo;
RCSwitch mySwitch = RCSwitch();

//
/************************************ Code Section **************************************/
//

// Wi-Fi Connection
void wifiSetup() {

  Serial.println("Setting up WiFi connection...");

  // Set WIFI module to STA mode
  WiFi.mode(WIFI_STA);

  // Connect
  Serial.printf("[WiFi] Connecting to %s ", WIFI_SSID.c_str());
  WiFi.begin(WIFI_SSID.c_str(), WIFI_PASSWORD.c_str());

  // Wait
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
  Serial.println();

  // Connected!
  Serial.printf("[WiFi] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}

void setup() {
  // Init serial port and clean garbage
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println();

  // Setup wifi connection
  wifiSetup();

  // Initialize LED indicator
  Serial.printf("Initializing blue status indicator on GPIO Pin %d\n", LED_PIN);
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Set up the RF transmitter
  Serial.printf("Enabling transmitter on GPIO Pin %d\n", Transmitter::PIN);
  pinMode(Transmitter::PIN, OUTPUT);  // Set GPIO to output
  mySwitch.enableTransmit(Transmitter::PIN);  // Set pin to transmit mode
  mySwitch.setProtocol(Transmitter::PROTOCOL); // Set the transmission protocol
  mySwitch.setPulseLength(Transmitter::PULSE_LENGTH);  // Set the pulse length

  // By default, fauxmoESP creates it's own webserver on the defined port
  // The TCP port must be 80 for gen3 devices (default is 1901)
  // This has to be done before the call to enable()
  fauxmo.createServer(true); // not needed, this is the default value
  fauxmo.setPort(SERVER_PORT); // This is required for gen3 devices

  // You have to call enable(true) once you have a WiFi connection
  // You can enable or disable the library at any moment
  // Disabling it will prevent the devices from being discovered and switched
  fauxmo.enable(true);
  // You can use different ways to invoke alexa to modify the devices state:
  // "Alexa, turn deck lights on"

  // Add virtual devices
  Serial.printf("Initializing device \"%s\" for use with Alexa interface\n", Transmitter::SWITCH_NAME.c_str());
  fauxmo.addDevice(Transmitter::SWITCH_NAME.c_str());

  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
    // Callback when a command from Alexa is received. 
    // You can use device_id or device_name to choose the element to perform an action onto (relay, LED,...)
    // State is a boolean (ON/OFF).
    // Just remember not to delay too much here, this is a callback, exit as soon as possible.
    // If you have to do something more involved here set a flag and process it in your main loop.
    if(strcmp(device_name, Transmitter::SWITCH_NAME.c_str()) == 0) {
      Serial.println("Transmitter triggered by Alexa command...");
      Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
      // State is true for "ON" and false for "OFF"
      if (state) {
        digitalWrite(LED_PIN, HIGH);  // Turn on the built-in LED
        mySwitch.send(Transmitter::SWITCH_ON.c_str()); // Send the "ON" signal via the transmitter
        mySwitch.send(Transmitter::SWITCH_ON.c_str());
        mySwitch.send(Transmitter::SWITCH_ON.c_str());
        Serial.printf("\"%s\" successfully switched on\n", device_name);
      } else {
        digitalWrite(LED_PIN, LOW); // Turn off the built-in LED
        mySwitch.send(Transmitter::SWITCH_OFF.c_str());  // Send the "OFF" signal via the transmitter
        mySwitch.send(Transmitter::SWITCH_OFF.c_str());
        mySwitch.send(Transmitter::SWITCH_OFF.c_str());
        Serial.printf("\"%s\" successfully switched off\n", device_name);
      }
      Serial.println("Awaiting command from Alexa...");
    }
  });

  // Waiting for command
  Serial.println("Awaiting command from Alexa...");
}

void loop() {
  // fauxmoESP uses an async TCP server but a sync UDP server
  // Therefore, we have to manually poll for UDP packets
  fauxmo.handle();

#ifdef DEBUG_ON
  static unsigned long last = millis();
  if (millis() - last > 5000) {
    last = millis();
    Serial.printf("Listening for Alexa command...\n[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
  }
#endif
}

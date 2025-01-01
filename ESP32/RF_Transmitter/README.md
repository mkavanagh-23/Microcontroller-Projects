## Deck Lights
### Control RF lights with Alexa using fauxmo and RCSwitch for the ESP32-WROOM.

This project utilizes an ESP32-WROOM and the fauxmo library to listen for atriggering event and transmit a 433MHz signal to turn on string lights.

#### Requirements:
 - ESP32-WROOM Dev Kit
 - Amazon Account
 - 433 MHz Receiver (Model MX-05V)
 - 433 MHz Trasmitter (Model FS-03V)
 - [fauxmoESP](https://github.com/vintlabs/fauxmoESP)
 - [RCSwitch](https://github.com/sui77/rc-switch)
 - [AsyncTCP](https://github.com/me-no-dev/AsyncTCP) / [ESPAsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP)

## Receiving RF Codes

The RF receiver module can be utilized with the RCSwitch library in order to read and decode binary codes transmitted by existing RF devices.

Flash the sketch contained in the "Receiver" directory and wire the controller as follows:

#### Wiring:
 - Connect either of the DATA pins (they are bridged) on the receiver module to a GPIO pin that supports interrupts. Update the code example to reflect your pin selection.
 - Connect the GND pin on the receiver to a GND pin on the module
 - Connect the VCC pin on the receiver to the 3.3V output on the module

![Receiver Circuit Diagram](https://github.com/mkavanagh-23/ESP-Projects/blob/8344896eed632ca1db0fbc4e6ec93560e863148d/ESP32/DECK_LIGHTS/images/receivecircuit.png)

## Transmitting RF Codes

The RF transmitter module is can then be utilized to transmit those decoded binary codes using the RCSwitch library. fauxmoESP is utilized to create a simple Amazon Alexa device with a provided callback function to transmit On and Off signals. The example builds a single device, however can easily be altered to create multiple devices with a customizable callback framework.

After creating devices in the setup() function, they can be added to an Alexa account by issuing the "Alexa, Discover devices..." voice command. This only needs to be done once as the connection will then persist across reboots.

To rename the device, first delete the device from your Alexa account, reflash the chip with the updated device name, and re-issue the "discover devices" command.


Flash the sketch contained in the "Transmitter" directory and wire the controller as follows:

#### Wiring:
 - Connect the DATA pin on the transmitter module to a GPIO (PWM) pin. Update the code example to reflect your pin choice.
 - Connect the NEG(-) pin on the transmitter to a GND pin on the module
 - Connect the POS(+) pin on the transmitter to the 3.3V output on the module*
 - Leave the EN pin disconnected (connecting disables the device)
 - (Optional) Solder an antenna to the ANT pad for better range
 - GPIO2 powers the built-in LED indicator when set as 'OUTPUT'

![3V3 Circuit Diagram](https://github.com/mkavanagh-23/ESP-Projects/blob/bc51acde9648dc5ce235bbe0daac2b86a4e39f2a/ESP32/DECK_LIGHTS/images/3v3circuit.png)

*\*note: if powered via USB you can connect POS to the VIN 5V port to get a stronger signal*:

![5V Circuit Diagram](https://github.com/mkavanagh-23/ESP-Projects/blob/bc51acde9648dc5ce235bbe0daac2b86a4e39f2a/ESP32/DECK_LIGHTS/images/5vcircuit.png)

**DO NOT supply power input to both the VIN5V and USB Port at the same time or you will cause a short**.

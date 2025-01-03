## Blink LED (Async)
### An example program to control a built-in LED blinking at a constant rate and a series of LEDs blinking at a dynamic rate asynchronously

This project utilizes an ESP32-WROOM and the ESP-IDF framework with RTOSin order to create several asynchronous tasks.

#### Requirements:
 - ESP32-WROOM Dev Kit
 - 4 Colored LEDs (I used RED, BLUE, GREEN, YELLOW)
 - 4 220Ω Resistors
 - 10K potentiometer (B103)

#### Wiring:
 - Connect each LED via a resistor to its respective GPIO pin and GND
 - Wire the potentiometer to 3.3v Power and GND, with the output going to its respective GPIO pin

![Async LED Diagram](https://github.com/mkavanagh-23/Microcontroller-Projects/blob/b977975206d3e4d2ec19a64fea3eecfeec8fa97b/ESP32/BLINK_LED_ASYNC/docs/circuit_diagram.png)

#### Usage:
The module will blink its builtin LED at a constant rate. The rate of the colored LED sequence will be determined by the value output by the potentiometer. Twist it to adjsut the rate. You can view the serial monitor to debug set delay values if needed.

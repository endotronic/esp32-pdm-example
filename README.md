I was having difficulty getting a PDM mic working with [arduino-esp32](https://github.com/espressif/arduino-esp32) (based on EDP-IDF v4.4) and tried ESP-IDF v5.1 examples. This worked for me.

This example is a proof-of-concept reading a PDM mic and showing a simple sound bar via serial monitor.

This is calibrated with the PDM mic I am using. Some constants may need to be changed for other microphones.

"blinky.dotstar"

This mcu app works on boards with a single dotstar LED. 

It was tested on the following boards:

    adafruit-itsybitsy-m4   (defined in RIOT/boards/adafruit-itsybitsy-m4)
    trinket-m0              (defined in ../../boards/trinket-m0)

The dotstar (apa102) LED hue circulates continuously from red (0 degrees) to
green (120 degrees) to blue (240 degrees) and back to red in 1 degree
increments.  It also blinks LED0 every 30 degrees of the hue circle.

If your board has BTN0 (button) defined, then pressing it will pause the hue
and blink LED0 at 2 Hz.

FWIW, dotstar LED strips are easier to drive than neopixels. Neopixels hava a
timing requirement that is difficult to meet with bit-banged gpio.


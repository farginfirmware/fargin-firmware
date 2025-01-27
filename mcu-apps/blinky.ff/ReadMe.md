
"fargin-firmware-blinky"

Now this is how you blink an LED!!

This app is intended to run on any [RIOT OS defined
board](https://github.com/RIOT-OS/RIOT/tree/master/boards) with:

 - ample code and data space (the [adafruit-itsybitsy-m4](https://www.adafruit.com/product/3800),
   for example)
 - an LED (either LED0 included in the board definition or any gpio output pin)
 - a button (either BTN0 included in the board definition or any gpio input pin)
 - spare gpio pins
 - USB
 - external flash for flash file system

Most [RIOT OS boards](https://github.com/RIOT-OS/RIOT/tree/master/boards)
define at least LED0.  Many boards also define BTN0.


This blinky app works in conjunction with a companion PC app. The PC app is used
to edit and test the mcu app.



This app merely blinks an LED (LED0). Pressing a button (BTN0) doubles the
blink rate.




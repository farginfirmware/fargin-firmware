
tbd

This 'threads-and-coroutines' app builds upon 'blinky-with-more-gpio'.

native mcu firmware
 - multi-threaded
   - Lua interpreter in its own thread
   - main thread (which merely sleeps after starting the Lua thread)
 - LED0 is no longer used

interpreted Lua app
 - added the LED class with red, green and blue instances
 - 3 "blink" co-routines for cooperative (non-preemptive) multi-threading: one for each of 3 LEDs
 - "LED0" is no longer directly accessible

test platform
 - [Makerdiary nRF52840 M.2 developer kit](https://makerdiary.com/products/nrf52840-m2-developer-kit)


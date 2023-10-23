
This 'threads-and-coroutines' app builds upon 'blinky-with-more-gpio'.

native MCU firmware
 - multi-threaded
   - Lua interpreter in its own thread
   - main thread (which merely sleeps after starting the Lua thread)
 - LED0 is used only for an error indication (see Lua.c)

interpreted Lua app
 - 3 "blink" co-routines for cooperative (non-preemptive) multi-threading: one for each of 3 LEDs
 - added the LED class for red, green and blue instances
 - "LED0" is no longer directly accessible

test platform
 - [Makerdiary nRF52840 M.2 developer kit](https://makerdiary.com/products/nrf52840-m2-developer-kit)


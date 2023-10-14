

This 'threads-and-coroutines' app builds upon 'blinky-with-more-gpio' as follows:
 - native MCU firmware has two threads:
  - a main thread (which merely sleeps for now)
  - a separate Lua thread
 - the Lua app uses co-routines for cooperative (non-preemptive) multi-threading
 - implement LED class for red, green and blue LEDs
 - "LED0" is no longer accessible from Lua ; it is only used in native mcu code for an error indication

summary:
 - native MCU code is multi-threaded
 - Lua app is also multi-threaded (with co-routines)
 - LED class and one instance for each LED
 - tested on a
    [Makerdiary nRF52840 M.2 developer kit](https://makerdiary.com/products/nrf52840-m2-developer-kit)



This folder contains sample projects which are intended to demonstrate how the
fargin firmware framework works. It is not complete - rather, it is being
continually developed.

The following mcu apps are ready to run. Each has its own ReadMe.md file.
 - blinky
 - dotstar-blinky
 - fargin-firmware-blinky
 - gpio
 - gpio-objects
 - lua-coroutines
 - serial-via-stdio

These apps were tested on the following boards. 
 - boards with existing RIOT OS board definitions
   - [Adafruit Feather M0 Basic Proto](https://www.adafruit.com/product/2772)
   - [Adafruit ItsyBitsy M4 Express](https://www.adafruit.com/product/3800)
 - boards with custom board definitions (in the ../boards folder)
   - [Adafruit Trinket M0](https://www.adafruit.com/product/3500)
   - [Adafruit Feather M4 Express](https://www.adafruit.com/product/3857)
   - [Makerdiary nRF52840 M.2 developer kit](https://makerdiary.com/products/nrf52840-m2-developer-kit)


Note that [Lua](https://www.lua.org/about.html) takes a lot of code (flash)
and data (RAM). Some small MCUs might not have enough of both. The examples in
this folder were tested on boards with at least 256K of flash and 32K of RAM.


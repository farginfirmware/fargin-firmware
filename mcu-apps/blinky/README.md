
This "blinky" app is a small, but important, first step. In particular, it
introduces the concept of service buffers. Service request buffers are used by
Lua to request service from the native mcu code. An associated response buffer
returns minimally a result indication and may contain response data. In this
example, Lua sends requests for LED0, BTN0 and time services, but does not yet
do anything with service responses.

This app is intended to run without modification on any [RIOT OS
defined board](https://github.com/RIOT-OS/RIOT/tree/master/boards) which
includes LED0. (Most boards have at least LED0.)

 - Note: Some small MCUs might not have enough flash or RAM.
([Lua](https://www.lua.org/about.html) takes a lot of both). This app was
tested on an [Adafruit Trinket M0](https://www.adafruit.com/product/3500) with
256K of flash and 32K of RAM.

If the selected board definition includes BTN0, then pressing that button will
change the blink rate.


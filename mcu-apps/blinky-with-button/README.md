
This "blinky-with-button" app is intended to run on any
[RIOT OS defined board](https://github.com/RIOT-OS/RIOT/tree/master/boards)
which includes both LED0 and BTN0.  (Many boards have both.)
Some small MCUs might not have enough CODE or DATA space to work
([Lua](http://lua.org) takes a lot of both).

This is a baby step which builds upon the "blinky" app.  It shows how results
(button pressed in this case) are returned from native mcu code to Lua.

native mcu code
 - single-threaded (the Lua interpreter is the only thread)

interpreted Lua app
 - single-threaded (i.e. no co-routines)
 - "LED0" is directly accessible
 - "BTN0" is directly accessible

development platform
 - [Makerdiary nRF52840 M.2 developer kit](https://makerdiary.com/products/nrf52840-m2-developer-kit)


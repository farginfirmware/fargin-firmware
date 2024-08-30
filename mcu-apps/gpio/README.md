
tbd

This "gpio" app ...


board requirements
 - [RIOT OS defined board](https://github.com/RIOT-OS/RIOT/tree/master/boards)
 - or compatible MCU
 - LED0
 - min code
 - min data

optional
 - BTN0 



This "blinky" app is intended to run unmodified on any
[RIOT OS defined board](https://github.com/RIOT-OS/RIOT/tree/master/boards)
which includes LED0.  (Most boards have at least LED0.) Some small MCUs might
not have enough CODE or DATA space to work ([Lua](http://lua.org) takes a lot
of both).

This is a small, but important, first step. In particular, it introduces the
concept of service buffers. Service request buffers are used by Lua to request
service from the native mcu code. An associated response buffer returns
minimally a result indication and may contain response data. In this example,
Lua sends requests for LED0 and time services, but does not yet do anything
with service responses.

The platform used to develop this app was:
 - [Makerdiary nRF52840 M.2 developer kit](https://makerdiary.com/products/nrf52840-m2-developer-kit)


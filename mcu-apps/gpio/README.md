
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








tbd

This 'blinky-with-more-gpio' builds upon the 'blinky' app in two important
ways:
 - any gpio may be initialized and configured by Lua ; in this case:
   - a button
   - a 2nd LED (i.e. in addition to LED0)
 - gpio service requests return values which Lua needs
   - get button handle and configure it as an input with a pull up
   - get handle for 2nd LED and configure it as a push-pull output

Unlike LED0, the button and 2nd LED are not already part of the board
definition. You must initialize and configure them in main.lua using the port
numbers and bit numbers for your board. This is the setup for the button for my board:

```
port =  0
bit  = 19
result, button = service_request (service.gpio, gpio.getHandle, port, bit)
                 service_request (service.gpio, gpio.configure, button, gpio.configureArgs.input.pullUp)
```

Notice that the first call to service_request() above saves the values returned
by the service request.
 - "result" indicates whether or not the service request succeeded
 - "button" is the handle for subsequent button functions provided the call succeeded

(I so appreciate functions which return multiple values!)


native mcu firmware
 - single-threaded (Lua interpreter)

interpreted Lua app
 - single-threaded (i.e. no co-routines)
 - "LED0" is directly accessible


development platform
 - [Makerdiary nRF52840 M.2 developer kit](https://makerdiary.com/products/nrf52840-m2-developer-kit)


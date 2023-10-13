
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
numbers and bit numbers for your board. This is the setup for my board:

```
port =  0
bit  = 19
result, button = service_request (service.gpio, gpio.getHandle, port, bit)
                 service_request (service.gpio, gpio.configure, button, gpio.configureArgs.input.pullUp)
```

Notice that the first call to service_request() above saves the values returned
by the service request.
 - "result" indicates whether or not the call succeeded
 - "button" is the handle for subsequent button functions provided the call succeeded

(I so appreciate functions which return multiple values!)


summary:
 - native MCU code is single-threaded
 - Lua app is also single-threaded (i.e. no co-routines yet)
 - tested on a
    [Makerdiary nRF52840 M.2 developer kit](https://makerdiary.com/products/nrf52840-m2-developer-kit)


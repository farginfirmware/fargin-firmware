
"gpio-objects"

This app does the same thing as the "gpio" app except that it does it with Lua
"classes" and instances thereof.

Two classes are defined:
 - GpioOutput with methods
   - new()
   - write()
 - GpioInput with methods
   - new()
   - read()

The 2nd LED and test input are instantiated as follows:

```
    port = 0   bit = 25     -- makerdiary-nrf52840-mdk-iot-kit
    local testInput = GpioInput:new (port, bit)

    port = 0   bit = 24     -- makerdiary-nrf52840-mdk-iot-kit blue LED
    local blueLED = GpioOutput:new (port, bit)
    blueLED:write (1)       -- initialize off
```

The run behavior is identical to the "gpio" app.

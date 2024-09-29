
"gpio"

This app builds upon the "blinky" app with the introduction of gpio services
 - any gpio bit may be initialized and configured from Lua ; in this case:
   - a button (or any port pin that can be pulled low)
   - a 2nd LED (i.e. in addition to LED0)
 - gpio service requests return values which Lua needs
   - get button handle and configure it as an input with a pull up
   - get handle for 2nd LED and configure it as a push-pull output

Unlike LED0, the 2nd LED and button are not already part of the board
definition. You must initialize and configure them in main.lua using the port
and pin numbers for your board. This is the setup for the button and 2nd LED
for my board:

```
    port = 0   bit = 25         -- makerdiary-nrf52840-mdk-iot-kit
    local testInput = gpio_configureInput (port, bit)

    port = 0   bit = 24         -- makerdiary-nrf52840-mdk-iot-kit
    local blueLED = gpio_configureOutput (port, bit)
    gpio_write (gpioHandle, 1)  -- init high (off)
```

While running...
 - if the testInput is pulled low
   - LED0 is off
   - the 2nd LED will blink with a rate of 2 Hz and a 10% duty cycle
 - otherwise
   - the 2nd LED is off
   - if BTN0 is not pressed
     - LED0 blinks at 1 Hz
   - if BTN0 is pressed
     - LED0 blinks at 2 Hz


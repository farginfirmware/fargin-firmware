
"lua.coroutines"

This app is similar to the "gpio.objects" app, but with the introduction of Lua
coroutines.

The same gpio classes are used:
 - GpioOutput
 - GpioInput

The same testInput is used:
```
    port = 0   bit = 25                     -- makerdiary-nrf52840-mdk-iot-kit
    local testInput = GpioInput:new (port, bit)
```

There are now 3 LEDs. For each LED, a coroutine is created, initiated and saved
in the blinkAll table:

```
    local blinkAll = {}
    port = 0
    for _, bit in ipairs {22, 23, 24} do    -- makerdiary-nrf52840-mdk-iot-kit green, red, blue LEDs
        local pin = GpioOutput:new (port, bit)
        local blinkOne = coroutine.create (blink)
        coroutine.resume (blinkOne, pin, 500, 0.25)
        table.insert (blinkAll, blinkOne)
    end
```


If the testInput is not pulled low
 - green, red and blue LEDs are off
 - LED0 blinks
   - 1 Hz if BTN0 not pressed
   - 2 Hz if BTN0 pressed

If the testInput is pulled low
 - LED0 is off
 - green, red and blue LEDs blink one at a time
   - blink rate doubles if BTN0 is pressed


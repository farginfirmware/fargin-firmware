
--[[
    This file would normally be only Lua startup code.
    This is not intended to be the whole app (although it certainly could be).
    But for this and other examples, this is the whole Lua app.
]]


-- services tightly coupled to requestServers[] in main.c
local service = {
    test = 0,   -- not used in this app
    time = 1,
    gpio = 2
}

-- gpio args (see gpio.c)
local gpio = {
    getHandle = 0,
    configure = 1,
         read = 2,
        write = 3,
    configureArgs = {
            input = { noPull=0, pullUp=1, pullDown=2 },
           output = { pushPull=3, openDrain=4, openDrainPullupEnabled=5 }
    }
}


local function delayMilliseconds (milliseconds)
    service_request (service.time, milliseconds)
end


-- this is the prototype for the red, green and blue LEDs
-- it is analogous to an object-oriented class definition
local LED = {
    new = function (self, port, bit)
        led = {}    -- create the instance
        setmetatable (led, self)
        self.__index = self

        _, _, led.handle = service_request (service.gpio, gpio.getHandle, port, bit)

        service_request (service.gpio, gpio.configure, led.handle, gpio.configureArgs.output.pushPull)

        return led
    end,

    set = function (self, state)
        service_request (service.gpio, gpio.write, self.handle, state)
    end
}


local function blink (led, millisecondPeriod, dutyCycle)

    local milliseconds_ON  = millisecondPeriod * dutyCycle
    local milliseconds_OFF = millisecondPeriod - milliseconds_ON

    while true do
        coroutine.yield ()
        led:set (0)     delayMilliseconds (milliseconds_ON)
        led:set (1)     delayMilliseconds (milliseconds_OFF)
    end
end


local function main()

    local red   = LED:new (0, 30)
    local green = LED:new (0, 29)
    local blue  = LED:new (0, 31)

    local allLeds = { red, green, blue }

    local blink_RED   = coroutine.create (blink)
    local blink_GREEN = coroutine.create (blink)
    local blink_BLUE  = coroutine.create (blink)

    local allBlink = { blink_RED, blink_GREEN, blink_BLUE}

    coroutine.resume (blink_RED,     red,  500, 0.25)
    coroutine.resume (blink_GREEN, green, 1000, 0.25)
    coroutine.resume (blink_BLUE,   blue, 1500, 0.25)

    while true do

        -- blink 1 at a time
        for _, blink in ipairs (allBlink) do coroutine.resume (blink) end

        -- turn them all on
        for _, led in ipairs (allLeds) do led:set (0) end   delayMilliseconds (2000)

        -- turn them all off
        for _, led in ipairs (allLeds) do led:set (1) end   delayMilliseconds (1000)

    end

end


main()  -- execute the function defined above


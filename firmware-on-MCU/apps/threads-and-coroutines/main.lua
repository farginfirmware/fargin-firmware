
--[[
    This file would normally be only Lua startup code.
    This is not intended to be the whole app (although it certainly could be).
    But for this and other examples, this is the whole Lua app.
]]


-- services available to Lua (see service_request() in Lua.c)
local service = {
    -- these defs are tightly coupled to nextLevelProcessor[] in service.c
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


local function blink (led, millisecondsOn)
    while true do
        coroutine.yield ()
        led.set (0)
        delayMilliseconds (millisecondsOn)
        led.set (1)
        delayMilliseconds (500)
    end
end


local LED = {
    new = function (self, port, bit)
        led = {}    -- create the instance
        setmetatable (led, self)
        self.__index = self

        _, led.handle = service_request (service.gpio, gpio.getHandle, port, bit)

        service_request (service.gpio, gpio.configure, led.handle, gpio.configureArgs.output.pushPull)

        return led
    end,

    set = function (self, state)
        service_request (service.gpio, gpio.write, self.handle, state)
    end
}


local function main()

--[[

    local green = {}

    _, green.handle = service_request (service.gpio, gpio.getHandle, 0, 29)

    service_request (service.gpio, gpio.configure, green.handle, gpio.configureArgs.output.pushPull)

    while true do
        service_request (service.gpio, gpio.write, green.handle, 0)
        delayMilliseconds (500)
        service_request (service.gpio, gpio.write, green.handle, 1)
        delayMilliseconds (1000)
    end

]]


    local red   = LED:new (0, 30)
    local green = LED:new (0, 29)
    local blue  = LED:new (0, 31)


    while true do

        red:set (0)
        delayMilliseconds (500)
        red:set (1)

        green:set (0)
        delayMilliseconds (500)
        green:set (1)

        blue:set (0)
        delayMilliseconds (500)
        blue:set (1)

        delayMilliseconds (1000)

          red:set (0)
        green:set (0)
         blue:set (0)

        delayMilliseconds (1000)

          red:set (1)
        green:set (1)
         blue:set (1)

        delayMilliseconds (1000)

    end

end


main()  -- execute the function defined above

